// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Semáforos e spinlocks

#ifndef __PPOS_SEMAPHORE__
#define __PPOS_SEMAPHORE__

// Estrutura que define um semáforo (struct opaco).
// Este struct deve ser redefinido em semaphore.c
struct semaphore_t;

// inicia o subsistema de semáforos
void sem_init();

// Cria um novo semáforo, inicializado com value >= 0.
// Retorno: ptr para o semáforo ou NULL (erro).
struct semaphore_t *sem_create(int value);

// Requisita acesso a um semáforo
// Retorno: NOERROR (0) ou ERROR (<0)
int sem_down(struct semaphore_t *s);

// libera o acesso a um semáforo
// Retorno: NOERROR (0) ou ERROR (<0)
int sem_up(struct semaphore_t *s);

// destrói um semáforo, liberando recursos e tarefas bloqueadas
// Retorno: NOERROR (0) ou ERROR (<0)
int sem_destroy(struct semaphore_t *s);

// trava um spin-lock (busy wait)
void spin_lock(int *lock);

// libera um spin-lock
void spin_unlock(int *lock);

#endif
