/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include "osif.h"
#include "hci_config.h"
#include "hci_common.h"
#include "hci_transport.h"
#include "hci_platform.h"
#include "hci_dbg.h"
#include "rtk_coex.h"
#include <string.h>

#define BT_HCI_EVT_LE_META_EVENT         0x3e
#define BT_HCI_EVT_LE_ADVERTISING_REPORT 0x02

#if defined (__ICCARM__)
#define __H4_PACKED 
#else
#define __H4_PACKED __attribute__ ((packed))
#endif

struct bt_hci_acl_hdr {
    uint16_t handle;
    uint16_t len;
} __H4_PACKED;

struct bt_hci_sco_hdr {
    uint16_t handle;
    uint8_t len;
} __H4_PACKED;

struct bt_hci_evt_hdr {
    uint8_t  evt;
    uint8_t  len;
} __H4_PACKED;

struct bt_hci_iso_hdr {
    uint16_t handle;
    uint16_t len;
} __H4_PACKED;

typedef union {
    struct bt_hci_evt_hdr evt;
    struct bt_hci_acl_hdr acl;
    struct bt_hci_iso_hdr iso;
    struct bt_hci_sco_hdr sco;
} HCI_HDR;

static struct hci_h4_t {
    void*    rx_ind_sema;
    void*    rx_run_sema;
    void*    rx_thread_hdl;
    uint8_t  rx_run;
    HCI_RECV recv;
    HCI_RECV_GET_BUF get_buf;
} *hci_h4 = NULL;

static uint8_t h4_recv_ind(void)
{
    if(!hci_h4->rx_run)
        return HCI_FAIL;

    osif_sem_give(hci_h4->rx_ind_sema);
    return HCI_SUCCESS;
}

static uint16_t h4_recv_data(uint8_t *buf, uint16_t len)
{
    uint16_t read_len = 0;
    uint16_t read_len_per = 0;

    while (1)
    {
        read_len_per = hci_uart_read(buf+read_len, len - read_len);
        read_len += read_len_per;
        if (read_len == len)
            break;
        if (0 == read_len_per)
        {
            osif_sem_take(hci_h4->rx_ind_sema, 0xffffffff);
            if(!hci_h4->rx_run)
                return 0;
        }
    }

    /* if read_len is not equal len, mean ERROR! */
    if (read_len != len)
        HCI_ERR("\n\rH4 Read FAILED! EXIT H4 RX Thread!");
    return read_len;
}

static void h4_rx_thread(void *context)
{
    (void)context;
    HCI_HDR hdr;
    uint8_t buffer[CONFIG_HCI_RX_BUF_LEN];
    uint8_t type, hdr_len, discardable, sub_event, *buf;
    uint16_t body_len;

    osif_sem_give(hci_h4->rx_run_sema);

    while (1) {
        type = H4_NONE; hdr_len = 0; body_len = 0; discardable = 0; sub_event = 0; buf = 0;
        //osif_sem_take(hci_h4->rx_ind_sema, BT_TIMEOUT_FOREVER);

        /* Read H4 Type */
        if (sizeof(type) != h4_recv_data(&type, sizeof(type)))
            break;

        if (H4_EVT == type)
            hdr_len = sizeof(struct bt_hci_evt_hdr);
        else if (type == H4_ACL)
            hdr_len = sizeof(struct bt_hci_acl_hdr);
#if HCI_ISO_DATA_PACKET
        else if (type == H4_ISO)
            hdr_len = sizeof(struct bt_hci_iso_hdr);
#endif
        else if (type == H4_SCO)
            hdr_len = sizeof(struct bt_hci_sco_hdr);
        else
            break;

        /* Read HCI Header */
        if (hdr_len != h4_recv_data((uint8_t *)&hdr, hdr_len))
            break;

        if (H4_EVT == type)
        {
            if (BT_HCI_EVT_LE_META_EVENT == hdr.evt.evt)
            {
                /* The first event parameter is always a subevent code identifying the specific event for LE meta event.
                   So the len should not be 0. */
                if (hdr.evt.len == 0)
                    break;

                if (sizeof(sub_event) != h4_recv_data(&sub_event, sizeof(sub_event)))
                    break;

                if (BT_HCI_EVT_LE_ADVERTISING_REPORT == sub_event)
                    discardable = 1;
            }

            body_len = hdr.evt.len;
            if (hci_h4->get_buf && hci_h4->recv)
                buf = hci_h4->get_buf(H4_EVT, hdr_len + body_len, discardable ? 0 : 0xffffffff);
            if (!buf)
            {
                if (discardable)
                {
                    if (body_len - 1 > CONFIG_HCI_RX_BUF_LEN)
                        break;
                    if (body_len - 1 != h4_recv_data(buffer, body_len - 1))
                        break;
                    continue;
                }
                else
                    break;
            }
            memcpy(buf, &hdr, hdr_len);
            if (BT_HCI_EVT_LE_META_EVENT == hdr.evt.evt)
            {
                buf[sizeof(struct bt_hci_evt_hdr)] = sub_event;
                hdr_len++;
                body_len--;
            }
        } 
        else if (H4_ACL == type)
        {
            body_len = hdr.acl.len;
            if (hci_h4->get_buf && hci_h4->recv)
                buf = hci_h4->get_buf(H4_ACL, hdr_len + body_len, 0xffffffff);
            if (!buf)
                break;
            memcpy(buf, &hdr, hdr_len);
        }
#if HCI_ISO_DATA_PACKET
        else if (H4_ISO == type)
        {
            body_len = hdr.iso.len;
            if (hci_h4->get_buf && hci_h4->recv)
                buf = hci_h4->get_buf(H4_ISO, hdr_len + body_len, 0xffffffff);
            if (!buf)
                break;
            memcpy(buf, &hdr, hdr_len);
        }
#endif
        else if (H4_SCO == type)
        {
            body_len = hdr.sco.len;
            if (hci_h4->get_buf && hci_h4->recv)
                buf = hci_h4->get_buf(H4_SCO, hdr_len + body_len, 0xffffffff);
            if (!buf)
                break;
            memcpy(buf, &hdr, hdr_len);
        }
        else
            break;

        /* Read HCI Body */
        if (body_len != h4_recv_data(buf + hdr_len, body_len))
            break;

        bt_coex_process_rx_frame(type, buf, hdr_len + body_len);

        //HCI_DUMP(type, 1, buf, hdr_len + body_len);

        if(hci_h4->recv)
            hci_h4->recv(type, buf, hdr_len + body_len);
    }
    
    osif_sem_give(hci_h4->rx_run_sema);

    osif_task_delete(NULL);
}

static void h4_set_recv(HCI_RECV hci_recv)
{
    hci_h4->recv = hci_recv;
}

static void h4_set_get_buf(HCI_RECV_GET_BUF get_buf)
{
    hci_h4->get_buf = get_buf;
}

static uint16_t h4_send(uint8_t type, uint8_t *buf, uint16_t len, uint8_t is_reserved)
{
    if (type <= H4_NONE || type > H4_ISO)
        return 0;

    bt_coex_process_tx_frame(type, buf, len);

    if (is_reserved) {
        *(buf-1) = type;
        /* Caller only send size of 'len' bytes, so return 'len' */
        return (hci_uart_send(buf-1, len+1)-1);
    } else {
        if (sizeof(type) != hci_uart_send(&type, sizeof(type))) {
            return 0;
        }
        return hci_uart_send(buf, len);
    }
}

static uint8_t h4_open(void)
{
    if (!hci_h4)
    {
        hci_h4 = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct hci_h4_t));
        if (!hci_h4)
            return HCI_FAIL;
        memset(hci_h4, 0, sizeof(struct hci_h4_t));
    }

    bt_coex_init();

    osif_sem_create(&hci_h4->rx_ind_sema, 0, 1);
    osif_sem_create(&hci_h4->rx_run_sema, 0, 1);
    hci_h4->rx_run = 1;
    osif_task_create(&hci_h4->rx_thread_hdl, "h4_rx_thread", h4_rx_thread, 0, CONFIG_HCI_RX_STACK_SIZE, CONFIG_HCI_RX_PRIO);
    osif_sem_take(hci_h4->rx_run_sema, 0xffffffff);

    return HCI_SUCCESS;
}

static uint8_t h4_close(void)
{
    if (!hci_h4)
        return HCI_FAIL;

    hci_h4->rx_run = 0;
    osif_sem_give(hci_h4->rx_ind_sema);
    osif_sem_take(hci_h4->rx_run_sema, 0xffffffff);

    bt_coex_deinit();

    return HCI_SUCCESS;
}

static uint8_t h4_free(void)
{
    if (!hci_h4)
        return HCI_FAIL;

    osif_sem_delete(hci_h4->rx_run_sema);
    osif_sem_delete(hci_h4->rx_ind_sema);

    if (hci_h4)
        osif_mem_free(hci_h4);
    hci_h4 = NULL;

    return HCI_SUCCESS;
}

HCI_TRANSPORT_OPS hci_transport_ops = {
    .open        = h4_open,
    .close       = h4_close,
    .free_ops    = h4_free,
    .send        = h4_send,
    .set_recv    = h4_set_recv,
    .set_get_buf = h4_set_get_buf,
    .recv_ind    = h4_recv_ind,
};
