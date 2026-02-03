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

#include <driver/int.h>
#include <os/mem.h>
#include "clock_driver.h"
#include "sys_driver.h"
#include "psram_hal.h"
#include "driver/psram_types.h"
#include "psram_driver.h"
#include <driver/psram.h>
#include <modules/pm.h>
#include <modules/chip_support.h>
#include <soc/bk7239n/soc.h>
#if (defined(CONFIG_PSRAM_AUTO_DETECT))
#include "bk_ef.h"
#endif

#define PSRAM_CHECK_FLAG   0x3CA5C3A5
typedef struct {
	uint32_t psram_id;
	uint32_t magic_code;
} psram_flash_t;

#if (defined(CONFIG_PSRAM_AUTO_DETECT))
static bool s_psram_id_need_write = false;
static beken_semaphore_t s_psram_sem = NULL;
static beken_thread_t psram_task = NULL;
#endif

extern void delay_us(uint32_t us);
static bool s_psram_server_is_init = false;
static bool s_psram_heap_is_init = false;
static uint8_t s_psram_channelmap = 0;
#if (defined(CONFIG_SYS_CPU0))
static psram_flash_t s_psram_id = {0};
#endif
#define PSRAM_RETURN_ON_SERVER_NOT_INIT() do {\
				if (!s_psram_server_is_init) {\
					return BK_ERR_PSRAM_SERVER_NOT_INIT;\
				}\
			} while(0)

#if (defined(CONFIG_SYS_CPU0))
uint32_t bk_psram_get_psram_id(void)
{
	return s_psram_id.psram_id;
}
#endif

uint32_t bk_psram_get_psram_data_length(uint32_t id)
{
#if defined(CONFIG_SUPPORT_PSRAM_GET_SIZE)
	if(PSRAM_APS6408L_ID == id)
	{
		return SOC_PSRAM_8M_SIZE;
	}
	else if(PSRAM_APS128XXO_OB9_ID == id)
	{
		return SOC_PSRAM_16M_SIZE;
	}
	else
	{
		return SOC_PSRAM_4M_SIZE;
	}
#else
	return 0;
#endif
}

__FLASH_BOOT_CODE bk_err_t bk_psram_set_clk(psram_clk_t clk)
{
	bk_err_t ret = BK_OK;

	psram_hal_set_clk(clk);

	return ret;
}
bk_err_t bk_psram_heap_init_flag_set(bool init)
{
	bk_err_t ret = BK_OK;
	s_psram_heap_is_init = init;
	return ret;
}
bool bk_psram_heap_init_flag_get()
{
	return s_psram_heap_is_init;
}

__FLASH_BOOT_CODE bk_err_t bk_psram_set_voltage(psram_voltage_t voltage)
{
	bk_err_t ret = BK_OK;

	psram_hal_set_voltage(voltage);

	return ret;
}

bk_err_t bk_psram_set_transfer_mode(psram_tansfer_mode_t transfer_mode)
{
	bk_err_t ret = BK_OK;

	psram_hal_set_transfer_mode(transfer_mode);

	return ret;
}

psram_write_through_area_t bk_psram_alloc_write_through_channel(void)
{
	uint8_t channel = 0;
	for (channel = 0; channel < PSRAM_WRITE_THROUGH_AREA_COUNT; channel++)
	{
		if ((s_psram_channelmap & (0x1 << channel)) == 0)
		{
			s_psram_channelmap |= (0x1 << channel);
			break;
		}
	}

	return channel;
}

bk_err_t bk_psram_free_write_through_channel(psram_write_through_area_t area)
{
	if (area > PSRAM_WRITE_THROUGH_AREA_COUNT)
	{
		PSRAM_LOGE("%s over range failed\r\n", __func__);
		return BK_OK;
	}

	if (s_psram_channelmap & (0x1 << area))
		s_psram_channelmap &= ~(0x1 << area);

	return BK_OK;
}

bk_err_t  bk_psram_enable_write_through(psram_write_through_area_t area, uint32_t start, uint32_t end)
{
	return psram_hal_set_write_through(area, 1, start, end);
}

bk_err_t bk_psram_disable_write_through(psram_write_through_area_t area)
{
	return psram_hal_set_write_through(area, 0, 0, 0);
}

__FLASH_BOOT_CODE bk_err_t bk_psram_calibrate(void)
{
#if defined(CONFIG_PSRAM_CALIBRATE)
	//20250925: psram calib data below is summed by asic for bk7239n v4
	psram_hal_set_reg5_value((3<<0) + (0<<3) + (0<<6) + (0<<9) + (0 << 12));
	return BK_OK;
#else
	return BK_OK;
#endif
}

#if (defined(CONFIG_PSRAM_AUTO_DETECT))
static void psram_id_write(beken_thread_arg_t data)
{
	rtos_get_semaphore(&s_psram_sem, BEKEN_WAIT_FOREVER);

	if (s_psram_id_need_write)
	{
		bk_set_env_enhance(PSRAM_CHIP_ID, (const void *)&s_psram_id, sizeof(psram_flash_t));
	}

	PSRAM_LOGI("psram id write to flash success\r\n");

	s_psram_id_need_write = false;

	rtos_deinit_semaphore(&s_psram_sem);
	s_psram_sem = NULL;

	psram_task = NULL;
	rtos_delete_thread(NULL);
}
#endif

bk_err_t bk_psram_id_auto_detect(void)
{
#if (defined(CONFIG_PSRAM_AUTO_DETECT))
	int ret = bk_get_env_enhance(PSRAM_CHIP_ID, (void *)&s_psram_id, sizeof(psram_flash_t));

	if (ret != 8)
	{
		PSRAM_LOGW("%s, %d ret:%d read error\r\n", __func__, sizeof(psram_flash_t), ret);
	}

	if (s_psram_id.magic_code == PSRAM_CHECK_FLAG)
	{
		return BK_OK;
	}

	if (s_psram_sem == NULL)
	{
		ret = rtos_init_semaphore(&s_psram_sem, 1);
		if (ret != BK_OK)
		{
			PSRAM_LOGE("%s, init s_psram_sem error\r\n", __func__);
			return ret;
		}
	}

	ret = rtos_create_thread(&psram_task,
						 4,
						 "psram_task",
						 (beken_thread_function_t)psram_id_write,
						 3072,
						 NULL);

	if (BK_OK != ret)
	{
		PSRAM_LOGE("%s psram_task init failed\n");
		rtos_deinit_semaphore(&s_psram_sem);
		s_psram_sem = NULL;
		return ret;
	}
#endif

	return BK_OK;
}

__FLASH_BOOT_CODE bk_err_t bk_psram_init(void)
{
#if (defined(CONFIG_SYS_CPU0))
	if (s_psram_server_is_init) {
		return BK_OK;
	}

	uint32_t chip_id = 0, actual_id = 0;

	hardware_chip_version_e chip_version;
	chip_version = bk_get_hardware_chip_id_version();
	//PSRAM_LOGI("chip_version:%d\r\n", chip_version);

	// psram voltage sel
	if (chip_version == CHIP_VERSION_D)
	{
		bk_psram_set_voltage(PSRAM_OUT_1_90V);
	}
	else
	{
		bk_psram_set_voltage(PSRAM_OUT_1_90V);
	}

	// power up and clk config
	psram_hal_power_clk_enable(1);

	if (s_psram_id.magic_code == PSRAM_CHECK_FLAG)
	{
		chip_id = s_psram_id.psram_id;
	}

	PSRAM_LOGI("%s, chip_id:%x\r\n", __func__, chip_id);

	// psram config
	actual_id =  psram_hal_config_init(chip_id);

	if (actual_id == 0)
	{
		PSRAM_LOGE("%s, fail!\r\n", __func__);
		return BK_FAIL;
	}

	delay_us(1000);
	// set psram clk
#if (defined(CONFIG_SOC_BK7239XX))
	if (chip_version == CHIP_VERSION_D)
	{
		bk_psram_set_clk(PSRAM_120M);
	}
	else
	{
		bk_psram_set_clk(PSRAM_160M);
	}
#else
	bk_psram_set_clk(PSRAM_160M);
#endif

	PSRAM_LOGI("%s, %x-%x\r\n", __func__, actual_id, chip_id);

	if (actual_id != chip_id)
	{
		s_psram_id.psram_id = actual_id;
		s_psram_id.magic_code = PSRAM_CHECK_FLAG;
#if (defined(CONFIG_PSRAM_AUTO_DETECT))
		if (s_psram_sem)
		{
			s_psram_id_need_write = true;

			rtos_set_semaphore(&s_psram_sem);
		}
#endif
	}
	else
	{
#if (defined(CONFIG_PSRAM_AUTO_DETECT))
		if (s_psram_sem)
		{
			rtos_set_semaphore(&s_psram_sem);
		}
#endif
	}

	s_psram_server_is_init = true;


	return BK_OK;
#else
	return BK_FAIL;
#endif
}

bk_err_t bk_psram_init_with_para(uint32_t psram_clk,uint32_t psram_vol)
{

#if (defined(CONFIG_SYS_CPU0))
	if (s_psram_server_is_init) {
		return BK_OK;
	}

	uint32_t chip_id = 0, actual_id = 0;

	bk_psram_set_voltage(psram_vol);

	// power up and clk config
	psram_hal_power_clk_enable(1);

	if (s_psram_id.magic_code == PSRAM_CHECK_FLAG)
	{
		chip_id = s_psram_id.psram_id;
	}

	PSRAM_LOGI("%s, chip_id:%x\r\n", __func__, chip_id);

	// psram config
	actual_id =  psram_hal_config_init(chip_id);

	if (actual_id == 0)
	{
		PSRAM_LOGE("%s, fail!\r\n", __func__);
		return BK_FAIL;
	}

	// set psram clk
	bk_psram_set_clk(psram_clk);

	PSRAM_LOGI("%s, %x-%x\r\n", __func__, actual_id, chip_id);

	if (actual_id != chip_id)
	{
		s_psram_id.psram_id = actual_id;
		s_psram_id.magic_code = PSRAM_CHECK_FLAG;
#if (defined(CONFIG_PSRAM_AUTO_DETECT))
		if (s_psram_sem)
		{
			s_psram_id_need_write = true;

			rtos_set_semaphore(&s_psram_sem);
		}
#endif
	}
	else
	{
#if (defined(CONFIG_PSRAM_AUTO_DETECT))
		if (s_psram_sem)
		{
			rtos_set_semaphore(&s_psram_sem);
		}
#endif
	}

	s_psram_server_is_init = true;

#if defined(CONFIG_PSRAM_TEST)
        int bk_psram_register_cli_test_feature(void);
        bk_psram_register_cli_test_feature();
#endif

	return BK_OK;
#else
	return BK_FAIL;
#endif
}

bk_err_t bk_psram_deinit(void)
{
#if (defined(CONFIG_SYS_CPU0))
	if (!s_psram_server_is_init) {
		return BK_OK;
	}

	psram_hal_power_clk_enable(0);

	s_psram_server_is_init = false;

	return BK_OK;
#else
	return BK_FAIL;
#endif
}

bk_err_t bk_psram_memcpy(uint8_t *start_addr, uint8_t *data_buf, uint32_t len)
{
	int i;
	uint32_t val;
	uint8_t *pb = NULL, *pd = NULL;

	PSRAM_RETURN_ON_SERVER_NOT_INIT();

	if (((uint32_t)start_addr & 0x3) != 0 || ((uint32_t)data_buf & 0x3) != 0)
	{
		PSRAM_LOGE("address not aligen 4 byte\r\n");
		return BK_FAIL;
	}

	while (len) {
		if (len < 4) {
			val = *((uint32_t *)(start_addr));
			pb = (uint8_t *)&val;
			pd = (uint8_t *)data_buf;
			for (i = 0; i < len; i++) {
				*pb++ = *pd++;
			}
			*(uint32_t *)(start_addr) = val;
			len = 0;
		} else {
			val = *(uint32_t *)data_buf;
			*(uint32_t *)(start_addr) = val;
			data_buf += 4;
			start_addr += 4;
			len -= 4;
		}
	}

	return BK_OK;
}

bk_err_t bk_psram_memread(uint8_t *start_addr, uint8_t *data_buf, uint32_t len)
{
	int i;
	uint32_t val;
	uint8_t *pb, *pd;

	PSRAM_RETURN_ON_SERVER_NOT_INIT();

	if (((uint32_t)start_addr & 0x3) != 0 || ((uint32_t)data_buf & 0x3) != 0)
	{
		PSRAM_LOGE("address not aligen 4 byte\r\n");
		return BK_FAIL;
	}

	while (len) {
		if (len < 4) {
			val = *((uint32_t *)(start_addr));
			pb = (uint8_t *)&val;
			pd = (uint8_t *)data_buf;
			for (i = 0; i < len; i++) {
				*pd++ = *pb++;
			}
			len = 0;
		} else {
			*(uint32_t *)(start_addr) = val;
			*data_buf++ = val;
			start_addr += 4;
			len -= 4;
		}
	}

	return BK_OK;
}

char *bk_psram_strcat(char *start_addr, const char *data_buf)
{
	int i;
	uint32_t val;
	uint8_t *pb;
	uint8_t *pd = (uint8_t *)data_buf;

	if (!s_psram_server_is_init) {
		return NULL;
	}

	if(*pd == '\0')
	{
		return start_addr;
	}
	do
	{
		val = *(uint32_t *)(start_addr);
		pb = (uint8_t *)&val;
		for (i = 0; i < 4; i++) {
			if(*(pb+i) == '\0')
			{
				if(*pd == '\0')
				{
					*(pb+i) = *pd;
					break;
				}
				*(pb+i) = *pd++;
			}
		}
		*(uint32_t *)(start_addr) = val;

		if(*pd == '\0')
		{
			break;
		}
		start_addr += 4;
	} while(true);

	return start_addr;
}



