/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <tinyara/mm/mm.h>
#include <hashmap.h>

/* this should be prime */
#define TABLE_STARTSIZE 1021

#define ACTIVE 1

static unsigned long is_prime(unsigned long val)
{
	int i = 0;
	int p = 0;
	int exp = 0;
	int a = 0;

	for (i = 9; i--;) {
		a = (rand() % (val - 4)) + 2;
		p = 1;
		exp = val - 1;
		while (exp) {
			if (exp & 1) {
				p = (p * a) % val;
			}

			a = (a * a) % val;
			exp >>= 1;
		}

		if (p != 1) {
			return 0;
		}
	}

	return 1;
}

static int find_prime_greater_than(int val)
{
	if (val & 1) {
		val += 2;
	} else {
		val++;
	}

	while (!is_prime(val)) {
		val += 2;
	}

	return val;
}

static void rehash(struct hashmap_s *hm)
{
	long size = hm->size;
	h_entry_t *table = hm->table;

	hm->size = find_prime_greater_than(size << 1);
#ifdef __KERNEL__
	hm->table = (h_entry_t *)kmm_calloc(sizeof(h_entry_t), hm->size);
#else
	hm->table = (h_entry_t *)calloc(sizeof(h_entry_t), hm->size);
#endif
	hm->count = 0;

	while (--size >= 0) {
		if (table[size].flags == ACTIVE) {
			hashmap_insert(hm, table[size].data, table[size].key);
		}
	}

#ifdef __KERNEL__
	kmm_free(table);
#else
	free(table);
#endif
}

struct hashmap_s *hashmap_create(int startsize)
{
#ifdef __KERNEL__
	struct hashmap_s *hm = (struct hashmap_s *)kmm_malloc(sizeof(struct hashmap_s));
#else
	struct hashmap_s *hm = (struct hashmap_s *)malloc(sizeof(struct hashmap_s));
#endif

	if (hm == NULL) {
		return NULL;
	}

	if (!startsize) {
		startsize = TABLE_STARTSIZE;
	} else {
		startsize = find_prime_greater_than(startsize - 2);
	}

#ifdef __KERNEL__
	hm->table = (h_entry_t *)kmm_calloc(sizeof(h_entry_t), startsize);
#else
	hm->table = (h_entry_t *)calloc(sizeof(h_entry_t), startsize);
#endif
	hm->size = startsize;
	hm->count = 0;

	return hm;
}

void hashmap_insert(struct hashmap_s *hash, const void *data, unsigned long key)
{
	long index;
	long i;
	long step;

	if (hash->size <= hash->count) {
		rehash(hash);
	}

	do {
		index = key % hash->size;
		step = (key % (hash->size - 2)) + 1;

		for (i = 0; i < hash->size; i++) {
			if (hash->table[index].flags & ACTIVE) {
				if (hash->table[index].key == key) {
					hash->table[index].data = (void *)data;
					return;
				}
			} else {
				hash->table[index].flags |= ACTIVE;
				hash->table[index].data = (void *)data;
				hash->table[index].key = key;
				++hash->count;
				return;
			}

			index = (index + step) % hash->size;
		}

		/* it should not be possible that we EVER come this far, but unfortunately
		   not every generated prime number is prime (Carmichael numbers...) */
		rehash(hash);
	} while (1);
}

void *hashmap_get(struct hashmap_s *hash, unsigned long key)
{
	if (hash->count) {
		long index = 0;
		long i = 0;
		long step = 0;

		index = key % hash->size;
		step = (key % (hash->size - 2)) + 1;

		for (i = 0; i < hash->size; i++) {
			if (hash->table[index].key == key) {
				if (hash->table[index].flags & ACTIVE) {
					return hash->table[index].data;
				}
				break;
			} else if (!hash->table[index].data) {
				break;
			}

			index = (index + step) % hash->size;
		}
	}

	return 0;
}

unsigned long* hashmap_get_keyset(struct hashmap_s *hash)
{
	unsigned long *keyset = NULL;

	if (hash->count) {
		int i = 0;
		int idx = 0;
#ifdef __KERNEL__
		keyset = (unsigned long *)kmm_malloc(sizeof(unsigned long) * hash->count);
#else
		keyset = (unsigned long *)malloc(sizeof(unsigned long) * hash->count);
#endif
		if (keyset != NULL) {
			for (i = 0; i < hash->size; i++) {
				if (hash->table[i].flags & ACTIVE) {
					keyset[idx++] = hash->table[i].key;
				}
			}
		}
	}

	return keyset;
}

long hashmap_count(struct hashmap_s *hash)
{
	return hash->count;
}

void hashmap_delete(struct hashmap_s *hash)
{
#ifdef __KERNEL__
	if (hash != NULL) {
		kmm_free(hash->table);
	}
	kmm_free(hash);
#else
	if (hash != NULL) {
		free(hash->table);
	}
	free(hash);
#endif
}

/*
 * Reference : http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long hashmap_get_hashval(unsigned char *str)
{
	unsigned long hash = 5381;
	int c = 0;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;	/* hash * 33 + c */
	}

	return hash;
}
