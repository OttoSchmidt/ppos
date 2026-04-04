// PingPongOS - PingPong Operating System
// GRR20244386 - Otto Schmidt
// GRR20244625 - Vinícius Hasse Nascimento

#include "task.h"
#include "macros.h"
#include "lib/libc.h"
#include "lib/queue.h"

#include <stdlib.h>

#define ERROR -1
#define NOERROR 0
#define STACK_SIZE 4096

int new_task_id = 1;
struct task_t *task_atual;
struct task_t *task_kernel;
extern struct queue_t *ready_queue;

void task_init() {
	// inicializa a tarefa do kernel (id=0)
	task_atual = (struct task_t *) malloc(sizeof(struct task_t));
	if (!task_atual) {
		ppos_panic("Nao foi possivel alocar tarefa do kernel!\n");
	}

	task_atual->id = 0;
	task_atual->name = "kernel";
	task_atual->status = TASK_RUNNING;
	task_atual->owner = NULL;
	task_atual->priostatic = 0;
	task_atual->priodinamic = 0;
	task_kernel = task_atual;
}

struct task_t *task_create(char *name, void (*entry)(void *),
                           void *arg) 
{
	struct task_t *nova_tarefa = (struct task_t *) malloc(sizeof(struct task_t));
	if (!nova_tarefa)
		return NULL;

	nova_tarefa->id = new_task_id;
	nova_tarefa->name = name;
	nova_tarefa->status = TASK_NEW;
	nova_tarefa->owner = task_atual; // definir a tarefa que criou esta tarefa
	nova_tarefa->priostatic = 0;
	nova_tarefa->priodinamic = 0;

	// alocar pilha p/ o contexto
	void *stack = malloc(STACK_SIZE);
	if (!stack) {
		free(nova_tarefa);
		return NULL;
	}

	// criar contexto
	if (ctx_create(&nova_tarefa->context, entry, arg, stack, STACK_SIZE) == ERROR) {
		free(stack);
		free(nova_tarefa);
		return NULL;
	}
	
	new_task_id++;
	nova_tarefa->status = TASK_READY; // tarefa pronta para ser executada
	queue_add(ready_queue, nova_tarefa);

	#ifdef DEBUG
	ppos_debug("tarefa criada: '%s'\n", nova_tarefa->name);
	#endif

	return nova_tarefa;
}

int task_destroy(struct task_t *task) {
	if (!task || task->status != TASK_TERMINATED)
		return ERROR;

	free(task->context.stack); // liberar pilha
	free(task); // liberar TCB

	return NOERROR;
}

int task_switch(struct task_t *task) {
	if (!task) {
		// como task == NULL, deve transferir a tarefa
		// para o dono da tarefa atual

		if (!task_atual->owner) { 
			// tarefa atual é a do kernel, não tem dono
			#ifdef DEBUG
				ppos_debug("tarefa atual '%s' finalizou, porem nao possui dono\n", task_atual->name);
			#endif
			return ERROR;
		}

		struct task_t *task_anterior = task_atual;
		task_atual = task_atual->owner;

		#ifdef DEBUG
		ppos_debug("tarefa atual '%s' finalizou. trocando tarefa para dono '%s'\n", task_anterior->name, task_atual->name);
		#endif

		ctx_swap(&task_anterior->context, &task_atual->context);

		return NOERROR;
	}
	
	if (task->status != TASK_TERMINATED) { // ignorar sem erro
		// transferir para a nova tarefa. a execucao da tarefa atual foi suspensa

		struct task_t *task_anterior = task_atual; // salvar tarefa atual
		task_atual = task; // atualizar tarefa atual

		#ifdef DEBUG
		ppos_debug("tarefa '%s' finalizou. trocando para tarefa '%s'\n", task_anterior->name, task_atual->name);
		#endif

		// ctx_swap troca o contexto da cpu, então, por enquanto,
		// essa funcao encerra aqui. a execucao da cpu continua no novo 
		// contexto e, somente quando essa nova tarefa retornar, a execucao 
		// voltara aqui após a função ctx_swap.
		ctx_swap(&task_anterior->context, &task->context);
	} else {
		#ifdef DEBUG
		ppos_debug("tarefa '%s' nao foi trocada pois ja esta encerrada\n", task->name);
		#endif
	}

	return NOERROR;
}

int task_id(struct task_t *task) {
	if (!task)
		return task_atual->id;
	return task->id;
}

char *task_name(struct task_t *task) {
	if (!task)
		return task_atual->name;
	if (!task->name)
		return "SEM_NOME";
	return task->name;
}