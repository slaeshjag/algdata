#include <stdio.h>
#include <stdlib.h>
#include "collection.h"

int collection_number_exist(COLLECTION_DATA data, int limit, int find) {
	struct COLLECTION_LINK *l;
	int i;

	if (limit < 00)	{	/* Mängden är implementerad som en länkad lista */
		for (l = *data.link; l; l = l->next)
			if (l->entry == find)
				return 1;
		return 0;
	}

	for (i = 0; i < limit; i++) 
		if (data.entries[i] == find)
			return 1;
	return 0;
}
