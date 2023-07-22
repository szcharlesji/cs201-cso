#pragma once
#include "list.h"

typedef struct htable {
  lnode_t **arr; //an array of lnode_t pointers, each is the head of a linked
        //list storing tuples whose key hashed to the same index
  unsigned int arr_capacity; //size of the previous array
  unsigned int size; //number of tuples stored in the hash table
} htable_t;

htable_t *htable_create(unsigned int capacity);
unsigned int hashcode(char *s);
void htable_put(htable_t *ht, char *key, int val,  
    void (*accum)(int *existing_val, int new_val));
int htable_get(htable_t *ht, char *key);
int htable_get_all_tuples(htable_t *ht, kv_t *tuples, int max);
