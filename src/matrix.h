#ifndef __MATRIX_H__
#define __MATRIX_H__


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "functions.h"
#include "sc.tab.h"

extern FILE *source_fd;
int (* procesos[12][21])(char **, int *, char);
int nuevo_estado[12][21];

//extern YYSTYPE yylval;

/****************************************************
 *              MATRIZ DE ADYACENCIA				*
 ****************************************************/

int inic_cte(char **output, int *length, char input);
int inic_id(char **output, int *length, char input);
int concat_cte(char **output, int *length, char input);
int concat_id(char **output, int *length, char input);
int null(char **output, int *length, char input);
int fin_cte(char **output, int *length, char input);
int fin_id(char **output, int *length, char input);
int errror(char **output, int *length, char input);
int asignacion(char **output, int *length, char input);
int mayor(char **output, int *length, char input);
int igual(char **output, int *length, char input);
int menor(char **output, int *length, char input);
int menorigual(char **output, int *length, char input);
int mayorigual(char **output, int *length, char input);
int not(char **output, int *length, char input);
int distinto(char **output, int *length, char input);
int and(char **output, int *length, char input);
int or(char **output, int *length, char input);
int division(char **output, int *length, char input);
int multiplicacion(char **output, int *length, char input);
int mas(char **output, int *length, char input);
int menos(char **output, int *length, char input);
int parentesis_a(char **output, int *length, char input);
int parentesis_c(char **output, int *length, char input);
int llave_a(char **output, int *length, char input);
int llave_c(char **output, int *length, char input);
int puntoycoma(char **output, int *length, char input);
int get_evento (char *output);


#endif

