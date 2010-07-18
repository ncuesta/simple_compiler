/******* Test Case *******
 *	 				     *
 *  While & if anidados  *
 *				         *
 *************************/

int a = 0;
int b;
int resp = 0;
while (a < 9) {	
	b = 0;
	while (b <= 3) {
		if (!(b / 2 == 0)) {
			resp = resp + 1;
		}
		b = b + 1;
	}
	if (a < 5) {
		a = a + 2;
	} else {
		a = a + 1;
	}
}
