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

#ifndef __INCLUDE_TINYARA_HASHMAP_H
#define __INCLUDE_TINYARA_HASHMAP_H

/** Hashmap structure (forward declaration) */
struct h_entry_s {
	void *data;
	int flags;
	long key;
};
typedef struct h_entry_s h_entry_t;

struct hashmap_s {
	h_entry_t *table;
	long size;
	long count;
};

// struct s_hashmap;
// typedef struct s_hashmap hashmap;

/** Creates a new hashmap near the given size. */
struct hashmap_s *hashmap_create(int startsize);

/** Inserts a new element into the hashmap. */
void hashmap_insert(struct hashmap_s *hash, const void *data, unsigned long key);

/** Returns the element for the key. */
void *hashmap_get(struct hashmap_s *hash, unsigned long key);

/** Returns the number of saved elements. */
long hashmap_count(struct hashmap_s *hash);

/** Removes the hashmap structure. */
void hashmap_delete(struct hashmap_s *hash);

unsigned long hashmap_get_hashval(unsigned char *str);

unsigned long *hashmap_get_keyset(struct hashmap_s *hash);

#endif	//__INCLUDE_TINYARA_HASHMAP_H
