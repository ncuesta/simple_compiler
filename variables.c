#include "variables.h"
#include "constants.h"

char *get_type_string(Symbol *sym) {
	if (sym->type == INT)
		return "int";
	else if (sym->type == BOOL)
		return "bool";
	else
		return "undefined (tipo no asignado)";
}
void declare(Symbol *sym, int type, int initial_value) {
	sym->type = type;
	sym->value = initial_value;
	add_symbol(sym);
}

int compatible(Symbol *sym1, Symbol *sym2) {
	return (sym1->type == sym2->type);
}

void check_overflow(Symbol *value)
{
	if (value->type == INT && value->value > MAXINT)
	{
		printf("sc: Aviso: desborde de entero con el valor %d (MAXINT = %d).\n", value->value, MAXINT);
	}
}

void check_declare(Symbol *sym, int type) {
	if (is_defined(sym->name))
		printf("sc: Aviso: redefinicion de '%s'. Se ignora la ultima declaracion.\n", sym->name);
	else {
		declare(sym, type, 0);
	}
}

void check_declare_assign(Symbol *sym, int type, Symbol *value, Element **elem_stack) {
	if (is_defined(sym->name)) {
		printf("sc: Aviso: redefinicion de '%s'. Se ignora la ultima declaracion.\n", sym->name);
	} else {
		sym->type = type;
		if (compatible(sym, value)) {
			declare(sym, type, value->value);
			*elem_stack = push(IDENTIFICADOR, 0, sym->name);
			*elem_stack = push('=', 0, (char *)0);
			sym->value = value->value;
			check_overflow(sym);
		} else {
			error = TRUE;
			printf("sc: Error: tipos incompatibles en la asignacion: %s y %s.\n", get_type_string(sym), get_type_string(value));
		}
	}
}

void check_assign(Symbol *sym, Symbol *value, Element **elem_stack) {
	if (is_defined(sym->name)) {
		if (!((compatible(sym, value)) || (sym->type == UNDEFINED))) {
			error = TRUE;
			printf("sc: Error: tipos incompatibles en la asignacion: %s y %s.\n", get_type_string(sym), get_type_string(value));
		}
		if (sym->type == UNDEFINED) {
			printf("sc: Aviso: identificador '%s' sin tipo asignado aun.\n\t\tSe infiere el tipo %s del miembro derecho de la asignacion.\n", sym->name, get_type_string(value));
			sym->type = value->type;
		}
		*elem_stack = push(IDENTIFICADOR, 0, sym->name);
		*elem_stack = push('=', 0, (char *)0);
		sym->value = value->value;
		check_overflow(sym);
	} else {
		printf("sc: Aviso: identificador '%s' aun no declarado.\n\t\tSe lo declara e infiere el tipo %s del miembro derecho de la asignacion.\n", sym->name, get_type_string(value));
		declare(sym, value->type, value->value);
		*elem_stack = push(IDENTIFICADOR, 0, sym->name);
		*elem_stack = push('=', 0, (char *)0);
		check_overflow(sym);
	}
}

Symbol *check_substract(Symbol *sym1, Symbol *sym2, Element **elem_stack) {
	check_overflow(sym1);
	check_overflow(sym2);
	Symbol *resp = (Symbol *)malloc(sizeof(Symbol));
	resp->type = INT;
	if (!((((sym1->type == INT) && compatible(sym1, sym2))) || (sym1->type == UNDEFINED) || (sym2->type == UNDEFINED))) {
		error = TRUE;
		printf("sc: Error: tipos incompatibles en la resta: %s y %s.\n", get_type_string(sym1), get_type_string(sym2));
	}
	if (sym1->type == UNDEFINED) {
		printf("sc: Aviso: Tipo int inferido de la resta para '%s' sin tipo asignado aun.\n", sym1->name);
		sym1->type = INT;
	}
	if (sym2->type == UNDEFINED) {
		printf("sc: Aviso: Tipo int inferido de la resta para '%s' sin tipo asignado aun.\n", sym2->name);
		sym2->type = INT;
	}
	*elem_stack = push('-', 0, (char *)0);
	resp->value = sym1->value - sym2->value;

	return resp;
}

Symbol *check_add(Symbol *sym1, Symbol *sym2, Element **elem_stack) {
	check_overflow(sym1);
	check_overflow(sym2);
	Symbol *resp = (Symbol *)malloc(sizeof(Symbol));
	resp->type = INT;
	if (!((((sym1->type == INT) && compatible(sym1, sym2))) || (sym1->type == UNDEFINED) || (sym2->type == UNDEFINED))) {
		error = TRUE;
		printf("sc: Error: tipos incompatibles en la suma: %s y %s.\n", get_type_string(sym1), get_type_string(sym2));
	}
	if (sym1->type == UNDEFINED) {
		printf("sc: Aviso: Tipo int inferido de la suma para '%s' sin tipo asignado aun.\n", sym1->name);
		sym1->type = INT;
	}
	if (sym2->type == UNDEFINED) {
		printf("sc: Aviso: Tipo int inferido de la suma para '%s' sin tipo asignado aun.\n", sym2->name);
		sym2->type = INT;
	}
	*elem_stack = push('+', 0, (char *)0);
	resp->value = sym1->value + sym2->value;
	check_overflow(resp);

	return resp;
}

Symbol *check_multiply(Symbol *sym1, Symbol *sym2, Element **elem_stack) {
	check_overflow(sym1);
	check_overflow(sym2);
	Symbol *resp = (Symbol *)malloc(sizeof(Symbol));
	resp->type = INT;
	if (!((((sym1->type == INT) && compatible(sym1, sym2))) || (sym1->type == UNDEFINED) || (sym2->type == UNDEFINED))) {
		error = TRUE;
		printf("sc: Error: tipos incompatibles en la multiplicacion: %s y %s.\n", get_type_string(sym1), get_type_string(sym2));
	}
	if (sym1->type == UNDEFINED) {
		printf("sc: Aviso: Tipo int inferido de la multiplicacion para '%s' sin tipo asignado aun.\n", sym1->name);
		sym1->type = INT;
	}
	if (sym2->type == UNDEFINED) {
		printf("sc: Aviso: Tipo int inferido de la multiplicacion para '%s' sin tipo asignado aun.\n", sym2->name);
		sym2->type = INT;
	}
	*elem_stack = push('*', 0, (char *)0);
	resp->value = sym1->value * sym2->value;
	check_overflow(resp);

	return resp;
}

Symbol *check_divide(Symbol *sym1, Symbol *sym2, Element **elem_stack) {
	check_overflow(sym1);
	check_overflow(sym2);
	Symbol *resp = (Symbol *)malloc(sizeof(Symbol));
	resp->type = INT;
	if (!((((sym1->type == INT) && compatible(sym1, sym2))) || (sym1->type == UNDEFINED) || (sym2->type == UNDEFINED))) {
		error = TRUE;
		printf("sc: Error: tipos incompatibles en la division: %s y %s.\n", get_type_string(sym1), get_type_string(sym2));
	}
	if (sym1->type == UNDEFINED) {
		printf("sc: Aviso: Tipo int inferido de la division para '%s' sin tipo asignado aun.\n", sym1->name);
		sym1->type = INT;
	}
	if (sym2->type == UNDEFINED) {
		printf("sc: Aviso: Tipo int inferido de la division para '%s' sin tipo asignado aun.\n", sym2->name);
		sym2->type = INT;
		sym2->value = 1;
	}
	if (sym2->value == 0) {
/*		error = TRUE;*/
		printf("sc: Advertencia: posibe division por 0!\n");
		resp->value = MAXINT;
	}
	else
	{
		resp->value = sym1->value / sym2->value;
	}
	
	*elem_stack = push('/', 0, (char *)0);

	return resp;
}

Symbol *check_or(Symbol *sym1, Symbol *sym2, Element **elem_stack) {
	Symbol *resp = (Symbol *)malloc(sizeof(Symbol));
	resp->type = BOOL;
	if (!(((sym1->type == BOOL) && (compatible(sym1, sym2))) || (sym1->type == UNDEFINED) || (sym2->type == UNDEFINED))) {
		error = TRUE;
		printf("sc: Error: tipos incompatibles en la disyuncion (OR logico): %s y %s.\n", get_type_string(sym1), get_type_string(sym2));
	}
	if (sym1->type == UNDEFINED) {
		printf("sc: Aviso: identificador '%s' sin tipo asignado aun. Se infiere su tipo a bool.\n", sym1->name);
		sym1->type = BOOL;
	}
	if (sym2->type == UNDEFINED) {
		printf("sc: Aviso: identificador '%s' sin tipo asignado aun. Se infiere su tipo a bool.\n", sym2->name);
		sym2->type = BOOL;
	}
	*elem_stack = push(OR, 0, (char *)0);
	resp->value = sym1->value || sym2->value;

	return resp;
}

Symbol *check_and(Symbol *sym1, Symbol *sym2, Element **elem_stack) {
	Symbol *resp = (Symbol *)malloc(sizeof(Symbol));
	resp->type = BOOL;
	if (!(((sym1->type == BOOL) && (compatible(sym1, sym2))) || (sym1->type == UNDEFINED) || (sym2->type == UNDEFINED))) {
		error = TRUE;
		printf("sc: Error: tipos incompatibles en la conjuncion (AND logico): %s y %s.\n", get_type_string(sym1), get_type_string(sym2));
	}
	if (sym1->type == UNDEFINED) {
		printf("sc: Aviso: identificador '%s' sin tipo asignado aun. Se infiere su tipo a bool.\n", sym1->name);
		sym1->type = BOOL;
	}
	if (sym2->type == UNDEFINED) {
		printf("sc: Aviso: identificador '%s' sin tipo asignado aun. Se infiere su tipo a bool.\n", sym2->name);
		sym2->type = BOOL;
	}
	*elem_stack = push(AND, 0, (char *)0);
	resp->value = sym1->value && sym2->value;

	return resp;
}

Symbol *check_not(Symbol *sym, Element **elem_stack) {
	Symbol *resp = (Symbol *)malloc(sizeof(Symbol));
	resp->type = BOOL;
	if (!((sym->type == BOOL) || (sym->type == UNDEFINED))) {
		error = TRUE;
		printf("sc: Error: tipo incompatible en la negacion (NOT logico): %s.\n", get_type_string(sym));
	}
	if (sym->type == UNDEFINED) {
		printf("sc: Aviso: identificador '%s' sin tipo asignado aun. Se infiere su tipo a bool.\n", sym->name);
		sym->type = BOOL;
	}
	*elem_stack = push('!', 0, (char *)0);
	resp->value = !sym->value;

	return resp;
}

Symbol *check_compare(Symbol *sym1, Symbol *sym2, int operator, Element **elem_stack) {
	Symbol *resp = (Symbol *)malloc(sizeof(Symbol));
	resp->type = BOOL;
	if (!((sym1->type == INT) && (compatible(sym1, sym2)))) {
		error = TRUE;
		printf("sc: Error: tipos incompatibles en la comparacion: %s y %s.\n", get_type_string(sym1), get_type_string(sym2));
	}
	check_overflow(sym1);
	check_overflow(sym2);
	*elem_stack = push(operator, 0, (char *)0);
	switch(operator) {
		case '>':
			resp->value = sym1->value > sym2->value;
			break;
		case '<':
			resp->value = sym1->value < sym2->value;
			break;
		case MENOR_IGUAL:
			resp->value = sym1->value <= sym2->value;
			break;
		case MAYOR_IGUAL:
			resp->value = sym1->value >= sym2->value;
			break;
	}

	return resp;
}

Symbol *check_equality(Symbol *sym1, Symbol *sym2, Element **elem_stack) {
	Symbol *resp = (Symbol *)malloc(sizeof(Symbol));
	resp->type = BOOL;
	if (!compatible(sym1, sym2)) {
		error = TRUE;
		printf("sc: Error: tipos incompatibles en la comparacion por igualdad: %s y %s.\n", get_type_string(sym1), get_type_string(sym2));
	}
	*elem_stack = push(IGUAL, 0, (char *)0);

	return resp;
}

Symbol *check_inequality(Symbol *sym1, Symbol *sym2, Element **elem_stack) {
	Symbol *resp = (Symbol *)malloc(sizeof(Symbol));
	resp->type = BOOL;
	if (!compatible(sym1, sym2)) {
		error = TRUE;
		printf("sc: Error: tipos incompatibles en la comparacion por desigualdad: %s y %s.\n", get_type_string(sym1), get_type_string(sym2));
	}
	*elem_stack = push(DISTINTO, 0, (char *)0);

	return resp;
}

void insert_BF(Stack **st, Element **elem_stack) {
	*elem_stack = push(PASO, -1, (char *)0);
	spush (st, (void *)(*elem_stack));
	*elem_stack = push(BF, 0, (char *)0);
}

void check_simple_if(Symbol *condition, Stack **st, Element **elem_stack, int paso) {
	void *pointer;
	
	if (condition->type != BOOL) {
		error = TRUE;
		printf("sc: Error: condicion no booleana en la seleccion (if).\n");
	}
	spop(st, &pointer);
	((Element *)pointer)->value = paso;
	*elem_stack = push(CL, paso, (char *)0);
}

void check_complex_if(Symbol *condition, Stack **st, Element **elem_stack, int paso) {
	void *pointer;
	
	if (condition->type != BOOL) {
		error = TRUE;
		printf("sc: Error: condicion no booleana en la selccion (if).\n");
	}
	spop(st, &pointer);
	*elem_stack = push(PASO, -1, (char *)0);
	spush(st, (void *)(*elem_stack));
	*elem_stack = push(BI, 0, (char *)0);
	((Element *)pointer)->value = paso;
	*elem_stack = push(CL, paso, (char *)0);
}

void check_else(Stack **st, Element **elem_stack, int paso) {
	void *pointer;
	
	spop(st, &pointer);
	((Element *)pointer)->value = paso;
	*elem_stack = push(CL, paso, (char *)0);
}

void subrout_while(Stack **st, Element **elem_stack, int paso) {
	void *pointer = (void *)malloc(sizeof(int));
	*(int *)pointer = paso;
	spush(st, pointer);
	*elem_stack = push(CL, paso, (char *)0);
}

void check_while(Symbol *condition, Stack **st, Element **elem_stack, int paso) {
	if (condition->type != BOOL) {
		error = TRUE;
		printf("sc: Error: condicion no booleana en el bucle while.\n");
	}
	*elem_stack = push(PASO, -1, (char *)0);
	spush (st, (void *)(*elem_stack));
	*elem_stack = push(BF, 0, (char *)0);
}

void check_while_block(Stack **st, Element **elem_stack, int paso) {
	void *pointer, *pointer2;
	
	spop(st, &pointer2);
	spop(st, &pointer);
	*elem_stack = push(PASO, *(int *)pointer, (char *)0);
	*elem_stack = push(BI, 0, (char *)0);
	((Element *)pointer2)->value = paso;
	*elem_stack = push(CL, paso, (char *)0);
}
