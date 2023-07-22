//Implement a C function called ex3 such that it achieves 
//the same functionality as the machine code of objs/ex3_sol.o
//Note: you need to figure out ex3()'s function signature yourself; 
//the signature is not void ex3()
  
#include <assert.h>
#include <string.h>

int ex3(char *str1, char *str2, int max)
{
  // int size = strlen(str1);
  // int i;
  // for (i = 0; i < strlen(str2); i++)
  //     {
  //     if (size > max) break;
  //     str1[size] = str2[i];
  //     size ++;
  //     }
  // str1[size] = '\0';
  // return size;

  int len = 0;
  while (*str1!='\0') 
  {
    str1++;
    len++;
  }
  // str1++;
  // len++;

  int pos = 0;
  while (pos < max && *str2 != '\0')
  {
    *str1 = *str2;
    str1++;
    str2++;
    len++;
    pos++;
  }
  *str1 = '\0';
  return len;

}
