// Copyright 2020-2021 Beken
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

static inline void bk_psram_byte_write(unsigned char *addr, unsigned char value)
{
	uint32_t addr_align = (uint32_t)addr;
	uint32_t temp = 0;
	uint8_t index = 0;

	index = (addr_align & 0x3);
	addr_align &= 0xFFFFFFFC;
	temp = *((volatile uint32_t *)addr_align);
	switch (index)
	{
		case 0:
			temp &= ~(0xFF);
			temp |= value;
			break;
		case 1:
			temp &= ~(0xFF << 8);
			temp |= (value << 8);
			break;
		case 2:
			temp &= ~(0xFF << 16);
			temp |= (value << 16);
			break;
		case 3:
			temp &= ~(0xFF << 24);
			temp |= (value << 24);
		default:
			break;
	}

	*((volatile uint32_t *)addr_align) = temp;
}

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


/* @brief Overview about this API header
 *
 */

/**
 * @brief psram API
 * @defgroup bk_api_psram PSRAM API group
 * @{
 */

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
 * @brief     close psram function
 *
 * This API will total clear psram function, cannot called repeate whitout called bk_psram_resume
 *
 * @attation 1. if call bk_psram_resume psram will enable function again.
 * @attation 2. this api will only valied when bk_psram_init have been called before
 *
 * @return void
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_suspend(void);

/**
 * @brief     enable psram function function
 *
 * This API will total enable psram function, cannot called repeate whitout called bk_psram_suspend
 *
 * @attation 1. if call bk_psram_resume psram will enable function again
 * @attation 2. this api will only valied when bk_psram_init have been called before
 *
 * @return void
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_psram_resume(void);

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
 * @return void
 */
void bk_psram_memcpy(uint32_t start_addr, uint32_t *data_buf, uint32_t len);

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
 * @return void
 */
void bk_psram_memread(uint32_t start_addr, uint32_t *data_buf, uint32_t len);

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
 * @}
 */
#ifdef __cplusplus
}
#endif

