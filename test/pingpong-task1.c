// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste da gestão básica de tarefas

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

static struct task_t *ping, *pong;

// corpo da tarefa Ping
void body_ping(void *)
{
    char *name = task_name(NULL);

    printf("\t%s: inicio\n", name);
    for (int i = 0; i < 4; i++)
    {
        printf("\t%s: %d\n", name, i);
        task_switch(pong);
    }
    printf("\t%s: fim\n", name);
    task_switch(NULL);
}

// corpo da tarefa Pong
void body_pong(void *)
{
    char *name = task_name(NULL);

    printf("\t\t%s: inicio\n", name);
    for (int i = 0; i < 4; i++)
    {
        printf("\t\t%s: %d\n", name, i);
        task_switch(ping);
    }
    printf("\t\t%s: fim\n", name);
    task_switch(NULL);
}

// corpo da tarefa principal
void user_main(void *arg)
{
    int status;
    char *name = task_name(NULL);

    printf("%s: inicio\n", name);

    ping = task_create("ping", body_ping, NULL);
    assert(ping);
    pong = task_create("pong", body_pong, NULL);
    assert(pong);

    status = task_switch(ping);
    assert(status == NOERROR);

    status = task_switch(pong);
    assert(status == NOERROR);

    printf("%s: fim\n", name);

    status = task_destroy(ping);
    assert(status == NOERROR);

    status = task_destroy(pong);
    assert(status == NOERROR);

    task_switch(NULL);
}
