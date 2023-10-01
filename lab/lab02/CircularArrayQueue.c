// Implementation of the Queue ADT using a circular array

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

#define DEFAULT_SIZE 16 // DO NOT change this line

// DO NOT modify this struct
struct queue {
	Item *items;
	int size;
	int capacity;
	int frontIndex;
};

/**
 * Creates a new empty queue
 */
Queue QueueNew(void) {
	Queue q = malloc(sizeof(*q));
	if (q == NULL) {
		fprintf(stderr, "couldn't allocate Queue");
		exit(EXIT_FAILURE);
	}
	
	q->items = malloc(DEFAULT_SIZE * sizeof(Item));
	if (q->items == NULL) {
		fprintf(stderr, "couldn't allocate Queue");
		exit(EXIT_FAILURE);
	}
	
	q->size = 0;
	q->capacity = DEFAULT_SIZE;
	q->frontIndex = 0;
	return q;
}

/**
 * Frees all resources associated with the given queue
 */
void QueueFree(Queue q) {
	free(q->items);
	free(q);
}

/**
 * Adds an item to the end of the queue
 */
void QueueEnqueue(Queue q, Item it) {

	int front = q->frontIndex;
	int back = front + q->size;
	
	// When the array has full of elements, we need to resize array.
	if (q->size == q->capacity) {
	    q->capacity *= 2;
	    q->items = realloc(q->items, q->capacity * sizeof(Item));
	    // If the frontIndex is not at first, we need move all elements which 
	    // are after frontIndex to the end.
	    if (q->frontIndex != 0) {
	        int end_before = q->size - 1;
	        int new_end = q->capacity - 1;
	        while (end_before > q->frontIndex) {
	            q->items[new_end] = q->items[end_before];
	            q->items[end_before] = q->items[end_before + 1];
	            new_end--;
	            end_before--;   
	        }
	        q->frontIndex = new_end;
	    }	    
	}	
	
	// When the array becomes full, we need insert item at the start of queue.
	if (back >= q->capacity) {
	    back = back - q->capacity;
	    q->items[back] = it;    
	} 
	
	// When array is not full, just insert item at the end.
	else {
	   q->items[back] = it;
	}
	
	q->size++;
}

/**
 * Removes an item from the front of the queue and returns it
 * Assumes that the queue is not empty
 */
Item QueueDequeue(Queue q) {

	// Move the front to the next.
	// When frontindex is at the end of array.
	if (q->frontIndex == q->capacity - 1) {
	    q->frontIndex = 0;
	} else {
	    q->frontIndex++;
	}
	
	q->size--;
	return delete;
}

/**
 * Gets the item at the front of the queue without removing it
 * Assumes that the queue is not empty
 */
Item QueueFront(Queue q) {
	assert(q->size > 0);
	
	return q->items[q->frontIndex];
}

/**
 * Gets the size of the given queue
 */
int QueueSize(Queue q) {
	return q->size;
}

/**
 * Returns true if the queue is empty, and false otherwise
 */
bool QueueIsEmpty(Queue q) {
	return q->size == 0;
}

/**
 * Prints the queue to the given file with items space-separated
 */
void QueueDump(Queue q, FILE *fp) {
	for (int i = q->frontIndex, j = 0; j < q->size; i = (i + 1) % q->capacity, j++) {
		fprintf(fp, "%d ", q->items[i]);
	}
	fprintf(fp, "\n");
}

