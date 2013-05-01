#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define	NUMBERS		1000


typedef struct {
	unsigned int		*factor;
	unsigned int		factors;
} NUMBER;


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
	int i, j;
	NUMBER *number = malloc(sizeof(NUMBER) * NUMBERS);
	FILE *fp;
	
	/* Hitta faktorer för alla heltal */
	fprintf(stderr, "Calculating primes...\n");
	for (i = 1; i <= NUMBERS; i++)
		number[i-1].factor = find_factors(&number[i-1].factors, i);
	
	/* Spara alla icke-primtal till fil */
	fprintf(stderr, "Saving non-primes...\n");

	fp = fopen("non-primes.txt", "w");
	for (i = 0; i < NUMBERS; i++) {
		if (number[i].factors == 1)
			continue;
		fprintf(fp, "%i: ", i + 1);
		for (j = 0; j < number[i].factors - 1; j++)
			fprintf(fp, "%i * ", number[i].factor[j]);
		fprintf(fp, "%i\n", number[i].factor[j]);
	}

	fclose(fp);

	/* Spara alla primtal till en annan fil */
	fprintf(stderr, "Saving primes...\n");
	
	fp = fopen("primes.txt", "w");
	for (i = 0; i < NUMBERS; i++) {
		if (number[i].factors > 1)
			continue;
		fprintf(fp, "%i\n", i + 1);
	}

	fclose(fp);

	return 0;
}
