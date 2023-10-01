
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

int main(void) {
	Queue q = QueueNew();
	
	// Write a benchmark test to demonstrate the difference between
	// ArrayQueue and CircularArrayQueue
	
	// enqueue 1 to 10000
	for (int i = 1; i <= 20000; i++) {
		QueueEnqueue(q, i);
	}
	
	// dequeue 1 to 5000
	for (int j = 1; j <= 7000; j++) {
		QueueDequeue(q);
	}	
	
	// dequeue and enqueue 7000 numbers
	for (int i = 1; i <= 7000; i++) {
		QueueEnqueue(q, i);
		QueueDequeue(q);
	}	
	
	QueueFree(q);
}

