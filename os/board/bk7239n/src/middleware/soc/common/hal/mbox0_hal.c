#include <stddef.h>
#include "mbox0_hal.h"

static int mbox0_hal_chn0_cfg_fifo(mbox0_hal_t* hal, uint8_t start, uint8_t len)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	if((len > hal->fifo_size) || ((start + len) > hal->fifo_size))
		return MBOX0_HAL_SW_FIFO_OVERFLOW;

	mbox0_ll_set_reg_0x10_fifo_start(hw, start);
	mbox0_ll_set_reg_0x11_fifo_length(hw, len);
	mbox0_ll_set_reg_0x11_chn_enable(hw, 1);

	return MBOX0_HAL_OK;
}

static int mbox0_hal_chn1_cfg_fifo(mbox0_hal_t* hal, uint8_t start, uint8_t len)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	if((len > hal->fifo_size) || ((start + len) > hal->fifo_size))
		return MBOX0_HAL_SW_FIFO_OVERFLOW;

	mbox0_ll_set_reg_0x20_fifo_start(hw, start);
	mbox0_ll_set_reg_0x21_fifo_length(hw, len);
	mbox0_ll_set_reg_0x21_chn_enable(hw, 1);

	return MBOX0_HAL_OK;
}

static int mbox0_hal_chn2_cfg_fifo(mbox0_hal_t* hal, uint8_t start, uint8_t len)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	if((len > hal->fifo_size) || ((start + len) > hal->fifo_size))
		return MBOX0_HAL_SW_FIFO_OVERFLOW;

	mbox0_ll_set_reg_0x30_fifo_start(hw, start);
	mbox0_ll_set_reg_0x31_fifo_length(hw, len);
	mbox0_ll_set_reg_0x31_chn_enable(hw, 1);

	return MBOX0_HAL_OK;
}

static int mbox0_hal_chn0_send(mbox0_hal_t* hal, mbox0_message_t * msg)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	mbox0_ll_set_reg_0x12_mail_tdata0(hw, msg->data[0]);
	mbox0_ll_set_reg_0x13_mail_tdata1(hw, msg->data[1]);
	mbox0_ll_set_reg_0x14_mail_tid(hw, msg->dest_cpu);
	
	return MBOX0_HAL_OK;
}

static int mbox0_hal_chn1_send(mbox0_hal_t* hal, mbox0_message_t * msg)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	mbox0_ll_set_reg_0x22_mail_tdata0(hw, msg->data[0]);
	mbox0_ll_set_reg_0x23_mail_tdata1(hw, msg->data[1]);
	mbox0_ll_set_reg_0x24_mail_tid(hw, msg->dest_cpu);
	
	return MBOX0_HAL_OK;
}

static int mbox0_hal_chn2_send(mbox0_hal_t* hal, mbox0_message_t * msg)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	mbox0_ll_set_reg_0x32_mail_tdata0(hw, msg->data[0]);
	mbox0_ll_set_reg_0x33_mail_tdata1(hw, msg->data[1]);
	mbox0_ll_set_reg_0x34_mail_tid(hw, msg->dest_cpu);
	
	return MBOX0_HAL_OK;
}

static int mbox0_hal_chn0_recv(mbox0_hal_t* hal, mbox0_message_t * msg)
{
	mbox0_hw_t * hw = hal->hw;

	msg->src_cpu = -1;
	msg->data[0] = -1;
	msg->data[1] = -1;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	msg->src_cpu = mbox0_ll_get_reg_0x15_mail_sid(hw);
	msg->data[0] = mbox0_ll_get_reg_0x16_mail_rdata0(hw);
	msg->data[1] = mbox0_ll_get_reg_0x17_mail_rdata1(hw);

	return MBOX0_HAL_OK;
}

static int mbox0_hal_chn1_recv(mbox0_hal_t* hal, mbox0_message_t * msg)
{
	mbox0_hw_t * hw = hal->hw;

	msg->src_cpu = -1;
	msg->data[0] = -1;
	msg->data[1] = -1;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	msg->src_cpu = mbox0_ll_get_reg_0x25_mail_sid(hw);
	msg->data[0] = mbox0_ll_get_reg_0x26_mail_rdata0(hw);
	msg->data[1] = mbox0_ll_get_reg_0x27_mail_rdata1(hw);

	return MBOX0_HAL_OK;
}

static int mbox0_hal_chn2_recv(mbox0_hal_t* hal, mbox0_message_t * msg)
{
	mbox0_hw_t * hw = hal->hw;

	msg->src_cpu = -1;
	msg->data[0] = -1;
	msg->data[1] = -1;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	msg->src_cpu = mbox0_ll_get_reg_0x35_mail_sid(hw);
	msg->data[0] = mbox0_ll_get_reg_0x36_mail_rdata0(hw);
	msg->data[1] = mbox0_ll_get_reg_0x37_mail_rdata1(hw);

	return MBOX0_HAL_OK;
}

static int mbox0_hal_chn0_int_enable(mbox0_hal_t* hal, uint8_t enable)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	mbox0_ll_set_reg_0x10_int_wrerr_en(hw, 0);
	mbox0_ll_set_reg_0x10_int_rderr_en(hw, 0);
	mbox0_ll_set_reg_0x10_int_wrfull_en(hw, 0);

	mbox0_ll_set_reg_0x10_int_en(hw, enable);

	return MBOX0_HAL_OK;
}

static int mbox0_hal_chn1_int_enable(mbox0_hal_t* hal, uint8_t enable)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	mbox0_ll_set_reg_0x20_int_wrerr_en(hw, 0);
	mbox0_ll_set_reg_0x20_int_rderr_en(hw, 0);
	mbox0_ll_set_reg_0x20_int_wrfull_en(hw, 0);

	mbox0_ll_set_reg_0x20_int_en(hw, enable);

	return MBOX0_HAL_OK;
}

static int mbox0_hal_chn2_int_enable(mbox0_hal_t* hal, uint8_t enable)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	mbox0_ll_set_reg_0x30_int_wrerr_en(hw, 0);
	mbox0_ll_set_reg_0x30_int_rderr_en(hw, 0);
	mbox0_ll_set_reg_0x30_int_wrfull_en(hw, 0);

	mbox0_ll_set_reg_0x30_int_en(hw, enable);

	return MBOX0_HAL_OK;
}

static uint32_t mbox0_hal_chn0_get_int_status(mbox0_hal_t* hal)
{
	mbox0_hw_t * hw = hal->hw;

	uint32_t int_status = 0;

	if(hw == NULL)
		return int_status;

	int_status = mbox0_ll_get_reg_0x3_value(hw);

	int_status = (int_status >> 0) & 0x01;

	return int_status;
}

static uint32_t mbox0_hal_chn1_get_int_status(mbox0_hal_t* hal)
{
	mbox0_hw_t * hw = hal->hw;

	uint32_t int_status = 0;

	if(hw == NULL)
		return int_status;

	int_status = mbox0_ll_get_reg_0x3_value(hw);

	int_status = (int_status >> 1) & 0x01;

	return int_status;
}

static uint32_t mbox0_hal_chn2_get_int_status(mbox0_hal_t* hal)
{
	mbox0_hw_t * hw = hal->hw;

	uint32_t int_status = 0;

	if(hw == NULL)
		return int_status;

	int_status = mbox0_ll_get_reg_0x3_value(hw);

	int_status = (int_status >> 2) & 0x01;

	return int_status;
}

static uint32_t mbox0_hal_chn0_get_rx_fifo_stat(mbox0_hal_t* hal)
{
	mbox0_hw_t * hw = hal->hw;

	uint32_t    rx_fifo_stat = 0;

	if(hw == NULL)
		return rx_fifo_stat;

	if(mbox0_ll_get_reg_0x10_fifo_noempt(hw) != 0)
		rx_fifo_stat |= RX_FIFO_STAT_NOT_EMPTY;
	if(mbox0_ll_get_reg_0x18_fifo_empt(hw) != 0)
		rx_fifo_stat |= RX_FIFO_STAT_EMPTY;
	if(mbox0_ll_get_reg_0x18_fifo_full(hw) != 0)
		rx_fifo_stat |= RX_FIFO_STAT_FULL;

	return rx_fifo_stat;
}

static uint32_t mbox0_hal_chn1_get_rx_fifo_stat(mbox0_hal_t* hal)
{
	mbox0_hw_t * hw = hal->hw;

	uint32_t    rx_fifo_stat = 0;

	if(hw == NULL)
		return rx_fifo_stat;

	if(mbox0_ll_get_reg_0x20_fifo_noempt(hw) != 0)
		rx_fifo_stat |= RX_FIFO_STAT_NOT_EMPTY;
	if(mbox0_ll_get_reg_0x28_fifo_empt(hw) != 0)
		rx_fifo_stat |= RX_FIFO_STAT_EMPTY;
	if(mbox0_ll_get_reg_0x28_fifo_full(hw) != 0)
		rx_fifo_stat |= RX_FIFO_STAT_FULL;

	return rx_fifo_stat;
}

static uint32_t mbox0_hal_chn2_get_rx_fifo_stat(mbox0_hal_t* hal)
{
	mbox0_hw_t * hw = hal->hw;

	uint32_t    rx_fifo_stat = 0;

	if(hw == NULL)
		return rx_fifo_stat;

	if(mbox0_ll_get_reg_0x30_fifo_noempt(hw) != 0)
		rx_fifo_stat |= RX_FIFO_STAT_NOT_EMPTY;
	if(mbox0_ll_get_reg_0x38_fifo_empt(hw) != 0)
		rx_fifo_stat |= RX_FIFO_STAT_EMPTY;
	if(mbox0_ll_get_reg_0x38_fifo_full(hw) != 0)
		rx_fifo_stat |= RX_FIFO_STAT_FULL;

	return rx_fifo_stat;
}

static uint32_t mbox0_hal_chn0_get_rx_fifo_count(mbox0_hal_t* hal)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return 0;

	return mbox0_ll_get_reg_0x18_fifo_count(hw);
}

static uint32_t mbox0_hal_chn1_get_rx_fifo_count(mbox0_hal_t* hal)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return 0;

	return mbox0_ll_get_reg_0x28_fifo_count(hw);
}

static uint32_t mbox0_hal_chn2_get_rx_fifo_count(mbox0_hal_t* hal)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return 0;

	return mbox0_ll_get_reg_0x38_fifo_count(hw);
}

static uint32_t mbox0_hal_chn0_clear_tx_fifo_stat(mbox0_hal_t* hal)
{
	mbox0_hw_t * hw = hal->hw;

	uint32_t    tx_fifo_stat = 0;

	if(hw == NULL)
		return tx_fifo_stat;

	if(mbox0_ll_get_reg_0x10_wrerr_int_sta(hw) != 0)
	{
		tx_fifo_stat |= TX_FIFO_STAT_WR_ERR;
	}
	if(mbox0_ll_get_reg_0x10_rderr_int_sta(hw) != 0)
	{
		tx_fifo_stat |= TX_FIFO_STAT_RD_ERR;
	}
	if(mbox0_ll_get_reg_0x10_wrfull_int_sta(hw) != 0)
	{
		tx_fifo_stat |= TX_FIFO_STAT_WR_FULL;
	}
	
	mbox0_ll_set_reg_0x10_wrerr_int_sta(hw, 1);
	mbox0_ll_set_reg_0x10_rderr_int_sta(hw, 1);
	mbox0_ll_set_reg_0x10_wrfull_int_sta(hw, 1);

	return tx_fifo_stat;
}

static uint32_t mbox0_hal_chn1_clear_tx_fifo_stat(mbox0_hal_t* hal)
{
	mbox0_hw_t * hw = hal->hw;

	uint32_t    tx_fifo_stat = 0;

	if(hw == NULL)
		return tx_fifo_stat;

	if(mbox0_ll_get_reg_0x20_wrerr_int_sta(hw) != 0)
	{
		tx_fifo_stat |= TX_FIFO_STAT_WR_ERR;
	}
	if(mbox0_ll_get_reg_0x20_rderr_int_sta(hw) != 0)
	{
		tx_fifo_stat |= TX_FIFO_STAT_RD_ERR;
	}
	if(mbox0_ll_get_reg_0x20_wrfull_int_sta(hw) != 0)
	{
		tx_fifo_stat |= TX_FIFO_STAT_WR_FULL;
	}
	
	mbox0_ll_set_reg_0x20_wrerr_int_sta(hw, 1);
	mbox0_ll_set_reg_0x20_rderr_int_sta(hw, 1);
	mbox0_ll_set_reg_0x20_wrfull_int_sta(hw, 1);

	return tx_fifo_stat;
}

static uint32_t mbox0_hal_chn2_clear_tx_fifo_stat(mbox0_hal_t* hal)
{
	mbox0_hw_t * hw = hal->hw;

	uint32_t    tx_fifo_stat = 0;

	if(hw == NULL)
		return tx_fifo_stat;

	if(mbox0_ll_get_reg_0x30_wrerr_int_sta(hw) != 0)
	{
		tx_fifo_stat |= TX_FIFO_STAT_WR_ERR;
	}
	if(mbox0_ll_get_reg_0x30_rderr_int_sta(hw) != 0)
	{
		tx_fifo_stat |= TX_FIFO_STAT_RD_ERR;
	}
	if(mbox0_ll_get_reg_0x30_wrfull_int_sta(hw) != 0)
	{
		tx_fifo_stat |= TX_FIFO_STAT_WR_FULL;
	}

	mbox0_ll_set_reg_0x30_wrerr_int_sta(hw, 1);
	mbox0_ll_set_reg_0x30_rderr_int_sta(hw, 1);
	mbox0_ll_set_reg_0x30_wrfull_int_sta(hw, 1);

	return tx_fifo_stat;
}

int mbox0_hal_init(mbox0_hal_t * hal)
{
	if(hal->hw != NULL)
		return MBOX0_HAL_OK;
	
	hal->hw = (mbox0_hw_t *)mbox0_ll_get_reg_base();
	hal->fifo_size = mbox0_ll_get_fifo_size();

	return MBOX0_HAL_OK;
}

int mbox0_hal_dev_init(mbox0_hal_t * hal)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	mbox0_ll_set_reg_0x2_softrstn(hw, 1);
	mbox0_ll_set_reg_0x2_hclk_bps(hw, 0);
	mbox0_ll_set_reg_0x2_chn_pro_disable(hw, 0);

	return MBOX0_HAL_OK;
}

int mbox0_hal_deinit(mbox0_hal_t * hal)
{
	if(hal->hw == NULL)
		return MBOX0_HAL_OK;

	hal->hw = NULL;
	hal->fifo_size = 0;

	return MBOX0_HAL_OK;
}

int mbox0_hal_dev_deinit(mbox0_hal_t * hal)
{
	mbox0_hw_t * hw = hal->hw;

	if(hw == NULL)
		return MBOX0_HAL_SW_NO_INIT;

	mbox0_ll_set_reg_0x2_softrstn(hw, 0);

	return MBOX0_HAL_OK;
}

const hal_chn_drv_t  hal_chn0_drv =
{
	.chn_cfg_fifo = mbox0_hal_chn0_cfg_fifo,
	.chn_send = mbox0_hal_chn0_send,
	.chn_recv = mbox0_hal_chn0_recv,
	.chn_int_enable = mbox0_hal_chn0_int_enable,
	.chn_get_int_status = mbox0_hal_chn0_get_int_status,
	.chn_get_rx_fifo_stat = mbox0_hal_chn0_get_rx_fifo_stat,
	.chn_get_rx_fifo_count = mbox0_hal_chn0_get_rx_fifo_count,
	.chn_clear_tx_fifo_stat = mbox0_hal_chn0_clear_tx_fifo_stat,
} ;

const hal_chn_drv_t  hal_chn1_drv =
{
	.chn_cfg_fifo = mbox0_hal_chn1_cfg_fifo,
	.chn_send = mbox0_hal_chn1_send,
	.chn_recv = mbox0_hal_chn1_recv,
	.chn_int_enable = mbox0_hal_chn1_int_enable,
	.chn_get_int_status = mbox0_hal_chn1_get_int_status,
	.chn_get_rx_fifo_stat = mbox0_hal_chn1_get_rx_fifo_stat,
	.chn_get_rx_fifo_count = mbox0_hal_chn1_get_rx_fifo_count,
	.chn_clear_tx_fifo_stat = mbox0_hal_chn1_clear_tx_fifo_stat,
} ;

const hal_chn_drv_t  hal_chn2_drv =
{
	.chn_cfg_fifo = mbox0_hal_chn2_cfg_fifo,
	.chn_send = mbox0_hal_chn2_send,
	.chn_recv = mbox0_hal_chn2_recv,
	.chn_int_enable = mbox0_hal_chn2_int_enable,
	.chn_get_int_status = mbox0_hal_chn2_get_int_status,
	.chn_get_rx_fifo_stat = mbox0_hal_chn2_get_rx_fifo_stat,
	.chn_get_rx_fifo_count = mbox0_hal_chn2_get_rx_fifo_count,
	.chn_clear_tx_fifo_stat = mbox0_hal_chn2_clear_tx_fifo_stat,
} ;

