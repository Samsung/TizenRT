#ifndef _WIFI_P2P_SUPPLICANT_H_
#define _WIFI_P2P_SUPPLICANT_H_

#include <p2p/p2p.h>

enum rtw_p2p_state {
	P2P_STATE_NONE = 0,							//	P2P disable
	P2P_STATE_IDLE = 1,								//	P2P had enabled and do nothing
	P2P_STATE_LISTEN = 2,							//	In pure listen state
	P2P_STATE_SCAN = 3,							//	In scan phase
	P2P_STATE_FIND_PHASE_LISTEN = 4,				//	In the listen state of find phase
	P2P_STATE_FIND_PHASE_SEARCH = 5,				//	In the search state of find phase
	P2P_STATE_TX_PROVISION_DIS_REQ = 6,			//	In P2P provisioning discovery
	P2P_STATE_RX_PROVISION_DIS_RSP = 7,
	P2P_STATE_RX_PROVISION_DIS_REQ = 8,
	P2P_STATE_GONEGO_ING = 9,						//	Doing the group owner negoitation handshake
	P2P_STATE_GONEGO_OK = 10,						//	finish the group negoitation handshake with success
	P2P_STATE_GONEGO_FAIL = 11,					//	finish the group negoitation handshake with failure
	P2P_STATE_RECV_INVITE_REQ_MATCH = 12,		//	receiving the P2P Inviation request and match with the profile.
	P2P_STATE_PROVISIONING_ING = 13,				//	Doing the P2P WPS
	P2P_STATE_PROVISIONING_DONE = 14,			//	Finish the P2P WPS
	P2P_STATE_TX_INVITE_REQ = 15,					//	Transmit the P2P Invitation request
	P2P_STATE_RX_INVITE_RESP = 16,				//	Receiving the P2P Invitation response
	P2P_STATE_RECV_INVITE_REQ_DISMATCH = 17,	//	receiving the P2P Inviation request and dismatch with the profile.
	P2P_STATE_RECV_INVITE_REQ_GO = 18,			//	receiving the P2P Inviation request and this wifi is GO.
	P2P_STATE_RECV_INVITE_REQ_JOIN = 19,			//	receiving the P2P Inviation request to join an existing P2P Group.
	P2P_STATE_FORMATION_COMPLETE = 20,
	P2P_STATE_CONNECTED = 21,
	P2P_STATE_DISCONNECT = 22,
	P2P_STATE_FOUND = 23,
	P2P_STATE_WSC_EXCHAGE_START = 24,
};

enum p2p_group_removal_reason {
	P2P_GROUP_REMOVAL_UNKNOWN,
	P2P_GROUP_REMOVAL_SILENT,
	P2P_GROUP_REMOVAL_FORMATION_FAILED,
	P2P_GROUP_REMOVAL_REQUESTED,
	P2P_GROUP_REMOVAL_IDLE_TIMEOUT,
	P2P_GROUP_REMOVAL_UNAVAILABLE,
	P2P_GROUP_REMOVAL_GO_ENDING_SESSION
};

/**
 * struct wpa_scan_res - Scan result for an BSS/IBSS
 * @flags: information flags about the BSS/IBSS (WPA_SCAN_*)
 * @bssid: BSSID
 * @freq: frequency of the channel in MHz (e.g., 2412 = channel 1)
 * @beacon_int: beacon interval in TUs (host byte order)
 * @caps: capability information field in host byte order
 * @qual: signal quality
 * @noise: noise level
 * @level: signal level
 * @tsf: Timestamp
 * @age: Age of the information in milliseconds (i.e., how many milliseconds
 * ago the last Beacon or Probe Response frame was received)
 * @ie_len: length of the following IE field in octets
 * @beacon_ie_len: length of the following Beacon IE field in octets
 *
 * This structure is used as a generic format for scan results from the
 * driver. Each driver interface implementation is responsible for converting
 * the driver or OS specific scan results into this format.
 *
 * If the driver does not support reporting all IEs, the IE data structure is
 * constructed of the IEs that are available. This field will also need to
 * include SSID in IE format. All drivers are encouraged to be extended to
 * report all IEs to make it easier to support future additions.
 */
struct wpa_scan_res {
	unsigned int flags;
	u8 bssid[ETH_ALEN];
	int freq;
	u16 beacon_int;
	u16 caps;
	int qual;
	int noise;
	int level;
	u64 tsf;
	unsigned int age;
	size_t ie_len;
	size_t beacon_ie_len;
	u8 *ie;
	/*
	 * Followed by ie_len octets of IEs from Probe Response frame (or if
	 * the driver does not indicate source of IEs, these may also be from
	 * Beacon frame). After the first set of IEs, another set of IEs may
	 * follow (with beacon_ie_len octets of data) if the driver provides
	 * both IE sets.
	 */
};

struct p2p_scan_report_node {
	struct list_head			list;
	struct rtw_scan_result		*p_scanned_AP_info;
	u8							*ies;
	u32							ie_len;
	u8							role;
};

struct p2p_cmd_priv {
	struct	list_head	queue;
	rtos_mutex_t		lock;
	rtos_sema_t wakeup_sema;
};

struct p2p_cmd_obj {
	struct list_head	list;
	u8	*parmbuf;
	void(*cmd_hdl)(u8 *parmbuf);
};

/** P2P device scanned (only basic information) */
#define P2P_EVENT_DEVICE_SCANNED "P2P-DEVICE-SCANNED "

/** P2P device found */
#define P2P_EVENT_DEVICE_FOUND "P2P-DEVICE-FOUND "

/** P2P device lost */
#define P2P_EVENT_DEVICE_LOST "P2P-DEVICE-LOST "

/** P2P find stopped */
#define P2P_EVENT_FIND_STOPPED "P2P-FIND-STOPPED "

/** A P2P device requested GO negotiation, but we were not ready to start the
 * negotiation */
#define P2P_EVENT_GO_NEG_REQUEST "P2P-GO-NEG-REQUEST "
#define P2P_EVENT_GO_NEG_SUCCESS "P2P-GO-NEG-SUCCESS "
#define P2P_EVENT_GO_NEG_FAILURE "P2P-GO-NEG-FAILURE "
#define P2P_EVENT_GROUP_FORMATION_SUCCESS "P2P-GROUP-FORMATION-SUCCESS "
#define P2P_EVENT_GROUP_FORMATION_FAILURE "P2P-GROUP-FORMATION-FAILURE "

/* P2P Group started or removed */
#define P2P_EVENT_GROUP_STARTED "P2P-GROUP-STARTED "
#define P2P_EVENT_GROUP_REMOVED "P2P-GROUP-REMOVED "

#define AP_STA_CONNECTED "AP-STA-CONNECTED "
#define AP_STA_DISCONNECTED "AP-STA-DISCONNECTED "

void wifi_p2p_rx_mgnt(u8 *evt_info);
int wifi_p2p_connect(const u8 *peer_addr,
					 const char *pin, enum p2p_wps_method wps_method,
					 int persistent_group, int auto_join, int join, int auth,
					 int go_intent, int freq, int persistent_id, int pd);
int wifi_p2p_start_wps(void *res);
void wifi_p2p_set_state(u8 p2p_state);
int wifi_p2p_group_notify_assoc(u8 *buf, u16 buf_len);
int wifi_p2p_group_notify_disassoc(u8 *addr);

int wifi_cmd_p2p_listen(unsigned int timeout);
void wifi_cmd_p2p_find(u32 timeout);
void wifi_cmd_p2p_peers(void);
void wifi_cmd_p2p_state(void);
int wifi_cmd_p2p_connect(u8 *dest, enum p2p_wps_method config_method, char *pin, int go_intent, u32 timeout_sec);
void wifi_cmd_p2p_disconnect(void);

int wifi_p2p_init(u8 *dev_addr, u8 listen_ch, u8 op_ch);
void wifi_p2p_deinit(void);
void wifi_p2p_set_dev_name(const char *dev_name);
void wifi_p2p_set_manufacturer(const char *manufacturer);
void wifi_p2p_set_model_name(const char *model_name);
void wifi_p2p_set_model_number(const char *model_number);
void wifi_p2p_set_serial_number(const char *serial_number);
void wifi_p2p_set_pri_dev_type(const u8 *pri_dev_type);
void wifi_p2p_set_ssid(const char *ssid_in);
void wifi_p2p_set_config_methods(u16 config_methods);
void wifi_p2p_init_auto_go_params(void *res, u8 *passphrase, u8 channel);

void wifi_p2p_set_role(u32 role);
u8 wifi_p2p_check_go(void);
int wifi_p2p_set_remain_on_ch(unsigned char wlan_idx, u8 enable);
void wifi_p2p_join_status_hdl(u8 *evt_info);
void wifi_p2p_channel_switch_ready(u8 *evt_info);
int wifi_p2p_start_go(char *ssid, char *passphrase, u8 channel);
int wifi_p2p_start_auto_go(u8 channel);

#endif //_WIFI_P2P_SUPPLICANT_H_

