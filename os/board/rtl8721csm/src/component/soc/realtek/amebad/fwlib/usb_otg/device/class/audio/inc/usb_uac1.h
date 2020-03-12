
/*
 * u_uac1.h -- interface to USB gadget "ALSA AUDIO" utilities
 *
 * Copyright (C) 2008 Bryan Wu <cooloney@kernel.org>
 * Copyright (C) 2008 Analog Devices, Inc
 *
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * Licensed under the GPL-2 or later.
 */
#ifndef __U_AUDIO_H
#define __U_AUDIO_H

#include <platform_opts.h>

#ifdef CONFIG_USBD_AUDIO

#include "usb_composite.h"
#include "audio/inc/usb_audio_if.h"
#include "audio/inc/usb_audio.h"
#include "ameba_otg.h"

#define UAC1_OUT_EP_MAX_PACKET_SIZE     512
#define UAC1_REQ_COUNT                  1

#define UAC1_AUDIO_BUF_SIZE             1024
#define SUPPORT_ISO_MAX_PACKET_SIZE     512 // for 19k2 *2 channel*16 bit

#define UAC1_ISO_OUT_EP_MAX_PACKET_SIZE 576 // must be 576 for 96K + 24-bit, ??? what about P & R ???
#define UAC1_ISO_IN_EP_MAX_PACKET_SIZE  288 // 492, Max Packet, or Play and Record Fail in Win7

#define UAC1_ISO_OUT_EP_MAX_BUFFER_SIZE 576 // 24 -> 32 ( 576 -> 768 ) 
#define UAC1_ISO_IN_EP_MAX_BUFFER_SIZE  288 // 32 -> 24 ( 384 -> 288 ) 

/*
 * This represents the USB side of an audio card device, managed by a USB
 * function which provides control and stream interfaces.
 */

struct gaudio_snd_dev {
    struct gaudio *card;
    //struct file *filp;
    //struct snd_pcm_substream *substream;
    int access;
    int format;// bbpro: 0: 16bit, 1 : 24bit
    int channels;// bbpro:
    int rate;// ex 44100
    int sample_freq_idx;// bbpro, map to rate
};

struct gaudio {
    struct usb_function func;
    struct usb_gadget *gadget;

    /* ALSA sound device interfaces */
    struct gaudio_snd_dev control;
    struct gaudio_snd_dev playback;
    struct gaudio_snd_dev capture;
};

struct f_audio {
    struct gaudio card;
    //bbpro
    uint8_t ac_intf;
    uint8_t ac_alt;
    uint8_t as_out_intf;
    uint8_t as_out_alt;
    uint8_t as_in_intf;
    uint8_t as_in_alt;

    /* endpoints handle full and/or high speeds */
    struct usb_ep *out_ep;
    struct usb_ep *in_ep;

    dwc_list_link_t cs;
    uint8_t set_cmd;
    struct usb_audio_control *set_con;
    //bbpro
    struct usb_gadget *gadget;
};

#endif // CONFIG_USBD_AUDIO

#endif // __U_AUDIO_H
