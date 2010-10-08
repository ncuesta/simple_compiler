#ifndef __VARIABLES_H__
#define __VARIABLES_H__


#include <stdlib.h>
#include <stdio.h>
#include "symbol_table.h"
#include "stack.h"
#include "reverse_polish.h"
#include "constants.h"
#include "sc.tab.h"


extern int error;

char *get_type_string(Symbol *sym);
void declare(Symbol *sym, int type, int initial_value);
int compatible(Symbol *sym1, Symbol *sym2);
void check_overflow(Symbol *value);
void check_declare(Symbol *sym, int type);
void check_declare_assign(Symbol *sym, int type, Symbol *value, Element **elem_stack);
void check_assign(Symbol *sym, Symbol *value, Element **elem_stack);
Symbol *check_substract(Symbol *sym1, Symbol *sym2, Element **elem_stack);
Symbol *check_add(Symbol *sym1, Symbol *sym2, Element **elem_stack);
Symbol *check_multiply(Symbol *sym1, Symbol *sym2, Element **elem_stack);
Symbol *check_divide(Symbol *sym1, Symbol *sym2, Element **elem_stack);
Symbol *check_or(Symbol *sym1, Symbol *sym2, Element **elem_stack);
Symbol *check_and(Symbol *sym1, Symbol *sym2, Element **elem_stack);
Symbol *check_not(Symbol *sym, Element **elem_stack);
Symbol *check_compare(Symbol *sym1, Symbol *sym2, int operator, Element **elem_stack);
Symbol *check_equality(Symbol *sym1, Symbol *sym2, Element **elem_stack);
Symbol *check_inequality(Symbol *sym1, Symbol *sym2, Element **elem_stack);
void insert_BF(Stack **st, Element **elem_stack);
void check_simple_if(Symbol *condition, Stack **st, Element **elem_stack, int paso);
void check_complex_if(Symbol *condition, Stack **st, Element **elem_stack, int paso);
void check_else(Stack **st, Element **elem_stack, int paso);
void subrout_while(Stack **st, Element **elem_stack, int paso);
void check_while(Symbol *condition, Stack **st, Element **elem_stack, int paso);
void check_while_block(Stack **st, Element **elem_stack, int paso);


#endif
