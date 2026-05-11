// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Gerência básica de tarefas.

#ifndef __PPOS_TASK__
#define __PPOS_TASK__

#include "tcb.h"

// inicializa o subsistema de tarefas
void task_init();

// cria uma nova tarefa: "name" é o nome da tarefa, "entry"
// é a função que ela irá executar e "arg" aponta para o valor
// recebido por "entry" ao iniciar (pode ser NULL).
// retorno: ptr para o descritor da tarefa ou NULL se houver erro
struct task_t *task_create(char *name, void (*entry)(void *),
                           void *arg);

// destroi uma tarefa e libera seus recursos;
// somente deve atuar sobre tarefas terminadas.
// Retorno: NOERROR (0) ou ERROR (<0)
int task_destroy(struct task_t *task);

// transfere a cpu da tarefa atual para outra tarefa;
// se task == NULL, transfere para a tarefa que a criou.
// ignora sem erro se "task" já tiver terminado.
// Retorno: NOERROR (0) ou ERROR (<0)
int task_switch(struct task_t *task);

// informa o ID de uma tarefa (ou da tarefa atual se NULL)
int task_id(struct task_t *task);

// informa o nome de uma tarefa (ou da tarefa atual se NULL)
char *task_name(struct task_t *task);

#endif
