/* uppg4.c - Steven Arnow <s@rdw.se>,  2013 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "graph.h"

#define	NODES_MAX		64
char **names;


int print_links(GRAPH *g) {
	int i, j;

	for (i = 0; i < NODES_MAX; i++)
		for (j = 0; j < graph_neighbours(g, i); j++)
			fprintf(stderr, "%s -> %s\n", names[i], names[graph_neighbour(g, i, j)]);
	return 0;
}
			


int main(int argc, char **argv) {
	GRAPH *g = graph_new(NODES_MAX);
	int i, j;
	char buff[512];
	names = calloc(NODES_MAX, sizeof(char **));
	FILE *fp;

	fp = fopen("nodes.txt", "r");
	for (i = 0; !feof(fp); i++) {
		fscanf(fp, "%s\n", buff);
		graph_add_node(g);
		names[i] = strdup(buff);
	}

	fclose(fp);
	fp = fopen("links.txt", "r");
	while (!feof(fp)) {
		fscanf(fp, "%i %i\n", &i, &j);
		graph_add_link(g, i, j, 1, 0);
		fprintf(stderr, "Linked %s <-> %s\n", names[i], names[j]);
	}

	fclose(fp);

	print_links(g);

	return 0;
}
