#include "matrix.h"

extern FILE *source_fd;

/* Nuevos estados */
int nuevo_estado[12][21] =	{
								{1, 2, 3, 4, 5, 6, 7, 8, 9, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, 0, 0, 0, FIN},
								{1, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN},
								{2, 2, FIN, FIN, FIN, FIN, FIN, FIN, FIN,  FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN},
								{FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN},
								{FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN},
								{FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN},
								{FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN},
								{FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN},
								{FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN},
								{FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, 10, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN, FIN},
								{10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
								{10, 10, 10, 10, 10, 10, 10, 10, 0, 11, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}
							};
							
/* Procesos */
int (* procesos[12][21])(char **, int *, char) =	{
	{inic_cte, inic_id, null, null, null, null, null, null, null, multiplicacion, mas, menos, parentesis_a, parentesis_c, llave_a, llave_c, puntoycoma, null, null, null, errror},
	{concat_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte, fin_cte},
	{concat_id, concat_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id, fin_id},
	{asignacion, asignacion, igual, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion, asignacion},
	{mayor, mayor, mayorigual, mayor, mayor, mayor, mayor, mayor, mayor, mayor, mayor, mayor, mayor, mayor, mayor, mayor, mayor, mayor, mayor, mayor, mayor},
	{menor, menor, menorigual, menor, menor, menor, menor, menor, menor, menor, menor, menor, menor, menor, menor, menor, menor, menor, menor, menor, menor},
	{not, not, distinto, not, not, not, not, not, not, not, not, not, not, not, not, not, not, not, not, not, not},
	{errror, errror, errror, errror, errror, errror, and, errror, errror, errror, errror, errror, errror, errror, errror, errror, errror, errror, errror, errror, errror},
	{errror, errror, errror, errror, errror, errror, errror, or, errror, errror, errror, errror, errror, errror, errror, errror, errror, errror, errror, errror, errror},
	{division, division, division, division, division, division, division, division, division, null, division, division, division, division, division, division, division, division, division, division, division},
	{null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null},
	{null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null}
};

int inic_cte(char **output, int *length, char input) {
	*output = malloc(2 * sizeof(char));
	(*output)[0] = input;
	(*output)[1] = '\0';
	*length = 1;
	return 0;
}

int inic_id(char **output, int *length, char input) {
	*output = malloc(2 * sizeof(char));
	(*output)[0] = input;
	(*output)[1] = '\0';
	*length = 1;
	return 0;
}

int concat_cte(char **output, int *length, char input) {
	(*length)++;
	char *tmp = malloc(sizeof(char) * *length);
	strcpy(tmp, *output);
	tmp[(*length)-1] = input;
	tmp[(*length)] = '\0';
	strcpy(*output, tmp);
	free(tmp);
	return 0;
}

int concat_id(char **output, int *length, char input) {
	(*length)++;
	char *tmp = malloc(sizeof(char) * *length);
	strcpy(tmp, *output);
	tmp[(*length)-1] = input;
	tmp[(*length)] = '\0';
	strcpy(*output, tmp);
	free(tmp);
	return 0;
}

int null(char **output, int *length, char input) {
	return 0;
}

int fin_cte(char **output, int *length, char input) {
	ungetc(input, source_fd);
	yylval.entero = atoi(*output);
	return NUMERO;
}

int fin_id(char **output, int *length, char input) {
	ungetc(input, source_fd);
	int retorno = get_tag_name(*output);
	return retorno;
}

int errror(char **output, int *length, char input) {
	printf("Error de Sintaxis. Imposible proseguir.\n\n");
	exit(EXIT_FAILURE);
}

int asignacion(char **output, int *length, char input) {
	ungetc(input, source_fd);
	return '=';
}

int mayor(char **output, int *length, char input) {
	ungetc(input, source_fd);
	return '>';
}

int igual(char **output, int *length, char input) {
	return IGUAL;
}

int menor(char **output, int *length, char input) {
	ungetc(input, source_fd);
	return '<';
}

int menorigual(char **output, int *length, char input) {
	return MENOR_IGUAL;
}

int mayorigual(char **output, int *length, char input) {
	return MAYOR_IGUAL;
}

int not(char **output, int *length, char input) {
	ungetc(input, source_fd);
	return '!';
}

int distinto(char **output, int *length, char input) {
	return DISTINTO;
}

int and(char **output, int *length, char input) {
	return AND;
}

int or(char **output, int *length, char input) {
	return OR;
}

int division(char **output, int *length, char input) {
	ungetc(input, source_fd);
	return '/';
}

int multiplicacion(char **output, int *length, char input) {
	return '*';
}

int mas(char **output, int *length, char input) {
	return '+';
}

int menos(char **output, int *length, char input) {
	return '-';
}

int parentesis_a(char **output, int *length, char input) {
	return '(';
}

int parentesis_c(char **output, int *length, char input) {
	return ')';
}

int llave_a(char **output, int *length, char input) {
	return '{';
}

int llave_c(char **output, int *length, char input) {
	return '}';
}

int puntoycoma(char **output, int *length, char input) {
	return ';';
}

int get_evento (char *output) {
	int car;

	car = fgetc(source_fd);	
	*output = (char)car;
	if (car >= '0' && car <= '9')
		return(0);
	else if ((car <= 'z' && car >= 'a') || (car <= 'Z' && car >= 'A'))
		return(1);
		else {
			switch (car) {
				case '=': return(2);
				case '>': return(3);
				case '<': return(4);
				case '!': return(5);
				case '&': return(6);
				case '|': return(7);
				case '/': return(8);
				case '*': return(9);
				case '+': return(10);
				case '-': return(11);
				case '(': return(12);
				case ')': return(13);
				case '{': return(14);
				case '}': return(15);
				case ';': return(16);
				case '\t': return(17);
				case '\n': return(18);
				case ' ': return(19);
				case EOF: return(-1);
				default: return(20);
			}
		}
}
