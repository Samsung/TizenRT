/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */
#include <osif.h>
#include "hci_transport.h"
#include "hci_process.h"

static void* recv_sem = NULL;
static uint8_t* hci_buf;
//static uint16_t hci_buf_len;

uint8_t* hci_sa_recv_get_buf(uint8_t type, uint16_t len, uint32_t timeout)
{
    (void)timeout;
    (void)len;

    if (H4_EVT != type)
        return NULL;
    
    /* Use Send Buf as Recv Buf */
    return hci_buf;
}

uint8_t hci_sa_recv(uint8_t type, uint8_t* buf, uint16_t len)
{
    (void)type;
    (void)buf;
    (void)len;

    osif_sem_give(recv_sem);
    
    return HCI_SUCCESS;
}

uint8_t hci_sa_send(uint8_t type, uint8_t* buf, uint16_t len, uint8_t is_sync)
{
    if (HCI_NOT_SYNC == is_sync)
    {
        if (len != hci_transport_send(type, buf, len, 1))
            return HCI_FAIL;

        return HCI_SUCCESS;
    }

    osif_sem_create(&recv_sem, 0, 1);
    hci_buf = buf;

    if (len != hci_transport_send(type, buf, len, 1))
        return HCI_FAIL;

    osif_sem_take(recv_sem, 0xffffffff);
    osif_sem_delete(recv_sem);
    recv_sem = NULL;
    hci_buf = NULL;

    /* Then We can process Response */
    return HCI_SUCCESS;
}