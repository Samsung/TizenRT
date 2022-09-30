/*
 * Copyright (c) 2017, Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef _GAP_HANDOVER_H_
#define _GAP_HANDOVER_H_

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#include "gap.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup    GAP_HANDOVER       GAP Handover
 *
 * \brief   Provide Bluetooth stack handover related functions.
 *
 */

#define GAP_ACL_FLOW_STOP       0
#define GAP_ACL_FLOW_GO         1

#define GAP_ACL_ARQN_NACK       0
#define GAP_ACL_ARQN_ACK        1
typedef enum {
	GAP_HANDOVER_TYPE_ACL,
	GAP_HANDOVER_TYPE_SM,
	GAP_HANDOVER_TYPE_L2C,
	GAP_HANDOVER_TYPE_SCO,
	GAP_HANDOVER_TYPE_BUD
} T_GAP_HANDOVER_TYPE;

typedef enum {
	GAP_SHADOW_SNIFF_OP_NO_SNIFFING,
	GAP_SHADOW_SNIFF_OP_SELF_SNIFFING,
	GAP_SHADOW_SNIFF_OP_PEER_SNIFFING
} T_GAP_SHADOW_SNIFF_OP;

typedef enum {
	GAP_ACL_SUSPEND_TYPE_STOP_ALL = 0,
	GAP_ACL_SUSPEND_TYPE_NAK = 1,
	GAP_ACL_SUSPEND_TYPE_STOP_PER_LINK = 2,
	GAP_ACL_SUSPEND_BY_STOP_TRX = 3
} T_GAP_ACL_SUSPEND_TYPE;

typedef struct {
	uint8_t     bd_addr[6];
	uint16_t    handle;
	uint8_t     role;
	uint8_t     mode;
	uint16_t    link_policy;
	uint16_t    superv_tout;
	uint8_t     encrypt_state;
	uint8_t     bd_type;
	uint8_t     conn_type;
} T_GAP_HANDOVER_ACL_INFO;

typedef struct {
	uint8_t     bd_addr[6];
	uint32_t    mode;
	uint8_t     state;
	uint8_t     sec_state;
	uint8_t     remote_authen;
	uint8_t     remote_io;
} T_GAP_HANDOVER_SM_INFO;

typedef struct {
	uint8_t     bd_addr[6];
	uint16_t    local_cid;
	uint16_t    remote_cid;
	uint16_t    local_mtu;
	uint16_t    remote_mtu;
	uint16_t    local_mps;
	uint16_t    remote_mps;
	uint16_t    psm;
	uint8_t     role;
	uint8_t     mode;
} T_GAP_HANDOVER_L2C_INFO;

typedef struct {
	uint8_t     bd_addr[6];
	uint16_t    handle;
	uint8_t     type;
} T_GAP_HANDOVER_SCO_INFO;

typedef struct {
	uint8_t     pre_bd_addr[6];
	uint8_t     curr_bd_addr[6];
	uint8_t     curr_link_role;
} T_GAP_HANDOVER_BUD_INFO;

typedef struct {
	uint16_t    handle;
	uint16_t    cause;
	uint8_t     param_len;
	uint8_t     param[14];
} T_GAP_SNIFFING_STATE_SYNC_INFO;

typedef struct {
	uint16_t        cause;
} T_GAP_SHADOW_PRE_SYNC_INFO_RSP;

typedef struct {
	uint16_t        cause;
} T_GAP_SET_ACL_ACTIVE_STATE_RSP;

typedef struct {
	uint16_t        cause;
} T_GAP_SHADOW_LINK_RSP;

typedef struct {
	uint16_t        cause;
	uint16_t        handle;
} T_GAP_ACL_SUSPEND_RX_EMPTY_INFO;

typedef struct {
	uint16_t        cause;
	uint16_t        handle;
	uint8_t         bd_addr[6];
	uint8_t         link_type;
	uint8_t         encrypt_enabled;
	uint8_t         esco_interval;
	uint8_t         esco_retx_window;
	uint8_t         esco_rx_packet_length[2];
	uint8_t         esco_tx_packet_length[2];
	uint8_t         esco_air_mode;
	uint8_t         acl_role;
	uint8_t         acl_mode;
	uint8_t         acl_link_policy;
	uint8_t         acl_superv_tout;
} T_GAP_HANDOVER_CONN_CMPL_INFO;

typedef struct {
	uint16_t        cause;
	uint8_t         sniff_op;
} T_GAP_HANDOVER_CMPL_INFO;

typedef struct {
	uint16_t        cause;
} T_GAP_SETUP_AUDIO_RECOVERY_LINK_RSP;

typedef struct {
	uint8_t         bd_addr[6];
	uint8_t         audio_type;
} T_GAP_AUDIO_RECOVERY_LINK_REQ_IND;

typedef struct {
	uint16_t        cause;
	uint16_t        recovery_handle;
	uint16_t        ctrl_handle;
	uint16_t        target_handle;
	uint8_t         audio_type;
	uint16_t        a2dp_cid;
	uint16_t        a2dp_interval;
	uint16_t        a2dp_flush_tout;
	uint8_t         a2dp_rsvd_slot;
	uint8_t         a2dp_idle_slot;
	uint8_t         a2dp_idle_skip;
} T_GAP_AUDIO_RECOVERY_LINK_CONN_CMPL_INFO;

typedef struct {
	uint16_t        cause;
	uint16_t        recovery_handle;
	uint16_t        reason;
} T_GAP_AUDIO_RECOVERY_LINK_DISCONN_CMPL_INFO;

typedef struct {
	uint16_t        cause;
	uint16_t        target_handle;
	uint8_t         current_sniffing_mode;
} T_GAP_SHADOW_SNIFFING_MODE_CHANGE_INFO;

typedef struct {
	uint16_t        cause;
	uint16_t        recovery_conn_handle;
	uint8_t         audio_type;
	uint16_t        audio_conn_handle;
	uint16_t        media_l2cap_cid;
	uint16_t        a2dp_interval;
	uint16_t        a2dp_flush_timeout;
	uint8_t         a2dp_ctrl_resvd_slot;
	uint8_t         a2dp_idle_slot;
	uint8_t         a2dp_idle_skip;
} T_GAP_AUDIO_RECOVERY_LINK_CONNECTION_CHANGE_INFO;

typedef struct {
	uint16_t        cause;
	uint16_t        target_handle;
	uint16_t        ctrl_handle;
} T_GAP_SHADOW_CTRL_LINK_CHANGE_INFO;

typedef struct {
	uint16_t        cause;
	uint16_t        recovery_conn_handle;
} T_GAP_AUDIO_RECOVERY_LINK_CONNECTION_RESET_INFO;

typedef struct {
	uint16_t        cause;
	uint16_t        recovery_conn_handle;
} T_GAP_AUDIO_RECOVERY_LINK_FLUSH_COMPLETE;

typedef struct {
	uint16_t        handle;
	uint16_t        reason;
} T_GAP_SHADOW_LINK_LOSS_INFO;

typedef struct {
	uint16_t        cause;
	uint16_t        handle;
	uint8_t         new_role;
	uint8_t         local_addr[6];
	uint8_t         remote_addr[6];
} T_GAP_VENDOR_ROLE_SWITCH;

// for RTK HFP Sniffing
typedef struct {
	uint16_t        handle;
	uint8_t         esco_dup_number;
} T_GAP_VENDOR_ESCO_DUP_NUM;

typedef struct {
	uint16_t        cause;
	uint16_t        handle;
	uint8_t         enable;
} T_GAP_SET_CONTINUOUS_TRX_CMPL;

typedef enum {
	GET_ACL_INFO_RSP = 0x00,
	GET_SCO_INFO_RSP = 0x01,
	GET_SM_INFO_RSP  = 0x02,
	GET_L2C_INFO_RSP = 0x03,
	SET_ACL_INFO_RSP = 0x04,
	SET_SCO_INFO_RSP = 0x05,
	SET_SM_INFO_RSP  = 0x06,
	SET_L2C_INFO_RSP = 0x07,
	SET_BUD_INFO_RSP = 0x08,
	DEL_ACL_INFO_RSP = 0x09,
	DEL_SCO_INFO_RSP = 0x0A,
	DEL_SM_INFO_RSP  = 0x0B,
	DEL_L2C_INFO_RSP = 0x0C,
	SHADOW_PRE_SYNC_INFO_RSP                    = 0x0D,
	SET_ACL_ACTIVE_STATE_RSP                    = 0x0E,
	ACL_SUSPEND_RX_EMPTY_INFO                   = 0x0F,
	SHADOW_LINK_RSP                             = 0x10,
	HANDOVER_CONN_CMPL_INFO                     = 0x11,
	HANDOVER_CMPL_INFO                          = 0x12,
	SETUP_AUDIO_RECOVERY_LINK_RSP               = 0x13,
	AUDIO_RECOVERY_LINK_REQ_IND                 = 0x14,
	AUDIO_RECOVERY_LINK_CONN_CMPL_INFO          = 0x15,
	AUDIO_RECOVERY_LINK_DISCONN_CMPL_INFO       = 0x16,
	AUDIO_RECOVERY_LINK_CONNECTION_RESET_INFO   = 0x17,
	AUDIO_RECOVERY_LINK_CONNECTION_CHANGE_INFO  = 0x18,
	AUDIO_RECOVERY_LINK_FLUSH_COMPLETE_INFO     = 0x19,
	SHADOW_CTRL_LINK_CHANGE_INFO                = 0x1A,
	SHADOW_SNIFFING_MODE_CHANGE_INFO            = 0x1B,
	VENDOR_ROLE_SWITCH                          = 0x1C,
	SHADOW_LINK_LOSS_INFO                       = 0x1D,
	SHADOW_ESCO_DUPLICATE_NUMBER_CHANGE_INFO    = 0x1E,
	SET_CONTINUOUS_TRX_CMPL                     = 0x1F,
	SNIFFING_STATE_SYNC_INFO                    = 0x20,
} T_HANDOVER_MSG_TYPE;

typedef void (* P_HANDOVER_CB)(void *p_buf, T_HANDOVER_MSG_TYPE msg);

void legacy_reg_handover_cb(P_HANDOVER_CB p_func);

T_GAP_CAUSE legacy_get_handover_acl_info(uint8_t *bd_addr);

T_GAP_CAUSE legacy_get_handover_sco_info(uint8_t *bd_addr);

T_GAP_CAUSE legacy_get_handover_sm_info(uint8_t *bd_addr);

T_GAP_CAUSE legacy_get_handover_l2c_info(uint16_t cid);

T_GAP_CAUSE legacy_set_handover_bud_info(T_GAP_HANDOVER_BUD_INFO *p_info);

T_GAP_CAUSE legacy_set_handover_acl_info(T_GAP_HANDOVER_ACL_INFO *p_info);

T_GAP_CAUSE legacy_set_handover_sco_info(T_GAP_HANDOVER_SCO_INFO *p_info);

T_GAP_CAUSE legacy_set_handover_sm_info(T_GAP_HANDOVER_SM_INFO *p_info);

T_GAP_CAUSE legacy_set_handover_l2c_info(T_GAP_HANDOVER_L2C_INFO *p_info);

T_GAP_CAUSE legacy_del_handover_acl_info(uint8_t *bd_addr);

T_GAP_CAUSE legacy_del_handover_sco_info(uint8_t *bd_addr);

T_GAP_CAUSE legacy_del_handover_sm_info(uint8_t *bd_addr);

T_GAP_CAUSE legacy_del_handover_l2c_info(uint16_t cid);

T_GAP_CAUSE legacy_sniffing_state_sync(uint16_t ctrl_handle, uint8_t len, uint8_t *p_param);

T_GAP_CAUSE legacy_shadow_link(uint16_t tgt_handle, uint16_t ctrl_handle,
							   T_GAP_SHADOW_SNIFF_OP sniff_op);

T_GAP_CAUSE legacy_shadow_pre_sync_info(uint16_t tgt_handle, uint16_t ctrl_handle,
										uint8_t sync_type);

T_GAP_CAUSE legacy_set_acl_arqn(uint16_t acl_handle, uint8_t arqn);

T_GAP_CAUSE legacy_set_acl_active_state(uint16_t acl_handle, uint8_t state, uint8_t suspend_type);

T_GAP_CAUSE legacy_disconn_sniffing_link(uint16_t handle, uint8_t reason);

T_GAP_CAUSE legacy_set_continuous_txrx(uint16_t ctrl_handle, uint8_t enable, uint8_t option);

T_GAP_CAUSE legacy_setup_audio_recovery_link(uint16_t ctrl_handle, uint8_t audio_type,
		uint16_t tgt_handle, uint16_t media_cid,
		uint16_t interval, uint16_t flush_tout,
		uint8_t rsvd_slot, uint16_t a2dp_init_expected_seq,
		uint8_t idle_slot, uint8_t idle_skip);

T_GAP_CAUSE legacy_audio_recovery_link_req_reply(uint16_t ctrl_handle, uint8_t audio_type,
		uint16_t tgt_handle, uint8_t audio_in_order);

T_GAP_CAUSE legacy_remove_audio_recovery_link(uint16_t recovery_handle, uint8_t reason);

T_GAP_CAUSE legacy_reset_audio_recovery_link(uint16_t recovery_handle);

T_GAP_CAUSE legacy_flush_audio_recovery_link(uint16_t recovery_handle, uint16_t flush_timeout);

T_GAP_CAUSE legacy_vendor_set_esco_dup_num(uint16_t tgt_handle, uint16_t ctrl_handle,
		uint8_t dup_num);

T_GAP_CAUSE legacy_vendor_set_a2dp_dup_num(uint8_t enable, uint8_t dup_num, uint8_t quick_flush);

T_GAP_CAUSE legacy_vendor_role_switch(uint16_t ctrl_handle, uint8_t role, uint16_t flush_timeout);

#ifdef __cplusplus
}
#endif    /*  __cplusplus */

#endif

#endif    /*  __GAP_HANDOVER_H__*/
