#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "parse_words.h"

#define MAXCHAR 500

// read the content of file "fname" into 
// a byte array terminated by NULL character.
// This function internally allocates enough space
// to store the file data read.
char *read_file_data(char *fname)
{
  //obtain size of the file
  struct stat st;
  assert(stat(fname, &st) == 0);

  char *buf = malloc(st.st_size+1);
  assert(buf);

  //read the entire ASCII file content to buf 
  int fd = open(fname, O_RDONLY);
  assert(read(fd, buf, st.st_size) == st.st_size);
  close(fd);

  //NULL-terminate buf
  buf[st.st_size] = '\0';
  assert(strlen(buf) == st.st_size);

  return buf;
}

// This function compares two tuples pointed to by x and y 
// based on their corresponding values, returning negative, zero or 
// positive if one is smaller, equal or larger than the other.
// This function is to be used by C library function qsort 
// (learn what qsort does by typing `man qsort` in the terminal)
// which is invoked in print_wordcount_stats
int count_cmp(const void *x, const void *y)
{
	return ((kv_t *)x)->val-((kv_t *)y)->val;
}

// print the most frequent topK words stored in the hash table
void print_wordcount_stats(htable_t *ht, int topK)
{
  kv_t *tuples = malloc(sizeof(kv_t)*ht->size);
  bzero(tuples, sizeof(kv_t)*ht->size);
  assert(tuples);
  int r = htable_get_all_tuples(ht, tuples, ht->size);
  assert(r == ht->size);
  qsort(tuples, ht->size, sizeof(kv_t), count_cmp);
  printf("%d unique words. Top-%d popular words are:\n", ht->size, topK);
  for (int i = ht->size-1; i > ht->size-1-topK ; i--) {
    printf("%s freq=%d\n", tuples[i].key, tuples[i].val);
  }
}

int main(int argc, char **argv)
{

  // getopt is a C library function to parse command line options 
  int c;
  int topK = 10;
  while ((c = getopt(argc, argv, "k:")) != -1) {
    switch (c) {
      case 'k':
        topK = atoi(optarg);
        break;
      default:
        printf("wordcount -k <number of topK words> <filename>\n");
        exit(1);
    }
  }

  // optind is a global variable set by getopt() 
	// it now contains the index of the first arguments
  // that is not an option
	if (argc - optind < 1) {
    printf("wordcount -k <number of topK words> <filename>\n");
		exit(1);
	}

  char *buf = read_file_data(argv[optind]);
  htable_t *ht = htable_create(100000);
  parse_n_store_words(buf, ht);
  print_wordcount_stats(ht, topK);
}
