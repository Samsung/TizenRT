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

#ifndef _INDIGO_API_CALLBACK
#define _INDIGO_API_CALLBACK


#define LOOPBACK_TIMEOUT 180


struct tlv_to_config_name {
    unsigned short tlv_id;
    char config_name[NAME_SIZE];
    int quoted;
};

struct tlv_to_config_name maps[] = {
    /* hapds */
    { TLV_SSID, "ssid", 0 },
    { TLV_CHANNEL, "channel", 0 },
    { TLV_WEP_KEY0, "wep_key0", 0 },
    { TLV_HW_MODE, "hw_mode", 0 },
    { TLV_AUTH_ALGORITHM, "auth_algs", 0 },
    { TLV_WEP_DEFAULT_KEY, "wep_default_key", 0 },
    { TLV_IEEE80211_D, "ieee80211d", 0 },
    { TLV_IEEE80211_N, "ieee80211n", 0 },
    { TLV_IEEE80211_AC, "ieee80211ac", 0 },
    { TLV_COUNTRY_CODE, "country_code", 0 },
    { TLV_WMM_ENABLED, "wmm_enabled", 0 },
    { TLV_WPA, "wpa", 0 },
    { TLV_WPA_KEY_MGMT, "wpa_key_mgmt", 0 },
    { TLV_RSN_PAIRWISE, "rsn_pairwise", 0 },
    { TLV_WPA_PASSPHRASE, "wpa_passphrase", 0 },
    { TLV_WPA_PAIRWISE, "wpa_pairwise", 0 },
    { TLV_HT_CAPB, "ht_capab", 0 },
    { TLV_IEEE80211_W, "ieee80211w", 0 },
    { TLV_IEEE80211_H, "ieee80211h", 0 },
    { TLV_VHT_OPER_CHWIDTH, "vht_oper_chwidth", 0 },
    { TLV_VHT_OPER_CENTR_FREQ, "vht_oper_centr_freq_seg0_idx", 0 },
    { TLV_VHT_CAPB, "vht_capab", 0 },
    { TLV_IEEE8021_X, "ieee8021x", 0 },
    { TLV_EAP_SERVER, "eap_server", 0 },
    { TLV_AUTH_SERVER_ADDR, "auth_server_addr", 0 },
    { TLV_AUTH_SERVER_PORT, "auth_server_port", 0 },
    { TLV_AUTH_SERVER_SHARED_SECRET, "auth_server_shared_secret", 0 },
    { TLV_IE_OVERRIDE, "own_ie_override", 0 }, // HostAPD Python Interface
    { TLV_SAE_ANTI_CLOGGING_THRESHOLD, "sae_anti_clogging_threshold", 0 }, // HostAPD Python Interface
    { TLV_DISABLE_PMKSA_CACHING, "disable_pmksa_caching", 0 },  // HostAPD Python Interface
    { TLV_SAE_GROUPS, "sae_groups", 0 },
    { TLV_IEEE80211_AX, "ieee80211ax", 0 },
    { TLV_HE_OPER_CHWIDTH, "he_oper_chwidth", 0 },
    { TLV_HE_OPER_CENTR_FREQ, "he_oper_centr_freq_seg0_idx", 0 },
    { TLV_MBO, "mbo", 0 },
    { TLV_MBO_CELL_DATA_CONN_PREF, "mbo_cell_data_conn_pref", 0 },
    { TLV_BSS_TRANSITION, "bss_transition", 0 },
    { TLV_INTERWORKING, "interworking", 0 },
    { TLV_RRM_NEIGHBOR_REPORT, "rrm_neighbor_report", 0 },
    { TLV_RRM_BEACON_REPORT, "rrm_beacon_report", 0 },
    { TLV_COUNTRY3, "country3", 0 },
    { TLV_MBO_CELL_CAPA, "mbo_cell_capa", 0 },
    { TLV_MBO_ASSOC_DISALLOW, "mbo_assoc_disallow", 0 },
    { TLV_GAS_COMEBACK_DELAY, "gas_comeback_delay", 0 },
    { TLV_SAE_PWE, "sae_pwe", 0 },
    { TLV_OWE_GROUPS, "owe_groups", 0 },
    { TLV_HE_MU_EDCA, "he_mu_edca_qos_info_param_count", 0 },
    { TLV_TRANSITION_DISABLE, "transition_disable", 0 },
    { TLV_CONTROL_INTERFACE, "ctrl_interface", 0 },
    { TLV_RSNXE_OVERRIDE_EAPOL, "rsnxe_override_eapol", 0 },
    { TLV_SAE_CONFIRM_IMMEDIATE, "sae_confirm_immediate", 0 },
    { TLV_OWE_TRANSITION_BSS_IDENTIFIER, "owe_transition_ifname", 0 },
    { TLV_OP_CLASS, "op_class", 0 },
    { TLV_HE_UNSOL_PR_RESP_CADENCE, "unsol_bcast_probe_resp_interval", 0 },
    { TLV_HE_FILS_DISCOVERY_TX, "fils_discovery_max_interval", 0 },
    { TLV_SKIP_6G_BSS_SECURITY_CHECK, "skip_6g_bss_security_check", 0 },

    /* wpas, seperate? */
    { TLV_STA_SSID, "ssid", 1 },
    { TLV_KEY_MGMT, "key_mgmt", 0 },
    { TLV_STA_WEP_KEY0, "wep_key0", 0 },
    { TLV_WEP_TX_KEYIDX, "wep_tx_keyidx", 0 },
    { TLV_GROUP, "group", 0 },
    { TLV_PSK, "psk", 1 },
    { TLV_PROTO, "proto", 0 },
    { TLV_STA_IEEE80211_W, "ieee80211w", 0 },
    { TLV_PAIRWISE, "pairwise", 0 },
    { TLV_EAP, "eap", 0 },
    { TLV_PHASE1, "phase1", 1 },
    { TLV_PHASE2, "phase2", 1 },
    { TLV_IDENTITY, "identity", 1 },
    { TLV_PASSWORD, "password", 1 },
    { TLV_CA_CERT, "ca_cert", 1 },
    { TLV_SERVER_CERT, "ca_cert", 1 },
    { TLV_PRIVATE_KEY, "private_key", 1 },
    { TLV_CLIENT_CERT, "client_cert", 1 },
    { TLV_DOMAIN_MATCH, "domain_match", 1 },
    { TLV_DOMAIN_SUFFIX_MATCH, "domain_suffix_match", 1 },
    { TLV_PAC_FILE, "pac_file", 1 },
    { TLV_STA_OWE_GROUP, "owe_group", 0 },
    { TLV_BSSID, "bssid", 0 },
};

char* find_tlv_config_name(int tlv_id) {
    int i;
    for (i = 0; i < sizeof(maps)/sizeof(struct tlv_to_config_name); i++) {
        if (tlv_id == maps[i].tlv_id) {
            return maps[i].config_name;
        }
    }
    return NULL;
}

struct tlv_to_config_name* find_tlv_config(int tlv_id) {
    int i;
    for (i = 0; i < sizeof(maps)/sizeof(struct tlv_to_config_name); i++) {
        if (tlv_id == maps[i].tlv_id) {
            return &maps[i];
        }
    }
    return NULL;
}

struct tlv_to_config_name wpas_global_maps[] = {
    { TLV_STA_SAE_GROUPS, "sae_groups", 0 },
    { TLV_MBO_CELL_CAPA, "mbo_cell_capa", 0 },
    { TLV_SAE_PWE, "sae_pwe", 0 },
    { TLV_CONTROL_INTERFACE, "ctrl_interface", 0 },
    { TLV_RAND_MAC_ADDR, "mac_addr", 0 },
    { TLV_PREASSOC_RAND_MAC_ADDR, "preassoc_mac_addr", 0 },
    { TLV_RAND_ADDR_LIFETIME, "rand_addr_lifetime", 0 },
};

struct tlv_to_config_name* find_wpas_global_config_name(int tlv_id) {
    int i;
    for (i = 0; i < sizeof(wpas_global_maps)/sizeof(struct tlv_to_config_name); i++) {
        if (tlv_id == wpas_global_maps[i].tlv_id) {
            return &wpas_global_maps[i];
        }
    }
    return NULL;
}


/* Basic */
static int get_control_app_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int start_loopback_server(struct packet_wrapper *req, struct packet_wrapper *resp);
static int stop_loop_back_server_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
#ifdef _TEST_PLATFORM_
static int send_loopback_data_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int stop_loopback_data_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
#endif
static int create_bridge_network_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int assign_static_ip_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int get_mac_addr_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int get_ip_addr_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int reset_device_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
/* AP */
static int stop_ap_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int configure_ap_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int start_ap_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int send_ap_disconnect_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int set_ap_parameter_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int send_ap_btm_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int trigger_ap_channel_switch(struct packet_wrapper *req, struct packet_wrapper *resp);
#ifdef _TEST_PLATFORM_
static int send_ap_arp_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
#endif
/* STA */
static int stop_sta_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int configure_sta_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int associate_sta_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
#ifdef _TEST_PLATFORM_
static int start_up_sta_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
#endif
static int send_sta_disconnect_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int send_sta_reconnect_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int send_sta_btm_query_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int send_sta_anqp_query_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int set_sta_parameter_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
#ifdef _TEST_PLATFORM_
static int set_sta_phy_mode_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int set_sta_channel_width_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
static int set_sta_power_save_handler(struct packet_wrapper *req, struct packet_wrapper *resp);
#endif
#endif // __INDIGO_API_CALLBACK
