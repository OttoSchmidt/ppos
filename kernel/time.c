// PingPongOS - PingPong Operating System

#include "time.h"
#include "hardware/cpu.h"
#include "tcb.h"
#include "dispatcher.h"

#include <stdio.h>

extern struct task_t *task_atual;
static unsigned int ticks = 0;

void treat_tick(int time);

void time_init()
{
    hw_irq_handle(IRQ_TIMER, treat_tick);
    hw_timer(1, 1);
}

void treat_tick(int time) {
    // verificar se o quantum chegaria a 0 (se fosse decrementado).
    // a tarefa do kernel sempre possui quantum 0.
    ticks++;
    if (task_atual->quantum > 1) {
        task_atual->quantum--;
    } else if (task_atual->quantum == 1) {
        task_atual->quantum = QUANTUM; // resetar quantum
        task_yield(); // voltar p/ dispatcher
    }
}

int systime()
{
    return ticks;
}
