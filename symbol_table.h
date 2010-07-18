#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "sc.tab.h"

typedef struct Symbol {
	char *name; 	/* identificador de la variable */
	int type;     	/* tipo de la variable: BOOL, INT o UNDEFINED */
   	int value;  	/* valor de la variable */
	struct Symbol *next; 	/* link al siguiente simbolo */
} Symbol;

void add_symbol (Symbol *sym);
Symbol *get_symbol (char *symbol_name);
void show_all ();
Symbol *get_first();
Symbol *create_symbol(char *symbol_name);
Symbol *symbol_from_string(char *symbol_name);
int is_defined(char *symbol_name);


#endif
