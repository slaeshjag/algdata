#include "graph.h"

#include <stdlib.h>
#include <string.h>

GRAPH *graph_new(int nodes_max) {
	GRAPH *g;

	g = calloc(sizeof(GRAPH), 1);
	g->links = calloc(nodes_max * nodes_max, sizeof(int));
	g->usage = calloc(nodes_max, sizeof(int));
	g->visit = calloc(nodes_max, sizeof(int));
	g->nodes_max = nodes_max;
	g->nodes_max2 = nodes_max * nodes_max;

	return g;
}


GRAPH *graph_free(GRAPH *g) {
	free(g->links);
	free(g->usage);
	free(g->visit);
	free(g);

	return NULL;
}


int graph_find_link(GRAPH *g, int n, int l) {
	int i;

	for (i = n; i < g->nodes_max2; i += g->nodes_max)
		if (g->links[i] && i / g->nodes_max == l)
			return 1;
	return 0;
}


void graph_add_link(GRAPH *g, int l1, int l2, int weight, int dl) {
	if (!graph_find_link(g, l1, l2)) {
		g->links[l2 * g->nodes_max + l1] = weight;
		g->usage[l1]++;
	}

	if (dl && !graph_find_link(g, l2, l1)) {
		g->links[l1 * g->nodes_max + l2] = weight;
		g->usage[l2]++;
	}

	return;
}
	


void graph_remove_link(GRAPH *g, int l1, int l2) {
	if (g->links[l2 * g->nodes_max + l1] > 0)
		g->usage[l1]--;
	if (g->links[l1 * g->nodes_max + l2] > 0)
		g->usage[l2]--;

	g->links[l1 * g->nodes_max + l2] = 0;
	g->links[l2 * g->nodes_max + l1] = 0;

	return;
}


int graph_add_node(GRAPH *g) {
	int i;

	for (i = 0; i < g->nodes_max; i++)
		if (!g->usage[i])
			return (!(g->usage[i]++)) ? i : -1;
	return -1;
}


void graph_remove_node(GRAPH *g, int node) {
	int i;

	for (i = node; i < g->nodes_max2; i += g->nodes_max)
		if (g->links[i])
			graph_remove_link(g, i / g->nodes_max, i % g->nodes_max);
	g->usage[node]--;
	return;
}


int graph_neighbours(GRAPH *g, int node) {
	if (!g->usage[node])
		return 0;
	return g->usage[node] - 1;
}

int graph_neighbour(GRAPH *g, int node, int neighbour) {
	int i, j;

	for (i = node, j = 0; i < g->nodes_max2; i += g->nodes_max) {
		if (!g->links[i])
			continue;
		if ((j++) == neighbour)
			return i / g->nodes_max;
	}

	return -1;
}


void graph_set_visit(GRAPH *g, int node) {
	g->visit[node] = 1;

	return;
}


void graph_set_not_visited(GRAPH *g, int node) {
	g->visit[node] = 0;

	return;
}


int graph_get_visit(GRAPH *g, int node) {
	return g->visit[node];
}


void graph_visit_reset(GRAPH *g) {
	memset(g->visit, 0, sizeof(int) * g->nodes_max);

	return;
}
