// This file gives you a starting point to implement malloc using implicit list
// Each chunk has a header (of type header_t) and does *not* include a footer
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "mm-common.h"
#include "mm-implicit.h"
#include "memlib.h"

// turn "debug" on while you are debugging correctness. 
// Turn it off when you want to measure performance
static bool debug = false; 

size_t hdr_size = sizeof(header_t);

void 
init_chunk(header_t *p, size_t csz, bool allocated)
{
	p->size = csz;
	p->allocated = allocated;
}

// Helper function next_chunk returns a pointer to the header of 
// next chunk after the current chunk h.
// It returns NULL if h is the last chunk on the heap.
// If h is NULL, next_chunk returns the first chunk if heap is non-empty, and NULL otherwise.
header_t *
next_chunk(header_t *h)
{
	//Your code here
    
    // When the heap is empty
    if (mem_heapsize() == 0) return NULL;

    // When h is NULL
	if (h == NULL)
	{
		return (header_t *) mem_heap_lo();
	}

    // When h is not NULL
    // if h is the last chunk on the heap
	if ((char *) mem_heap_hi() != (char *) h + h->size - 1)
	{
	    return (header_t *) ((char *) h + h->size);
    }
    // if h is not the last chunk on the heap
    else {
        return NULL;
    }
}

/* 
 * mm_init initializes the malloc package.
 */
int mm_init(void)
{
	//double check that hdr_size should be 16-byte aligned
	assert(hdr_size == align(hdr_size));
	// start with an empty heap. 
	// no additional initialization code is necessary for implicit list.
	return 0;
}


// helper function first_fit traverses the entire heap chunk by chunk from the begining. 
// It returns the first free chunk encountered whose size is bigger or equal to "csz".  
// It returns NULL if no large enough free chunk is found.
// Please use helper function next_chunk when traversing the heap
header_t *
first_fit(size_t csz)
{
	// point h to the first chunk
    if (mem_heapsize() == 0) return NULL;
    
    header_t* h = mem_heap_lo();
	
	while (h != NULL)
	{
        // if unallocated and size is bigger or equal to csz
		if (h->allocated==0 && csz <= h->size) return h;
		// otherwise go to the next chunk
        h = next_chunk(h);
	}
    // if not found, return NULL
	return NULL;
}

// helper function split cuts the chunk into two chunks. The first chunk is of size "csz", 
// the second chunk contains the remaining bytes. 
// You must check that the size of the original chunk is big enough to enable such a cut.
void
split(header_t *original, size_t csz)
{
	//Your code here
    //
    // if the original chunk is big enough
    if (original->size >= csz + hdr_size)
	{
        // creates the next chunk
		init_chunk((header_t *)((char *)original + csz), original->size - csz, false);
        //header_t *next = (header_t *)((char *)original + csz);
        //next->size = original->size - csz;
        //next->allocated = 0;
		// shrinks the current chunk
        original->size = csz;
	}
}

// helper function ask_os_for_chunk invokes the mem_sbrk function to ask for a chunk of 
// memory (of size csz) from the "operating system". It initializes the new chunk 
// using helper function init_chunk and returns the initialized chunk.
header_t *
ask_os_for_chunk(size_t csz)
{
	//Your code here

	header_t * h = (header_t *) mem_sbrk(csz);
	h->size = csz;
	h->allocated = 0;
    return h;
}

/* 
 * mm_malloc allocates a memory block of size bytes
 */
void *
mm_malloc(size_t size)
{
    if (mem_heapsize()==0) mm_init();
	//make requested payload size aligned
	size = align(size);
	//chunk size is aligned because both payload and header sizes
	//are aligned
	size_t csz = hdr_size + align(size);

	header_t *p = NULL;

	//Your code here 
	//to obtain a free chunk p to satisfy this request.
	//
	//The code logic should be:
	//Try to find a free chunk using helper function first_fit
	//    If found, split the chunk (using helper function split).
	//    If not found, ask OS for new memory using helper ask_os_for_chunk
	//Set the chunk's status to be allocated
    
    p = first_fit(csz);
    
    if (p != NULL)
    {
        split(p, csz);
    }
    else
    {
        p = ask_os_for_chunk(csz);
    }
    p->size = csz;
    p->allocated = true;
    

	//After finishing obtaining free chunk p, 
	//check heap correctness to catch bugs
	if (debug) {
		mm_checkheap(true);
	}
	return (void *)((char *)p + hdr_size);
}

// Helper function payload_to_header returns a pointer to the 
// chunk header given a pointer to the payload of the chunk 
header_t *
payload2header(void *p)
{
	//Your code here
    
    return (header_t *)((char *)p - hdr_size);
}

// Helper function coalesce merges free chunk h with subsequent 
// consecutive free chunks to become one large free chunk.
// You should use next_chunk when implementing this function
void
coalesce(header_t *h)
{
	//Your code here
    if (next_chunk(h) && next_chunk(h)->allocated == false)
    {
        h->size += next_chunk(h)->size;    
        coalesce(h);
    }
}

/*
 * mm_free frees the previously allocated memory block
 */
void 
mm_free(void *p)
{
	// Your code here
	// 
	// The code logic should be:
	// Obtain pointer to current chunk using helper payload_to_header 
	// Set current chunk status to "free"
	// Call coalesce() to merge current chunk with subsequent free chunks
    	
    payload2header(p)->allocated = false;
    coalesce(payload2header(p));
	// After freeing the chunk, check heap correctness to catch bugs
	if (debug) {
		mm_checkheap(true);
	}
}	

/*
 * mm_realloc changes the size of the memory block pointed to by ptr to size bytes.  
 * The contents will be unchanged in the range from the start of the region up to the minimum of   
 * the  old  and  new sizes.  If the new size is larger than the old size, the added memory will   
 * not be initialized.  If ptr is NULL, then the call is equivalent  to  malloc(size).
 * if size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr).
 */
void *
mm_realloc(void *ptr, size_t size)
{
	// Your code here
    //
    // if ptr is NULL
    if (ptr==NULL) return mm_malloc(size);
    // if size is equal to zero
    if (size == 0)
    {
        mm_free(ptr);
        return NULL;
    }
    // if ptr is not NULL and size is not zero
    header_t *h = payload2header(ptr);
    mm_free(ptr);
    coalesce(h);
    char *payload = mm_malloc(size);
	// Check heap correctness after realloc to catch bugs
	if (debug) {
		mm_checkheap(true);
	}
    if (size <= h->size) return memcpy(payload, ptr, size);
    else return memcpy(payload, ptr, h->size);
}


/*
 * mm_checkheap checks the integrity of the heap and returns a struct containing 
 * basic statistics about the heap. Please use helper function next_chunk when 
 * traversing the heap
 */
heap_info_t 
mm_checkheap(bool verbose) 
{
	heap_info_t info;
	// Your code here
	// 
	// traverse the heap to fill in the fields of info
	
	
	info.num_allocated_chunks = 0;
	info.num_free_chunks = 0;
	info.allocated_size = 0;
	info.free_size = 0;

	header_t *h = mem_heap_lo();
	while (h != NULL)
	{
		if (h->allocated)
		{
			info.num_allocated_chunks++;
			info.allocated_size += h->size;
		}
		else
		{
			info.num_free_chunks++;
            info.free_size += h->size;	
		}
		
		h = next_chunk(h);
	}
	// correctness of implicit heap amounts to the following assertion.
	assert(mem_heapsize() == (info.allocated_size + info.free_size));
	return info;
}
