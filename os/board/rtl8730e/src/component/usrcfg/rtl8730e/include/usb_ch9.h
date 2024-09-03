/**
  ******************************************************************************
  * @file    usb_ch9.h
  * @author  Realsil WLAN5 Team
  * @brief   This file provides general defines for USB SPEC CH9
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USB_CH9_H
#define USB_CH9_H

/* Includes ------------------------------------------------------------------*/

#include "basic_types.h"

/* Exported defines ----------------------------------------------------------*/

/* USB speed */
#define USB_SPEED_HIGH                                 0
#define USB_SPEED_HIGH_IN_FULL                         1
#define USB_SPEED_LOW                                  2
#define USB_SPEED_FULL                                 3

/* USB descriptor length */
#define USB_LEN_DEV_QUALIFIER_DESC                     0x0AU
#define USB_LEN_DEV_DESC                               0x12U
#define USB_LEN_CFG_DESC                               0x09U
#define USB_LEN_IAD_DESC                               0x08U
#define USB_LEN_IF_DESC                                0x09U
#define USB_LEN_EP_DESC                                0x07U
#define USB_LEN_OTG_DESC                               0x03U
#define USB_LEN_LANGID_STR_DESC                        0x04U
#define USB_LEN_OTHER_SPEED_DESC_SIZ                   0x09U

/* Device descriptor offset */
#define USB_DEV_DESC_OFFSET_VID                        8U
#define USB_DEV_DESC_OFFSET_PID                        10U

/* bmRequestType :D7 Data Phase Transfer Direction */
#define USB_REQ_DIR_MASK                               0x80U
#define USB_REQ_NUM_MASK                               0x7FU
#define USB_H2D                                        0x00U
#define USB_D2H                                        0x80U

/* bmRequestType D6..5 Type */
#define USB_REQ_TYPE_STANDARD                          0x00U
#define USB_REQ_TYPE_CLASS                             0x20U
#define USB_REQ_TYPE_VENDOR                            0x40U
#define USB_REQ_TYPE_MASK                              0x60U

/* bmRequestType D4..0 Recipient */
#define USB_REQ_RECIPIENT_DEVICE                       0x00U
#define USB_REQ_RECIPIENT_INTERFACE                    0x01U
#define USB_REQ_RECIPIENT_ENDPOINT                     0x02U
#define USB_REQ_RECIPIENT_MASK                         0x03U

/* Table 9-4. Standard Request Codes  */
/* bRequest , Value */
#define USB_REQ_GET_STATUS                             0x00U
#define USB_REQ_CLEAR_FEATURE                          0x01U
#define USB_REQ_SET_FEATURE                            0x03U
#define USB_REQ_SET_ADDRESS                            0x05U
#define USB_REQ_GET_DESCRIPTOR                         0x06U
#define USB_REQ_SET_DESCRIPTOR                         0x07U
#define USB_REQ_GET_CONFIGURATION                      0x08U
#define USB_REQ_SET_CONFIGURATION                      0x09U
#define USB_REQ_GET_INTERFACE                          0x0AU
#define USB_REQ_SET_INTERFACE                          0x0BU
#define USB_REQ_SYNCH_FRAME                            0x0CU

/* Table 9-5. Descriptor Types of USB Specifications */
#define USB_DESC_TYPE_DEVICE                           0x01U
#define USB_DESC_TYPE_CONFIGURATION                    0x02U
#define USB_DESC_TYPE_STRING                           0x03U
#define USB_DESC_TYPE_INTERFACE                        0x04U
#define USB_DESC_TYPE_ENDPOINT                         0x05U
#define USB_DESC_TYPE_DEVICE_QUALIFIER                 0x06U
#define USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION        0x07U
#define USB_DESC_TYPE_INTERFACE_POWER                  0x08U
#define USB_DESC_TYPE_IAD                              0x0BU

#define USB_DESC_DEVICE                                ((USB_DESC_TYPE_DEVICE << 8) & 0xFF00U)
#define USB_DESC_CONFIGURATION                         ((USB_DESC_TYPE_CONFIGURATION << 8) & 0xFF00U)
#define USB_DESC_STRING                                ((USB_DESC_TYPE_STRING << 8) & 0xFF00U)
#define USB_DESC_INTERFACE                             ((USB_DESC_TYPE_INTERFACE << 8) & 0xFF00U)
#define USB_DESC_ENDPOINT                              ((USB_DESC_TYPE_INTERFACE << 8) & 0xFF00U)
#define USB_DESC_DEVICE_QUALIFIER                      ((USB_DESC_TYPE_DEVICE_QUALIFIER << 8) & 0xFF00U)
#define USB_DESC_OTHER_SPEED_CONFIGURATION             ((USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION << 8) & 0xFF00U)
#define USB_DESC_INTERFACE_POWER                       ((USB_DESC_TYPE_INTERFACE_POWER << 8) & 0xFF00U)

/* Configuration descriptor offset */
#define USB_CFG_DESC_OFFSET_TYPE                       1U
#define USB_CFG_DESC_OFFSET_TOTAL_LEN                  2U
#define USB_CFG_DESC_OFFSET_ATTR                       7U
#define USB_CFG_DESC_OFFSET_ATTR_BIT_SELF_POWERED      BIT(6)
#define USB_CFG_DESC_OFFSET_ATTR_BIT_REMOTE_WAKEUP     BIT(5)

#define USB_CONFIG_REMOTE_WAKEUP                       0x02U
#define USB_CONFIG_SELF_POWERED                        0x01U

#define USB_FEATURE_EP_HALT                            0x00U
#define USB_FEATURE_REMOTE_WAKEUP                      0x01U
#define USB_FEATURE_TEST_MODE                          0x02U

#define USB_DEVICE_CAPABITY_TYPE                       0x10U

#define USB_HS_MAX_PACKET_SIZE                         512U
#define USB_FS_MAX_PACKET_SIZE                         64U
#define USB_MAX_EP0_SIZE                               64U

/* bmAttributes D1..0 Transfer type */
#define USB_EP_XFER_TYPE_MASK                          0x03U

/* EP0 address */
#define USB_EP0_OUT                                    (0U | USB_H2D)
#define USB_EP0_IN                                     (0U | USB_D2H)

/* Exported types ------------------------------------------------------------*/

/* USB Endpoint Type */
typedef enum {
	USB_CH_EP_TYPE_CTRL = 0U,
	USB_CH_EP_TYPE_ISOC,
	USB_CH_EP_TYPE_BULK,
	USB_CH_EP_TYPE_INTR
} usb_ch_ep_type_t;

/* USB setup request */
typedef struct {
	u8  bmRequestType;	/* Request type */
	u8  bRequest;		/* Request */
	u16 wValue;			/* Value */
	u16 wIndex;			/* Index */
	u16 wLength;		/* Number of bytes to transfer if there is a data phase */
} usb_setup_req_t;

/* USB speed, refer to USB_SPEED_XXX definitions */
typedef int usb_speed_type_t;

/* USB mode */
typedef int usb_otg_mode_t;

/* Exported macros -----------------------------------------------------------*/

#define USB_EP_IS_IN(ep_addr)	(((ep_addr) & USB_REQ_DIR_MASK) == USB_D2H)
#define USB_EP_IS_OUT(ep_addr)	(((ep_addr) & USB_REQ_DIR_MASK) == USB_H2D)
#define USB_EP_NUM(ep_addr)		((ep_addr) & USB_REQ_NUM_MASK)

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#endif /* USB_CH9_H */

