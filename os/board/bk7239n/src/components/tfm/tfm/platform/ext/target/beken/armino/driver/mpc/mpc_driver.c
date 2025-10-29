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

#include <common/bk_include.h>
#include <components/system.h>
#include <driver/mpc.h>
#include <os/os.h>
#include <os/mem.h>
#include "mpc_driver.h"
#include "mpc_hal.h"

typedef struct {
	mpc_hal_t hal;
} mpc_driver_t;

#define MPC_RETURN_ON_DRIVER_NOT_INIT() do {\
	if (!s_mpc_driver_is_init) {\
		MPC_LOGE("MPC driver not init\r\n");\
		return BK_ERR_MPC_DRIVER_NOT_INIT;\
	}\
} while(0)

#define MPC_RETURN_ON_INVALID_ID(dev) do {\
	if ((dev) > MPC_DEV_MAX) {\
		MPC_LOGE("Invalid mpc device id=%d\r\n", (dev));\
		return BK_ERR_MPC_INVALID_DEV;\
	}\
} while(0)

static mpc_driver_t s_mpc[MPC_DEV_MAX] = {0};
static bool s_mpc_driver_is_init = false;

bk_err_t bk_mpc_driver_init(void)
{
	if (s_mpc_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_mpc, 0, sizeof(s_mpc));
	for (uint32_t dev_id = 0; dev_id < MPC_DEV_MAX; dev_id++) {
		s_mpc[dev_id].hal.dev = dev_id;
		mpc_hal_init(&s_mpc[dev_id].hal);
	}
	s_mpc_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_mpc_driver_deinit(void)
{
	if (!s_mpc_driver_is_init) {
		return BK_OK;
	}
	os_memset(&s_mpc, 0, sizeof(s_mpc));
	s_mpc_driver_is_init = false;

	return BK_OK;
}

uint32_t bk_mpc_get_block_size(mpc_dev_t dev)
{
	MPC_RETURN_ON_DRIVER_NOT_INIT();
	MPC_RETURN_ON_INVALID_ID(dev);
	return mpc_hal_get_block_size(&s_mpc[dev].hal);
}

uint32_t bk_mpc_get_max_block_index(mpc_dev_t dev)
{
	MPC_RETURN_ON_DRIVER_NOT_INIT();
	MPC_RETURN_ON_INVALID_ID(dev);
	return mpc_hal_get_max_block_index(&s_mpc[dev].hal);
}

bk_err_t bk_mpc_dump_secure_attribute(mpc_dev_t dev)
{
	mpc_hal_t *hal = &s_mpc[dev].hal;
	uint32_t block_size = bk_mpc_get_block_size(dev);
	uint32_t max_index = bk_mpc_get_max_block_index(dev);

	MPC_LOGD("mpc dev%d: block_size=0x%x, max_block_index=%d\r\n", dev, block_size, max_index);
	BK_LOG_RAW("%4s    %8s\r\n", "id", "sec bits");
	BK_LOG_RAW("----------------\r\n");
	mpc_hal_disable_auto_increase(hal);
	for (uint32_t block_index = 0; block_index <= max_index; block_index++) {
		mpc_hal_set_block_index(hal, block_index);
		uint32_t lut = mpc_hal_get_block_lut(hal);
		BK_LOG_RAW("%4u    %08x\r\n", mpc_hal_get_block_index(hal), lut);
	}
	BK_LOG_RAW("\r\n");
	return BK_OK;
}

bk_err_t bk_mpc_set_secure_attribute(mpc_dev_t dev, uint32_t mem_addr_offset, uint32_t mem_block_num, mpc_block_secure_type_t secure_type)
{
	MPC_RETURN_ON_DRIVER_NOT_INIT();
	MPC_RETURN_ON_INVALID_ID(dev);

	uint32_t mem_block_index = 0;
	uint32_t lut_block_index = 0;
	uint32_t mem_block_size = bk_mpc_get_block_size(dev);
	bk_err_t ret = BK_OK;
	mpc_hal_t *hal = &s_mpc[dev].hal;

	if (mem_addr_offset % mem_block_size) {
		MPC_LOGW("mpc dev%d mem_addr_offset not block size:%d aligned\r\n", dev, mem_block_size);
	}

	mpc_hal_disable_auto_increase(hal);
	MPC_LOGD("mpc dev%d offset=%x, mem_block_num=%d, secure_type=%d\r\n", dev, mem_addr_offset, mem_block_num, secure_type);
	MPC_LOGD("mpc mem_block_size=%x, current_idx=%d\r\n", mem_block_size, mpc_hal_get_block_index(hal));

	/* set mpc block index */
	mem_block_index = mem_addr_offset / mem_block_size;
	lut_block_index = mem_block_index / MPC_BLOCK_LUT_MAX_BIT_NUM;

	if (lut_block_index > bk_mpc_get_max_block_index(dev)) {
		MPC_LOGW("block index:%d is out of range\r\n", lut_block_index);
		return BK_ERR_MPC_BLOCK_INDEX_OUT_OF_RANGE;
	}

	/* set block lut */
	ret = mpc_hal_config_block_lut(hal, mem_block_index, mem_block_num, secure_type);

	return ret;
}

bk_err_t bk_mpc_lockdown(mpc_dev_t dev)
{
	MPC_RETURN_ON_DRIVER_NOT_INIT();
	MPC_RETURN_ON_INVALID_ID(dev);
	mpc_hal_enable_sec_lock(&s_mpc[dev].hal);
	return BK_OK;
}

bk_err_t bk_mpc_enable_secure_exception(mpc_dev_t dev)
{
	MPC_RETURN_ON_DRIVER_NOT_INIT();
	MPC_RETURN_ON_INVALID_ID(dev);
	mpc_hal_enable_sec_exception(&s_mpc[dev].hal);
	return BK_OK;
}

bk_err_t bk_mpc_disable_secure_exception(mpc_dev_t dev)
{
	MPC_RETURN_ON_DRIVER_NOT_INIT();
	MPC_RETURN_ON_INVALID_ID(dev);
	mpc_hal_disable_sec_exception(&s_mpc[dev].hal);
	return BK_OK;
}

