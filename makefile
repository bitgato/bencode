CC = gcc
CFLAGS = -c -Wall -Wextra
LIBS = -lssl -lcrypto

SOURCE = bencode.c dict.h list.h
TEST = test.c

TARGET=test

.PHONY: all $(TARGET) clean

all: bencode.o dict.o

bencode.o: bencode.c bencode.h list.h
	$(CC) $(CFLAGS) bencode.c -o $@ $(LIBS)

dict.o: dict.c bencode.h list.h
	$(CC) $(CFLAGS) dict.c -o $@ $(LIBS)

$(TARGET): bencode.o dict.o
	$(CC) $(CCFLAGS) $(TEST) bencode.o dict.o -o $@ $(LIBS)
	valgrind --leak-check=full --error-exitcode=1 ./test

clean:
	rm -f $(TARGET) *.o
