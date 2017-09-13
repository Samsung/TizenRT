#include "oichashmap.h"
#include <stdio.h>
#include <stdlib.h>

/* this should be prime */
#define TABLE_STARTSIZE 1021

#define ACTIVE 1

static unsigned long isPrime(unsigned long val)
{
  int i = 0, p = 0, exp = 0, a = 0;

  for (i = 9; i--;)
  {
    a = (rand() % (val-4)) + 2;
    p = 1;
    exp = val-1;
    while (exp)
    {
      if (exp & 1)
      {
        p = (p*a)%val;
      }

      a = (a*a)%val;
      exp >>= 1;
    }

    if (p != 1)
      return 0;
  }

  return 1;
}

static int findPrimeGreaterThan(int val)
{
  if (val & 1)
    val+=2;
  else
    val++;

  while (!isPrime(val))
  {
     val+=2;
  }

  return val;
}

static void rehash(struct hashmap* hm)
{
  long size = hm->size;
  hEntry* table = hm->table;

  hm->size = findPrimeGreaterThan(size<<1);
  hm->table = (hEntry*)calloc(sizeof(hEntry), hm->size);
  hm->count = 0;

  while(--size >= 0)
  {
      if (table[size].flags == ACTIVE)
        hashmapInsert(hm, table[size].data, table[size].key);
  }

  free(table);
}

struct hashmap* hashmapCreate(int startsize)
{
  struct hashmap* hm = (struct hashmap*)malloc(sizeof(struct hashmap));

  if (!startsize)
  {
    startsize = TABLE_STARTSIZE;
  }
  else
    startsize = findPrimeGreaterThan(startsize-2);

  hm->table = (hEntry*)calloc(sizeof(hEntry), startsize);
  hm->size = startsize;
  hm->count = 0;

  return hm;
}

void hashmapInsert(struct hashmap* hash, const void* data, unsigned long key)
{
  long index, i, step;

  if (hash->size <= hash->count)
  {
    rehash(hash);
  }

  do
  {
    index = key % hash->size;
    step = (key % (hash->size-2)) + 1;

    for (i = 0; i < hash->size; i++)
    {
      if (hash->table[index].flags & ACTIVE)
      {
        if (hash->table[index].key == key)
        {
          hash->table[index].data = (void*)data;
          return;
        }
      }
      else
      {
        hash->table[index].flags |= ACTIVE;
        hash->table[index].data = (void*)data;
        hash->table[index].key = key;
        ++hash->count;
        return;
      }

      index = (index + step) % hash->size;
    }

    /* it should not be possible that we EVER come this far, but unfortunately
       not every generated prime number is prime (Carmichael numbers...) */
    rehash(hash);
  }
  while (1);
}

void* hashmapRemove(struct hashmap* hash, unsigned long key)
{
  long index, i, step;

  index = key % hash->size;
  step = (key % (hash->size-2)) + 1;

  for (i = 0; i < hash->size; i++)
  {
    if (hash->table[index].data)
    {
      if (hash->table[index].key == key)
      {
        if (hash->table[index].flags & ACTIVE)
        {
          hash->table[index].flags &= ~ACTIVE;
          --hash->count;
          return hash->table[index].data;
        }
        else /* in, but not active (i.e. deleted) */
          return 0;
      }
    }
    else /* found an empty place (can't be in) */
    {
      return 0;
    }

    index = (index + step) % hash->size;
  }
  /* everything searched through, but not in */
  return 0;
}

void* hashmapGet(struct hashmap* hash, unsigned long key)
{
  if (hash->count)
  {
    long index = 0 , i = 0 , step = 0;
    index = key % hash->size;
    step = (key % (hash->size-2)) + 1;

    for (i = 0; i < hash->size; i++)
    {
      if (hash->table[index].key == key)
      {
        if (hash->table[index].flags & ACTIVE)
        {
          return hash->table[index].data;
        }
        break;
      }
      else
        if (!hash->table[index].data)
          break;

      index = (index + step) % hash->size;
    }
  }

  return 0;
}

long hashmapCount(struct hashmap* hash)
{
  return hash->count;
}

void hashmapDelete(struct hashmap* hash)
{
  if(hash != NULL)
    free(hash->table);
  free(hash);
}

/*
 * Reference : http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long hashmapGetHashVal(unsigned char *str)
{
    unsigned long hash = 5381;
    int c = 0;

    while (c = *str++)
    {
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}