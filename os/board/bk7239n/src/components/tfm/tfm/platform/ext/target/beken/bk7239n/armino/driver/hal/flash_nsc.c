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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!
//
#include <soc/soc.h>
#include "hal_port.h"
#include "system_hw.h"
#include "tfm_secure_api.h"
#include "flash_driver.h"
#include "partitions_gen.h"

#define PAGE_SHIFT (12)
extern int flash_op_enable_ctrl(uint32_t module,uint32_t enable);
/* Calling this API in tfm_s.bin to trigger the compiler to generate NSC symbols in tfm_s_veneers.o.
 **/
void psa_flash_nsc_stub(void)
{
	return;
}

static inline void enable_flash_s(void)
{
	uint32_t v = REG_READ(0x41040018);
	v &= ~(1<<6);
	REG_WRITE(0x41040018, v);
}

static inline void disable_flash_s(void)
{
	uint32_t v = REG_READ(0x41040018);
	v |= (1<<6);
	REG_WRITE(0x41040018, v);
}
__tfm_psa_secure_gateway_no_naked_attributes__ flash_protect_type_t psa_flash_get_protect_type(void)
{
	flash_protect_type_t protect_type = 0;
	enable_flash_s();
	protect_type = bk_flash_get_protect_type();
	disable_flash_s();
	return protect_type;
}
__tfm_psa_secure_gateway_no_naked_attributes__ int psa_flash_set_protect_type(uint32_t type)
{
	int ret = 0;
	enable_flash_s();
	ret = bk_flash_set_protect_type(type);
	disable_flash_s();
	return ret;
}

__tfm_psa_secure_gateway_no_naked_attributes__ int psa_flash_erase_sector(uint32_t address)
{
	int ret = 0;
	flash_protect_type_t protect_type = 0;

	protect_type = psa_flash_get_protect_type();
	psa_flash_set_protect_type(FLASH_PROTECT_NONE);
	enable_flash_s();
	if ((address >= CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET) &&
		(address < (CONFIG_SECONDARY_ALL_PHY_PARTITION_OFFSET + CONFIG_SECONDARY_ALL_PHY_PARTITION_SIZE))) {
			flash_op_enable_ctrl(1,1);
			ret = bk_flash_erase_sector(address);
			flash_op_enable_ctrl(1,0);
		} else {
			ret = -1;
		}
	disable_flash_s();
	psa_flash_set_protect_type(protect_type);
	return ret;
}

__tfm_psa_secure_gateway_no_naked_attributes__ int bk_erase_page(uint32_t page)
{
	uint32_t address = (page << PAGE_SHIFT);
	ssize_t ret;

	ret = psa_flash_erase_sector(address);
	return ret;
}

__tfm_psa_secure_gateway_no_naked_attributes__ int psa_flash_read_bytes(uint32_t address, uint8_t *user_buf, uint32_t size)
{
	int ret = 0;
	enable_flash_s();
	if ((address >= CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET) &&
		(address < (CONFIG_SECONDARY_ALL_PHY_PARTITION_OFFSET + CONFIG_SECONDARY_ALL_PHY_PARTITION_SIZE))) {
			ret = bk_flash_read_bytes(address, user_buf, size);
		} else {
			ret = -1;
		}
	disable_flash_s();
	return ret;
}

__tfm_psa_secure_gateway_no_naked_attributes__ int psa_flash_write_bytes(uint32_t address, const uint8_t *user_buf, uint32_t size)
{
	int ret = 0;
	flash_protect_type_t protect_type = 0;

	protect_type = psa_flash_get_protect_type();
	psa_flash_set_protect_type(FLASH_PROTECT_NONE);
	enable_flash_s();
	if ((address >= CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET) &&
		(address < (CONFIG_SECONDARY_ALL_PHY_PARTITION_OFFSET + CONFIG_SECONDARY_ALL_PHY_PARTITION_SIZE))) {
			flash_op_enable_ctrl(1,1);
			ret = bk_flash_write_bytes(address, user_buf, size);
			flash_op_enable_ctrl(1,0);
		} else {
			ret = -1;
		}
	disable_flash_s();
	psa_flash_set_protect_type(protect_type);
	return ret;
}
