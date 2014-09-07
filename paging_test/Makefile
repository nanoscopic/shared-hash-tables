CC = g++
CFLAGS = -Wall -O2
COMPILE = $(CC) $(CFLAGS) -c

all: test

test: main.o
	$(CC) -o test main.o

main.o: main.cc
	$(COMPILE) -o main.o main.cc

hash.o: hash.cc
	$(COMPILE) -o hash.o hash.cc
	
clean:	
	rm -rf *.o test.exe
