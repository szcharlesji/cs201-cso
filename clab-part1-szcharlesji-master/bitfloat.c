#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

// Return the bit value (0 or 1) at position pos of unsigned int n
// Note that the least significant bit position is 0. 
int get_bit_at_pos(unsigned int n, int pos)
{
	unsigned int valueAtPos = n & (1 << pos);
	return valueAtPos >> pos;
}

// Flip the bit value (1->0 or 0->1) at position pos of unsigned int n,
// while leaving other bits unchanged. 
// Return the changed integer.
// Note that the least significant bit position is 0. 
int flip_bit_at_pos(unsigned int n, int pos)
{
	unsigned int mask = 1 << pos;
	return n ^ mask;	
}

// Return the most significant byte of unsigned int n
char get_most_significant_byte(unsigned int n)
{
	return n >> 24;	
}

// Return true if n1+n2 causes overflow, return false otherwise
bool sum_overflowed(int n1, int n2)
{
	return ((long)n1 + (long)n2 >= 2147483647) || ((long)n1 + (long)n2 <= -2147483647);
}

//Extract the 8-bit exponent field of single precision floating point number f 
//and return it as an unsigned byte
unsigned char get_exponent_field(float f) 
{
	return (*(unsigned int*)&f >> 23) & 0b011111111;
}

//Bonus problem: Return the precision of floating point number f
//Precision is the difference f'-f such that f' is the 
//smallest *representable* floating point number larger than f  
float get_precision(float f)
{
	if (get_exponent_field(f) == 0)
	{
		int i = 0x00000001;
		return *(float*)&i;
	}

	unsigned int d = (*(unsigned int*)&f);
	unsigned int d_prime = ((d & 0xff800000) + 1);
	return *(float*)&d_prime;

}
