#ifndef __STACK_H__
#define __STACK_H__


#include <stdlib.h>
typedef struct Stack {
	void *ptr;		/* elemento */
	struct Stack *next; 	/* link al siguiente simbolo */
} Stack;

void spush(Stack **where, void *input);
void spop (Stack **where, void **output);


void sprint(Stack *stack);

#endif
