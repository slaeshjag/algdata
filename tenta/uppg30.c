#include <stdio.h>
#include <stdlib.h>

#include "binary_tree.h"

void btree_print(struct TREE *t) {
	if (!t)
		return;
	if (t->low)
		btree_print(t->low);
	fprintf(stderr, "%c ", t->val);
	if (t->high)
		btree_print(t->high);
	return;	
}


int main(int argc, char **argv) {
	struct TREE *t;

	t = NULL;
	btree_add_rec(&t, 'M');
	btree_add_rec(&t, 'P');
	btree_add_rec(&t, 'G');
	btree_add_rec(&t, 'U');
	btree_add_rec(&t, 'J');
	btree_add_rec(&t, 'O');
	btree_add_rec(&t, 'R');
	btree_add_rec(&t, 'V');

	btree_print(t);
	fprintf(stderr, "\n");

	return 0;
}
