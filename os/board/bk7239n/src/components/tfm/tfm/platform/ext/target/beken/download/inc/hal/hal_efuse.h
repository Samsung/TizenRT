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

#define HAL_EFUSE_SECURE_BOOT_ENABLE_BIT        (1<<0)
#define HAL_EFUSE_SECURE_BOOT_DEBUG_DISABLE_BIT (1<<1)
#define HAL_EFUSE_FAST_BOOT_DISABLE_BIT         (1<<2)
#define HAL_EFUSE_SECURE_BOOT_SUPPORTED_BIT     (1<<3)
#define HAL_EFUSE_PLL_ENABLE_BIT                (1<<4)
#define HAL_EFUSE_FIH_DELAY_ENABLE_BIT          (1<<5)
#define HAL_EFUSE_DIRECT_JUMP_ENABLE_BIT        (1<<6)
#define HAL_EFUSE_CRITICAL_ERR_DISABLE_BIT      (1<<7)

#define HAL_EFUSE_SPI_TO_AHB_DISABLE_BIT        (1<<21)
#define HAL_EFUSE_AUTO_RESET_ENABLE_BIT         (1<<22)

#define HAL_EFUSE_FLASH_AES_ENABLE_BIT          (1<<29)
#define HAL_EFUSE_SPI_DOWNLOAD_DISABLE_BIT      (1<<30)
#define HAL_EFUSE_JTAG_DISABLE_BIT              (1<<31)

extern uint32_t s_efuse_data;

int hal_efuse_init(void);
void hal_efuse_dump(void);

static inline bool hal_efuse_is_security_boot_supported(void)
{
	return !!(s_efuse_data & HAL_EFUSE_SECURE_BOOT_SUPPORTED_BIT);
}

static inline bool hal_efuse_is_security_boot_enabled(void)
{
	return !!(s_efuse_data & HAL_EFUSE_SECURE_BOOT_ENABLE_BIT);
}


static inline bool hal_efuse_is_debug_disabled(void)
{
	return !!(s_efuse_data & HAL_EFUSE_SECURE_BOOT_DEBUG_DISABLE_BIT);
}

static inline bool hal_efuse_is_fast_boot_disabled(void)
{
	return !!(s_efuse_data & HAL_EFUSE_FAST_BOOT_DISABLE_BIT);
}

static inline bool hal_efuse_is_pll_enabled(void)
{
	return !!(s_efuse_data & HAL_EFUSE_PLL_ENABLE_BIT);
}

static inline bool hal_efuse_is_bc_disabled(void)
{
	return hal_efuse_is_debug_disabled();
}

static inline bool hal_efuse_is_flash_aes_enc_enabled(void)
{
	return !!(s_efuse_data & HAL_EFUSE_FLASH_AES_ENABLE_BIT);
}

static inline bool hal_efuse_is_spi_download_disabled(void)
{
	return !!(s_efuse_data & HAL_EFUSE_SPI_DOWNLOAD_DISABLE_BIT) ;
}

static inline bool hal_efuse_is_jtag_disabled(void)
{
	return !!(s_efuse_data & HAL_EFUSE_JTAG_DISABLE_BIT) ;
}

static inline bool hal_efuse_is_fih_delay_enabled(void)
{
	return !!(s_efuse_data & HAL_EFUSE_FIH_DELAY_ENABLE_BIT) ;
}

static inline bool hal_efuse_is_direct_jump_enabled(void)
{
	return !!(s_efuse_data & HAL_EFUSE_DIRECT_JUMP_ENABLE_BIT) ;
}

static inline bool hal_efuse_critical_err_disabled(void)
{
	return !!(s_efuse_data & HAL_EFUSE_CRITICAL_ERR_DISABLE_BIT) ;
}

