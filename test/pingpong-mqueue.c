// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste das filas de mensagens

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

// estrutura com vários valores para teste
struct pacote_t
{
    int v[3], prod;
};

static struct task_t *prod[3], *mult, *cons[2];
static struct mqueue_t *fila_inteiros, *fila_pacotes;

// corpo da tarefa produtor
void body_prod()
{
    int valor, status;
    char *name;

    name = task_name(NULL);

    printf("%5d ms: %s inicia\n", systime(), name);

    for (;;)
    {
        // sorteia um valor inteiro e o envia na fila de valores
        valor = 1 + randnum() % 20;
        status = mqueue_send(fila_inteiros, &valor);
        if (status < 0)
            break;
        printf("%5d ms: %s envia %d\n", systime(), name, valor);

        // dorme um intervalo aleatorio
        task_sleep(randnum() % 3000);
    }

    printf("%5d ms: %s termina\n", systime(), name);
    task_exit(0);
}

// corpo da tarefa agrupador
void body_agrup()
{
    struct pacote_t pacote;
    char *name;
    int status;

    name = task_name(NULL);

    printf("%5d ms: \t\t\t%s inicia\n", systime(), name);

    for (int i = 0; i < 10; i++)
    {
        // recebe N valores inteiros e os põe no pacote
        pacote.prod = 1 ;
        for (int j = 0; j < 3; j++)
        {
            status = mqueue_recv(fila_inteiros, &pacote.v[j]);
            assert(status == NOERROR);

            printf("%5d ms: \t\t\t%s recebe %d\n", systime(), name,
                   pacote.v[j]);
            pacote.prod *= pacote.v[j] ;
        }

        // mostra o pacote
        printf("%5d ms: \t\t\t%s envia [%d*%d*%d = %d]\n",
               systime(), name, pacote.v[0], pacote.v[1], pacote.v[2],
               pacote.prod);

        // envia o pacote
        status = mqueue_send(fila_pacotes, &pacote);
        assert(status == NOERROR);

        // dorme um intervalo aleatorio
        task_sleep(randnum() % 3000);
    }

    printf("%5d ms: \t\t\t%s termina\n", systime(), name);
    task_exit(0);
}

// corpo da tarefa consumidor
void body_cons()
{
    struct pacote_t pacote;
    char *name;
    int status;

    name = task_name(NULL);

    printf("%5d ms: \t\t\t\t\t\t%s inicia\n", systime(), name);

    for (;;)
    {
        // recebe um pacote e o imprime
        status = mqueue_recv(fila_pacotes, &pacote);
        if (status < 0)
            break;
        printf("%5d ms: \t\t\t\t\t\t%s recebe [%d*%d*%d = %d]\n",
               systime(), name, pacote.v[0], pacote.v[1], pacote.v[2],
               pacote.prod);

        // dorme um intervalo aleatorio
        task_sleep(randnum() % 3000);
    }

    printf("%5d ms: \t\t\t\t\t\t%s termina\n", systime(), name);
    task_exit(0);
}

// corpo da tarefa principal
void user_main(void *arg)
{
    int status;

    printf("%5d ms: user inicia\n", systime());

    // cria as filas de mensagens (5 valores cada)
    fila_inteiros = mqueue_create(5, sizeof(int));
    assert(fila_inteiros);
    fila_pacotes = mqueue_create(5, sizeof(struct pacote_t));
    assert(fila_pacotes);

    // cria tarefas
    mult = task_create("agrup", body_agrup, NULL);
    assert(mult);
    cons[0] = task_create("cons0", body_cons, NULL);
    assert(cons[0]);
    cons[1] = task_create("cons1", body_cons, NULL);
    assert(cons[1]);
    prod[0] = task_create("prod0", body_prod, NULL);
    assert(prod[0]);
    prod[1] = task_create("prod1", body_prod, NULL);
    assert(prod[1]);
    prod[2] = task_create("prod2", body_prod, NULL);
    assert(prod[2]);

    // aguarda o multiplicador encerrar
    status = task_wait(mult);
    assert(status == NOERROR);

    // destroi as filas de mensagens
    printf("%5d ms: user destroi fila_inteiros\n", systime());
    status = mqueue_destroy(fila_inteiros);
    assert(status == NOERROR);
    printf("%5d ms: user destroi fila_pacotes\n", systime());
    status = mqueue_destroy(fila_pacotes);
    assert(status == NOERROR);

    printf("%5d ms: user termina\n", systime());

    task_exit(0);
}
