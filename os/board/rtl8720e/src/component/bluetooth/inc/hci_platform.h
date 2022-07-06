/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef _HCI_PLATFORM_H_
#define _HCI_PLATFORM_H_

#include "hci_common.h"
#include "./hci_uart.h"
#include "ameba_soc.h"
#include "wifi_conf.h"

//#define hci_platform_START_IQK
#define hci_platform_WRITE_IQK
#define HCI_WRITE_IQK_DATA_LEN 	 0x70

#define HCI_DEFAULT_LMP_SUBVER   0x8720
#define HCI_PATCH_PROJECT_ID     0x27

typedef enum
{
	OPCODE_PATCH_SNIPPETS = 0x01,
	OPCODE_DUMMY_HEADER = 0x02,
	OPCODE_SECURITY_HEADER = 0x03,
	OPCODE_OTA_FLAG = 0x04
} SECTION_OPCODE;

void hci_platform_get_baudrate(uint8_t *baudrate, uint8_t len);
void hci_platform_cfg_bd_addr(uint8_t *bdaddr);
void hci_platform_record_chipid(uint8_t chipid);
void hci_platform_dl_patch_done(void);
uint8_t hci_platform_set_baudrate(void);
uint8_t hci_platform_dl_patch_init(void);
uint8_t hci_platform_get_patch_cmd_len(uint8_t *cmd_len);
uint8_t hci_platform_get_patch_cmd_buf(uint8_t *cmd_buf, uint8_t cmd_len);

uint8_t hci_platform_init(void);
uint8_t hci_platform_deinit(void);

void hci_platform_bt_log_init(void);
void hci_platform_bt_log_deinit(void);
void hci_platform_bt_fw_log_open(void);
void hci_platform_bt_fw_log_close(void);
void hci_platform_bt_trace_log_open(void);
void hci_platform_bt_trace_log_close(void);

bool rtk_bt_pre_enable(void);
#endif
