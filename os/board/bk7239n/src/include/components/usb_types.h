// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <common/bk_include.h>
#include <common/bk_err.h>
#include "usb_audio.h"
#include "usb_video.h"
#include <driver/media_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_USB_NOT_POWER                  (BK_ERR_USB_BASE - 1) /**< USB vbus poweron, etc */
#define BK_ERR_USB_NOT_OPEN                   (BK_ERR_USB_BASE - 2) /**< USB dose not turn on clock and power, etc */
#define BK_ERR_USB_NOT_CLOSE                  (BK_ERR_USB_BASE - 3) /**< USB dose not turn off clock and power, etc */
#define BK_ERR_USB_NOT_CONNECT                (BK_ERR_USB_BASE - 4) /**< USB device not connected */
#define BK_ERR_USB_OPERATION_NULL_POINTER     (BK_ERR_USB_BASE - 5) /**< USB Operated with null pointer */

#define BK_ERR_USB_UVC_NOT_START              (BK_ERR_USB_BASE - 6) /**< UVC Transfer not started */
#define BK_ERR_USB_UVC_NOT_STOP               (BK_ERR_USB_BASE - 7) /**< UVC Transfer not stopped */
#define BK_ERR_USB_UVC_NOT_PAUSE              (BK_ERR_USB_BASE - 8) /**< UVC Transfer not paused */
#define BK_ERR_USB_UVC_GET_PARAMETER_FAILED   (BK_ERR_USB_BASE - 9) /**< UVC The command to obtain parameters did not respond */
#define BK_ERR_USB_UVC_SET_PARAMETER_FAILED   (BK_ERR_USB_BASE - 10) /**< UVC Configuration parameter command did not respond */
#define BK_ERR_USB_UVC_SET_INTERFACE_FAILED   (BK_ERR_USB_BASE - 11) /**< UVC The set port command did not respond */
#define BK_ERR_USB_UVC_NOSUPPORT_ATTRIBUTE    (BK_ERR_USB_BASE - 12) /**< UVC The attribute configuration is not supported */

#define BK_ERR_USB_UAC_NOT_START              (BK_ERR_USB_BASE - 13) /**< UAC Transfer not started */
#define BK_ERR_USB_UAC_NOT_STOP               (BK_ERR_USB_BASE - 14) /**< UAC Transfer not stopped */
#define BK_ERR_USB_UAC_NOT_PAUSE              (BK_ERR_USB_BASE - 15) /**< UAC Transfer not paused */
#define BK_ERR_USB_UAC_GET_PARAMETER_FAILED   (BK_ERR_USB_BASE - 16) /**< UAC The command to obtain parameters did not respond */
#define BK_ERR_USB_UAC_SET_PARAMETER_FAILED   (BK_ERR_USB_BASE - 17) /**< UAC Configuration parameter command did not respond */
#define BK_ERR_USB_UAC_SET_INTERFACE_FAILED   (BK_ERR_USB_BASE - 18) /**< UAC The set port command did not respond */
#define BK_ERR_USB_UAC_NOSUPPORT_ATTRIBUTE    (BK_ERR_USB_BASE - 19) /**< UAC The attribute configuration is not supported */


typedef enum
{
    USB_HOST_MODE   = 0,
    USB_DEVICE_MODE = 1
} E_USB_MODE;

typedef enum
{
    USB_DEVICE_DISCONNECT = 0,
    USB_DEVICE_CONNECT = 1
} E_USB_CONNECTION_STATUS;

typedef enum
{
    FPS_60 = 60,
    FPS_30 = 30,
    FPS_25 = 25,
    FPS_20 = 20,
    FPS_15 = 15,
    FPS_10 = 10,
    FPS_5  = 5,
} E_FRAME_RATE_ID;

typedef enum
{
    USB_UVC_DEVICE = 0,
    USB_UAC_MIC_DEVICE = 1,
    USB_UAC_SPEAKER_DEVICE = 2,
    USB_MSD_DEVICE = 3,
    USB_OTA_DEVICE = 4,
    USB_USER_CHECK_DEVICE = 5,
    USB_DEVICE_MAX,
} E_USB_DEVICE_T;

#define BK_USB_ENDPOINT_XFERTYPE_MASK 0x03
typedef enum
{
    USB_ENDPOINT_CONTROL_TRANSFER = 0,
    USB_ENDPOINT_ISOCH_TRANSFER = 1,
    USB_ENDPOINT_BULK_TRANSFER = 2,
    USB_ENDPOINT_INT_TRANSFER = 3,
    USB_ENDPOINT_INVALID_TRANSFER,
} E_USB_ENDPOINT_TRANSFER_MODE_T;

struct usb_device_descriptor_t
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdUSB;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
} __attribute__ ((__packed__));

struct usb_configuration_descriptor_t
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wTotalLength;
	uint8_t bNumInterfaces;
	uint8_t bConfigurationValue;
	uint8_t iConfiguration;
	uint8_t bmAttributes;
	uint8_t bMaxPower;
} __attribute__ ((__packed__));

/* USB Interface Association Descriptor */
struct usb_interface_association_descriptor_t
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bFirstInterface;
	uint8_t bInterfaceCount;
	uint8_t bFunctionClass;
	uint8_t bFunctionSubClass;
	uint8_t bFunctionProtocol;
	uint8_t iFunction;
} __attribute__ ((__packed__));

struct usb_interface_descriptor_t
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndpoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;
} __attribute__ ((__packed__));

struct usb_audio_endpoint_general_descriptor_t
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmAttributes;
    uint8_t bLockDelayUnits;
    uint16_t wLockDelay;
} __attribute__((__packed__));

struct s_bk_usb_endpoint_descriptor
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
} __attribute__ ((__packed__));

typedef struct
{
    uint8_t mjpeg_format_index;
    uint8_t h264_format_index;
    uint8_t h265_format_index;
    uint8_t yuv_format_index;
} bk_uvc_format_index;

typedef struct
{
    uint16_t  width;
    uint16_t  height;
    uint16_t  index;
    uint16_t  fps_num;
    uint32_t  *fps;
} bk_uvc_frame;

typedef struct
{
    bk_uvc_frame *mjpeg_frame;
    uint8_t mjpeg_frame_num;
    bk_uvc_frame *h264_frame;
    uint8_t h264_frame_num;
    bk_uvc_frame *h265_frame;
    uint8_t h265_frame_num;
    bk_uvc_frame *yuv_frame;
    uint8_t yuv_frame_num;
} bk_uvc_all_frame;

typedef struct
{
    uint32_t n_uvc_dev;
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t device_bcd;
    uint32_t support_devs;
    bk_uvc_format_index format_index;
    bk_uvc_all_frame all_frame;
    uint32_t endpoints_num;
    struct s_bk_usb_endpoint_descriptor *ep_desc;
} bk_uvc_device_brief_info_t;

typedef struct
{
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t width;
    uint16_t height;
    uint32_t fps;
    uint16_t frame_index;
    uint16_t format_index;
    struct s_bk_usb_endpoint_descriptor *ep_desc;
} bk_uvc_config_t;

/* UAC OPS */
typedef struct
{
	uint8_t *data_buffer;
	uint16_t num_packets;
	uint16_t data_buffer_size;
	uint8_t *state;
	uint16_t *num_byte;
	uint16_t *actual_num_byte;
	E_USB_DEVICE_T dev;
} audio_packet_t;

struct audio_packet_control_t
{
	audio_packet_t * (*_uac_malloc)(uint32_t dev);
	void              (*_uac_push)(audio_packet_t *packet);
	audio_packet_t * (*_uac_pop)(uint32_t dev);
	void              (*_uac_free)(audio_packet_t *packet);
};

struct bk_uac_ring_buffer_t
{
	void *mic_ring_buffer_p;
	void *spk_ring_buffer_p;
};

typedef struct
{
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t mic_format_tag;
    uint8_t mic_samples_frequence_num;
    uint8_t *mic_samples_frequence;
    struct audio_ep_descriptor *mic_ep_desc;
    uint16_t spk_format_tag;
    uint8_t spk_samples_frequence_num;
    uint8_t *spk_samples_frequence;
    struct audio_ep_descriptor *spk_ep_desc;

} bk_uac_device_brief_info_t;


typedef struct {
    uint16_t mic_format_tag;
    uint32_t mic_samples_frequence;
    struct audio_ep_descriptor *mic_ep_desc;
} bk_uac_mic_config_t;

typedef struct {
    uint16_t spk_format_tag;
    uint32_t spk_samples_frequence;
    struct audio_ep_descriptor *spk_ep_desc;
} bk_uac_spk_config_t;

typedef struct {
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t mic_format_tag;
    uint32_t mic_samples_frequence;
    struct audio_ep_descriptor *mic_ep_desc;
    uint16_t spk_format_tag;
    uint32_t spk_samples_frequence;
    struct audio_ep_descriptor *spk_ep_desc;
} bk_uac_config_t;

typedef struct {
	E_USB_DEVICE_T dev;
	FUNCPTR uvc_uac_connect_cb;
	FUNCPTR uvc_uac_disconnect_cb;
} bk_uvc_uac_app_callback;

typedef struct
{
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} s_usb_device_request;

typedef void (*control_irpcomplete)(void *, void *);

typedef struct
{
	const uint8_t *pOutBuffer;
	uint32_t dwOutLength;
	uint8_t *pInBuffer;
	uint32_t dwInLength;

	control_irpcomplete pfIrpComplete;
	void *pCompleteParam;
} s_usb_transfer_buffer_info;

typedef enum {

	USB_DRV_IDLE = 0,
	USB_DRV_EXIT,
	USB_POWER_OPS,

	USB_DRV_USB_INIT = 3,
	USB_DRV_USB_DEINIT,

	USB_DRV_SET_DEV_USE = 5,

	USB_DRV_USB_OPEN = 6,
	USB_DRV_USB_CLOSE,

	USB_DRV_CONNECT_HANDLE = 8,
	USB_DRV_DISCONNECT_HANDLE,

	USB_DRV_GET_DEV_CONNECT_STATUS,
	USB_DRV_CHECK_DEV_SUPPORT,

	USB_DRV_PRASE_DESCRIPTOR,

	USB_DRV_INIT_CONTROL_TRANS,
	USB_DRV_DEINIT_CONTROL_TRANS,
	USB_DRV_CONTROL_TRANSFER,

	USB_DRV_UVC_REGISTER_CONNECT_CB = 16,
	USB_DRV_UVC_REGISTER_DISCONNECT_CB,
	USB_DRV_UVC_GET_PARAM,
	USB_DRV_UVC_SET_PARAM,
	USB_DRV_UVC_REGISTER_TRANSFER_OPS,
	USB_DRV_UVC_TRANSFER_BUFFER_MALLOC,
	USB_DRV_UVC_TRANSFER_BUFFER_PUSH,
	USB_DRV_UVC_TRANSFER_BUFFER_POP,
	USB_DRV_UVC_TRANSFER_BUFFER_FREE,
	USB_DRV_VIDEO_OPEN_PIPE,
	USB_DRV_VIDEO_CLOSE_PIPE,
	USB_DRV_VIDEO_START,
	USB_DRV_VIDEO_STOP,
	USB_DRV_VIDEO_RX,
	USB_DRV_VIDEO_RXED,

	USB_DRV_UAC_REGISTER_CONNECT_CB = 31,
	USB_DRV_UAC_REGISTER_DISCONNECT_CB,
	USB_DRV_UAC_GET_PARAM,
	USB_DRV_UAC_SET_PARAM,
	USB_DRV_UAC_REGISTER_TRANSFER_OPS,
	USB_DRV_UAC_TRANSFER_BUFFER_MALLOC,
	USB_DRV_UAC_TRANSFER_BUFFER_PUSH,
	USB_DRV_UAC_TRANSFER_BUFFER_POP,
	USB_DRV_UAC_TRANSFER_BUFFER_FREE,
	USB_DRV_AUDIO_OPEN_PIPE,
	USB_DRV_AUDIO_CLOSE_PIPE,
	USB_DRV_AUDIO_MIC_START,
	USB_DRV_AUDIO_MIC_STOP,
	USB_DRV_AUDIO_SPK_START,
	USB_DRV_AUDIO_SPK_STOP,
	USB_DRV_AUDIO_RX,
	USB_DRV_AUDIO_RXED,
	USB_DRV_AUDIO_TX,
	USB_DRV_AUDIO_TXED,

	/* For Dual Stream */
	USB_DRV_UVC_TRANSFER_DUAL_BUFFER_MALLOC,
	USB_DRV_UVC_TRANSFER_DUAL_BUFFER_PUSH,
	USB_DRV_UVC_TRANSFER_DUAL_BUFFER_POP,
	USB_DRV_UVC_TRANSFER_DUAL_BUFFER_FREE,

    USB_DRV_VIDEO_DUAL_START,
    USB_DRV_VIDEO_DUAL_STOP,
    USB_DRV_VIDEO_DUAL_RX,
    USB_DRV_VIDEO_DUAL_RXED,

} bk_usb_drv_op_t;

/* Standard Protocol Enumeration Types */
/* Video Class-Specific Request Codes */
typedef enum
{
	USB_ATTRIBUTE_RC_UNDEFINED = 0,
	USB_ATTRIBUTE_SET_CUR = 1,
	USB_ATTRIBUTE_GET_CUR = 2,
	USB_ATTRIBUTE_GET_MIN,
	USB_ATTRIBUTE_GET_MAX,
	USB_ATTRIBUTE_GET_RES,
	USB_ATTRIBUTE_GET_LEN,
	USB_ATTRIBUTE_GET_INFO,
	USB_ATTRIBUTE_GET_DEF,
} E_USB_ATTRIBUTE_OP;

/* Processing Unit Control Selectors */
typedef enum
{
	UVC_ATTRIBUTE_CONTROL_UNDEFINED = 0x00,
	UVC_ATTRIBUTE_BACKLIGHT_COMPENSATION = 0x01,
	UVC_ATTRIBUTE_BRIGHTNESS,
	UVC_ATTRIBUTE_CONTRAST,
	UVC_ATTRIBUTE_GAIN,
	UVC_ATTRIBUTE_POWER_LINE_FREQUENCY,
	UVC_ATTRIBUTE_HUE,
	UVC_ATTRIBUTE_SATURATION,
	UVC_ATTRIBUTE_SHARPNESS,
	UVC_ATTRIBUTE_GAMMA,
	UVC_ATTRIBUTE_WHITE_BALANCE_TEMPERATURE,
	UVC_ATTRIBUTE_WHITE_BALANCE_TEMPERATURE_AUTO,
	UVC_ATTRIBUTE_WHITE_BALANCE_COMPONENT,
	UVC_ATTRIBUTE_WHITE_BALANCE_COMPONENT_AUTO,
	UVC_ATTRIBUTE_DIGITAL_MULTIPLIER,
	UVC_ATTRIBUTE_DIGITAL_MULTIPLIER_LIMIT,
	UVC_ATTRIBUTE_HUE_AUTO,
	UVC_ATTRIBUTE_ANALOG_VIDEO_STANDARD,
	UVC_ATTRIBUTE_ANALOG_LOCK_STATUS,
	UVC_ATTRIBUTE_NUM,
} E_UVC_ATTRIBUTE_T;

typedef enum
{
	UAC_ATTRIBUTE_UNDEF = 0x0,
	UAC_ATTRIBUTE_MUTE = 0x1,
	UAC_ATTRIBUTE_VOLUME = 0x2,
	UAC_ATTRIBUTE_BASS = 0x3,
	UAC_ATTRIBUTE_MID = 0x4,
	UAC_ATTRIBUTE_TREBLE = 0x5,
	UAC_ATTRIBUTE_GRAPHIC_EQUALIZER = 0x6,
	UAC_ATTRIBUTE_AUTOMATIC_GAIN = 0x7,
	UAC_ATTRIBUTE_DEALY = 0x8,
	UAC_ATTRIBUTE_BASS_BOOST = 0x9,
	UAC_ATTRIBUTE_LOUDNESS = 0xa,
	UAC_ATTRIBUTE_INP_GAIN,
	UAC_ATTRIBUTE_INP_GAIN_PAD,
	UAC_ATTRIBUTE_PHASE_INVERT,
	UAC_ATTRIBUTE_UNDERFLOW,
	UAC_ATTRIBUTE_OVERFLOW,
	UAC_ATTRIBUTE_LATENCY,
	UAC_ATTRIBUTE_NUM,
} E_UAC_ATTRIBUTE_T;

#ifdef __cplusplus
}
#endif

