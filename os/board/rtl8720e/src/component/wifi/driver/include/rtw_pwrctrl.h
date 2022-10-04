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
#ifndef __RTW_PWRCTRL_H_
#define __RTW_PWRCTRL_H_

#define FW_PWR0		0
#define FW_PWR1 	1
#define FW_PWR2 	2
#define FW_PWR3 	3


#define HW_PWR0		7
#define HW_PWR1 	6
#define HW_PWR2 	2
#define HW_PWR3		0
#define HW_PWR4		8

#define FW_PWRMSK	0x7


#define XMIT_ALIVE	BIT(0)
#define RECV_ALIVE	BIT(1)
#define CMD_ALIVE	BIT(2)
#define EVT_ALIVE	BIT(3)
#ifdef CONFIG_BT_COEXIST
#define BTCOEX_ALIVE	BIT(4)
#endif /* CONFIG_BT_COEXIST */

#ifdef CONFIG_WOWLAN
#define DEFAULT_PATTERN_NUM 1  		/* ICMP */
#define MAX_CUSTOM_PATTERN_NUM 20
#define MAX_WKFM_CAM_NUM	22		/* Frame Mask Cam number for pattern match */
#define MAX_WKFM_SIZE		16 /* (16 bytes for WKFM bit mask, 16*8 = 128 bits) */
#define MAX_WKFM_PATTERN_SIZE	128
#define WKFMCAM_ADDR_NUM 8//6
//#define WKFMCAM_SIZE 		24 /* each entry need 6*4 bytes */

enum pattern_type {
	PATTERN_BROADCAST = 0,
	PATTERN_MULTICAST,
	PATTERN_UNICAST,
	PATTERN_VALID,
	PATTERN_INVALID,
};

typedef struct rtl_priv_pattern {
	int len;
	char content[MAX_WKFM_PATTERN_SIZE];
	char mask[MAX_WKFM_SIZE];
} rtl_priv_pattern_t;

#endif // CONFIG_WOWLAN

#define LPS_TX_TP_TH	3000	//Kbps
#define LPS_RX_TP_TH	3000	//Kbps
#define LPS_BI_TP_TH	5000	//Kbps
#define LPS_TP_CHK_CNT	5

/*
	BIT[2:0] = HW state
	BIT[3] = Protocol PS state,   0: register active state , 1: register sleep state
	BIT[4] = sub-state
*/

#define PS_DPS				BIT(0)
#define PS_LCLK				(PS_DPS)
#define PS_RF_OFF			BIT(1)
#define PS_ALL_ON			BIT(2)
#define PS_ST_ACTIVE		BIT(3)

#define PS_ISR_ENABLE		BIT(4)
#define PS_IMR_ENABLE		BIT(5)
#define PS_ACK				BIT(6)
#define PS_TOGGLE			BIT(7)

#define PS_STATE_MASK		(0x0F)
#define PS_STATE_HW_MASK	(0x07)
#define PS_SEQ_MASK			(0xc0)

#define PS_STATE(x)		(PS_STATE_MASK & (x))
#define PS_STATE_HW(x)	(PS_STATE_HW_MASK & (x))
#define PS_SEQ(x)		(PS_SEQ_MASK & (x))

#define PS_STATE_S0		(PS_DPS)
#define PS_STATE_S1		(PS_LCLK)
#define PS_STATE_S2		(PS_RF_OFF)
#define PS_STATE_S3		(PS_ALL_ON)
#define PS_STATE_S4		((PS_ST_ACTIVE) | (PS_ALL_ON))


#define PS_IS_RF_ON(x)	((x) & (PS_ALL_ON))
#define PS_IS_ACTIVE(x)	((x) & (PS_ST_ACTIVE))
#define CLR_PS_STATE(x)	((x) = ((x) & (0xF0)))


struct reportpwrstate_parm {
	unsigned char mode;
	unsigned char state; //the CPWM value
	unsigned short rsvd;
};


typedef _sema _pwrlock;


__inline static void _init_pwrlock(_pwrlock *plock)
{
	rtw_init_sema(plock, 1);
}

__inline static void _free_pwrlock(_pwrlock *plock)
{
	rtw_free_sema(plock);
}


__inline static void _enter_pwrlock(_pwrlock *plock)
{
	rtw_down_sema(plock);
}


__inline static void _exit_pwrlock(_pwrlock *plock)
{
	rtw_up_sema(plock);
}

#define LPS_DELAY_TIME	1 // 1 sec

#define EXE_PWR_NONE	0x01
#define EXE_PWR_IPS		0x02
#define EXE_PWR_LPS		0x04

// RF state.
typedef enum _rt_rf_power_state {
	rf_on,		// RF is on after RFSleep or RFOff
	rf_sleep,	// 802.11 Power Save mode
	rf_off,		// HW/SW Radio OFF or Inactive Power Save
	//=====Add the new RF state above this line=====//
	rf_max
} rt_rf_power_state;

// RF Off Level for IPS or HW/SW radio off
#define	RT_RF_OFF_LEVL_ASPM			BIT(0)	// PCI ASPM
#define	RT_RF_OFF_LEVL_CLK_REQ		BIT(1)	// PCI clock request
#define	RT_RF_OFF_LEVL_PCI_D3			BIT(2)	// PCI D3 mode
#define	RT_RF_OFF_LEVL_HALT_NIC		BIT(3)	// NIC halt, re-initialize hw parameters
#define	RT_RF_OFF_LEVL_FREE_FW		BIT(4)	// FW free, re-download the FW
#define	RT_RF_OFF_LEVL_FW_32K		BIT(5)	// FW in 32k
#define	RT_RF_PS_LEVEL_ALWAYS_ASPM	BIT(6)	// Always enable ASPM and Clock Req in initialization.
#define	RT_RF_LPS_DISALBE_2R			BIT(30)	// When LPS is on, disable 2R if no packet is received or transmittd.
#define	RT_RF_LPS_LEVEL_ASPM			BIT(31)	// LPS with ASPM

#define	RT_IN_PS_LEVEL(ppsc, _PS_FLAG)		((ppsc->cur_ps_level & _PS_FLAG) ? _TRUE : _FALSE)
#define	RT_CLEAR_PS_LEVEL(ppsc, _PS_FLAG)	(ppsc->cur_ps_level &= (~(_PS_FLAG)))
#define	RT_SET_PS_LEVEL(ppsc, _PS_FLAG)		(ppsc->cur_ps_level |= _PS_FLAG)


enum _PS_BBRegBackup_ {
	PSBBREG_RF0 = 0,
	PSBBREG_RF1,
	PSBBREG_RF2,
	PSBBREG_AFE0,
	PSBBREG_TOTALCNT
};

struct pwrctrl_priv {
	_pwrlock	lock;
	volatile u8 rpwm; // requested power state for fw
	volatile u8 cpwm; // fw current power state. updated when 1. read from HCPWM 2. driver lowers power level
	volatile u8 tog; // toggling
	volatile u8 cpwm_tog; // toggling

	u8	pwr_mode;
	u8	bcn_ant_mode;
	u8	dtim;
	u32	alives;
	u64  wowlan_fw_iv;
//TODO
//	_workitem cpwm_event;
#ifdef CONFIG_LPS_RPWM_TIMER
	u8 brpwmtimeout;
	_workitem rpwmtimeoutwi;
	_timer pwr_rpwm_timer;
#endif // CONFIG_LPS_RPWM_TIMER
	u8	bpower_saving;
	u8	bTDMA;

	u8	b_hw_radio_off;
	u8	reg_rfoff;
	u8	reg_pdnmode; //powerdown mode
	u32	rfoff_reason;

	//RF OFF Level
	u32	cur_ps_level;
	u32	reg_rfps_level;



#if defined(CONFIG_PCI_HCI) || defined(CONFIG_LX_HCI)
	//just for PCIE ASPM
	u8	b_support_aspm; // If it supports ASPM, Offset[560h] = 0x40, otherwise Offset[560h] = 0x00.
	u8	b_support_backdoor;

	//just for PCIE ASPM
	u8	const_amdpci_aspm;
#endif

	uint 	ips_enter_cnts;
	uint 	ips_leave_cnts;

	u8	ps_enable;
	u8	ips_mode;
	u8	ips_org_mode;
	u8	ips_mode_req; // used to accept the mode setting request, will update to ipsmode later
	uint bips_processing;
	u32 ips_deny_time; /* will deny IPS when system time is smaller than this */
	u8 ps_processing; /* temporarily used to mark whether in rtw_ps_processor */

	u8	bLeisurePs;
	u8	LpsIdleCount;
	u8	power_mgnt;
	u8	org_power_mgnt;
	u8	bFwCurrentInPSMode;
	u32	DelayLPSLastTimeStamp;
	u8 	btcoex_rfon;
	s32		pnp_current_pwr_state;
	u8		pnp_bstop_trx;

	u8		bInSuspend;
#ifdef	CONFIG_BT_COEXIST
	u8		bAutoResume;
	u8		autopm_cnt;
#endif
	u8		bSupportRemoteWakeup;
#ifdef CONFIG_WOWLAN
	u8      wowlan_txpause_status;
	u8		wowlan_mode;
#if defined(CONFIG_WOWLAN_HW_CAM)
	u8		wowlan_pattern;
	u8		wowlan_magic;
	u8		wowlan_unicast;
	bool	default_patterns_en;
	_mutex	wowlan_pattern_cam_mutex;
	u8		wowlan_pattern_idx;
	struct rtl_priv_pattern	patterns[MAX_WKFM_CAM_NUM];
#endif
	u8		wowlan_wake_reason;
#ifdef CONFIG_WOWLAN_CUSTOM_PATTERN
	wowlan_pattern_t	wowlan_custom_pattern[MAX_CUSTOM_PATTERN_NUM];
	u8		wowlan_custom_pattern_count;
#endif
	u8		bLowPwrRxBCN;
	u8		bLowPwrRxBCNOfdmOff;
#endif // CONFIG_WOWLAN
	_timer 	pwr_state_check_timer;
	int		pwr_state_check_interval;
	u8		pwr_state_check_cnts;

	int 		ps_flag;

	rt_rf_power_state	rf_pwrstate;//cur power state
	//rt_rf_power_state 	current_rfpwrstate;
	rt_rf_power_state	change_rfpwrstate;

	u8		wepkeymask;
	u8		bHWPowerdown;//if support hw power down
	u8		bHWPwrPindetect;
	u8		bkeepfwalive;
	u8		brfoffbyhw;
	unsigned long PS_BBRegBackup[PSBBREG_TOTALCNT];

#ifdef CONFIG_INTEL_PROXIM
	u8	stored_power_mgnt;
#endif

	u8 lps_level_bk;
	u8 lps_level; /*LPS_NORMAL,LPA_CG,LPS_PG*/
#ifdef CONFIG_LPS_PG
	u8 lpspg_rsvd_page_locate;
	u8 blpspg_info_up;
#endif

#ifdef CONFIG_WOWLAN_SSL_KEEP_ALIVE
	u8 ssl_info_rsvd_page_locate;
	u8 ssl_pattern_rsvd_page_locate;
#endif

#ifdef TDMA_POWER_SAVING
	u8	tdma_slot_period;
	u8	tdma_rfon_period_len_1;
	u8	tdma_rfon_period_len_2;
	u8	tdma_rfon_period_len_3;
#endif

	u8	ps_timeout;

	u8	lps_dtim;
	u8  lps_enter_threshold;

	u8	lps_chk_by_tp;
	u16 lps_tx_tp_th; // Kbps
	u16 lps_rx_tp_th; // Kbps
	u16 lps_bi_tp_th; // Kbps // TRX TP
	int lps_chk_cnt_th;
	int lps_chk_cnt;
	u32 lps_tx_pkts;
	u32 lps_rx_pkts;

#ifdef CONFIG_WOWLAN_TCP_KEEP_ALIVE
	u8  tcp_keep_alive_rsvd_page_locate;
	u32 tcp_keep_alive_interval_ms;
	u32 tcp_keep_alive_resend_ms;
	u8  tcp_keep_alive_wake_sys;
	u8 *tcp_keep_alive_pkt;
	u32 tcp_keep_alive_pkt_size;
#ifdef CONFIG_WOWLAN_DTIMTO
	u8  tcp_keep_alive_dtimto_enable;
	u8  tcp_keep_alive_dtimto_retry_inc;
	u8  tcp_keep_alive_dtimto_ack_timeout;
#endif
#endif

#ifdef CONFIG_WOWLAN_DHCP_RENEW
	u8  dhcp_renew_rsvd_page_locate;
	u8 *dhcp_renew_pkt;
	u32 dhcp_renew_pkt_size;
	u16 dhcp_renew_lease_time;
	u16 dhcp_renew_t1;
#endif

#ifdef CONFIG_WOWLAN_DYNAMIC_TX_PWR
	u8  dynamic_tx_pwr_threshold_rssi_low;
	u8  dynamic_tx_pwr_threshold_rssi_high;
	u8  dynamic_tx_pwr_offset_db_low;
	u8  dynamic_tx_pwr_offset_db_high;
#endif

#ifdef CONFIG_ARP_KEEP_ALIVE
	u8  arp_rsp_keep_alive_enable;
	u8  gw_ip[4];
	u8  tcp_ip[4];
	u8  tcp_mac[6];
#endif

#ifdef USER_CTL_POWER_SAVE
	u8 uc_ps_enable;
	/*auto turn off radio timer in user ctrl ps mode*/
	u16 auto_rfoff_timer;
	/*after enter wowlan,wifi FW sniffer for some
	wakeup broadcast packet for sniffer timer
	every sniffer_interval.*/
	u8 sniffer_timer;
	u32 sniffer_interval;
#endif

};

#define RTW_PWR_STATE_CHK_INTERVAL 2000

#define _rtw_set_pwr_state_check_timer(pwrctrlpriv, ms) \
	do { \
		/*DBG_871X("%s _rtw_set_pwr_state_check_timer(%p, %d)\n", __FUNCTION__, (pwrctrlpriv), (ms));*/ \
		rtw_set_timer(&(pwrctrlpriv)->pwr_state_check_timer, (ms)); \
	} while(0)

#define rtw_set_pwr_state_check_timer(pwrctrlpriv) \
	_rtw_set_pwr_state_check_timer((pwrctrlpriv), (pwrctrlpriv)->pwr_state_check_interval)

extern void rtw_init_pwrctrl_priv(_adapter *adapter);
extern void rtw_free_pwrctrl_priv(_adapter *adapter);
extern void rtw_set_ps_mode(PADAPTER padapter, u8 ps_mode, u8 smart_ps, u8 bcn_ant_mode);
extern void rtw_set_rpwm(_adapter *padapter, u8 val8);
extern void LeaveAllPowerSaveMode(PADAPTER Adapter);
void ips_enter(_adapter *padapter);
int ips_leave(_adapter *padapter);

void rtw_ps_processor(_adapter *padapter);

s32 LPS_RF_ON_check(PADAPTER padapter, u32 delay_ms);
void LPS_Enter(PADAPTER padapter);
void LPS_Leave(PADAPTER padapter);

//TODO
//u8 rtw_interface_ps_func(_adapter *padapter,HAL_INTF_PS_FUNC efunc_id,u8* val);
int _rtw_pwr_wakeup(_adapter *padapter, u32 ips_deffer_ms, const char *caller);
#define rtw_pwr_wakeup(adapter) _rtw_pwr_wakeup(adapter, RTW_PWR_STATE_CHK_INTERVAL, __FUNCTION__)
int rtw_pm_set_ips(_adapter *padapter, u8 mode);
int rtw_pm_set_lps(_adapter *padapter, u8 mode);
int rtw_pm_set_lps_level(_adapter *padapter, u8 level);
int rtw_pm_set_tdma_param(_adapter *padapter, u8 tdma_slot_period, u8 tdma_rfon_period_len_1, u8 tdma_rfon_period_len_2, u8 tdma_rfon_period_len_3);
#endif  //__RTL871X_PWRCTRL_H_

#ifdef USER_CTL_POWER_SAVE
void rtw_ucps_cmd(_adapter *padapter, u8 enable);
void rtw_ucps_close_rf(_adapter *padapter, u8 enable);
#endif

