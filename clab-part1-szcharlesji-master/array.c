#include <stdio.h>
#include <assert.h>

#include "ptr.h"

// Return the sum of all elements in the given array.  The array
// "arr" has "n" elements.  We must explicitly pass in the array size
// because C array does not contain size information.
//
// Here, we use pointer "arr", which points to the first element of array, 
// to represent the array.  One can also write the function's signature as
// int array_sum(int array[], int n)
int array_sum(int *arr, int n)
{
	int sum = 0;
	for (int i = 0; i < n; i++)
	{
		sum +=  *(arr+i);
	}
	return sum;
}

// Copy the first n elements of array "src" into array "dst".
// We assume caller has already allocated an array 
// of at least n elements as "dst".
void array_cpy(int *dst, int *src, int n)
{
	for (int i = 0; i < n; i++)
	{
		*(dst+i) = *(src+i);
	}
}

// bubble_sort implements the bubble sort algorithm.
// https://en.wikipedia.org/wiki/Bubble_sort
// It is given an integer array "arr" of "n" elements. 
// Upon return, the integer array should contain the sorted numbers
// in increasing order. 
// You should use swap_int function you've already implemented in ptr.c 
// when implementing bubble_sort (that's why we included ptr.h here)
void bubble_sort(int *arr, int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (*(arr+j) > *(arr+j+1))
			{
				swap_int((arr+j), (arr+j+1));
			}
		}
	}
}

//arr is a 4 byte array containing an integer in big endian format,
//i.e. arr[0] has the most significant byte...
//Convert the integer to little endian and return the converted int.
int big_to_little_endian(char *arr)
{
	char temp1 = *arr;
	char temp2 = *(arr+1);
	*(arr) = *(arr+3);
	*(arr+3) = temp1;
	*(arr+1) = *(arr+2);
	*(arr+2) = temp2;
	return *(int*)arr;
}

