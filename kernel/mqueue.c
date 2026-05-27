// PingPongOS - PingPong Operating System
// GRR20244386 - Otto Schmidt
// GRR20244625 - Vinícius Hasse Nascimento
#include <stdlib.h>
#include <string.h>

#include "mqueue.h"
#include "semaphore.h"
#include "lib/libc.h"


struct mqueue_t {
    void *buffer;                   // buffer circular
    int max_msgs;                   // capacidade da fila
    int msg_size;                   // tamanho de cada mensagem
    int head;                       // posição de leitura
    int tail;                       // posição de escrita
    int msgs;                       // número atual de mensagens
    int destroyed;                  // flag pra se a fila foi destruída
    struct semaphore_t *s_buffer;
    struct semaphore_t *s_item;
    struct semaphore_t *s_vaga;
};

// inicia o subsistema de filas de mensagens
void mqueue_init()
{
}

// cria uma fila de mensagens.
// Retorno: ptr para a nova fila ou NULL
struct mqueue_t *mqueue_create(int max_msgs, int msg_size){
    if (max_msgs <= 0 || msg_size <= 0)
        return NULL;

    struct mqueue_t *queue = malloc(sizeof(struct mqueue_t));

    if(!queue)
        return NULL;

    queue->buffer = malloc(max_msgs * msg_size);

    if(!queue->buffer){
        free(queue);
        return NULL;
    }

    queue->max_msgs = max_msgs;
    queue->msg_size = msg_size;
    queue->head = 0;
    queue->tail = 0;
    queue->msgs = 0;
    queue->destroyed = 0;
    queue->s_buffer = sem_create(1);
    queue->s_item   = sem_create(0);
    queue->s_vaga   = sem_create(max_msgs);

    if (!queue->s_buffer || !queue->s_item || !queue->s_vaga){
        free(queue->buffer);
        free(queue);
        return NULL;
    }

    return queue;
}

// destroi uma fila de mensagens, liberando recursos e tarefas
// Retorno: NOERROR ou ERROR
int mqueue_destroy(struct mqueue_t *queue){
    if(!queue || queue->destroyed)
        return ERROR;

    queue->destroyed = 1;

    sem_destroy(queue->s_buffer);
    sem_destroy(queue->s_item);
    sem_destroy(queue->s_vaga);

    free(queue->buffer);
    free(queue);

    return NOERROR;
}

// envia uma mensagem
// Retorno: NOERROR ou ERROR
int mqueue_send(struct mqueue_t *queue, void *msg){
    if(!queue || !msg || queue->destroyed)
        return ERROR;
    
    // Espera ter vaga na fila e entra na região crítica
    if(sem_down(queue->s_vaga) < 0)
        return ERROR;

    if (sem_down(queue->s_buffer) < 0){
        sem_up(queue->s_vaga);
        return ERROR;
    }

    // Calcula endereço de escrita
    void *dest = (char*) queue->buffer + (queue->tail * queue->msg_size);

    // Copia a mensagem para dentro da fila
    memcpy(dest, msg, queue->msg_size);

    // Avança a posição de escrita
    queue->tail = (queue->tail + 1) % queue->max_msgs;

    queue->msgs++;

    // Sai da região crítica e avisa que tem item disponível
    sem_up(queue->s_buffer);
    sem_up(queue->s_item);

    return NOERROR;
}

// recebe uma mensagem
// Retorno: NOERROR ou ERROR
int mqueue_recv(struct mqueue_t *queue, void *msg){
    if(!queue || !msg || queue->destroyed)
        return ERROR;

    // Espera ter item disponível e entra região crítica
    if(sem_down(queue->s_item) < 0)
        return ERROR;

    if(sem_down(queue->s_buffer) < 0){
        sem_up(queue->s_item);
        return ERROR;
    }

    // Calcula endereço de leitura
    void *src = (char*) queue->buffer + (queue->head * queue->msg_size);

    // Copia a mensagem da fila para 
    memcpy(msg, src, queue->msg_size);

    // Avança a posição de leitura
    queue->head = (queue->head + 1) % queue->max_msgs;

    queue->msgs--;

    // Saí da região crítica e avisa que tem vaga livre
    sem_up(queue->s_buffer);
    sem_up(queue->s_vaga);

    return NOERROR;
}

// retorna o numero de mensagens em uma fila
// Retorno: número >= 0 ou ERROR
int mqueue_msgs(struct mqueue_t *queue){
    if(!queue || queue->destroyed)
        return ERROR;
    return queue->msgs;
}