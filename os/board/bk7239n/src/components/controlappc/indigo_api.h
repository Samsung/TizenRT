/* Copyright (c) 2020 Wi-Fi Alliance                                                */

/* Permission to use, copy, modify, and/or distribute this software for any         */
/* purpose with or without fee is hereby granted, provided that the above           */
/* copyright notice and this permission notice appear in all copies.                */

/* THE SOFTWARE IS PROVIDED 'AS IS' AND THE AUTHOR DISCLAIMS ALL                    */
/* WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                    */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL                     */
/* THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR                       */
/* CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING                        */
/* FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF                       */
/* CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT                       */
/* OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS                          */
/* SOFTWARE. */

#ifndef _INDIGO_API_
#define _INDIGO_API_
#include "indigo_packet.h"

/* API */
#define NAME_SIZE     64

struct indigo_tlv {
    unsigned short id;
    char name[NAME_SIZE];
};

struct indigo_api {
    unsigned short type;
    char name[NAME_SIZE];
    int (*verify)(struct packet_wrapper *req, struct packet_wrapper *resp);
    int (*handle)(struct packet_wrapper *req, struct packet_wrapper *resp);
};

/* API definition */
#define API_VERSION                             0x01
#define API_RESERVED_BYTE                       0xff

/* Message type definition */
#define API_CMD_RESPONSE                        0x0000
#define API_CMD_ACK                             0x0001

#define API_AP_START_UP                         0x1000
#define API_AP_STOP                             0x1001
#define API_AP_CONFIGURE                        0x1002
#define API_AP_TRIGGER_CHANSWITCH               0x1003
#define API_AP_SEND_DISCONNECT                  0x1004
#define API_AP_SET_PARAM                        0x1005
#define API_AP_SEND_BTM_REQ                     0x1006
#define API_AP_SEND_ARP_MSGS                    0x1007

#define API_STA_ASSOCIATE                       0x2000
#define API_STA_CONFIGURE                       0x2001
#define API_STA_DISCONNECT                      0x2002
#define API_STA_SEND_DISCONNECT                 0x2003
#define API_STA_REASSOCIATE                     0x2004
#define API_STA_SET_PARAM                       0x2005
#define API_STA_SEND_BTM_QUERY                  0x2006
#define API_STA_SEND_ANQP_QUERY                 0x2007
#define API_STA_START_UP                        0x2008
#define API_STA_SET_PHY_MODE                    0x2009
#define API_STA_SET_CHANNEL_WIDTH               0x200a
#define API_STA_POWER_SAVE                      0x200b

#define API_GET_IP_ADDR                         0x5000
#define API_GET_MAC_ADDR                        0x5001
#define API_GET_CONTROL_APP_VERSION             0x5002
#define API_START_LOOP_BACK_SERVER              0x5003
#define API_STOP_LOOP_BACK_SERVER               0x5004
#define API_CREATE_NEW_INTERFACE_BRIDGE_NETWORK 0x5005
#define API_ASSIGN_STATIC_IP                    0x5006
#define API_DEVICE_RESET                        0x5007
#define API_SEND_LOOP_BACK_DATA                 0x5008
#define API_STOP_LOOP_BACK_DATA                 0x5009

/* TLV definition */
#define TLV_SSID                                0x0001
#define TLV_CHANNEL                             0x0002
#define TLV_WEP_KEY0                            0x0003
#define TLV_AUTH_ALGORITHM                      0x0004
#define TLV_WEP_DEFAULT_KEY                     0x0005
#define TLV_IEEE80211_D                         0x0006
#define TLV_IEEE80211_N                         0x0007
#define TLV_IEEE80211_AC                        0x0008
#define TLV_COUNTRY_CODE                        0x0009
#define TLV_WMM_ENABLED                         0x000a
#define TLV_WPA                                 0x000b
#define TLV_WPA_KEY_MGMT                        0x000c
#define TLV_RSN_PAIRWISE                        0x000d
#define TLV_WPA_PASSPHRASE                      0x000e
#define TLV_WPA_PAIRWISE                        0x000f
#define TLV_HT_CAPB                             0x0010
#define TLV_IEEE80211_H                         0x0011
#define TLV_IEEE80211_W                         0x0012
#define TLV_VHT_OPER_CHWIDTH                    0x0013
#define TLV_VHT_CAPB                            0x0014
#define TLV_IEEE8021_X                          0x0015
#define TLV_EAP_SERVER                          0x0016
#define TLV_AUTH_SERVER_ADDR                    0x0017
#define TLV_AUTH_SERVER_PORT                    0x0018
#define TLV_AUTH_SERVER_SHARED_SECRET           0x0019
#define TLV_INTERFACE_NAME                      0x001a
#define TLV_NEW_INTERFACE_NAME                  0x001b
#define TLV_FREQUENCY                           0x001c
#define TLV_BSS_IDENTIFIER                      0x001d
#define TLV_HW_MODE                             0x001e
#define TLV_VHT_OPER_CENTR_FREQ                 0x001f
#define TLV_RESET_TYPE                          0x0020
#define APP_TYPE                                0x0021
#define TLV_OP_CLASS                            0x0022
#define TLV_IE_OVERRIDE                         0x0023
#define TLV_ADDRESS                             0x0028
#define TLV_DISABLE_PMKSA_CACHING               0x0033
#define TLV_SAE_ANTI_CLOGGING_THRESHOLD         0x0034
#define TLV_STA_SSID                            0x0035
#define TLV_KEY_MGMT                            0x0036
#define TLV_STA_WEP_KEY0                        0x0037
#define TLV_WEP_TX_KEYIDX                       0x0038
#define TLV_GROUP                               0x0039
#define TLV_PSK                                 0x003a
#define TLV_PROTO                               0x003b
#define TLV_STA_IEEE80211_W                     0x003c
#define TLV_PAIRWISE                            0x003d
#define TLV_EAP                                 0x003e
#define TLV_PHASE2                              0x003f
#define TLV_IDENTITY                            0x0040
#define TLV_PASSWORD                            0x0041
#define TLV_CA_CERT                             0x0042
#define TLV_PHASE1                              0x0043
#define TLV_CLIENT_CERT                         0x0044
#define TLV_PRIVATE_KEY                         0x0045
#define TLV_STA_POWER_SAVE                      0x0052
#define TLV_STATIC_IP                           0x0055
#define TLV_DEBUG_LEVEL                         0x0057
#define TLV_DUT_IP_ADDRESS                      0x0058
#define TLV_HOSTAPD_FILE_NAME                   0x0059
#define TLV_ROLE                                0x005c
#define TLV_BAND                                0x005d
#define TLV_BSSID                               0x005e
#define TLV_ARP_TRANSMISSION_RATE               0x005f
#define TLV_ARP_TARGET_IP                       0x0060
#define TLV_ARP_FRAME_COUNT                     0x0062
#define TLV_PACKET_COUNT                        0x0067
#define TLV_PACKET_TYPE                         0x0068
#define TLV_PACKET_RATE                         0x0069
#define TLV_PHYMODE                             0x006a
#define TLV_CHANNEL_WIDTH                       0x006b
#define TLV_PAC_FILE                            0x006d
#define TLV_STA_SAE_GROUPS                      0x006e
#define TLV_SAE_GROUPS                          0x0071
#define TLV_IEEE80211_AX                        0x0072
#define TLV_HE_OPER_CHWIDTH                     0x0073
#define TLV_HE_OPER_CENTR_FREQ                  0x0074
#define TLV_MBO                                 0x0075
#define TLV_MBO_CELL_DATA_CONN_PREF             0x0076
#define TLV_BSS_TRANSITION                      0x0077
#define TLV_INTERWORKING                        0x0078
#define TLV_RRM_NEIGHBOR_REPORT                 0x0079
#define TLV_RRM_BEACON_REPORT                   0x007a
#define TLV_COUNTRY3                            0x007b
#define TLV_MBO_CELL_CAPA                       0x007c
#define TLV_DOMAIN_MATCH                        0x007d
#define TLV_DOMAIN_SUFFIX_MATCH                 0x007e
#define TLV_MBO_ASSOC_DISALLOW                  0x007f
#define TLV_DISASSOC_IMMINENT                   0x0081
#define TLV_BSS_TERMINATION                     0x0082
#define TLV_DISASSOC_TIMER                      0x0083
#define TLV_BSS_TERMINATION_TSF                 0x0084
#define TLV_BSS_TERMINATION_DURATION            0x0085
#define TLV_REASSOCIAITION_RETRY_DELAY          0x0086
#define TLV_BTMQUERY_REASON_CODE                0x0087
#define TLV_CANDIDATE_LIST                      0x0088
#define TLV_ANQP_INFO_ID                        0x0089
#define TLV_GAS_COMEBACK_DELAY                  0x008a
#define TLV_SAE_PWE                             0x008d
#define TLV_OWE_GROUPS                          0x008e
#define TLV_STA_OWE_GROUP                       0x008f
#define TLV_HE_MU_EDCA                          0x0090
#define TLV_RSNXE_OVERRIDE_EAPOL                0x0092
#define TLV_TRANSITION_DISABLE                  0x0093
#define TLV_SAE_CONFIRM_IMMEDIATE               0x0094
#define TLV_RAND_MAC_ADDR                       0x0095
#define TLV_PREASSOC_RAND_MAC_ADDR              0x0096
#define TLV_RAND_ADDR_LIFETIME                  0x0097
#define TLV_DROP_SA                             0x0098
#define TLV_SERVER_CERT                         0x0099

#define TLV_CONTROL_INTERFACE                   0x009c
#define TLV_PACKET_SIZE                         0x009d
#define TLV_DUT_UDP_PORT                        0x009e
#define TLV_SKIP_6G_BSS_SECURITY_CHECK          0x00a1
#define TLV_OWE_TRANSITION_BSS_IDENTIFIER       0x00a2
#define TLV_FREQ_LIST                           0x00a3
#define TLV_SHORT_SSID                          0x00a4
#define TLV_HE_BEACON_TX_SU_PPDU                0x00a5
#define TLV_HE_6G_ONLY                          0x00a6
#define TLV_HE_UNSOL_PR_RESP_CADENCE            0x00a7
#define TLV_HE_FILS_DISCOVERY_TX                0x00a8

// class ResponseTLV
// List of TLV used in the QuickTrack API response and ACK messages from the DUT
#define TLV_MESSAGE                             0xa000
#define TLV_STATUS                              0xa001
#define TLV_DUT_WLAN_IP_ADDR                    0xa002
#define TLV_DUT_MAC_ADDR                        0xa003
#define TLV_CONTROL_APP_VERSION                 0xa004
#define TLV_LOOP_BACK_DATA_RECEIVED             0xa005
#define TLV_LOOP_BACK_DATA_SENT                 0xa006
#define TLV_ARP_RECV_NUM                        0xa007
#define TLV_TEST_PLATFORM_APP_VERSION           0xa008
#define TLV_LOOP_BACK_SERVER_PORT               0xa009

/* TLV Value */
#define DUT_TYPE_STAUT                          0x01
#define DUT_TYPE_APUT                           0x02

#define TLV_BAND_24GHZ                          "2.4GHz"
#define TLV_BAND_5GHZ                           "5GHz"
#define TLV_BAND_6GHZ                           "6GHz"

#define TLV_VALUE_APP_VERSION                   "v1.0"
#define TLV_VALUE_TEST_PLATFORM_APP_VERSION     "v1.0"
#define TLV_VALUE_OK                            "OK"
#define TLV_VALUE_NOT_OK                        "Failed"
#define TLV_VALUE_INSUFFICIENT_TLV              "TLV is insufficient to run the command"
#define TLV_VALUE_STATUS_OK                     0x30
#define TLV_VALUE_STATUS_NOT_OK                 0x31
#define TLV_VALUE_LOOP_BACK_STOP_OK             "Loopback server in idle state"
#define TLV_VALUE_HOSTAPD_STOP_OK               "AP stop completed : Hostapd service is inactive."
#define TLV_VALUE_HOSTAPD_STOP_NOT_OK           "Failed to stop hostapd service."
#define TLV_VALUE_WPA_SET_PARAMETER_OK          "Set parameter action was successful."
#define TLV_VALUE_WPA_SET_PARAMETER_NO_OK       "Failed to set parameter."
#define TLV_VALUE_WPA_PARAMETER_NOT_SUPPORT     "The set parameter is not supported"
#define TLV_VALUE_HOSTAPD_START_OK              "AP is up : Hostapd service is active"
#define TLV_VALUE_ASSIGN_STATIC_IP_OK           "Static IP successfully assigned to wireless interface"
#define TLV_VALUE_ASSIGN_STATIC_IP_NOT_OK       "Static IP failed to be assigned to wireless interface"
#define TLV_VALUE_LOOPBACK_SVR_START_OK         "Loop back server initialized"
#define TLV_VALUE_LOOPBACK_SVR_START_NOT_OK     "Failed to initialise loop back server"
#define TLV_VALUE_SEND_LOOPBACK_DATA_OK         "Send Loop back data successful"
#define TLV_VALUE_SEND_LOOPBACK_DATA_NOT_OK     "Send Loop back data failed"
#define TLV_VALUE_WIRELESS_INTERFACE_NOT_OK     "Wireless interface is not available"
#define TLV_VALUE_HOSTAPD_CTRL_NOT_OK           "Failed to connect to hostapd control interface"
#define TLV_VALUE_HOSTAPD_NOT_OK                "Failed to find hostapd PID"
#define TLV_VALUE_HOSTAPD_RESP_NOT_OK           "Hostapd response is failed"
#define TLV_VALUE_BROADCAST_ARP_TEST_OK         "Broadcast ARP test successful"
#define TLV_VALUE_BROADCAST_ARP_TEST_NOT_OK     "Broadcast ARP test failed"
#define TLV_VALUE_CREATE_BRIDGE_OK              "Bridge network is created successfully"
#define TLV_VALUE_CREATE_BRIDGE_NOT_OK          "Failed to create bridge network"

#define TLV_VALUE_WPA_S_START_UP_OK             "wpa_supplicant is initialized successfully"
#define TLV_VALUE_WPA_S_START_UP_NOT_OK         "The wpa_supplicant was unable to initialize."
#define TLV_VALUE_WPA_S_STOP_NOT_OK             "Failed to stop wpa supplicant service."
#define TLV_VALUE_WPA_S_STOP_OK                 "QuickTrack tool STA was successfully disconnected"
#define TLV_VALUE_WPA_S_ASSOC_OK                "STA is up: WPA supplicant associated"
#define TLV_VALUE_WPA_S_ASSOC_NOT_OK            "WPA supplicant cannot associate with AP"
#define TLV_VALUE_WPA_S_DISCONNECT_OK           "Sent DISCONNECT message"
#define TLV_VALUE_WPA_S_DISCONNECT_NOT_OK       "Failed to send DISCONNECT message"
#define TLV_VALUE_WPA_S_RECONNECT_OK            "Sent RECONNECT message"
#define TLV_VALUE_WPA_S_RECONNECT_NOT_OK        "Failed to send RECONNECT message"
#define TLV_VALUE_WPA_S_CTRL_NOT_OK             "Failed to connect to WPA supplicant control interface"
#define TLV_VALUE_WPA_S_BTM_QUERY_OK            "Sent WNM_BSS_QUERY"
#define TLV_VALUE_WPA_S_BTM_QUERY_NOT_OK        "Failed to WNM_BSS_QUERY"
#define TLV_VALUE_RESET_OK                      "Device reset successfully"
#define TLV_VALUE_RESET_NOT_OK                  "Failed to run Device reset"
#define TLV_VALUE_POWER_SAVE_OK                 "Set power save value successfully"
#define TLV_VALUE_POWER_SAVE_NOT_OK             "Failed to set power save value"

#define RESET_TYPE_INIT                         0x01
#define RESET_TYPE_TEARDOWN                     0x02

#define WPA_CTRL_OK                             "OK"

struct indigo_api* get_api_by_id(int id);
struct indigo_tlv* get_tlv_by_id(int id);
char* get_api_type_by_id(int id);

typedef int (*api_callback_func)(struct packet_wrapper *req, struct packet_wrapper *resp);
void register_api(int id, api_callback_func verify, api_callback_func handle);

void fill_wrapper_ack(struct packet_wrapper *wrapper, int seq, int status, char *reason);

void register_api(int id, api_callback_func verify, api_callback_func handle);
void fill_wrapper_message_hdr(struct packet_wrapper *wrapper, int msg_type, int seq);
void fill_wrapper_tlv_byte(struct packet_wrapper *wrapper, int id, char value);
void fill_wrapper_tlv_bytes(struct packet_wrapper *wrapper, int id, int len, char* value);

/* Solution Vendor */
void register_apis();
#endif // __INDIGO_API_
