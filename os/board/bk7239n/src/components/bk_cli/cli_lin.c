// Copyright 2023-2025 Beken
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

#include <os/os.h>
#include <driver/lin.h>
#include <driver/int.h>
#include <driver/trng.h>
#include "cli.h"

#include <driver/gpio.h>
#include "gpio_driver.h"
#include "lin_hal.h"

#include <driver/lin_types.h>
#include "clock_driver.h"
#include <os/mem.h>
#include "power_driver.h"
#include <os/os.h>
#include "lin_hw.h"
#include "lin_driver.h"
#include "lin_hal.h"
#include "sys_driver.h"
#include "lin_statis.h"

#define LIN_MAX_DELAY	(0xFFFFFFFF)
#define LIN_DELAY		(65)

static uint8_t tx_buf[LIN_DATA_LEN_MAX];
static uint8_t rx_buf[LIN_DATA_LEN_MAX];
static lin_config_t s_lin_cfg_test;

static void cli_lin_help(void)
{
	CLI_LOGI("lin_cfg {master|slave} {channel} {checksum} {rate} {bus_inactiv_time} {wup_repeat_time}\r\n");
	CLI_LOGI("lin {id} {tx|rx} {data}\r\n");
	CLI_LOGI("lin_stress {master|slaver}\r\n");
	CLI_LOGI("lin_chn {0|1|2}\r\n");
	CLI_LOGI("lin_sleep_mode {wake|sleep}");
	CLI_LOGI("lin_statis {id} {dump|reset}\r\n");
}

static void cli_lin_cfg(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 6) {
		cli_lin_help();
		return;
	}

	if (os_strcmp(argv[1], "master") == 0) {
		CLI_LOGI("lin master config\r\n");
		s_lin_cfg_test.dev = LIN_MASTER;
		s_lin_cfg_test.chn = os_strtoul(argv[2], NULL, 10);
		s_lin_cfg_test.rate = os_strtoul(argv[4], NULL, 10);
		s_lin_cfg_test.checksum = os_strtoul(argv[3], NULL, 10);
	} else if (os_strcmp(argv[1], "slave") == 0) {
		CLI_LOGI("lin slave config\r\n");
		s_lin_cfg_test.dev = LIN_SLAVE;
		s_lin_cfg_test.chn = os_strtoul(argv[2], NULL, 10);
		s_lin_cfg_test.checksum = os_strtoul(argv[3], NULL, 10);
		s_lin_cfg_test.bus_inactiv_time = os_strtoul(argv[5], NULL, 10);
		s_lin_cfg_test.wup_repeat_time = os_strtoul(argv[6], NULL, 10);
	} else {
		cli_lin_help();
		return;
	}
	bk_lin_cfg(&s_lin_cfg_test);
}

static void cli_lin_trans(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int size;

	if (argc < 4) {
		cli_lin_help();
		return;
	}
	BK_LOG_ON_ERR(bk_lin_driver_init());

	LIN_LOGI("dev = %s\r\n", bk_lin_get_dev() ? "master" : "slave");
	LIN_LOGI("length = %d\r\n", bk_lin_get_data_length());
	LIN_LOGI("enc_check = %d\r\n", bk_lin_get_enh_check());
	LIN_LOGI("rate = %lf\r\n", bk_lin_get_rate());
	LIN_LOGI("bus_inactiv_time = %d\r\n", bk_lin_get_bus_inactivity_time());
	LIN_LOGI("wup_repeat_time = %d\r\n", bk_lin_get_wup_repeat_time());

	uint32_t id = os_strtoul(argv[1], NULL, 10);
	os_strncpy((char *)tx_buf, argv[3], LIN_DATA_LEN_8BYTES);
	if (os_strcmp(argv[2], "tx") == 0) {
		size = os_strlen(argv[3]);
		CLI_LOGI("tx_buf:[%s]\r\n", tx_buf);
		bk_lin_tx(id, tx_buf, size);
	} else if (os_strcmp(argv[2], "rx") == 0) {
		size = os_strtoul(argv[3], NULL, 10);
		os_memset(rx_buf, 0 ,LIN_DATA_LEN_MAX);
		bk_lin_rx(id, rx_buf, size, 15000);
		CLI_LOGI("rx_buf:[%s]\r\n", rx_buf);
	} else {
		CLI_LOGI("input format or para error\r\n ");
	}

}

void cli_lin_master_stress_test(void)
{
	uint32_t i = 0;
	uint32_t len = 0;
	uint32_t rand_count = 0;
	bk_trng_driver_init();
	bk_trng_start();
	BK_LOG_ON_ERR(bk_lin_driver_init());
	CLI_LOGI("master stress begin*******************************\r\n");
	while (1) {
		CLI_LOGI("++++++++++++++++++++case[%d] master start+++++++++\r\n", i);
		rand_count = bk_rand() % 10 + 1;
		os_memset(tx_buf, 0 ,LIN_DATA_LEN_MAX);
		os_memset(rx_buf, 0 ,LIN_DATA_LEN_MAX);
		sprintf((char *)tx_buf, "%08d", rand_count);
		CLI_LOGI("tx_buf:[%s]\r\n", tx_buf);
		bk_lin_tx(1, tx_buf, LIN_DATA_LEN_8BYTES);

		rtos_delay_milliseconds(LIN_DELAY);
		bk_lin_rx(1, rx_buf, LIN_DATA_LEN_8BYTES, LIN_MAX_DELAY);
		len = os_strlen((char *)rx_buf);
		CLI_LOGI("rx_buf:[%s]\r\n", rx_buf);
		if (len == 0) {
			CLI_LOGI("-------------------case[%d] master result:FAILED---\r\n", i++);
		} else {
			CLI_LOGI("-------------------case[%d] master result:PASS---\r\n", i++);
		}
		rtos_delay_milliseconds((bk_rand() % 10 + 1) * 1000);
	}
}

void cli_lin_slave_stress_test(void)
{

	uint32_t i = 0;
	uint32_t rand_count = 0;
	bk_trng_driver_init();
	bk_trng_start();
	BK_LOG_ON_ERR(bk_lin_driver_init());
	CLI_LOGI("slave stress begin*******************************\r\n");
	while (1) {
		CLI_LOGI("++++++++++++++++++++case[%d] slave start++++++++++\r\n", i);
		rand_count = bk_rand() % 10 + 11;
		os_memset(tx_buf, 0 ,LIN_DATA_LEN_MAX);
		os_memset(rx_buf, 0 ,LIN_DATA_LEN_MAX);
		bk_lin_rx(1, rx_buf, LIN_DATA_LEN_8BYTES, LIN_MAX_DELAY);
		CLI_LOGI("rx_buf:[%s]\r\n", rx_buf);

		sprintf((char *)tx_buf, "%08d", rand_count);
		CLI_LOGI("tx_buf:[%s]\r\n", tx_buf);
		bk_lin_tx(1, tx_buf, LIN_DATA_LEN_8BYTES);
		rtos_delay_milliseconds(LIN_DELAY);
		CLI_LOGI("-------------------case[%d] slave result:PASS----\r\n", i++);
	}
}

static void cli_lin_driver_stress(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_lin_help();
		return;
	}
	if(os_strcmp(argv[1], "master") == 0) {
		cli_lin_master_stress_test();
	} else {
		cli_lin_slave_stress_test();
	}
	return;

}

static void cli_lin_chn(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_lin_help();
		return;
	}

	uint32_t chn = os_strtoul(argv[1], NULL, 10);
	CLI_LOGI("chn=%d\r\n", chn);
	bk_lin_gpio_init(chn);

	return;
}

static void cli_lin_sleep_mode(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_lin_help();
		return;
	}
	BK_LOG_ON_ERR(bk_lin_driver_init());
	if(os_strcmp(argv[1], "sleep") == 0) {
		bk_lin_set_sleep();
	} else {
		bk_lin_set_wakeup();
	}
}

static void cli_lin_statis(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_lin_help();
		return;
	}

#if defined(CONFIG_LIN_STATIS)
	uint32_t lin_id;
	lin_id = os_strtoul(argv[1], NULL, 10);
	if (os_strcmp(argv[2], "dump") == 0) {
		lin_statis_dump(lin_id);
		CLI_LOGI("lin dump statis ok\r\n");
	} else if (os_strcmp(argv[2], "reset") == 0) {
		lin_statis_id_init(lin_id);
		CLI_LOGI("lin reset statis ok\r\n");
	}
#endif

	return;
}

#define LIN_CMD_CNT (sizeof(s_lin_commands) / sizeof(struct cli_command))
static const struct cli_command s_lin_commands[] = {
	{"lin_cfg", "lin_cfg {master|slave} {channel} {checksum} {rate} {bus_inactiv_time} {wup_repeat_time}", cli_lin_cfg},
	{"lin", "lin {id} {tx|rx} {data}", cli_lin_trans},
	{"lin_stress", "lin_stress {master|slaver}", cli_lin_driver_stress},
	{"lin_chn", "lin_chn {0|1|2}", cli_lin_chn},
	{"lin_sleep_mode", "lin_sleep_mode {wake|sleep}", cli_lin_sleep_mode},
	{"lin_statis", "lin_statis {id} {dump|reset}", cli_lin_statis},
};

int cli_lin_init(void)
{
	BK_LOG_ON_ERR(bk_lin_driver_init());
	return cli_register_commands(s_lin_commands, LIN_CMD_CNT);
}