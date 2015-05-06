#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

struct node{
	int frame; 
	struct node *next;
};

struct node *head_node = NULL; 

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int lru_evict() {

	// return the head of the linked list 
	struct node *temp = head_node; 
	head_node = head_node->next; 
	return temp->frame;
}

/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {

	int input_frame = p->frame >> PAGE_SHIFT; 

	if (head_node == NULL){ // initialize head node
		struct node *new_node = malloc(sizeof(struct node)); 
		new_node->frame = input_frame; 
		new_node->next = NULL; 
		head_node = new_node; 
	}

	else{ // not the head node 
		struct node *temp = head_node; 
		while (1) { // loop over the linked list 
			if (temp->next != NULL && temp->next->frame == input_frame){ // check if it hits
				temp->next = temp->next->next;
			}
	    	if (temp->next == NULL){
	    		struct node *end_node = malloc(sizeof(struct node)); 
	    		end_node->frame = input_frame; 
	    		end_node->next = NULL;
	    		temp->next = end_node;
	    		return; 
	    	}
		    temp = temp->next;    
		}
	}
	return; 
}

/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
}
