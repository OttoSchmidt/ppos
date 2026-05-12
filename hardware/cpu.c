// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Emulação de alguns mecanismos do hardware da CPU

// padrão de API UNIX a usar (para sigaction)
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/times.h>
#include "cpu.h"

#define NOERROR 0
#define ERROR -1

// para os sinais
static struct sigaction action;

// para o timer
static timer_t timer;
static struct itimerspec delay;
static struct sigevent sigev; 

//----------------------------------------------------------------------

int hw_irq_handle(int irq, void (*handle)(int))
{
    if (irq < 1)
        return (ERROR);

    if ((irq + SIGRTMIN) >= SIGRTMAX)
        return (ERROR);

    if (!handle)
        return (ERROR);

    // registra a ação para o sinal indicado
    action.sa_handler = handle;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_NODEFER;
    if (sigaction(irq + SIGRTMIN, &action, 0) < 0)
    {
        perror("IRQ:");
        abort();
    }
    return (NOERROR);
}

//----------------------------------------------------------------------

void hw_irq_enable(int enable)
{
    sigset_t mask;

    if (enable)
        sigemptyset(&mask);
    else
        sigfillset(&mask);

    sigprocmask(SIG_SETMASK, &mask, NULL);
}

//----------------------------------------------------------------------

int hw_timer(int first, int next)
{
    if (first < 0 || next < 0)
        return (ERROR);

    // cria o timer
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = IRQ_TIMER + SIGRTMIN;
    if (timer_create(CLOCK_REALTIME, &sigev, &timer) == -1)
    {
        perror("TIMER:");
        abort();
    }

    // configura o timer POSIX
    delay.it_value.tv_nsec = first * 1000000;
    delay.it_value.tv_sec = 0;
    delay.it_interval.tv_nsec = first * 1000000;
    delay.it_interval.tv_sec = 0;

    // arma o timer POSIX
    if (timer_settime(timer, 0, &delay, NULL) == -1)
    {
        perror("TIMER:");
        abort();
    }
    return (NOERROR);
}

//----------------------------------------------------------------------

void hw_wfi()
{
    // A syscall pause permite emular o comportamento da instrução HLT
    // (halt) ou WFI (wait for interrupt), que suspendem a CPU até a
    // próxima interrupção de hardware, para economizar energia.
    // Neste caso, pause() suspende este processo até o próximo sinal
    // UNIX, que emula uma interrupção.
    pause();
}

//----------------------------------------------------------------------

void hw_poweroff(int error)
{
    //struct tms time;

    // print CPU usage
    //times(&time);
    //printf("Hardware shutdown, cpu active for %ld ms\n",
    //       10 * (time.tms_utime + time.tms_stime));

    // encerra com erro, GDB pode ver o call stack
    if (error)
        abort();

    exit(0);
}

//----------------------------------------------------------------------
