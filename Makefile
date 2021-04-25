CFLAGS = -g -Wall -std=gnu99
BLOCK_SIZE = 4096
PATH_TO_CUST_ALLOCATOR = /home/ryan/Projects/C/DynamicMemoryAllocator

all: libryan.so

# Build the test program
allocator_test: libryan.so
		gcc -L${PATH_TO_CUST_ALLOCATOR} ${CFLAGS} -o $@ allocator_test.c -l:libryan.so

# Make the shared object file
libryan.so: allocator.o print.o
		gcc -shared -o libryan.so allocator.o print.o

# Compile library source code into position-independent code
allocator.o: allocator.c allocator.h print.h
		gcc ${CFLAGS} -DBLOCK_SIZE=${BLOCK_SIZE} -fpic -c $<

print.o: print.c print.h
		gcc ${CFLAGS} -fpic -c $<

.PHONY: clean all tests set_block_size

clean:
		rm -f *.o allocator_test libryan.so

# Run the tests (In a subshell)
tests: clean allocator_test
		(export LD_LIBRARY_PATH=`pwd`:`${LD_LIBRARY_PATH}`; ./allocator_test > output.txt);                     \
		diff correct_output.txt output.txt > /dev/null;    \
		if [ $$? -eq 0 ]; then echo "Tests Passed"; else diff -y --color=always --suppress-common-lines correct_output.txt output.txt || true; fi;\
		rm -f output.txt   		
