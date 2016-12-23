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
 *      Utilities for building the internal representation of an AQL command.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "aql.h"
#include "db_debug.h"


/****************************************************************************
 * Private Functions
 ****************************************************************************/
static aql_attribute_t *get_attribute(aql_adt_t *adt, char *name)
{
	int i;

	for (i = 0; i < AQL_ATTRIBUTE_COUNT(adt); i++) {
		if (strncmp(adt->attributes[i].name, name, ATTRIBUTE_NAME_LENGTH) == 0) {
			return &adt->attributes[i];
		}
	}
	return NULL;
}

void aql_clear(aql_adt_t *adt)
{
	adt->optype = AQL_TYPE_NONE;
	adt->relation_count = 0;
	adt->attribute_count = 0;
	adt->value_count = 0;
	adt->flags = 0;
	memset(adt->aggregators, 0, sizeof(adt->aggregators));
}

void aql_add_relation(aql_adt_t *adt, char *rel)
{
	if (adt->relation_count < AQL_RELATION_LIMIT - 1) {
		int len;
		len = strlen(rel) + 1;
		snprintf(adt->relations[adt->relation_count++], len, "%s\0", rel);
	}
}

db_result_t aql_add_attribute(aql_adt_t *adt, char *name, domain_t domain, unsigned element_size, int processed_only)
{
	aql_attribute_t *attr;

	if (adt->attribute_count == AQL_ATTRIBUTE_LIMIT) {
		return DB_LIMIT_ERROR;
	}

	if (processed_only && get_attribute(adt, name)) {
		/* No need to have multiple instances of attributes that are only
		   used for processing in the PLE. */
		return DB_OK;
	}

	attr = &adt->attributes[adt->attribute_count++];

	if (strlen(name) + 1 > sizeof(attr->name)) {
		return DB_LIMIT_ERROR;
	}

	strncpy(attr->name, name, sizeof(attr->name));
	attr->name[sizeof(attr->name) - 1] = '\0';
	attr->domain = domain;
	attr->element_size = element_size;
	attr->flags = processed_only ? ATTRIBUTE_FLAG_NO_STORE : 0;

	return DB_OK;
}

db_result_t aql_add_value(aql_adt_t *adt, domain_t domain, void *value_ptr)
{
	attribute_value_t *value;
	unsigned char *str;
	int str_size;

	if (adt->value_count == AQL_ATTRIBUTE_LIMIT) {
		return DB_LIMIT_ERROR;
	}

	value = &adt->values[adt->value_count++];
	value->domain = domain;

	switch (domain) {
	case DOMAIN_INT:
		VALUE_LONG(value) = *(long *)value_ptr;
		break;
	case DOMAIN_STRING:
		str_size = strlen((const char *)value_ptr);
		str = (unsigned char *)malloc(sizeof(char) * str_size + 1);
		if (str != NULL) {
			memcpy(str, value_ptr, str_size);
			str[str_size] = '\0';
			VALUE_STRING(value) = (unsigned char *)str;
			if (VALUE_STRING(value) != NULL) {
				break;
			}
		}
	default:
		return DB_TYPE_ERROR;
	}

	return DB_OK;
}
