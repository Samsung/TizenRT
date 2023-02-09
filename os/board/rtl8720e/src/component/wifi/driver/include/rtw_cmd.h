/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
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
#ifndef __RTW_CMD_H_
#define __RTW_CMD_H_

#include <rtw_rf.h>

#define C2H_MEM_SZ (16*1024)
//#define CMD_DBG 0

#ifndef CONFIG_RTL8711FW

#include <ieee80211.h> // <ieee80211/ieee80211.h>
#include <rom_ieee80211.h> // <ieee80211/ieee80211.h>

#define FREE_CMDOBJ_SZ	128

#define MAX_CMDSZ	1024
#define MAX_RSPSZ	512
#define MAX_EVTSZ	1024

#define CMDBUFF_ALIGN_SZ 4

struct cmd_obj {
	_adapter *padapter;
	u16	cmdcode;
	u8	res;
	u8	*parmbuf;
	u32	cmdsz;
	u32	parm_nobuf;
	u8	*rsp;
	u32	rspsz;
	struct submit_ctx *sctx;
	//_sema 	cmd_sem;
	_list	list;
};

struct cmd_hdl_callback {
	u32	cmd_code;
	u8(*h2cfuns)(_adapter *padapter, struct cmd_obj *pcmd);
	void (*callback)(_adapter  *padapter, struct cmd_obj *cmd);

};


/* cmd flags */
enum {
	RTW_CMDF_DIRECTLY = BIT0,
	RTW_CMDF_WAIT_ACK = BIT1,
};

struct cmd_priv {
	_queue	cmd_queue;
	u8 	cmdthd_running;
	_adapter *padapter;
};

#define init_h2fwcmd_w_parm_no_rsp(pcmd, pparm, code) \
do {\
	rtw_init_listhead(&pcmd->list);\
	pcmd->cmdcode = code;\
	pcmd->parmbuf = (u8 *)(pparm);\
	pcmd->cmdsz = sizeof (*pparm);\
	pcmd->parm_nobuf = 0;\
	pcmd->rsp = NULL;\
	pcmd->rspsz = 0;\
} while(0)

extern u32 rtw_enqueue_cmd(struct cmd_priv *pcmdpriv, struct cmd_obj *obj);
extern struct cmd_obj *rtw_dequeue_cmd(struct cmd_priv *pcmdpriv);
extern void rtw_free_cmd_obj(struct cmd_obj *pcmd);

void rtw_cmd_resource_free(_adapter *padapter);
u8 rtw_cmd_process(_adapter *padapter, void *task);

extern void rtw_cmd_clr_isr(struct cmd_priv *pcmdpriv);

#else	/* CONFIG_RTL8711FW */
#include <ieee80211.h>
#endif	/* CONFIG_RTL8711FW */

enum LPS_CTRL_TYPE {
	LPS_CTRL_SCAN = 0,
	LPS_CTRL_JOINBSS = 1,
	LPS_CTRL_CONNECT = 2,
	LPS_CTRL_DISCONNECT = 3,
	LPS_CTRL_SPECIAL_PACKET = 4,
	LPS_CTRL_LEAVE = 5,
	LPS_CTRL_DENY = 6,
	LPS_CTRL_TRAFFIC_BUSY = 7,
	LPS_CTRL_TX_TRAFFIC_LEAVE = 8,
	LPS_CTRL_RX_TRAFFIC_LEAVE = 9,
};

enum RFINTFS {
	SWSI,
	HWSI,
	HWPI,
};

/*
Caller Mode: Infra, Ad-HoC(C)

Notes: To enter USB suspend mode

Command Mode

*/
struct usb_suspend_parm {
	u32 action;// 1: sleep, 0:resume
};

/*
Caller Mode: Infra, Ad-HoC

Notes: To join a known BSS.

Command-Event Mode

*/

/*
Caller Mode: Infra, Ad-Hoc

Notes: To join the specified bss

Command Event Mode

*/
struct joinbss_parm {
	WLAN_BSSID_EX network;
};

/*
Caller Mode: Infra, Ad-HoC(C)

Notes: To disconnect the current associated BSS

Command Mode

*/
struct disconnect_parm {
	u32 rsvd;
};

/*
Caller Mode: AP, Ad-HoC(M)

Notes: To create a BSS

Command Mode
*/
struct createbss_parm {
	WLAN_BSSID_EX network;

	/* used by AP/Mesh mode now */
	u8 ifbmp;
	u8 excl_ifbmp;
	s16 req_ch;
	s8 req_bw;
	s8 req_offset;
};

/*
Caller Mode: AP, Ad-HoC, Infra

Notes: To set the NIC mode of RTL8711

Command Mode

The definition of mode:

#define IW_MODE_AUTO	0	// Let the driver decides which AP to join
#define IW_MODE_ADHOC	1	// Single cell network (Ad-Hoc Clients)
#define IW_MODE_INFRA	2	// Multi cell network, roaming, ..
#define IW_MODE_MASTER	3	// Synchronisation master or Access Point
#define IW_MODE_REPEAT	4	// Wireless Repeater (forwarder)
#define IW_MODE_SECOND	5	// Secondary master/repeater (backup)
#define IW_MODE_MONITOR	6	// Passive monitor (listen only)

*/
struct	setopmode_parm {
	u8	mode;
	u8	rsvd[3];
};

/*
Caller Mode: AP, Ad-HoC, Infra

Notes: To ask RTL8711 performing site-survey

Command-Event Mode

*/

#define RTW_SSID_SCAN_AMOUNT 1 //Reduce ssid scan amount due to memory limitation - Alex Fang
#define RTW_CHANNEL_SCAN_AMOUNT (14+37)

struct sitesurvey_parm {
	sint scan_mode;	//active: 1, passive: 0
	//sint bsslimit;	// 1 ~ 48
	// for up to 9 probreq with specific ssid
	NDIS_802_11_SSID ssid[RTW_SSID_SCAN_AMOUNT];
	u8 ssid_num;
#if  defined(CONFIG_IEEE80211K) || defined(CONFIG_RTW_WNM)
	struct rtw_ieee80211_channel ch[RTW_CHANNEL_SCAN_AMOUNT];
	u8 ch_num;
	u32 token; 	/* 80211k use it to identify caller */
	u16 duration;	/* 0: use default, otherwise: channel scan time */
#endif
};

/*
Caller Mode: Any

Notes: To set the auth type of RTL8711. open/shared/802.1x

Command Mode

*/
struct setauth_parm {
	u8 mode;  //0: legacy open, 1: legacy shared 2: 802.1x
	u8 _1x;   //0: PSK, 1: TLS
	u8 rsvd[2];
};

/*
Caller Mode: Infra

a. algorithm: wep40, wep104, tkip & aes
b. keytype: grp key/unicast key
c. key contents

when shared key ==> keyid is the camid
when 802.1x ==> keyid [0:1] ==> grp key
when 802.1x ==> keyid > 2 ==> unicast key

*/
struct setkey_parm {
	u8	algorithm;	// encryption algorithm, could be none, wep40, TKIP, CCMP, wep104
	u8	keyid;
	u8 	grpkey;		// 1: this is the grpkey for 802.1x. 0: this is the unicast key for 802.1x
	u8 	set_tx;		// 1: main tx key for wep. 0: other key.
	u8	key[16];	// this could be 40 or 104
};

/*
When in AP or Ad-Hoc mode, this is used to
allocate an sw/hw entry for a newly associated sta.

Command

when shared key ==> algorithm/keyid

*/
struct set_stakey_parm {
	u8	addr[ETH_ALEN];
	u8	algorithm;
	u8 	id;// currently for erasing cam entry if algorithm == _NO_PRIVACY_
	u8	key[16];
};

struct set_stakey_rsp {
	u8	addr[ETH_ALEN];
	u8	keyid;
	u8	rsvd;
};

/*
Caller Ad-Hoc/AP

Command -Rsp(AID == CAMID) mode

This is to force fw to add an sta_data entry per driver's request.

FW will write an cam entry associated with it.

*/
struct set_assocsta_parm {
	u8	addr[ETH_ALEN];
};

struct set_assocsta_rsp {
	u8	cam_id;
	u8	rsvd[3];
};

/*
	Caller Ad-Hoc/AP

	Command mode

	This is to force fw to del an sta_data entry per driver's request

	FW will invalidate the cam entry associated with it.

*/
struct del_assocsta_parm {
	u8  	addr[ETH_ALEN];
};

/*
Caller Mode: AP/Ad-HoC(M)

Notes: To notify fw that given staid has changed its power state

Command Mode

*/
struct setstapwrstate_parm {
	u8	staid;
	u8	status;
	u8	hwaddr[6];
};

/*
Caller Mode: Any

Notes: To setup the basic rate of RTL8711

Command Mode

*/
struct	setbasicrate_parm {
	u8	basicrates[NumRates];
};

/*
Caller Mode: Any

Notes: To read the current basic rate

Command-Rsp Mode

*/
struct getbasicrate_parm {
	u32 rsvd;
};

struct getbasicrate_rsp {
	u8 basicrates[NumRates];
};

/*
Caller Mode: Any

Notes: To setup the data rate of RTL8711

Command Mode

*/
struct setdatarate_parm {
#ifdef MP_FIRMWARE_OFFLOAD
	u32	curr_rateidx;
#else
	u8	mac_id;
	u8	datarates[NumRates];
#endif
};

/*
Caller Mode: Any

Notes: To read the current data rate

Command-Rsp Mode

*/
struct getdatarate_parm {
	u32 rsvd;

};
struct getdatarate_rsp {
	u8 datarates[NumRates];
};


/*
Caller Mode: Any
AP: AP can use the info for the contents of beacon frame
Infra: STA can use the info when sitesurveying
Ad-HoC(M): Like AP
Ad-HoC(C): Like STA


Notes: To set the phy capability of the NIC

Command Mode

*/

struct	setphyinfo_parm {
	struct regulatory_class class_sets[NUM_REGULATORYS];
	u8	status;
};

struct	getphyinfo_parm {
	u32 rsvd;
};

struct	getphyinfo_rsp {
	struct regulatory_class class_sets[NUM_REGULATORYS];
	u8	status;
};

/*
Caller Mode: Any

Notes: To set the channel/modem/band
This command will be used when channel/modem/band is changed.

Command Mode

*/
struct	setphy_parm {
	u8	rfchannel;
	u8	modem;
};

/*
Caller Mode: Any

Notes: To get the current setting of channel/modem/band

Command-Rsp Mode

*/
struct	getphy_parm {
	u32 rsvd;

};
struct	getphy_rsp {
	u8	rfchannel;
	u8	modem;
};

struct readBB_parm {
	u8	offset;
};
struct readBB_rsp {
	u8	value;
};

struct readTSSI_parm {
	u8	offset;
};
struct readTSSI_rsp {
	u8	value;
};

struct writeBB_parm {
	u8	offset;
	u8	value;
};

struct readRF_parm {
	u8	offset;
};
struct readRF_rsp {
	u32	value;
};

struct writeRF_parm {
	u32	offset;
	u32	value;
};

struct getrfintfs_parm {
	u8	rfintfs;
};


struct Tx_Beacon_param {
	WLAN_BSSID_EX network;
};

/*
	Notes: This command is used for H2C/C2H loopback testing

	mac[0] == 0
	==> CMD mode, return H2C_SUCCESS.
	The following condition must be ture under CMD mode
		mac[1] == mac[4], mac[2] == mac[3], mac[0]=mac[5]= 0;
		s0 == 0x1234, s1 == 0xabcd, w0 == 0x78563412, w1 == 0x5aa5def7;
		s2 == (b1 << 8 | b0);

	mac[0] == 1
	==> CMD_RSP mode, return H2C_SUCCESS_RSP

	The rsp layout shall be:
	rsp: 			parm:
		mac[0]  =   mac[5];
		mac[1]  =   mac[4];
		mac[2]  =   mac[3];
		mac[3]  =   mac[2];
		mac[4]  =   mac[1];
		mac[5]  =   mac[0];
		s0		=   s1;
		s1		=   swap16(s0);
		w0		=  	swap32(w1);
		b0		= 	b1
		s2		= 	s0 + s1
		b1		= 	b0
		w1		=	w0

	mac[0] == 	2
	==> CMD_EVENT mode, return 	H2C_SUCCESS
	The event layout shall be:
	event:			parm:
		mac[0]  =   mac[5];
		mac[1]  =   mac[4];
		mac[2]  =   event's sequence number, starting from 1 to parm's marc[3]
		mac[3]  =   mac[2];
		mac[4]  =   mac[1];
		mac[5]  =   mac[0];
		s0		=   swap16(s0) - event.mac[2];
		s1		=   s1 + event.mac[2];
		w0		=  	swap32(w0);
		b0		= 	b1
		s2		= 	s0 + event.mac[2]
		b1		= 	b0
		w1		=	swap32(w1) - event.mac[2];

		parm->mac[3] is the total event counts that host requested.


	event will be the same with the cmd's param.

*/

// CMD param Formart for driver extra cmd handler
struct drvextra_cmd_parm {
	int ec_id; //extra cmd id
	int type; /* Can use this field as the type id or command size */
	int size; /* buffer size */
	unsigned char *pbuf;
};

/*------------------- Below are used for RF/BB tunning ---------------------*/

struct	setantenna_parm {
	u8	tx_antset;
	u8	rx_antset;
	u8	tx_antenna;
	u8	rx_antenna;
};

struct	enrateadaptive_parm {
	u32	en;
};

struct settxagctbl_parm {
	u32	txagc[MAX_RATES_LENGTH];
};

struct gettxagctbl_parm {
	u32 rsvd;
};
struct gettxagctbl_rsp {
	u32	txagc[MAX_RATES_LENGTH];
};

struct setagcctrl_parm {
	u32	agcctrl;		// 0: pure hw, 1: fw
};


struct setssup_parm	{
	u32	ss_ForceUp[MAX_RATES_LENGTH];
};

struct getssup_parm	{
	u32 rsvd;
};
struct getssup_rsp	{
	u8	ss_ForceUp[MAX_RATES_LENGTH];
};


struct setssdlevel_parm	{
	u8	ss_DLevel[MAX_RATES_LENGTH];
};

struct getssdlevel_parm	{
	u32 rsvd;
};
struct getssdlevel_rsp	{
	u8	ss_DLevel[MAX_RATES_LENGTH];
};

struct setssulevel_parm	{
	u8	ss_ULevel[MAX_RATES_LENGTH];
};

struct getssulevel_parm	{
	u32 rsvd;
};
struct getssulevel_rsp	{
	u8	ss_ULevel[MAX_RATES_LENGTH];
};


struct	setcountjudge_parm {
	u8	count_judge[MAX_RATES_LENGTH];
};

struct	getcountjudge_parm {
	u32 rsvd;
};
struct	getcountjudge_rsp {
	u8	count_judge[MAX_RATES_LENGTH];
};


struct setratable_parm {
	u8 ss_ForceUp[NumRates];
	u8 ss_ULevel[NumRates];
	u8 ss_DLevel[NumRates];
	u8 count_judge[NumRates];
};

struct getratable_parm {
	uint rsvd;
};
struct getratable_rsp {
	u8 ss_ForceUp[NumRates];
	u8 ss_ULevel[NumRates];
	u8 ss_DLevel[NumRates];
	u8 count_judge[NumRates];
};


//to get TX,RX retry count
struct gettxretrycnt_parm {
	unsigned int rsvd;
};
struct gettxretrycnt_rsp {
	unsigned long tx_retrycnt;
};

struct getrxretrycnt_parm {
	unsigned int rsvd;
};
struct getrxretrycnt_rsp {
	unsigned long rx_retrycnt;
};

//to get BCNOK,BCNERR count
struct getbcnokcnt_parm {
	unsigned int rsvd;
};
struct getbcnokcnt_rsp {
	unsigned long  bcnokcnt;
};

struct getbcnerrcnt_parm {
	unsigned int rsvd;
};
struct getbcnerrcnt_rsp {
	unsigned long bcnerrcnt;
};

// to get current TX power level
struct getcurtxpwrlevel_parm {
	unsigned int rsvd;
};
struct getcurtxpwrlevel_rsp {
	unsigned short tx_power;
};

struct addBaReq_parm {
	unsigned int tid;
	u8	addr[ETH_ALEN];
};

/*H2C Handler index: 46 */
struct SetChannel_parm {
	u32 curr_ch;
};

#ifdef MP_FIRMWARE_OFFLOAD
/*H2C Handler index: 47 */
struct SetTxPower_parm {
	u8 TxPower;
};

/*H2C Handler index: 48 */
struct SwitchAntenna_parm {
	u16 antenna_tx;
	u16 antenna_rx;
//	R_ANTENNA_SELECT_CCK cck_txrx;
	u8 cck_txrx;
};

/*H2C Handler index: 49 */
struct SetCrystalCap_parm {
	u32 curr_crystalcap;
};

/*H2C Handler index: 50 */
struct SetSingleCarrierTx_parm {
	u8 bStart;
};

/*H2C Handler index: 51 */
struct SetSingleToneTx_parm {
	u8 bStart;
	u8 curr_rfpath;
};

/*H2C Handler index: 52 */
struct SetCarrierSuppressionTx_parm {
	u8 bStart;
	u32 curr_rateidx;
};

/*H2C Handler index: 53 */
struct SetContinuousTx_parm {
	u8 bStart;
	u8 CCK_flag; /*1:CCK 2:OFDM*/
	u32 curr_rateidx;
};

/*H2C Handler index: 54 */
struct SwitchBandwidth_parm {
	u8 curr_bandwidth;
};

#endif	/* MP_FIRMWARE_OFFLOAD */

/*H2C Handler index: 62 */
struct TDLSoption_param {
	u8 addr[ETH_ALEN];
	u8 option;
};


/*
Result:
0x00: success
0x01: sucess, and check Response.
0x02: cmd ignored due to duplicated sequcne number
0x03: cmd dropped due to invalid cmd code
0x04: reserved.

*/

#define H2C_RSP_OFFSET			512

#define H2C_SUCCESS			0x00
#define H2C_SUCCESS_RSP			0x01
#define H2C_DUPLICATED			0x02
#define H2C_DROPPED			0x03
#define H2C_PARAMETERS_ERROR		0x04
#define H2C_REJECTED			0x05
#define H2C_CMD_OVERFLOW		0x06
#define H2C_RESERVED			0x07

#define REQ_CH_NONE		-1
#define REQ_BW_NONE		-1
#define REQ_BW_ORI		-2
#define REQ_OFFSET_NONE	-1

extern u8 rtw_setassocsta_cmd(_adapter  *padapter, u8 *mac_addr);
extern u8 rtw_setstandby_cmd(_adapter *padapter, uint action);
extern u8 rtw_scan_start_cmd(_adapter  *padapter, struct sitesurvey_parm *pparm);
extern u8 rtw_createbss_cmd(_adapter  *padapter);
extern u8 rtw_createbss_cmd_ex(_adapter  *padapter, unsigned char *pbss, unsigned int sz);
extern u8 rtw_setphy_cmd(_adapter  *padapter, u8 modem, u8 ch);
extern u8 rtw_setstakey_cmd(_adapter  *padapter, u8 *psta, u8 unicast_key);
extern u8 rtw_clearstakey_cmd(_adapter *padapter, u8 *psta, u8 entry);
extern u8 rtw_joinbss_cmd(_adapter  *padapter, struct wlan_network *pnetwork);
extern u8 rtw_disassoc_cmd(_adapter  *padapter);
extern u8 rtw_setdatarate_cmd(_adapter  *padapter, u8 *rateset);
extern u8 rtw_setbasicrate_cmd(_adapter  *padapter, u8 *rateset);
extern u8 rtw_setbbreg_cmd(_adapter *padapter, u8 offset, u8 val);
extern u8 rtw_setrfreg_cmd(_adapter *padapter, u8 offset, u32 val);
extern u8 rtw_getbbreg_cmd(_adapter *padapter, u8 offset, u8 *pval);
extern u8 rtw_getrfreg_cmd(_adapter *padapter, u8 offset, u8 *pval);
extern u8 rtw_setrfintfs_cmd(_adapter  *padapter, u8 mode);
extern u8 rtw_setrttbl_cmd(_adapter  *padapter, struct setratable_parm *prate_table);
extern u8 rtw_getrttbl_cmd(_adapter  *padapter, struct getratable_rsp *pval);

extern u8 rtw_gettssi_cmd(_adapter  *padapter, u8 offset, u8 *pval);
extern u8 rtw_setfwdig_cmd(_adapter *padapter, u8 type);
extern u8 rtw_setfwra_cmd(_adapter *padapter, u8 type);

extern u8 rtw_addbareq_cmd(_adapter *padapter, u8 tid, u8 *addr);

extern u8 rtw_dynamic_chk_wk_hdl(_adapter *padapter,  struct cmd_obj *cmd_obj);
extern u8 rtw_dynamic_chk_wk_cmd(_adapter *adapter);

extern u8 rtw_lps_ctrl_wk_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
u8 rtw_lps_ctrl_wk_cmd(_adapter *padapter, u8 lps_ctrl_type, u8 enqueue);

#ifdef CONFIG_ANTENNA_DIVERSITY
extern  u8 rtw_antenna_select_cmd(_adapter *padapter, u8 antenna);
#endif

extern u8 rtw_ps_wk_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
extern u8 rtw_ps_cmd(_adapter *padapter);

extern u8 rtw_set_chplan(u8 chplan);
u8 rtw_rm_post_event_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);

u8 createbss_hdl(_adapter *padapter, u8 *pbuf);
u8 join_cmd_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
u8 disconnect_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
u8 rtw_scan_cmd_hdl(_adapter *padapter,  struct cmd_obj *cmd_obj);
u8 setkey_hdl(_adapter *padapter,  struct cmd_obj *cmd_obj);
u8 set_stakey_hdl(_adapter *padapter,  struct cmd_obj *cmd_obj);
u8 add_ba_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
u8 tx_beacon_hdl(_adapter *padapter,  struct cmd_obj *cmd_obj);
u8 rtw_joinbss_event_callback(_adapter *adapter, struct cmd_obj *cmd_obj);
u8 rtw_stassoc_event_callback(_adapter *adapter, struct cmd_obj *cmd_obj);
u8 rtw_stadel_event_callback(_adapter *adapter, struct cmd_obj *cmd_obj);
#if defined(CONFIG_IEEE80211W)
u8 rtw_sta_timeout_event_callback(_adapter *adapter, struct cmd_obj *cmd_obj);
#endif /* CONFIG_IEEE80211W */
u8 rtw_free_stainfo_hdl(_adapter *padapter, struct cmd_obj *cmd_obj);
u8  rtw_hal_c2h_handler(_adapter *padapter, u8 id, u8 seq, u8 plen, u8 *payload);

extern void rtw_scan_cmd_callback(_adapter  *padapter, struct cmd_obj *pcmd);
extern void rtw_disassoc_cmd_callback(_adapter  *padapter, struct cmd_obj *pcmd);
extern void rtw_joinbss_cmd_callback(_adapter  *padapter, struct cmd_obj *pcmd);
extern void rtw_createbss_cmd_callback(_adapter  *padapter, struct cmd_obj *pcmd);
extern void rtw_getbbrfreg_cmdrsp_callback(_adapter  *padapter, struct cmd_obj *pcmd);
extern void rtw_readtssi_cmdrsp_callback(_adapter	*padapter,  struct cmd_obj *pcmd);

extern void rtw_setstaKey_cmdrsp_callback(_adapter  *padapter,  struct cmd_obj *pcmd);
extern void rtw_setassocsta_cmdrsp_callback(_adapter  *padapter,  struct cmd_obj *pcmd);
extern void rtw_getrttbl_cmdrsp_callback(_adapter  *padapter,  struct cmd_obj *pcmd);
extern void rtw_set_channel_plan_cmd_callback(_adapter	*padapter,  struct cmd_obj *pcmd);

enum rtw_h2c_cmd {
	_JoinBss_CMD_,   /*0*/
	_DisConnect_CMD_,
	_SiteSurvey_CMD_,
	_SetKey_CMD_,	/*5*/
	_SetStaKey_CMD_,
	_AddBAReq_CMD_,
	_TX_Beacon_CMD_,
	_RM_POST_EVENT_CMD_,
	_FREE_STA_INFO_CMD_,
	_JoinBSSRes_CMD_, /*16*/
	_AddSTA_CMD_,
	_DelSTA_CMD_,
#ifdef CONFIG_IEEE80211W
	_TimeoutSTA_CMD_,
#endif
	_Dynamic_Chk_CMD_,
	_PowerSaveCtrl_CMD_,/*IPS,AUTOSuspend*/
	_LPSCtrl_CMD_,
	MAX_H2CCMD
};


#ifdef _RTW_CMD_C_
const struct cmd_hdl_callback wlancmds[] = {
	{_JoinBss_CMD_, join_cmd_hdl, &rtw_joinbss_cmd_callback}, /*0*/
	{_DisConnect_CMD_, disconnect_hdl, &rtw_disassoc_cmd_callback},
	{_SiteSurvey_CMD_, rtw_scan_cmd_hdl, &rtw_scan_cmd_callback},
	{_SetKey_CMD_, setkey_hdl, NULL},  /*5*/
	{_SetStaKey_CMD_, set_stakey_hdl, &rtw_setstaKey_cmdrsp_callback},
	{_AddBAReq_CMD_, add_ba_hdl, NULL},
	{_TX_Beacon_CMD_, tx_beacon_hdl, NULL},
	{_RM_POST_EVENT_CMD_, rtw_rm_post_event_hdl, NULL},
	{_FREE_STA_INFO_CMD_, rtw_free_stainfo_hdl, NULL},
	{_JoinBSSRes_CMD_, rtw_joinbss_event_callback, NULL}, /*16*/
	{_AddSTA_CMD_, rtw_stassoc_event_callback, NULL},
	{_DelSTA_CMD_, rtw_stadel_event_callback, NULL},
#if defined(CONFIG_IEEE80211W)
	{_TimeoutSTA_CMD_, rtw_sta_timeout_event_callback, NULL},
#endif /* CONFIG_IEEE80211W */
	{_Dynamic_Chk_CMD_, rtw_dynamic_chk_wk_hdl, NULL},
	{_PowerSaveCtrl_CMD_, rtw_ps_wk_hdl, NULL},
	{_LPSCtrl_CMD_, rtw_lps_ctrl_wk_hdl, NULL},
};

#endif

#endif // _CMD_H_

