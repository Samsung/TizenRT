/**
 * Copyright (c) 2017, Realsil Semiconductor Corporation. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>

#include "os_mem.h"
#include "hci_tp.h"
#include "hci_process.h"
#include "bt_types.h"

#include "hci_uart.h"
#include "bt_board.h"
#include "hci_board.h"

#include "trace_app.h"


typedef struct
{
    P_HCI_TP_OPEN_CB    open_cb;
    uint8_t            *tx_buf;
} T_HCI_UART;

T_HCI_UART hci_rtk;
T_HCI_UART *p_hci_rtk = &hci_rtk;

uint8_t flag_for_hci_trx = 0;
uint8_t g_hci_step = 0;
extern HCI_PROCESS_TABLE hci_process_table[];
extern uint8_t hci_total_step;
extern bool hci_board_complete(void);
extern void bte_init_buff_free(void);
/* ================================internal=========================== */
bool hci_rtk_tx_cb(void)
{
    return true;
}
/* =================================external========================== */
bool hci_adapter_send(uint8_t *p_buf, uint16_t len)
{
    p_hci_rtk->tx_buf  = p_buf;
    //dbg("-- [TEST]  %p len %d--\n", __builtin_return_address(0), len);
    return hci_tp_send(p_buf, len, hci_rtk_tx_cb);
}

/* ====================hci_tp.h================ */
void hci_tp_open(P_HCI_TP_OPEN_CB open_cb, P_HCI_TP_RX_IND rx_ind)
{
    uint8_t ret = 0xff;
    p_hci_rtk->open_cb = open_cb;
    if(hci_board_init() == false)
    {
        hci_board_debug("%s:hci_board_init fail\n", __FUNCTION__);
        p_hci_rtk->open_cb(false);
        return;
    }
    bt_reset();

    if(!hci_uart_init(rx_ind))
    {
        hci_board_debug("%s:uart_init fail\n", __FUNCTION__);
        p_hci_rtk->open_cb(false);
        return;
    }

    g_hci_step = 0;

    ret = hci_process_table[g_hci_step].start_pro();

    if(ret == HCI_TP_NOT_SEND)
    {
        g_hci_step++;
        hci_process_table[g_hci_step].start_pro();
    }
    else
    {
        /* NORMAL START */
    }
}

void hci_tp_close(void)
{
    HCI_PRINT_INFO0("hci_tp_close");
    bt_power_off();
    hci_uart_deinit();
    return;
}

void hci_tp_del(uint8_t param)
{
    if (param == 0) {
        flag_for_hci_trx = 1;
    }
    else if (param == 1) {
        HCI_PRINT_INFO0("hci_tp_del");
        bt_power_off();
        hci_uart_deinit();
        flag_for_hci_trx = 0;
    }
}

bool hci_tp_send(uint8_t *p_buf, uint16_t len, P_HCI_TP_TX_CB tx_cb)
{
    return hci_uart_tx(p_buf, len, tx_cb);
}

uint16_t hci_tp_recv(uint8_t *p_buf, uint16_t size)
{
    return hci_uart_recv(p_buf, size);
}

void hci_tp_config(uint8_t *p_buf, uint16_t len)
{
    (void)len;
    uint8_t  pkt_type;
    uint8_t  evt_code;
    uint16_t opcode;
    uint8_t  status;
    uint8_t  p_buf_len;

    LE_STREAM_TO_UINT8(pkt_type, p_buf);

    if (pkt_type != HCI_EVT_PKT)
    {
        /* Skip non-hci event pkt. */
        hci_board_debug("ERROR:%s:packet type is %x\n",__FUNCTION__, pkt_type);
        return;
    }

    LE_STREAM_TO_UINT8(evt_code, p_buf);
    LE_STREAM_TO_UINT8(p_buf_len, p_buf);
   
    if (evt_code == HCI_COMMAND_COMPLETE)
    {
        STREAM_SKIP_LEN(p_buf, 1);  /* Skip num of hci cmd pkts */
        LE_STREAM_TO_UINT16(opcode, p_buf);
        if (opcode == hci_process_table[g_hci_step].opcode)/* check the opcode */
        {
            LE_STREAM_TO_UINT8(status, p_buf);
            if(status == 0)
            {
                if(hci_process_table[g_hci_step].check_func != NULL)
                {
                    uint8_t ret;
                    ret = hci_process_table[g_hci_step].check_func(p_buf_len, p_buf);
                    switch(ret)
                    {
                        case HCI_TP_CHECK_OK:
                            goto hci_tp_config_ok;
                            break;
                        case HCI_TP_CHECK_AGAIN:
                            goto hci_tp_config_again;
                            break;
                        case HCI_TP_CONFIG_FAIL:
                            goto hci_tp_config_fail;
                            break;
                        case HCI_TP_CONFIG_END:
                            goto hci_tp_config_end;
                            break;
                        case HCI_TP_CHECK_ERROR:
                            p_hci_rtk->open_cb(false);
                            bte_init_buff_free();
                            return;
                            break;
                        default:
                            hci_board_debug("%s:unexpect status is %x\n",__FUNCTION__,ret);
                            break;
                    }
                }
                else
                {
                    if(p_buf_len == 4)
                    {
                    }
                }
hci_tp_config_ok:
                /* next step */
                g_hci_step++;
                //dbg("-- [hci_tp_config_ok]  %p %d--\n", __builtin_return_address(0), g_hci_step);
                if(g_hci_step == hci_total_step)
                {
                    hci_board_debug("%s:BT INIT success %x\n",__FUNCTION__,g_hci_step);
                    bte_init_buff_free();
                    if(hci_board_complete() == true)
                    {
                         p_hci_rtk->open_cb(true);
                         //dbg("-- [hci_tp_config_complete]  %p %d--\n", __builtin_return_address(0), g_hci_step);
                    }
                    else
                    {
                         p_hci_rtk->open_cb(false);
                         return; 
                    }
                }
                else
                {
                    if(hci_process_table[g_hci_step].start_pro != NULL)
                    {
                        uint8_t ret;
                        ret = hci_process_table[g_hci_step].start_pro();
                            while(ret == HCI_TP_NOT_SEND)
                            {
                                g_hci_step++;
                                ret = hci_process_table[g_hci_step].start_pro();
                            }
                    }
                    else
                    {
                        hci_board_debug("ERROR:%s:start_pro is null %x\n",__FUNCTION__, g_hci_step);

                    }
                }
            }
            else
            {
                hci_board_debug("ERROR:%s:wrong status is %x, opcode is 0x%x\n",__FUNCTION__,
                        status,opcode);
            }

        }
        else
        {
            hci_board_debug("ERROR:%s:wrong type is %x\n",__FUNCTION__, opcode);
        }

    }
    else
    {
        hci_board_debug("ERROR:%s:unhandle evt is %x\n", __FUNCTION__ ,evt_code);
    }
    return;

hci_tp_config_fail:
    p_hci_rtk->open_cb(false);
    bte_init_buff_free();
    //dbg("-- [hci_tp_config_fail]  %p %d--\n", __builtin_return_address(0), g_hci_step);
    return;
    
hci_tp_config_end:
    p_hci_rtk->open_cb(true);
    bte_init_buff_free();
    //dbg("-- [hci_tp_config_end]  %p %d--\n", __builtin_return_address(0), g_hci_step);
    return;

hci_tp_config_again:
    hci_process_table[g_hci_step].start_pro();
    //dbg("-- [hci_tp_config_again]  %p %d--\n", __builtin_return_address(0), g_hci_step);
    return;
}
