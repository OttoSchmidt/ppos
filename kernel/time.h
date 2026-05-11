// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Gerência básica do tempo.

#ifndef __PPOS_TIME__
#define __PPOS_TIME__

// duracao default do quantum em ticks (ms)
#define QUANTUM 10

// inicia o subsistema de gestão do tempo
void time_init();

// informa o valor atual do relógio (milissegundos)
int systime();

#endif
