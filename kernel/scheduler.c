// PingPongOS - PingPong Operating System
// GRR20244386 - Otto Schmidt
// GRR20244625 - Vinícius Hasse Nascimento

#include "scheduler.h"
#include "task.h"

extern struct task_t *task_atual;

void sched_init()
{
}

struct task_t *scheduler(struct queue_t *ready_queue) {
	if (!ready_queue) 
		return NULL;

	struct task_t *task = queue_head(ready_queue);
	struct task_t *next_task = task;

	// busca a próxima task
	while(task){
        task->priodinamic--;
        if (task->priodinamic < next_task->priodinamic)
            next_task = task;
        task = queue_next(ready_queue);
    }

	next_task->priodinamic = next_task->priostatic;

	return next_task;
}

// muda a prioridade de uma tarefa
void sched_setprio(struct task_t *task, int prio){
	if (prio > 20 || prio < -20) 
		return;

	// caso nulo, muda a prioridade da tarefa atual
	if (!task){
		task_atual->priostatic = prio;
		task_atual->priodinamic = prio;
		return;
	}

	task->priostatic = prio;
	task->priodinamic = prio;
}

// obtem a prioridade de uma tarefa
int sched_getprio(struct task_t *task){
	// caso nulo, retorna a prioridade da tarefa atual
	if(!task)
		return task_atual->priostatic;

	return task->priostatic;
}