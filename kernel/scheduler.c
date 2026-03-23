// PingPongOS - PingPong Operating System
// GRR20244386 - Otto Schmidt
// GRR20244625 - Vinícius Hasse Nascimento

#include "scheduler.h"

void sched_init()
{
}

struct task_t *scheduler(struct queue_t *ready_queue) {
	if (!ready_queue) return NULL;

	queue_head(ready_queue);

	return queue_item(ready_queue);
}

