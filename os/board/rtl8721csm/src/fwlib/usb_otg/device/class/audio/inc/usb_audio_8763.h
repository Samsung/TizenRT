#ifndef __USB_AUDIO_8763_H
#define __USB_AUDIO_8763_H

#include <platform_opts.h>

#ifdef CONFIG_USBD_AUDIO

#define INPUT_TERMINAL_2CH_ID       0x01
#define INPUT_TERMINAL_MIC_ID       0x02
#define OUTPUT_TERMINAL_SPEAKER_ID  0x06
#define OUTPUT_TERMINAL_USB_ID      0x07
#define SELECTOR_UNIT_ID            0x08
#define FEATURE_UNIT_0_ID           0x09
#define FEATURE_UNIT_1_ID           0x0A
#define FEATURE_UNIT_2_ID           0x0D
#define MIXER_UNIT_2_ID             0x0F

#define AUDIO_CONTROL_INTF_NUM      0
#define AUDIO_STREAM_OUT_INTF_NUM   1
#define AUDIO_STREAM_IN_INTF_NUM    2

// 8763 PCM setting
#define BIT_RES_16                  16
#define BIT_RES_24                  24
#define DEFAULT_BIT_RES             BIT_RES_16

#define ICODEC_SR_8K                0
#define ICODEC_SR_16K               1
#define ICODEC_SR_32K               2
#define ICODEC_SR_44K               3
#define ICODEC_SR_48K               4
#define ICODEC_SR_88K               5
#define ICODEC_SR_96K               6
#define ICODEC_SR_192K              7
#define DEFAULT_SR                  ICODEC_SR_48K

#define CHANNEL_MODE_1CH            0x01
#define CHANNEL_MODE_2CH            0x02
#define DEFAULT_PLAY_CH             CHANNEL_MODE_2CH
#define DEFAULT_RECORD_CH           CHANNEL_MODE_2CH //CHANNEL_MODE_1CH

typedef struct USB_DSP_STATE_ {
    uint8_t usb_ad_state;
    uint8_t usb_da_state;
} USB_DSP_STATE;

enum { DSP_STOP, DSP_START};

typedef struct USB_ISOEP_STATE_ {
    uint8_t usb_OUTEP_state;
    uint8_t usb_INEP_state;
} USB_ISOEP_STATE;

enum {ISOEP_DISABLE, ISOEP_ENABLE};
enum {IDX_DA = 1, IDX_AD = 2};
enum {L_GAIN = 1, R_GAIN = 2};

#endif // CONFIG_USBD_AUDIO

#endif // __USB_AUDIO_8763_H