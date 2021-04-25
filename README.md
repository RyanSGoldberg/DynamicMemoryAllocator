# DynamicMemoryAllocator

## How to replace the malloc and free in stdlib.h
1. Run ```make```
2. Add the current directory absolute path to LD_LIBRARY_PATH
    ```export LD_LIBRARY_PATH=$PWD:${LD_LIBRARY_PATH}```
3. Run your programs as normal

## Running the tests
Run ```make BLOCK_SIZE=128 tests```