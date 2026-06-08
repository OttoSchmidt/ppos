// PingPongOS - PingPong Operating System
// GRR20244386 - Otto Schmidt
// GRR20244625 - Vinícius Hasse Nascimento

#include <stdlib.h>

#include "task.h"
#include "block.h"
#include "macros.h"
#include "semaphore.h"
#include "dispatcher.h"
#include "hardware/disk.h"
#include "hardware/cpu.h"

extern struct task_t *task_atual;
extern int task_count;

int irq_disk = 0;
struct queue_t *disk_pedidos = NULL;
struct semaphore_t *sem_pedido = NULL;
struct task_t *disk_manager = NULL;

struct disk_pedido_t {
	struct task_t *task;
	int type;
	int block;
	void *buffer;
};

void treat_disk(int n) {
	irq_disk = 1;
	#ifdef DEBUG
	ppos_debug("interrupcao do disco gerada %d\n", n);
	#endif
	task_awake(disk_manager);
}

void disk_manager_body();

// inicia o subsistema de gestão do disco virtual armazenado em "disk_image"
void block_init(char *disk_image) {
	int status = hw_disk_cmd(DISK_CMD_INIT, 0, disk_image);
	if (status < 0) {
		ppos_panic("nao foi possivel abrir disco indicado");
	}

	disk_pedidos = queue_create();
	if (!disk_pedidos)
		ppos_panic("nao foi possivel criar a fila de pedidos do disco");

	disk_manager = task_create("disk-manager", disk_manager_body, NULL);
	if (!disk_manager)
		ppos_panic("nao foi possivel criar a tarefa de gerenciamento do disco");
	disk_manager->quantum = 0;
	task_count--;

	sem_pedido = sem_create(1);
	if (!sem_pedido)
		ppos_panic("nao foi possivel criar semaforo do disco");

	// registrar interrupcao
	hw_irq_handle(IRQ_DISK, treat_disk);

	#ifdef DEBUG
	ppos_debug("disco inicializado\n");
	#endif
}

// encerra o subsistema de gestão do disco virtual
void block_stop(char *disk_image) {
	int status = sem_destroy(sem_pedido);
	if (status < 0)
		ppos_panic("nao foi possivel destruir semaforo do disco");

	status = task_destroy(disk_manager);
	if (status < 0)
		ppos_panic("nao foi possivel encerrar tarefa do disco");

	status = queue_destroy(disk_pedidos);
	if (status < 0)
		ppos_panic("nao foi possivel destruir fila de pedidos do disco");
}

// retorna o tamanho de cada bloco do disco, em bytes
int block_size() {
	return hw_disk_cmd(DISK_CMD_BLOCKSIZE, 0, 0);
}

// retorna o tamanho do disco, em blocos
int block_blocks() {
	return hw_disk_cmd(DISK_CMD_DISKSIZE, 0, 0);
}

// leitura de um bloco, do disco para o buffer
int block_read(int block, void *buffer) {
	if (!buffer || block < 0)
		return -1;
	
	struct disk_pedido_t *pedido = malloc(sizeof(struct disk_pedido_t));
	if (!pedido)
		ppos_panic("nao foi possivel criar pedido de leitura");

	pedido->type = DISK_CMD_READ;
	pedido->task = task_atual;
	pedido->block = block;
	pedido->buffer = buffer;

	sem_down(sem_pedido);
	queue_add(disk_pedidos, pedido);
	sem_up(sem_pedido);

	// acordar tarefa de gerenciador do disco
	task_awake(disk_manager);

	// leitura agendada, dormir tarefa
	task_suspend(NULL);

	return 0;
}

// escrita de um bloco, do buffer para o disco
int block_write(int block, void *buffer) {
	if (!buffer || block < 0)
		return -1;

	struct disk_pedido_t *pedido = malloc(sizeof(struct disk_pedido_t));
	if (!pedido)
		ppos_panic("nao foi possivel criar pedido de leitura");

	pedido->type = DISK_CMD_WRITE;
	pedido->task = task_atual;
	pedido->block = block;
	pedido->buffer = buffer;
	
	sem_down(sem_pedido);
	queue_add(disk_pedidos, pedido);
	sem_up(sem_pedido);

	// acordar tarefa de gerenciador do disco
	task_awake(disk_manager);

	// escrita agendada, dormir tarefa
	task_suspend(NULL);

	return 0;
}

void disk_manager_body() {
	struct disk_pedido_t *pedido = NULL;

	for (;;) {
		if (irq_disk) {
			irq_disk = 0;

			pedido = queue_head(disk_pedidos);
			if (!pedido)
				ppos_panic("pedido do disco que gerou interrupcao nao esta na fila");

			#ifdef DEBUG
			ppos_debug("acordando task %s\n", task_name(pedido->task));
			#endif
			task_awake(pedido->task);

			sem_down(sem_pedido);
			queue_del(disk_pedidos, pedido);
			sem_up(sem_pedido);

			free(pedido);
		}

		int status = hw_disk_cmd(DISK_CMD_STATUS, 0, 0);
		if (status == DISK_STATUS_IDLE) {
			pedido = queue_head(disk_pedidos);

			if (pedido) {
				status = hw_disk_cmd(pedido->type, pedido->block, pedido->buffer);
				if (status < 0) {
					ppos_panic("erro ao ler/escrever do disco");
				}
			}
		}

		#ifdef DEBUG
		ppos_debug("disk_manager_body suspenso\n");
		#endif
		task_suspend(NULL);
	}
}
