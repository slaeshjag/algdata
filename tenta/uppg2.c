#include <stdio.h>
#include <stdlib.h>


int fibo_iter(int num) {
	int prev = 0, cur = 1, i;

	for (i = 1; i < num; i++) {
		cur = prev + cur;
		prev = cur - prev;
	}

	return cur;
}

int fibo_rec(int prev, int sum, int i, int max) {
	if (i == max)
		return sum;
	return fibo_rec(sum, prev + sum, i + 1, max);
}
	

int main(int argc, char **argv) {
	//fprintf(stderr, "Fibonaccital %i är %i\n", 5, fibo_rec(0, 1, 1, 5));
	fprintf(stderr, "Fibonaccital %i är %i\n", 5, fibo_iter(5));

	return 0;
}
