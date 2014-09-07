CC = g++
CFLAGS = -Wall -O2
COMPILE = $(CC) $(CFLAGS) -c

all: test

test: main.o hash.o
	$(CC) -o test main.o hash.o

main.o: main.cc
	$(COMPILE) -o main.o main.cc

hash.o: hash.cc
	$(COMPILE) -o hash.o hash.cc
