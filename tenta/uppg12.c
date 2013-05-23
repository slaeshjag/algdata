#include <stdio.h>
#include <stdlib.h>


int search(int *arr, int find, int min, int max) {
	int mid;

	if (min > max)	/* nu börjar vi gå baklänges, inget passande element fångades i mitten */
		return -1;
	mid = ((max - min) >> 1) + min;

	if (arr[mid] < find) 
		return search(arr, find, mid + 1, max);
	if (arr[mid] > find)
		return search(arr, find, min, mid - 1);

	/* Eftersom mittpunkten är varken mindre eller större så måste det här vara rätt element */
	return mid;
}

int main(int argc, char **argv) {
	int arr[] = { 1, 2, 5, 6, 7, 8, 9, 13, 15, 18, 20, 25, 30 };
	int pos, i;

	for (i = 0; i < 13; i++) {
		if ((pos = search(arr, arr[i], 0, 12)) < 0)
			fprintf(stderr, "Unable to find %i in array\n", arr[i]);
		else
			fprintf(stderr, "Found %i in array at position %i\n", arr[i], pos);
	}
	return 0;
}
