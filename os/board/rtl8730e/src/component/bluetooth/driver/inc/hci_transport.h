/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef _HCI_TRANSPORTS_H_
#define _HCI_TRANSPORTS_H_

#include "hci_common.h"

typedef struct
{
    /* Open the HCI proto transport */
    uint8_t (*open)(void);

    /* Close the HCI proto transport */
    uint8_t (*close)(void);

    /* Free the HCI proto transport */
    uint8_t (*free_ops)(void);

    /* Send HCI buffer to bus */
    uint16_t (*send)(uint8_t type, uint8_t *buf, uint16_t len, uint8_t is_reserved);

    /* Set Targte where HCI send to */
    void (*set_recv)(HCI_RECV hci_recv);

    /* Set HCI buffer Source */
    void (*set_get_buf)(HCI_RECV_GET_BUF get_buf);

    /* Recv bus buffer to HCI drv */
    uint8_t (*recv_ind)(void);

} HCI_TRANSPORT_OPS;

extern HCI_TRANSPORT_OPS hci_transport_ops;

static inline void hci_transport_set_recv(HCI_RECV hci_recv)
{
    hci_transport_ops.set_recv(hci_recv);
}

static inline void hci_transport_set_get_buf(HCI_RECV_GET_BUF get_buf)
{
    hci_transport_ops.set_get_buf(get_buf);
}

static inline uint8_t hci_transport_recv_ind(void)
{
    return hci_transport_ops.recv_ind();
}

static inline uint16_t hci_transport_send(uint8_t type, uint8_t *buf, uint16_t len, uint8_t is_reserved)
{
    return hci_transport_ops.send(type, buf, len, is_reserved);
}

static inline uint8_t hci_transport_open(void)
{
    return hci_transport_ops.open();
}

static inline uint8_t hci_transport_close(void)
{
    return hci_transport_ops.close();
}

static inline uint8_t hci_transport_free(void)
{
    return hci_transport_ops.free_ops();
}

#endif