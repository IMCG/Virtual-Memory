#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

int *frames; 

int i; // clock hand 

/* Page to evict is chosen using the accurate clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int clock_evict() {
	
	while (frames[i] != 0){
		frames[i] = 0; 
		i++;
		if (i == memsize){
			i = 0;
		}
	}
	return i;
}

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p) {
	frames[p->frame >> PAGE_SHIFT] = 1;
	return; 
}

/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void clock_init() {
	frames = malloc(memsize * sizeof(int)); 
	int i=0;
	for (i = 0;i<memsize; i++){
		frames[i] = 0; 
	}
}
