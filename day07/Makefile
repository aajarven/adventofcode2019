CC=gcc
CFLAGS=-pedantic -Wall -Wextra -std=gnu99 -Wno-format-security

all: intcode

intcode: intcode.o ../utils/filereader.o bin
	$(CC) $(CFLAGS) -o bin/intcode intcode.o ../utils/filereader.o

intcode.o: intcode.c intcode.h ../utils/filereader.o
	$(CC) $(CFLAGS) -c intcode.c

../utils/filereader.o: ../utils/filereader.c ../utils/filereader.h
	$(CC) $(CFLAGS) -c ../utils/filereader.c -o ../utils/filereader.o

bin:
	mkdir -p bin

clean: 
	rm bin/* *.o
	rm ../utils/*.o
