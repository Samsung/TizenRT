/**
 ****************************************************************************************
 *
 * @file rwnx_tx_buffering.c
 *
 * @brief TX buffering module
 *
 * TX buffering module supports buffering data upon MAC layer when MAC is not
 * able to transmit successfully as some known reasons which the transmitting data
 * will be NOT delivered to receiver, such as the receiver is in PS mode, and channel 
 * was switched to un-operation channel...
 *
 * Copyright (C) BEKEN corperation 2021
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "bk_compiler.h"
#include "co_bit.h"
#include "rw_tx_buffering.h"
//#include "ps.h"
#include "rw_msdu.h"
#include "bk_ps.h"
#include <os/mem.h>
#include "fhost_msg.h"
#include "bk_rw.h"
#include "rwnx_defs.h"

#if CONFIG_TX_BUFING

/*
 * GLOBAL VARIABLE DEFINITION
 ****************************************************************************************
 */
static tx_bufing g_tx_bufing = {0};
/*
 * EXTERNAL FUNCTION DECLARE
 ****************************************************************************************
 */
extern void bmsg_tx_bufing_sender(uint32_t cid_and_flag);
/*
 *  INTERNAL FUNCTION DECLARE
 ****************************************************************************************
 */
static MSDU_NODE_T *rwm_pop_bufing_data(uint8_t client_idx,uint8_t flag);
/*
 * FUNCTION BODIES
 ****************************************************************************************
 */

//get role
static uint8_t rwm_get_bufing_role(uint32_t trigger_source)
{
    if(trigger_source == TX_BUFING_SRC_P2P_PS)
    {
        return VIF_STA;
    }
    else if(trigger_source == TX_BUFING_SRC_STA_PS)
    {
        return VIF_AP;
    }
    else if(trigger_source == TX_BUFING_SRC_TWT)
    {
        return VIF_STA;
    }
    else
    {
        return VIF_UNKNOWN;
    }

}

//get list
static LIST_HEADER_T *rwm_get_bufing_list(uint8_t client_idx,uint8_t flag)
{
    if(flag == VIF_STA)
    {
        return &g_tx_bufing.bufing_gc_sta.bufing_list;
    }
    else if(flag ==  VIF_AP)
    {
        return &g_tx_bufing.bufing_go_sap[client_idx].bufing_list;
    }
    else
    {
        return NULL;
    }
}

//get timer
static beken_timer_t *rwm_get_bufing_timer(uint8_t client_idx,uint8_t flag)
{
    if(flag == VIF_STA)
    {
        return &g_tx_bufing.bufing_gc_sta.timer;
    }
    else if(flag ==  VIF_AP)
    {
        return &g_tx_bufing.bufing_go_sap[client_idx].timer;
    }
    else
    {
        return NULL;
    }
}

//get chain
static tx_bufing_client *rwm_get_bufing_chain(uint8_t client_idx,uint8_t flag)
{
    if(flag == VIF_STA)
    {
        return &g_tx_bufing.bufing_gc_sta;
    }
    else if(flag ==  VIF_AP)
    {
        return &g_tx_bufing.bufing_go_sap[client_idx];
    }
    else
    {
        return NULL;
    }
}

static uint32_t rwm_get_client_bufing_count(uint8_t client_idx,uint8_t flag)
{
    LIST_HEADER_T *bufing_list = rwm_get_bufing_list(client_idx,flag);

    if (client_idx >= MAX_BUFING_CLIENT_NUM||bufing_list == NULL)
    {
        return 0;
    }

    return list_size(bufing_list);
 }

static void rwm_push_bufing_data(MSDU_NODE_T *node, uint8_t client_idx)
{
    MSDU_NODE_T *discard_node = NULL;

    uint8_t flag = vif_mgmt_get_type(node->vif_idx);
    uint32_t bufing_cnt = rwm_get_client_bufing_count(client_idx,flag);
    LIST_HEADER_T *bufing_list = rwm_get_bufing_list(client_idx,flag);

    if(bufing_cnt >= MAX_TXBUFFER_COUNT)
    {
        discard_node = rwm_pop_bufing_data(client_idx,flag);

        if(discard_node)
            wifi_os_free(discard_node);
        else
        {
            return;
        }
    }

    WIFI_GLOBAL_INT_DECLARATION();
    if (client_idx >= MAX_BUFING_CLIENT_NUM||bufing_list == NULL)
    {
        return;
    }

    WIFI_GLOBAL_INT_DISABLE();
    list_add_tail(&node->hdr, bufing_list);
    WIFI_GLOBAL_INT_RESTORE();
}

static MSDU_NODE_T *rwm_pop_bufing_data(uint8_t client_idx,uint8_t flag)
{
    LIST_HEADER_T *tmp;
    LIST_HEADER_T *pos;
    MSDU_NODE_PTR node;

    LIST_HEADER_T *bufing_list = rwm_get_bufing_list(client_idx,flag);

    WIFI_GLOBAL_INT_DECLARATION();

    if (client_idx >= MAX_BUFING_CLIENT_NUM||bufing_list == NULL)
    {
        return NULL;
    }

    WIFI_GLOBAL_INT_DISABLE();
    node = NULLPTR;
    list_for_each_safe(pos, tmp, bufing_list)
    {
        list_del(pos);
        node = list_entry(pos, MSDU_NODE_T, hdr);
        break;
    }
    WIFI_GLOBAL_INT_RESTORE();

    return node;
}

static void rwm_flush_bufing_list(uint8_t client_idx,uint8_t flag)
{
    MSDU_NODE_T *node_ptr;
    int ret ;

    beken_timer_t *bufing_timer = rwm_get_bufing_timer(client_idx,flag);

    if (client_idx >= MAX_BUFING_CLIENT_NUM||bufing_timer == NULL)
    {
        return;
    }

    if(rwm_get_client_bufing_count(client_idx,flag))
    {
        while(1) 
        {
            node_ptr = rwm_pop_bufing_data(client_idx,flag);
            if(node_ptr)
                wifi_os_free(node_ptr);
            else
                break;
        }
    }
    
    if(wifi_rtos_is_timer_running(bufing_timer))
    {
        ret = wifi_rtos_stop_timer(bufing_timer);
        ASSERT_ERR(0 == ret); /* ASSERT VERIFIED */
    }
}

void rwm_tx_bufing_end(uint8_t client_idx)
{
	UINT8 tx_vif_idx = sta_mgmt_get_vif_idx(client_idx);
	uint8_t flag = vif_mgmt_get_type(tx_vif_idx);
	tx_bufing_client *txbufing = rwm_get_bufing_chain(client_idx,flag);

    txbufing->cntrl_src = 0;

    rwm_flush_bufing_list(client_idx,flag);
}
void rwm_trigger_tx_bufing_start(uint32_t trigger_source, uint8_t client_idx)
{
    tx_bufing_client *txbufing = NULL;
    beken_timer_t *bufing_timer = NULL;
    int ret = 0;
    ret = ret;

    uint8_t flag = rwm_get_bufing_role(trigger_source);


    if ((client_idx >= MAX_BUFING_CLIENT_NUM) || (trigger_source == 0)||flag == VIF_UNKNOWN)
    {
        return;
    }

    txbufing = rwm_get_bufing_chain(client_idx,flag);
    txbufing->cntrl_src |=trigger_source;
    bufing_timer = rwm_get_bufing_timer(client_idx,flag);

    if(bufing_timer == NULL)
    {
        return;
    }

    if(!wifi_rtos_is_timer_running(bufing_timer))
    {
        ret = rtos_start_timer(bufing_timer);
        ASSERT_ERR(0 == ret); /* ASSERT VERIFIED */
    }
}

void rwm_trigger_tx_bufing_stop(uint32_t trigger_source, uint8_t client_idx)
{
    uint32_t bufing_cnt;
    uint32_t cid_and_flag;
    beken_timer_t *bufing_timer = NULL;

    uint8_t flag = rwm_get_bufing_role(trigger_source);
    tx_bufing_client *txbufing = rwm_get_bufing_chain(client_idx,flag);
    int ret = 0;
    ret = ret;

    if ((client_idx >= MAX_BUFING_CLIENT_NUM) || (trigger_source == 0)||flag == VIF_UNKNOWN||txbufing == NULL)
    {
         return;
    }
    
    if ((txbufing->cntrl_src & trigger_source) == 0)
    {
        return;
    }

    txbufing->cntrl_src &= (~trigger_source);

    if (txbufing->cntrl_src)
    {
        return;
    }

    bufing_cnt = rwm_get_client_bufing_count(client_idx,flag);
    bufing_timer = rwm_get_bufing_timer(client_idx,flag);

    if (bufing_timer == NULL)
    {
        return;
    }

    if (bufing_cnt > 0)
    {
        cid_and_flag = flag<<8|client_idx;
        bmsg_tx_bufing_sender(cid_and_flag);
    }

    if(wifi_rtos_is_timer_running(bufing_timer))
    {
        ret = wifi_rtos_stop_timer(bufing_timer);
        ASSERT_ERR(0 == ret); /* ASSERT VERIFIED */
    }

}

void rwm_gc_tx_bufing_timeout(void *data)
{
    uint8_t client_idx = (uint32_t)data;
    uint8_t flag = VIF_STA;

    rwm_flush_bufing_list(client_idx,flag);
}

void rwm_tx_bufing_timeout(void *data)
{
    uint8_t client_idx = (uint32_t)data;
    uint8_t flag = VIF_AP;

    rwm_flush_bufing_list(client_idx,flag);
}

void rwm_tx_bufing_save_data(MSDU_NODE_T *node)
{
    uint8_t client_idx;
    beken_timer_t *bufing_timer = NULL;


     int ret = 0;
     ret = ret;
    uint8_t flag = vif_mgmt_get_type(node->vif_idx);

    if(!node)
    {
        return;
    }

    client_idx = node->sta_idx;
    bufing_timer = rwm_get_bufing_timer(client_idx,flag);

    if (client_idx >= MAX_BUFING_CLIENT_NUM||bufing_timer == NULL)
    {
        return;
    }

    if(!rwm_get_client_bufing_count(client_idx,flag))
    {
        if(!wifi_rtos_is_timer_running(bufing_timer))
        {
            ret = rtos_start_timer(bufing_timer);
            ASSERT_ERR(0 == ret); /* ASSERT VERIFIED */
        }
    }

    rwm_push_bufing_data(node, client_idx);
}

void rwm_tx_bufing_restore_data(uint32_t cid_and_flag)
{
    uint32_t bufing_cnt;
    MSDU_NODE_T *node = NULL;

    uint8_t flag = (uint8_t)((0xFF00&cid_and_flag)>>8);
    uint8_t client_idx = (uint8_t)(0x00FF&cid_and_flag);
    tx_bufing_client *txbufing = rwm_get_bufing_chain(client_idx,flag);
    #if (NX_P2P)
    struct vif_info_tag *p_vif_entry = &vif_info_tab[client_idx];
    #endif
    struct sta_info_tag *sta = &sta_info_tab[client_idx];

    if (txbufing ==NULL || (client_idx >= MAX_BUFING_CLIENT_NUM) || (g_tx_bufing.tx_bufing_en == false))
    {
        return;
    }

    #if (NX_P2P)
    if( sta->ps_state == PS_MODE_ON||txbufing->cntrl_src != 0||!p2p_is_present(p_vif_entry->p2p_index))
    #else
    if( sta->ps_state == PS_MODE_ON||txbufing->cntrl_src != 0)
    #endif
    {
        return;
    }

    node = rwm_pop_bufing_data(client_idx,flag);

    if(!node)
    {
        return;
    }

    bufing_cnt = rwm_get_client_bufing_count(client_idx,flag);
    RWNX_LOGI("tx buffer: bufing_cnt %d restore node %x\n",bufing_cnt,node);
    rwm_transfer_node(node, TXU_CNTRL_MORE_DATA
        #if CONFIG_SPECIAL_TX_TYPE
        ,0
        #endif
        );

    if(bufing_cnt && (txbufing->cntrl_src == 0 ))
    {
        // trigger sending txing again
        bmsg_tx_bufing_sender(cid_and_flag);
    }
    else if(!bufing_cnt)
    {
        u8 vif_idx = sta_mgmt_get_vif_idx(client_idx);
        u16 aid = sta_mgmt_get_aid(client_idx);
        if (flag == VIF_AP)
        {
            //RWNX_LOGI("tx buffer: flag %x\n",flag);
            rw_msg_send_tim_update(vif_idx, aid, 0);
        }
    }
    else
    {
        return;
    }

}

bool rwm_check_tx_bufing(MSDU_NODE_T *node)
{
    uint8_t client_idx;
    tx_bufing_client *txbufing = NULL;

    uint8_t flag = vif_mgmt_get_type(node->vif_idx);

    if(!node)
    {
        return false;
    }

    if(flag == VIF_UNKNOWN)
    {
        return false;
    }

    if(flag == VIF_STA)
    {
         client_idx = node->vif_idx;
    }
    else if(flag == VIF_AP)
    {
         client_idx = node->sta_idx;
    }
    else
    {
         return false;
    }

    txbufing = rwm_get_bufing_chain(client_idx,flag);

    if ((client_idx >= MAX_BUFING_CLIENT_NUM) || (g_tx_bufing.tx_bufing_en == false)||txbufing == NULL)
    {
        return false;
    }

    if (txbufing->cntrl_src == 0)
    {
        return false;
    }

    return true;
}

void rwm_tx_bufing_init(void)
{
    g_tx_bufing.tx_bufing_en = true;

    int ret = 0;
    ret = ret;
    int i = 0;
    i = i;
    for( int i=0; i < MAX_BUFING_CLIENT_NUM; i++)
    {
        if(0 == i) {
            INIT_LIST_HEAD(&g_tx_bufing.bufing_gc_sta.bufing_list);
            ret =wifi_rtos_init_timer(&g_tx_bufing.bufing_gc_sta.timer,
                                   MAX_BUFFER_TIME,
                                   rwm_gc_tx_bufing_timeout,
                                   (void *)i);
            ASSERT_ERR(0 == ret); /* ASSERT VERIFIED */
        }

        INIT_LIST_HEAD(&g_tx_bufing.bufing_go_sap[i].bufing_list);
        ret =wifi_rtos_init_timer(&g_tx_bufing.bufing_go_sap[i].timer,
                               MAX_BUFFER_TIME,
                               rwm_tx_bufing_timeout,
                               (void *)i);
        ASSERT_ERR(0 == ret); /* ASSERT VERIFIED */
    }
}
#endif


