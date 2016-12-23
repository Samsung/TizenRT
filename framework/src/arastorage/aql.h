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
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *      Definitions and declarations for AQL, the Antelope Query Language.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

#ifndef AQL_H
#define AQL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "db_options.h"
#include "index.h"
#include "relation.h"
#include "result.h"

/****************************************************************************
* Pre-processor Definitions
****************************************************************************/
#define AQL_OP_TYPE_NONE                   0x00000000
#define AQL_OP_TYPE_EXEC                   0x00010000
#define AQL_OP_TYPE_QUERY                  0x00020000
#define AQL_OP_TYPE_MASK                   0x00030000

#define AQL_DATA_TYPE_MASK                 0x0000000F

#define AQL_TYPE_NONE                      (AQL_OP_TYPE_NONE)

#define AQL_TYPE_CREATE_ATTRIBUTE          (AQL_OP_TYPE_EXEC | 0x00000001)
#define AQL_TYPE_CREATE_INDEX              (AQL_OP_TYPE_EXEC | 0x00000002)
#define AQL_TYPE_CREATE_RELATION           (AQL_OP_TYPE_EXEC | 0x00000003)

#define AQL_TYPE_INSERT                    (AQL_OP_TYPE_EXEC | 0x00000004)

#define AQL_TYPE_REMOVE_ATTRIBUTE          (AQL_OP_TYPE_EXEC | 0x00000005)
#define AQL_TYPE_REMOVE_INDEX              (AQL_OP_TYPE_EXEC | 0x00000006)
#define AQL_TYPE_REMOVE_RELATION           (AQL_OP_TYPE_EXEC | 0x00000007)
#define AQL_TYPE_FLUSH                     (AQL_OP_TYPE_EXEC | 0x00000008)

#define AQL_TYPE_SELECT                    (AQL_OP_TYPE_QUERY | 0x00000009)
#define AQL_TYPE_REMOVE_TUPLES             (AQL_OP_TYPE_QUERY | 0x0000000A)

#define AQL_TYPE_MASK                      (AQL_OP_TYPE_MASK | AQL_DATA_TYPE_MASK)

#define AQL_FLAG_AGGREGATE              1
#define AQL_FLAG_SELECT_ALL             2
#define AQL_FLAG_ASSIGN                 4

#define AQL_CLEAR(adt)                  aql_clear(adt)
#define AQL_SET_TYPE(adt, type)  (((adt))->optype = (type))
#define AQL_GET_OP_TYPE(optype)  ((optype) & (AQL_OP_TYPE_MASK))
#define AQL_GET_EXEC_TYPE(optype)   ((optype) & (AQL_TYPE_MASK))

#define AQL_GET_TYPE(adt)               ((adt)->optype)

#define AQL_SET_INDEX_TYPE(adt, type)   ((adt)->index_type = (type))
#define AQL_GET_INDEX_TYPE(adt) ((adt)->index_type)

#define AQL_SET_FLAG(adt, flag) (((adt)->flags) |= (flag))
#define AQL_GET_FLAGS(adt)              ((adt)->flags)
#define AQL_ADD_RELATION(adt, rel) aql_add_relation(adt, rel)
#define AQL_RELATION_COUNT(adt) ((adt)->relation_count)
#define AQL_ADD_ATTRIBUTE(adt, attr, dom, size)                 \
	aql_add_attribute(adt, attr, dom, size, 0)
#define AQL_ADD_PROCESSING_ATTRIBUTE(adt, attr)                 \
	aql_add_attribute((adt), (attr), DOMAIN_UNSPECIFIED, 0, 1)
#define AQL_ADD_AGGREGATE(adt, function, attr)                      \
	do {                                                            \
		(adt)->aggregators[(adt)->attribute_count] = (function);    \
		aql_add_attribute((adt), (attr), DOMAIN_UNSPECIFIED, 0, 0); \
	} while (0)
#define AQL_ADD_AGGREGATE_VALUE(adt, value) \
	aql_add_operand_value((adt), (value))
#define AQL_ATTRIBUTE_COUNT(adt)        ((adt)->attribute_count)
#define AQL_SET_CONDITION(adt, cond)    ((adt)->lvm_instance = (cond))
#define AQL_ADD_VALUE(adt, domain, value)                               \
	aql_add_value((adt), (domain), (value))

/****************************************************************************
* Public Type Definitions
****************************************************************************/
enum aql_status_e {
	STATUS_OK = 2,
	SYNTAX_ERROR = 3,
	INVALID_TOKEN = 9,
	PLE_ERROR = 12
};
typedef enum aql_status_e aql_status_t;
#define AQL_ERROR(x)     ((x) >= 3)

enum token_e {
	END = 0,
	LEFT_PAREN,
	RIGHT_PAREN,
	COMMA,
	EQUAL,
	GT,
	LT,
	DOT,
	ADD,
	SUB,
	MUL,						/* 10 */
	DIV,
	COMMENT,
	GEQ,
	LEQ,
	NOT_EQUAL,
	ASSIGN,
	OR,
	IS,
	ON,
	IN,							/* 20 */
	ALL,
	AND,
	NOT,
	SUM,
	MAX,
	MIN,
	INT,
	INTO,
	FROM,
	MEAN,
	JOIN,						/* 30 */
	LONG,
	TYPE,
	WHERE,
	COUNT,
	INDEX,
	INSERT,
	SELECT,
	REMOVE,
	CREATE,
	MEDIAN,						/* 40 */
	DOMAIN,
	STRING,
	INLINE,
	REMAIN,

	PROJECT,

	RELATION,

	ATTRIBUTE,
	BPLUSTREE,					/* 48 */

	INTEGER_VALUE = 251,
	FLOAT_VALUE = 252,
	STRING_VALUE = 253,
	IDENTIFIER = 254,
	NONE = 255
};

typedef enum token_e token_t;

typedef char value_t[DB_MAX_ELEMENT_SIZE];

struct lexer_s {
	const char *input;
	const char *prev_pos;
	token_t *token;
	value_t *value;
};

typedef struct lexer_s lexer_t;

enum aql_aggregator_e {
	AQL_NONE = 0,
	AQL_COUNT = 1,
	AQL_SUM = 2,
	AQL_MIN = 3,
	AQL_MAX = 4,
	AQL_MEAN = 5,
};

typedef enum aql_aggregator_e aql_aggregator_t;

struct aql_attribute_s {
	domain_t domain;
	uint8_t element_size;
	uint8_t flags;
	char name[ATTRIBUTE_NAME_LENGTH + 1];
};
typedef struct aql_attribute_s aql_attribute_t;

struct aql_adt_s {
	char relations[AQL_RELATION_LIMIT][RELATION_NAME_LENGTH + 1];
	aql_attribute_t attributes[AQL_ATTRIBUTE_LIMIT];
	aql_aggregator_t aggregators[AQL_ATTRIBUTE_LIMIT];
	attribute_value_t values[AQL_ATTRIBUTE_LIMIT];
	index_type_t index_type;
	uint8_t relation_count;
	uint8_t attribute_count;
	uint8_t value_count;
	uint32_t optype;
	uint8_t flags;
	void *lvm_instance;
};
typedef struct aql_adt_s aql_adt_t;

/****************************************************************************
* Global Function Prototypes
****************************************************************************/
int lexer_start(lexer_t *, char *, token_t *, value_t *);
int lexer_next(lexer_t *);
void lexer_rewind(lexer_t *);

void aql_clear(aql_adt_t *adt);
void aql_add_relation(aql_adt_t *adt, char *rel);
aql_status_t aql_parse(aql_adt_t *adt, char *query_string);
db_result_t aql_add_attribute(aql_adt_t *adt, char *name, domain_t domain, unsigned element_size, int processed_only);
db_result_t aql_add_value(aql_adt_t *adt, domain_t domain, void *value);

#endif							/* !AQL_H */
