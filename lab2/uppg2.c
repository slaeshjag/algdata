#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


#define	FIFO_NEXT(val, max)		(((val) + 1 == (max)) ? 0 : (val) + 1)

typedef struct {
	int		*entry;
	int		entries;
	int		write_pos;
	int		read_pos;
} FIFO_QUEUE;


FIFO_QUEUE *fifo_new(int entries) {
	FIFO_QUEUE *fifo;

	if (!(fifo = malloc(sizeof(FIFO_QUEUE))))
		return NULL;
	if (!(fifo->entry = malloc(sizeof(int) * (entries + 1)))) {
		free(fifo);
		return NULL;
	}

	fifo->entries = entries + 1;
	fifo->write_pos = fifo->read_pos = 0;
	return fifo;
}


int fifo_get_fast(FIFO_QUEUE *fifo) {
	int entry;

	if (fifo->write_pos == fifo->read_pos)
		return -1;
	entry = fifo->entry[fifo->read_pos];
	fifo->read_pos = FIFO_NEXT(fifo->read_pos, fifo->entries);
	return entry;
}


void fifo_set_fast(FIFO_QUEUE *fifo, int entry) {
	if (FIFO_NEXT(fifo->write_pos, fifo->entries) == fifo->read_pos)
		return;
	fifo->entry[fifo->write_pos] = entry;
	fifo->write_pos = FIFO_NEXT(fifo->write_pos, fifo->entries);
	return;
}


int fifo_get_slow(FIFO_QUEUE *fifo) {
	int entry;

	if (fifo->write_pos == fifo->read_pos)
		return -1;
	entry = fifo->entry[fifo->write_pos - 1];
	fifo->write_pos--;
	return entry;
}


void fifo_set_slow(FIFO_QUEUE *fifo, int entry) {
	if (FIFO_NEXT(fifo->write_pos, fifo->entries) == fifo->read_pos)
		return;
	memmove(&fifo->entry[1], fifo->entry, sizeof(int) * fifo->write_pos);
	fifo->entry[0] = entry;
	fifo->write_pos = FIFO_NEXT(fifo->write_pos, fifo->entries);
	return;
}


int main(int argc, char **argv) {
	int i, iter = atoi(argv[1]);
	FIFO_QUEUE *fifo = fifo_new(iter);
	
	for (i = 0; i < iter; i++) 
		(argc <3 ? fifo_set_fast : fifo_set_slow)(fifo, i);
	for (i = 0; i < iter; i++)
		if ((argc <3 ? fifo_get_fast : fifo_get_slow)(fifo) != i)
			fprintf(stderr, "FIFO ERROR at %i\n", i);
	return 0;
}
