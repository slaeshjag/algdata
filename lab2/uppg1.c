#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


typedef struct {
	int		*entry;
	unsigned int	entries;
	unsigned int	write_pos;
} LIFO_QUEUE;


LIFO_QUEUE *lifo_new(int entries) {
	LIFO_QUEUE *lifo;

	if (!(lifo = malloc(sizeof(LIFO_QUEUE))))
		return NULL;
	if (!(lifo->entry = malloc(sizeof(int) * entries))) {
		free(lifo);
		return NULL;
	}

	lifo->entries = entries;
	lifo->write_pos = 0;

	return lifo;
}


int lifo_get_fast(LIFO_QUEUE *lifo) {
	if (!lifo->write_pos)
		return 0;
	lifo->write_pos--;

	return lifo->entry[lifo->write_pos];
}


void lifo_set_fast(LIFO_QUEUE *lifo, int entry) {
	if (!lifo->write_pos + 1 == lifo->entries)
		return;
	lifo->entry[lifo->write_pos] = entry;
	lifo->write_pos++;
	
	return;
}


int lifo_get_slow(LIFO_QUEUE *lifo) {
	int entry;
	
	if (!lifo->write_pos)
		return 0;

	entry = lifo->entry[0];
	lifo->write_pos--;
	memmove(lifo->entry, &lifo->entry[1], sizeof(int) * lifo->write_pos);

	return entry;
}


void lifo_set_slow(LIFO_QUEUE *lifo, int entry) {
	if (!lifo->write_pos + 1 == lifo->entries)
		return;
	memmove(&lifo->entry[1], lifo->entry, sizeof(int) * lifo->write_pos);
	lifo->write_pos++;
	lifo->entry[0] = entry;

	return;
}


int main(int argc, char **argv) {
	int i, iter = atoi(argv[1]);
	LIFO_QUEUE *lifo = lifo_new(iter);

	for (i = 0; i < iter; i++)
		(argc <3 ? lifo_set_fast : lifo_set_slow)(lifo, i);
	for (i = iter - 1; i >= 0; i--)
		if ((argc <3 ? lifo_get_fast : lifo_get_slow)(lifo) != i)
			fprintf(stderr, "LIFO ERROR AT %i\n", i);
	return 0;
}
