/******* Test Case *******
 *	 				     *
 *   Errores de Tipos    *
 *				         *
 *************************/

int a = 0;
int b = 2;
/* Error: and entre dos int */
int resp = a && b;
bool x = true;
/* OK */
bool y = a * b == 2;
/* Error: intenta sumar 1 a un bool */
int c = y + 1;
