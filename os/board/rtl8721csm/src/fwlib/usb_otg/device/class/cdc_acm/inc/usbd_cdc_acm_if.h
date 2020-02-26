#ifndef _USBD_CDC_ACM_IF_H_
#define _USBD_CDC_ACM_IF_H_

#include <platform_opts.h>

#ifdef CONFIG_USBD_CDC_ACM

typedef struct {
    int(* init)(void);
    int(* deinit)(void);
    int(* receive)(void *buf, u16 length);
} usbd_cdc_acm_usr_cb_t;

int usbd_cdc_acm_register(usbd_cdc_acm_usr_cb_t *cb);

void usbd_cdc_acm_set_bulk_transfer_buffer_size(u16 bulk_out_size, u16 bulk_in_size);

int usbd_cdc_acm_transmit_data(void *buf, u16 length);

#endif // CONFIG_USBD_CDC_ACM

#endif // _USBD_CDC_ACM_IF_H_

