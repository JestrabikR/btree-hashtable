CC=gcc
CFLAGS=-Wall -std=c11 -pedantic
FILES=hashtable.c test-hashtable.c test_util.c 

.PHONY: test clean

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)

clean:
	rm -f test
