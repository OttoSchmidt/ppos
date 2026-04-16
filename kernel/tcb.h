// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// TCB - Task Control Block do sistema operacional

#ifndef __PPOS_TCB__
#define __PPOS_TCB__

#define STACK_SIZE 4096
#define TASK_NEW 0
#define TASK_READY 1
#define TASK_RUNNING 2
#define TASK_SUSPENDED 3
#define TASK_TERMINATED 4

#include "ctx.h"
#include <stdbool.h>

// Task Control Block (TCB), infos sobre uma tarefa
struct task_t
{
    int id;                     // identificador da tarefa
    char *name;                 // nome da tarefa
    struct ctx_t context;       // contexto armazenado da tarefa
    int status;                 // status da tarefa (ex: pronta, executando, terminada)
    struct task_t *owner;       // tarefa que criou esta tarefa (NULL para a tarefa do kernel)
    int priostatic;             // prioridade estática da tarefa
    int priodinamic;            // prioridade dinâmica da tarefa
    int quantum;
};

#endif
