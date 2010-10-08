CC=gcc
CFLAGS=-O3 -lm -Wall
BISON=`which bison`
SRC="src"
INPUT_FILES=$(SRC)/sc.tab.c $(SRC)/matrix.c $(SRC)/reverse_polish.c $(SRC)/stack.c $(SRC)/symbol_table.c $(SRC)/codegen.c $(SRC)/functions.c $(SRC)/variables.c

all: clean bison 
	@$(CC) $(CFLAGS) $(INPUT_FILES) -o sc

clean:
	@rm -f *.o sc $(SRC)/sc.tab.c

bison:
	@$(BISON) $(SRC)/sc.y --output=$(SRC)/sc.tab.c
