// nodesNotBalanced.c ... implementation of nodesNotBalanced function

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "BSTree.h"

#define MAX(a,b) (((a)>=(b))?(a):(b))

/* 
    You will submit only this one file.

    Implement the function "nodesNotBalanced" below. Read the exam paper for a
    detailed specification and description of your task.  

    - DO NOT modify the code in any other files. 
    - If you wish, you can add static variable and/or 
      helper functions to this file.  
    - DO NOT add a "main" function to this file. 
*/

static int height(BSTree t, int *p);


int nodesNotBalanced(BSTree t) {
  int Notbal = 0;
  int *p = &Notbal;
  height(t, p);
	
	return *p;
}

static int height(BSTree t, int *p) {
  if (t == NULL) {
    return 0;
  }

  int lh = height(left(t), p); 
  int rh = height(right(t), p); 
  
  if (abs(lh - rh) > 1) {
    *p += 1;
  }

  return 1 + MAX(lh, rh); 
}





