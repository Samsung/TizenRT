/******************************************************************************
 *
 * Copyright(c) 2016 - 2017 Realtek Corporation.
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

#include "rtw_coex_host_api.h"
#include "rtw_coex_api_ext.h"

/////////////////////////////////////////////////////////////////////
//////// for COMMON
/////////////////////////////////////////////////////////////////////
static int rtk_coex_c2h_com_indicate(u16 type, u8 *pdata, u16 len)
{
	(void) pdata;
	(void) len;

	switch (COEX_SUBTYPE_GET(type)) {
	case COEX_C2H_COM_UNDEF:
		break;
	default:
		break;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////
//////// for BT
/////////////////////////////////////////////////////////////////////
extern void rtk_coex_btc_wl_send_sw_mailbox(u8 type, u8 *data, u16 length);
static int rtk_coex_c2h_bt_indicate(u16 type, u8 *pdata, u16 len)
{
	(void) pdata;
	(void) len;

	switch (COEX_SUBTYPE_GET(type)) {
	case COEX_C2H_BT_UNDEF:
		break;
	case COEX_C2H_BT_HCI_MSG_MAILBOX_TRIGGER:
		rtk_coex_btc_wl_send_sw_mailbox(COEX_SUBTYPE_GET(type), pdata, len);
		break;
	default:
		break;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////
//////// for EXT
/////////////////////////////////////////////////////////////////////
static int rtk_coex_c2h_ext_indicate(u16 type, u8 *pdata, u16 len)
{
	(void) pdata;
	(void) len;

	int ret = -1;

	switch (COEX_SUBTYPE_GET(type)) {
	case COEX_C2H_EXT_UNDEF:
		break;
	case COEX_C2H_EXT_GET_INITPARAMS:
		if ((len == sizeof(struct extchip_para_t)) && (pdata != NULL)) {
			ret = rtk_coex_extc_get_init_params((struct extchip_para_t *)pdata);
		}
		break;
	default:
		break;
	}

	return ret;
}

/*
* drv->app
* parse type & handle @ app side
*/
int rtk_coex_ipc_c2h_info_indicate(u16 type, u8 *pdata, u16 data_len)
{
	int ret = -1;

	switch (COEX_TYPE_GET(type)) {
	case RTK_COEX_TYPE_BT:
		ret = rtk_coex_c2h_bt_indicate(type, pdata, data_len);
		break;
	case RTK_COEX_TYPE_EXT:
		ret = rtk_coex_c2h_ext_indicate(type, pdata, data_len);
		break;
	case RTK_COEX_TYPE_COM:
		ret = rtk_coex_c2h_com_indicate(type, pdata, data_len);
	default:
		break;
	}

	return ret;
}

