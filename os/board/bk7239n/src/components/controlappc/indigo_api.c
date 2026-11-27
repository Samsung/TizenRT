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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <os/mem.h>

#include "vendor_specific.h"
#include "indigo_api.h"
#include "utils.h"

/* Structure to initiate the API list and handlers */
struct indigo_api indigo_api_list[] = {
    /* Common */
    { API_CMD_RESPONSE, "CMD_RESPONSE", NULL, NULL },
    { API_CMD_ACK, "CMD_ACK", NULL, NULL },
    /* AP specific */
    { API_AP_START_UP, "AP_START_UP", NULL, NULL },
    { API_AP_STOP, "AP_STOP", NULL, NULL },
    { API_AP_CONFIGURE, "AP_CONFIGURE", NULL, NULL },
    { API_AP_TRIGGER_CHANSWITCH, "AP_TRIGGER_CHANSWITCH", NULL, NULL },
    { API_AP_SEND_DISCONNECT, "AP_SEND_DISCONNECT", NULL, NULL },
    { API_AP_SET_PARAM, "API_AP_SET_PARAM", NULL, NULL },
    { API_AP_SEND_BTM_REQ, "API_AP_SEND_BTM_REQ", NULL, NULL },
    { API_AP_SEND_ARP_MSGS, "API_AP_SEND_ARP_MSGS", NULL, NULL },
    /* Station specific */
    { API_STA_START_UP, "STA_START_UP", NULL, NULL },
    { API_STA_ASSOCIATE, "STA_ASSOCIATE", NULL, NULL },
    { API_STA_CONFIGURE, "STA_CONFIGURE", NULL, NULL },
    { API_STA_DISCONNECT, "STA_DISCONNECT", NULL, NULL },
    { API_STA_SEND_DISCONNECT, "STA_SEND_DISCONNECT", NULL, NULL },
    { API_STA_REASSOCIATE, "STA_REASSOCIATE", NULL, NULL },
    { API_STA_SET_PARAM, "STA_SET_PARAM", NULL, NULL },
    { API_STA_SEND_BTM_QUERY, "STA_SEND_BTM_QUERY", NULL, NULL },
    { API_STA_SEND_ANQP_QUERY, "STA_SEND_ANQP_QUERY", NULL, NULL },
    { API_STA_SET_PHY_MODE, "STA_SET_PHY_MODE", NULL, NULL },
    { API_STA_SET_CHANNEL_WIDTH, "STA_SET_CHANNEL_WIDTH", NULL, NULL },
    { API_STA_POWER_SAVE, "STA_POWER_SAVE", NULL, NULL },
    /* Network operation. E.g., get/set IP address, get MAC address, send the UDP data and reset */
    { API_GET_IP_ADDR, "GET_IP_ADDR", NULL, NULL },
    { API_GET_MAC_ADDR, "GET_MAC_ADDR", NULL, NULL },
    { API_GET_CONTROL_APP_VERSION, "GET_CONTROL_APP_VERSION", NULL, NULL },
    { API_START_LOOP_BACK_SERVER, "START_LOOP_BACK_SERVER", NULL, NULL },
    { API_STOP_LOOP_BACK_SERVER, "STOP_LOOP_BACK_SERVER", NULL, NULL },
    { API_CREATE_NEW_INTERFACE_BRIDGE_NETWORK, "CREATE_NEW_INTERFACE_BRIDGE_NETWORK", NULL, NULL },
    { API_ASSIGN_STATIC_IP, "ASSIGN_STATIC_IP", NULL, NULL },
    { API_DEVICE_RESET, "DEVICE_RESET", NULL, NULL },
    { API_SEND_LOOP_BACK_DATA, "SEND_LOOP_BACK_DATA", NULL, NULL },
    { API_STOP_LOOP_BACK_DATA, "STOP_LOOP_BACK_DATA", NULL, NULL },
};

/* Structure to declare the TLV list */
struct indigo_tlv indigo_tlv_list[] = {
    { TLV_SSID, "SSID" },
    { TLV_CHANNEL, "CHANNEL" },
    { TLV_WEP_KEY0, "WEP_KEY0" },
    { TLV_AUTH_ALGORITHM, "AUTH_ALGORITHM" },
    { TLV_WEP_DEFAULT_KEY, "WEP_DEFAULT_KEY" },
    { TLV_IEEE80211_D, "IEEE80211_D" },
    { TLV_IEEE80211_N, "IEEE80211_N" },
    { TLV_IEEE80211_AC, "IEEE80211_AC" },
    { TLV_COUNTRY_CODE, "COUNTRY_CODE" },
    { TLV_WMM_ENABLED, "WMM_ENABLED" },
    { TLV_WPA, "WPA" },
    { TLV_WPA_KEY_MGMT, "WPA_KEY_MGMT" },
    { TLV_RSN_PAIRWISE, "RSN_PAIRWISE" },
    { TLV_WPA_PASSPHRASE, "WPA_PASSPHRASE" },
    { TLV_WPA_PAIRWISE, "WPA_PAIRWISE" },
    { TLV_HT_CAPB, "HT_CAPB" },
    { TLV_IEEE80211_H, "IEEE80211_H" },
    { TLV_IEEE80211_W, "IEEE80211_W" },
    { TLV_VHT_OPER_CHWIDTH, "VHT_OPER_CHWIDTH" },
    { TLV_VHT_CAPB, "VHT_CAPB" },
    { TLV_IEEE8021_X, "IEEE8021_X" },
    { TLV_EAP_SERVER, "EAP_SERVER" },
    { TLV_AUTH_SERVER_ADDR, "AUTH_SERVER_ADDR" },
    { TLV_AUTH_SERVER_PORT, "AUTH_SERVER_PORT" },
    { TLV_AUTH_SERVER_SHARED_SECRET, "AUTH_SERVER_SHARED_SECRET" },
    { TLV_INTERFACE_NAME, "INTERFACE_NAME" },
    { TLV_NEW_INTERFACE_NAME, "NEW_INTERFACE_NAME" },
    { TLV_FREQUENCY, "FREQUENCY" },
    { TLV_BSS_IDENTIFIER, "BSS_IDENTIFIER" },
    { TLV_HW_MODE, "HW_MODE" },
    { TLV_VHT_OPER_CENTR_FREQ, "VHT_OPER_CENTR_FREQ" },
    { TLV_RESET_TYPE, "RESET_TYPE" },
    { APP_TYPE, "APP_TYPE" },
    { TLV_IE_OVERRIDE, "IE_OVERRIDE" },
    { TLV_ADDRESS, "ADDRESS" },
    { TLV_DISABLE_PMKSA_CACHING, "DISABLE_PMKSA_CACHING" },
    { TLV_SAE_ANTI_CLOGGING_THRESHOLD, "SAE_ANTI_CLOGGING_THRESHOLD" },
    { TLV_STA_SSID, "STA_SSID" },
    { TLV_KEY_MGMT, "KEY_MGMT" },
    { TLV_STA_WEP_KEY0, "STA_WEP_KEY0" },
    { TLV_WEP_TX_KEYIDX, "WEP_TX_KEYIDX" },
    { TLV_GROUP, "GROUP" },
    { TLV_PSK, "PSK" },
    { TLV_PROTO, "PROTO" },
    { TLV_STA_IEEE80211_W, "STA_IEEE80211_W" },
    { TLV_PAIRWISE, "PAIRWISE" },
    { TLV_EAP, "EAP" },
    { TLV_PHASE2, "PHASE2" },
    { TLV_IDENTITY, "IDENTITY" },
    { TLV_PASSWORD, "PASSWORD" },
    { TLV_CA_CERT, "CA_CERT" },
    { TLV_PHASE1, "PHASE1" },
    { TLV_CLIENT_CERT, "CLIENT_CERT" },
    { TLV_PRIVATE_KEY, "PRIVATE_KEY" },
    { TLV_STA_POWER_SAVE, "STA_POWER_SAVE" },
    { TLV_STATIC_IP, "STATIC_IP" },
    { TLV_DEBUG_LEVEL, "DEBUG_LEVEL" },
    { TLV_DUT_IP_ADDRESS, "DUT_IP_ADDRESS" },
    { TLV_HOSTAPD_FILE_NAME, "HOSTAPD_FILE_NAME" },
    { TLV_ROLE, "ROLE" },
    { TLV_BAND, "BAND" },
    { TLV_BSSID, "BSSID" },
    { TLV_ARP_TRANSMISSION_RATE, "ARP_TRANSMISSION_RATE" },
    { TLV_ARP_TARGET_IP, "ARP_TARGET_IP" },
    { TLV_ARP_FRAME_COUNT, "ARP_FRAME_COUNT" },
    { TLV_PACKET_COUNT, "PACKET_COUNT" },
    { TLV_PACKET_TYPE, "PACKET_TYPE" },
    { TLV_PACKET_RATE, "PACKET_RATE" },
    { TLV_PHYMODE, "PHYMODE" },
    { TLV_CHANNEL_WIDTH, "CHANNEL_WIDTH" },
    { TLV_PAC_FILE, "PAC_FILE" },
    { TLV_STA_SAE_GROUPS, "STA_SAE_GROUPS" },
    { TLV_SAE_GROUPS, "SAE_GROUPS" },
    { TLV_IEEE80211_AX, "IEEE80211_AX" },
    { TLV_HE_OPER_CHWIDTH, "HE_OPER_CHWIDTH" },
    { TLV_HE_OPER_CENTR_FREQ, "HE_OPER_CENTR_FREQ" },
    { TLV_MBO, "MBO" },
    { TLV_MBO_CELL_DATA_CONN_PREF, "MBO_CELL_DATA_CONN_PREF" },
    { TLV_BSS_TRANSITION, "BSS_TRANSITION" },
    { TLV_INTERWORKING, "INTERWORKING" },
    { TLV_RRM_NEIGHBOR_REPORT, "RRM_NEIGHBOR_REPORT" },
    { TLV_RRM_BEACON_REPORT, "RRM_BEACON_REPORT" },
    { TLV_COUNTRY3, "COUNTRY3" },
    { TLV_MBO_CELL_CAPA, "MBO_CELL_CAPA" },
    { TLV_DOMAIN_MATCH, "TLV_DOMAIN_MATCH" },
    { TLV_DOMAIN_SUFFIX_MATCH, "TLV_DOMAIN_SUFFIX_MATCH" },
    { TLV_MBO_ASSOC_DISALLOW, "TLV_MBO_ASSOC_DISALLOW" },
    { TLV_DISASSOC_IMMINENT, "TLV_DISASSOC_IMMINENT" },
    { TLV_BSS_TERMINATION, "TLV_BSS_TERMINATION" },
    { TLV_DISASSOC_TIMER, "TLV_DISASSOC_TIMER" },
    { TLV_BSS_TERMINATION_TSF, "TLV_BSS_TERMINATION_TSF" },
    { TLV_BSS_TERMINATION_DURATION, "TLV_BSS_TERMINATION_DURATION" },
    { TLV_REASSOCIAITION_RETRY_DELAY, "TLV_REASSOCIAITION_RETRY_DELAY" },
    { TLV_BTMQUERY_REASON_CODE, "TLV_BTMQUERY_REASON_CODE" },
    { TLV_CANDIDATE_LIST, "TLV_CANDIDATE_LIST" },
    { TLV_ANQP_INFO_ID, "TLV_ANQP_INFO_ID" },
    { TLV_GAS_COMEBACK_DELAY, "TLV_GAS_COMEBACK_DELAY" },
    { TLV_SAE_PWE, "TLV_SAE_PWE" },
    { TLV_OWE_GROUPS, "TLV_OWE_GROUPS" },
    { TLV_STA_OWE_GROUP, "TLV_STA_OWE_GROUP" },
    { TLV_HE_MU_EDCA, "TLV_HE_MU_EDCA" },
    { TLV_RSNXE_OVERRIDE_EAPOL, "TLV_RSNXE_OVERRIDE_EAPOL" },
    { TLV_TRANSITION_DISABLE, "TLV_TRANSITION_DISABLE" },
    { TLV_SAE_CONFIRM_IMMEDIATE, "SAE_CONFIRM_IMMEDIATE" },
    { TLV_SERVER_CERT, "SERVER_CERT" },
    { TLV_CONTROL_INTERFACE, "CONTROL_INTERFACE" },
    { TLV_PACKET_SIZE, "PACKET_SIZE" },
    { TLV_DUT_UDP_PORT, "DUT_UDP_PORT" },
    { TLV_OWE_TRANSITION_BSS_IDENTIFIER, "OWE_TRANSITION_BSS_IDENTIFIER" },
    { TLV_MESSAGE, "MESSAGE" },
    { TLV_STATUS, "STATUS" },
    { TLV_DUT_WLAN_IP_ADDR, "DUT_WLAN_IP_ADDR" },
    { TLV_DUT_MAC_ADDR, "DUT_MAC_ADDR" },
    { TLV_CONTROL_APP_VERSION, "CONTROL_APP_VERSION" },
    { TLV_FREQ_LIST, "FREQ_LIST" },
    { TLV_OP_CLASS, "OP_CLASS" },
    { TLV_HE_6G_ONLY, "HE_6G_ONLY"},
    { TLV_HE_UNSOL_PR_RESP_CADENCE, "UNSOL_PR_RESP_CADENCE" },
    { TLV_HE_FILS_DISCOVERY_TX, "FILS_DISCOERY_TX" },
    { TLV_SKIP_6G_BSS_SECURITY_CHECK, "SKIP_6G_BSS_SECURITY_CHECK" },
    { TLV_RAND_MAC_ADDR, "RAND_MAC_ADDR" },
    { TLV_PREASSOC_RAND_MAC_ADDR, "PREASSOC_RAND_MAC_ADDR" },
    { TLV_RAND_ADDR_LIFETIME, "RAND_ADDR_LIFETIME" },
};

/* Find the type of the API stucture by the ID from the list */
char* get_api_type_by_id(int id) {
    int i = 0;
    for (i = 0; i < sizeof(indigo_api_list)/sizeof(struct indigo_api); i++) {
        if (id == indigo_api_list[i].type) {
            return indigo_api_list[i].name;
        }
    }
    return "Unknown";
}

/* Find the API stucture by the ID from the list */
struct indigo_api* get_api_by_id(int id) {
    int i = 0;
    for (i = 0; i < sizeof(indigo_api_list)/sizeof(struct indigo_api); i++) {
        if (id == indigo_api_list[i].type) {
            return &indigo_api_list[i];
        }
    }
    return NULL;
}

/* Find the TLV by the ID from the list */
struct indigo_tlv* get_tlv_by_id(int id) {
    int i = 0;

    for (i = 0; i < sizeof(indigo_tlv_list)/sizeof(struct indigo_tlv); i++) {
        if (id == indigo_tlv_list[i].id) {
            return &indigo_tlv_list[i];
        }
    }
    return NULL;
}

/* The generic function generates the ACK/NACK response */
/* seq:    integer which should match the sequence of the request */
/* status: 0 - ACK, 1 - NACK */
/* reason: string for the reason code */
void fill_wrapper_ack(struct packet_wrapper *wrapper, int seq, int status, char *reason) {
    wrapper->hdr.version = API_VERSION;
    wrapper->hdr.type = API_CMD_ACK;
    wrapper->hdr.seq = seq;
    wrapper->hdr.reserved = API_RESERVED_BYTE;
    wrapper->hdr.reserved2 = API_RESERVED_BYTE;

    wrapper->tlv_num =  2;
    wrapper->tlv[0] = os_malloc(sizeof(struct tlv_hdr));
    wrapper->tlv[0]->id = TLV_STATUS;
    wrapper->tlv[0]->len = 1;
    wrapper->tlv[0]->value = (unsigned char*)os_malloc(wrapper->tlv[0]->len);
    wrapper->tlv[0]->value[0] = status;

    wrapper->tlv[1] = os_malloc(sizeof(struct tlv_hdr));
    wrapper->tlv[1]->id = TLV_MESSAGE;
    wrapper->tlv[1]->len = strlen(reason);
    wrapper->tlv[1]->value = (unsigned char*)os_malloc(wrapper->tlv[1]->len);
    memcpy(wrapper->tlv[1]->value, reason, wrapper->tlv[1]->len);
}

/* Provide the function to register the API handler */
void register_api(int id, api_callback_func verify, api_callback_func handle) {
    struct indigo_api *api = NULL;

    api = get_api_by_id(id);
    if (api) {
        api->verify = verify;
        api->handle = handle;
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "API 0x%04x has no callback function", id);
    }
}

/* Fill the message header structure to the wrapper */
void fill_wrapper_message_hdr(struct packet_wrapper *wrapper, int msg_type, int seq) {
    wrapper->hdr.version = API_VERSION;
    wrapper->hdr.type = msg_type;
    wrapper->hdr.seq = seq;
    wrapper->hdr.reserved = API_RESERVED_BYTE;
    wrapper->hdr.reserved2 = API_RESERVED_BYTE;
}

/* Fill the TLV structure to the wrapper (for one byte value) */
void fill_wrapper_tlv_byte(struct packet_wrapper *wrapper, int id, char value) {
    wrapper->tlv[wrapper->tlv_num] = os_malloc(sizeof(struct tlv_hdr));
    wrapper->tlv[wrapper->tlv_num]->id = id;
    wrapper->tlv[wrapper->tlv_num]->len = 1;
    wrapper->tlv[wrapper->tlv_num]->value = (unsigned char*)os_malloc(1);
    wrapper->tlv[wrapper->tlv_num]->value[0] = value;
    wrapper->tlv_num++;
}

/* Fill the TLV structure to the wrapper (for multiple bytes value) */
void fill_wrapper_tlv_bytes(struct packet_wrapper *wrapper, int id, int len, char* value) {
    wrapper->tlv[wrapper->tlv_num] = os_malloc(sizeof(struct tlv_hdr));
    wrapper->tlv[wrapper->tlv_num]->id = id;
    wrapper->tlv[wrapper->tlv_num]->len = len;
    wrapper->tlv[wrapper->tlv_num]->value = (unsigned char*)os_malloc(len);
    memcpy(wrapper->tlv[wrapper->tlv_num]->value, value, len);
    wrapper->tlv_num++;
}
