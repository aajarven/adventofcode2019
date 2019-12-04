CC=gcc
CFLAGS=-pedantic -Wall -Wextra -std=gnu99

all: passwords

passwords: passwords.o ../utils/filereader.o bin
	$(CC) $(CFLAGS) -o bin/passwords passwords.o ../utils/filereader.o

passwords.o: passwords.c passwords.h ../utils/filereader.o
	$(CC) $(CFLAGS) -c passwords.c

../utils/filereader.o: ../utils/filereader.c ../utils/filereader.h
	$(CC) $(CFLAGS) -c ../utils/filereader.c -o ../utils/filereader.o

bin:
	mkdir -p bin

clean:
	rm bin/* *.o
	rm ../utils/*.o
