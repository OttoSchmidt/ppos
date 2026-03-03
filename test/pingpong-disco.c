// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste das operações de acesso a disco com uma única tarefa,
// que lê e escreve/altera todos os blocos do disco.

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

static int num_blk;     // numero de blocos no disco
static int blk_size;    // tamanho de cada bloco (bytes)

// corpo da tarefa principal
void user_main(void *arg)
{
    int status;
    unsigned char c, *buffer;

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

    // lê e imprime todos os blocos do disco, um a um
    for (int i = 0; i < num_blk; i++)
    {
        // lê o bloco i do disco no buffer
        printf("%5d ms: lendo bloco %d\n", systime(), i);
        status = block_read(i, buffer);
        if (status)
            printf("Erro ao ler bloco %d!\n", i);

        // mostra o conteudo do buffer
        printf("%5d ms: buffer: [", systime());
        for (int j = 0; j < blk_size; j++)
            printf("%c", buffer[j]);
        printf("]\n");
    }

    // inicia gerador de números aleatórios
    randseed(systime());

    // lê e imprime todos os blocos do disco, um a um
    for (int i = 0; i < num_blk; i++)
    {

        // preenche o buffer com um caractere aleatório
        c = 32 + randnum() % 48;
        for (int j = 0; j < blk_size; j++)
            buffer[j] = c;

        // escreve o buffer no bloco i do disco
        printf("%5d ms: escrevendo bloco %d com caracteres \"%c\"\n",
               systime(), i, c);
        status = block_write(i, buffer);
        if (status)
            printf("Erro ao escrever bloco %d!\n", i);
    }

    // libera o buffer de blocos do disco
    mem_free(buffer);

    printf("%5d ms: user fim\n", systime());

    task_exit(0);
}
