# DynamicMemoryAllocator

## How to replace the malloc and free in stdlib.h
1. Run ```make```
    * Use BLOCKSIZE=x in order to change the default blocksize
2. Add the current directory absolute path to LD_LIBRARY_PATH  
    ```export LD_PRELOAD=$PWD/libryan.so```
3. Run your programs as normal

## Running the tests
Run ```make BLOCK_SIZE=128 tests```