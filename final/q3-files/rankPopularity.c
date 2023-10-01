// rankPopularity.c ... implementation of rankPopularity function

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

/* 
    You will submit only this one file.

    Implement the function "rankPopularity" below. Read the exam paper for a
    detailed specification and description of your task.  

    - DO NOT modify the code in any other files. 
    - You can add static helper functions to this file.  
    - DO NOT add a "main" function to this file. 
*/

static void nodeReached(Graph g, int src, double *mnV);

void rankPopularity(Graph g, int src, double *mnV) {
	// count rankPopularity of src.
    double in = 0;
    double out = 0;
    for(int w = 0; w < g->nV; w++) {
        if (g->edges[src][w] != 0 && src != w) {
            out++;
        }
        if (g->edges[w][src] != 0 && src != w) {
            in++;
        }
    }

    if (out == 0) {
        // If outDegree(v) is zero, replace it with 0.5.
        out = 0.5;
    }  
    if (in != 0) {
        // src must be reachable
        mnV[src] = in / out;
    }      
    
    // count rankPopularity of reached vertex
    nodeReached(g, src, mnV);
}

static void nodeReached(Graph g, int src, double *mnV) {
    for (int v = 0; v < g->nV; v++) {
		// find vertex that src can reached.
        if (g->edges[src][v] != 0 && mnV[v] == -1) {
			// count popularityRank of v
            double in = 0;
            double out = 0;
            for(int w = 0; w < g->nV; w++) {
               if (g->edges[v][w] != 0 && v != w) {
                    out++;
                }
               if (g->edges[w][v] != 0 && v != w) {
                   in++;
                }
            }

            if (out == 0) {
                out = 0.5;
            }
            
            mnV[v] = in / out;
			nodeReached(g, v, mnV);
		}
    }
}




