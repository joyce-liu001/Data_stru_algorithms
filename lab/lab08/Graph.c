// Implementation of the undirected weighted graph ADT

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

#define NOT_FOUND 0
#define FOUND 1

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;      // #vertices
	int nE;      // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;

static int validVertex(Graph g, Vertex v);

////////////////////////////////////////////////////////////////////////

Graph GraphNew(int nV)
{
	assert(nV > 0);

	Graph new = malloc(sizeof(*new));
	assert(new != NULL);
	new->nV = nV;
	new->nE = 0;

	new->edges = calloc(nV, sizeof(int *));
	assert(new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc(nV, sizeof(int));
		assert(new->edges[v] != 0);
	}

	return new;
}

void GraphFree(Graph g)
{
	assert(g != NULL);
	for (int v = 0; v < g->nV; v++)
		free(g->edges[v]);
	free(g->edges);
	free(g);
}

////////////////////////////////////////////////////////////////////////

void GraphInsertEdge(Graph g, Vertex v, Vertex w, int weight)
{
	assert(g != NULL && validVertex(g, v) && validVertex(g, w));

	if (g->edges[v][w] != 0 && g->edges[w][v] != 0)
		return; // an edge already exists; do nothing.

	g->edges[v][w] = weight;
	g->edges[w][v] = weight;
	g->nE++;
}

void GraphRemoveEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validVertex(g, v) && validVertex(g, w));
	if (g->edges[v][w] == 0 && g->edges[w][v] == 0)
		return; // the edge doesn't exist; do nothing.

	g->edges[v][w] = 0;
	g->edges[w][v] = 0;
	g->nE--;
}

////////////////////////////////////////////////////////////////////////

/**
 * Finds  the  shortest path (in terms of the number of hops) from `src`
 * to `dest` such that no edge on the path has weight larger than `max`.
 * Stores  the  path  in the given `path` array including both `src` and
 * `dest` and returns the number of vertices stored in the  path  array.
 * Returns 0 if there is no such path.
 */
int findPath(Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert(g != NULL);
	
	// create an array to store previously visited node for each vertex.
	int visited[g->nV];
	for (int i = 0;i < g->nV; i++) {
	    visited[i] = -1;
	}
	visited[src] = src; // source of graph 
	
    // create a queue.
    Queue q = QueueNew();
    
    // enqueue src into queue Q
    QueueEnqueue(q, src);
	int find = NOT_FOUND;
	while (!QueueIsEmpty(q) && find == NOT_FOUND) {
		
		int v = QueueDequeue(q);
		if (v == dest) {
			find = FOUND;
			// found path
		} else {
			for (int w = 0; w < g->nV; w++) {
				// if have eadge and weight not over max
				// w haven't been visited
				if (visited[w] == -1 && g->edges[v][w] != 0 && g->edges[v][w] < max) { 
					visited[w] = v;
					QueueEnqueue(q, w);
				}	
			}		
		}
	}
	QueueFree(q);
	
	if (find == NOT_FOUND) { // not found path
	    return NOT_FOUND;
	}
	
	// calculate length 
	int len = 1;
	int v = dest;
	while (v != src) {
		v = visited[v];
		len++;
	}
	
	// find path
	int i = len - 1;
	v = dest;
	while (i >= 0) {
		path[i] = v;
		v = visited[v];
		i--;
	}

	return len;
}

////////////////////////////////////////////////////////////////////////

void GraphShow(Graph g, char **names)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf("\n");
	}
}

////////////////////////////////////////////////////////////////////////
// Helper Functions

static int validVertex(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

