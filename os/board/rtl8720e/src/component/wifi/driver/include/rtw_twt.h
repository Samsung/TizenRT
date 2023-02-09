/******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *****************************************************************************/
#ifndef _RTW_TWT_H_
#define _RTW_TWT_H_

#define TWT_IE_LEN	15

/* set twt element control field: 1-octet */
#define SET_TWT_CTRL_NDP_PADDING_IND(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart, 0, 1, _val)
#define SET_TWT_CTRL_RESPONDER_PM_MODE(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart, 1, 1, _val)
#define SET_TWT_CTRL_NEGOTIATION_TYPE(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart, 2, 2, _val)
#define SET_TWT_CTRL_INFO_DISABLE(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart, 4, 1, _val)
#define SET_TWT_CTRL_DURATION_UNIT(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart, 5, 1, _val)

/* set twt element TWT parameter field: 14-octet */
#define SET_TWT_PARA_REQ_TYPE_TWT_REQ(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart, 0, 1, _val)
#define SET_TWT_PARA_REQ_TYPE_SET_CMD(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart, 1, 3, _val)
#define SET_TWT_PARA_REQ_TYPE_TRIGGER(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart, 4, 1, _val)
#define SET_TWT_PARA_REQ_TYPE_IMPLICIT(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart, 5, 1, _val)
#define SET_TWT_PARA_REQ_TYPE_FLOW_TYPE(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart, 6, 1, _val)
#define SET_TWT_PARA_REQ_TYPE_FLOW_ID(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart, 7, 3, _val)
#define SET_TWT_PARA_REQ_TYPE_WAKE_INTERVAL_EXP(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart + 1, 2, 5, _val)
#define SET_TWT_PARA_REQ_TYPE_TWT_PROTECTION(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart + 1, 7, 1, _val)

#define SET_TWT_PARA_TWT_L(_pEleStart, _val) \
	SET_BITS_TO_LE_4BYTE(_pEleStart + 2, 0, 32, _val)
#define SET_TWT_PARA_TWT_H(_pEleStart, _val) \
	SET_BITS_TO_LE_4BYTE(_pEleStart + 6, 0, 32, _val)

#define SET_TWT_PARA_NORMINAL_DURATION(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart +10, 0, 8, _val)

#define SET_TWT_PARA_WAKE_INTERVAL_MAN(_pEleStart, _val) \
	SET_BITS_TO_LE_2BYTE(_pEleStart +11, 0, 16, _val)

#define SET_TWT_PARA_CHANNEL(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(_pEleStart +13, 0, 8, _val)

/* get twt element control field: 1-octet */
#define GET_TWT_CTRL_NDP_PADDING_IND(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart, 0, 1)
#define GET_TWT_CTRL_RESPONDER_PM_MODE(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart, 1, 1)
#define GET_TWT_CTRL_NEGOTIATION_TYPE(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart, 2, 2)
#define GET_TWT_CTRL_INFO_DISABLE(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart, 4, 1)
#define GET_TWT_CTRL_DURATION_UNIT(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart, 5, 1)

/* get twt element TWT parameter field: 14-octet */
#define GET_TWT_PARA_REQ_TYPE_TWT_REQ(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart, 0, 1)
#define GET_TWT_PARA_REQ_TYPE_SET_CMD(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart, 1, 3)
#define GET_TWT_PARA_REQ_TYPE_TRIGGER(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart, 4, 1)
#define GET_TWT_PARA_REQ_TYPE_IMPLICIT(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart, 5, 1)
#define GET_TWT_PARA_REQ_TYPE_FLOW_TYPE(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart, 6, 1)
#define GET_TWT_PARA_REQ_TYPE_FLOW_ID(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart, 7, 3)
#define GET_TWT_PARA_REQ_TYPE_WAKE_INTERVAL_EXP(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart + 1, 2, 5)
#define GET_TWT_PARA_REQ_TYPE_TWT_PROTECTION(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart + 1, 7, 1)

#define GET_TWT_PARA_TWT_L(_pEleStart) \
	LE_BITS_TO_4BYTE(_pEleStart + 2, 0, 32)
#define GET_TWT_PARA_TWT_H(_pEleStart) \
	LE_BITS_TO_4BYTE(_pEleStart + 6, 0, 32)

#define GET_TWT_PARA_NORMINAL_DURATION(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart +10, 0, 8)

#define GET_TWT_PARA_WAKE_INTERVAL_MAN(_pEleStart) \
	LE_BITS_TO_2BYTE(_pEleStart +11, 0, 16)

#define GET_TWT_PARA_CHANNEL(_pEleStart) \
	LE_BITS_TO_1BYTE(_pEleStart +13, 0, 8)

typedef struct {
	unsigned char id;
	unsigned char announced;
	unsigned char trigger;
	u64 target_time;//unit: us
	u32 interval;//unit: us
	u16 duration;//unit: us
} twt_para_t;

struct twt_control_t {
	unsigned char rsvd: 2;
	unsigned char wake_duration_unit: 1;	/* 0: the unit is 256 us, 1: the unit is a TU. A non-HE STA sets to 0 */
	unsigned char info_disable: 1;
	unsigned char neg_type: 2;	// 0 for individual; 3 for broadcast
	unsigned char responder_pm_mode: 1;
	unsigned char ndp: 1;
};

/* setup_cmd		twt_req		meaning
		0			1		request, without parameter request
		1			1		suggest, with parameter request, can accept parameter change
		2			1		demond, with parameter requset, cannot accept parameter change
		3			0		not support
		4			0		accept, accept from responder
		5			0		alternate, suggest different parameter
		6			0		dictate, suggest different parameter
		7			0		reject */
struct twt_req_type_t {
	u16 protection: 1;		/* 1: request NAV protection during SP */
	u16 interval_exp: 5;
	u16 twt_id: 3;		/* the maximum twt connection between two STAs is 4 */
	u16 flow_type: 1;		/* 1: unaonunced, 0: aonunced */
	u16 implict: 1;		/* 1: implicit, 0: explicit. An HE STA negotiate individual TWT agreements shall set the Implicit subfield to 1 */
	u16 trigger: 1;		/* 1: trigger, 0: no-trigger */
	u16 setup_cmd: 3;
	u16 twt_req: 1;
};

/* twt_wake_interval = interval_mantissa*2exp(interval_exp) */
struct twt_ie_other_t {
	u32 twt_l;	/* low 32bit of target wake time, corresponding to TSF time */
	u32 twt_h;	/* high 32bit of target wake time, corresponding to TSF time */
	unsigned char norminal_wake_duration;
	u16 interval_mantissa;
	unsigned char twt_channel;		/* 0: 20MHz */
};

struct twt_ie_t {
	struct twt_control_t twt_ctrl;
	struct twt_req_type_t twt_para;
	struct twt_ie_other_t twt_ie_o;
};

enum twt_class {
	TWTCLIENTA               = 0x0,
	TWTCLIENTB               = 0x1,
	TWTCLIENTC               = 0x2,
	TWTCLIENTD               = 0x3,
	TWTCLIENTE               = 0x4,
};

void rtw_twt_timer_enable(u8 idx, u8 port);
void rtw_twt_timer_disable(u8 idx);
void rtw_twt_set_early_time(u8 idx, u8 early_time);
void rtw_twt_set_target_wake_time(u8 idx, u64 twt);
#endif

