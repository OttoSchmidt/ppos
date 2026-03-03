// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste da função task_sleep()

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

static struct task_t *pang, *peng, *ping, *pong, *pung;

// corpo das tarefas
void body(void *arg)
{
    int i, t_sleep, t_before, t_after;
    char *status;

    printf("%5d ms: %s: inicio\n", systime(), (char *)arg);
    for (i = 0; i < 20; i++)
    {
        // sorteia tempo entre 0 e 2000 ms (2s), em saltos de 100 ms
        t_sleep = 100 * (randnum() % 21);

        // informa o quanto vai dormir
        printf("%5d ms: %s vai dormir %d ms\n",
               systime(), (char *)arg, t_sleep);

        // registra tempo antes e depois de dormir
        t_before = systime();
        task_sleep(t_sleep);
        t_after = systime();

        // verifica se dormiu o intervalo especificado
        status = (t_after - t_before) == t_sleep ? "ok" : "ERRADO";

        // informa o quanto efetivamente dormiu
        printf("%5d ms: %s dormiu     %d ms (%s)\n", systime(),
               (char *)arg, t_after - t_before, status);
    }
    printf("%5d ms: %s: fim\n", systime(), (char *)arg);
    task_exit(0);
}

// corpo da tarefa principal
void user_main(void *arg)
{
    int status;

    printf("%5d ms: user: inicio\n", systime());

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

    // aguarda tarefas concluirem
    printf("%5d ms: user: espera Pang...\n", systime());
    status = task_wait(pang);
    assert(status == NOERROR);
    printf("%5d ms: user: Pang acabou\n", systime());

    printf("%5d ms: user: espera Peng...\n", systime());
    status = task_wait(peng);
    assert(status == NOERROR);
    printf("%5d ms: user: Peng acabou\n", systime());

    printf("%5d ms: user: espera Ping...\n", systime());
    status = task_wait(ping);
    assert(status == NOERROR);
    printf("%5d ms: user: Ping acabou\n", systime());

    printf("%5d ms: user: espera Pong...\n", systime());
    status = task_wait(pong);
    assert(status == NOERROR);
    printf("%5d ms: user: Pong acabou\n", systime());

    printf("%5d ms: user: espera Pung...\n", systime());
    status = task_wait(pung);
    assert(status == NOERROR);
    printf("%5d ms: user: Pung acabou\n", systime());

    // destroi descritores
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

    printf("%5d ms: user: fim\n", systime());

    task_exit(0);
}
