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

#include <os/os.h>
#include <driver/i2c.h>
#include "cli.h"

#include <driver/gpio.h>
#include "gpio_driver.h"

#define EEPROM_DEV_ADDR          0x50
#define EEPROM_MEM_ADDR          0x10
#define I2C_SLAVE_ADDR           0x42
#define I2C_WRITE_WAIT_MAX_MS    (500)
#define I2C_READ_WAIT_MAX_MS     (500)
#define CAMERA_DEV_ADDR          (0x21)

uint8_t sensor_gc0328c_init_talbe_test1[][2] =
{
{0xF1, 0x00},
{0xF2, 0x00},
{0xFE, 0x00},
{0x4F, 0x00},
{0x42, 0x00},
{0x77, 0x5A},
{0x78, 0x40},
{0x79, 0x56},
{0xFE, 0x00},
{0x0D, 0x01},
{0x0E, 0xE8},//480 + 8
{0x0F, 0x02},
{0x81, 0x58},
{0x82, 0x98},
{0x83, 0x60},
{0x84, 0x58},
};

static void gpio_debug(uint32_t gpio_id)
{
	gpio_dev_unmap(gpio_id);
	BK_LOG_ON_ERR(bk_gpio_disable_input(gpio_id));
	BK_LOG_ON_ERR(bk_gpio_enable_output(gpio_id));
	bk_gpio_set_output_high(gpio_id);
	bk_gpio_set_output_low(gpio_id);
	bk_gpio_set_output_high(gpio_id);
	bk_gpio_set_output_low(gpio_id);
}

static void cli_i2c_help(void)
{
	CLI_LOGI("i2c_driver init\r\n");
	CLI_LOGI("i2c_driver deinit\r\n");
	CLI_LOGI("i2c {id} init\r\n");
	CLI_LOGI("i2c {id} deinit\r\n");
	CLI_LOGI("i2c {id} memory_write {data_size}\r\n");
	CLI_LOGI("i2c {id} memory_read {data_size}\r\n");
}

static void cli_i2c_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_i2c_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_i2c_driver_init());
		CLI_LOGI("i2c driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_i2c_driver_deinit());
		CLI_LOGI("i2c driver deinit\n");
	} else {
		cli_i2c_help();
		return;
	}
}

static void cli_i2c_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_i2c_help();
		return;
	}

	uint32_t i2c_id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "init") == 0) {
		i2c_config_t i2c_cfg = {0};
		i2c_cfg.baud_rate = 400000;
		i2c_cfg.addr_mode = I2C_ADDR_MODE_7BIT;
		i2c_cfg.slave_addr = I2C_SLAVE_ADDR;

		BK_LOG_ON_ERR(bk_i2c_init(i2c_id, &i2c_cfg));
		CLI_LOGI("i2c(%d) init\n", i2c_id);
	} else if (os_strcmp(argv[2], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_i2c_deinit(i2c_id));
		CLI_LOGI("i2c(%d) deinit\n", i2c_id);
	} else if (os_strcmp(argv[2], "master_write") == 0) {
		uint8_t data_buf[10] = {0};
		for (uint32_t i = 0; i < 10; i++) {
			data_buf[i] = i & 0xff;
		}
		BK_LOG_ON_ERR(bk_i2c_master_write(i2c_id, I2C_SLAVE_ADDR, data_buf, 10, I2C_WRITE_WAIT_MAX_MS));
	} else if (os_strcmp(argv[2], "master_read") == 0) {
		uint8_t data_buf[10] = {0};
		BK_LOG_ON_ERR(bk_i2c_master_read(i2c_id, I2C_SLAVE_ADDR, data_buf, 10, I2C_READ_WAIT_MAX_MS));
		for (uint32_t i = 0; i < 10; i++) {
			CLI_LOGI("cli_test i2c_master read 0x%x,\n", data_buf[i]);
		}
#ifndef CONFIG_SIM_I2C
	} else if (os_strcmp(argv[2], "slave_write") == 0) {
		uint8_t data_buf[10] = {0};
		for (uint32_t i = 0; i < 10; i++) {
			data_buf[i] = i & 0xff;
		}
		BK_LOG_ON_ERR(bk_i2c_slave_write(i2c_id, data_buf, 10, BEKEN_NEVER_TIMEOUT));
	} else if (os_strcmp(argv[2], "slave_read") == 0) {
		uint8_t data_buf[10] = {0};
		BK_LOG_ON_ERR(bk_i2c_slave_read(i2c_id, data_buf, 10, BEKEN_NEVER_TIMEOUT));
		for (uint32_t i = 0; i < 10; i++) {
			CLI_LOGI("cli_test i2c_slave read 0x%x,\n", data_buf[i]);
		}
	} else if (os_strcmp(argv[2], "set_slave_addr") == 0) {
		uint32_t slave_addr = os_strtoul(argv[3], NULL, 16);
		bk_i2c_set_slave_address(i2c_id, slave_addr);
		CLI_LOGI("i2c_slave set address 0x%x.\n", slave_addr);
#endif
	} else if (os_strcmp(argv[2], "memory_write") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *data_buf = os_malloc(buf_len);
		if(NULL == data_buf)
		{
			CLI_LOGE("malloc fail\r\n");
			return;
		}

		for (uint32_t i = 0; i < buf_len; i++) {
			data_buf[i] = (i + 1) & 0xff;
		}
		uint32_t dev_addr = os_strtoul(argv[4], NULL, 16);
		i2c_mem_param_t mem_param = {0};
		mem_param.dev_addr = dev_addr;
		mem_param.mem_addr = EEPROM_MEM_ADDR;
		mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
		mem_param.data = data_buf;
		mem_param.data_size = buf_len;
		mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
		BK_LOG_ON_ERR(bk_i2c_memory_write(i2c_id, &mem_param));
		if (data_buf) {
			os_free(data_buf);
			data_buf = NULL;
		}
		CLI_LOGI("i2c(%d) memory_write buf_len:%d\r\n", i2c_id, buf_len);
	} else if (os_strcmp(argv[2], "memory_read") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *data_buf = os_zalloc(buf_len);
		if(NULL == data_buf)
		{
			CLI_LOGE("os_zalloc fail\r\n");
			return;
		}

		i2c_mem_param_t mem_param = {0};
		mem_param.dev_addr = EEPROM_DEV_ADDR;
		mem_param.mem_addr = EEPROM_MEM_ADDR;
		mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
		mem_param.data = data_buf;
		mem_param.data_size = buf_len;
		mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
		BK_LOG_ON_ERR(bk_i2c_memory_read(i2c_id, &mem_param));
		for (uint32_t i = 0; i < buf_len; i++) {
			CLI_LOGI("i2c_read_buf[%d]=%x\r\n", i, data_buf[i]);
		}
		if (data_buf) {
			os_free(data_buf);
			data_buf = NULL;
		}
		CLI_LOGI("i2c(%d) memory_read buf_len:%d\r\n", i2c_id, buf_len);
	} else if (os_strcmp(argv[2], "cam_write") == 0) {
		uint32_t buf_len = 1;
		uint8_t data = os_strtoul(argv[3], NULL, 16);
		uint32_t dev_addr = os_strtoul(argv[4], NULL, 16);
		uint32_t mem_addr = os_strtoul(argv[5], NULL, 16);
		i2c_mem_param_t mem_param = {0};
		mem_param.dev_addr = dev_addr;
		mem_param.mem_addr = mem_addr;
		mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
		mem_param.data = &data;
		mem_param.data_size = buf_len;
		mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
		BK_LOG_ON_ERR(bk_i2c_memory_write(i2c_id, &mem_param));
		CLI_LOGI("i2c(%d) cam_write buf_len:%d\r\n", i2c_id, buf_len);
	} else if (os_strcmp(argv[2], "cam_read") == 0) {
		uint32_t buf_len = 1;
		uint8_t data = os_strtoul(argv[3], NULL, 16);
		uint32_t dev_addr = os_strtoul(argv[4], NULL, 16);
		uint32_t mem_addr = os_strtoul(argv[5], NULL, 16);
		i2c_mem_param_t mem_param = {0};
		mem_param.dev_addr = dev_addr;
		mem_param.mem_addr = mem_addr;
		mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
		mem_param.data = &data;
		mem_param.data_size = buf_len;
		mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
		BK_LOG_ON_ERR(bk_i2c_memory_read(i2c_id, &mem_param));
		CLI_LOGI("i2c_read_buf = %x\r\n", data);
	    CLI_LOGI("i2c_read_buf = %x\r\n", *(mem_param.data));
		CLI_LOGI("i2c(%d) cam_read buf_len:%d\r\n", i2c_id, buf_len);
	} else if (os_strcmp(argv[2], "cam_test1") == 0) {
		uint32_t buf_len = 1;
		uint32_t cmp_len = sizeof(sensor_gc0328c_init_talbe_test1)/2;
		uint8_t *data_buf = os_zalloc(cmp_len);
		if(NULL == data_buf)
		{
			CLI_LOGE("os_zalloc fail\r\n");
			return;
		}
		os_printf("sizeof(sensor_gc0328c_init_talbe_test1) = %d\n ", sizeof(sensor_gc0328c_init_talbe_test1));
		for(int i = 0; i < cmp_len; i++) {
			uint32_t dev_addr = CAMERA_DEV_ADDR;
			uint32_t mem_addr = sensor_gc0328c_init_talbe_test1[i][0];

			i2c_mem_param_t mem_param_w = {0};
			mem_param_w.dev_addr = dev_addr;
			mem_param_w.mem_addr = mem_addr;
			mem_param_w.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
			mem_param_w.data = &(sensor_gc0328c_init_talbe_test1[i][1]);
			mem_param_w.data_size = buf_len;
			mem_param_w.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
			BK_LOG_ON_ERR(bk_i2c_memory_write(i2c_id, &mem_param_w));

			i2c_mem_param_t mem_param = {0};
			mem_param.dev_addr = dev_addr;
			mem_param.mem_addr = mem_addr;
			mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
			mem_param.data = data_buf + i;
			mem_param.data_size = buf_len;
			mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
			BK_LOG_ON_ERR(bk_i2c_memory_read(i2c_id, &mem_param));
			CLI_LOGI("i2c(%d) cam_read addr:0x%x, data:0x%x\r\n", i2c_id, sensor_gc0328c_init_talbe_test1[i][0], data_buf[i]);
			if(sensor_gc0328c_init_talbe_test1[i][1] != data_buf[i]) {
				CLI_LOGE("i2c(%d) cam_read addr:0x%x, data=0x%x,  correct_data=0x%x.\r\n", i2c_id, sensor_gc0328c_init_talbe_test1[i][0], data_buf[i], sensor_gc0328c_init_talbe_test1[i][1]);
				gpio_debug(19);
			}
		}
		if (data_buf) {
			os_free(data_buf);
			data_buf = NULL;
		}
	} else if (os_strcmp(argv[2], "cam_test") == 0) {
		uint32_t buf_len = 1;
		uint32_t cmp_len = 2048;
		uint8_t *data_buf = os_zalloc(cmp_len);
		if(NULL == data_buf)
		{
			CLI_LOGE("os_zalloc fail\r\n");
			return;
		}
		for(int i = 0; i < cmp_len; i++) {
			uint32_t dev_addr = CAMERA_DEV_ADDR;
			uint32_t mem_addr = 0x84;
			uint8_t test_data = i & 0xff;

			i2c_mem_param_t mem_param_w = {0};
			mem_param_w.dev_addr = dev_addr;
			mem_param_w.mem_addr = mem_addr;
			mem_param_w.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
			mem_param_w.data = &(test_data);
			mem_param_w.data_size = buf_len;
			mem_param_w.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
			BK_LOG_ON_ERR(bk_i2c_memory_write(i2c_id, &mem_param_w));

			i2c_mem_param_t mem_param = {0};
			mem_param.dev_addr = dev_addr;
			mem_param.mem_addr = mem_addr;
			mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
			mem_param.data = data_buf + i;
			mem_param.data_size = buf_len;
			mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
			BK_LOG_ON_ERR(bk_i2c_memory_read(i2c_id, &mem_param));
			CLI_LOGI("i2c(%d) cam_read addr:0x%x, data:0x%x\r\n", i2c_id, mem_addr, data_buf[i]);
			if(test_data != data_buf[i]) {
				CLI_LOGE("i2c(%d) cam_read addr:0x%x, data=0x%x,  correct_data=0x%x.\r\n", i2c_id, mem_addr, data_buf[i], test_data);
				gpio_debug(18);
			}
		}
		if (data_buf) {
			os_free(data_buf);
			data_buf = NULL;
		}
	} else if (os_strcmp(argv[2], "cam_dump") == 0) {
		uint32_t buf_len = 1;
		uint32_t cmp_len = 256;
		uint8_t *data_buf = os_zalloc(cmp_len);
		if(NULL == data_buf)
		{
			CLI_LOGE("os_zalloc fail\r\n");
			return;
		}
		uint32_t dev_addr = CAMERA_DEV_ADDR;
		uint32_t mem_addr = 0;
		i2c_mem_param_t mem_param = {0};

		for(int i = 0; i < cmp_len; i++) {
			mem_param.dev_addr = dev_addr;
			mem_param.mem_addr = mem_addr;
			mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
			mem_param.data = data_buf + i;
			mem_param.data_size = buf_len;
			mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
			BK_LOG_ON_ERR(bk_i2c_memory_read(i2c_id, &mem_param));
			mem_addr++;
		}
		os_printf("\r\n");
		for (uint32_t i = 0; i < 16; i++) {
			os_printf("REG%01x0 : ", i);
			for (uint32_t j = 0; j < 16; j++) {
				os_printf("%02x ", data_buf[i * 16 + j]);
			}
			os_printf("\r\n");
		}
		if (data_buf) {
			os_free(data_buf);
			data_buf = NULL;
		}
	} else {
		cli_i2c_help();
		return;
	}
}

#define I2C_CMD_CNT (sizeof(s_i2c_commands) / sizeof(struct cli_command))
static const struct cli_command s_i2c_commands[] = {
	{"i2c_driver", "i2c_driver {init|deinit}", cli_i2c_driver_cmd},
	{"i2c", "i2c {init|write|read}", cli_i2c_cmd},
};

int cli_i2c_init(void)
{
	BK_LOG_ON_ERR(bk_i2c_driver_init());
	return cli_register_commands(s_i2c_commands, I2C_CMD_CNT);
}

