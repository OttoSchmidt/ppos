// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste da contabilização com muitas tarefas (stress)

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

#define WORKLOAD 5000
#define NUMTASKS 512

static struct task_t *task[NUMTASKS];

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
void body()
{
    for (int i = 0; i < 3; i++)
        hardwork(WORKLOAD);
    task_exit(0);
}

// corpo da tarefa principal
void user_main(void *arg)
{
    printf("user: inicio\n");

    // cria tarefas
    printf("Creating %d tasks\n", NUMTASKS);
    for (int i = 0; i < NUMTASKS; i++)
    {
        task[i] = task_create(NULL, body, NULL);
        assert(task[i]);
    }

    printf("user: fim\n");

    task_exit(0);
}
