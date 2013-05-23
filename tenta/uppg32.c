#include <stdio.h>
#include <stdlib.h>

#include "binary_tree.h"

int btree_find_iter(struct TREE *t, int val) {
	for (;;) {
		if (t->val == val)
			return 1;
		if (t->val < val) {
			if (!t->high)
				return 0;
			t = t->high;
			continue;
		}

		if (!t->low)
			return 0;
		t = t->low;
		continue;
	}

	return 0;
}


int btree_find_rec(struct TREE *t, int val) {
	if (!t)
		return 0;
	
	if (t->val == val)
		return 1;
	if (t->val > val)
		return btree_find_rec(t->low, val);
	return btree_find_rec(t->high, val);
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

	if (btree_find_rec(t, 'O'))
		fprintf(stderr, "Recursive function found O\n");
	else
		fprintf(stderr, "Recursive function didn't find O\n");
	if (btree_find_iter(t, 'R'))
		fprintf(stderr, "Iterative function found R\n");
	else
		fprintf(stderr, "Iterative function didn't find R\n");
	

	return 0;
}

