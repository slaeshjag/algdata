#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

typedef struct DOCUMENT {
	int		pages;
	int		id;
	int		weight;
	struct DOCUMENT	*next;
} DOCUMENT;


DOCUMENT *document_new(DOCUMENT *next) {
	static int id = 0;
	DOCUMENT *doc = malloc(sizeof(DOCUMENT));

	doc->pages = rand() % 0xFFF;
	doc->id = id++;
	doc->weight = doc->pages;
	doc->next = next;

	fprintf(stderr, "New job #%i with %i pages\n", doc->id, doc->pages);

	return doc;
}


void document_pick(DOCUMENT **doc) {
	DOCUMENT **ptr = NULL, *use;
	int weight = INT_MAX;

	while (*doc) {
		if ((*doc)->weight < weight) {
			ptr = doc;
			weight = (*doc)->weight;
		}

		(*doc)->weight -= 100;
		doc = &((*doc)->next);
	}

	if (!ptr)
		return;
	use = *ptr;
	*ptr = (void *) ((*ptr)->next);

	fprintf(stderr, "Printing job #%i with %i pages (weight: %i)\n", use->id, use->pages, use->weight);
	free(use);

	return;
}


int main(int argc, char **argv) {
	int i;

	DOCUMENT *doc = NULL;
	srand(time(NULL));
	
	for (i = 0; i < 5; i++)
		doc = document_new(doc);

	for (;;) {
		doc = document_new(doc);
		sleep(2);
		document_pick(&doc);
		sleep(2);
		if ((rand() % 6) < 2) {
			fprintf(stderr, "\x1B[31mOUT OF TONER! Press enter to replace\x1B[0m\n");
			getchar();
			fprintf(stderr, "Printing resumed...\n");
		}
	}

	return 0;
}
