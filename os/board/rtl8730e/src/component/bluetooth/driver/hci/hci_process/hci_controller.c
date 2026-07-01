/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "osif.h"
#include "hci_platform.h"
#include "hci_controller.h"
#include "hci_transport.h"
#include "bt_debug.h"
#include "dlist.h"

#define HCI_PATCH_FRAG_SIZE        252
#define HCI_BAUDRATE_SIZE          4

static uint8_t patch_chip_id = 0;
static uint8_t default_baud[HCI_BAUDRATE_SIZE] = {0x1d, 0x70, 0x00, 0x00}; /* 115200 */
static uint8_t work_baud[HCI_BAUDRATE_SIZE] = {0};

extern const unsigned char rtlbt_fw[];
extern unsigned int rtlbt_fw_len;
extern const unsigned char rtlbt_mp_fw[];
extern unsigned int rtlbt_mp_fw_len;

void hci_patch_set_chipid(uint8_t chipid)
{
	patch_chip_id = hci_platform_record_chipid(chipid);
}

uint8_t hci_patch_get_chipid(void)
{
	return patch_chip_id;
}

uint8_t hci_patch_get_patch_version(uint8_t **pp_patch_buf, uint32_t *p_patch_len)
{
	const uint8_t patch_sig_v1[] = {0x52, 0x65, 0x61, 0x6C, 0x74, 0x65, 0x63, 0x68}; /* V1 signature: Realtech */
	const uint8_t patch_sig_v2[] = {0x52, 0x54, 0x42, 0x54, 0x43, 0x6F, 0x72, 0x65}; /* V2 signature: RTBTCore */
	const uint8_t patch_sig_v3[] = {0x42, 0x54, 0x4E, 0x49, 0x43, 0x30, 0x30, 0x33}; /* V3 signature: BTNIC003 */
	const uint8_t ext_section_sig[] = {0x51, 0x04, 0xFD, 0x77};                      /* Extension section signature */
	bool ext_section_check;
	uint8_t *p_patch = NULL;
	uint32_t patch_len;
	uint8_t patch_version = PATCH_VERSION_INVALID;

	if (CHECK_CFG_SW(CFG_SW_USE_FLASH_PATCH)) {
		if (hci_is_mp_mode()) {
			p_patch = (uint8_t *)(void *)rtlbt_mp_fw;
			patch_len = rtlbt_mp_fw_len;
		} else {
			p_patch = (uint8_t *)(void *)rtlbt_fw;
			patch_len = rtlbt_fw_len;
		}
		ext_section_check = true;
	} else {
		p_patch = (uint8_t *)HCI_PATCH_FLASH_ADDRESS;
		ext_section_check = false;
	}

	if (!memcmp(p_patch, patch_sig_v3, sizeof(patch_sig_v3))) {
		patch_version = PATCH_VERSION_V3;
	} else {
		if (ext_section_check) {
			if ((!memcmp(p_patch, patch_sig_v1, sizeof(patch_sig_v1))) &&
				(!memcmp(p_patch + patch_len - sizeof(ext_section_sig), ext_section_sig, sizeof(ext_section_sig)))) {
				patch_version = PATCH_VERSION_V1;
			} else if ((!memcmp(p_patch, patch_sig_v2, sizeof(patch_sig_v2))) &&
					   (!memcmp(p_patch + patch_len - sizeof(ext_section_sig), ext_section_sig, sizeof(ext_section_sig)))) {
				patch_version = PATCH_VERSION_V2;
			} else {
				return PATCH_VERSION_INVALID;
			}
		} else {
			if (!memcmp(p_patch, patch_sig_v1, sizeof(patch_sig_v1))) {
				patch_version = PATCH_VERSION_V1;
			} else if (!memcmp(p_patch, patch_sig_v2, sizeof(patch_sig_v2))) {
				patch_version = PATCH_VERSION_V2;
			} else {
				return PATCH_VERSION_INVALID;
			}
		}
	}

	if (pp_patch_buf) {
		*pp_patch_buf = p_patch;
	}

	if (p_patch_len) {
		*p_patch_len = patch_len;
	}

	return patch_version;
}

uint32_t _convert_baudrate(uint8_t *baudrate)
{
	int i, len;

	struct {
		uint8_t bt_baudrate[HCI_BAUDRATE_SIZE];
		uint32_t uart_baudrate;
	} br_map[] = {
		{{0x1d, 0x70, 0x00, 0x00}, 115200},
		{{0x0A, 0xC0, 0x52, 0x02}, 230400},
		{{0x04, 0x50, 0xF7, 0x03}, 921600},
		{{0x04, 0x50, 0xF7, 0x05}, 921600},
		{{0x04, 0x60, 0x00, 0x00}, 921600},
		{{0x04, 0x50, 0x00, 0x00}, 1000000},
		{{0x02, 0x80, 0x92, 0x04}, 1500000},
		{{0x02, 0x50, 0x00, 0x00}, 2000000},
		{{0x01, 0xB0, 0x00, 0x00}, 2500000},
		{{0x01, 0x80, 0x92, 0x04}, 3000000},
		{{0x01, 0x60, 0x2A, 0x05}, 3500000},
		{{0x01, 0x50, 0x00, 0x00}, 4000000},
		{{0x01, 0x00, 0x00, 0x00}, 8000000},
	};

	len = sizeof(br_map) / sizeof(br_map[0]);

	for (i = 0; i < len; i++) {
		if (memcmp(baudrate, br_map[i].bt_baudrate, HCI_BAUDRATE_SIZE) == 0) {
			return br_map[i].uart_baudrate;
		}
	}

	return 0;
}

void hci_set_work_baudrate(uint8_t *baudrate)
{
	memcpy(work_baud, baudrate, HCI_BAUDRATE_SIZE);
}

void hci_get_baudrate(uint8_t *baudrate, bool use_default_rate)
{
	if (use_default_rate) {
		/* Init baudrate */
		memcpy(baudrate, default_baud, HCI_BAUDRATE_SIZE);
	} else {
		/* Work baudrate */
		memcpy(baudrate, work_baud, HCI_BAUDRATE_SIZE);
	}
}

uint8_t hci_update_uart_baudrate(bool use_default_rate)
{
	uint8_t *baud = work_baud;

	if (use_default_rate) {
		baud = default_baud;
	}

	hci_uart_set_bdrate(_convert_baudrate(baud));
	osif_delay(10);

	return HCI_SUCCESS;
}

static bool _controller_is_opened = false;
uint8_t hci_process(void);
bool hci_controller_open(void)
{
	if (_controller_is_opened) {
		BT_LOGE("Controller Already Opened!\r\n");
		return false;
	}

	/* Set WiFi leave powersave mode */
	if (rtk_bt_pre_enable() == false) {
		BT_LOGE("rtk_bt_pre_enable fail!\r\n");
		return false;
	}

	/* Platform Open */
	if (HCI_FAIL == hci_platform_open()) {
		BT_LOGE("hci_platform_open fail!\r\n");
		return false;
	}

	/* HCI Transport Open */
	if (HCI_FAIL == hci_transport_open()) {
		BT_LOGE("hci_transport_open fail!\r\n");
		goto transport_fail;
	}

	/* HCI StandAlone Open */
	if (HCI_FAIL == hci_sa_open()) {
		BT_LOGE("hci_sa_open fail!\r\n");
		goto sa_fail;
	}

	/* HCI Transport Bridge to StandAlone */
	hci_transport_register(&hci_sa_cb);

	if (HCI_FAIL == hci_process()) {
		BT_LOGE("hci_process fail!\r\n");
		goto process_fail;
	}

	/* Recover WiFi powersave mode */
	rtk_bt_post_enable();

	_controller_is_opened = true;
	return true;

transport_fail:
	hci_platform_close();
	hci_platform_free();
	return false;

process_fail:
	hci_sa_free();
sa_fail:
	hci_platform_close();
	hci_transport_close();
	hci_platform_free();
	hci_transport_free();
	return false;
}

void hci_controller_close(void)
{
	hci_platform_close();   /* Platform Close First */
	hci_transport_close();  /* HCI Transport Close */

	_controller_is_opened = false;
}

void hci_controller_free(void)
{
	hci_platform_free();    /* Platform Free */
	hci_transport_free();   /* HCI Transport Free */
	hci_sa_free();          /* HCI StandAlone Free */
}

bool hci_controller_is_opened(void)
{
	return _controller_is_opened;
}