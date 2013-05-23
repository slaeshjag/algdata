#include <stdlib.h>
#include <stdio.h>

#include "binary_tree.h"
#pragma warning Iterativa funkar inte korrekt


typedef struct LIFO {
	void			*element;
	struct LIFO		*next;
} LIFO;


LIFO *lifo_new() {
	return NULL;
}


LIFO *lifo_push(LIFO *l, void *element) {
	struct LIFO *n = malloc(sizeof(LIFO));

	n->next = l;
	n->element = element;

	return (void *) n;
}


void *lifo_pop(LIFO **l) {
	void *ret;

	if (!(*l))
		return NULL;
	ret = (*l)->element;
	*l = (*l)->next;

	return ret;
}

int btree_elements_rec(struct TREE *t) {
	int i = 1;

	if (!t)
		return 0;

	i += btree_elements_rec(t->low);
	i += btree_elements_rec(t->high);
	
	return i;
}


int btree_elements_iter(struct TREE *t) {
	int i;
	struct TREE *tmp;
	LIFO *lifo = NULL;

	i = 0;
	tmp = t;

	/* Jätteineffektiv, men jag är för trött för att tänka ut en icke-rekursiv som inte suger */
	do {
		i++;
		for (;tmp;) {
			lifo = lifo_push(lifo, tmp);
			if (tmp->low) {
				tmp = tmp->low;
				continue;
			}

			break;
		}

		i++;
		tmp = lifo_pop(&lifo);
		if (tmp->high) {
			tmp = tmp->high;
			continue;
		}
	
		tmp = NULL;
	} while (lifo);

	return i;
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

	fprintf(stderr, "Iterative counting: %i elements\n", btree_elements_iter(t));
	fprintf(stderr, "Recursive counting: %i elements\n", btree_elements_rec(t));

	return 0;
}
