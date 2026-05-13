// PingPongOS - PingPong Operating System
// GRR20244386 - Otto Schmidt
// GRR20244625 - Vinícius Hasse Nascimento

#include "semaphore.h"
#include "task.h"
#include "dispatcher.h"
#include "macros.h"
#include "lib/queue.h"

#include <stdlib.h>

struct semaphore_t {
	int n, s;
	struct queue_t *waiting;
};

extern struct task_t *task_atual;

// inicia o subsistema de semáforos
void sem_init() {

}

// Cria um novo semáforo, inicializado com value >= 0.
// Retorno: ptr para o semáforo ou NULL (erro).
struct semaphore_t *sem_create(int value) {
	if (value < 0)
		return NULL;

	struct semaphore_t *sem = malloc(sizeof(struct semaphore_t));
	if (!sem)
		return NULL;

	sem->s = 0;
	sem->n = value;
	sem->waiting = queue_create();
	if (!sem->waiting) {
		free(sem);
		return NULL;
	}

	return sem;
}

// Requisita acesso a um semáforo
// Retorno: NOERROR (0) ou ERROR (<0)
int sem_down(struct semaphore_t *s) {
	if (!s)
		return ERROR;

	spin_lock(&s->s);

	s->n--;

	if (s->n < 0) {
		spin_unlock(&s->s);
		task_suspend(s->waiting);
		
		#ifdef DEBUG
		printf("acordou do semaforo (task: %s)\n", task_name(task_atual));
		#endif

		// verificar se semaforo foi destruido
		if (task_atual->waited_exit_code == 1)
			return ERROR;
	} else {
		spin_unlock(&s->s);
	}

	return NOERROR;
}

// libera o acesso a um semáforo
// Retorno: NOERROR (0) ou ERROR (<0)
int sem_up(struct semaphore_t *s) {
	if (!s)
		return ERROR;

	spin_lock(&s->s);

	s->n++;

	// acordar primeira tarefa da fila
	if (s->n < 1) {
		struct task_t *head = queue_head(s->waiting);
		if (head) {
			queue_del(s->waiting, head);
			head->waited_exit_code = 0;
			task_awake(head);
		}
	}

	spin_unlock(&s->s);

	return NOERROR;
}

// destrói um semáforo, liberando recursos e tarefas bloqueadas
// Retorno: NOERROR (0) ou ERROR (<0)
int sem_destroy(struct semaphore_t *s) {
	if (!s)
		return ERROR;

	// liberar tarefas caso estejam esperando. deve retornar -1
	struct task_t *item = queue_head(s->waiting);
	while (item) {
		#ifdef DEBUG
		ppos_debug("tarefa %s (aguardando semafaro) acordada por %s\n", task_name(item), task_name(task_atual));
		#endif

		item->waited_exit_code = 1;
		task_awake(item);
		queue_del(s->waiting, item);
		item = queue_head(s->waiting);
	}

	queue_destroy(s->waiting);
	free(s);

	return NOERROR;
}

// trava um spin-lock (busy wait)
void spin_lock(int *lock) {
	while (__sync_fetch_and_or (lock, 1));	
}

// libera um spin-lock
void spin_unlock(int *lock) {
	(*lock) = 0;
}
