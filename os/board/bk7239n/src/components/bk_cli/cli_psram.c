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

#include "cli.h"

#include <os/os.h>
#include <driver/psram.h>
#include <driver/aon_rtc.h>
#if defined(CONFIG_TRNG_SUPPORT)
#include <driver/trng.h>
#endif
#include "bk_general_dma.h"
#include <driver/dma.h>
#include "soc/mapping.h"
#if (defined(CONFIG_CACHE_ENABLE))
#include "cache.h"
#endif
#include "temp_detect_pub.h"

#if (defined(CONFIG_PSRAM_AUTO_DETECT))
#include "bk_ef.h"
#endif

#define TEST_PSRAM_ACCURACY     1

#define write_data(addr,val)                 *((volatile uint32_t *)(addr)) = val
#define read_data(addr,val)                  val = *((volatile uint32_t *)(addr))
#define get_addr_data(addr)                  *((volatile uint32_t *)(addr))

#if (defined(CONFIG_ARCH_RISCV))
extern u64 riscv_get_mtimer(void);
#endif

static void cli_psram_help(void)
{
	CLI_LOGI("psram_test {start|stop}\n");
}

#define PSRAM_TEST_LEN               (1024 * 4)
beken_thread_t  psram_thread_hdl = NULL;

typedef struct {
	uint8_t test_running;
	uint8_t test_mode;
	uint8_t cacheable;
	uint8_t dma_channel;
	uint32_t length;
	uint32_t *data;
	uint32_t delay_time;
} psram_debug_t;

psram_debug_t * psram_debug = NULL;

static uint64_t bk_get_current_timer(void)
{
	uint64_t timer = 0;

#ifdef CONFIG_ARCH_RISCV
	timer = riscv_get_mtimer();// tick
#else // CONFIG_ARCH_RISCV

#ifdef CONFIG_AON_RTC
	timer = bk_aon_rtc_get_us();
#endif

#endif // CONFIG_ARCH_RISCV

	return timer;
}

static uint64_t bk_get_spend_time_us(uint64_t before, uint64_t after)
{
	uint64_t spend_time = 0;

	if (after == 0 || before >= after)
	{
		spend_time = 0;
		return spend_time;
	}

#ifdef CONFIG_ARCH_RISCV
	spend_time = (after - before) / 26;
#else // CONFIG_ARCH_RISCV

#ifdef CONFIG_AON_RTC
	spend_time = after - before;
#endif

#endif // CONFIG_ARCH_RISCV

	return spend_time;
}

static void psram_cpu_write_test(void)
{
	uint32_t i = 0;
#if TEST_PSRAM_ACCURACY
	uint32_t j = 0;
	uint32_t error_num = 0;
#endif

	uint64_t rate = 0;
	uint64_t timer0, timer1;
	uint64_t total_time = 0;
	uint32_t value = 0;
	uint32_t base_addr = 0x60000000;

	if (psram_debug->cacheable)
	{
		#if !defined(CONFIG_SOC_BK7236XX)
		base_addr = 0x64000000;
		#endif
	}

#if (defined(CONFIG_PSRAM_APS6408L_O))
	uint32_t test_len = 1024 * 1024 * 8;
#elif (defined(CONFIG_PSRAM_W955D8MKY_5J))
	uint32_t test_len = 1024 * 1024 * 4;
#else //CONFIG_PSRAM_APS128XXO_OB9
	uint32_t test_len = 1024 * 1024 * 16;
#endif

	CLI_LOGI("begin write %08x-%08x test\r\n", base_addr, base_addr + test_len);

	timer0 = bk_get_current_timer();

#if TEST_PSRAM_ACCURACY
	for (j = 0; j < test_len / psram_debug->length; j ++)
	{
		for (i = 0; i < psram_debug->length; i += 4)
			write_data((base_addr + i + (j << 15)), psram_debug->data[(i >> 2) & 0x1FFF]);
	}
#else
	for (i = 0; i < test_len; i +=4)
	{
		write_data(base_addr + i, 0x11223344);
	}
#endif

	timer1 = bk_get_current_timer();

	if (timer1 > timer0)
	{
		total_time = bk_get_spend_time_us(timer0, timer1);
		rate = ((uint64_t)test_len) * 1000000 / total_time;
		CLI_LOGI("finish write, use time: %ld ms, write_rate:%ld%ld byte/s\r\n", (uint32_t)(total_time / 1000),
			(uint32_t)(rate >> 32), (uint32_t)(rate & 0xFFFFFFFF));
	}

	CLI_LOGI("begin read %08x-%08x test\r\n", base_addr, base_addr + test_len);

	timer0 = bk_get_current_timer();

	for (i = 0; i < test_len / 4; i++)
		read_data((base_addr + i * 0x4), value);

	timer1 = bk_get_current_timer();
	if (timer1 > timer0)
	{
		total_time = bk_get_spend_time_us(timer0, timer1);
		rate = ((uint64_t)test_len) * 1000000 / total_time;
		CLI_LOGI("finish read, use time: %ld ms, read_rate:%d%ld byte/s\r\n", (uint32_t)(total_time / 1000),
			(uint32_t)(rate >> 32), (uint32_t)(rate & 0xFFFFFFFF));
	}

	CLI_LOGI("%s, %d\r\n", __func__, value);

#if TEST_PSRAM_ACCURACY
	for (i = 0; i < test_len / 4; i++)
	{
		value = get_addr_data(base_addr + i * 0x4);
		if (value != (psram_debug->data[i & 0x1FFF]))
		{
			CLI_LOGI("==========%08x %08x %08x=======\n", value, psram_debug->data[i & 0x1FFF], value^psram_debug->data[i & 0x1FFF]);
			error_num++;
		}
	}

	CLI_LOGI("finish compare, error_num: %ld, corr_rate: %ld.\r\n", error_num, ((test_len / 4 - error_num) * 100 / (test_len / 4)));
#endif

	rtos_delay_milliseconds(psram_debug->delay_time);

}

static bk_err_t psram_dma_memcpy_by_chnl(void *out, const void *in, uint32_t len, dma_id_t cpy_chnl)
{
	dma_config_t dma_config = {0};

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;

	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.src.start_addr = (uint32_t)in;
	dma_config.src.end_addr = (uint32_t)(in + len);

	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.start_addr = (uint32_t)out;
	dma_config.dst.end_addr = (uint32_t)(out + len);


	bk_dma_init(cpy_chnl, &dma_config);
	bk_dma_set_transfer_len(cpy_chnl, len);
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)

	if (psram_debug->cacheable)
	{
		bk_dma_set_src_burst_len(cpy_chnl, BURST_LEN_INC8);
		bk_dma_set_dest_burst_len(cpy_chnl, BURST_LEN_INC8);
	}

	bk_dma_set_src_sec_attr(cpy_chnl, DMA_ATTR_SEC);
	bk_dma_set_dest_sec_attr(cpy_chnl, DMA_ATTR_SEC);
#endif
	bk_dma_start(cpy_chnl);

	BK_WHILE(bk_dma_get_enable_status(cpy_chnl));

	return BK_OK;
}

static void psram_dma_write_test(void)
{
	uint32_t i = 0;
	uint32_t error_num = 0;
	uint64_t rate = 0;
	uint64_t timer0, timer1;
	uint32_t total_time = 0;

	uint32_t value = 0;
	uint32_t base_addr = 0x60000000;

	if (psram_debug->cacheable)
	{
		#if !defined(CONFIG_SOC_BK7236XX)
		base_addr = 0x64000000;
		#endif
	}

#if (defined(CONFIG_PSRAM_APS6408L_O))
	uint32_t test_len = 1024 * 1024 * 8;
#elif (defined(CONFIG_PSRAM_W955D8MKY_5J))
	uint32_t test_len = 1024 * 1024 * 4;
#else //CONFIG_PSRAM_APS128XXO_OB9
	uint32_t test_len = 1024 * 1024 * 16;
#endif

	CLI_LOGI("begin write %08x-%08x test\r\n", base_addr, base_addr + test_len);

	timer0 = bk_get_current_timer();

	for (i = 0; i < test_len / psram_debug->length; i++)
	{
		psram_dma_memcpy_by_chnl((void *)(base_addr + i * psram_debug->length), psram_debug->data, psram_debug->length, psram_debug->dma_channel);
	}

	timer1 = bk_get_current_timer();
	if (timer1 > timer0)
	{
		total_time = bk_get_spend_time_us(timer0, timer1);
		rate = (test_len) * 8 / total_time;
		CLI_LOGI("finish write, use time: %ld ms, write_rate:%d Mbps\r\n", (uint32_t)(total_time / 1000), rate);
	}

	CLI_LOGI("begin read %08x-%08x test\r\n", base_addr, base_addr + test_len);

	timer0 = bk_get_current_timer();

	for (i = 0; i < test_len / 4; i++)
		read_data((base_addr + i * 0x4), value);

	timer1 = bk_get_current_timer();
	if (timer1 > timer0)
	{
		total_time = bk_get_spend_time_us(timer0, timer1);
		rate = ((uint64_t)test_len) * 8/ total_time;
		CLI_LOGI("finish read, use time: %ld ms, read_rate:%d Mbps\r\n", (uint32_t)(total_time / 1000), rate);
	}

	for (i = 0; i < test_len / psram_debug->length; i++)
	{
		for (uint32_t k = 0; k < psram_debug->length / 4; k++)
		{
			value = get_addr_data(base_addr + i * psram_debug->length + k * 0x4);

			if (value != psram_debug->data[k])
			{
				CLI_LOGI("==========%08x %08x %08x=======\n", value, psram_debug->data[k], value^psram_debug->data[k]);
				error_num++;
			}
		}
	}

	CLI_LOGI("finish compare, error_num: %ld, corr_rate: %ld.\r\n", error_num, ((test_len / 4 - error_num) * 100 / (test_len / 4)));

	rtos_delay_milliseconds(psram_debug->delay_time);

}

static void psram_write_continue_test(void)
{
	uint32_t i = 0;
	uint32_t error_num = 0;
	uint64_t rate = 0;
	uint64_t timer0, timer1;
	uint32_t total_time = 0;

	uint32_t value = 0;
	uint32_t base_addr = 0x60000000;

	if (psram_debug->cacheable)
	{
		#if !defined(CONFIG_SOC_BK7236XX)
		base_addr = 0x64000000;
		#endif
	}

#if (defined(CONFIG_PSRAM_APS6408L_O))
	uint32_t test_len = 1024 * 1024 * 8;
#elif (defined(CONFIG_PSRAM_W955D8MKY_5J))
	uint32_t test_len = 1024 * 1024 * 4;
#else //CONFIG_PSRAM_APS128XXO_OB9
	uint32_t test_len = 1024 * 1024 * 16;
#endif


	CLI_LOGI("begin write %08x-%08x test\r\n", base_addr, base_addr + test_len);
	timer0 = bk_get_current_timer();
	for (i = 0; i < test_len; i += psram_debug->length)
	{
		bk_psram_memcpy((uint8_t *)(base_addr + i), (uint8_t *)&psram_debug->data[0], psram_debug->length);
	}

	timer1 = bk_get_current_timer();
	if (timer1 > timer0)
	{
		total_time = bk_get_spend_time_us(timer0, timer1);
		rate = ((uint64_t)test_len) * 1000000 / total_time;
		CLI_LOGI("finish write, use time: %ld ms, write_rate:%d%ld byte/s\r\n", (uint32_t)(total_time / 1000),
			(uint32_t)(rate >> 32), (uint32_t)(rate & 0xFFFFFFFF));
	}


	CLI_LOGI("begin read %08x-%08x test\r\n", base_addr, base_addr + test_len);
	timer0 = bk_get_current_timer();
	for (i = 0; i < test_len / 4; i++)
		read_data((base_addr + i * 0x4), value);

	timer1 = bk_get_current_timer();
	if (timer1 > timer0)
	{
		total_time = bk_get_spend_time_us(timer0, timer1);
		rate = ((uint64_t)test_len) * 1000000 / total_time;
		CLI_LOGI("finish read, use time: %ld ms, read_rate:%d%ld byte/s\r\n", (uint32_t)(total_time / 1000),
			(uint32_t)(rate >> 32), (uint32_t)(rate & 0xFFFFFFFF));
	}

	for (i = 0; i < test_len / psram_debug->length; i++)
	{
		for (uint32_t k = 0; k < psram_debug->length / 4; k++)
		{
			value = get_addr_data(base_addr + i * psram_debug->length + k * 0x4);

			if (value != psram_debug->data[k])
			{
				error_num++;
			}
		}
	}

	CLI_LOGI("finish compare, error_num: %ld, corr_rate: %ld.\r\n", error_num, (test_len / 4 - error_num) * 100 / (test_len / 4));

	rtos_delay_milliseconds(psram_debug->delay_time);

}

static void psram_test_unit(void)
{
#if (defined(CONFIG_BK7256_SOC))

	uint32_t i = 0;
	uint32_t error_num = 0;
	uint64_t rate = 0;
	uint64_t timer0, timer1;
	uint32_t total_time = 0;

	uint32_t start_address = (uint32_t)&psram_map->reserved;
	uint32_t end_address = (8 * 1024 * 1024 + 0x60000000);
	uint32_t test_len = end_address - start_address;

	uint32_t value = 0;
	CLI_LOGI("begin write %08X-%08X, count: %d\n", start_address, end_address, test_len / 4);

	timer0 = bk_get_current_timer();

	for (i = 0; i < test_len / 4; i++)
		write_data((start_address + i * 0x4), 0x11111111 + i);

	timer1 = bk_get_current_timer();
	if (timer1 > timer0)
	{
		total_time = bk_get_spend_time_us(timer0, timer1);
		rate = ((uint64_t)test_len) * 1000000 / total_time;
		CLI_LOGI("finish write, use time: %ld ms, write_rate:%d%ld byte/s\r\n", (uint32_t)(total_time / 1000),
			(uint32_t)(rate >> 32), (uint32_t)(rate & 0xFFFFFFFF));
	}



	CLI_LOGI("begin write %08X-%08X, count: %d\n", start_address, end_address, test_len);
	timer0 = bk_get_current_timer();
	for (i = 0; i < test_len / 4; i++)
		read_data((start_address + i * 0x4), value);

	timer1 = bk_get_current_timer();
	if (timer1 > timer0)
	{
		total_time = bk_get_spend_time_us(timer0, timer1);
		rate = ((uint64_t)test_len) * 1000000 / total_time;
		CLI_LOGI("finish read, use time: %ld ms, read_rate:%d%ld byte/s\r\n", (uint32_t)(total_time / 1000),
			(uint32_t)(rate >> 32), (uint32_t)(rate & 0xFFFFFFFF));
	}



	CLI_LOGI("begin write %08X-%08X, count: %d\n", start_address, end_address, test_len);

	for (i = 0; i < test_len / 4; i++)
	{
		value = get_addr_data(start_address + i * 0x4);
		if (value != (0x11111111 + i))
			error_num++;
	}

	CLI_LOGI("finish compare, error_num: %ld, corr_rate: %ld\n", error_num, ((test_len / 4 - error_num) * 100 / (test_len / 4 ));

	rtos_delay_milliseconds(psram_debug->delay_time);
#endif
}

//TODO fix/refactoring it during v5 verification
static uint32_t psram_calibrate_read_write_test(uint32_t start_address, uint32_t test_len)
{
	uint32_t error_num = 0;
	uint32_t value = 0;
	uint32_t i = 0;

	for (i = 0; i < test_len / 4; i++)
		write_data((start_address + i * 0x4), 0x11111111 + i);

	for (i = 0; i < test_len / 4; i++)
	{
		value = get_addr_data(start_address + i * 0x4);
		if (value != (0x11111111 + i))
			error_num++;
	}

	return error_num;
}

static void psram_calibrate_test(void)
{
#if CNOFIG_PSRAM_CALIBRATE
	static uint32_t s_temperature = 0;
	uint32_t cur_temperature = -1;
	uint32_t err_cnt = 0;
	uint32_t v = 0;

	if (BK_OK != temp_detect_get_temperature(&cur_temperature)) {
		CLI_LOGE("failed to get temperature\r\n");
		return;
	}

	uint32_t diff = (cur_temperature > s_temperature) ? (cur_temperature - s_temperature) : (s_temperature - cur_temperature);
	if (diff > 100 ) { //TODO give a reasonable value
		for (int i = 0; i < 63; i++) {
			uint32_t v = (i & 7) | ((i & ~7) << 3);
			REG_WRITE(0x46080000 + (5 << 2), v);
			err_cnt = psram_calibrate_read_write_test(0x60000000, 10240);
			if (err_cnt) {
				os_printf("%d %d %d %d %d\r\n", cur_temperature, (i & 7), (i >> 6) & 3, (i >> 8) & 3, err_cnt);
			}
		}
	}
#endif
}

static void psram_test_main(void)
{
	while (psram_debug->test_running)
	{
		if (psram_debug->test_mode == 0)
		{
			psram_cpu_write_test();
		} else if (psram_debug->test_mode == 1) {
			psram_test_unit();
		} else if (psram_debug->test_mode == 2) {
			psram_write_continue_test();
		} else if (psram_debug->test_mode == 3) {
			psram_dma_write_test();
		} else {
			psram_calibrate_test();
		}
	}

	CLI_LOGI("psram_test task exit\n");

	if (psram_debug)
	{
		if (psram_debug->data)
		{
			os_free(psram_debug->data);
			psram_debug->data = NULL;
		}

		bk_dma_free(DMA_DEV_DTCM, psram_debug->dma_channel);

		os_free(psram_debug);
		psram_debug = NULL;
	}

	psram_thread_hdl = NULL;
	rtos_delete_thread(NULL);
}

static bk_err_t psram_task_init(void)
{
	bk_err_t ret = BK_OK;

	if (!psram_thread_hdl)
	{
		ret = rtos_create_thread(&psram_thread_hdl,
								 4,
								 "psram_debug",
								 (beken_thread_function_t)psram_test_main,
								 4 * 1024,
								 (beken_thread_arg_t)NULL);
		if (ret != BK_OK)
		{
			psram_thread_hdl = NULL;
			CLI_LOGE("Error: Failed to create psram test task: %d\r\n", ret);
			return BK_ERR_NOT_INIT;
		}

		return BK_OK;
	}
	else
		return BK_OK;
}

static void cli_psram_cmd_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;

	if (os_strcmp(argv[1], "start") == 0)
	{
		bk_psram_init();

		psram_debug = (psram_debug_t *)os_malloc(sizeof(psram_debug_t));

		if (psram_debug == NULL)
		{
			CLI_LOGE("psram test malloc failed!\r\n");
			msg = CLI_CMD_RSP_ERROR;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return;
		}

		os_memset(psram_debug, 0, sizeof(psram_debug_t));

		if (psram_debug->data == NULL)
		{
			psram_debug->length = 1024 * 32;
			psram_debug->data = (uint32_t *)os_malloc(psram_debug->length);
			if (psram_debug->data == NULL)
			{
				CLI_LOGE("malloc error!\r\n");
				os_free(psram_debug);
				psram_debug = NULL;
				return;
			}
		}

		for (int i = 0; i < psram_debug->length / 4; i++)
		{
#if defined(CONFIG_TRNG_SUPPORT)
			psram_debug->data[i] = bk_rand() + i;
#else
			psram_debug->data[i] = 0xA55AA55A + i + (i << 8) + (i << 16) + (i << 24);
#endif
		}

		psram_debug->test_running = 1;

		if (os_strcmp(argv[2], "cpu") == 0)
		{
			psram_debug->test_mode = 0;
		}
		else if (os_strcmp(argv[2], "conexist") == 0)
		{
			psram_debug->test_mode = 1;
		}
		else if (os_strcmp(argv[2], "continue_write") == 0)
		{
			psram_debug->test_mode = 2;
		} else if (os_strcmp(argv[2], "dma") == 0) {
			psram_debug->test_mode = 3;
			psram_debug->dma_channel = bk_dma_alloc(DMA_DEV_DTCM);
		} else if (os_strcmp(argv[2], "calibrate") == 0) {
			psram_debug->test_mode = 4;
		}

		if (os_strcmp(argv[3], "1") == 0)
		{
			psram_debug->cacheable = 1;
		}
		else
		{
			psram_debug->cacheable = 0;
		}

		psram_debug->delay_time = 500;

		if (argc >= 5)
		{
			psram_debug->delay_time = os_strtoul(argv[4], NULL, 10);

			if (psram_debug->delay_time == 0)
				psram_debug->delay_time = 500;
		}

		if (psram_task_init() != kNoErr)
		{
			CLI_LOGE("psram test failed!\r\n");
			msg = CLI_CMD_RSP_ERROR;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return;
		}

		CLI_LOGI("psram test start success!\r\n");
		msg = CLI_CMD_RSP_SUCCEED;
	}
	else if (os_strcmp(argv[1], "stop") == 0)
	{
		if (psram_thread_hdl)
		{
			psram_debug->test_running = 0;
		}

		while (psram_thread_hdl)
		{
			rtos_delay_milliseconds(10);
		}

		bk_psram_deinit();
		CLI_LOGI("psram test stop success!\r\n");
		msg = CLI_CMD_RSP_SUCCEED;
	}
	else if (os_strcmp(argv[1], "strcat") == 0)
	{
		uint8_t *data = psram_malloc(20);
		if (data == NULL)
		{
			CLI_LOGI("psram malloc error!\r\n");
			msg = CLI_CMD_RSP_ERROR;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return;
		}
		os_memset(data, 0, 20);
		bk_psram_strcat((char *)data, (char *)argv[2]);
		bk_psram_strcat((char *)data, (char *)argv[2]);
		bk_psram_strcat((char *)data, (char *)argv[2]);
		psram_free(data);
		msg = CLI_CMD_RSP_SUCCEED;
	}
	else
	{
		cli_psram_help();
		msg = CLI_CMD_RSP_ERROR;
	}

	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

static uint32_t test_frame_strip(uint8_t *src, uint32_t size)
{
	uint8_t sram_tmp[16] = {0x71, 0xfb, 0x84, 0x1f, 0x53, 0x5a, 0xd9, 0xd9, 0x8e, 0xd2, 0x76, 0x3f, 0xff, 0xff, 0xff, 0xd9};

	static uint8_t flag = 0;
	flag ++;
	sram_tmp[0] += flag;

	{
		os_printf("1====>>>> %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %p %d\n",
			src[size - 16], src[size - 15], src[size - 14], src[size - 13],
			src[size - 12], src[size - 11], src[size - 10], src[size - 9],
			src[size - 8], src[size - 7], src[size - 6], src[size - 5],
			src[size - 4], src[size - 3], src[size - 2], src[size - 1], src, size);
//		for (uint8_t i = 0; i < 16; i++)
//		{
//			src[i] = sram_tmp[i];
//		}
		bk_psram_word_memcpy(src, sram_tmp, 16);

		os_printf("2===>>> %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %p %d\n",
			src[size - 16], src[size - 15], src[size - 14], src[size - 13],
			src[size - 12], src[size - 11], src[size - 10], src[size - 9],
			src[size - 8], src[size - 7], src[size - 6], src[size - 5],
			src[size - 4], src[size - 3], src[size - 2], src[size - 1], src, size);

#if (defined(CONFIG_CACHE_ENABLE))
		flush_dcache(src, 16);
#endif

		os_printf("3==>> %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %p %d\n",
			src[size - 16], src[size - 15], src[size - 14], src[size - 13],
			src[size - 12], src[size - 11], src[size - 10], src[size - 9],
			src[size - 8], src[size - 7], src[size - 6], src[size - 5],
			src[size - 4], src[size - 3], src[size - 2], src[size - 1], src, size);

		bk_psram_word_memcpy(src, sram_tmp, 16);

		os_printf("4=> %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %p %d\n",
			src[size - 16], src[size - 15], src[size - 14], src[size - 13],
			src[size - 12], src[size - 11], src[size - 10], src[size - 9],
			src[size - 8], src[size - 7], src[size - 6], src[size - 5],
			src[size - 4], src[size - 3], src[size - 2], src[size - 1], src, size);
	}

	return size;
}


void cli_test_psram_cache_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if defined(CONFIG_DEBUG_FIRMWARE)
	uint32_t address, size;
	os_printf("cli_test_psram_cache_cmd\r\n");
	if (argc >= 3)
	{
		address = strtol(argv[1], NULL, 16);
		size = strtol(argv[2], NULL, 16);
		os_printf("test psram cache, address: 0x%08X size: 0x%08X\r\n", address, 16);

		test_frame_strip((uint8_t *)address, size);
	} else {
		os_printf("psram_cache <addr> <size>\r\n");
	}
#endif
}

#if (defined(CONFIG_MPC))
#include <driver/mpc.h>

#define BUFFER_SIZE         (34)
#define TEST_VALUE_START    0x41

static void fill_buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
	uint32_t tmpIndex = 0;

	/* Put in global buffer different values */
	for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ ) {
		pBuffer[tmpIndex] = tmpIndex + uwOffset;
	}
}

static void cli_psram_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i;
	uint8_t *test_addr_sec = NULL;
	char *msg = NULL;
	uint8_t psram_tx_buffer[BUFFER_SIZE] = {0};
	uint8_t psram_rx_buffer[BUFFER_SIZE] = {0};

	fill_buffer(psram_tx_buffer, BUFFER_SIZE, TEST_VALUE_START);

	/*set first block non-sec and second block sec*/
	bk_mpc_driver_init();
	bk_mpc_set_secure_attribute(MPC_DEV_PSRAM, 0, 1, MPC_BLOCK_NON_SECURE);
	bk_mpc_set_secure_attribute(MPC_DEV_PSRAM, bk_mpc_get_block_size(MPC_DEV_PSRAM), 1, MPC_BLOCK_SECURE);

	test_addr_sec = (uint8_t *)(SOC_PSRAM_DATA_ADDR_SEC + bk_mpc_get_block_size(MPC_DEV_PSRAM));
	bk_psram_memcpy(test_addr_sec, psram_tx_buffer, BUFFER_SIZE);
	bk_psram_memread(test_addr_sec, psram_rx_buffer, BUFFER_SIZE);

	for (i = 0; i < BUFFER_SIZE; i++) {
		bk_printf("%02x ", psram_rx_buffer[i]);
	}
	bk_printf("\r\n");
	msg = CLI_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}
#endif


static void cli_psram_cmd_handle_ext(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t addr = 0x60000000;
	uint32_t i = 0;
	uint32_t length = 512;
	char *msg = NULL;

	if (argc < 2)
	{
		msg = CLI_CMD_RSP_ERROR;
		goto out;
	}

	if (os_strcmp(argv[1], "init") == 0)
	{
		/*init psram*/
		bk_psram_init();
		msg = CLI_CMD_RSP_SUCCEED;
	}
	else if (os_strcmp(argv[1], "delete_flash") == 0)
	{
#if (defined(CONFIG_PSRAM_AUTO_DETECT))
		bk_set_env_enhance(PSRAM_CHIP_ID, NULL, 0);
		msg = CLI_CMD_RSP_SUCCEED;
#else
		msg = CLI_CMD_RSP_ERROR;
#endif
	}
	else if (os_strcmp(argv[1], "clk") == 0)
	{
		uint16_t clk = os_strtoul(argv[2], NULL, 10);

		switch (clk)
		{
			case 80:
				bk_psram_set_clk(PSRAM_80M);
				break;

			case 120:
				bk_psram_set_clk(PSRAM_120M);
				break;

			case 160:
				bk_psram_set_clk(PSRAM_160M);
				break;

			case 240:
				bk_psram_set_clk(PSRAM_240M);
				break;

			default:
				CLI_LOGE("can not support this clk!\r\n");
				break;
		}

		msg = CLI_CMD_RSP_SUCCEED;
	}
	else if (os_strcmp(argv[1], "byte") == 0)
	{
		uint8_t  value = 0;
		if (argc < 4)
		{
			msg = CLI_CMD_RSP_ERROR;
			goto out;
		}

		addr = os_strtoul(argv[2], NULL, 16);
		length = os_strtoul(argv[3], NULL, 10);
		if (length & 0x3)
		{
			length = ((length >> 2) + 1) << 2;
		}

		if (addr > 0x60800000 || addr < 0x60000000 || length == 0)
		{
			msg = CLI_CMD_RSP_ERROR;
		}
		else
		{
			for (i = 0; i < length; i++)
			{
				*((volatile uint8_t *)addr + i) = i;
			}

			for (i = 0; i < length; i++)
			{
				value = *((volatile uint8_t *)addr + i);

				if (value != (i % 256))
				{
					CLI_LOGI("index:%d, value:%d\r\n", i, value);
					break;
				}
			}

			if (i < length)
			{
				msg = CLI_CMD_RSP_ERROR;
			}
			else
			{
				msg = CLI_CMD_RSP_SUCCEED;
			}
		}
	}
	else if (os_strcmp(argv[1], "word") == 0)
	{
		if (argc < 4)
		{
			msg = CLI_CMD_RSP_ERROR;
			goto out;
		}

		addr = os_strtoul(argv[2], NULL, 16);
		length = os_strtoul(argv[3], NULL, 10);
		if (length & 0x3)
		{
			length = ((length >> 2) + 1) << 2;
		}

		if (addr > 0x60800000 || addr < 0x60000000 || length == 0)
		{
			msg = CLI_CMD_RSP_ERROR;
		}
		else
		{
			for (i = 0; i < length; i += 4)
			{
				*(((volatile uint32_t *)(addr + i))) = (i << 24) + (i << 16) + (i << 8) + i + 0x11223344;
			}
		}

		msg = CLI_CMD_RSP_SUCCEED;
	}
	else if (os_strcmp(argv[1], "rewrite") == 0)
	{
		if (argc < 3)
		{
			msg = CLI_CMD_RSP_ERROR;
			goto out;
		}

		addr = os_strtoul(argv[2], NULL, 16);
		length = 20;

		if (addr > 0x60800000 || addr < 0x60000000 || length == 0)
		{
			msg = CLI_CMD_RSP_ERROR;
			goto out;
		}

		for (i = 0; i < length; i += 4)
		{
			*(((volatile uint32_t *)(addr + i))) = 0x11111111 * i + 0x11223344;
		}

		for (i = 0; i < length * 100; i++)
		{
			*((volatile uint32_t *)addr) = 0x44332211;
		}

		for (i = 0; i < length; i ++)
		{
			CLI_LOGI("0x%08x\r\n", *((volatile uint32_t *)(addr + i * 4)));
		}

		msg = CLI_CMD_RSP_SUCCEED;
	}
	else if (os_strcmp(argv[1], "read") == 0)
	{
		if (argc < 4)
		{
			msg = CLI_CMD_RSP_ERROR;
			goto out;
		}

		addr = os_strtoul(argv[2], NULL, 16);
		length = os_strtoul(argv[3], NULL, 10);
		if (length & 0x3)
		{
			length = ((length >> 2) + 1) << 2;
		}

		if (length == 0)
		{
			msg = CLI_CMD_RSP_ERROR;
		}
		else
		{
			uint8_t *src = (uint8_t *)addr;

			for (i = 0; i < length; i++)
			{
				if ((i % 32) == 0)
				{
					CLI_LOGI("\r\n");
				}

				CLI_LOGI("%02x ", *(src + i));
			}
			CLI_LOGI("\r\n");
			msg = CLI_CMD_RSP_SUCCEED;
		}
	}
	else if (os_strcmp(argv[1], "cache") == 0)
	{
		if (argc < 4)
		{
			msg = CLI_CMD_RSP_ERROR;
			goto out;
		}

		addr = os_strtoul(argv[2], NULL, 16);
		length = os_strtoul(argv[3], NULL, 10);
		if (length & 0x3)
		{
			length = ((length >> 2) + 1) << 2;
		}

		if (length == 0)
		{
			msg = CLI_CMD_RSP_ERROR;
		}
		else
		{
#if (defined(CONFIG_CACHE_ENABLE))
			flush_dcache((uint8_t *)addr, length);
#endif
			msg = CLI_CMD_RSP_SUCCEED;
		}
	}
	else if (os_strcmp(argv[1], "cover_write") == 0)
	{
		int ret = 0;
		uint32_t start = 0, end = 0;
		uint8_t enable = 0;
		uint8_t id = 0;

		if (argc < 4)
		{
			msg = CLI_CMD_RSP_ERROR;
			goto out;
		}

		id = os_strtoul(argv[2], NULL, 10);

		if (os_strcmp(argv[3], "1") == 0)
		{
			if (argc < 6)
			{
				msg = CLI_CMD_RSP_ERROR;
				goto out;
			}
			enable = 1;
			start = os_strtoul(argv[4], NULL, 16);
			end = os_strtoul(argv[5], NULL, 16);
		}
			
		else
			enable = 0;

		if (enable)
			ret = bk_psram_enable_write_through(id, start, end);
		else
			ret = bk_psram_disable_write_through(id);

		if (ret == BK_OK)
			msg = CLI_CMD_RSP_SUCCEED;
		else
			msg = CLI_CMD_RSP_ERROR;
	}
	else if (os_strcmp(argv[1], "deinit") == 0)
	{
		/*init psram*/
		bk_psram_deinit();
		msg = CLI_CMD_RSP_SUCCEED;
	}

out:
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}


beken_thread_t  psram_task_hdl = NULL;

static void psram_task_main(void)
{
	while (1) {
		rtos_delay_milliseconds(3000);
		CLI_LOGI("psram_task_main is running.\r\n");
	}

	psram_task_hdl = NULL;
	rtos_delete_thread(NULL);
}

static void cli_create_psram_task_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;

	if (!psram_task_hdl)
	{
		ret = rtos_create_psram_thread(&psram_task_hdl,
								 4,
								 "psram_task",
								 (beken_thread_function_t)psram_task_main,
								 4 * 1024,
								 (beken_thread_arg_t)NULL);
		if (ret != BK_OK)
		{
			psram_task_hdl = NULL;
			CLI_LOGE("Error: Failed to create psram test task: %d\r\n", ret);
			return;
		}

		return;
	}
}

static void cli_delete_psram_task_handle(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if(psram_task_hdl) {
		rtos_delete_thread(&psram_task_hdl);
		psram_task_hdl = NULL;
	}
}


#define PSRAM_CNT (sizeof(s_psram_commands) / sizeof(struct cli_command))
static const struct cli_command s_psram_commands[] = {
	{"psram_test_ext", "init|byte|word|rewirte|deinit", cli_psram_cmd_handle_ext},
	{"psram_test", "start|stop", cli_psram_cmd_handle},
	{"psram_cache", "psram_cache <addr> <size>", cli_test_psram_cache_cmd},
#if (defined(CONFIG_MPC))
	{"psram_mpc", "", cli_psram_test},
#endif
	{"psram_task_create", "create task on psram", cli_create_psram_task_handle},
	{"psram_task_delete", "delete task on psram", cli_delete_psram_task_handle},
};

int cli_psram_init(void)
{
	return cli_register_commands(s_psram_commands, PSRAM_CNT);
}


