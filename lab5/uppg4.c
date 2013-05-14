/* uppg4.c - Steven Arnow <s@rdw.se>,  2013 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "graph.h"

#define	NODES_MAX		64
char **names;

int path_find_do(GRAPH *g, int n1, int n2) {
	int i;

	if (graph_get_visit(g, n1))
		return 0;
	graph_set_visit(g, n1);
	if (n1 == n2) {
		fprintf(stderr, "%s ", names[n1]);
		return 1;
	}

	for (i = 0; i < graph_neighbours(g, n1); i++) {
		if (path_find_do(g, graph_neighbour(g, n1, i), n2)) {
			fprintf(stderr, "<- %s", names[n1]);
			return 1;
		}
	}
	
	return 0;
}


int find_path(GRAPH *g, int n1, int n2) {
	fprintf(stderr, "Finding path between %s and %s\n", names[n1], names[n2]);

	if (!path_find_do(g, n1, n2))
		fprintf(stderr, "No path found");
	fprintf(stderr, "\n");

	return 0;
}


int main(int argc, char **argv) {
	GRAPH *g = graph_new(8);
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
		graph_add_link(g, i, j, 1, 1);
		fprintf(stderr, "Linked %s <-> %s\n", names[i], names[j]);
	}

	fclose(fp);

	find_path(g, 0, 4);
		

	return 0;
}
