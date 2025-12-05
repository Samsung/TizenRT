// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <driver/psram_types.h>


#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief psram API
 * @defgroup bk_api_psram PSRAM API group
 * @{
 */

/**
 * @brief     write one byte of data to psram
 *
 * @param addr the psram address to write to
 * @param value the value of one byte of data
 *
 * @return
 *    - NULL
 */
static inline void bk_psram_byte_write(unsigned char *addr, unsigned char value)
{
	uint32_t addr_align = (uint32_t)addr;
	uint32_t temp = 0;
	uint8_t index = 0;

	index = (addr_align & 0x3);
	addr_align &= 0xFFFFFFFC;
	temp = *((volatile uint32_t *)addr_align);
	uint32_t set_value = value;
	switch (index)
	{
		case 0:
			temp &= ~(0xFF);
			temp |= set_value;
			break;
		case 1:
			temp &= ~(0xFF << 8);
			temp |= (set_value << 8);
			break;
		case 2:
			temp &= ~(0xFF << 16);
			temp |= (set_value << 16);
			break;
		case 3:
			temp &= ~(0xFF << 24);
			temp |= (set_value << 24);
		default:
			break;
	}

	*((volatile uint32_t *)addr_align) = temp;
}

/**
 * @brief     write two bytes of data to psram
 *
 * @param addr the psram address to write to
 * @param value the value of two bytes of data
 *
 * @return
 *    - NULL
 */
static inline void bk_psram_half_word_write(unsigned short *dst, unsigned short value)
{
	int index = (unsigned int)dst & 0x03;
	unsigned int *dst_t = (unsigned int*)((unsigned int)dst & 0xFFFFFFFC);
	unsigned int load[2] = {0};
	unsigned short *p;

	if (index < 3)
	{
		load[0] = *dst_t;

		p = (unsigned short*)((unsigned char *)&load[0] + index);
		*p = value;
		*dst_t = load[0];
	}
	else
	{
		load[0] = *dst_t;
		load[1] = *(dst_t + 1);

		p = (unsigned short*)((unsigned char *)&load[0] + index);
		*p = value;

		*dst_t = load[0];
		*(dst_t  + 1) = load[1];
	}

}

/**
 * @brief     write one word of data to psram
 *
 * @param addr the psram address to write to
 * @param value the value of one word of data
 *
 * @return
 *    - NULL
 */
static inline void bk_psram_word_write(unsigned int *src, unsigned int value)
{
	int index = (unsigned int)src & 0x03;
	unsigned int *dst = (unsigned int*)((unsigned int)src & 0xFFFFFFFC);

	if (!index)
	{
		*dst = value;
	}
	else
	{
		unsigned int load[2] = {0};
		unsigned int *p_dst = (unsigned int*)(((unsigned char*)&load[0]) + index);

		load[0] = *dst;
		load[1] = *(dst + 1);
		*p_dst = value;

		*dst = load[0];
		*(dst + 1) = load[1];

	}
}

/**
 * @brief     psram memory copy
 *
 * @param dst_t the destination address to be copied to
 * @param src_t the source address to be copied
 * @param length length of the data copy
 *
 * @attation 1. length unit is byte
 *
 * @return
 *    - NULL
 */
static inline void bk_psram_word_memcpy(void *dst_t, void *src_t, unsigned int length)
{
	unsigned int *dst = (unsigned int*)dst_t, *src = (unsigned int*)src_t;
	int index = (unsigned int)dst & 0x03;
	int count = length >> 2, tail = length & 0x3;
	unsigned int tmp = 0;
	unsigned char *p, *pp = (unsigned char *)src;

	if (!index)
	{
		while (count--)
		{
			*dst++ = pp[0] | pp[1] << 8 | pp[2] << 16 | pp[3] << 24;
			pp += 4;
		}

		if (tail)
		{
			tmp = *dst;
			p = (unsigned char *)&tmp;

			while(tail--)
			{
				*p++ = *pp++;
			}
			*dst = tmp;
		}
	}
	else
	{
		unsigned int *pre_dst = (unsigned int*)((unsigned int)dst & 0xFFFFFFFC);
		unsigned int pre_count = 4 - index;
		tmp = *pre_dst;
		p = (unsigned char *)&tmp + index;

		if (pre_count > length) {
			pre_count = length;
		}

		while (pre_count--)
		{
			*p++ = *pp++;
			length--;
		}

		*pre_dst = tmp;

		if (length <= 0)
		{
			return;
		}

		dst = pre_dst + 1;
		count = length >> 2;
		tail = length & 0x3;

		while (count--)
		{
			*dst++ = pp[0] | pp[1] << 8 | pp[2] << 16 | pp[3] << 24;
			pp += 4;
		}

		if (tail)
		{
			tmp = *dst;
			p = (unsigned char *)&tmp;

			while(tail--)
			{
				*p++ = *pp++;
			}
			*dst = tmp;
		}
	}
}
/**
 * @brief get psram heap init flag
 *
 * This API to get psram heap init flag:
 *   - init: true:init ;false:uninit
 * 
 * @return
 *  heap_init_flag
 * 
 */
bool bk_psram_heap_init_flag_get(void);
/**
 * @brief set psram heap init flag
 *
 * This API set psram heap init flag:
 *   - init: true:init;false:uninit
 *
 * This API should be called before using the psram heap.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_heap_init_flag_set(bool init);

/**
 * @brief detect psram id
 *
 * This API get psram chip id from easy flash, if easy flash function enable
 *
 * This API should be called when chip boot, must before bk_psram_init.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_id_auto_detect(void);

/**
 * @brief     Init the PSRAM HW
 *
 * This API init the resoure common to all PSRAM:
 *   - Psram power up, configure the clock control register
 *   - Select a frequency multiple
 *   - Psram config
 *
 * This API should be called before any other PSRAM APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_init(void);

/**
 * @brief     Deinit the PSRAM HW
 *
 * This API free the resoure common to PSRAM:
 *   - Init PSRAM HW control register
 *   - power down PSRAM voltage
 *
 * @attation 1. after calling this function, this api bk_psram_resume can not enable psram function
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_deinit(void);

/**
 * @brief     continue write data to psram
 *
 * This API will write more fast than dtim
 *
 * @param start_addr write to psram start addr
 * @param data_buf data buffer pointer
 * @param len data len need write to psram
 *
 * @attation 1. len uint byte
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_memcpy(uint8_t *start_addr, uint8_t *data_buf, uint32_t len);

/**
 * @brief     continue read data from psram
 *
 * This API will read more fast than dtim
 *
 * @param start_addr read from psram start addr
 * @param data_buf data buffer pointer
 * @param len data len need read from psram
 *
 * @attation 1. len uint byte
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_memread(uint8_t *start_addr, uint8_t *data_buf, uint32_t len);

/**
 * @brief     set psram clk
 *
 * This API used to set psram work clk
 *
 * @param clk:80/120/160/240
 *
 * @attation 1. current only support 80/120/160
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_set_clk(psram_clk_t clk);

/**
 * @brief     set psram voltage
 *
 * This API used to set psram work voltage
 *
 * @param voltage 1.8/2.0/3.0/3.2
 *
 * @attation 1. current only used 1.8v
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_set_voltage(psram_voltage_t voltage);

/**
 * @brief     string cat to psram
 *
 * This API is like strcat
 *
 * @param start_addr write to psram start addr
 * @param data_buf string buffer pointer
 *
 * @return psram start addr
 */
char *bk_psram_strcat(char *start_addr, const char *data_buf);

/**
 * @brief     alloc write_through_area
 *
 * This API is alloc a area
 *
 * @attention 1. The write through area is used for DMA2D accessing only, do not use it for CPU
 *               accessing PSRAM
 *
 * @return channel
 */
psram_write_through_area_t bk_psram_alloc_write_through_channel(void);

/**
 * @brief     free write_through_area
 *
 * This API is free a area
 *
 * @attention 1. The write through area is used for DMA2D accessing only, do not use it for CPU
 *               accessing PSRAM
 *
 * @return
 *
 *    - BK_OK: succeed
 *    - BK_ERR_PSRAM_ADDR_ALIGN: start or end is not 32 bytes aligned
 *    - BK_ERR_PSRAM_ADDR_OUT_OF_RANGE: start or end is invalid PSRAM address
 *    - BK_ERR_PSRAM_ADDR_RELATION: start >= end
 *    - BK_ERR_PSRAM_AREA: invalid area
 */
bk_err_t bk_psram_free_write_through_channel(psram_write_through_area_t area);

/**
 * @brief     Enable and config PSRAM write through area
 *
 * This API enables PSRAM write through area. Each time the DMA2D writes
 * the area with write through enabled, the PSRAM hardware will always flush
 * it's internal cache-line, without considering whether the cache line is full
 * or not, thus to improve the writing performance.
 *
 * @attention 1. The write through area is used for DMA2D accessing only, do not use it for CPU
 *               accessing PSRAM
 *
 * @param area PSRAM write through area
 * @param start PSRAM write through area start address, should be 32 bytes aligned
 * @param start PSRAM write through area end address, should be 32 bytes aligned
 *
 * @return
 *
 *    - BK_OK: succeed
 *    - BK_ERR_PSRAM_ADDR_ALIGN: start or end is not 32 bytes aligned
 *    - BK_ERR_PSRAM_ADDR_OUT_OF_RANGE: start or end is invalid PSRAM address
 *    - BK_ERR_PSRAM_ADDR_RELATION: start >= end
 *    - BK_ERR_PSRAM_AREA: invalid area
 */
bk_err_t bk_psram_enable_write_through(psram_write_through_area_t area, uint32_t start, uint32_t end);


/**
 * @brief     Disable PSRAM write through area
 *
 *
 * @param area PSRAM write through area id
 *
 * @return
 *
 *    - BK_OK: succeed
 *    - BK_ERR_PSRAM_AREA: invalid area
 */
bk_err_t bk_psram_disable_write_through(psram_write_through_area_t area);

bk_err_t bk_psram_calibrate(void);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

