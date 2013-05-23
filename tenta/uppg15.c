#include <stdlib.h>
#include <stdio.h>

#include "uppg13.h"


int main() {
	LIFO *stack = lifo_new(512);
	char buff[512];
	int i;

	fprintf(stderr, "Ange en textsträng: ");
	fgets(buff, 512, stdin);
	for (i = 0; buff[i]; i++) {
		if (buff[i] == '(')
			lifo_push(stack, 1);
		else if (buff[i] == ')') {
			if (!lifo_size(stack))
				break;
			else
				lifo_pop(stack);
		}
	}

	if (buff[i] || lifo_size(stack))
		fprintf(stderr, "Strängen hade parenteser som inte var i balans\n");
	else
		fprintf(stderr, "Strängens parenteser var i balans\n");
	return 0;
}
