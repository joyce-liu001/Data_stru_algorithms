// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"

#define EMPTY 0
#define HAS_PATH 1

static NodeValues nodeNew(Graph g);
static double calculateBetweenness(Vertex v, Vertex src, Graph g, NodeValues nvs);
static double countTotalPath(ShortestPaths sps, Vertex src, Vertex curr);
static double countThoughPath(ShortestPaths sps, Vertex v, Vertex curr);
/////////////////////////////////////////////////////////

NodeValues closenessCentrality(Graph g) {
	// Create a new NodeValues nvs
	NodeValues nvs = nodeNew(g);

	// Calculate each vertex's closeness
	for (Vertex src = 0; src < nvs.numNodes; src++) {
		ShortestPaths sps = dijkstra(g, src);
		
		// Count how many vertex can src reached, and 
		// calculate sum of shortest paths of src.
		double reached = 1; // Reached itself.
		double sum_path = 0;
		for (int i = 0; i < sps.numNodes; i++) {
			if (sps.dist[i] != EMPTY) {
				sum_path += sps.dist[i];
				reached++;
			}
		}

		if (reached == 1) { 
			// Node is not connected to any other node (isolated), 
			// its closeness value is 0.
			nvs.values[src] = EMPTY;
		} else {
			// Calculate closeness.
			double closeness = ((reached - 1) / (nvs.numNodes - 1)) * ((reached - 1) / sum_path);
			nvs.values[src] = closeness;
		}
		freeShortestPaths(sps);
	}

	return nvs;
}

NodeValues betweennessCentrality(Graph g) {
	// Create a new NodeValues nvs.
	NodeValues nvs = nodeNew(g);
	
	// Calculate each v's Betweenness.
	for (Vertex v = 0; v < nvs.numNodes; v++) {	
		
		// Loop sources.
		for (Vertex src = 0; src < nvs.numNodes; src++) {
			nvs.values[v] += calculateBetweenness(v, src, g, nvs);	
		}
	}
	return nvs;
}


NodeValues betweennessCentralityNormalised(Graph g) {
	// Finds the betweenness centrality for each vertex.
	NodeValues nvs = betweennessCentrality(g);
	
	double n = nvs.numNodes;
	// n-2 and n-1 cannot be 0.
	assert(n > 2);

	// Calculate betweennessCentralityNormalised for each vertex.
	for (int i = 0; i < n; i++) {
		nvs.values[i] = nvs.values[i]/ ((n - 1) * (n - 2));
	}
	return nvs;
}

// Prints the values in the given NodeValues structure.
void showNodeValues(NodeValues nvs) {
	for (int i = 0; i < nvs.numNodes; i++) {
		printf("%d: %lf\n", i, nvs.values[i]);
	}
}


// Frees all memory associated with the given NodeValues structure.
void freeNodeValues(NodeValues nvs) {
	free(nvs.values);
}

///////////////////////Helper Functions///////////////////////////

// Create a new NodeValues structure.
static NodeValues nodeNew(Graph g) {

    NodeValues nvs = {0};
    nvs.numNodes = GraphNumVertices(g);
    
    nvs.values= malloc((nvs.numNodes) * sizeof(double)); 
    assert(nvs.values != NULL);
    for (Vertex i = 0; i < nvs.numNodes; i++) {
        // Initialize the array.
        nvs.values[i] = EMPTY;
    }
	return nvs;
}

// Calculate the Betweenness of v from src to each t
static double calculateBetweenness(Vertex v, Vertex src, Graph g, NodeValues nvs) {	
	// Find shortestpaths structure of src.
	ShortestPaths sps = dijkstra(g, src);
	double betweenness = 0;
	for (Vertex t = 0; t < nvs.numNodes; t++) {
		if (src != t && v != t && src != v) {
			// v is a passing point, cannot be the terminus and source.
			// source cannot be the terminus.
			
			// Count st which is total number of shortest paths.
			double st = 0;
		
			// Count number of those paths that pass through.
			double pass = 0;
		
			for (PredNode *p = sps.pred[t]; p != NULL; p = p->next) {
				st += countTotalPath(sps, sps.src, p->v);
				pass += countThoughPath(sps, v, p->v);
			}
				
			if (st != EMPTY) {
				// The denominator cannot be 0.
				// Sum betweennewss.
				betweenness += pass / st;
			} 
		}			
	}
	freeShortestPaths(sps);
	return betweenness;
}

// Return total number of smallest paths from src to t. 
static double countTotalPath(ShortestPaths sps, Vertex src, Vertex curr) {
	if (curr == src) {
		// Find source mean has path.
		return HAS_PATH;
	}

	double counter = 0;
	PredNode *p = sps.pred[curr];
	while (p != NULL) {
		// Sum all of path.		
		counter += countTotalPath(sps, src, p->v);
		p = p->next;
	}

	return counter;
}

// Return number of smallest paths from src to v 
// if v is in the path from 'src' to 'dest' in the given path.
static double countThoughPath(ShortestPaths sps, Vertex v, Vertex curr) {
	if (curr == v) {
		// Find all of path from src to v. 
		return countTotalPath(sps, sps.src, curr);
	}
	
	PredNode *p = sps.pred[curr];
	double counter = 0;
	while (p != NULL) {
		// Sum all of path through v.
		counter += countThoughPath(sps, v, p->v);
		p = p->next;
	}
	return counter;
}