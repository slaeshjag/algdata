#include <stdio.h>
#include <stdlib.h>


void sort(int *input, int elements) {
	int i, j, tmp;

	for (i = 0; i < elements; i++)
		for (j = i; j > 0 && input[j] < input[j-1]; j--) {
			tmp = input[j];
			input[j] = input[j - 1];
			input[j - 1] = tmp;
		}
	return;
}


int main(int argc, char **argv) {
	int arr[] = { 453534, 45, 763, 3424, 342, 7, 234, 231, 676, 2341, 45, -1, 54, 0 };
	int i;

	fprintf(stderr, "Before sorting: ");
	for (i = 0; i < 14; i++)
		fprintf(stderr, "%i, ", arr[i]);

	sort(arr, 14);
	fprintf(stderr, "\nAfter sorting: ");
	for (i = 0; i < 14; i++)
		fprintf(stderr, "%i, ", arr[i]);
	fprintf(stderr, "\n");

	return 0;
}
