#include "functions.h"

int get_tag_name(char *string) {
	if (strcmp(string, "int") == 0) {
		return INT;
	} else {
		if (strcmp(string, "bool") == 0) {
			return BOOL;
		} else {
			if (strcmp(string, "true") == 0) {
				return T;
			} else {
				if (strcmp(string, "false") == 0) {
					return F;
				} else {
					if (strcmp(string, "if") == 0) {
						return IF;
					} else {
						if (strcmp(string, "else") == 0) {
							return ELSE;
						} else {
							if (strcmp(string, "while") == 0) {
								return WHILE;
							} else {
								yylval.simbolo = symbol_from_string(string);
								return IDENTIFICADOR;
							}
						}
					}
				}
			}
		}
	}
}

int yylex() {
	int estado = 0, evento, retorno = 0, length;
	char *string;
	char caracter;
	while (estado != FIN) {
		evento = get_evento(&caracter);
		if (evento == -1) {
			return 0;
		}
		retorno = procesos[estado][evento](&string, &length, caracter);
		estado = nuevo_estado[estado][evento];
	}
	return retorno;
}

void yyerror(char *string)	{
	fprintf(stderr, "sc: %s.\n", string);
}
