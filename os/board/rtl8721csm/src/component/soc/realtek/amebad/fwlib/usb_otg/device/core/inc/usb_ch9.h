/*
 * This file holds USB constants and structures that are needed for USB
 * device APIs.  These are used by the USB device model, which is defined
 * in chapter 9 of the USB 2.0 specification.  Linux has several APIs in C
 * that need these:
 *
 * - the master/host side Linux-USB kernel driver API;
 * - the "usbfs" user space API; and
 * - the Linux "gadget" slave/device/peripheral side driver API.
 *
 * USB 2.0 adds an additional "On The Go" (OTG) mode, which lets systems
 * act either as a USB master/host or as a USB slave/device.  That means
 * the master and slave side APIs benefit from working well together.
 *
 * There's also "Wireless USB", using low power short range radios for
 * peripheral interconnection but otherwise building on the USB framework.
 */


#ifndef _USB_CH9_H_
#define _USB_CH9_H_

#include "basic_types.h"

/*-------------------------------------------------------------------------*/
#pragma pack(push)
#pragma pack(1)

/* CONTROL REQUEST SUPPORT */

/*
 * USB directions
 *
 * This bit flag is used in endpoint descriptors' bEndpointAddress field.
 * It's also one of three fields in control requests bmRequestType.
 */
#define USB_DIR_OUT                  0x00    /* to device */
#define USB_DIR_IN                   0x80    /* to host */

/*
 * USB types, the second of three bmRequestType fields
 */
#define USB_TYPE_MASK                (0x03 << 5)
#define USB_GET_TYPE(a)              ((a) & USB_TYPE_MASK)
#define USB_TYPE_STANDARD            (0x00 << 5)
#define USB_TYPE_CLASS               (0x01 << 5)
#define USB_TYPE_VENDOR              (0x02 << 5)
#define USB_TYPE_RESERVED            (0x03 << 5)

/*
 * USB recipients, the third of three bmRequestType fields
 */
#define USB_RECIP_MASK               0x1F
#define USB_GET_RECIP(a)             ((a) & USB_RECIP_MASK)
#define USB_RECIP_DEVICE             0x00
#define USB_RECIP_INTERFACE          0x01
#define USB_RECIP_ENDPOINT           0x02
#define USB_RECIP_OTHER              0x03
/* From Wireless USB 1.0 */
#define USB_RECIP_PORT               0x04
#define USB_RECIP_RPIPE              0x05

/*
 * USB request types, composite bmRequestType fields
 */
#define USB_REQ_TYPE_READ_STANDARD_DEVICE     (USB_DIR_IN  | USB_TYPE_STANDARD | USB_RECIP_DEVICE)
#define USB_REQ_TYPE_WRITE_STANDARD_DEVICE    (USB_DIR_OUT | USB_TYPE_STANDARD | USB_RECIP_DEVICE)
#define USB_REQ_TYPE_READ_STANDARD_INTERFACE  (USB_DIR_IN  | USB_TYPE_STANDARD | USB_RECIP_INTERFACE)
#define USB_REQ_TYPE_WRITE_STANDARD_INTERFACE (USB_DIR_OUT | USB_TYPE_STANDARD | USB_RECIP_INTERFACE)
#define USB_REQ_TYPE_READ_STANDARD_ENDPOINT   (USB_DIR_IN  | USB_TYPE_STANDARD | USB_RECIP_ENDPOINT)
#define USB_REQ_TYPE_WRITE_STANDARD_ENDPOINT  (USB_DIR_OUT | USB_TYPE_STANDARD | USB_RECIP_ENDPOINT)
#define USB_REQ_TYPE_READ_CLASS_DEVICE        (USB_DIR_IN  | USB_TYPE_CLASS    | USB_RECIP_DEVICE)
#define USB_REQ_TYPE_WRITE_CLASS_DEVICE       (USB_DIR_OUT | USB_TYPE_CLASS    | USB_RECIP_DEVICE)
#define USB_REQ_TYPE_READ_CLASS_INTERFACE     (USB_DIR_IN  | USB_TYPE_CLASS    | USB_RECIP_INTERFACE)
#define USB_REQ_TYPE_WRITE_CLASS_INTERFACE    (USB_DIR_OUT | USB_TYPE_CLASS    | USB_RECIP_INTERFACE)
#define USB_REQ_TYPE_READ_CLASS_ENDPOINT      (USB_DIR_IN  | USB_TYPE_CLASS    | USB_RECIP_ENDPOINT)
#define USB_REQ_TYPE_WRITE_CLASS_ENDPOINT     (USB_DIR_OUT | USB_TYPE_CLASS    | USB_RECIP_ENDPOINT)
#define USB_REQ_TYPE_READ_VENDOR_DEVICE       (USB_DIR_IN  | USB_TYPE_VENDOR   | USB_RECIP_DEVICE)
#define USB_REQ_TYPE_WRITE_VENDOR_DEVICE      (USB_DIR_OUT | USB_TYPE_VENDOR   | USB_RECIP_DEVICE)
#define USB_REQ_TYPE_READ_VENDOR_INTERFACE    (USB_DIR_IN  | USB_TYPE_VENDOR   | USB_RECIP_INTERFACE)
#define USB_REQ_TYPE_WRITE_VENDOR_INTERFACE   (USB_DIR_OUT | USB_TYPE_VENDOR   | USB_RECIP_INTERFACE)
#define USB_REQ_TYPE_READ_VENDOR_ENDPOINT     (USB_DIR_IN  | USB_TYPE_VENDOR   | USB_RECIP_ENDPOINT)
#define USB_REQ_TYPE_WRITE_VENDOR_ENDPOINT    (USB_DIR_OUT | USB_TYPE_VENDOR   | USB_RECIP_ENDPOINT)

/*
 * Standard requests, for the bRequest field of a SETUP packet.
 *
 * These are qualified by the bmRequestType field, so that for example
 * TYPE_CLASS or TYPE_VENDOR specific feature flags could be retrieved
 * by a GET_STATUS request.
 */
#define USB_REQ_GET_STATUS           0x00
#define USB_REQ_CLEAR_FEATURE        0x01
#define USB_REQ_SET_FEATURE          0x03
#define USB_REQ_SET_ADDRESS          0x05
#define USB_REQ_GET_DESCRIPTOR       0x06
#define USB_REQ_SET_DESCRIPTOR       0x07
#define USB_REQ_GET_CONFIGURATION    0x08
#define USB_REQ_SET_CONFIGURATION    0x09
#define USB_REQ_GET_INTERFACE        0x0A
#define USB_REQ_SET_INTERFACE        0x0B
#define USB_REQ_SYNCH_FRAME          0x0C

#define USB_REQ_SET_ENCRYPTION       0x0D    /* Wireless USB */
#define USB_REQ_GET_ENCRYPTION       0x0E
#define USB_REQ_RPIPE_ABORT          0x0E
#define USB_REQ_SET_HANDSHAKE        0x0F
#define USB_REQ_RPIPE_RESET          0x0F
#define USB_REQ_GET_HANDSHAKE        0x10
#define USB_REQ_SET_CONNECTION       0x11
#define USB_REQ_SET_SECURITY_DATA    0x12
#define USB_REQ_GET_SECURITY_DATA    0x13
#define USB_REQ_SET_WUSB_DATA        0x14
#define USB_REQ_LOOPBACK_DATA_WRITE  0x15
#define USB_REQ_LOOPBACK_DATA_READ   0x16
#define USB_REQ_SET_INTERFACE_DS     0x17

/*
 * USB feature flags are written using USB_REQ_{CLEAR,SET}_FEATURE, and
 * are read as a bit array returned by USB_REQ_GET_STATUS.  (So there
 * are at most sixteen features of each type.)
 */
#define USB_DEVICE_SELF_POWERED      0   /* (read only) */
#define USB_DEVICE_REMOTE_WAKEUP     1   /* dev may initiate wakeup */
#define USB_DEVICE_TEST_MODE         2   /* (wired high speed only) */
#define USB_DEVICE_BATTERY           2   /* (wireless) */
#define USB_DEVICE_B_HNP_ENABLE      3   /* (otg) dev may initiate HNP */
#define USB_DEVICE_WUSB_DEVICE       3   /* (wireless)*/
#define USB_DEVICE_A_HNP_SUPPORT     4   /* (otg) RH port supports HNP */
#define USB_DEVICE_A_ALT_HNP_SUPPORT 5   /* (otg) other RH port does */
#define USB_DEVICE_DEBUG_MODE        6   /* (special devices only) */

#define USB_ENDPOINT_HALT            0   /* IN/OUT will STALL */

/**
 * struct usb_control_request - SETUP data for a USB device control request
 * @bmRequestType: matches the USB bmRequestType field
 * @bRequest: matches the USB bRequest field
 * @wValue: matches the USB wValue field (le16 byte order)
 * @wIndex: matches the USB wIndex field (le16 byte order)
 * @wLength: matches the USB wLength field (le16 byte order)
 *
 * This structure is used to send control requests to a USB device.  It matches
 * the different fields of the USB 2.0 Spec section 9.3, table 9-2.  See the
 * USB spec for a fuller description of the different fields, and what they are
 * used for.
 *
 * Note that the driver for any interface can issue control requests.
 * For most devices, interfaces don't coordinate with each other, so
 * such requests may be made at any time.
 */
struct usb_control_request {
    u8 bmRequestType;
    u8 bRequest;
    u16 wValue;
    u16 wIndex;
    u16 wLength;
};

/*-------------------------------------------------------------------------*/

/*
 * STANDARD DESCRIPTORS ... as returned by GET_DESCRIPTOR, or
 * (rarely) accepted by SET_DESCRIPTOR.
 *
 * Note that all multi-byte values here are encoded in little endian
 * byte order "on the wire".  But when exposed through Linux-USB APIs,
 * they've been converted to cpu byte order.
 */

/*
 * Descriptor types ... USB 2.0 spec table 9.5
 */
#define USB_DT_DEVICE                  0x01
#define USB_DT_CONFIG                  0x02
#define USB_DT_STRING                  0x03
#define USB_DT_INTERFACE               0x04
#define USB_DT_ENDPOINT                0x05
#define USB_DT_DEVICE_QUALIFIER        0x06
#define USB_DT_OTHER_SPEED_CONFIG      0x07
#define USB_DT_INTERFACE_POWER         0x08
/* these are from a minor usb 2.0 revision (ECN) */
#define USB_DT_OTG                     0x09
#define USB_DT_DEBUG                   0x0a
#define USB_DT_INTERFACE_ASSOCIATION   0x0b
/* these are from the Wireless USB spec */
#define USB_DT_SECURITY                0x0c
#define USB_DT_KEY                     0x0d
#define USB_DT_ENCRYPTION_TYPE         0x0e
#define USB_DT_BOS                     0x0f
#define USB_DT_DEVICE_CAPABILITY       0x10
#define USB_DT_WIRELESS_ENDPOINT_COMP  0x11
#define USB_DT_WIRE_ADAPTER            0x21
#define USB_DT_RPIPE                   0x22

/* conventional codes for class-specific descriptors */
#define USB_DT_CS_DEVICE               0x21
#define USB_DT_CS_CONFIG               0x22
#define USB_DT_CS_STRING               0x23
#define USB_DT_CS_INTERFACE            0x24
#define USB_DT_CS_ENDPOINT             0x25

/* All standard descriptors have these 2 fields at the beginning */
struct usb_descriptor_header {
    u8  bLength;
    u8  bDescriptorType;
};

/*-------------------------------------------------------------------------*/

/* USB_DT_DEVICE: Device descriptor */
struct usb_device_descriptor {
    u8  bLength;
    u8  bDescriptorType;

    u16 bcdUSB;
    u8  bDeviceClass;
    u8  bDeviceSubClass;
    u8  bDeviceProtocol;
    u8  bMaxPacketSize0;
    u16 idVendor;
    u16 idProduct;
    u16 bcdDevice;
    u8  iManufacturer;
    u8  iProduct;
    u8  iSerialNumber;
    u8  bNumConfigurations;
};

#define USB_DT_DEVICE_SIZE             18

/*
 * Device and/or Interface Class codes
 * as found in bDeviceClass or bInterfaceClass
 * and defined by www.usb.org documents
 */
#define USB_CLASS_PER_INTERFACE        0       /* for DeviceClass */
#define USB_CLASS_AUDIO                1
#define USB_CLASS_COMM                 2
#define USB_CLASS_HID                  3
#define USB_CLASS_PHYSICAL             5
#define USB_CLASS_STILL_IMAGE          6
#define USB_CLASS_PRINTER              7
#define USB_CLASS_MASS_STORAGE         8
#define USB_CLASS_HUB                  9
#define USB_CLASS_CDC_DATA             0x0a
#define USB_CLASS_CSCID                0x0b    /* chip+ smart card */
#define USB_CLASS_CONTENT_SEC          0x0d    /* content security */
#define USB_CLASS_VIDEO                0x0e
#define USB_CLASS_WIRELESS_CONTROLLER  0xe0
#define USB_CLASS_APP_SPEC             0xfe
#define USB_CLASS_VENDOR_SPEC          0xff

/*-------------------------------------------------------------------------*/

/* USB_DT_CONFIG: Configuration descriptor information.
 *
 * USB_DT_OTHER_SPEED_CONFIG is the same descriptor, except that the
 * descriptor type is different.  Highspeed-capable devices can look
 * different depending on what speed they're currently running.  Only
 * devices with a USB_DT_DEVICE_QUALIFIER have any OTHER_SPEED_CONFIG
 * descriptors.
 */
struct usb_config_descriptor {
    u8  bLength;
    u8  bDescriptorType;

    u16 wTotalLength;
    u8  bNumInterfaces;
    u8  bConfigurationValue;
    u8  iConfiguration;
    u8  bmAttributes;
    u8  bMaxPower;
};

#define USB_DT_CONFIG_SIZE          9

/* from config descriptor bmAttributes */
#define USB_CONFIG_ATT_ONE          (1 << 7)    /* must be set */
#define USB_CONFIG_ATT_SELFPOWER    (1 << 6)    /* self powered */
#define USB_CONFIG_ATT_WAKEUP       (1 << 5)    /* can wakeup */
#define USB_CONFIG_ATT_BATTERY      (1 << 4)    /* battery powered */

/*-------------------------------------------------------------------------*/

/* USB_DT_STRING: String descriptor */
struct usb_string_descriptor {
    u8  bLength;
    u8  bDescriptorType;

    u16 wData[1];       /* UTF-16LE encoded */
};

/* note that "string" zero is special, it holds language codes that
 * the device supports, not Unicode characters.
 */

/*-------------------------------------------------------------------------*/

/* USB_DT_INTERFACE: Interface descriptor */
struct usb_interface_descriptor {
    u8  bLength;
    u8  bDescriptorType;

    u8  bInterfaceNumber;
    u8  bAlternateSetting;
    u8  bNumEndpoints;
    u8  bInterfaceClass;
    u8  bInterfaceSubClass;
    u8  bInterfaceProtocol;
    u8  iInterface;
};

#define USB_DT_INTERFACE_SIZE       9

/*-------------------------------------------------------------------------*/

/* Endpoint descriptor */
struct usb_endpoint_descriptor {
    u8  bLength;
    u8  bDescriptorType;
    u8  bEndpointAddress;
    u8  bmAttributes;
    u16 wMaxPacketSize;
    u8  bInterval;
    //uint8_t  bRefresh; // bb3 remove
    //uint8_t  bSynchAddress; // bb3 remove

    //unsigned char *extra;   /* Extra descriptors */ // bb3 remove
    //int extralen; // bb3 remove
};

// for audio endpoint
/* Endpoint descriptor */
struct usb_audio_endpoint_descriptor {
    u8  bLength;
    u8  bDescriptorType;
    u8  bEndpointAddress;
    u8  bmAttributes;
    u16 wMaxPacketSize;
    u8  bInterval;
    u8  bRefresh; // bb3 remove
    u8  bSynchAddress; // bb3 remove
} __attribute__((packed));

#define USB_DT_ENDPOINT_SIZE        7
#define USB_DT_ENDPOINT_AUDIO_SIZE  9   /* Audio extension */

/*
 * Endpoints
 */

#define USB_ENDPOINT_DIR_MASK       0x80
#define USB_ENDPOINT_GET_DIR(a)     ((a) & USB_ENDPOINT_DIR_MASK)
#define USB_ENDPOINT_ADDR_MASK      0x0F
#define USB_ENDPOINT_GET_ADDR(a)    ((a) & USB_ENDPOINT_ADDR_MASK)

#define USB_ENDPOINT_XFERTYPE_MASK  0x03    /* in bmAttributes */
#define USB_ENDPOINT_XFER_CONTROL   0x00
#define USB_ENDPOINT_XFER_ISOC      0x01
#define USB_ENDPOINT_XFER_BULK      0x02
#define USB_ENDPOINT_XFER_INT       0x03
#define USB_ENDPOINT_MAX_ADJUSTABLE 0x80

#define USB_ENDPOINT_SYNCTYPE       0x0C
#define USB_ENDPOINT_SYNC_NONE      (0 << 2)
#define USB_ENDPOINT_SYNC_ASYNC     (1 << 2)
#define USB_ENDPOINT_SYNC_ADAPTIVE  (2 << 2)
#define USB_ENDPOINT_SYNC_SYNC      (3 << 2)

/*-------------------------------------------------------------------------*/

/* USB_DT_DEVICE_QUALIFIER: Device Qualifier descriptor */
struct usb_qualifier_descriptor {
    u8  bLength;
    u8  bDescriptorType;

    u16 bcdUSB;
    u8  bDeviceClass;
    u8  bDeviceSubClass;
    u8  bDeviceProtocol;
    u8  bMaxPacketSize0;
    u8  bNumConfigurations;
    u8  bRESERVED;
};

/*-------------------------------------------------------------------------*/

/* USB_DT_INTERFACE_ASSOCIATION: groups interfaces */
struct usb_interface_assoc_descriptor {
    u8  bLength;
    u8  bDescriptorType;

    u8  bFirstInterface;
    u8  bInterfaceCount;
    u8  bFunctionClass;
    u8  bFunctionSubClass;
    u8  bFunctionProtocol;
    u8  iFunction;
};

/*-------------------------------------------------------------------------*/

/* USB_DT_BOS:  group of wireless capabilities */
struct usb_bos_descriptor {
    u8  bLength;
    u8  bDescriptorType;

    u16 wTotalLength;
    u8  bNumDeviceCaps;
};

/*-------------------------------------------------------------------------*/

/* USB_DT_DEVICE_CAPABILITY:  grouped with BOS */
struct usb_dev_cap_header {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bDevCapabilityType;
};

/*-------------------------------------------------------------------------*/

enum usb_device_state {
    /* NOTATTACHED isn't in the USB spec, and this state acts
     * the same as ATTACHED ... but it's clearer this way.
     */
    USB_STATE_NOTATTACHED = 0,

    /* chapter 9 and authentication (wireless) device states */
    USB_STATE_ATTACHED,
    USB_STATE_POWERED,          /* wired */
    USB_STATE_UNAUTHENTICATED,  /* auth */
    USB_STATE_RECONNECTING,     /* auth */
    USB_STATE_DEFAULT,          /* limited function */
    USB_STATE_ADDRESS,
    USB_STATE_CONFIGURED,       /* most functions */

    USB_STATE_SUSPENDED

    /* NOTE:  there are actually four different SUSPENDED
     * states, returning to POWERED, DEFAULT, ADDRESS, or
     * CONFIGURED respectively when SOF tokens flow again.
     */
};
#pragma pack(pop)

#endif  /* _USB_CH9_H_ */

