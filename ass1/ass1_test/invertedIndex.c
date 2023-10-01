// z5271698

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "invertedIndex.h"

#define MAX_NAME_SIZE 101
#define QUESTION '?'
#define COMMA ','
#define SEMICOLON ';'
#define FULLSTOP '.'
#define SPACE ' '

// =========   Part-1 Function  =========  

InvertedIndexBST readFiles(char *collectionFilename, InvertedIndexBST tree);
double calculateTF (char *filename, char *word);
InvertedIndexBST doTreeAdd(InvertedIndexBST tree, char *word, FileList new);
InvertedIndexBST BSTNew(char *word, FileList new);
FileList doListAdd(FileList head, FileList new);
FileList listNew(char *filename, double tf);
void BSTreeInfix(FILE *output,InvertedIndexBST tree);

// =========   Part-2 Function  =========  

InvertedIndexBST search(InvertedIndexBST tree, char *searchWord);
double caculateIdf(FileList head, int D);
TfIdfList tfNew(char *filename, double tfIdf);
TfIdfList doTfIdfAdd(TfIdfList head, TfIdfList new);
TfIdfList searchFilename(TfIdfList head, char *filename, double tfIdf);


// =========   Part-1 Function  =========  


// Normalises a given string.
char *normaliseWord(char *str) {       
    // Converting all characters to lowercase. 
    int counter = 0;
    while (str[counter] != '\0') {      
        str[counter] = tolower(str[counter]);
        counter++;
    } 
     
    counter = counter - 1;
    // Remove the punctuation marks and trailing spaces.
    if (str[counter] == FULLSTOP || str[counter] == COMMA || str[counter] == QUESTION 
    || str[counter] == SEMICOLON || str[counter] == SPACE) {
        str[counter] = str[counter + 1];     
    } 

    return str;
}


// The function needs to read a given file with collection of file names, 
// read each of these files, generate inverted index and return the inverted index. 
InvertedIndexBST generateInvertedIndex(char *collectionFilename) {   
    InvertedIndexBST tree = NULL; 
    tree = readFiles(collectionFilename, tree);
    return tree;
}


// Read files' name from collectionFilename. And generate BSTtree and FileList.
InvertedIndexBST readFiles(char *collectionFilename, InvertedIndexBST tree) {
    
    // Open and read all text from collection.
    char fName[MAX_NAME_SIZE];
    FILE *file = fopen(collectionFilename, "r");
    assert(file != NULL);

    while (fscanf(file, "%s", fName) == 1) {
        
        // Read all words from each file.  
        char fword[MAX_NAME_SIZE];
        FILE *words = fopen(fName, "r");  
        assert(words != NULL);
        
        while (fscanf(words, "%s", fword) == 1) {               
            
            // And generate BSTtree and FileList.
            strcpy(fword, normaliseWord(fword));
            FileList new = listNew(fName, calculateTF(fName, fword));
            tree = doTreeAdd(tree, fword, new);
        }                 
        fclose(words);

    }  
    fclose(file);
    
    return tree;
}


// Calculate value of tf.
double calculateTF (char *filename, char *word) {
    
    // Count how many times word appears, and total words in file.
    double time = 0;
    double total = 0;
    char find[MAX_NAME_SIZE];
    FILE *file = fopen(filename, "r");     
    assert(file != NULL);
    
    while (fscanf(file, "%s", find) == 1) { 
        strcpy(find, normaliseWord(find));            
        if (strcmp(find, word) == 0) { 
            // Word appears.
            time++;
        }
        total++;           
    }

    fclose(file);

    // Calculate tf value.
    double tf = time / total;
    return tf; 
}


// Add a value of BST tree. Duplicate word are not allowed.
// In this function, I use code from the course material
InvertedIndexBST doTreeAdd(InvertedIndexBST tree, char *word, FileList new) {
	if (tree == NULL) {
        return BSTNew(word, new);
	}
	
	if (strcmp(word, tree->word) < 0) {
		tree->left = doTreeAdd(tree->left, word, new);
	} else if (strcmp(word, tree->word) > 0) {
		tree->right = doTreeAdd(tree->right, word, new);
	} 
    else { 
        // strcmp(word, tree->word) = 0
	    tree->fileList = doListAdd(tree->fileList, new);
	}	
    return tree;
}


// Creates a new node of InvertedIndexBST.
InvertedIndexBST BSTNew(char *word, FileList new) {
	InvertedIndexBST tree = malloc(sizeof(struct InvertedIndexNode));
	assert(tree != NULL);
    
	tree->right = NULL;
	tree->left = NULL;
	tree->fileList = new;
	
    tree->word = malloc(sizeof(char[MAX_NAME_SIZE]));
    strcpy(tree->word, word);
	return tree;
}


// Add a value of file node list. list of filenames in inverted index 
// should be alphabetically ordered and duplicate filenames are not allowed.
FileList doListAdd(FileList head, FileList new) {
    FileList node = head;
    if (node == NULL) { // When fileList is empty.   
        return new;
    }    
    
    FileList pre = NULL;
    while (node != NULL) {
        if (strcmp(new->filename, node->filename) == 0) { 
            // This filename already exist.
            return head;
        } else if (strcmp(new->filename, node->filename) < 0) {             
            if (pre == NULL) { // Insert in head of list.
                head = new;
                new->next = node;
                return head;  
            } else { // Insert in middle.         
                new->next = node;
                pre->next = new;
                return head;
            }     
        } else {
            pre = node;
            node = node->next;
        }
           
    }
    
    // Insert at the end.
    pre->next = new;
    return head;
}


// Creates a new node of FileListNode.
FileList listNew(char *filename, double tf) {
    struct FileListNode *n = malloc(sizeof(struct FileListNode));
    assert(n != NULL);

	n->next = NULL;
	n->tf = tf;
	
    n->filename = malloc(sizeof(char[MAX_NAME_SIZE]));
    strcpy(n->filename, filename);

	return n;
}


// Outputs the given inverted index to a file named invertedIndex.txt.
// The output should contain one line per word, with the words ordered
// alphabetically in ascending order.
void printInvertedIndex(InvertedIndexBST tree) {
    
    // Write output in "invertedIndex.txt".
    FILE *output = fopen ("invertedIndex.txt", "w");     
    BSTreeInfix(output, tree); 
    fclose(output);  
}


// Prints the values in the given BSTree in infix order and print the list.
// In this function, I use code from the course material.
void BSTreeInfix(FILE *output, InvertedIndexBST tree)
{
	if (tree == NULL) return;
	BSTreeInfix(output, tree->left);
	
    // Print Tree Index.
    fprintf(output, "%s ", tree->word);
    
    // Print filenames and tf value.  
    FileList n = tree->fileList;
    while (n != NULL) {
        fprintf(output, "%s (%lf) ", n->filename, n->tf);
        n = n->next;
    }
    fprintf(output, "\n"); 

	BSTreeInfix(output, tree->right);
}



// =========   Part-2 Function  =========  



// Returns an ordered list where each node contains a filename and the 
// corresponding tf-idf value for a given searchWord. You only need to
// include documents (files) that contain the given searchWord.
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {
    TfIdfList List = NULL;
    
    // Find the searchword and calculate tfidf value.
    tree = search(tree, searchWord);
    FileList node = tree->fileList;
    double idf = caculateIdf(node, D);
    
    // Loop to insert code in List.
    while (node != NULL) {
        double tfIdf = (node->tf) * idf;
        TfIdfList New = tfNew(node->filename, tfIdf);
        List = doTfIdfAdd(List, New);
        node = node->next;
    }
    return List;
}


// Search for searchWord in BSTtree.
// In this function, I use code from the course material.
InvertedIndexBST search(InvertedIndexBST tree, char *searchWord) {
    
    // Iterative search
	while (tree != NULL) {
		if (strcmp(searchWord, tree->word) == 0) {
            return tree;
		} else if (strcmp(searchWord, tree->word) < 0) {
			tree = tree->left;
		} else if (strcmp(searchWord, tree->word) > 0) {
			tree = tree->right;
		}
	}	

    // Couldn't find searchWord.
	return NULL; 
}


// Caculate the value of idf.
double caculateIdf(FileList head, int D) {
    double times = 0;
    FileList n = head;
    while (n != NULL) {
        times++;
        n = n->next;
    }

    double idf = log10(D / times);
    return idf;
}


// Creates a new node of TfIdfList.
TfIdfList tfNew(char *filename, double tfIdf) {
    struct TfIdfNode *n = malloc(sizeof(struct TfIdfNode));
    assert(n != NULL);

	n->next = NULL;
	n->tfIdfSum = tfIdf;
	
    n->filename = malloc(sizeof(char[MAX_NAME_SIZE]));
    strcpy(n->filename, filename);
    n->filename[strlen(filename)] = '\0';
	return n;
}


// Add a value of file TfIdf list.  The list must be 
// in descending order of tf-idf value. If there are 
// multple files with the same tf-idf, order them by their filename 
// in ascending order. 
TfIdfList doTfIdfAdd(TfIdfList head, TfIdfList new) {
    TfIdfList curr = head;
    if (curr == NULL) { 
        // TfIdfList is empty.   
        return new;
    }    
    
    TfIdfList pre = NULL;
    while (curr != NULL) {
        if (new->tfIdfSum > curr->tfIdfSum || ((curr->tfIdfSum == new->tfIdfSum) 
        && strcmp(curr->filename, new->filename) > 0)) {             
            if (pre == NULL) { 
                // Insert in head of list.
                head = new;
                new->next = curr;  
            } else { 
                // Insert before curr.        
                new->next = curr;
                pre->next = new;
            } 
            return head;   
        } else { 
            pre = curr;
            curr = curr->next;
        }       
    }
    
    // Inser at the end.
    pre->next = new;
    return head;
}


// Returns an ordered list where each node contains a filename and the
// summation of tf-idf values of all the matching search words for that
// file. You only need to include documents (files) that contain one or
// more of the given search words. 
TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D) {

    // Create a TfIdflist for first searchWord.
    TfIdfList List = NULL;
    int i = 0;
    List = calculateTfIdf(tree, searchWords[i], D);
    
    // Loop from second searchWord, calculate tfidf value and find a node 
    // with same filename.
    i++;
    while (searchWords[i] != NULL) {
        
        // Find search word and calculate idf value.
        InvertedIndexBST search_word = search(tree, searchWords[i]);
        double idf = caculateIdf(search_word->fileList, D);
        
        FileList node = search_word->fileList;
        while (node != NULL) {
            double tfIdf = (node->tf) * idf;
            List = searchFilename(List, node->filename, tfIdf);
            node = node->next;
        }   

        i++;
    }   
    return List;
}


// Search filename TfIdfList. if find it, sum the tfIdf; 
// If don't find it, add a new node in TfIdfList.
TfIdfList searchFilename(TfIdfList head, char *filename, double tfIdf) {
    TfIdfList curr = head;
    TfIdfList pre = NULL;
    while (curr != NULL) {
        if (strcmp(curr->filename, filename) == 0) { // When find same filename.
            curr->tfIdfSum = curr->tfIdfSum + tfIdf;
            
            // TfIdfList is in descending order of tf-idf values. 
            // When pre == NULL, 
            // curr is the first in list, we don't need to change order
            if (pre != NULL) { 
                pre->next = curr->next;
                curr->next = NULL;
                head = doTfIdfAdd(head, curr);
            }   
            return head;
        }
        pre = curr;
        curr = curr->next;
    }

    // Did not find same filename, insert a new node in TfIdfList.
    TfIdfList new = tfNew(filename, tfIdf);
    head = doTfIdfAdd(head, new);
    return head;
}
