int bbb;

void funcion_v_v () {
	int x;

	x = 10;
}

void funcion_v_i (int a) {
	int y;

	y = 20;
}

void funcion_v_ii (int a, int b) {
	int z;

	z = 30;
}

void funcion_v_iii (int a, int b, int c) {
	int w;

	w = 40;
}

int funcion_i_v () {
	int c;

	c = 50;
}

int funcion_i_void () {
	int d;

	return d;
	d = 60;
}

int funcion_i_i (int x) {
	int e;

	e = 70 + 322 - 123123 * 332;
}

int funcion_i_ii (int x, int y);

int main() {
	int a;
	a = 2312;
	bbb = a;
	funcion_i_ii(a,a);
}

int funcion_i_ii (int x, int y) {
	int f;

	f = 80;
}
