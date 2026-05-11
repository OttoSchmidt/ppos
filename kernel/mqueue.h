// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Gerência de filas de mensagens

#ifndef __PPOS_MQUEUE__
#define __PPOS_MQUEUE__

// Estrutura que define uma fila de mensagens (struct opaco).
// Este struct deve ser redefinido em mqueue.c
struct mqueue_t;

// inicia o subsistema de filas de mensagens
void mqueue_init();

// cria uma fila de mensagens.
// Retorno: ptr para a nova fila ou NULL
struct mqueue_t *mqueue_create(int max_msgs, int msg_size);

// destroi uma fila de mensagens, liberando recursos e tarefas
// Retorno: NOERROR ou ERROR
int mqueue_destroy(struct mqueue_t *queue);

// envia uma mensagem
// Retorno: NOERROR ou ERROR
int mqueue_send(struct mqueue_t *queue, void *msg);

// recebe uma mensagem
// Retorno: NOERROR ou ERROR
int mqueue_recv(struct mqueue_t *queue, void *msg);

// retorna o numero de mensagens em uma fila
// Retorno: número >= 0 ou ERROR
int mqueue_msgs(struct mqueue_t *queue);

#endif
