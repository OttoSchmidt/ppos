// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste da gestão básica de tarefas

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

#define NUMTASKS 512

static struct task_t *task;

// corpo das tarefas
void body_task(void *)
{
    printf("\tEstou na tarefa %5d\n", task_id(NULL));
    task_switch(NULL);
}

// corpo da tarefa principal
void user_main(void *arg)
{
    int status;
    char *name = task_name(NULL);

    printf("%s: inicio\n", name);

    for (int i = 0; i < NUMTASKS; i++)
    {
        // cria a tarefa
        task = task_create(NULL, body_task, NULL);
        assert(task);

        // ativa a tarefa
        status = task_switch(task);
        assert(status == NOERROR);

        // após retornar, destroi o descritor
        status = task_destroy(task);
        assert(status == NOERROR);
    }

    printf("%s: fim\n", name);

    task_switch(NULL);
}
