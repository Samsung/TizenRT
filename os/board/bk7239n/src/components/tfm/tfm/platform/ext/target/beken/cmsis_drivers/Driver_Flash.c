// Copyright 2022-2025 Beken
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

#include <string.h>
#include <stdint.h>
#include "Driver_Flash.h"
#include "platform_retarget.h"
#include "RTE_Device.h"
#include "driver/flash.h"
#include "components/log.h"
#include "common/bk_err.h"
#include "bk_tfm_log.h"
#include "sys_driver.h"
#include "flash_driver.h"
#include "partitions_gen.h"

#define TAG "cmsis_flash"

#define BK_TFM_FLASH_LOGD BK_LOGD
#define BK_TFM_FLASH_LOGI BK_LOGD
#define BK_TFM_FLASH_LOGW BK_LOGW
#define BK_TFM_FLASH_LOGE BK_LOGE

#ifndef KB
#define KB(x) ((x) << 10)
#endif

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  ((void)arg)
#endif

/* Driver version */
#define ARM_FLASH_DRV_VERSION      ARM_DRIVER_VERSION_MAJOR_MINOR(1, 1)
#define ARM_FLASH_DRV_ERASE_VALUE  0xFF

/**
 * Data width values for ARM_FLASH_CAPABILITIES::data_width
 * \ref ARM_FLASH_CAPABILITIES
 */
 enum {
    DATA_WIDTH_8BIT   = 0u,
    DATA_WIDTH_16BIT,
    DATA_WIDTH_32BIT,
    DATA_WIDTH_ENUM_SIZE
};

static const uint32_t data_width_byte[DATA_WIDTH_ENUM_SIZE] = {
    sizeof(uint8_t),
    sizeof(uint16_t),
    sizeof(uint32_t),
};

static uint32_t s_flash_sector_count = 0;

/*
 * ARM FLASH device structure
 *
 * There is no real flash memory for code on MPS2 board. Instead a code SRAM is
 * used for code storage: ZBT SSRAM1. This driver just emulates a flash
 * interface and behaviour on top of the SRAM memory.
 */
struct arm_flash_dev_t {
    const uint32_t memory_base;   /*!< FLASH memory base address */
    ARM_FLASH_INFO *data;         /*!< FLASH data */
};

/* Flash Status */
static ARM_FLASH_STATUS FlashStatus = {0, 0, 0};

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_FLASH_API_VERSION,
    ARM_FLASH_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES DriverCapabilities = {
    0, /* event_ready */
    0, /* data_width = 0:8-bit, 1:16-bit, 2:32-bit */
    1  /* erase_chip */
};

static bool is_access_from_code_bus(uint32_t absolute_addr)
{
	return false;
}

static uint32_t flash_sector_count(void)
{
    return s_flash_sector_count;
}

static int32_t is_range_valid(struct arm_flash_dev_t *flash_dev,
                              uint32_t offset)
{
    uint32_t flash_limit = 0;
    int32_t rc = 0;

    flash_limit = (flash_sector_count() * flash_dev->data->sector_size)
                   - 1;

    if (offset > flash_limit) {
        rc = -1;
    }
    return rc;
}

static int32_t is_write_aligned(struct arm_flash_dev_t *flash_dev,
                                uint32_t param)
{
    int32_t rc = 0;

    if ((param % flash_dev->data->program_unit) != 0) {
        rc = -1;
    }
    return rc;
}

static int32_t is_sector_aligned(struct arm_flash_dev_t *flash_dev,
                                 uint32_t offset)
{
    int32_t rc = 0;

    if ((offset % flash_dev->data->sector_size) != 0) {
        rc = -1;
    }
    return rc;
}

static int32_t is_flash_ready_to_write(const uint8_t *start_addr, uint32_t cnt)
{
    int32_t rc = 0;
    uint32_t i;

    for (i = 0; i < cnt; i++) {
        if(start_addr[i] != ARM_FLASH_DRV_ERASE_VALUE) {
            rc = -1;
            break;
        }
    }

    return rc;
}

#if (RTE_FLASH0)
static ARM_FLASH_INFO ARM_FLASH0_DEV_DATA = {
    .sector_info  = NULL,                  /* Uniform sector layout */
    .sector_count = FLASH0_SIZE / FLASH0_SECTOR_SIZE,
    .sector_size  = FLASH0_SECTOR_SIZE,
    .page_size    = FLASH0_PAGE_SIZE,
    .program_unit = FLASH0_PROGRAM_UNIT,
    .erased_value = ARM_FLASH_DRV_ERASE_VALUE};

static struct arm_flash_dev_t ARM_FLASH0_DEV = {
#if (__DOMAIN_NS == 1)
    .memory_base = FLASH0_BASE_NS,
#else
    .memory_base = FLASH0_BASE_S,
#endif /* __DOMAIN_NS == 1 */
    .data        = &(ARM_FLASH0_DEV_DATA)};

struct arm_flash_dev_t *FLASH0_DEV = &ARM_FLASH0_DEV;

/*
 * Functions
 */

static ARM_DRIVER_VERSION Flash_GetVersion(void)
{
    return DriverVersion;
}

static ARM_FLASH_CAPABILITIES Flash_GetCapabilities(void)
{
    return DriverCapabilities;
}

static int32_t Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
	SYS_LOCK_DECLARATION();
    uint32_t flash_size = 0;
    ARG_UNUSED(cb_event);

    SYS_LOCK();
    BK_LOG_ON_ERR(bk_flash_driver_init());
    bk_flash_set_protect_type(FLASH_PROTECT_NONE); //TODO wangzhilei double check

    flash_size = bk_flash_get_current_total_size();
    /* Optimze it if we support more than one flash */
    s_flash_sector_count = flash_size / FLASH0_SECTOR_SIZE;
    // s_flash_sector_count = flash_size >> 12;    //optimize code size

    SYS_UNLOCK();

    return ARM_DRIVER_OK;
}

static int32_t Flash_Uninitialize(void)
{
    SYS_LOCK_DECLARATION();

    SYS_LOCK();
    BK_LOG_ON_ERR(bk_flash_driver_deinit());
    SYS_UNLOCK();

    return ARM_DRIVER_OK;
}

#if 0   //optimize code size
static int32_t Flash_PowerControl(ARM_POWER_STATE state)
{
    return ARM_DRIVER_OK;
}
#endif

static int32_t Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
	SYS_LOCK_DECLARATION();
    bool use_cbus = false;
    int32_t rc = 0;

    BK_TFM_FLASH_LOGD(TAG, "read off=%x cnt=%x\r\n", addr, cnt);

    if (cnt == 0) {
        BK_TFM_FLASH_LOGW(TAG, "read 0B\r\n");
        return ARM_DRIVER_OK;
    }

    use_cbus = is_access_from_code_bus(addr);
    FLASH_CLR_CBUS_ADDR_FLAG(addr);

    /* CMSIS ARM_FLASH_ReadData API requires the `addr` data type size aligned.
     * Data type size is specified by the data_width in ARM_FLASH_CAPABILITIES.
     */
    if (addr % data_width_byte[DriverCapabilities.data_width] != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check flash memory boundaries */
    rc = is_range_valid(FLASH0_DEV, addr + cnt);
    if (rc != 0) {
        BK_TFM_FLASH_LOGE(TAG, "invalid addr range, addr=%x cnt=%x\r\n", addr, cnt);
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if (use_cbus) {
        BK_TFM_FLASH_LOGD(TAG, "flash cbus read, addr=%x cnt=%x+\r\n", FLASH0_DEV->memory_base + addr, cnt);
        memcpy(data, (void *)(FLASH0_DEV->memory_base + addr), cnt);
        BK_TFM_FLASH_LOGD(TAG, "flash cbus read-\r\n");
    } else {
        SYS_LOCK();
        BK_LOG_ON_ERR(bk_flash_read_bytes(addr, data, cnt));
        SYS_UNLOCK();
    }

    /* Conversion between bytes and data items */
    cnt /= data_width_byte[DriverCapabilities.data_width];

    // BK_TFM_DUMP_BUF("cmsis flash read", data, cnt);
    return cnt;
}
int flash_check_its_ps_addr_valid(uint32_t addr,uint32_t len)
{
	int ret = 0;
	uint32_t its_valide = (((addr) >= CONFIG_SYS_ITS_PHY_PARTITION_OFFSET)&&((addr+len) <= (CONFIG_SYS_ITS_PHY_PARTITION_OFFSET+CONFIG_SYS_ITS_PHY_PARTITION_SIZE)));

	uint32_t ps_valide = (((addr) >= CONFIG_SYS_PS_PHY_PARTITION_OFFSET)&&((addr+len) <= (CONFIG_SYS_PS_PHY_PARTITION_OFFSET+CONFIG_SYS_PS_PHY_PARTITION_SIZE)));

	if((its_valide == 0)&&(ps_valide == 0))
	{
		ret = -1;
	}

	return ret;
}
static int32_t Flash_ProgramData(uint32_t addr, const void *data,
                                     uint32_t cnt)
{
	SYS_LOCK_DECLARATION();
    bool use_cbus = false;
    int32_t rc = 0;
	if(flash_check_its_ps_addr_valid(addr,cnt) != 0)
	{
		printf("IPS_w NO PMIT[addr:0x%x 0x%x]\r\n",addr,cnt);
		return ARM_DRIVER_ERROR_FLASH_OP_NOT_PERMIT;
	}
    BK_TFM_FLASH_LOGD(TAG, "write off=%x cnt=%x base=%x\r\n", addr, cnt, FLASH0_DEV->memory_base);

    if (cnt == 0) {
        BK_TFM_FLASH_LOGW(TAG, "write 0B\r\n");
        return ARM_DRIVER_OK;
    }
    use_cbus = is_access_from_code_bus(addr);
    FLASH_CLR_CBUS_ADDR_FLAG(addr);

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check flash memory boundaries and alignment with minimal write size */
    rc  = is_range_valid(FLASH0_DEV, addr + cnt);
    rc |= is_write_aligned(FLASH0_DEV, addr);
    rc |= is_write_aligned(FLASH0_DEV, cnt);
    if (rc != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* Check if the flash area to write the data was erased previously */
    if (use_cbus) {
        memcpy((uint8_t*)(FLASH0_DEV->memory_base + addr), (uint8_t*)data, cnt);
    } else {
        SYS_LOCK();
        uint32_t protect_type = bk_flash_get_protect_type();
        bk_flash_set_protect_type(FLASH_PROTECT_NONE);
        rc = is_flash_ready_to_write((const uint8_t*)addr, cnt);
        BK_LOG_ON_ERR(bk_flash_write_bytes(addr, data, cnt));
        bk_flash_set_protect_type(protect_type);
        SYS_UNLOCK();
    }

    /* Conversion between bytes and data items */
    cnt /= data_width_byte[DriverCapabilities.data_width];

    return cnt;
}

static int32_t Flash_EraseSector(uint32_t addr)
{
	SYS_LOCK_DECLARATION();
    uint32_t offset = addr;
    uint32_t rc = 0;

	if(flash_check_its_ps_addr_valid(addr,0) != 0)
	{
		printf("IPS_e NO PMIT[addr:0x%x]\r\n",addr);
		return ARM_DRIVER_ERROR_FLASH_OP_NOT_PERMIT;
	}
    BK_TFM_FLASH_LOGD(TAG, "flash erase off=%x\r\n", addr);

    rc  = is_range_valid(FLASH0_DEV, addr);
    rc |= is_sector_aligned(FLASH0_DEV, addr);
    if (rc != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    SYS_LOCK();
    uint32_t protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    BK_LOG_ON_ERR(bk_flash_erase_sector(offset));
    bk_flash_set_protect_type(protect_type);
    SYS_UNLOCK();

    return ARM_DRIVER_OK;
}

static int32_t Flash_EraseChip(void)
{
    int32_t rc = ARM_DRIVER_ERROR_UNSUPPORTED;
#if 0
    uint32_t i;
    uint32_t addr = FLASH0_DEV->memory_base;
    uint32_t offset = 0;

    SYS_LOCK_DECLARATION();
    /* Check driver capability erase_chip bit */
    SYS_LOCK();
    uint32_t protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);

    if (DriverCapabilities.erase_chip == 1) {
        for (i = 0; i < flash_sector_count(); i++) {
                offset = addr - FLASH0_DEV->memory_base;
                BK_LOG_ON_ERR(bk_flash_erase_sector(addr)); //TODO double check address offset

            addr += FLASH0_DEV->data->sector_size;
            rc = ARM_DRIVER_OK;
        }
    }

    bk_flash_set_protect_type(protect_type);
    SYS_UNLOCK();
#endif
    return rc;
}

static ARM_FLASH_STATUS Flash_GetStatus(void)
{
    return FlashStatus;
}

static ARM_FLASH_INFO * Flash_GetInfo(void)
{
    return FLASH0_DEV->data;
}

ARM_DRIVER_FLASH Driver_FLASH0 = {
    Flash_GetVersion,
    Flash_GetCapabilities,
    Flash_Initialize,
    Flash_Uninitialize,
    NULL,      /*Flash_PowerControl,  optimize code size */
    Flash_ReadData,
    Flash_ProgramData,
    Flash_EraseSector,
    Flash_EraseChip,
    Flash_GetStatus,
    Flash_GetInfo
};
#endif /* RTE_FLASH0 */

static inline bool is_64k_aligned(uint32_t addr)
{
	return ((addr & (KB(64) - 1)) == 0);
}

static inline bool is_32k_aligned(uint32_t addr)
{
	return ((addr & (KB(32) - 1)) == 0);
}

int flash_area_erase_fast(uint32_t erase_off, uint32_t len)
{
	uint32_t erase_size = 0;
	int erase_remain = len;

	SYS_LOCK_DECLARATION();
	SYS_LOCK();
	while (erase_remain > 0) {
		if ((erase_remain >= KB(64)) && is_64k_aligned(erase_off)) {
			BK_TFM_FLASH_LOGI(TAG, "64k erase: off=%x remain=%x\r\n", erase_off, erase_remain);
			bk_flash_erase_block_64k(erase_off);
			erase_size = KB(64);
		} else if ((erase_remain >= KB(32)) && is_32k_aligned(erase_off)) {
			BK_TFM_FLASH_LOGI(TAG, "32k erase: off=%x remain=%x\r\n", erase_off, erase_remain);
			bk_flash_erase_block_32k(erase_off);
			erase_size = KB(32);
		} else {
			BK_TFM_FLASH_LOGI(TAG, "4k erase: off=%x remain=%x\r\n", erase_off, erase_remain);
			bk_flash_erase_sector(erase_off);
			erase_size = KB(4);
		}
		erase_off += erase_size;
		erase_remain -= erase_size;
	}
	SYS_UNLOCK();

	return 0;
}

uint32_t flash_area_read_offset_enable()
{
	SYS_LOCK_DECLARATION();
	SYS_LOCK();
	uint32_t enable = flash_get_excute_enable();
	SYS_UNLOCK();
	return enable;
}
