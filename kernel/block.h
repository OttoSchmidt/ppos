// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Gerência de um dispositivo orientado a blocos.

#ifndef __PPOS_BLOCK__
#define __PPOS_BLOCK__

// inicia o subsistema de gestão do disco virtual armazenado em "disk_image"
void block_init(char *disk_image);

// encerra o subsistema de gestão do disco virtual
void block_stop(char *disk_image);

// retorna o tamanho de cada bloco do disco, em bytes
int block_size();

// retorna o tamanho do disco, em blocos
int block_blocks();

// leitura de um bloco, do disco para o buffer
int block_read(int block, void *buffer);

// escrita de um bloco, do buffer para o disco
int block_write(int block, void *buffer);

#endif
