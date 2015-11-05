# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=gcc
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -Wall
SRC=./*.c

all: 1.1 3.1 4.1 5.1

1.1: 1.1.o
	$(CC) 1.1.o -o 1.1

3.1: 3.1.o

4.1: 4.1.o -lrt

5.1: 5.1.o -lrt

1.1.o: 1.1.c
	$(CC) $(CFLAGS) $(SRC)
3.1.o: 3.1.c
	$(CC) $(CFLAGS) $(SRC)
4.1.o: 4.1.c
	$(CC) $(CFLAGS) $(SRC)
5.1.o: 5.1.c
	$(cc) $(CFLAGS) $(SRC)
clean:
	rm -rf *o 1.1 3.1 4.1 5.1

