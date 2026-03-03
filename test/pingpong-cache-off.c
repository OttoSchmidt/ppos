// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste das operações de acesso ao disco com uma única tarefa,
// que lê e escreve/altera blocos do disco SEM CACHE.

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

// corpo da tarefa principal
void user_main(void *arg)
{
    int status;
    unsigned char *buffer;
    int num_blk;     // numero de blocos no disco
    int blk_size;    // tamanho de cada bloco (bytes)
    int time_start;
    int blk_inicio, blk_final;

    printf("%5d ms: user: inicio\n", systime());

    // busca geometria do disco
    num_blk = block_blocks();
    assert(num_blk);
    blk_size = block_size();
    assert(blk_size);

    printf("%5d ms: disco contem %d blocos de %d bytes cada\n",
           systime(), num_blk, blk_size);

    // aloca o buffer para ler blocos do disco
    buffer = mem_alloc(blk_size);
    assert(buffer);

    time_start = systime();

    // escreve blocos
    blk_inicio = 0;
    blk_final  = num_blk / 4;
    for (int i = blk_inicio; i < blk_final; i++)
    {
        printf("%5d ms: escreve bloco %d\n", systime(), i);
        status = block_write(i, buffer);
        if (status)
            printf("Erro ao escrever bloco %d!\n", i);
    }

    // lê blocos
    blk_inicio += num_blk / 8;
    blk_final  += num_blk / 8;
    for (int i = blk_inicio; i < blk_final; i++)
    {
        printf("%5d ms: lendo bloco %d\n", systime(), i);
        status = block_read(i, buffer);
        if (status)
            printf("Erro ao ler bloco %d!\n", i);
    }

    // lê blocos
    blk_inicio += num_blk / 8;
    blk_final  += num_blk / 8;
    for (int i = blk_inicio; i < blk_final; i++)
    {
        printf("%5d ms: lendo bloco %d\n", systime(), i);
        status = block_read(i, buffer);
        if (status)
            printf("Erro ao ler bloco %d!\n", i);
    }

    printf("As operações SEM CACHE demoraram %d ms\n", systime() - time_start);

    // libera o buffer de blocos do disco
    mem_free(buffer);

    printf("%5d ms: user fim\n", systime());

    task_exit(0);
}

