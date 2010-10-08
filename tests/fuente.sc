/* enteros */
int b = 2;
int c = 3;
int a = b + c;
int d = a + b;
int e = b + c + d + a * 2 * c * d / b;
a = b / 1 + 2 / 2 * 1 + 3;
d = d * c * c / a + b * c;

/* booleanos */
bool x = true;
bool y;
y = x && true;
bool z = x || false;
bool w = 2 == 2;
bool booleanito = (2 > 3) && ((2 <= 2) || (3 > 2)) == ((2 == 2) || (43 < (2 - 3)));

/* if simple */
int a = 24;
if (a == 24) {
	a = 6;
}

/* if con else */
int c;
if (!false) {
	c = 0;
} else {
	c = 23;
}

/* while */
int a = 0;
while (a < 10) {
	a = a + 1;
}

