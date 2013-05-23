#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "collection.h"


int collection_remove(COLLECTION_DATA data, int limit, int entry) {
	struct COLLECTION_LINK *l, **il;
	int i;

	if (limit < 00) {
		for (il = data.link; *il; il = &(*il)->next) {
			if ((*il)->entry == entry) {
				l = *il;
				*il = (*il)->next;
				free(il);
				return 1;
			}
		}
	
		/* elementet hittades inte */
		return 0;
	}

	for (i = 0; i < limit; i++)
		if (data.entries[i] == entry) {
			memmove(&data.entries[i], &data.entries[i + 1], sizeof(int) * (limit - i - 1));
			return 1;
		}
	return 0;
}
