#include <stdio.h>
#include <stdlib.h>


int count(int cur, int max) {
	if (cur == max)
		return max;
	return cur + count(cur + 1, max);
}


int main(int argc, char **argv) {
	fprintf(stderr, "Summa: %i\n", count(1, 5));

	return 0;
}
