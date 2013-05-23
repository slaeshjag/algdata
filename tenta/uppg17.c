#include <stdio.h>
#include <stdlib.h>

#define	FIFO_NEXT(cur, max)	((cur) + 1 == (max) ? 0 : (cur + 1))

typedef struct {
	int			*element;
	int			elements;
	int			write_pos;
	int			read_pos;
} FIFO;


FIFO *fifo_new(int max_elements) {
	FIFO *f = malloc(sizeof(FIFO));

	f->element = malloc(sizeof(int) * max_elements);
	f->elements = max_elements;
	f->write_pos = f->read_pos = 0;

	return f;
}


void fifo_push(FIFO *f, int element) {
	if (FIFO_NEXT(f->write_pos, f->elements) == f->read_pos)
		return;
	f->element[f->write_pos] = element;
	f->write_pos = FIFO_NEXT(f->write_pos, f->elements);

	return;
}


int fifo_pop(FIFO *f) {
	int i;
	if (f->write_pos == f->read_pos)
		return 0;
	i = f->read_pos;
	f->read_pos = FIFO_NEXT(f->read_pos, f->elements);
	return f->element[i];
}


int fifo_empty(FIFO *f) {
	return f->read_pos == f->write_pos;
}
	
