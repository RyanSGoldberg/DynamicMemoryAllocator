CFLAGS = -g -Wall -std=gnu99
BLOCK_SIZE = 4096

all: allocator_test

allocator_test: allocator_test.o allocator.o
		gcc ${CFLAGS} -o $@ $^

%.o: %.c allocator.h
		gcc ${CFLAGS} -DBLOCK_SIZE=${BLOCK_SIZE} -c $<

.PHONY: clean all tests set_block_size

clean:
		rm -f *.o allocator_test

tests: clean allocator_test
		./allocator_test > output.txt;                     \
		diff correct_output.txt output.txt > /dev/null;    \
		if [ $$? -eq 0 ]; then echo "Tests Passed"; else diff -y --suppress-common-lines correct_output.txt output.txt || true; fi;\
		rm -f output.txt   		
