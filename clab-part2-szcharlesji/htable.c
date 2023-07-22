#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
//Please read through the type declaration in htable.h
#include "htable.h"

// This function creates and initializes a new htable 
// It returns a pointer to the newly created htable struct (htable_t)
// The argument "capacity" specifies the size of 
// internal hash table array to allocate.
// Note that this function needs to perform 2 allocations;
// one for the htable struct, the other for the internal 
// hash table array.
// Note that you need to initialize the htable struct fields,
// including all the entries in the internal hash table array.
htable_t *htable_create(unsigned int capacity)
{
	htable_t *h = (htable_t *) malloc(sizeof(htable_t));
	h->arr_capacity = capacity;
	lnode_t **arr = (lnode_t **) malloc(capacity*sizeof(lnode_t *));
	//lnode_t **arr;
	h->arr = arr;
	h->size = 0;
	return h;
}

// This function is used internally by the hash table to calculate 
// the hashcode for a string with n characters, s[0...n-1]
//
// We'll use the common formula for calculating a string's hashcode:
// hashcode = s[0]*31^(n-1) + s[1]*31^(n-2) + .... + s[n-1]
// where ^ means exponentiation.
unsigned int hashcode(char *s)
{ 
	unsigned int hash = 0;
	
	for (int i = 0; i < strlen(s); i++) {
		hash = s[i] + 31 * hash;
	}
	return hash;
}

// This function inserts a key value pair to the hash table.
// If the key already exists, accumulate the new value into the existing value 
// using the given function pointer "accum". 
//
// The insertion takes the following steps:
// 1) calculate the hashcode of the given key  "key"
// 2) use the calculated hashcode to index into the table's 
// array of linked list. i = hashcode % ht->arr_capacity
// 3) insert the key in the indexed list ht->arr[i] using 
// list_insert_with_accum that you've implmeneted in list.c
//
// Note that you should update the ht->size field accordingly
void htable_put(htable_t *ht, char *key, int val, 
    void (*accum)(int *existing_val, int new_val))
{
	unsigned int hash = hashcode(key);
	unsigned int i = hash % ht->arr_capacity;
	//lnode_t ** head;
	//*head = ht->arr[i];	
	if (list_insert_with_accum(&(ht->arr[i]), key, val, sum_accum))
		ht->size++;
/*	

	if (ht->arr[i] == NULL) {
		struct lnode_t* head = NULL;
		struct lnode_t* second = NULL;
		head = (struct lnode_t)malloc(sizeof(struct lnode_t));
		ht->arr[i] = head;
		strcpy(head->tuple->key, key);
		head
	*/
}

// This function finds "key" in the hash table
// and returns the pointer to its associated value.
// It takes the following steps:
// 1) calculate the hashcode of the given key  "key"
// 2) use the calculated hashcode to index into the table's 
// array of linked list. i = hashcode % ht->arr_capacity
// 3) find the value associated with key in the indexed list ht->arr[i] using 
// list_find that you've implmeneted in list.c 
int htable_get(htable_t *ht, char *key)
{
  //TODO: Your code here
	unsigned int hash = hashcode(key);
	unsigned int i = hash % ht->arr_capacity;
	//lnode_t * head;
	//*head = ht->arr[i];
	return list_find(ht->arr[i], key);
}

// Traverse the hash table pointed to by "ht" and store 
// all the key-value tuples encountered in the hash table in the 
// array "tuples".  The capacity of the array "tuples" is "max". 
// Thus, the function should not store more than "max" entries
// in "tuples".
//
// Return the actual number of key-value tuples 
// written to "tuples".
int htable_get_all_tuples(htable_t *ht, kv_t *tuples, int max)
{
	int count = 0;
	
	for (int i = 0; i < ht->arr_capacity; i++) {
		//if (max <= 0) break; 
		//int num = list_get_all_tuples(ht->arr[i], tuples, max);	
		
		//max -= num;
		//count += num;
		lnode_t *current = ht->arr[i];
		while (current!=NULL && count < max) {
			*tuples = current->tuple;
			current = current->next;
			count++;
			tuples++;
		}
	}
	return count;
}
