// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste de semáforos (stress)

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

#define NUMTASKS 64
#define NUMSTEPS 1000000

static struct task_t *task[NUMTASKS];
static struct semaphore_t *s;
static int soma = 0;

// corpo das tarefas
void body()
{
    int status;

    for (int i = 0; i < NUMSTEPS; i++)
    {
        // incrementa contador (seção crítica)
        status = sem_down(s);
        if (status == ERROR)
            break;

        soma += 1;

        status = sem_up(s);
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

    // inicia semáforo em 0 (bloqueado)
    s = sem_create(0);
    assert(s);

    printf("%d tarefas somando %d vezes cada, aguarde...\n",
           NUMTASKS, NUMSTEPS);

    // cria tarefas
    for (int i = 0; i < NUMTASKS; i++)
    {
        task[i] = task_create(NULL, body, "Task");
        assert(task[i]);
    }

    // espera um pouco para liberar o semáforo; isso faz com que todas
    // as tarefas tenham sido criadas e possam competir pelo semáforo
    // em pé de igualdade.
    task_sleep(100);
    status = sem_up(s);
    assert(status == NOERROR);

    // aguarda as tarefas encerrarem
    for (int i = 0; i < NUMTASKS; i++)
    {
        status = task_wait(task[i]);
        assert(status == NOERROR);
    }

    // destroi o semáforo
    status = sem_destroy(s);
    assert(status == NOERROR);

    // verifica se a soma está correta
    if (soma == (NUMTASKS * NUMSTEPS))
        printf("A soma deu %d, valor correto!\n", soma);
    else
        printf("A soma deu %d, mas deveria dar %d!\n",
               soma, NUMTASKS * NUMSTEPS);

    // destroi os descritores
    for (int i = 0; i < NUMTASKS; i++)
    {
        status = task_destroy(task[i]);
        assert(status == NOERROR);
    }

    printf("user: fim\n");

    task_exit(0);
}
