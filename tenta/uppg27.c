#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collection.h"

int collection_remove(COLLECTION_DDATA data, int limit, int index) {
	int i;
	struct COLLECTION_DLINK *l, *t;

	if (limit < 00) {
		for (i = 0, l = *data.link; l && i != limit; l = l->next);
		if (i != index)		/* elementet finns ikkje */
			return 0;
		t = l;
		l->prev->next = l->next;
		if (l->next)
			l->next->prev = l->prev;
		free(t);
		return 1;
	}

	if (index >= limit)
		return 0;
	memmove(&(*data.entries)[index], &(*data.entries)[index + 1], limit - index - 1);
	return 1;
}
		
