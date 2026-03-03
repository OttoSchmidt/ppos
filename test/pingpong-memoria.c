// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.0 -- Junho de 2025

// ATENÇÃO: ESTE ARQUIVO NÃO DEVE SER ALTERADO;
// ALTERAÇÕES SERÃO DESCARTADAS NA CORREÇÃO.

// Teste das operações de alocação de memória heap

#include <assert.h>
#include "lib/libc.h"
#include "ppos.h"

extern int bkl;    // Big Kernel Lock (from task.c)

#define MAX_BLOCKS (1024 * 1024)

static unsigned char *block[MAX_BLOCKS];
static int bsize[MAX_BLOCKS];

// corpo da tarefa principal
void user_main()
{
    int size, nblocks, avail, avail_before, avail_after;
    void *ptr;
    int status;

    // testa mem_size
    printf("Testando mem_size\n");
    size = mem_size();
    printf("mem_size  retornou %d bytes\n", size);
    assert(size > 0);

    // testa mem_avail
    printf("Testando mem_avail\n");
    avail = mem_avail();
    printf("mem_avail retornou %d bytes\n", avail);
    assert(avail > 0);
    assert(avail <= size);

    // testa alocação de tamanho zero
    printf("Testando mem_alloc com tamanho 0\n");
    ptr = mem_alloc(0);
    assert(ptr == NULL);

    // testa alocação negativa
    printf("Testando mem_alloc com tamanho negativo\n");
    ptr = mem_alloc(-100);
    assert(ptr == NULL);

    // testa alocação maior que a memória disponível
    printf("Testando mem_alloc tamanho excessivo\n");
    avail = mem_avail();
    ptr = mem_alloc(avail + 1);
    assert(ptr == NULL);

    // testa free nulo
    printf("Testando mem_free com ponteiro nulo\n");
    status = mem_free(NULL);
    assert(status == ERROR);

    // testa free inválido
    printf("Testando mem_free com ponteiro inválido\n");
    status = mem_free(&status);
    assert(status == ERROR);

    // alocações aleatórias
    printf("Criando blocos de tamanho aleatório\n");
    avail_before = mem_avail();
    nblocks = 0;
    do
    {
        // aloca um bloco de tamanho aleatorio
        size = randnum() % (mem_size() / 1024);
        ptr = mem_alloc(size);

        if (ptr)
        {
            // guarda o bloco alocado
            block[nblocks] = ptr;
            bsize[nblocks] = size;
            nblocks++;

            // ao chegar em 10 blocos, gera relatório
            if (nblocks == 10)
            {
                printf("Testando mem_report (10 alocações)\n");
                mem_report();
            }
        }
    } while (mem_avail() > 4096);

    printf("Foram alocados %d blocos\n", nblocks);

    // testa se alocações estão alinhadas em 16 bytes
    printf("Testando alinhamento dos blocos alocados\n");
    for (int i = 0; i < nblocks; i++)
        assert(((unsigned long)block[i] % 16) == 0);

    // preenche os blocos alocados
    printf("Preenchendo blocos com bytes fixos\n");
    for (int i = 0; i < nblocks; i++)
    {
        // preenche bloco com valor fixo
        for (int j = 0; j < bsize[i]; j++)
            block[i][j] = i % 256;
    }

    // verifica o conteúdo dos blocos alocados
    printf("Verificando conteúdo dos blocos preenchidos\n");
    for (int i = 0; i < nblocks; i++)
    {
        // verifica o conteúdo do bloco
        for (int j = 0; j < bsize[i]; j++)
            assert(block[i][j] == i % 256);
    }

    // liberando os blocos alocados acima
    printf("Liberando os blocos alocados\n");
    for (int i = 0; i < nblocks; i++)
    {
        status = mem_free(block[i]);
        assert(status == NOERROR);

        // ao chegar aos últimos N blocos, gera relatório
        if (nblocks - (i + 1) == 10)
        {
            printf("Testando mem_report (10 alocações)\n");
            mem_report();
        }
    }

    // verificando se toda a memória foi liberada
    printf("Verificando a memória disponível\n");
    avail_after = mem_avail();
    printf("mem_avail retornou %d bytes\n", avail_after);
    assert(avail_after == avail_before);
    printf("Testando mem_report (sem alocações do usuário)\n");
    mem_report();

    printf("Testes concluídos com sucesso!\n");

    task_exit(0);
}
