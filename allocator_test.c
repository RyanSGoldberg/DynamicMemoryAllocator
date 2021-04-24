#include <stdio.h>
#include "allocator.h"
#include <string.h>

#define a "HelloWorld"
#define b "FooBar!"
#define c "ShalomOlam"
#define d '5'



int main(int argc, char **argv){
    printf("Beginnings Tests\n");
    // Fill the heap with back to back elements
    char *x = malloc(strlen(a)+1);
    strncpy(x, a, strlen(a)+1);
    char *y = malloc(strlen(b)+1);
    strncpy(y, b, strlen(b)+1);
    char *z = malloc(strlen(c)+1);
    strncpy(z, c, strlen(c)+1);
    // Exactly 128 bytes are on the heap rn
    char *i = malloc(1);
    *i = '5';
    // Free the last element and make sure the block size decreases then put it back
    free(i);
    heapdump();
    i = malloc(1);
    *i = '5';
    // Free the 'middle' element and try put something too big into that slot
    free(y);
    y = malloc(strlen("foobar!1")+1);
    strncpy(y, "foobar!1", strlen("foobar!1")+1);
    heapdump();
    // Remove the first element and make sure something new will fill that slot if it fits
    free(x);
    x = malloc(strlen(b)+1);
    strncpy(x, b, strlen(b)+1);
    heapdump();
    // Freeing the last element left will free up everything
    free(x);
    free(z);
    free(i);
    free(y);
    heapdump();
    char *s = malloc(257);
    for(int i = 0; i < 257; i++){
        s[i] = 'a';
    }
    s[256] = '\0';
    heapdump();
    free(s);
    heapdump();
    
}