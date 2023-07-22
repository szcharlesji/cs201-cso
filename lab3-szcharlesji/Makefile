OBJS := ex1.o ex2.o ex3.o ex4.o ex5.o
SOLOBJS := objs/ex1_sol.o objs/ex2_sol.o objs/ex3_sol.o objs/ex4_sol.o objs/ex5_sol.o
TESTEROBJ := objs/tester.o
CFLAGS := -Werror -c -Og -g -std=gnu11
SOLCFLAGS := -Werror -c -Og -std=gnu11

GCCVERSION = $(shell gcc -dumpversion)
ifeq "$(GCCVERSION)" "4.8.5"
	GCC = gcc
else
	GCC = gcc-4.8
endif


all : tester tester_sol

submitfiles: 
	zip submitfiles.zip *.c *.h Makefile

%.o : %.c
	$(GCC) $(CFLAGS) $*.c

tester : $(OBJS) $(TESTEROBJ)
	$(GCC) $^ -o $@

tester_sol : $(SOLOBJS) $(TESTEROBJ)
	$(GCC) $^ -o $@

clean :
	rm -f $(OBJS) tester tester_sol
