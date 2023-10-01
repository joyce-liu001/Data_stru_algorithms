// containsSequence.c ... implementation of containsSequence function

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "list.h"

#define FALSE 0
#define TRUE 1


/* 
    You will submit only this one file.

    Implement the function "containsSequence" below. Read the exam paper for 
    detailed specification and description of your task.  

    - DO NOT modify code in the file List.h . 
    - If you wish, you can add helper static functions/variable in this file.  
    - DO NOT add "main" function in this file. 
    
*/

/* 
	We say seq1 contains seq2 if all numbers in seq2 also appear in
	seq1, in the same order as they appear in seq2. 
	
	For example: 
	
	  seq1: 10, 5, 23, 56, 8, 92, 5, 7
	  seq2: 5, 8, 92
	  containsSequence(seq1, seq2) should return 1.
	
	  seq1: 10, 5, 23, 56, 8, 92, 5, 7
	  seq2: 8, 5, 92
	  containsSequence(seq1, seq2) should return 0.
	  	
	  seq1: 10, 5, 23, 56, 8, 92, 5, 7
	  seq2: 25, 23, 56, 8
	  containsSequence(seq1, seq2) should return 0.
	
	  seq1: 10, 5, 23, 56, 8, 92, 5, 7
	  seq2: 10, 7
	  containsSequence(seq1, seq2) should return 1.	

	  seq1: 10, 5, 23, 56, 8, 92, 5, 7
	  seq2: 8, 92, 7
	  containsSequence(seq1, seq2) should return 1.	
	  
	  seq1: 10, 5, 23, 56, 8, 92, 5, 7
	  seq2: 56
	  containsSequence(seq1, seq2) should return 1.	

	  seq1: 10, 5, 23, 56, 8, 92, 5, 7
	  seq2: 5, 5, 7
	  containsSequence(seq1, seq2) should return 1.	
	  
*/



int containsSequence (List seq1, List seq2) {
	struct _node *head2 = seq2->first;
	
	for (struct _node *head1 = seq1->first; head1 != NULL; head1 = head1->next) {
		if (head2 == NULL) {
			return TRUE;
		} else if (head2->value == head1->value) {
			head2 = head2->next;	
		}
	}

	if (head2 == NULL) {
		return TRUE;
	}
	
	return FALSE; 
}



