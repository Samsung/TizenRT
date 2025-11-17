/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>
#include <tinyara/mm/mm.h>

void free_user_at(struct mm_heap_s *heap, void *mem)
{
	mm_free(heap, mem);
}

/************************************************************************
 * Name: calloc_user_at
 *
 * Description:
 *   This is a simple wrapper for the mm_calloc() function.
 *   This function is used to allocate the memory from kernel side
 *   to specified user heap.
 *
 ************************************************************************/
void *calloc_user_at(struct mm_heap_s *heap, size_t n, size_t elem_size)
{
	if (n == 0 || elem_size == 0) {
		return NULL;
	}

	mmaddress_t caller_retaddr = NULL;	/* for generalising the call to mm_calloc api */
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	caller_retaddr = GET_RETURN_ADDRESS();
#endif

	return mm_calloc(heap, n, elem_size, caller_retaddr);
}

/************************************************************************
 * Name: memalign_user_at
 *
 * Description:
 *   This is a simple wrapper for the mm_memalign() function.
 *   This function is used to allocate the memory from kernel side
 *   to specified user heap.
 *
 ************************************************************************/
void *memalign_user_at(struct mm_heap_s *heap, size_t alignment, size_t size)
{
	if (size == 0) {
		return NULL;
	}

	mmaddress_t caller_retaddr = NULL;	/* for generalising the call to mm_memalign api */
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	caller_retaddr = GET_RETURN_ADDRESS();
#endif

	return mm_memalign(heap, alignment, size, caller_retaddr);
}

/************************************************************************
 * Name: malloc_user_at
 *
 * Description:
 *   This is a simple wrapper for the mm_malloc() function.
 *   This function is used to allocate the memory from kernel side
 *   to specified user heap.
 *
 * Parameters:
 *   heap - User heap to allocate
 *   size  - The size (in bytes)
 *
 * Return Value:
 *   The starting address of allocated memory on success, NULL on failure.
 *
 ************************************************************************/
void *malloc_user_at(struct mm_heap_s *heap, size_t size)
{
	if (size == 0) {
		return NULL;
	}

	mmaddress_t caller_retaddr = NULL;	/* for generalising the call to mm_malloc api */
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	caller_retaddr = GET_RETURN_ADDRESS();
#endif

	return mm_malloc(heap, size, caller_retaddr);
}

/************************************************************************
 * Name: realloc_user_at
 *
 * Description:
 *   This is a simple wrapper for the mm_realloc() function.
 *   This function is used to allocate the memory from kernel side
 *   to specified user heap.
 *
 ************************************************************************/
void *realloc_user_at(struct mm_heap_s *heap, void *oldmem, size_t newsize)
{
	if (newsize == 0) {
		free_user_at(heap, oldmem);
		return NULL;
	}

	mmaddress_t caller_retaddr = NULL;	/* for generalising the call to mm_realloc api */
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	caller_retaddr = GET_RETURN_ADDRESS();
#endif

	return mm_realloc(heap, oldmem, newsize, caller_retaddr);
}

/************************************************************************
 * Name: zalloc_user_at
 *
 * Description:
 *   This is a simple wrapper for the mm_zalloc() function.
 *   This function is used to allocate the memory from kernel side
 *   to specified user heap.
 *
 ************************************************************************/
void *zalloc_user_at(struct mm_heap_s *heap, size_t size)
{
	if (size == 0) {
		return NULL;
	}

	mmaddress_t caller_retaddr = NULL;	/* for generalising the call to mm_zalloc api */
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	caller_retaddr = GET_RETURN_ADDRESS();
#endif

	return mm_zalloc(heap, size, caller_retaddr);
}
