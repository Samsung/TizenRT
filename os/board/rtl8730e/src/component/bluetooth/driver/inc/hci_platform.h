/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef _HCI_PLATFORM_H_
#define _HCI_PLATFORM_H_

#include "hci_common.h"
#include "hci_uart.h"
#include "ameba_soc.h"
#include "wifi_conf.h"

#define hci_platform_DOWNLOAD_PATCH
#define hci_platform_MP_RESET_BAUDRATE
#define hci_platform_WRITE_PHY_EFUSE
#define hci_platform_SET_CUT_VER

#ifdef hci_platform_WRITE_PHY_EFUSE
#define HCI_WRITE_PHY_EFUSE_LEN    0x6D
#endif

#define HCI_DEFAULT_LMP_SUBVER     0x8730
#define HCI_PATCH_PROJECT_ID       0x28

#define HCI_BT_COEX_ENABLE         1
#define HCI_BT_COEX_SW_MAILBOX     0

typedef enum
{
	OPCODE_PATCH_SNIPPETS = 0x01,
	OPCODE_DUMMY_HEADER = 0x02,
	OPCODE_SECURITY_HEADER = 0x03,
	OPCODE_OTA_FLAG = 0x04
} SECTION_OPCODE;

/**
* @typedef   bt_ant_t
* @brief     Bluetooth rf path.
*/
typedef enum
{
	ANT_S0 = 0,                      /*!< 0, means bt_rfafe */
	ANT_S1,                          /*!< 1, means wl_rfafe */
} bt_ant_t;

void hci_platform_get_baudrate(uint8_t *baudrate, uint8_t len, uint8_t init_or_work);
void hci_platform_cfg_bd_addr(uint8_t *bdaddr);
void hci_platform_record_chipid(uint8_t chipid);
void hci_platform_dl_patch_done(void);
uint8_t hci_platform_set_baudrate(uint8_t init_or_work);
uint8_t hci_platform_dl_patch_init(void);
uint8_t hci_platform_get_patch_cmd_len(uint8_t *cmd_len);
uint8_t hci_platform_get_patch_cmd_buf(uint8_t *cmd_buf, uint8_t cmd_len);
int hci_platform_get_write_phy_efuse_data(uint8_t *data, uint8_t len);
uint8_t hci_platform_get_rom_ver(void);
bool hci_platform_check_lmp_subver(uint16_t lmp_subver);

uint8_t hci_platform_init(void);
uint8_t hci_platform_deinit(void);

void hci_platform_bt_log_init(void);
void hci_platform_bt_log_deinit(void);
void hci_platform_bt_fw_log_open(void);
void hci_platform_bt_fw_log_close(void);
void hci_platform_bt_trace_log_open(void);
void hci_platform_bt_trace_log_close(void);

bool rtk_bt_pre_enable(void);
bool rtk_bt_post_enable(void);
uint8_t hci_platform_check_mp(void);
#endif