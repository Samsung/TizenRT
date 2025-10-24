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

#include <driver/qspi.h>
#include <driver/qspi_flash.h>
#include <driver/qspi_psram.h>
#include "cli.h"
#include "qspi_hw.h"

#define PSRAM_TEST_START_ADDR(_id)         (QSPI_DCACHE_BASE_ADDR(_id))
#define PSRAM_TEST_LEN                (1024 * 10)

static void cli_qspi_help(void)
{
	CLI_LOGI("qspi_driver init\r\n");
	CLI_LOGI("qspi_driver deinit\r\n");
	CLI_LOGI("qspi init\r\n");
	CLI_LOGI("qspi enter_quad_mode\r\n");
	CLI_LOGI("qspi exit_quad_mode\r\n");
	CLI_LOGI("qspi quad_write\r\n");
	CLI_LOGI("qspi quad_read\r\n");
	CLI_LOGI("qspi compare\r\n");
	CLI_LOGI("qspi_flash get_id\r\n");
	CLI_LOGI("qspi_flash erase 0 256\r\n");
	CLI_LOGI("qspi_flash single_write 0 256\r\n");
	CLI_LOGI("qspi_flash single_read 0 256\r\n");
}

static void cli_qspi_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_qspi_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_qspi_driver_init());
		CLI_LOGI("qspi driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_qspi_driver_deinit());
		CLI_LOGI("qspi driver deinit\n");
	} else {
		cli_qspi_help();
		return;
	}
}

static bk_err_t cli_qspi_psram_8bit_increase_init_memory(uint8_t *buf, uint32_t count)
{
	BK_RETURN_ON_NULL(buf);
	uint8_t *ptr = buf;
	for (int i = 0; i < count; i++) {
		ptr[i] = i & 0xff;
	}
	return BK_OK;
}

static bk_err_t cli_qspi_psram_8bit_increase_compare(uint8_t *buf, uint32_t count)
{
	BK_RETURN_ON_NULL(buf);
	uint8_t *ptr = buf;
	for (int i = 0; i < count; i ++) {
		if ((i & 0xff) != ptr[i]) {
			CLI_LOGW("qspi dcache failed [%d] addr:0x%x, value:%x/%x\r\n", i, &ptr[i], (i & 0xff), ptr[i]);
			return -1;
		}
	}
	return BK_OK;
}

static bk_err_t cli_qspi_psram_8bit_init_fixed_value(uint8_t *buf, uint32_t count, uint8_t val)
{
	BK_RETURN_ON_NULL(buf);
	uint8_t *ptr = buf;
	for (int i = 0; i < count; i++) {
		ptr[i] = val;
	}
	return BK_OK;
}

static bk_err_t cli_qspi_psram_8bit_cmp_fixed_value(uint8_t *buf, uint32_t count, uint8_t val)
{
	BK_RETURN_ON_NULL(buf);
	uint8_t *ptr = buf;
	for (int i = 0; i < count; i ++) {
		if (val != ptr[i]) {
			CLI_LOGW("qspi dcache [%d] addr:0x%x, %x/%x\r\n", i, &ptr[i], val, ptr[i]);
			return -1;
		}
	}
	return BK_OK;
}

static void cli_qspi_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_qspi_help();
		return;
	}

	uint32_t qspi_id = os_strtoul(argv[1], NULL, 10);
	CLI_LOGI("qspi_id:%08x\r\n",qspi_id);

	if (os_strcmp(argv[2], "init") == 0) {
		qspi_config_t config = {0};
		config.src_clk = os_strtoul(argv[3], NULL, 10);
		config.src_clk_div = os_strtoul(argv[4], NULL, 10);
		config.clk_div = os_strtoul(argv[5], NULL, 10);
		BK_LOG_ON_ERR(bk_qspi_init(qspi_id, &config));
		CLI_LOGI("qspi init\r\n");
	} else if (os_strcmp(argv[2], "flash_test") == 0) {
		extern void test_qspi_flash(uint32_t id, uint32_t base_addr, uint32_t buf_len);
		uint32_t base_addr = os_strtoul(argv[3], NULL, 16);
		uint32_t buf_len = os_strtoul(argv[4], NULL, 10);
		test_qspi_flash(qspi_id, base_addr, buf_len);
		CLI_LOGI("qspi flash test end\r\n");
	} else if (os_strcmp(argv[2], "enter_quad_mode") == 0) {
		BK_LOG_ON_ERR(bk_qspi_psram_enter_quad_mode(qspi_id));
		CLI_LOGI("qspi enter quad mode\r\n");
	} else if (os_strcmp(argv[2], "exit_quad_mode") == 0) {
		BK_LOG_ON_ERR(bk_qspi_psram_exit_quad_mode(qspi_id));
		CLI_LOGI("qspi exit quad mode\r\n");
	} else if (os_strcmp(argv[2], "quad_write") == 0) {
		BK_LOG_ON_ERR(bk_qspi_psram_quad_write(qspi_id));
		CLI_LOGI("qspi psram quad write mode\r\n");
	} else if (os_strcmp(argv[2], "quad_read") == 0) {
		BK_LOG_ON_ERR(bk_qspi_psram_quad_read(qspi_id));
		CLI_LOGI("qspi psram quad read mode\r\n");
	} else if (os_strcmp(argv[2], "single_write") == 0) {
		BK_LOG_ON_ERR(bk_qspi_psram_single_write(qspi_id));
		CLI_LOGI("qspi psram single write mode\r\n");
	} else if (os_strcmp(argv[2], "single_read") == 0) {
		BK_LOG_ON_ERR(bk_qspi_psram_single_read(qspi_id));
		CLI_LOGI("qspi psram single read mode\r\n");
	} else if (os_strcmp(argv[2], "compare") == 0) {
		cli_qspi_psram_8bit_increase_init_memory((uint8_t *)PSRAM_TEST_START_ADDR(qspi_id), PSRAM_TEST_LEN);
		cli_qspi_psram_8bit_increase_compare((uint8_t *)PSRAM_TEST_START_ADDR(qspi_id), PSRAM_TEST_LEN);

		cli_qspi_psram_8bit_init_fixed_value((uint8_t *)PSRAM_TEST_START_ADDR(qspi_id), PSRAM_TEST_LEN, 0xff);
		cli_qspi_psram_8bit_cmp_fixed_value((uint8_t *)PSRAM_TEST_START_ADDR(qspi_id), PSRAM_TEST_LEN, 0xff);

		cli_qspi_psram_8bit_init_fixed_value((uint8_t *)PSRAM_TEST_START_ADDR(qspi_id), PSRAM_TEST_LEN, 0x5a);
		cli_qspi_psram_8bit_cmp_fixed_value((uint8_t *)PSRAM_TEST_START_ADDR(qspi_id), PSRAM_TEST_LEN, 0x5a);
		CLI_LOGI("qspi psram write and read ok\r\n");
	} else if (os_strcmp(argv[2], "write") == 0) {
		uint32_t base_addr = os_strtoul(argv[3], NULL, 16);
		uint8_t write_data = os_strtoul(argv[4], NULL, 16);
		uint32_t write_size = os_strtoul(argv[5], NULL, 10);
		uint8_t *wr_buf = (uint8_t *)os_malloc(write_size);
		if (!wr_buf) {
			CLI_LOGE("qspi write buff malloc failed\r\n");
			return;
		}
		os_memset(wr_buf, write_data, write_size);
		BK_LOG_ON_ERR(bk_qspi_psram_write(qspi_id, base_addr, wr_buf, write_size));
		if (wr_buf) {
			os_free(wr_buf);
			wr_buf = NULL;
		}
		CLI_LOGI("qspi psram write\r\n");
	} else if (os_strcmp(argv[2], "read") == 0) {
		uint32_t base_addr = os_strtoul(argv[3], NULL, 16);
		uint32_t read_size = os_strtoul(argv[4], NULL, 10);
		uint8_t *rd_buf = (uint8_t *)os_zalloc(read_size);
		if (!rd_buf) {
			CLI_LOGE("qspi read buff malloc failed\r\n");
			return;
		}
		BK_LOG_ON_ERR(bk_qspi_psram_read(qspi_id, base_addr, rd_buf, read_size));
		for (int i = 0; i < read_size; i++) {
			CLI_LOGI("read_buf[%d]=%x\r\n", i, rd_buf[i]);
		}
		if (rd_buf) {
			os_free(rd_buf);
			rd_buf = NULL;
		}
		CLI_LOGI("qspi psram read\r\n");
	} else {
		cli_qspi_help();
	}
}

#define FLASH_PAGE_SIZE 256
#define FLASH_SECTOR_SIZE 0x1000

static void cli_qspi_flash_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_qspi_help();
		return;
	}

	uint32_t qspi_id = os_strtoul(argv[1], NULL, 10);
	uint32_t start_addr = os_strtoul(argv[3], NULL, 16);
	uint32_t len = os_strtoul(argv[4], NULL, 10);

	if (os_strcmp(argv[2], "erase") == 0) {
		for (uint32_t addr = start_addr; addr < (start_addr + len); addr += FLASH_SECTOR_SIZE) {
			bk_qspi_flash_erase_sector(qspi_id, addr);
		}

	} else if (os_strcmp(argv[2], "read") == 0) {
		uint8_t buf[FLASH_PAGE_SIZE] = {0};
		for (uint32_t addr = start_addr; addr < (start_addr + len); addr += FLASH_PAGE_SIZE) {
			os_memset(buf, 0, FLASH_PAGE_SIZE);
			bk_qspi_flash_read(qspi_id, addr, buf, FLASH_PAGE_SIZE);
			CLI_LOGI("flash read addr:%x\r\n", addr);

			CLI_LOGI("dump read flash data:\r\n");
			for (uint32_t i = 0; i < 16; i++) {
				for (uint32_t j = 0; j < 16; j++) {
					os_printf("%02x ", buf[i * 16 + j]);
				}
				os_printf("\r\n");
			}
		}
	} else if (os_strcmp(argv[2], "write") == 0) {
		uint8_t buf[FLASH_PAGE_SIZE] = {0};
		for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i++) {
			buf[i] = i;
		}

		for (uint32_t addr = start_addr; addr < (start_addr + len); addr += FLASH_PAGE_SIZE) {
			bk_qspi_flash_write(qspi_id, addr, buf, FLASH_PAGE_SIZE);
		}
	} else if (os_strcmp(argv[2], "single_read") == 0) {
		uint8_t buf[FLASH_PAGE_SIZE] = {0};
		for (uint32_t addr = start_addr; addr < (start_addr + len); addr += FLASH_PAGE_SIZE) {
			os_memset(buf, 0, FLASH_PAGE_SIZE);
			bk_qspi_flash_single_read(qspi_id, addr, buf, FLASH_PAGE_SIZE);
			CLI_LOGI("flash read addr:%x\r\n", addr);

			CLI_LOGI("dump read flash data:\r\n");
			for (uint32_t i = 0; i < 16; i++) {
				for (uint32_t j = 0; j < 16; j++) {
					os_printf("%02x ", buf[i * 16 + j]);
				}
				os_printf("\r\n");
			}
		}
	} else if (os_strcmp(argv[2], "single_write") == 0) {
		uint8_t buf[FLASH_PAGE_SIZE] = {0};
		for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i++) {
			buf[i] = i;
		}

		for (uint32_t addr = start_addr; addr < (start_addr + len); addr += FLASH_PAGE_SIZE) {
			bk_qspi_flash_single_page_program(qspi_id, addr, buf, FLASH_PAGE_SIZE);
		}

	} else if (os_strcmp(argv[2], "get_id") == 0) {
		uint32_t flash_id = bk_qspi_flash_read_id(qspi_id);
		bk_qspi_flash_set_protect_none(qspi_id);
		bk_qspi_flash_quad_enable(qspi_id);
		CLI_LOGI("flash_id:%x\r\n", flash_id);
	}
}


#define QSPI_CMD_CNT (sizeof(s_qspi_commands) / sizeof(struct cli_command))
static const struct cli_command s_qspi_commands[] = {
	{"qspi_driver", "qspi_driver {init|deinit}", cli_qspi_driver_cmd},
	{"qspi", "qspi {init|write|read}", cli_qspi_cmd},
	{"qspi_flash", "qspi_flash {write|read}", cli_qspi_flash_cmd},
};

int cli_qspi_init(void)
{
	BK_LOG_ON_ERR(bk_qspi_driver_init());
	return cli_register_commands(s_qspi_commands, QSPI_CMD_CNT);
}

