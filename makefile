CC = gcc
CFLAGS = -c -Wall -Wextra
LIBS = -lssl -lcrypto

SOURCE = bencode.c dict.h list.h
TEST = test.c

.PHONY: all test clean

all: bencode.o dict.o

bencode.o: bencode.c bencode.h list.h
	$(CC) $(CFLAGS) bencode.c -o $@ $(LIBS)

dict.o: dict.c bencode.h list.h
	$(CC) $(CFLAGS) dict.c -o $@ $(LIBS)

test: bencode.o dict.o
	$(CC) --coverage $(CCFLAGS) $(TEST) bencode.o dict.o -o $@ $(LIBS)
	valgrind --leak-check=full --error-exitcode=1 ./test

clean:
	rm -f test *.o *.gcno *gcda *.gcov
