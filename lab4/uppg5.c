/* 'uppg5.c - Steven ARnow <s@rdw.se>, 2013 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define	DEF_FIND		10000
int search_levels = 0;
int print_found = 0;


typedef struct SEARCH_TREE {
	struct SEARCH_TREE		*prev;
	struct SEARCH_TREE		*next;
	int				key;
	void				*val;
} SEARCH_TREE;


typedef struct {
	unsigned int			*factor;
	int				factors;
} FACTOR;


void *memdup(void *ptr, size_t size) {
	void *dst;

	if (!(dst = malloc(size)))
		return dst;
	memcpy(dst, ptr, size);
	return dst;
}


SEARCH_TREE *tree_add(struct SEARCH_TREE *tree, void *val, int key) {
	struct SEARCH_TREE *new, *next;

	new = malloc(sizeof(struct SEARCH_TREE));
	new->prev = NULL;
	new->next = NULL;
	new->key = key;
	new->val = val;

	if (!tree)
		return new;
	for (next = tree;;) {
		if (next->key > key) {
			if (next->prev) {
				next = next->prev;
				continue;
			} else {
				next->prev = new;
				return tree;
			}
		} else {
			if (next->next) {
				next = next->next;
				continue;
			} else {
				next->next = new;
				return tree;
			}
		}
	}

	return NULL;
}


void *tree_search(struct SEARCH_TREE *tree, int key) {
	search_levels = 0;

	while (tree) {
		if (tree->key == key) {
			if (print_found)
				fprintf(stderr, "Found in tree! (At %i levels down)\n", search_levels);
			return tree->val;
		} if (tree->key < key)
			tree = tree->next;
		else
			tree = tree->prev;
		search_levels++;
	}

	return NULL;
}


unsigned int *find_factors(unsigned int *num, int val) {
	int root, x;
	unsigned int *factor = NULL, factors = 0;
	root = sqrtf(val);

	for (x = 2; x <= root; x++) {
		if (!(val % x)) {
			factors++;
			factor = realloc(factor, sizeof(int) * factors);
			factor[factors-1] = x;
			val /= x;
			x = 1;
		}
	}

	factors++;
	factor = realloc(factor, sizeof(int) * factors);
	factor[factors-1] = val;
	
	*num = factors;
	return factor;
}


FACTOR *get_factors(struct SEARCH_TREE **tree, int num) {
	FACTOR *factor;
	if (!(factor = tree_search(*tree, num))) {
		factor = malloc(sizeof(FACTOR));
		factor->factor = find_factors(&factor->factors, num);
		*tree = tree_add(*tree, factor, num);

	}

	return factor;
}


int main(int argc, char **argv) {
	struct SEARCH_TREE *tree;
	int i, t, n, h, j, k, x, y, *mod;
	FACTOR *fac1, *fac2;

	srand(time(NULL));
	tree = NULL;
	
	fprintf(stderr, "Faktoriserar %i slumptal... ", DEF_FIND);
	for (i = 0; i < DEF_FIND; i++)
		get_factors(&tree, rand());
	
	print_found = 1;
	fprintf(stderr, "klar\n");
	
	for (;;) {
		fprintf(stderr, "Ange täljare: ");
		fscanf(stdin, "%i", &t);
		fprintf(stderr, "Ange nämnare: ");
		fscanf(stdin, "%i", &n);

		fac1 = get_factors(&tree, t);
		fac2 = get_factors(&tree, n);
		mod = memdup(fac2->factor, fac2->factors * sizeof(int));
		
		for (h = 0, x = 1; h < fac1->factors; h++) {
			for (i = 0; i < fac2->factors; i++)
				if (fac1->factor[h] == mod[i]) {
					mod[i] = 1;
					break;
				}
			if (i == fac2->factors)
				x *= fac1->factor[h];
		}

		for (h = 0, y = 1; h < fac2->factors; h++)
			y *= mod[h];
		fprintf(stderr, "%i / %i = %i / %i\n", t, n, x, y);
	}

	return 0;
}
