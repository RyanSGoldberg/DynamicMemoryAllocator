#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include "print.h"


#ifndef BLOCK_SIZE
    #define BLOCK_SIZE 4096
#endif 

typedef struct {
    void *prev_start;
    void *chunk_start;
    void *chunk_end;
    void *next_start;
}  Chunk;

// The number of allocated blocks
long num_blks = 0;
// A pointer to the top of the heap
void *top = NULL;
// A pointer to the first Chunk on the heap
void *first;

/*
* Returns the block that ptr is in (as a relative offset from top)
*/
int block_number(void *ptr){
    return 1 + (uintptr_t)(ptr - top) / BLOCK_SIZE;
}


/*
* Checks if the boundary between 2 blocks in crossed by a chunk
*/
int crosses_block_boundary(void * chunk_start, size_t total_size){
    return block_number(chunk_start) != block_number(chunk_start + total_size);
}

/*
* Initialize a chunk at <chunk_start> of size <size>
*/
void init_chunk(void *prev_start, void *chunk_start, size_t size, void *next_start){
    Chunk* chunk = (Chunk *)chunk_start;
    chunk->prev_start = prev_start;
    chunk->chunk_start = chunk_start;
    // Space for size bytes + the metadata assosiated with each Chuch
    chunk->chunk_end = chunk_start + size + sizeof(Chunk); 
    chunk->next_start = next_start;
}

/*
* malloc which uses a first fit algorithm
*/
void *malloc(size_t size){
    int blocks_needed = 1 + size / BLOCK_SIZE;
    // A pointer to the new chunk to be created
    void *new_chunk_start;
    Chunk * curr = (Chunk *)first;

    // For the very first chunk allocated, do some initial setup
    if(NULL == first){
        top = sbrk(0);
        if((void *)-1 == sbrk(blocks_needed* BLOCK_SIZE)) {perror("malloc"); exit(1);}
        num_blks+= blocks_needed;
        first = top;
        new_chunk_start = first;
        init_chunk(NULL, first, size, NULL);
    // Fills in top to first if there is enough space and it is empty
    }else if(first != top && curr->chunk_start - top > size+sizeof(Chunk)+1){
        first = top;
        new_chunk_start = first;
        init_chunk(NULL, first, size, curr->chunk_start);
    }else{
        // Finds the next empty space with enough room or the end of the list
        Chunk *next;
        for(next = (Chunk *)(curr->next_start);
            NULL != curr->next_start && (next->chunk_start - curr->chunk_end <= size+sizeof(Chunk)+1);
            curr = next, next = (Chunk *)(next->next_start));
        new_chunk_start = curr->chunk_end+1;
        if(crosses_block_boundary(new_chunk_start, size+sizeof(Chunk))){
            if((void*) -1 == sbrk(blocks_needed* BLOCK_SIZE)){perror("malloc"); exit(1);}
            num_blks+= blocks_needed;
        }

        // We have reached the end since there was no space earlier
        if(NULL == curr->next_start){ 
            // Insert the new chunk at the end
            init_chunk(curr->chunk_start, new_chunk_start, size, NULL);
            // Attach it on the back end
            curr->next_start = ((Chunk *)new_chunk_start)->chunk_start;
        }else{
            // Insert the new chunk into the open slot between 2 chunks
            init_chunk(curr->chunk_start, new_chunk_start, size, next->chunk_start);
            // Attach it on either end
            curr->next_start = new_chunk_start;
            next->prev_start = new_chunk_start;
        }
    }
    // Return the offset of the pointer with just the space for the data to be allocated (no metadata)
    return new_chunk_start+sizeof(Chunk);
}

/*
* Free an allocated chunk of memory
* It is assumed the user will pass a valid pointer (i.e one which was returned my cust_malloc)
*/
void free(void *ptr){
    Chunk * curr = (Chunk *)(ptr-sizeof(Chunk));
    Chunk *prev = ((Chunk *)curr->prev_start);
    Chunk *next = ((Chunk *)curr->next_start);
    // The last element on the heap
    if(prev != NULL && next == NULL){ 
        prev->next_start = NULL;
    // The first element on the heap
    }else if(prev == NULL && next != NULL){ 
        next->prev_start = NULL;
        first = next;
    // The general case
    }else if(prev != NULL && next != NULL){
        prev->next_start = next->chunk_start;
        next->prev_start = prev->chunk_start;
    // Remove the only element
    }else{ 
        first = NULL; 
    }

     // Remove the pointers from curr to its neighbours
    curr->next_start = NULL;
    curr->prev_start = NULL;

    // Remove a block if it is no longer needed
    if(NULL == first ||
    (NULL != prev && NULL == prev->next_start 
        && !crosses_block_boundary(curr->chunk_start, curr->chunk_end-curr->chunk_start) &&
        (block_number(curr->chunk_start) != block_number(prev->chunk_end)))){
        int blks_to_remove = block_number(curr->chunk_end);
        if((void *)-1 == sbrk(-1 * blks_to_remove * BLOCK_SIZE)) perror("free");
        num_blks-= blks_to_remove;
    }
}

/*
* Print the contents of the heap to stdout
*/
void heapdump(){
    print("Printing the values on the heap ... \n");
    printd("Block Count: %d\n", num_blks);
    Chunk * curr = (Chunk *)first;
    for(int i = 0; NULL != curr; curr = (Chunk *)(curr->next_start), i++){
        print("###################################\n");
        printd("Chunk #%d - ", (long) i);
        if(crosses_block_boundary(curr->chunk_start, curr->chunk_end-curr->chunk_start)){
            printd("Blocks %d-", (long)block_number(curr->chunk_start));
            printd("%d\n", (long)block_number(curr->chunk_end));
        }else{
            printd("Block %d\n", (long)block_number(curr->chunk_end));
        }

        if(0 > curr->prev_start-top){
            print("\tprev_start: (nil)\n");
        }else{
            printd("\tprev_start: %d\n", (long)(curr->prev_start-top));
        }
        printd("\tchunk_start: %d\n", (long)(curr->chunk_start-top));
            prints("\t\tValue   :%s\n", (char*)curr->chunk_start+sizeof(Chunk));
            printd("\t\tVal size:%d\n", (long)(curr->chunk_end-curr->chunk_start-sizeof(Chunk)));
        printd("\tchunk_end: %d\n", (long)(curr->chunk_end-top));
        if(0 > curr->next_start-top){
            print("\tnext_start: (nil)\n");
        }else{
            printd("\tnext_start: %d\n", (long)(curr->next_start-top));
        }
    }
}