#ifndef __GRAPH_H__
#define	__GRAPH_H__

typedef struct {
	int		*links;
	int		*usage;
	int		*visit;
	int		nodes_max;
	int		nodes_max2;
} GRAPH;

GRAPH *graph_new(int nodes_max);
GRAPH *graph_free(GRAPH *g);
void graph_add_link(GRAPH *g, int l1, int l2, int weight, int dl);
void graph_remove_link(GRAPH *g, int l1, int l2);
int graph_add_node(GRAPH *g);
void graph_remove_node(GRAPH *g, int node);
int graph_neighbours(GRAPH *g, int node);
int graph_neighbour(GRAPH *g, int node, int neighbour);
void graph_set_visit(GRAPH *g, int node);
void graph_set_not_visited(GRAPH *g, int node);
int graph_get_visit(GRAPH *g, int node);
void graph_visit_reset(GRAPH *g);


#endif
