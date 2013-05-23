#include <stdio.h>
#include <stdlib.h>

typedef struct FIFO_ELEMENT {
	int			element;
	struct FIFO_ELEMENT	*next;
} FIFO_ELEMENT;


typedef struct {
	struct FIFO_ELEMENT	*fifo;
} FIFO;


FIFO *fifo_new() {
	FIFO *f = malloc(sizeof(FIFO));

	f->fifo = NULL;

	return f;
}


int fifo_pop(FIFO *f) {
	int i;
	struct FIFO_ELEMENT *ft;

	if (!f->fifo)
		return -1;
	i = f->fifo->element;
	ft = f->fifo;
	f->fifo = f->fifo->next;
	free(ft);

	return i;
}


void fifo_push(FIFO *f, int element) {
	struct FIFO_ELEMENT *n;

	n = malloc(sizeof(struct FIFO_ELEMENT));
	n->next = f->fifo;
	n->element = element;
	f->fifo = n;

	return;
}


int fifo_empty(FIFO *f) {
	return !f->fifo;
}
	
