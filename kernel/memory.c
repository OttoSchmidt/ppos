// PingPongOS - PingPong Operating System

// implementação trivial, a ser substituída no projeto de alocação de heap.

#include <stdlib.h>

void mem_init()
{
}

void *mem_alloc(int size)
{
    return (malloc(size));
}

int mem_free(void *ptr)
{
    free(ptr);
    return(0);
}
