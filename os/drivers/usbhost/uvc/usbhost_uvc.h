/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef __USBH_VIDEO_H
#define __USBH_VIDEO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdint.h>
#include <limits.h>

#include <tinyara/wqueue.h>
#include <tinyara/usb/usb.h>
#include <tinyara/usb/usbhost.h>
#include <video/video.h>
#include <video/video_halif.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define max_t(a, b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a > _b ? _a : _b; })

#define min_t(a, b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a < _b ? _a : _b; })

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define V4L2_CTRL_FLAG_NEXT_CTRL  0x80000000

#define CHECK_RANGE(value, min, max, step) \ 
do { \
	if ((value < min) || \
	(value > max) || \
	((value - min) % step != 0)) \
	{ \
		return -EINVAL;\
	} \
} while (0)

#define INIT_QUEUE_HEAD(ptr)  \
do {                     \
	(ptr)->head = (dq_entry_t*)(ptr); \
	(ptr)->tail = (dq_entry_t*)(ptr); \
} while (0)

static inline int queue_empty(struct dq_queue_s *queue)
{
	return ((dq_queue_t *) queue->head) == queue;
}
#define queue_entry(ptr, type, member) \
	((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

/* ISOC Transfer daemon */

#ifndef CONFIG_USBUVC_TRF_PRIO
#define CONFIG_USBUVC_TRF_PRIO 220
#endif

#ifndef CONFIG_USBUVC_TRF_STACKSIZE
#define CONFIG_USBUVC_TRF_STACKSIZE 2048
#endif

#define USBUVC_STATE_NOTSTARTED 0x0
#define USBUVC_STATE_IDLE       0x1
#define USBUVC_STATE_TERMINATED 0x2

/* UVC constants */
#define UVC_TERM_INPUT			0x0000
#define UVC_TERM_OUTPUT			0x8000
#define UVC_ENTITY_TYPE(entity)		((entity)->type & 0x7fff)
#define UVC_ENTITY_IS_TERM(entity)	(((entity)->type & 0xff00) != 0)
#define UVC_ENTITY_IS_ITERM(entity) \
	(UVC_ENTITY_IS_TERM(entity) && \
	((entity)->type & 0x8000) == UVC_TERM_INPUT)
#define UVC_ENTITY_IS_OTERM(entity) \
	(UVC_ENTITY_IS_TERM(entity) && \
	((entity)->type & 0x8000) == UVC_TERM_OUTPUT)

/* Unit GUID */
#define UVC_GUID_UVC_CAMERA \
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}
#define UVC_GUID_UVC_OUTPUT \
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}
#define UVC_GUID_UVC_MEDIA_TRANSPORT_INPUT \
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03}
#define UVC_GUID_UVC_PROCESSING \
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01}
#define UVC_GUID_UVC_SELECTOR \
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02}
/* Format GUID */
#define UVC_GUID_FORMAT_MJPEG \
	{ 'M',  'J',  'P',  'G', 0x00, 0x00, 0x10, 0x00, \
	 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}
#define UVC_GUID_FORMAT_YUY2 \
	{ 'Y',  'U',  'Y',  '2', 0x00, 0x00, 0x10, 0x00, \
	 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}
#define UVC_GUID_FORMAT_UYVY \
	{ 'U',  'Y',  'V',  'Y', 0x00, 0x00, 0x10, 0x00, \
	 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}
#define UVC_GUID_FORMAT_RGBP \
	{ 'R',  'G',  'B',  'P', 0x00, 0x00, 0x10, 0x00, \
	 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}

#define CTRL_SET_SUPPORTED(a) (a |= (1 << 1))
#define CTRL_SET_CACHED(a)    (a |= (1 << 2))
#define CTRL_SET_MODIFIED(a)  (a |= (1 << 3))
#define CTRL_SET_LOADED(a)    (a |= (1 << 4))
#define CTRL_SET_DIRTY(a)     (a |= (1 << 5))

#define CTRL_CLEAR_SUPPORTED(a) (a &= ~(1 << 1))
#define CTRL_CLEAR_CACHED(a)    (a &= ~(1 << 2))
#define CTRL_CLEAR_MODIFIED(a)  (a &= ~(1 << 3))
#define CTRL_CLEAR_LOADED(a)    (a &= ~(1 << 4))
#define CTRL_CLEAR_DIRTY(a)     (a &= ~(1 << 5))

#define CTRL_IS_SUPPORTED(a) (a & (1 << 1))
#define CTRL_IS_CACHED(a)    (a & (1 << 2))
#define CTRL_IS_MODIFIED(a)  (a & (1 << 3))
#define CTRL_IS_LOADED(a)    (a & (1 << 4))
#define CTRL_IS_DIRTY(a)     (a & (1 << 5))

#define VS_PROBE_CONTROL                              0x01
#define VS_COMMIT_CONTROL                             0x02
/* UVC class, subclass codes
 * (USB_Video_Class_1.1.pdf, 3.2 Device Descriptor)
 */
#define UVC_DEVICE_SUBCLASS                           0x02
#define UVC_DEVICE_PROTOCOL                           0x01
/*Video Interface Subclass Codes*/
#define CC_VIDEO                                      0x0E
/* Video Interface Subclass Codes */
#define USB_SUBCLASS_VIDEOCONTROL                     0x01
#define USB_SUBCLASS_VIDEOSTREAMING                   0x02
#define USB_SUBCLASS_VIDEO_INTERFACE_COLLECTION       0x03
/* USB UVC Class specific */
#define USB_DESC_TYPE_CS_INTERFACE                    0x24
#define USB_DESC_TYPE_CS_ENDPOINT                     0x25
/* Video Class-Specific VideoControl Interface Descriptor Subtypes
 * (USB_Video_Class_1.1.pdf, A.5 Video Class-Specific VC Interface Descriptor Subtypes)
 */
#define UVC_VC_HEADER                                 0x01
#define UVC_VC_INPUT_TERMINAL                         0x02
#define UVC_VC_OUTPUT_TERMINAL                        0x03
#define UVC_VC_SELECTOR_UNIT                          0x04
#define UVC_VC_PROCESSING_UNIT                        0x05
#define UVC_VC_EXTENSION_UNIT                         0x06
/* Video Class-Specific VideoStreaming Interface Descriptor Subtypes
 * (USB_Video_Class_1.1.pdf, A.6 Video Class-Specific VS Interface Descriptor Subtypes)
 */
#define UVC_VS_UNDEFINED                              0x00
#define UVC_VS_INPUT_HEADER                           0x01
#define UVC_VS_OUTPUT_HEADER                          0x02
#define UVC_VS_STILL_IMAGE_FRAME                      0x03
#define UVC_VS_FORMAT_UNCOMPRESSED                    0x04
#define UVC_VS_FRAME_UNCOMPRESSED                     0x05
#define UVC_VS_FORMAT_MJPEG                           0x06
#define UVC_VS_FRAME_MJPEG                            0x07
#define UVC_VS_FORMAT_MPEG2TS                         0x0A
#define UVC_VS_FORMAT_DV                              0x0C
#define UVC_VS_COLORFORMAT                            0x0D
#define UVC_VS_FORMAT_FRAME_BASED                     0x10
#define UVC_VS_FRAME_FRAME_BASED                      0x11
#define UVC_VS_FORMAT_STREAM_BASED                    0x12
#define UVC_VS_FORMAT_H264                            0x13
#define UVC_VS_FRAME_H264                             0x14
#define UVC_VS_FORMAT_H264_SIMULCAST                  0x15
#define UVC_VS_FORMAT_VP8                             0x16
#define UVC_VS_FRAME_VP8                              0x17
#define UVC_VS_FORMAT_VP8_SIMULCAST                   0x18
#define UVC_AS_GENERAL                                0x01
#define UVC_FORMAT_TYPE                               0x02
#define UVC_FORMAT_SPECIFIC                           0x03
/* Video Class-Specific Endpoint Descriptor Subtypes */
#define UVC_EP_GENERAL                                0x01
/* Video Class-Specific Request Codes */
#define UVC_SET                                       0x00
#define UVC_GET                                       0x80
#define UVC_GET_ALL                                   0x90
#define UVC_SET_ALL                                   0x10
#define UVC_CUR                                       0x1
#define UVC_MIN                                       0x2
#define UVC_MAX                                       0x3
#define UVC_RES                                       0x4
#define UVC_MEM                                       0x5
#define UVC_INF                                       0x6
#define UVC_DEF                                       0x7
#define UVC_SET_CUR                                   (UVC_SET | UVC_CUR)
#define UVC_SET_CUR_ALL                               (UVC_SET_ALL | UVC_CUR)
#define UVC_GET_CUR                                   (UVC_GET | UVC_CUR)
#define UVC_SET_MIN                                   (UVC_SET | UVC_MIN)
#define UVC_GET_MIN                                   (UVC_GET | UVC_MIN)
#define UVC_SET_MAX                                   (UVC_SET | UVC_MAX)
#define UVC_GET_MAX                                   (UVC_GET | UVC_MAX)
#define UVC_SET_RES                                   (UVC_SET | UVC_RES)
#define UVC_GET_RES                                   (UVC_GET | UVC_RES)
#define UVC_SET_LEN                                   (UVC_SET | UVC_MEM)
#define UVC_GET_LEN                                   (UVC_GET | UVC_MEM)
#define UVC_SET_INF                                   (UVC_SET | UVC_INF)
#define UVC_GET_INF                                   (UVC_GET | UVC_INF)
#define UVC_SET_DEF                                   (UVC_SET | UVC_DEF)
#define UVC_GET_DEF                                   (UVC_GET | UVC_DEF)
#define UVC_GET_CUR_ALL                               (UVC_GET_ALL | UVC_CUR)
#define UVC_GET_MIN_ALL                               (UVC_GET_ALL | UVC_MIN)
#define UVC_GET_MAX_ALL                               (UVC_GET_ALL | UVC_MAX)
#define UVC_GET_RES_ALL                               (UVC_GET_ALL | UVC_RES)
#define UVC_GET_DEF_ALL                               (UVC_GET_ALL | UVC_DEF)
#define UVC_GET_STAT                                  0xff
/* Control flags */
#define F_SET_CUR                                      (1 << 0)
#define F_GET_CUR                                      (1 << 1)
#define F_GET_MIN                                      (1 << 2)
#define F_GET_MAX                                      (1 << 3)
#define F_GET_RES                                      (1 << 4)
#define F_GET_DEF                                      (1 << 5)
#define F_RESTORE                                      (1 << 6)
#define F_AUTO                                         (1 << 7)
#define F_ASYNCH                                       (1 << 8)
#define F_GET_RANGE \
	(F_GET_CUR | F_GET_MIN | \
	F_GET_MAX | F_GET_RES | \
	F_GET_DEF)
/* Terminals - 2.1 USB Terminal Types */
#define UVC_TERMINAL_UNDEFINED                          0x100
#define UVC_TERMINAL_STREAMING                          0x101
#define UVC_TERMINAL_VENDOR_SPEC                        0x1FF
/* A.9.1. VideoControl Interface Control Selectors */
#define UVC_VC_CONTROL_UNDEFINED                        0x00
#define UVC_VC_VIDEO_POWER_MODE_CONTROL                 0x01
#define UVC_VC_REQUEST_ERROR_CODE_CONTROL               0x02
/* A.9.2. Terminal Control Selectors */
#define UVC_TE_CONTROL_UNDEFINED                        0x00
/* A.9.3. Selector Unit Control Selectors */
#define UVC_SU_CONTROL_UNDEFINED                        0x00
#define UVC_SU_INPUT_SELECT_CONTROL                     0x01
/* A.9.4. Camera Terminal Control Selectors */
#define UVC_CT_CONTROL_UNDEFINED                        0x00
#define UVC_CT_SCANNING_MODE_CONTROL                    0x01
#define UVC_CT_AE_MODE_CONTROL                          0x02
#define UVC_CT_AE_PRIORITY_CONTROL                      0x03
#define UVC_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL           0x04
#define UVC_CT_EXPOSURE_TIME_RELATIVE_CONTROL           0x05
#define UVC_CT_FOCUS_ABSOLUTE_CONTROL                   0x06
#define UVC_CT_FOCUS_RELATIVE_CONTROL                   0x07
#define UVC_CT_FOCUS_AUTO_CONTROL                       0x08
#define UVC_CT_IRIS_ABSOLUTE_CONTROL                    0x09
#define UVC_CT_IRIS_RELATIVE_CONTROL                    0x0A
#define UVC_CT_ZOOM_ABSOLUTE_CONTROL                    0x0B
#define UVC_CT_ZOOM_RELATIVE_CONTROL                    0x0C
#define UVC_CT_PANTILT_ABSOLUTE_CONTROL                 0x0D
#define UVC_CT_PANTILT_RELATIVE_CONTROL                 0x0E
#define UVC_CT_ROLL_ABSOLUTE_CONTROL                    0x0F
#define UVC_CT_ROLL_RELATIVE_CONTROL                    0x10
#define UVC_CT_PRIVACY_CONTROL                          0x11
#define UVC_CT_FOCUS_SIMPLE_CONTROL                     0x12
#define UVC_CT_WINDOW_CONTROL                           0x13
#define UVC_CT_REGION_OF_INTEREST_CONTROL               0x14
/* A.9.5. Processing Unit Control Selectors */
#define UVC_PU_CONTROL_UNDEFINED                        0x00
#define UVC_PU_BACKLIGHT_COMPENSATION_CONTROL           0x01
#define UVC_PU_BRIGHTNESS_CONTROL                       0x02
#define UVC_PU_CONTRAST_CONTROL                         0x03
#define UVC_PU_GAIN_CONTROL                             0x04
#define UVC_PU_POWER_LINE_FREQUENCY_CONTROL             0x05
#define UVC_PU_HUE_CONTROL                              0x06
#define UVC_PU_SATURATION_CONTROL                       0x07
#define UVC_PU_SHARPNESS_CONTROL                        0x08
#define UVC_PU_GAMMA_CONTROL                            0x09
#define UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL        0x0A
#define UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL   0x0B
#define UVC_PU_WHITE_BALANCE_COMPONENT_CONTROL          0x0C
#define UVC_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL     0x0D
#define UVC_PU_DIGITAL_MULTIPLIER_CONTROL               0x0E
#define UVC_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL         0x0F
#define UVC_PU_HUE_AUTO_CONTROL                         0x10
#define UVC_PU_ANALOG_VIDEO_STANDARD_CONTROL            0x11
#define UVC_PU_ANALOG_LOCK_STATUS_CONTROL               0x12
#define UVC_PU_CONTRAST_AUTO_CONTROL                    0x13
/* A.9.6. Encoding Unit Control Selectors */
#define UVC_EU_CONTROL_UNDEFINED                        0x00
#define UVC_EU_SELECT_LAYER_CONTROL                     0x01
#define UVC_EU_PROFILE_TOOLSET_CONTROL                  0x02
#define UVC_EU_VIDEO_RESOLUTION_CONTROL                 0x03
#define UVC_EU_MIN_FRAME_INTERVAL_CONTROL               0x04
#define UVC_EU_SLICE_MODE_CONTROL                       0x05
#define UVC_EU_RATE_CONTROL_MODE_CONTROL                0x06
#define UVC_EU_AVERAGE_BITRATE_CONTROL                  0x07
#define UVC_EU_CPB_SIZE_CONTROL                         0x08
#define UVC_EU_PEAK_BIT_RATE_CONTROL                    0x09
#define UVC_EU_QUANTIZATION_PARAMS_CONTROL              0x0A
#define UVC_EU_SYNC_REF_FRAME_CONTROL                   0x0B
#define UVC_EU_LTR_BUFFER_ CONTROL                      0x0C
#define UVC_EU_LTR_PICTURE_CONTROL                      0x0D
#define UVC_EU_LTR_VALIDATION_CONTROL                   0x0E
#define UVC_EU_LEVEL_IDC_LIMIT_CONTROL                  0x0F
#define UVC_EU_SEI_PAYLOADTYPE_CONTROL                  0x10
#define UVC_EU_QP_RANGE_CONTROL                         0x11
#define UVC_EU_PRIORITY_CONTROL                         0x12
#define UVC_EU_START_OR_STOP_LAYER_CONTROL              0x13
#define UVC_EU_ERROR_RESILIENCY_CONTROL                 0x14
/* A.9.7. Extension Unit Control Selectors */
#define UVC_XU_CONTROL_UNDEFINED                        0x00
/* A.9.8. VideoStreaming Interface Control Selectors */
#define UVC_VS_CONTROL_UNDEFINED                        0x00
#define UVC_VS_PROBE_CONTROL                            0x01
#define UVC_VS_COMMIT_CONTROL                           0x02
#define UVC_VS_STILL_PROBE_CONTROL                      0x03
#define UVC_VS_STILL_COMMIT_CONTROL                     0x04
#define UVC_VS_STILL_IMAGE_TRIGGER_CONTROL              0x05
#define UVC_VS_STREAM_ERROR_CODE_CONTROL                0x06
#define UVC_VS_GENERATE_KEY_FRAME_CONTROL               0x07
#define UVC_VS_UPDATE_FRAME_SEGMENT_CONTROL             0x08
#define UVC_VS_SYNCH_DELAY_CONTROL                      0x09
/* B.1. USB Terminal Types */
#define UVC_TT_VENDOR_SPECIFIC                          0x0100
#define UVC_TT_STREAMING                                0x0101
/* B.2. Input Terminal Types */
#define UVC_ITT_VENDOR_SPECIFIC                         0x0200
#define UVC_ITT_CAMERA                                  0x0201
#define UVC_ITT_MEDIA_TRANSPORT_INPUT                   0x0202
/* B.3. Output Terminal Types */
#define UVC_OTT_VENDOR_SPECIFIC                         0x0300
#define UVC_OTT_DISPLAY                                 0x0301
#define UVC_OTT_MEDIA_TRANSPORT_OUTPUT                  0x0302
/* B.4. External Terminal Types */
#define UVC_EXTERNAL_VENDOR_SPECIFIC                    0x0400
#define UVC_COMPOSITE_CONNECTOR                         0x0401
#define UVC_SVIDEO_CONNECTOR                            0x0402
#define UVC_COMPONENT_CONNECTOR                         0x0403
/* 2.4.2.2. Status Packet Type */
#define UVC_STATUS_TYPE_CONTROL                         1
#define UVC_STATUS_TYPE_STREAMING                       2
/* 2.4.3.3. Payload Header Information */
#define UVC_STREAM_EOH                                  (1 << 7)
#define UVC_STREAM_ERR                                  (1 << 6)
#define UVC_STREAM_STI                                  (1 << 5)
#define UVC_STREAM_RES                                  (1 << 4)
#define UVC_STREAM_SCR                                  (1 << 3)
#define UVC_STREAM_PTS                                  (1 << 2)
#define UVC_STREAM_EOF                                  (1 << 1)
#define UVC_STREAM_FID                                  (1 << 0)
/* 4.1.2. Control Capabilities */
#define UVC_CONTROL_CAP_GET                             (1 << 0)
#define UVC_CONTROL_CAP_SET                             (1 << 1)
#define UVC_CONTROL_CAP_DISABLED                        (1 << 2)
#define UVC_CONTROL_CAP_AUTOUPDATE                      (1 << 3)
#define UVC_CONTROL_CAP_ASYNCHRONOUS                    (1 << 4)
/* Data types for UVC control data */
#define UVC_CTRL_DATA_TYPE_RAW                           0
#define UVC_CTRL_DATA_TYPE_SIGNED                        1
#define UVC_CTRL_DATA_TYPE_UNSIGNED                      2
#define UVC_CTRL_DATA_TYPE_BOOLEAN                       3
#define UVC_CTRL_DATA_TYPE_ENUM                          4
#define UVC_CTRL_DATA_TYPE_BITMASK                       5
/* Format flags */
#define UVC_FMT_FLAG_COMPRESSED                        0x00000001
#define UVC_FMT_FLAG_STREAM                            0x00000002


/*  Class-Specific VC Header Descriptor */
struct uvc_hdesc_s {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bcdUVC[2];
	uint8_t wTotalLength[2];
	uint8_t dwClockFrequency[4];
	uint8_t bInCollection;
	uint8_t baInterfaceNr[0];
};

/* VC Input Terminal Descriptor */
struct uvc_itdesc_s {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bTerminalID;
	uint8_t wTerminalType[2];
	uint8_t bAssocTerminal;
	uint8_t iTerminal;
	uint8_t wObjectiveFocalLengthMin[2];
	uint8_t wObjectiveFocalLengthMax[2];
	uint8_t wOcularFocalLength[2];
	uint8_t bControlSize;
	uint8_t bmControls[3];
};

/* VC Output Terminal Descriptor */
struct uvc_otdesc_s {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bTerminalID;
	uint8_t wTerminalType[2];
	uint8_t bAssocTerminal;
	uint8_t bSourceID;
	uint8_t iTerminal;
};

/* Feature Descriptor */
struct uvc_featdesc_s {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bUnitID;
	uint8_t bSourceID;
	uint8_t bControlSize;
	uint8_t bmaControls[0];
};

/* VC Selector Unit Descriptor */
struct uvc_seldesc_s {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bUnitID;
	uint8_t bNrInPins;
	uint8_t bSourceID[0];
	/*uint8_t iSelector; */
};

/* VC Processing Unit Descriptor */
struct uvc_procdesc_s {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bUnitID;
	uint8_t bSourceID;
	uint8_t wMaxMultiplier[2];
	uint8_t bControlSize;
	uint8_t bmControls[0];
	/* bmControls*bControlSize followed by iProcessing */
	/*uint8_t iProcessing; */
};

/* VC Extension Unit Descriptor */
struct uvc_extdesc_s {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bUnitID;
	uint8_t guidExtensionCode[16];
	uint8_t bNumControls;
	uint8_t bNrInPins;
	uint8_t baSourceID[0];
	/* bmControls*bControlSize followed by iExtension */
	/*uint8_t iExtension; */
};

/* VS Input Header Descriptor */
struct uvc_inhdesc_s {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;	//must be UVC_VS_INPUT_HEADER
	uint8_t bNumFormats;
	uint8_t wTotalLength[2];
	uint8_t bEndPointAddress;
	uint8_t bmInfo;
	uint8_t bTerminalLink;
	uint8_t bStillCaptureMethod;
	uint8_t bTriggerSupport;
	uint8_t bTriggerUsage;
	uint8_t bControlSize;
	uint8_t bmaControls[0];
};

/* VS MJPEG Format Descriptor */
struct uvc_mjpegformatdesc_s {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bFormatIndex;
	uint8_t bNumFrameDescriptors;
	uint8_t bmFlags;
	uint8_t bDefaultFrameIndex;
	uint8_t bAspectRatioX;
	uint8_t bAspectRatioY;
	uint8_t bmInterlaceFlags;
	uint8_t bCopyProtect;
};

/* VS MJPEG Frame Descriptor */
struct uvc_mjpegframedesc_s {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bFrameIndex;
	uint8_t bmCapabilities;
	uint8_t wWidth[2];
	uint8_t wHeight[2];
	uint8_t dwMinBitRate[4];
	uint8_t dwMaxBitRate[4];
	uint8_t dwDefaultFrameInterval[4];
	uint8_t bFrameIntervalType;
	uint32_t dwFrameInterval[0];
	//dwFrameInterval*N is here
};

/* VS Uncompressed Format Type Descriptor */
struct uvc_ucformatdesc_s {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bFormatIndex;
	uint8_t bNumFrameDescriptors;
	uint8_t guidFormat[16];
	uint8_t bBitsPerPixel;
	uint8_t bDefaultFrameIndex;
	uint8_t bAspectRatioX;
	uint8_t bAspectRatioY;
	uint8_t bmInterfaceFlags;
	uint8_t bCopyProtect;
};

/* VS Uncompressed Frame Descriptor */
struct uvc_ucframedesc_s {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;	//must be UVC_VS_INPUT_HEADER
	uint8_t bFrameIndex;
	uint8_t bmCapabilities;
	uint8_t wWidth[2];
	uint8_t wHeight[2];
	uint8_t dwMinBitRate[4];
	uint8_t dwMaxBitRate[4];
	uint8_t dwDefaultFrameInterval[4];
	uint8_t bFrameIntervalType;
	uint32_t dwFrameInterval[0];
	//dwFrameInterval*N is here
};

/* VS Stream Control request */
struct uvc_streaming_control {
	uint16_t bmHint;
	uint8_t bFormatIndex;
	uint8_t bFrameIndex;
	uint32_t dwFrameInterval;
	uint16_t wKeyFrameRate;
	uint16_t wPFrameRate;
	uint16_t wCompQuality;
	uint16_t wCompWindowSize;
	uint16_t wDelay;
	uint32_t dwMaxVideoFrameSize;
	uint32_t dwMaxPayloadTransferSize;
	uint32_t dwClockFrequency;
	uint8_t bmFramingInfo;
	uint8_t bPreferedVersion;
	uint8_t bMinVersion;
	uint8_t bMaxVersion;
};

/* UVC standard streaming header structure */
struct uvc_streaming_header {
	uint8_t bNumFormats;
	uint8_t bEndpointAddress;
	uint8_t bTerminalLink;
	uint8_t bControlSize;
	uint8_t *bmaControls;
	uint8_t bmInfo;
	uint8_t bStillCaptureMethod;
	uint8_t bTriggerSupport;
	uint8_t bTriggerUsage;
};

/* UVC camera driver related structures */

/* Control information structure */
struct uvc_control_info {
	uint8_t entity[16];                 /* GUID of entity */
	uint8_t index;                      /* Index of control */
	uint8_t selector;                   /* UVC selector ID */
	uint16_t size;                      /* Size of control item */
	uint32_t flags;                     /* Flag to mark the supported features */
	struct dq_queue_s mappings;         /* V4L2 mapping queue */
};

/* V4L2, UVC control mapping */
struct uvc_control_mapping {
	uint32_t class;                     /* V4L2 control class */
	uint32_t id;                        /* V4L2 control ID */
	uint8_t name[32];                   /* Name of the control */
	uint8_t entity[16];                 /* GUID of entity */
	uint8_t selector;                   /* UVC selector ID */
	uint8_t size;                       /* Size of control */
	uint8_t offset;                     /* Offset of control */
	enum v4l2_ctrl_type v4l2_type;      /* V4L2 control type */
	uint32_t data_type;                 /* UVC data type */
	struct uvc_menu_info *menu_info;    /* Pointer to menu info */
	uint32_t menu_count;                /* Count of menu items */
	uint32_t master_id;                 /* Master ID of this control */
	uint32_t master_class;              /* Master class of this control item */
	int32_t master_manual;              /* Master Manual */
	uint32_t slave_ids[2];              /* Slave ID's of the control item */
	struct dq_entry_s list;             /* flink/blink for this control to add in queue */
};

/* UVC camera driver control */
struct uvc_control {
	struct uvc_entity *entity;          /* Pointer to the entity */
	struct uvc_control_info info;       /* Control information */
	uint8_t index;                      /* Used to match the uvc_control entry with a uvc_control_info. */
	uint8_t status;                     /* Various states of control, supported, loaded, modified etc */
	uint8_t *max;                       /* Pointer to hold the max value supported by this control item */
	uint8_t *min;                       /* Pointer to hold the min value supported by this control item */
	uint8_t *def;                       /* Pointer to hold the default value supported by this control item */
	uint8_t *cur;                       /* Pointer to hold the current value supported by this control item */
	uint8_t *bak;                       /* Pointer to hold the backup value supported by this control item */
	uint8_t *step;                      /* Pointer to hold the step value supported by this control item */

};

/* UVC format descriptor table */
struct uvc_format_desc {
	char *name;                         /* Name of the format */
	uint8_t guid[16];                   /* GUID of the format */
	uint32_t fcc;                       /* FCC */
};

#define UVC_ENTITY_FLAG_DEFAULT         (1 << 0)
/* UVC camera driver entity structure to represent the units and terminals */
struct uvc_entity {
	struct dq_entry_s list;             /* flink/blink of entity list */
	struct dq_queue_s chain;            /* Entity as part of a video chain */
	unsigned int flags;                 /* Flag to represent the features */

	uint8_t id;                         /* ID of this entity */
	uint16_t type;                      /* Type of this entity */
	char name[64];                      /* Name of this entity */

	union {
		/* Input Unit(Camera) */
		struct {
			uint16_t wObjectiveFocalLengthMin;/* Objective Focal length min */
			uint16_t wObjectiveFocalLengthMax;/* Objective Focal length min */
			uint16_t wOcularFocalLength;/* Ocular Focal length */
			uint8_t bControlSize;      /* Camera Control size */
			uint8_t *bmControls;       /* Pointer to camera controls array */
		} camera;
		/* Processing unit(PU) */
		struct {
			uint16_t wMaxMultiplier;   /* Max Multiplier supported */
			uint8_t bControlSize;      /* Processing unit control size */
			uint8_t *bmControls;       /* Pointer to controls array */
		} processing;
		/* Extention unit(EU) */
		struct {
			uint8_t guidExtensionCode[16]; /* GUID of EU */
			uint8_t bNumControls;       /* Number of controls supported by EU */
			uint8_t bControlSize;       /* Control size, or number of control items supported */
			uint8_t *bmControls;        /* Pointer to array of control items */
			uint8_t *bmControlsType;    /* Pointer to array of controle types */
		} extension;
	};

	uint8_t bNrInPins;                  /* Number of input pins */
	uint8_t *baSourceID;                /* Pointer to Source ID array */

	unsigned int ncontrols;             /* Number of controls supported */
	struct uvc_control *controls;       /* Pointer to Control object */
};

/* UVC camera driver video frame structure */
struct uvc_frame {
	struct dq_entry_s list;             /* flink/blink for frames queue */
	uint8_t bFrameIndex;                /* frame index */
	uint8_t bmCapabilities;             /* Capabilities */
	uint16_t wWidth;                    /* Width of the video frame */
	uint16_t wHeight;                   /* Height of the video frame */
	uint32_t dwMinBitRate;              /* Min video bit rate */
	uint32_t dwMaxBitRate;              /* Max video bit rate */
	uint32_t dwMaxVideoFrameBufferSize; /* Video frame buffer size*/
	uint8_t bFrameIntervalType;         /* Frame interval type*/
	uint32_t dwDefaultFrameInterval;    /* Default Frame interval */
	uint32_t dwFrameInterval[];         /* Array of frame intervals */
};

/* UVC camera driver video format structure */
struct uvc_format {
	uint8_t type;                       /* Format type */
	uint8_t index;                      /* Format Index */
	uint8_t bpp;                        /* Supported bit per pixel */
	uint8_t colorspace;                 /* Colorspace supported by the format */
	uint32_t fcc;
	uint32_t flags;                     /* Flags to set the features */
	char name[32];                      /* Format name MJPEG, YUV etc*/
	uint8_t ftype;                      /* Format type */
	unsigned int nframes;               /* Number of frames */
	struct dq_queue_s frames;           /* Queue to hold the frames associated with format */
	struct dq_entry_s list;             /* flink/blink for format queue */
};

/* UVC camera driver video chain structure */
struct uvc_video_chain {
	struct usbhost_uvc_state_s *dev;            /* Reference to device object */
	struct dq_entry_s list;                     /* flink/blink for the object */
	struct dq_queue_s entities;                 /* Queue of all entities */
	struct uvc_entity *processing;              /* Processing unit object*/
	struct uvc_entity *selector;                /* Selector unit object*/
	sem_t ctrl_mutex;                           /* Protects ctrl.info */
};

/* Circular buffer for reading stream data */
#define UVC_MAX_TRF_BUF 8
struct uvc_buf_object {
	uint8_t * buffer;                             /* Transfer Buffer */
	uint32_t bufsize;                             /* Size of transfer buffer */
	uint32_t recsize;                             /* Recevied data size */
	uint8_t *context;                             /* Device context passed to USB HAL */
};

struct uvc_circ_buf {
	struct uvc_buf_object *bobj[UVC_MAX_TRF_BUF]; /* Transfer Object holder */
	int head;                                     /* Head pointer of Circular Buffer */
	int tail;                                     /* Tail pointer of Circuler Buffer */
	int maxlen;                                   /* Max size of Circular Buffer */
};

/* UVC Camera driver stream structure */
struct uvc_streaming {
	struct dq_entry_s list;                       /* object next/prev links */
	struct usbhost_uvc_state_s *dev;              /* device object reference */
	struct uvc_video_chain *chain;                /* Video chain for this stream */
	int intfnum;                                  /* Id of interface to which this stream is attached */
	int altinfcount;                              /* Id of aternate interface */
	uint16_t maxpsize;                            /* Max packet size supported by the endpoint in this interface */
	struct uvc_streaming_header header;           /* Stream header information */
	enum v4l2_buf_type type;                      /* V4L2 buffer type, Capture/Still etc */
	unsigned int nformats;                        /* Number of formats supported by this stream */
	struct uvc_streaming_control ctrl;            /* Reference to stream control object */
	struct uvc_format *def_format;                /* Reference to default format */
	struct uvc_format *cur_format;                /* Reference to current format */
	struct uvc_frame *cur_frame;                  /* Reference to current frame */
	struct dq_queue_s formats;                    /* Queue of format associate to this stream */
	sem_t mutex;                                  /* Mutux to protect this stream object */
	/* Stream transfer related items */
	struct work_s trfwork;                        /* Work queue handle */
	usbhost_ep_t bulkin;                          /* Bulk IN endpoint */
	usbhost_ep_t isocin;                          /* ISOC IN endpoint */
	struct uvc_buf_object bufobj[UVC_MAX_TRF_BUF];/* Memory for tansfer buffer object */
	struct uvc_circ_buf cbuf;                     /* Circular Buffer */
	struct uvc_circ_buf fbuf;                     /* Circular Buffer */
	uint32_t bufsize;                             /* Transfer buffer size */
	uint8_t *reqbuff;                             /* User request buffer */
	uint32_t reqsize;                             /* User request buffer size */
	uint32_t trfsize;                             /* Data recevied so far from camera */
	bool reqcancel;                               /* Request status */
	enum v4l2_buf_type reqtype;                   /* Type of the buffer stream or capture */
	bool active;                                  /* Represents the stream status */
	uint32_t sequence;                            /* Sequence number of the frame */
	uint8_t last_fid;                             /* Last Frame ID */
	sem_t trfmutex;                               /* Mutex to protect transfer buffer */
	sem_t waitsem;                                /* Signal buffer write thread */
};

/* USB Endpoint object structure */
struct usb_ep_desc {
	struct usb_epdesc_s ep;             /* Endpoint descriptor */
	struct dq_entry_s list;             /* flink/blink for the chaining endpoint */
};

/* USB Interface & Endpoint list structure */
struct usb_inf_desc {
	struct usb_ifdesc_s intf;           /* Interface descriptor */
	struct dq_queue_s ep;               /* Endpoint queue */
	struct dq_entry_s list;             /* flink/blink for the chaining interface */
};

/* USB Configuration & interface list structure */
struct usb_conf_desc {
	struct usb_cfgdesc_s cfg;           /* Configuration descriptor */
	struct dq_queue_s interface;        /* Interface queue */
	int ifcount;                        /* Interface count */
};

/* UVC control data structure */
struct uvc_ctrl_value {
	uint32_t step;                      /* Step supported */
	int32_t min;                        /* Minimum value supported */
	int32_t max;                        /* Maximum value supported */
	int32_t def;                        /* Default value supported */
};

/* UVC menu structure */
struct uvc_menu_info {
	uint32_t value;                     /* Menu value */
	uint8_t name[32];                   /* Menu name */
};

/* This structure contains the internal, private state of the USB host class
 * driver.
 */

struct usbhost_uvc_state_s {
	/* This is the externally visible portion of the state */
	struct usbhost_class_s usbclass;

	/* The remainder of the fields are provide to the class driver */

	uint8_t minor;                      /* Integer identifying the /dev/video[n] device */
	volatile bool disconnected;         /* TRUE: Device has been disconnected */
	int16_t crefs;                      /* Reference count on the driver instance */
	sem_t exclsem;                      /* Used to maintain mutual exclusive access */
	struct work_s work;                 /* For interacting with the worker thread */
	FAR uint8_t *tbuffer;               /* The allocated transfer buffer */
	size_t tbuflen;                     /* Size of the allocated transfer buffer */
	FAR uint8_t *ctrlreq;               /* Allocated ctrl request structure */
	FAR uint8_t *inbuf;                 /* Allocated RX buffer for the ISOC IN endpoint */
	uint16_t pktsize;                   /* Allocated size of ISOC transfer buffers */
	FAR void *video_priv;               /* Video private data */
	uint16_t uvc_version;               /* USB Video class version information */
	uint32_t clock_frequency;           /* UVC clock frequency */
	struct dq_queue_s entities;         /* Queue of entities */
	struct dq_queue_s chains;           /* Queue of video chain */
	struct dq_queue_s streams;          /* Queue of Streaming interfaces */
	struct uvc_streaming *stream;       /* Current active stream, here in this case camera stream object */
	uint8_t intfnum;
	struct usb_conf_desc config;        /* Device configuration, interface, endpoint details */
	sem_t opsem;                        /* Used to maintain mutual exclusive access */
	pid_t thpid;                        /* The worker thread task ID */
	volatile uint8_t thstate;           /* State of the worker thread */
};

typedef struct usbhost_uvc_state_s uvc_state_t;


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Video */
int uvc_video_init(struct uvc_streaming *stream);
void uvc_video_deinit(struct uvc_streaming *stream);
int uvc_video_start_streaming(struct uvc_streaming *stream);
void uvc_video_stop_streaming(struct uvc_streaming *stream);
int uvc_probe_video(struct uvc_streaming *stream, struct uvc_streaming_control *probe);
int uvc_query_ctrl(FAR uvc_state_t *dev, uint8_t query, uint8_t unit, uint8_t intfnum, uint8_t cs, void *data, uint16_t size);
int uvc_video_init_transfer(struct uvc_streaming *stream);
struct uvc_format *uvc_format_by_fcc(FAR struct uvc_streaming *stream, uint32_t fcc);
struct uvc_format *uvc_format_by_index(FAR struct uvc_streaming *stream, uint8_t index);
void uvc_circ_bbuf_init(struct uvc_circ_buf *c);

/* Controls */
int uvc_query_v4l2_ctrl(struct uvc_video_chain *chain, struct v4l2_queryctrl *v4l2_ctrl);
int uvc_query_v4l2_menu(struct uvc_video_chain *chain, struct v4l2_querymenu *query_menu);
int uvc_ctrl_add_mapping(struct uvc_video_chain *chain, const struct uvc_control_mapping *mapping);
int uvc_ctrl_init_device(FAR uvc_state_t *dev);
void uvc_ctrl_deinit_device(uvc_state_t *dev);
int uvc_ctrl_restore_values(uvc_state_t *dev);
int uvc_ctrl_commit(struct uvc_video_chain *chain, int rollback, const struct v4l2_ext_control *xctrls, unsigned int xctrls_count);
int uvc_ctrl_get(struct uvc_video_chain *chain, struct uvc_control *ctrl, uint16_t ctrl_class, struct v4l2_ext_control *xctrl);
int uvc_ctrl_set(struct uvc_video_chain *chain, uint16_t ctrl_class, struct v4l2_ext_control *xctrl);
int uvc_ctrl_populate_values(struct uvc_video_chain *chain, struct uvc_control *ctrl);
struct uvc_control *uvc_search_control(struct uvc_video_chain *chain, uint32_t v4l2_id, uint32_t v4l2_class, struct uvc_control_mapping **mapping);

/* (Little Endian) Data helpers */
uint16_t usbhost_uvc_getle16(const uint8_t *val);
void usbhost_uvc_putle16(uint8_t *dest, uint16_t val);
uint32_t usbhost_uvc_getle32(const uint8_t *val);
void usbhost_uvc_putle32(uint8_t *dest, uint32_t val);

/* Semaphores */
void usbhost_uvc_takesem(sem_t *sem);
#define usbhost_uvc_givesem(s) sem_post(s);

int usbhost_uvc_setinterface(FAR uvc_state_t *dev, uint8_t intfnum, uint8_t atlintf);
struct usb_inf_desc *uvc_interface_by_number(FAR uvc_state_t *dev, int ifno, int altno);
struct usb_ep_desc *uvc_endpoint_by_address(struct usb_inf_desc *intf, int address);
int32_t uvc_get_value(struct uvc_control_mapping *mapping, uint8_t query, const uint8_t *data);

#ifdef __cplusplus
}
#endif
#endif							/* __USBH_VIDEO_H */
