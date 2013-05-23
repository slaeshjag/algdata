#include <stdio.h>
#include <stdlib.h>

#include "mängd.h"

COLLECTION *collection_new(int max_entries) {
	COLLECTION *c;

	c = malloc(sizeof(COLLECTION));
	c->entry = malloc(sizeof(int) * max_entries);
	c->entries = entries;

	c->used = 0;

	return c;
}

