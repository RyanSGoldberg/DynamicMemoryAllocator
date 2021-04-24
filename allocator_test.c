#include <stdio.h>
#include <string.h>
#include "allocator.h"

#define a "HelloWorld"
#define b "FooBar!"
#define c "ShalomOlam"
#define d '5'



int main(int argc, char **argv){
    printf("Beginnings Tests\n");
    // Fill the heap with back to back elements
    char *x = cust_malloc(strlen(a)+1);
    strncpy(x, a, strlen(a)+1);
    char *y = cust_malloc(strlen(b)+1);
    strncpy(y, b, strlen(b)+1);
    char *z = cust_malloc(strlen(c)+1);
    strncpy(z, c, strlen(c)+1);
    // Exactly 128 bytes are on the heap rn
    char *i = cust_malloc(1);
    *i = '5';
    // Free the last element and make sure the block size decreases then put it back
     cust_free(i);
    heapdump();
    i = cust_malloc(1);
    *i = '5';
    // Free the 'middle' element and try put something too big into that slot
    cust_free(y);
    y = cust_malloc(strlen("foobar!1")+1);
    strncpy(y, "foobar!1", strlen("foobar!1")+1);
    heapdump();
    // Remove the first element and make sure something new will fill that slot if it fits
    cust_free(x);
    x = cust_malloc(strlen(b)+1);
    strncpy(x, b, strlen(b)+1);
    heapdump();
    // Freeing the last element left will free up everything
    cust_free(x);
    cust_free(z);
    cust_free(i);
    cust_free(y);
    heapdump();
    char *s = cust_malloc(257);
    for(int i = 0; i < 257; i++){
        s[i] = 'a';
    }
    s[256] = '\0';
    heapdump();
    cust_free(s);
    heapdump();
    
}