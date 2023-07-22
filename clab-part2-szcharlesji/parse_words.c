#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "htable.h"
#include "list.h"
#include "parse_words.h"

/////////  Bonus exercise //////////
//This function parses NULL-terminated byte array "buf" 
//to extract words and stores them in the given htable "ht"
//Words are separated from each other by a single newline chracter '\n'
//Return the number of words parsed/stored
//
//Hint: consider using the C library function strtok_r
unsigned int parse_n_store_words(char *buf, htable_t *ht)
{
	unsigned int count = 0;
	/*
 	while (buf[0]!=0)
	{
		if (buf[0] == '\n')
		{
			count++;
		}
	buf++;
	}
	return count;
	*/
	char *word;
	/*
 * 	word = strtok(buf, "\n");
	htable_put(ht, word, 1, sum_accum);
	while (word != NULL) {
		htable_put(ht, word, 1, sum_accum);
		word = strtok(NULL, "\n");
	 	//htable_put(ht, word, 1, sum_accum);
		count ++;
	}
	return count;
	*/
	char *new = buf;
	while ((word = strtok_r(new, "\n", &new))) {
		htable_put(ht, word, 1, sum_accum);
		count++;
	}
	return count;
	
}


