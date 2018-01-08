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

#ifndef _THINGS_HASHMAP_H
#define _THINGS_HASHMAP_H

/** Hashmap structure (forward declaration) */
typedef struct {
	void *data;
	int flags;
	long key;
} h_entry_s;

typedef struct hashmap_s {
	h_entry_s *table;
	long size, count;
} hashmap_s;

// struct s_hashmap;
// typedef struct s_hashmap hashmap;

/** Creates a new hashmap near the given size. */
extern struct hashmap_s *hashmap_create(int startsize);

/** Inserts a new element into the hashmap. */
extern void hashmap_insert(struct hashmap_s *, const void *data, unsigned long key);

/** Returns the element for the key. */
extern void *hashmap_get(struct hashmap_s *, unsigned long key);

/** Returns the number of saved elements. */
extern long hashmap_count(struct hashmap_s *);

/** Removes the hashmap structure. */
extern void hashmap_delete(struct hashmap_s *);

extern unsigned long hashmap_get_hashval(unsigned char *str);

#endif							//_THINGS_HASHMAP_H
