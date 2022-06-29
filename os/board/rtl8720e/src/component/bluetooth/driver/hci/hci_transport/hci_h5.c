/**
  ******************************************************************************
  * File           : hci_h5.c
  * Author         : Harvey_Guo
  ******************************************************************************
 * Copyright (C) 2019 Realtek Semiconductor Corporation
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "osif_tp.h"
#include "hci_protocol.h"
#include "hci_ps.h"
#include "hci_tp_dbg.h"
#include "hci_if.h"
#include "hci_if_task.h"
#include "hci_platform_if.h"

//#include "string.h"
/* Private macro -------------------------------------------------------------*/
#ifdef USE_HCI_H5
#if 1
#define HCI_H5_ERR  hci_tp_err
#define HCI_H5_WARN hci_tp_warn
#define HCI_H5_INFO  hci_tp_hci_stack_dbg
#define HCI_H5_DBG  hci_tp_hci_stack_dbg
#else
#define HCI_H5_ERR  tp_osif_printf
#define HCI_H5_WARN(...)
#define HCI_H5_INFO(...)
#define HCI_H5_DBG(...)
#endif

/* Private define ------------------------------------------------------------*/
#define H5_Tx_Retransmit_Timeout    200
/* Used by upper layer */
#ifdef INTERNAL_UPPER_STACK
#define HCI_H5_RX_ACL_PKT_BUF_OFFSET               (HCI_RX_ACL_PKT_BUF_OFFSET+1)
#define HCI_H5_TX_ACL_PKT_BUF_OFFSET               8
#if HCI_ISO_DATA_PACKET
#define H5_RESERVED1                               (HCI_RX_ACL_PKT_BUF_OFFSET+1)
#define HCI_STACK_TX_ISO_RSVD_SIZE                 8
#endif
#else
#define HCI_H5_RX_ACL_PKT_BUF_OFFSET               1
#define HCI_H5_TX_ACL_PKT_BUF_OFFSET               0
#if HCI_ISO_DATA_PACKET
#define H5_RESERVED1                               1
#define HCI_STACK_TX_ISO_RSVD_SIZE                 0
#endif
#endif
#define H5_RESERVEDALIGN          (sizeof(void*)-4)  //for 64bit system
#define BUFF_STRUCT_LEN         (sizeof(struct h5buf))
#define MAX_BUF1_LEN            (BUFF_STRUCT_LEN + HCI_H5_RX_ACL_PKT_BUF_OFFSET + (4 + (4 + 1021) + 2))
#define MAX_BUF2_LEN            (BUFF_STRUCT_LEN + HCI_H5_RX_ACL_PKT_BUF_OFFSET + (4 + (4 + 256) + 2))
/* including the beginning and end char 0xc0 */
#define H5_PREP_BUFLEN          (sizeof(struct h5buf) + (4 + (1021 + 4) + 2) * 2)

#define H5_HDR_SEQ(hdr)         ((hdr)[0] & 0x07)
#define H5_HDR_ACK(hdr)         (((hdr)[0] >> 3) & 0x07)
#define H5_HDR_CRC(hdr)         (((hdr)[0] >> 6) & 0x01)
#define H5_HDR_RELIABLE(hdr)    (((hdr)[0] >> 7) & 0x01)
#define H5_HDR_PKT_TYPE(hdr)    ((hdr)[1] & 0x0f)
#define H5_HDR_LEN(hdr)         ((((hdr)[1] >> 4) & 0xff) + ((hdr)[2] << 4))
#define H5_HDR_SIZE             4

#define HCI_PROTOCOL_CTRL_RETRY_MAX         50

#define H5_CRC_INIT(x)                      x = 0xffff

#define H5_W4_PKT_DELIMITER             0x00
#define H5_W4_PKT_START                 0x01
#define H5_W4_HDR                       0x02
#define H5_W4_DATA                      0x03
#define H5_W4_CRC                       0x04

enum H5_RX_ESC_STATE
{
    H5_ESCSTATE_NOESC,
    H5_ESCSTATE_ESC
};

#define H5_ACK_PKT              0x00
#define HCI_COMMAND_PKT         0x01
#define HCI_ACLDATA_PKT         0x02
#define HCI_SCODATA_PKT         0x03
#define HCI_EVENT_PKT           0x04
/* #define H5_VDRSPEC_PKT          0x0E */
#define H5_LINK_CTL_PKT         0x0F

#define RTL_TXWINSIZE   4


/* Private typedef -----------------------------------------------------------*/
typedef struct _h5sk_buff_head
{
    /* These two members must be first.
     * We will fetch the first sk_buff via the address of sk_buff_head.
     * */
    struct h5buf  *next;
    struct h5buf  *prev;
    uint8_t            qlen;
} h5sk_buff_head;

typedef struct T_HCI_H5_PROTOCOL_CTRL
{
    uint32_t                    retry_count;
    void                        *tx_timer_handle;

    h5sk_buff_head              tx_unack;      /* TX Unack'ed packets queue */
    h5sk_buff_head              tx_rel;        /* TX Reliable packets queue */
    h5sk_buff_head              tx_unrel;      /* TX Unreliable packets queue */
    h5sk_buff_head              rx_unnotice;   /* RX packets need pass to upperstack queue */
    h5sk_buff_head              rx_uncfm;      /* RX packets without upperstack confirmed queue */

    uint8_t rxseq_txack;   /* expected rx seq number */
    uint8_t rxack;     /* last packet sent that the peer ack'ed */
    uint8_t use_crc;
    uint8_t txack_needs;   /* txack required */
    uint8_t msgq_txseq;    /* next pkt seq */

} HCI_H5_PROTOCOL_CTRL;

/* #######################################################################*/
/* #######################################################################*/
/* #######################################################################*/
//typedef struct t_rtlbt
//{
//    uint8_t proto;
//    void *user_data;
//    int state;
//} T_RTLBT;

struct h5buf
{
    //
    /* These two members must be first. */
    struct h5buf      *next;   //4*
    struct h5buf      *prev;   //4*
    uint8_t    *data;

    uint8_t    pkt_type;       //1
#define H5SK_BUFF_OUTSIDE       (1 << 0)
#define H5SK_CRC_CAL            (1 << 1)
    uint16_t   flags;
    uint16_t   crc;
    uint32_t   buff_sz;        /* initial buffer size */
    uint32_t   data_cnt;       /* actual data length */

    /* store pointer and len for upstream */
    uint32_t   us_len;
    uint8_t    *us_ptr;
};

/* Private variables ---------------------------------------------------------*/
HCI_PROTOCOL                hci_protocol  = {0};
HCI_H5_PROTOCOL_CTRL        hci_h5_protocol_ctrl = {0};
//static uint8_t              h5sk_prep_buf[H5_PREP_BUFLEN];
static uint8_t                     *h5sk_prep_buf;
static uint8_t              h5sk_prep_used = 0;
static uint16_t             h5_parse_remainder;
static uint8_t              h5_rx_state;

const uint8_t h5_sync[] = { 0x01, 0x7E };
const uint8_t h5_sync_rsp[] = { 0x02, 0x7D };
const uint8_t h5_conf[] = { 0x03, 0xFC, 0x14 };
const uint8_t h5_conf_rsp[] = { 0x04, 0x7B };

static const uint8_t byte_rev_table[256] =
{
    0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
    0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
    0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
    0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
    0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
    0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
    0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
    0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
    0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
    0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
    0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
    0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
    0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
    0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
    0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
    0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
    0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
    0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
    0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
    0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
    0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
    0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
    0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
    0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
    0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
    0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
    0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
    0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
    0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
    0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
    0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
    0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
};

static const uint16_t crc_table[] =
{
    0x0000, 0x1081, 0x2102, 0x3183,
    0x4204, 0x5285, 0x6306, 0x7387,
    0x8408, 0x9489, 0xa50a, 0xb58b,
    0xc60c, 0xd68d, 0xe70e, 0xf78f
};

/* Private function prototypes -----------------------------------------------*/
bool hci_h5_open(void);
bool hci_h5_close(void);
bool hci_h5_pack(uint8_t *p_data_buf, uint16_t data_length);
bool hci_h5_unpack(uint8_t *p_data_buf, uint16_t data_length);
bool hci_h5_get_package_tx(uint8_t **pp_data_buf, uint16_t *p_data_length);
bool hci_h5_get_package_rx(uint8_t **pp_data_buf, uint16_t *p_data_length);
bool hci_h5_tx_package_prune(uint8_t *p_data_buf, uint16_t data_length);
bool hci_h5_rx_package_prune(uint8_t *p_data_buf, uint16_t data_length);
bool hci_h5_tx_finish_callback(void);
bool hci_h5_rx_finish_callback(void);
bool hci_h5_connect_response(void *);

extern void hci_error_indicate(int level);

void hci_h5_sync_timer_event(void *xtimer);
void hci_h5_link_control(const void *data, uint16_t len);

static int hci_h5_allocator_init(void);

struct h5buf *h5buf_alloc(unsigned int len, int reserved);
void h5buf_free(struct h5buf *h5b);
uint8_t *h5buf_put(struct h5buf *h5b, uint32_t len);
struct h5buf *h5buf_init(void *p, uint32_t count);

static inline void h5buf_queue_init(h5sk_buff_head *list);
static inline void h5buf_queue_purge(h5sk_buff_head *list);
void h5buf_queue_tail(h5sk_buff_head *list, struct h5buf *newsk);
void h5buf_queue_head(h5sk_buff_head *list, struct h5buf *newsk);
struct h5buf *h5buf_dequeue(h5sk_buff_head *list);
bool h5_enqueue(struct h5buf *h5b);

static inline void __h5buf_unlink(struct h5buf *h5b, h5sk_buff_head *list);

void h5_complete_rx_pkt(struct h5buf *h5);
void h5_remove_acked_pkt(void);
/* #######################################################################*/
/* #######################################################################*/
/* #######################################################################*/

/* Private code --------------------------------------------------------------*/

bool hci_protocol_init(HCI_PROTOCOL **protocol_handle)
{
    hci_protocol.open = hci_h5_open;
    hci_protocol.close = hci_h5_close;
    hci_protocol.pack = hci_h5_pack;
    hci_protocol.unpack = hci_h5_unpack;
    hci_protocol.get_package_tx = hci_h5_get_package_tx;
    hci_protocol.get_package_rx = hci_h5_get_package_rx;
    hci_protocol.tx_package_prune = NULL; //h5 don't need this
    hci_protocol.rx_package_prune = hci_h5_rx_package_prune;
    hci_protocol.connect_response = hci_h5_connect_response;
    hci_protocol.tx_finish_callback = hci_h5_tx_finish_callback;
    hci_protocol.rx_finish_callback = hci_h5_rx_finish_callback;
    *protocol_handle = &hci_protocol;
    hci_h5_allocator_init();
    h5sk_prep_buf = tp_osif_malloc(H5_PREP_BUFLEN);
    return true;
}

bool hci_protocol_deinit(void)
{
    tp_osif_free(h5sk_prep_buf);
    memset(&hci_protocol, 0, sizeof(hci_protocol));
    return true;
}

/* #######################################################################*/
/* #######################################################################*/
/* #######################################################################*/

bool hci_h5_open(void)
{
    bool ret;
    HCI_H5_INFO("open h5.\n");
    hci_h5_protocol_ctrl.retry_count = HCI_PROTOCOL_CTRL_RETRY_MAX;
    h5_parse_remainder = 0;
    h5_rx_state = 0;

    h5buf_queue_init(&hci_h5_protocol_ctrl.tx_unack);
    h5buf_queue_init(&hci_h5_protocol_ctrl.tx_rel);
    h5buf_queue_init(&hci_h5_protocol_ctrl.tx_unrel);
    h5buf_queue_init(&hci_h5_protocol_ctrl.rx_unnotice);
    h5buf_queue_init(&hci_h5_protocol_ctrl.rx_uncfm);

    ret = tp_osif_timer_create(&hci_h5_protocol_ctrl.tx_timer_handle, "sync_timer", 0,
                               100, 1, hci_h5_sync_timer_event);
    if (!ret)
    {
        HCI_H5_ERR("create sync timer error\n");
        return -1;
    }

    /* send sync packet */
    tp_osif_timer_start(&hci_h5_protocol_ctrl.tx_timer_handle);
    return 0;
}

bool hci_h5_close(void)
{
    HCI_H5_INFO("close h5.\n");

    if (hci_h5_protocol_ctrl.tx_timer_handle != NULL)
    {
        tp_osif_timer_stop(&hci_h5_protocol_ctrl.tx_timer_handle);
        tp_osif_timer_delete(&hci_h5_protocol_ctrl.tx_timer_handle);
        hci_h5_protocol_ctrl.tx_timer_handle = NULL;
    }

    h5buf_queue_purge(&hci_h5_protocol_ctrl.tx_unack);
    h5buf_queue_purge(&hci_h5_protocol_ctrl.tx_rel);
    h5buf_queue_purge(&hci_h5_protocol_ctrl.tx_unrel);
    h5buf_queue_purge(&hci_h5_protocol_ctrl.rx_unnotice);
    h5buf_queue_purge(&hci_h5_protocol_ctrl.rx_uncfm);

    memset(&hci_h5_protocol_ctrl, 0, sizeof(hci_h5_protocol_ctrl));
    return 0;
}

static inline uint8_t bit_rev8(uint8_t byte)
{
    return byte_rev_table[byte];
}

static inline uint16_t bit_rev16(uint16_t x)
{
    return (bit_rev8(x & 0xff) << 8) | bit_rev8(x >> 8);
}

/* add 'd' into crc scope, caculate the new crc value
 *
 * @crc:    crc data
 * @d:      one byte data */
static inline void h5_crc_update(uint16_t *crc, uint8_t d)
{
    uint16_t reg = *crc;

    reg = (reg >> 4) ^ crc_table[(reg ^ d) & 0x000f];
    reg = (reg >> 4) ^ crc_table[(reg ^ (d >> 4)) & 0x000f];

    *crc = reg;
}

static inline uint16_t h5_get_crc(struct h5buf *h5_item)
{
    uint16_t crc = 0;

    uint8_t *data = h5_item->data + h5_item->data_cnt - 2;
    crc = data[1] + (data[0] << 8);

    return crc;
}

/* decode one byte in h5 proto, as follows:
 * 0xdb, 0xdc -> 0xc0
 * 0xdb, 0xdd -> 0xdb
 * 0xdb, 0xde -> 0x11
 * 0xdb, 0xdf -> 0x13
 * others will not change
 *
 * @byte: pure data in the one byte
 * @return 0 for fail, 1 for normal, 2 for cut
*/
uint8_t h5_unslip_one_byte(struct h5buf *rx_h5b, unsigned char byte)
{
    static enum H5_RX_ESC_STATE rx_esc_state;
    uint8_t *put_pointer;
    if (H5_ESCSTATE_NOESC == rx_esc_state)
    {
        if (byte == 0xdb)
        {
            rx_esc_state = H5_ESCSTATE_ESC;
            return byte;
        }
    }
    else if (H5_ESCSTATE_ESC == rx_esc_state)
    {
        switch (byte)
        {
        case 0xdc:
            byte = 0xc0;
            break;
        case 0xdd:
            byte = 0xdb;
            break;

#ifdef CONFIG_OOF_FLOW_CONTROL
        case 0xde:
            byte = 0x11;
            break;
        case 0xdf:
            byte = 0x13;
            break;
#endif
        default:
            HCI_H5_ERR("invalid byte %02x after esc byte\n", byte);
            rx_esc_state = H5_ESCSTATE_NOESC;
            return false;
        }
    }
    put_pointer = h5buf_put(rx_h5b, 1);
    if (put_pointer == NULL)
    {
        rx_esc_state = H5_ESCSTATE_NOESC;
        return false;
    }
    *put_pointer = byte;
    /* Check CRC bit */
    if ((rx_h5b->data[0] & 0x40) != 0 && (rx_h5b->flags & H5SK_CRC_CAL) != H5SK_CRC_CAL)
    {
        h5_crc_update(&rx_h5b->crc, byte);
    }
    rx_esc_state = H5_ESCSTATE_NOESC;
    return true;
}

bool hci_h5_unpack(uint8_t *p_data_buf, uint16_t count)
{
    uint8_t checksum;
    static uint8_t header_data[4];
    static struct h5buf temp_buf;
    static struct h5buf *h5_item;

    while (count)
    {
        /* parse hdr or payload or dic */
        if (h5_parse_remainder)
        {
            if (*p_data_buf == 0xc0)
            {
                HCI_H5_ERR("short h5 packet.\n");
                if ((h5_item != NULL) && (h5_item != &temp_buf))
                {
                    h5buf_free(h5_item);
                }
                h5_item = NULL;
                h5_rx_state = H5_W4_PKT_START;
                h5_parse_remainder = 0;
            }
            else
            {
                switch (h5_unslip_one_byte(h5_item, *p_data_buf))
                {
                case 0:
                    HCI_H5_ERR("Failed hci slip layer decode.\n");
                    if (h5_item != &temp_buf)
                    {
                        h5buf_free(h5_item);
                    }
                    h5_item = NULL;
                    h5_rx_state = H5_W4_PKT_DELIMITER;
                    h5_parse_remainder = 0;
                    break;
                case 1:
                    h5_parse_remainder--;
                default:
                    break;
                }
            }

            p_data_buf++;
            count--;
            continue;
        }

        switch (h5_rx_state)
        {
        case H5_W4_PKT_DELIMITER:
            if (*p_data_buf == 0xc0)
            {
                h5_rx_state = H5_W4_PKT_START;
            }
            p_data_buf++;
            count--;
            break;

        case H5_W4_PKT_START:
            if (*p_data_buf != 0xc0)
            {
                h5_rx_state = H5_W4_HDR;
                h5_parse_remainder = 4;
                /* We do not allocate memory here because of unknown packet len.
                 * packet mem will be allocated when h5 header received. */

                memset(&temp_buf, 0, sizeof(temp_buf));
                memset(&header_data, 0, sizeof(header_data));
                temp_buf.data_cnt = 0;
                temp_buf.buff_sz = sizeof(header_data);
                temp_buf.data = header_data;
                h5_item = &temp_buf;
                H5_CRC_INIT(h5_item->crc);
            }
            else
            {
                /* If the current byte is still 0x0c, it means that the previous
                 * 0x0c is the last byte of the previous packet */
                p_data_buf++;
                count--;
            }
            break;
        case H5_W4_HDR:
            checksum = header_data[0] + header_data[1] + header_data[2];
            /* check header checksum. see Core Spec V4 "3-wire uart" page 67 */
            if ((0xff & (uint8_t)~checksum) != header_data[3])
            {
                HCI_H5_ERR("h5 hdr checksum error! head:%x,%x,%x,%x\n", header_data[0], header_data[1],
                           header_data[2], header_data[3]);
                /* Do not free mem because no mem allocated before h5 header
                 * received */
                /* h5buf_free(h5->rx_h5b); */
                header_data[0] = header_data[1] = header_data[2] = header_data[3] = 0;
                h5_rx_state = H5_W4_PKT_DELIMITER;
                h5_parse_remainder = 0;
                continue;
            }

            /* reliable pkt  & h5->hdr->SeqNumber != h5->rxseq_txack */
            if (H5_HDR_RELIABLE(header_data) &&
                H5_HDR_SEQ(header_data) != hci_h5_protocol_ctrl.rxseq_txack)
            {
                HCI_H5_ERR("Out-of-order packet arrived, got(%u)expected(%u),count(%d),\n",
                           H5_HDR_SEQ(header_data), hci_h5_protocol_ctrl.rxseq_txack, h5_parse_remainder);
                hci_error_indicate(0);
                hci_ps_util_hexdump('M', header_data, 4);

                hci_h5_protocol_ctrl.txack_needs = 1;

                /* Do not free mem because no mem allocated before h5 header
                 * received */
                /* h5buf_free(h5->rx_h5b); */

                h5_rx_state = H5_W4_PKT_DELIMITER;
                h5_parse_remainder = 0;

                /* we should send pure ack and re-transmit rel packet later */
                hci_if_send_message_to_ifctrl(HCI_IF_MSG_TX_UART_TRANSMIT);

                continue;
            }

            h5_rx_state = H5_W4_DATA;
            /* calculate the packet payload length and alloc mem */
            h5_parse_remainder = (header_data[1] >> 4) +
                                 (header_data[2] << 4);

            if ((header_data[1] & 0x0f) == HCI_ACL_PKT)
            {
                h5_item = h5buf_alloc(4 + h5_parse_remainder + 2, HCI_H5_RX_ACL_PKT_BUF_OFFSET + H5_RESERVEDALIGN);
            }
#if HCI_ISO_DATA_PACKET
            else if ((header_data[1] & 0x0f) == HCI_ISO_PKT)
            {
                h5_item = h5buf_alloc(4 + h5_parse_remainder + 2, H5_RESERVED1 + H5_RESERVEDALIGN);
            }
#endif
            else
            {
                //For 4-btyes aligment of the h5b->data which sent to upperstack, parameter <reserved> of h5buf_alloc must be 1 here
                h5_item = h5buf_alloc(4 + h5_parse_remainder + 2, 1);
            }

            if (h5_item)
            {
                memcpy(h5buf_put(h5_item, 4), header_data, 4);
                h5_item->crc = temp_buf.crc;
            }
            else
            {
                h5_rx_state = H5_W4_PKT_DELIMITER;
                h5_parse_remainder = 0;
                HCI_H5_ERR("alloc h5 sk buf error for data recv\n");
                return count;
            }
            continue;

        case H5_W4_DATA:

            /* Packet with crc */
            if (H5_HDR_CRC(h5_item->data))
            {
                h5_item->flags = h5_item->flags | H5SK_CRC_CAL;
                h5_rx_state = H5_W4_CRC;
                h5_parse_remainder = 2;
            }
            else
            {
                h5_complete_rx_pkt(h5_item);
                h5_rx_state = H5_W4_PKT_DELIMITER;
            }
            continue;

        case H5_W4_CRC:
            if (bit_rev16(h5_item->crc) != h5_get_crc(h5_item))
            {
                HCI_H5_ERR("checksum error, computed(%04x) received(%04x)\r\n",
                           bit_rev16(h5_item->crc),
                           h5_get_crc(h5_item));
                h5buf_free(h5_item);
                h5_item = NULL;
                h5_rx_state = H5_W4_PKT_DELIMITER;
                h5_parse_remainder = 0;
                continue;
            }
            //util_hexdump('r',h5->rx_h5b->data,(h5->rx_h5b->data_cnt >16)? 16:h5->rx_h5b->data_cnt);
            h5_item->data_cnt -= 2;
            /* received a complete packet. */
            h5_complete_rx_pkt(h5_item);
            h5_rx_state = H5_W4_PKT_DELIMITER;
            continue;

        default:
            break;
        } /* end of switch (h5->rx_state) */
    }

    return count;
}

/* We got a complete rx frame stored in h5->rx_h5b
 * 1. Check if it's a HCI frame. If it is, complete it with response or ack
 * 2. See the ack number, free acked frame in queue
 * 3. Reset h5->rx_state, set rx_h5b to null.
 *
 * @h5: h5 struct */
void h5_complete_rx_pkt(struct h5buf *rx_h5b)
{
    uint8_t *h5_hdr = NULL;
    HCI_H5_DBG("rx t:%d l:%d s:%d a:%d\r\n", H5_HDR_PKT_TYPE(rx_h5b->data), H5_HDR_LEN(rx_h5b->data),
               H5_HDR_SEQ(rx_h5b->data), H5_HDR_ACK(rx_h5b->data));
    h5_hdr = (uint8_t *)(rx_h5b->data);
    if (H5_HDR_RELIABLE(h5_hdr))
    {
        HCI_H5_DBG("rel pkg rxseq_txack %u\n", hci_h5_protocol_ctrl.rxseq_txack);
        hci_h5_protocol_ctrl.rxseq_txack = H5_HDR_SEQ(h5_hdr) + 1;
        hci_h5_protocol_ctrl.rxseq_txack %= 8;
        hci_h5_protocol_ctrl.txack_needs = 1;
        /* there is an ack needs to send. */
        hci_if_send_message_to_ifctrl(HCI_IF_MSG_TX_UART_TRANSMIT);
    }

    hci_h5_protocol_ctrl.rxack = H5_HDR_ACK(h5_hdr);
    //  HCI_H5_DBG("\r\n===debug_rx, %x==\r\nCURRENT:Received SEQUNUMBER:%x, ACK:%x, RELI:%x:PACKETTYPE:%x PAYLOAD_LENGTH:%x\r\n",h5_sta.txack_needs, H5_HDR_SEQ(h5_hdr), H5_HDR_ACK(h5_hdr), H5_HDR_RELIABLE(h5_hdr),H5_HDR_PKT_TYPE(h5_hdr),H5_HDR_LEN(h5_hdr));

    /* Unreliable packets have been freed when prepare packet.
     * For reliable packets, free them or notify upper layer in
     * h5_remove_acked_pkt() */
    h5_remove_acked_pkt();

    switch (H5_HDR_PKT_TYPE(h5_hdr))
    {
    case HCI_ACLDATA_PKT:
    case HCI_EVENT_PKT:
    case HCI_SCODATA_PKT:
    case HCI_COMMAND_PKT:
#if HCI_ISO_DATA_PACKET
    case HCI_ISO_PKT:
#endif
    case H5_LINK_CTL_PKT:
        rx_h5b->data_cnt -= H5_HDR_SIZE;
        rx_h5b->data += H5_HDR_SIZE;
        rx_h5b->pkt_type = H5_HDR_PKT_TYPE(h5_hdr);
        h5buf_queue_tail(&hci_h5_protocol_ctrl.rx_unnotice, rx_h5b);
        HCI_H5_DBG("rx package enqueue t:%d p:%x c:%d q:%d\n", rx_h5b->pkt_type, rx_h5b->data,
                   rx_h5b->data_cnt, hci_h5_protocol_ctrl.rx_unnotice.qlen);
        break;
    default:
        h5buf_free(rx_h5b);
        break;
    }
}

/* removed controller acked packet from host's unacked lists
 *
 * @h5: h5 struct
 */
void h5_remove_acked_pkt()
{
    struct h5buf *h5b, *tmp;
    int pkts_to_remove = 0;
    int seqno = 0;
    int i = 0;
    uint32_t flags;

    flags = tp_osif_lock();

    seqno = hci_h5_protocol_ctrl.msgq_txseq;
    HCI_H5_DBG("%s: seqno %u, rxack %u\n", __func__, seqno, hci_h5_protocol_ctrl.rxack);

    pkts_to_remove = hci_h5_protocol_ctrl.tx_unack.qlen;

    while (pkts_to_remove)
    {
        if (hci_h5_protocol_ctrl.rxack == seqno)
        {
            break;
        }

        pkts_to_remove--;

        /* Move backward */
        seqno = (seqno - 1) & 0x07;
    }

    if (hci_h5_protocol_ctrl.rxack != seqno)
    {
        HCI_H5_WARN("Peer acked invalid packet, rxack %u, seqno %u\n",
                    hci_h5_protocol_ctrl.rxack, seqno);
    }

    HCI_H5_DBG("Removing %u pkts out of %u, up to seqno %u\n",
               pkts_to_remove, hci_h5_protocol_ctrl.tx_unack.qlen,
               (seqno - 1) & 0x07);

    /* Remove ack'ed packet from h5->unack queue */
    for (h5b = hci_h5_protocol_ctrl.tx_unack.next, tmp = h5b->next;    \
         h5b != (struct h5buf *)(&hci_h5_protocol_ctrl.tx_unack);      \
         h5b = tmp, tmp = h5b->next)
    {
        if (i >= pkts_to_remove)
        {
            break;
        }
        i++;
        __h5buf_unlink(h5b, &hci_h5_protocol_ctrl.tx_unack);
        /* notify upper layer to free reliable packet buffer if the mem is
         * out of h5 */
        if (h5b->flags & H5SK_BUFF_OUTSIDE)
        {
            if (!hci_if_send_message_to_upperstack(HCI_IF_EVT_DATA_XMIT, true, h5b->us_ptr, h5b->us_len))
            {
                HCI_H5_ERR("failed to inform upperstack Tx ok\n");
            }
        }

        /* Even the packet from upper layer, the h5buf struct also needs
         * to free, because it is allocated in h5 when enqueue */
        h5buf_free(h5b);
    }

    if (hci_h5_protocol_ctrl.tx_unack.qlen == 0)
    {
        tp_osif_timer_stop(&hci_h5_protocol_ctrl.tx_timer_handle);
    }

    if (i != pkts_to_remove)
    {
        HCI_H5_INFO("Removed only (%u) out of (%u) pkts\n",
                    i, pkts_to_remove);
    }

    tp_osif_unlock(flags);
}

bool hci_h5_get_package_rx(uint8_t **pp_data_buf, uint16_t *p_data_length)
{
    struct h5buf *h5b = h5buf_dequeue(&hci_h5_protocol_ctrl.rx_unnotice);
    if (h5b != NULL)
    {
        if (h5b->pkt_type == HCI_ACL_PKT)
        {
            h5b->data = h5b->data - HCI_H5_RX_ACL_PKT_BUF_OFFSET;
            h5b->data[0] = h5b->pkt_type;
            h5b->data_cnt = h5b->data_cnt + HCI_H5_RX_ACL_PKT_BUF_OFFSET;
            //hci_tp_err("ACL_DATA:THOMAS2:p_buf %p, sizeof(%d), pbuf:%x, reserved:%d, allock %x\r\n", buf, sizeof(struct h5buf), pbuf, HCI_H5_RX_ACL_PKT_BUF_OFFSET, temp_buffer);
            // HCI_PRINT_TRACE5("AS2:p_buf %p, sizeof(%d), pbuf:%x, reserved:%d, allock %x\r\n", buf, sizeof(struct h5buf), pbuf, HCI_H5_RX_ACL_PKT_BUF_OFFSET, temp_buffer);
#ifdef      RTK_COEX
            rtl_coex_hci_process_acl(pbuf, len);
#endif

        }
#if HCI_ISO_DATA_PACKET
        else if (h5b->pkt_type == HCI_ISO_PKT)
        {
            h5b->data = h5b->data - H5_RESERVED1;
            h5b->data[0] = h5b->pkt_type;
            h5b->data_cnt = h5b->data_cnt + H5_RESERVED1;
        }
#endif
        else
        {
            h5b->data = h5b->data - 1;
            h5b->data[0] = h5b->pkt_type;
            h5b->data_cnt = h5b->data_cnt + 1;
        }
        *pp_data_buf = h5b->data;
        *p_data_length = h5b->data_cnt;
        h5buf_queue_tail(&hci_h5_protocol_ctrl.rx_uncfm, h5b);
        HCI_H5_DBG("rx package passed up t:%d p:%x c:%d q:%d\n", h5b->pkt_type, h5b->data, h5b->data_cnt,
                   hci_h5_protocol_ctrl.rx_unnotice.qlen);
        //hci_ps_util_hexdump('U', h5b->data, h5b->data_cnt);
        return true;
    }
    return false;
}

bool hci_h5_rx_package_prune(uint8_t *p_data_buf, uint16_t data_length)
{
    struct h5buf *item = hci_h5_protocol_ctrl.rx_uncfm.next;
    uint32_t lockflag = tp_osif_lock();
    while ((h5sk_buff_head *)item != &hci_h5_protocol_ctrl.rx_uncfm)
    {
        if (item->data == p_data_buf)
        {
            item->prev->next = item->next;
            item->next->prev = item->prev;
            tp_osif_unlock(lockflag);
            tp_osif_free(item);
            return true;
        }
        item = item->next;
    }
    tp_osif_unlock(lockflag);
    HCI_H5_ERR("upper stack confirm an unknow package %x \r\n", p_data_buf);
    return true;
}

bool hci_h5_rx_finish_callback(void)
{
    hci_if_send_message_to_ifctrl(HCI_IF_MSG_RX_INFORM_UPPERSTACK);
    return true;
}

/* #######################################################################*/
/* ###########                 tx                  ####################*/
/* #######################################################################*/

struct h5buf *h5sk_prep_buf_get(uint32_t len, uint32_t reserved)
{
    struct h5buf *h5b;
    if (!h5sk_prep_used)
    {
        h5sk_prep_used = 1;
        h5b = (struct h5buf *)h5sk_prep_buf;
        memset(h5b, 0, sizeof(struct h5buf));
        h5b->data = h5sk_prep_buf + sizeof(struct h5buf) + reserved;
        h5b->buff_sz = len;
        h5b->data_cnt = 0;
        h5b->flags = 0;
        return h5b;
    }
    else
    {
        HCI_H5_ERR("%s: prepared buf has been used\n", __func__);
        return NULL;
    }

}

/* Just add 0xc0 at the end of h5b,
 * add 0xc0 to the start if there is no data in h5b */
static __inline void h5_slip_msgdelim(struct h5buf *h5b)
{
    const char pkt_delim = 0xc0;
    memcpy(h5buf_put(h5b, 1), &pkt_delim, 1);
}

/* encode one byte in h5 proto, as follows:
 * 0xc0 -> 0xdb, 0xdc
 * 0xdb -> 0xdb, 0xdd
 * 0x11 -> 0xdb, 0xde
 * 0x13 -> 0xdb, 0xdf
 * others will not change
 *
 * @c pure data in the one byte */
static void h5_slip_one_byte(struct h5buf *h5b, uint8_t c)
{
    const char esc_c0[2] = { 0xdb, 0xdc };
    const char esc_db[2] = { 0xdb, 0xdd };
#ifdef CONFIG_OOF_FLOW_CONTROL
    const char esc_11[2] = { 0xdb, 0xde };
    const char esc_13[2] = { 0xdb, 0xdf };
#endif

    switch (c)
    {
    case 0xc0:
        memcpy(h5buf_put(h5b, 2), &esc_c0, 2);
        break;

    case 0xdb:
        memcpy(h5buf_put(h5b, 2), &esc_db, 2);
        break;

#ifdef CONFIG_OOF_FLOW_CONTROL
    case 0x11:
        memcpy(h5buf_put(h5b, 2), &esc_11, 2);
        break;

    case 0x13:
        memcpy(h5buf_put(h5b, 2), &esc_13, 2);
        break;
#endif

    default:
        memcpy(h5buf_put(h5b, 1), &c, 1);
        break;
    }
}

/* Prepare h5 packet, packet format as follow:
 *  |LSB 4 octets  | 0 ~4095 | 2-byte MSB           |
 *  |packet header | payload | data integrity check |
 *
 * pakcket header fromat is show below:
 *  | LSB
 *  | 3 bits | 3 bits | 1 bits | 1 bits | 4 bits | 12 bits | 8 bits MSB
 *  | seqn   | ackn   | dic    | rel    | pktype | payld l | hd cksum
 *
 * @h5:         h5 struct
 * @data:       pure data
 * @len:        the length of data
 * @pkt_type:   packet type
 *
 * return the buff after preparation in h5 proto */
struct h5buf *h5_prepare_pkt(uint8_t *data, int32_t len, int32_t pkt_type)
{
    struct h5buf *nh5b;
    uint8_t hdr[4];
    uint16_t H5_CRC_INIT(h5_txmsg_crc);
    int rel, i;

    switch (pkt_type)
    {
    /* Reliable */
    case HCI_ACLDATA_PKT:
    case HCI_COMMAND_PKT:
    case HCI_EVENT_PKT:
#if HCI_ISO_DATA_PACKET
    case HCI_ISO_PKT:
#endif
        rel = 1;
        break;

    /* Unreliable */
    case H5_ACK_PKT:
    /* case H5_VDRSPEC_PKT: */
    case H5_LINK_CTL_PKT:
    case HCI_SCODATA_PKT:
        rel = 0;
        break;

    default:
        HCI_H5_ERR("Unknown packet type\n");
        return NULL;
    }

    /* Max len of packet: (original len +4(h5 hdr) +2(crc))*2 + 2
     * Because bytes 0xc0 and 0xdb are escaped, worst case is
     * when the packet is all made of 0xc0 and 0xdb, + 2 for 0xc0
     * delimiters at start and end. */
    nh5b = h5sk_prep_buf_get((len + 6) * 2 + 2, 0);
    if (!nh5b)
    {
        return NULL;
    }

    /* Add start byte 0xc0 in SLIP packet */
    h5_slip_msgdelim(nh5b);

    /* Set ack number in SLIP header */
    hdr[0] = hci_h5_protocol_ctrl.rxseq_txack << 3;
    hci_h5_protocol_ctrl.txack_needs = 0;

//      HCI_H5_DBG("!!!!!!!!Sending packet with seqno %u and wait %u, rel:%x!!!!!!!!!\r\n",
//             h5->msgq_txseq, h5->rxseq_txack,rel);
    if (rel)
    {
        HCI_H5_DBG("%s: msgq_txseq %u, rxseq_txack %u\n", __func__,
                   hci_h5_protocol_ctrl.msgq_txseq, hci_h5_protocol_ctrl.rxseq_txack);
        /* Set reliable pkt bit and seq number */
        hdr[0] |= (0x80 + hci_h5_protocol_ctrl.msgq_txseq);
        /* HCI_H5_DBG("Sending packet with seqno(%u)", h5->msgq_txseq); */
        (hci_h5_protocol_ctrl.msgq_txseq)++;
        hci_h5_protocol_ctrl.msgq_txseq = (hci_h5_protocol_ctrl.msgq_txseq & 0x07);
    }

    /* Set DIC bit */
    if (hci_h5_protocol_ctrl.use_crc)
    {
        hdr[0] |= 0x40;
    }

    /* Set packet type and payload length */
    hdr[1] = ((len << 4) & 0xff) | pkt_type;
    hdr[2] = (uint8_t)(len >> 4);

    /* Set checksum */
    hdr[3] = ~(hdr[0] + hdr[1] + hdr[2]);

    /* Put h5 header */
    for (i = 0; i < 4; i++)
    {
        h5_slip_one_byte(nh5b, hdr[i]);

        if (hci_h5_protocol_ctrl.use_crc)
        {
            h5_crc_update(&h5_txmsg_crc, hdr[i]);
        }
    }

    /* Put payload */
    for (i = 0; i < len; i++)
    {
        h5_slip_one_byte(nh5b, data[i]);

        if (hci_h5_protocol_ctrl.use_crc)
        {
            h5_crc_update(&h5_txmsg_crc, data[i]);
        }
    }

    /* Put CRC */
    if (hci_h5_protocol_ctrl.use_crc)
    {
        h5_txmsg_crc = bit_rev16(h5_txmsg_crc);
        h5_slip_one_byte(nh5b, (uint8_t)((h5_txmsg_crc >> 8) & 0x00ff));
        h5_slip_one_byte(nh5b, (uint8_t)(h5_txmsg_crc & 0x00ff));
    }
    HCI_H5_DBG("tx t:%d l:%d s:%d a:%d\n", H5_HDR_PKT_TYPE((&nh5b->data[1])),
               H5_HDR_LEN((&nh5b->data[1])), H5_HDR_SEQ((&nh5b->data[1])), H5_HDR_ACK((&nh5b->data[1])));
    /* Add SLIP end byte 0xc0 */
    h5_slip_msgdelim(nh5b);

    return nh5b;
}

bool hci_h5_get_package_tx(uint8_t **p_buf, uint16_t *len)
{
    uint32_t flags;
    struct h5buf *h5b;

    /* First of all, check for unreliable messages in the queue,
     * since they have priority */
    h5b = h5buf_dequeue(&hci_h5_protocol_ctrl.tx_unrel);
    if (h5b)
    {
        struct h5buf *nh5b;

        HCI_H5_DBG("dequeue a h5b from tx_unrel, plen %d, blen %d\n", hci_h5_protocol_ctrl.tx_unrel.qlen,
                   h5b->data_cnt);
        nh5b = h5_prepare_pkt(h5b->data, h5b->data_cnt, h5b->pkt_type);
        if (nh5b)
        {
            /* upper layer is only allowed to send one kind of unreliable pkt
             * which is HCI_SCODATA_PKT. All other unreliable packets are
             * freed here. */
            if (h5b->pkt_type == HCI_SCODATA_PKT && (h5b->flags & H5SK_BUFF_OUTSIDE))
            {
                if (!hci_if_send_message_to_upperstack(HCI_IF_EVT_DATA_XMIT, true, h5b->us_ptr, h5b->us_len))
                {
                    HCI_H5_ERR("failed to inform upperstack Tx ok\n");
                }
            }

            /* Even the packet from upper layer, the h5buf struct also needs
             * to free */
            h5buf_free(h5b);
            *p_buf = nh5b->data;
            *len = nh5b->data_cnt;
            return true;
        }
        else
        {
            h5buf_queue_head(&hci_h5_protocol_ctrl.tx_unrel, h5b);
            HCI_H5_ERR("prepare packet failed when dequeue.\n");
        }
    }

    /* Now, try to send a reliable pkt. We can only send a
     * reliable packet if the number of packets sent but not yet ack'ed
     * is < than the winsize
     */

    flags = tp_osif_lock();

    if (hci_h5_protocol_ctrl.tx_unack.qlen < RTL_TXWINSIZE)
    {
        h5b = h5buf_dequeue(&hci_h5_protocol_ctrl.tx_rel);
        if (h5b)
        {
            struct h5buf *nh5b;

            HCI_H5_DBG("dequeue a h5b from tx_rel, plen %d, blen %d\n", hci_h5_protocol_ctrl.tx_rel.qlen,
                       h5b->data_cnt);
            nh5b = h5_prepare_pkt(h5b->data, h5b->data_cnt,
                                  h5b->pkt_type);
            if (nh5b)
            {
                h5buf_queue_tail(&hci_h5_protocol_ctrl.tx_unack, h5b);
                tp_osif_unlock(flags);
                //RTLBT_DBG("Restart tx timer\n");
                /* Modify the timer after unlock to avoid dead lock. */
                // tp_osif_timer_restart(&h5->timer_handle, h5->interval_ms);
                *p_buf = nh5b->data;
                *len = nh5b->data_cnt;
                tp_osif_timer_restart(&hci_h5_protocol_ctrl.tx_timer_handle, H5_Tx_Retransmit_Timeout);
                return true;
            }
            else
            {
                h5buf_queue_head(&hci_h5_protocol_ctrl.tx_rel, h5b);
                HCI_H5_ERR("can not dequeue pkt because alloc h5b failed\n");
            }
        }
    }
    else
    {
        HCI_H5_WARN("tx win is full, qlen %u\n", hci_h5_protocol_ctrl.tx_unack.qlen);
    }

    tp_osif_unlock(flags);

    /* We could not send a reliable packet, either because there are
     * none or because there are too many unack'ed pkts. Did we receive
     * any packets we have not acknowledged yet ? */

    if (hci_h5_protocol_ctrl.txack_needs)
    {
        /* if so, craft an empty ACK pkt and send it on unreliable channel 0 */
        struct h5buf *nh5b = h5_prepare_pkt(NULL, 0, H5_ACK_PKT);
        *p_buf = nh5b->data;
        *len = nh5b->data_cnt;
        return true;
    }

    /* We have nothing to send */
    return false;
}

bool hci_h5_tx_finish_callback(void)
{
    h5sk_prep_used = 0;
    hci_if_send_message_to_ifctrl(HCI_IF_MSG_TX_UART_TRANSMIT);
    return true;
}

bool hci_h5_pack(uint8_t *p_data_buf, uint16_t data_length)
{
    struct h5buf *h5b;
    uint8_t *head;
    uint32_t length;

    HCI_H5_DBG("%s: %08x %d\n", __func__, p_data_buf, data_length);
    if (p_data_buf[0] == HCI_ACL_PKT)
    {
        //  hci_tp_info("\r\n====h5_tx_buffer=%p==%x, old_buf %p=\r\n", h5b, len, p_buf);
        head = p_data_buf + HCI_H5_TX_ACL_PKT_BUF_OFFSET;
        length  = data_length - HCI_H5_TX_ACL_PKT_BUF_OFFSET;
#ifdef  RTK_COEX
        rtl_coex_hci_process_txacl(head + 1, length - 1);
#endif
    }
#if HCI_ISO_DATA_PACKET
    else if (p_data_buf[0] == HCI_ISO_PKT)
    {
        head = p_data_buf + HCI_STACK_TX_ISO_RSVD_SIZE;
        length = data_length - HCI_STACK_TX_ISO_RSVD_SIZE;
    }
#endif
    else
    {
        head = p_data_buf;
        length  = data_length;
    }

    /* Skip the packet type of HCI packet from upper stack */
    h5b = h5buf_init(head + 1, length - 1);

    if (!h5b)
    {
        HCI_H5_ERR("%s: h5 h5b init err\n", __func__);
        return false;
    }

    /* used when indicating data transmitted */
    h5b->us_ptr = p_data_buf;
    h5b->us_len = data_length;
    h5b->pkt_type = p_data_buf[0];

    h5_enqueue(h5b);

    return true;
}

void tx_time_out_event(void *xtimer)
{
    uint32_t flags;
    struct h5buf *h5b, *h5bt;
    HCI_H5_WARN(" Tx Timedout, re-transmission, unacked q:%d.\n", hci_h5_protocol_ctrl.tx_unack.qlen);
    hci_error_indicate(0);
    flags = tp_osif_lock();
    h5bt = hci_h5_protocol_ctrl.tx_unack.prev;
    while ((h5sk_buff_head *)h5bt != &hci_h5_protocol_ctrl.tx_unack)
    {
        h5b = h5bt;
        h5bt = h5bt->prev;
        hci_h5_protocol_ctrl.msgq_txseq = (hci_h5_protocol_ctrl.msgq_txseq - 1) & 0x07;
        __h5buf_unlink(h5b, &hci_h5_protocol_ctrl.tx_unack);
        h5buf_queue_head(&hci_h5_protocol_ctrl.tx_rel, h5b);
    }

    tp_osif_unlock(flags);
    hci_if_send_message_to_ifctrl(HCI_IF_MSG_TX_UART_TRANSMIT);
    return;
}

/* #######################################################################*/
/* ###########       buffer & pool  control           ####################*/
/* #######################################################################*/
//#define OS_H5_POOL
static int hci_h5_allocator_init(void)
{

#ifdef OS_H5_POOL
    bool ret;
    int size, count;
    size = MAX_BUF1_LEN;
    count = 20;

    ret = os_pool_create(&h5_pool, RAM_TYPE_BUFFER_ON, size, count);
    if (!ret)
    {
        HCI_H5_ERR("%s: os pool create error\n", __func__);
        return -1;
    }

    size = MAX_BUF2_LEN;
    count = 16;

    ret = os_pool_extend(h5_pool, size, count);
    if (!ret)
    {
        HCI_H5_ERR("%s: os pool extend error for 256 sz buf\n", __func__);
        goto err1;
    }

    size = BUFF_STRUCT_LEN;
    count = 16;
    ret = os_pool_extend(h5_pool, size, count);
    if (!ret)
    {
        HCI_H5_ERR("%s: os pool extend error for buffstruct\n", __func__);
        goto err2;
    }
    return 0;
err2:
err1:
    return -1;
#endif
    return 0;

}

void *h5_mem_alloc(int len)
{
#ifdef OS_H5_POOL
    return (void *)os_buffer_get(h5_pool, len);
#else
    //hci_tp_err("======= %x", len);
    return tp_osif_malloc(len);
#endif
}

void h5_mem_free(void *p)
{
#ifdef OS_H5_POOL
    os_buffer_put(p);
#else
    tp_osif_free(p);
#endif

}

/* return buf including (h5buf + reserved + len) */
struct h5buf *h5buf_alloc(unsigned int len, int reserved)
{
    struct h5buf *h5b = NULL;
    uint8_t *p = NULL;
    int i;

    i = sizeof(struct h5buf) + reserved + len;
    p = h5_mem_alloc(i);
    //hci_tp_info("a:%x p:%d h:%d \n\r",p,i, osif_mem_peek(0));
    if (p)
    {
        h5b = (struct h5buf *)p;
        memset(h5b, 0, sizeof(struct h5buf));
        h5b->data = p + sizeof(struct h5buf) + reserved;
        h5b->buff_sz = len;
        h5b->data_cnt = 0;
        h5b->flags = 0;
    }
    else
    {
        HCI_H5_ERR("Failed to allocate h5buf\n");
        return NULL;
    }

    return h5b;
}

void h5buf_free(struct h5buf *h5b)
{
    if (!h5b)
    {
        return;
    }
    //hci_tp_info("f:%x h:%d \n\r", h5b, osif_mem_peek(0));
    h5_mem_free((uint8_t *)h5b);
    return;
}

/* increase the date length in sk_buffer by len,
 * return the pointer to the first byte of new extra data. */
uint8_t *h5buf_put(struct h5buf *h5b, uint32_t len)
{
    uint32_t i = h5b->data_cnt;

    if (h5b->data_cnt + len > h5b->buff_sz)
    {
        HCI_H5_ERR("Buffer too small\n");
        return NULL;
    }

    h5b->data_cnt += len;

    return (h5b->data + i);
}

struct h5buf *h5buf_init(void *p, uint32_t count)
{
    struct h5buf *h5b;

    h5b = h5_mem_alloc(sizeof(struct h5buf));
    if (!h5b)
    {
        HCI_H5_ERR("Failed to allocate h5buf for com write\n");
        return NULL;
    }
    h5b->data       = p;
    h5b->buff_sz    = count;
    h5b->data_cnt   = count;
    h5b->flags      = H5SK_BUFF_OUTSIDE;
    h5b->next       = NULL;
    h5b->prev       = NULL;
    return h5b;
}

/* #######################################################################*/
/* ###########              queue  control           ####################*/
/* #######################################################################*/
static inline void __h5buf_unlink(struct h5buf *h5b, h5sk_buff_head *list)
{
    list->qlen--;
    h5b->next->prev = h5b->prev;
    h5b->prev->next = h5b->next;
}

struct h5buf *h5buf_dequeue(h5sk_buff_head *list)
{
    struct h5buf *item = list->next;
    if ((h5sk_buff_head *)item != list)
    {
        uint32_t lockflag = tp_osif_lock();
        list->next = item->next;
        list->next->prev = item->prev;
        list->qlen--;
        tp_osif_unlock(lockflag);
        return item;
    }
    return NULL;
}

static inline void h5buf_queue_init(h5sk_buff_head *list)
{
    list->prev = list->next = (struct h5buf *)list;
    list->qlen = 0;
}

static inline void h5buf_queue_purge(h5sk_buff_head *list)
{
    struct h5buf *h5b;
    while ((h5b = h5buf_dequeue(list)) != NULL)
    {
        h5buf_free(h5b);
    }
}

void h5buf_queue_tail(h5sk_buff_head *list, struct h5buf *newsk)
{
    uint32_t flags;
    flags = tp_osif_lock();
    newsk->next = (struct h5buf *)list;
    newsk->prev = list->prev;
    list->prev->next = newsk;
    list->prev = newsk;
    list->qlen++;
    tp_osif_unlock(flags);
}

void h5buf_queue_head(h5sk_buff_head *list, struct h5buf *newsk)
{
    uint32_t flags;

    flags = tp_osif_lock();
    newsk->next = list->next;
    newsk->prev = (struct h5buf *)list;
    list->next->prev = newsk;
    list->next = newsk;
    list->qlen++;
    tp_osif_unlock(flags);
    HCI_H5_DBG("h5 enqueue head, qlen %d, blen%d.\n", list->qlen, newsk->data_cnt);
}

bool h5_enqueue(struct h5buf *h5b)
{
    if (h5b->data_cnt > 0xFFF)
    {
        HCI_H5_ERR("packet too long\n");
        h5buf_free(h5b);
        return false;
    }
    switch (h5b->pkt_type)
    {
    case HCI_ACLDATA_PKT:
    case HCI_COMMAND_PKT:
#if HCI_ISO_DATA_PACKET
    case HCI_ISO_PKT:
#endif
        h5buf_queue_tail(&hci_h5_protocol_ctrl.tx_rel, h5b);
        HCI_H5_DBG("h5 enqueue tx_rel, qlen %d, blen%d.\n", hci_h5_protocol_ctrl.tx_rel.qlen,
                   h5b->data_cnt);
        break;
    case HCI_SCODATA_PKT:
    case H5_ACK_PKT:
    case H5_LINK_CTL_PKT:
        h5buf_queue_tail(&hci_h5_protocol_ctrl.tx_unrel, h5b);
        HCI_H5_DBG("h5 enqueue tx_unrel, qlen %d, blen%d.\n", hci_h5_protocol_ctrl.tx_unrel.qlen,
                   h5b->data_cnt);
        break;
    default:
        HCI_H5_ERR("Unknown packet type %u\n", h5b->pkt_type);
        h5buf_free(h5b);
        return false;
    }
    hci_if_send_message_to_ifctrl(HCI_IF_MSG_TX_UART_TRANSMIT);
    return true;
}

/* #######################################################################*/
/* ###########           initialize control           ####################*/
/* #######################################################################*/

void hci_h5_link_control(const void *data, uint16_t len)
{
    struct h5buf *nh5b;
    nh5b = h5buf_alloc(len, 0);
    if (!nh5b)
    {
        return;
    }
    nh5b->pkt_type = H5_LINK_CTL_PKT;
    memcpy(h5buf_put(nh5b, len), data, len);
    h5_enqueue(nh5b);
}

void hci_h5_sync_timer_event(void *xtimer)
{
    if (hci_h5_protocol_ctrl.retry_count > 0)
    {
        hci_h5_link_control(h5_sync, sizeof(h5_sync));
        hci_tp_warn("Sync Timedout, re-transmission., %d\n", hci_h5_protocol_ctrl.retry_count);
        hci_error_indicate(0);
        hci_h5_protocol_ctrl.retry_count--;
    }
    else
    {
        tp_osif_timer_stop(&hci_h5_protocol_ctrl.tx_timer_handle);
        hci_tp_err("Sync retry count down, reset BT and retry more.\n");
        rtkbt_reset();
        hci_h5_protocol_ctrl.retry_count = 50;
        tp_osif_timer_start(&hci_h5_protocol_ctrl.tx_timer_handle);
        hci_tp_warn("Reset BT and start retry more.\n");
        hci_h5_link_control(h5_sync, sizeof(h5_sync));
    }
    return;
}

bool hci_h5_connect_response(void *para)
{
    HCI_H5_DBG("connect_response receive packets state %x \r\n", *(uint8_t *)para);
    struct h5buf *h5b = h5buf_dequeue(&hci_h5_protocol_ctrl.rx_unnotice);
    while (h5b != NULL)
    {
        if (*(uint8_t *)para == HCI_IF_STATE_SYNC)
        {
            if (!memcmp(h5b->data, h5_sync, sizeof(h5_sync)))
            {
                /* If received SYNC packet, send SYNC rsp packet.
                 * But remain the old status H5_SYNC */
                HCI_H5_INFO("receive sync pkt in h5 sync state, responding\n");
                hci_h5_link_control(h5_sync_rsp, sizeof(h5_sync_rsp));
            }
            else if (!memcmp(h5b->data, h5_sync_rsp, sizeof(h5_sync_rsp)))
            {
                tp_osif_timer_stop(&hci_h5_protocol_ctrl.tx_timer_handle);
                tp_osif_timer_delete(&hci_h5_protocol_ctrl.tx_timer_handle);
                if (!tp_osif_timer_create(&hci_h5_protocol_ctrl.tx_timer_handle, "h5_tx_timer", 0,
                                          H5_Tx_Retransmit_Timeout, 0, tx_time_out_event))
                {
                    HCI_H5_ERR("create h5 tx timer error\n");
                    return -1;
                }
                HCI_H5_INFO("receive sync rsp pkt, move to h5 config state\n");
                *(uint8_t *)para = HCI_IF_STATE_CONFIG;
                hci_h5_link_control(h5_conf, sizeof(h5_conf));
            }
        }
        else if (*(uint8_t *)para  == HCI_IF_STATE_CONFIG)
        {
            if (!memcmp(h5b->data, h5_sync, 2))
            {
                /* Reply with sync rsp if there is sync packet
                 * received during H5_CONFIG */
                hci_h5_link_control(h5_sync_rsp, sizeof(h5_sync_rsp));
                HCI_H5_INFO("receive sync pkt in h5 config state, responding\n");
            }
            else if (!memcmp(h5b->data, h5_conf, 2))
            {
                /* Receive CONFIG pkt from controller after sending
                 * CONFIG pkt, reply with config rsp but no config
                 * field */
                HCI_H5_INFO("receive confg pkt in h5 config state, responding\n");
                hci_h5_link_control(h5_conf_rsp, sizeof(h5_conf_rsp));
                hci_h5_link_control(h5_conf, sizeof(h5_conf));
            }
            else if (!memcmp(h5b->data, h5_conf_rsp, 0x2))
            {
                /* Move to active state */
                HCI_H5_INFO("receive confg rsp pkt, move to h5 patching\n");
                if (h5b->data_cnt > 2)
                {
                    HCI_H5_INFO("Config rsp field %x\n", h5b->data[2]);
                    hci_h5_protocol_ctrl.use_crc = ((h5b->data[2] & 0x10) == 0x10);
                }
                *(uint8_t *)para = HCI_IF_STATE_PATCH;
                hci_ps_start_next_process();
            }
            else
            {
                HCI_H5_INFO("receive link packet (len %u, %02x %02x) in config state\n",
                            h5b->data_cnt, h5b->data[0], h5b->data[1]);
            }
        }
        else if (*(uint8_t *)para  == HCI_IF_STATE_PATCH)
        {
#if 0
            util_hexdump('R', h5b->data, h5b->data_cnt);
#endif
            switch (h5b->pkt_type)
            {
            case HCI_EVENT_PKT:
                //only deal with event pkt
                hci_ps_check_process(h5b->data, h5b->data_cnt);
                break;
            case HCI_ACLDATA_PKT:
            case HCI_SCODATA_PKT:
#if HCI_ISO_DATA_PACKET
            case HCI_ISO_PKT:
#endif
            default:
                HCI_H5_ERR("unexpected hci packet [%02x] received\n",
                           h5b->pkt_type);
                break;
            }
        }
        else /* In active state */
        {
            HCI_H5_WARN("receive packets in %d state\n", *(uint8_t *)para);
        }
        h5buf_free(h5b);
        h5b = h5buf_dequeue(&hci_h5_protocol_ctrl.rx_unnotice);
    }
    return true;
}

bool hci_proto_send(uint8_t *p_data_buf, uint16_t data_length, P_HCI_TP_TX_CB callback)
{
    struct h5buf *h5b;
    h5b = h5buf_alloc(data_length, 0);
    if (!h5b)
    {
        hci_tp_err("h5b alloc failed\n");
        return false;
    }
    h5b->pkt_type = p_data_buf[0];//first is type skip
    p_data_buf++;
    data_length--;
    memcpy(h5buf_put(h5b, data_length), p_data_buf, data_length);
    h5_enqueue(h5b);
    if (callback)
    {
        callback();
    }
    return true;
}

bool hci_ps_complete(bool flag)
{
    if (flag)
    {
        HCI_H5_INFO("complete downloading, move to h5 active state\n");
        hci_if_send_message_to_ifctrl(HCI_IF_MSG_READY);
    }
    else
    {
        hci_if_send_message_to_ifctrl(HCI_IF_MSG_FAIL);
    }
    return true;
}

void baudrate_send_the_ack(void)
{
    //ack the baudrate packet
    hci_h5_protocol_ctrl.txack_needs = 1;
    /* we should send pure ack and re-transmit rel packet later */
    hci_if_send_message_to_ifctrl(HCI_IF_MSG_TX_UART_TRANSMIT);
}

#endif //USE_HCI_H5

