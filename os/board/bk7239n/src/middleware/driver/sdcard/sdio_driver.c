#include <common/bk_include.h>
#include "bk_arm_arch.h"

#if defined(CONFIG_SDCARD)
#include "sdio_driver.h"
#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include <os/mem.h>
#include "bk_icu.h"
#include "bk_gpio.h"
#include <os/os.h>

#include "gpio_driver.h"
#include <driver/hal/hal_gpio_types.h>

#if defined(CONFIG_SOC_BK7256XX)
#include "sys_types.h"
#include "sys_driver.h"
#include <driver/gpio.h>
#endif

/******************************************************************************/
/**************************** platform function *******************************/
/******************************************************************************/

/******************************************************************************/
/**************************** interface function ******************************/
/******************************************************************************/
#if (defined(CONFIG_SOC_BK7256XX))
/* clk_index: includes clock src and clock divider info */
void sdio_set_clock(UINT8 clk_index)
{
	//temp code, will be switch to sdcard_driver.c
	*((volatile unsigned long *) (0x44010000+0x9*4)) = (((*((volatile unsigned long *) (0x44010000+0x9*4))) & (~0x3C000)) | ((clk_index) << 14));
}
#else
static void beken_sdcard_set_clk_div(UINT8 clkdiv)
{
	UINT32 reg;

	reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
	reg &= ~(SDCARD_FIFO_SD_RATE_SELECT_MASK << SDCARD_FIFO_SD_RATE_SELECT_POSI);
	reg |= ((clkdiv & SDCARD_FIFO_SD_RATE_SELECT_MASK)
			<< SDCARD_FIFO_SD_RATE_SELECT_POSI);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);
}

void sdio_set_clock(UINT8 clk_index)
{
	beken_sdcard_set_clk_div(clk_index);
}
#endif

#if (defined(CONFIG_SOC_BK7256XX))
#if (defined(CONFIG_PIN_SDIO_GROUP_0))
#define PIN_ID_SDIO_CLK (GPIO_14)
#define PIN_ID_SDIO_CMD (GPIO_15)
#define PIN_ID_SDIO_D0 (GPIO_16)
#define PIN_ID_SDIO_D1 (GPIO_17)
#define PIN_ID_SDIO_D2 (GPIO_18)
#define PIN_ID_SDIO_D3 (GPIO_19)
#else
#define PIN_ID_SDIO_CLK (GPIO_2)
#define PIN_ID_SDIO_CMD (GPIO_3)
#define PIN_ID_SDIO_D0 (GPIO_4)
#define PIN_ID_SDIO_D1 (GPIO_5)
#define PIN_ID_SDIO_D2 (GPIO_10)
#define PIN_ID_SDIO_D3 (GPIO_11)
#endif
void sdio_gpio_config(void)
{
	gpio_dev_unmap(PIN_ID_SDIO_CLK);
	gpio_dev_unmap(PIN_ID_SDIO_CMD);
	gpio_dev_unmap(PIN_ID_SDIO_D0);

#if defined(CONFIG_SDCARD_BUSWIDTH_4LINE)
	gpio_dev_unmap(PIN_ID_SDIO_D1);
	gpio_dev_unmap(PIN_ID_SDIO_D2);
	gpio_dev_unmap(PIN_ID_SDIO_D3);
#endif
	gpio_dev_map(PIN_ID_SDIO_CLK, GPIO_DEV_SDIO_HOST_CLK);
	bk_gpio_pull_up(PIN_ID_SDIO_CLK);
	bk_gpio_set_capacity(PIN_ID_SDIO_CLK, 3);

	gpio_dev_map(PIN_ID_SDIO_CMD, GPIO_DEV_SDIO_HOST_CMD);
	bk_gpio_pull_up(PIN_ID_SDIO_CMD);
	bk_gpio_set_capacity(PIN_ID_SDIO_CMD, 3);
	gpio_dev_map(PIN_ID_SDIO_D0, GPIO_DEV_SDIO_HOST_DATA0);
	bk_gpio_pull_up(PIN_ID_SDIO_D0);
	bk_gpio_set_capacity(PIN_ID_SDIO_D0, 3);

#if defined(CONFIG_SDCARD_BUSWIDTH_4LINE)
	gpio_dev_map(PIN_ID_SDIO_D1, GPIO_DEV_SDIO_HOST_DATA1);
	bk_gpio_pull_up(PIN_ID_SDIO_D1);
	bk_gpio_set_capacity(PIN_ID_SDIO_D1, 3);
	gpio_dev_map(PIN_ID_SDIO_D2, GPIO_DEV_SDIO_HOST_DATA2);
	bk_gpio_pull_up(PIN_ID_SDIO_D2);
	bk_gpio_set_capacity(PIN_ID_SDIO_D2, 3);
	gpio_dev_map(PIN_ID_SDIO_D3, GPIO_DEV_SDIO_HOST_DATA3);
	bk_gpio_pull_up(PIN_ID_SDIO_D3);
	bk_gpio_set_capacity(PIN_ID_SDIO_D3, 3);
#endif
}

#if defined(CONFIG_SOC_BK7256XX)
//1:clock always on, 0:auto gate
void sdio_clk_gate_config(uint8_t enable)
{
	uint32_t reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);

	//SDCARD_LOGI("%s:reg=0x%x, en=%d\r\n", __func__, reg, enable);
	if(enable)
		reg |= (1<<SDIO_REG0XD_CLK_GATE_ON_POS);
	else
		reg &= ~(1<<SDIO_REG0XD_CLK_GATE_ON_POS);

	//reset the fifo realte status
	if(!enable)	//auto gate
		reg &= ~(SDCARD_FIFO_RX_FIFO_RST | SDCARD_FIFO_TX_FIFO_RST | SDCARD_FIFO_SD_STA_RST);

	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);
}
#endif

void sdio_clk_config(UINT8 enable)
{
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SDIO, enable);
}

#else
void sdio_gpio_config(void)
{
	UINT32 param;
#if (defined(CONFIG_SOC_BK7251))
#if (CFG_SD_HOST_INTF == defined(CONFIG_SD1_HOST_INTF))
	param = GFUNC_MODE_SD1_HOST;
#else
	param = GFUNC_MODE_SD_HOST;
#endif

#else
#if (CFG_SD_HOST_INTF == defined(CONFIG_SD1_HOST_INTF))
	param = GFUNC_MODE_SD1_HOST;
#else
	param = GFUNC_MODE_SD_HOST;
#endif
#endif
	sddev_control(DD_DEV_TYPE_GPIO, CMD_GPIO_ENABLE_SECOND, &param);
}

void sdio_clk_config(UINT8 enable)
{
	UINT32 param;
	UINT32 cmd;

	if (enable)
		cmd = CMD_CLK_PWR_UP;
	else
		cmd = CMD_CLK_PWR_DOWN;

	param = PWD_SDIO_CLK_BIT;
	sddev_control(DD_DEV_TYPE_ICU, cmd, &param);
}
#endif


void sdio_register_reset(void)
{
	UINT32 reg;

	/* Clear cmd rsp int bit */
	reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, reg);

	/* Clear tx/rx fifo */
	reg = SDCARD_FIFO_RX_FIFO_RST | SDCARD_FIFO_TX_FIFO_RST;
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);

	/* Disabe all sdio interrupt */
	reg = 0;
	REG_WRITE(REG_SDCARD_CMD_RSP_INT_MASK, reg);

	/* Config tx/rx fifo threshold */
	reg = ((SDCARD_RX_FIFO_THRD & SDCARD_FIFO_RX_FIFO_THRESHOLD_MASK)
		   << SDCARD_FIFO_RX_FIFO_THRESHOLD_POSI)
		  | ((SDCARD_TX_FIFO_THRD & SDCARD_FIFO_TX_FIFO_THRESHOLD_MASK)
			 << SDCARD_FIFO_TX_FIFO_THRESHOLD_POSI);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);

#if (defined(CONFIG_SOC_BK7256XX))
	reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg | (1<<SDIO_REG0XD_CLK_REC_SEL_POS));
#endif
}

#if 0
void sdio_register_reenable(void)
{
	UINT32 reg;

	reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, reg);

	reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
	reg |= (SDCARD_FIFO_RX_FIFO_RST | SDCARD_FIFO_TX_FIFO_RST | SDCARD_FIFO_SD_STA_RST);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);

	rtos_delay_milliseconds(5);
	/*Config tx/rx fifo threshold*/
	reg = ((SDCARD_RX_FIFO_THRD & SDCARD_FIFO_RX_FIFO_THRESHOLD_MASK)
		   << SDCARD_FIFO_RX_FIFO_THRESHOLD_POSI)
		  | ((SDCARD_TX_FIFO_THRD & SDCARD_FIFO_TX_FIFO_THRESHOLD_MASK)
			 << SDCARD_FIFO_TX_FIFO_THRESHOLD_POSI);
	REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);
}
#endif

void sdio_sendcmd_function(UINT8 cmd_index, UINT32 flag,
						   UINT32 timeout, VOID *arg)
{
	UINT32 reg;
	flag &= CMD_FLAG_MASK;

	reg = (UINT32)arg;
	REG_WRITE(REG_SDCARD_CMD_SEND_AGUMENT, reg);

	reg = timeout;
	REG_WRITE(REG_SDCARD_CMD_RSP_TIMER, reg);

	reg = ((((UINT32)cmd_index)&SDCARD_CMD_SEND_CTRL_CMD_INDEX_MASK)
		   << SDCARD_CMD_SEND_CTRL_CMD_INDEX_POSI)
		  | ((flag & SDCARD_CMD_SEND_CTRL_CMD_FLAGS_MASK)
			 << SDCARD_CMD_SEND_CTRL_CMD_FLAGS_POSI)
		  | SDCARD_CMD_SEND_CTRL_CMD_START;
	REG_WRITE(REG_SDCARD_CMD_SEND_CTRL, reg);
}

SDIO_Error sdio_wait_cmd_response(UINT32 cmd)
{
	UINT32 reg;
#if (defined(CONFIG_SOC_BK7256XX))
	uint32_t i_timeout = 0x1000000;
#endif

	while (1) {
		reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
		//wait until cmd response
		if (reg & (SDCARD_CMDRSP_NORSP_END_INT
				   | SDCARD_CMDRSP_RSP_END_INT
				   | SDCARD_CMDRSP_TIMEOUT_INT))
			break;

#if (defined(CONFIG_SOC_BK7256XX))
		i_timeout--;
		if(i_timeout == 0)
		{
			SDCARD_LOGW("%s:cmd%d fatal err:no resp/end/timeout int\r\n", __func__, cmd);
			return SD_CMD_RSP_TIMEOUT;
		}
#endif
	}

	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, SD_CMD_RSP);//clear the int flag
	if ((reg & SDCARD_CMDRSP_TIMEOUT_INT) /*||(reg&SDCARD_CMDRSP_NORSP_END_INT)*/) {
		if ((cmd != 1))
			SDCARD_LOGW("sdcard cmd %d timeout,cmdresp_int_reg:0x%x\r\n", cmd, reg);
		return SD_CMD_RSP_TIMEOUT;
	}
	if (reg & SDCARD_CMDRSP_CMD_CRC_FAIL) {

		if ((cmd != 41) && (cmd != 2) && (cmd != 9) && (cmd != 1)) {
			SDCARD_LOGW("sdcard cmd %d crcfail,cmdresp_int_reg:0x%x\r\n", cmd, reg);
			return SD_CMD_CRC_FAIL;
		}
	}

	//SDCARD_LOGI("%s cmd=%d\r\n", __func__, cmd);

	return SD_OK;
}

void sdio_get_cmdresponse_argument(UINT8 num, UINT32 *resp)
{
	switch (num) {
	case 0:
		*resp = REG_READ(REG_SDCARD_CMD_RSP_AGUMENT0);
		break;
	case 1:
		*resp = REG_READ(REG_SDCARD_CMD_RSP_AGUMENT1);
		break;
	case 2:
		*resp = REG_READ(REG_SDCARD_CMD_RSP_AGUMENT2);
		break;
	case 3:
		*resp = REG_READ(REG_SDCARD_CMD_RSP_AGUMENT3);
		break;
	default:
		break;
	}
}

void sdio_setup_data(UINT32 data_dir, UINT32 byte_len)
{
	UINT32 reg;
	if (data_dir == SD_DATA_DIR_RD) {
		reg = REG_READ(REG_SDCARD_FIFO_THRESHOLD);
		reg &= (3 << 21);
		reg |= 0x3ffff; // set fifo
		REG_WRITE(REG_SDCARD_FIFO_THRESHOLD, reg);

		reg = SDCARD_DATA_REC_CTRL_DATA_EN;
	} else
		reg = SDCARD_DATA_REC_CTRL_DATA_WR_DATA_EN;

	reg |= SDCARD_DATA_REC_CTRL_DATA_BYTE_SEL
		   | ((byte_len & SDCARD_DATA_REC_CTRL_BLK_SIZE_MASK)
			  << SDCARD_DATA_REC_CTRL_BLK_SIZE_POSI)
#ifdef CONFIG_SDCARD_BUSWIDTH_4LINE
		   | SDCARD_DATA_REC_CTRL_DATA_BUS
#endif
		   ;

	REG_WRITE(REG_SDCARD_DATA_REC_CTRL, reg);
}

void sdio_set_data_timeout(UINT32 timeout)
{
	REG_WRITE(REG_SDCARD_DATA_REC_TIMER, timeout);
}

void driver_sdcard_recv_data_start(int timeout)
{
	REG_WRITE(REG_SDCARD_DATA_REC_TIMER, timeout);
#ifdef CONFIG_SDCARD_BUSWIDTH_4LINE
	REG_WRITE(REG_SDCARD_DATA_REC_CTRL, (0x1 | (1 << 2) | (512 << 4) | (1 << 17)));
#else
	REG_WRITE(REG_SDCARD_DATA_REC_CTRL, (0x1 | (512 << 4) | (1 << 17)));
#endif
}

SDIO_Error sdcard_wait_receive_data(UINT8 *receive_buf)
{
	#define WAIT_MAX_CNT (0x1000000)	//Temp code
	UINT32 reg, i = 0;

	while (1) {
		reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
		if (reg & (SDCARD_CMDRSP_DATA_REC_END_INT
				   | SDCARD_CMDRSP_DATA_CRC_FAIL
				   | SDCARD_CMDRSP_DATA_TIME_OUT_INT))
			break;

		//Temp code
		i++;
		if((i >= WAIT_MAX_CNT) && (i % 0x100000 == 0))
			SDCARD_LOGI("%s:i=%x,timeout\r\n", __func__, i);
	}

	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, SD_DATA_RSP);//clear the int flag
	if (reg & SDCARD_CMDRSP_DATA_TIME_OUT_INT)
		return SD_DATA_TIMEOUT;
	if (reg & SDCARD_CMDRSP_DATA_CRC_FAIL) {
		SDCARD_LOGW("sdcard data crcfail,cmdresp_int_reg:0x%x\r\n", reg);
		return SD_DATA_CRC_FAIL;
	}

	for (i = 0; i < SD_DEFAULT_BLOCK_SIZE ;) {
		/* wait fifo data valid */
		while (1) {
			//software  needn't handle dead-loop,hardware can garantee
			if (REG_READ(REG_SDCARD_FIFO_THRESHOLD)&SDCARD_FIFO_RXFIFO_RD_READY)
				break;
		}
		reg = REG_READ(REG_SDCARD_RD_DATA_ADDR);
		*(receive_buf + i++) = reg & 0xff;
		*(receive_buf + i++) = (reg >> 8) & 0xff;
		*(receive_buf + i++) = (reg >> 16) & 0xff;
		*(receive_buf + i++) = (reg >> 24) & 0xff;
	}

	return SD_OK;
}

#if 0
SDIO_Error sdcard_write_data(UINT8 *writebuff, UINT32 block)
{
	UINT32 i, j, reg, tmpval;

	i = 0;
	// 1. fill the first block to fifo and start write data enable
	while (REG_READ(REG_SDCARD_FIFO_THRESHOLD) & SDCARD_FIFO_TXFIFO_WR_READY) {
		tmpval = (writebuff[i] << 24) | (writebuff[i + 1] << 16) | (writebuff[i + 2] << 8) | writebuff[i + 3];
		REG_WRITE(REG_SDCARD_WR_DATA_ADDR, tmpval);
		i += 4;
		if (SD_DEFAULT_BLOCK_SIZE <= i)
			break;
	}

	reg = REG_READ(REG_SDCARD_CMD_RSP_INT_MASK);
	reg |= SDCARD_CMDRSP_TX_FIFO_EMPTY_MASK;
	REG_WRITE(REG_SDCARD_CMD_RSP_INT_MASK, reg);

	REG_WRITE(REG_SDCARD_DATA_REC_TIMER, DEF_HIGH_SPEED_DATA_TIMEOUT * block);
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
	while (--block) {
		j = 0;
		while (j < SD_DEFAULT_BLOCK_SIZE) {
			if (REG_READ(REG_SDCARD_FIFO_THRESHOLD) & SDCARD_FIFO_TXFIFO_WR_READY) {
				tmpval = (writebuff[i] << 24) | (writebuff[i + 1] << 16) | (writebuff[i + 2] << 8) | writebuff[i + 3];
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

		if (2 != ((reg & SDCARD_CMDRSP_WR_STATU) >> 20))
			return SD_ERROR;
	}

	// 3. after the last block,write zero
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
	} while (!(reg & SDCARD_CMDRSP_DATA_WR_END_INT));
	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, SDCARD_CMDRSP_DATA_WR_END_INT);

	if (2 != ((reg & SDCARD_CMDRSP_WR_STATU) >> 20))
		return SD_ERROR;

	return SD_OK;
}

SDIO_Error sdcard_wait_write_end(void)
{
	UINT32 reg;
	while (1) {
		reg = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
		if (reg & (SDCARD_CMDRSP_DATA_WR_END_INT
				   | SDCARD_CMDRSP_DATA_CRC_FAIL
				   | SDCARD_CMDRSP_DATA_TIME_OUT_INT))
			break;
	}

	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, SD_DATA_RSP);//clear the int flag

	if (reg & SDCARD_CMDRSP_DATA_TIME_OUT_INT)
		return SD_DATA_TIMEOUT;
	if (reg & SDCARD_CMDRSP_DATA_CRC_FAIL) {
		SDCARD_LOGW("sdcard write data crcfail,cmdresp_int_reg:0x%x\r\n", reg);
		return SD_DATA_CRC_FAIL;
	}

	return SD_OK;
}

#endif
int wait_Receive_Data(void)
{
	uint32 ret = SD_ERR_LONG_TIME_NO_RESPONS, status = 0;
	uint32 start_tm = rtos_get_time();
	while (1) {
		if (rtos_get_time() > start_tm + 4000) { // 4s
			ret = SD_ERR_LONG_TIME_NO_RESPONS;
			break;
		}

		status = REG_READ(REG_SDCARD_CMD_RSP_INT_SEL);
		if (status & SDCARD_CMDRSP_DATA_REC_END_INT) {
			if (status & SDCARD_CMDRSP_DATA_CRC_FAIL) {
				SDCARD_LOGI("aaa\r\n");
				//ret = SD_DATA_CRC_FAIL;
				ret = SD_OK;
			} else
				ret = SD_OK;
			break;
		} else if (status & SDCARD_CMDRSP_DATA_CRC_FAIL) {
			ret = SD_DATA_CRC_FAIL;
			break;
		} else if (status & SDCARD_CMDRSP_DATA_TIME_OUT_INT) {
			ret = SD_DATA_TIMEOUT;
			break;
		}
	}
	REG_WRITE(REG_SDCARD_CMD_RSP_INT_SEL, SD_DATA_RSP);/*< clear the int flag */
	return ret;
}

#endif // CONFIG_SDCARD
// EOF

