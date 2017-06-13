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
 * Copyright (c) 2010, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \file
 *      Database configuration options.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

#ifndef DB_OPTIONS_H
#define DB_OPTIONS_H

/*----------------------------------------------------------------------------*/

/* Optional Antelope features. Include only what is needed
   in order to save space. */

/* Support floating-point values in attributes. */
#ifndef DB_FEATURE_FLOATS
#define DB_FEATURE_FLOATS               0
#endif							/* DB_FEATURE_FLOATS */

/*----------------------------------------------------------------------------*/

/* Configuration parameters that may be trimmed to save space. */

/* The size of the error message buffer used by the parser. */
#ifndef DB_ERROR_BUF_SIZE
#define DB_ERROR_BUF_SIZE               50
#endif							/* DB_ERROR_BUF_SIZE */

/* The maximum number of indexes in use by all relations loaded in memory. */
#ifndef DB_INDEX_POOL_SIZE
#define DB_INDEX_POOL_SIZE              3
#endif							/* DB_INDEX_POOL_SIZE */

/* The maximum number of relations loaded in memory. */
#ifndef DB_RELATION_POOL_SIZE
#define DB_RELATION_POOL_SIZE           5
#endif							/* DB_RELATION_POOL_SIZE */

/* The maximum number of attributes loaded in memory. */
#ifndef DB_ATTRIBUTE_POOL_SIZE
#define DB_ATTRIBUTE_POOL_SIZE          16
#endif							/* DB_ATTRIBUTE_POOL_SIZE */

/* The maximum number of attributes in a relation. */
#ifndef DB_MAX_ATTRIBUTES_PER_RELATION
#define DB_MAX_ATTRIBUTES_PER_RELATION  6
#endif							/* DB_MAX_ATTRIBUTES_PER_RELATION */

/* The maximum physical storage size on an attribute value. */
#ifndef DB_MAX_ELEMENT_SIZE
#define DB_MAX_ELEMENT_SIZE             32
#endif							/* DB_MAX_ELEMENT_SIZE */

/* The maximum size of the LVM bytecode compiled from a
   single database query. */
#ifndef DB_VM_BYTECODE_SIZE
#define DB_VM_BYTECODE_SIZE             32
#endif							/* DB_VM_BYTECODE_SIZE */

/*----------------------------------------------------------------------------*/

/* Language options. */

/* The maximum length of a database query in AQL text format. */
#ifndef AQL_MAX_QUERY_LENGTH
#define AQL_MAX_QUERY_LENGTH            128
#endif							/* AQL_MAX_QUERY_LENGTH */

#ifndef AQL_MAX_VALUE_LENGTH
#define AQL_MAX_VALUE_LENGTH            DB_MAX_ELEMENT_SIZE
#endif							/* AQL_MAX_VALUE_LENGTH */

/* The maximum number of relations used in a single query. */
#ifndef AQL_RELATION_LIMIT
#define AQL_RELATION_LIMIT              3
#endif							/* AQL_RELATION_LIMIT */

/* The maximum number of attributes used in a single query. */
#ifndef AQL_ATTRIBUTE_LIMIT
#define AQL_ATTRIBUTE_LIMIT             6
#endif							/* AQL_ATTRIBUTE_LIMIT */

/*----------------------------------------------------------------------------*/

/*
 * Physical storage options. Changing these options might cause
 * compatibility problems if the database files are moved between
 * different installations of Antelope.
 */

/* The default relation file size to reserve when using Coffee. */

/* The maximum size of the physical storage of a tuple (labelled a "row"
   in Antelope's terminology. */
#ifndef DB_MAX_CHAR_SIZE_PER_ROW
#define DB_MAX_CHAR_SIZE_PER_ROW        64
#endif							/* DB_MAX_CHAR_SIZE_PER_ROW */

/* The maximum file name length to use for creating various database file. */
#ifndef DB_MAX_FILENAME_LENGTH
#define DB_MAX_FILENAME_LENGTH          16
#endif							/* DB_MAX_FILENAME_LENGTH */

/* The maximum length of an attribute name. */
#ifndef ATTRIBUTE_NAME_LENGTH
#define ATTRIBUTE_NAME_LENGTH           12
#endif							/* ATTRIBUTE_NAME_LENGTH */

/* The maximum length on a relation name. */
#ifndef RELATION_NAME_LENGTH
#define RELATION_NAME_LENGTH            16
#endif							/* RELATION_NAME_LENGTH */

/* File system mount point. */
#ifndef CONFIG_MOUNT_POINT
#define CONFIG_MOUNT_POINT "/mnt/"
#endif

/* The maximum number of tuples in a relation. */
#ifndef DB_TUPLE_LIMIT
#define DB_TUPLE_LIMIT          2000
#endif							/* DB_TUPLE_LIMIT */

/* The number of int array in a cursor. */
#ifndef DB_CURSOR_LIMIT
#define DB_CURSOR_LIMIT          ((DB_TUPLE_LIMIT / (sizeof(uint32_t)*8)) + 1)
#endif							/* DB_CURSOR_LIMIT */

/* The maximum number of tuples in a cursor. */
#ifndef DB_CURSOR_RESULT_ENTRY
#define DB_CURSOR_RESULT_ENTRY          ((DB_CURSOR_LIMIT) * (sizeof(uint32_t)*8))
#endif							/* DB_CURSOR_RESULT_ENTRY */

/* The name of the intermediate "result" relation file, which is used
   for presenting the result of a query to a user. */
#ifndef RESULT_RELATION
#define RESULT_RELATION "db-res"
#endif							/* RESULT_RELATION */

/* The name of the relation used for processing a REMOVE query. */
#ifndef REMOVE_RELATION
#define REMOVE_RELATION "db-rem"
#endif							/* REMOVE_RELATION */

#define INDEX_NAME_SUFFIX ".idx"

#define INDEX_NAME_LENGTH (RELATION_NAME_LENGTH + sizeof(INDEX_NAME_SUFFIX) - 1)

#define TUPLE_FILE_NAME "tup"

#define TUPLE_NAME_LENGTH 14

#define HEAP_FILE_NAME "heap"

#define HEAP_FILE_LENGTH 15

#define BUCKET_FILE_NAME "buck"

#define BUCKET_FILE_LENGTH 15

#define TEMP_FILE_SUFFIX ".tmp"

#define TEMP_FILE_SUFFIX_LENGTH 4
/*----------------------------------------------------------------------------*/

/* Index options. */

#ifndef DB_INDEX_COST
#define DB_INDEX_COST                   64
#endif							/* DB_INDEX_COST */

/* The maximum number of Maxheap indexes. */
#ifndef DB_HEAP_INDEX_LIMIT
#define DB_HEAP_INDEX_LIMIT             1
#endif							/* DB_HEAP_INDEX_LIMIT */

/* The maximum number of buckets cached in the MaxHeap index. */
#ifndef DB_HEAP_CACHE_LIMIT
#define DB_HEAP_CACHE_LIMIT             6
#endif							/* DB_HEAP_CACHE_LIMIT */

#ifndef DB_TREE_CACHE_LIMIT
#define DB_TREE_CACHE_LIMIT             10
#endif

#ifdef DB_WIP
#undef DB_WIP 						/* DB WORK IN PROGRESS */
#endif
/*----------------------------------------------------------------------------*/

/* LVM options. */

/* The maximum length of a variable in LVM. This value should preferably
   be identical to the maximum attribute name length. */
#ifndef LVM_MAX_NAME_LENGTH
#define LVM_MAX_NAME_LENGTH             ATTRIBUTE_NAME_LENGTH
#endif							/* LVM_MAX_NAME_LENGTH */

/* The maximum variable identifier number in the LVM. The default
   value corresponds to the highest attribute ID. */
#ifndef LVM_MAX_VARIABLE_ID
#define LVM_MAX_VARIABLE_ID             AQL_ATTRIBUTE_LIMIT - 1
#endif							/* LVM_MAX_VARIABLE_ID */

/* Specify whether floats should be used or not inside the LVM. */
#ifndef LVM_USE_FLOATS
#define LVM_USE_FLOATS                  DB_FEATURE_FLOATS
#endif							/* LVM_USE_FLOATS */

#endif							/* !DB_OPTIONS_H */
