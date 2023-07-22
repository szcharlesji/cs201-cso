#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>

#include "panic_cond.h"
#include "str.h"
#include "list.h"
#include "htable.h"
#include "parse_words.h"

char *test_s1s[6] = {"aaa", "abc", "wzdkd", "1253", " ^%x", "egyptian"};
char *test_s2s[6] = {"aaa0", "bd", "13", "wzvf&*", " ", "egyptian"};

char *test_keys[6] = {"b", "aaa", "012", "xyz", "()", "9a"};
int test_values[6] = {0, 1, 2, 3, 4, 5};

int str_comparator(const void *a, const void *b)
{
  return strcmp(*(char **)a, *(char **)b);
}

void strcat_all(char *dst, int dst_len, char **strs, int n) 
{
  int d = 0;
  dst[0] = '\0';
  for (int i = 0; i < n; i++) {
    strncpy(dst+d, strs[i], dst_len-d);
    d+=strlen(strs[i]);
    assert(d < dst_len);
    dst[d++] = ' ';
    dst[d++] = '\0';
  }
}

int sgn(int x)
{
  if (x > 0)
    return 1;
  else if (x < 0)
    return -1;
  else 
    return 0;
}


void TestStr()
{
  printf("---Start testing str.c\n");  

  printf("      string_len...");
  char *s = "abcd1234%^&* ";
  int r = string_len("");
  int slen = strlen(s);
  panic_cond(r == 0, "an empty string has 0 length instead of %d");
  for (int i = 0; i < slen; i++) {
    r = string_len(s+i);
    panic_cond(r == strlen(s+i), "string (%s) has %d length instead of %d", 
        s+i, strlen(s+i), r);
  }
  printf("passed\n");

  printf("      string_cmp...");
  for (int i = 0; i < 6; i++) {
    r = string_cmp(test_s1s[i], test_s2s[i]);
    panic_cond(sgn(r)==sgn(strcmp(test_s1s[i], test_s2s[i])), 
          "string_cmp s1=%s s2=%s should return sign %d (instead of sign %d)", 
          test_s1s[i], test_s2s[i], sgn(strcmp(test_s1s[i], test_s2s[i])), sgn(r));
    r = string_cmp(test_s2s[i], test_s1s[i]);
    panic_cond(sgn(r) == sgn(strcmp(test_s2s[i], test_s1s[i])), 
          "string_cmp s1=%s s2=%s should return sign %d (instead of sign %d)", 
          test_s2s[i], test_s1s[i], sgn(strcmp(test_s2s[i], test_s1s[i])), sgn(r));
  }
  printf("passed\n");

  printf("      int_to_hex...");
  unsigned int x = 10;
  char buf[9];
  int_to_hex(x, buf);
  panic_cond(strcmp(buf, "0000000a")==0, 
    "x's hex string should be %08x instead of %s\n", x, buf);
  x = 25;
  int_to_hex(x, buf);
  panic_cond(strcmp(buf, "00000019")==0, 
    "x's hex string should be %08x instead of %s\n", x, buf);
  x = 0x12345678;
  int_to_hex(x, buf);
  panic_cond(strcmp(buf, "12345678")==0, 
    "x's hex string should be %08x instead of %s\n", x, buf);
  x = 0xdeadbeef;
  int_to_hex(x, buf);
  panic_cond(strcmp(buf, "deadbeef")==0, 
    "x's hex string should be %08x instead of %s\n", x, buf);
  printf("passed\n");

  printf("---Test str.c Passed\n");  

}

void simple_list_test()
{
  printf("      list_init...");
  lnode_t *headp;
  list_init(&headp);
  panic_cond(headp == NULL, 
      "after list_init, headp should be NULL instead of %p", headp);
  printf("passed\n");

  printf("      simple list_insert_with_accum, list_find, list_get_all_tuples...");
  bool r = list_insert_with_accum(&headp, "world", 1, sum_accum);
  panic_cond(r==true, "insert a previously unseen key should return true");
  
  int v = list_find(headp, "world");
  panic_cond(v==1, "list_find should return 1 instead of %d\n", v);

  kv_t tups[3];
  int n = list_get_all_tuples(headp, tups, 3);
  panic_cond(n==1, "list_get_all_tuples should return 1 instead of %d tuples\n", n);
  panic_cond(strcmp(tups[0].key, "world")==0, 
      "list_get_all_tuples should return tuple with key %s instead of %s\n", 
      "world", tups[0].key);

  list_insert_with_accum(&headp, "hello", 1, sum_accum);
  n = list_get_all_tuples(headp, tups, 3);
  panic_cond(n==2, "list_get_all_tuples should return 2 instead of %d tuples\n", n);

  panic_cond(strcmp(tups[0].key, "hello")==0 && strcmp(tups[1].key, "world")==0, 
      "list_get_all_tuples should return tuples with keys hello world instead of %s %s\n", 
      tups[0].key, tups[1].key);

  printf("passed\n");
}

void TestList()
{
  printf("---Start list.c\n");  

  simple_list_test();
  lnode_t *headp;
  list_init(&headp);
  
  printf("      list_insert_with_accum, list_get_all_tuples...");

  for (int dup = 0; dup <= 1; dup++) {
    for (int i = 0; i < 6; i++) {
      bool r = list_insert_with_accum(&headp, test_keys[i], test_values[i], sum_accum);
      panic_cond(dup?r==false:r==true, "insert previously %s key %s, \
          list_insert_with_accum should return %d instead of %d", 
          dup?"seen":"unseen", test_keys[i], dup?false:true, r);

      if (dup && i < 5)
        continue;

      kv_t tups[i+2];
      bzero(tups, sizeof(tups));
      int n = list_get_all_tuples(headp, tups, i+2);
      panic_cond(n==(i+1), "after inserting %d distinct tuples, list_get_all_tuples should\
          return %d instead of %d tuples", i+1, i+1, n);

      char *sorted_test_keys[i+1];
      for (int j = 0; j <= i; j++) 
        sorted_test_keys[j] = test_keys[j];
      qsort(sorted_test_keys, i+1, sizeof(test_keys[0]), str_comparator);

      char tmp_buf1[100];
      strcat_all(tmp_buf1, 100, sorted_test_keys, i+1);
      char *all_keys[i+1];
      for (int j = 0; j <= i; j++) {
        all_keys[j] = tups[j].key;
      }
      char tmp_buf2[100];
      strcat_all(tmp_buf2, 100, all_keys, i+1);
      panic_cond(strcmp(tmp_buf1, tmp_buf2)==0, "list_get_all_tuples should return sorted keys %s\
          (instead of %s)", tmp_buf1, tmp_buf2);

      for (int j = 0; j <= i; j++) {
        int v = -1;
        for (int k = 0; k <= i; k++) {
          if (strcmp(tups[j].key, test_keys[k]) == 0) {
            v = test_values[k];
            break;
          }
        }
        assert(v >= 0);
        panic_cond(dup?tups[j].val==2*v:tups[j].val==v, 
            "list_get_all_tuples should return val=%d (instead of %d)\
            associated with key=%s\n", dup?2*v:v, tups[j].val, tups[j].key);
      }
    }
  }
  printf("passed\n");

  printf("      list_find..");
  for (int i = 0; i < 6; i++) {
    int v = list_find(headp, test_keys[i]);
    panic_cond(v >= 0,
        "list_find %s should return a valid value instead of %d\n", test_keys[i], v);
    panic_cond(v == test_values[i]*2, 
        "list_find %s should return value %d instead of %d\n", test_keys[i], test_values[i]*2, v);
  }
  printf("passed\n");

  printf("---Test list.c Passed\n");  

}

void TestHashTable()
{
  printf("---Start htable.c\n");  
  
  printf("      htable_create...");
  int cap = 10;
  htable_t *ht = htable_create(cap);
  assert(ht);
  panic_cond(ht->size == 0 && ht->arr_capacity == cap, 
      "after htable_create, ht->size, ht->arr_capacity should be 0,%d instead of %d,%d",
      cap, ht->size, ht->arr_capacity);
  printf("passed\n");

  printf("      hashcode...");
  int r = hashcode("");
  panic_cond(r == 0, "hashcode of empty string should be 0 instead of %d", r);

  char *s = "a";
  r = hashcode(s);
  unsigned int result = s[0];
  panic_cond(r == result, "hashcode of %s should be %d instead of %d", s, result, r);

  s = "aa";
  r = hashcode(s);
  result = 31*s[0]+s[1];
  panic_cond(r == result, "hashcode of %s should be %d instead of %d", s, result, r);

  s = "abc";
  r = hashcode(s);
  result = 31*31*s[0]+31*s[1]+s[2];
  panic_cond(r == result, "hashcode of %s should be %d instead of %d", s, result, r);

  printf("passed\n");

  printf("      htable_put...");
  s = "abcdefghijklmnopqrstuvwxyz0123456789";
  int slen = strlen(s);
  
  for (int i = 0;  i < slen; i++) 
    htable_put(ht, s+i, i, sum_accum);

  for (int i = 0;  i < slen; i++) 
    htable_put(ht, s+i, 1, sum_accum);

  printf("passed\n");

  printf("      htable_get...");
  for (int i = 0; i < slen; i++) {
    int v = htable_get(ht, s+i);
    panic_cond(v == (i+1), "htable_get %s should return value %d instead of %d", 
        s+i, i+1, v);
  }
  printf("passed\n");

  printf("      htable_get_all_tuples...");
  kv_t *tuples = malloc(sizeof(kv_t)*(slen+1));
  assert(tuples);
  int sz = htable_get_all_tuples(ht, tuples, slen+1);
  panic_cond(sz == slen, 
      "htable_get_all_tuples should return %d tuples instead of %d", slen, sz); 
  for (int i = 0; i < slen; i++) {
    int j = 0;
    for (j = 0; j < slen; j++) {
      if (strcmp(tuples[i].key, s+j) == 0) {
        break;
      }
    }
    panic_cond(j < slen, "key %s from htable not found among those inserted", tuples[i].key);
    panic_cond(tuples[i].val == (j+1), "value associated with key %s should be %d instead of %d", 
        tuples[i].key, j+1, tuples[i].val);
  }
  printf("passed\n");


  printf("---Test htable.c Passed\n");  
}

void TestBonusWordCount()
{
  printf("---Bonus: Test parse_word.c\n");  
  char buf[5000];
  bzero(buf, 5000);
  char *s = "abcdefghijklmnopqrstuvwxyz";
  int slen = strlen(s);
  int sz = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < slen; j++) {
      strcpy(buf+sz, s+j);
      sz += strlen(s+j);
      buf[sz++] = '\n';
    }
  }
  buf[sz] = '\0';
  assert(strlen(buf)<5000); 
  
  printf("      parse_store_words...");
  htable_t *ht = htable_create(20);
  parse_n_store_words(buf, ht);

  kv_t tuples[30];
  int l = htable_get_all_tuples(ht, tuples, 30);
  panic_cond(l == slen, 
      "after parse_n_store_words, there should be %d instead of %d tuples\
      in the hash table", slen, l);
  for (int i = 0; i < slen; i++) {
    int keyi = tuples[i].key[0] - 'a';
    panic_cond(strcmp(tuples[i].key, s+keyi) == 0,
        "key %s has never been inserted", tuples[i].key);
    panic_cond((tuples[i].val)==2, "val should be 2 instead of %d",
        tuples[i].val);
  }
  printf("passed\n");
  printf("---Bonus: Test parse_word.c Passed\n");  
}

typedef void (*TestFunc)();

int main(int argc, char **argv)
{
#define NUM_TESTS 4
	TestFunc fs[NUM_TESTS]= {TestStr, TestList, TestHashTable, TestBonusWordCount};
	int which_test = 0;
	int c;
	while ((c = getopt(argc, argv, "t:")) != -1) {
		switch (c) {
			case 't':
				which_test = atoi(optarg);
				break;
    }
  }
  
  for (int i = 0; i < NUM_TESTS; i++) {
		if ((which_test == 0) || (which_test == (i+1))) {
			fs[i]();
      if (which_test > 0) 
        break;
    }
  }
}
