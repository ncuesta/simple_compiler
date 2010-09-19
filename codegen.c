#include "codegen.h"

extern FILE *output_fd;
int labelnumber = 0;
/*0 = FREE, 1 = IN USE*/
int state[8] = {0};
Stack *pila = (Stack *)0;

int incluir_rutina_div_cero = FALSE;

/* Generar el codigo con el algoritmo de seguimiento de registros */
void generate() {
	Element *obj;
	extern Symbol *symbol_table;

	fprintf(output_fd, ".MODEL SMALL\n.CODE\n\t\tORG\t100h\n__MAIN:\n");
	while (element_stack != (Element *)0) {
		obj = pop();
		if (is_operand(obj)) {
			spush(&pila, (void *)obj);
		} else {
			switch (obj->code) {
				case '+':
					generar_suma(obj);
					break;
				case '-':
					generar_resta(obj);
					break;
				case '*':
					generar_multiplicacion(obj);
					break;
				case '/':
					generar_division(obj);
					break;
				case '=':
					generar_asignacion(obj);
					break;
				case '!':
					generar_not(obj);
					break;
				case AND:
					generar_and(obj);
					break;
				case OR:
					generar_or(obj);
					break;
				case MENOR_IGUAL:
					generar_comparacion(obj, "JBE");
					break;
				case MAYOR_IGUAL:
					generar_comparacion(obj, "JAE");
					break;
				case DISTINTO:
					generar_comparacion(obj, "JNE");
					break;
				case IGUAL:
					generar_comparacion(obj, "JE");
					break;
				case '>':
					generar_comparacion(obj, "JA");
					break;
				case '<':
					generar_comparacion(obj, "JB");
					break;
				case BF:
					generar_BF(obj);
					break;
				case BI:
					generar_BI(obj);
					break;
				case CL:
					create_label(obj);
					break;
			}
		}
	}
	// Rutina de fin de programa
	fprintf(output_fd, "\tLABEL FIN:          ;Fin de programa\n");
	fprintf(output_fd, "\t\tMOV AH, 00h\n");
	fprintf(output_fd, "\t\tINT 21h\n");
	
	if (incluir_rutina_div_cero)
	{
		fprintf(output_fd, "\n\tLABEL ZERODIV:      ;Rutina de notificacion de division por cero\n");
		fprintf(output_fd, "\t\tNOTIFICAR ERROR!\n");
		fprintf(output_fd, "\t\tJMP FIN\n\n");
	}
	
	/* Generar las declaraciones de variables a partir de la tabla de simbolos */
	Symbol *sym;

	fprintf(output_fd, "\n\n\t\tORG\t1000h\n");
	while (symbol_table != (Symbol *)0) {
		sym = get_first();
		fprintf(output_fd, "\t\t_%s", sym->name);
		fprintf(output_fd, "\tDB\t0\n");
	}
	
	// Cierre de __MAIN
	fprintf(output_fd, "END __MAIN\n");
}

void generar_suma(Element *obj) {
	Element *operando1 = (Element *)malloc(sizeof(Element));
	Element *operando2 = (Element *)malloc(sizeof(Element));

	spop(&pila, (void *)&operando2);
	spop(&pila, (void *)&operando1);
	if (operando1->code == REGISTRO) {
		if (operando2->code == REGISTRO) {
			//Registro-Registro
			fprintf(output_fd, "\t\tADD\t%s, %s\n", operando2->name, operando1->name);
			state[operando1->value] = 0; /* Libero el registro */
			spush(&pila, (void *)operando2);
		} else {
			//Registro-Operando
			if (operando2->code == IDENTIFICADOR) {
				fprintf(output_fd, "\t\tADD\t%s, _%s\n", operando1->name, operando2->name);
			} else {
				fprintf(output_fd, "\t\tADD\t%s, %d\n", operando1->name, operando2->value);
			}
			spush(&pila, (void *)operando1);
		}
	} else {
		if (operando2->code == REGISTRO) {
			//Operando-Registro
			if (operando1->code == IDENTIFICADOR) {
				fprintf(output_fd, "\t\tADD\t%s, _%s\n", operando2->name, operando1->name);
			} else {
				fprintf(output_fd, "\t\tADD\t%s, %d\n", operando2->name, operando1->value);
			}
			spush(&pila, (void *)operando2);
		} else {
			//Operando-Operando
			Element *op_aux = get_free();
			//op_aux es un registro
			if (operando1->code == IDENTIFICADOR) {
				if (operando2->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando1->name);
					fprintf(output_fd, "\t\tADD\t%s, _%s\n", op_aux->name, operando2->name);
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando1->name);
					fprintf(output_fd, "\t\tADD\t%s, %d\n", op_aux->name, operando2->value);
				}
			} else {
				if (operando2->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando1->value);
					fprintf(output_fd, "\t\tADD\t%s, _%s\n", op_aux->name, operando2->name);
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando1->value);
					fprintf(output_fd, "\t\tADD\t%s, %d\n", op_aux->name, operando2->value);
				}
			}
			state[op_aux->value] = 1;
			spush(&pila, (void *)op_aux);
		}
	}
}

void generar_resta(Element *obj) {
	Element *operando1 = (Element *)malloc(sizeof(Element));
	Element *operando2 = (Element *)malloc(sizeof(Element));

	spop(&pila, (void *)&operando2);
	spop(&pila, (void *)&operando1);
	if (operando1->code == REGISTRO) {
		if (operando2->code == REGISTRO) {
			//Registro-Registro
			fprintf(output_fd, "\t\tSUB\t%s, %s\n", operando1->name, operando2->name);
			state[operando2->value] = 0; /*Libero el registro*/
			spush(&pila, (void *)operando1);
		} else {
			//Registro-Operando
			Element *op_aux = get_free();
			if (operando2->code == IDENTIFICADOR) {
				fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando2->name);
				fprintf(output_fd, "\t\tSUB\t%s, %s\n", operando1->name, op_aux->name);
			} else {
				fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando2->value);
				fprintf(output_fd, "\t\tSUB\t%s, %s\n", operando1->name, op_aux->name);
			}
			spush(&pila, (void *)operando1);
		}
	} else {
		Element *op_aux = get_free();
		if (operando2->code == REGISTRO) {
			//Operando-Registro
			if (operando1->code == IDENTIFICADOR) {
				fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando1->name);
				fprintf(output_fd, "\t\tSUB\t%s, %s\n", op_aux->name, operando2->name);
			} else {
				fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando1->value);
				fprintf(output_fd, "\t\tSUB\t%s, %s\n", op_aux->name, operando2->name);
			}
			state[operando2->value] = 0;
			state[op_aux->value] = 1;
			spush(&pila, (void *)op_aux);
		} else {
			//Operando-Operando
			Element *op_aux = get_free();
			if (operando1->code == IDENTIFICADOR) {
				if (operando2->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando1->name);
					fprintf(output_fd, "\t\tSUB\t%s, _%s\n", op_aux->name, operando2->name);
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, %s\n", op_aux->name, operando1->name);
					fprintf(output_fd, "\t\tSUB\t%s, %d\n", op_aux->name, operando2->value);
				}
			} else {
				if (operando2->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando1->value);
					fprintf(output_fd, "\t\tSUB\t%s, _%s\n", op_aux->name, operando2->name);
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando1->value);
					fprintf(output_fd, "\t\tSUB\t%s, %d\n", op_aux->name, operando2->value);
				}
			}
			state[op_aux->value] = 1;
			spush(&pila, (void *)op_aux);
		}
	}
}

void generar_multiplicacion(Element *obj) {
	Element *operando1 = (Element *)malloc(sizeof(Element));
	Element *operando2 = (Element *)malloc(sizeof(Element));

	spop(&pila, (void *)&operando2);
	spop(&pila, (void *)&operando1);
	if (operando1->code == REGISTRO && operando1->value == 0) {
		//operando1 ya esta en AL, pasar operando2 a un registro distinto de AH, si es necesario
		;
	} else {
		if (operando2->code == REGISTRO && operando2->value == 0) {
			//operando2 esta en AL, swapear las variables y pasar a operando2 a un registro distinto de AH, si es necesario
			Element *aux = operando1;
			operando1 = operando2;
			operando2 = aux;
		} else {
			//ninguno esta en AL. Pasar operando1 a AL y operando2 a un registro distinto de AH, si es necesario
			Element *op_aux = get_free_AL();
			if (operando1->code == REGISTRO) {
				fprintf(output_fd, "\t\tMOV\t%s, %s\n", op_aux->name, operando1->name);
				state[operando1->value] = 0;
				operando1 = op_aux;
			} else {
				if (operando1->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando1->name);
					operando1 = op_aux;
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando1->value);
					operando1 = op_aux;				
				}
			}
			state[0] = 1;
		}
	}
	if (operando2->code == REGISTRO) {
		if (operando2->value == 1) {
			//esta en AH, moverlo a otro registro libre.
			Element *op_aux = get_free_notA();
			fprintf(output_fd, "\t\tMOV\t%s, %s\n", op_aux->name, operando2->name);
			operando2 = op_aux;
			state[operando2->value] = 1;
		} else {
			//esta en otro registro. No hacer nada.
			;
		}
	} else {
		//conseguir un registro libre distinto de AH y mover operando2 alli.
		Element *op_aux = get_free_notA();
		if (operando2->code == IDENTIFICADOR) {
			fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando2->name);
			operando2 = op_aux;
		} else {
			fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando2->value);
			operando2 = op_aux;
		}
		state[operando2->value] = 1;
	}
	state[1] = 0;

	//A esta altura, operando1 es AL y operando2 es otro registro distinto de AH
	fprintf(output_fd, "\t\tIMUL\t%s\n", operando2->name);
	state[operando2->value] = 0; /*Libero el registro*/
	//Apilo AL, la parte baja del resultado
	spush(&pila, (void *)operando1);
}

/*
void generar_rutina_div_cero()
{
	fprintf(output_fd, "");
}
*/

void generar_division(Element *obj) {
	Element *operando1 = (Element *)malloc(sizeof(Element));
	Element *operando2 = (Element *)malloc(sizeof(Element));

	spop(&pila, (void *)&operando2);
	spop(&pila, (void *)&operando1);
	if (operando1->code == REGISTRO && operando1->value == 0) {
		//operando1 ya esta en AL, pasar operando2 a un registro distinto de AH, si es necesario
		;
	} else {
		if (operando2->code == REGISTRO && operando2->value == 0) {
			//operando2 esta en AL, pasar a operando2 a un registro distinto y pasar operando1 a AL
			Element *op_aux = get_free_notA();
			fprintf(output_fd, "\t\tMOV\t%s, %s\n", op_aux->name, operando2->name);
			fprintf(output_fd, "\t\tMOV\t%s, %s\n", op_aux->name, operando2->name);
			operando2 = op_aux;
			state[operando2->value] = 1;
			
			Element *op_aux2 = get_free_AL();
			if (operando1->code == REGISTRO) {
				fprintf(output_fd, "\t\tMOV\t%s, %s\n", op_aux2->name, operando1->name);
				state[operando1->value] = 0;
				operando1 = op_aux2;
			} else {
				if (operando1->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux2->name, operando1->name);
					operando1 = op_aux2;
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux2->name, operando1->value);
					operando1 = op_aux2;				
				}
			}
		} else {
			//ninguno esta en AL. Pasar operando1 a AL y operando2 a un registro distinto de AH, si es necesario
			Element *op_aux = get_free_AL();
			if (operando1->code == REGISTRO) {
				fprintf(output_fd, "\t\tMOV\t%s, %s\n", op_aux->name, operando1->name);
				state[operando1->value] = 0;
				operando1 = op_aux;
			} else {
				if (operando1->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando1->name);
					operando1 = op_aux;
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando1->value);
					operando1 = op_aux;
				}
			}
			state[0] = 1;
		}
	}
	if (operando2->code == REGISTRO) {
		if (operando2->value == 1) {
			//esta en AH, moverlo a otro registro libre.
			Element *op_aux = get_free_notA();
			fprintf(output_fd, "\t\tMOV\t%s, %s\n", op_aux->name, operando2->name);
			operando2 = op_aux;
			state[operando2->value] = 1;
		} else {
			//esta en otro registro. No hacer nada.
			;
		}
	} else {
		//conseguir un registro libre distinto de AH y mover operando2 alli.
		Element *op_aux = get_free_notA();
		if (operando2->code == IDENTIFICADOR) {
			fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando2->name);
			operando2 = op_aux;
		} else {
			fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando2->value);
			operando2 = op_aux;
		}
		state[operando2->value] = 1;
	}
	state[1] = 0;
	
	incluir_rutina_div_cero = TRUE;
	
	// Chequear por división por cero
	fprintf(output_fd, "\t\tJNZ LABEL00%d\n", labelnumber);
	fprintf(output_fd, "\t\tJMP ZERODIV\n");
	fprintf(output_fd, "\tLABEL00%d:\n\t\tCBW\n", labelnumber);
	labelnumber++;

	//A esta altura, operando1 es AL y operando2 es otro registro distinto de AH
	fprintf(output_fd, "\t\tIDIV\t%s\n", operando2->name);
	state[operando2->value] = 0; /*Libero el registro*/
	//Apilo AL, el cociente
	spush(&pila, (void *)operando1);
}

void generar_asignacion(Element *obj) {
	Element *operando1 = (Element *)malloc(sizeof(Element));
	Element *operando2 = (Element *)malloc(sizeof(Element));
	
	spop(&pila, (void *)&operando1);
	spop(&pila, (void *)&operando2);
	if (operando1->code == REGISTRO) {
		if (operando2->code == REGISTRO) {
			//Registro-Registro
			fprintf(output_fd, "\t\tMOV\t%s, %s\n", operando2->name, operando1->name);
			state[operando1->value] = 0; /*Libero el registro*/
		} else {
			//Registro-Operando
			if (operando2->code == IDENTIFICADOR)
				fprintf(output_fd, "\t\tMOV\t%s, _%s\n", operando1->name, operando2->name);
			else
				fprintf(output_fd, "\t\tMOV\t%s, %d\n", operando1->name, operando2->value);
		}
	} else {
		if (operando2->code == REGISTRO) {
			//Operando-Registro
			fprintf(output_fd, "\t\tMOV\t_%s, %s\n", operando1->name, operando2->name);
			state[operando2->value] = 0; /*Libero el registro*/
		} else {
			//Operando-Operando: operando1 es si o si IDENTIFICADOR
			Element *op_aux = get_free();
			if (operando2->code == IDENTIFICADOR) {
				fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando2->name);
				fprintf(output_fd, "\t\tMOV\t_%s, %s\n", operando1->name, op_aux->name);
			} else {
				fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando2->value);
				fprintf(output_fd, "\t\tMOV\t_%s, %s\n", operando1->name, op_aux->name);
			}
			state[op_aux->value] = 0;
		}
	}
}

void generar_not(Element *obj) {
	Element *operando = (Element *)malloc(sizeof(Element));

	spop(&pila, (void *)&operando);
	if (operando->code == REGISTRO) {
		fprintf(output_fd, "\t\tNOT\t%s\n", operando->name);
		spush(&pila, (void *)operando);
	} else {
		Element *op_aux = get_free();
		if (operando->code == IDENTIFICADOR) {
			fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando->name);
		} else {
			fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando->value);
		}
		fprintf(output_fd, "\t\tNOT\t%s\n", op_aux->name);
		state[op_aux->value] = 1;
		spush(&pila, (void *)op_aux);
	}
}

void generar_and(Element *obj) {
	Element *operando1 = (Element *)malloc(sizeof(Element));
	Element *operando2 = (Element *)malloc(sizeof(Element));
	
	spop(&pila, (void *)&operando2);
	spop(&pila, (void *)&operando1);
	if (operando1->code == REGISTRO) {
		if (operando2->code == REGISTRO) {
			//Registro-Registro
			fprintf(output_fd, "\t\tAND\t%s, %s\n", operando2->name, operando1->name);
			state[operando1->value] = 0; /*Libero el registro*/
			spush(&pila, (void *)operando2);
		} else {
			//Registro-Operando
			if (operando2->code == IDENTIFICADOR)
				fprintf(output_fd, "\t\tAND\t%s, _%s\n", operando1->name, operando2->name);
			else
				fprintf(output_fd, "\t\tAND\t%s, %d\n", operando1->name, operando2->value);
			spush(&pila, (void *)operando1);
		}
	} else {
		if (operando2->code == REGISTRO) {
			//Operando-Registro
			if (operando1->code == IDENTIFICADOR)
				fprintf(output_fd, "\t\tAND\t%s, _%s\n", operando2->name, operando1->name);
			else
				fprintf(output_fd, "\t\tAND\t%s, %d\n", operando2->name, operando1->value);
			spush(&pila, (void *)operando2);
		} else {
			//Operando-Operando
			Element *op_aux = get_free();
			if (operando1->code == IDENTIFICADOR) {
				if (operando2->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando1->name);
					fprintf(output_fd, "\t\tAND\t%s, _%s\n", op_aux->name, operando2->name);
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando1->name);
					fprintf(output_fd, "\t\tAND\t%s, %d\n", op_aux->name, operando2->value);
				}
			} else {
				if (operando2->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando1->value);
					fprintf(output_fd, "\t\tAND\t%s, _%s\n", op_aux->name, operando2->name);
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando1->value);
					fprintf(output_fd, "\t\tAND\t%s, %d\n", op_aux->name, operando2->value);
				}
			}
			spush(&pila, (void *)op_aux);
			state[op_aux->value] = 1;
		}
	}
}

void generar_or(Element *obj) {
	Element *operando1 = (Element *)malloc(sizeof(Element));
	Element *operando2 = (Element *)malloc(sizeof(Element));
	
	spop(&pila, (void *)&operando2);
	spop(&pila, (void *)&operando1);
	if (operando1->code == REGISTRO) {
		if (operando2->code == REGISTRO) {
			//Registro-Registro
			fprintf(output_fd, "\t\tOR\t%s, %s\n", operando2->name, operando1->name);
			state[operando1->value] = 0; /*Libero el registro*/
			spush(&pila, (void *)operando2);
		} else {
			//Registro-Operando
			if (operando2->code == IDENTIFICADOR)
				fprintf(output_fd, "\t\tOR\t%s, _%s\n", operando1->name, operando2->name);
			else
				fprintf(output_fd, "\t\tOR\t%s, %d\n", operando1->name, operando2->value);
			spush(&pila, (void *)operando1);
		}
	} else {
		if (operando2->code == REGISTRO) {
			//Operando-Registro
			if (operando1->code == IDENTIFICADOR)
				fprintf(output_fd, "\t\tOR\t%s, _%s\n", operando2->name, operando1->name);
			else
				fprintf(output_fd, "\t\tOR\t%s, %d\n", operando2->name, operando1->value);
			spush(&pila, (void *)operando2);
		} else {
			//Operando-Operando
			Element *op_aux = get_free();
			if (operando1->code == IDENTIFICADOR) {
				if (operando2->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando1->name);
					fprintf(output_fd, "\t\tOR\t%s, _%s\n", op_aux->name, operando2->name);
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux->name, operando1->name);
					fprintf(output_fd, "\t\tOR\t%s, %d\n", op_aux->name, operando2->value);
				}
			} else {
				if (operando2->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando1->value);
					fprintf(output_fd, "\t\tOR\t%s, _%s\n", op_aux->name, operando2->name);
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux->name, operando1->value);
					fprintf(output_fd, "\t\tOR\t%s, %d\n", op_aux->name, operando2->value);
				}
			}
			spush(&pila, (void *)op_aux);
			state[op_aux->value] = 1;
		}
	}
}

void generar_comparacion(Element *obj, char *etiqueta) {
	Element *operando1 = (Element *)malloc(sizeof(Element));
	Element *operando2 = (Element *)malloc(sizeof(Element));

	spop(&pila, (void *)&operando2);
	spop(&pila, (void *)&operando1);
	if (operando1->code == REGISTRO) {
		if (operando2->code == REGISTRO) {
			//Registro-Registro
			fprintf(output_fd, "\t\tCMP\t%s, %s\n", operando1->name, operando2->name);
		} else {
			//Registro-Operando
			if (operando2->code == IDENTIFICADOR)
				fprintf(output_fd, "\t\tCMP\t%s, _%s\n", operando1->name, operando2->name);
			else
				fprintf(output_fd, "\t\tCMP\t%s, %d\n", operando1->name, operando2->value);
		}
	} else {
		Element *op_aux2 = get_free(state);
		if (operando2->code == REGISTRO) {
			//Operando-Registro
			if (operando1->code == IDENTIFICADOR) {
				fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux2->name, operando1->name);
				fprintf(output_fd, "\t\tCMP\t%s, %s\n", operando2->name, op_aux2->name);
			} else {
				fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux2->name, operando1->value);
				fprintf(output_fd, "\t\tCMP\t%s, %s\n", operando2->name, op_aux2->name);
			}
		} else {
			//Operando-Operando
			if (operando1->code == IDENTIFICADOR) {
				if (operando2->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux2->name, operando1->name);
					fprintf(output_fd, "\t\tCMP\t%s, _%s\n", op_aux2->name, operando2->name);
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, _%s\n", op_aux2->name, operando1->name);
					fprintf(output_fd, "\t\tCMP\t%s, %d\n", op_aux2->name, operando2->value);
				}
			} else {
				if (operando2->code == IDENTIFICADOR) {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux2->name, operando1->value);
					fprintf(output_fd, "\t\tCMP\t%s, _%s\n", op_aux2->name, operando2->name);
				} else {
					fprintf(output_fd, "\t\tMOV\t%s, %d\n", op_aux2->name, operando1->value);
					fprintf(output_fd, "\t\tCMP\t%s, %d\n", op_aux2->name, operando2->value);
				}
			}
		}
	}
	Element *op_aux = get_free();
	state[op_aux->value] = 1;
	spush(&pila, (void *)op_aux);
	fprintf(output_fd, "\t\t%s\tLABEL00%d\n", etiqueta, labelnumber);
	fprintf(output_fd, "\t\tMOV\t%s, 0\n", op_aux->name);
	fprintf(output_fd, "\t\tJMP\tLABEL00%d\n", labelnumber+1);
	fprintf(output_fd, "\tLABEL00%d:\n\t\tMOV\t%s, 1\n", labelnumber, op_aux->name);
	labelnumber++;
	fprintf(output_fd, "\tLABEL00%d:\n", labelnumber);
	labelnumber++;
}

void generar_BF(Element *obj) {
	Element *numero_paso = (Element *)malloc(sizeof(Element));
	Element *operando = (Element *)malloc(sizeof(Element));

	spop(&pila, (void *)&numero_paso);
	spop(&pila, (void *)&operando);
	if (operando->code == REGISTRO) {
		fprintf(output_fd, "\t\tCMP\t%s, 1\n", operando->name);
	} else {
		if (operando->code == IDENTIFICADOR)
			fprintf(output_fd, "\t\tCMP\t_%s, 1\n", operando->name);
		else
			fprintf(output_fd, "\t\tCMP\t%d, 1\n", operando->value);
	}
	fprintf(output_fd, "\t\tJNE\tLJMP%d\n", numero_paso->value);
}

void generar_BI(Element *obj) {
	Element *operando = (Element *)malloc(sizeof(Element));
	
	spop(&pila, (void *)&operando); //PASO
	fprintf(output_fd, "\t\tJMP\tLJMP%d\n", operando->value);
}

void create_label(Element *obj) {
	fprintf(output_fd, "\tLJMP%d:\n", obj->value);
}

/* Da de alta una nueva variable en la tabla de simbolos y la retorna */
Element *new_variable() {
	Element *e = (Element *)malloc(sizeof(Element));
	static int numero_va = 0;
	char *first_part = "$aux";
	char aux_num[6];
	numero_va++;
	sprintf(aux_num, "%d", numero_va);
	char *name = (char *)calloc(strlen(aux_num) + strlen(first_part) + 1, sizeof(char));
	strcat(name, first_part);
	strcat(name, aux_num);
	Symbol *s = create_symbol(name);
	s->type = INT;
	add_symbol(s);
	e->code = IDENTIFICADOR;
	e->name = name;
	
	return e;
}

/* Traduce un indice a la notacion de registros (AL, AH, BL, BH, ..., DH) */
char *translate_index(int index) {
	char *name = (char *)malloc(sizeof(char)*3);
	name[0] = 'A' + (index / 2);
	name[1] = (index % 2 == 0)?'L':'H';
	name[2] = '\0';
	
	return name;
}

/* Retorna un element cargado como el registro con value index */
Element *load_register(int index) {
	Element *e = (Element *)malloc(sizeof(Element));
	e->code = REGISTRO;
	e->value = index;
	char *aux = malloc(sizeof(char)*3);
	aux = translate_index(index);
	e->name = aux;
	
	return e;
}

/* Libera un registro cualquiera y lo retorna 			*
 * Siempre se retorna un registro, haya o no uno libre 	*
 * El registro quedara marcado como libre 				*/
Element *get_free() {
	int i = N - 1;
	Element *ret = (Element *)malloc(sizeof(Element));
	while (i >= 0 && state[i] == 1) i--;
	if (i >= 0) {
		/* Hay un registro libre, se lo marca como ocupado. */
		ret = load_register(i);
	} else {
		/* No hay registros libres, dar de alta una nueva variable en la tabla de simbolos
		 * y liberar un registro. */
		ret = free_any(new_variable());
		state[ret->value] = 0;
	}
	
	return ret;
}

/* Libera el registro AL y lo retorna */
Element *get_free_AL() {	
	Element *ret = (Element *)malloc(sizeof(Element));
	
	if (state[0] != 0) {
		Element *aux = (Element *)malloc(sizeof(Element));
		aux = new_variable();
		free_AL(aux);
		state[1] = 0;
	}
	ret = load_register(0);
	return ret;
}

/* Libera cualquier registro ocupado usando la variable parametro y lo retorna */
Element *free_any(Element *variable) {
	Stack *pila_aux;
	Element *elem_aux = (Element *)malloc(sizeof(Element));
	Element *ret = (Element *)malloc(sizeof(Element));
	
	/* Busco un registro ocupado para liberarlo */
	spop(&pila, (void *)&elem_aux);
	while (elem_aux->code != REGISTRO) {
		spush(&pila_aux, (void *)elem_aux);
		spop(&pila, (void *)&elem_aux);
	}
	/* Vuelco el contenido del registro a liberar en la variable parametro */
	fprintf(output_fd, "\t\tMOV\t_%s, %s\n", variable->name, elem_aux->name);
	/* Apilo la variable en lugar del registro y    *
	 * guardo en la variable el registro a retornar */
	spush(&pila, (void *)variable);
	ret->code = REGISTRO;
	strcpy(ret->name, elem_aux->name);
	ret->value = elem_aux->value;
	/* Apilo nuevamente los demas operandos sacados de la pila */
	while (elem_aux != (Element *)0) {
		spop(&pila_aux, (void *)&elem_aux);
		spush(&pila, (void *)elem_aux);
	}
	
	return ret;
}

/* Libera el registro AL ocupado usando la variable parametro y lo retorna */
Element *free_AL(Element *variable) {
	Stack *pila_aux = (Stack *)0;
	Element *elem_aux = (Element *)malloc(sizeof(Element));
	Element *ret = (Element *)malloc(sizeof(Element));
	
	/* Busco el registro AL ocupado para liberarlo */
	spop(&pila, (void *)&elem_aux);
	while (elem_aux->code != REGISTRO || (elem_aux->code == REGISTRO && elem_aux->value != 0)) {
		spush(&pila_aux, (void *)elem_aux);
		spop(&pila, (void *)&elem_aux);
	}
	/* Vuelco el contenido del registro a liberar en la variable parametro */
	fprintf(output_fd, "\t\tMOV\t_%s, %s\n", variable->name, elem_aux->name);
	/* Apilo la variable en lugar del registro y    *
	 * guardo en la variable el registro a retornar */
	spush(&pila, (void *)variable);
	ret = load_register(0);
	/* Apilo nuevamente los demas operandos sacados de la pila */	
	while (pila_aux != (Stack *)0) {
		spop(&pila_aux, (void *)&elem_aux);
		spush(&pila, (void *)elem_aux);
	}
	
	return ret;
}

/* Libera un registro que no sea AH o AL y lo retorna	*
 * Siempre se retorna un registro, haya o no uno libre 	*
 * El registro quedara marcado como libre 				*/
Element *get_free_notA() {
	int i = N - 1;
	Element *ret = (Element *)malloc(sizeof(Element));
	
	while (i > 1 && state[i] == 1) i--;
	if (i > 1) {
		/* Hay un registro libre, se lo retorna */
		ret = load_register(i);
	} else {
		/* No hay registros libres, dar de alta una nueva variable en la tabla de simbolos
		 * y liberar un registro. */
		ret = free_any_notA(new_variable());
		state[ret->value] = 0;
	}
	return ret;
}

/* Libera cualquier registro ocupado usando la variable parametro y lo retorna */
Element *free_any_notA(Element *variable) {
	Stack *pila_aux;
	Element *elem_aux = (Element *)malloc(sizeof(Element));
	Element *ret = (Element *)malloc(sizeof(Element));
	
	/* Busco un registro ocupado que no sea ni AH ni AL, para liberarlo */
	spop(&pila, (void *)&elem_aux);
	while (elem_aux->code != REGISTRO || (elem_aux->code == REGISTRO && (elem_aux->value == 0 || elem_aux->value == 1))) {
		spush(&pila_aux, (void *)elem_aux);
		spop(&pila, (void *)&elem_aux);
	}
	/* Vuelco el contenido del registro a liberar en la variable parametro */
	fprintf(output_fd, "\t\tMOV\t_%s, %s\n", variable->name, elem_aux->name);
	/* Apilo la variable en lugar del registro y    *
	 * guardo en la variable el registro a retornar */
	spush(&pila, (void *)variable);
	ret->code = REGISTRO;
	strcpy(ret->name, elem_aux->name);
	ret->value = elem_aux->value;
	/* Apilo nuevamente los demas operandos sacados de la pila */
	spop(&pila_aux, (void *)&elem_aux);
	while (elem_aux != (Element *)0) {
		spush(&pila, (void *)elem_aux);
		spop(&pila_aux, (void *)&elem_aux);
	}
	
	return ret;
}

int is_operand(Element *el) {
	return ((el->code == NUMERO) || (el->code == IDENTIFICADOR) || (el->code == T) || (el->code == F) || (el->code == PASO));
}
