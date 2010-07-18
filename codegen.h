#ifndef __CODEGEN_H__
#define __CODEGEN_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symbol_table.h"
#include "reverse_polish.h"
#include "stack.h"
#include "constants.h"
#include "sc.tab.h"

int is_operand(Element *el);
void generate();
void generar_suma(Element *obj);
void generar_resta(Element *obj);
void generar_multiplicacion(Element *obj);
void generar_division(Element *obj);
void generar_asignacion(Element *obj);
void generar_not(Element *obj);
void generar_and(Element *obj);
void generar_or(Element *obj);
void generar_comparacion(Element *obj, char *etiqueta);
void generar_BF(Element *obj);
void generar_BI(Element *obj);
void create_label(Element *obj);
Element *new_variable();
char *translate_index(int index);
Element *load_register(int index);
Element *get_free();
Element *get_free_AL();
Element *free_any(Element *variable);
Element *free_AL(Element *variable);
Element *get_free_notA();
Element *free_any_notA(Element *variable);


#endif
