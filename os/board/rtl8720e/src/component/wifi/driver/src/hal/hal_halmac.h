/******************************************************************************
 *
 * Copyright(c) 2015 - 2016 Realtek Corporation. All rights reserved.
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
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef _HAL_HALMAC_H_
#define _HAL_HALMAC_H_

#include <drv_types.h>		/* adapter_to_dvobj(), struct intf_hdl and etc. */
#include <hal_data.h>		/* struct hal_spec_t */
#include "halmac_hw_cfg.h"	/* PHALMAC_ADAPTER and etc. */
#include "halmac_api.h"	/* PHALMAC_ADAPTER and etc. */
#if defined(CONFIG_RTL8195B)
#include "halmac_88xx/halmac_88xx_cfg.h"
#include "halmac_88xx/halmac_8195b/halmac_8195b_cfg.h"
#endif
#if defined(CONFIG_RTL8710C)
#include "halmac_88xx/halmac_88xx_cfg.h"
#include "halmac_88xx/halmac_8710c/halmac_8710c_cfg.h"
#endif
#if defined(CONFIG_RTL8735B)
#include "halmac_88xx/halmac_88xx_cfg.h"
#include "halmac_88xx/halmac_8735b/halmac_8735b_cfg.h"
#endif

/* HALMAC Definition for Driver */
#define RTW_HALMAC_H2C_MAX_SIZE		8
#define RTW_HALMAC_BA_SSN_RPT_SIZE	4

#define set_adapter_halmac(p, mac)	(rtw_get_pshare(p)->halmac = (mac))
#define adapter_to_halmac(p)		(rtw_get_pshare(p)->halmac)
//#define dvobj_to_regsty(dvobj) (&(((_adapter*)(dvobj->if1))->registrypriv))

/* for H2C cmd */
#define MAX_H2C_BOX_NUMS 4
#define MESSAGE_BOX_SIZE 4
#define EX_MESSAGE_BOX_SIZE 4

#define rtw_hal_power_on(padapter)	wifi_hal_power_on(padapter)
#define rtw_hal_power_off(padapter)	wifi_hal_power_off(padapter)
#define rtw_hal_init_mac_register(padapter)	wifi_hal_init_phy_parameter_mac(padapter)
#define rtw_hal_init_phy(padapter)	wifi_hal_phy_init(padapter)

#ifdef RTW_HALMAC
struct halmac_indicator {
	struct submit_ctx *sctx;
	u8 *buffer;
	u32 buf_size;
	u32 ret_size;
	u32 status;
};
#endif

typedef enum _RTW_HALMAC_MODE {
	RTW_HALMAC_MODE_NORMAL,
	RTW_HALMAC_MODE_WIFI_TEST,
} RTW_HALMAC_MODE;

extern HALMAC_PLATFORM_API rtw_halmac_platform_api;

/* HALMAC API for Driver(HAL) */
u8 rtw_halmac_read8(_adapter *padapter, u32 addr, s32 *err);
u16 rtw_halmac_read16(_adapter *padapter, u32 addr, s32 *err);
u32 rtw_halmac_read32(_adapter *padapter, u32 addr, s32 *err);
void rtw_halmac_read_mem(_adapter *padapter, u32 addr, u32 cnt, u8 *pmem);
#ifdef CONFIG_SDIO_INDIRECT_ACCESS
u8 rtw_halmac_iread8(struct intf_hdl *pintfhdl, u32 addr);
u16 rtw_halmac_iread16(struct intf_hdl *pintfhdl, u32 addr);
u32 rtw_halmac_iread32(struct intf_hdl *pintfhdl, u32 addr);
#endif
s32 rtw_halmac_write8(_adapter *padapter, u32 addr, u8 value, s32 *err);
s32 rtw_halmac_write16(_adapter *padapter, u32 addr, u16 value, s32 *err);
s32 rtw_halmac_write32(_adapter *padapter, u32 addr, u32 value, s32 *err);

int rtw_halmac_init_adapter(_adapter *padapter, PHALMAC_PLATFORM_API);
int rtw_halmac_deinit_adapter(_adapter *padapter);
int rtw_halmac_poweron(_adapter *padapter);
int rtw_halmac_poweroff(_adapter *padapter);

int rtw_halmac_init_hal(_adapter *padapter);
int rtw_halmac_init_hal_fw(_adapter *padapter, u8 *fw, u32 fwsize);
int rtw_halmac_init_hal_fw_file(_adapter *padapter, u8 *fwpath);
int rtw_halmac_deinit_hal(_adapter *padapter);
int rtw_halmac_dlfw(_adapter *padapter, u8 *fw, u32 fwsize);
int rtw_halmac_dlfw_from_file(_adapter *padapter, u8 *fwpath);
int rtw_halmac_phy_power_switch(_adapter *padapter, u8 enable);
int rtw_halmac_send_h2c(_adapter *padapter, u8 *h2c);
int rtw_halmac_c2h_handle(_adapter *padapter, u8 *c2h, u32 size);

int rtw_halmac_get_physical_efuse_size(_adapter *padapter, u32 *size);
int rtw_halmac_read_physical_efuse_map(_adapter *padapter, u8 *map, u32 size);
int rtw_halmac_get_logical_efuse_size(_adapter *padapter, u32 *size);
int rtw_halmac_read_logical_efuse_map(_adapter *padapter, u8 *map, u32 size);
int rtw_halmac_write_logical_efuse_map(_adapter *padapter, u8 *map, u32 size, u8 *maskmap, u32 masksize);
int rtw_halmac_read_logical_efuse(_adapter *padapter, u32 offset, u32 cnt, u8 *data);
int rtw_halmac_write_logical_efuse(_adapter *padapter, u32 offset, u32 cnt, u8 *data);

int rtw_halmac_config_rx_info(_adapter *padapter, HALMAC_DRV_INFO);
int rtw_halmac_set_mac_address(_adapter *padapter, enum _IFACE_TYPE, u8 *addr);
int rtw_halmac_set_bssid(_adapter *padapter, enum _IFACE_TYPE hwport, u8 *addr);

int rtw_halmac_set_bandwidth(_adapter *padapter, u8 channel, u8 pri_ch_idx, u8 bw);
int rtw_halmac_rx_agg_switch(_adapter *padapter, u8 enable);
int rtw_halmac_get_hw_value(_adapter *padapter, HALMAC_HW_ID hw_id, VOID *pvalue);
int rtw_halmac_get_wow_reason(_adapter *padapter, u8 *reason);
int rtw_halmac_get_drv_info_sz(_adapter *padapter, u8 *sz);
int rtw_halmac_get_rsvd_drv_pg_bndy(_adapter *padapter, u16 *drv_pg);
int rtw_halmac_fill_hal_spec(_adapter *padapter, struct hal_spec_t *);

u32 rtw_hal_power_on(_adapter *padapter);

#ifdef CONFIG_SUPPORT_TRX_SHARED
void dump_trx_share_mode(void *sel, _adapter *adapter);
#endif
#endif /* _HAL_HALMAC_H_ */
