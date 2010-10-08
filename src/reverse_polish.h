#ifndef __REVERSEPOLISH_H__
#define __REVERSEPOLISH_H__


#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "sc.tab.h"

typedef struct Element {
	int code;				/* codigo para el tipo de elemento: NUMERO, IDENTIFICADOR, DISTINTO, '>', etc */
	int value;				/* valor asociado a un operando */
	char *name;				/* nombre de un IDENTIFICADOR */	
	struct Element *next; 	/* link al siguiente simbolo */
} Element;

extern Element *element_stack;
extern int paso;

Element *push(int element_code, int element_value, char *element_name);
Element *pop();
char *translate(int code);
void printstack(Element *ptr);
void reverse();
void show_stack();


#endif
