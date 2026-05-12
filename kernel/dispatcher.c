// PingPongOS - PingPong Operating System
// GRR20244386 - Otto Schmidt
// GRR20244625 - Vinícius Hasse Nascimento

#include "dispatcher.h"
#include "task.h"
#include "scheduler.h"
#include "time.h"

#include "macros.h"

struct queue_t *ready_queue;
struct queue_t *suspended_queue;
struct queue_t *sleeping_queue;

void user_main(void *arg);
extern struct task_t *task_atual;
extern struct task_t *task_kernel;
extern int task_count;

void print_stats(struct task_t *task, int exit_code, int lifetime) {
	printf("PPOS: task %d (%s) exit code %d, %5d ms elapsed time, %5d ms cpu time, %5d activations\n", task_id(task), task_name(task), exit_code, lifetime, task->cpu_time, task->activations);
}

void dispatcher_init()
{
	ready_queue = queue_create();
	suspended_queue = queue_create();
	sleeping_queue = queue_create();
	task_count = 0;
}

// iterar toda lista em busca de tarefas com um certo pai
// para substituir esse pai por um novo
void adopt_children(struct queue_t *queue, struct task_t *father_task, struct task_t *new_father_task) {
	// se queue_head retornar NULL, a fila esta vazia
	if (!queue_head(queue)) return;

	do {
		struct task_t *iter = queue_item(queue);

		if (iter->owner == father_task) {
			#ifdef DEBUG
			ppos_debug("o dono da tarefa '%s' foi trocado para '%s'\n", task_name(iter), task_name(new_father_task));
			#endif

			iter->owner = new_father_task;
		}
	} while (queue_next(queue));

	queue_head(queue);
}

// acorda todas as tarefas que estavam esperando o encerramento de task,
// tambem passa o exit code para elas e remove-as da fila.
void awake_waiting_tasks(struct task_t *task) {
	if (!task || !task->waiting_tasks)
		return;

	struct task_t *item = queue_head(task->waiting_tasks);
	while (item) {
		#ifdef DEBUG
		ppos_debug("tarefa %s (dependia de %s) acordada\n", task_name(item), task_name(task));
		#endif

		item->waited_exit_code = task->exit_code;
		task_awake(item);
		queue_del(task->waiting_tasks, item);
		item = queue_head(task->waiting_tasks);
	}
}

void awake_sleeping_tasks() {
	struct task_t *task = queue_head(sleeping_queue);

	while (task) {
		struct task_t *next = queue_next(sleeping_queue);

		if (task->wake_time <= systime()) {
            queue_del(sleeping_queue, task);
            task->status = TASK_READY;
            queue_add(ready_queue, task);
        }

		task = next;
	}
}

void dispatcher()
{
	struct task_t *task_user = task_create("user", user_main, NULL);
	if (!task_user) {
		ppos_panic("Nao foi possivel criar a task do usuario\n");
		return;
	}

	while (task_count > 0) {
		awake_sleeping_tasks();

		struct task_t *executar_task = scheduler(ready_queue);
		#ifdef DEBUG
		ppos_debug("proxima task: %s\n", task_name(executar_task));
		#endif

		if (executar_task) {
			task_run(executar_task);

			switch(executar_task->status) {
				case TASK_READY:
				case TASK_SUSPENDED: break;
				case TASK_TERMINATED:
					adopt_children(ready_queue, executar_task, task_atual);
					awake_waiting_tasks(executar_task);
					queue_del(ready_queue, executar_task);
					break;
				default:
			}
		} else {
			#ifdef DEBUG
			ppos_debug("Nao existe proxima task\n");
			#endif
		}
	}

	int now = systime();
	task_kernel->cpu_time += now - task_kernel->last_start;
	int lifetime = now - task_kernel->start_time;
	print_stats(task_kernel, 0, lifetime);
}


// executa a tarefa indicada: a retira da fila de prontas,
// muda seu status para RODANDO e transfere a CPU para ela.
void task_run(struct task_t *task) {
	if (!task) return;

	if (queue_del(ready_queue, task) == ERROR)
		return;
	
	task->status = TASK_RUNNING;
	task->quantum = QUANTUM;

	task_switch(task);
}

// a tarefa atual libera a CPU para o dispatcher,
// voltando para a fila de prontas
void task_yield() {
	task_atual->quantum = 0;
	task_atual->status = TASK_READY;
	
	queue_add(ready_queue, task_atual);

	task_switch(task_kernel);
}

// suspende a tarefa atual: a retira da fila de prontas,
// a insere na fila "queue" (se não for NULL) e retorna
// ao dispatcher.
void task_suspend(struct queue_t *queue) {
	task_atual->quantum = 0; // impedir preempcao

	task_atual->status = TASK_SUSPENDED;
	queue_del(ready_queue, task_atual);
	
	if (queue && queue_add(queue, task_atual) == ERROR) 
		ppos_panic("Nao foi possivel adicionar tarefa '%s' na fila\n", task_name(task_atual));

	task_switch(task_kernel);
}

// acorda uma tarefa: a retira da fila onde se encontra
// suspensa (se estiver em uma) e a insere na fila de
// prontas, para retomar (ou iniciar) sua execução.
void task_awake(struct task_t *task) {
	if (!task) return;

	if (queue_del(suspended_queue, task) == ERROR) {
		#ifdef DEBUG
		ppos_debug("Tarefa '%s' nao esta presente na fila de tarefas suspensas\n", task_name(task));
		#endif
	}

	task->status = TASK_READY;

	if (queue_add(ready_queue, task))
		ppos_panic("Nao foi possivel adicionar tarefa '%s' na fila de tarefas prontas\n", task_name(task));
}

// encerra a execução da tarefa atual, informando um
// "exit code", e retorna ao dispatcher.
void task_exit(int exit_code) {
	if (!task_atual)
		ppos_panic("Nao foi possivel encontrar a task_atual para encerra-la\n");

	task_atual->quantum = 0; // impedir preempcao

	// contabiliza o tempo final de CPU
	int now = systime();
	task_atual->cpu_time += now - task_atual->last_start;
	int lifetime = now - task_atual->start_time;

	print_stats(task_atual, exit_code, lifetime);

	#ifdef DEBUG
	ppos_debug("encerrando a task_atual: %s\n", task_name(task_atual));
	#endif

	task_count--;

	task_atual->exit_code = exit_code;

	task_atual->status = TASK_TERMINATED;
	task_switch(task_kernel);
}


int task_wait(struct task_t *task) {
	if (!task)
		return -1;

	if (task->status == TASK_TERMINATED)
		return task->exit_code;

	task_atual->quantum = 0; // impedir preempcao

	task_suspend(task->waiting_tasks);

	return task_atual->waited_exit_code;
}

void task_sleep(int t) {
	task_atual->quantum = 0; // impedir preempcao

	task_atual->wake_time = systime() + t;
	task_suspend(sleeping_queue);
}