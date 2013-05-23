#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int		*element;
	int		elements;
	int		pos;
} LIFO;


LIFO *lifo_new(int elements_max) {
	LIFO *l = malloc(sizeof(LIFO));
	l->element = malloc(sizeof(int) * elements_max);
	l->elements = elements_max;
	l->pos = 0;

	return l;
}


int lifo_pop(LIFO *l) {
	if (!l->pos)
		return -1;
	return l->element[--l->pos];
}


void lifo_push(LIFO *l, int element) {
	if (l->pos == l->elements)
		return;
	l->element[l->pos++] = element;
	return;
}


int lifo_size(LIFO *l) {
	return l->pos;
}
