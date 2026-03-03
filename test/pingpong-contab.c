// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste da contabilização com tarefas de prioridades iguais

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

#define WORKLOAD 40000

static struct task_t *pang, *peng, *ping, *pong, *pung;

// simula um processamento pesado
int hardwork(int n)
{
    int soma = 0;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            soma += j;
    return (soma);
}

// corpo das tarefas
void body(void *arg)
{
    printf("%5d ms: %s inicia\n", systime(), (char *)arg);
    hardwork(WORKLOAD);
    printf("%5d ms: %s termina\n", systime(), (char *)arg);
    task_exit(0);
}

// corpo da tarefa principal
void user_main(void *arg)
{
    printf("user: inicio\n");

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
