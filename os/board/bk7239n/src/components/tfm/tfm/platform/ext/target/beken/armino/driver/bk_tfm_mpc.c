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

#include "components/log.h"
#include "driver/mpc.h"
#include "bk_tfm_mpc.h"
#include "cmsis.h"
#include "flash_partition.h"
#include "_otp.h"
#include "armino_config.h"

#define TAG "mpc"

static void flash_mpc_cfg(void)
{
	uint32_t block_sz = bk_mpc_get_block_size(MPC_DEV_FLASH);
        uint32_t max_block_num = 32 * (bk_mpc_get_max_block_index(MPC_DEV_FLASH) + 1);
	uint32_t nspe_phy_offset = partition_get_phy_offset(PARTITION_NSPE);
	uint32_t nspe_vir_offset = FLASH_PHY2VIRTUAL(nspe_phy_offset);
#if CONFIG_OTA_OVERWRITE
	uint32_t ota_phy_offset = partition_get_phy_offset(PARTITION_OTA);
	uint32_t ota_phy_size = partition_get_phy_size(PARTITION_OTA);
#else
	uint32_t ota_phy_offset = partition_get_phy_offset(PARTITION_SECONDARY_ALL);
	uint32_t ota_phy_size = partition_get_phy_size(PARTITION_SECONDARY_ALL);
#endif
	uint32_t ota_vir_offset = FLASH_PHY2VIRTUAL(ota_phy_offset);
	uint32_t ota_vir_size = FLASH_PHY2VIRTUAL(ota_phy_size);

	uint32_t block_size = bk_mpc_get_block_size(MPC_DEV_FLASH);
	uint32_t ns_total_size = (ota_phy_offset - nspe_phy_offset + block_size - 1)/block_size * block_size;
	uint32_t ns_block_num = ns_total_size / block_sz;

	if (((nspe_vir_offset % block_sz) != 0) || ((ota_vir_offset % block_sz) != 0) || ((ota_vir_size % block_sz) != 0)) {
		BK_LOGI(TAG, "off %x, %x, %x not %x aligned\r\n", nspe_vir_offset, ota_vir_offset, ota_vir_size, block_sz);
	}

	BK_LOGI(TAG, "ns_phy_offset=%x ns_vir_offset=%x ota_phy_offset=%x ota_vir_offset=%x ns_block_num=%x, block_sz=%x\r\n",
		nspe_phy_offset, nspe_vir_offset, ota_phy_offset, ota_vir_offset, ns_block_num, block_sz);
	BK_LOG_ON_ERR(bk_mpc_set_secure_attribute(MPC_DEV_FLASH, nspe_phy_offset, ns_block_num, MPC_BLOCK_NON_SECURE));

	ns_total_size = (ota_phy_offset + ota_phy_size + block_size - 1)/block_size * block_size;
	ns_block_num = max_block_num - ((ns_total_size) / block_sz);

	BK_LOGI(TAG, "max_block_num=%x, offset=%x, ns_block_num=%x\r\n", max_block_num, ota_vir_offset + ota_vir_size, ns_block_num);
	BK_LOG_ON_ERR(bk_mpc_set_secure_attribute(MPC_DEV_FLASH, ota_phy_offset + ota_phy_size, ns_block_num, MPC_BLOCK_NON_SECURE));

	return;
}

static void otp2_mpc_cfg()
{
    uint32_t block_sz = bk_mpc_get_block_size(MPC_DEV_OTP2); // 256 Bytes
    uint32_t block_num;
    uint32_t cols = otp_map_2_col();//otp_map_col();
    uint32_t size = otp_map_2[0].allocated_size;
    uint32_t offset = otp_map_2[0].offset;
    uint32_t security = otp_map_2[0].security;
    uint8_t i;
    for(i = 1; i < cols; ++i) {
        if (security && otp_map_2[i].security) {
            size += otp_map_2[i].allocated_size;
        } else {
            block_num = size / block_sz; //TODO: ensure size % block_sz == 0 && size / block_sz > 0 in python
            bk_mpc_set_secure_attribute(MPC_DEV_OTP2, offset, block_num, otp_map_2[i-1].security);
            offset = otp_map_2[i].offset; //TODO: ensure offset % block_sz == 0 in python
            size = otp_map_2[i].allocated_size;
            security = otp_map_2[i].security;
        }
    }
    block_num = size / block_sz;
    bk_mpc_set_secure_attribute(MPC_DEV_OTP2, offset, block_num, otp_map_2[i-1].security);
}

static void ram_mpc_cfg(void)
{
	uint32_t ram_size[] = {KB(64), KB(64), KB(128), KB(128), KB(128)};
	uint32_t total_remain_s_size = CONFIG_TFM_RAM_SIZE;
	uint32_t s_size = 0;
	uint32_t s_block_num = 0;
	uint32_t ns_block_num = 0;
	uint32_t max_block_num = 0;
	uint32_t block_sz;
	uint32_t idx;

	for (uint32_t dev = MPC_DEV_SMEM0; dev <= MPC_DEV_SMEM4; dev ++) {
		idx = dev - MPC_DEV_SMEM0;
        	block_sz = bk_mpc_get_block_size(dev);

		if (total_remain_s_size > ram_size[idx]) {
			s_size = ram_size[idx];
		} else {
			s_size = total_remain_s_size;
		}
		total_remain_s_size -= s_size;

        	max_block_num = ram_size[idx] / block_sz;
		s_block_num = s_size / block_sz;
		if (s_block_num > 0) {
            		BK_LOG_ON_ERR(bk_mpc_set_secure_attribute(dev, 0, s_block_num, MPC_BLOCK_SECURE));
		}

		ns_block_num = max_block_num - s_block_num;
		if (ns_block_num > 0) {
			BK_LOG_ON_ERR(bk_mpc_set_secure_attribute(dev, s_size, ns_block_num, MPC_BLOCK_NON_SECURE));
		}
	}
}

int bk_mpc_cfg(void)
{
    mpc_alloc_scheme_t alloc_schemes[] = MPC_ALLOC_SCHEMES;
    mpc_dev_t dev;
    int schem_idx;

    BK_LOG_ON_ERR(bk_mpc_driver_init());

    for (schem_idx = 0; schem_idx < ARRAY_SIZE(alloc_schemes); schem_idx++) {
        uint32_t s_block_num;
        uint32_t ns_block_num;
        uint32_t ns_addr_offset;
        uint32_t block_sz;
        uint32_t max_block_num;
        int scheme;

        dev = alloc_schemes[schem_idx].dev;
        scheme = alloc_schemes[schem_idx].bytes_for_spe;
        if (dev == MPC_DEV_FLASH) {
            uint32_t nspe_phy_offset = 0;
#if (CONFIG_ENABLE_TEST_NS) || (CONFIG_ENABLE_TEST_S)
            nspe_phy_offset = partition_get_phy_offset(PARTITION_TFM_NS);
#endif
            scheme = FLASH_PHY2VIRTUAL(nspe_phy_offset);
        }
        block_sz = bk_mpc_get_block_size(dev);
        max_block_num = 32 * (bk_mpc_get_max_block_index(dev) + 1);

        if (scheme == MPC_ALLOC_SCHEME_ALL)
            s_block_num = max_block_num;
        else if (scheme == MPC_ALLOC_SCHEME_NONE)
            s_block_num = 0;
        else if (scheme == MPC_ALLOC_SCHEME_HALF)
            s_block_num = max_block_num >> 1;
        else
            s_block_num = scheme/block_sz;

        if (s_block_num > 0)
            BK_LOG_ON_ERR(bk_mpc_set_secure_attribute(dev, 0, s_block_num, MPC_BLOCK_SECURE));

        ns_block_num = max_block_num - s_block_num;
        ns_addr_offset = s_block_num * block_sz;
        if (ns_block_num > 0)
            BK_LOG_ON_ERR(bk_mpc_set_secure_attribute(dev, ns_addr_offset, ns_block_num, MPC_BLOCK_NON_SECURE));

        BK_LOGI(TAG, "dev=%d s blocks=%d ns offset=%d blocks=%d\n", dev, s_block_num, ns_addr_offset, ns_block_num);
    }

#if CONFIG_TFM_S_JUMP_TO_CPU0_APP
    ram_mpc_cfg();
    flash_mpc_cfg();
    otp2_mpc_cfg();
#endif

#if (0 == CONFIG_ENABLE_DEBUG)
    for (dev = 0; dev < MPC_DEV_MAX; dev++) {
        BK_LOG_ON_ERR(bk_mpc_lockdown(dev));
    }
#endif

    __DSB();
    __ISB();

    return BK_OK;
}
