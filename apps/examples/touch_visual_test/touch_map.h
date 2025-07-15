/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#ifndef MAP_h
#define MAP_h

#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 15

// Structure to represent a pair_t
typedef struct {
    int first;
    int second;
} pair_t;

// Structure to represent a node_t in the linked list
typedef struct node {
    pair_t data;
    struct node* next;
} node_t;

// Structure to represent the hash table
typedef struct {
    node_t* buckets[TABLE_SIZE];
} hashtable_t;

static pair_t create_pair(int, int);

static node_t* create_node(pair_t);

static int hash_function(int);

static void insert_pair(hashtable_t*, int, pair_t);

static node_t* get_pairs(hashtable_t*, int);

static void delete_pairs(hashtable_t*, int);

static void clear_map(hashtable_t*);

static int find_pair_in_range(hashtable_t*, int, int, int);

#endif

