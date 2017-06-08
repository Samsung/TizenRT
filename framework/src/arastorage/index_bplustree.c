/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/*
 * Copyright (c) 2016 Samsung Electronics co. ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the Arastorage database
 *
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>

#include "result.h"
#include "db_options.h"
#include "db_debug.h"
#include "storage.h"
#include "memb.h"
#include "random.h"
#include "rw_locks.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define BRANCH_FACTOR CONFIG_BRANCH_FACTOR
#define DB_TUPLES_LIMIT CONFIG_DB_TUPLES_LIMIT
#define PG_SIZE        1*sizeof(struct key_value_pair)
#define BUCKET_SIZE      48
#define NODE_DEPTH      2
#define LEAF_NODES      pow(BRANCH_FACTOR, NODE_DEPTH)
#define EMPTY_NODE(node)        (node)->val[BRANCH_FACTOR-1] == 0
#define KEY_MAX 32768
#define ROW_XOR 0xf6U
#define NODE_STATE_VALID 1
#define NODE_STATE_LOCK 2
#define NODE_STATE_DIRTY 4
#define ROOT_NODE_PARENT 255

/* The total number of states possible of a node */
#define NODE_STATES 255
#define CONFIG_VACUUM_THRESHOLD 40

#ifdef CONFIG_ARASTORAGE_ENABLE_VACUUM
#define VACUUM_THRESHOLD CONFIG_VACUUM_THRESHOLD
#endif

#define max(a, b) ({ __typeof__(a) _a = (a);  __typeof__(b) _b = (b); _a > _b ? _a : _b; })
#define min(a, b) ({ __typeof__(a) _a = (a);  __typeof__(b) _b = (b); _a < _b ? _a : _b; })

#define SET_NODE_STATE(node, type) \
	do { \
		node->node_state |= type; \
	} while (0)

#define UNSET_NODE_STATE(node, type) \
	do { \
		node->node_state &= ((type) ^ NODE_STATES); \
	} while (0)

#define REMOVE_ENTRY(node) \
	do { \
		node->prev->next = node->next; \
		node->next->prev = node->prev; \
	} while (0)

#define PLACE_AT_HEAD(node, cache_type) \
	do { \
		cache_type->in_cache.head->next->prev = node; \
		node->next = cache_type->in_cache.head->next; \
		node->prev = cache_type->in_cache.head; \
		cache_type->in_cache.head->next = node; \
	} while (0)

#define PLACE_AT_TAIL(node, cache_type) \
	do { \
		cache_type->in_cache.tail->prev->next = node; \
		node->next = cache_type->in_cache.tail; \
		node->prev = cache_type->in_cache.tail->prev; \
		cache_type->in_cache.tail->prev = node; \
	} while (0)

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct key_value_pair_s {
	uint16_t key;
	uint16_t value;
};
typedef struct key_value_pair_s pair_t;

struct tree_node_s {
	uint16_t val[BRANCH_FACTOR];
	uint16_t id[BRANCH_FACTOR];
	uint16_t is_leaf;
};
typedef struct tree_node_s tree_node_t;

struct bucket_s {
	pair_t pairs[BUCKET_SIZE];
	uint8_t next_free_slot;
	int info[3];
};
typedef struct bucket_s bucket_t;

struct qnode_s {
	struct qnode_s *next;
	struct qnode_s *prev;
	uint8_t id;
	uint8_t pos;
	uint8_t node_state;
};

typedef struct qnode_s qnode_t;
struct queue_s {
	qnode_t *head;
	qnode_t *tail;
};
typedef struct queue_s queue_t;

/* A Bucket Cache Entry */
struct bucket_cache_s {
	bucket_t bucket;
};

/* A Tree Cache Entry */
struct tree_cache_s {
	tree_node_t node;
};

/* Bucket Cache Structure */
typedef struct {
	struct bucket_cache_s cache_t[DB_HEAP_CACHE_LIMIT];
	queue_t in_cache;
	uint8_t num;
} bucket_cache_t;

/* Tree Cache Structure */
typedef struct {
	struct tree_cache_s cache_t[DB_TREE_CACHE_LIMIT];
	queue_t in_cache;
	uint8_t num;
} tree_cache_t;

typedef enum {
	NODE = 0,
	BUCKET = 1
} cache_type_t;

typedef enum {
	INVALIDATE = 0,
	UNLOCK = 1,
	DIRTY = 2
} op_type_t;

enum tsplit_status_e {
	TSPLIT_ERROR = -1,
	TSPLIT_FAIL,
	TSPLIT_OK
};

typedef enum tsplit_status_e tsplit_status_t;

enum bsplit_status_e {
	BSPLIT_RETRY = -1,
	BSPLIT_FAIL,
	BSPLIT_OK
};

enum cache_result_e {
	CACHE_FULL = 1,
	CACHE_OK = 0,
	CACHE_NOT_EXIST = -1,
	CACHE_ALLOCATION_ERROR = -2,
	CACHE_BUCKET_FULL = -3,
	CACHE_UNLOCKED = -4
};

enum tree_result_e {
	TREE_LOCK_ERROR = -3,
	TREE_READ_FAIL = -2,
	TREE_INSERT_FAIL = -1,
	TREE_OK = 0
};
typedef enum bsplit_status_e bsplit_status_t;
typedef enum cache_result_e cache_result_t;
typedef enum tree_result_e tree_result_t;

/* Tree Metadata maintained in RAM */
struct tree_s {
	db_storage_id_t tree_storage;	/* The fd to tree storage file */
	db_storage_id_t bucket_storage;	/* The fd to bucket storage file */
	uint8_t off_nodes, off_buckets;	/*  Maintaining number of nodes and buckets used by the index structure */
	uint8_t root;				/*   The node id of the root of the bplus-tree */
	uint8_t lock_buckets[CONFIG_BUCKETS_LIMIT];	/* The structure to prevent to tasks to simultaneously edit same buckets  */
	uint16_t inserted;			/*  Count of total number of tuples inserted  */
	uint16_t deleted;			/*    Count of total number of tuples deleted  */
	uint8_t levels;				/*  The depth of the bplus-tree including the buckets  */
	tree_cache_t *node_cache;	/*  Structure to maintain node cache  */
	bucket_cache_t *buck_cache;	/*   Structure to maintain bucket cache  */
	pthread_mutex_t node_cache_lock;	/*  Maintains concurrency control over Node Cache  */
	pthread_mutex_t buck_cache_lock;	/*  Maintains concurrency control over Bucket Cache  */
	pthread_mutex_t bucket_lock;	/*  Maintains serialisability over in RAM Tree Structure  */
	struct rw_lock_s tree_lock;	/*  A Reader Writer Lock used to maintain consistency in tree structure */
};
typedef struct tree_s tree_t;

/****************************************************************************
 * Private variables
 ****************************************************************************/
static int base_offset = 0;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int transform_key(int);
static tree_node_t *tree_read(tree_t *, int);
static int tree_write(tree_t *, int, tree_node_t *);
static tree_result_t tree_insert(tree_t *, int);
static pair_t *tree_find(tree_t *, int key);
tree_result_t insert_item_btree(tree_t *, int, int);

static bucket_t *bucket_read(tree_t *, int);
static int bucket_write(tree_t *, int, bucket_t *);
static bsplit_status_t bucket_split(tree_t *, int, int, pair_t *);
static cache_result_t cache_bucket_append(tree_t *, int, pair_t *);
static cache_result_t cache_write_bucket(tree_t *, int, bucket_t *);

static cache_result_t modify_cache(tree_t *, int, cache_type_t, op_type_t);
static cache_result_t cache_write_node(tree_t *, int, tree_node_t *);
static cache_result_t cache_replace_node(tree_t *, int, tree_node_t *);

static db_result_t create(index_t *);
static db_result_t destroy(index_t *);
static db_result_t load(index_t *);
static db_result_t release(index_t *);
static db_result_t insert(index_t *, attribute_value_t *, tuple_id_t);
static db_result_t delete(index_t *, attribute_value_t *);
static tuple_id_t get_next(index_iterator_t *, uint8_t);
static db_result_t vacuum(tree_t *, relation_t *);

/****************************************************************************
* Private Functions
****************************************************************************/
#ifdef CONFIG_ARASTORAGE_ENABLE_FLUSHING
static int db_flush(tree_t *, relation_t *);
#endif

index_api_t index_bplustree = {
	INDEX_BPLUSTREE,
	INDEX_API_EXTERNAL | INDEX_API_RANGE_QUERIES,
	create,
	destroy,
	load,
	release,
	insert,
	delete,
	get_next
};

/****************************************************************************
 * Name: compare
 *
 * Description: Comparator to sort the bucket entries before splitting the bucket
 *
 ****************************************************************************/
int compare(const void *p1, const void *p2)
{
	if (((pair_t *)p1)->key <= ((pair_t *)p2)->key) {
		return -1;
	}
	return 1;
}

/****************************************************************************
 * Name: create
 *
 * Description: The function initialises all the structures required for the
 *              bplus-tree both in memory and on flash.
 *              The tree_filename, bucket_filename and tree structure are
 *              saved the flash and also in the index_t structure in RAM,
 *              The cache for both node and buckets is present in RAM and
 *              is de-allocated when the index exits in the release function
 *
 ****************************************************************************/
static db_result_t create(index_t *index)
{
	/* Files storing tree and bucket data */
	char tree_filename[DB_MAX_FILENAME_LENGTH];
	char bucket_filename[DB_MAX_FILENAME_LENGTH];
	tree_node_t tree_node;
	bucket_t buck;
	int offset = 0;
	db_result_t result;
	uint8_t success = 0;
	int curtime;

	curtime = time(NULL);
	random_init(curtime);
	tree_t *tree = malloc(sizeof(tree_t));
	if (tree == NULL) {
		DB_LOG_E("DB: Failed to allocate a tree\n");
		result = DB_ALLOCATION_ERROR;
		return result;

	}

	/* Generating the file to store the tree structure */
	snprintf(tree_filename, HEAP_FILE_LENGTH, "%s.%x\0", HEAP_FILE_NAME, (unsigned)(random_rand() & 0xffff));

	result = storage_generate_file(tree_filename);
	if (result == DB_INDEX_ERROR) {
		DB_LOG_E("DB: Failed to generate a tree file\n");
		free(tree);
		return result;
	}

	memcpy(index->descriptor_file, tree_filename, sizeof(index->descriptor_file));
	DB_LOG_D("DB: Generated the tree file \"%s\" using %lu bytes of space\n", index->descriptor_file, (unsigned long)CONFIG_NODE_LIMIT * sizeof(tree_node_t));

	/* Generating bucket file to store <key, tuple_id> pair */
	snprintf(bucket_filename, BUCKET_FILE_LENGTH, "%s.%x\0", BUCKET_FILE_NAME, (unsigned)(random_rand() & 0xffff));

	result = storage_generate_file(bucket_filename);
	if (result == DB_INDEX_ERROR) {
		DB_LOG_E("DB: Failed to generate a bucket file\n");
		storage_remove(tree_filename);
		free(tree);
		return result;
	}
	DB_LOG_D("DB: Generated the bucket file \"%s\" using %lu bytes of space\n", bucket_filename, (unsigned long)CONFIG_BUCKETS_LIMIT * sizeof(bucket_t));

	/* Initialising both tree and bucket storage files */
	tree->tree_storage = storage_open(tree_filename, O_RDWR);
	if (tree->tree_storage < 0) {
		result = DB_STORAGE_ERROR;
		storage_remove(tree_filename);
		storage_remove(bucket_filename);
		free(tree);
		return result;
	}
	tree->bucket_storage = storage_open(bucket_filename, O_RDWR);
	if (tree->bucket_storage < 0) {
		result = DB_STORAGE_ERROR;
		storage_close(tree->tree_storage);
		storage_remove(tree_filename);
		storage_remove(bucket_filename);
		free(tree);
		return result;

	}
	storage_write_to(tree->tree_storage, tree, offset, sizeof(tree_t));
	offset += sizeof(tree_t);
	storage_write_to(tree->tree_storage, bucket_filename, offset, sizeof(bucket_filename));
	offset += sizeof(bucket_filename);
	base_offset = offset;
	storage_write_to(tree->tree_storage, &tree_node, offset, sizeof(tree_node_t));
	buck.next_free_slot = 0;
	buck.info[0] = CONFIG_BUCKETS_LIMIT - 1;
	buck.info[1] = KEY_MAX;
	buck.info[2] = 0;
	storage_write_to(tree->bucket_storage, &buck, 0, sizeof(bucket_t));

	/* One is the root node and one is the bucket layer */
	tree->levels = 2;

	index->opaque_data = tree;
	/* Initialize the tree metadata. */
	memset(&tree->lock_buckets, 0, sizeof(tree->lock_buckets));

	/* Allocating node cache and initialising it */
	tree->node_cache = malloc(sizeof(tree_cache_t));
	if (tree->node_cache != NULL) {
		success |= 1;
		qnode_t *head = (qnode_t *)malloc(sizeof(qnode_t));
		if (head != NULL) {
			success |= 2;
			tree->node_cache->in_cache.head = head;
			qnode_t *end = (qnode_t *)malloc(sizeof(qnode_t));
			if (end != NULL) {
				success |= 4;
				head->prev = NULL;
				end->next = NULL;
				head->next = end;
				end->prev = head;
				tree->node_cache->in_cache.tail = end;
				tree->node_cache->num = 0;
			}
		}
	}
	if (success != 7) {
		DB_LOG_E("FAILED TO ALLOCATE NODE CACHE\n");
		result = DB_ALLOCATION_ERROR;
		storage_remove(tree_filename);
		storage_remove(bucket_filename);
		if (success & 1) {
			if (success & 2) {
				free(tree->node_cache->in_cache.head);
			}
			free(tree->node_cache);
		}
		free(tree);
		return result;
	}

	/* Allocating bucket cache and initialising it */
	tree->buck_cache = malloc(sizeof(bucket_cache_t));
	success = 0;
	if (tree->buck_cache != NULL) {
		success |= 1;
		qnode_t *head = (qnode_t *)malloc(sizeof(qnode_t));
		if (head != NULL) {
			success |= 2;
			tree->buck_cache->in_cache.head = head;
			qnode_t *end = (qnode_t *)malloc(sizeof(qnode_t));
			if (end != NULL) {
				success |= 4;
				head->prev = NULL;
				end->next = NULL;
				head->next = end;
				end->prev = head;
				tree->buck_cache->in_cache.tail = end;
				tree->buck_cache->num = 0;
			}
		}
	}
	if (success != 7) {
		DB_LOG_E("FAILED TO ALLOCATE BUCKET CACHE\n");
		result = DB_ALLOCATION_ERROR;
		free(tree->node_cache->in_cache.tail);
		free(tree->node_cache->in_cache.head);
		free(tree->node_cache);
		storage_remove(tree_filename);
		storage_remove(bucket_filename);
		if (success & 1) {
			if (success & 2) {
				free(tree->buck_cache->in_cache.head);
			}
			free(tree->buck_cache);
		}
		free(tree);
		return result;
	}

	tree->inserted = 0;
	tree->deleted = 0;

	/* Initialising Locks for concurrency control */
	pthread_mutex_init(&(tree->node_cache_lock), NULL);
	pthread_mutex_init(&(tree->bucket_lock), NULL);
	pthread_mutex_init(&(tree->buck_cache_lock), NULL);
	rw_init(&(tree->tree_lock));

	tree->off_nodes = tree->off_buckets = 0;

	/* Inserting first value to initialise the tree data structure */
	if (tree_insert(tree, KEY_MAX) != TREE_OK) {
		result = DB_STORAGE_ERROR;
		return result;
	}

	DB_LOG_D("DB: Created a bplus-tree index\n");
	result = DB_OK;
	return result;
}

static db_result_t destroy(index_t *index)
{
	db_storage_id_t fd;
	size_t r;
	char bucket_file[DB_MAX_FILENAME_LENGTH];
	if (DB_ERROR(release(index))) {
		return DB_INDEX_ERROR;
	}
	fd = storage_open(index->descriptor_file, O_RDWR);
	if (fd < 0) {
		return DB_STORAGE_ERROR;
	}
	if (DB_ERROR(storage_read_from(fd, bucket_file, sizeof(tree_t), sizeof(bucket_file)))) {
		return DB_STORAGE_ERROR;
	}
	storage_close(fd);
	r = storage_remove(bucket_file);
	if (DB_ERROR(r)) {
		return DB_STORAGE_ERROR;
	}
	return DB_OK;
}

static db_result_t load(index_t *index)
{
	tree_t *tree;
	db_storage_id_t fd;
	char bucket_file[DB_MAX_FILENAME_LENGTH];
	db_result_t result;
	uint8_t success = 0;

	index->opaque_data = tree = malloc(sizeof(tree_t));
	if (tree == NULL) {
		DB_LOG_E("DB: Failed to allocate a tree while loading\n");
		return DB_ALLOCATION_ERROR;
	}

	DB_LOG_D("load index : descriptor file name : %s\n", index->descriptor_file);
	fd = storage_open(index->descriptor_file, O_RDWR);
	if (fd < 0) {
		DB_LOG_E("Failed opening index descriptor file\n");
		goto storage_error;
	}
	if (DB_ERROR(storage_read_from(fd, bucket_file, sizeof(tree_t), sizeof(bucket_file)))) {
		DB_LOG_E("Failed reading bucket file\n");
		storage_close(fd);
		goto storage_error;
	}
	if (DB_ERROR(storage_read_from(fd, tree, 0, sizeof(tree_t)))) {
		DB_LOG_E("Failed  reading tree structure from descriptor file\n");
		storage_close(fd);
		goto storage_error;
	}
	storage_close(fd);

	tree->node_cache = malloc(sizeof(tree_cache_t));
	if (tree->node_cache != NULL) {
		success |= 1;
		qnode_t *head = (qnode_t *)malloc(sizeof(qnode_t));
		if (head != NULL) {
			success |= 2;
			tree->node_cache->in_cache.head = head;
			qnode_t *end = (qnode_t *)malloc(sizeof(qnode_t));
			if (end != NULL) {
				success |= 4;
				head->prev = NULL;
				end->next = NULL;
				head->next = end;
				end->prev = head;
				tree->node_cache->in_cache.tail = end;
				tree->node_cache->num = 0;
			}
		}
	}
	if (success != 7) {
		DB_LOG_E("FAILED TO ALLOCATE NODE CACHE\n");
		result = DB_ALLOCATION_ERROR;
		if (success & 1) {
			if (success & 2) {
				free(tree->node_cache->in_cache.head);
			}
			free(tree->node_cache);
		}
		free(tree);
		return result;
	}

	/* Allocating bucket cache and initialising it  */
	tree->buck_cache = malloc(sizeof(bucket_cache_t));
	success = 0;
	if (tree->buck_cache != NULL) {
		success |= 1;
		qnode_t *head = (qnode_t *)malloc(sizeof(qnode_t));
		if (head != NULL) {
			success |= 2;
			tree->buck_cache->in_cache.head = head;
			qnode_t *end = (qnode_t *)malloc(sizeof(qnode_t));
			if (end != NULL) {
				success |= 4;
				head->prev = NULL;
				end->next = NULL;
				head->next = end;
				end->prev = head;
				tree->buck_cache->in_cache.tail = end;
				tree->buck_cache->num = 0;
			}
		}
	}
	if (success != 7) {
		DB_LOG_E("FAILED TO ALLOCATE BUCKET CACHE\n");
		result = DB_ALLOCATION_ERROR;
		free(tree->node_cache->in_cache.tail);
		free(tree->node_cache->in_cache.head);
		free(tree->node_cache);
		if (success & 1) {
			if (success & 2) {
				free(tree->buck_cache->in_cache.head);
			}
			free(tree->buck_cache);
		}
		free(tree);
		return result;
	}

	base_offset = sizeof(tree_t) + sizeof(bucket_file);
	tree->tree_storage = storage_open(index->descriptor_file, O_RDWR);
	tree->bucket_storage = storage_open(bucket_file, O_RDWR);

	DB_LOG_D("DB: Loaded btree index from file %s and bucket file %s\n", index->descriptor_file, bucket_file);

	return DB_OK;

storage_error:
	DB_LOG_E("DB: Storage error while loading index\n");
	free(tree->node_cache->in_cache.head);
	free(tree->node_cache->in_cache.tail);
	free(tree->buck_cache->in_cache.head);
	free(tree->buck_cache->in_cache.tail);
	free(tree->buck_cache);
	free(tree->node_cache);
	free(tree);
	return DB_STORAGE_ERROR;

}

static db_result_t release(index_t *index)
{
	tree_t *tree;
	qnode_t *tmp_node;

	tree = index->opaque_data;
	if (tree == NULL) {
		return DB_ALLOCATION_ERROR;
	}
	if (tree->node_cache == NULL || tree->buck_cache == NULL) {
		return DB_ALLOCATION_ERROR;
	}
	if ((tree->node_cache->in_cache.tail == NULL) || (tree->node_cache->in_cache.head == NULL)) {
		return DB_ALLOCATION_ERROR;
	}
	if ((tree->buck_cache->in_cache.tail == NULL) || (tree->buck_cache->in_cache.head == NULL)) {
		return DB_ALLOCATION_ERROR;
	}
	storage_write_to(tree->tree_storage, tree, 0, sizeof(tree_t));

	/* Bucket Cache being flushed */
	tmp_node = tree->buck_cache->in_cache.head->next;
	free(tmp_node->prev);
	while (tmp_node != tree->buck_cache->in_cache.tail) {
		if ((tmp_node->node_state & NODE_STATE_DIRTY) && (tmp_node->node_state & NODE_STATE_VALID)) {
			bucket_write(tree, tmp_node->id, &(tree->buck_cache->cache_t[tmp_node->pos].bucket));
		}
		tmp_node = tmp_node->next;
		if (tmp_node->prev) {
			free(tmp_node->prev);
		}
	}
	free(tree->buck_cache->in_cache.tail);
	tmp_node = tree->node_cache->in_cache.head->next;
	free(tmp_node->prev);
	while (tmp_node != tree->node_cache->in_cache.tail) {
		if ((tmp_node->node_state & NODE_STATE_DIRTY) && (tmp_node->node_state & NODE_STATE_VALID)) {
			tree_write(tree, tmp_node->id, &(tree->node_cache->cache_t[tmp_node->pos].node));
		}
		tmp_node = tmp_node->next;
		if (tmp_node->prev) {
			free(tmp_node->prev);
		}
	}
	free(tree->node_cache->in_cache.tail);
	storage_close(tree->bucket_storage);
	storage_close(tree->tree_storage);

	free(tree->node_cache);
	free(tree->buck_cache);
	free(tree);
	return DB_OK;
}

/****************************************************************************
 * Name: insert
 *
 * Description: This routine is called by the antelope engine for insertion
 *              which in turns calls index insert routines to insert index
 *              entries.
 *
 ****************************************************************************/
static db_result_t insert(index_t *index, attribute_value_t *key, tuple_id_t value)
{
	tree_t *tree;
	long long_key;

	tree = (tree_t *)index->opaque_data;
	long_key = db_value_to_long(key);

#ifdef CONFIG_ARASTORAGE_ENABLE_FLUSHING
	if ((tree->inserted) >= DB_TUPLES_LIMIT) {
		db_flush(tree, index->rel);
		value = value - DB_TUPLES_LIMIT / 2;
	}
#endif
	if (insert_item_btree(tree, (int)long_key, (int)value) == TREE_INSERT_FAIL) {
		DB_LOG_E("DB: Failed to insert key %ld into a bplus-tree index\n", long_key);
		return DB_INDEX_ERROR;
	}

	/***************************************************************************************
	 *	The following code is to implement write through caching structure.
	 *	The write back cache gives better performance as compared to write through cache
	 *	and write back is preferred.
	 ***************************************************************************************/
#ifdef DB_WIP
	qnode_t *tmp_node;
	storage_write_to(tree->tree_storage, tree, 0, sizeof(tree_t));

	/* Bucket Cache being flushed */

	tmp_node = tree->buck_cache->in_cache.head->next;
	while (tmp_node != tree->buck_cache->in_cache.tail) {
		if ((tmp_node->node_state & NODE_STATE_DIRTY) && (tmp_node->node_state & NODE_STATE_VALID)) {
			bucket_write(tree, tmp_node->id, &(tree->buck_cache->cache_t[tmp_node->pos].bucket));
			UNSET_NODE_STATE(tmp_node, NODE_STATE_DIRTY);
		}
		tmp_node = tmp_node->next;
	}
	tmp_node = tree->node_cache->in_cache.head->next;
	while (tmp_node != tree->node_cache->in_cache.tail) {
		if ((tmp_node->node_state & NODE_STATE_DIRTY) && (tmp_node->node_state & NODE_STATE_VALID)) {
			tree_write(tree, tmp_node->id, &(tree->node_cache->cache_t[tmp_node->pos].node));
			UNSET_NODE_STATE(tmp_node, NODE_STATE_DIRTY);
		}
		tmp_node = tmp_node->next;
	}
#endif
	return DB_OK;
}

static db_result_t delete(index_t *index, attribute_value_t *value)
{
	return DB_INDEX_ERROR;
}

/****************************************************************************
 * Name: next_bucket
 *
 * Description: Helper function for get_next.
 *              Finds the bucket id of the bucket which is next in increasing
 *              order of keys
 *
 ****************************************************************************/
static uint16_t next_bucket(tree_t *tree, bucket_t *bucket)
{
	uint16_t next_bucket;
	next_bucket = bucket->info[0];
	if ((next_bucket == (uint16_t)-1) || (next_bucket == CONFIG_BUCKETS_LIMIT - 1)) {
		return (uint16_t)-1;
	}
	return next_bucket;
}

/****************************************************************************
 * Name: get_next
 *
 * Description: Returns the tuple id of the next valid tuple for the case of
 *              select and remove queries
 *
 ****************************************************************************/
static tuple_id_t get_next(index_iterator_t *iterator, uint8_t matched_condition)
{

	/* This cache holds a pointer to a bucket in the main cache and other
	 * information required to iterate over the bucket
	 */
	struct iteration_cache {
		index_iterator_t *index_iterator;
		uint16_t bucket_id;
		bucket_t *bucket;
		tuple_id_t found_items;
		uint8_t start;
		uint8_t end;
	};
	int i;
	static struct iteration_cache cache;
	uint16_t key_max;
	uint16_t key_min;
	tree_t *tree;
	key_min = *(int *)&iterator->min_value;
	key_max = *(int *)&iterator->max_value;
	tree = (tree_t *)iterator->index->opaque_data;

	/* To initialize the iterator_cache */
	if (iterator->next_item_no == 0) {	/* removed the condition of iterator inequality */
		if (iterator->found_items == 0) {
			rw_lock_write(&(tree->tree_lock));
			pair_t *path = tree_find(tree, key_min);
			if (path == NULL) {
				return INVALID_TUPLE;
			}
			uint16_t bucket_id = path[tree->levels].key;
			free(path);
			/* TODO
			 * Absent of non-cast return handling, should be taken care in the definition
			 */
			cache.bucket = bucket_read(tree, bucket_id);
			cache.bucket_id = bucket_id;
			cache.start = 0;
			cache.end = cache.bucket->next_free_slot;
		}
	}

	/* Iterate over the key-value pairs in the bucket and find the ones which satisfy the condition */
	for (i = cache.start; i < cache.end; i++) {
		if ((key_min <= cache.bucket->pairs[i].key) && (cache.bucket->pairs[i].key <= key_max)) {
			iterator->found_items++;
			iterator->next_item_no = iterator->found_items;

			/* matched condition is FALSE when the query is for remove tuples */
			if (matched_condition == FALSE) {
				tuple_id_t tmp = cache.bucket->pairs[i].value;
				if (cache.end > (i + 1)) {
					cache.bucket->pairs[i] = cache.bucket->pairs[cache.end - 1];

					/* Start Bucket chaining */
					int iter = 0;
					uint16_t new_min = cache.bucket->info[1];
					uint16_t new_max = cache.bucket->info[2];
					for (; iter < cache.bucket->next_free_slot - 1; iter++) {
						new_min = min(cache.bucket->pairs[iter].key, new_min);
						new_max = max(cache.bucket->pairs[iter].key, new_max);
					}
					cache.bucket->info[1] = new_min;
					cache.bucket->info[2] = new_max;
					/* End of Bucket chaining */
				}

				cache.bucket->next_free_slot--;
				tree->deleted++;
				cache.end--;
				cache.start = i;
				return tmp;
			} else {
				cache.start = i + 1;
			}
			return cache.bucket->pairs[i].value;
		}
	}

	/* case when delete query comes */
	if (matched_condition == FALSE) {
		modify_cache(tree, cache.bucket_id, BUCKET, INVALIDATE);
		cache_write_bucket(tree, cache.bucket_id, cache.bucket);
		if ((double)(tree->deleted) / tree->inserted >= VACUUM_THRESHOLD) {
			vacuum(tree, iterator->index->rel);
		}
	} else {
		modify_cache(tree, cache.bucket_id, BUCKET, UNLOCK);
	}
	pthread_mutex_lock(&(tree->bucket_lock));
	tree->lock_buckets[cache.bucket_id] = 0;
	cache.bucket_id = next_bucket(tree, cache.bucket);
	if (cache.bucket_id == (uint16_t)-1) {
		if (iterator->found_items == 0) {
			iterator->next_item_no = 0;
		} else {
			iterator->next_item_no = 1;
		}
		pthread_mutex_unlock(&(tree->bucket_lock));
		rw_unlock_write(&(tree->tree_lock));
		return INVALID_TUPLE;
	}
	while (tree->lock_buckets[cache.bucket_id] == 1) {
		pthread_mutex_unlock(&(tree->bucket_lock));
		DB_LOG_D("BUCKET ALREADY LOCKED IN GET NEXT SPINNING\n");
		pthread_mutex_lock(&(tree->bucket_lock));
	}
	tree->lock_buckets[cache.bucket_id] = 1;
	pthread_mutex_unlock(&(tree->bucket_lock));

	cache.start = 0;
	cache.end = cache.bucket->next_free_slot;

	/* TODO
	 * Absent of non-cast return handling, should be taken care in the definition
	 */
	cache.bucket = bucket_read(tree, cache.bucket_id);
	if (cache.bucket->info[1] > key_max) {
		modify_cache(tree, cache.bucket_id, BUCKET, UNLOCK);
		if (iterator->found_items == 0) {
			iterator->next_item_no = 0;
		} else {
			iterator->next_item_no = 1;
		}
		pthread_mutex_unlock(&(tree->bucket_lock));
		rw_unlock_write(&(tree->tree_lock));
		return INVALID_TUPLE;

	}
	iterator->next_item_no = 1;
	return get_next(iterator, matched_condition);
}

/****************************************************************************
 * Name: vacuum
 *
 * Description: Reclaims the space of all the deleted tuples.
 *              Creates a new tuple storage file with all the valid tuples
 *              and removes the old one.
 *
 ****************************************************************************/
static db_result_t vacuum(tree_t *tree, relation_t *rel)
{
	char tuple_path[TUPLE_NAME_LENGTH];
	db_result_t result;
	relation_t old_rel;
	memcpy(&old_rel, rel, sizeof(relation_t));
	unsigned long long offset;
	int fd;
	int id;
	int num_tuples = 0;

	offset = 0;

	fd = storage_open(rel->name, O_WRONLY);
	result = storage_write_to(fd, rel->name, offset, sizeof(rel->name));
	if (result != DB_OK) {
		storage_close(fd);
		storage_remove(rel->name);
		return DB_STORAGE_ERROR;
	}
	offset += sizeof(rel->name);

	/* Generate new tuple file */
	snprintf(tuple_path, TUPLE_NAME_LENGTH, "%s.%x\0", TUPLE_FILE_NAME, (unsigned)(random_rand() & 0xffff));
	result = storage_generate_file(tuple_path);
	if (result == DB_STORAGE_ERROR) {
		storage_close(fd);
		storage_remove(rel->tuple_filename);
		return DB_STORAGE_ERROR;
	}
	strncpy(rel->tuple_filename, tuple_path, TUPLE_NAME_LENGTH);

	result = storage_write_to(fd, rel->tuple_filename, offset, sizeof(rel->tuple_filename));

	storage_close(fd);
	if (result != DB_OK) {
		storage_remove(rel->tuple_filename);
		return DB_STORAGE_ERROR;
	}

	/* Set the fields of the relation structure */
	rel->tuple_storage = storage_open(rel->tuple_filename, O_RDWR);

	rel->next_row = num_tuples;
	rel->cardinality = num_tuples;
	storage_row_t temp = malloc(sizeof(rel->row_length));
	if (temp == NULL) {
		return DB_ALLOCATION_ERROR;
	}

	/* Iterate over all the buckets to read the tuples from storage and put them in new tuple file */
	for (id = 0; id < tree->off_buckets; id++) {
		bucket_t *bucket = bucket_read(tree, id);
		int num;
		for (num = 0; num < bucket->next_free_slot; num++) {
			tuple_id_t tup;
			tup = bucket->pairs[num].value;

			storage_get_row(&old_rel, &tup, temp);
			storage_put_row(rel, temp, FALSE);
			bucket->pairs[num].value = num_tuples;
			num_tuples++;
		}
		modify_cache(tree, id, BUCKET, DIRTY);
		modify_cache(tree, id, BUCKET, UNLOCK);
	}
	free(temp);
	tree->inserted -= tree->deleted;
	tree->deleted = 0;

	storage_remove(old_rel.tuple_filename);
	return DB_OK;
}

/****************************************************************************
 * Name: modify_cache
 *
 * Description: Modifying the cache entries to mark the entry dirty,
 *              invalid or unlocking it
 *
 ****************************************************************************/
static cache_result_t modify_cache(tree_t *tree, int id, cache_type_t cache, op_type_t op)
{
	qnode_t *temp;
	qnode_t *end;
	if (cache == NODE) {
		pthread_mutex_lock(&(tree->node_cache_lock));
		temp = tree->node_cache->in_cache.tail->prev;
		end = tree->node_cache->in_cache.head;
	} else {
		pthread_mutex_lock(&(tree->buck_cache_lock));
		temp = tree->buck_cache->in_cache.tail->prev;
		end = tree->buck_cache->in_cache.head;
	}
	while (temp != end) {
		if ((temp->id == id) && (temp->node_state & NODE_STATE_VALID)) {
			if (op == UNLOCK) {
				UNSET_NODE_STATE(temp, NODE_STATE_LOCK);
			} else if (op == DIRTY) {
				SET_NODE_STATE(temp, NODE_STATE_DIRTY);
			} else {
				UNSET_NODE_STATE(temp, NODE_STATE_VALID | NODE_STATE_DIRTY | NODE_STATE_LOCK);
				REMOVE_ENTRY(temp);
				if (cache == NODE) {
					PLACE_AT_HEAD(temp, tree->node_cache);
				} else {
					PLACE_AT_HEAD(temp, tree->buck_cache);
				}
			}
			break;
		}
		temp = temp->prev;
	}
	if (temp == end) {
		DB_LOG_E("PANIC CACHE OPERATION FOR A NON EXISTENT ENTRY\n");
		return CACHE_NOT_EXIST;
	}
	if (cache == NODE) {
		pthread_mutex_unlock(&(tree->node_cache_lock));
	} else {
		pthread_mutex_unlock(&(tree->buck_cache_lock));
	}
	return CACHE_OK;
}

/****************************************************************************
 * Name: cache_write_node
 *
 * Description: Routine enabling to put a new cache entry in Node Cache.
 *              Required when new nodes are generated resulting from splits
 *
 ****************************************************************************/
static cache_result_t cache_write_node(tree_t *tree, int id, tree_node_t *node)
{
	pthread_mutex_lock(&(tree->node_cache_lock));

	qnode_t *new_node = (qnode_t *)malloc(sizeof(qnode_t));
	if (new_node == NULL) {
		DB_LOG_E("Failed allocation of node\n");
		pthread_mutex_unlock(&(tree->node_cache_lock));
		return CACHE_ALLOCATION_ERROR;
	}

	if (tree->node_cache->num < DB_TREE_CACHE_LIMIT) {
		new_node->pos = tree->node_cache->num++;
	} else {
		qnode_t *iter_node;
		iter_node = tree->node_cache->in_cache.head->next;
		while ((iter_node->node_state | NODE_STATE_LOCK) && iter_node != tree->node_cache->in_cache.tail) {
			iter_node = iter_node->next;
		}
		if (iter_node == tree->node_cache->in_cache.tail) {
			free(new_node);
			DB_LOG_E("NO SLOT AVAIABLE IN CACHE\n");
			pthread_mutex_unlock(&(tree->node_cache_lock));
			return CACHE_FULL;
		}
		if ((iter_node->node_state & NODE_STATE_DIRTY) && (iter_node->node_state & NODE_STATE_DIRTY)) {
			tree_write(tree, iter_node->id, &(tree->node_cache->cache_t[iter_node->pos].node));
		}
		new_node->pos = iter_node->pos;
		/* iter_node is the node which has to be evicted from cache.
		 * It is removed the queue maintaining the LRU status.
		 */
		REMOVE_ENTRY(iter_node);
		free(iter_node);
	}

	PLACE_AT_TAIL(new_node, tree->node_cache);
	new_node->id = id;
	UNSET_NODE_STATE(new_node, NODE_STATE_LOCK);
	SET_NODE_STATE(new_node, NODE_STATE_VALID);
	SET_NODE_STATE(new_node, NODE_STATE_DIRTY);

	memcpy(&(tree->node_cache->cache_t[new_node->pos].node), node, sizeof(tree_node_t));

	pthread_mutex_unlock(&(tree->node_cache_lock));

	return CACHE_OK;
}

/****************************************************************************
 * Name: cache_replace_node
 *
 * Description: Routine to replace cache entry.
 *              Required when a node needs to be rewritten
 *
 ****************************************************************************/
static cache_result_t cache_replace_node(tree_t *tree, int id, tree_node_t *node)
{
	pthread_mutex_lock(&(tree->node_cache_lock));

	qnode_t *replace_node;
	replace_node = tree->node_cache->in_cache.head->next;

	while ((replace_node->id != id) && (replace_node != tree->node_cache->in_cache.tail)) {
		replace_node = replace_node->next;
	}
	if (replace_node == tree->node_cache->in_cache.tail || !(replace_node->node_state & NODE_STATE_LOCK)) {
		DB_LOG_E("PANIC REPLACE FOR NON_EXISTENT OR NON_LOCKED ENTRY\n");
		pthread_mutex_unlock(&(tree->node_cache_lock));
		return CACHE_NOT_EXIST;
	}
	UNSET_NODE_STATE(replace_node, NODE_STATE_LOCK);
	SET_NODE_STATE(replace_node, NODE_STATE_VALID | NODE_STATE_DIRTY);

	memcpy(&(tree->node_cache->cache_t[replace_node->pos].node), node, sizeof(tree_node_t));

	pthread_mutex_unlock(&(tree->node_cache_lock));

	return CACHE_OK;
}

/****************************************************************************
 * Name: cache_write_bucket
 *
 * Description: Routine enabling to put a new cache entry in Bucket Cache.
 *              Required when new buckets are generated resulting from splits.
 *
 ****************************************************************************/
static cache_result_t cache_write_bucket(tree_t *tree, int id, bucket_t *bucket)
{
	pthread_mutex_lock(&(tree->buck_cache_lock));

	qnode_t *new_node = (qnode_t *)malloc(sizeof(qnode_t));
	if (new_node == NULL) {
		pthread_mutex_unlock(&(tree->buck_cache_lock));
		return CACHE_ALLOCATION_ERROR;
	}

	if (tree->buck_cache->num < DB_HEAP_CACHE_LIMIT) {
		new_node->pos = tree->buck_cache->num++;
	} else {
		qnode_t *iter_node;
		iter_node = tree->buck_cache->in_cache.head->next;
		while ((iter_node->node_state & NODE_STATE_LOCK) && (iter_node != tree->buck_cache->in_cache.tail)) {
			iter_node = iter_node->next;
		}
		if (iter_node == tree->buck_cache->in_cache.tail) {
			free(new_node);
			DB_LOG_E("NO SLOT AVAILABLE IN CACHE bucket\n");
			pthread_mutex_unlock(&(tree->buck_cache_lock));
			return CACHE_FULL;
		}

		if ((iter_node->node_state & NODE_STATE_DIRTY) && (iter_node->node_state & NODE_STATE_VALID)) {
			bucket_write(tree, iter_node->id, &(tree->buck_cache->cache_t[iter_node->pos].bucket));
		}
		new_node->pos = iter_node->pos;
		REMOVE_ENTRY(iter_node);
		free(iter_node);
	}
	PLACE_AT_TAIL(new_node, tree->buck_cache);
	new_node->id = id;
	UNSET_NODE_STATE(new_node, NODE_STATE_LOCK);
	SET_NODE_STATE(new_node, NODE_STATE_DIRTY | NODE_STATE_VALID);

	memcpy(&(tree->buck_cache->cache_t[new_node->pos].bucket), bucket, sizeof(bucket_t));

	pthread_mutex_unlock(&(tree->buck_cache_lock));

	return CACHE_OK;
}

/****************************************************************************
 * Name: transform_key
 *
 * Description: Routine to tranform key to a type acceptable by index.
 *              Different data types of keys can be supported.
 *              some codes will be added in future.
 *
 ****************************************************************************/
static int transform_key(int key)
{
	return key;
}

/****************************************************************************
 * Name: tree_read
 *
 * Description: Fetches nodes from the flash and suitably evicts the nodes
 *              from cache, in case the cache is full
 *
 ****************************************************************************/
static tree_node_t *tree_read(tree_t *tree, int bucket_id)
{
	pthread_mutex_lock(&(tree->node_cache_lock));

	bool found = false;
	qnode_t *iter = (tree->node_cache->in_cache.tail)->prev;

	while (iter != (tree->node_cache->in_cache.head)) {
		if (bucket_id == iter->id && (iter->node_state & NODE_STATE_VALID)) {
			found = true;
			break;
		}
		iter = iter->prev;
	}
	if (found) {
		/* Case when node is found in the cache */
		if (iter->node_state & NODE_STATE_LOCK) {
			pthread_mutex_unlock(&(tree->node_cache_lock));
			return NULL;
		}
		SET_NODE_STATE(iter, NODE_STATE_LOCK);
		REMOVE_ENTRY(iter);
		PLACE_AT_TAIL(iter, tree->node_cache);
		pthread_mutex_unlock(&(tree->node_cache_lock));
		return &(tree->node_cache->cache_t[iter->pos].node);
	} else {
		qnode_t *new_node = (qnode_t *)malloc(sizeof(qnode_t));
		if (new_node == NULL) {
			pthread_mutex_unlock(&(tree->node_cache_lock));
			return NULL;
		}
		if (tree->node_cache->num < DB_TREE_CACHE_LIMIT) {
			new_node->pos = tree->node_cache->num++;
		} else {
			/* Case when the least recently used node needs to be evicted to make place for new node */
			qnode_t *replace_node = tree->node_cache->in_cache.head->next;
			while ((replace_node->node_state & NODE_STATE_LOCK) && replace_node != tree->node_cache->in_cache.tail) {
				replace_node = replace_node->next;
			}
			if (replace_node == tree->node_cache->in_cache.tail) {
				free(new_node);
				pthread_mutex_unlock(&(tree->node_cache_lock));
				return NULL;
			}
			if ((replace_node->node_state & NODE_STATE_DIRTY) && (replace_node->node_state & NODE_STATE_VALID)) {
				tree_write(tree, replace_node->id, &(tree->node_cache->cache_t[replace_node->pos].node));
			}
			new_node->pos = replace_node->pos;
			REMOVE_ENTRY(replace_node);
			free(replace_node);
		}
		/* Adjusting the pointers */
		PLACE_AT_TAIL(new_node, tree->node_cache);
		new_node->id = bucket_id;
		SET_NODE_STATE(new_node, NODE_STATE_LOCK | NODE_STATE_VALID | NODE_STATE_DIRTY);

		/* Reading from flash */
		if (DB_ERROR(storage_read_from(tree->tree_storage, &(tree->node_cache->cache_t[new_node->pos].node), base_offset + (unsigned long)bucket_id * sizeof(tree_node_t), sizeof(tree_node_t)))) {
			DB_LOG_E("PANIC TREE READ FAILED AT NODE ID %d\n", new_node->id);
			UNSET_NODE_STATE(new_node, NODE_STATE_LOCK | NODE_STATE_VALID);
			pthread_mutex_unlock(&(tree->node_cache_lock));
			return NULL;
		}

		pthread_mutex_unlock(&(tree->node_cache_lock));

		return &(tree->node_cache->cache_t[new_node->pos].node);
	}
}

/****************************************************************************
 * Name: tree_write
 *
 * Description: Routine to issue a flash write of node in the tree.
 *              Required while evicting dirty cache entries
 *
 ****************************************************************************/
static int tree_write(tree_t *tree, int offset, tree_node_t *node)
{
	if (DB_ERROR(storage_write_to(tree->tree_storage, node, base_offset + (unsigned long)offset * sizeof(*node), sizeof(*node)))) {
		DB_LOG_E("TREE WRITE FAILED AT NODE ID %d\n", offset);
		return 0;
	}
	return 1;
}

/****************************************************************************
 * Name: bucket_write
 *
 * Description: Routine to issue a flash write of bucket in the tree.
 *              Required while evicting dirty cache entries
 *
 ****************************************************************************/
static int bucket_write(tree_t *tree, int pos, bucket_t *bucket)
{
	if (DB_ERROR(storage_write_to(tree->bucket_storage, bucket, (unsigned long)pos * sizeof(bucket_t), sizeof(bucket_t)))) {
		DB_LOG_E("BUCKET WRITE FAILED AT BUCKET ID %d\n", pos);
		return 0;
	}
	return 1;
}

/****************************************************************************
 * Name: tree_insert
 *
 * Description: Initialises the bplus tree by putting a dummy entry with
 *              value max(uint16_t) and two buckets
 *
 ****************************************************************************/
static tree_result_t tree_insert(tree_t *tree, int max)
{
	int i = tree->off_nodes;
	tree_node_t *node;

	/* TODO
	 * Absent of noncast return handling, should be taken care in the definition
	 */
	node = tree_read(tree, i);
	if (node == NULL) {
		return TREE_READ_FAIL;
	}
	node->val[0] = max;
	node->val[BRANCH_FACTOR - 1] = 1;
	node->id[0] = tree->off_buckets++;
	node->id[1] = CONFIG_BUCKETS_LIMIT - 1;
	node->is_leaf = 1;
	tree->root = i;
	tree->off_nodes++;

	modify_cache(tree, tree->root, NODE, DIRTY);
	modify_cache(tree, tree->root, NODE, UNLOCK);

	return TREE_OK;
}

/****************************************************************************
 * Name: tree_find
 *
 * Description: Traverses the bplus tree to find the appropriate bucket
 *              for an insertion
 *
 ****************************************************************************/
static pair_t *tree_find(tree_t *tree, int key)
{
	int hashed_key;
	uint8_t id;
	tree_node_t *node;
	int index;
	hashed_key = transform_key(key);
	bool iset;
	int j;
	pair_t *path = malloc(sizeof(pair_t) * ((tree->levels) + 1));
	if (path == NULL) {
		return NULL;
	}
	uint8_t tree_level = 1;
	path[0].key = ROOT_NODE_PARENT;
	path[0].value = 0;
	id = tree->root;
	/* Keep traversing down the tree until leaf node is reached */
	while (true) {
		/* TODO
		 * Absent of noncast return handling, should be taken care in the definition
		 */
		node = tree_read(tree, id);
		if (node == NULL) {
			return NULL;
		}
		index = id;
		iset = false;
		/* If leaf is found iterate over the keys and find the appropriate bucket */
		for (j = 0; j < node->val[BRANCH_FACTOR - 1]; j++) {
			if (node->val[j] >= hashed_key) {
				iset = true;
				break;
			}
		}
		if (node->is_leaf) {
			if (iset) {
				index = j;
			} else {
				index = node->val[BRANCH_FACTOR - 1];
			}

			/* Bucket id should be returned */
			pthread_mutex_lock(&(tree->bucket_lock));
			if (tree->lock_buckets[node->id[index]]) {
				pthread_mutex_unlock(&(tree->bucket_lock));
				modify_cache(tree, id, NODE, UNLOCK);
				return NULL;
			} else {
				tree->lock_buckets[node->id[index]] = 1;
				pthread_mutex_unlock(&(tree->bucket_lock));
			}
			modify_cache(tree, id, NODE, UNLOCK);
			path[tree_level].key = id;
			path[tree_level].value = index;
			tree_level++;
			path[tree_level].key = node->id[index];
			return path;
		} else {
			path[tree_level].key = id;
			if (iset) {
				id = node->id[j];
				path[tree_level].value = j;
			} else {
				id = node->id[node->val[BRANCH_FACTOR - 1]];
				path[tree_level].value = node->val[BRANCH_FACTOR - 1];
			}
			tree_level++;
			modify_cache(tree, index, NODE, UNLOCK);
		}
	}
	return NULL;
}

/****************************************************************************
 * Name: tree_print
 *
 * Description: Debugging Function to print the whole tree structure
 *
 ****************************************************************************/
void tree_print(tree_t *tree, int id)
{
	tree_node_t *node;
	bucket_t *bucket;
	int i, j;
	node = tree_read(tree, id);
	DB_LOG_D("Node %d:", id);
	if (node->is_leaf) {
		for (i = 0; i < node->val[BRANCH_FACTOR - 1]; i++) {
			DB_LOG_V(" Key: %d\n", node->val[i]);
			bucket = bucket_read(tree, node->id[i]);
			DB_LOG_V("Bucket id:%d\n", node->id[i]);
			for (j = 0; j < bucket->next_free_slot; j++) {
				DB_LOG_V("Key %d, Value %d\n", bucket->pairs[j].key, bucket->pairs[j].value);
			}
			modify_cache(tree, node->id[i], BUCKET, UNLOCK);
		}
		bucket = bucket_read(tree, node->id[node->val[BRANCH_FACTOR - 1]]);
		DB_LOG_D("Bucket id:%d\n", node->id[node->val[BRANCH_FACTOR - 1]]);
		for (j = 0; j < bucket->next_free_slot; j++) {
			DB_LOG_V("Key %d, Value %d\n", bucket->pairs[j].key, bucket->pairs[j].value);
		}
		modify_cache(tree, node->id[node->val[BRANCH_FACTOR - 1]], BUCKET, UNLOCK);

	} else {
		for (i = 0; i < node->val[BRANCH_FACTOR - 1]; i++) {
			DB_LOG_V("Key: %d\n", node->val[i]);
			tree_print(tree, node->id[i]);
		}
		tree_print(tree, node->id[node->val[BRANCH_FACTOR - 1]]);
	}
	modify_cache(tree, id, NODE, UNLOCK);
	DB_LOG_D("Node End\n");
}

/****************************************************************************
 * Name: bucket_read
 *
 * Description: Reads buckets from the cache and fetches them from flash
 *              when the entry does not exist in cache.
 *              Also evicts the suitable entries from cache when cache is full
 *
 ****************************************************************************/
static bucket_t *bucket_read(tree_t *tree, int bucket_id)
{
	pthread_mutex_lock(&(tree->buck_cache_lock));

	bool found = false;
	qnode_t *iter = (tree->buck_cache->in_cache.tail)->prev;

	while (iter != (tree->buck_cache->in_cache.head)) {
		if (bucket_id == iter->id && (iter->node_state & NODE_STATE_VALID)) {
			found = true;
			break;
		}
		iter = iter->prev;
	}
	if (found) {
		/* If the bucket is found in the cache */
		if (iter->node_state & NODE_STATE_LOCK) {
			pthread_mutex_unlock(&(tree->buck_cache_lock));
			return NULL;
		}
		SET_NODE_STATE(iter, NODE_STATE_LOCK);
		REMOVE_ENTRY(iter);
		PLACE_AT_TAIL(iter, tree->buck_cache);
		pthread_mutex_unlock(&(tree->buck_cache_lock));
		return &(tree->buck_cache->cache_t[iter->pos].bucket);
	} else {
		/* Bucket has to be read from flash into the cache */
		qnode_t *new_node = (qnode_t *)malloc(sizeof(qnode_t));
		if (new_node == NULL) {
			pthread_mutex_unlock(&(tree->buck_cache_lock));
			return NULL;
		}
		if (tree->buck_cache->num < DB_HEAP_CACHE_LIMIT) {
			new_node->pos = tree->buck_cache->num++;
		} else {
			/* Cache doesn't have enough space and the least recently used bucket needs to be evicted */
			qnode_t *replace_node = tree->buck_cache->in_cache.head->next;
			while ((replace_node->node_state & NODE_STATE_LOCK) && (replace_node != tree->buck_cache->in_cache.tail)) {
				replace_node = replace_node->next;
			}
			if (replace_node == tree->buck_cache->in_cache.tail) {
				free(new_node);
				pthread_mutex_unlock(&(tree->buck_cache_lock));
				return NULL;
			}
			if ((replace_node->node_state & NODE_STATE_DIRTY) && (replace_node->node_state & NODE_STATE_VALID)) {
				bucket_write(tree, replace_node->id, &(tree->buck_cache->cache_t[replace_node->pos].bucket));
			}
			new_node->pos = replace_node->pos;
			REMOVE_ENTRY(replace_node);
			free(replace_node);
		}

		/* Adjust the pointers */
		PLACE_AT_TAIL(new_node, tree->buck_cache);
		new_node->id = bucket_id;
		SET_NODE_STATE(new_node, NODE_STATE_LOCK | NODE_STATE_VALID);
		UNSET_NODE_STATE(new_node, NODE_STATE_DIRTY);

		/* Read from flash */
		if (DB_ERROR(storage_read_from(tree->bucket_storage, (void *)&(tree->buck_cache->cache_t[new_node->pos].bucket), (unsigned long)bucket_id * sizeof(bucket_t), sizeof(bucket_t)))) {
			DB_LOG_E("PANIC BUCKET READ FAILED AT ID %d\n", bucket_id);
			UNSET_NODE_STATE(new_node, (NODE_STATE_LOCK | NODE_STATE_VALID));
			pthread_mutex_unlock(&(tree->buck_cache_lock));
			return NULL;
		}
		pthread_mutex_unlock(&(tree->buck_cache_lock));
		return &(tree->buck_cache->cache_t[new_node->pos].bucket);
	}
}

/****************************************************************************
 * Name: cache_bucket_append
 *
 * Description: Routine to append an index entry to a bucket in cache
 *
 ****************************************************************************/
static cache_result_t cache_bucket_append(tree_t *tree, int bucket_id, pair_t *pair)
{
	bucket_t *bucket_tmp = bucket_read(tree, bucket_id);
	if (bucket_tmp == NULL) {
		return CACHE_NOT_EXIST;
	}
	if (bucket_tmp->next_free_slot >= BUCKET_SIZE) {
		modify_cache(tree, bucket_id, BUCKET, UNLOCK);
		return CACHE_BUCKET_FULL;
	}
	bucket_tmp->info[1] = min(bucket_tmp->info[1], pair->key);
	bucket_tmp->info[2] = max(bucket_tmp->info[2], pair->key);
	bucket_tmp->pairs[bucket_tmp->next_free_slot].key = pair->key;
	bucket_tmp->pairs[bucket_tmp->next_free_slot].value = pair->value;
	bucket_tmp->next_free_slot++;
	pthread_mutex_lock(&(tree->bucket_lock));
	if (tree->lock_buckets[bucket_id] == 0) {
		DB_LOG_E("PANIC EDITED BUCKET WITHOUT LOCK\n");
		pthread_mutex_unlock(&(tree->bucket_lock));
		return CACHE_UNLOCKED;
	} else {
		tree->lock_buckets[bucket_id] = 0;
		pthread_mutex_unlock(&(tree->bucket_lock));
	}
	modify_cache(tree, bucket_id, BUCKET, DIRTY);
	modify_cache(tree, bucket_id, BUCKET, UNLOCK);

	return CACHE_OK;
}

/****************************************************************************
 * Name: tree_split
 *
 * Description: A recursive function implementing the tree splitting logic
 *              which is called if the insertion fails and the tree needs to
 *              grow. The nodes are split in a top-down approach.
 *              i.e. higher nodes are split then the lower nodes are split.
 *
 ****************************************************************************/
static tsplit_status_t tree_split(tree_t *tree, int key, int id, pair_t *path, int level)
{
	if (level < 0 || level > (tree->levels - 1)) {
		DB_LOG_E("PANIC: Tree level out of bounds\n");
	}
	/* TODO
	 * It is very hard to understand n, node, n1, n2 means exactly.
	 * Please rename variable & add descriptions more in detail for each progress
	 */
	tree_node_t *n;
	tree_node_t node;
	int i, j;
	if (path[level].key == ROOT_NODE_PARENT) {
		/* Case when root has split and new root node requires to be created */
		uint8_t root = tree->root;
		uint8_t new_root = tree->off_nodes++;
		if (tree->off_nodes > CONFIG_NODE_LIMIT) {
			tree->off_nodes--;
			return TSPLIT_FAIL;
		}
		node.val[0] = key;
		node.id[0] = root;
		node.id[1] = id;
		node.val[BRANCH_FACTOR - 1] = 1;
		node.is_leaf = 0;
		while (cache_write_node(tree, new_root, &node) != CACHE_OK) ;
		tree->root = new_root;
		tree->levels++;
		return TSPLIT_OK;
	}
	/* TODO
	 * Absent of noncast return handling, should be taken care in the definition
	 */
	n = tree_read(tree, path[level].key);
	if (n == NULL) {
		DB_LOG_E("CACHE FULL:All writed locked\n");
		return TSPLIT_ERROR;
	}

	if ((n->val[BRANCH_FACTOR - 1] < BRANCH_FACTOR - 1)) {
		/* The case when the node has space for new entry */
		node.is_leaf = n->is_leaf;
		node.val[BRANCH_FACTOR - 1] = n->val[BRANCH_FACTOR - 1] + 1;
		node.id[0] = n->id[0];
		/* Insert the key at appropriate place in the node */
		j = 0;
		for (i = 0; i < node.val[BRANCH_FACTOR - 1]; i++) {
			if (i == path[level].value) {
				node.val[i] = key;
				node.id[i + 1] = id;
			} else {
				node.val[i] = n->val[j];
				node.id[i + 1] = n->id[j + 1];
				j++;
			}
		}

		/* Replace the entry for the node in the cache with the new node after inserting the key */
		if (cache_replace_node(tree, path[level].key, &node) != CACHE_OK) {
			return TSPLIT_ERROR;
		}
		return TSPLIT_OK;
	} else {
		/* Case when the node doesn't have space for new key and requires splitting */
		tree_node_t node1;
		int res;
		int nid = tree->off_nodes++;
		/* Create dummy arrays to facilitate splitting */
		int key_arr[BRANCH_FACTOR];
		int ids_arr[BRANCH_FACTOR + 1];

		if (tree->off_nodes > CONFIG_NODE_LIMIT) {
			tree->off_nodes--;
			modify_cache(tree, path[level].key, NODE, UNLOCK);
			return TSPLIT_FAIL;
		}

		ids_arr[0] = n->id[0];
		j = 0;
		for (i = 0; i < BRANCH_FACTOR; i++) {
			if (i == path[level].value) {
				key_arr[i] = key;
				ids_arr[i + 1] = id;
			} else {
				key_arr[i] = n->val[j];
				ids_arr[i + 1] = n->id[j + 1];
				j++;
			}
		}
		/* Call tree_split to insert the median value(BRANCH_FACTOR/2) in the upper level node(level-1) */
		res = tree_split(tree, key_arr[BRANCH_FACTOR / 2], nid, path, level - 1);
		if (res == TSPLIT_OK) {
			node.val[BRANCH_FACTOR - 1] = BRANCH_FACTOR / 2;
			node1.val[BRANCH_FACTOR - 1] = (BRANCH_FACTOR - 1) / 2;
			node.is_leaf = n->is_leaf;
			node1.is_leaf = n->is_leaf;
			int ind = 0;
			node.id[0] = ids_arr[0];

			/* Copy left half into the same node and right half into new node */
			for (; ind < BRANCH_FACTOR / 2; ind++) {
				node.val[ind] = key_arr[ind];
				node.id[ind + 1] = ids_arr[ind + 1];
			}
			ind++;
			node1.id[0] = ids_arr[BRANCH_FACTOR / 2 + 1];
			for (; ind < BRANCH_FACTOR; ind++) {
				node1.val[ind - BRANCH_FACTOR / 2 - 1] = key_arr[ind];
				node1.id[ind - BRANCH_FACTOR / 2] = ids_arr[ind + 1];
			}

			/* Replace the left node and write the right node in the cache */
			cache_replace_node(tree, path[level].key, &node);
			cache_write_node(tree, nid, &node1);
		} else {
			modify_cache(tree, path[level].key, NODE, UNLOCK);
			tree->off_nodes--;
		}
		return res;
	}
}

/****************************************************************************
 * Name: tree_split
 *
 * Description: This routine splits the bucket and calls the tree_split
 *              function. This is called when a bucket is full and is part of
 *              the insertion process of an index entry
 *
 ****************************************************************************/
static bsplit_status_t bucket_split(tree_t *tree, int key, int value, pair_t *path)
{
	int median;
	bucket_t *bucket;
	uint16_t bucket_id = path[tree->levels].key;
	int i;
	if (tree->off_buckets == CONFIG_BUCKETS_LIMIT - 1) {
		DB_LOG_E("TREE FULL !");
		return BSPLIT_FAIL;
	}

	/* TODO
	 * Absent of non-cast return handling, should be taken care in the definition
	 */
	bucket = bucket_read(tree, bucket_id);

	/* Sort the key-value pairs in the bucket according to the keys and pick the median */
	pair_t bucket_tuples[BUCKET_SIZE + 1];
	for (i = 0; i < BUCKET_SIZE; i++) {
		bucket_tuples[i].key = bucket->pairs[i].key;
		bucket_tuples[i].value = bucket->pairs[i].value;
	}
	bucket_tuples[BUCKET_SIZE].key = key;
	bucket_tuples[BUCKET_SIZE].value = value;

	qsort(bucket_tuples, BUCKET_SIZE + 1, sizeof(pair_t), compare);

	median = bucket_tuples[(BUCKET_SIZE + 1) / 2].key;
	/* Call tree_split before creating a new bucket and dividing the entries */
	int b_id = tree->off_buckets++;
	int res = tree_split(tree, median, b_id, path, tree->levels - 1);

	/* Proceed only if the tree split is successful. This ensures top-down splitting of the tree nodes */
	if (res == TSPLIT_OK) {
		bucket_t b1, b2;
		int ind = 0;
		int off;
		for (;; ind++) {
			if (ind == (BUCKET_SIZE + 1) / 2) {
				break;
			}
			b1.pairs[ind] = bucket_tuples[ind];
		}
		off = ind;
		for (; ind < BUCKET_SIZE + 1; ind++) {
			b2.pairs[ind - off] = bucket_tuples[ind];
		}

		b2.next_free_slot = BUCKET_SIZE + 1 - off;
		b1.next_free_slot = off;
		/* Start Bucket chaining */
		b2.info[0] = bucket->info[0];
		b1.info[0] = b_id;
		b1.info[1] = b1.pairs[0].key;
		b1.info[2] = b1.pairs[b1.next_free_slot - 1].key;
		b2.info[1] = b2.pairs[0].key;
		b2.info[2] = b2.pairs[b2.next_free_slot - 1].key;
		/* End of Bucket chaining */

		modify_cache(tree, bucket_id, BUCKET, INVALIDATE);
		cache_write_bucket(tree, bucket_id, &b1);
		cache_write_bucket(tree, b_id, &b2);
	} else {
		tree->off_buckets--;
		modify_cache(tree, bucket_id, BUCKET, UNLOCK);
	}
	return BSPLIT_OK;
}

/****************************************************************************
 * Name: insert_item_btree
 *
 * Description: This implements the whole insertion logic by using the
 *              routines defined above.
 *
 ****************************************************************************/
tree_result_t insert_item_btree(tree_t *tree, int key, int value)
{
	int bucket_id;
	pair_t *path;
	bucket_t *tmp_bucket;
	int num_entries_bucket = 0;
start:
	bucket_id = -1;
	pair_t pair;
	while (bucket_id < 0) {
		rw_lock_read(&(tree->tree_lock));
		path = tree_find(tree, key);
		if (path == NULL) {
			rw_unlock_read(&(tree->tree_lock));
			continue;
		}
		bucket_id = path[tree->levels].key;
		rw_unlock_read(&(tree->tree_lock));
	}
	pair.key = key;
	pair.value = value;
	tmp_bucket = bucket_read(tree, bucket_id);
	num_entries_bucket = tmp_bucket->next_free_slot;
	modify_cache(tree, bucket_id, BUCKET, UNLOCK);

	/* When the bucket is full and requires splitting */
	if (num_entries_bucket == BUCKET_SIZE) {
		DB_LOG_D("DB: Bucket %d is full\n", bucket_id);
retry:
		if (rw_trylock_write(&(tree->tree_lock)) == 0) {
			pthread_mutex_lock(&(tree->bucket_lock));
			tree->lock_buckets[bucket_id] = 0;
			pthread_mutex_unlock(&(tree->bucket_lock));
			free(path);
			goto start;
		}
		/* Call the bucket split function and depending on the return value take further action */
		int res = bucket_split(tree, key, value, path);
		if (res == BSPLIT_FAIL) {
			pthread_mutex_lock(&(tree->bucket_lock));
			if (tree->lock_buckets[bucket_id] == 0) {
				DB_LOG_E("PANIC EDITED BUCKET WITHOUT LOCK\n");
				pthread_mutex_unlock(&(tree->bucket_lock));
				rw_unlock_write(&(tree->tree_lock));
				free(path);
				return TREE_LOCK_ERROR;
			} else {
				tree->lock_buckets[bucket_id] = 0;
				pthread_mutex_unlock(&(tree->bucket_lock));
				rw_unlock_write(&(tree->tree_lock));
			}
			free(path);
			return TREE_INSERT_FAIL;
		} else if (res == BSPLIT_RETRY) {
			rw_unlock_write(&(tree->tree_lock));
			goto retry;
		}

		pthread_mutex_lock(&(tree->bucket_lock));
		if (tree->lock_buckets[bucket_id] == 0) {
			DB_LOG_E("PANIC EDITED BUCKET WITHOUT LOCK\n");
			pthread_mutex_unlock(&(tree->bucket_lock));
			rw_unlock_write(&(tree->tree_lock));
			free(path);
			return TREE_LOCK_ERROR;
		} else {
			tree->lock_buckets[bucket_id] = 0;
			pthread_mutex_unlock(&(tree->bucket_lock));
			rw_unlock_write(&(tree->tree_lock));
		}
	} else {
		/* If the bucket has space for the entry, just append the new entry in the cache */
		cache_bucket_append(tree, bucket_id, &pair);
		tree->inserted++;
	}
	free(path);
	return TREE_OK;
}

#ifdef CONFIG_ARASTORAGE_ENABLE_FLUSHING
/****************************************************************************
 * Name: db_flush
 *
 * Description: Removes the old tuples from storage in case the tuple
 *              storage limit is reached
 *
 ****************************************************************************/
static int db_flush(tree_t *tree, relation_t *rel)
{
	DB_LOG_D("Started flushing the database. Deleted till now: %d\n", tree->deleted);
	char tuple_path[TUPLE_NAME_LENGTH];
	db_result_t result;
	relation_t old_rel;
	int flush_threshold = DB_TUPLES_LIMIT / 2;
	memcpy(&old_rel, rel, sizeof(relation_t));
	unsigned long long offset;
	int fd;

	offset = 0;
	fd = storage_open(rel->name, O_WRONLY);
	result = storage_write_to(fd, rel->name, offset, sizeof(rel->name));
	if (result != DB_OK) {
		storage_close(fd);
		storage_remove(rel->name);
		return DB_STORAGE_ERROR;
	}
	offset += sizeof(rel->name);

	/* Create a new tuple file */
	snprintf(tuple_path, TUPLE_NAME_LENGTH, "%s.%x\0", TUPLE_FILE_NAME, (unsigned)(random_rand() & 0xffff));
	result = storage_generate_file(tuple_path);
	if (result == DB_STORAGE_ERROR) {
		storage_close(fd);
		storage_remove(rel->tuple_filename);
		return DB_STORAGE_ERROR;
	}
	strncpy(rel->tuple_filename, tuple_path, TUPLE_NAME_LENGTH);

	/* char buff[sizeof(rel->tuple_filename)+13] */
	result = storage_write_to(fd, rel->tuple_filename, offset, sizeof(rel->tuple_filename));

	storage_close(fd);
	if (result != DB_OK) {
		storage_remove(rel->tuple_filename);
		return DB_STORAGE_ERROR;
	}
	rel->tuple_storage = storage_open(rel->tuple_filename, O_RDWR);

	int id;
	int num_tuples = 0;
	rel->next_row = num_tuples;
	rel->cardinality = num_tuples;
	storage_row_t temp = malloc(sizeof(rel->row_length));
	if (temp == NULL) {
		return DB_ALLOCATION_ERROR;
	}
	/* Iterate over the buckets and pick the tuples which should be retained i.e. written
	 * to the new storage file. Currently tuple_id decides this.
	 */
	for (id = 0; id < tree->off_buckets; id++) {
		/* TODO
		 * Absent of non-cast return handling, should be taken care in the definition
		 */
		bucket_t *bucket = bucket_read(tree, id);

		int num, ind;
		int num_entries = bucket->next_free_slot;
		for (num = 0, ind = 0; num < num_entries; num++) {
			tuple_id_t tup;
			tup = bucket->pairs[num].value;

			if (tup >= flush_threshold) {
				storage_get_row(&old_rel, &tup, temp);
				storage_put_row(rel, temp, FALSE);
				uint16_t tmp_key = bucket->pairs[num].key;
				bucket->pairs[ind].key = tmp_key;
				bucket->pairs[ind].value = num_tuples;
				num_tuples++;
				ind++;
			} else {
				bucket->next_free_slot--;
				tree->deleted++;
			}
		}
		/* Start Bucket chaining */
		int iter = 0;
		uint16_t new_min = bucket->info[1];
		uint16_t new_max = bucket->info[2];
		for (; iter < bucket->next_free_slot; iter++) {
			new_min = min(bucket->pairs[iter].key, new_min);
			new_max = max(bucket->pairs[iter].key, new_max);
		}
		bucket->info[1] = new_min;
		bucket->info[2] = new_max;
		/* End of Bucket chaining */

		modify_cache(tree, id, BUCKET, DIRTY);
		modify_cache(tree, id, BUCKET, UNLOCK);
	}
	free(temp);
	tree->inserted -= tree->deleted;
	tree->deleted = 0;
	storage_remove(old_rel.tuple_filename);
	DB_LOG_D("Flushed the database.\n");
	return DB_OK;
}
#endif
