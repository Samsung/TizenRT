/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     gap_fix_chann_conn.h
* @brief    header file of LE Fixed Channel Connection message handle.
* @details  none.
* @author   Tifnan
* @date     2016-03-16
* @version  v0.1
*********************************************************************************************************
*/

#ifndef _LE_FIX_CHANN_CONN_H_
#define _LE_FIX_CHANN_CONN_H_

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#include "upperstack_config.h"
#include "gap_le.h"
#include "gap.h"

#if F_BT_LE_FIX_CHANN_SUPPORT
T_GAP_CAUSE le_fixed_chann_reg(uint16_t cid);

T_GAP_CAUSE le_fixed_chann_data_send(uint8_t conn_id, uint16_t cid, uint8_t *p_data,
                                     uint16_t data_len);
#endif

#endif

#endif /* _LE_FIX_CHANN_CONN_H_ */
