#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>


typedef struct {
	int			num;
	unsigned int		used;
} NUMBER_ENTRY;

typedef struct {
	NUMBER_ENTRY		*entry;
	int			entries;
	int			used;
} LH_QUEUE;


LH_QUEUE *queue_lh_init(int queue_size) {
	LH_QUEUE *lh;
	int i;

	if (!(lh = malloc(sizeof(LH_QUEUE))))
		return NULL;
	if (!(lh->entry = malloc(sizeof(NUMBER_ENTRY) * queue_size))) {
		free(lh);
		return NULL;
	}

	lh->entries = queue_size;
	lh->used = 0;
	
	for (i = 0; i < queue_size; lh->entry[i].used = 0, i++);
	
	return lh;
}


LH_QUEUE *queue_lh_destroy(LH_QUEUE *lh) {
	if (!lh)
		return NULL;
	free(lh->entry);
	free(lh);
	return NULL;
}


void queue_lh_push(LH_QUEUE *lh, int val) {
	int i;

	if (!lh)
		return;
	if (lh->used == lh->entries)
		return;
	for (i = 0; i < lh->entries; i++)
		if (!lh->entry[i].used) {
			lh->entry[i].num = val;
			lh->entry[i].used = 1;
			lh->used++;
			break;
		}
	return;
}


int queue_lh_pop_lowest(LH_QUEUE *lh) {
	int i, tested, index, lowest;

	if (!lh)
		return 0;
	if (!lh->used)
		return 0;

	lowest = INT_MAX;
	index = -1;

	for (i = tested = 0; i < lh->entries && tested < lh->used; i++)
		if (lh->entry[i].used && lh->entry[i].num < lowest) {
			index = i;
			lowest = lh->entry[i].num;
			tested++;
		}
	lh->entry[index].used = 0;
	lh->used--;
	return lh->entry[index].num;
}


int queue_lh_pop_highest(LH_QUEUE *lh) {
	int i, tested, index, highest;

	if (!lh)
		return 0;
	if (!lh->used)
		return 0;
	
	highest = INT_MIN;
	index = -1;
	
	for (i = tested = 0; i < lh->entries && tested < lh->used; i++)
		if (lh->entry[i].used && lh->entry[i].num > highest) {
			index = i;
			highest = lh->entry[i].num;
			tested++;
		}
	lh->entry[index].used = 0;
	lh->used--;
	return lh->entry[index].num;
}


int queue_lh_size(LH_QUEUE *lh) {
	return lh->used;
}


int main(int argc, char **argv) {
	int i;
	LH_QUEUE *lh;

	srand(time(NULL));
	lh = queue_lh_init(20);
	fprintf(stderr, "Adding 0..15 to a LH-queue\n");
	
	for (i = 0; i < 16; i++)
		queue_lh_push(lh, i);
	
	fprintf(stderr, "Poping numbers from LH-queue, alternating between lowest and highest...\n");

	for (i = 0; queue_lh_size(lh); i++)
		fprintf(stderr, "%i ", (!(i & 1) ? queue_lh_pop_lowest : queue_lh_pop_highest)(lh));

	fprintf(stderr, "\n");

	return 0;
}
