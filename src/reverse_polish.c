#include "reverse_polish.h"

Element *push(int element_code, int element_value, char *element_name) {
	Element *pointer;
	pointer = (Element *)malloc(sizeof(Element));
	pointer->code = element_code;
	pointer->value = element_value;
	pointer->name = (char *)element_name;
	pointer->next = element_stack;
	paso++;
	return pointer;
}

Element *pop () {
	Element *pointer;
	pointer = element_stack;
	element_stack = element_stack->next;
	paso--;
	return pointer;
}

char *translate(int code) {
	char *c = (char *)malloc(sizeof(char));
	*c = (char)code;
	switch (code) {
		case BOOL: return("bool");
		case INT: return("int");
		case T: return("true");
		case F: return("false");
		case IF: return("if");
		case ELSE: return("else");
		case WHILE: return("while");
		case AND: return("&&");
		case OR: return("||");
		case MENOR_IGUAL: return("<=");
		case MAYOR_IGUAL: return(">=");
		case DISTINTO: return("!=");
		case IGUAL: return("==");
		case BI: return("BI");
		case BF: return("BF");
		case CL: return("CL");
		default: return c;
	}
}

void printstack(Element *ptr) {
	if (ptr != (Element *)0) {
		switch (ptr->code) {
			case NUMERO: printf(" %d |", ptr->value); break;
			case IDENTIFICADOR: printf(" %s |", ptr->name); break;
			case PASO: printf(" [%d] |", ptr->value); break;
			default: printf(" %s |", translate(ptr->code)); break;
		}
		printstack(ptr->next);
	}
}

void reverse() {
	Element *ptr, *pointer, *ret = (Element *)0;
	pointer = element_stack;
	while (pointer != (Element *)0) {
		ptr = (Element *)malloc(sizeof(Element));
		ptr->code = pointer->code;
		ptr->value = pointer->value;
		ptr->name = pointer->name;
		ptr->next = ret;
		ret = ptr;
		pointer = pointer->next;
	}
	element_stack = ret;
}

void show_stack() {
	Element *pointer;
	pointer = element_stack;
	printf("\n\n\tContenido de la lista en polaca inversa:\n\t");
	printstack(pointer);
	printf("\n\n");
}
