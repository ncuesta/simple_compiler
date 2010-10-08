#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__


#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "symbol_table.h"
#include "matrix.h"
#include "sc.tab.h"

extern FILE *source_fd;

int get_tag_name(char *string);
int yylex();
void yyerror(char *string);


#endif
