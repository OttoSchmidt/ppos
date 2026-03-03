// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste da função task_wait() (leve)

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

#define WORKLOAD 20000

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
    int max, tid;

    tid = task_id(NULL);
    max = tid * 2;

    printf("%5d ms: %s inicia\n", systime(), (char *)arg);
    for (int i = 0; i < max; i++)
    {
        printf("%5d ms: %s %d\n", systime(), (char *)arg, i);
        hardwork(WORKLOAD);
    }
    printf("%5d ms: %s termina\n", systime(), (char *)arg);

    task_exit(tid);
}

// corpo da tarefa principal
void user_main(void *arg)
{
    int exit_code, status;

    printf("user: inicio\n");

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

    for (int i = 0; i < 2; i++)
    {
        printf("%5d ms: user %d\n", systime(), i);
        hardwork(WORKLOAD);
    }

    printf("%5d ms: user esperando Pang...\n", systime());
    exit_code = task_wait(pang);
    assert(exit_code == task_id(pang));

    printf("%5d ms: user esperando Peng...\n", systime());
    exit_code = task_wait(peng);
    assert(exit_code == task_id(peng));

    printf("%5d ms: user esperando Ping...\n", systime());
    exit_code = task_wait(ping);
    assert(exit_code == task_id(ping));

    printf("%5d ms: user esperando Pong...\n", systime());
    exit_code = task_wait(pong);
    assert(exit_code == task_id(pong));

    printf("%5d ms: user esperando Pung...\n", systime());
    exit_code = task_wait(pung);
    assert(exit_code == task_id(pung));

    status = task_destroy(pang);
    assert(status == NOERROR);
    status = task_destroy(peng);
    assert(status == NOERROR);
    status = task_destroy(ping);
    assert(status == NOERROR);
    status = task_destroy(pong);
    assert(status == NOERROR);
    status = task_destroy(pung);
    assert(status == NOERROR);

    printf("%5d ms: user termina\n", systime());

    task_exit(0);
}
