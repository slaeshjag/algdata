#include <stdio.h>
#include <stdlib.h>

#include "binary_tree.h"


void btree_add_rec_do(struct TREE *t, struct TREE *n) {
	if (t->val < n->val) {
		if (t->high)
			btree_add_rec_do(t->high, n);
		else {
			t->high = n;
			return;
		}
	} else {
		if (t->low)
			btree_add_rec_do(t->low, n);
		else {
			t->low = n;
			return;
		}
	}

	return;
}

void btree_add_rec(struct TREE **t, int val) {
	struct TREE *n = malloc(sizeof(TREE));
	
	n->val = val;
	n->low = NULL;
	n->high = NULL;

	if (!(*t)) {
		*t = n;
		return;
	}

	btree_add_rec_do(*t, n);

	return;
}


void btree_add_iter(struct TREE **t, int val) {
	struct TREE *n = malloc(sizeof(TREE));
	struct TREE *next;

	n->val = val;
	n->low = NULL;
	n->high = NULL;

	if (!(*t)) {
		*t = n;
		return;
	}
	
	for (next = *t;;) {
		if (next->val < n->val) {
			if (next->high) {
				next = next->high;
				continue;
			} else {
				next->high = n;
				return;
			}
		} else {
			if (next->low) {
				next = next->low;
				continue;
			} else {
				next->low = n;
				return;
			}
		}
	}

	return;
}
