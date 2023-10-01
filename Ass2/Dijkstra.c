// Dijkstra ADT implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "PQ.h"

#define NO_PATH 0
#define INF 9999


static ShortestPaths spsNew(Graph g, Vertex src);
static void freePredNode(PredNode *p);
static PredNode *insertNode(Vertex v, PredNode *head);

////////////////////////////////////////////////////////////////////////

ShortestPaths dijkstra(Graph g, Vertex src) {
    // Create a new ShortestPaths
    ShortestPaths sps = spsNew(g, src);
    sps.dist[src] = 0;
    
    // Create a empty queue.
    PQ q = PQNew();

    // Queue q containing all vertices of g 
    // and using sps->dist as a priority.
    for (int i = 0; i < sps.numNodes; i++) {
        PQInsert(q, i, sps.dist[i]);
    }
    

    while (!PQIsEmpty(q)) {
        // Remove the smallest priority value from the priority queue.
        int v = PQDequeue(q);
        
        // For each w which is adjacent to v.
        AdjList w = GraphOutIncident(g, v);

        while (w != NULL) { 

            // Check whether add this edge is still the min distence or not.
            if (sps.dist[v] + w->weight < sps.dist[w->v]) {
                // Relax edge, clear PredNode of w and insert v
                // to be the w's predecessor.
                sps.dist[w->v] = sps.dist[v] + w->weight;
                freePredNode(sps.pred[w->v]);
                sps.pred[w->v] = NULL;
                sps.pred[w->v] = insertNode(v, sps.pred[w->v]);
                
                // And update w's distence.
                PQUpdate(q, w->v, sps.dist[w->v]);  

            } else if (sps.dist[v] + w->weight == sps.dist[w->v]) {
                // Both min distances.
                // insert v to be the predecessor.
                sps.pred[w->v] = insertNode(v, sps.pred[w->v]); 
            }
            w = w->next;
        }
    } 
    PQFree(q);

    for (int i = 0; i < sps.numNodes; i++) {
        // Change the distence for vertex which cannot reach src.
        if (sps.dist[i] == INF) {
            sps.dist[i] = NO_PATH;
        }
    }

    return sps;
}


// Print out the ShortestPaths structure.
void showShortestPaths(ShortestPaths sps) {
    printf("Source %d\n", sps.src);
    
    // Print out distances.
    printf("Distance\n");
    for (Vertex i = 0; i < sps.numNodes; i++) {
        printf("%d : %d\n", i, sps.dist[i]);
    }

    // Print out predecessors
    printf("Preds\n");
    for (Vertex i = 0; i < sps.numNodes; i++) {
        printf("%d : ", i);
        
        // Print out link list of pred
        for (PredNode *p = sps.pred[i]; p != NULL; p = p->next) {
            printf("[%d]-> ", p->v);
        }
        printf("NULL\n");
    }
}


// Frees all memory associated with the given ShortestPaths structure.
void freeShortestPaths(ShortestPaths sps) {
    for(int i = 0; i < sps.numNodes; i++) {
        // free each vertex's PredNode
        freePredNode(sps.pred[i]);
    }
    free(sps.pred);
    free(sps.dist);
}



///////////////////////Helper Functions///////////////////////////

// Create a new ShortestPaths
static ShortestPaths spsNew(Graph g, Vertex src) {
    ShortestPaths sps = {0};

    sps.src = src;
    sps.numNodes = GraphNumVertices(g);
    
    sps.dist = malloc((sps.numNodes) * sizeof(int)); 
    assert(sps.dist != NULL);
    for (Vertex i = 0; i < sps.numNodes; i++) {
        // Initialize the distence array.
        sps.dist[i] = INF;
    }

    // Initialize the array list.
    sps.pred = malloc((sps.numNodes) * sizeof(PredNode *));
    for (Vertex i = 0; i < sps.numNodes; i++) {
        sps.pred[i] = NULL;
    }

    return sps;
}


// Frees all memory allocated for the given PredNode.
static void freePredNode(PredNode *p) {
    if (p == NULL) {
        return;
    }
    freePredNode(p->next);
    free(p);
}


// Inserts a node at end of the list.
static PredNode *insertNode(Vertex v, PredNode *head) {
    PredNode *p = malloc(sizeof(PredNode));
    p->next = NULL;
    p->v = v;
    if (head == NULL) { 
        // If PredNode is empty.
        return p;
    }

    PredNode *curr = head;
    while (curr != NULL) {
        if (curr->v == v) {
            // when curr and p is same. Don't insert.
            free(p);
            return head;
        } else if (curr->next == NULL) {
            // curr is the last node in the list
            // insert p at the end.
            curr->next = p;
            return head;
        }
        curr = curr->next;
    }   
    return head;
}
