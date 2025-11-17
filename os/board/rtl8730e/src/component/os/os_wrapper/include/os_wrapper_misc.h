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

#ifndef __OS_WRAPPER_MISC_H_
#define __OS_WRAPPER_MISC_H_

#define RTK_HALT()   \
	do {         \
		cli();   \
		for (;;) \
			;    \
	} while (0)
#undef RTK_ASSERT
#define RTK_ASSERT(x)			do { \
						if((x) == 0){\
							printf("\n\rAssert(" #x ") failed on line %d in file %s\n", __LINE__, __FILE__); \
						RTK_HALT();}\
					} while(0)

#undef RTK_DBG_ASSERT
#define RTK_DBG_ASSERT(x, msg)		do { \
						if((x) == 0) \
							printf("\n\r%s, Assert(" #x ") failed on line %d in file %s", msg, __LINE__, __FILE__); \
					} while(0)



#ifdef LIST_CONTAINOR
#undef LIST_CONTAINOR
#endif
#define LIST_CONTAINOR(ptr, type, member) \
	((type *)(void*)((char *)(ptr)-(unsigned int)((char *)&((type *)(void*)ptr)->member - (char *)ptr)))
//#define container_of(p,t,n) (t*)((p)-&(((t*)0)->n))
#if !defined(container_of)
#define container_of(ptr, type, member) \
			((type *)(void*)((char *)(ptr)-(SIZE_T)(&((type *)0)->member)))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FIELD_OFFSET
#define FIELD_OFFSET(s,field)	((SSIZE_T)&((s*)(0))->field)
#endif
#define rtw_min(a, b) ((a > b) ? b : a)

/**
 * @brief  This function generates random bytes.
 * @param[in] dst: The pointer to the buffer to store the random bytes.
 * @param[in] size: The size of the random bytes.
 * @return	  0
*/
int	rtw_get_random_bytes(void *dst, uint32_t size);

/**
 * @brief  This function generates random bytes. For use of mbedtls 3.x.x
 * @param  p_rng: Unused
 * @param  output: The pointer to the buffer to store the random bytes.
 * @param  output_size: The size of the random bytes.
 * @retval
 */
int rtw_get_random_bytes_f_rng(void *p_rng, unsigned char *output, size_t output_size);

/**
 * @brief  This function gets the available heap size.
 * @return	  The value of the available heap size.
*/
uint32_t	rtw_getFreeHeapSize(void);

int rtw_in_interrupt(void);

/**
 * @brief  This function converts the initial portion of the string to integer.
 * @param[in] s: The pointer to the string to be converted.
 * @return	  The converted value.
*/
uint32_t 	rtw_atoi(uint8_t *s);

__inline static uint32_t _RND4(uint32_t sz)
{
	uint32_t	val;

	val = ((sz >> 2) + ((sz & 3) ? 1 : 0)) << 2;

	return val;
}

__inline static uint32_t _RND8(uint32_t sz)
{
	uint32_t	val;

	val = ((sz >> 3) + ((sz & 7) ? 1 : 0)) << 3;

	return val;
}

__inline static uint32_t _RND128(uint32_t sz)
{
	uint32_t	val;

	val = ((sz >> 7) + ((sz & 127) ? 1 : 0)) << 7;

	return val;
}

__inline static uint32_t _RND256(uint32_t sz)
{
	uint32_t	val;

	val = ((sz >> 8) + ((sz & 255) ? 1 : 0)) << 8;

	return val;
}

__inline static uint32_t _RND512(uint32_t sz)
{
	uint32_t	val;

	val = ((sz >> 9) + ((sz & 511) ? 1 : 0)) << 9;

	return val;
}

__inline static uint32_t bitshift(uint32_t bitmask)
{
	uint32_t i;

	for (i = 0; i <= 31; i++)
		if (((bitmask >> i) &  0x1) == 1) {
			break;
		}

	return i;
}

__inline static u16 _htons(u16 n)
{
	return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

__inline static u16 _ntohs(u16 n)
{
	return _htons(n);
}

__inline static u32 _htonl(u32 n)
{
	return ((n & 0xff) << 24) |
		   ((n & 0xff00) << 8) |
		   ((n & 0xff0000UL) >> 8) |
		   ((n & 0xff000000UL) >> 24);
}

__inline static u32 _ntohl(u32 n)
{
	return _htonl(n);
}
#ifdef __cplusplus
}
#endif

#endif
