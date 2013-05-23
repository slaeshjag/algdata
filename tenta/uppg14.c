#include <stdlib.h>
#include <stdio.h>


typedef struct LIFO {
	int			element;
	struct LIFO		*next;
} LIFO;


LIFO *lifo_new() {
	return NULL;
}


LIFO *lifo_push(LIFO *l, int element) {
	struct LIFO *n = malloc(sizeof(LIFO));

	n->next = l;
	n->element = element;

	return (void *) n;
}


int lifo_pop(LIFO **l) {
	int ret;

	if (!(*l))
		return -1;
	ret = (*l)->element;
	*l = (*l)->next;

	return ret;
}

