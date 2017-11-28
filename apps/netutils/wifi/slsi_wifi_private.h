/*
 * slsi_wifi.h
 *
 *  Created on: Apr 28, 2017
 */

#ifndef SLSI_WIFI_H_
#define SLSI_WIFI_H_

#define WPA_COMMAND_SCAN                "SCAN"
#define WPA_COMMAND_SCAN_STOP           "ABORT_SCAN"
#define WPA_COMMAND_SCAN_INTERVAL       "SCAN_INTERVAL "
#define WPA_COMMAND_BSS_EXPIRE_AGE      "BSS_EXPIRE_AGE "
#define WPA_COMMAND_SCAN_RESULTS        "SCAN_RESULTS"
#define WPA_COMMAND_AUTOCONNECT         "STA_AUTOCONNECT "
#define WPA_COMMAND_LIST_NETWORKS       "LIST_NETWORKS"
#define WPA_COMMAND_ADD_NETWORK         "ADD_NETWORK"
#define WPA_COMMAND_REMOVE_NETWORK      "REMOVE_NETWORK "
#define WPA_COMMAND_BSS                 "BSS "
#define WPA_COMMAND_UPDATE_CONFIG       "SET update_config "
#define WPA_COMMAND_SAVE_CONFIG         "SAVE_CONFIG"

#define WPA_COMMAND_SET                 "SET "
#define WPA_COMMAND_SET_NETWORK         "SET_NETWORK "
#define WPA_PARAM_KEY_MGMT_NONE         "key_mgmt NONE"
#define WPA_PARAM_KEY_MGMT_WEP          "key_mgmt NONE"
#define WPA_PARAM_KEY_MGMT_WEP_SHARED   "key_mgmt NONE"
#define WPA_PARAM_KEY_MGMT_WPA_PSK      "key_mgmt WPA-PSK"
#define WPA_PARAM_SSID_                 "ssid "
#define WPA_PARAM_SCAN_SSID             "scan_ssid "
#define WPA_PARAM_SCAN_ID               "scan_ssid="
#define WPA_PARAM_BSSID_                "bssid "
#define WPA_PARAM_PSK                   "psk "
#define WPA_PARAM_WEPKEY                "wep_key0 "
#define WPA_PARAM_FREQUENCY_            "frequency "
#define WPA_PARAM_MODE_AP               "mode 2"
#define WPA_PARAM_MODE_STA              "mode 0"
#define WPA_PARAM_NETWORK_DISABLED      "disabled 0"
#define WPA_PARAM_AUTH_ALG              "auth_alg "
#define WPA_PARAM_PAIRWISE              "pairwise "
#define WPA_PARAM_CIPHER                "cipher "
#define WPA_PARAM_GROUP                 "group "
#define WPA_PARAM_PROTO                 "proto "
#define WPA_PARAM_BEACON_INT_           "beacon_int "
#define WPA_PARAM_DTIM_PERIOD_          "dtim_period "
#define WPA_PARAM_VSIE                  "ap_vendor_ie "
#define WPA_PARAM_DISABLE_HT            "disable_ht 1"
#define WPA_PARAM_DISABLE_SHORT_GI      "disable_sgi 1"
#define WPA_PARAM_HT_CAPAB              "ht_capab "
#define WPA_PARAM_HT_MCS                "ht_mcs "

#define WPA_COMMAND_SELECT_NETWORK      "SELECT_NETWORK "
#define WPA_COMMAND_ENABLE_NETWORK      "ENABLE_NETWORK "
#define WPA_COMMAND_DISABLE_NETWORK     "DISABLE_NETWORK all"
#define WPA_COMMAND_RECONNECT           "RECONNECT"
#define WPA_COMMAND_DISCONNECT          "DISCONNECT"
#define WPA_COMMAND_STOP_AP             "STOP_AP"

#define WPA_COMMAND_TERMINATE           "TERMINATE"

#define WPA_COMMAND_STATUS              "STATUS"
#define WPA_STATE_COMPLETED             "COMPLETED"
#define WPA_STATE_DISCONNECTED          "DISCONNECTED"
#define WPA_STATE_SCANNING              "SCANNING"
#define WPA_PARAM_SSID                  "ssid="
#define WPA_PARAM_IP_ADDRESS            "ip_address="
#define WPA_PARAM_ADDRESS               "address="
#define WPA_PARAM_BSSID                 "bssid="
#define WPA_PARAM_WPA_STATE             "wpa_state="
#define WPA_PARAM_WPS_STATE             "wps_state="
#define WPA_PARAM_BEACON_INT            "beacon_int="
#define WPA_PARAM_DTIM_PERIOD           "dtim_period="
#define WPA_PARAM_CAPABILITIES          "capabilities="
#define WPA_PARAM_LEVEL                 "level="
#define WPA_PARAM_QUAL                  "qual="
#define WPA_PARAM_FLAGS                 "flags="
#define WPA_PARAM_FREQ                  "freq="
#define WPA_PARAM_FREQUENCY             "frequency="
#define WPA_PARAM_REASON                "reason="
#define WPA_PARAM_REASON_CODE           "reason_code "
#define WPA_PARAM_LOACLLY_GENERATED     "locally_generated="
#define WPA_PARAM_IE                    "ie="
#define WPA_PARAM_P2P_LISTEN_CHANNEL    "listen_channel"
#define WPA_VALUE_AUTH_ALG_OPEN         "OPEN"
#define WPA_VALUE_AUTH_ALG_SHARED       "SHARED"
#define WPA_VALUE_AUTH_ALG_OPEN_SHARED  "OPEN SHARED"
#define WPA_VALUE_PAIRWISE_TKIP         "TKIP"
#define WPA_VALUE_PAIRWISE_MIXED        "CCMP TKIP"
#define WPA_VALUE_PAIRWISE_AES          "CCMP"
#define WPA_VALUE_CIPHER_NONE BIT       "NONE"
#define WPA_VALUE_CIPHER_WEP40          "WEP40"
#define WPA_VALUE_CIPHER_WEP104         "WEP104"
#define WPA_VALUE_CIPHER_TKIP           "TKIP"
#define WPA_VALUE_CIPHER_CCMP           "CCMP"
#define WPA_VALUE_CIPHER_MIXED          "TKIP CCMP"
#define WPA_VALUE_PROTO_WPA             "WPA"
#define WPA_VALUE_PROTO_RSN             "RSN"	// Same as WPA2
#define WPA_VALUE_PROTO_MIXED           "WPA RSN"
#define WPA_VALUE_GROUP_CCMP            "CCMP"
#define WPA_VALUE_GROUP_TKIP            "TKIP"
#define WPA_VALUE_GROUP_WEP40           "WEP40"
#define WPA_VALUE_GROUP_WEP104          "WEP104"

#define WPA_COMMAND_DRIVER              "DRIVER "
#define WPA_PARAM_COUNTRY               "COUNTRY"
#define WPA_PARAM_FORCE_PANIC           "FORCE_PANIC"
#define WPA_COMMAND_SET_TX_POWER        "SET set_tx_power "
#define WPA_COMMAND_GET_TX_POWER        "GET get_tx_power"

#define WPA_COMMAND_SIGNAL_POLL         "SIGNAL_POLL"
#define WPA_PARAM_RSSI                  "RSSI="
#define WPA_PARAM_LINKSPEED             "LINKSPEED="

#ifdef CONFIG_SLSI_WIFI_P2P_API
#define WPA_COMMAND_P2P_FIND            "P2P_FIND "
#define WPA_COMMAND_P2P_STOP_FIND       "P2P_STOP_FIND"
#define WPA_COMMAND_P2P_LISTEN          "P2P_LISTEN "
#define WPA_COMMAND_P2P_CONNECT         "P2P_CONNECT "
#define WPA_COMMAND_P2P_GROUP_ADD       "P2P_GROUP_ADD"
#define WPA_COMMAND_P2P_REJECT          "P2P_REJECT "
#define WPA_COMMAND_P2P_GROUP_REMOVE    "P2P_GROUP_REMOVE "
#define WPA_COMMAND_P2P_FLUSH           "P2P_FLUSH"
#define WPA_COMMAND_P2P_CANCEL          "P2P_CANCEL"
#define WPA_COMMAND_P2P_PROV_DISC       "P2P_PROV_DISC "
#define WPA_COMMAND_P2P_PEER            "P2P_PEER "
#define WPA_COMMAND_P2P_SET             "P2P_SET "
#define WPA_COMMAND_P2P_WPS_PIN         "WPS_PIN "
#define WPA_COMMAND_P2P_WPS_PBC         "WPS_PBC"
#define WPA_VALUE_P2P_WPS_ANY           "any"
#define WPA_VALUE_FREQ                  "freq="
#define WPA_PARAM_DEV_ID                "dev_id="
#define WPA_PARAM_P2P_DEV_ADDR          "p2p_dev_addr="
#define WPA_PARAM_PRI_DEV_TYPE          "pri_dev_type="
#define WPA_PARAM_NAME                  "name="
#define WPA_PARAM_CONFIG_METHODS        "config_methods="
#define WPA_PARAM_DEV_CAPAB             "dev_capab="
#define WPA_PARAM_GROUP_CAPAB           "group_capab="
#define WPA_PARAM_GO_INTENT             "go_intent="
#define WPA_PARAM_DEVICE_TYPE           "device_type="
#define WPA_PARAM_CONFIG_PEER_DEV       "peer_dev="
#define WPA_PARAM_ROLE                  "role="
#define WPA_VALUE_CLIENT                "client"
#define WPA_VALUE_GO                    "GO"

#define WPA_EMPTY                       ""
#define WPA_SPACE                       " "
#define WPA_PARAM_PERSISTENT            "persistent"
#define WPA_PARAM_PIN                   "pin"
#define WPA_PARAM_PBC                   "pbc"
#define WPA_PARAM_DISPLAY               "display"
#define WPA_PARAM_KEYPAD                "keypad"
#define WPA_PARAM_JOIN                  "join"
#define WPA_PARAM_AUTH                  "auth"
#define WPA_PARAM_AUTO                  "auto"
#define WPA_PARAM_PROVDISC              "provdisc"
#define WPA_PARAM_REJCT                 "reject"
#define WPA_PARAM_DEVICE_NAME           "device_name"
#define WPA_PARAM_SSID_POSTFIX          "ssid_postfix"

/* TODO: From ctrl_iface.c:
"P2P_ASP_PROVISION "
"P2P_ASP_PROVISION_RESP "
"P2P_GROUP_ADD"
"P2P_PROV_DISC "
"P2P_GET_PASSPHRASE"
"P2P_SERV_DISC_REQ "
"P2P_SERV_DISC_CANCEL_REQ "
"P2P_SERV_DISC_RESP "
"P2P_SERVICE_UPDATE"
"P2P_SERV_DISC_EXTERNAL "
"P2P_SERVICE_FLUSH"
"P2P_SERVICE_ADD "
"P2P_SERVICE_DEL "
"P2P_SERVICE_REP "
"P2P_REJECT "
"P2P_INVITE "
"P2P_UNAUTHORIZE "
"P2P_CANCEL"
"P2P_PRESENCE_REQ"
"P2P_EXT_LISTEN"
"P2P_REMOVE_CLIENT "
 */
#endif							//CONFIG_SLSI_WIFI_P2P_API

#ifdef CONFIG_DEBUG_WLAN_API_VERBOSE
#define VPRINT(format, ...) dbg("SLSI API VERBOSE: " format, ##__VA_ARGS__)
#define SLSI_API_VERBOSE  (1)
#ifndef CONFIG_DEBUG_WLAN_API_DEBUG
#define CONFIG_DEBUG_WLAN_API_DEBUG
#endif
#else
#define VPRINT(a, ...) (void)0
#define SLSI_API_VERBOSE  (0)
#endif							//CONFIG_DEBUG_WLAN_API_VERBOSE

#ifdef CONFIG_DEBUG_WLAN_API_DEBUG
#define DPRINT(format, ...) dbg("SLSI API DEBUG: " format, ##__VA_ARGS__)
#define SLSI_API_DEBUG  (1)
#ifndef CONFIG_DEBUG_WLAN_API_ERROR
#define CONFIG_DEBUG_WLAN_API_ERROR
#endif
#else
#define DPRINT(a, ...) (void)0
#define SLSI_API_DEBUG  (0)
#endif							//CONFIG_DEBUG_WLAN_API_DEBUG

#ifdef CONFIG_DEBUG_WLAN_API_ERROR
#define EPRINT(format, ...) dbg("SLSI API ERROR: " format, ##__VA_ARGS__)
#else
#define EPRINT(a, ...) (void)0
#endif							//CONFIG_DEBUG_WLAN_API_ERROR

#define WPA_MAX_SSID_LEN                (4*32+1)	/* SSID encoded in a string - worst case is all 4-octet hex digits + '\0' */
#define WPA_COMMAND_MAX_SIZE            (30 + WPA_MAX_SSID_LEN)
#define WPA_BUFFER_SIZE                 4096
#define REQUEST_RETRY_COUNT             5

typedef enum {
	SLSI_CALLBACK_SHUTDOWN,
	SLSI_CALLBACK_LINK_UP,
	SLSI_CALLBACK_LINK_DOWN,
	SLSI_CALLBACK_SCAN_RESULT,
	SLSI_CALLBACK_P2P_FOUND,
	SLSI_CALLBACK_P2P_LINK_UP,
	SLSI_CALLBACK_P2P_LINK_DOWN
} slsi_callback_t;

typedef union {
#ifdef CONFIG_SLSI_WIFI_P2P_API
	slsi_find_info_t find_info;
#endif
	slsi_reason_t reason;
} slsi_msg_callback_t;

uint8_t slsi_send_mqueue(slsi_callback_t event, slsi_msg_callback_t *msg);

/* Shared functionality between sta/ap and p2p source */
char *slsi_send_request(char *ifname, char *cmd, int8_t *result);
void slsi_send_command_str_digit(char *ifname, char *string, int digit, int8_t *result);
void slsi_send_command_str_upto_4(char *ifname, char *one, char *two, char *three, char *four, int8_t *result);
bool slsi_event_received(const char *str, const char *event);
void slsi_send_command_str(char *ifname, int8_t *result, const char *fmt, ...);
int8_t slsi_leave_network(char *ifname);

#ifdef CONFIG_SLSI_WIFI_P2P_API
slsi_network_link_callback_t g_p2p_link_up;
slsi_network_link_callback_t g_p2p_link_down;
slsi_p2p_find_callback_t g_find_results_handler;

/* Only needed when P2P mode is enabled */
int8_t slsi_p2p_start(void);
int8_t slsi_p2p_stop(void);
void slsi_p2p_event_handle(const char *str);
int8_t slsi_get_country_code(char *country_code);

/* TE interfaces */
int8_t slsi_p2p_connect(char device_address[SLSI_MACADDR_STR_LEN], slsi_config_method_t config_method, char passphrase[SLSI_PASSPHRASE_LEN], uint8_t go_intent, bool persistent, bool join, bool auto_mode, bool auth, bool provdisc, bool reject);
//p2p_connect <peer device address> <pbc|pin|PIN#|p2ps> [display|keypad|p2ps]
//[persistent|persistent=<network id>] [join|auth]
//[go_intent=<0..15>] [freq=<in MHz>] [ht40] [vht] [provdisc] [auto]
//[ssid=<hexdump>]
int8_t slsi_p2p_flush(void);	//-It will do a stop find and clear the p2p device details
int8_t slsi_p2p_prov_disc(char device_address[SLSI_MACADDR_STR_LEN], slsi_config_method_t config_method, bool join, bool auto_mode);
//- TE api requires Provision discovery to be sent
//p2p_prov_disc <peer device address> <display|keypad|pbc> [join|auto]

int8_t slsi_p2p_set(char *field, char *value);
int8_t slsi_p2p_peer(char device_address[SLSI_MACADDR_STR_LEN]);

//void slsi_p2p_reset(void); //-P2P Reset functions calls (bss_flush , remove_network,
// Remove_Group, Remove Persitent group) and brings back to
// the p2p Initialized State

int8_t slsi_p2p_stop(void);

#endif							//CONFIG_SLSI_WIFI_P2P_API

#endif							/* SLSI_WIFI_H_ */
