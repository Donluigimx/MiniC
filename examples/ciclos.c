int main () {
	int g, h;
	
	g = 0;
	while (g < 10) {
		
		g = g + 1;
	}
	
	h = 20;
	
	while (h > 0) {
		g = 0;
		while (g < 100) {
			g = g + 1;
		}
		h = h - 1;
	}
	
	g = 0;
	do {
		h = 6;
		while (g > 0) {
			h = h - 2;
		}
		g = g + 3;
	} while (g < 100);
	
	g = 20;
	while (g >= 10) {
		h = 0;
		do {
			h = h + 20;
		} while (h < 400);
	}
	
	for (g = 0; g < 4;) {
		h = 0;
		while (h < 5) {
			h = h + 1;
		}
	}
}
