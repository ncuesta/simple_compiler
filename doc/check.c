#include <stdio.h>
#include "stack.h"

int main (int argc, char *argv[]) {
	int r;
	if (argc == 2) {
		char *fn = argv[1];
		FILE *fd = fopen(fn, "r");
		int curls = 0, parentheses = 0;
		char c;
		
		while ((c = fgetc(fd)) != EOF) {
			switch (c) {
				case '{':
					curls++;
					break;
				case '(':
					parentheses++;
					break;
				case '}':
					curls--;
					break;
				case ')':
					parentheses--;
					break;
				default:
					break;
			}
		}
		
		if (curls == 0) {
			printf("\tCurls OK\n");
		} else {
			if (curls > 0)
				printf("\tMissing Closing Curls. Count is: %d\n", curls);
			else
				printf("\tMissing Opening Curls. Count is: %d\n", curls);
		}	
		if (parentheses == 0) {
			printf("\tParentheses OK\n");
		} else {
			if (parentheses > 0)
				printf("\tMissing Closing Parentheses. Count is: %d\n", parentheses);
			else
				printf("\tMissing Opening Parentheses. Count is: %d\n", parentheses);
		}
			
		fclose(fd);
		r = 0;
		
	} else {
		r = 1;
	}
	return r;
}
