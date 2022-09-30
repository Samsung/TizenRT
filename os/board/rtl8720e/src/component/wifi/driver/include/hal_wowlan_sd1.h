#ifndef __HAL_WOWLAN_SD1_H__
#define __HAL_WOWLAN_SD1_H__

#include "HalVerDef.h"
#include "hal_pg.h"
#include "hal_intf.h"
#include "hal_phy.h"
#include "hal_phy_reg.h"
#if  defined(CONFIG_RTL8730A) || defined(CONFIG_RTL8730E)
#include "wififw_reg_rom.h"
#elif defined(CONFIG_RTL8720E)
#include "wififw_reg_ram.h"
#else
#include "hal_com_reg.h"
#endif
#include "hal_com_phycfg.h"
#ifdef RTW_HALMAC
#include "hal_com_c2h.h"
#endif

#if(PHYDM_VERSION > 1)
#include "hal_com_c2h.h"
#endif


#ifdef CONFIG_WOWLAN_SD1
typedef struct rtl_wow_pattern {
	u16	crc;
	u8	type;
	u32	mask[4];
} rtl_wow_pattern_t;

int rtw_hal_wow_set_pattern(_adapter *adapter, wowlan_pattern_t pattern);

void rtw_hal_wow_enable(_adapter *adapter);
void rtw_hal_wow_disable(_adapter *adapter);

#define REMOTE_INFO_CTRL_SET_VALD_EN(target, _value) \
	SET_BITS_TO_LE_4BYTE(target + 0, 0, 8, _value)
#define REMOTE_INFO_CTRL_SET_PTK_EN(target, _value) \
	SET_BITS_TO_LE_4BYTE(target + 1, 0, 1, _value)
#define REMOTE_INFO_CTRL_SET_GTK_EN(target, _value) \
	SET_BITS_TO_LE_4BYTE(target + 1, 1, 1, _value)
#define REMOTE_INFO_CTRL_SET_GTK_IDX(target, _value) \
	SET_BITS_TO_LE_4BYTE(target + 2, 0, 8, _value)

#endif
void rtw_hal_set_fw_rsvd_page(_adapter *adapter, bool finished);

#ifdef CONFIG_LPS_PG
#define LPSPG_RSVD_PAGE_SET_MACID(_rsvd_pag, _value)		SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x00, 0, 8, _value)/*used macid*/
#define LPSPG_RSVD_PAGE_SET_MBSSCAMID(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x00, 8, 8, _value)/*used BSSID CAM entry*/
#define LPSPG_RSVD_PAGE_SET_PMC_NUM(_rsvd_pag, _value)		SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x00, 16, 8, _value)/*Max used Pattern Match CAM entry*/
#define LPSPG_RSVD_PAGE_SET_MU_RAID_GID(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x00, 24, 8, _value)/*Max MU rate table Group ID*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_NUM(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x04, 0, 8, _value)/*used Security CAM entry number*/
#define LPSPG_RSVD_PAGE_SET_DRV_RSVDPAGE_NUM(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x04, 8, 8, _value)/*Txbuf used page number for fw offload*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID1(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x08, 0, 8, _value)/*used Security CAM entry -1*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID2(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x08, 8, 8, _value)/*used Security CAM entry -2*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID3(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x08, 16, 8, _value)/*used Security CAM entry -3*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID4(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x08, 24, 8, _value)/*used Security CAM entry -4*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID5(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x0C, 0, 8, _value)/*used Security CAM entry -5*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID6(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x0C, 8, 8, _value)/*used Security CAM entry -6*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID7(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x0C, 16, 8, _value)/*used Security CAM entry -7*/
#define LPSPG_RSVD_PAGE_SET_SEC_CAM_ID8(_rsvd_pag, _value)	SET_BITS_TO_LE_4BYTE(_rsvd_pag+0x0C, 24, 8, _value)/*used Security CAM entry -8*/
enum lps_pg_hdl_id {
	LPS_PG_INFO_CFG = 0,
	LPS_PG_REDLEMEM,
	LPS_PG_PHYDM_DIS,
	LPS_PG_PHYDM_EN,
};

u8 rtw_hal_set_lps_pg_info(_adapter *adapter);

#endif

#endif //__HAL_WOWLAN_SD1_H__