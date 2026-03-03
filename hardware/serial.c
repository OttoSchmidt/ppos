// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

/*

Emula operações de E/S de caracteres em uma porta serial,
que normalmente corresponde a um terminal de texto.

Esta implementação usando stdio é trivial, deve ser substituída
por uma emulação mais detalhada de uma UART (Universal Asynchronous
Receiver-Transmitter) gerenciando a linha serial.

*/

#include <stdio.h>
#include "serial.h"

#define NOERROR 0
#define ERROR -1

//----------------------------------------------------------------------

int hw_serial_init ()
{
    return(NOERROR);
}

//----------------------------------------------------------------------

int hw_serial_put(char c)
{
    // implementação trivial, a ser substituída pela emulação da UART.
    return(putchar(c));
}

//----------------------------------------------------------------------

char hw_serial_get()
{
    // a ser implementado
    return (0);
}

//----------------------------------------------------------------------
