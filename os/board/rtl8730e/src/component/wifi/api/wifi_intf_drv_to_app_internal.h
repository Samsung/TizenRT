/**
  ******************************************************************************
  * @file    wifi_intf_drv_to_app_internal.h
  * @author
  * @version
  * @date
  * @brief   The API of this file is for internal use and does not guarantee stability.
  *          If customers using RTOS need to use it, please contact realtek to provide an official API.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __WIFI_CONF_INTERNAL_H
#define __WIFI_CONF_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  get country code info
 * @param[in]  table: Pointer to the currently obtained country code table
 * @return  RTW_SUCCESS or RTW_ERROR
 */
u8 wifi_get_countrycode(struct country_code_table_t *table);

/**
 * @brief  Set the current Media Access Control (MAC) address
 *	(or Ethernet hardware address) of the 802.11 device.
 * @param[in]  idx: Set STA or SoftAP mac address.
 * @param[in]  mac: Pointer to the mac address.
 * @param[in]  efuse: Set mac address to efuse or set to RAM.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  Set to Efuse(efuse = 1): effective after reboot.
 * Set to RAM(efuse = 0): reboot will lose effectiveness. (RECOMMENDED)
 */
int wifi_set_mac_address(int idx, unsigned char *mac, u8 efuse);

/**
 * @brief  for wifi certification of ETSI mode
 * @param[in]  edcca_mode: 0: normal; 1: ETSI; 2 Janpan; 9: Disable
 * @return  null.
 */
void wifi_set_edcca_mode(u8 edcca_mode);

/**
 * @brief  Set PTA type when coex.
 * @param[in]  type: the PTA type(PTA_BT/PTA_WIFI/PTA_AUTO).
 * @param[in]  role, PTA_HOST_BT / PTA_HOST_WIFI
 * @param[in]  process, CALIBRATION_START / CALIBRATION_STOP / COMMON_ACTION
 * @return  Null.
 */
void wifi_btcoex_set_pta(enum pta_type type, u8 role, u8 process);

/**
 * @brief  Set BTS0 or BTS1.
 * @param[in]  bt_ant: the BT ant(BTS0/BTS1).
 * @return  Null.
 */
void wifi_btcoex_set_bt_ant(u8 bt_ant);

/**
  * @brief Set vendor info to coex
  * @param[in]  p_vendor_info: vendor info
  * @param[in]  length: vendor info length
  * @retval  Null
  */
void wifi_btcoex_vendor_info_set(void *p_vendor_info, u8 length);

/**
  * @brief Write BT RFK  data to RFC
  * @param  calibration data
  * @retval  BT RFK result(1:success  0:failed)
  */
int wifi_btcoex_bt_rfk(struct bt_rfk_param *rfk_param);

/**
  * @brief Notify extwpan data to COEX
  * @param[in]  type: type for exewpan send
  * @param[in]  data; data pointer
  * @param[in]  data_len; data length
  * @retval  result(1:success  0:failed)
  */
int wifi_extchip_coex_notify(u32 type, u32 data, u32 data_len);

/**
  * @brief Zigbee Call WL RFK
  * @param  void
  * @retval  ZBC RFK result(0:success  -1:failed)
  */
int wifi_zigbee_coex_zb_rfk(void);

/**
 * @brief  Set global variable wifi_wpa_mode.
 * @param[in]  wpa_mode:
 	(WPA_AUTO_MODE,WPA_ONLY_MODE, WPA2_ONLY_MODE,
	WPA3_ONLY_MODE,WPA_WPA2_MIXED_MODE, WPA2_WPA3_MIXED_MODE).
 * @return  0:success  -1:fail.
 */
int wifi_set_wpa_mode(enum rtw_wpa_mode_type wpa_mode);

/**
 * @brief  Dynamically modify the working mode of pmf.
 * @param[in]  pmf_mode:
 	0: none
 	1: optional
 	2: required
 * @return  0:success  -1:fail.
 */
int wifi_set_pmf_mode(u8 pmf_mode);

/**
 * @brief  wpa notify wifi driver status of 4-way/2-way handshake.
 * wifi driver will do disconnect and autoreconnect when fail & inform coex
 * @param[in] rtw_wap_4way_status
 * @return  null.
 */
void wifi_wpa_4way_status_indicate(struct rtw_wpa_4way_status *rpt_4way);

/**
 * @brief  for wpa to set key to driver
 * @param[in]  rtw_crypt_info
 * @return  null.
 */
void wifi_wpa_add_key(struct rtw_crypt_info *crypt);

/**
 * @brief  for wpa to set/del/flush pmksa
 * @param[in]  pmksa_ops
 * @return  null.
 */
void wifi_wpa_pmksa_ops(struct rtw_pmksa_ops_t *pmksa_ops);

/**
 * @brief  for owe to set DH params
 * @param[in]  owe_param
 * @return  null.
 */
void wifi_set_owe_param(struct rtw_owe_param_t *owe_param);

/**
 * @brief  Set Management Frame Protection Support.
 * @param[in] value:
 * 	0) NO_MGMT_FRAME_PROTECTION: not support
 * 	1) MGMT_FRAME_PROTECTION_OPTIONAL: capable
 * 	2) MGMT_FRAME_PROTECTION_REQUIRED: required
 * @return  RTW_SUCCESS if setting Management Frame Protection Support successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_mfp_support(unsigned char value);


/**
 * @brief  Set group id of SAE.
 * @param[in] value:group id which want to be set
 * @return  RTW_SUCCESS if setting is successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_group_id(unsigned char value);

/**
 * @brief  for wpa supplicant indicate sae status.
 * @param[in] wlan_idx:STA_WLAN_IDX or SOFTAP_WLAN_IDX.
 * @param[in] status:sae_status which will be indicated.
 * @param[in] mac_addr:pointer of sae peer's mac_addr .
 * @return  RTW_SUCCESS if setting is successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_sae_status_indicate(u8 wlan_idx, u16 status, u8 *mac_addr);

/**
 * @brief  send raw frame
 * @param[in]  raw_data_desc: the pointer of struct _raw_data_desc_t,
 * 	which describe related information, include the pointer of raw frame and so on.
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_send_mgnt(struct _raw_data_desc_t *raw_data_desc);

/**
 * @brief  for wpa supplicant indicate ft status.
 * @param[in] status:ft_status which will be indicated.
 * @return  null.
 */
int wifi_ft_status_indicate(struct rtw_kvr_param_t *kvr_param, u16 status);

/**
 * @brief  enable or disable pmk cache.
 * @param[in] value:1 for enable, 0 for disable
 * @return  RTW_SUCCESS if setting is successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_pmk_cache_enable(unsigned char value);

/**
 * @brief  Set the wps phase
 * 	wps: Wi-Fi Protected Setup
 * @param[in]  is_trigger_wps: to trigger wps function or not
 * 	is_trigger_wps value should only be 0 or 1
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_wps_phase(unsigned char is_trigger_wps);

/**
 * @brief  Set the eap phase to wifi driver
 * @param[in]  is_trigger_eap: to trigger 802.1X EAP function or not
 * @return    RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_eap_phase(unsigned char is_trigger_eap);

/**
 * @brief  get the current eap phase from wifi driver
 * @param[in]  None
 * @return  1: eap_phase is equal to 1, indicate 802.1X EAP is triggered
 * @return  0: eap_phase is equal to 0, indicate 802.1X EAP is not triggered
 */
unsigned char wifi_get_eap_phase(void);

/**
 * @brief  Set the current eap authentication method to wifi driver
 * @param[in]  eap_method: the current 802.1x EAP authentication method
 * @return    RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_eap_method(unsigned char eap_method);

/**
 * @brief  send 802.1X EAP frame
 * @param[in]  wlan_idx: the wlan interface index,
 * 	wlan_idx should be STA_WLAN_INDEX or SOFTAP_WLAN_INDEX
 * @param[in]  buf: the pointer to buf which store the EAP frame
 * @param[in]  buf_len: the length of buf
 * @param[in]  flags: reserved, set to 0
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_if_send_eapol(unsigned char wlan_idx, char *buf, __u16 buf_len, __u16 flags);


void wifi_btcoex_bt_hci_notify(uint8_t *pdata, uint16_t len, uint8_t type);

#ifdef __cplusplus
}
#endif

#endif

