#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collection.h"

void collection_insert(COLLECTION_DDATA data, int limit, int entry, int pos) {
	int i;
	struct COLLECTION_DLINK *l, *n;

	if (limit < 00) {
		n = malloc(sizeof(struct COLLECTION_DLINK));
		n->entry = entry;
		
		for (i = 0, l = *data.link; l; l = l->next) 
			if (i == pos) {
				n->prev = l->prev;
				n->next = l;
				l->prev->next = n;
				l->prev = n;
				return;
			}
		n->prev = l;
		if (!l)
			*data.link = n;
		else
			l->next = n;
		n->next = NULL;
		return;
	}

	if (pos >= limit)
		pos = limit;
	*data.entries = realloc(data.entries, sizeof(int) * (limit + 1));
	memmove(&(*data.entries)[pos + 1], &(*data.entries)[pos], sizeof(int) * pos - limit);
	(*data.entries)[pos] = entry;
	return;
}
	
