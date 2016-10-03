CC = gcc
CFLAGS = -g -Wall

default: osh

osh: tokenize.o execute.o parse.o	
	$(CC) $(CFLAGS) -o osh osh.c tokenize.o parse.o execute.o

tokenize.o: 
	$(CC) $(CFLAGS) -c tokenize.c

parse.o:
	$(CC) $(CFLAGS) -c parse.c

execute.o:
	$(CC) $(CFLAGS) -c execute.c

test1: execute.o
	$(CC) $(CFLAGS) -o test1 test1.c execute.o

test2: execute.o
	$(CC) $(CFLAGS) -o test2 test2.c execute.o

test3: tokenize.o
	$(CC) $(CFLAGS) -o test3 test3.c tokenize.o

test4: tokenize.o parse.o execute.o
	$(CC) $(CFLAGS) -o test4 test4.c tokenize.o parse.o execute.o

test5: tokenize.o parse.o execute.o
	$(CC) $(CFLAGS) -o test5 test5.c tokenize.o parse.o execute.o

clean:
	$(RM) osh test1 test2 test3 *.o *~
