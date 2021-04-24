CFLAGS = -g -Wall -std=gnu99

all: allocator_test

verbose: allocator_test.o allocator.o
		gcc ${CFLAGS} -DVERBOSE=1  -o $@ $^	

allocator_test: allocator_test.o allocator.o
		gcc ${CFLAGS} -o $@ $^

%.o: %.c allocator.h
		gcc ${CFLAGS} -c $<

.PHONY: clean all tests

clean:
		rm -f *.o allocator_test

tests: allocator_test
		./allocator_test > output.txt;                     \
		diff correct_output.txt output.txt > /dev/null;    \
		if [ $$? -eq 0 ]; then echo "Tests Passed"; else diff -y --suppress-common-lines correct_output.txt output.txt || exit 0; fi  		
