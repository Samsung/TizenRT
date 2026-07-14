/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "osif.h"
#include "hci_common.h"
#include "bt_debug.h"
#include "hal_platform.h"

static bool hci_is_mp = false;

void hci_set_mp(bool is_mp)
{
	hci_is_mp = is_mp;
}

bool hci_check_mp(void)
{
	return hci_is_mp;
}

void set_reg_value(uint32_t reg_address, uint32_t Mask, uint32_t val)
{
	uint32_t shift = 0;
	uint32_t data = 0;

	for (shift = 0; shift < 31; shift++) {
		if (((Mask >> shift) & 0x1) == 1) {
			break;
		}
	}

	data = HAL_READ32(reg_address, 0);
	data = ((data & (~Mask)) | (val << shift));
	HAL_WRITE32(reg_address, 0, data);
	data = HAL_READ32(reg_address, 0);
}

uint8_t hci_get_hdr_len(uint8_t type)
{
	if (type == HCI_CMD) {
		return sizeof(struct hci_cmd_hdr);
	} else if (type == HCI_EVT) {
		return sizeof(struct hci_evt_hdr);
	} else if (type == HCI_ACL) {
		return sizeof(struct hci_acl_hdr);
	} else if (type == HCI_ISO) {
		return sizeof(struct hci_iso_hdr);
	} else if (type == HCI_SCO) {
		return sizeof(struct hci_sco_hdr);
	}

	return 0;
}

uint16_t hci_get_body_len(const void *hdr, uint8_t type)
{
	uint16_t len = 0;
	if (type == HCI_CMD) {
		len = ((const struct hci_cmd_hdr *)hdr)->param_len;
	} else if (type == HCI_EVT) {
		len = ((const struct hci_evt_hdr *)hdr)->len;
	} else if (type == HCI_ISO) {
		LE_TO_UINT16(len, &(((const struct hci_iso_hdr *)hdr)->len));
		len &= 0x3FFF;
	} else if (type == HCI_ACL) {
		LE_TO_UINT16(len, &(((const struct hci_acl_hdr *)hdr)->len));
	} else if (type == HCI_SCO) {
		len = ((const struct hci_sco_hdr *)hdr)->len;
	}

	return len;
}
