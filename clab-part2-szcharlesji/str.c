#include <stdio.h>
#include <assert.h>

#include "string.h"


// Return the length of the string pointed to by "s".
//
// Note: C standard library function strlen accomplishes
// exactly the same functionality as string_len. In this exercise,
// please do not use strlen. In subsequent exercises (list.c, htable.c), 
// it is recommended that you use strlen instead of string_len
int string_len(char *s)
{
	int count = 0;
	while (s[count] != '\0')
	{
		count++;
	}
	return count;
}

// Compare strings "s1" and "s2". It returns less than, equal to or great than zero 
// if "s1" is found to be less than, match, or larger than "s2" alphabetically.
// For example, string_cmp("aa", "aaa") should return a negative number
// and string_cmp("aa", "a0") should return a positive number
// Note that we use the ASCII values of characters as basis for comparison.
// As the ascii value of 'a' is greater than the ascii value of '0', 
// "aa" is considered to be greater than "a0".
//
// Note: C standard library function strcmp accomplishes
// exactly the same functionality as string_cmp. In this exercise,
// please do not use strcmp. In subsequent exercises (list.c, htable.c), 
// it is recommended that you use strcmp instead of string_cmp.
int string_cmp(char *s1, char *s2)
{
	while (*s1) {
		if (*s1 != *s2) {
			return *s1-*s2;
		}
	s1++;
	s2++;
}
	return *s1-*s2;	
}

// Convert integer x into hex format and store the resulting hex string to "str",
// e.g., if x=16, then str should be "00000010".
// If x=26, then str should be "0000001a";
// The resulting hex string should always have exactly 8 hex characters 
// followed by the null character.
// We assume the caller has allocated an array of at least 9 characters for "str", 
// Note: please do not use formatted output, such as sprintf
void int_to_hex(unsigned int x, char *str)
{
	/*
	int digit;
	int i = 1;
	char hex[8];
	while (x!=0)
	{
		digit = x % 16;
		if (digit < 10)
		{
			digit += 48;
		} else {digit += 87;}
		hex[i] = digit;
		i++;
		x /= 16;
	}
	//strrev(str);
	//str = '\0';
	for (int j = i-1; j>0; j--)
	{
	//	strcat(str, hex[j]);
	}
	
	sprintf(str, "%08x", x);
	
	//while (strlen(str)<9)
	//{
	//	memmove(str+1, str, strlen(str)-1);
	//	str = "0";
	//}
	*/
	/*
	int length = 0;

	if (x ==0)
	{length = 1;}
	int temp = x;
	for (; temp; temp >>= 4) {
		length++;
	}
	
	
	for (int i = 0; i < 8-length-1; i++)
	{
		str[i] = '0';
	}
	str = str - 8 + length;
	length--;
	while (length >=0)
	{
		if (x&0xf < 10)
		{
			str[length] = x&0xf + 48;
		}
		else {
			str[length] = x&0xf + 87;
		}
	x >>= 4;
	length--;
	}
	*/
	sprintf(str, "%08x", x);
	
}

