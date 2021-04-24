# DynamicMemoryAllocator

## How to replace the dynamic memory allocator in stdlib.h
1. (Recommended) Start a subshell  
2. Add the the shared obect to LD_PRELOAD  
    ```export LD_PRELOAD=`pwd`"/libryan.so"```
3. Run you program as normal

## Running the tests
Run ```make BLOCK_SIZE=128 tests```
