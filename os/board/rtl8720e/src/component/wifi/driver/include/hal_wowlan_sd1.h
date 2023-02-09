#ifndef __HAL_WOWLAN_SD1_H__
#define __HAL_WOWLAN_SD1_H__

#include "HalVerDef.h"
#include "hal_pg.h"
#include "hal_intf.h"
#include "hal_phy.h"
#include "hal_phy_reg.h"
#if  defined(CONFIG_RTL8730A) || defined(CONFIG_RTL8730E)
#include "wififw_reg_rom.h"
#elif defined(CONFIG_RTL8720E)  || defined(CONFIG_RTL8721F)
#include "wififw_reg_ram.h"
#else
#include "hal_com_reg.h"
#endif
#include "hal_com_phycfg.h"
#ifdef RTW_HALMAC
#include "hal_com_c2h.h"
#endif

#include "hal_com_c2h.h"


#ifdef CONFIG_WOWLAN_SD1
typedef struct rtl_wow_pattern {
	u16	crc;
	u8	type;
	u32	mask[4];
} rtl_wow_pattern_t;

typedef enum _wowlan_subcode {
	WOWLAN_PATTERN_MATCH	= 1,
	WOWLAN_MAGIC_PACKET		= 2,
	WOWLAN_UNICAST			= 3,
	WOWLAN_SET_PATTERN		= 4,
	WOWLAN_DUMP_REG			= 5,
	WOWLAN_ENABLE			= 6,
	WOWLAN_DISABLE			= 7,
	WOWLAN_STATUS			= 8,
	WOWLAN_DEBUG_RELOAD_FW	= 9,
	WOWLAN_DEBUG_1			= 10,
	WOWLAN_DEBUG_2			= 11
} wowlan_subcode;

struct wowlan_ioctl_param {
	unsigned int subcode;
	unsigned int subcode_value;
	unsigned int wakeup_reason;
	unsigned int len;
	wowlan_pattern_t pattern;
};

#define REMOTE_INFO_CTRL_SET_VALD_EN(target, _value) \
	SET_BITS_TO_LE_4BYTE(target + 0, 0, 8, _value)
#define REMOTE_INFO_CTRL_SET_PTK_EN(target, _value) \
	SET_BITS_TO_LE_4BYTE(target + 1, 0, 1, _value)
#define REMOTE_INFO_CTRL_SET_GTK_EN(target, _value) \
	SET_BITS_TO_LE_4BYTE(target + 1, 1, 1, _value)
#define REMOTE_INFO_CTRL_SET_GTK_IDX(target, _value) \
	SET_BITS_TO_LE_4BYTE(target + 2, 0, 8, _value)

int rtw_hal_wow_set_pattern(_adapter *adapter, wowlan_pattern_t pattern);
void rtw_hal_wow_enable(_adapter *adapter);
void rtw_hal_wow_disable(_adapter *adapter);

#endif
void rtw_hal_set_fw_rsvd_page(_adapter *adapter, bool finished);

#ifdef CONFIG_LPS_PG

#define H2C_LPS_PG_INFO_LEN		2
#define H2C_LPSPG_LEN			16

#define SET_H2CCMD_LPSPG_SEC_CAM_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)/*SecurityCAM_En*/
#define SET_H2CCMD_LPSPG_MBID_CAM_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)/*BSSIDCAM_En*/
#define SET_H2CCMD_LPSPG_PMC_CAM_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 2, 1, __Value)/*PatternMatchCAM_En*/
#define SET_H2CCMD_LPSPG_MACID_SEARCH_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 3, 1, __Value)/*MACIDSearch_En*/
#define SET_H2CCMD_LPSPG_TXSC_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 4, 1, __Value)/*TXSC_En*/
#define SET_H2CCMD_LPSPG_MU_RATE_TB_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 5, 1, __Value)/*MURateTable_En*/
#define SET_H2CCMD_LPSPG_LOC(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd+1, 0, 8, __Value)/*Loc_LPS_PG*/

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

#ifdef CONFIG_WOWLAN_SSL_KEEP_ALIVE
#define SET_H2CCMD_MQTT_SSL_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 0, 1, __Value)/*SSL EN*/
#define SET_H2CCMD_MQTT_PATTERN_MATCH_EN(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd, 1, 1, __Value)/*PATTERN_MATCH_EN EN*/
#define SET_H2CCMD_MQTT_PUBLISH_WAKE(__pH2CCmd, __Value) SET_BITS_TO_LE_1BYTE(__pH2CCmd, 2, 1, __Value)/*PUBLISH_WAKE EN*/
#define SET_H2CCMD_TCP_SSL_MODE(__pH2CCmd, __Value) SET_BITS_TO_LE_1BYTE(__pH2CCmd, 3, 1, __Value)/*Header ignore*/
#define SET_H2CCMD_SSL_INFO_LOC(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd+1, 0, 8, __Value)/*SSL_INFO_LOC*/
#define SET_H2CCMD_SSL_PATTERN_LOC(__pH2CCmd, __Value)	SET_BITS_TO_LE_1BYTE(__pH2CCmd+2, 0, 8, __Value)/*SSL_PATTERN_LOC*/
#define SET_H2CCMD_TCP_PARM_SERVERTO_EN(__pH2CCmd, __Value) SET_BITS_TO_LE_1BYTE(__pH2CCmd, 4, 1, __Value)
#define SET_H2CCMD_TCP_PARM_SERVER_TIMEOUT_L(__pH2CCmd, __Value) SET_BITS_TO_LE_1BYTE(__pH2CCmd+3, 0, 8, __Value)
#define SET_H2CCMD_TCP_PARM_SERVER_TIMEOUT_H(__pH2CCmd, __Value) SET_BITS_TO_LE_1BYTE(__pH2CCmd+4, 0, 8, __Value)
#endif

#if defined(CONFIG_WOWLAN) || defined(CONFIG_AP_WOWLAN)
/* ARP element */
#define SET_ARP_PKT_HW(__pHeader, __Value)					WriteEF2Byte(((u8 *)(__pHeader)) + 0, __Value)
#define SET_ARP_PKT_PROTOCOL(__pHeader, __Value)			WriteEF2Byte(((u8 *)(__pHeader)) + 2, __Value)
#define SET_ARP_PKT_HW_ADDR_LEN(__pHeader, __Value)			WriteEF1Byte(((u8 *)(__pHeader)) + 4, __Value)
#define SET_ARP_PKT_PROTOCOL_ADDR_LEN(__pHeader, __Value)	WriteEF1Byte(((u8 *)(__pHeader)) + 5, __Value)
#define SET_ARP_PKT_OPERATION(__pHeader, __Value)			WriteEF2Byte(((u8 *)(__pHeader)) + 6, __Value)
#define SET_ARP_PKT_SENDER_MAC_ADDR(__pHeader, _val)	cp_mac_addr(((u8 *)(__pHeader))+8, (u8 *)(_val))
#define SET_ARP_PKT_SENDER_IP_ADDR(__pHeader, _val)		cpIpAddr(((u8 *)(__pHeader))+14, (u8 *)(_val))
#define SET_ARP_PKT_TARGET_MAC_ADDR(__pHeader, _val)	cp_mac_addr(((u8 *)(__pHeader))+18, (u8 *)(_val))
#define SET_ARP_PKT_TARGET_IP_ADDR(__pHeader, _val)		cpIpAddr(((u8 *)(__pHeader))+24, (u8 *)(_val))

#endif /* CONFIG_WOWLAN */


#endif //__HAL_WOWLAN_SD1_H__
