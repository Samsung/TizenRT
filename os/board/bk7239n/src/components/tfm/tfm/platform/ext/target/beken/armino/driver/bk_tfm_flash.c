// Copyright 2022-2023 Beken
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
#include "components/log.h"
#include "driver/flash.h"
#include "soc/soc.h"
#include "bk_tfm_flash.h"
#include "cmsis.h"

#define TAG "bk_tfm_flash"

// FLASH Configurable options for debug
#define BK_TFM_FLASH_ERASE_USE_STATIC_BUF  1
#define BK_TFM_FLASH_CHECK_API_RESULT      0

// Log options
#define BK_TFM_FLASH_LOGD BK_LOGD
#define BK_TFM_FLASH_LOGI BK_LOGI
#define BK_TFM_FLASH_LOGW BK_LOGW
#define BK_TFM_FLASH_LOGE BK_LOGE

// FLASH constants
#define BK_TFM_FLASH_SECTOR_SIZE           0x1000
#define BK_TFM_FLASH_ERASE_SECTOR_MASK     0xFFF000
#define BK_TFM_FLASH_SECTOR_SIZE_MASK      0x000FFF
#define BK_TFM_FLASH_RW_UNIT_LEN           32
#define BK_TFM_FLASH_INTERNAL_OFFSET_MASK  0xFFFFFF
#define BK_TFM_FLASH_ERASED_VALUE          0xFF

static inline uint32_t flash_base_addr(void)
{
	return SOC_FLASH_DATA_BASE;
}

static inline bool flash_is_crc_aligned(uint32_t addr)
{
	if ((addr % 32) == 0)
		return true;
	else
		return false;
}

static inline uint32_t flash_cpu_to_physical_addr(uint32_t addr)
{
	uint32_t base = flash_base_addr();
	uint32_t offset = addr & BK_TFM_FLASH_INTERNAL_OFFSET_MASK;
	return base + (offset + (offset >> 4));
}

static inline uint32_t flash_physical_to_cpu_addr(uint32_t addr)
{
	uint32_t base = flash_base_addr();
	uint32_t offset = addr & BK_TFM_FLASH_INTERNAL_OFFSET_MASK;
	return base + ((offset / 17) << 4);
}

static inline uint32_t flash_cpu_to_physical_len(uint32_t len)
{
	return len + (len >> 4);
}

static inline uint32_t flash_physical_to_cpu_len(uint32_t len)
{
	return (len / 17) << 4;
}

#if BK_TFM_FLASH_CHECK_API_RESULT

static bool flash_is_buf_erased(const uint8_t* buf, uint32_t len)
{
	for (int i = 0; i < len; i++) {
		if (buf[i] != BK_TFM_FLASH_ERASED_VALUE) return false;
	}
	return true;
}

static bool flash_is_range_erased(uint32_t addr, uint32_t len)
{
	uint8_t buf[BK_TFM_FLASH_RW_UNIT_LEN] = {BK_TFM_FLASH_ERASED_VALUE};
	uint32_t off = 0;
	bool erased = true;

	while ( (off < len) && (len - off) >= BK_TFM_FLASH_RW_UNIT_LEN) {
		bk_flash_read_bytes(addr + off, buf, BK_TFM_FLASH_RW_UNIT_LEN);
		off += BK_TFM_FLASH_RW_UNIT_LEN;
		if (!flash_is_buf_erased(buf, BK_TFM_FLASH_RW_UNIT_LEN)) {
			BK_TFM_FLASH_LOGI(TAG, "off=%x is not erased\r\n", addr + off);
			erased = false;
		}
	}

	if ((off < len) && (len - off)) {
		bk_flash_read_bytes(addr + off, buf, (len - off));
		if (!flash_is_buf_erased(buf, len - off)) {
			BK_TFM_FLASH_LOGI(TAG, "off=%x is not erased\r\n", addr + off);
			erased = false;
		}
	}

	if (erased)
		BK_TFM_FLASH_LOGI(TAG, "erased addr=%x OK\r\n", addr);
	else
		BK_TFM_FLASH_LOGI(TAG, "erased addr=%x NOK\r\n", addr);

	return erased;
}
#define FLASH_DEBUG_CHECK_ERASED_RANGE(addr, len) flash_is_range_erased((addr), (len))
#else
#define FLASH_DEBUG_CHECK_ERASED_RANGE(addr, len)
#endif

static bk_err_t bk_flash_erase_one_sector_partially(uint32_t sector_start_addr,
	uint32_t head_unerase_len, uint32_t tail_unerase_len)
{
#if BK_TFM_FLASH_ERASE_USE_STATIC_BUF
	static uint8_t s_buf[BK_TFM_FLASH_SECTOR_SIZE] = {0};
#endif
	uint8_t *header_buf = NULL;
	uint8_t *tail_buf = NULL;
	bk_err_t ret = BK_OK;

	BK_TFM_FLASH_LOGD(TAG, "partial erase, start=%x head_unerase=%x tail_unerase=%x\r\n",
		sector_start_addr, head_unerase_len, tail_unerase_len);

	//TODO sanity check start_addr
	if ((head_unerase_len + tail_unerase_len) > BK_TFM_FLASH_SECTOR_SIZE) {
		BK_TFM_FLASH_LOGE(TAG, "invalid unerase len, head=%x tail=%x\r\n", head_unerase_len, tail_unerase_len);
		return BK_ERR_FLASH_LEN_TOO_BIG;
	}

	if ((head_unerase_len + tail_unerase_len) > 0) {

#if BK_TFM_FLASH_ERASE_USE_STATIC_BUF
		header_buf = &s_buf[0];
		tail_buf = &s_buf[head_unerase_len];
#else
		header_buf = os_malloc(head_unerase_len + tail_unerase_len);
		if (header_buf) {
			tail_buf = header_buf + head_unerase_len;
		}

		if (!header_buf) {
			BK_TFM_FLASH_LOGE(TAG, "erase range: oom\r\n");
			return BK_ERR_NO_MEM;
		}
#endif
	} else {
		BK_TFM_FLASH_LOGW(TAG, "not partial erase\r\n");
	}

	if (head_unerase_len) {
		os_memset(header_buf, 0xFF, head_unerase_len);
		bk_flash_read_bytes(sector_start_addr, header_buf, head_unerase_len);
	}

	uint32_t tail_unerase_addr = sector_start_addr + BK_TFM_FLASH_SECTOR_SIZE - tail_unerase_len;
	if (tail_unerase_len) {
		os_memset(tail_buf, 0xFF, tail_unerase_len);
		bk_flash_read_bytes(tail_unerase_addr, tail_buf, tail_unerase_len);
	}

	BK_TFM_FLASH_LOGD(TAG, "erase sector %x\r\n", sector_start_addr);
	bk_flash_erase_sector(sector_start_addr);

	if (head_unerase_len)
		bk_flash_write_bytes(sector_start_addr, header_buf, head_unerase_len);

	if (tail_unerase_len)
		bk_flash_write_bytes(tail_unerase_addr, tail_buf, tail_unerase_len);

#if !BK_TFM_FLASH_ERASE_USE_STATIC_BUF
	if (header_buf)
		os_free(header_buf);
#endif

	return ret;
}

bk_err_t bk_flash_erase_range(uint32_t start_addr, uint32_t len)
{
	start_addr &= BK_TFM_FLASH_INTERNAL_OFFSET_MASK;
	uint32_t first_sector_start_addr = start_addr & BK_TFM_FLASH_ERASE_SECTOR_MASK;
	uint32_t first_sector_end_addr = first_sector_start_addr + BK_TFM_FLASH_SECTOR_SIZE -1;
	uint32_t first_sector_unerase_len = start_addr - first_sector_start_addr;
	uint32_t end_addr = start_addr + len - 1; //indicate last byte to be erased
	uint32_t last_sector_start_addr = end_addr & BK_TFM_FLASH_ERASE_SECTOR_MASK;
	uint32_t last_sector_end_addr = last_sector_start_addr + BK_TFM_FLASH_SECTOR_SIZE - 1;
	uint32_t last_sector_unerase_len = last_sector_end_addr - end_addr;
	uint32_t max_unerase_len = first_sector_unerase_len > last_sector_unerase_len ? first_sector_unerase_len : last_sector_unerase_len;
	uint32_t sector_start_addr;

	BK_TFM_FLASH_LOGD(TAG, "start=%x len=%x end=%x\r\n", start_addr, len, end_addr);
	BK_TFM_FLASH_LOGD(TAG, "1st_start=%x 1st_end=%x 1st_align_len=%x\r\n",
		first_sector_start_addr, first_sector_end_addr, first_sector_unerase_len);
	BK_TFM_FLASH_LOGD(TAG, "last_start=%x last_end=%x last_align_len=%x\r\n",
		last_sector_start_addr, last_sector_end_addr, last_sector_unerase_len);

	if (len <= 0) {
		BK_TFM_FLASH_LOGW(TAG, "start=%x equals to end=%x\r\n", start_addr, end_addr);
		return BK_OK;
	}

	if (max_unerase_len == 0) {
		BK_TFM_FLASH_LOGD(TAG, "erase 1st sector %x\r\n", first_sector_start_addr);
		bk_flash_erase_sector(first_sector_start_addr);

		if (last_sector_start_addr != first_sector_start_addr) {
			BK_TFM_FLASH_LOGD(TAG, "erase last sector %x\r\n", first_sector_start_addr);
			bk_flash_erase_sector(last_sector_start_addr);
		}
	} else {
		if (first_sector_start_addr == last_sector_start_addr) {
			BK_TFM_FLASH_LOGD(TAG, "1st and last sector is same\r\n");
			return bk_flash_erase_one_sector_partially(first_sector_start_addr, first_sector_unerase_len, last_sector_unerase_len);
		} else {
			if (first_sector_unerase_len) {
				bk_flash_erase_one_sector_partially(first_sector_start_addr, first_sector_unerase_len, 0);
			} else {
				bk_flash_erase_sector(first_sector_start_addr);
			}

			if (last_sector_unerase_len) {
				bk_flash_erase_one_sector_partially(last_sector_start_addr, 0, last_sector_unerase_len);
			} else {
				bk_flash_erase_sector(last_sector_start_addr);
			}
		}
	}

	sector_start_addr = first_sector_end_addr + 1;
	while (sector_start_addr < last_sector_start_addr) {
		BK_TFM_FLASH_LOGD(TAG, "erase sector %x\r\n", sector_start_addr);
		bk_flash_erase_sector(sector_start_addr);
		sector_start_addr += BK_TFM_FLASH_SECTOR_SIZE;
	}

	FLASH_DEBUG_CHECK_ERASED_RANGE(start_addr, len);
	return BK_OK;
}

bk_err_t bk_flash_erase_range_with_crc(uint32_t start_addr, uint32_t len)
{
	if (len == 0) {
		BK_TFM_FLASH_LOGW(TAG, "erase 0B\r\n");
		return BK_OK;
	}

	if (flash_is_crc_aligned(start_addr) && flash_is_crc_aligned(len)) {
		uint32_t physical_start_addr = flash_cpu_to_physical_addr(start_addr);
		uint32_t physical_len = flash_cpu_to_physical_len(len);
		BK_TFM_FLASH_LOGD(TAG, "erase with crc, cpu: off=%x, len=%x, physical: off=%x, len=%x\r\n", start_addr, len, physical_start_addr, physical_len);
		return bk_flash_erase_range(physical_start_addr, physical_len);
	} else {
		BK_TFM_FLASH_LOGE(TAG, "addr is not CRC aligned, start=%x len=%x\r\n", start_addr, len);
		return BK_ERR_FLASH_ADDR_NOT_CRC_ALIGNED;
	}
}

bk_err_t bk_flash_copy(uint32_t src_addr, uint32_t dst_addr, uint32_t len)
{
	uint32_t copied_len = 0;

	while (len >= BK_TFM_FLASH_RW_UNIT_LEN) {
		flash_copy_one_unit(src_addr + copied_len, dst_addr + copied_len, BK_TFM_FLASH_RW_UNIT_LEN);
		len -= BK_TFM_FLASH_RW_UNIT_LEN;
		copied_len += BK_TFM_FLASH_RW_UNIT_LEN;
	}

	if (len > 0) {
		flash_copy_one_unit(src_addr + copied_len, dst_addr + copied_len, len);
	}

	return BK_OK;
}

bk_err_t bk_flash_copy_with_crc(uint32_t src_addr, uint32_t dst_addr, uint32_t len)
{
	BK_TFM_FLASH_LOGD(TAG, "copy with crc: src=%x dst=%x len=%x\r\n", src_addr, dst_addr, len);
	if (flash_is_crc_aligned(src_addr) && flash_is_crc_aligned(dst_addr)
		&& flash_is_crc_aligned(len)) {
		uint32_t physical_src = flash_cpu_to_physical_addr(src_addr);
		uint32_t physical_dst = flash_cpu_to_physical_addr(dst_addr);
		uint32_t physical_len = flash_cpu_to_physical_len(len);
		return bk_flash_copy(physical_src, physical_dst, physical_len);
	} else {
		BK_TFM_FLASH_LOGE(TAG, "addr not crc aligned\r\n");
		return BK_ERR_FLASH_ADDR_NOT_CRC_ALIGNED;
	}
}


