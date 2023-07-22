//Implement a C function called ex2 such that it achieves 
//the same functionality as the machine code of objs/ex2_sol.o
//Note: you need to figure out ex2()'s function signature yourself; 
//the signature is not void ex2()
  
#include <assert.h>

int ex2(int *arr, int x)
{
  int count = 0;
for (int i = 0; i < x; i++)
  {
if (arr[i] > 0) count+=arr[i];

  }
  return count;
}
