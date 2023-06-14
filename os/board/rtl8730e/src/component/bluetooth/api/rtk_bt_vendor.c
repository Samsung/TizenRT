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
#include <bt_ipc_profile_config.h>
#endif
#include <bt_api_config.h>
#if defined(RTK_BLE_TX_SOF_EOF_INDICATION) && RTK_BLE_TX_SOF_EOF_INDICATION
#include "ameba_page0.h"
#include "ameba_soc.h"
#include "rtk_bt_le_gap.h"
#include "rtk_bt_def.h"
#define BT_SCB_IRQ_PRIO   3
typedef void(*p_func_cb)(uint8_t ind);
static p_func_cb g_rtk_bt_scoreboard_isr_cb = NULL;
#endif

_WEAK void hci_platform_set_tx_power_gain_index(uint32_t index)
{
	(void)index;
	printf("Set tx power gain not supported in this platform!\r\n");
}

_WEAK void hci_platform_set_antenna(uint8_t ant)
{
	(void)ant;
	printf("Set antenna not supported in this platform!\r\n");
}

/* -------------------------------- Functions ------------------------------*/

/**
 * @brief     set bt tx power gain index.
 * @param[in] index: tx power gain index.
 * @return    None
 */
void rtk_bt_set_bt_tx_power_gain_index(uint32_t index)
{
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	int *ret = NULL;

	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_VENDOR, RTK_BT_ACT_IPC_SET_TX_POWER_GAIN,
									   (uint8_t *)&index, sizeof(uint32_t));
	osif_mem_free(ret);
#else
	hci_platform_set_tx_power_gain_index(index);
#endif
}

/**
 * @brief     set bt tx power gain index.
 * @param[in] index: tx power gain index.
 * @return    None
 */
void rtk_bt_set_bt_antenna(uint8_t ant)
{
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	int *ret = NULL;

	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_VENDOR, RTK_BT_ACT_IPC_SET_BT_ANTENNA,
										(uint8_t *)&ant, 1);
	osif_mem_free(ret);
#else
	hci_platform_set_antenna(ant);
#endif
}

#if !defined(CONFIG_BT_AP) || (!CONFIG_BT_AP)
extern void bt_hci_debug_enable(void);
#endif

/**
 * @brief     enable hci debug
 * @return    None
 */
void rtk_bt_hci_debug_enable(void)
{
#if defined(CONFIG_BT_AP) && CONFIG_BT_AP
	int *ret = NULL;

	ret = bt_ipc_api_host_message_send(RTK_BT_IPC_VENDOR, RTK_BT_ACT_IPC_SET_HCI_DEBUG_ENABLE, 
										NULL, 0);
	osif_mem_free(ret);
#else
	bt_hci_debug_enable();
#endif
}

#if defined(RTK_BLE_TX_SOF_EOF_INDICATION) && RTK_BLE_TX_SOF_EOF_INDICATION
void rtk_bt_scoreboard_isr_handler(void)
{
    rtk_bt_le_sof_eof_ind_t ind = (rtk_bt_le_sof_eof_ind_t)0;
	uint32_t value = 0;
	if (HAL_READ32(WIFI_REG_BASE, REG_FSISR_V1) & BIT_FS_BTON_STS_UPDATE_INT) {
		//Clear BT dedicated SCB int ISR, MAC Reg 0x44[0]
		HAL_WRITE32(WIFI_REG_BASE, REG_FSISR_V1, BIT_FS_BTON_STS_UPDATE_INT);
		value = HAL_READ32(WIFI_REG_BASE, REG_SCOREBOARD_RD_BT2WL);
		//SCOREBOARD: bit 17: 0：SOF, 1：EOF; bit 17 is valid when bit 18 is 1 
		//			  bit 18: 0:disable, 1:enable
		//printf("[KM4] BT2WL dedicated SCB int data is: 0x%x\r\n ",value);
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
            	printf("g_rtk_bt_scoreboard_isr_cb is not defined\r\n");			
		}
	}
}
static bool rtk_bt_scoreboard_isr_en(void)
{
    bool need_reset = false;
	uint32_t value = 0;

	/****BT_SCB_IRQ (67# NP/AP INT VECTOR)*****/
	if(false == InterruptRegister((IRQ_FUN)rtk_bt_scoreboard_isr_handler, BT_SCB_IRQ, (uint32_t)NULL, BT_SCB_IRQ_PRIO)) {
		printf("InterruptRegister for BT_SCB_IRQ fail\r\n");
		return false;
	}

	InterruptEn(BT_SCB_IRQ, BT_SCB_IRQ_PRIO);

    //set 0x42008218[5] = 1
    value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP1);
    if((value & BIT5) == 0) {
        HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP1, value | APBPeriph_WMAC_CLOCK);
        value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP1);
        printf("%s set bit 5, 0x%x = 0x%x\r\n",__func__,(unsigned int)(SYSTEM_CTRL_BASE_LP + REG_LSYS_CKE_GRP1), (unsigned int)value);  
        need_reset = true; 
    } 

	//Clear BT dedicated SCB int ISR, MAC Reg 0x44[0]
	HAL_WRITE32(WIFI_REG_BASE, REG_FSISR_V1, BIT_FS_BTON_STS_UPDATE_INT);

    //Enable BT dedicated SCB int IMR, MAC Reg 0x40[0]
	value = HAL_READ32(WIFI_REG_BASE, REG_FSIMR_V1);
	HAL_WRITE32(WIFI_REG_BASE, REG_FSIMR_V1, value | BIT_FS_BTON_STS_UPDATE_INT_EN);
	value = HAL_READ32(WIFI_REG_BASE, REG_FSIMR_V1);
	printf("%s Enable BT dedicated SCB int IMR, 0x%x = 0x%x\r\n",__func__,(unsigned int)(WIFI_REG_BASE+REG_FSIMR_V1), (unsigned int)value);

    if(need_reset) {
        value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP1);
        HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP1, value | APBPeriph_WMAC_CLOCK);
        value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP1);
        printf("%s reset bit 5, 0x%x = 0x%x\r\n",__func__,(unsigned int)(SYSTEM_CTRL_BASE_LP + REG_LSYS_CKE_GRP1), (unsigned int)value); 
    }

	return true;
}

uint16_t rtk_bt_le_sof_eof_ind(uint16_t conn_handle, uint8_t enable, void * cb)
{
	uint16_t ret = RTK_BT_OK;
	uint8_t data[3] = {0};	
	rtk_bt_le_gap_vendor_cmd_param_t param = {0};
	
	data[0] = enable;			//0:disable, 1:enable
	data[1] = (conn_handle) & 0xFF;	
	data[2] = (conn_handle >> 8) & 0xFF;

	param.op = 0xFC52;//HCI_LE_VENDOR_SOF_EOF_CTRL;
	param.len = 3;
	param.cmd_param = data;
	
	//set the vendor cmd
	ret = rtk_bt_le_gap_vendor_cmd_req(&param);
	if(ret != RTK_BT_OK) {
		printf("%s rtk_bt_le_gap_vendor_cmd_req fail,ret = 0x%x\r\n",__func__, ret);
		return ret; 
	}

	//enable bt scoreboard isr
	if(false == rtk_bt_scoreboard_isr_en())
		return RTK_BT_FAIL;

	//save the callback function
	g_rtk_bt_scoreboard_isr_cb = cb;
	
	return RTK_BT_OK;
}

#endif