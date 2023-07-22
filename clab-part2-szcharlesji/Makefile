CC     := gcc
CFLAGS := -Werror -std=gnu99 -g -Og


OBJS := str.o list.o htable.o parse_words.o 

all: clab2_test wordcount

submitfiles: 
	zip submitfiles.zip *.c *.h Makefile

wordcount: $(OBJS) wordcount.o
	gcc $^ -o $@ 


clab2_test: $(OBJS) clab2_test.o
	gcc $^ -o $@ 

%.o : %.c %.h
	gcc $(CFLAGS) -c ${<}


clean :
	rm -f $(OBJS) wordcount.o wordcount clab2_test.o clab2_test
