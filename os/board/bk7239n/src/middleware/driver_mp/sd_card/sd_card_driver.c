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

#include <os/mem.h>
#include <driver/sdio_host.h>
#include <driver/sd_card.h>
#include "sd_card_driver.h"
#include <driver/gpio.h>
#include "gpio_driver.h"

#if defined(CONFIG_SDIO_V2P0)
#include "sys_driver.h"

//TODO:These private API should move to private header file.
extern void bk_sdio_host_reset_sd_state(void);
extern void bk_sdio_clk_gate_config(uint32_t enable);
extern void bk_sdio_tx_fifo_clk_gate_config(uint32_t enable);
extern void bk_sdio_clock_en(uint32_t enable);

/**/
typedef enum
{
	SDCARD_OPS_READ,
	SDCARD_OPS_WRITE,
	SDCARD_OPS_SYNC_RW,	//sync read or write
}sdcard_rw_ops_t;

typedef enum
{
	SDCARD_RW_STATE_INITIALIZED,
	SDCARD_RW_STATE_READING,
	SDCARD_RW_STATE_WRITING,
	SDCARD_RW_STATE_ENDED,
}sdcard_rw_state_t;

static sdcard_rw_state_t sd_card_check_continious_rw(sdcard_rw_ops_t ops, uint32_t addr, uint32_t blk_cnt);
#endif

#define SD_CARD_MAX_VOLT_TRIAL_COUNT    0xff

#define SD_CARD_MAX_CMD_TRIAL_COUNT	(128)
#define SD_CARD_MAX_DATA_TRIAL_COUNT	(128)

#if (defined(CONFIG_SDIO_V2P0))	//Temp code, clock module should re-arch.
#define CMD_TIMEOUT_100K	2500
#define DATA_TIMEOUT_100K	10000

#define CMD_TIMEOUT_6_5_M	6500
#define DATA_TIMEOUT_6_5_M  208000

#define CMD_TIMEOUT_13M		13000	//1ms, max retry_cnt 128(SD_CARD_MAX_CMD_TRIAL_COUNT)
#define DATA_TIMEOUT_13M	416000	//32ms, max retry_cnt 128(SD_CARD_MAX_DATA_TRIAL_COUNT)

#define CMD_TIMEOUT_20M		20000	//1ms, max retry_cnt 128
#define DATA_TIMEOUT_20M	640000	//32ms, max retry_cnt 128

#define CMD_TIMEOUT_40M		4000	//1ms, max retry_cnt 128
#define DATA_TIMEOUT_40M	1280000	//32ms, max retry_cnt 128

#define CMD_TIMEOUT_80M		80000	//1ms, read/write file 60000 times, the max wait is about 2.4ms(cmd retry max count==24)
#define DATA_TIMEOUT_80M	2560000	//32ms, max retry_cnt 128:some SDCARD(Sandisk 16G,Class-4) after read 10M data later,read a block takes more then 16ms
#else
#define CMD_TIMEOUT_200K	5000	//about 5us per cycle (25ms)
#define DATA_TIMEOUT_200K	20000 //100ms

#define CMD_TIMEOUT_6_5_M	300000 //about 150ns per cycle (45ms)
#define DATA_TIMEOUT_6_5_M  3000000 //450ms

#define CMD_TIMEOUT_13M		600000 //about 77ns pr cycle (45ms)
#define DATA_TIMEOUT_13M	6000000 //450ms

#define CMD_TIMEOUT_26M		1200000//about 38ns pr cycle (45ms)
#define DATA_TIMEOUT_26M	12000000 //450ms
#endif

typedef struct {
	sd_card_info_t sd_card; /**< sd card information */
	uint32_t cid[4]; /**< sd card CID register, it contains the card identification information */
	sd_card_csd_t csd;
	sdio_host_clock_freq_t clock_freq;
} sd_card_obj_t;

#if defined(CONFIG_SDIO_V2P0)
static beken_mutex_t s_mutex_sdcard;
#define CONFIG_SDCARD_OPS_TRACE_EN (0)
#if (defined(CONFIG_SDCARD_OPS_TRACE_EN))
typedef struct
{
	uint32_t ops	: 3;	//last operation, init:1, deinit:2, read:3, write:4
	uint32_t init	: 4;	//init step trace
	uint32_t deinit	: 4;
	uint32_t read 	: 3;
	uint32_t write 	: 3;
}sd_card_sw_status_t;
static volatile sd_card_sw_status_t s_sdcard_sw_status;
#endif
#endif
static bool s_sd_card_is_init = false;
static sd_card_obj_t s_sd_card_obj = {0};

#if defined(CONFIG_SDCARD_CHECK_INSERTION_EN)
static bool sd_card_get_insert_status(void)
{
	gpio_id_t check_insert_gpio_id = CONFIG_SDCARD_CHECK_INSERTION_GPIO_ID;

	gpio_dev_unmap(check_insert_gpio_id);
	BK_LOG_ON_ERR(bk_gpio_disable_output(check_insert_gpio_id));
	BK_LOG_ON_ERR(bk_gpio_enable_input(check_insert_gpio_id));
	BK_LOG_ON_ERR(bk_gpio_enable_pull(check_insert_gpio_id));
	BK_LOG_ON_ERR(bk_gpio_pull_up(check_insert_gpio_id));

	return bk_gpio_get_input(check_insert_gpio_id);
}
#endif

static uint32 sd_card_get_cmd_timeout_param(void)
{
	uint32 timeout_param;
	switch (s_sd_card_obj.clock_freq) {
#if defined(CONFIG_SDIO_V2P0)
	case SDIO_HOST_CLK_100K:
		timeout_param = CMD_TIMEOUT_100K;
		break;
#else
	case SDIO_HOST_CLK_200K:
		timeout_param = CMD_TIMEOUT_200K;
		break;
#endif
	case SDIO_HOST_CLK_6_5M:
		timeout_param = CMD_TIMEOUT_6_5_M;
		break;
	case SDIO_HOST_CLK_13M:
		timeout_param = CMD_TIMEOUT_13M;
		break;

#if defined(CONFIG_SDIO_V2P0)	//hasn't 26M
	case SDIO_HOST_CLK_20M:
		timeout_param = CMD_TIMEOUT_20M;
		break;

	case SDIO_HOST_CLK_40M:
		timeout_param = CMD_TIMEOUT_40M;
		break;

	case SDIO_HOST_CLK_80M:
		timeout_param = CMD_TIMEOUT_80M;
		break;

	default:
		timeout_param = CMD_TIMEOUT_13M;
		break;
#else
	case SDIO_HOST_CLK_26M:
	default:
		timeout_param = CMD_TIMEOUT_26M;
		break;
#endif
	}
	return timeout_param;
}

static uint32 sd_card_get_data_timeout_param(void)
{
	uint32 timeout_param;
	switch (s_sd_card_obj.clock_freq) {
#if defined(CONFIG_SDIO_V2P0)
	case SDIO_HOST_CLK_100K:
		timeout_param = DATA_TIMEOUT_100K;
		break;
#else
	case SDIO_HOST_CLK_200K:
		timeout_param = DATA_TIMEOUT_200K;
		break;
#endif
	case SDIO_HOST_CLK_6_5M:
		timeout_param = DATA_TIMEOUT_6_5_M;
		break;
	case SDIO_HOST_CLK_13M:
		timeout_param = DATA_TIMEOUT_13M;
		break;
#if defined(CONFIG_SDIO_V2P0)	//hasn't 26M
	case SDIO_HOST_CLK_20M:
		timeout_param = DATA_TIMEOUT_20M;
		break;

	case SDIO_HOST_CLK_40M:
		timeout_param = DATA_TIMEOUT_40M;
		break;

	case SDIO_HOST_CLK_80M:
		timeout_param = DATA_TIMEOUT_80M;
		break;

	default:
		timeout_param = DATA_TIMEOUT_13M;
		break;
#else
	case SDIO_HOST_CLK_26M:
		default:
		timeout_param = DATA_TIMEOUT_26M;
		break;
#endif
	}
	return timeout_param;
}

/* CMD0 */
static bk_err_t sd_card_cmd_go_idle_state(void)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_GO_IDLE_STATE;
	cmd_cfg.argument = 0;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_NONE;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = false;	//no response, so no need to check slave response's crc

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD0 GO_IDLE_STATE err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD0(GO_IDLE_STATE) ok\r\n");

	return error_state;
}

/* CMD8 */
static bk_err_t sd_card_cmd_send_if_cond(void)
{
	bk_err_t error_state = 0;
	uint32_t rsp_arg = 0;
	uint8_t voltage_accpet, check_pattern = 0;
	sdio_host_cmd_cfg_t cmd_cfg = {0};
	uint32_t retry_cnt = SD_CARD_MAX_CMD_TRIAL_COUNT;

	/* Send CMD8 to verify SD card interface operation condition
	 * Argument:
	 *  - [31:12]: Reserved (shall be set to '0')
	 *  - [11:8]: Support Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
	 *  - [7:0]: Check Pattern (recommand 0xAA)
	 * CMD Response: R7
	 */
	cmd_cfg.cmd_index = SD_CMD_SEND_IF_COND;
	cmd_cfg.argument = SD_CHECK_PATTERN;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	while(retry_cnt)
	{
		retry_cnt--;

		bk_sdio_host_send_command(&cmd_cfg);
		error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
		if (error_state == BK_OK)
			break;
	}
	if((error_state != BK_OK))
	{
		SD_CARD_LOGW("CMD8 SEND_IF_COND err:-%x\r\n", -error_state);
		return error_state;
	}

	SD_CARD_LOGD("sdio host send CMD8(SEND_IF_COND) ok\r\n");

	/* Get Response R7 argument
	 * - [11:8]: Voltage accepted
	 * - [7:0]: Echo-back of check pattern
	 * Voltage accepted:
	 *  -0000b: Not Defined
	 *  -0001b: 2.7-3.6 V
	 */
	rsp_arg = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP0);
	check_pattern = rsp_arg & 0xff;
	voltage_accpet = (rsp_arg >> 8) & 0xf;

	if (check_pattern == 0xaa && voltage_accpet == 0x1) {
		SD_CARD_LOGI("sd card support voltage 2.7-3.6 V\r\n");
		return BK_OK;
	} else {
		SD_CARD_LOGW("sd card unsupport voltage\r\n");
		return BK_ERR_SDIO_HOST_INVALID_VOLT_RANGE;
	}

	return error_state;
}

/* CMD55 */
static bk_err_t sd_card_cmd_app_cmd(uint32_t argument)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_APP_CMD;
	cmd_cfg.argument = argument;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD55 APP_CMD err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD55(APP_CMD) ok\r\n");

	return error_state;
}

/* ACMD41
 * Send the command asking the accessed card to send its operating condition register (OCR)
 */
static bk_err_t sd_card_cmd_sd_send_op_cond(uint32_t ocr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_SD_SEND_OP_COND;
	cmd_cfg.argument = ocr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = false;	//response cmd-index is 63,not 41,and the crc is err value if checked.

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	// why acmd41 always return crc fail?
	// TODO
	if (error_state != BK_OK) {
		SD_CARD_LOGW("ACMD41 SD_SEND_OP_COND err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send ACMD41(SD_SEND_OP_COND) ok\r\n");

	return error_state;
}

/* CMD2 */
static bk_err_t sd_card_cmd_all_send_cid(void)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_ALL_SEND_CID;
	cmd_cfg.argument = 0;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_LONG;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = false;	//response cmd-index is 63,not 2,and the crc is err value if checked.

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD2 ALL_SEND_CID err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD2(ALL_SEND_CID) ok\r\n");

	return error_state;
}

/* CMD3 */
static bk_err_t sd_card_cmd_send_relative_addr(uint16_t *rca)
{
	bk_err_t error_state = BK_OK;
	uint32_t rsp_arg = 0;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_SEND_RELATIVE_ADDR;
	cmd_cfg.argument = 0;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD3 SEND_RELATIVE_ADDR err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD3(SEND_RELATIVE_ADDR) ok\r\n");

	/* R6
	 * BIT[0:15] card status bits
	 * BIT[16:31] new published RCA
	 */
	rsp_arg = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP0);
	*rca = (rsp_arg >> 16);

	return error_state;
}

/* CMD9 */
static bk_err_t sd_card_cmd_send_csd(uint32_t argument)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_SEND_CSD;
	cmd_cfg.argument = argument;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_LONG;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = false;	//response cmd-index is 63,not 9,and the crc is err value if checked.

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD9 SEND_CSD err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD9(SEND_CSD) ok\r\n");

	return error_state;
}

/* CMD7 */
static bk_err_t sd_card_cmd_select_deselect_card(uint32_t addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};
	uint32_t retry_cnt = SD_CARD_MAX_CMD_TRIAL_COUNT;

	cmd_cfg.cmd_index = SD_CMD_SELECT_DESELECT_CARD;
	cmd_cfg.argument = addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	while(retry_cnt)
	{
		retry_cnt--;

		bk_sdio_host_send_command(&cmd_cfg);
		error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
		if (error_state == BK_OK)
			break;
	}
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD7 SELECT_DESELECT_CARD err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD7(SELECT_DESELECT_CARD) ok\r\n");

	return error_state;
}

/* ACMD6 */
/* four_lines: == false means one line; == true: four lines(if host support four lines)
 * default sdcard uses 1 line after power on.
 */
static bk_err_t sd_card_cmd_set_bus_width(bool four_lines)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_APP_CMD;
	cmd_cfg.argument = (UINT32)(s_sd_card_obj.sd_card.relative_card_addr << 16);
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGD("CMD55 SET_BUS_WIDTH err:-%x\r\n", -error_state);
		return error_state;
	}

	cmd_cfg.cmd_index = SD_CMD_APP_CMD6_SET_BUS_WIDTH;
#if defined(CONFIG_SDCARD_BUSWIDTH_4LINE)
	if(four_lines)
		cmd_cfg.argument = 2;
	else
		cmd_cfg.argument = 0;
#else
	cmd_cfg.argument = 0;
#endif
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGD("ACMD6 SET_BUS_WIDTH err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send ACMD6(SET_BUS_WIDTH) = %d\r\n", four_lines);

	return error_state;
}

/* CMD16 */
static bk_err_t sd_card_cmd_set_block_len(uint32_t block_size)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_SET_BLOCKLEN;
	cmd_cfg.argument = block_size;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD16 SET_BLOCK_LEN err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD16(SET_BLOCK_LEN) ok, set_block_len:%d\r\n", block_size);

	return error_state;
}

/* CMD25 */
static bk_err_t sd_card_cmd_write_multiple_block(uint32_t write_addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};
	uint32_t retry_cnt = SD_CARD_MAX_CMD_TRIAL_COUNT;

	cmd_cfg.cmd_index = SD_CMD_WRITE_MULTIPLE_BLOCK;
	cmd_cfg.argument = write_addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;
	/* some sdcards maybe busy after CMD12, the first CMD25 maybe timeout */
	while(retry_cnt)
	{
		retry_cnt--;

		bk_sdio_host_send_command(&cmd_cfg);
		error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
		if (error_state == BK_OK)
		{
			break;
		}
	}
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD25 WRITE_MULTIPLE_BLOCK err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD25(WRITE_MULTIPLE_BLOCK) ok\r\n");

	return error_state;
}

#if defined(CONFIG_SDIO_V2P0)
//TODO:first version, not use single block
#else
/* CMD24 */
static bk_err_t sd_card_cmd_write_single_block(uint32_t write_addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_WRITE_BLOCK;
	cmd_cfg.argument = write_addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD24 WRITE_BLOCK err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD24(WRITE_BLOCK) ok\r\n");

	return error_state;
}
#endif

/* CMD18 */
static bk_err_t sd_card_cmd_read_multiple_block(uint32 addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};
	uint32_t retry_cnt = SD_CARD_MAX_CMD_TRIAL_COUNT;

	cmd_cfg.cmd_index = SD_CMD_READ_MULTIPLE_BLOCK;
	cmd_cfg.argument = addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	/* some sdcards maybe busy after CMD12(busy time over 2ms), the first CMD18 maybe timeout */
	while(retry_cnt)
	{
		retry_cnt--;
		bk_sdio_host_send_command(&cmd_cfg);
		error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
		if (error_state == BK_OK)
		{
			break;
		}
	}
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD18 READ_MULTIPLE_BLOCK err:-%x\r\n", -error_state);
			return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD18(READ_MULTIPLE_BLOCK) ok\r\n");

	return error_state;
}

#if defined(CONFIG_SDIO_V2P0)
//TODO:first version, not use single block
#else
/* CMD17 */
static bk_err_t sd_card_cmd_read_single_block(uint32 addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_READ_SINGLE_BLOCK;
	cmd_cfg.argument = addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD17 READ_SINGLE_BLOCK err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD17(READ_SINGLE_BLOCK) ok\r\n");

	return error_state;
}
#endif

#if defined(CONFIG_SDIO_V2P0)
/* CMD12 */
bk_err_t sd_card_cmd_stop_transmission(void)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};
	//uint32_t retry_cnt = 0;

	/*
	 * WARNING:
	 * When read multi-block data, SW read last block data out from sdio fifo,
	 * if enable clock for sdcard, sdio will continue receive invalid data
	 * which cause a receive data end interrupt.
	 * so before enable clock,reset the sdio fifo then hasn't clock output for sdcard
	 * and no invalid data coming.
	 */
	bk_sdio_host_reset_sd_state();

	//clock gate:clock always on
	bk_sdio_clk_gate_config(1);

	cmd_cfg.cmd_index = SD_CMD_STOP_TRANSMISSION;
	cmd_cfg.argument = 0;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	/*
	 * TODO:
	 * Don't do retry, if one of CMD12 is responsed by SDCARD,then the next other CMD12
	 * will not be responsed by SDCARD, it will cause every CMD12 retry timeout.
	 *
	 */
	//while(1)
	{
		//retry_cnt++;

		bk_sdio_host_send_command(&cmd_cfg);
		error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
		if (error_state != BK_OK)
		{
			SD_CARD_LOGE("CMD12 STOP_TRANSMISSION err:-%x\r\n", -error_state);
			//break;
		}
	}

	//module clock auto-gate on, tx fifo clock auto-gate disable
	bk_sdio_tx_fifo_clk_gate_config(0);
	bk_sdio_clk_gate_config(0);

	/*
	 * WARNING:
	 * When read multi-block data, SW read last block data finish from sdio fifo,
	 * if enable clock for sdcard, sdio will continue receive invalid data.
	 * after enable clock,before CMD12 responsed by SDCARD, sdio already read some
	 * data which maybe caused RX FIFO full, then clock gates cause SDIO
	 * hasn't clock. After this cycle, next cycle before read/write data will
	 * use ACMD6 to check SDCARD is whether busy failed.
	 */
	bk_sdio_host_reset_sd_state();

	//bk_sdio_clock_en(0);

	SD_CARD_LOGD("sdio host send CMD12(STOP_TRANSMISSION) ok\r\n");

	return error_state;
}
#else
/* CMD12 */
bk_err_t sd_card_cmd_stop_transmission(void)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_STOP_TRANSMISSION;
	cmd_cfg.argument = 0;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD12 STOP_TRANSMISSION err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD12(STOP_TRANSMISSION) ok\r\n");

	return error_state;
}
#endif

/* CMD32 */
static bk_err_t sd_card_cmd_set_erase_start_addr(uint32_t start_addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_ERASE_WR_BLK_START;
	cmd_cfg.argument = start_addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD32 ERASE_WR_BLK_START err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD32(ERASE_WR_BLK_START) ok\r\n");

	return error_state;
}

/* CMD33 */
static bk_err_t sd_card_cmd_set_erase_end_addr(uint32_t end_addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_ERASE_WR_BLK_END;
	cmd_cfg.argument = end_addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD33 ERASE_WR_BLK_END err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD33(ERASE_WR_BLK_END) ok\r\n");

	return error_state;
}

/* CMD38 */
static bk_err_t sd_card_cmd_erase(void)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_ERASE;
	cmd_cfg.argument = 0;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD38 ERASE err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD38(ERASE) ok\r\n");

	return error_state;
}

/* CMD13 */
static bk_err_t sd_card_cmd_send_status(uint32_t argument, uint32_t *card_status)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_SEND_STATUS;
	cmd_cfg.argument = argument;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();
	cmd_cfg.crc_check = true;

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD13 SEND_STATUS err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGD("sdio host send CMD13(SEND_STATUS) ok\r\n");

	*card_status = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP0);

	return error_state;
}

static bk_err_t sd_card_power_on(void)
{
	bk_err_t error_state;
	uint32_t count = 0, valid_volt = 0;
	uint32_t response = 0;

	/* CMD0: GO_IDLE_STATE */
	error_state = sd_card_cmd_go_idle_state();
	if (error_state != BK_OK) {
		return error_state;
	}
    rtos_delay_milliseconds(50);

	/* CMD8: SEND_IF_COND: Command available on V2.0 cards */
	error_state = sd_card_cmd_send_if_cond();
	if (error_state != BK_OK) {
		s_sd_card_obj.sd_card.card_version = SD_CARD_V1_X;
	} else {
		s_sd_card_obj.sd_card.card_version = SD_CARD_V2_X;
	}

	if (s_sd_card_obj.sd_card.card_version == SD_CARD_V2_X) {
		/* SEND CMD55 APP_CMD with RCA as 0 */
		error_state = sd_card_cmd_app_cmd(0);
		if (error_state != BK_OK) {
			SD_CARD_LOGW("unsupported feature\r\n");
			return BK_ERR_SDIO_HOST_UNSUPPORTED_FEATURE;
		}
	}

	while ((count < SD_CARD_MAX_VOLT_TRIAL_COUNT) && (valid_volt == 0)) {
		/* Send CMD55 APP_CMD with RCA as 0 */
		error_state = sd_card_cmd_app_cmd(0);
		if (error_state != BK_OK) {
			return error_state;
		}

		/* Send ACMD41 */
		error_state = sd_card_cmd_sd_send_op_cond(SD_DEFAULT_OCR);
		if (error_state != BK_OK) {
			SD_CARD_LOGW("unsupported feature\r\n");
			return BK_ERR_SDIO_HOST_UNSUPPORTED_FEATURE;
		}
		/* Get command response */
		response = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP0);

		/* Get operating voltage
		 * OCR register:
		 * BIT[31]: card power up status bit
		 */
		valid_volt = (((response >> 31) == 1) ? 1 : 0);

		count++;
		rtos_delay_milliseconds(2);
	}

	if (count >= SD_CARD_MAX_VOLT_TRIAL_COUNT) {
		SD_CARD_LOGW("send cmd55 and acmd41 retry time out\r\n");
		return BK_ERR_SDIO_HOST_INVALID_VOLT_RANGE;
	}

	/* OCR register BIT[30] */
	if ((response & SD_OCR_HIGH_CAPACITY) == SD_OCR_HIGH_CAPACITY) {
		SD_CARD_LOGI("card capacity SDHC_SDXC\r\n");
		s_sd_card_obj.sd_card.card_type = SD_CARD_TYPE_SDHC_SDXC;
	} else {
		s_sd_card_obj.sd_card.card_type = SD_CARD_TYPE_SDSC;
		SD_CARD_LOGI("card capacity SDSC\r\n");
	}

	return BK_OK;
}

static bk_err_t sd_card_init_card(void)
{
	bk_err_t error_state = BK_OK;
	uint16_t sd_rca = 1;

	/* Send CMD2 ALL_SEND_CID */
	error_state = sd_card_cmd_all_send_cid();
	if (error_state != BK_OK) {
		return error_state;
	} else {
		/* Get card identification number data */
		s_sd_card_obj.cid[0] = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP0);
		s_sd_card_obj.cid[1] = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP1);
		s_sd_card_obj.cid[2] = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP2);
		s_sd_card_obj.cid[3] = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP3);
	}

	rtos_delay_milliseconds(2);

	/* Send CMD3 SET_RELATIVE_ADDR with argument 0 */
	/* SD Card publishes its RCA */
	error_state = sd_card_cmd_send_relative_addr(&sd_rca);
	if (error_state != BK_OK) {
		return error_state;
	}
	s_sd_card_obj.sd_card.relative_card_addr = sd_rca;

	rtos_delay_milliseconds(2);

	/* Send CMD9 SEDN_CSD with argument as card's RCA */
	error_state = sd_card_cmd_send_csd((uint32_t)(s_sd_card_obj.sd_card.relative_card_addr << 16));
	if (error_state != BK_OK) {
		SD_CARD_LOGW("sd csd cmd err:%d\r\n", error_state);
		return error_state;
	} else {
		/* Get card specific data */
		s_sd_card_obj.csd.csd_3.v = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP0);
		s_sd_card_obj.csd.csd_2.v = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP1);
		s_sd_card_obj.csd.csd_1.v = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP2);
		s_sd_card_obj.csd.csd_0.v = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP3);
		SD_CARD_LOGD("csd[0]=0x%x, csd[1]=0x%x, csd[2]=0x%x, csd[3]=0x%x\r\n",
			s_sd_card_obj.csd.csd_0.v,s_sd_card_obj.csd.csd_1.v,s_sd_card_obj.csd.csd_2.v,s_sd_card_obj.csd.csd_3.v);
	}

	/* Get the card class */
	s_sd_card_obj.sd_card.class = ((bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP1)) >> 20);
	SD_CARD_LOGI("sd card class:0x%x\r\n", s_sd_card_obj.sd_card.class);

	/* TODO: Get CSD parameters */


	/* Send CMD7, Select the card */
	error_state = sd_card_cmd_select_deselect_card((uint32_t)(s_sd_card_obj.sd_card.relative_card_addr << 16));
	if (error_state != BK_OK) {
		return error_state;
	}
	rtos_delay_milliseconds(2);

	/* Set sdcard buswidth four lines or one line */
#if defined(CONFIG_SDCARD_BUSWIDTH_4LINE)
	error_state = sd_card_cmd_set_bus_width(true);
#else
	error_state = sd_card_cmd_set_bus_width(false);
#endif
	if (error_state != BK_OK) {
		return error_state;
	}

	/* Set block size for card */
	error_state = sd_card_cmd_set_block_len(SD_BLOCK_SIZE);
	if (error_state != BK_OK) {
		return error_state;
	}

	rtos_delay_milliseconds(2);
	/* improve sdio clock freq for sd card data transfer mode */
#if defined(CONFIG_SDIO_V2P0)
	s_sd_card_obj.clock_freq = CONFIG_SDCARD_DEFAULT_CLOCK_FREQ;
	bk_sdio_host_set_clock_freq(s_sd_card_obj.clock_freq);
	SD_CARD_LOGI("sdio clock freq:%d->%d\r\n", CONFIG_SDIO_HOST_DEFAULT_CLOCK_FREQ, s_sd_card_obj.clock_freq);
#else
	s_sd_card_obj.clock_freq = SDIO_HOST_CLK_13M;
	bk_sdio_host_set_clock_freq(SDIO_HOST_CLK_13M);
	SD_CARD_LOGI("sdio clock freq:%d->%d\r\n", CONFIG_SDIO_HOST_DEFAULT_CLOCK_FREQ, SDIO_HOST_CLK_13M);
#endif
	rtos_delay_milliseconds(2);

	/* All cards are initialized */
	return error_state;
}

bk_err_t bk_sd_card_set_clock(uint32_t clock)
{
	s_sd_card_obj.clock_freq = clock;
	return bk_sdio_host_set_clock_freq(s_sd_card_obj.clock_freq);
}

bk_err_t bk_sd_card_init(void)
{
	bk_err_t error_state = BK_OK;
	sdio_host_config_t sdio_cfg = {0};

#if defined(CONFIG_SDCARD_CHECK_INSERTION_EN)
	if(sd_card_get_insert_status()) {
		SD_CARD_LOGE("NO SDcard! Please insert the SDcard!\r\n");
		return BK_ERR_SDIO_HOST_NOT_INIT;
	}
#endif

#if defined(CONFIG_SDIO_V2P0)
	uint32_t int_level = 0;

	int_level = rtos_enter_critical();
#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.ops = 1;
	s_sdcard_sw_status.init = 1;
#endif
	//init once and no release, after inited mutex lock, use it to replace disable_irq
	if(s_mutex_sdcard == NULL) {
		error_state = rtos_init_mutex(&s_mutex_sdcard);
		if (error_state != BK_OK) {
			SD_CARD_LOGI("s_mutex_sdcard init failed\r\n");
			rtos_exit_critical(int_level);
			return error_state;
		}
	}
	rtos_exit_critical(int_level);

	rtos_lock_mutex(&s_mutex_sdcard);
#endif

	if (s_sd_card_is_init) {
		SD_CARD_LOGI("sd card has inited\r\n");
#if defined(CONFIG_SDIO_V2P0)
		rtos_unlock_mutex(&s_mutex_sdcard);
#endif
		return BK_OK;
	}

#if defined(CONFIG_SDIO_V2P0)
	sdio_cfg.clock_freq = SDIO_HOST_CLK_100K;
#else
	sdio_cfg.clock_freq = CONFIG_SDIO_HOST_DEFAULT_CLOCK_FREQ;
#endif
#if defined(CONFIG_SDIO_4LINES_EN)
	sdio_cfg.bus_width = SDIO_HOST_BUS_WIDTH_4LINE;
#else
	sdio_cfg.bus_width = SDIO_HOST_BUS_WIDTH_1LINE;
#endif
#if defined(CONFIG_SDIO_GDMA_EN)
	sdio_cfg.dma_tx_en = 1;
	sdio_cfg.dma_rx_en = 1;
#endif
	s_sd_card_obj.clock_freq = sdio_cfg.clock_freq;

#if defined(CONFIG_SDIO_V2P0)
	bk_sdio_clk_gate_config(1);
#endif

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.init = 2;
#endif
	/* Initialize SDIO peripheral interface with default configuration */
	BK_RETURN_ON_ERR(bk_sdio_host_init(&sdio_cfg));
	SD_CARD_LOGI("sdio host init ok, clock_freq:%d\r\n", sdio_cfg.clock_freq);
	rtos_delay_milliseconds(30);

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.init = 3;
#endif

	/* Identify card operating voltage */
	error_state = sd_card_power_on();
	if (error_state != BK_OK) {
		bk_sdio_host_deinit();
#if defined(CONFIG_SDIO_V2P0)
		rtos_unlock_mutex(&s_mutex_sdcard);
#endif
		return error_state;
	}

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.init = 4;
#endif

	rtos_delay_milliseconds(2);

	/* Card initialization */
	error_state = sd_card_init_card();
	if (error_state != BK_OK) {
		bk_sdio_host_deinit();
#if defined(CONFIG_SDIO_V2P0)
		rtos_unlock_mutex(&s_mutex_sdcard);
#endif
		return error_state;
	}

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.init = 5;
	s_sdcard_sw_status.deinit = 0;
#endif

	rtos_delay_milliseconds(2);

	s_sd_card_is_init = true;
#if defined(CONFIG_SDIO_V2P0)
	rtos_unlock_mutex(&s_mutex_sdcard);
#endif

	return error_state;
}

bk_err_t bk_sd_card_deinit(void)
{
	//bk_err_t error_state = BK_OK;

#if defined(CONFIG_SDIO_V2P0)
	if(s_mutex_sdcard) {
		rtos_lock_mutex(&s_mutex_sdcard);
	}
	else {
		SD_CARD_LOGI("lock no init\r\n");
		return BK_FAIL;
	}
#endif

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.ops = 2;
	s_sdcard_sw_status.deinit = 1;
#endif

	if (!s_sd_card_is_init) {
		SD_CARD_LOGI("hasn't init\r\n");
#if defined(CONFIG_SDIO_V2P0)
		rtos_unlock_mutex(&s_mutex_sdcard);
#endif
		return BK_FAIL;
	}

	//before deinit, sync history read/write data to sd-card
#if defined(CONFIG_SDIO_V2P0)
	sd_card_check_continious_rw(SDCARD_OPS_SYNC_RW, 0, 0);
#endif

	BK_RETURN_ON_ERR(bk_sdio_host_deinit());
	os_memset(&s_sd_card_obj, 0, sizeof(s_sd_card_obj));
#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.init = 0;
	s_sdcard_sw_status.deinit = 2;
#endif

	s_sd_card_is_init = false;

#if defined(CONFIG_SDIO_V2P0)
	rtos_unlock_mutex(&s_mutex_sdcard);
#endif

	return BK_OK;
}

bk_err_t bk_sd_card_erase(uint32_t start_block_addr, uint32_t end_block_addr)
{
	bk_err_t error_state = BK_OK;
	uint32_t start_addr = start_block_addr;
	uint32_t end_addr = end_block_addr;

	/* Check if the card command class supports erase command */
	if ((s_sd_card_obj.sd_card.class & SD_SUPPORT_ERASE_CMD) == 0) {
		SD_CARD_LOGI("the card command class not support erase command\r\n");
		//return BK_FAIL;
	}
	if (s_sd_card_obj.sd_card.card_type != SD_CARD_TYPE_SDHC_SDXC) {
		start_addr *= 512;
		end_addr *= 512;
	}

	/* Send CMD32 to set erase start addr */
	error_state = sd_card_cmd_set_erase_start_addr(start_addr);
	if (error_state != BK_OK) {
		return error_state;
	}
	rtos_delay_milliseconds(2);

	/* Send CMD33 to set erase end addr */
	error_state = sd_card_cmd_set_erase_end_addr(end_addr);
	if (error_state != BK_OK) {
		return error_state;
	}
	rtos_delay_milliseconds(2);

	/* Send CMD38 ERASE */
	error_state = sd_card_cmd_erase();
	if (error_state != BK_OK) {
		return error_state;
	}

	return BK_OK;
}

#if (defined(CONFIG_SDCARD_DEBUG_SUPPORT))
static void sdcard_dump_transfer_data(UINT8 *write_buff, uint32_t first_block, uint32_t cnt)
{
	uint32_t i = 0;

	SD_CARD_LOGD("[+]sdcard_dump_transfer_data:addr=0x%x,cnt=%d\r\n", write_buff, cnt);

#if 1	//the format the same with bk_mem_dump_ex:avoid 4 bytes un-align issue
	for (i = 0;  i < cnt * 512; i++) {
		if (((i+1) & 0xf) == 0) {
			BK_DUMP_OUT("\r\n");
			for(volatile int j = 0; j < 10; j++);
		}
		BK_DUMP_OUT("%02x ", write_buff[i]);

		if((i+1) % 512 == 0)
			rtos_delay_milliseconds(1);
	}
#else
	for(i = 0; i < cnt * 512; i+=16)
	{
		SD_CARD_LOGD("0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n",
			(uint32_t)(*(uint32_t *)&write_buff[i]),
			(uint32_t)(*(uint32_t *)&write_buff[i+4]),
			(uint32_t)(*(uint32_t *)&write_buff[i+8]),
			(uint32_t)(*(uint32_t *)&write_buff[i+12]));
	}
#endif

	SD_CARD_LOGD("[-]sdcard_dump_transfer_data:addr=0x%x,cnt=%d\r\n", first_block, cnt);
}
#endif

#if defined(CONFIG_SDIO_V2P0)
//TODO:use CMD13 to check sdcard status
bk_err_t bk_sdcard_wait_busy_to_idle(uint32_t max_time)
{
	uint32_t retry_cnt = 0;
	bk_err_t error_state = BK_OK;

	/*
	 * WORKAROUND:
	 * Some special sdcard maybe at busy status after CMD12(previous round write stop),
	 * so it can't response the next cmd,F.E:CMD18 which reads multi-block data.
	 * BK7256XX chip can't retry with CMD18, if it sends CMD18,BK7256 sdio
	 * will start to read data from data-lines to the sdio FIFO but the data is 0.
	 */
	while(1)
	{
		retry_cnt++;
#if defined(CONFIG_SDCARD_BUSWIDTH_4LINE)
		error_state = sd_card_cmd_set_bus_width(true);
#else
		error_state = sd_card_cmd_set_bus_width(false);
#endif
		if(error_state == BK_OK)
		{
			break;
		}
		else if (retry_cnt > (max_time/4)) 	//TODO:4 should be match with SDIO_CMD_WAIT_TIME
		{
			SD_CARD_LOGW("ACMD6 check SDCARD busy timeout%d\r\n", max_time);
			return error_state;
		}
	}

	return error_state;
}
#endif

#if defined(CONFIG_SDIO_V2P0)
/**
 * @brief - Some sd-cards stop command takes too much time(30ms or more), we'd better reduce stop command
 *          after read/write sd-card.
 *  -  Scheme:If APP requests read/write data from/to sd-card, driver doesn't send stop cmd to sd-card at once,
 *            until APP needs to sync data to sd-card,or next operation read/write isn't continue with previous one.
 *  - This function check the sd card read/write operation is whether continious with the previous operation.
 *    If current ops isn't continious with previous ops, it sends stop cmd to sd-card this time, and then do next operations.
 *
 *  - How to check whether the operation is continue with previous one:
 *    Bakeup last state and last ended addr.
 *    If previous read, current read address is continious, we can set it continious operation.
 *    If previous write, current write address is continious, we can set it continious operation.
 *
 * @return: Current reading/writing... state
 */
static sdcard_rw_state_t sd_card_check_continious_rw(sdcard_rw_ops_t ops, uint32_t addr, uint32_t blk_cnt)
{
	uint32_t int_level = 0;
	sdcard_rw_state_t current_state = SDCARD_RW_STATE_INITIALIZED;
	static sdcard_rw_state_t s_baked_state = SDCARD_RW_STATE_INITIALIZED;	//baked the state of "complete current operation"
	static uint32_t s_baked_addr = 0xffffffff;	//invalid last address

#if 0	//just debug
	static uint32_t s_continious_cnt = 0;
	static uint32_t s_total_cnt = 0;
#endif

	SD_CARD_LOGD("s_baked_state=%d,s_baked_addr=%d\r\n", s_baked_state, s_baked_addr);
	SD_CARD_LOGD("cur_ops=%d,cur_addr=%d,cnt=%d\r\n", ops, addr, blk_cnt);

	int_level = rtos_enter_critical();

#if 0	//just debug
	s_total_cnt++;
#endif

	switch(s_baked_state)
	{
		case SDCARD_RW_STATE_INITIALIZED:
		{
			//
			if(ops == SDCARD_OPS_READ)
			{
				current_state = SDCARD_RW_STATE_INITIALIZED;
				s_baked_state = SDCARD_RW_STATE_READING;
				s_baked_addr = addr + blk_cnt;
			}
			else if(ops == SDCARD_OPS_WRITE)
			{
				current_state = SDCARD_RW_STATE_INITIALIZED;
				s_baked_state = SDCARD_RW_STATE_WRITING;
				s_baked_addr = addr + blk_cnt;
			}
			else if(ops == SDCARD_OPS_SYNC_RW)
			{
				//do nothing
				SD_CARD_LOGD("no need sync\r\n");
			}

			break;
		}

		case SDCARD_RW_STATE_READING:
		{
			if((ops == SDCARD_OPS_READ) && (s_baked_addr == addr))
			{
				//continious read
				current_state = SDCARD_RW_STATE_READING;
				s_baked_state = SDCARD_RW_STATE_READING;
				s_baked_addr += blk_cnt;
#if 0	//just debug
				s_continious_cnt++;
#endif
			}
			else if(ops == SDCARD_OPS_SYNC_RW)
			{
				current_state = SDCARD_RW_STATE_ENDED;
				s_baked_state = SDCARD_RW_STATE_INITIALIZED;
				s_baked_addr = 0xffffffff;
			}
			else 	//end current read, switch to next ops
			{
				current_state = SDCARD_RW_STATE_ENDED;
				if(ops == SDCARD_OPS_READ)
					s_baked_state = SDCARD_RW_STATE_READING;
				else if(ops == SDCARD_OPS_WRITE)
					s_baked_state = SDCARD_RW_STATE_WRITING;
				
				s_baked_addr = addr + blk_cnt;
			}

			break;
		}

		case SDCARD_RW_STATE_WRITING:
		{
			if((ops == SDCARD_OPS_WRITE) && (s_baked_addr == addr))
			{
				//continious write
				current_state = SDCARD_RW_STATE_WRITING;
				s_baked_state = SDCARD_RW_STATE_WRITING;
				s_baked_addr += blk_cnt;
#if 0	//just debug
				s_continious_cnt++;
#endif
			}
			else if(ops == SDCARD_OPS_SYNC_RW)
			{
				current_state = SDCARD_RW_STATE_ENDED;
				s_baked_state = SDCARD_RW_STATE_INITIALIZED;
				s_baked_addr = 0xffffffff;
			}
			else 	//end current write, switch to next ops
			{
				current_state = SDCARD_RW_STATE_ENDED;
				if(ops == SDCARD_OPS_READ)
					s_baked_state = SDCARD_RW_STATE_READING;
				else if(ops == SDCARD_OPS_WRITE)
					s_baked_state = SDCARD_RW_STATE_WRITING;
				
				s_baked_addr = addr + blk_cnt;
			}

			break;
		}

		//middle state, no need to bake, it just returns to caller that current state.
		case SDCARD_RW_STATE_ENDED:
		{
			break;
		}

		default:
		{
			break;
		}
	}

	rtos_exit_critical(int_level);

	SD_CARD_LOGD("[-]current_state = %d, s_baked_state=%d,s_baked_addr=%d\r\n", current_state, s_baked_state, s_baked_addr);
#if 0	//just debug
	SD_CARD_LOGD("s_continious_cnt=%d,s_total_cnt=%d\r\n", s_continious_cnt, s_total_cnt);
#endif
	//move out, avoid disable IRQ too much time
	if(current_state == SDCARD_RW_STATE_ENDED)
		sd_card_cmd_stop_transmission();

	return current_state;
}

bk_err_t bk_sd_card_rw_sync(void)
{
	sd_card_check_continious_rw(SDCARD_OPS_SYNC_RW, 0, 0);
	return BK_OK;
}

bk_err_t bk_sd_card_write_blocks(const uint8_t *data, uint32_t block_addr, uint32_t block_num)
{
	//BK_RETURN_ON_NULL(data);

	bk_err_t error_state = BK_OK;
	uint32_t addr = block_addr;
	sdio_host_data_config_t data_config = {0};

	SD_CARD_LOGD("write[+]:tx data=0x%08x,block_addr=%08d,block_cnt=%08d\r\n", data, block_addr, block_num);

#if (defined(CONFIG_SDCARD_DEBUG_SUPPORT))
	sdcard_dump_transfer_data((uint8_t *)data, block_addr, block_num);
#endif

	if(s_mutex_sdcard) {
		rtos_lock_mutex(&s_mutex_sdcard);
	}
	else {
		SD_CARD_LOGI("sd card lock no init\r\n");
		return BK_FAIL;
	}

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.ops = 4;
	s_sdcard_sw_status.write = 1;
#endif

	if (!s_sd_card_is_init) {
		SD_CARD_LOGW("SDCARD driver not init\r\n");
		rtos_unlock_mutex(&s_mutex_sdcard);
		return BK_ERR_SDIO_HOST_NOT_INIT;
	}

	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SDIO, true);
	if(sd_card_check_continious_rw(SDCARD_OPS_WRITE, block_addr, block_num) != SDCARD_RW_STATE_WRITING)
	{
		error_state = bk_sdcard_wait_busy_to_idle(1000);
		if (error_state != BK_OK) {
			SD_CARD_LOGW("****sdcard is busy\r\n");
			rtos_unlock_mutex(&s_mutex_sdcard);
			return error_state;
		}

		if (s_sd_card_obj.sd_card.card_type != SD_CARD_TYPE_SDHC_SDXC) {
			addr *= 512;
		}

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
		s_sdcard_sw_status.write = 2;
#endif
		//before write data, reset it
		bk_sdio_host_reset_sd_state();

		//TODO:just use multi-block mode
		error_state = sd_card_cmd_write_multiple_block(addr);
		if (error_state != BK_OK) {
			rtos_unlock_mutex(&s_mutex_sdcard);
			return error_state;
		}

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
		s_sdcard_sw_status.write = 3;
#endif
	}

	/* config sdio data */
	data_config.data_timeout = sd_card_get_data_timeout_param();
	data_config.data_len = SD_BLOCK_SIZE * block_num;
	data_config.data_block_size = SD_BLOCK_SIZE;
	data_config.data_dir = SDIO_HOST_DATA_DIR_WR;
	bk_sdio_host_config_data(&data_config);

	error_state = bk_sdio_host_write_fifo(data, data_config.data_len);

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.write = 4;
#endif

	{
		//error_state = sd_card_cmd_stop_transmission();

		//TODO:disable clock for low power
		//sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SDIO, false);
	}

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.read = 0;
	s_sdcard_sw_status.write = 5;
#endif

	rtos_unlock_mutex(&s_mutex_sdcard);

	SD_CARD_LOGD("write[-]error_state=%d\r\n", error_state);

	return error_state;
}

bk_err_t bk_sd_card_read_blocks(uint8_t *data, uint32_t block_addr, uint32_t block_num)
{
	bk_err_t error_state = BK_OK;
	uint32_t addr = block_addr;
	sdio_host_data_config_t data_config = {0};

	if(s_mutex_sdcard) {
		rtos_lock_mutex(&s_mutex_sdcard);
	}
	else {
		SD_CARD_LOGI("sd card lock no init\r\n");
		return BK_FAIL;
	}

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.ops = 3;
	s_sdcard_sw_status.read = 1;
#endif

	if (!s_sd_card_is_init) {
		SD_CARD_LOGW("SDCARD driver not init\r\n");
		rtos_unlock_mutex(&s_mutex_sdcard);
		return BK_ERR_SDIO_HOST_NOT_INIT;
	}

	SD_CARD_LOGD("read[+]:rx data=0x%x,block_addr=0x%x,block_cnt=%d\r\n", data, block_addr, block_num);

	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SDIO, true);

	//check and do (send/no-send) multi-read CMD
	if(sd_card_check_continious_rw(SDCARD_OPS_READ, block_addr, block_num) != SDCARD_RW_STATE_READING)
	{
		extern void bk_sdio_host_discard_previous_receive_data_sema(void);

		error_state = bk_sdcard_wait_busy_to_idle(1000);
		if (error_state != BK_OK) {
			SD_CARD_LOGW("****sdcard is busy\r\n");
			rtos_unlock_mutex(&s_mutex_sdcard);
			return error_state;
		}

		//before read data, reset it
		bk_sdio_host_reset_sd_state();

		//before read data, discard previous read data sema if it has
		//(previous read feature cause SDIO hardware read one more block data)
		bk_sdio_host_discard_previous_receive_data_sema();

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
		s_sdcard_sw_status.read = 2;
#endif

		data_config.data_timeout = sd_card_get_data_timeout_param();
		data_config.data_len = SD_BLOCK_SIZE * block_num;
		data_config.data_block_size = SD_BLOCK_SIZE;
		data_config.data_dir = SDIO_HOST_DATA_DIR_RD;

		bk_sdio_host_config_data(&data_config);
		SD_CARD_LOGD("sdio host config data ok, data_len:%d\r\n", data_config.data_len);
		if (s_sd_card_obj.sd_card.card_type != SD_CARD_TYPE_SDHC_SDXC) {
			addr *= 512;
		}

		*((uint32_t volatile *)(0x448b0000 + 0x3 * 4 )) |= (0x200<<4);	//block_size

		//TODO:just use multi-block mode
		error_state = sd_card_cmd_read_multiple_block(addr);
		if (error_state != BK_OK) {
			rtos_unlock_mutex(&s_mutex_sdcard);
			return error_state;
		}

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
		s_sdcard_sw_status.read = 3;
#endif
	}
	else
		data_config.data_len = SD_BLOCK_SIZE * block_num;

	//read data from SDIO to buffer
	error_state = bk_sdio_host_read_blks_fifo(data, block_num);

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.read = 4;
#endif
	/* Send stop transmission command
	 * notice: need reset command_and_data state after recv data(REG0x9 BIT[20] for bk7256)
	 * otherwise wait_for_cmd_rsp failed
	 */
	{
		//error_state += sd_card_cmd_stop_transmission();
	}

#if defined(CONFIG_SDCARD_OPS_TRACE_EN)
	s_sdcard_sw_status.read = 5;
	s_sdcard_sw_status.write = 0;
#endif

	rtos_unlock_mutex(&s_mutex_sdcard);

#if (defined(CONFIG_SDCARD_DEBUG_SUPPORT))
	sdcard_dump_transfer_data(data, block_addr, block_num);
#endif

	SD_CARD_LOGD("read[-]\r\n");

	return error_state;
}

#else
bk_err_t bk_sd_card_rw_sync(void)
{
	SD_CARD_LOGD("TODO:no implement[-]\r\n");
	return BK_OK;
}

bk_err_t bk_sd_card_write_blocks(const uint8_t *data, uint32_t block_addr, uint32_t block_num)
{
	BK_RETURN_ON_NULL(data);

	bk_err_t error_state = BK_OK;
	uint32_t addr = block_addr;
	sdio_host_data_config_t data_config = {0};

	if (s_sd_card_obj.sd_card.card_type != SD_CARD_TYPE_SDHC_SDXC) {
		addr *= 512;
	}

	if (block_num > 1) {
		/* Send write multiple block command */
		error_state = sd_card_cmd_write_multiple_block(addr);
	} else {
		/* Send write single block command */
		error_state = sd_card_cmd_write_single_block(addr);
	}
	if (error_state != BK_OK) {
		return error_state;
	}

	/* config sdio data */
	data_config.data_timeout = sd_card_get_data_timeout_param();
	data_config.data_len = SD_BLOCK_SIZE * block_num;
	data_config.data_block_size = SD_BLOCK_SIZE;
	data_config.data_dir = SDIO_HOST_DATA_DIR_WR;
	bk_sdio_host_config_data(&data_config);

	bk_sdio_host_write_fifo(data, data_config.data_len);

	/* Send stop transmission command in case of mutiple block write */
	if (block_num > 1) {
		error_state = sd_card_cmd_stop_transmission();
	}

	return error_state;
}

bk_err_t bk_sd_card_read_blocks(uint8_t *data, uint32_t block_addr, uint32_t block_num)
{
	bk_err_t error_state = BK_OK;
	uint32_t addr = block_addr;
	uint32_t read_data = 0, index = 0;
	sdio_host_data_config_t data_config = {0};

	data_config.data_timeout = sd_card_get_data_timeout_param();
	data_config.data_len = SD_BLOCK_SIZE * block_num;
	data_config.data_block_size = SD_BLOCK_SIZE;
	data_config.data_dir = SDIO_HOST_DATA_DIR_RD;

	bk_sdio_host_config_data(&data_config);
	SD_CARD_LOGI("sdio host config data ok, data_len:%d\r\n", data_config.data_len);
	/* such delay is important, otherwise sdio recv_end_int will not triggered */
	rtos_delay_milliseconds(2);

	if (s_sd_card_obj.sd_card.card_type != SD_CARD_TYPE_SDHC_SDXC) {
		addr *= 512;
	}

	if (block_num > 1) {
		/* Send read multiple block command */
		error_state = sd_card_cmd_read_multiple_block(addr);
	} else {
		/* Send read single block command */
		error_state = sd_card_cmd_read_single_block(addr);
	}
	if (error_state != BK_OK) {
		return error_state;
	}
	rtos_delay_milliseconds(2);

	while (bk_sdio_host_wait_receive_data() == BK_OK) {
		do {
			/* Read data from SDIO Rx fifo */
			error_state = bk_sdio_host_read_fifo(&read_data);
			if(error_state == BK_OK)
			{
				data[index++] = read_data & 0xff;
				data[index++] = (read_data >> 8) & 0xff;
				data[index++] = (read_data >> 16) & 0xff;
				data[index++] = (read_data >> 24) & 0xff;
				//SD_CARD_LOGD("read_data:%x, index:%d\r\n", read_data, index);
			}
			else
			{
				SD_CARD_LOGW("read data fail from FIFO\r\n");
				break;
			}
		} while ((index % SD_BLOCK_SIZE) != 0);
		if (index >= data_config.data_len) {
			break;
		}
	}

	/* Send stop transmission command
	 * notice: need reset command_and_data state after recv data(REG0x9 BIT[20] for bk7271)
	 * otherwise wait_for_cmd_rsp failed
	 */
	if (block_num > 1) {
		error_state = sd_card_cmd_stop_transmission();
	}

	return error_state;
}

#endif

bk_err_t bk_sd_card_get_card_info(sd_card_info_t *card_info)
{
	*card_info = s_sd_card_obj.sd_card;
	return BK_OK;
}

/* size unit: sector counts,default sector size is 512 bytes */
uint32_t bk_sd_card_get_card_size(void)
{
	sd_card_csd_t *csd_p = (sd_card_csd_t *)&s_sd_card_obj.csd;
	uint32_t ver = 0, size = 0;

	//csd_structure-0:ver1.0; 1:ver2.0; 2:ver3.0
	ver = csd_p->csd_3.csd_structure+1;
	switch(ver)
	{
		case 1:	//ver1.0
		{
			uint32_t c_size, c_size_mul, block_nr, block_len, read_bl_len;
			c_size = (csd_p->csd_2.v1p0.c_size_high<<2) + csd_p->csd_1.v1p0.c_size_low;
			c_size_mul = 1 << (csd_p->csd_1.v1p0.c_size_mult + 2);
			block_nr = (c_size + 1) * c_size_mul;
			size = block_nr;

			read_bl_len = csd_p->csd_2.v1p0.read_bl_len;
			if(read_bl_len > 9)	//default:read_bl_len == 9;
			{
				block_len = 1 << read_bl_len;
				size = block_nr * block_len;
				SD_CARD_LOGW("card ver=%d.0,block_len=%d, block_nr= %d, total_size = %d.\r\n", ver, block_len, block_nr, size);
				size = size / (SD_BLOCK_SIZE);  // retutn sector for fatfs using, fatfs default SD_BLOCK_SIZE is 512
			}

			break;
		}

		case 2:	//ver2.0:(c_size:22bits == c_size_low << 16 + c_size_high), card_size == (c_size + 1) * 512 K bytes
		{
			size = ((csd_p->csd_2.v2p0.c_size_high<<16) + (csd_p->csd_1.v2p0_v3p0.c_size_low) + 1) << 10;
			break;
		}

		case 3:	//3.0:(c_size:28bits == c_size_low << 16 + c_size_high), card_size == (c_size + 1) * 512 K bytes
		{
			size = ((csd_p->csd_2.v3p0.c_size_high<<16) + (csd_p->csd_1.v2p0_v3p0.c_size_low) + 1) << 10;
			break;
		}

		default:
			SD_CARD_LOGE("card ver=%d.0\r\n", ver);
			break;
	}

	SD_CARD_LOGI("card ver=%d.0,size:0x%08x sector(sector=512bytes)\r\n", ver, (uint32_t)size);
	return size;
}

sd_card_state_t bk_sd_card_get_card_state(void)
{
	sd_card_state_t card_state = SD_CARD_TRANSFER;
	bk_err_t error_state = BK_OK;
	uint32_t resp1 = 0;

	/* Send status command(CMD13) */
	error_state = sd_card_cmd_send_status((uint32_t)(s_sd_card_obj.sd_card.relative_card_addr << 16), &resp1);
	if (error_state != BK_OK) {

	}
	/* The response format R1 contains a 32-bit field named card status.
	 * BIT[12:9] current_state
	 */
	card_state = (sd_card_state_t)((resp1 >> 0x9) & 0x0f);

	return card_state;
}

