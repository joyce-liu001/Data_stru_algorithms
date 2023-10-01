// Lance-Williams Algorithm for Hierarchical Agglomerative Clustering
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>



#include "Graph.h"
#include "LanceWilliamsHAC.h"

#define INF 999
#define ROOT -1
#define MIN(a,b) (((a)<=(b))?(a):(b))
#define MAX(a,b) (((a)>=(b))?(a):(b))

static Dendrogram newD(Vertex v);
static void freeDistMatrix(double **dist, int total);
static double matrixDistence(int v, int w, Graph g);
static void singleLink(double **dist, int total, Dendrogram *alist, int count);
static void completeLink(double **dist, int total, Dendrogram *alist, int count);

////////////////////////////////////////////////////////////////////////
/**
 * Generates  a Dendrogram using the Lance-Williams algorithm (discussed
 * in the spec) for the given graph  g  and  the  specified  method  for
 * agglomerative  clustering. The method can be either SINGLE_LINKAGE or
 * COMPLETE_LINKAGE (you only need to implement these two methods).
 * 
 * The function returns a 'Dendrogram' structure.
 */
Dendrogram LanceWilliamsHAC(Graph g, int method) {
    int total_vertex = GraphNumVertices(g);
    
    // Create a matrices to store distances.
    double **dist = malloc(total_vertex * sizeof(double *));
    for (int v = 0; v < total_vertex; v++) {
        dist[v] = malloc(total_vertex * sizeof(double));
        for (int w = 0; w < total_vertex; w++) {
            // Find the distence between v and w.
            if (v == w) {
                dist[v][w] = INF;
            } else {
                dist[v][w] = matrixDistence(v, w, g);
            }
        }
    }

    // Create a array list.
    Dendrogram *alist = malloc((total_vertex) * sizeof(Dendrogram));
    for (Vertex i = 0; i < total_vertex; i++) {
        alist[i] = newD(i);
    }

    if (method == SINGLE_LINKAGE) {
        // When method is Single link method.
        singleLink(dist, total_vertex, alist, total_vertex);
    } else {
        // When method is Complete link method.
        completeLink(dist, total_vertex, alist, total_vertex);
    }
    

    Dendrogram d = alist[0];
    
    // Free matrix and array list.
    freeDistMatrix(dist, total_vertex);
    alist[0] = NULL;
    free(alist);

    return d;
}

/**
 * Frees all memory associated with the given Dendrogram structure.
 */
void freeDendrogram(Dendrogram d) {
    if (d == NULL) {
        return;
    }
    freeDendrogram(d->left);
    freeDendrogram(d->right);
    free(d);
}



///////////////////////Helper Functions///////////////////////////

// Create a new Dendrogram node.
static Dendrogram newD(Vertex v) {
    Dendrogram new = malloc(sizeof(Dendrogram));
    new->vertex = v;
    new->left = NULL;
    new->right = NULL;
    return new;
}


// Frees all memory associated with the given distence matrix.
static void freeDistMatrix(double **dist, int total) {
    for(int i = 0; i < total; i++) {
        free(dist[i]);
    }      
    free(dist);
}


// Return a distence between w and v.
static double matrixDistence(int v, int w, Graph g) {
    
    // Find the mallest distence from v to w.
    double vTow = INF;
    for (AdjList node = GraphOutIncident(g, v); node != NULL; 
    node = node->next) {
        if (node->v == w && node->weight != 0) {
            vTow = 1.0 / (1.0 * node->weight);
        }
    }
    
    // Find the mallest distence from w to v.
    double wTov = INF;
    for (AdjList node = GraphInIncident(g, v); node != NULL; node = node->next) 
    {
        if (node->v == w && node->weight != 0) {
            wTov = 1.0 / (1.0 * node->weight);
        }
    }

    // Return the smallest distence.
    return MIN(vTow, wTov);
}



static void singleLink(double **dist, int total, Dendrogram *alist, int count) {
    if (count == 1) {
        // Already merge every nodes.
        return;
    }
    
    double smDist = INF;
    Vertex i, j;
    for (int v = 0; v < total; v++) {
        for (int w = 0; w < total; w++) {
            // Find the mallest distence in the dist matrix.
            if (smDist >= dist[v][w]) {
                smDist = dist[v][w];
                i = v;
                j = w;
            }    
        }
    }

    if (j < i) {
        // i is always smallest.
        int tmp = i;
        i = j;
        j = tmp;
    }

    // Merge two Dnodes together and smaller indext in arraylist always point a root.
    Dendrogram root = newD(ROOT);
    root->left = alist[i];
    root->right = alist[j];
    alist[i] = root;
    alist[j] = NULL;

    // Update the distence matrix.
    // Copy two columns become same and delete one of them.
    dist[i][j] = INF;
    dist[j][i] = INF;
    for (int v = 0; v < total; v++) {
        
        // Choose the smallest distence store to dist[i][v]
        dist[i][v] = MIN(dist[i][v], dist[j][v]);
        dist[v][i] = dist[i][v];

        dist[j][v] = INF;
        dist[v][j] = INF;
    }

    singleLink(dist, total, alist, count - 1);
}


static void completeLink(double **dist, int total, Dendrogram *alist, int count) {
    if (count == 1) {
        // Already merge every nodes.
        return;
    }
    
    double lgDist = 0;
    Vertex i, j;
    for (int v = 0; v < total; v++) {
        for (int w = 0; w < total; w++) {
            // Find the largest distence in the dist matrix, 
            // but this distence cannot be infinite.
            if (lgDist <= dist[v][w] && dist[v][w] != INF) {
                lgDist = dist[v][w];
                i = v;
                j = w;
            }    
        }
    }

    
    if (j < i) {
        // i is always smallest.
        int tmp = i;
        i = j;
        j = tmp;
    }

    // Let the smallest Dnode become root.
    // Merge two Dnodes together.
    Dendrogram root = newD(ROOT);
    root->left = alist[i];
    root->right = alist[j];
    alist[i] = root;
    alist[j] = NULL;

    // Update the distence matrix.
    // Merge two columns become same and delete one of them.
    dist[i][j] = INF;
    dist[j][i] = INF;
    for (int v = 0; v < total; v++) {
        if (dist[i][v] == INF && dist[j][v] != INF) {
            // Only left the valid distance -- dist[j][v].
            dist[i][v] = dist[j][v];
            dist[v][i] = dist[i][v];
        } else if (dist[i][v] != INF && dist[j][v] != INF) {
            // Store the largest distence.
            dist[i][v] = MAX(dist[i][v], dist[j][v]);
            dist[v][i] = dist[i][v];
        }
       
        dist[j][v] = INF;
        dist[v][j] = INF;
    }
    
    completeLink(dist, total, alist, count - 1);
}
