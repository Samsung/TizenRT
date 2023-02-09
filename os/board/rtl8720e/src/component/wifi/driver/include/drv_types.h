/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
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
/*-------------------------------------------------------------------------------

	For type defines and data structure defines

--------------------------------------------------------------------------------*/


#ifndef __DRV_TYPES_H__
#define __DRV_TYPES_H__

#include <drv_conf.h>

#if defined (__ICCARM__)
#define _PACKED         __packed
#define _WEAK           __weak
#else
#define _PACKED         __attribute__ ((packed))
#define _WEAK           __attribute__ ((weak))
#endif

// Assign memory sectinon usage
#if defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBADPLUS)
#include <section_config.h>
//#include "rtl_utility_ram.h"
#include "platform_stdlib.h"
#else
#define SRAM_BD_DATA_SECTION
#define WLAN_ROM_TEXT_SECTION
#define WLAN_ROM_DATA_SECTION
#define _LONG_CALL_
#endif

#define CONFIG_DOSCAN_IN_BUSYTRAFFIC
//#define CONFIG_SPCT_CH_SWITCH

enum _NIC_VERSION {

	RTL8711_NIC,
	RTL8712_NIC,
	RTL8713_NIC,
	RTL8716_NIC

};

/* Add for halbb, It may be wrong if g6 driver add other type for BIT6, */
enum rtw_hci_type {
	RTW_HCI_PCIE = (1U << 0),
	RTW_HCI_USB = (1U << 1),
	RTW_HCI_SDIO = (1U << 2),
	RTW_HCI_GSPI = (1U << 3),
	RTW_HCI_AXI = (1U << 6),
	RTW_HCI_MAX,
};

typedef struct _ADAPTER _adapter, ADAPTER, *PADAPTER;

#include "wireless.h"
#include <netdev.h>
#include <osdep_service.h>
#include <device_lock.h>
#include <rtw_byteorder.h>
#include <rtw_io.h>
#include <cmn_info/rtw_sta_info.h>
#include <wlan_bssdef.h>
#include <wifi.h>
//#include "p2p/p2p.h"
#include <rtw_ht.h>

#ifdef CONFIG_80211AX_HE
#include <rtw_he.h>
#endif

#ifdef CONFIG_TWT
#include <rtw_twt.h>
#endif

#include <wlan_basic_types.h>
#include <rtw_intfs.h>
//#include <hal_pg.h>
#include <rtw_cmd.h>
#include <rtw_xmit.h>
#include <rtw_recv.h>
#include <rtw_rm.h>
#include <phl_def.h>
#include <phl.h>
#include <hal_com.h>

#ifdef CONFIG_80211AC_VHT
#include <rtw_vht.h>
#endif

#ifdef RTW_HALMAC
#include <hal_com_h2c.h>
#endif
#include <rtw_qos.h>

#include <rtw_psk.h>

#ifdef CONFIG_SAE_SUPPORT
#include "rtw_sae.h"
#endif
#include "rtw_pmksa_cache.h"
#include <rtw_security.h>
#include <rom_rtw_security.h>
#include <rtw_pwrctrl.h>
#include <rtw_pwrctrl_sd1.h>
#include <sta_info.h>
#include <rtw_mlme.h>
#include <rtw_layer2_rm.h>
#include <rtw_debug.h>
#include <ieee80211.h>
#include <rom_ieee80211.h>
#include <rtw_ioctl_set.h>
#include <rtw_rf.h>
#include <rtw_event.h>
#include <rtw_mlme_ext.h>
#include <rtw_80211frame.h>
#include <rtw_dfs.h>
#include <wifi_performance_monitor.h>
//#include <rtw_tdls.h>
#include <rtw_ap.h>

#include <osdep_intf.h>

#include <hci_intfs.h>
#ifdef CONFIG_HARDWARE_8192E
#include <wifi_conf.h>
#endif

#ifdef CONFIG_MP_INCLUDED
#include <rtw_mp.h>
#endif

#ifdef CONFIG_BT_COEXIST
#include <btc_api.h>
#endif

#ifdef CONFIG_BT_COEXIST_SOC
#include <rtw_btcoex_soc.h>
#endif


#ifdef CONFIG_IEEE80211R
#include <rtw_ft.h>
#endif
#if defined(CONFIG_RTW_WNM) || defined(CONFIG_IEEE80211K)
#include <rtw_wnm.h>
#endif

#define CHANNEL_MAX_NUMBER						14	// 14 is the max channel number
#define CHANNEL_MAX_NUMBER_2G					14
#define CHANNEL_MAX_NUMBER_5G					54	// Please refer to "phy_GetChnlGroup8812A" and "Hal_ReadTxPowerInfo8812A"
#define CHANNEL_MAX_NUMBER_5G_80M				7

// Tx Power Limit Table Size
#define MAX_REGULATION_NUM						TXPWR_LMT_MAX_REGULATION_NUM
#define MAX_2_4G_BANDWITH_NUM					2 // 20M, 40M
#if defined(NOT_SUPPORT_RF_MULTIPATH) && (!defined(CONFIG_80211AC_VHT))
#define MAX_RATE_SECTION_NUM					3 // CCk, OFDM, HT
#define MAX_BASE_NUM_IN_PHY_REG_PG_2_4G			3 //  CCK:1,OFDM:1, HT:1(MCS0_MCS7)
#else
#define MAX_RATE_SECTION_NUM					10
#define MAX_BASE_NUM_IN_PHY_REG_PG_2_4G			10 //  CCK:1,OFDM:1, HT:4, VHT:4
#endif
#define MAX_5G_BANDWITH_NUM						4
#define MAX_BASE_NUM_IN_PHY_REG_PG_5G			9 // OFDM:1, HT:4, VHT:4

#define SPEC_DEV_ID_NONE		BIT(0)
#define SPEC_DEV_ID_DISABLE_HT		BIT(1)
#define SPEC_DEV_ID_ENABLE_PS		BIT(2)
#define SPEC_DEV_ID_RF_CONFIG_1T1R	BIT(3)
#define SPEC_DEV_ID_RF_CONFIG_2T2R	BIT(4)
#define SPEC_DEV_ID_ASSIGN_IFNAME	BIT(5)

#define BUFLEN(a)			strnlen(a,sizeof(a))

typedef unsigned char			BOOLEAN, *PBOOLEAN;
typedef unsigned char			u1Byte, *pu1Byte;
typedef unsigned short			u2Byte, *pu2Byte;
typedef unsigned int			u4Byte, *pu4Byte;
typedef unsigned long long		u8Byte, *pu8Byte;
typedef signed char			s1Byte, *ps1Byte;

struct specific_device_id {
	u32		flags;
	u16		idVendor;
	u16		idProduct;

};

#define BSSID_OFT(field) ((u32)FIELD_OFFSET(WLAN_BSSID_EX,field))
#define BSSID_SZ(field)   sizeof(((PWLAN_BSSID_EX) 0)->field)

#define BW_MODE_2G(bw_mode) ((bw_mode) & 0x0F)
#define BW_MODE_5G(bw_mode) ((bw_mode) >> 4)
#define REGSTY_BW_2G(bw_mode) BW_MODE_2G(bw_mode)
#define REGSTY_BW_5G(bw_mode) BW_MODE_5G(bw_mode)

#define REGSTY_IS_BW_2G_SUPPORT(bw_mode, bw) (REGSTY_BW_2G((bw_mode)) >= (bw))
#define REGSTY_IS_BW_5G_SUPPORT(bw_mode, bw) (REGSTY_BW_5G((bw_mode)) >= (bw))

#define MAX_CONTINUAL_URB_ERR 4

enum _IFACE_ID {
	IFACE_ID0, /*PRIMARY_ADAPTER*/
	IFACE_ID1,
	IFACE_ID2,
	IFACE_ID3,
	IFACE_ID4,
	IFACE_ID5,
	IFACE_ID6,
	IFACE_ID7,
	IFACE_ID_MAX,
};

enum _IFACE_TYPE {
	IFACE_PORT0, //mapping to port0 for C/D series chips
	IFACE_PORT1, //mapping to port1 for C/D series chip
	MAX_IFACE_PORT,
};

enum _hw_port {
	HW_PORT0,
	HW_PORT1,
	HW_PORT2,
	HW_PORT3,
	HW_PORT4,
	MAX_HW_PORT,
};

enum _ADAPTER_TYPE {
	PRIMARY_ADAPTER,
	SECONDARY_ADAPTER,
	MAX_ADAPTER,
};

struct rf_ctl_t {
	const struct country_chplan *country_ent;
	u8 ChannelPlan;
	u8 max_chan_nums;
	//RT_CHANNEL_INFO channel_set[MAX_CHANNEL_NUM];
	//struct p2p_channels channel_list;

	_mutex offch_mutex;
	u8 offch_state;

	/* used for debug or by tx power limit */
	u16 rate_bmp_cck_ofdm;		/* 20MHz */
	u32 rate_bmp_ht_by_bw[2];	/* 20MHz, 40MHz. 4SS supported */
	u32 rate_bmp_vht_by_bw[4];	/* 20MHz, 40MHz, 80MHz, 160MHz. up to 3SS supported */

	/* used by tx power limit */
	u8 highest_ht_rate_bw_bmp;
	u8 highest_vht_rate_bw_bmp;

#ifdef CONFIG_TXPWR_LIMIT
	_mutex txpwr_lmt_mutex;
	_list reg_exc_list;
	u8 regd_exc_num;
	_list txpwr_lmt_list;
	u8 txpwr_regd_num;
	const char *regd_name;

	u8 txpwr_lmt_2g_cck_ofdm_state;
#ifdef CONFIG_IEEE80211_BAND_5GHZ
	u8 txpwr_lmt_5g_cck_ofdm_state;
	u8 txpwr_lmt_5g_20_40_ref;
#endif
#endif

	u8 ch_sel_same_band_prefer;

#ifdef CONFIG_DFS
	u8 csa_ch;
#endif
};

#define RTW_CAC_STOPPED 0

#define IS_CAC_STOPPED(rfctl) 1
#define IS_CH_WAITING(rfctl) 0
#define IS_UNDER_CAC(rfctl) 0
#define IS_RADAR_DETECTED(rfctl) 0


#ifdef CONFIG_DFS_SLAVE_WITH_RADAR_DETECT
#define IS_DFS_SLAVE_WITH_RD(rfctl) ((rfctl)->dfs_slave_with_rd)
#else
#define IS_DFS_SLAVE_WITH_RD(rfctl) 0
#endif

#define SEC_CAP_CHK_BMC	BIT0

#define RTW_DEFAULT_MGMT_MACID 1

struct macid_bmp {
	u32 m0;
#if (MACID_NUM_SW_LIMIT > 32)
	u32 m1;
#endif
#if (MACID_NUM_SW_LIMIT > 64)
	u32 m2;
#endif
#if (MACID_NUM_SW_LIMIT > 96)
	u32 m3;
#endif
};

#define rfctl_to_share_adapter(rfctl) container_of((rfctl), struct share_adapter, rf_ctl)

typedef enum _DRIVER_STATE {
	DRIVER_NORMAL = 0,
	DRIVER_DISAPPEAR = 1,
	DRIVER_REPLACE_DONGLE = 2,
} DRIVER_STATE;

struct tsf_info {
	u8 sync_port;/*tsf sync from portx*/
	u8 offset; /*tsf timer offset*/
};

struct wps_priv {
#ifdef CONFIG_WPS
	u8 *wps_probe_req_ie;
	u16 wps_probe_req_ie_len;
	u8 *wps_assoc_req_ie;
	u16 wps_assoc_req_ie_len;

	u8 *wps_beacon_ie;
	u8 *wps_probe_resp_ie;
	u8 *wps_assoc_resp_ie; // for CONFIG_IOCTL_CFG80211, this IE could include p2p ie

	u16 wps_beacon_ie_len;
	u16 wps_probe_resp_ie_len;
	u16 wps_assoc_resp_ie_len;
#endif
};

struct halphy_data_t {
#if (PHYDM_VERSION < 3)
	s8			TxPwrByRateOffset[TX_PWR_BY_RATE_NUM_BAND]
	[TX_PWR_BY_RATE_NUM_RF]
	[TX_PWR_BY_RATE_NUM_RF]
	[TX_PWR_BY_RATE_NUM_RATE];
	u8			Index24G_CCK_Base[MAX_RF_PATH][CHANNEL_MAX_NUMBER];
	u8			Index24G_BW40_Base[MAX_RF_PATH][CHANNEL_MAX_NUMBER];
	//If only one tx, only BW20 and OFDM are used.
	s8			OFDM_24G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8			BW20_24G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#if !defined(NOT_SUPPORT_RF_MULTIPATH)
	s8			CCK_24G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8			BW40_24G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#endif
	/* 5G TX power info for target TX power*/
#if defined(SUPPORT_5G_CHANNEL)
	u8			Index5G_BW40_Base[MAX_RF_PATH][CENTER_CH_5G_ALL_NUM];
#if !defined(NOT_SUPPORT_80M)
	u8			Index5G_BW80_Base[MAX_RF_PATH][CENTER_CH_5G_80M_NUM];
#endif
	s8			OFDM_5G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8			BW20_5G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
	s8			BW40_5G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#if !defined(NOT_SUPPORT_80M)
	s8			BW80_5G_Diff[MAX_RF_PATH][MAX_TX_COUNT];
#endif
#endif
	/* 2 Power Limit Table */
	/* Power Limit Table for 2.4G */
	s8			TxPwrLimit_2_4G[MAX_REGULATION_NUM]
	[MAX_2_4G_BANDWITH_NUM]
	[MAX_RATE_SECTION_NUM]
	[CHANNEL_MAX_NUMBER_2G]
	[MAX_RF_PATH];
#if defined(SUPPORT_5G_CHANNEL)
	/* Power Limit Table for 5G */
	s8			TxPwrLimit_5G[MAX_REGULATION_NUM]
	[MAX_5G_BANDWITH_NUM]
	[MAX_RATE_SECTION_NUM]
	[CHANNEL_MAX_NUMBER_5G]
	[MAX_RF_PATH];
#endif
	/* Store the original power by rate value of the base of each rate section of rf path A & B */
	u8			TxPwrByRateBase2_4G[TX_PWR_BY_RATE_NUM_RF]
	[TX_PWR_BY_RATE_NUM_RF]
	[MAX_BASE_NUM_IN_PHY_REG_PG_2_4G];
#if defined(SUPPORT_5G_CHANNEL)
	u8			TxPwrByRateBase5G[TX_PWR_BY_RATE_NUM_RF]
	[TX_PWR_BY_RATE_NUM_RF]
	[MAX_BASE_NUM_IN_PHY_REG_PG_5G];
#endif
#endif // (PHYDM_VERSION < 3)
};

struct share_adapter {
	_adapter			*primary_adapter;
	_adapter			*second_adapter;
	u8				iface_nums;		/* total number of ifaces used runtime */
	struct recv_priv		recvpriv;
	struct xmit_priv		xmitpriv;
	struct rf_ctl_t			rf_ctl;
	struct ap_priv			appriv;
	struct pwrctrl_priv		pwrctrlpriv;
	struct mlme_ext_priv		mlmeextpriv;
	struct mlme_priv		mlmepriv;
	struct rtw_phl_com_t		phl_com;
	struct wps_priv			wpspriv;
	struct pwrctrl_priv_sd1		pwrctrlsd1_priv;
	struct rm_priv			*rmpriv;
	struct share_security_priv	share_securitypriv;
#ifdef CONFIG_PROMISC
	struct promisc_priv		promiscpriv;
#endif // CONFIG_PROMISC
#ifdef CONFIG_MP_INCLUDED
	struct	mp_priv			mppriv;
#endif
	PVOID				HalData;
	unsigned char			oper_channel;		/* saved channel info when call set_channel_bw */
	unsigned char			oper_bwmode;
	unsigned char			oper_ch_offset;		/* PRIME_CHNL_OFFSET */
	systime				on_oper_ch_time;
#ifdef RTW_HALMAC
	void				*halmac;
#endif
	u8				irq_alloc;
	u8				irq_enabled;
	_lock				irq_th_lock;
	struct task_struct		single_thread;
	struct task_struct		little_stack_thread;
#ifdef CONFIG_BT_COEXIST_SOC
	_queue				btmailboxQueue;
	struct coex_priv		*coexpriv;
	struct task_struct		btmailboxThread;
#endif
#if defined (CONFIG_WOWLAN) && defined (CONFIG_FW_DRIVER_COEXIST) && CONFIG_FW_DRIVER_COEXIST
	struct task_struct		driver_resumeThread;
#endif
};

struct _ADAPTER {
	struct share_adapter		*pshare_adapter;
	_adapter			*pbuddy_adapter;

	u32				work_mode;	//STA, AP, STA+AP, PROMISC, P2P
	struct mlme_ext_info		mlmext_info;	//for sta/adhoc mode, including current scanning/connecting/connected related info.
	struct cmd_priv			cmdpriv;
	struct sta_priv			stapriv;
	struct security_priv		securitypriv;
	struct qos_priv			qospriv;
	struct ht_priv			htpriv;
#ifdef CONFIG_80211AC_VHT
	struct vht_priv			vhtpriv;
#endif
#ifdef CONFIG_80211AX_HE
	struct he_priv			hepriv;
#endif
	struct halphy_data_t		halphy_data;
	struct wlan_network		cur_network;
	s32				bDriverStopped;
	u8				RxStop;	//Used to stop rx thread as early as possible
	u8				hw_init_completed;
	u8				bMpDriver;
	_nic_hdl			pnetdev;
	int				bup;
	struct net_device_stats		stats;
	u8				net_closed;
	sint				fw_state;	//shall we protect this variable? maybe not necessarily...
	u8				bFWReady;
	u8				iface_type; //interface port type
	/*extend to support multi interface*/
	u8				iface_id;
#if defined(CONFIG_CONCURRENT_MODE)
	u8				isprimary; //is primary padapter or not
	u8				adapter_type;
#if defined(CONFIG_RTL8735B)
	struct tsf_info			tsf;
#endif
#endif
	u16				fix_rate;
	u8				mac_addr[6];
	u8				fix_vcs;
	u8				driver_tx_bw_mode;
	struct hw_xmit			pending_queue;
};

#define intf_stop hci_intf_stop

/* Get elementes from padapter. */
#ifdef CONFIG_CONCURRENT_MODE
#define is_primary_adapter(padapter)		(padapter->adapter_type == PRIMARY_ADAPTER)
#define get_iface_type(padapter)		(padapter->iface_type)
#define get_hw_port(padapter)			(padapter->hw_port)
#else
#define is_primary_adapter(padapter)		(1)
#define get_iface_type(padapter)		(IFACE_PORT0)
#define get_hw_port(padapter)			(HW_PORT0)
#endif
#define rtw_get_stapriv(padapter)		(&(padapter->stapriv))
#define rtw_get_securepriv(padapter)		(&(padapter->securitypriv))
#define rtw_get_pmlmeinfo(padapter)		(&(padapter->mlmext_info))
#define rtw_get_qospriv(padapter)		(&(padapter->qospriv))
#define rtw_get_htpriv(padapter)		(&(padapter->htpriv))
#define rtw_get_vhtpriv(padapter)		(&(padapter->vhtpriv))
#define rtw_get_hepriv(padapter)		(&(padapter->hepriv))
#define rtw_get_cur_network(padapter)		(&(padapter->cur_network))
#define rtw_get_bssid(padapter)			(padapter->cur_network.network.MacAddress)
#define rtw_get_stats(padapter)			(&(padapter->stats))
#define rtw_get_pendingqueue(padapter)		(&(padapter->pending_queue))
#define rtw_get_cmdpriv(padapter)		(&(padapter->cmdpriv))
#define rtw_get_halphydata(padapter)		(&(padapter->halphy_data))
#define adapter_mac_addr(padapter)		(padapter->mac_addr)

/* Get elements from pshare_adapter. */
#define rtw_get_pshare(padapter)		((padapter)->pshare_adapter)
#define rtw_get_adapter(index, padapter)	((index == 0) ? ((_adapter *)padapter)->pshare_adapter->primary_adapter : ((_adapter *)padapter)->pshare_adapter->second_adapter)
#define rtw_get_xmitpriv(padapter)		(&(padapter->pshare_adapter->xmitpriv))
#define rtw_get_recvpriv(padapter)		(&(padapter->pshare_adapter->recvpriv))
#define rtw_get_mlmepriv(padapter)		(&(padapter->pshare_adapter->mlmepriv))
#define rtw_get_mlmeextpriv(padapter)		(&(padapter->pshare_adapter->mlmeextpriv))
#define rtw_get_appriv(padapter)		(&(padapter->pshare_adapter->appriv))
#define rtw_get_sharesecpriv(padapter)		(&(padapter->pshare_adapter->share_securitypriv))
#define rtw_get_singlethread(padapter)		(&(padapter->pshare_adapter->single_thread))
#define rtw_get_littlethread(padapter)		(&(padapter->pshare_adapter->little_stack_thread))
#define adapter_to_pwrctl_sd1(padapter)		(&(padapter->pshare_adapter->pwrctrlsd1_priv))
#define adapter_to_chset(padapter)		(padapter->pshare_adapter->mlmeextpriv.channel_set)
#if defined (CONFIG_WOWLAN) && defined (CONFIG_FW_DRIVER_COEXIST) && CONFIG_FW_DRIVER_COEXIST
#define rtw_get_driverresumethread(padapter)	(&(padapter->pshare_adapter->driver_resumeThread))
#endif
#ifdef CONFIG_BT_COEXIST_SOC
#define rtw_get_btmailboxthread(padapter)	(&(padapter->pshare_adapter->btmailboxThread))
#define rtw_get_coexpriv(padapter)		(padapter->pshare_adapter->coexpriv)
#define rtw_get_btmailboxqueue(padapter)	(&(padapter->pshare_adapter->btmailboxQueue))
#endif
#define rtw_get_wpspriv(padapter)		(&(padapter->pshare_adapter->wpspriv))
#define rtw_get_rmpriv(padapter)		(padapter->pshare_adapter->rmpriv)
#define rtw_set_rmpriv(padapter, value)		(padapter->pshare_adapter->rmpriv = value)
#ifdef CONFIG_PROMISC
#define rtw_get_promisc(padapter)		(&(padapter->pshare_adapter->promiscpriv))
#endif
#define rtw_get_pwrctrlpriv(padapter)		(&(padapter->pshare_adapter->pwrctrlpriv))
#define rtw_get_haldata(padapter)		((HAL_DATA_TYPE *)((padapter)->pshare_adapter->HalData))
#define rtw_set_haldata(padapter, hal_info)	(padapter->pshare_adapter->HalData = hal_info)
#define rtw_get_phlcom(padapter)		(&(padapter->pshare_adapter->phl_com))
#ifdef CONFIG_MP_INCLUDED
#define rtw_get_mppriv(padapter)		(&(padapter->pshare_adapter->mppriv))
#endif
#define adapter_to_rfctl(padapter)		(&(rtw_get_pshare(padapter)->rf_ctl))
#define rtw_get_intf_type(padapter)		(rtw_get_haldata(padapter)->interface_type)
#define adapter_to_phydm(padapter)		(&(rtw_get_haldata(padapter)->odmpriv))
#define adapter_to_csipriv(padapter)		(&(rtw_get_haldata(padapter)->csipriv))

/* Get special elements. */
#define rtw_set_signal_stat_timer(padapter)	rtw_set_timer(&(padapter->stats.signal_stat_timer), SIG_SAMP_INTERVAL)
#define rtw_get_pnetwork(pcur_network)		(&(((struct wlan_network *)pcur_network)->network))
#define get_my_bssid(pnetwork)			(((WLAN_BSSID_EX *)pnetwork)->MacAddress)
#define CHECK_STA_IS_LINKED(padapter)		((((padapter->fw_state) & (WIFI_STATION_STATE | _FW_LINKED)) == (WIFI_STATION_STATE | _FW_LINKED)) ? 1: 0)
#define CHECK_STA_UNDER_LINKING(padapter)	((((padapter->fw_state) & (WIFI_STATION_STATE | _FW_UNDER_LINKING)) == (WIFI_STATION_STATE | _FW_UNDER_LINKING)) ? 1: 0)
#define CHECK_AP_HAS_STA_LINKED(padapter)	(((&padapter->stapriv)->asoc_sta_count > 2) ? (((&padapter->stapriv)->asoc_sta_count) - 2) : 0)

extern struct wifi_user_conf			wifi_user_config;

__inline static sint check_fwstate(_adapter *padapter, sint state)
{
	if (padapter->fw_state & state) {
		return _TRUE;
	} else if ((state == WIFI_FW_NULL_STATE) &&
			   (padapter->fw_state == WIFI_FW_NULL_STATE)) {
		return _TRUE;
	}
	return _FALSE;
}

__inline static sint get_fwstate(_adapter *padapter)
{
	return padapter->fw_state;
}

/*
 * No Limit on the calling context,
 * therefore set it to be the critical section...
 *
 * ### NOTE:#### (!!!!)
 * MUST TAKE CARE THAT BEFORE CALLING THIS FUNC, YOU SHOULD HAVE LOCKED pmlmepriv->lock
 */
__inline static void set_fwstate(_adapter *padapter, sint state)
{
	struct mlme_priv *pmlmepriv = rtw_get_mlmepriv(padapter);
	padapter->fw_state |= state;
	//FOR HW integration
	if (_FW_UNDER_SURVEY == state) {
		pmlmepriv->bScanInProcess = _TRUE;
	}
}

__inline static void _clr_fwstate_(_adapter *padapter, sint state)
{
	struct mlme_priv *pmlmepriv = rtw_get_mlmepriv(padapter);

	padapter->fw_state &= ~state;
	//FOR HW integration
	if (_FW_UNDER_SURVEY == state) {
		pmlmepriv->bScanInProcess = _FALSE;
	}
}

__inline static void clr_fwstate(_adapter *padapter, sint state)
{
	_irqL irqL;
	struct mlme_priv *pmlmepriv = rtw_get_mlmepriv(padapter);

	rtw_enter_critical_bh(&pmlmepriv->lock, &irqL);
	if (check_fwstate(padapter, state) == _TRUE) {
		padapter->fw_state ^= state;
	}
	rtw_exit_critical_bh(&pmlmepriv->lock, &irqL);
}

#if 0 //#if (CONFIG_LWIP_LAYER == 0)
// For FPGA test program
#define _htons(x) (x)
#define _htons(x) (x)
#define _htons(x) (x)
#define _htons(x) (x)
#endif

#endif //__DRV_TYPES_H__

