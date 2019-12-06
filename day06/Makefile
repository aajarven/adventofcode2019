CC=gcc
CFLAGS=-pedantic -Wall -Wextra -std=gnu99 -Wno-format-security

all: orbit

orbit: orbit.o ../utils/filereader.o ../utils/filereader.o ../utils/linkedlist.o ../utils/tree.o bin
	$(CC) $(CFLAGS) -o bin/orbit orbit.o ../utils/filereader.o ../utils/linkedlist.o ../utils/tree.o

orbit.o: orbit.c orbit.h
	$(CC) $(CFLAGS) -c orbit.c

../utils/filereader.o: ../utils/filereader.c ../utils/filereader.h
	$(CC) $(CFLAGS) -c ../utils/filereader.c -o ../utils/filereader.o

../utils/linkedlist.o: ../utils/linkedlist.c ../utils/linkedlist.h
	$(CC) $(CFLAGS) -c ../utils/linkedlist.c -o ../utils/linkedlist.o

../utils/tree.o: ../utils/tree.c ../utils/tree.h
	$(CC) $(CFLAGS) -c ../utils/tree.c -o ../utils/tree.o

bin:
	mkdir -p bin

clean:
	rm bin/* *.o
	rm ../utils/*.o
