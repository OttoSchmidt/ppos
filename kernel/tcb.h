// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// TCB - Task Control Block do sistema operacional

#ifndef __PPOS_TCB__
#define __PPOS_TCB__

#include "ctx.h"

// Task Control Block (TCB), infos sobre uma tarefa
struct task_t
{
    int id;                     // identificador da tarefa
    char *name;                 // nome da tarefa
    struct ctx_t context;       // contexto armazenado da tarefa
                                // ...
};

#endif
