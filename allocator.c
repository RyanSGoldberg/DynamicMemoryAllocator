#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define BLOCK_SIZE  4096 // Pagesize on my machine

typedef struct {
    void *end_prev;
    void *end_blk;
    void *start_blk;
    void *start_next;
    int   size; 
}  Block;

void *top = NULL;

void *cust_malloc(size_t size){
    if(NULL == top){
        top = sbrk(0);
        printf("%p\n", top);
        top = sbrk(0);
        printf("%p\n", top);
        top = sbrk(BLOCK_SIZE);
        printf("%p\n", top);
        top = sbrk(0);
        printf("%p\n", top);
        top = sbrk(0);
        printf("%p\n", top);
    }
    return NULL;
}

void cust_free(void *ptr){

}

// Blocks of memory allocated on the heap are actually a special 
// type of data structure consisting of (1) A pointer to the end
//  of the previous block, (2) a pointer to the end of this block,
//   (3) the allocated block of memory which can vary in size depending
//    on its use, (4) a pointer to the beginning of this block, and (5)
//     a pointer to the next block.
