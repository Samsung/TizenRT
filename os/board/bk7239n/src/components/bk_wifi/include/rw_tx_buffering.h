/**
 ****************************************************************************************
 *
 * @file rw_tx_buffering.h
 *
 * @brief TX buffering module.
 *
 * Copyright (C) BEKEN corperation 2021
 *
 ****************************************************************************************
 */
#ifndef _RW_TX_BUFFERING_
#define _RW_TX_BUFFERING_
/*
 * INCLUDE FILES
 ****************************************************************************************
 */

//#include <common/bk_list.h>
#include "rw_msdu.h"
#if CONFIG_TX_BUFING
/*
 * TYPE and STRUCT DEFINITIONS
 ****************************************************************************************
 */

#define MAX_BUFING_CLIENT_NUM           NX_REMOTE_STA_MAX      // CFG_STA_MAX
#define MAX_BUFFER_TIME                 3000                   // 3S
#define MAX_TXBUFFER_COUNT              15                     // MAX TXBUFFER COUNT

#define TX_BUFING_SRC_STA_PS            (CO_BIT(0))
#define TX_BUFING_SRC_STA_SCAN          (CO_BIT(1))
#define TX_BUFING_SRC_STA_MCC           (CO_BIT(2))
#define TX_BUFING_SRC_STA_BT_COEX       (CO_BIT(3))
#define TX_BUFING_SRC_P2P_PS            (CO_BIT(4))
#define TX_BUFING_SRC_P2P_NOA           (CO_BIT(5))
#define TX_BUFING_SRC_TWT               (CO_BIT(6))

//tx buffering client struct
typedef struct tx_buffering_client_st {
    beken_timer_t timer;                //timer used to flush the buffering data
    uint32_t cntrl_src;                   //source of triggering tx buffering
    struct list_head bufing_list;       //list to buffering data
} tx_bufing_client;

//tx buffering module struct
typedef struct rwm_tx_buffering_st {
    bool tx_bufing_en;

    tx_bufing_client bufing_go_sap[MAX_BUFING_CLIENT_NUM];
	tx_bufing_client bufing_gc_sta; //client is GO	
} tx_bufing;


/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */



/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initializes the CFM and BA queues.
 * These queues are useful for sending confirmation to UMAC for the txdescs in order.
 *
 * The CFM queue will handle the linked list of txdescs, each having a certain status
 * after handling the TX trigger from HW (acked, waiting for BA analysis, not acked at all)
 *
 * The BA queue holds the linked list of rxdescs of BA frames per AC, which will be used and
 * freed when the TX_CFM kernel event is handled in order to update the MPDU status
 * of those MPDUs part of AMPDUs in the confirm list.
 *
 * For singleton MPDUs the BA queue is of no use, they should be confirmed according to
 * their already set ok/not acked status.
 *
 ****************************************************************************************
 */
void rwm_trigger_tx_bufing_start(uint32_t trigger_source, uint8_t client_idx);
void rwm_trigger_tx_bufing_stop(uint32_t trigger_source, uint8_t client_idx);
bool rwm_check_tx_bufing(MSDU_NODE_T *node);
void rwm_tx_bufing_save_data(MSDU_NODE_T *node);
void rwm_tx_bufing_restore_data(uint32_t cid_and_flag);
void rwm_gc_tx_bufing_timeout(void *data);
void rwm_tx_bufing_timeout(void *data);
void rwm_tx_bufing_end(uint8_t client_idx);
void rwm_tx_bufing_init(void);
#endif
#endif
