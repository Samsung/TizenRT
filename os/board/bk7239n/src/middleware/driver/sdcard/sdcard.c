#include <common/bk_include.h>
#include "sys_rtos.h"
#include <common/bk_kernel_err.h>

#if defined(CONFIG_SDCARD)
#include "sdio_driver.h"
#include <os/os.h>
#include "sys_rtos.h"
#include "sdcard.h"
#include "sdcard_pub.h"
#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include <os/mem.h>
#include "bk_arm_arch.h"
#include "bk_gpio.h"
#include "bk_misc.h"
#include <driver/gpio.h>

#if defined(CONFIG_SOC_BK7256XX)
#include "gpio_driver.h"
#endif

/* Standard sd  commands (  )           type  argument     response */
#define GO_IDLE_STATE             0   /* bc                          */
#define ALL_SEND_CID              2
#define SEND_RELATIVE_ADDR        3   /* ac   [31:16] RCA        R6  */
#define IO_SEND_OP_COND  		  5   /* ac                      R4  */
#define SWITCH_FUNC               6
#define SELECT_CARD               7   /* ac   [31:16] RCA        R7  */
#define SEND_IF_COND              8   /* adtc                    R1  */
#define SEND_CSD                  9
#define SEND_STATUS               13
#define READ_SINGLE_BLOCK         17
#define WRITE_BLOCK               24
#define WRITE_MULTI_BLOCK         25
#define SD_APP_OP_COND            41
#define IO_RW_DIRECT              52  /* ac   [31:0] See below   R5  */
#define IO_RW_EXTENDED            53  /* adtc [31:0] See below   R5  */
#define APP_CMD                   55

#define R5_COM_CRC_ERROR	      (1 << 15)	/* er, b */
#define R5_ILLEGAL_COMMAND	      (1 << 14)	/* er, b */
#define R5_ERROR		          (1 << 11)	/* erx, c */
#define R5_FUNCTION_NUMBER	      (1 << 9)	/* er, c */
#define R5_OUT_OF_RANGE		      (1 << 8)	/* er, c */
#define R5_STATUS(x)		      (x & 0xCB00)
#define R5_IO_CURRENT_STATE(x)	  ((x & 0x3000) >> 12) /* s, b */

/*STM32 register bit define*/
#define SDIO_ICR_MASK             0x5FF
#define SDIO_STATIC_FLAGS         ((UINT32)0x000005FF)
#define SDIO_FIFO_ADDRESS         ((UINT32)0x40018080)

#define OCR_MSK_BUSY             0x80000000 // Busy flag
#define OCR_MSK_HC               0x40000000 // High Capacity flag
#define OCR_MSK_VOLTAGE_ALL      0x00FF8000 // All Voltage flag

#define SD_DEFAULT_OCR           (OCR_MSK_VOLTAGE_ALL|OCR_MSK_HC)

//#define SD_CLK_PIN                              34
#define REG_A2_CONFIG                        ((0x0802800) + 50*4)

typedef enum {
	SD_CARD_IDLE                 = ((UINT32)0),
	SD_CARD_READY                = ((UINT32)1),
	SD_CARD_IDENTIFICATION       = ((UINT32)2),
	SD_CARD_STANDBY              = ((UINT32)3),
	SD_CARD_TRANSFER             = ((UINT32)4),
	SD_CARD_SENDING              = ((UINT32)5),
	SD_CARD_RECEIVING            = ((UINT32)6),
	SD_CARD_PROGRAMMING          = ((UINT32)7),
	SD_CARD_DISCONNECTED         = ((UINT32)8),
	SD_CARD_ERROR                = ((UINT32)0xff)
} SDCardState;

typedef struct sdio_command {
	UINT32	index;
	UINT32  arg;
	UINT32	flags;		    /* expected response type */
	UINT32  timeout;
	bool crc_check;
	UINT32	resp[4];
	void    *data;		    /* data segment associated with cmd */
	SDIO_Error	err;		/* command error */
} SDIO_CMD_S, *SDIO_CMD_PTR;

static SDCARD_S sdcard;
static const DD_OPERATIONS sdcard_op = {
	sdcard_open,
	sdcard_close,
	sdcard_read,
	sdcard_write,
	sdcard_ctrl
};


//#define SD_DEBOUNCE_COUNT 			    10

//static uint8 sd_online = SD_CARD_OFFLINE;
//static uint32 sd_clk_pin = SD_CLK_PIN;
//static uint32 sd_cd_pin = SD_DETECT_DEFAULT_GPIO;

static uint16 NoneedInitflag = 0;
//uint8 SD_det_gpio_flag = 1;
//static uint16 Sd_MMC_flag = 0;
//static uint16 cnt_online = 0;
//static beken_timer_t sd_cd_timer = {0};



static UINT8 no_need_send_cmd12_flag = 1;
static UINT8 SDIO_WR_flag = SDIO_RD_DATA;
static UINT32 last_WR_addr = 0;
static sdcard_ps_callback_t s_sdcard_ps_suspend_cb = NULL;
static sdcard_ps_callback_t s_sdcard_ps_resume_cb = NULL;

#define SDIO_RD_DATA             0
#define SDIO_WR_DATA             1

/*
 * Func: check and wait the SD-CARD inserted
 *
 */
static bool sdcard_check_inserted(void)
{
	UINT32 sd_data0;
	gpio_config_t gpio_config = {0};
	volatile uint32_t i = 0;
	bool is_inserted = true;
#if defined(CONFIG_SOC_BK7256XX)	//Temp code.
	sd_data0 = 6;	//GPIO_6;
#else
#if (defined(CONFIG_SD1_HOST_INTF))
	sd_data0 = 36;
#else
	sd_data0 = 17;
#endif
#endif

#if defined(CONFIG_SOC_BK7256XX)	//Temp code.
	gpio_dev_unmap(sd_data0);
#endif
	gpio_config.io_mode = GPIO_INPUT_ENABLE;
	gpio_config.pull_mode = GPIO_PULL_UP_EN;
	gpio_config.func_mode = GPIO_SECOND_FUNC_DISABLE;
	bk_gpio_set_config(sd_data0, &gpio_config);
	delay_us(125);	//confirm the gpio switch to input mode finish.
	while (!bk_gpio_get_input(sd_data0))
	{
		i++;
		if(i > 80000)	//240M CPU clock, 60M * 4wire flash:i++ is 125ns,total is about ~~10ms
		{
			SDCARD_LOGI("sdcard isn't insert\r\n");
			SDCARD_LOGI("WARNIG:HW will change detect pin, do not forget to modify code\r\n");
			is_inserted = false;
			break;
		}
	}

#if defined(CONFIG_SOC_BK7256XX)	//Temp code.
	gpio_dev_map(sd_data0, GPIO_DEV_SDIO_HOST_DATA0);
	bk_gpio_pull_up(sd_data0);
	bk_gpio_set_capacity(sd_data0, 3);
#else
	gpio_config.io_mode = GPIO_IO_DISABLE;
	gpio_config.pull_mode = GPIO_PULL_UP_EN;
	gpio_config.func_mode = GPIO_SECOND_FUNC_ENABLE;
	bk_gpio_set_config(sd_data0, &gpio_config);
#endif

//WARNING:temp workaround for HW card insert detect PIN is error.
//HW PIN6 can't detect voltage switch if card plug-in/out.
#if defined(CONFIG_SOC_BK7256XX)
	is_inserted = true;
#endif

	return is_inserted;
}

/******************************************************************************/
/***************************** public function ********************************/
/******************************************************************************/
#if (defined(CONFIG_SOC_BK7256XX))	//temp code, will be switch to sdcard_driver.c
static void sdcard_clock_set(uint8 clk_index)
{
	sdio_set_clock(clk_index);
	sdcard.clk_cfg = clk_index;
}
#else
static void sdcard_clock_set(uint8 clk_index)
{
	if (clk_index <= CLK_LOWEST)
	{
		sdio_set_clock(clk_index);
		sdcard.clk_cfg = clk_index;
	}
}
#endif

static void sdio_hw_init(void)
{
#if (defined(CONFIG_SOC_BK7271))
	UINT32 param;

	param = BLK_BIT_MIC_QSPI_RAM_OR_FLASH;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_ENABLE, &param);

	param = PSRAM_VDD_3_3V;
	//sddev_control(DD_DEV_TYPE_SCTRL, CMD_QSPI_VDDRAM_VOLTAGE, &param);
	sys_drv_set_qspi_vddram_voltage(param);
#endif

#if defined(CONFIG_SDCARD)
	if(sdcard_check_inserted() == false)
		return;
#endif

	/* config sdcard gpio */
	sdio_gpio_config();

	/* reset sdcard moudle register */
	sdio_register_reset();

	/* set sdcard low clk */
	sdcard_clock_set(CLK_LOWEST);

	/* set sdcard  clk enable*/
	sdio_clk_config(1);

	SDCARD_LOGI("%s exit\r\n", __func__);
}

static void sdio_send_cmd(SDIO_CMD_PTR sdio_cmd_ptr)
{
	sdio_sendcmd_function(sdio_cmd_ptr->index,
						  sdio_cmd_ptr->flags,
						  sdio_cmd_ptr->timeout,
						  (void *)sdio_cmd_ptr->arg);
}

static void sdio_hw_uninit(void)
{
	sdio_clk_config(0);
}

static void sdio_sw_init(void)
{
	os_memset((void *)&sdcard, 0, sizeof(SDCARD_S));
	sdcard.clk_cfg = CLK_LOWEST;
}

uint32 get_timeout_param(uint8 cmd_or_data)
{
	uint32 timeout_param;
	switch (sdcard.clk_cfg) {
#if defined(CONFIG_SOC_BK7256XX)	//Temp code, clock module should re-arch.
	case CLK_100K:
		if (cmd_or_data)
			timeout_param = CMD_TIMEOUT_100K;	//CMD_TIMEOUT_200K;	//really is 100K
		else
			timeout_param = DATA_TIMEOUT_100K;
		break;
#else
	case CLK_200K:
		if (cmd_or_data)
			timeout_param = CMD_TIMEOUT_200K;
		else
			timeout_param = DATA_TIMEOUT_200K;
		break;
#endif

	case CLK_6_5M:
		if (cmd_or_data)
			timeout_param = CMD_TIMEOUT_6_5_M;
		else
			timeout_param = DATA_TIMEOUT_6_5_M;
		break;

	case CLK_13M:
		if (cmd_or_data)
			timeout_param = CMD_TIMEOUT_13M;
		else
			timeout_param = DATA_TIMEOUT_13M;
		break;

#if defined(CONFIG_SOC_BK7256XX)
	case CLK_20M:
		if (cmd_or_data)
			timeout_param = CMD_TIMEOUT_20M;
		else
			timeout_param = DATA_TIMEOUT_20M;
		break;
		
	//hasn't 26M
	case CLK_40M:
		if (cmd_or_data)
			timeout_param = CMD_TIMEOUT_40M;
		else
			timeout_param = DATA_TIMEOUT_40M;
		break;

	case CLK_80M:
	default:
		if (cmd_or_data)
			timeout_param = CMD_TIMEOUT_80M;
		else
			timeout_param = DATA_TIMEOUT_80M;
		break;
#else
	case CLK_26M:
	default:
		if (cmd_or_data)
			timeout_param = CMD_TIMEOUT_26M;
		else
			timeout_param = DATA_TIMEOUT_26M;
		break;
#endif

	}
	return timeout_param;
}

/******************************************************************************/
/***************************** sdcard function ********************************/
/******************************************************************************/
/* GO_IDLE_STATE */
static SDIO_Error sdcard_cmd0_process(void)
{
	SDIO_CMD_S cmd;

	cmd.index = GO_IDLE_STATE;
	cmd.arg = 0;
	cmd.flags = SD_CMD_NORESP;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = false;	//no response, so no need to check slave response's crc
	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);
	return cmd.err;
}

static SDIO_Error sdcard_cmd1_process(void)
{
	SDIO_CMD_S cmd;
	uint32 response, reg;

	cmd.index = 1;
	cmd.arg = 0x40ff8000;
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;	//multi-media card will response, sdcard no response
cmd1_loop:
	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);
	if (cmd.err == SD_OK) {
		sdio_get_cmdresponse_argument(0, &response);
		if (!(response & OCR_MSK_VOLTAGE_ALL))
			cmd.err = SD_ERR_CMD41_CNT;
		if (!(response & OCR_MSK_BUSY))
			goto cmd1_loop;
		if (response & OCR_MSK_HC)
			sdcard.Addr_shift_bit = 0;
		else
			sdcard.Addr_shift_bit = 9;
	}
	reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
#if defined(CONFIG_SOC_BK7256XX)
	reg |= SDCARD_FIFO_SD_STA_RST;
#else
	reg |= 20;
#endif
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);

	return cmd.err;
}

static SDIO_Error sdcard_mmc_cmd8_process(void)
{
	int i;
	SDIO_CMD_S cmd;
	uint32 tmp;
	uint8 *tmpptr = (uint8 *)os_malloc(512);
	if (tmpptr == NULL)
		return 1;
	os_memset(tmpptr, 0, 512);

	cmd.index = SEND_IF_COND;
	cmd.arg = 0;
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;
	sdio_send_cmd(&cmd);

	if (cmd.err != SD_OK)
		goto freebuf;
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, (1 << 20));// reset first
#if defined(CONFIG_SOC_BK7256XX)
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, SDIO_REG0XD_CLK_REC_SEL | SDIO_REG0XD_SD_RD_WAIT_SEL | 0x3ffff);// set fifo later
#else
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, 0x3ffff);// set fifo later
#endif
	driver_sdcard_recv_data_start(get_timeout_param(0));

	tmp = 0;
	cmd.err = wait_Receive_Data();
	if (cmd.err == SD_OK) {
		for (i = 0; i < 128; i++) {
			while (!(REG_READ(REG_SDCARD_FIFO_THRESHOLD) & (0x1 << 18))) {
				tmp++;
				if (tmp > 0x20)
					break;
			}

			*((uint32 *)tmpptr + i) = REG_READ(REG_SDCARD_RD_DATA_ADDR);
		}
		sdcard.total_block = tmpptr[212] | (tmpptr[213] << 8) | (tmpptr[214] << 16) | (tmpptr[215] << 24);
	}

freebuf:
	os_free(tmpptr);
	return cmd.err;
}


static SDIO_Error sdcard_cmd8_process(void)
{
	SDIO_CMD_S cmd;
	UINT8 voltage_accpet, check_pattern;

	cmd.index = SEND_IF_COND;
	cmd.arg = 0x1AA;
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;

	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);

	if (cmd.err == SD_CMD_RSP_TIMEOUT) {
		SDCARD_WARN("cmd8 noresp, voltage mismatch or Ver1.X SD or not SD\r\n");
		return SD_CMD_RSP_TIMEOUT;
	} else if (cmd.err == SD_CMD_CRC_FAIL) {
		SDCARD_WARN("cmd8 cmdcrc err\r\n");
		return SD_CMD_CRC_FAIL;
	}

	SDCARD_PRT("found a Ver2.00 or later SDCard\r\n");

	// check Valid Response,
	// R7-[11:8]:voltage accepted, [7:0] echo-back of check pattern
	sdio_get_cmdresponse_argument(0, &cmd.resp[0]);

	check_pattern = cmd.resp[0] & 0xff;
	voltage_accpet = cmd.resp[0] >> 8 & 0xf;

	if (voltage_accpet == 0x1 && check_pattern == 0xaa) {
		SDCARD_PRT("support 2.7~3.6V\r\n");
		return SD_OK;
	} else {
		SDCARD_WARN("unsupport voltage\r\n");
		return SD_INVALID_VOLTRANGE;
	}
	return SD_OK;
}

/*Send host capacity support information(HCS) and  asks
  the card to send its OCR in the response on CMD line*/
static SDIO_Error sdcard_acmd41_process(UINT32 ocr)
{
	SDIO_CMD_S cmd;


	cmd.index = APP_CMD;
	cmd.arg = 0;
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;

	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);
	if (cmd.err != SD_OK) {
		SDCARD_WARN("send cmd55 err:%d\r\n", cmd.err);
		return cmd.err;
	}


	cmd.index = SD_APP_OP_COND;
	cmd.arg = ocr;
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = false;	//response cmd-index is 63,not 41, and the crc is err value if checked.
	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);
	// why cmd41 always return crc fail?
	if (cmd.err != SD_OK && cmd.err != SD_CMD_CRC_FAIL) {
		SDCARD_WARN("send cmd41 err:%d\r\n", cmd.err);
		return cmd.err;
	}

	return SD_OK;
}

/*ask the CID number on the CMD line*/
// Manufacturer ID	        MID	    8	[127:120]
// OEM/Application          ID	OID	16	[119:104]
// Product name	            PNM	    40	[103:64]
// Product revision	        PRV	    8	[63:56]
// Product serial number	PSN	    32	[55:24]
// reserved	                --	    4	[23:20]
// Manufacturing date	    MDT	    12	[19:8]
static SDIO_Error sdcard_cmd2_process(void)
{
	SDIO_CMD_S cmd;

	cmd.index = ALL_SEND_CID;
	cmd.arg = 0;
	cmd.flags = SD_CMD_LONG;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = false;	//response cmd-index is 63,not 2,and the crc is err value if checked.	
	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);

	// dismiss the CID info

	return cmd.err;
}

static SDIO_Error sdcard_mmc_cmd3_process(void)
{
	SDIO_CMD_S cmd;

	cmd.index = SEND_RELATIVE_ADDR;
	sdcard.card_rca = 1;
	cmd.arg = (sdcard.card_rca << 16);
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;

	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);

	if (cmd.err == SD_CMD_RSP_TIMEOUT) {
		SDCARD_WARN("mmc cmd3 noresp \r\n");
		return SD_CMD_RSP_TIMEOUT;
	} else if (cmd.err == SD_CMD_CRC_FAIL) {
		SDCARD_WARN("mmc cmd3 cmdcrc err\r\n");
		return SD_CMD_CRC_FAIL;
	}

	SDCARD_PRT("mmc cmd3 is ok, card rca:0x%x\r\n", sdcard.card_rca);
	return SD_OK;
}

/*ask the card to publish a new RCA*/
static SDIO_Error sdcard_cmd3_process(void)
{
	SDIO_CMD_S cmd;

	cmd.index = SEND_RELATIVE_ADDR;
	cmd.arg = 0;
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;

	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);

	if (cmd.err == SD_CMD_RSP_TIMEOUT) {
		SDCARD_WARN("cmd3 noresp \r\n");
		return SD_CMD_RSP_TIMEOUT;
	} else if (cmd.err == SD_CMD_CRC_FAIL) {
		SDCARD_WARN("cmd3 cmdcrc err\r\n");
		return SD_CMD_CRC_FAIL;
	}

	sdio_get_cmdresponse_argument(0, &cmd.resp[0]);
	sdcard.card_rca = (UINT16)(cmd.resp[0] >> 16);
	SDCARD_PRT("cmd3 is ok, card rca:0x%x\r\n", sdcard.card_rca);
	return SD_OK;
}

#define SD_CARD 0
#define MMC_CARD 1
/*get CSD Register content*/
static SDIO_Error sdcard_cmd9_process(uint8 card_type)
{
	SDIO_CMD_S cmd;
	int mult, csize;

	cmd.index = SEND_CSD;
	cmd.arg = (UINT32)(sdcard.card_rca << 16);
	cmd.flags = SD_CMD_LONG;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = false;	//response cmd-index is 63,not 9,and the crc is err value if checked.	

	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);
	if (cmd.err != SD_OK)
		return cmd.err;

	sdio_get_cmdresponse_argument(0, &cmd.resp[0]);
	sdio_get_cmdresponse_argument(1, &cmd.resp[1]);
	sdio_get_cmdresponse_argument(2, &cmd.resp[2]);

	sdcard.block_size = 1 << ((cmd.resp[1] >> 16) & 0xf);
	//SDCARD_LOGI("arg:%x %x %x %x size:%x\r\n", cmd.resp[0],
	//          cmd.resp[1], cmd.resp[2], cmd.resp[3], sdcard.block_size);

	if (card_type == SD_CARD) {

		if (((cmd.resp[0] >> 30) & 0x3) == 0) {
			csize = (((cmd.resp[1] & 0x3FF) << 2) | ((cmd.resp[2] >> 30) & 0x3));
			mult  = (cmd.resp[2] >> 15) & 0x7;

			sdcard.total_block = (csize + 1) * (1 << (mult + 2));
			sdcard.total_block *= (sdcard.block_size >> 9);
		} else {
			csize = (((cmd.resp[1] & 0x3F) << 16) | ((cmd.resp[2] >> 16) & 0xFFFF));
			sdcard.total_block = (csize + 1) * 1024;
		}

		SDCARD_LOGI("size:%x total_block:%x\r\n", sdcard.block_size, sdcard.total_block);
	} else {
		if (sdcard.Addr_shift_bit != 0) {
			csize = (((cmd.resp[1] & 0x3FF) << 2)
					 | ((cmd.resp[2] >> 30) & 0x3));
			mult = (cmd.resp[2] >> 15) & 0x7;

			sdcard.total_block = (csize + 1) * (1 << (mult + 2));
			sdcard.total_block *= (sdcard.block_size >> 9);
		} else
			sdcard.total_block = 0;
	}

	sdcard.block_size = SD_DEFAULT_BLOCK_SIZE;
	SDCARD_PRT("Bsize:%x;Total_block:%x\r\n", sdcard.block_size, sdcard.total_block);
	//BK_ASSERT(sdcard.block_size == SD_DEFAULT_BLOCK_SIZE);
	if(sdcard.block_size != SD_DEFAULT_BLOCK_SIZE)
	{
		SDCARD_PRT("Err:block size\r\n");
		return SD_ERROR;
	}

	return SD_OK;
}


/*select/deselect card*/
static SDIO_Error sdcard_cmd7_process(void)
{
	SDIO_CMD_S cmd;

	cmd.index = SELECT_CARD;
	cmd.arg = (UINT32)(sdcard.card_rca << 16);
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;

	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);
	return cmd.err;
}

/*set bus width*/
static SDIO_Error sdcard_acmd6_process(void)
{
	SDIO_CMD_S cmd;
	cmd.index = APP_CMD;
	cmd.arg = (UINT32)(sdcard.card_rca << 16);
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;
	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);
	if (cmd.err != SD_OK)
		return cmd.err;

	cmd.index = SWITCH_FUNC;
#ifdef CONFIG_SDCARD_BUSWIDTH_4LINE
	cmd.arg = 2;
#else
	cmd.arg = 0;
#endif
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout =  get_timeout_param(1);
	cmd.crc_check = true;

	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);

	return cmd.err;
}

static SDIO_Error sdcard_cmd18_process(uint32 addr)
{
	SDIO_CMD_S cmd;

	cmd.index = 18;
	cmd.arg = (UINT32)(addr << sdcard.Addr_shift_bit);
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;
	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);
	return cmd.err;
}
static SDIO_Error sdcard_cmd12_process(uint32 addr)
{
	SDIO_CMD_S cmd;

#if defined(CONFIG_SOC_BK7256XX)
	//1:clock always on, 0:auto gate
	//after one block transfer finish, the clock is auto gate, and next cmd can't send out
	{
		sdio_clk_gate_config(1);
	}
#endif

	cmd.index = 12;
	cmd.arg = addr;
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;
	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);

#if defined(CONFIG_SOC_BK7256XX)
	{
		uint32_t reg = REG_READ(REG_SDCARD_CMD_RSP_INT_MASK);
		reg &= ~(0x1 << SDIO_REG0XA_TX_FIFO_NEED_WRITE_MASK_CG_POS);
		REG_WRITE(REG_SDCARD_CMD_RSP_INT_MASK, reg);
	}

	//1:clock always on, 0:auto gate
	//after cmd12 send out,restore the clock to auto gate status
	{
		sdio_clk_gate_config(0);
	}
#endif

	return cmd.err;
}

static SDIO_Error sdcard_send_read_stop(void)
{
	//send stop command
	UINT32 reg;
	int Ret = 0;

	reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
	reg &= ~(0xFFFF | (1 << 16) | (1 << 20));
	reg |= (0x0101 | ((1 << 16) | (1 << 20)));
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);
	Ret = sdcard_cmd12_process(0);

	//close clock
	sdio_clk_config(0);
	return Ret;
}

__maybe_unused static SDIO_Error sdcard_cmd17_process(uint32 addr);
static SDIO_Error sdcard_cmd17_process(uint32 addr)
{
	SDIO_CMD_S cmd;

	cmd.index = 17;
	cmd.arg = addr;
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;
	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);
	return cmd.err;
}

SDIO_Error sdcard_initialize(void)
{
	SDIO_Error err = SD_OK;
	sdio_sw_init();
	rtos_delay_milliseconds(20);
	sdio_hw_init();
	rtos_delay_milliseconds(30);
	SDIO_WR_flag = SDIO_RD_DATA;
	no_need_send_cmd12_flag = 1;
	last_WR_addr = 0;
	// rest card
	err = sdcard_cmd0_process();
	if (err != SD_OK) {
		SDCARD_FATAL("send cmd0 err\r\n");
		goto err_return;
	}
	rtos_delay_milliseconds(5);

	rtos_delay_milliseconds(50);
	err = sdcard_cmd1_process();
	SDCARD_LOGI("cmd 1:%x \r\n", err);
	if (err == SD_OK)
		goto MMC_init;

	rtos_delay_milliseconds(5);


	// check support voltage
	err = sdcard_cmd8_process();
	if (err != SD_OK && err != SD_CMD_RSP_TIMEOUT) {
		SDCARD_FATAL("send cmd8 err\r\n");
		goto err_return;
	}

	if (err == SD_OK) {
		int retry_time = SD_MAX_VOLT_TRIAL;
		UINT32 resp0;
		while (retry_time) {
			err = sdcard_acmd41_process(SD_DEFAULT_OCR);
			if (err != SD_OK) {
				SDCARD_FATAL("send cmd55&cmd41 err:%d, quite loop\r\n", err);
				goto err_return;
			}
			sdio_get_cmdresponse_argument(0, &resp0);
			if (resp0 & OCR_MSK_BUSY) {
				if (resp0 & OCR_MSK_HC)
					sdcard.Addr_shift_bit = 0;
				else
					sdcard.Addr_shift_bit = 9;
				break;
			}

			rtos_delay_milliseconds(2);
			retry_time--;
		}
		if (!retry_time) {
			SDCARD_FATAL("send cmd55&cmd41 retry time out\r\n");
			return SD_INVALID_VOLTRANGE;
		}

		SDCARD_PRT("send cmd55&cmd41 complete, card is ready, retry_time=%d\r\n", (SD_MAX_VOLT_TRIAL-retry_time));

		if (resp0 & OCR_MSK_HC)
			SDCARD_PRT("High Capacity SD Memory Card\r\n");
		else
			SDCARD_PRT("Standard Capacity SD Memory Card\r\n");
	} else if (err == SD_CMD_RSP_TIMEOUT) {
		int retry_time = SD_MAX_VOLT_TRIAL;
		UINT32 resp0;
		while (retry_time) {
			err = sdcard_acmd41_process(OCR_MSK_VOLTAGE_ALL);
			if (err != SD_OK) {
				SDCARD_FATAL("send cmd55&cmd41 err, quite loop\r\n");
				goto err_return;
			}
			sdio_get_cmdresponse_argument(0, &resp0);
			if (resp0 & OCR_MSK_BUSY) {
				if (resp0 & OCR_MSK_HC)
					sdcard.Addr_shift_bit = 0;
				else
					sdcard.Addr_shift_bit = 9;
				break;
			}
			rtos_delay_milliseconds(2);
			retry_time--;
		}
		if (!retry_time) {
			SDCARD_FATAL("send cmd55&cmd41 retry time out, maybe a MMC card\r\n");
			err = SD_ERROR;
			goto err_return;
		}
		SDCARD_PRT("send cmd55&cmd41 complete, SD V1.X card is ready\r\n");
	}
	rtos_delay_milliseconds(2);
	// get CID, return R2
	err = sdcard_cmd2_process();
	if (err != SD_OK) {
		SDCARD_FATAL("send cmd2 err:%d\r\n", err);
		goto err_return;
	}
	rtos_delay_milliseconds(2);
	// get RCA,
	err = sdcard_cmd3_process();
	if (err != SD_OK) {
		SDCARD_FATAL("send cmd3 err:%d\r\n", err);
		goto err_return;
	}

#if (defined(CONFIG_SOC_BK7256XX))	//temp code, will be switch to sdcard_driver.c
	sdcard_clock_set(CLK_80M);
#else
	// change to default speed clk
	sdcard_clock_set(CLK_13M);
#endif

	rtos_delay_milliseconds(2);
	// get CSD
	err = sdcard_cmd9_process(SD_CARD);
	if (err != SD_OK) {
		SDCARD_FATAL("send cmd9 err:%d\r\n", err);
		goto err_return;
	}
	rtos_delay_milliseconds(2);
	// select card
	err = sdcard_cmd7_process();
	if (err != SD_OK) {
		SDCARD_FATAL("send cmd7 err:%d\r\n", err);
		goto err_return;
	}
	rtos_delay_milliseconds(2);
	// change bus width, for high speed
	err = sdcard_acmd6_process();
	if (err != SD_OK) {
		SDCARD_FATAL("send acmd6 err:%d\r\n", err);
		goto err_return;
	}

	// Sd_MMC_flag = SD_CARD;
	err = SD_OK;
	SDCARD_PRT("sdcard initialize is done\r\n");
	goto right_return;

MMC_init:
	err = sdcard_cmd2_process();
	SDCARD_LOGI("cmd 2 :%x\r\n", err);
	if (err != SD_OK)
		goto err_return;
	err = sdcard_mmc_cmd3_process();
	SDCARD_LOGI("cmd 3 :%x\r\n", err);

#if (defined(CONFIG_SOC_BK7256XX))	//temp code, will be switch to sdcard_driver.c
	sdcard_clock_set(CLK_80M);
#else
	sdcard_clock_set(CLK_13M);
#endif

	err = sdcard_cmd9_process(MMC_CARD);
	SDCARD_LOGI("cmd 9 :%x\r\n", err);
	if (sdcard.Addr_shift_bit == 0) {
		err = sdcard_mmc_cmd8_process();
		SDCARD_LOGI("cmd 8 :%x\r\n", err);
	}
	if (err != SD_OK)
		goto err_return;
	err = sdcard_cmd7_process();
	if (err != SD_OK)
		goto err_return;
	// Sd_MMC_flag = MMC_CARD;
	goto right_return;

right_return:
	NoneedInitflag = 1;
err_return:
	sdio_clk_config(0);
	return err;
}

void sdcard_uninitialize(void)
{
	sdio_hw_uninit();
	sdio_sw_init();
	NoneedInitflag = 0;
}

void sdcard_get_card_info(SDCARD_S *card_info)
{
	card_info->total_block = sdcard.total_block;
	card_info->block_size = sdcard.block_size;
	card_info->card_rca = sdcard.card_rca;
	card_info->init_flag = sdcard.init_flag;
	card_info->clk_cfg = sdcard.clk_cfg;
	card_info->Addr_shift_bit = sdcard.Addr_shift_bit;
}

#if defined(CONFIG_SOC_BK7256XX)
static void sdcard_dump_transfer_data(UINT8 *write_buff, uint32_t first_block, uint32_t cnt)
{
	uint32_t i = 0;

	SDCARD_DBG("[+]sdcard_dump_transfer_data:addr=0x%x,cnt=%d\r\n", write_buff, cnt);

	for(i = 0; i < cnt; i+=16)
	{
		SDCARD_DBG("0x%08x, 0x%08x, 0x%08x, 0x%08x\r\n",
			(uint32_t)(*(uint32_t *)&write_buff[i]),
			(uint32_t)(*(uint32_t *)&write_buff[i+4]),
			(uint32_t)(*(uint32_t *)&write_buff[i+8]),
			(uint32_t)(*(uint32_t *)&write_buff[i+12]));
	}

	SDCARD_DBG("[-]sdcard_dump_transfer_data:addr=0x%x,cnt=%d\r\n", first_block, cnt);
}
#endif

SDIO_Error
sdcard_read_single_block(UINT8 *readbuff, UINT32 readaddr, UINT32 blocksize)
{
	SDIO_CMD_S cmd;
	SDIO_Error ret;

#if defined(CONFIG_SOC_BK7256)

#else
#if defined(CONFIG_SDCARD)
	if(sdcard_check_inserted() == false)
		return SD_ERROR;
#endif
#endif

	sdio_clk_config(1);

	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, 0xffffffff);
	// setup data reg first
	sdio_set_data_timeout(get_timeout_param(0)); //DEF_DATA_TIME_OUT);
	sdio_setup_data(SDIO_RD_DATA, blocksize);

	cmd.index = READ_SINGLE_BLOCK;
	cmd.arg = (UINT32)(readaddr << sdcard.Addr_shift_bit);
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;
	sdio_send_cmd(&cmd);
	cmd.err = sdio_wait_cmd_response(cmd.index);

	ret = cmd.err;

	if (ret == SD_CMD_RSP_TIMEOUT) {
		SDCARD_FATAL("cmd17 noresp, readsingle block err\r\n");
		goto read_return;
	} else if (ret == SD_CMD_CRC_FAIL) {
		SDCARD_FATAL("cmd17 cmdcrc err, readsingle block err\r\n");
		goto read_return;
	}

	cmd.err = sdcard_wait_receive_data(readbuff);
	ret = cmd.err;

	if (ret != SD_OK) {
		SDCARD_FATAL("read single block wait data receive err:%d\r\n", cmd.err);
		goto read_return;
	}
read_return:
	sdio_clk_config(0);
	return ret;
}

static SDIO_Error sdcard_send_write_stop(int err);
/////////read:first phase////////////
static void sd_read_data_init(void)
{
	UINT32 reg;

#if defined(CONFIG_SOC_BK7256XX)

#else
#if defined(CONFIG_SDCARD)
	if(sdcard_check_inserted() == false)
		return;
#endif
#endif

	sdio_clk_config(1);
	reg  = get_timeout_param(0);
	REG_WRITE(REG_SDCARD_DATA_REC_TIMER, reg);
	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, 0xFFFFFFFF);

#if defined(CONFIG_SOC_BK7256XX)
		reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);

		//reset to 0
		reg &= ~(SDCARD_FIFO_SD_STA_RST | SDCARD_FIFO_TX_FIFO_RST | SDCARD_FIFO_RX_FIFO_RST);
		REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);

		//reset to 1
		reg |= (SDCARD_FIFO_SD_STA_RST | SDCARD_FIFO_TX_FIFO_RST | SDCARD_FIFO_RX_FIFO_RST);
		REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);

		reg &= (0xffff | (SDCARD_FIFO_SD_RATE_SELECT_MASK << SDCARD_FIFO_SD_RATE_SELECT_POSI) | SDIO_REG0XD_CLK_REC_SEL | SDIO_REG0XD_SD_RD_WAIT_SEL);
		reg |= (0x0101 | SDCARD_FIFO_RX_FIFO_RST);
		REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);
#else
		reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
		reg &= ~(0xFFFF | (1 << 16) | (1 << 20));
		reg  |= (0x0101 | ((1 << 16) | (1 << 20)));
		REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);
#endif

#ifdef CONFIG_SDCARD_BUSWIDTH_4LINE
	reg = 0x1 | (1 << 2) | (1 << 3) | (512 << 4) | (1 << 17);
	REG_WRITE(REG_SDCARD_DATA_REC_CTRL, reg);
#else
	reg = 0x1 | (0 << 2) | (1 << 3) | (512 << 4) | (1 << 17);
	REG_WRITE(REG_SDCARD_DATA_REC_CTRL, reg);
#endif
}

/////////read:second phase////////////
static SDIO_Error sdcard_rcv_data(UINT8 *read_buff, int block_num)
{
	int i, Ret, reg;
	UINT32 size;
	Ret = SD_OK;
	i = 0;

	size = block_num << 9;/* *512*/;

	while (1) {
		reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
		if (reg & SDCARD_CMDRSP_DATA_REC_END_INT) {
			REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, SDCARD_CMDRSP_DATA_REC_END_INT);
			if (reg & SDCARD_CMDRSP_DATA_CRC_FAIL)
				Ret = SD_DATA_CRC_FAIL;

			while (1) {
				reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
				if (reg & SDCARD_FIFO_RXFIFO_RD_READY) {
					reg = REG_READ(REG_SDCARD_RD_DATA_ADDR);
					*(read_buff + i++) = reg & 0xff;
					*(read_buff + i++) = (reg >> 8) & 0xff;
					*(read_buff + i++) = (reg >> 16) & 0xff;
					*(read_buff + i++) = (reg >> 24) & 0xff;
					if ((i % 512) == 0)
						break;
				}
			}

			if (i >= size)
				break;
		} else {
			if (reg & SDCARD_CMDRSP_DATA_TIME_OUT_INT) {
				Ret = SD_DATA_TIMEOUT;
				break;
			}
		}
	}
	return Ret;
}

#if 1
SDIO_Error sdcard_read_multi_block(UINT8 *read_buffer, int first_block, int block_num)
{
	int ret = SD_OK;
	UINT8 op_flag = 0;

	if (SDIO_WR_flag == SDIO_WR_DATA) {
		op_flag = 1;	//write stop
	} else {
		if (last_WR_addr == first_block) {
			op_flag = 0;//continue read
		} else {
			op_flag = 2;//read stop
		}
	}

	if (1 == no_need_send_cmd12_flag)
		op_flag = 3;//stop has send

	no_need_send_cmd12_flag = 0;
	if (0 == op_flag)
		ret = sdcard_rcv_data(read_buffer, block_num);
	else {
#if defined(CONFIG_SOC_BK7256XX)
		uint32_t ret2 = 0;
		uint32_t retry_cnt = 0;
#endif

		if (1 == op_flag)
			ret = sdcard_send_write_stop(0);
		else if (2 == op_flag)
			ret = sdcard_send_read_stop();

/*
 * WORKAROUND:
 * Some special sdcard maybe at busy status after CMD12,
 * so it can't response the next cmd,F.E:CMD18 which reads multi-block data.
 * BK7256XX_MP chip can't retry with CMD18, if it sends CMD18,BK7256 sdio
 * will start to read data from data-lines to the sdio FIFO but the data is 0.
 */
#if defined(CONFIG_SOC_BK7256XX)
		/*
		 * after stop cmd, should do check busy;
		 * 1.read to switch start address
		 * 2.write to read
		 * 3.write and has sent stop(file system use sdcard_ctrl), and then start read
		 */
		if((1 == op_flag) || (2 == op_flag) || ((3 == op_flag) && SDIO_WR_flag == SDIO_WR_DATA))
		{
			sdio_clk_config(1);
			while(retry_cnt < 256)
			{
				retry_cnt++;

				ret2 = sdcard_acmd6_process();
				if (SD_OK == ret2)
				{
					SDCARD_DBG("check sdcard ready:acmd6 pass,retry_cnt=%d\r\n", retry_cnt);
					break;
				}
			}
		}
#endif
		sd_read_data_init();
#if defined(CONFIG_SOC_BK7256XX)
		if(ret2 == SD_OK)	//ACMD6 pass means sdcard is not busy.
		{
			//CMD18:notify sdcard,will read multi-block data
			ret2 = sdcard_cmd18_process(first_block);
			if (SD_OK == ret2)
			{
				ret2 += sdcard_rcv_data(read_buffer, block_num);
			}
			else
				SDCARD_FATAL("--cmd18 send error:ret=%d\r\n", ret2);
		}

		ret += ret2;
#else
		ret += sdcard_cmd18_process(first_block);
		if (SD_OK == ret)
		{
			ret = sdcard_rcv_data(read_buffer, block_num);
		}
#endif

	}

	if (SD_OK != ret) {
		no_need_send_cmd12_flag = 1;
		ret += sdcard_send_read_stop();
	}

#if defined(CONFIG_SOC_BK7256XX)
	{
		SDCARD_DBG("[-]%s:first_block=%d,block_num=%d\r\n", __func__, first_block, block_num);
		sdcard_dump_transfer_data(read_buffer, first_block, block_num*512);
	}
#endif

	SDIO_WR_flag = SDIO_RD_DATA;
	last_WR_addr = first_block + block_num;
	return ret;
}
#else
/********* actual function:  single read*********/
SDIO_Error sdcard_read_multi_block(UINT8 *read_buff, int first_block, int block_num)
{
	int Ret = SD_OK;
	unsigned int i;
	SDIO_CMD_S cmd;
	UINT32 reg;

	//rt_kprintf("----single read: %d\r\n",SDIO_WR_flag);

	if (SDIO_WR_flag == SDIO_WR_DATA) {		// if last state is multi write ,send stop cmd
		SDIO_WR_flag = SDIO_RD_DATA;
		Ret = sdcard_send_write_stop(0);		//write stop
	}

#if defined(CONFIG_SOC_BK7256)

#else
#if defined(CONFIG_SDCARD)
	if(sdcard_check_inserted() == false)
		return SD_ERROR;
#endif
#endif

	sdio_clk_config(1);
	reg  = get_timeout_param(0);
	REG_WRITE(REG_SDCARD_DATA_REC_TIMER, reg);
	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, 0xFFFFFFFF);
	reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
	reg &= ~(0xFFFF | (1 << 16) | (1 << 20));
	reg  |= (0x0101 | ((1 << 16) | (1 << 20)));
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);

#ifdef CONFIG_SDCARD_BUSWIDTH_4LINE
	reg = 0x1 | (1 << 2) | (1 << 3) | (512 << 4) | (1 << 17);
	REG_WRITE(REG_SDCARD_DATA_REC_CTRL, reg);
#else
	reg = 0x1 | (0 << 2) | (1 << 3) | (512 << 4) | (1 << 17);
	REG_WRITE(REG_SDCARD_DATA_REC_CTRL, reg);
#endif

	// if((SD_CARD == Sd_MMC_flag)&&(driver_sdcard.total_block > 0x100000))
	{
		cmd.index = 18;
		cmd.arg = (UINT32)(first_block << sdcard.Addr_shift_bit);
		cmd.flags = SD_CMD_SHORT;
		cmd.timeout = get_timeout_param(1);
		cmd.crc_check = true;
		sdio_send_cmd(&cmd);
		cmd.err = sdio_wait_cmd_response(cmd.index);

		Ret = cmd.err;
	}
	if (Ret == SD_OK) {
		unsigned int size = SD_DEFAULT_BLOCK_SIZE  * block_num;
		//reveive data
		i = 0;
		while (1) {
			reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
			if (reg & SDCARD_CMDRSP_DATA_REC_END_INT) {
				REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, SDCARD_CMDRSP_DATA_REC_END_INT);
				if (reg & SDCARD_CMDRSP_DATA_CRC_FAIL)
					Ret = SD_DATA_CRC_FAIL;

				while (1) {
					reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
					if (reg & SDCARD_FIFO_RXFIFO_RD_READY) {
						reg = REG_READ(REG_SDCARD_RD_DATA_ADDR);
						*(read_buff + i++) = reg & 0xff;
						*(read_buff + i++) = (reg >> 8) & 0xff;
						*(read_buff + i++) = (reg >> 16) & 0xff;
						*(read_buff + i++) = (reg >> 24) & 0xff;
						if ((i % 512) == 0)
							break;
					}
				}
				if (i >= size)
					break;
			} else {
				if (reg & SDCARD_CMDRSP_DATA_TIME_OUT_INT) {
					Ret = SD_DATA_TIMEOUT;
					break;
				}
			}
		}
	}
	reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
	reg &= ~(0xFFFF | (1 << 16) | (1 << 20));
	reg |= (0x0101 | ((1 << 16) | (1 << 20)));
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);
	Ret += sdcard_cmd12_process(get_timeout_param(1));

	sdio_clk_config(0);

	if (Ret != SD_OK)
		SDCARD_LOGI("SD Ret:%d\r\n", Ret);
	return Ret;
}
#endif
SDIO_Error sdcard_write_single_block(UINT8 *writebuff, UINT32 writeaddr)
{
	int  i, ret;
	SDIO_CMD_S cmd;
	UINT32 tmpval, reg;

#if defined(CONFIG_SOC_BK7256XX)

#else
#if defined(CONFIG_SDCARD)
	if(sdcard_check_inserted() == false)
		return SD_ERROR;
#endif
#endif

	sdio_clk_config(1);

	reg = REG_READ(REG_SDCARD_DATA_REC_CTRL);
	reg &= (~((1 << 16) | (1 << 0) | (1 << 1) | (1 << 3)));
	REG_WRITE(REG_SDCARD_DATA_REC_CTRL, reg);

	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, 0xffffffff);

	reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg | SDCARD_FIFO_SD_STA_RST);

#if defined(CONFIG_SOC_BK7256XX)
	reg &= (0xffff | (SDCARD_FIFO_SD_RATE_SELECT_MASK << SDCARD_FIFO_SD_RATE_SELECT_POSI) | SDIO_REG0XD_CLK_REC_SEL | SDIO_REG0XD_SD_RD_WAIT_SEL);
#else
	reg &= (0xffff | (SDCARD_FIFO_SD_RATE_SELECT_MASK << SDCARD_FIFO_SD_RATE_SELECT_POSI));
#endif
	reg |= (0x0101 /*| SDCARD_FIFO_TX_FIFO_RST*/);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);

	i = 0;
	while (REG_READ(REG_SDCARD_FIFO_THRESHOLD) & SDCARD_FIFO_TXFIFO_WR_READY) {
		tmpval = (writebuff[i] << 24) | (writebuff[i + 1] << 16) | (writebuff[i + 2] << 8) | writebuff[i + 3];
		REG_WRITE(REG_SDCARD_WR_DATA_ADDR, tmpval);
		i += 4;
		if (SD_DEFAULT_BLOCK_SIZE <= i)
			break;
	}

	cmd.index = 24;//WRITE_MULTIPLE_BLOCK;
	cmd.arg = (UINT32)(writeaddr << sdcard.Addr_shift_bit);
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;
	sdio_send_cmd(&cmd);

	cmd.err = sdio_wait_cmd_response(cmd.index);
	ret = cmd.err;

	if (SD_OK == ret) {
		REG_WRITE(REG_SDCARD_DATA_REC_TIMER, get_timeout_param(1));
		reg = (1 << 16) | (0 << 3) | (512 << 4) | (1 << 17)
#ifdef CONFIG_SDCARD_BUSWIDTH_4LINE
			  | SDCARD_DATA_REC_CTRL_DATA_BUS
#endif
			  ;
		REG_WRITE(REG_SDCARD_DATA_REC_CTRL, reg);

#if defined(CONFIG_SOC_BK7256XX)
	//no need to check tx fifo need write
#else
		do {
			reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
		} while (!(reg & SDCARD_CMDRSP_TX_FIFO_NEED_WRITE));
#endif

		while (1) {
			reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);

			if (reg & (SDCARD_CMDRSP_DATA_TIME_OUT_INT)) {
				ret = SD_ERROR;
				break;
			}

			if (reg & SDCARD_CMDRSP_DATA_WR_END_INT) {
				if (2 != ((reg & SDCARD_CMDRSP_WR_STATU) >> 20))
					ret = SD_ERROR;
				else
					ret = SD_OK;
				break;
			}
		}
		reg = REG_READ(REG_SDCARD_DATA_REC_CTRL);
		reg &= (~((1 << 16) | (1 << 0) | (1 << 1) | (1 << 3))
#ifdef CONFIG_SDCARD_BUSWIDTH_4LINE
				| SDCARD_DATA_REC_CTRL_DATA_BUS
#endif
			   );
		REG_WRITE(REG_SDCARD_DATA_REC_CTRL, reg);
	}

	if (ret != SD_OK)
		SDCARD_LOGI("--single blk write err:%d---\r\n", ret);
	sdio_clk_config(0);

	return ret;
}

////////////////write:first phase///////////
static SDIO_Error sdcard_cmd25_process(UINT32 block_addr)
{
	SDIO_CMD_S cmd;
	UINT32 reg;

#if defined(CONFIG_SOC_BK7256XX)

#else
#if defined(CONFIG_SDCARD)
	if(sdcard_check_inserted() == false)
		return SD_ERROR;
#endif
#endif

	sdio_clk_config(1);
	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, 0xffffffff);

#if defined(CONFIG_SOC_BK7256XX)
	reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);

	//reset to 0: 0 active
	reg &= ~(SDCARD_FIFO_SD_STA_RST | SDCARD_FIFO_TX_FIFO_RST | SDCARD_FIFO_RX_FIFO_RST);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);

	//reset to 1
	reg |= (SDCARD_FIFO_SD_STA_RST | SDCARD_FIFO_TX_FIFO_RST | SDCARD_FIFO_RX_FIFO_RST);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);

	reg &= (0xffff | (SDCARD_FIFO_SD_RATE_SELECT_MASK << SDCARD_FIFO_SD_RATE_SELECT_POSI) | SDIO_REG0XD_CLK_REC_SEL | SDIO_REG0XD_SD_RD_WAIT_SEL);
	reg |= (0x0101 | SDCARD_FIFO_TX_FIFO_RST);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);
#else
	reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg | SDCARD_FIFO_SD_STA_RST);

	reg &= (0xffff | (SDCARD_FIFO_SD_RATE_SELECT_MASK << SDCARD_FIFO_SD_RATE_SELECT_POSI));
	reg |= (0x0101 | SDCARD_FIFO_TX_FIFO_RST);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);
#endif

	cmd.index = 25;//WRITE_MULTIPLE_BLOCK;
	cmd.arg = (UINT32)(block_addr << sdcard.Addr_shift_bit);
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;
	sdio_send_cmd(&cmd);

	cmd.err = sdio_wait_cmd_response(cmd.index);
	return cmd.err;
}

////////////////write:second phase///////////
static SDIO_Error sdcard_write_data(UINT8 *write_buff, UINT32 block_num, UINT8 first_data_after_cmd)
{
	int i, j, tmpval, reg, ret;
	i = 0;
	ret = SD_OK;

	if (1 == first_data_after_cmd) {
		// 1. fill the first block to fifo and start write data enable
		while (REG_READ(REG_SDCARD_FIFO_THRESHOLD) & SDCARD_FIFO_TXFIFO_WR_READY) {
			tmpval = (write_buff[i] << 24) | (write_buff[i + 1] << 16) | (write_buff[i + 2] << 8) | write_buff[i + 3];
			REG_WRITE(REG_SDCARD_WR_DATA_ADDR, tmpval);
			i += 4;
			if (SD_DEFAULT_BLOCK_SIZE <= i)
				break;
		}

#if defined(CONFIG_SOC_BK7256XX)
		//soc modification:enable clock gate function.
		reg = REG_READ(REG_SDCARD_CMD_RSP_INT_MASK);
		reg |= (0x1 << SDIO_REG0XA_TX_FIFO_NEED_WRITE_MASK_CG_POS);
		REG_WRITE(REG_SDCARD_CMD_RSP_INT_MASK, reg);
#endif

		reg = REG_READ(REG_SDCARD_CMD_RSP_INT_MASK);
		reg |= SDCARD_CMDRSP_TX_FIFO_EMPTY_MASK;
		REG_WRITE(REG_SDCARD_CMD_RSP_INT_MASK, reg);

		REG_WRITE(REG_SDCARD_DATA_REC_TIMER, get_timeout_param(0));
		reg = (SD_DEFAULT_BLOCK_SIZE << SDCARD_DATA_REC_CTRL_BLK_SIZE_POSI) |
			  SDCARD_DATA_REC_CTRL_DATA_MUL_BLK | SDCARD_DATA_REC_CTRL_DATA_BYTE_SEL |
			  SDCARD_DATA_REC_CTRL_DATA_WR_DATA_EN
#ifdef CONFIG_SDCARD_BUSWIDTH_4LINE
			  | SDCARD_DATA_REC_CTRL_DATA_BUS
#endif
			  ;
		REG_WRITE(REG_SDCARD_DATA_REC_CTRL, reg);

#if defined(CONFIG_SOC_BK7256XX)
		{
			uint32_t k = 0;
			do {
				reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);

				k++;
				if(k > 0x10000000)
				{
					SDCARD_FATAL("write data too much time\r\n");
					break;
				}
			} while (!(reg & SDCARD_CMDRSP_DATA_WR_END_INT));
			SDCARD_DBG("SD Info:write blk data end, k=%d\r\n", k);
			REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, SDCARD_CMDRSP_DATA_WR_END_INT);
		}
#endif

		do {
			reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
		} while (!(reg & SDCARD_CMDRSP_TX_FIFO_NEED_WRITE));

		block_num -= 1;
	}

	// 2. write other blocks
	while (block_num) {
		block_num--;
		j = 0;
		while (j < SD_DEFAULT_BLOCK_SIZE) {
			if (REG_READ(REG_SDCARD_FIFO_THRESHOLD) & SDCARD_FIFO_TXFIFO_WR_READY) {
				tmpval = (write_buff[i] << 24) | (write_buff[i + 1] << 16) | (write_buff[i + 2] << 8) | write_buff[i + 3];
				REG_WRITE(REG_SDCARD_WR_DATA_ADDR, tmpval);
				i += 4;
				j += 4;
			}
		}

		do {
			reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
		} while (!(reg & SDCARD_CMDRSP_DATA_WR_END_INT));

		REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, SDCARD_CMDRSP_DATA_WR_END_INT);

#if defined(CONFIG_SOC_BK7256XX)

#else
		do {
			reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
		} while (!(reg & SDCARD_CMDRSP_TX_FIFO_NEED_WRITE));
#endif
		if (2 != ((reg & SDCARD_CMDRSP_WR_STATU) >> 20)) {
			ret = SD_ERROR;
			SDCARD_LOGI("write data error !!!\r\n");
			break;
		}
	}
	return ret;
}
////////////////write:last phase///////////
static SDIO_Error sdcard_send_write_stop(int err)
{
	int reg, ret;
	GLOBAL_INT_DECLARATION();
	ret = SD_OK;
	//	if(err != ret)
	{
		// 3. after the last block,write zero
		GLOBAL_INT_DISABLE();
#if defined(CONFIG_SOC_BK7256XX)
		//soc modification:uses recovery function to instead fill-0.
#else
		while (1) {
			reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
			if (reg & SDCARD_FIFO_TXFIFO_WR_READY) {
				REG_WRITE(REG_SDCARD_WR_DATA_ADDR, 0);
				break;
			}
		}
		// 4.wait and clear flag
		do {
			reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
			if (reg & (SDCARD_CMDRSP_DATA_BUSY | SDCARD_CMDRSP_DATA_WR_END_INT))
				break;
		} while (1); //BUSY
		if ((reg & SDCARD_CMDRSP_DATA_BUSY)) {
			reg = REG_READ(REG_SDCARD_CMD_RSP_INT_MASK);
			reg &= ~SDCARD_CMDRSP_TX_FIFO_EMPTY_MASK;
			REG_WRITE(REG_SDCARD_CMD_RSP_INT_MASK, reg);
		}
		if ((reg & SDCARD_CMDRSP_DATA_WR_END_INT))
			REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, SDCARD_CMDRSP_DATA_WR_END_INT);

		reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
		if (2 != ((reg & SDCARD_CMDRSP_WR_STATU) >> 20))
			ret =  SD_ERROR;
		else
			ret =  SD_OK;
#endif
		reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
		reg |= SDCARD_FIFO_TX_FIFO_RST;
		REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);

		reg = REG_READ(REG_SDCARD_CMD_RSP_INT_MASK);
		reg &= ~SDCARD_CMDRSP_TX_FIFO_EMPTY_MASK;
		REG_WRITE(REG_SDCARD_CMD_RSP_INT_MASK, reg);
		GLOBAL_INT_RESTORE();

#if defined(CONFIG_SOC_BK7256XX)

#else
#if defined(CONFIG_SDCARD)
		if(sdcard_check_inserted() == false)
			return SD_ERROR;
#endif
#endif
	}
	ret += sdcard_cmd12_process(0);
	if (ret != SD_OK)
		SDCARD_FATAL("===write err:%x====\r\n", ret);
	ret += err;
	return ret;
}

#if 1
SDIO_Error sdcard_write_multi_block(UINT8 *write_buff, UINT32 first_block, UINT32 block_num)
{
	int ret = SD_OK;
	UINT8 op_flag = 0;
	if (SDIO_WR_flag == SDIO_RD_DATA) {
		op_flag = 1;	//read stop
	} else {
		if (last_WR_addr == first_block) {
			op_flag = 0;//continue write
		} else {
			op_flag = 2;//write stop
		}
	}

#if defined(CONFIG_SOC_BK7256XX)
	{
		SDCARD_DBG("%s:start dump first_block=%d, block_num = %d\r\n", __func__, first_block, block_num);
		sdcard_dump_transfer_data(write_buff, first_block, block_num*512);
	}
#endif

	if (1 == no_need_send_cmd12_flag)
		op_flag = 3;//stop has send

	//SDCARD_LOGI("===sd write: start = %d,block_num = %d,op_flag = %d=====\r\n", first_block, block_num, op_flag);

	no_need_send_cmd12_flag = 0;
	if (0 == op_flag)//continue write
		ret = sdcard_write_data(write_buff, block_num, 0);
	else {
#if defined(CONFIG_SOC_BK7256XX)
		uint32_t retry_cnt = 0;
		uint32_t ret2 = 0;
#endif
		if (1 == op_flag) {
			/************if last state is single read:not send stop;***************/
			ret = sdcard_send_read_stop();
		} else if (op_flag == 2) {
			ret = sdcard_send_write_stop(0);
		}

#if defined(CONFIG_SOC_BK7256XX)
		if(ret == SD_OK)
		{
			//CMD25:notify sdcard,will write multi-block data
			while(retry_cnt < 16)	//add retry count,maybe the card is busy after CMD12.
			{
				retry_cnt++;

				ret2 = sdcard_cmd25_process(first_block);
				if (SD_OK == ret2)
				{
					ret2 += sdcard_write_data(write_buff, block_num, 1);
					if(SD_OK == ret2)
					{
						SDCARD_DBG("write data try_cnt=%d pass\r\n", retry_cnt);
						break;
					}
					else
						SDCARD_FATAL("sdcard write data fail \r\n");
				}
			}
			if(retry_cnt >= 16)
				SDCARD_FATAL("cmd25 retry_cnt=%d fail:ret=%d\r\n", retry_cnt);

			ret += ret2;
		}
#else
	//CMD25:notify sdcard,will write multi-block data
	ret += sdcard_cmd25_process(first_block);
	if (SD_OK == ret)
		ret += sdcard_write_data(write_buff, block_num, 1);
	else
		SDCARD_FATAL("--cmd25 send error:ret=%d\r\n", ret);
#endif
	}
	if (ret != SD_OK) {
		ret += sdcard_send_write_stop(ret);
		no_need_send_cmd12_flag = 1;
	}
	last_WR_addr = first_block + block_num;
	SDIO_WR_flag = SDIO_WR_DATA;
	return ret;
}
#else
SDIO_Error sdcard_write_multi_block(UINT8 *write_buff, UINT32 first_block, UINT32 block_num)
{
	SDIO_CMD_S cmd;
	int ret;
	UINT32 i, j, reg, tmpval;
	GLOBAL_INT_DECLARATION();

#if defined(CONFIG_SOC_BK7256)

#else
#if defined(CONFIG_SDCARD)
	if(sdcard_check_inserted() == false)
		return SD_ERROR;
#endif
#endif

	sdio_clk_config(1);
	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, 0xffffffff);
	reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);

	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg | SDCARD_FIFO_SD_STA_RST);

#if defined(CONFIG_SOC_BK7256XX)
	reg &= (0xffff | (SDCARD_FIFO_SD_RATE_SELECT_MASK << SDCARD_FIFO_SD_RATE_SELECT_POSI) | SDIO_REG0XD_SD_RD_WAIT_SEL | SDIO_REG0XD_CLK_REC_SEL);
	reg |= (0x0101 | SDCARD_FIFO_TX_FIFO_RST);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);
#else
	reg &= (0xffff | (SDCARD_FIFO_SD_RATE_SELECT_MASK << SDCARD_FIFO_SD_RATE_SELECT_POSI));
	reg |= (0x0101 | SDCARD_FIFO_TX_FIFO_RST);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);
#endif

	cmd.index = 25;//WRITE_MULTIPLE_BLOCK;
	cmd.arg = (UINT32)(first_block << sdcard.Addr_shift_bit);
	cmd.flags = SD_CMD_SHORT;
	cmd.timeout = get_timeout_param(1);
	cmd.crc_check = true;
	sdio_send_cmd(&cmd);

	cmd.err = sdio_wait_cmd_response(cmd.index);
	ret = cmd.err;

	if (SD_OK == ret) {
		i = 0;
		// 1. fill the first block to fifo and start write data enable
		while (REG_READ(REG_SDCARD_FIFO_THRESHOLD) & SDCARD_FIFO_TXFIFO_WR_READY) {
			tmpval = (write_buff[i] << 24) | (write_buff[i + 1] << 16) | (write_buff[i + 2] << 8) | write_buff[i + 3];
			REG_WRITE(REG_SDCARD_WR_DATA_ADDR, tmpval);
			i += 4;
			if (SD_DEFAULT_BLOCK_SIZE <= i)
				break;
		}
		reg = REG_READ(REG_SDCARD_CMD_RSP_INT_MASK);
		reg |= SDCARD_CMDRSP_TX_FIFO_EMPTY_MASK;
		REG_WRITE(REG_SDCARD_CMD_RSP_INT_MASK, reg);

		REG_WRITE(REG_SDCARD_DATA_REC_TIMER, get_timeout_param(0));
		reg = (SD_DEFAULT_BLOCK_SIZE << SDCARD_DATA_REC_CTRL_BLK_SIZE_POSI) |
			  SDCARD_DATA_REC_CTRL_DATA_MUL_BLK | SDCARD_DATA_REC_CTRL_DATA_BYTE_SEL |
			  SDCARD_DATA_REC_CTRL_DATA_WR_DATA_EN
#ifdef CONFIG_SDCARD_BUSWIDTH_4LINE
			  | SDCARD_DATA_REC_CTRL_DATA_BUS
#endif
			  ;
		REG_WRITE(REG_SDCARD_DATA_REC_CTRL, reg);

#if defined(CONFIG_SOC_BK7256XX)

#else
		do {
			reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
		} while (!(reg & SDCARD_CMDRSP_TX_FIFO_NEED_WRITE));
#endif

		// 2. write other blocks
		while (--block_num) {
			j = 0;
			while (j < SD_DEFAULT_BLOCK_SIZE) {
				if (REG_READ(REG_SDCARD_FIFO_THRESHOLD) & SDCARD_FIFO_TXFIFO_WR_READY) {
					tmpval = (write_buff[i] << 24) | (write_buff[i + 1] << 16) | (write_buff[i + 2] << 8) | write_buff[i + 3];
					REG_WRITE(REG_SDCARD_WR_DATA_ADDR, tmpval);
					i += 4;
					j += 4;
				}
			}

			do {
				reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
			} while (!(reg & SDCARD_CMDRSP_DATA_WR_END_INT));
			REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, SDCARD_CMDRSP_DATA_WR_END_INT);

#if defined(CONFIG_SOC_BK7256XX)

#else
			do {
				reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
			} while (!(reg & SDCARD_CMDRSP_TX_FIFO_NEED_WRITE));
#endif

			if (2 != ((reg & SDCARD_CMDRSP_WR_STATU) >> 20)) {
				ret = SD_ERROR;
				goto sndcmd12;
			}
		}

		// 3. after the last block,write zero
		GLOBAL_INT_DISABLE();
		while (1) {
			reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
			if (reg & SDCARD_FIFO_TXFIFO_WR_READY) {
				REG_WRITE(REG_SDCARD_WR_DATA_ADDR, 0);
				break;
			}
		}
		// 4.wait and clear flag
		do {
			reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
			if (reg & (SDCARD_CMDRSP_DATA_BUSY | SDCARD_CMDRSP_DATA_WR_END_INT))
				break;
		} while (1); //BUSY

		if ((reg & SDCARD_CMDRSP_DATA_BUSY)) {
			reg = REG_READ(REG_SDCARD_CMD_RSP_INT_MASK);
			reg &= ~SDCARD_CMDRSP_TX_FIFO_EMPTY_MASK;
			REG_WRITE(REG_SDCARD_CMD_RSP_INT_MASK, reg);
		}
		if ((reg & SDCARD_CMDRSP_DATA_WR_END_INT))
			REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, SDCARD_CMDRSP_DATA_WR_END_INT);

		reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
		if (2 != ((reg & SDCARD_CMDRSP_WR_STATU) >> 20))
			ret =  SD_ERROR;
		else
			ret =  SD_OK;
	}
	reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
	reg |= SDCARD_FIFO_TX_FIFO_RST;
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);
	reg = REG_READ(REG_SDCARD_CMD_RSP_INT_MASK);
	reg &= ~SDCARD_CMDRSP_TX_FIFO_EMPTY_MASK;
	REG_WRITE(REG_SDCARD_CMD_RSP_INT_MASK, reg);

	GLOBAL_INT_RESTORE();

#if defined(CONFIG_SOC_BK7256XX)

#else
#if defined(CONFIG_SDCARD)
	if(sdcard_check_inserted() == false)
		return SD_ERROR;
#endif
#endif

sndcmd12:
	ret += sdcard_cmd12_process(0);
	if (ret != SD_OK)
		SDCARD_FATAL("===write err:%x,%x,%x====\r\n", first_block, ret, cmd.err);
#if 0
#if defined(CONFIG_SDCARD)
	if(sdcard_check_inserted() == false)
		return SD_ERROR;
#endif
	sdio_clk_config(0);
#endif
	return ret;
}
#endif
void sdcard_init(void)
{
	ddev_register_dev(DD_DEV_TYPE_SDCARD, (DD_OPERATIONS *)&sdcard_op);
	//sdcard_cd_timer_init();
}

void sdcard_exit(void)
{
	ddev_unregister_dev(DD_DEV_TYPE_SDCARD);
}


/******************************************************************************/
/***************************** sdcard API function ****************************/
/******************************************************************************/

UINT32 sdcard_open(UINT32 op_flag)
{
	UINT8 cnt;

	SDCARD_LOGI("===sd card open:%d===\r\n", NoneedInitflag);
	cnt = 3;
	while (1) {
		if (sdcard_initialize() == SD_OK)
			break;
		if (--cnt == 0) {
			SDCARD_FATAL("sdcard_open err\r\n");
			return SDCARD_FAILURE;
		}
	}

	return SDCARD_SUCCESS;
}

UINT32 sdcard_close(void)
{
	sdcard_uninitialize();
	return SDCARD_SUCCESS;
}

UINT32 sdcard_read(char *user_buf, UINT32 count, UINT32 op_flag)
{
	UINT32 result = SD_OK;
#if 1
	//SDCARD_LOGI("sd_read:buf = %x, count=%d,sector num=%d\r\n", user_buf, count, op_flag);
	result = sdcard_read_multi_block((uint8 *)user_buf, op_flag, count);
	if(result)
		SDCARD_LOGI("read err:%d\r\n", result);
	return result;
#else

	UINT32 start_blk_addr;
	UINT8  read_blk_numb, numb;
	UINT8 *read_data_buf;
	peri_busy_count_add();
	// check operate parameter
	start_blk_addr = op_flag;
	read_blk_numb = count;
	read_data_buf = (UINT8 *)user_buf;

	SDCARD_LOGI("sd_read:buf = %x, count=%d,op_flag=%d\r\n", user_buf, count, op_flag);



	{
		for (numb = 0; numb < read_blk_numb; numb++) {
			result = sdcard_read_single_block(read_data_buf, start_blk_addr,
											  SD_DEFAULT_BLOCK_SIZE);
			if (result != SD_OK) {
				SDCARD_LOGI("sdcard_read err:%d, curblk:0x%x\r\n", result, start_blk_addr);
				count = 0;
				goto exit;
			}

			start_blk_addr++;
			read_data_buf += SD_DEFAULT_BLOCK_SIZE;
		}
	}
exit:
	peri_busy_count_dec();
	return result;
#endif
}

UINT32 sdcard_write_new(int first_block, int block_num, uint8_t *data)
{
	return sdcard_write_multi_block(data, first_block, block_num);
}

UINT32 sdcard_read_new(int first_block, int block_num, uint8 *dest)
{
	return sdcard_read_multi_block(dest, first_block, block_num);
}

//op_flag:first start block ID.
UINT32 sdcard_write(char *user_buf, UINT32 count, UINT32 op_flag)
{
	SDIO_Error err = SD_OK;
	UINT32 start_blk_addr;

	if (s_sdcard_ps_suspend_cb) {
		s_sdcard_ps_suspend_cb();
	}
	// check operate parameter
	start_blk_addr = op_flag;
	err = sdcard_write_multi_block((UINT8 *)user_buf, start_blk_addr, count);
	if (s_sdcard_ps_resume_cb) {
		s_sdcard_ps_resume_cb();
	}
	return err;
}

UINT32 sdcard_ctrl(UINT32 cmd, void *parm)
{
	if (s_sdcard_ps_suspend_cb) {
		s_sdcard_ps_suspend_cb();
	}
	switch (cmd) {
		case 0:	//it's called in fatfs:disk_io.c
#if defined(CONFIG_SOC_BK7256XX)	//JIRA BK7256-1674
		if(no_need_send_cmd12_flag == 0)
		{
			sdcard_send_write_stop(0);
			no_need_send_cmd12_flag = 1;
		}
#else	//reserve previous codes.
		sdcard_cmd12_process(0);
#endif
			break;
	default:
		break;
	}
	if (s_sdcard_ps_resume_cb) {
		s_sdcard_ps_resume_cb();
	}

	return 0;
}


int sdcard_get_size(void)
{
	return sdcard.total_block;
}
int sdcard_get_block_size(void)
{
	return sdcard.block_size;
}
void clr_sd_noinitial_flag(void)
{
	NoneedInitflag = 0;
}

void sdcard_register_ps_suspend_callback(sdcard_ps_callback_t ps_suspend_cb)
{
	s_sdcard_ps_suspend_cb = ps_suspend_cb;
}

void sdcard_register_ps_resume_callback(sdcard_ps_callback_t ps_resume_cb)
{
	s_sdcard_ps_resume_cb = ps_resume_cb;
}

#endif  // CONFIG_SDCARD
// EOF
