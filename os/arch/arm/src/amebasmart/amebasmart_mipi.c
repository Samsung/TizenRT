/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <semaphore.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/semaphore.h>
#include <tinyara/mipidsi/mipi_dsi.h>
#include <tinyara/mipidsi/mipi_display.h>
#include <arch/board/board.h>
#include <tinyara/spinlock.h>

#include "chip.h"
#include "PinNames.h"
#include "pwmout_api.h"
#include <tinyara/lcd/mipi_lcd.h>
#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
#endif

static u32 vo_freq;
static volatile u8 send_cmd_done = 0;
static volatile u8 receive_cmd_done = 1;
static volatile bool rx_data_rdy = FALSE;
static uint8_t rx_data_buff[5]; /*long response have 5 bytes in payload, short response have 1 or 2 bytes in payload */
struct amebasmart_mipi_dsi_host_s {
	struct mipi_dsi_host dsi_host;
	MIPI_TypeDef *MIPIx;
	MIPI_InitTypeDef *MIPI_InitStruct;
};

struct irq {
	u32 num;
	u32 data;
	u32 priority;
};

struct rtl8730e_lcdc_info_s {
	struct mipi_lcd_config_s lcd_config;
	pwmout_t pwm_led;
	uint8_t pwm_level;
	bool underflow;
};
extern struct rtl8730e_lcdc_info_s g_rtl8730e_config_dev_s;
extern volatile spinlock_t g_rtl8730e_config_dev_s_underflow;
/* Helpers */
static void amebasmart_set_clock(void);
static void amebasmart_check_freq(struct lcd_data *data);
static void amebasmart_mipi_init_helper(FAR struct amebasmart_mipi_dsi_host_s *priv);
static void amebasmart_register_interrupt(void);

/* MIPI methods */
static void amebasmart_mipidsi_send_cmd(MIPI_TypeDef *MIPIx, u8 cmd, u8 payload_len, const u8 *para_list, u8 cmd_type);
static int amebasmart_mipi_attach(FAR struct mipi_dsi_host *dsi_host, FAR struct mipi_dsi_device *dsi_device);
static int amebasmart_mipi_detach(FAR struct mipi_dsi_host *dsi_host, FAR struct mipi_dsi_device *dsi_device);
static int amebasmart_mipi_transfer(FAR struct mipi_dsi_host *dsi_host, FAR const struct mipi_dsi_msg *msg);

/************************************************************************************
 * Private Data
 ************************************************************************************/
/*initialize the MIPI IRQ info*/
struct irq mipi_irq_info = {
	.num = MIPI_DSI_IRQ,
	.data = (u32) MIPI,
	.priority = INT_PRI_MIDDLE,
};

static const struct mipi_dsi_host_ops g_dsi_ops = {
	.attach = amebasmart_mipi_attach,
	.detach = amebasmart_mipi_detach,
	.transfer = amebasmart_mipi_transfer,
};

MIPI_InitTypeDef MIPI_InitStruct_g;

static struct amebasmart_mipi_dsi_host_s g_dsi_host = {
	.dsi_host = {
		&g_dsi_ops
	},
	.MIPIx = MIPI,
	.MIPI_InitStruct = &MIPI_InitStruct_g,
};

/***********Helpers ***********/

static void amebasmart_mipi_fill_buffer(FAR struct mipi_dsi_host *dsi_host)
{
	FAR struct amebasmart_mipi_dsi_host_s *priv = (FAR struct amebasmart_mipi_dsi_host_s *)dsi_host;
	u32 vtotal, htotal_bits, bit_per_pixel, overhead_cycles, overhead_bits, total_bits;
	priv->MIPI_InitStruct->MIPI_VideoDataFormat = MIPI_VIDEO_DATA_FORMAT_RGB565;
	switch (priv->MIPI_InitStruct->MIPI_VideoDataFormat) {
	case MIPI_VIDEO_DATA_FORMAT_RGB565:
		bit_per_pixel = 16;
		break;
	case MIPI_VIDEO_DATA_FORMAT_RGB666_PACKED:
		bit_per_pixel = 18;
		break;
	case MIPI_VIDEO_DATA_FORMAT_RGB666_LOOSELY:
	case MIPI_VIDEO_DATA_FORMAT_RGB888:
	default:
		bit_per_pixel = 24;
		break;
	}
	priv->MIPI_InitStruct->MIPI_LaneNum = dsi_host->config.lcd_lane_num;
	priv->MIPI_InitStruct->MIPI_FrameRate = dsi_host->config.mipi_frame_rate;
	priv->MIPI_InitStruct->MIPI_HSA = dsi_host->config.mipi_dsi_HSA * bit_per_pixel / 8;	//- 10; /* here the unit is pixel but not us */
	if (priv->MIPI_InitStruct->MIPI_VideoModeInterface == MIPI_VIDEO_NON_BURST_MODE_WITH_SYNC_PULSES) {
		priv->MIPI_InitStruct->MIPI_HBP = dsi_host->config.mipi_dsi_HBP * bit_per_pixel / 8;	//- 10;
	} else {
		priv->MIPI_InitStruct->MIPI_HBP = (dsi_host->config.mipi_dsi_HSA + dsi_host->config.mipi_dsi_HBP) * bit_per_pixel / 8;	//-10 ;
	}
	priv->MIPI_InitStruct->MIPI_HACT = dsi_host->config.XPixels;
	priv->MIPI_InitStruct->MIPI_HFP = dsi_host->config.mipi_dsi_HFP * bit_per_pixel / 8;	//-12;
	priv->MIPI_InitStruct->MIPI_VSA = dsi_host->config.mipi_dsi_VSA;
	priv->MIPI_InitStruct->MIPI_VBP = dsi_host->config.mipi_dsi_VBP;
	priv->MIPI_InitStruct->MIPI_VACT = dsi_host->config.YPixels;
	priv->MIPI_InitStruct->MIPI_VFP = dsi_host->config.mipi_dsi_VFP;
	vtotal = priv->MIPI_InitStruct->MIPI_VSA + priv->MIPI_InitStruct->MIPI_VBP + priv->MIPI_InitStruct->MIPI_VACT + priv->MIPI_InitStruct->MIPI_VFP;
	htotal_bits = (dsi_host->config.mipi_dsi_HSA + dsi_host->config.mipi_dsi_HBP + priv->MIPI_InitStruct->MIPI_HACT + dsi_host->config.mipi_dsi_HFP) * bit_per_pixel;
	overhead_cycles = T_LPX + T_HS_PREP + T_HS_ZERO + T_HS_TRAIL + T_HS_EXIT;
	overhead_bits = overhead_cycles * priv->MIPI_InitStruct->MIPI_LaneNum * 8;
	total_bits = htotal_bits + overhead_bits;

	priv->MIPI_InitStruct->MIPI_VideDataLaneFreq = priv->MIPI_InitStruct->MIPI_FrameRate * total_bits * vtotal / priv->MIPI_InitStruct->MIPI_LaneNum / Mhz + 20;
	priv->MIPI_InitStruct->MIPI_LineTime = (priv->MIPI_InitStruct->MIPI_VideDataLaneFreq * Mhz) / 8 / priv->MIPI_InitStruct->MIPI_FrameRate / vtotal;
	priv->MIPI_InitStruct->MIPI_BllpLen = priv->MIPI_InitStruct->MIPI_LineTime / 2;

	if (dsi_host->config.mipi_dsi_HSA + dsi_host->config.mipi_dsi_HBP + dsi_host->config.XPixels + dsi_host->config.mipi_dsi_HFP < dsi_host->config.mipi_lcd_limit + dsi_host->config.mipi_dsi_RTNI * 16) {
		mipidbg("!!ERROR!!, LCM NOT SUPPORT\n");
	}

	if (priv->MIPI_InitStruct->MIPI_LineTime * priv->MIPI_InitStruct->MIPI_LaneNum < total_bits / 8) {
		mipidbg("!!ERROR!!, LINE TIME TOO SHORT!\n");
	}
	if (priv->MIPI_InitStruct->MIPI_VideDataLaneFreq * priv->MIPI_InitStruct->MIPI_LaneNum / 24 >= vo_freq) {
		mipidbg("!!ERROR!!, VO CLK too slow!\n");
	}
	mipivdbg("DataLaneFreq: %d, LineTime: %d...bitsperpixel:%d\n", priv->MIPI_InitStruct->MIPI_VideDataLaneFreq, priv->MIPI_InitStruct->MIPI_LineTime, bit_per_pixel);
}

static void amebasmart_mipi_init_helper(FAR struct amebasmart_mipi_dsi_host_s *priv)
{
	amebasmart_check_freq(&(priv->dsi_host.config));
	amebasmart_set_clock();

	MIPI_StructInit(priv->MIPI_InitStruct);
	amebasmart_mipi_fill_buffer(&(priv->dsi_host));
	MIPI_Init(priv->MIPIx, priv->MIPI_InitStruct);

	MIPI_DSI_TO1_Set(priv->MIPIx, DISABLE, 0);
	MIPI_DSI_TO2_Set(priv->MIPIx, ENABLE, 0x7FFFFFFF);
	MIPI_DSI_TO3_Set(priv->MIPIx, DISABLE, 0);

	amebasmart_register_interrupt();
	MIPI_DSI_INT_Config(priv->MIPIx, DISABLE, ENABLE, FALSE);
}

static void amebasmart_mipidsi_send_cmd(MIPI_TypeDef *MIPIx, u8 cmd, u8 payload_len, const u8 *para_list, u8 cmd_type)
{
	u32 word0, word1, addr, idx;
	u8 cmd_addr[128];
	if (MIPI_LPTX_IS_READ(cmd_type)) {
		receive_cmd_done = 0;
	}
	if (payload_len == 0) {
		MIPI_DSI_CMD_Send(MIPIx, cmd_type, cmd, 0);
		return;
	} else if (payload_len == 1) {
		MIPI_DSI_CMD_Send(MIPIx, cmd_type, cmd, para_list[0]);
		return;
	}

	cmd_addr[0] = cmd;
	for (idx = 0; idx < payload_len; idx++) {
		cmd_addr[idx + 1] = para_list[idx];
	}

	payload_len = payload_len + 1;
	/* the addr payload_len 1 ~ 8 is 0 */
	for (addr = 0; addr < (u32)(payload_len + 7) / 8; addr++) {
		idx = addr * 8;
		word0 = (cmd_addr[idx + 3] << 24) + (cmd_addr[idx + 2] << 16) + (cmd_addr[idx + 1] << 8) + cmd_addr[idx + 0];
		word1 = (cmd_addr[idx + 7] << 24) + (cmd_addr[idx + 6] << 16) + (cmd_addr[idx + 5] << 8) + cmd_addr[idx + 4];
		MIPI_DSI_CMD_LongPkt_MemQWordRW(MIPIx, addr, &word0, &word1, FALSE);
	}
	MIPI_DSI_CMD_Send(MIPIx, cmd_type, payload_len, 0);
}
static void amebasmart_mipidsi_rcmd_decode(MIPI_TypeDef *MIPIx, u8 rcmd_idx)
{
	u32 rcmd_val, payload_len, addr, word0, word1;
	u8 data_id, byte0, byte1, rx_offset;
	rcmd_val = MIPI_DSI_CMD_Rxcv_CMD(MIPIx, rcmd_idx);
	if (rcmd_val & (MIPI_BIT_RCMDx_CRC_CHK_ERR | MIPI_BIT_RCMDx_ECC_CHK_ERR | MIPI_BIT_RCMDx_ECC_ERR_FIX)) {
		mipidbg("RCMD[%d] Error\n", rcmd_idx + 1);
	}
	if (rcmd_val & MIPI_BIT_RCMDx_ECC_NO_ERR) {
		data_id = MIPI_GET_RCMDx_DATAID(rcmd_val);
		byte0 = MIPI_GET_RCMDx_BYTE0(rcmd_val);
		byte1 = MIPI_GET_RCMDx_BYTE1(rcmd_val);
		/*For short read, it is normally returns only two bytes, byte0: payload, byte1: checksum (if present)*/
		rx_data_buff[0] = byte0;
		rx_data_buff[1] = byte1;

		/* Peripheral to Processor Transactions Long Packet is 0x1A or 0x1C, byte0 and byte1 will then be the length of payload */
		if (MIPI_LPRX_IS_LONGRead(data_id)) {
			payload_len = (byte1 << 8) + byte0;
		} else {
			payload_len = 0;
		}
		rx_offset = 0;
		/* the addr payload_len 1 ~ 8 is 0 */
		for (addr = 0; addr < (payload_len + 7) / 8; addr++) {
			MIPI_DSI_CMD_LongPkt_MemQWordRW(MIPIx, addr, &word0, &word1, TRUE);
			/*unpack word0*/
			for (int i = 0; i < 4 && rx_offset < payload_len; i++) {
				rx_data_buff[rx_offset] = (word0 >> (i * 8)) & 0xFF;
				rx_offset++;
			}
			/*unpack word1*/
			for (int i = 0; i < 4 && rx_offset < payload_len; i++) {
				rx_data_buff[rx_offset] = (word1 >> (i * 8)) & 0xFF;
				rx_offset++;
			}
		}
		rx_data_rdy = TRUE;
	}

	receive_cmd_done = 1;
}
static void amebasmart_mipidsi_isr(void)
{
	MIPI_TypeDef *MIPIx = g_dsi_host.MIPIx;
	u32 reg_val, reg_val2, reg_dphy_err;
	u32 count = 0;
	reg_val = MIPI_DSI_INTS_Get(MIPIx);
	MIPI_DSI_INTS_Clr(MIPIx, reg_val);

	reg_val2 = MIPI_DSI_INTS_ACPU_Get(MIPIx);
	MIPI_DSI_INTS_ACPU_Clr(MIPIx, reg_val2);
	if (reg_val & MIPI_BIT_CMD_TXDONE) {
		reg_val &= ~MIPI_BIT_CMD_TXDONE;
		send_cmd_done = 1;
	}
	if (reg_val & MIPI_BIT_RCMD1) {
		amebasmart_mipidsi_rcmd_decode(MIPIx, 0);
	}
	if (reg_val & MIPI_BIT_RCMD2) {
		amebasmart_mipidsi_rcmd_decode(MIPIx, 1);
	}
	if (reg_val & MIPI_BIT_RCMD3) {
		amebasmart_mipidsi_rcmd_decode(MIPIx, 2);
	}

	if (reg_val & (MIPI_BIT_LPRX_TIMEOUT | MIPI_BIT_LPTX_TIMEOUT)) {
		if (reg_val & MIPI_BIT_LPRX_TIMEOUT) {
			mipidbg("LPRX TimeOut\n");
			while (MIPIx->MIPI_DPHY_STATUS0 & MIPI_BIT_DIRECTION) {
				DelayUs(1);
				if (count > 100000) {	/* If wait time more than 100ms */
					mipidbg("MIPI DPHY STATUS0 Wait Timeout\n");
					break;
				}
				count++;
			}
		} else {
			mipidbg("LPTX TimeOut\n");
		}
		MIPIx->MIPI_CKLANE_CTRL = (MIPIx->MIPI_CKLANE_CTRL & ~MIPI_MASK_FORCETXSTOPMODE) | MIPI_FORCETXSTOPMODE(1);
		DelayUs(1);
		MIPIx->MIPI_CKLANE_CTRL = (MIPIx->MIPI_CKLANE_CTRL & ~MIPI_MASK_FORCETXSTOPMODE) | MIPI_FORCETXSTOPMODE(0);

		MIPIx->MIPI_MAIN_CTRL = (MIPIx->MIPI_MAIN_CTRL & ~MIPI_BIT_DSI_MODE) | MIPI_BIT_LPTX_RST | MIPI_BIT_LPRX_RST;
		DelayUs(1);
		MIPIx->MIPI_MAIN_CTRL = (MIPIx->MIPI_MAIN_CTRL & ~(MIPI_BIT_DSI_MODE | MIPI_BIT_LPTX_RST | MIPI_BIT_LPRX_RST));
	}

	if (reg_val & MIPI_BIT_ERROR) {
		reg_dphy_err = MIPIx->MIPI_DPHY_ERR;
		MIPIx->MIPI_DPHY_ERR = reg_dphy_err;
		mipidbg("LPTX Error: 0x%x, DPHY Error: 0x%x\n", reg_val, reg_dphy_err);

		if (MIPIx->MIPI_CONTENTION_DETECTOR_AND_STOPSTATE_DT & MIPI_MASK_DETECT_ENABLE) {
			MIPIx->MIPI_CONTENTION_DETECTOR_AND_STOPSTATE_DT &= ~MIPI_MASK_DETECT_ENABLE;

			MIPIx->MIPI_DPHY_ERR = reg_dphy_err;
			MIPI_DSI_INTS_Clr(MIPIx, MIPI_BIT_ERROR);
			mipidbg("LPTX Error CLR: 0x%x, DPHY: 0x%x\n", MIPIx->MIPI_INTS, MIPIx->MIPI_DPHY_ERR);
		}

		if (MIPIx->MIPI_DPHY_ERR == reg_dphy_err) {
			mipidbg("LPTX Still Error\n");
			MIPI_DSI_INT_Config(MIPIx, ENABLE, DISABLE, FALSE);
		}
	}

	if (reg_val) {
		mipidbg("LPTX Error Occur: 0x%x\n", reg_val);
	}

	if (reg_val2) {
		mipidbg("error occured #\n");
	}
}

static void amebasmart_check_freq(struct lcd_data *data)
{
	u32 totaly = data->mipi_dsi_VSA + data->mipi_dsi_VBP + data->mipi_dsi_VFP + data->YPixels;
	u32 totalx = data->mipi_dsi_HSA + data->mipi_dsi_HBP + data->mipi_dsi_HFP + data->XPixels;
	vo_freq = totaly * totalx * data->mipi_frame_rate / Mhz + 4;
	mipivdbg("vo_freq: %d\n", vo_freq);
	assert_param(vo_freq < 67);
}

static void amebasmart_set_clock(void)
{
	u32 PLLDiv = PLL_GET_NPLL_DIVN_SDM(PLL_BASE->PLL_NPPLL_CTRL1) + 2;
	u32 PLL_CLK = XTAL_ClkGet() * PLLDiv;
	u32 mipi_ckd = PLL_CLK / vo_freq - 1;
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0) & ~LSYS_MASK_CKD_MIPI) | LSYS_CKD_MIPI(mipi_ckd));
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0) & ~LSYS_MASK_CKD_HPERI) | LSYS_CKD_HPERI(3));

	RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_HPERI_CLOCK, ENABLE);
	RCC_PeriphClockCmd(APBPeriph_LCDC, APBPeriph_LCDCMIPI_CLOCK, ENABLE);

}

static void amebasmart_register_interrupt(void)
{
	InterruptDis(mipi_irq_info.num);
	InterruptUnRegister(mipi_irq_info.num);
	InterruptRegister((IRQ_FUN) amebasmart_mipidsi_isr, mipi_irq_info.num, (u32) mipi_irq_info.data, mipi_irq_info.priority);
	InterruptEn(mipi_irq_info.num, mipi_irq_info.priority);
}

void mipi_mode_switch_to_video(bool do_enable){
	if(do_enable){
		MIPI_DSI_Mode_Switch(g_dsi_host.MIPIx, ENABLE);
	}
	else{
		MIPI_DSI_Mode_Switch(g_dsi_host.MIPIx, DISABLE);
	}
}

void mipidsi_acpu_reg_clear(void){
	MIPI_DSI_INTS_ACPU_Clr(g_dsi_host.MIPIx, MIPI_DSI_INTS_ACPU_Get(g_dsi_host.MIPIx));
}

/************************************************************************************
 * Private Functions
 ************************************************************************************/
static int amebasmart_mipi_attach(FAR struct mipi_dsi_host *dsi_host, FAR struct mipi_dsi_device *dsi_device)
{
	return 0;
}

static int amebasmart_mipi_detach(FAR struct mipi_dsi_host *dsi_host, FAR struct mipi_dsi_device *dsi_device)
{
	return 0;
}

static int amebasmart_mipi_transfer(FAR struct mipi_dsi_host *dsi_host, FAR const struct mipi_dsi_msg *msg)
{
#ifdef CONFIG_PM
	bsp_pm_domain_control(BSP_MIPI_DRV, 1);
#endif
	FAR struct amebasmart_mipi_dsi_host_s *priv = (FAR struct amebasmart_mipi_dsi_host_s *)dsi_host;
	struct mipi_dsi_packet packet;
	int cnt = 5000;
	/*When underflow happens, mipi's irq will be registered to rtl8730e_mipidsi_underflowreset callback to handle video mode frame done interrupt.
	  only one type of interrupt can be enabled at any one time, either cmd mode interrupt or video mode interrupt, so we need to re-register mipi's
	  irq cmd mode callback here
	*/
#ifdef CONFIG_SMP
	spin_lock(&g_rtl8730e_config_dev_s_underflow);
#endif
	if (g_rtl8730e_config_dev_s.underflow) {
		amebasmart_register_interrupt();
		MIPI_DSI_INT_Config(priv->MIPIx, DISABLE, ENABLE, FALSE);
		g_rtl8730e_config_dev_s.underflow = 0;
		mipidbg("underflow happened, re-register mipi irq to handle cmd mode interrupt\n");
	}
	if (msg->type == MIPI_DSI_END_OF_TRANSMISSION) {
		MIPI_DSI_INT_Config(g_dsi_host.MIPIx, DISABLE, DISABLE, FALSE);
#ifdef CONFIG_SMP
		spin_unlock(&g_rtl8730e_config_dev_s_underflow);
#endif
#ifdef CONFIG_PM
		bsp_pm_domain_control(BSP_MIPI_DRV, 0);
#endif
		return OK;
	}
	int ret = mipi_dsi_create_packet(&packet, msg);
	if (ret != OK) {
#ifdef CONFIG_SMP
		spin_unlock(&g_rtl8730e_config_dev_s_underflow);
#endif
#ifdef CONFIG_PM
		bsp_pm_domain_control(BSP_MIPI_DRV, 0);
#endif
		return ret;
	}
	send_cmd_done = 0;
	rx_data_rdy = FALSE;
	if (mipi_dsi_packet_format_is_short(msg->type)) {
		if (packet.header[1] == 0) {
			amebasmart_mipidsi_send_cmd(priv->MIPIx, packet.header[0], 0, NULL, msg->type);
		} else {
			amebasmart_mipidsi_send_cmd(priv->MIPIx, packet.header[0], 1, packet.header + 1, msg->type);
		}
	} else {
		amebasmart_mipidsi_send_cmd(priv->MIPIx, packet.header[0], packet.payload_length, packet.payload, msg->type);
	}
	while(send_cmd_done != 1 || receive_cmd_done != 1) {
		DelayMs(1);
		cnt--;
		if (cnt == 0) {
#ifdef CONFIG_PM
			bsp_pm_domain_control(BSP_MIPI_DRV, 0);
#endif
#ifdef CONFIG_SMP
			spin_unlock(&g_rtl8730e_config_dev_s_underflow);
#endif
			return FAIL;
		}
	}
#ifdef CONFIG_SMP
	spin_unlock(&g_rtl8730e_config_dev_s_underflow);
#endif
#ifdef CONFIG_PM
	bsp_pm_domain_control(BSP_MIPI_DRV, 0);
#endif
	if (rx_data_rdy) {
		memcpy(msg->rx_buf, rx_data_buff, msg->rx_len);
	}
	return OK;
}

#ifdef CONFIG_PM
static uint32_t rtk_mipi_suspend(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;

	/* Check anything is needed? */
	return 1;
}

static uint32_t rtk_mipi_resume(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;

	FAR struct amebasmart_mipi_dsi_host_s *priv = &g_dsi_host;
	/* For MIPI */
	amebasmart_mipi_init_helper(priv);
	/* For LCDC */
	rtl8730e_lcdc_pm();

	return 1;
}
#endif

void amebasmart_mipi_dsi_host_reinitialize(void)
{
	FAR struct amebasmart_mipi_dsi_host_s *priv = &g_dsi_host;

	/* For MIPI */
	amebasmart_mipi_init_helper(priv);
}

struct mipi_dsi_host *amebasmart_mipi_dsi_host_initialize(struct lcd_data *config)
{
	FAR struct amebasmart_mipi_dsi_host_s *priv = NULL;
	priv = &g_dsi_host;
	priv->dsi_host.config = *config;
	amebasmart_mipi_init_helper(priv);
	mipi_dsi_host_register(&priv->dsi_host);
#ifdef CONFIG_PM
	bsp_pm_domain_register("MIPI", BSP_MIPI_DRV);
	pmu_register_sleep_callback(PMU_MIPI_DEVICE, (PSM_HOOK_FUN)rtk_mipi_suspend, NULL, (PSM_HOOK_FUN)rtk_mipi_resume, NULL);
#endif

	return (struct mipi_dsi_host *)priv;
}
