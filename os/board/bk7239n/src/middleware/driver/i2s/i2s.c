#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include <common/bk_typedef.h>
#include "i2s.h"
#include "bk_i2s.h"
#include "bk_gpio.h"
#include "bk_drv_model.h"
#include <driver/int.h>
#include "bk_icu.h"
#include "bk_music_msg.h"
#include "bk_driver_audio.h"
#include "bk_uart.h"
#include <common/sys_config.h>
#include <common/bk_kernel_err.h>
#include <os/os.h>
#include <os/mem.h>
#include "bk_misc.h"
#include "bk_sys_ctrl.h"
#include "sys_ctrl.h"
#include "sys_driver.h"
#include <common/bk_generic.h>

#if defined(CONFIG_I2S)

#define RT_I2S_BIT_DEBUG
#ifdef  RT_I2S_BIT_DEBUG
#define bit_dbg(fmt, ...)   BK_LOG_RAW(fmt, ##__VA_ARGS__)
#else
#define bit_dbg(fmt, ...)
#endif

struct bk_i2s_dev {
	UINT8 *tx_ptr;
	UINT32 tx_len;
	beken_semaphore_t tx_sem;

	UINT8 *rx_ptr;
	UINT32 rx_len;
	UINT32 rx_offset;
	UINT32 rx_drop;

	UINT32 total_len;
	UINT32 flag;

	beken_mutex_t mutex;
};

volatile i2s_trans_t i2s_trans;
i2s_level_t  i2s_fifo_level;

#if (!defined(CONFIG_SOC_BK7271))
static UINT32 i2s_ctrl(UINT32 cmd, void *param);

static const DD_OPERATIONS i2s_op = {
	NULL,
	NULL,
	NULL,
	NULL,
	i2s_ctrl
};

static void i2s_active(UINT8 enable)
{
	UINT32 value_ctrl;

	value_ctrl = REG_READ(PCM_CTRL);

	if (enable)
		value_ctrl |= I2S_PCM_EN;
	else
		value_ctrl &= ~I2S_PCM_EN;

	REG_WRITE(PCM_CTRL, value_ctrl);
}

static void i2s_set_msten(UINT8 enable)
{
	UINT32 value_ctrl;

	value_ctrl = REG_READ(PCM_CTRL);

	if (enable)
		value_ctrl |= MSTEN;
	else
		value_ctrl &= (~MSTEN);

	REG_WRITE(PCM_CTRL, value_ctrl);
}

static void i2s_select_mode(UINT8 val)
{
	UINT32 value;

	if (val == 3 || val > 7)
		return;

	value = REG_READ(PCM_CTRL);
	value &= ~(MODE_SEL_MASK << MODE_SEL_POSI);
	value |= (val << MODE_SEL_POSI);
	REG_WRITE(PCM_CTRL, value);
}

static void i2s_set_lrck(UINT8 val)
{
	UINT32 value;

	value = REG_READ(PCM_CTRL);
	if (val)
		value |= LRCK_RP;
	else
		value &= ~LRCK_RP;
	REG_WRITE(PCM_CTRL, value);
}

static void i2s_set_sck_inv(UINT8 val)
{
	UINT32 value;

	value = REG_READ(PCM_CTRL);

	if (val)
		value |= SCK_INV;
	else
		value &= ~SCK_INV;

	REG_WRITE(PCM_CTRL, value);
}

static void i2s_set_sck_lsb(UINT8 val)
{
	UINT32 value;

	value = REG_READ(PCM_CTRL);
	if (val)
		value |= LSB_FIRST;
	else
		value &= ~LSB_FIRST;
	REG_WRITE(PCM_CTRL, value);
}

static void i2s_set_sck_synclen(UINT8 val)
{
	UINT32 value;

	value = REG_READ(PCM_CTRL);
	value &= ~ SYNCLEN_MASK;
	value |= (val << SYNCLEN_POSI);
	REG_WRITE(PCM_CTRL, value);
}

static void i2s_set_pcm_dlen(UINT8 val)
{
	UINT32 value;

	value = REG_READ(PCM_CTRL);
	value &= ~ PCM_DLEN_MASK;
	value |= (val << PCM_DLEN_POSI);
	REG_WRITE(PCM_CTRL, value);
}

static void i2s_set_freq_datawidth(i2s_rate_t *p_rate)
{
	UINT32 bitratio, value = 0, lrck_div, sys_clk = 0;

	if ((p_rate->freq != 8000) && (p_rate->freq != 16000) &&
		(p_rate->freq != 24000) && (p_rate->freq != 32000) && (p_rate->freq != 48000) &&
		(p_rate->freq != 11025) && (p_rate->freq != 22050) && (p_rate->freq != 44100))
		return;

	/*set irck div*/
	if (p_rate->datawidth == 8)
		lrck_div = 7;
	else if (p_rate->datawidth == 16)
		lrck_div = 15;
	else if (p_rate->datawidth == 24)
		lrck_div = 23;
	else
		lrck_div = 31;

	/*set system  clock*/
	if (p_rate->freq == 8000) {
		if (p_rate->datawidth == 24) {
			sys_clk = 48384000;
			sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_AUDIO_PLL, &sys_clk);
		} else {
			sys_clk = 48128000;
			sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_AUDIO_PLL, &sys_clk);
		}
	} else if (p_rate->freq == 16000) {
		if ((p_rate->datawidth == 16) || (p_rate->datawidth == 8)) {
			sys_clk = 48128000;
			sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_AUDIO_PLL, &sys_clk);
		} else {
			sys_clk = 49152000;
			sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_AUDIO_PLL, &sys_clk);
		}
	} else if (p_rate->freq == 44100) {
		sys_clk = 50803200;
		sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_AUDIO_PLL, &sys_clk);
	} else {
		if (p_rate->datawidth == 24) {
			sys_clk = 50688000;
			sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_AUDIO_PLL, &sys_clk);
		} else {
			sys_clk = 49152000;
			sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_AUDIO_PLL, &sys_clk);
		}
	}

	/*set bit clock divd*/
	bitratio = MAX((NUMBER_ROUND_UP((sys_clk / 2), (p_rate->freq  * 2 * (lrck_div + 1))) - 1), 5);
	value = value 	| ((p_rate->datawidth - 1) << DATALEN_POSI)
			| (lrck_div << SMPRATIO_POSI)
			| (bitratio << BITRATIO_POSI);//this value is unused in slave mode

	REG_WRITE(PCM_CTRL, value);
}

static void i2s_rxint_enable(UINT8 val)
{
	UINT32 value;

	value = REG_READ(PCM_CN);
	if (val)
		value |= RX_INT_EN;
	else
		value &= ~RX_INT_EN;
	REG_WRITE(PCM_CN, value);
}

static void i2s_txint_enable(UINT8 val)
{
	UINT32 value;

	value = REG_READ(PCM_CN);
	if (val)
		value |= TX_INT0_EN;
	else
		value &= ~TX_INT0_EN;
	REG_WRITE(PCM_CN, value);
}

static void i2s_rxovr_enable(UINT8 val)
{
	UINT32 value;

	value = REG_READ(PCM_CN);
	if (val)
		value |= RX_OVF_EN;
	else
		value &= ~RX_OVF_EN;
	REG_WRITE(PCM_CN, value);
}

static void i2s_txovr_enable(UINT8 val)
{
	UINT32 value;

	value = REG_READ(PCM_CN);
	if (val)
		value |= TX_UDF0_EN;
	else
		value &= ~TX_UDF0_EN;
	REG_WRITE(PCM_CN, value);
}


static void i2s_rxint_mode(UINT8 val)
{
	UINT32 value;

	value = REG_READ(PCM_CN);

	value &= ~(RX_FIFO_LEVEL_MASK << RX_FIFO_LEVEL_POSI) ;
	value |= ((val & RX_FIFO_LEVEL_MASK) << RX_FIFO_LEVEL_POSI);

	REG_WRITE(PCM_CN, value);
}

static void i2s_txint_mode(UINT8 val)
{
	UINT32 value;

	value = REG_READ(PCM_CN);

	value &= ~(RX_FIFO_LEVEL_MASK << TX_FIFO0_LEVEL_POSI) ;
	value |= ((val & RX_FIFO_LEVEL_MASK) << TX_FIFO0_LEVEL_POSI);

	REG_WRITE(PCM_CN, value);
}

static void i2s_rxfifo_clr_enable(void)
{
	UINT32 value;

	value = REG_READ(PCM_CN);
	value |= RX_FIFO_CLR;

	REG_WRITE(PCM_CN, value);
}

static void i2s_txfifo_clr_enable(void)
{
	UINT32 value;

	value = REG_READ(PCM_CN);
	value = value | TX_FIFO0_CLR | TX_FIFO1_CLR | TX_FIFO2_CLR;

	REG_WRITE(PCM_CN, value);
}

static void i2s_icu_configuration(UINT32 enable)
{
	UINT32 param;

	if (enable) {
		param = PWD_I2S_PCM_CLK_BIT;
		sddev_control(DD_DEV_TYPE_ICU, CMD_CLK_PWR_UP, &param);

		param = (IRQ_I2S_PCM_BIT);
		// sddev_control(DD_DEV_TYPE_ICU, CMD_ICU_INT_ENABLE, &param);
		(void)sys_drv_int_enable(param);
	} else {
		param = (IRQ_I2S_PCM_BIT);
		// sddev_control(DD_DEV_TYPE_ICU, CMD_ICU_INT_DISABLE, &param);
		(void)sys_drv_int_disable(param);

		param = PWD_I2S_PCM_CLK_BIT;
		sddev_control(DD_DEV_TYPE_ICU, CMD_CLK_PWR_DOWN, &param);
	}
}

static void i2s_gpio_configuration()
{
	uint32_t val;
	val = GFUNC_MODE_I2S;				/*gpio 2-5*/
	sddev_control(DD_DEV_TYPE_GPIO, CMD_GPIO_ENABLE_SECOND, &val);
}

static UINT8 i2s_get_busy(void)
{
	//TODO
	return 0;
}

static void i2s_master_enable(UINT32 enable)
{
	UINT32 value, ultemp;

	ultemp = 1;

	value = REG_READ(PCM_CN);
	value = value | (RX_INT_EN | TX_INT0_EN);
	REG_WRITE(PCM_CN, value);

	/* enable i2s unit */
	i2s_ctrl(I2S_CMD_UNIT_ENABLE, (void *) &ultemp);

	/* 1:MASTER   0:SLAVE */
	if (enable)
		i2s_ctrl(I2S_CMD_SET_MSTEN, (void *) &enable);
	else
		i2s_ctrl(I2S_CMD_SET_MSTEN, (void *)& enable);

	bit_dbg("[-ISR-]I2S_DEBUG: pcm_ctrl=0x%X,pcm_cn =0x%08X,pcm_stat =0x%X\r\n", REG_READ(PCM_CTRL), REG_READ(PCM_CN), REG_READ(PCM_STAT));

	i2s_icu_configuration(1);  //enable clock;
}

static void i2s_dma_master_enable(UINT32 enable)
{
	UINT32 value, ultemp;

	ultemp = 1;

	value = REG_READ(PCM_CN);
	value = value | (RX_INT_EN | TX_INT0_EN);
	REG_WRITE(PCM_CN, value);

	/* enable i2s unit */
	i2s_ctrl(I2S_CMD_UNIT_ENABLE, (void *) &ultemp);

	/* 1:MASTER   0:SLAVE */
	if (enable)
		i2s_ctrl(I2S_CMD_SET_MSTEN, (void *) &enable);
	else
		i2s_ctrl(I2S_CMD_SET_MSTEN, (void *)& enable);

	bit_dbg("[-DMA-]I2S_DEBUG: pcm_ctrl=0x%X,pcm_cn =0x%08X,pcm_stat =0x%X\r\n", REG_READ(PCM_CTRL), REG_READ(PCM_CN), REG_READ(PCM_STAT));

	{
		UINT32 param = PWD_I2S_PCM_CLK_BIT;
		sddev_control(DD_DEV_TYPE_ICU, CMD_CLK_PWR_UP, &param);
	}
}

static UINT8 i2s_disable_i2s(void)
{
	UINT8  param;
	UINT32 status;

	param = 0;
	i2s_ctrl(I2S_CMD_UNIT_ENABLE, (void *)&param);

	status = REG_READ(PCM_STAT);

	REG_WRITE(PCM_CTRL, 0);
	REG_WRITE(PCM_CN, 0);
	REG_WRITE(PCM_STAT, status);

	i2s_icu_configuration(0);  //disable clock;
	return 0;
}

__maybe_unused static UINT32 i2s_read_rxfifo(UINT8 *data);
static UINT32 i2s_read_rxfifo(UINT8 *data)
{
	UINT32 value;

	value = REG_READ(PCM_STAT);

	if ((value & RX_FIFO0_EMPTY) == 0) {
		value = REG_READ(PCM_STAT);
		if (data)
			*data = value;
		return 1;
	}
	return 0;
}

__maybe_unused static void i2s_txfifo_fill(void);
static void i2s_txfifo_fill(void)
{
	UINT32 value;

	value = REG_READ(PCM_STAT);

	while ((value & TX_FIFO0_FULL) == 0)
		REG_WRITE(PCM_STAT, 0xff);
}

UINT32 i2s_write_txfifo(UINT8 data)
{
	UINT32 value;

	value = REG_READ(PCM_STAT);

	if ((value & TX_FIFO0_FULL) == 0) {
		REG_WRITE(PCM_STAT, data);
		return 1;
	}

	return 0;
}



void i2s_init(int register_isr)
{
	if (register_isr)
		bk_int_isr_register(INT_SRC_I2S, i2s_isr, NULL);
	sddev_register_dev(DD_DEV_TYPE_I2S, (DD_OPERATIONS *)&i2s_op);
}


void i2s_exit(void)
{
	sddev_unregister_dev(DD_DEV_TYPE_I2S);
}

static void i2s_enable_interrupt(void)
{
	UINT32 param;
	param = (IRQ_I2S_PCM_BIT);
	// sddev_control(DD_DEV_TYPE_ICU, CMD_ICU_INT_ENABLE, &param);
	(void)sys_drv_int_enable(param);
}

static void i2s_disable_interrupt(void)
{
	UINT32 param;
	param = (IRQ_I2S_PCM_BIT);
	// sddev_control(DD_DEV_TYPE_ICU, CMD_ICU_INT_DISABLE, &param);
	(void)sys_drv_int_disable(param);
}



static UINT32 i2s_ctrl(UINT32 cmd, void *param)
{
	UINT8 ret = I2S_SUCCESS;

	//peri_busy_count_add();


	switch (cmd) {
	case I2S_CMD_UNIT_ENABLE:
		i2s_active(*(UINT8 *)param);
		break;
	case I2S_CMD_SET_MSTEN:
		i2s_set_msten(*(UINT8 *)param);
		break;
	case I2S_CMD_SELECT_MODE:
		i2s_select_mode(*(UINT8 *)param);
		break;
	case I2S_CMD_SET_LRCK:
		i2s_set_lrck(*(UINT8 *)param);
		break;
	case I2S_CMD_SET_SCK_INV:
		i2s_set_sck_inv(*(UINT8 *)param);
		break;
	case I2S_CMD_SET_SCK_LSB:
		i2s_set_sck_lsb(*(UINT8 *)param);
		break;
	case I2S_CMD_SET_SCK_SYNCLEN:
		i2s_set_sck_synclen(*(UINT8 *)param);
		break;
	case I2S_CMD_SET_PCM_DLEN:
		i2s_set_pcm_dlen(*(UINT8 *)param);
		break;
	case I2S_CMD_SET_FREQ_DATAWIDTH:
		i2s_set_freq_datawidth((i2s_rate_t *)param);
		break;
	case I2S_CMD_RXINT_EN:
		i2s_rxint_enable(*(UINT8 *)param);
		break;
	case I2S_CMD_TXINT_EN:
		i2s_txint_enable(*(UINT8 *)param);
		break;
	case I2S_CMD_RXOVR_EN:
		i2s_rxovr_enable(*(UINT8 *)param);
		break;
	case I2S_CMD_TXOVR_EN:
		i2s_txovr_enable(*(UINT8 *)param);
		break;
	case I2S_CMD_RXFIFO_CLR_EN:
		i2s_rxfifo_clr_enable();
		break;
	case I2S_CMD_TXFIFO_CLR_EN:
		i2s_txfifo_clr_enable();
		break;
	case I2S_CMD_RXINT_MODE:
		i2s_rxint_mode(*(UINT8 *)param);
		break;
	case I2S_CMD_TXINT_MODE:
		i2s_txint_mode(*(UINT8 *)param);
		break;
	case I2S_CMD_GET_BUSY:
		i2s_get_busy();
		break;
	case I2S_CMD_ENABLE_INTERRUPT:
		i2s_enable_interrupt();
		break;
	case I2S_CMD_DISABLE_INTERRUPT:
		i2s_disable_interrupt();
		break;
	case I2S_CMD_MASTER_ENABLE:
		i2s_master_enable(*(UINT32 *)param);
		break;
	case I2S_CMD_DISABLE_I2S:
		i2s_disable_i2s();
		break;
	case I2S_CMD_DMA_MASTER_ENABLE:
		i2s_dma_master_enable(*(UINT32 *)param);
		break;
	case I2S_CMD_DMA_ISR:
		bk_int_isr_register(INT_SRC_I2S, (FUNCPTR)param, NULL);
		break;

	default:
		ret = I2S_FAILURE;
		break;
	}

	// peri_busy_count_dec();

	return ret;
}


UINT32 i2s_configure(UINT32 fifo_level, UINT32 sample_rate, UINT32 bits_per_sample, UINT32 mode)
{
	UINT32 param;
	i2s_rate_t rate;
	rate.datawidth = bits_per_sample;
	rate.freq = sample_rate;
	/*
		mode:
		bit0~2: 	mode & 0x7  	000:I2S
							 		001:Left Justified
							 		010:Right Justified
							 		011:reserve
							 		100:Short Frame Sync
							 		101:Long Frame Sync
							 		110:Normal 2B+D
							 		111:Delay 2B+D

		bit3: 		mode & 0x08  	0:LRCK	no turn
									1:LRCK	turn

		bit4: 		mode & 0x10  	0:SCK	no turn
									1:SCK	turn

		bit5: 		mode & 0x20  	0:MSB	first send/receive
									1:LSB	first send/receive

		bit8~10:	mode & 0x700 	0~7:Sync length only Long Frame Sync effective

		bit12~14:	mode & 0x7000 	0~7: 2B+D PCM :D length

	*/
	/* set work mode */
	param = (mode & 0x7) ;
	i2s_ctrl(I2S_CMD_SELECT_MODE, (void *)&param);	// i2s mode set

	/* set lrckrp */
	param = (mode & 0x8);
	i2s_ctrl(I2S_CMD_SET_LRCK, (void *)&param);

	/* set sclkinv */
	param = (mode & 0x10);
	i2s_ctrl(I2S_CMD_SET_SCK_INV, (void *)&param);

	/* set lsbfirst */
	param = (mode & 0x20);
	i2s_ctrl(I2S_CMD_SET_SCK_LSB, (void *)&param);

	/* set synclen */
	param = (mode & 0x700);
	param = param >> 8;
	i2s_ctrl(I2S_CMD_SET_SCK_SYNCLEN, (void *)&param);

	/* set pcm_dlen */
	param = (mode & 0x7000);
	param = param >> 12;
	i2s_ctrl(I2S_CMD_SET_PCM_DLEN, (void *)&param);

	/* set txfifo level */
	param = fifo_level;
	i2s_ctrl(I2S_CMD_TXINT_MODE, (void *)&param);

	/* set rxfifo level */
	param = fifo_level;
	i2s_ctrl(I2S_CMD_RXINT_MODE, (void *)&param);

	/* enable txover int */
	param = 1;
	i2s_ctrl(I2S_CMD_TXOVR_EN, (void *)&param);

	/* enable rxover int */
	param = 1;
	i2s_ctrl(I2S_CMD_RXOVR_EN, (void *)&param);

	i2s_gpio_configuration();  //set gpio

	param = REG_READ(SCTRL_BLOCK_EN_CFG);
	param = (param & (~(0x0FFFUL << 20))) | (BLOCK_EN_WORD_PWD << 20) | (1 << 16) | (1 << 17);
	REG_WRITE(SCTRL_BLOCK_EN_CFG, param);    // audio pll audio enable

#if (defined(CONFIG_SOC_BK7256XX))
    //sys_drv_analog_reg6_set(param);// to do,need remove old interface after all adaption is finished
    sys_drv_set_dpll_for_i2s();
#else
    sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_SET_ANALOG6, NULL);//  DPLL AUDIO OPEN, DPLL divider
#endif

	/* set freq_datawidth */
	i2s_ctrl(I2S_CMD_SET_FREQ_DATAWIDTH, (void *)&rate);

	/*	clear state	*/
	REG_WRITE(PCM_STAT, 0x0000FFFF);

	bit_dbg("[---]I2S_CONF:PCM_CTRL = 0x%x\n", REG_READ(PCM_CTRL));
	bit_dbg("[---]I2S_CONF:PCM_CN   = 0x%x\n", REG_READ(PCM_CN));

	return I2S_SUCCESS;
}

UINT32 i2s_close(void)
{
	UINT32 param = 0;

	i2s_icu_configuration(0);  //close clock;

	i2s_ctrl(I2S_CMD_DISABLE_INTERRUPT, (void *)&param);
	i2s_ctrl(I2S_CMD_UNIT_ENABLE, (void *)&param);


	return I2S_SUCCESS;
}


UINT32 i2s_transfer(UINT32 *i2s_send_buf, UINT32 *i2s_recv_buf, UINT32 count, UINT32 param)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	i2s_trans.trans_done = 0;
	i2s_trans.tx_remain_data_cnt = count;
	i2s_trans.rx_remain_data_cnt = count;
	i2s_trans.p_tx_buf = (UINT32 *) i2s_send_buf;
	i2s_trans.p_rx_buf = (UINT32 *) i2s_recv_buf;
	i2s_fifo_level.tx_level = FIFO_LEVEL_32;
	i2s_fifo_level.rx_level = FIFO_LEVEL_32;
	GLOBAL_INT_RESTORE();

	if (param)
		delay_ms(1000);

	/* rxfifo clear enable*/
	//i2s_ctrl(I2S_CMD_RXFIFO_CLR_EN, NULL);

	/* enable */
	i2s_ctrl(I2S_CMD_MASTER_ENABLE, (void *)&param);

	while (!i2s_trans.trans_done) {
	}

	delay_ms(1000);
	i2s_trans.trans_done = 0;

	i2s_ctrl(I2S_CMD_DISABLE_I2S, NULL);

	return i2s_trans.trans_done;
}

void i2s_isr(void)
{
	uint16_t i, rxint, txint0, ultemp;
	uint32_t i2s_status;
	volatile uint16_t data_num ;

	i2s_status = REG_READ(PCM_STAT);
	rxint  = i2s_status & 0x01;
	txint0 = i2s_status & 0x02;

	if (txint0) {
		switch (i2s_fifo_level.tx_level) {
		case 0	:
			data_num = 8;
			break;
		case 1	:
			data_num = 16;
			break;
		case 2	:
			data_num = 32;
			break;
		default :
			data_num = 48;
			break;
		}

		if (i2s_trans.p_tx_buf == NULL) {
			for (i = 0; i < data_num; i++)
				REG_WRITE(PCM_DAT0, 0xEEEEEEEE);
		} else {
			for (i = 0; i < data_num; i ++) {
				REG_WRITE(PCM_DAT0, *i2s_trans.p_tx_buf);
				i2s_trans.p_tx_buf ++;
			}
		}
		i2s_status |= 0x2;
	}

	if (rxint) {
		switch (i2s_fifo_level.rx_level) {
		case 0	:
			data_num = 8;
			break;
		case 1	:
			data_num = 16;
			break;
		case 2	:
			data_num = 32;
			break;
		default :
			data_num = 48;
			break;
		}

		if (data_num > i2s_trans.rx_remain_data_cnt)
			data_num = i2s_trans.rx_remain_data_cnt;

		if ((i2s_trans.p_rx_buf == NULL) || (i2s_status & RX_FIFO0_EMPTY)) {
			for (i = 0; i < data_num; i++) {
				ultemp = REG_READ(PCM_DAT0);
				(void) ultemp;
			}
		} else {
			for (i = 0; i < data_num; i++) {
				*i2s_trans.p_rx_buf = REG_READ(PCM_DAT0);
				i2s_trans.p_rx_buf++;
			}

			i2s_trans.rx_remain_data_cnt -= data_num;

			if (i2s_trans.rx_remain_data_cnt <= 0) {
				i = 0;
				i2s_trans.trans_done = 1;
				i2s_ctrl(I2S_CMD_TXINT_EN, (void *)&i);
				i2s_ctrl(I2S_CMD_RXINT_EN, (void *)&i);
			}
		}
		i2s_status |= 0x1;
	}

	REG_WRITE(PCM_STAT, i2s_status);

}
#endif
#endif
// eof

