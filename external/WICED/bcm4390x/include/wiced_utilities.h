/****************************************************************************
 *
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
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

#pragma once

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 * @cond                       Macros
 ******************************************************/

#ifdef LINT
/* Lint does not know about inline functions */
extern uint16_t htobe16(uint16_t v);
extern uint32_t htobe32(uint32_t v);

#else							/* ifdef LINT */

#endif							/* ifdef LINT */

#ifndef MIN
#define MIN(x,y)  ((x) < (y) ? (x) : (y))
#endif							/* ifndef MIN */

#ifndef MAX
#define MAX(x,y)  ((x) > (y) ? (x) : (y))
#endif							/* ifndef MAX */

#ifndef ROUND_UP
#define ROUND_UP(x, y)    ((x) % (y) ? (x) + (y) - ((x) % (y)) : (x))
#endif							/* ifndef ROUND_UP */

#ifndef DIV_ROUND_UP
#define DIV_ROUND_UP(m, n)    (((m) + (n) - 1) / (n))
#endif							/* ifndef DIV_ROUND_UP */

#define WICED_VERIFY(x)                               {wiced_result_t res = (x); if (res != WICED_SUCCESS){return res;}}

#define WICED_VERIFY_GOTO( expr, res_var, label )     {res_var = (expr); if (res_var != WICED_SUCCESS){goto label;}}

#define MEMCAT(destination, source, source_length)    (void*)((uint8_t*)memcpy((destination),(source),(source_length)) + (source_length))

#define MALLOC_OBJECT(name,object_type)               ((object_type*)malloc_named(name,sizeof(object_type)))

#define OFFSET(type, member)                          ((uint32_t)&((type *)0)->member)

#define ARRAY_SIZE(a)                                ( sizeof(a) / sizeof(a[0]) )
#define ARRAY_POSITION( array, element_pointer )     ( ((uint32_t)element_pointer - (uint32_t)array) / sizeof(array[0]) )

/* Macros for comparing MAC addresses */
#define CMP_MAC( a, b )  (((((unsigned char*)a)[0])==(((unsigned char*)b)[0]))&& \
                          ((((unsigned char*)a)[1])==(((unsigned char*)b)[1]))&& \
                          ((((unsigned char*)a)[2])==(((unsigned char*)b)[2]))&& \
                          ((((unsigned char*)a)[3])==(((unsigned char*)b)[3]))&& \
                          ((((unsigned char*)a)[4])==(((unsigned char*)b)[4]))&& \
                          ((((unsigned char*)a)[5])==(((unsigned char*)b)[5])))

#define NULL_MAC( a )  (((((unsigned char*)a)[0])==0)&& \
                        ((((unsigned char*)a)[1])==0)&& \
                        ((((unsigned char*)a)[2])==0)&& \
                        ((((unsigned char*)a)[3])==0)&& \
                        ((((unsigned char*)a)[4])==0)&& \
                        ((((unsigned char*)a)[5])==0))

#define MEMORY_BARRIER_AGAINST_COMPILER_REORDERING()  __asm__ __volatile__ ("" : : : "memory")	/* assume registers are Device memory, so have implicit CPU memory barriers */

#define REGISTER_WRITE_WITH_BARRIER(type, address, value) do {*(volatile type *)(address) = (type)(value); MEMORY_BARRIER_AGAINST_COMPILER_REORDERING();} while (0)
#define REGISTER_READ(type, address)                      (*(volatile type *)(address))

typedef enum {
	LEAK_CHECK_THREAD,
	LEAK_CHECK_GLOBAL,
} leak_check_scope_t;

#define calloc_named(name, nelems, elemsize) calloc(nelems, elemsize)
#define calloc_named_hideleak(name, nelems, elemsize)  calloc(nelems, elemsize)
#define realloc_named(name, ptr, size)      realloc(ptr, size)
#define malloc_named(name, size)            malloc(size)
#define malloc_named_hideleak(name, size)   malloc(size)
#define malloc_set_name(name)
#define malloc_leak_set_ignored(global_flag)
#define malloc_leak_set_base(global_flag)
#define malloc_leak_check(thread, global_flag)
#define malloc_transfer_to_curr_thread(block)
#define malloc_transfer_to_thread(block, thread)
#define malloc_print_mallocs(void)
#define malloc_debug_startup_finished()

#ifdef __cplusplus
}								/*extern "C" */
#endif
