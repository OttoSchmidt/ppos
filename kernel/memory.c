// PingPongOS - PingPong Operating System
// GRR20244386 - Otto Schmidt
// GRR20244625 - Vinícius Hasse Nascimento

#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>

#include "macros.h"

// tamanho total da heap: 64 MB
#define HEAP_SIZE 64*1024*1024
#define MAX_DESC HEAP_SIZE/16

struct mem_t {
    bool free;
    char *start;
    unsigned int size;
    struct mem_t *prev;
    struct mem_t *prox;
};

struct mem_state_t {
    int used_memory;
    struct mem_t *blocks;
    struct mem_t *unused_blocks;
};

static char heap[HEAP_SIZE];
static struct mem_state_t ms;
static struct mem_t mem_blocks[MAX_DESC];

// inicia o subsistema de memória RAM (heap)
void mem_init() {
    ms.used_memory = 0;

    // primeiro bloco representa toda a heap
    mem_blocks[0].free = true;
    mem_blocks[0].start = heap;
    mem_blocks[0].size = HEAP_SIZE;
    mem_blocks[0].prev = &mem_blocks[0];
    mem_blocks[0].prox = &mem_blocks[0];

    ms.blocks = &mem_blocks[0];

    // outros blocos ficam na lista de livres
    if (MAX_DESC > 1) {
        ms.unused_blocks = &mem_blocks[1];
        for (int i = 1; i < MAX_DESC - 1; i++) {
            mem_blocks[i].prox = &mem_blocks[i+1];
        }
        mem_blocks[MAX_DESC - 1].prox = NULL;
    } else {
        ms.unused_blocks = NULL;
    }
}

// informa a quantidade de memória total, em bytes
int mem_size() {
    return HEAP_SIZE;
}

// informa a quantidade de memória disponível, em bytes
int mem_avail() {
    return HEAP_SIZE - ms.used_memory;
}

// aloca um bloco de memória com o tamanho indicado
// retorna ponteiro ou NULL se houver erro
void *mem_alloc(int size) {
    if (size <= 0)
        return NULL;

    // ajustar tamanho para ser multiplo de 16
    if (size % 16 != 0) {
        size += 16 - (size % 16);
    }

    if (mem_avail() < size) {
        return NULL;
    }

    struct mem_t *first = ms.blocks;
    if (!first)
        return NULL;

    struct mem_t *b = first;
    struct mem_t *chosen = NULL;

    // procurar bloco livre compatível (first fit)
    do {
        if (b->free && b->size >= (unsigned int)size) {
            chosen = b;
            break;
        }
        b = b->prox;
    } while (b != first);

    // memoria indisponivel
    if (!chosen) {
        #ifdef DEBUG
        ppos_debug("[MEM] bloco livre compativel nao foi encontrado\n");
        #endif
        return NULL;
    }

    // verificar se o bloco pode ser dividido
    if (chosen->size > (unsigned int)size && ms.unused_blocks != NULL) {
        struct mem_t *new_block = ms.unused_blocks;
        ms.unused_blocks = new_block->prox;

        new_block->start = chosen->start + size;
        new_block->size = chosen->size - size;
        new_block->free = true;

        new_block->prox = chosen->prox;
        new_block->prev = chosen;
        chosen->prox->prev = new_block;
        chosen->prox = new_block;

        chosen->size = size;
    }

    chosen->free = false;
    ms.used_memory += chosen->size;

    return chosen->start;
}

// libera um bloco de memória previamente alocado
// retorna NOERROR se ok ou ERROR se ptr for NULL ou inválido
int mem_free(void *ptr) {
    if (!ptr)
        return -1;

    struct mem_t *first = ms.blocks;
    if (!first)
        return -1;

    struct mem_t *b = first;
    struct mem_t *found = NULL;

    do {
        if (b->start == ptr && !b->free) {
            found = b;
            break;
        }
        b = b->prox;
    } while (b != first);

    // bloco nao encontrado ou ja esta livre
    if (!found)
        return -1;

    found->free = true;
    ms.used_memory -= found->size;

    // merge com o proximo
    if (found->prox != found && found->prox->free) {
        if (found->start + found->size == found->prox->start) {
            struct mem_t *next = found->prox;
            found->size += next->size;

            found->prox = next->prox;
            next->prox->prev = found;

            if (ms.blocks == next) {
                ms.blocks = found;
            }

            next->prox = ms.unused_blocks;
            ms.unused_blocks = next;
        }
    }

    // merge com o anterior
    if (found->prev != found && found->prev->free) {
        if (found->prev->start + found->prev->size == found->start) {
            struct mem_t *prev = found->prev;
            prev->size += found->size;

            prev->prox = found->prox;
            found->prox->prev = prev;

            if (ms.blocks == found) {
                ms.blocks = prev;
            }

            found->prox = ms.unused_blocks;
            ms.unused_blocks = found;
        }
    }

    return 0;
}

// gera um relatório sobre o uso da memória
void mem_report() {
    int alloc_bytes = 0, alloc_blocks = 0;
    int free_bytes = 0, free_blocks = 0;
    struct mem_t *b = ms.blocks;

    if (b) {
        do {
            if (b->free) {
                free_bytes += b->size;
                free_blocks++;
            } else {
                alloc_bytes += b->size;
                alloc_blocks++;
            }
            b = b->prox;
        } while (b != ms.blocks && b != NULL);
    }

    printf("heap: %d KB allocated (%d blocks), %d KB free (%d blocks)\n", 
            alloc_bytes / 1024, alloc_blocks, free_bytes / 1024, free_blocks);

    b = ms.blocks;
    if (b) {
        do {
            int idx = b - mem_blocks;
            int prev_idx = b->prev ? b->prev - mem_blocks : -1;
            int next_idx = b->prox ? b->prox - mem_blocks : -1;
            
            printf("heap: block %5d: %p - %p %s prev %5d next %5d size %d\n", 
                    idx, b->start, b->start + b->size - 1, b->free ? "FREE" : "aloc", prev_idx, next_idx, b->size);
            b = b->prox;
        } while (b != ms.blocks && b != NULL);
    }
}
