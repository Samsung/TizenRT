/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef __OSDEP_SERVICE_MEMORY_H_
#define __OSDEP_SERVICE_MEMORY_H_

#ifdef __cplusplus
extern "C" {
#endif
/*************************** Memory Management *******************************/

/**
 * @brief  This function allocates the virtually contiguous memory.
 * @param[in] sz: The size of memory to be allocated.
 * @return	  The pointer to the beginning of the memory
 */
void	*rtw_vmalloc(uint32_t sz);

/**
 * @brief  This function allocates the virtually contiguous memory
 *		   and the values of the memory are setted to 0.
 * @param[in] sz: The size of memory to be allocated.
 * @return	  The pointer to the beginning of the memory
 */
void	*rtw_zvmalloc(uint32_t sz);

/**
 * @brief  This function frees the virtually contiguous memory.
 * @param[in] pbuf: The pointer to the beginning of the memory to be free
 * @param[in] sz: The size of memory allocated.
 * @return	  None
 */
void	rtw_vmfree(uint8_t *pbuf, uint32_t sz);

/**
 * @brief  This function allocates the memory
 *		   and the values of the memory are setted to 0.
 * @param[in] sz: The size of memory to be allocated.
 * @return	  The pointer to the beginning of the memory
 */
void	*rtw_zmalloc(uint32_t sz);

/**
 * @brief  This function allocates the memory.
 * @param[in] sz: The size of memory to be allocated.
 * @return	  The pointer to the beginning of the memory
 */
void	*rtw_malloc(uint32_t sz);

/**
 * @brief  This function allocates the memory, nlements*elementSize bytes in size.
 * @param[in] nelements: The number of elements.
 * @param[in] elementSize: The size of each element
 * @return	  The pointer to the beginning of the memory
 */
void	*rtw_calloc(uint32_t nelements, uint32_t elmentSize);

/**
 * @brief  This function frees the virtually contiguous memory.
 * @param[in] pbuf: The pointer to the beginning of the memory to be free
 * @param[in] sz: The size of memory allocated.
 * @return	  None
 */
void	rtw_mfree(uint8_t *pbuf, uint32_t sz);

#define rtw_free(buf)		rtw_mfree((uint8_t *)buf, 0)

/**
 * @brief  This function copies the values of "sz" bytes from the location pointed to by "src"
 *         directly to the memory block pointed to by "des".
 * @param[in] dst: Pointer to the destination array where the content is to be copied, type-casted to a pointer of type void*.
 * @param[in] src: Pointer to the source of data to be copied, type-casted to a pointer of type void*.
 * @param[in] sz: Size of memory to copy.
 * @return	  None
 */
void	rtw_memcpy(void *dst, void *src, uint32_t sz);

/**
 * @brief  This function compares the first "sz" bytes of the block of memory pointed by "dst"
 *		   to the first "sz" bytes pointed by "src".
 * @param[in] dst: Pointer to block of memory to be compared.
 * @param[in] src: pointer to block of memory to compare.
 * @param[in] sz: Size of memory to compare.
 * @return	<0: The first byte that does not match in both memory blocks has a lower value in dst than in src.
 * @return	0: The contents of both memory blocks are equal.
 * @return	<0: The first byte that does not match in both memory blocks has a greater value in dst than in src.
 */
int	rtw_memcmp(void *dst, void *src, uint32_t sz);

/**
 * @brief  This function sets the first "sz" bytes of the block of memory pointed by "pbuf" to the specified "c".
 * @param[in] pbuf: Pointer to the block of memory to fill.
 * @param[in] c: Value to be set.
 * @param[in] sz: Size of memory to be set to the value "c".
 * @return	  None
 */
void	rtw_memset(void *pbuf, int c, uint32_t sz);
/*************************** End Memory Management *******************************/
#ifdef __cplusplus
}
#endif

#endif
