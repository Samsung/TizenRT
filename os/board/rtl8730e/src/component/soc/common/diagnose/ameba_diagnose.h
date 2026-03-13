/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_DIAGNOSE_H_
#define _AMEBA_DIAGNOSE_H_

#include "ameba_soc.h"
#include "ameba_diagnose_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************************
 *                                   Event User's Definitions
 ****************************************************************************************************/

#define RTK_DIAG_EVENT_DEFAULT_LEVEL RTK_EVENT_LEVEL_INFO

// Only level >= RTK_EVENT_COMPIL_LEVEL will be added to the linked list
#define RTK_DIAG_EVENT_ADD(_level, _type, _info, _len) \
    ((RTK_DIAG_EVENT_DEFAULT_LEVEL >= (_level)) ? \
        rtk_diag_event_add(_level, _type, _info, _len) : \
        RTK_ERR_DIAG_EVT_ADD_FAIL)


#define RTK_EVENT_A(_type, _info, _len)  RTK_DIAG_EVENT_ADD(RTK_EVENT_LEVEL_ALWAYS, _type, _info, _len)
#define RTK_EVENT_E(_type, _info, _len)  RTK_DIAG_EVENT_ADD(RTK_EVENT_LEVEL_ERROR, _type, _info, _len)
#define RTK_EVENT_W(_type, _info, _len)  RTK_DIAG_EVENT_ADD(RTK_EVENT_LEVEL_WARNING, _type, _info, _len)
#define RTK_EVENT_I(_type, _info, _len)  RTK_DIAG_EVENT_ADD(RTK_EVENT_LEVEL_INFO, _type, _info, _len)
#define RTK_EVENT_D(_type, _info, _len)  RTK_DIAG_EVENT_ADD(RTK_EVENT_LEVEL_DEBUG, _type, _info, _len)


/*****************************************************************************************************
 *                                   Event Manager's Definitions
 ****************************************************************************************************/

//NOTE: Porting ipc for each ic
#if defined(CONFIG_AMEBAD)
#define DIAG_IPC_DIR NULL
#define DIAG_IPC_CHANNEL 0 //TODO:
#elif defined(CONFIG_AMEBADPLUS)
#define DIAG_IPC_DIR IPC_KM4_TO_KM0
#define DIAG_IPC_CHANNEL IPC_A2N_DIAGNOSE
#elif defined(CONFIG_AMEBAGREEN2) || defined(CONFIG_AMEBAL2) || defined(CONFIG_AMEBASMART) || defined(CONFIG_AMEBAPRO3) || defined(CONFIG_RTL8720F)
#define DIAG_IPC_DIR IPC_AP_TO_NP
#define DIAG_IPC_CHANNEL IPC_A2N_DIAGNOSE
#elif defined(CONFIG_AMEBALITE)
#define DIAG_IPC_DIR IPC_KM4_TO_KR4
#define DIAG_IPC_CHANNEL IPC_M2R_DIAGNOSE
#else
#error no ic defined
#endif

int rtk_diag_init(u16 heap_capacity, u16 sender_buffer_size);
void rtk_diag_deinit(void);
int rtk_diag_event_add(u8 evt_level, u16 evt_type, const u8 *evt_info, u16 evt_len);

int rtk_diag_config_transform(rtk_diag_sender_t sender, u8 *sender_buffer, u16 sender_buffer_size);
bool rtk_diag_transform_has_configed(void);

//handle atcmd request and response
int rtk_diag_req_timestamp(void);
int rtk_diag_req_version(void);
int rtk_diag_req_set_buf_com_capacity(u8 *sender_buffer, u16 capacity); //set send buffer capacity
int rtk_diag_req_set_buf_evt_capacity(u16 capacity); //set heap usage limit of event
int rtk_diag_req_get_event(u32 timestamp, u16 offset);
int rtk_diag_req_del_event_before(u32 timestamp);
int rtk_diag_req_del_event_after(u32 timestamp); //This api will delete all events actually
int rtk_diag_req_get_del_events(void); //get deleted events recently
int rtk_diag_req_clr_del_events(void); //clear deleted events records
int rtk_diag_req_log_enable(u8 state);

void rtk_diag_proto_pack_crc(RtkDiagDataFrame_t *frame);
int rtk_diag_req_low(u8 cmd, const u8 *payload, u16 payload_length, u8 error);
//for debug
#ifdef DIAG_DEBUG_TEST
int rtk_diag_req_add_event_demo1(u8 evt_level, const char *data);
int rtk_diag_req_dbg_log_enable(u8 state);
int rtk_diag_dbg_send(const u8 *data, u16 len);
#endif

#ifdef __cplusplus
}
#endif

#endif //_AMEBA_DIAGNOSE_H_
