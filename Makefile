CFLAGS = -g -Wall -std=gnu99

all: allocator_test

allocator_test: allocator_test.o allocator.o
		gcc ${CFLAGS} -o $@ $^

%.o: %.c allocator.h
		gcc ${CFLAGS} -c $<

.PHONY: clean all

clean:
		rm -f *.o allocator_test
