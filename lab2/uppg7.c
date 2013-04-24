#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define	QUEUE_POS_NEXT(pos, max)	(((pos) + 1 == (max)) ? 0 : (pos) + 1)
#define	QUEUE_POS_BACK(pos, max)	(((pos) - 1 == -1) ? (max) - 1 : (pos) - 1)
#define	MIN(a, b)			((a) > (b) ? (b) : (a))
#define	MAX(a, b)			((a) > (b) ? (a) : (b))

typedef struct {
	unsigned int	read_pos;
	unsigned int	write_pos;
	unsigned int	max_pos;
	unsigned int	count;
	int		*entries;
} FL_QUEUE;

FL_QUEUE *queue_fl_init(unsigned int size) {
	FL_QUEUE *fl;
	
	if (!(fl = malloc(sizeof(FL_QUEUE))))
		return NULL;
	
	if (!(fl->entries = malloc(sizeof(int) * (size + 1)))) {
		free(fl);
		return NULL;
	}

	fl->read_pos = fl->write_pos = 0;
	fl->max_pos = size + 1;
	fl->count = 0;

	return fl;
}


FL_QUEUE *queue_fl_destroy(FL_QUEUE *fl) {
	if (!fl)
		return NULL;
	free(fl->entries);
	free(fl);

	return NULL;
}


inline int queue_fl_full(FL_QUEUE *queue) {
	if (!queue) return 1;
	return QUEUE_POS_NEXT(queue->write_pos, queue->max_pos) == queue->read_pos;
}


inline int queue_fl_empty(FL_QUEUE *queue) {
	if (!queue) return 1;
	return queue->read_pos == queue->write_pos;
}


inline int queue_fl_size(FL_QUEUE *queue) {
	if (queue_fl_empty(queue)) return 0;

	return queue->count;
}


int queue_fl_peek_first(FL_QUEUE *queue) {
	return (queue_fl_empty(queue)) ? 0 : queue->entries[queue->read_pos];
}


int queue_fl_peek_last(FL_QUEUE *queue) {
	return (queue_fl_empty(queue)) ? 0 : queue->entries[QUEUE_POS_BACK(queue->write_pos, queue->max_pos)];
}

int queue_fl_take_first(FL_QUEUE *queue) {
	int res;
	if (queue_fl_empty(queue))
		return 0;
	res = queue_fl_peek_first(queue);
	queue->read_pos = QUEUE_POS_NEXT(queue->read_pos, queue->max_pos);
	queue->count--;

	return res;
}


int queue_fl_take_last(FL_QUEUE *queue) {
	int res;
	if (queue_fl_empty(queue))
		return 0;
	res = queue_fl_peek_last(queue);
	queue->write_pos = QUEUE_POS_BACK(queue->write_pos, queue->max_pos);
	queue->count--;

	return res;
}


void queue_fl_put(FL_QUEUE *queue, int entry) {
	if (queue_fl_full(queue))
		/* Overflow! */
		return;
	queue->entries[queue->write_pos] = entry;
	queue->write_pos = QUEUE_POS_NEXT(queue->write_pos, queue->max_pos);
	queue->count++;
	return;
}



int main(int argc, char **argv) {
	int i, last, first;
	FL_QUEUE *queue;

	if (!(queue = queue_fl_init(2048))) {
		fprintf(stderr, "ERROR: Queue could not be allocated\n");
		return -1;
	}

	srand(time(NULL));

	fprintf(stderr, "Filling queue with random numers in range 0-1023\n");
	for (i = 0; i < 2047; i++)
		queue_fl_put(queue, rand() & 0x3FF);
	
	i = queue_fl_size(queue);
	fprintf(stderr, "Popping one entry at a time from queue, picking first one if it's bigger than the last one\n");

	for (last = first = 0; !queue_fl_empty(queue); ) {
		if (queue_fl_peek_first(queue) > queue_fl_peek_last(queue)) {
			queue_fl_take_first(queue);
			first++;
		} else {
			queue_fl_take_last(queue);
			last++;
		}
	}

	fprintf(stderr, "Picked first entry %i out of %i times, %i the rest of the times\n", first, i, last);
	
	return 0;
}
