/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     BOOL = 9998,
     INT = 9999,
     IDENTIFICADOR = 10000,
     NUMERO = 10001,
     T = 10002,
     F = 10003,
     IF = 10004,
     ELSE = 10005,
     WHILE = 10006,
     AND = 10007,
     OR = 10008,
     MENOR_IGUAL = 10009,
     MAYOR_IGUAL = 10010,
     DISTINTO = 10011,
     IGUAL = 10012
   };
#endif
/* Tokens.  */
#define BOOL 9998
#define INT 9999
#define IDENTIFICADOR 10000
#define NUMERO 10001
#define T 10002
#define F 10003
#define IF 10004
#define ELSE 10005
#define WHILE 10006
#define AND 10007
#define OR 10008
#define MENOR_IGUAL 10009
#define MAYOR_IGUAL 10010
#define DISTINTO 10011
#define IGUAL 10012




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 21 "sc.y"
{
		int entero;
		struct Symbol *simbolo;
	}
/* Line 1489 of yacc.c.  */
#line 88 "sc.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

