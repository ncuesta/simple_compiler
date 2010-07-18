	%{
		#include <stdlib.h>
		#include <stdio.h>
		#include "constants.h"
		#include "symbol_table.h"
		#include "variables.h"
		#include "functions.h"
		#include "codegen.h"
		#include "matrix.h"

		FILE *source_fd, *output_fd;
		int paso = 0;
		Element *element_stack = (Element *)0;
		Stack *astack = (Stack *)0;
		
		int error = FALSE;
	%}

	%defines
	
	%union {
		int entero;
		struct Symbol *simbolo;
	}
	%start programa
	%token	<entero>	BOOL			9998
	%token	<entero>	INT				9999
	%token	<simbolo>	IDENTIFICADOR	10000
	%token	<entero>	NUMERO			10001
	%token	<entero>	T				10002
	%token	<entero>	F				10003
	%token	<entero>	IF				10004
	%token	<entero>	ELSE			10005
	%token	<entero>	WHILE			10006
	%token	<entero>	AND				10007
	%token	<entero>	OR				10008
	%token	<entero>	MENOR_IGUAL		10009
	%token	<entero>	MAYOR_IGUAL		10010
	%token	<entero>	DISTINTO		10011
	%token	<entero>	IGUAL			10012
	%left '-' '+' '*' '/' AND IF /* decision? */
	%right '=' OR
	%nonassoc '>' '<' IGUAL MENOR_IGUAL MAYOR_IGUAL DISTINTO
	%type <simbolo> termino expresion comparacion factor
	
%%

	programa	:	bloque
				;
	
	bloque		:	declaracion
				|	asignacion
				|	decision
				|	iteracion
				|	declaracion bloque
				|	asignacion bloque
				|	decision bloque
				|	iteracion bloque
				|	error ';'
						{	error = TRUE;	}
				|	error ';' bloque
						{	error = TRUE;	}
				;
	
	declaracion	:	INT IDENTIFICADOR ';'
						{	check_declare($2, INT);	}
				|	INT IDENTIFICADOR '=' expresion ';'
						{	check_declare_assign($2, INT, $4, &element_stack);	}
				|	BOOL IDENTIFICADOR ';'
						{	check_declare($2, BOOL);	}
				|	BOOL IDENTIFICADOR '=' expresion ';'
						{	check_declare_assign($2, BOOL, $4, &element_stack);	}
				;
	
	asignacion	:	IDENTIFICADOR '=' expresion ';'
						{	check_assign($1, $3, &element_stack);	}
				;
	
	expresion	:	termino
						{	$$ = $1;	}
				|	expresion '-' termino
						{	$$ = check_substract($1, $3, &element_stack);	}
				|	expresion '+' termino
						{	$$ = check_add($1, $3, &element_stack);	}
				|	expresion OR termino
						{	$$ = check_or($1, $3, &element_stack);	}
				|	comparacion
						{	$$ = $1;	}
				;
						
	termino		:	factor
						{	$$ = $1;	}
				|	termino '*' factor
						{	$$ = check_multiply($1, $3, &element_stack);	}
				|	termino '/' factor
						{	$$ = check_divide($1, $3, &element_stack);	}
				|	termino AND factor
						{	$$ = check_and($1, $3, &element_stack);	}
				|	'!' factor
						{	$$ = check_not($2, &element_stack);	}
				;
	
	factor		:	IDENTIFICADOR
						{	element_stack = push(IDENTIFICADOR, (int)NULL, $1->name);
							$$ = $1;	}
				|	NUMERO
						{	element_stack = push(NUMERO, $1, (char *)0);
							$$ = (Symbol *)malloc(sizeof(Symbol));
							$$->type = INT;
							$$->value = yylval.entero;	}
				|	T
						{	element_stack = push(T, TRUE, (char *)0);
							$$ = (Symbol *)malloc(sizeof(Symbol));
							$$->type = BOOL;
							$$->value = TRUE;	}
				|	F
						{	element_stack = push(F, FALSE, (char *)0);
							$$ = (Symbol *)malloc(sizeof(Symbol));
							$$->type = BOOL;
							$$->value = FALSE;	}
				|	'(' expresion ')'
						{	$$ = $2;	}
				;
	
	comparacion	:	expresion '>' expresion
						{	$$ = check_compare($1, $3, '>', &element_stack);	}
				|	expresion '<' expresion
						{	$$ = check_compare($1, $3, '<', &element_stack);	}
				|	expresion MAYOR_IGUAL expresion
						{	$$ = check_compare($1, $3, MAYOR_IGUAL, &element_stack);	}
				|	expresion MENOR_IGUAL expresion
						{	$$ = check_compare($1, $3, MENOR_IGUAL, &element_stack);	}
				|	expresion IGUAL expresion
						{	$$ = check_equality($1, $3, &element_stack);	}
				|	expresion DISTINTO expresion
						{	$$ = check_inequality($1, $3, &element_stack);	}
				;
				
	subrout_if	:	/* vacio */
						{	insert_BF(&astack, &element_stack);	}
				;
				
	decision	:	IF '(' expresion ')' subrout_if '{' bloque '}'
						{	check_simple_if($3, &astack, &element_stack, paso);	}
				|	IF '(' expresion ')' subrout_if '{' bloque '}'
						{	check_complex_if($3, &astack, &element_stack, paso);	}
					ELSE '{' bloque '}'
						{	check_else(&astack, &element_stack, paso);	}
				;

	iteracion	:	WHILE 
						{	subrout_while(&astack, &element_stack, paso);	}
					'(' expresion ')'
						{	check_while($4, &astack, &element_stack, paso);	}
					'{' bloque '}'
						{	check_while_block(&astack, &element_stack, paso);	}
				;
%%

	int main(int argc, char *argv[]) {
		char *outfn;

		switch (argc) {
			case 2:
				outfn = "default.asm";
				break;
			case 3:
				outfn = malloc(sizeof(char)*24);
				strcpy(outfn, argv[2]);
				break;
			default:
				printf("sc: Parametros incorrectos.\nUso correcto:\n\tsc FUENTE [OBJETO]\n\nFUENTE es el archivo con el codigo fuente a compilar, y OBJETO es el nombre del archivo objeto al que se desea hacer la salida objeto (i.e. salida.asm).\nSi no se provee un archivo OBJETO, la salida se realizara por defecto al archivo 'default.asm' del directorio actual.\n\n");
				exit(1);
		}
		if ((source_fd = fopen(argv[1], "r")) == NULL) {
			printf("sc: No se pudo abrir el archivo fuente '%s'. Imposible continuar.\n", argv[1]);
			exit(1);
		}

		switch (yyparse()) {
			case 0:
				if (!error) {
					if ((output_fd = fopen(outfn, "w")) == NULL) {
						printf("sc: No se pudo abrir el archivo objeto '%s'. Imposible continuar.\n", outfn);
						exit(1);
					}
					printf("\nsc: Compilacion satisfactoria.\nProcediendo a la generacion del codigo objeto...\n\n");
					reverse();		//Hace falta dar vuelta la polaca para que sea interpretable.
					show_stack();
					generate();
					fclose(output_fd);
					fclose(source_fd);
					return 0;
				}
			case 1:
				printf("\nsc: No se puede generar la salida objeto. Chequee su codigo fuente.\n\n");
				fclose(source_fd);
				return 1;
			case 2:
				printf("\nsc: Se agoto la memoria. Imposible proseguir.\n\n");
				fclose(source_fd);
				return 2;
			default:
				fclose(source_fd);
				return 0;
		}
	}
