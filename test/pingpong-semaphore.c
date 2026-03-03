// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste de semáforos (leve)

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

static struct task_t *a1, *a2, *b1, *b2;
static struct semaphore_t *s1, *s2;

// corpo da tarefa A
void body_a(void *arg)
{
    int status;

    for (int i = 0; i < 5; i++)
    {
        status = sem_down(s1);
        if (status == ERROR)
            break;

        printf("%5d ms: %s zig (%d)\n", systime(), (char *)arg, i);
        task_sleep(1000);

        status = sem_up(s2);
        if (status == ERROR)
            break;
    }
    task_exit(0);
}

// corpo da tarefa B
void body_b(void *arg)
{
    int status;

    for (int i = 0; i < 5; i++)
    {
        status = sem_down(s2);
        if (status == ERROR)
            break;

        printf("%5d ms: %s zag (%d)\n", systime(), (char *)arg, i);
        task_sleep(1000);

        status = sem_up(s1);
        if (status == ERROR)
            break;
    }
    task_exit(0);
}

// corpo da tarefa principal
void user_main(void *arg)
{
    int status;

    printf("user: inicio\n");

    // inicia semáforos
    s1 = sem_create(1);
    assert(s1);
    s2 = sem_create(0);
    assert(s2);

    // cria tarefas
    a1 = task_create("a1", body_a, "A1");
    assert(a1);
    a2 = task_create("a2", body_a, "\tA2");
    assert(a2);
    b1 = task_create("b1", body_b, "\t\t\tB1");
    assert(b1);
    b2 = task_create("b2", body_b, "\t\t\t\tB2");
    assert(b2);

    // aguarda a1 encerrar
    status = task_wait(a1);
    assert(status == NOERROR);

    // destroi semáforos
    status = sem_destroy(s1);
    assert(status == NOERROR);
    status = sem_destroy(s2);
    assert(status == NOERROR);

    // aguarda a2, b1 e b2 encerrarem
    status = task_wait(a2);
    assert(status == NOERROR);
    status = task_wait(b1);
    assert(status == NOERROR);
    status = task_wait(b2);
    assert(status == NOERROR);

    // destroi os descritores das tarefas
    status = task_destroy(a1);
    assert(status == NOERROR);
    status = task_destroy(a2);
    assert(status == NOERROR);
    status = task_destroy(b1);
    assert(status == NOERROR);
    status = task_destroy(b2);
    assert(status == NOERROR);

    printf("user: fim\n");

    task_exit(0);
}
