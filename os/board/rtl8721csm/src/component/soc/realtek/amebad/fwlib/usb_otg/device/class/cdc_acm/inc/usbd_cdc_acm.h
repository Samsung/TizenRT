#ifndef _USBD_CDC_ACM_H_
#define _USBD_CDC_ACM_H_

#include <platform_opts.h>

#ifdef CONFIG_USBD_CDC_ACM

#include <platform/platform_stdlib.h>
#include "usb.h"
#include "usb_gadget.h"
#include "usb_composite.h"
#include "osdep_service.h"
#include "cdc_acm/inc/usbd_cdc_acm_if.h"

// Bulk transfer buffer length
#define CDC_ACM_DEFAULT_BULK_IN_BUF_LEN     256
#define CDC_ACM_DEFAULT_BULK_OUT_BUF_LEN    256

// Endpoint address
#define USBD_CDC_ACM_BULK_IN_EP_ADDRESS     0x81
#define USBD_CDC_ACM_BULK_OUT_EP_ADDRESS    0x02

// CDC descriptor types
#define USBD_CDC_HEADER_TYPE                0x00
#define USBD_CDC_CALL_MANAGEMENT_TYPE       0x01
#define USBD_CDC_ACM_TYPE                   0x02
#define USBD_CDC_UNION_TYPE                 0x06
#define USBD_CDC_COUNTRY_TYPE               0x07
#define USBD_CDC_NETWORK_TERMINAL_TYPE      0x0A
#define USBD_CDC_ETHERNET_TYPE              0x0F
#define USBD_CDC_WHCM_TYPE                  0x11
#define USBD_CDC_MDLM_TYPE                  0x12
#define USBD_CDC_MDLM_DETAIL_TYPE           0x13
#define USBD_CDC_DMM_TYPE                   0x14
#define USBD_CDC_OBEX_TYPE                  0x15

// CDC subclass
#define USBD_CDC_SUBCLASS_ACM               0x02
#define USBD_CDC_SUBCLASS_ETHERNET          0x06
#define USBD_CDC_SUBCLASS_WHCM              0x08
#define USBD_CDC_SUBCLASS_DMM               0x09
#define USBD_CDC_SUBCLASS_MDLM              0x0A
#define USBD_CDC_SUBCLASS_OBEX              0x0B

// CDC ACM protocol
#define USBD_CDC_ACM_PROTO_NONE             0x00
#define USBD_CDC_ACM_PROTO_AT               0x01
#define USBD_CDC_ACM_PROTO_VENDOR           0xFF

// CDC ACM requests
#define USBD_CDC_SEND_ENCAPSULATED_COMMAND  0x00U
#define USBD_CDC_GET_ENCAPSULATED_RESPONSE  0x01U
#define USBD_CDC_SET_COMM_FEATURE           0x02U
#define USBD_CDC_GET_COMM_FEATURE           0x03U
#define USBD_CDC_CLEAR_COMM_FEATURE         0x04U
#define USBD_CDC_SET_LINE_CODING            0x20U
#define USBD_CDC_GET_LINE_CODING            0x21U
#define USBD_CDC_SET_CONTROL_LINE_STATE     0x22U
#define USBD_CDC_SEND_BREAK                 0x23U

// CDC ACM debug options
#define USBD_CDC_ACM_DEBUG                  0
#if USBD_CDC_ACM_DEBUG
#define USBD_CDC_INFO(fmt, args...)         printf("\n\r[INFO]%s: " fmt, __FUNCTION__, ## args)
#define USBD_CDC_WARN(fmt, args...)         printf("\n\r[WARN]%s: " fmt, __FUNCTION__, ## args)
#define USBD_CDC_ERROR(fmt, args...)        printf("\n\r[ERROR]%s: " fmt, __FUNCTION__, ## args)
#define USBD_CDC_ENTER                      printf("\n\r%s: =>", __FUNCTION__)
#define USBD_CDC_EXIT                       printf("\n\r%s: <=", __FUNCTION__)
#define USBD_CDC_EXIT_ERR                   printf("\n\r%s: ERR <=", __FUNCTION__)
#else
#define USBD_CDC_INFO(fmt, args...)
#define USBD_CDC_WARN(fmt, args...)
#define USBD_CDC_ERROR(fmt, args...)
#define USBD_CDC_ENTER
#define USBD_CDC_EXIT
#define USBD_CDC_EXIT_ERR
#endif

#pragma pack(push)
#pragma pack(1)

// Header Functional Descriptor
struct usb_cdc_header_desc {
    u8  bLength;
    u8  bDescriptorType;
    u8  bDescriptorSubType;

    u16 bcdCDC;
} __attribute__((packed));

#define USBD_CDC_DT_HEADER_SIZE 5

// Call Management Descriptor
struct usb_cdc_call_mgmt_descriptor {
    u8  bLength;
    u8  bDescriptorType;
    u8  bDescriptorSubType;

    u8  bmCapabilities;
#define USB_CDC_CALL_MGMT_CAP_CALL_MGMT     0x01
#define USB_CDC_CALL_MGMT_CAP_DATA_INTF     0x02

    u8  bDataInterface;
} __attribute__((packed));

#define USBD_CDC_DT_CALL_MGMT_SIZE 5

// Abstract Control Management Descriptor
struct usb_cdc_acm_descriptor {
    u8  bLength;
    u8  bDescriptorType;
    u8  bDescriptorSubType;

    u8  bmCapabilities;
} __attribute__((packed));

#define USBD_CDC_DT_ACM_SIZE 4

// Union Functional Descriptor
struct usb_cdc_union_desc {
    u8  bLength;
    u8  bDescriptorType;
    u8  bDescriptorSubType;

    u8  bControlInterface;
    u8  bSubordinateInterface0;
} __attribute__((packed));

#define USBD_CDC_DT_UNION_SIZE 5

typedef struct {
    u32 dwDTERate;
    u8  bCharFormat;
    u8  bParityType;
    u8  bDataBits;
}  __attribute__((packed)) usbd_cdc_acm_line_coding_t;

#define USBD_CDC_ACM_LINE_CODING_SIZE      7

#define USBD_CDC_ACM_CHAR_FORMAT_STOP_1    0
#define USBD_CDC_ACM_CHAR_FORMAT_STOP_1_5  1
#define USBD_CDC_ACM_CHAR_FORMAT_STOP_2    2

#define USBD_CDC_ACM_PARITY_TYPE_NONE      0
#define USBD_CDC_ACM_PARITY_TYPE_ODD       1
#define USBD_CDC_ACM_PARITY_TYPE_EVEN      2
#define USBD_CDC_ACM_PARITY_TYPE_MARK      3
#define USBD_CDC_ACM_PARITY_TYPE_SPACE     4

#define USBD_CDC_ACM_DATA_BIT_5            5
#define USBD_CDC_ACM_DATA_BIT_6            6
#define USBD_CDC_ACM_DATA_BIT_7            7
#define USBD_CDC_ACM_DATA_BIT_8            8
#define USBD_CDC_ACM_DATA_BIT_16           16

typedef union {
    u16 d16;
    struct {
		unsigned present:1;
		unsigned activate:1;
		unsigned reserved_2_15:14;
    } b;
}  __attribute__((packed)) usbd_cdc_acm_ctrl_line_state_t;

#pragma pack(pop)

struct usb_cdc_dev_t {
    u8 interface_number;

    struct usb_gadget *gadget;
    
    struct usb_ep *bulk_out_ep;
    struct usb_request *bulk_out_req;
    u8 *bulk_out_buf;
    u16 bulk_out_buf_len;

    struct usb_ep *bulk_in_ep;    
    struct usb_request *bulk_in_req;
    u8 *bulk_in_buf;
    u16 bulk_in_buf_len;

    struct usb_function funs;
    usbd_cdc_acm_usr_cb_t *cb;
};

#endif // CONFIG_USBD_CDC_ACM

#endif // _USBD_CDC_ACM_H_

