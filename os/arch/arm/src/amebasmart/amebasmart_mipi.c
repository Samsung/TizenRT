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

#include "chip.h"
#include "PinNames.h"

#undef mipierr
#undef mipiinfo
#if defined(CONFIG_DEBUG_MIPI)
#define mipiinfo(format, ...)   printf(format, ##__VA_ARGS__)
#define mipierr(format, ...)    printf(format, ##__VA_ARGS__)
#else
#define mipiinfo(format, ...)
#endif
#define mipierr(format, ...)   printf(format, ##__VA_ARGS__)

u32 vo_freq;
u32 send_cmd_done = 0;

struct amebasmart_mipi_dsi_host_s {
	struct mipi_dsi_host dsi_host;
	MIPI_TypeDef *MIPIx;
	MIPI_InitTypeDef *MIPI_InitStruct;
};

struct mipi_irq {
	u32 irq_num;
	u32 irq_data;
	u32 irq_priority;
};

/* Helpers */
static void set_clock();
static void check_freq(struct lcd_data *data);
static void mipi_init_helper(FAR struct amebasmart_mipi_dsi_host_s *priv);
static void register_interrupt();
static bool is_transfer_completed(void);

/* MIPI methods */
static void mipidsi_send_cmd(MIPI_TypeDef *MIPIx, u8 cmd, u8 payload_len, u8 *para_list);
static int amebasmart_mipi_attach(FAR struct mipi_dsi_host *dsi_host, FAR struct mipi_dsi_device *dsi_device);
static int amebasmart_mipi_detach(FAR struct mipi_dsi_host *dsi_host, FAR struct mipi_dsi_device *dsi_device);
static int amebasmart_mipi_transfer(FAR struct mipi_dsi_host *dsi_host, FAR const struct mipi_dsi_msg *msg);

/* Initialization */
static void amebasmart_spi_bus_initialize(FAR struct amebasmart_spidev_s *priv);

/************************************************************************************
 * Private Data
 ************************************************************************************/
/*initialize the MIPI IRQ info*/
struct mipi_irq mipi_irq_info = {
	.irq_num = MIPI_DSI_IRQ,
	.irq_data = (u32) MIPI,
	.irq_priority = INT_PRI_MIDDLE,
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

static void MIPI_InitStruct_Config_helper(FAR struct mipi_dsi_host *dsi_host)
{
	FAR struct amebasmart_mipi_dsi_host_s *priv = (FAR struct amebasmart_mipi_dsi_host_s *)dsi_host;
	u32 vtotal, htotal_bits, bit_per_pixel, overhead_cycles, overhead_bits, total_bits;
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
	priv->MIPI_InitStruct->MIPI_LaneNum = 2;
	priv->MIPI_InitStruct->MIPI_FrameRate = MIPI_FRAME_RATE;
	priv->MIPI_InitStruct->MIPI_HSA = MIPI_DSI_HSA * bit_per_pixel / 8;	//- 10; /* here the unit is pixel but not us */
	if (priv->MIPI_InitStruct->MIPI_VideoModeInterface == MIPI_VIDEO_NON_BURST_MODE_WITH_SYNC_PULSES) {
		priv->MIPI_InitStruct->MIPI_HBP = MIPI_DSI_HBP * bit_per_pixel / 8;	//- 10;
	} else {
		priv->MIPI_InitStruct->MIPI_HBP = (MIPI_DSI_HSA + MIPI_DSI_HBP) * bit_per_pixel / 8;	//-10 ;
	}
	priv->MIPI_InitStruct->MIPI_HACT = dsi_host->config.XPixels;
	priv->MIPI_InitStruct->MIPI_HFP = MIPI_DSI_HFP * bit_per_pixel / 8;	//-12;
	priv->MIPI_InitStruct->MIPI_VSA = MIPI_DSI_VSA;
	priv->MIPI_InitStruct->MIPI_VBP = MIPI_DSI_VBP;
	priv->MIPI_InitStruct->MIPI_VACT = dsi_host->config.YPixels;
	priv->MIPI_InitStruct->MIPI_VFP = MIPI_DSI_VFP;
	vtotal = priv->MIPI_InitStruct->MIPI_VSA + priv->MIPI_InitStruct->MIPI_VBP + priv->MIPI_InitStruct->MIPI_VACT + priv->MIPI_InitStruct->MIPI_VFP;
	htotal_bits = (MIPI_DSI_HSA + MIPI_DSI_HBP + priv->MIPI_InitStruct->MIPI_HACT + MIPI_DSI_HFP) * bit_per_pixel;
	overhead_cycles = T_LPX + T_HS_PREP + T_HS_ZERO + T_HS_TRAIL + T_HS_EXIT;
	overhead_bits = overhead_cycles * priv->MIPI_InitStruct->MIPI_LaneNum * 8;
	total_bits = htotal_bits + overhead_bits;
	priv->MIPI_InitStruct->MIPI_VideDataLaneFreq = priv->MIPI_InitStruct->MIPI_FrameRate * total_bits * vtotal / priv->MIPI_InitStruct->MIPI_LaneNum / Mhz + 20;
	priv->MIPI_InitStruct->MIPI_LineTime = (priv->MIPI_InitStruct->MIPI_VideDataLaneFreq * Mhz) / 8 / priv->MIPI_InitStruct->MIPI_FrameRate / vtotal;
	priv->MIPI_InitStruct->MIPI_BllpLen = priv->MIPI_InitStruct->MIPI_LineTime / 2;

	if (MIPI_DSI_HSA + MIPI_DSI_HBP + dsi_host->config.XPixels + MIPI_DSI_HFP < (512 + MIPI_DSI_RTNI * 16)) {
		//lldbg(MODULE_BOOT, LEVEL_ERROR, "!!ERROR!!, LCM NOT SUPPORT\n");
	}
	if (priv->MIPI_InitStruct->MIPI_LineTime * priv->MIPI_InitStruct->MIPI_LaneNum < total_bits / 8) {
		//lldbg(MODULE_BOOT, LEVEL_ERROR, "!!ERROR!!, LINE TIME TOO SHORT!\n");
	}
	if (priv->MIPI_InitStruct->MIPI_VideDataLaneFreq * 2 / 24 >= vo_freq) {
		//lldbg(MODULE_BOOT, LEVEL_ERROR, "%x %x\n", MIPI_InitStruct->MIPI_VideDataLaneFreq * 2 / 24, vo_freq);
		//lldbg(MODULE_BOOT, LEVEL_ERROR, "!!ERROR!!, VO CLK too slow!\n");
	}
	//lldbg(MODULE_BOOT, LEVEL_INFO, "DataLaneFreq: %d, LineTime: %d\n", MIPI_InitStruct->MIPI_VideDataLaneFreq, MIPI_InitStruct->MIPI_LineTime);
}

static void mipi_init_helper(FAR struct amebasmart_mipi_dsi_host_s *priv)
{
	check_freq(&(priv->dsi_host.config));
	set_clock();

	MIPI_StructInit(priv->MIPI_InitStruct);
	MIPI_InitStruct_Config_helper(&(priv->dsi_host));
	MIPI_Init(priv->MIPIx, priv->MIPI_InitStruct);

	MIPI_DSI_TO1_Set(priv->MIPIx, DISABLE, 0);
	MIPI_DSI_TO2_Set(priv->MIPIx, ENABLE, 0x7FFFFFFF);
	MIPI_DSI_TO3_Set(priv->MIPIx, DISABLE, 0);

	register_interrupt();
	MIPI_DSI_INT_Config(priv->MIPIx, DISABLE, ENABLE, FALSE);
	MIPI_DSI_init(priv->MIPIx, priv->MIPI_InitStruct);
}

static void mipidsi_send_cmd(MIPI_TypeDef *MIPIx, u8 cmd, u8 payload_len, u8 *para_list)
{
	u32 word0, word1, addr, idx;
	u8 cmd_addr[128];
	if (payload_len == 0) {
		MIPI_DSI_CMD_Send(MIPIx, MIPI_DSI_DCS_SHORT_WRITE, cmd, 0);
		return;
	} else if (payload_len == 1) {
		MIPI_DSI_CMD_Send(MIPIx, MIPI_DSI_DCS_SHORT_WRITE_PARAM, cmd, para_list[0]);
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

	MIPI_DSI_CMD_Send(MIPIx, MIPI_DSI_DCS_LONG_WRITE, payload_len, 0);
}

static void mipidsi_isr(void)
{
	MIPI_TypeDef *MIPIx = MIPI;
	u32 reg_val, reg_val2, reg_dphy_err;

	reg_val = MIPI_DSI_INTS_Get(MIPIx);
	MIPI_DSI_INTS_Clr(MIPIx, reg_val);

	reg_val2 = MIPI_DSI_INTS_ACPU_Get(MIPIx);
	MIPI_DSI_INTS_ACPU_Clr(MIPIx, reg_val2);
	if (reg_val & MIPI_BIT_CMD_TXDONE) {
		reg_val &= ~MIPI_BIT_CMD_TXDONE;
		send_cmd_done = 1;
	}

	if (reg_val & MIPI_BIT_ERROR) {
		reg_dphy_err = MIPIx->MIPI_DPHY_ERR;
		MIPIx->MIPI_DPHY_ERR = reg_dphy_err;
		if (MIPIx->MIPI_CONTENTION_DETECTOR_AND_STOPSTATE_DT & MIPI_MASK_DETECT_ENABLE) {
			MIPIx->MIPI_CONTENTION_DETECTOR_AND_STOPSTATE_DT &= ~MIPI_MASK_DETECT_ENABLE;

			MIPIx->MIPI_DPHY_ERR = reg_dphy_err;
			MIPI_DSI_INTS_Clr(MIPIx, MIPI_BIT_ERROR);
		}

		if (MIPIx->MIPI_DPHY_ERR == reg_dphy_err) {
			lldbg("LPTX Still Error\n");
			MIPI_DSI_INT_Config(MIPIx, ENABLE, DISABLE, FALSE);
		}
		reg_val &= ~MIPI_BIT_ERROR;
	}

	if (reg_val) {
		lldbg("LPTX Error Occur: 0x%x\n", reg_val);
	}

	if (reg_val2) {
		lldbg("error occured #\n");
	}
}

static void check_freq(struct lcd_data *data)
{
	u32 totaly = MIPI_DSI_VSA + MIPI_DSI_VBP + MIPI_DSI_VFP + data->XPixels;
	u32 totalx = MIPI_DSI_HSA + MIPI_DSI_HBP + MIPI_DSI_HFP + data->YPixels;
	vo_freq = totaly * totalx * MIPI_FRAME_RATE / Mhz + 4;
	lldbg("vo_freq: %d\n", vo_freq);
	assert_param(vo_freq < 67);
}

static void set_clock()
{
	u32 PLLDiv = PLL_GET_NPLL_DIVN_SDM(PLL_BASE->PLL_NPPLL_CTRL1) + 2;
	u32 PLL_CLK = XTAL_ClkGet() * PLLDiv;
	u32 mipi_ckd = PLL_CLK / vo_freq - 1;
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0) & ~LSYS_MASK_CKD_MIPI) | LSYS_CKD_MIPI(mipi_ckd));
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0) & ~LSYS_MASK_CKD_HPERI) | LSYS_CKD_HPERI(3));

	RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_HPERI_CLOCK, ENABLE);
	RCC_PeriphClockCmd(APBPeriph_LCDC, APBPeriph_LCDCMIPI_CLOCK, ENABLE);

}

static void register_interrupt()
{
	InterruptDis(mipi_irq_info.irq_num);
	InterruptUnRegister(mipi_irq_info.irq_num);
	InterruptRegister((IRQ_FUN) mipidsi_isr, mipi_irq_info.irq_num, (u32) mipi_irq_info.irq_data, mipi_irq_info.irq_priority);
	InterruptEn(mipi_irq_info.irq_num, mipi_irq_info.irq_priority);
}

static bool is_transfer_completed(void)
{
	if (send_cmd_done == 1) {
		return true;
	}
	return false;
}

void mipidsi_mode_switch(bool do_enable){
	if(do_enable){
		MIPI_DSI_Mode_Switch(MIPI, ENABLE);
	}
	else{
		MIPI_DSI_Mode_Switch(MIPI, DISABLE);
	}
	return;
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
	MIPI_DSI_INT_Config(g_dsi_host.MIPIx, DISABLE, DISABLE, FALSE);
	return 0;
}

static int amebasmart_mipi_transfer(FAR struct mipi_dsi_host *dsi_host, FAR const struct mipi_dsi_msg *msg)
{
	FAR struct amebasmart_mipi_dsi_host_s *priv = (FAR struct amebasmart_mipi_dsi_host_s *)dsi_host;
	struct mipi_dsi_packet packet;
	int ret = mipi_dsi_create_packet(&packet, msg);
	if (ret != OK) {
		return ret;
	}
	send_cmd_done = 0;
	if (mipi_dsi_packet_format_is_short(msg->type)) {
		if (packet.header[1] == 0) {
			mipidsi_send_cmd(priv->MIPIx, packet.header[0], 0, NULL);
		} else {
			mipidsi_send_cmd(priv->MIPIx, packet.header[0], 1, packet.header + 1);
		}
	} else {
		mipidsi_send_cmd(priv->MIPIx, packet.header[0], packet.payload_length, packet.payload);
	}

	while (!is_transfer_completed()) {
		usleep(1000);
	}
	return OK;
}

struct mipi_dsi_host *mipi_dsi_host_initialize(struct lcd_data *config)
{
	FAR struct amebasmart_mipi_dsi_host_s *priv = NULL;
	priv = &g_dsi_host;
	priv->dsi_host.config = *config;
	mipi_init_helper(priv);
	mipi_dsi_host_register(&priv->dsi_host);
	return (struct mipi_dsi_host *)priv;
}
