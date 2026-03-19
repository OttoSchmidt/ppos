// PingPongOS - PingPong Operating System
// GRR20244386 - Otto Schmidt
// GRR20244625 - Vinícius Hasse Nascimento

#include "dispatcher.h"
#include "task.h"

#include <stdio.h>

void user_main(void *arg);

void dispatcher_init()
{
}

void dispatcher()
{
	struct task_t *task_user = task_create("user", user_main, NULL);
	if (!task_user) {
		printf("Erro ao criar tarefa do usuário\n");
		return;
	}

	task_switch(task_user);

	task_destroy(task_user);
}
