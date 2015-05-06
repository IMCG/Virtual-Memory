#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"

#define MAXLINE 256

extern int memsize;

extern int debug;

extern struct frame *coremap;

extern char* tracefile;

unsigned long *vaddr_array;

unsigned long *memory_array; 

int size = 0;

int vaddr_array_index = 0; 

/* Page to evict is chosen using the optimal (aka MIN) algorithm. 
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int opt_evict() {

	int i, j; 
	unsigned long MAX = 0; 

	int return_frame;

	// loop ove the memory array
	for (j = 0; j<memsize; j++){
		// loop over the tracefile
		for (i = vaddr_array_index; i<size; i++){
			if (vaddr_array[i] == memory_array[j]){
				if (i >= MAX){
					MAX = i; 
					return_frame = j;
				}
				break; 
			}
		} 
		if (i == size){
			return j;
		}
	}	
	return return_frame;
}

/* This function is called on each access to a page to update any information
 * needed by the opt algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void opt_ref(pgtbl_entry_t *p) {

	int input_frame_index = p->frame >> PAGE_SHIFT; 

	memory_array[input_frame_index] = vaddr_array[vaddr_array_index];

	vaddr_array_index++; 

	return;
}

/* Initializes any data structures needed for this
 * replacement algorithm.
 */
void opt_init() {

	memory_array = malloc(memsize * sizeof(unsigned long));

	// pre-load the tracefile
	FILE *tfp;
	if((tfp = fopen(tracefile, "r")) == NULL) {
		perror("Error opening tracefile:");
		exit(1);
	}

	// read over the tracefile to get the size 
	char buf[MAXLINE]; 
	addr_t vaddr = 0;
	char type;
	while(fgets(buf, MAXLINE, tfp) != NULL){
		size++; 
	}

	// malloc using the size 
	vaddr_array = malloc(size * sizeof(unsigned long));

	if((tfp = fopen(tracefile, "r")) == NULL) {
		perror("Error opening tracefile:");
		exit(1);
	}

	// save the vaddrs to the array
	int i = 0;
	while(fgets(buf, MAXLINE, tfp) != NULL){
		if(buf[0] != '=') {
			sscanf(buf, "%c %lx", &type, &vaddr);
			vaddr_array[i] =  vaddr; 
			i++; 
		} 
	}
}

