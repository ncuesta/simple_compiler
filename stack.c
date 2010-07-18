#include "stack.h"

void spush(Stack **where, void *input) {
	Stack *pointer = (Stack *)calloc(1, sizeof(Stack));
	
	pointer->ptr = input;
	pointer->next = *where;
	*where = pointer;
}

void spop(Stack **where, void **output) {
	Stack *pointer = (Stack *)calloc(1, sizeof(Stack));
	
	pointer = *where;
	*where = (*where)->next;
	*output = pointer->ptr;
}


#include "sc.tab.h"
#include "reverse_polish.h"
#include <stdio.h>
void sprint(Stack *stack) {
	Stack *pointer = stack;
	Element *aux;
	
	printf("))))) Contenido de la pila (((((\n");
	while (pointer != (Stack *)0) {
		spop(&pointer, (void *)(&aux));
		if (aux->code == REGISTRO)
			printf("\t\tREGISTRO %s\n", aux->name);
		else if (aux->code == IDENTIFICADOR)
			printf("\t\tIDENTIFICADOR %s\n", aux->name);
		else
			printf("\t\tNUMERO %d\n", aux->value);
	}
	printf(")))))))))))))))))(((((((((((((((\n\n");
}
