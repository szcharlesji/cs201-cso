//Implement a C function called ex1 such that it achieves 
//the same functionality as the machine code of objs/ex1_sol.o
//Note: you need to figure out ex1()'s function signature yourself; 
//the signature is not void ex1()
  
#include <assert.h>
#include <stdbool.h>

bool ex1(int a, int b, long c) 
{
	if (a+b==c)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
