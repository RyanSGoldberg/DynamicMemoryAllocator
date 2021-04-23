#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define BLOCK_SIZE  4096 // Pagesize on my machine

typedef struct {
    void *prev_start;
    void *chunk_start;
    void *chunk_end;
    void *next_start;
}  Chunk;

void *top= NULL;

/*
* Initialize a chunk at chunk_start of size size
*/
void init_chunk(void *prev_start, void *chunk_start, size_t size, void *next_start){
    Chunk* chunk = (Chunk *)chunk_start;
    chunk->prev_start = prev_start;
    chunk->chunk_start = chunk_start;
    chunk->chunk_end = chunk_start + size + sizeof(Chunk);
    chunk->next_start = next_start;
}

void *cust_malloc(size_t size){
    // FOR NOW ASSUME size < SIZE_BLOCK
    printf("Malloc called with size %lu\n", size);
    void *new_chunk_start;
    if(NULL == top){
        printf("First\n");
        top = sbrk(0);
        sbrk(BLOCK_SIZE);
        new_chunk_start = top;
        init_chunk(NULL, top, size, NULL);
    }else{
        printf("Subsequent\n");
        Chunk * curr = (Chunk *)top;
        Chunk * next = (Chunk *)(curr->next_start);
        for(;NULL != curr->next_start && (curr->chunk_end - next->chunk_start < size+sizeof(Chunk));
        curr = next, next = (Chunk *)(next->next_start));
        printf("%p\n", curr->next_start);


        new_chunk_start = curr->chunk_end+1; // TODO Assume for now that we don't need additional blocks
        

        if(NULL == curr->next_start){ 
            // We have reached the end since there was no space earlier
            init_chunk(curr->chunk_start, new_chunk_start, size, NULL);
            // Attach it on the back end
            curr->next_start = new_chunk_start;
        }else{
            // Insert the new chunk into the open slot
            init_chunk(curr->chunk_start, new_chunk_start, size, next->chunk_start);
            // Attach it on either end
            curr->next_start = new_chunk_start;
            next->prev_start = new_chunk_start;
        }
    }
    return new_chunk_start+sizeof(Chunk);
}

void cust_free(void *ptr){

}