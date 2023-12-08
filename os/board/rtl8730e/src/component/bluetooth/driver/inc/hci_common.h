/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef _HCI_COMMON_H_
#define _HCI_COMMON_H_

#include <stdint.h>
#include <stddef.h>

#define HCI_ISO_DATA_PACKET  1

#define HCI_FAIL      0
#define HCI_SUCCESS   1
#define HCI_IGNORE    2

#if 0
#define HCI_FAIL_RETURN(func) \
    if (HCI_FAIL == (func))   \
        return HCI_FAIL;
#endif

#define H4_HEADER_LEN 1
#define H5_HEADER_LEN 4

#define H4_NONE       0x00
#define H4_CMD        0x01
#define H4_ACL        0x02
#define H4_SCO        0x03
#define H4_EVT        0x04
#define H4_ISO        0x05

typedef uint8_t (*HCI_OPEN_CB)(uint8_t status);
typedef uint8_t (*HCI_RECV)(uint8_t type, uint8_t *buf, uint16_t len);
typedef uint8_t *(*HCI_RECV_GET_BUF)(uint8_t type, uint16_t len, uint32_t timeout);
typedef uint8_t (*HCI_SEND_CB)(void);
typedef uint8_t (*HCI_RECV_IND)(void);

#endif
