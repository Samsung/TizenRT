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
/****************************************************************************
 * Copyright (c) 2010--2011, Olaf Bergmann
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  o Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *  o Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 ****************************************************************************/
/* t_list -- tinydtls lists
 */

/**
 * @file t_list.h
 * @brief Wrappers for list structures and functions
 */

#ifndef _DTLS_LIST_H_
#define _DTLS_LIST_H_

#ifndef WITH_CONTIKI
#include "uthash.h"
#include "utlist.h"

/* We define list structures and utility functions to be compatible
 * with Contiki list structures. The Contiki list API is part of the
 * Contiki operating system, and therefore the following licensing
 * terms apply (taken from contiki/core/lib/list.h):
 *
 * Copyright (c) 2004, Swedish Institute of Computer Science.
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
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $ Id: list.h,v 1.5 2010/09/13 13:31:00 adamdunkels Exp $
 */

typedef void **list_t;
struct list {
	struct list *next;
};

#define LIST_CONCAT(s1, s2) s1##s2

#define LIST_STRUCT(name)			\
  void *LIST_CONCAT(name, _list);		\
  list_t name

#define LIST_STRUCT_INIT(struct_ptr, name)  {				\
    (struct_ptr)->name = &((struct_ptr)->LIST_CONCAT(name,_list));	\
    (struct_ptr)->LIST_CONCAT(name,_list) = NULL;			\
  }

static inline void *list_head(list_t the_list)
{
	return *the_list;
}

static inline void list_remove(list_t the_list, void *item)
{
	if (list_head(the_list)) {
		LL_DELETE(*(struct list **)the_list, (struct list *)item);
	}
}

static inline void list_add(list_t the_list, void *item)
{
	list_remove(the_list, item);
	LL_APPEND(*(struct list **)the_list, (struct list *)item);
}

static inline void list_push(list_t the_list, void *item)
{
	LL_PREPEND(*(struct list **)the_list, (struct list *)item);
}

static inline void *list_pop(list_t the_list)
{
	struct list *l;
	l = (struct list *)*the_list;
	if (l) {
		list_remove(the_list, l);
	}

	return l;
}

static inline void list_insert(list_t the_list, void *previtem, void *newitem)
{
	if (previtem == NULL) {
		list_push(the_list, newitem);
	} else {
		((struct list *)newitem)->next = ((struct list *)previtem)->next;
		((struct list *)previtem)->next = (struct list *)newitem;
	}
}

static inline void *list_item_next(void *item)
{
	return item == NULL ? NULL : ((struct list *)item)->next;
}

#else							/* WITH_CONTIKI */
#include "list.h"
#endif							/* WITH_CONTIKI */

#endif							/* _DTLS_LIST_H_ */
