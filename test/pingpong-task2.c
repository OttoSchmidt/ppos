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

static struct task_t *task[NUMTASKS];

// corpo das tarefas
void body_task(void *arg)
{
    int id;
    int next;

    id = task_id(NULL);
    printf("\tIniciei  tarefa %5d\n", id);

    // passa o controle para a proxima tarefa
    next = ((long)arg + 1) % NUMTASKS;
    printf("\tVou mudar para a tarefa %d\n", task_id(task[next]));
    task_switch(task[next]);

    printf("\tEncerrei tarefa %5d\n", id);

    task_switch(NULL);
}

// corpo da tarefa principal
void user_main(void *arg)
{
    int status;
    char *name = task_name(NULL);

    printf("%s: inicio\n", name);

    // inicia tarefas
    for (long i = 0; i < NUMTASKS; i++)
    {
        task[i] = task_create(NULL, body_task, (void *)i);
        assert(task[i]);
        printf("%s: criei a tarefa %d\n", name, task_id(task[i]));
    }

    // passa o controle para cada uma delas em sequencia
    for (long i = 0; i < NUMTASKS; i++)
    {
        printf("%s: vou ativar a tarefa %d\n", name, task_id(task[i]));
        status = task_switch(task[i]);
        assert(status == NOERROR);
    }

    // destroi os descritores
    for (long i = 0; i < NUMTASKS; i++)
    {
        printf("%s: vou destruir a tarefa %d\n", name, task_id(task[i]));
        status = task_destroy(task[i]);
        assert(status == NOERROR);
    }

    printf("%s: fim\n", name);

    task_switch(NULL);
}
