CC=gcc
CFLAGS=-pedantic -Wall -Wextra -std=gnu99 -Wno-format-security

all: crossing

crossing: crossing.o ../utils/filereader.o bin
	$(CC) $(CFLAGS) -o bin/crossing crossing.o ../utils/filereader.o -lm

crossing.o: crossing.c crossing.h ../utils/filereader.o
	$(CC) $(CFLAGS) -c crossing.c

../utils/filereader.o: ../utils/filereader.c ../utils/filereader.h
	$(CC) $(CFLAGS) -c ../utils/filereader.c -o ../utils/filereader.o

bin:
	mkdir -p bin

clean:
	rm bin/* *.o
	rm ../utils/*.o
