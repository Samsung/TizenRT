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
 *      A recursive parser for AQL, the Antelope Query Language.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "attribute.h"
#include "aql.h"
#include "db_options.h"
#include "db_debug.h"
#include "lvm.h"

/****************************************************************************
* Pre-processor Definitions
****************************************************************************/
#if DEBUG
static char error_message[DB_ERROR_BUF_SIZE];
static int error_line;
static const char *error_function;
#define RETURN(value)                                                          \
	do {                                                                       \
		if (error_message[0] == '\0') {                                        \
			strncpy(error_message, lexer->input, sizeof(error_message) - 1);   \
			error_line = __LINE__;                                             \
			error_function = __func__;                                         \
		}                                                                      \
	} while (0);                                                               \
	return (value)
#define RESET_ERROR()                               \
	do {                                            \
		error_message[0] = '\0';                    \
		error_line = 0;                             \
		error_function = NULL;                      \
	} while (0)
#else
#define RETURN(value) return (value)
#define RESET_ERROR()
#endif
#define PARSER(name)                                            \
		static aql_status_t                                     \
		parse_##name(aql_adt_t* adt, lexer_t *lexer)
#define PARSER_ARG(name, arg)                                   \
		static aql_status_t                                     \
		parse_##name(aql_adt_t* adt, lexer_t *lexer, arg)
#define PARSER_TOKEN(name)                                      \
		static token_t                                          \
		parse_##name(lexer_t *lexer)
#define PARSE(name)                                             \
		!AQL_ERROR(parse_##name(adt, lexer))
#define PARSE_TOKEN(name)                                       \
		parse_##name(lexer)

#define NEXT   lexer_next(lexer)
#define REWIND lexer_rewind(lexer); RESET_ERROR()
#define TOKEN  *lexer->token
#define VALUE  *lexer->value

#define CONSUME(token)                          \
	do {                                        \
		NEXT;                                   \
		if (TOKEN != (token)) {                 \
			RETURN(SYNTAX_ERROR);               \
		}                                       \
	} while (0)

/****************************************************************************
* Private Functions
****************************************************************************/

/* Parsing functions for AQL. */
PARSER_TOKEN(cmp)
{
	NEXT;
	switch (TOKEN) {
	case EQUAL:
	case NOT_EQUAL:
	case GT:
	case LT:
	case GEQ:
	case LEQ:
		return TOKEN;
	default:
		return NONE;
	}
}

PARSER_TOKEN(op)
{
	NEXT;
	switch (TOKEN) {
	case ADD:
	case SUB:
	case MUL:
	case DIV:
	case RIGHT_PAREN:
		return TOKEN;
	default:
		return NONE;
	}
}

PARSER_TOKEN(aggregator)
{
	NEXT;
	switch (TOKEN) {
	case COUNT:
	case SUM:
	case MEAN:
	case MAX:
	case MIN:
		return TOKEN;
	default:
		return NONE;
	}
}

PARSER(attributes)
{
	token_t token;
	aql_aggregator_t function;

	token = PARSE_TOKEN(aggregator);
	if (token != NONE) {
		switch (TOKEN) {
		case COUNT:
			function = AQL_COUNT;
			break;
		case SUM:
			function = AQL_SUM;
			break;
		case MEAN:
			function = AQL_MEAN;
			break;
		case MAX:
			function = AQL_MAX;
			break;
		case MIN:
			function = AQL_MIN;
			break;
		default:
			RETURN(SYNTAX_ERROR);
		}

		DB_LOG_V("aggregator: %d\n", TOKEN);

		/* Parse the attribute to aggregate. */
		CONSUME(LEFT_PAREN);
		CONSUME(IDENTIFIER);

		AQL_ADD_AGGREGATE(adt, function, VALUE);
		DB_LOG_V("aggregated attribute: %s\n", VALUE);

		AQL_SET_FLAG(adt, AQL_FLAG_AGGREGATE);

		CONSUME(RIGHT_PAREN);
		goto check_more_attributes;
	} else {
		REWIND;
	}

	/* Plain identifier. */
	CONSUME(IDENTIFIER);
	AQL_ADD_ATTRIBUTE(adt, VALUE, DOMAIN_UNSPECIFIED, 0);

check_more_attributes:
	NEXT;
	if (TOKEN == COMMA) {
		if (!PARSE(attributes)) {
			RETURN(SYNTAX_ERROR);
		}
	} else {
		REWIND;
	}

	RETURN(STATUS_OK);
}

PARSER(relations)
{
	/* Parse comma-separated identifiers for relations. */
	CONSUME(IDENTIFIER);

	AQL_ADD_RELATION(adt, VALUE);
	NEXT;
	if (TOKEN == COMMA) {
		if (!PARSE(relations)) {
			RETURN(SYNTAX_ERROR);
		}
	} else {
		REWIND;
	}

	RETURN(STATUS_OK);
}

PARSER(values)
{
	/* Parse comma-separated attribute values. */
	NEXT;
	switch (TOKEN) {
	case STRING_VALUE:
		AQL_ADD_VALUE(adt, DOMAIN_STRING, VALUE);
		break;
	case INTEGER_VALUE:
		AQL_ADD_VALUE(adt, DOMAIN_INT, VALUE);
		break;
	default:
		RETURN(SYNTAX_ERROR);
	}

	NEXT;
	if (TOKEN == COMMA) {
		return PARSE(values);
	} else {
		REWIND;
	}

	RETURN(STATUS_OK);
}

PARSER(operand)
{
	lvm_instance_t *p;

	p = adt->lvm_instance;

	NEXT;
	switch (TOKEN) {
	case IDENTIFIER:
		if (LVM_ERROR(lvm_register_variable(p, VALUE, LVM_LONG)) || LVM_ERROR(lvm_set_variable(p, VALUE))) {
			RETURN(SYNTAX_ERROR);
		}
		AQL_ADD_PROCESSING_ATTRIBUTE(adt, VALUE);
		break;
	case STRING_VALUE:
		break;
	case FLOAT_VALUE:
		break;
	case INTEGER_VALUE:
		if (LVM_ERROR(lvm_set_long(p, *(long *)lexer->value))) {
			RETURN(SYNTAX_ERROR);
		}
		break;
	default:
		RETURN(SYNTAX_ERROR);
	}

	RETURN(STATUS_OK);
}

PARSER(expr)
{
	token_t token;
	size_t saved_end;
	operator_t op;
	lvm_instance_t *p;

	p = adt->lvm_instance;
	saved_end = lvm_get_end(p);

	NEXT;
	if (TOKEN == LEFT_PAREN) {
		if (!PARSE(expr)) {
			RETURN(SYNTAX_ERROR);
		}
		CONSUME(RIGHT_PAREN);
	} else {
		REWIND;
		if (!PARSE(operand)) {
			RETURN(SYNTAX_ERROR);
		}
	}

	while (1) {
		token = PARSE_TOKEN(op);
		if (token == NONE) {
			saved_end = lvm_get_end(p);
			REWIND;
			break;
		} else if (token == RIGHT_PAREN) {
			break;
		}

		if (!PARSE(operand) && !PARSE(expr)) {
			RETURN(SYNTAX_ERROR);
		}

		saved_end = lvm_shift_for_operator(p, saved_end);

		switch (token) {
		case ADD:
			op = LVM_ADD;
			break;
		case SUB:
			op = LVM_SUB;
			break;
		case MUL:
			op = LVM_MUL;
			break;
		case DIV:
			op = LVM_DIV;
			break;
		default:
			RETURN(SYNTAX_ERROR);
		}
		if (LVM_ERROR(lvm_set_op(p, op))) {
			RETURN(SYNTAX_ERROR);
		}
		lvm_set_end(p, saved_end);
	}

	return STATUS_OK;
}

PARSER(comparison)
{
	token_t token;
	size_t saved_end;
	operator_t rel;
	lvm_instance_t *p;

	p = adt->lvm_instance;

	saved_end = lvm_jump_to_operand(p);

	if (!PARSE(expr)) {
		RETURN(SYNTAX_ERROR);
	}

	saved_end = lvm_set_end(p, saved_end);

	token = PARSE_TOKEN(cmp);
	if (token == NONE) {
		RETURN(SYNTAX_ERROR);
	}

	switch (token) {
	case GT:
		rel = LVM_GE;
		break;
	case GEQ:
		rel = LVM_GEQ;
		break;
	case LT:
		rel = LVM_LE;
		break;
	case LEQ:
		rel = LVM_LEQ;
		break;
	case EQUAL:
		rel = LVM_EQ;
		break;
	case NOT_EQUAL:
		rel = LVM_NEQ;
		break;
	default:
		RETURN(SYNTAX_ERROR);
	}

	if (LVM_ERROR(lvm_set_relation(p, rel))) {
		RETURN(SYNTAX_ERROR);
	}
	lvm_set_end(p, saved_end);

	if (!PARSE(expr)) {
		RETURN(SYNTAX_ERROR);
	}

	RETURN(STATUS_OK);
}

PARSER(where)
{
	int r;
	lvm_instance_t *p;
	operator_t connective;
	size_t saved_end;

	p = adt->lvm_instance;

	if (!PARSE(comparison)) {
		RETURN(SYNTAX_ERROR);
	}

	saved_end = 0;

	/* The WHERE clause can consist of multiple prepositions. */
	for (;;) {
		NEXT;
		if (TOKEN != AND && TOKEN != OR) {
			REWIND;
			break;
		}

		connective = TOKEN == AND ? LVM_AND : LVM_OR;

		saved_end = lvm_shift_for_operator(p, saved_end);
		if (LVM_ERROR(lvm_set_relation(p, connective))) {
			RETURN(SYNTAX_ERROR);
		}
		lvm_set_end(p, saved_end);

		NEXT;
		if (TOKEN == LEFT_PAREN) {
			r = PARSE(where);
			if (!r) {
				RETURN(SYNTAX_ERROR);
			}
			CONSUME(RIGHT_PAREN);
		} else {
			REWIND;
			r = PARSE(comparison);
			if (!r) {
				RETURN(r);
			}
		}
	}

	lvm_print_code(p);

	return STATUS_OK;
}

PARSER(select)
{
	lvm_instance_t *lvm;

	AQL_SET_TYPE(adt, AQL_TYPE_SELECT);

	NEXT;
	if (TOKEN == ALL) {
		AQL_SET_FLAG(adt, AQL_FLAG_SELECT_ALL);
	} else {
		REWIND;

		/* projection attributes... */
		if (!PARSE(attributes)) {
			RETURN(SYNTAX_ERROR);
		}
	}

	CONSUME(FROM);
	if (!PARSE(relations)) {
		RETURN(SYNTAX_ERROR);
	}

	NEXT;
	if (TOKEN == WHERE) {
		lvm = (lvm_instance_t *)malloc(sizeof(lvm_instance_t));
		if (lvm == NULL) {
			DB_LOG_V("DB: Failed to malloc lvm instance\n");
			return DB_ALLOCATION_ERROR;
		}

		lvm_reset(lvm);
		AQL_SET_CONDITION(adt, lvm);

		if (!PARSE(where)) {
			free(lvm);
			AQL_SET_CONDITION(adt, NULL);
			RETURN(SYNTAX_ERROR);
		}
	} else {
		REWIND;
		RETURN(STATUS_OK);
	}

	CONSUME(END);

	return STATUS_OK;
}

PARSER(remain)
{
	lvm_instance_t *lvm;

	AQL_SET_TYPE(adt, AQL_TYPE_FLUSH);

	/* projection attributes... */
	if (!PARSE(attributes)) {
		RETURN(SYNTAX_ERROR);
	}

	CONSUME(FROM);
	if (!PARSE(relations)) {
		RETURN(SYNTAX_ERROR);
	}

	NEXT;
	if (TOKEN == WHERE) {
		lvm = (lvm_instance_t *)malloc(sizeof(lvm_instance_t));
		if (lvm == NULL) {
			DB_LOG_V("Failed to malloc lvm instance\n");
			return DB_ALLOCATION_ERROR;
		}

		lvm_reset(lvm);
		AQL_SET_CONDITION(adt, lvm);

		if (!PARSE(where)) {
			free(lvm);
			AQL_SET_CONDITION(adt, NULL);
			RETURN(SYNTAX_ERROR);
		}
	} else {
		REWIND;
		RETURN(STATUS_OK);
	}

	CONSUME(END);

	return STATUS_OK;
}

PARSER(insert)
{
	AQL_SET_TYPE(adt, AQL_TYPE_INSERT);

	CONSUME(LEFT_PAREN);

	if (!PARSE(values)) {
		RETURN(SYNTAX_ERROR);
	}

	CONSUME(RIGHT_PAREN);
	CONSUME(INTO);

	if (!PARSE(relations)) {
		RETURN(SYNTAX_ERROR);
	}

	RETURN(STATUS_OK);
}

PARSER(remove_attribute)
{
	AQL_SET_TYPE(adt, AQL_TYPE_REMOVE_ATTRIBUTE);

	CONSUME(IDENTIFIER);
	AQL_ADD_RELATION(adt, VALUE);

	CONSUME(DOT);
	CONSUME(IDENTIFIER);

	DB_LOG_V("Removing the index for the attribute %s\n", VALUE);
	AQL_ADD_ATTRIBUTE(adt, VALUE, DOMAIN_UNSPECIFIED, 0);

	RETURN(STATUS_OK);
}

PARSER(remove_from)
{
	lvm_instance_t *lvm;

	AQL_SET_TYPE(adt, AQL_TYPE_REMOVE_TUPLES);

	/* Use a temporary persistent relation to assign the query result to. */
	AQL_SET_FLAG(adt, AQL_FLAG_ASSIGN);
	AQL_ADD_RELATION(adt, REMOVE_RELATION);

	CONSUME(IDENTIFIER);
	AQL_ADD_RELATION(adt, VALUE);

	CONSUME(WHERE);

	lvm = (lvm_instance_t *)malloc(sizeof(lvm_instance_t));
	if (lvm == NULL) {
		DB_LOG_V("Failed to malloc lvm instance\n");
		return DB_ALLOCATION_ERROR;
	}

	lvm_reset(lvm);
	AQL_SET_CONDITION(adt, lvm);

	if (!PARSE(where)) {
		free(lvm);
		RETURN(SYNTAX_ERROR);
	}

	return STATUS_OK;
}

PARSER(remove_index)
{
	AQL_SET_TYPE(adt, AQL_TYPE_REMOVE_INDEX);

	CONSUME(IDENTIFIER);
	AQL_ADD_RELATION(adt, VALUE);

	CONSUME(DOT);
	CONSUME(IDENTIFIER);

	DB_LOG_V("remove index: %s\n", VALUE);
	AQL_ADD_ATTRIBUTE(adt, VALUE, DOMAIN_UNSPECIFIED, 0);

	RETURN(STATUS_OK);
}

PARSER(remove_relation)
{
	AQL_SET_TYPE(adt, AQL_TYPE_REMOVE_RELATION);

	CONSUME(IDENTIFIER);
	DB_LOG_V("remove relation: %s\n", VALUE);
	AQL_ADD_RELATION(adt, VALUE);

	RETURN(STATUS_OK);
}

PARSER(remove)
{
	aql_status_t r;

	NEXT;
	switch (TOKEN) {
	case ATTRIBUTE:
		r = PARSE(remove_attribute);
		break;
	case FROM:
		r = PARSE(remove_from);
		break;
	case INDEX:
		r = PARSE(remove_index);
		break;
	case RELATION:
		r = PARSE(remove_relation);
		break;
	default:
		RETURN(SYNTAX_ERROR);
	}

	if (!r) {
		RETURN(SYNTAX_ERROR);
	}

	CONSUME(END);

	RETURN(STATUS_OK);
}

PARSER_TOKEN(index_type)
{
	NEXT;
	switch (TOKEN) {
	case INLINE:
	case BPLUSTREE:
		return TOKEN;
	default:
		return NONE;
	}
}

PARSER(create_index)
{
	token_t token;
	index_type_t type;

	AQL_SET_TYPE(adt, AQL_TYPE_CREATE_INDEX);

	CONSUME(IDENTIFIER);
	AQL_ADD_RELATION(adt, VALUE);

	CONSUME(DOT);
	CONSUME(IDENTIFIER);

	DB_LOG_V("Creating an index for the attribute %s\n", VALUE);
	AQL_ADD_ATTRIBUTE(adt, VALUE, DOMAIN_UNSPECIFIED, 0);

	CONSUME(TYPE);

	token = PARSE_TOKEN(index_type);

	if (token == NONE) {
		RETURN(SYNTAX_ERROR);
	}

	switch (TOKEN) {
	case INLINE:
		type = INDEX_INLINE;
		break;
	case BPLUSTREE:
		type = INDEX_BPLUSTREE;
		break;
	default:
		RETURN(SYNTAX_ERROR);
	}

	DB_LOG_V("index type : %s\n", TOKEN);
	AQL_SET_INDEX_TYPE(adt, type);

	RETURN(STATUS_OK);
}

PARSER(create_relation)
{
	CONSUME(IDENTIFIER);

	AQL_SET_TYPE(adt, AQL_TYPE_CREATE_RELATION);
	AQL_ADD_RELATION(adt, VALUE);

	RETURN(STATUS_OK);
}

PARSER_ARG(domain, char *name)
{
	domain_t domain;
	unsigned element_size;

	NEXT;
	switch (TOKEN) {
	case STRING:
		domain = DOMAIN_STRING;

		/* Parse the amount of characters for this domain. */
		CONSUME(LEFT_PAREN);
		CONSUME(INTEGER_VALUE);
		element_size = *(long *)lexer->value;
		CONSUME(RIGHT_PAREN);

		break;
	case LONG:
		domain = DOMAIN_LONG;
		element_size = 4;
		break;
	case INT:
		domain = DOMAIN_INT;
		element_size = 2;
		break;
	default:
		return NONE;
	}

	AQL_ADD_ATTRIBUTE(adt, name, domain, element_size);

	return STATUS_OK;
}

PARSER(create_attributes)
{
	aql_status_t r;
	char name[ATTRIBUTE_NAME_LENGTH];

	AQL_SET_TYPE(adt, AQL_TYPE_CREATE_ATTRIBUTE);

	CONSUME(IDENTIFIER);

	strncpy(name, VALUE, sizeof(name) - 1);
	name[sizeof(name) - 1] = '\0';

	CONSUME(DOMAIN);

	r = parse_domain(adt, lexer, name);
	if (AQL_ERROR(r)) {
		RETURN(r);
	}

	CONSUME(IN);
	CONSUME(IDENTIFIER);

	AQL_ADD_RELATION(adt, VALUE);

	RETURN(STATUS_OK);
}

PARSER(create)
{
	aql_status_t r;

	NEXT;
	switch (TOKEN) {
	case ATTRIBUTE:
		r = PARSE(create_attributes);
		break;
	case INDEX:
		r = PARSE(create_index);
		break;
	case RELATION:
		r = PARSE(create_relation);
		break;
	default:
		RETURN(SYNTAX_ERROR);
	}

	if (!r) {
		RETURN(SYNTAX_ERROR);
	}

	CONSUME(END);

	RETURN(STATUS_OK);
}

/****************************************************************************
* Public Functions
****************************************************************************/
aql_status_t aql_parse(aql_adt_t *adt, char *input_string)
{
	lexer_t lex;
	token_t token = NONE;
	value_t value;
	aql_status_t result;
	RESET_ERROR();

	DB_LOG_V("Parsing \"%s\"\n", input_string);

	AQL_CLEAR(adt);
	AQL_SET_CONDITION(adt, NULL);

	//initialize lexer
	lex.input = input_string;
	lex.prev_pos = input_string;
	lex.token = &token;
	lex.value = &value;

	result = lexer_next(&lex);
	if (!AQL_ERROR(result)) {
		switch (token) {
		case IDENTIFIER:
			DB_LOG_V("Assign the result to relation %s\n", *lex.value);
			AQL_ADD_RELATION(adt, *lex.value);
			AQL_SET_FLAG(adt, AQL_FLAG_ASSIGN);
			if (AQL_ERROR(lexer_next(&lex))) {
				result = SYNTAX_ERROR;
				break;
			}
			if (*lex.token != ASSIGN) {
				result = SYNTAX_ERROR;
				break;
			}
			if (AQL_ERROR(lexer_next(&lex))) {
				result = SYNTAX_ERROR;
				break;
			}
			switch (*lex.token) {
			case SELECT:
				result = parse_select(adt, &lex);
				break;
			default:
				result = SYNTAX_ERROR;
			}
			break;
		case CREATE:
			result = parse_create(adt, &lex);
			break;
		case REMOVE:
			result = parse_remove(adt, &lex);
			break;
		case INSERT:
			result = parse_insert(adt, &lex);
			break;
		case SELECT:
			result = parse_select(adt, &lex);
			break;
		case REMAIN:
			result = parse_remain(adt, &lex);

		/* FALLTHROUGH */

		case NONE:
		case COMMENT:
			result = STATUS_OK;
		case END:
			break;
		default:
			result = SYNTAX_ERROR;
		}
	}

	if (AQL_ERROR(result)) {
		DB_LOG_E("Error in function %s, line %d: input \"%s\"\n", error_function, error_line, error_message);
	}

	return result;
}
