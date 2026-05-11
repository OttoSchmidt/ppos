// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Dispatcher: gerencia os estados das tarefas.

#ifndef __PPOS_DISPATCHER__
#define __PPOS_DISPATCHER__

#include "lib/queue.h"
#include "tcb.h"

// inicia o subsistema dispatcher
void dispatcher_init();

// executa o dispatcher
void dispatcher();

// executa a tarefa indicada: a retira da fila de prontas,
// muda seu status para RODANDO e transfere a CPU para ela.
void task_run(struct task_t *task);

// a tarefa atual libera a CPU para o dispatcher,
// voltando para a fila de prontas
void task_yield();

// suspende a tarefa atual: a retira da fila de prontas,
// a insere na fila "queue" (se não for NULL) e retorna
// ao dispatcher.
void task_suspend(struct queue_t *queue);

// acorda uma tarefa: a retira da fila onde se encontra
// suspensa (se estiver em uma) e a insere na fila de
// prontas, para retomar (ou iniciar) sua execução.
void task_awake(struct task_t *task);

// encerra a execução da tarefa atual, informando um
// "exit code", e retorna ao dispatcher.
void task_exit(int exit_code);

// a tarefa atual fica suspensa aguardando que a tarefa
// indicada termine; a execução retorna ao dispatcher.
// retorno: "exit code" da tarefa indicada ou ERROR
int task_wait(struct task_t *task);

// a tarefa atual fica suspensa por t milissegundos;
// a execução retorna ao dispatcher.
void task_sleep(int t);

#endif
