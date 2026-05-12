// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Funções básicas/iniciais do PPOS

#include "hardware/cpu.h"
#include "lib/libc.h"
#include "ppos.h"

//----------------------------------------------------------------------

static void ppos_start()
{
    printf("PPOS: system starting\n");

    // inicia os vários subsistemas
    mem_init();
    task_init();
    dispatcher_init();
    sched_init();
    time_init();
    sem_init();
    mqueue_init();
    block_init("hardware/disk.dat");

    printf("PPOS: system started (uptime %d ms)\n", systime());
}

//----------------------------------------------------------------------

void ppos_stop()
{
    printf("PPOS: system stopping\n", systime());

    // encerra os vários subsistemas (em ordem contrária ao init)
    //block_stop("hardware/disk.dat");
    //mqueue_stop();
    //sem_stop();
    //time_stop();
    //sched_stop();
    //dispatcher_stop();
    //task_stop();
    //mem_stop();

    printf("PPOS: system stopped (uptime %d ms)\n", systime());
}

//----------------------------------------------------------------------

int main()
{
    ppos_start();
    dispatcher();
    ppos_stop();
    hw_poweroff(0);
}

//----------------------------------------------------------------------

