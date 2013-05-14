/* uppg4.c - Steven Arnow <s@rdw.se>,  2013 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "graph.h"

#define	NODES_MAX		64
char **names;


int determine_links(GRAPH *g, int n) {
	int j, l;

	if (graph_get_visit(g, n))
		return 0;
	graph_set_visit(g, n);
	l = 1;
	
	for (j = 0; j < graph_neighbours(g, n); j++)
		l += determine_links(g, graph_neighbour(g, n, j));
	return l;
}
			


int main(int argc, char **argv) {
	GRAPH *g = graph_new(NODES_MAX);
	int i;
	names = calloc(NODES_MAX, sizeof(char **));

	srand(time(NULL));

	for (i = 0; i < NODES_MAX; i++)
		graph_add_node(g);
	for (i = 0; i < NODES_MAX; i++) {
		graph_add_link(g, i, i / 4, 1, 1);
	}
		

	fprintf(stderr, "Found %i nodes\n", determine_links(g, 0));

	return 0;
}
