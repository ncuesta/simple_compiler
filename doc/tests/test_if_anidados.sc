/**** Test Case ****
 *				   *
 *   If anidados   *
 *				   *
 *******************/
 
int a = 2;
int resp;
bool c = true;
if (a == 2) {
	bool d = false;

	if (c && d) {
		a = a * 2;
	} else {
		a = a * 10;
		d = true;
	}
	
	if (d) {
		resp = 0;
	} else {
		resp = 1;
	}
	
} else {
	resp = 2;
}
