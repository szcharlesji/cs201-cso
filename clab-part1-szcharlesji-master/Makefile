CC     := gcc
CFLAGS := -Werror -std=gnu11 -g -Og


OBJS := bitfloat.o ptr.o array.o clab_test.o

all: clab_test

submitfiles: 
	zip submitfiles.zip *.c *.h Makefile

clab_test: $(OBJS)
	gcc $^ -o $@ 

%.o : %.c %.h
	gcc $(CFLAGS) -c ${<}


clean :
	rm -f $(OBJS) clab_test
