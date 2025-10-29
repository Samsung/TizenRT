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

#include "cli.h"
#include <components/system.h>
#include <os/str.h>
#include "sys_hal.h"
#include "icu_hal.h"
#include "pwm_hal.h"
#include "timer_hal.h"
#include "gpio_hal.h"
#include "dma_hal.h"
#include "uart_hal.h"
#include "wdt_hal.h"
#include "trng_hal.h"
#include "efuse_hal.h"
#include "adc_hal.h"
#include "spi_hal.h"
#include "i2c_hal.h"

#if defined(CONFIG_QSPI)
#include "qspi_hal.h"
#endif
#if defined(CONFIG_AON_RTC)
#include "aon_rtc_hal.h"
#endif
#if defined(CONFIG_CALENDAR)
#include "calendar_hal.h"
#endif
#if defined(CONFIG_FLASH)
#include "flash_hal.h"
#endif
#if defined(CONFIG_SDIO_HOST)
#include "sdio_host_hal.h"
#endif
#if defined(CONFIG_YUV_BUF)
#include "yuv_buf_hal.h"
#endif
#if defined(CONFIG_PRRO)
#include "prro_hal.h"
#endif
#if defined(CONFIG_MPC_TEST)
#include "mpc_hal.h"
#endif
#if defined(CONFIG_AON_PMU_TEST)
#include "aon_pmu_hal.h"
#endif
#if defined(CONFIG_SDMADC)
#include "sdmadc_hal.h"
#endif

#if defined(CONFIG_TFM_REG_ACCESS_NSC)
#include "tfm_reg_nsc.h"
#endif


static int hex2num(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

static int hex2byte(const char *hex)
{
	int a, b;
	a = hex2num(*hex++);
	if (a < 0)
		return -1;
	b = hex2num(*hex++);
	if (b < 0)
		return -1;
	return (a << 4) | b;
}

/**
 * hexstr2bin - Convert ASCII hex string into binary data
 * @hex: ASCII hex string (e.g., "01ab")
 * @buf: Buffer for the binary data
 * @len: Length of the text to convert in bytes (of buf); hex will be double
 * this size
 * Returns: 0 on success, -1 on failure (invalid hex string)
 */
static int cli_hexstr2bin(const char *hex, u8 *buf, size_t len)
{
	size_t i;
	int a;
	const char *ipos = hex;
	u8 *opos = buf;

	for (i = 0; i < len; i++) {
		a = hex2byte(ipos);
		if (a < 0)
			return -1;
		*opos++ = a;
		ipos += 2;
	}
	return 0;
}

#if defined(CONFIG_JPEGENC_HW)
#include "jpeg_hal.h"
#endif


static void cli_reg_write_read_cmd_common(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv, bool is_secure)
{
	UINT32 reg_addr = 0, reg_value = 0;
	UINT8 optr_len = 0, optr_tab[9];

	os_memset(optr_tab, 0, 9);
	os_memset(optr_tab, 0x30, 8);

	if (os_strncmp(argv[1], "-r", 2) == 0) {
		if (argc != 3) {
			CLI_LOGI("regshow -r addr\r\n");
			return;
		}

		optr_len = os_strlen(argv[2]);
		if (optr_len > 8) {
			CLI_LOGI("addr 0-FFFFFFFF\r\n");
			return;
		}
		optr_len = 8 - optr_len;
		os_memcpy(&optr_tab[optr_len], argv[2], os_strlen(argv[2]));

		cli_hexstr2bin((char *)optr_tab, (u8 *)&reg_addr, 4);
		reg_addr = __ntohl(reg_addr);
		if (is_secure) {
			CLI_LOGI("regshow R: addr:0x%08x, value:0x%08x\r\n", reg_addr, REG_READ(reg_addr));
		} else {
#if defined(CONFIG_TFM_REG_ACCESS_NSC)
			CLI_LOGI("regshow R: addr:0x%08x, value:0x%08x\r\n", reg_addr, psa_reg_read(reg_addr));
#endif
		}
	} else if (os_strncmp(argv[1], "-w", 2) == 0) {
		if (argc != 4) {
			CLI_LOGI("regshow -w addr value\r\n");
			return;
		}

		optr_len = os_strlen(argv[2]);
		if (optr_len > 8) {
			CLI_LOGI("addr 0-FFFFFFFF\r\n");
			return;
		}
		optr_len = 8 - optr_len;
		os_memcpy(&optr_tab[optr_len], argv[2], os_strlen(argv[2]));

		cli_hexstr2bin((char *)optr_tab, (u8 *)&reg_addr, 4);
		reg_addr = __ntohl(reg_addr);


		os_memset(optr_tab, 0x30, 8);
		optr_len = os_strlen(argv[3]);
		if (optr_len > 8) {
			CLI_LOGI("value 0-FFFFFFFF\r\n");
			return;
		}
		optr_len = 8 - optr_len;
		os_memcpy(&optr_tab[optr_len], argv[3], os_strlen(argv[3]));
		cli_hexstr2bin((char *)optr_tab, (u8 *)&reg_value, 4);
		reg_value = __ntohl(reg_value);

		if (is_secure == false) {
#if defined(CONFIG_TFM_REG_ACCESS_NSC)
			psa_reg_write(reg_addr, reg_value);
			return;
#endif
		} else {
			REG_WRITE(reg_addr, reg_value);
		}

#if (CLI_CFG_WIFI == 1)
		extern INT32 rwnx_cal_save_trx_rcbekn_reg_val(void);
		// when write trx and rc beken regs, updata registers save.
		if ((reg_addr & 0xfff0000) == 0x1050000)
			rwnx_cal_save_trx_rcbekn_reg_val();
#endif

		CLI_LOGI("regshow W: addr:0x%08x, value:0x%08x - check:0x%08x\r\n",
				  reg_addr, reg_value, REG_READ(reg_addr));
	} else
		CLI_LOGI("regshow -w/r addr [value]\r\n");
}

static void cli_reg_write_read_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	cli_reg_write_read_cmd_common(pcWriteBuffer, xWriteBufferLen, argc, argv, true);
}


#if defined(CONFIG_TZ) && (CONFIG_SPE == 0)
static void cli_reg_write_read_s_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	cli_reg_write_read_cmd_common(pcWriteBuffer, xWriteBufferLen, argc, argv, false);
}
#endif



static void cli_reg_dump_help(void)
{
	CLI_LOGI("regdump icu\n");
	CLI_LOGI("        pwm\n");
	CLI_LOGI("        gpio [index]\n");
	CLI_LOGI("        timer\n");
	CLI_LOGI("        dma [channel]\n");
	CLI_LOGI("        uart id\n");
	CLI_LOGI("        wdt\n");
	CLI_LOGI("        trng\n");
	CLI_LOGI("        efuse\n");
	CLI_LOGI("        adc\n");
	CLI_LOGI("        sdmadc\n");
	CLI_LOGI("        spi\n");
}


static void cli_reg_dump_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc == 1) {
		cli_reg_dump_help();
		return;
	}
#if defined(CONFIG_DEBUG_FIRMWARE)
	uint8_t index = 0;

	if (argc > 1) {
		index = os_strtoul(argv[2], NULL, 10);
	}

	if (os_strcmp(argv[1], "icu") == 0) {
#if (defined(CONFIG_ICU))
		icu_struct_dump();
#endif
	} else if (os_strcmp(argv[1], "sys") == 0){
#if (defined(CONFIG_HAL_DEBUG_SYS))
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t start = 0;
		uint32_t end = 0;
		start = os_strtoul(argv[2], NULL, 10);
		end = os_strtoul(argv[3], NULL, 10);
		sys_struct_dump(start, end);
#endif
	} else if (os_strcmp(argv[1], "pwm") == 0) {
		pwm_struct_dump();
	} else if (os_strcmp(argv[1], "timer") == 0) {
		timer_struct_dump();
	} else if (os_strcmp(argv[1], "gpio") == 0) {
		gpio_struct_dump(index);
	} else if (os_strcmp(argv[1], "dma") == 0) {
		dma_struct_dump(index);
	} else if (os_strcmp(argv[1], "uart") == 0) {
		uart_struct_dump(index);
	} else if (os_strcmp(argv[1], "wdt") == 0) {
		wdt_struct_dump();
	} else if (os_strcmp(argv[1], "trng") == 0) {
#if (defined(CONFIG_TRNG_SUPPORT))
		trng_struct_dump();
#endif
	} else if (os_strcmp(argv[1], "efuse") == 0) {
#if (defined(CONFIG_EFUSE))
		efuse_struct_dump();
#endif
	} else if (os_strcmp(argv[1], "adc") == 0) {
		adc_struct_dump();
	}
#if (defined(CONFIG_SDMADC))
	else if (os_strcmp(argv[1], "sdmadc") == 0) {
		sdmadc_struct_dump(0x2,0x8);
	}
#endif 
	else if (os_strcmp(argv[1], "spi") == 0) {
		spi_struct_dump(index);
	} else if (os_strcmp(argv[1], "i2c") == 0) {
		i2c_struct_dump(index);
	}
#if defined(CONFIG_QSPI)
	else if (os_strcmp(argv[1], "qspi") == 0) {
		uint32_t id = 0;
		qspi_struct_dump(id);
	}
#endif
#if defined(CONFIG_AON_RTC)
	else if (os_strcmp(argv[1], "aon_rtc") == 0) {
		aon_rtc_struct_dump();
	}
#endif
#if defined(CONFIG_JPEGENC_HW)
	else if (os_strcmp(argv[1], "jpeg") == 0) {
		jpeg_struct_dump();
	}
#endif
#if defined(CONFIG_CALENDAR)
	else if (os_strcmp(argv[1], "calendar") == 0) {
		calendar_struct_dump();
	}
#endif
#if defined(CONFIG_FLASH)
	else if (os_strcmp(argv[1], "flash") == 0) {
		flash_struct_dump();
	}
#endif
#if defined(CONFIG_SDIO_HOST)
	else if (os_strcmp(argv[1], "sdio_host") == 0) {
		sdio_host_struct_dump();
	}
#endif
#if defined(CONFIG_YUV_BUF)
	else if (os_strcmp(argv[1], "yuv_buf") == 0) {
		yuv_buf_struct_dump();
	}
#endif
#if defined(CONFIG_SYSTEM_CLOCK)
	else if (os_strcmp(argv[1], "sys_clock") == 0) {
		//extern void sys_clock_struct_dump(void);
		//sys_clock_struct_dump();
	}
#endif

#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
#if defined(CONFIG_PRRO_TEST)
	else if (os_strcmp(argv[1], "prro") == 0) {
		prro_struct_dump();
	}
#endif
#if defined(CONFIG_MPC_TEST)
	else if (os_strcmp(argv[1], "mpc") == 0) {
		mpc_dev_t dev =  os_strtoul(argv[2], NULL, 10);
		mpc_struct_dump(dev);
	}
#endif
#endif

#if defined(CONFIG_AON_PMU_TEST)
	else if (os_strcmp(argv[1], "pmu") == 0) {
		aon_pmu_struct_dump(0, -1);
	}
#endif
	else {
		cli_reg_dump_help();
		return;
	}
#endif
}

#define REG_CMD_CNT (sizeof(s_reg_commands) / sizeof(struct cli_command))
static const struct cli_command s_reg_commands[] = {
#if defined(CONFIG_TFM_REG_ACCESS_NSC)
	{"regshow_s", "regshow -w/r addr [value]", cli_reg_write_read_s_cmd},
#endif
	{"regshow", "regshow -w/r addr [value]", cli_reg_write_read_cmd},
	{"regdump", "regdump {module}", cli_reg_dump_cmd},
};

int cli_reg_init(void)
{
	return cli_register_commands(s_reg_commands, REG_CMD_CNT);
}
