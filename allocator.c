#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define BLOCK_SIZE  128 // 4096 // Pagesize on my machine

typedef struct {
    void *prev_start;
    void *chunk_start;
    void *chunk_end;
    void *next_start;
}  Chunk;

// A pointer to the top of the heap
void *top = NULL;
// A pointer to the first Chunk on the heap
void *first;

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

void *cust_malloc(size_t size){
    // FOR NOW ASSUME size < SIZE_BLOCK
    // A pointer to the new chunk to be created
    void *new_chunk_start;
    Chunk * curr = (Chunk *)first;

    // For the very first chunk allocated, do some initial setup
    if(NULL == first){
        top = sbrk(0);
        sbrk(BLOCK_SIZE); // TODO check for return -1
        first = top;
        new_chunk_start = first;
        init_chunk(NULL, first, size, NULL);
    // Fills in top to first if there is enough space and it is empty
    }else if(first != top && curr->chunk_start - top > size+sizeof(Chunk)){
        first = top;
        new_chunk_start = first;
        init_chunk(NULL, first, size, curr->chunk_start);
    }else{
        // Finds the next empty space with enough room or the end of the list
        Chunk * next = (Chunk *)(curr->next_start);
        for(;NULL != curr->next_start && (next->chunk_start - curr->chunk_end < size+sizeof(Chunk));
        curr = next, next = (Chunk *)(next->next_start));

        new_chunk_start = curr->chunk_end+1; // TODO Assume for now that we don't need additional blocks
        
        // We have reached the end since there was no space earlier
        if(NULL == curr->next_start){ 
            // Insert the new chunk at the end
            init_chunk(curr->chunk_start, new_chunk_start, size, NULL);
            // Attach it on the back end
            curr->next_start = ((Chunk *)new_chunk_start)->chunk_start;
        }else{
            // Insert the new chunk into the open slot
            init_chunk(curr->chunk_start, new_chunk_start, size, next->chunk_start);
            // Attach it on either end
            curr->next_start = new_chunk_start;
            next->prev_start = new_chunk_start;
        }
    }
    // Return the offset of the pointer with just the space for the data to be allocated (no metadata)
    return new_chunk_start+sizeof(Chunk);
}

void cust_free(void *ptr){ // Assume for now that they MUST pass a valid pointer
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
    // Remove the last element
    }else{ 
        first = NULL; 
    }

     // Remove the pointers from curr to its neighbours
    curr->next_start = NULL;
    curr->prev_start = NULL;
}

/*
* Print the contents of the heap to stdout
*/
void heapdump(){
    printf("Printing the values on the heap ... \n");
    Chunk * curr = (Chunk *)first;
    for(int i = 0; NULL != curr; curr = (Chunk *)(curr->next_start), i++){
        printf("###################################\n");
        printf("Chunk #%d\n", i);
        printf("\tprev_start: %ld\n", curr->prev_start-top);
        printf("\tchunk_start: %ld\n", curr->chunk_start-top);
            printf("\t\tValue   :%s\n", (char*)curr->chunk_start+sizeof(Chunk));
            printf("\t\tVal size:%ld\n", curr->chunk_end-curr->chunk_start-sizeof(Chunk));
        printf("\tchunk_end: %ld\n", curr->chunk_end-top);
        printf("\tnext_start: %ld\n", curr->next_start-top);
    }
}