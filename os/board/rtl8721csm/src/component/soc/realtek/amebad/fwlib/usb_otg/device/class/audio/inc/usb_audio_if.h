#ifndef __USB_AUDIO_IF_H
#define __USB_AUDIO_IF_H

#include <platform_opts.h>

#ifdef CONFIG_USBD_AUDIO

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void (* plug)(uint8_t audio_path, uint8_t bit_res, uint8_t sf, uint8_t chann_mode);
    void (* unplug)(uint8_t audio_path);
    bool (* put_data)(uint8_t *p_data, uint16_t len);
    void (* set_gain)(uint8_t lr_chann, uint8_t gain);
    bool (* pull_data)(uint8_t *p_data, uint16_t len);
    int (* init)(void);
    void (* deinit)(void);
} usbd_audio_usr_cb_t;

int usbd_audio_register(usbd_audio_usr_cb_t *cb);

#endif // CONFIG_USBD_AUDIO

#endif // __USB_AUDIO_IF_H