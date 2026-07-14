/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include "platform_autoconf.h"
#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <basic_types.h>
#include <bt_api_config.h>
#include <bt_vendor_config.h>
#include <rtk_bt_gap.h>
#include "rtk_bt_def.h"
#include "rtk_bt_vendor.h"
#include "rtk_bt_le_gap.h"
#include "bt_debug.h"
#include "rtk_bt_common.h"

#if defined(RTK_BT_HC_CLOCK_OFFSET_SUPPORT) && RTK_BT_HC_CLOCK_OFFSET_SUPPORT
#include "ameba_soc.h"
static rtk_bt_vendor_free_run_clock_t hc_free_run_clock = {0};
#endif

#if defined(RTK_BT_GET_LE_ISO_SYNC_REF_AP_INFO_SUPPORT) && RTK_BT_GET_LE_ISO_SYNC_REF_AP_INFO_SUPPORT
static rtk_bt_vendor_sync_ref_ap_info_t ref_ap_info = {0};
#endif

#if defined(RTK_BLE_GET_SLAVE_CONN_CLOCK_SUPPORT) && RTK_BLE_GET_SLAVE_CONN_CLOCK_SUPPORT
static rtk_bt_vendor_conn_clock_info_t slave_clock_info = {0};
#endif

_WEAK void hci_platform_external_fw_log_pin(void)
{
	BT_LOGE("External fw log pin is not supported!\r\n");
}

_WEAK void hci_platform_controller_reset(void)
{
	BT_LOGE("Controller reset is not supported!\r\n");
}

_WEAK void bt_power_off(void)
{
	BT_LOGE("BT power off is not supported!\r\n");
}

_WEAK void hci_platform_set_tx_power_gain_index(uint32_t index)
{
	(void)index;
	BT_LOGE("Set tx power gain is not supported!\r\n");
}

_WEAK void hci_platform_set_antenna(uint8_t ant)
{
	(void)ant;
	BT_LOGE("Set antenna is not supported!\r\n");
}

_WEAK void hci_platform_debug_enable(void)
{
	BT_LOGE("HCI debug is not supported!\r\n");
}

_WEAK void hci_platform_debug_port_mask_enable(uint8_t bt_sel, uint32_t bt_bdg_mask)
{
	(void)bt_sel;
	(void)bt_bdg_mask;
	BT_LOGE("BT Debug Port enable by mask is not supported!\r\n");
}

_WEAK void hci_platform_debug_port_pad_enable(uint8_t bt_sel, uint8_t bt_dbg_port, char *pad)
{
	(void)bt_sel;
	(void)bt_dbg_port;
	(void)pad;
	BT_LOGE("BT Debug Port enable by pad is not supported!\r\n");
}

_WEAK void hci_platform_debug_port_shift(uint8_t original, uint8_t mapping)
{
	(void)original;
	(void)mapping;
	BT_LOGE("BT Debug Port shift is not supported!\r\n");
}

_WEAK void hci_platform_gpio_enable(uint8_t bt_gpio, char *pad)
{
	(void)bt_gpio;
	(void)pad;
	BT_LOGE("BT GPIO enable is not supported!\r\n");
}

/* -------------------------------- Functions ------------------------------*/

void rtk_bt_controller_power_on(void)
{
	hci_platform_external_fw_log_pin();
	hci_platform_controller_reset();
}

void rtk_bt_controller_power_off(void)
{
	bt_power_off();
}

void rtk_bt_set_bt_tx_power_gain_index(uint32_t index)
{
	hci_platform_set_tx_power_gain_index(index);
}

void rtk_bt_set_bt_antenna(uint8_t ant)
{
	hci_platform_set_antenna(ant);
}

void rtk_bt_hci_debug_enable(void)
{
	hci_platform_debug_enable();
}

void rtk_bt_debug_port_mask_enable(uint8_t bt_sel, uint32_t bt_bdg_mask)
{
	hci_platform_debug_port_mask_enable(bt_sel, bt_bdg_mask);
}

void rtk_bt_debug_port_pad_enable(uint8_t bt_sel, uint8_t bt_dbg_port, char *pad)
{
	hci_platform_debug_port_pad_enable(bt_sel, bt_dbg_port, pad);
}

void rtk_bt_debug_port_shift(uint8_t original, uint8_t mapping)
{
	hci_platform_debug_port_shift(original, mapping);
}

void rtk_bt_gpio_enable(uint8_t bt_gpio, char *pad)
{
	hci_platform_gpio_enable(bt_gpio, pad);
}

uint16_t rtk_bt_set_tx_power(rtk_bt_vendor_tx_power_param_t *tx_power)
{
#if defined(VENDOR_CMD_SET_TX_POWER_SUPPORT) && VENDOR_CMD_SET_TX_POWER_SUPPORT
	rtk_bt_gap_vendor_cmd_param_t param;
	uint8_t data[5] = {0};

	if (0 == tx_power->tx_power_type) {
		data[0] = SUB_CMD_SET_ADV_TX_POWER;
		data[1] = tx_power->adv_tx_power.type;
		data[2] = tx_power->tx_gain;
		param.len = 3;
	} else if (1 == tx_power->tx_power_type) {
		uint8_t conn_id = 0;
		if (RTK_BT_OK != rtk_bt_le_gap_get_conn_id(tx_power->conn_tx_power.conn_handle, &conn_id)) {
			BT_LOGE("%s: conn_handle %d is not connect!\r\n", __func__, tx_power->conn_tx_power.conn_handle);
			return RTK_BT_FAIL;
		}
		data[0] = SUB_CMD_SET_CONN_TX_POWER;
		data[1] = tx_power->conn_tx_power.conn_handle & 0xFF;
		data[2] = (tx_power->conn_tx_power.conn_handle >> 8) & 0xFF;
		data[3] = tx_power->conn_tx_power.is_reset;
		data[4] = tx_power->tx_gain;
		param.len = 5;
	} else {
		BT_LOGE("%s: wrong tx power type!\r\n", __func__);
		return RTK_BT_FAIL;
	}

	param.op = VENDOR_CMD_LE_EXTENSION_FEATURE_OPCODE;
	param.cmd_param = data;

	return rtk_bt_gap_vendor_cmd_req(&param);
#else
	(void)tx_power;
	BT_LOGE("Set tx power is not supported!\r\n");
	return RTK_BT_FAIL;
#endif
}

#if defined(RTK_BT_HC_CLOCK_OFFSET_SUPPORT) && RTK_BT_HC_CLOCK_OFFSET_SUPPORT
void bt_vendor_process_tx_frame(uint8_t type, uint8_t *pdata, uint16_t len)
{
#if defined(VENDOR_CMD_FREE_RUN_CLOCK_LATCH_SUPPORT) && VENDOR_CMD_FREE_RUN_CLOCK_LATCH_SUPPORT
	if (!hc_free_run_clock.enable) {
		return;
	}

	if (type != 0x01/*HCI_CMD*/ || !pdata || len != 4) {
		return;
	}

	uint16_t opcode = (uint16_t)((pdata[1] << 8) | pdata[0]);
	uint8_t subcode = pdata[3];
	if (VENDOR_CMD_FREE_RUN_CLOCK_LATCH_OPCODE == opcode && SUB_CMD_FREE_RUN_CLOCK_LATCH == subcode) {
		BT_LOGD("Get host free run clock\r\n");
		hc_free_run_clock.host_free_run_clock[hc_free_run_clock.index] = (uint64_t)DTimestamp_Get();
	}
#else
	(void)type;
	(void)pdata;
	(void)len;
#endif
}

rtk_bt_vendor_free_run_clock_t *rtk_bt_get_hc_free_run_clock(void)
{
	return &hc_free_run_clock;
}

static int64_t _get_middle(int64_t a, int64_t b, int64_t c)
{
	return (a > b) ? (b > c ? b : (a > c ? c : a)) : (a > c ? a : (b > c ? c : b));
}

uint16_t rtk_bt_get_hc_clock_offset(int64_t *p_offset)
{
#if defined(VENDOR_CMD_FREE_RUN_CLOCK_LATCH_SUPPORT) && VENDOR_CMD_FREE_RUN_CLOCK_LATCH_SUPPORT
	if (!p_offset) {
		BT_LOGE("p_offset is NULL!\r\n");
		return RTK_BT_FAIL;
	}

	uint8_t data[2] = {0};
	rtk_bt_gap_vendor_cmd_param_t param;
	uint16_t ret = 0;

	data[0] = SUB_CMD_FREE_RUN_CLOCK_LATCH_ENABLE;
	data[1] = 0x1;

	param.op = VENDOR_CMD_FREE_RUN_CLOCK_LATCH_OPCODE;
	param.len = 2;
	param.cmd_param = data;

	memset(&hc_free_run_clock, 0, sizeof(rtk_bt_vendor_free_run_clock_t));

	if (false == osif_sem_create(&hc_free_run_clock.sem, 0, 1)) {
		BT_LOGE("hc_free_run_clock.sem create fail!\r\n");
		return RTK_BT_FAIL;
	}
	hc_free_run_clock.enable = true;

	BT_LOGD("Free run clock latch enable\r\n");
	ret = rtk_bt_gap_vendor_cmd_req(&param);
	if (ret) {
		BT_LOGE("rtk_bt_gap_vendor_cmd_req fail!\r\n");
		ret = RTK_BT_FAIL;
		goto out;
	}

	if (osif_sem_take(hc_free_run_clock.sem, 0xFFFFFFFF) == false) {
		BT_LOGE("hc_free_run_clock.sem take fail!\r\n");
		ret = RTK_BT_FAIL;
		goto out;
	}

	if (hc_free_run_clock.index != 2) {
		BT_LOGE("hc_free_run_clock.index != 2!\r\n");
		ret = RTK_BT_FAIL;
		goto out;
	}

	if (hc_free_run_clock.host_free_run_clock[1] < hc_free_run_clock.host_free_run_clock[0]) {                    //host_free_run_clock[1] overflow
		hc_free_run_clock.host_free_run_clock[1] += 0xFFFFFFFF;
		hc_free_run_clock.host_free_run_clock[2] += 0xFFFFFFFF;
	} else if (hc_free_run_clock.host_free_run_clock[2] < hc_free_run_clock.host_free_run_clock[1]) {             //host_free_run_clock[2] overflow
		hc_free_run_clock.host_free_run_clock[2] += 0xFFFFFFFF;
	}

	if (hc_free_run_clock.controller_free_run_clock[1] < hc_free_run_clock.controller_free_run_clock[0]) {        //controller_free_run_clock[1] overflow
		hc_free_run_clock.controller_free_run_clock[1] += 0xFFFFFFFF;
		hc_free_run_clock.controller_free_run_clock[2] += 0xFFFFFFFF;
	} else if (hc_free_run_clock.controller_free_run_clock[2] < hc_free_run_clock.controller_free_run_clock[1]) { //controller_free_run_clock[2] overflow
		hc_free_run_clock.controller_free_run_clock[2] += 0xFFFFFFFF;
	}

	for (uint8_t i = 0; i < 3; i++) {
		hc_free_run_clock.hc_clock_offset[i] = hc_free_run_clock.host_free_run_clock[i] - hc_free_run_clock.controller_free_run_clock[i];
	}

	*p_offset = _get_middle(hc_free_run_clock.hc_clock_offset[0], hc_free_run_clock.hc_clock_offset[1], hc_free_run_clock.hc_clock_offset[2]);

	ret = RTK_BT_OK;

out:
	hc_free_run_clock.enable = false;
	osif_sem_delete(hc_free_run_clock.sem);

	return ret;
#else
	(void)p_offset;
	BT_LOGE("Get host controller free run clock offset is not supported!\r\n");
	return RTK_BT_FAIL;
#endif
}
#endif

#if defined(RTK_BT_GET_LE_ISO_SYNC_REF_AP_INFO_SUPPORT) && RTK_BT_GET_LE_ISO_SYNC_REF_AP_INFO_SUPPORT
rtk_bt_vendor_sync_ref_ap_info_t *rtk_bt_get_ref_ap_info(void)
{
	return &ref_ap_info;
}

uint16_t rtk_bt_get_le_iso_sync_ref_ap_info(uint16_t conn_handle, uint8_t dir, rtk_bt_le_iso_sync_ref_ap_info_t *p_info)
{
#if defined(VENDOR_CMD_GET_LE_ISO_SYNC_REF_AP_INFO_SUPPORT) && VENDOR_CMD_GET_LE_ISO_SYNC_REF_AP_INFO_SUPPORT
	uint8_t data[3] = {0};
	rtk_bt_gap_vendor_cmd_param_t param;
	uint16_t ret = 0;

	if (!p_info) {
		BT_LOGE("p_info is NULL!\r\n");
		return RTK_BT_FAIL;
	}
	memcpy((void *)data, (void *)&conn_handle, sizeof(uint16_t));
	data[2] = dir;
	param.op = VENDOR_CMD_GET_LE_ISO_SYNC_REF_AP_INFO_OPCODE;
	param.len = 3;
	param.cmd_param = data;
	/* init sync parameter */
	memset(&ref_ap_info, 0, sizeof(rtk_bt_vendor_sync_ref_ap_info_t));
	if (false == osif_sem_create(&ref_ap_info.sem, 0, 1)) {
		BT_LOGE("ref_ap_info.sem create fail!\r\n");
		return RTK_BT_FAIL;
	}
	ref_ap_info.enable = true;
	BT_LOGD("Get le iso sync anchor point information \r\n");
	ret = rtk_bt_gap_vendor_cmd_req(&param);
	if (ret) {
		BT_LOGE("rtk_bt_gap_vendor_cmd_req fail!\r\n");
		ret = RTK_BT_FAIL;
		goto out;
	}
	if (osif_sem_take(ref_ap_info.sem, 0xFFFFFFFF) == false) {
		BT_LOGE("ref_ap_info.sem take fail!\r\n");
		ret = RTK_BT_FAIL;
		goto out;
	}
	memcpy((void *)p_info, (void *)&ref_ap_info.info, sizeof(rtk_bt_le_iso_sync_ref_ap_info_t));

out:
	ref_ap_info.enable = false;
	osif_sem_delete(ref_ap_info.sem);

	return ret;
#else
	(void)dir;
	(void)conn_handle;
	(void)p_info;
	BT_LOGE("Get le iso ap info is not supported!\r\n");
	return RTK_BT_FAIL;
#endif
}
#endif

#if defined(RTK_BLE_GET_SLAVE_CONN_CLOCK_SUPPORT) && RTK_BLE_GET_SLAVE_CONN_CLOCK_SUPPORT
rtk_bt_vendor_conn_clock_info_t *rtk_bt_get_slave_clock_info(void)
{
	return &slave_clock_info;
}

uint16_t rtk_bt_le_get_slave_conn_clock(uint16_t conn_handle, rtk_bt_le_conn_clock_info_t *clock_info)
{
#if defined(VENDOR_CMD_READ_ACC_HIT_COUNTER_SUPPORT) && VENDOR_CMD_READ_ACC_HIT_COUNTER_SUPPORT
	uint8_t data[3] = {0};
	uint16_t ret = RTK_BT_OK;
	rtk_bt_gap_vendor_cmd_param_t param = {0};

	if (!clock_info) {
		BT_LOGE("clock_info is NULL!\r\n");
		return RTK_BT_FAIL;
	}
	memset(clock_info, 0, sizeof(clock_info));

	data[0] = SUB_CMD_READ_ACC_HIT_COUNTER;
	U16_TO_LE(&(data[1]), conn_handle);
	param.op = VENDOR_CMD_LE_EXTENSION_FEATURE_OPCODE;
	param.len = 3;
	param.cmd_param = data;

	memset(&slave_clock_info, 0, sizeof(rtk_bt_vendor_conn_clock_info_t));
	if (false == osif_sem_create(&slave_clock_info.sem, 0, 1)) {
		BT_LOGE("slave_clock_info.sem create fail!\r\n");
		return RTK_BT_FAIL;
	}

	ret = rtk_bt_gap_vendor_cmd_req(&param);
	if (ret) {
		BT_LOGE("rtk_bt_gap_vendor_cmd_req fail!\r\n");
		ret = RTK_BT_FAIL;
		goto out;
	}

	if (osif_sem_take(slave_clock_info.sem, 0xFFFFFFFF) == false) {
		BT_LOGE("slave_clock_info.sem take fail!\r\n");
		ret = RTK_BT_FAIL;
		goto out;
	}

	memcpy((void *)clock_info, (void *)&slave_clock_info.info, sizeof(rtk_bt_le_conn_clock_info_t));

out:
	osif_sem_delete(slave_clock_info.sem);
	slave_clock_info.sem = NULL;

	return ret;
#else
	(void)conn_handle;
	(void)clock_info;
	BT_LOGE("Get slave connection link clock is not supported!\r\n");
	return RTK_BT_FAIL;
#endif
}
#endif /* RTK_BLE_GET_SLAVE_CONN_CLOCK_SUPPORT */