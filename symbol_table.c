#include "symbol_table.h"

Symbol *symbol_table;

void add_symbol (Symbol *sym) {
	sym->next = (struct Symbol *)symbol_table;
	symbol_table = sym;
}

Symbol *get_symbol (char *symbol_name) {
	Symbol *ptr;
	for (ptr = symbol_table; ptr != (Symbol *) 0; ptr = (Symbol *)ptr->next)
	if (strcmp (ptr->name,symbol_name) == 0)
		return ptr;
	return NULL;
}

void show_all () {
	Symbol *ptr;
	printf("\n\n\tContenido de la tabla de simbolos:\n\t(tipo\tidentificador -> valor)\n");
	for (ptr = symbol_table; ptr != (Symbol *) 0; ptr = (Symbol *)ptr->next) {
		printf("\t%s\t%s -> ", (ptr->type == INT) ? "int" : "bool", ptr->name);
		if (ptr->type == BOOL) {
			printf("%s\n", (ptr->value == TRUE) ? "true" : "false");
		} else {
			printf("%d\n", ptr->value);
		}
	}
	printf("\n\n");
}

Symbol *get_first() {
	Symbol *ptr;
	ptr = symbol_table;
	symbol_table = symbol_table->next;
	return ptr;
}

Symbol *create_symbol(char *symbol_name) {
	Symbol *ptr;
	ptr = (Symbol *)malloc(sizeof(Symbol));
	ptr->name = (char *)malloc(strlen(symbol_name)+1);
	strcpy(ptr->name, symbol_name);
	ptr->type = UNDEFINED;
	return ptr;
}

Symbol *symbol_from_string(char *symbol_name) {
	Symbol *ptr;
	if ((ptr = get_symbol(symbol_name)) == NULL)
		ptr = create_symbol(symbol_name);
	return ptr;
}

int is_defined(char *symbol_name) {
	return (get_symbol(symbol_name) != NULL);
}
