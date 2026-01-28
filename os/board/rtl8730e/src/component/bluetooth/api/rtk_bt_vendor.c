/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */
#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <basic_types.h>
#include <platform_opts_bt.h>
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
#include <bt_ipc_host_api.h>
#include <bt_ipc_host_drc.h>
#include <bt_ipc_profile_config.h>
#endif
#include <bt_api_config.h>
#include <bt_vendor_config.h>
#include <rtk_bt_gap.h>
#include "rtk_bt_def.h"
#include "rtk_bt_vendor.h"
#include "rtk_bt_le_gap.h"
#if defined(RTK_BLE_TX_SOF_EOF_INDICATION) && RTK_BLE_TX_SOF_EOF_INDICATION
#include "wifi_reg_page0.h"
#include "ameba_soc.h"
#define BT_SCB_IRQ_PRIO   3
typedef void(*p_func_cb)(uint8_t ind);
static p_func_cb g_rtk_bt_scoreboard_isr_cb = NULL;
#endif

#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
extern bool rtk_bt_check_act_direct_calling(uint8_t group, uint8_t act_code);
#endif

_WEAK void hci_platform_set_tx_power_gain_index(uint32_t index)
{
	(void)index;
	dbg("Set tx power gain is not supported on this platform!\r\n");
}

_WEAK void hci_platform_set_antenna(uint8_t ant)
{
	(void)ant;
	dbg("Set antenna is not supported on this platform!\r\n");
}

_WEAK void hci_debug_enable(void)
{
	dbg("HCI debug is not supported on this platform!\r\n");
}

/* -------------------------------- Functions ------------------------------*/

void rtk_bt_set_bt_tx_power_gain_index(uint32_t index)
{
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	int *ret = NULL;

	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_VENDOR, RTK_BT_ACT_IPC_SET_TX_POWER_GAIN)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_VENDOR, RTK_BT_ACT_IPC_SET_TX_POWER_GAIN,
										(uint8_t *)&index, sizeof(uint32_t));
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_VENDOR, RTK_BT_ACT_IPC_SET_TX_POWER_GAIN,
										(uint8_t *)&index, sizeof(uint32_t));
	}
	osif_mem_free(ret);
#else
	hci_platform_set_tx_power_gain_index(index);
#endif
}

void rtk_bt_set_bt_antenna(uint8_t ant)
{
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	int *ret = NULL;

	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_VENDOR, RTK_BT_ACT_IPC_SET_BT_ANTENNA)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_VENDOR, RTK_BT_ACT_IPC_SET_BT_ANTENNA,
										(uint8_t *)&ant, 1);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_VENDOR, RTK_BT_ACT_IPC_SET_BT_ANTENNA,
										(uint8_t *)&ant, 1);
	}
	osif_mem_free(ret);
#else
	hci_platform_set_antenna(ant);
#endif
}

void rtk_bt_hci_debug_enable(void)
{
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	int *ret = NULL;

	if (true == rtk_bt_check_act_direct_calling(RTK_BT_IPC_VENDOR, RTK_BT_ACT_IPC_SET_HCI_DEBUG_ENABLE)) {
		ret = bt_ipc_drc_host_message_send(RTK_BT_IPC_VENDOR, RTK_BT_ACT_IPC_SET_HCI_DEBUG_ENABLE,
										NULL, 0);
	} else {
		ret = bt_ipc_api_host_message_send(RTK_BT_IPC_VENDOR, RTK_BT_ACT_IPC_SET_HCI_DEBUG_ENABLE,
										NULL, 0);
	}
	osif_mem_free(ret);
#else
	hci_debug_enable();
#endif
}

void rtk_bt_sleep_mode(unsigned int mode)
{
#if defined(VENDOR_CMD_BT_SLEEP_MODE_SUPPORT) && VENDOR_CMD_BT_SLEEP_MODE_SUPPORT
	uint8_t data[4] = {0};
	rtk_bt_gap_vendor_cmd_param_t param;

	data[0] = mode & 0xFF;			//0:lps, 1:dlps, 2:active
	data[1] = (mode >> 8) & 0xFF;	//0:log off, 1:log on
	data[2] = (mode >> 16) & 0xFF;	//minimum adv slot[LSB] to enter sleep
	data[3] = (mode >> 24) & 0xFF;	//minimum adv slot[MSB] to enter sleep

	param.op = VENDOR_CMD_BT_SLEEP_MODE_OPCODE;
	param.len = 4;
	param.cmd_param = data;

	rtk_bt_gap_vendor_cmd_req(&param);
#else
	(void)mode;
	dbg("[Vendor CMD] BT sleep mode is not supported on this platform!\r\n");
#endif
}

void rtk_bt_read_register(unsigned int address)
{
#if defined(VENDOR_CMD_READ_BT_REGISTER_SUPPORT) && VENDOR_CMD_READ_BT_REGISTER_SUPPORT
	uint8_t data[5] = {0};
	rtk_bt_gap_vendor_cmd_param_t param;

	data[0] = 0x10;
	memcpy((void *)&data[1], (void *)&address, 4);

	param.op = VENDOR_CMD_READ_BT_REGISTER_OPCODE;
	param.len = 5;
	param.cmd_param = data;

	rtk_bt_gap_vendor_cmd_req(&param);
#else
	(void)address;
	dbg("[Vendor CMD] BT read register is not supported on this platform!\r\n");
#endif
}

uint16_t rtk_bt_set_tx_power(rtk_bt_vendor_tx_power_param_t *tx_power)
{
#if defined(VENDOR_CMD_SET_TX_POWER_SUPPORT) && VENDOR_CMD_SET_TX_POWER_SUPPORT
	rtk_bt_gap_vendor_cmd_param_t param;
	uint8_t data[5] = {0};

	if(0 == tx_power->tx_power_type){
		data[0] = SUB_CMD_SET_ADV_TX_POWER;
		data[1] = tx_power->adv_tx_power.type;
		data[2] = tx_power->tx_gain;
		param.len = 3;
	} else if(1 == tx_power->tx_power_type) {
		uint8_t conn_id = 0;
		if(RTK_BT_OK != rtk_bt_le_gap_get_conn_id(tx_power->conn_tx_power.conn_handle, &conn_id)) {
			dbg("%s: conn_handle %d is not connect!\r\n", __func__, tx_power->conn_tx_power.conn_handle);
			return RTK_BT_FAIL;
		}
		data[0] = SUB_CMD_SET_CONN_TX_POWER;
		data[1] = tx_power->conn_tx_power.conn_handle & 0xFF;
		data[2] = (tx_power->conn_tx_power.conn_handle >> 8) & 0xFF;
		data[3] = tx_power->conn_tx_power.is_reset;
		data[4] = tx_power->tx_gain;
		param.len = 5;
	} else {
		dbg("%s: wrong tx power type!\r\n", __func__);
		return RTK_BT_FAIL;
	}

	param.op = VENDOR_CMD_SET_TX_POWER_OPCODE;
	param.cmd_param = data;

	return rtk_bt_gap_vendor_cmd_req(&param);
#else
	(void)tx_power;
	dbg("[Vendor CMD] BT set tx power is not supported on this platform!\r\n");
	return RTK_BT_FAIL;
#endif
}

#if defined(RTK_BLE_TX_SOF_EOF_INDICATION) && RTK_BLE_TX_SOF_EOF_INDICATION
uint16_t rtk_bt_le_sof_eof_ctrl(uint16_t conn_handle, uint8_t enable)
{
#if defined(VENDOR_CMD_SOF_EOF_CTRL_SUPPORT) && VENDOR_CMD_SOF_EOF_CTRL_SUPPORT
	uint8_t data[3] = {0};
	rtk_bt_gap_vendor_cmd_param_t param = {0};

	data[0] = enable;			//0:disable, 1:enable
	data[1] = (conn_handle) & 0xFF;
	data[2] = (conn_handle >> 8) & 0xFF;

	param.op = VENDOR_CMD_SOF_EOF_CTRL_OPCODE;
	param.len = 3;
	param.cmd_param = data;

	return rtk_bt_gap_vendor_cmd_req(&param);
#else
	(void)conn_handle;
	(void)enable;
	dbg("[Vendor CMD] BT SOF and EOF control is not supported on this platform!\r\n");
	return RTK_BT_FAIL;
#endif
}

void rtk_bt_scoreboard_isr_handler(void)
{
	rtk_bt_le_sof_eof_ind_t ind = RTK_BT_LE_TX_SOF;
	uint32_t value = 0;
	if (HAL_READ32(WIFI_REG_BASE, REG_FSISR_V1) & BIT_FS_BTON_STS_UPDATE_INT) {
		//Clear BT dedicated SCB int ISR, MAC Reg 0x44[0]
		HAL_WRITE32(WIFI_REG_BASE, REG_FSISR_V1, BIT_FS_BTON_STS_UPDATE_INT);
		value = HAL_READ32(WIFI_REG_BASE, REG_SCOREBOARD_RD_BT2WL);
		//SCOREBOARD: bit 17: 0:SOF, 1:EOF; bit 17 is valid when bit 18 is 1
		//			  bit 18: 0:disable, 1:enable
		//dbg("[KM4] BT2WL dedicated SCB int data is: 0x%x\r\n", value);
		if(value & BIT18) {
			if(value & BIT17) {
				ind = RTK_BT_LE_TX_EOF;
			}
			else {
				ind = RTK_BT_LE_TX_SOF;
			}
			if(g_rtk_bt_scoreboard_isr_cb)
				g_rtk_bt_scoreboard_isr_cb((uint8_t)ind);
			else
				dbg("g_rtk_bt_scoreboard_isr_cb is not defined\r\n");
		}
	}
}

//extern int wifi_set_ips_internal(u8 enable);
static bool need_reset_wmac_clk = false, need_reset_wlon = false;
static bool rtk_bt_scoreboard_isr_enable(void)
{
	uint32_t value = 0;

	/****BT_SCB_IRQ (67# NP/AP INT VECTOR)*****/
	if(false == InterruptRegister((IRQ_FUN)rtk_bt_scoreboard_isr_handler, BT_SCB_IRQ, (uint32_t)NULL, BT_SCB_IRQ_PRIO)) {
		dbg("InterruptRegister for BT_SCB_IRQ fail\r\n");
		return false;
	}
	InterruptEn(BT_SCB_IRQ, BT_SCB_IRQ_PRIO);

	//value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_HSYS_HP_CKE);
	//dbg("%s REG_HSYS_HP_CKE 0x%x = 0x%x\r\n",__func__,(unsigned int)(SYSTEM_CTRL_BASE_LP + REG_HSYS_HP_CKE), (unsigned int)value);
#if 1 //FIXME: use leave ips instead now
	//set 0x42008218[5] = 1, APBPeriph_WMAC_CLOCK
	value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP1);
	if((value & BIT5) == 0) {
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP1, value | APBPeriph_WMAC_CLOCK);
		value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP1);
		dbg("%s set APBPeriph_WMAC_CLOCK, 0x%x = 0x%x\r\n",__func__,(unsigned int)(SYSTEM_CTRL_BASE_LP + REG_LSYS_CKE_GRP1), (unsigned int)value);
		need_reset_wmac_clk = true;
	}

	//set 0x42008208[7] = 1, APBPeriph_WLON,enable WLON function for write REG_FSIMR_V1
	value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FEN_GRP0);
	if((value & BIT7) == 0) {
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FEN_GRP0, value | APBPeriph_WLON);
		value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FEN_GRP0);
		dbg("%s set APBPeriph_WLON , 0x%x = 0x%x\r\n",__func__,(unsigned int)(SYSTEM_CTRL_BASE_LP + REG_LSYS_FEN_GRP0), (unsigned int)value);
		need_reset_wlon = true;
	}
#else
	wifi_set_ips_internal(FALSE);
#endif
	//Clear BT dedicated SCB int ISR, MAC Reg 0x44[0]
	HAL_WRITE32(WIFI_REG_BASE, REG_FSISR_V1, BIT_FS_BTON_STS_UPDATE_INT);
	value = HAL_READ32(WIFI_REG_BASE, REG_FSISR_V1);
	dbg("%s Clear BT dedicated SCB int ISR , 0x%x = 0x%x\r\n",__func__,(unsigned int)(WIFI_REG_BASE+REG_FSISR_V1), (unsigned int)value);

	//Enable BT dedicated SCB int IMR, MAC Reg 0x40[0]
	value = HAL_READ32(WIFI_REG_BASE, REG_FSIMR_V1);
	HAL_WRITE32(WIFI_REG_BASE, REG_FSIMR_V1, value | BIT_FS_BTON_STS_UPDATE_INT_EN);
	value = HAL_READ32(WIFI_REG_BASE, REG_FSIMR_V1);
	dbg("%s Enable BT dedicated SCB int IMR , 0x%x = 0x%x\r\n",__func__,(unsigned int)(WIFI_REG_BASE+REG_FSIMR_V1), (unsigned int)value);

	return true;
}


static bool rtk_bt_scoreboard_isr_disable(void)
{
	uint32_t value = 0;

	InterruptDis(BT_SCB_IRQ);
	InterruptUnRegister(BT_SCB_IRQ);

	//Clear BT dedicated SCB int ISR, MAC Reg 0x44[0]
	HAL_WRITE32(WIFI_REG_BASE, REG_FSISR_V1, BIT_FS_BTON_STS_UPDATE_INT);
	value = HAL_READ32(WIFI_REG_BASE, REG_FSISR_V1);
	dbg("%s Clear BT dedicated SCB int ISR , 0x%x = 0x%x\r\n",__func__,(unsigned int)(WIFI_REG_BASE+REG_FSISR_V1), (unsigned int)value);

	//Disable BT dedicated SCB int IMR, MAC Reg 0x40[0] = 0
	value = HAL_READ32(WIFI_REG_BASE, REG_FSIMR_V1);
	HAL_WRITE32(WIFI_REG_BASE, REG_FSIMR_V1, value & ~BIT_FS_BTON_STS_UPDATE_INT_EN);
	value = HAL_READ32(WIFI_REG_BASE, REG_FSIMR_V1);
	dbg("%s Disable BT dedicated SCB int IMR , 0x%x = 0x%x\r\n",__func__,(unsigned int)(WIFI_REG_BASE+REG_FSIMR_V1), (unsigned int)value);

#if 1 //FIXME: use leave ips instead now
	if(need_reset_wlon) {
		value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FEN_GRP0);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FEN_GRP0, value & (~BIT7));
		value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FEN_GRP0);
		dbg("%s reset APBPeriph_WLON, 0x%x = 0x%x\r\n",__func__,(unsigned int)(SYSTEM_CTRL_BASE_LP + REG_LSYS_FEN_GRP0), (unsigned int)value);
	}

	if(need_reset_wmac_clk) {
		value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP1);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP1, value & (~BIT5));
		value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP1);
		dbg("%s reset APBPeriph_WMAC_CLOCK, 0x%x = 0x%x\r\n",__func__,(unsigned int)(SYSTEM_CTRL_BASE_LP + REG_LSYS_CKE_GRP1), (unsigned int)value);
	}
#else
	wifi_set_ips_internal(TRUE);
#endif
	return true;
}

uint16_t rtk_bt_le_sof_eof_ind(uint16_t conn_handle, uint8_t enable, void *cb)
{
	uint16_t ret = RTK_BT_OK;
	uint8_t conn_id = 0;

	if(0 != rtk_bt_le_gap_get_conn_id(conn_handle,&conn_id)) {
		dbg("%s: conn_handle %d is not connect!\r\n", __func__, conn_handle);
		return RTK_BT_FAIL;
	}

	if(enable == 1) {
		if(g_rtk_bt_scoreboard_isr_cb) {
			dbg("bt_scoreboard_isr is enabled!\r\n");
			return RTK_BT_OK;
		}
		//enable bt scoreboard isr
		if(false == rtk_bt_scoreboard_isr_enable())
			return RTK_BT_FAIL;

		//save the callback function
		g_rtk_bt_scoreboard_isr_cb = (p_func_cb)cb;

	} else if(enable == 0) {
		if(g_rtk_bt_scoreboard_isr_cb == NULL) {
			dbg("bt_scoreboard_isr not enable!\r\n");
			return RTK_BT_OK;
		}

		//clear the callback function
		g_rtk_bt_scoreboard_isr_cb = NULL;

		//disable bt scoreboard isr
		if(false == rtk_bt_scoreboard_isr_disable())
			return RTK_BT_FAIL;
	} else {
		dbg("%s unsupport value %d!\r\n",__func__,enable);
		return RTK_BT_ERR_PARAM_INVALID;
	}

	//set the vendor cmd to inform fw start indicate sof and eof
	ret = rtk_bt_le_sof_eof_ctrl(conn_handle, enable);
	if(ret != RTK_BT_OK) {
		dbg("%s rtk_bt_le_sof_eof_ctrl fail, ret = 0x%x\r\n", __func__, ret);
		return ret;
	}

	return RTK_BT_OK;
}

#endif

