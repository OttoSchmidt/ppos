// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Emulação de uma porta serial com operações de E/S de caracteres.

#ifndef __HW_SERIAL__
#define __HW_SERIAL__

// Inicia a porta serial.
// Return: NOERROR or ERROR.
int hw_serial_init ();

// Envia um caractere à interface serial.
// Return: NOERROR ou ERROR.
int hw_serial_put (char c);

// Recebe um caractere da interface serial (não bloqueante).
// Return: caractere ASCII ou 0 se não há nada a ser lido.
char hw_serial_get ();

#endif
