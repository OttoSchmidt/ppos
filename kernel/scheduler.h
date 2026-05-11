// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Escalonador de tarefas prontas.

#ifndef __PPOS_SCHEDULER__
#define __PPOS_SCHEDULER__

#include "lib/queue.h"
#include "tcb.h"

// inicia o subsistema do escalonador de tarefas
void sched_init();

// função escalonador: devolve a próxima tarefa a escalonar na fila
struct task_t *scheduler(struct queue_t *ready_queue);

// muda a prioridade de uma tarefa
void sched_setprio(struct task_t *task, int prio);

// obtem a prioridade de uma tarefa
int sched_getprio(struct task_t *task);

#endif
