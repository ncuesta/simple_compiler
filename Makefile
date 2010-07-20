CC=gcc
CFLAGS=-O3 -lm -Wall
BISON=`which bison`
INPUT_FILES=sc.tab.c matrix.c reverse_polish.c stack.c symbol_table.c codegen.c functions.c variables.c

all: clean bison 
	@$(CC) $(CFLAGS) $(INPUT_FILES) -o sc

clean:
	@rm -f *.o sc sc.tab.c

bison:
	@$(BISON) sc.y