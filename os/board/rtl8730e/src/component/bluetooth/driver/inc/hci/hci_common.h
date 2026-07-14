/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _HCI_COMMON_H_
#define _HCI_COMMON_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <platform_autoconf.h>

#define HCI_FAIL      0
#define HCI_SUCCESS   1
#define HCI_IGNORE    2

/* hci packet type */
#define HCI_CMD                     0x01
#define HCI_ACL                     0x02
#define HCI_SCO                     0x03
#define HCI_EVT                     0x04
#define HCI_ISO                     0x05

/* hci event */
#define BT_HCI_EVT_CMD_COMPLETE                 0x0e
#define BT_HCI_EVT_CMD_STATUS                   0x0f
#define BT_HCI_EVT_LE_META_EVENT                0x3e
#define BT_HCI_EVT_LE_ADVERTISING_REPORT        0x02
#define BT_HCI_EVT_LE_EXT_ADVERTISING_REPORT    0x0d

/* hci packet header struct */
struct hci_acl_hdr {
	uint16_t handle;
	uint16_t len;
} __attribute__((packed));

struct hci_sco_hdr {
	uint16_t handle;
	uint8_t len;
} __attribute__((packed));

struct hci_iso_hdr {
	uint16_t handle;
	uint16_t len;
} __attribute__((packed));

struct hci_cmd_hdr {
	uint16_t opcode;
	uint8_t  param_len;
} __attribute__((packed));

struct hci_evt_hdr {
	uint8_t  evt;
	uint8_t  len;
} __attribute__((packed));

#define HCI_HDR_MAX_SIZE 4

#define LE_TO_UINT16(_data, _array)    {            \
        _data = ((uint16_t)(*((uint8_t *)(_array) + 0)) << 0) |        \
                ((uint16_t)(*((uint8_t *)(_array) + 1)) << 8);         \
    }

#define LE_TO_UINT32(_data, _array)    {            \
        _data = ((uint32_t)(*((uint8_t *)(_array) + 0)) <<  0) |       \
                ((uint32_t)(*((uint8_t *)(_array) + 1)) <<  8) |       \
                ((uint32_t)(*((uint8_t *)(_array) + 2)) << 16) |       \
                ((uint32_t)(*((uint8_t *)(_array) + 3)) << 24);        \
    }

#define LE_TO_UINT64(_data, _array)    {            \
        _data = ((uint64_t)(*((uint8_t *)(_array) + 0)) <<  0) |       \
                ((uint64_t)(*((uint8_t *)(_array) + 1)) <<  8) |       \
                ((uint64_t)(*((uint8_t *)(_array) + 2)) << 16) |       \
                ((uint64_t)(*((uint8_t *)(_array) + 3)) << 24) |       \
                ((uint64_t)(*((uint8_t *)(_array) + 4)) << 32) |       \
                ((uint64_t)(*((uint8_t *)(_array) + 5)) << 40) |       \
                ((uint64_t)(*((uint8_t *)(_array) + 6)) << 48) |       \
                ((uint64_t)(*((uint8_t *)(_array) + 7)) << 56);        \
    }

#if defined(CONFIG_MP_INCLUDED) && CONFIG_MP_INCLUDED
#if defined(CONFIG_MP_SHRINK) && CONFIG_MP_SHRINK
#define hci_is_mp_mode()            true
#else /* CONFIG_MP_SHRINK */
#define hci_is_mp_mode              hci_check_mp
#endif /* CONFIG_MP_SHRINK */
#else /* CONFIG_MP_INCLUDED */
#define hci_is_mp_mode()            false
#endif /* CONFIG_MP_INCLUDED */

void hci_set_mp(bool is_mp);
bool hci_check_mp(void);
void set_reg_value(uint32_t reg_address, uint32_t Mask, uint32_t val);
uint8_t hci_get_hdr_len(uint8_t type);
uint16_t hci_get_body_len(const void *hdr, uint8_t type);

#endif /* _HCI_COMMON_H_ */
