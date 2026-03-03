#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../lib/queue.h"

void print_elem(void *ptr) {
    if (ptr) {
        printf("%d ", *(int *)ptr);
    }
}

int main() {
    printf("Iniciando testes da fila...\n");

    struct queue_t *q1 = queue_create();
    assert(q1 != NULL);
    assert(queue_size(q1) == 0);

    int v1 = 1, v2 = 2, v3 = 3;

    // Testar queue_add
    assert(queue_add(q1, &v1) == NOERROR);
    assert(queue_size(q1) == 1);
    assert(queue_has(q1, &v1) == true);
	assert(queue_head(q1) == &v1);
	assert(queue_item(q1) == &v1);

    assert(queue_add(q1, &v2) == NOERROR);
    assert(queue_add(q1, &v3) == NOERROR);
    assert(queue_size(q1) == 3);

    // Testar iterator
    void *item = queue_head(q1);
    assert(item == &v1);

    item = queue_next(q1);
    assert(item == &v2);

    item = queue_next(q1);
    assert(item == &v3);

    item = queue_next(q1);
    assert(item == NULL);

    // Testar impressao
    printf("Esperado: Fila 1: [ 1 2 3 ] (3 itens)\n");
    queue_print("Fila 1", q1, print_elem);

    // Testar queue_del
    assert(queue_del(q1, &v2) == NOERROR);
    assert(queue_size(q1) == 2);
    assert(queue_has(q1, &v2) == false);

    assert(queue_del(q1, &v1) == NOERROR);
    assert(queue_size(q1) == 1);

    assert(queue_del(q1, &v3) == NOERROR);
    assert(queue_size(q1) == 0);

    // Test errors
    assert(queue_del(q1, &v1) == ERROR);
    assert(queue_add(NULL, &v1) == ERROR);
    assert(queue_add(q1, NULL) == ERROR);

    assert(queue_destroy(q1) == NOERROR);

    printf("Todos os testes passaram!\n");

    return 0;
}
