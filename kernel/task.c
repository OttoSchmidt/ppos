// PingPongOS - PingPong Operating System
// GRR20244386 - Otto Schmidt

#include "task.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ERROR -1
#define NOERROR 0
#define STACK_SIZE 4096

int new_task_id = 1;
struct task_t *task_atual;

// inicializa o subsistema de tarefas
void task_init() {
	// inicializa a tarefa do kernel (id=0)
	task_atual = (struct task_t *) malloc(sizeof(struct task_t));
	task_atual->id = 0;
	task_atual->name = "kernel";
	task_atual->status = TASK_RUNNING;
	task_atual->owner = NULL;
}

// cria uma nova tarefa: "name" é o nome da tarefa, "entry"
// é a função que ela irá executar e "arg" aponta para o valor
// recebido por "entry" ao iniciar (pode ser NULL).
// retorno: ptr para o descritor da tarefa ou NULL se houver erro
struct task_t *task_create(char *name, void (*entry)(void *),
                           void *arg) 
{
	struct task_t *nova_tarefa = (struct task_t *) malloc(sizeof(struct task_t));
	if (!nova_tarefa)
		return NULL;

	nova_tarefa->id = new_task_id;
	nova_tarefa->name = name;
	nova_tarefa->status = TASK_NEW;
	nova_tarefa->owner = task_atual; // definir a tarefa que criou esta tarefa

	// alocar pilha p/ o contexto
	void *stack = malloc(STACK_SIZE);
	if (!stack) {
		free(nova_tarefa);
		return NULL;
	}

	// criar contexto
	if (ctx_create(&nova_tarefa->context, entry, arg, stack, STACK_SIZE) == ERROR) {
		free(stack);
		free(nova_tarefa);
		return NULL;
	}
	
	new_task_id++;
	nova_tarefa->status = TASK_READY; // tarefa pronta para ser executada

	#ifdef DEBUG
	printf("tarefa criada: '%s'\n", nova_tarefa->name);
	#endif

	return nova_tarefa;
}

// destroi uma tarefa e libera seus recursos;
// somente deve atuar sobre tarefas terminadas.
// Retorno: NOERROR (0) ou ERROR (<0)
int task_destroy(struct task_t *task) {
	if (!task || task->status != TASK_TERMINATED)
		return ERROR;

	free(task->context.stack); // liberar pilha
	free(task); // liberar TCB

	return NOERROR;
}

// transfere a cpu da tarefa atual para outra tarefa;
// se task == NULL, transfere para a tarefa que a criou.
// ignora sem erro se "task" já tiver terminado.
// Retorno: NOERROR (0) ou ERROR (<0)
int task_switch(struct task_t *task) {
	if (!task) {
		// transferir para o dono da tarefa atual, pois a execucao
		// da tarefa atual terminou

		if (!task_atual->owner) { 
			// tarefa atual é a do kernel, não tem dono
			return ERROR;
		}

		struct task_t *task_anterior = task_atual;
		task_atual = task_atual->owner;

		task_atual->status = TASK_RUNNING;
		task_anterior->status = TASK_TERMINATED;

		#ifdef DEBUG
		printf("tarefa '%s' finalizou. trocando tarefa para dono '%s'\n", task_anterior->name, task_atual->name);
		#endif

		ctx_swap(&task_anterior->context, &task_atual->context);

		return NOERROR;
	}

	#ifdef DEBUG
	printf("status da tarefa '%s': %d\n", task->name, task->status);
	#endif
	
	if (task->status != TASK_TERMINATED) { // ignorar sem erro
		// transferir para a nova tarefa. a execucao da tarefa atual foi suspensa

		struct task_t *task_anterior = task_atual; // salvar tarefa atual
		task_atual = task; // atualizar tarefa atual

		task_atual->status = TASK_RUNNING;
		task_anterior->status = TASK_SUSPENDED;

		#ifdef DEBUG
		printf("trocou para tarefa '%s'. tarefa '%s' suspensa\n", task_atual->name, task_anterior->name);
		#endif

		// a função abaixo troca o contexto da cpu, então, por enquanto,
		// essa funcao encerra aqui. a execucao da cpu continua no novo 
		// contexto e somente quando essa nova tarefa retornar, a execucao 
		// voltara após a função ctx_swap.
		ctx_swap(&task_anterior->context, &task->context);
	} else {
		#ifdef DEBUG
		printf("tarefa '%s' nao foi trocada pois ja esta encerrada\n", task->name);
		#endif
	}

	return NOERROR;
}

// informa o ID de uma tarefa (ou da tarefa atual se NULL)
int task_id(struct task_t *task) {
	if (!task)
		return task_atual->id;
	return task->id;
}

// informa o nome de uma tarefa (ou da tarefa atual se NULL)
char *task_name(struct task_t *task) {
	if (!task)
		return task_atual->name;
	if (!task->name)
		return "SEM_NOME";
	return task->name;
}