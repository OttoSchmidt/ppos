// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste da função task_wait() com muitas tarefas

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

#define WORKLOAD 3000
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
    int max = 1 + randnum() % 5 ;
    
    for (int i = 0; i < max; i++)
        hardwork(WORKLOAD + randnum() % WORKLOAD);

    printf("task %3d: fim\n", task_id(NULL));
    task_exit(task_id(NULL));
}

// corpo da tarefa principal
void user_main(void *arg)
{
    int status, exit_code;

    printf("user: inicio\n");

    // cria tarefas
    printf("Creating %d tasks\n", NUMTASKS);
    for (int i = 0; i < NUMTASKS; i++)
    {
        task[i] = task_create(NULL, body, NULL);
        assert(task[i]);
    }

    // espera tarefas encerrarem
    for (int i = 0; i < NUMTASKS; i++)
    {
        printf("Waiting for task %d\n", task_id(task[i]));
        exit_code = task_wait(task[i]);
        assert(exit_code == task_id(task[i]));
    }

    // destroi descritores
    for (int i = 0; i < NUMTASKS; i++)
    {
        status = task_destroy(task[i]);
        assert(status == NOERROR);
    }

    printf("user: fim\n");

    task_exit(0);
}
