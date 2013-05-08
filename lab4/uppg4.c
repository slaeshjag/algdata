/* uppg4.c - Steven Arnow <s@rdw.se>,  2013 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define	MAX_NUMBER		10000

typedef struct {
	unsigned int	*factor;
	int		factors;
} FACTOR;

typedef struct {
	FACTOR		*factor;
	int		factors;
} FACTOR_LIST;

void *memdup(void *ptr, size_t size) {
	void *dst;

	if (!(dst = malloc(size)))
		return dst;
	memcpy(dst, ptr, size);
	return dst;
}


unsigned int *find_factors(unsigned int *num, int val) {
	int root, x;
	unsigned int *factor = NULL, factors = 0;
	root = sqrtf(val);

	for (x = 2; x <= root; x++) {
		if (!(val % x)) {
			factors++;
			factor = realloc(factor, sizeof(int) * factors);
			factor[factors-1] = x;
			val /= x;
			x = 1;
		}
	}

	factors++;
	factor = realloc(factor, sizeof(int) * factors);
	factor[factors-1] = val;
	
	*num = factors;
	return factor;
}

int main(int argc, char **argv) {
	FACTOR_LIST l;
	int h, i, j, k, *fac2, x, y;

	l.factor = calloc(MAX_NUMBER, sizeof(FACTOR));
	l.factors = MAX_NUMBER;

	for (i = 0; i < MAX_NUMBER; i++)
		l.factor[i].factor = find_factors(&l.factor[i].factors, i + 1);

	for (;;) {
		fprintf(stderr, "Ange täljare: ");
		fscanf(stdin, "%i", &j);
		fprintf(stderr, "Ange nämnare: ");
		fscanf(stdin, "%i", &k);
		fac2 = memdup(l.factor[k - 1].factor, l.factor[k - 1].factors * sizeof(int));
		fprintf(stderr, "Result: %i / %i = ", j, k);
		for (h = 0, x = 1; h < l.factor[j - 1].factors; h++) {
			for (i = 0; i < l.factor[k - 1].factors; i++)
				if (fac2[i] == l.factor[j - 1].factor[h]) {
					fac2[i] = 1;
					break;
				}
			if (i == l.factor[k - 1].factors)
				x *= l.factor[j - 1].factor[h];
		}

		for (h = 0, y = 1; h < l.factor[k - 1].factors; h++)
			y *= fac2[h];
		fprintf(stderr, "%i / %i\n", x, y);
		free(fac2);
	}


	return 0;
}
