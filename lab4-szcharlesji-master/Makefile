CC = gcc
CFLAGS = -Wall -Wno-unused-result -std=gnu99 -Og -g

OBJS = mdriver.o memlib.o 

all: mdriver unittest implicit-unittest implicit-mdriver

submitfiles:
	zip submitfiles.zip *.c *.h Makefile

implicit-unittest: memlib.o mm-implicit.o mm-implicit-unittest.o
	$(CC) $(CFLAGS) -o $@ $^

implicit-mdriver: $(OBJS) mm-implicit.o
	$(CC) $(CFLAGS) -o $@ $^

unittest: memlib.o  mm.o mm-unittest.o
	$(CC) $(CFLAGS) -o $@ $^

mdriver: $(OBJS) mm.o
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c %.h
	$(CC) $(CFLAGS) -c ${<}

clean:
	rm -f *~ *.o mdriver implicit-mdriver implicit-unittest unittest


