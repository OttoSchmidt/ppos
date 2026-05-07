// PingPongOS - PingPong Operating System
// GRR20244386 - Otto Schmidt
// GRR20244625 - Vinícius Hasse Nascimento

#include "queue.h"

#include <stdio.h>
#include <stdlib.h>

struct queue_item_t {
	void *item;
	struct queue_item_t *prox;
};

struct queue_t {
	unsigned int tam;
	struct queue_item_t *head;
	struct queue_item_t *tail;
	struct queue_item_t *iter;
};

// Cria uma fila inicialmente vazia.
// Retorno: ponteiro p/ a nova fila
//          NULL se houver erro
struct queue_t *queue_create() {
	struct queue_t* fila = (struct queue_t*) calloc(1, sizeof(struct queue_t));
	return fila;
}

// Destroi uma fila, liberando a memória alocada por ela.
// IMPORTANTE: os itens apontados pela fila NÃO devem ser liberados,
// pois a aplicação que os criou e pôs na fila é responsável por eles.
// Retorno: NOERROR ou ERROR (se a fila não existir)
int queue_destroy(struct queue_t *queue) {
	if (!queue)
		return ERROR;

	queue->iter = queue->head;
	while (queue->iter != queue->tail) {
		struct queue_item_t *prox = queue->iter->prox;
		free(queue->iter);
		queue->iter = prox;
	}
	if (queue->tail)
		free(queue->tail);

	free(queue);
	return NOERROR;
}

// Adiciona um item no fim da fila; ajusta o iterador para ele
// se for o primeiro item (ou seja, se a fila estiver vazia).
// Retorno: NOERROR ou ERROR (se fila ou item não existir)
int queue_add(struct queue_t *queue, void *item) {
	if (!queue || !item)
		return ERROR;

	struct queue_item_t *novo_item = (struct queue_item_t*) malloc(sizeof(struct queue_item_t));
	novo_item->item = item;
	novo_item->prox = NULL;

	// primeiro elemento na fila
	if (queue->tam == 0) {
		queue->head = novo_item;
		queue->iter = novo_item;
	} else {
		queue->tail->prox = novo_item;
	}

	queue->tail = novo_item;
	(queue->tam)++;

	return NOERROR;
}

// Retira da fila o item com o valor indicado; se o item estiver
// em mais de uma posição da fila, retira apenas da primeira posição
// encontrada; se o item estiver apontado pelo iterador, este avança
// para o próximo item da fila (ou para NULL, se for o último).
// Retorno: NOERROR ou ERROR (não encontrou ou outro erro).
int queue_del(struct queue_t *queue, void *item) {
	if (!queue || !item || queue->tam == 0)
		return ERROR;

	struct queue_item_t *iter = queue->head;
	struct queue_item_t *prev = NULL;

	while (iter) {
		if (iter->item == item) {
			if (prev)
				prev->prox = iter->prox;
			
			if (iter == queue->head) { 
				// verifica se o item esta na cabeca; se for o ultimo
				// elemento da lista, a cabeca sera NULL
				queue->head = iter->prox;
			}
			
			if (iter == queue->tail) { 
				// verifica se o item esta no final; se for o ultimo
				// elemento da lista, a cauda sera NULL
				queue->tail = prev;
				if (prev) 
					prev->prox = NULL;
			}
			
			if (iter == queue->iter) {
				// verifica se o item eh o iterador da lista; se for
				// o ultimo, o iterador sera NULL
				queue->iter = iter->prox;
			}

			free(iter);
			(queue->tam)--;

			return NOERROR;
		}

		prev = iter;
		iter = iter->prox;
	}

	return ERROR;
}

// Informa se o item indicado está na fila.
// Retorno: true/false (error: false).
bool queue_has(struct queue_t *queue, void *item) {
	if (!queue || !item || queue->tam == 0) 
		return false;

	struct queue_item_t *iter = queue->head;
	while (iter) {
		if (iter->item == item)
			return true;

		iter = iter->prox;
	}

	return false;
}

// Informa o número de itens na fila.
// Retorno: número de itens na fila (>= 0)
//          ERROR se a fila não existir
int queue_size(struct queue_t *queue) {
	if (!queue)
		return ERROR;

	return queue->tam;
}

// Põe o iterador no início da fila.
// Retorno: ptr para o item apontado pelo iterador
//          NULL se a fila estiver vazia ou não existir
void *queue_head(struct queue_t *queue) {
	if (!queue || !queue->head)
		return NULL;
	
	queue->iter = queue->head;

	return queue->iter->item;
}

// Avança o iterador ao próximo item na fila.
// Retorno: ptr para o item apontado pelo iterador após avançar
//          NULL se o iterador passou do último item da fila
//          NULL se a fila estiver vazia ou não existir
void *queue_next(struct queue_t *queue) {
	if (!queue || !queue->iter)
		return NULL;

	queue->iter = queue->iter->prox;

	if (!queue->iter)
		return NULL;
	return queue->iter->item;
}

// Informa o item atualmente sob o iterador na fila.
// Retorno: ptr para o item apontado pelo iterador
//          NULL se a fila estiver vazia ou não existir
//          NULL se o iterador passou do fim da fila
void *queue_item(struct queue_t *queue) {
	if (!queue || !queue->iter)
		return NULL;
	
	return queue->iter->item;
}

// Imprime os elementos de uma fila; a função externa "func"
// deve ser chamada para imprimir cada item.
// Exemplos de saída, com name == "Frutas":
// Frutas: [ banana pera ameixa uva ] (4 itens)
// Frutas: [ ] (0 itens)
// Frutas: undef   se queue == NULL
// Frutas: [ undef undef undef ] (3 itens)  se func == NULL
void queue_print(char *name, struct queue_t *queue, void(func)(void *)) {
	printf("%s: ", name);

	if (!queue) {
		printf("undef\n");
	} else if (queue->tam == 0) {
		printf("[ ] (0 itens)\n");
	} else if (!func) {
		printf("[ ");
		for (unsigned i = 0; i < queue->tam; i++) {
			printf("undef ");
		}
		printf("] (%d itens)\n", queue->tam);
	} else {
		struct queue_item_t *iter = queue->head;

		printf("[ ");
		while (iter) {
			func(iter->item);
			iter = iter->prox;
		}
		printf("] (%d itens)\n", queue->tam);
	}
}
