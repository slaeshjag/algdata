#include <stdio.h>
#include <stdlib.h>

#include "collection.h"

int collection_entry(COLLECTION_DATA data, int limit, int n) {
	int i;
	struct COLLECTION_LINK *l;

	if (limit < 00) {
		for (l = *data.link, i = 0; l; l = l->next, i++)
			if (i == n)
				return l->entry;
		return -1;
	}

	if (n >= limit)
		return -1;
	return data.entries[n];
}


int main(int argc, char **argv) {
	COLLECTION_DATA d;
	int maengd[] = { 3, 56, 234, 12, 6564, 32423, 7567, 2323, 77, 04343, 0xFFD, 555 };
	int i;

	d.entries = maengd;
	for (i = 0; collection_entry(d, 12, i) != -1; i++)
		fprintf(stderr, "element %i: %i\n", i, collection_entry(d, 12, i));
	
	return 0;
}
