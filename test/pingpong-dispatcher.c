// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste do dispatcher com escalonador FCFS

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

static struct task_t *pang, *peng, *ping, *pong, *pung;

// corpo das tarefas
void body(void *arg)
{
    printf("%s: inicio\n", (char *)arg);
    for (int i = 0; i < 5; i++)
    {
        printf("%s: %d\n", (char *)arg, i);
        task_yield();
    }
    printf("%s: fim\n", (char *)arg);
    task_exit(0);
}

// corpo da tarefa principal
void user_main(void *arg)
{
    printf("user: criando as tarefas\n");

    // cria tarefas
    pang = task_create("pang", body, "\tPang");
    assert(pang);
    peng = task_create("peng", body, "\t\tPeng");
    assert(peng);
    ping = task_create("ping", body, "\t\t\tPing");
    assert(ping);
    pong = task_create("pong", body, "\t\t\t\tPong");
    assert(pong);
    pung = task_create("pung", body, "\t\t\t\t\tPung");
    assert(pung);

    printf("user: fim\n");

    task_exit(0);
}
