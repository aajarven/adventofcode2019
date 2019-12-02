CC=gcc
CFLAGS=-pedantic -Wall -Wextra -std=gnu99 -Wno-format-security

all: fuel

fuel: fuel.o ../utils/filereader.o bin
	$(CC) $(CFLAGS) -o bin/fuel fuel.o ../utils/filereader.o

fuel.o: fuel.c fuel.h ../utils/filereader.o
	$(CC) $(CFLAGS) -c fuel.c

../utils/filereader.o: ../utils/filereader.c ../utils/filereader.h
	$(CC) $(CFLAGS) -c ../utils/filereader.c -o ../utils/filereader.o

bin:
	mkdir -p bin

clean: 
	rm bin/* *.o
	rm ../utils/*.o
