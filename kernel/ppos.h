// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Funções básicas/iniciais do PPOS

#ifndef __PPOS__
#define __PPOS__

// este código deve ser compilado em sistemas UNIX-like
#ifndef __linux__
#error "Este codigo foi planejado para ambientes Linux."
#endif

// módulos do sistema operacional
#include "macros.h"
#include "tcb.h"
#include "task.h"
#include "dispatcher.h"
#include "scheduler.h"
#include "time.h"
#include "memory.h"
#include "semaphore.h"
#include "mqueue.h"
#include "block.h"

// Inicia o sistema operacional
static void ppos_start();

// Encerra o sistema operacional
void ppos_stop();

#endif
