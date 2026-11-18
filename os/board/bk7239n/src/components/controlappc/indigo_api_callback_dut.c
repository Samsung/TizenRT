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
#include <unistd.h>

#include "indigo_api.h"
#include "vendor_specific.h"
#include "utils.h"
#include "wpa_ctrl.h"
#include "indigo_api_callback.h"
#ifdef BEKEN_API
#include <common/bk_include.h>
#include "bk_netif_addr.h"
#include <os/str.h>
#include <os/mem.h>
#include <arpa/inet.h>
/* wpa defs.h uses BIT() in enum initializers. A prior BIT() that is not an
 * integer constant expression (or a missing BIT()) makes those enums fail. */
#undef BIT
#define BIT(x) (1U << (x))
#include "common/defs.h"
#include <modules/wifi.h>
#include "bk_wifi.h"
#include <components/system.h>
#endif

extern int cmd_wpas_parse_key_mgmt(const char *value);
extern int cmd_wpas_parse_cipher(const char *value);
extern int cmd_wpas_parse_proto(const char *value);

//static char pac_file_path[S_BUFFER_LEN] = {0};
struct interface_info* band_transmitter[16];

static network_InitTypeDef_st quickTrackNetwork;

void register_apis() {
    /* Basic */
    register_api(API_GET_IP_ADDR, NULL, get_ip_addr_handler);
    register_api(API_GET_MAC_ADDR, NULL, get_mac_addr_handler);
    register_api(API_GET_CONTROL_APP_VERSION, NULL, get_control_app_handler);
    register_api(API_START_LOOP_BACK_SERVER, NULL, start_loopback_server);
    register_api(API_STOP_LOOP_BACK_SERVER, NULL, stop_loop_back_server_handler);
    register_api(API_CREATE_NEW_INTERFACE_BRIDGE_NETWORK, NULL, create_bridge_network_handler); /* deprecated */
    register_api(API_ASSIGN_STATIC_IP, NULL, assign_static_ip_handler);
    register_api(API_DEVICE_RESET, NULL, reset_device_handler);
    /* AP */
    register_api(API_AP_START_UP, NULL, start_ap_handler);
    register_api(API_AP_STOP, NULL, stop_ap_handler);
    register_api(API_AP_CONFIGURE, NULL, configure_ap_handler);
    register_api(API_AP_TRIGGER_CHANSWITCH, NULL, trigger_ap_channel_switch);
    register_api(API_AP_SEND_DISCONNECT, NULL, send_ap_disconnect_handler);
    register_api(API_AP_SET_PARAM , NULL, set_ap_parameter_handler);
    register_api(API_AP_SEND_BTM_REQ, NULL, send_ap_btm_handler);
    /* STA */
    register_api(API_STA_ASSOCIATE, NULL, associate_sta_handler);
    register_api(API_STA_CONFIGURE, NULL, configure_sta_handler);
    register_api(API_STA_DISCONNECT, NULL, stop_sta_handler);
    register_api(API_STA_SEND_DISCONNECT, NULL, send_sta_disconnect_handler);
    register_api(API_STA_REASSOCIATE, NULL, send_sta_reconnect_handler);
    register_api(API_STA_SET_PARAM, NULL, set_sta_parameter_handler);
    register_api(API_STA_SEND_BTM_QUERY, NULL, send_sta_btm_query_handler);
    register_api(API_STA_SEND_ANQP_QUERY, NULL, send_sta_anqp_query_handler);
    /* TODO: Add the handlers */
    register_api(API_STA_SET_CHANNEL_WIDTH, NULL, NULL);
    register_api(API_STA_POWER_SAVE, NULL, NULL);
}

static int get_control_app_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, TLV_VALUE_STATUS_OK);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(TLV_VALUE_OK), TLV_VALUE_OK);
    fill_wrapper_tlv_bytes(resp, TLV_CONTROL_APP_VERSION, strlen(TLV_VALUE_APP_VERSION), TLV_VALUE_APP_VERSION);
    return 0;
}

static int reset_device_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int status = TLV_VALUE_STATUS_NOT_OK;
    char *message = TLV_VALUE_RESET_NOT_OK;
#ifdef CONTROLAPPC_LINUX_API
    char buffer[TLV_VALUE_SIZE];
#endif
    char role[TLV_VALUE_SIZE], log_level[TLV_VALUE_SIZE], band[TLV_VALUE_SIZE];
    struct tlv_hdr *tlv = NULL;

    /* TLV: ROLE */
    tlv = find_wrapper_tlv_by_id(req, TLV_ROLE);
    memset(role, 0, sizeof(role));
    if (tlv) {
        memcpy(role, tlv->value, tlv->len);
    } else {
        goto done;
    }
    /* TLV: DEBUG_LEVEL */
    tlv = find_wrapper_tlv_by_id(req, TLV_DEBUG_LEVEL);
    memset(log_level, 0, sizeof(log_level));
    if (tlv) {
        memcpy(log_level, tlv->value, tlv->len);
    }
    /* TLV: TLV_BAND */
    memset(band, 0, sizeof(band));
    tlv = find_wrapper_tlv_by_id(req, TLV_BAND);
    if (tlv) {
        memcpy(band, tlv->value, tlv->len);
    }

    if (atoi(role) == DUT_TYPE_STAUT) {
#ifdef CONTROLAPPC_LINUX_API
        /* stop the wpa_supplicant and release IP address */
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "killall %s 1>/dev/null 2>/dev/null", get_wpas_exec_file());
        system(buffer);
        sleep(1);
        reset_interface_ip(get_wireless_interface());
        if (strlen(log_level)) {
            set_wpas_debug_level(get_debug_level(atoi(log_level)));
        }
#endif
    } else if (atoi(role) == DUT_TYPE_APUT) {
#ifdef CONTROLAPPC_LINUX_API
        /* stop the hostapd and release IP address */
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "killall %s 1>/dev/null 2>/dev/null", get_hapd_exec_file());
        system(buffer);
        sleep(1);
#endif
        reset_interface_ip(get_wireless_interface());
        if (strlen(log_level)) {
            set_hostapd_debug_level(get_debug_level(atoi(log_level)));
        }
        reset_bridge(BRIDGE_WLANS);
        /* reset interfaces info */
        clear_interfaces_resource();
    }

    if (strcmp(band, TLV_BAND_24GHZ) == 0) {
        set_default_wireless_interface_info(BAND_24GHZ);
    } else if (strcmp(band, TLV_BAND_5GHZ) == 0) {
        set_default_wireless_interface_info(BAND_5GHZ);
    } else if (strcmp(band, TLV_BAND_6GHZ) == 0) {
        set_default_wireless_interface_info(BAND_6GHZ);
    }

    memset(band_transmitter, 0, sizeof(band_transmitter));

    vendor_device_reset();

#if 1
	/* default value for WFA certification */

	network_InitTypeDef_st wNetConfig;
	char *oob_ssid = "Wi-Fi";
	char *connect_key = "12345678";

	os_memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));

	os_strcpy((char *)wNetConfig.wifi_ssid, oob_ssid);
	os_strcpy((char *)wNetConfig.wifi_key, connect_key);

	wNetConfig.wifi_mode = BK_STATION;
	wNetConfig.dhcp_mode = DHCP_CLIENT;
	wNetConfig.wifi_retry_interval = 100;

	bk_wlan_sta_init(&wNetConfig);
	wlan_sta_enable();
#endif

#ifdef CONTROLAPPC_LINUX_API
    sleep(1);
#endif
    status = TLV_VALUE_STATUS_OK;
    message = TLV_VALUE_RESET_OK;

done:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);

    return 0;
}

// RESP: {<ResponseTLV.STATUS: 40961>: '0', <ResponseTLV.MESSAGE: 40960>: 'AP stop completed : Hostapd service is inactive.'}
static int stop_ap_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int len = 0, reset = 0;
    char reset_type[16];
    char *message = TLV_VALUE_HOSTAPD_STOP_OK;
    struct tlv_hdr *tlv = NULL;
#ifdef CONTROLAPPC_LINUX_API
	char buffer[S_BUFFER_LEN];
	char *parameter[] = {"pidof", get_hapd_exec_file(), NULL};
#endif
    /* TLV: RESET_TYPE */
    tlv = find_wrapper_tlv_by_id(req, TLV_RESET_TYPE);
    memset(reset_type, 0, sizeof(reset_type));
    if (tlv) {
        memcpy(reset_type, tlv->value, tlv->len);
        reset = atoi(reset_type);
        indigo_logger(LOG_LEVEL_DEBUG, "Reset Type: %d", reset);
    }
#ifdef CONTROLAPPC_LINUX_API
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "killall %s 1>/dev/null 2>/dev/null", get_hapd_exec_file());
    system(buffer);
    sleep(2);

#ifdef _OPENWRT_
#else
    len = system("rfkill unblock wlan");
    if (len) {
        indigo_logger(LOG_LEVEL_DEBUG, "Failed to run rfkill unblock wlan");
    }
    sleep(1);
#endif

    memset(buffer, 0, sizeof(buffer));
    len = pipe_command(buffer, sizeof(buffer), "/bin/pidof", parameter);
    if (len) {
        message = TLV_VALUE_HOSTAPD_STOP_NOT_OK;
    } else {
        message = TLV_VALUE_HOSTAPD_STOP_OK;
    }
#endif
    /* Test case teardown case */
    if (reset == RESET_TYPE_TEARDOWN) {
    }

    /* reset interfaces info */
    if (clear_interfaces_resource()) {
    }

#ifdef CONTROLAPPC_LINUX_API
    if (reset == RESET_TYPE_INIT) {
        system("rm -rf /var/log/hostapd.log >/dev/null 2>/dev/null");
    }
#endif

    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, len == 0 ? TLV_VALUE_STATUS_OK : TLV_VALUE_STATUS_NOT_OK);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);

    return 0;
}

#ifdef _RESERVED_
/* The function is reserved for the defeault hostapd config */
#define HOSTAPD_DEFAULT_CONFIG_SSID                 "QuickTrack"
#define HOSTAPD_DEFAULT_CONFIG_CHANNEL              "36"
#define HOSTAPD_DEFAULT_CONFIG_HW_MODE              "a"
#define HOSTAPD_DEFAULT_CONFIG_WPA_PASSPHRASE       "12345678"
#define HOSTAPD_DEFAULT_CONFIG_IEEE80211N           "1"
#define HOSTAPD_DEFAULT_CONFIG_WPA                  "2"
#define HOSTAPD_DEFAULT_CONFIG_WPA_KEY_MGMT         "WPA-PSK"
#define HOSTAPD_DEFAULT_CONFIG_RSN_PAIRWISE         "CCMP"

static void append_hostapd_default_config(struct packet_wrapper *wrapper) {
    if (find_wrapper_tlv_by_id(wrapper, TLV_SSID) == NULL) {
        add_wrapper_tlv(wrapper, TLV_SSID, strlen(HOSTAPD_DEFAULT_CONFIG_SSID), HOSTAPD_DEFAULT_CONFIG_SSID);
    }
    if (find_wrapper_tlv_by_id(wrapper, TLV_CHANNEL) == NULL) {
        add_wrapper_tlv(wrapper, TLV_CHANNEL, strlen(HOSTAPD_DEFAULT_CONFIG_CHANNEL), HOSTAPD_DEFAULT_CONFIG_CHANNEL);
    }
    if (find_wrapper_tlv_by_id(wrapper, TLV_HW_MODE) == NULL) {
        add_wrapper_tlv(wrapper, TLV_HW_MODE, strlen(HOSTAPD_DEFAULT_CONFIG_HW_MODE), HOSTAPD_DEFAULT_CONFIG_HW_MODE);
    }
    if (find_wrapper_tlv_by_id(wrapper, TLV_WPA_PASSPHRASE) == NULL) {
        add_wrapper_tlv(wrapper, TLV_WPA_PASSPHRASE, strlen(HOSTAPD_DEFAULT_CONFIG_WPA_PASSPHRASE), HOSTAPD_DEFAULT_CONFIG_WPA_PASSPHRASE);
    }
    if (find_wrapper_tlv_by_id(wrapper, TLV_IEEE80211_N) == NULL) {
        add_wrapper_tlv(wrapper, TLV_IEEE80211_N, strlen(HOSTAPD_DEFAULT_CONFIG_IEEE80211N), HOSTAPD_DEFAULT_CONFIG_IEEE80211N);
    }
    if (find_wrapper_tlv_by_id(wrapper, TLV_WPA) == NULL) {
        add_wrapper_tlv(wrapper, TLV_WPA, strlen(HOSTAPD_DEFAULT_CONFIG_WPA), HOSTAPD_DEFAULT_CONFIG_WPA);
    }
    if (find_wrapper_tlv_by_id(wrapper, TLV_WPA_KEY_MGMT) == NULL) {
        add_wrapper_tlv(wrapper, TLV_WPA_KEY_MGMT, strlen(HOSTAPD_DEFAULT_CONFIG_WPA_KEY_MGMT), HOSTAPD_DEFAULT_CONFIG_WPA_KEY_MGMT);
    }
    if (find_wrapper_tlv_by_id(wrapper, TLV_RSN_PAIRWISE) == NULL) {
        add_wrapper_tlv(wrapper, TLV_RSN_PAIRWISE, strlen(HOSTAPD_DEFAULT_CONFIG_RSN_PAIRWISE), HOSTAPD_DEFAULT_CONFIG_RSN_PAIRWISE);
    }
}
#endif /* _RESERVED_ */

static int generate_hostapd_config(char *output, int output_size, struct packet_wrapper *wrapper, struct interface_info* wlanp) {
    int has_sae = 0, has_wpa = 0, has_pmf = 0, has_owe = 0, has_transition = 0, has_sae_groups = 0;
    int channel = 0, chwidth = 1, enable_ax = 0, chwidthset = 0, enable_muedca = 0, vht_chwidthset = 0;
    int i, enable_ac = 0, enable_11h __maybe_unused = 0;
    char buffer[S_BUFFER_LEN], cfg_item[2*S_BUFFER_LEN];
    char band[64], value[16];
    char country[16];
    struct tlv_to_config_name* cfg = NULL;
    struct tlv_hdr *tlv = NULL;
    int is_6g_only = 0, unsol_pr_resp_interval = 0;

#if HOSTAPD_SUPPORT_MBSSID
    if (wlanp->mbssid_enable && !wlanp->transmitter)
        sprintf(output, "bss=%s\nctrl_interface=%s\n", wlanp->ifname, HAPD_CTRL_PATH_DEFAULT);
    else
        sprintf(output, "ctrl_interface=%s\nctrl_interface_group=0\ninterface=%s\n", HAPD_CTRL_PATH_DEFAULT, wlanp->ifname);
#else
    sprintf(output, "ctrl_interface=%s\nctrl_interface_group=0\ninterface=%s\n", HAPD_CTRL_PATH_DEFAULT, wlanp->ifname);
#endif

#ifdef _RESERVED_
    /* The function is reserved for the defeault hostapd config */
    append_hostapd_default_config(wrapper);
#endif

    memset(country, 0, sizeof(country));

    /* QCA WTS image doesn't apply 11ax, mu_edca, country, 11d, 11h in hostapd */
    for (i = 0; i < wrapper->tlv_num; i++) {
        tlv = wrapper->tlv[i];

        if (tlv->id == TLV_HE_6G_ONLY) {
            is_6g_only = 1;
            continue;
        }

        if (tlv->id == TLV_BSS_IDENTIFIER) {
            if (is_band_enabled(BAND_6GHZ) && !wlanp->mbssid_enable) {
                strcat(output, "rnr=1\n");
            }
            continue;
        }

        cfg = find_tlv_config(tlv->id);
        if (!cfg) {
            indigo_logger(LOG_LEVEL_ERROR, "Unknown AP configuration name: TLV ID 0x%04x", tlv->id);
            continue;
        }

        if (tlv->id == TLV_WPA_KEY_MGMT && strstr((char *)tlv->value, "SAE") && strstr((char *)tlv->value, "WPA-PSK")) {
            has_transition = 1;
        }

        if (tlv->id == TLV_WPA_KEY_MGMT && strstr((char *)tlv->value, "OWE")) {
            has_owe = 1;
        }

        if (tlv->id == TLV_WPA_KEY_MGMT && strstr((char *)tlv->value, "SAE")) {
            has_sae = 1;
        }

        if (tlv->id == TLV_WPA && strstr((char *)tlv->value, "2")) {
            has_wpa = 1;
        }

        if (tlv->id == TLV_IEEE80211_W) {
            has_pmf = 1;
        }

        if (tlv->id == TLV_HW_MODE) {
            memset(band, 0, sizeof(band));
            memcpy(band, tlv->value, tlv->len);
        }

        if (tlv->id == TLV_CHANNEL) {
            memset(value, 0, sizeof(value));
            memcpy(value, tlv->value, tlv->len);
            channel = atoi(value);
        }

        if (tlv->id == TLV_HE_OPER_CHWIDTH) {
            memset(value, 0, sizeof(value));
            memcpy(value, tlv->value, tlv->len);
            chwidth = atoi(value);
            chwidthset = 1;
#ifdef _WTS_OPENWRT_
            continue;
#endif
        }

        if (tlv->id == TLV_VHT_OPER_CHWIDTH) {
            memset(value, 0, sizeof(value));
            memcpy(value, tlv->value, tlv->len);
            chwidth = atoi(value);
            vht_chwidthset = 1;
        }

        if (tlv->id == TLV_IEEE80211_AC && strstr((char *)tlv->value, "1")) {
            enable_ac = 1;
        }

        if (tlv->id == TLV_IEEE80211_AX && strstr((char *)tlv->value, "1")) {
            enable_ax = 1;
#ifdef _WTS_OPENWRT_
            continue;
#endif
        }

        if (tlv->id == TLV_HE_MU_EDCA) {
#ifdef _WTS_OPENWRT_
            continue;
#endif
            enable_muedca = 1;
        }

        if (tlv->id == TLV_SAE_GROUPS) {
            has_sae_groups = 1;
        }

        if (tlv->id == TLV_COUNTRY_CODE) {
            memcpy(country, tlv->value, tlv->len);
#ifdef _WTS_OPENWRT_
            continue;
#endif
        }

        if (tlv->id == TLV_IEEE80211_H) {
#ifdef _WTS_OPENWRT_
            continue;
#endif
            enable_11h = 1;
        }

#ifdef _WTS_OPENWRT_
        if (tlv->id == TLV_IEEE80211_D || tlv->id == TLV_HE_OPER_CENTR_FREQ)
            continue;

#endif

        if (tlv->id == TLV_HE_UNSOL_PR_RESP_CADENCE) {
            memset(value, 0, sizeof(value));
            memcpy(value, tlv->value, tlv->len);
            unsol_pr_resp_interval = atoi(value);
        }

        memset(buffer, 0, sizeof(buffer));
        memset(cfg_item, 0, sizeof(cfg_item));
        if (tlv->id == TLV_OWE_TRANSITION_BSS_IDENTIFIER) {
            struct bss_identifier_info bss_info;
            struct interface_info *wlan;
            int bss_identifier;
            char bss_identifier_str[8];
            memset(&bss_info, 0, sizeof(bss_info));
            memset(bss_identifier_str, 0, sizeof(bss_identifier_str));
            memcpy(bss_identifier_str, tlv->value, tlv->len);
            bss_identifier = atoi(bss_identifier_str);
            parse_bss_identifier(bss_identifier, &bss_info);
            wlan = get_wireless_interface_info(bss_info.band, bss_info.identifier);
            if (NULL == wlan) {
                wlan = assign_wireless_interface_info(&bss_info);
            }
            printf("TLV_OWE_TRANSITION_BSS_IDENTIFIER: TLV_BSS_IDENTIFIER 0x%x identifier %d mapping ifname %s\n",
                    bss_identifier,
                    bss_info.identifier,
                    wlan ? wlan->ifname : "n/a"
                    );
            if (wlan) {
                memcpy(buffer, wlan->ifname, strlen(wlan->ifname));
                sprintf(cfg_item, "%s=%s\n", cfg->config_name, buffer);
                strcat(output, cfg_item);
                if (has_owe) {
                    memset(cfg_item, 0, sizeof(cfg_item));
                    sprintf(cfg_item, "ignore_broadcast_ssid=1\n");
                    strcat(output, cfg_item);
                }
            }
        } else {
            memcpy(buffer, tlv->value, tlv->len);
            sprintf(cfg_item, "%s=%s\n", cfg->config_name, buffer);
            strcat(output, cfg_item);
        }
    }

    if (has_pmf == 0) {
        if (has_transition) {
            strcat(output, "ieee80211w=1\n");
        } else if (has_sae && has_wpa) {
            strcat(output, "ieee80211w=2\n");
        } else if (has_owe) {
            strcat(output, "ieee80211w=2\n");
        } else if (has_wpa) {
            strcat(output, "ieee80211w=1\n");
        }
    }

    if (has_sae == 1) {
        strcat(output, "sae_require_mfp=1\n");
    }

#if HOSTAPD_SUPPORT_MBSSID
    if (wlanp->mbssid_enable && wlanp->transmitter) {
        strcat(output, "multiple_bssid=1\n");
    }
#endif

    // Note: if any new DUT configuration is added for sae_groups,
    // then the following unconditional sae_groups addition should be
    // changed to become conditional on there being no other sae_groups
    // configuration
    // e.g.:
    // if RequestTLV.SAE_GROUPS not in tlv_values:
    //     field_name = tlv_hostapd_config_mapper.get(RequestTLV.SAE_GROUPS)
    //     hostapd_config += "\n" + field_name + "=15 16 17 18 19 20 21"
    // Append the default SAE groups for SAE and no SAE groups TLV
    if (has_sae && has_sae_groups == 0) {
        strcat(output, "sae_groups=15 16 17 18 19 20 21\n");
    }

    // Channel width configuration
    // Default: 20MHz in 2.4G(No configuration required) 80MHz(40MHz for 11N only) in 5G
    if (enable_ac == 0 && enable_ax == 0)
        chwidth = 0;

    if (is_6g_only) {
        if (chwidthset == 0) {
            sprintf(buffer, "he_oper_chwidth=%d\n", chwidth);
            strcat(output, buffer);
        }
        if (chwidth == 1)
            strcat(output, "op_class=133\n");
        else if (chwidth == 2)
            strcat(output, "op_class=134\n");
        sprintf(buffer, "he_oper_centr_freq_seg0_idx=%d\n", get_6g_center_freq_index(channel, chwidth));
        strcat(output, buffer);
        if (unsol_pr_resp_interval) {
            sprintf(buffer, "unsol_bcast_probe_resp_interval=%d\n", unsol_pr_resp_interval);
            strcat(output, buffer);
        } else {
            strcat(output, "fils_discovery_max_interval=20\n");
        }
        /* Enable bss_color and country IE */
        strcat(output, "he_bss_color=19\n");
        strcat(output, "ieee80211d=1\n");
        strcat(output, "country_code=US\n");
    } else if (strstr(band, "a")) {
        if (is_ht40plus_chan(channel))
            strcat(output, "ht_capab=[HT40+]\n");
        else if (is_ht40minus_chan(channel))
            strcat(output, "ht_capab=[HT40-]\n");
        else // Ch 165 and avoid hostapd configuration error
            chwidth = 0;
        if (chwidth > 0) {
            int center_freq = get_center_freq_index(channel, chwidth);
#ifndef _WTS_OPENWRT_
            if (chwidth == 2) {
                /* 160M: Need to enable 11h for DFS and enable 11d for 11h */
                strcat(output, "ieee80211d=1\n");
                strcat(output, "country_code=US\n");
                strcat(output, "ieee80211h=1\n");
            }
#endif
            if (enable_ac) {
                if (vht_chwidthset == 0) {
                    sprintf(buffer, "vht_oper_chwidth=%d\n", chwidth);
                    strcat(output, buffer);
                }
                sprintf(buffer, "vht_oper_centr_freq_seg0_idx=%d\n", center_freq);
                strcat(output, buffer);
#ifndef _WTS_OPENWRT_
                if (chwidth == 2) {
                    strcat(output, "vht_capab=[VHT160]\n");
                }
#endif
            }
            if (enable_ax) {
#ifndef _WTS_OPENWRT_
                if (chwidthset == 0) {
                    sprintf(buffer, "he_oper_chwidth=%d\n", chwidth);
                    strcat(output, buffer);
                }
                sprintf(buffer, "he_oper_centr_freq_seg0_idx=%d\n", center_freq);
                strcat(output, buffer);
#endif
            }
        }
    }

    if (enable_muedca) {
        strcat(output, "he_mu_edca_qos_info_queue_request=1\n");
        strcat(output, "he_mu_edca_ac_be_aifsn=0\n");
        strcat(output, "he_mu_edca_ac_be_ecwmin=15\n");
        strcat(output, "he_mu_edca_ac_be_ecwmax=15\n");
        strcat(output, "he_mu_edca_ac_be_timer=255\n");
        strcat(output, "he_mu_edca_ac_bk_aifsn=0\n");
        strcat(output, "he_mu_edca_ac_bk_aci=1\n");
        strcat(output, "he_mu_edca_ac_bk_ecwmin=15\n");
        strcat(output, "he_mu_edca_ac_bk_ecwmax=15\n");
        strcat(output, "he_mu_edca_ac_bk_timer=255\n");
        strcat(output, "he_mu_edca_ac_vi_ecwmin=15\n");
        strcat(output, "he_mu_edca_ac_vi_ecwmax=15\n");
        strcat(output, "he_mu_edca_ac_vi_aifsn=0\n");
        strcat(output, "he_mu_edca_ac_vi_aci=2\n");
        strcat(output, "he_mu_edca_ac_vi_timer=255\n");
        strcat(output, "he_mu_edca_ac_vo_aifsn=0\n");
        strcat(output, "he_mu_edca_ac_vo_aci=3\n");
        strcat(output, "he_mu_edca_ac_vo_ecwmin=15\n");
        strcat(output, "he_mu_edca_ac_vo_ecwmax=15\n");
        strcat(output, "he_mu_edca_ac_vo_timer=255\n");
    }

#if defined(_OPENWRT_) && !defined(_WTS_OPENWRT_)
    /* Make sure AP include power constranit element even in non DFS channel */
    if (enable_11h) {
        strcat(output, "spectrum_mgmt_required=1\n");
        strcat(output, "local_pwr_constraint=3\n");
    }
#endif

    /* vendor specific config, not via hostapd */
    configure_ap_radio_params(band, country, channel, chwidth);

    return strlen(output);
}

// RESP: {<ResponseTLV.STATUS: 40961>: '0', <ResponseTLV.MESSAGE: 40960>: 'DUT configured as AP : Configuration file created'}
static int configure_ap_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int len = 0;
    char buffer[L_BUFFER_LEN], ifname[S_BUFFER_LEN];
    struct tlv_hdr *tlv;
    char *message = "DUT configured as AP : Configuration file created";
    int bss_identifier = 0, band;
    struct interface_info* wlan = NULL;
    char bss_identifier_str[16], hw_mode_str[8];
    struct bss_identifier_info bss_info;

    memset(buffer, 0, sizeof(buffer));
    tlv = find_wrapper_tlv_by_id(req, TLV_BSS_IDENTIFIER);
    memset(ifname, 0, sizeof(ifname));
    memset(&bss_info, 0, sizeof(bss_info));
    if (tlv) {
        /* Multiple wlans configure must carry TLV_BSS_IDENTIFIER */
        memset(bss_identifier_str, 0, sizeof(bss_identifier_str));
        memcpy(bss_identifier_str, tlv->value, tlv->len);
        bss_identifier = atoi(bss_identifier_str);
        parse_bss_identifier(bss_identifier, &bss_info);
        wlan = get_wireless_interface_info(bss_info.band, bss_info.identifier);
        if (NULL == wlan) {
            wlan = assign_wireless_interface_info(&bss_info);
        }
        if (wlan && bss_info.mbssid_enable) {
            configure_ap_enable_mbssid();
            if (bss_info.transmitter) {
                band_transmitter[bss_info.band] = wlan;
            }
        }
        printf("TLV_BSS_IDENTIFIER 0x%x band %d multiple_bssid %d transmitter %d identifier %d\n",
               bss_identifier,
               bss_info.band,
               bss_info.mbssid_enable,
               bss_info.transmitter,
               bss_info.identifier
               );
    } else {
        /* Single wlan case */
        tlv = find_wrapper_tlv_by_id(req, TLV_HW_MODE);
        if (tlv)
        {
            memset(hw_mode_str, 0, sizeof(hw_mode_str));
            memcpy(hw_mode_str, tlv->value, tlv->len);
            if (find_wrapper_tlv_by_id(req, TLV_HE_6G_ONLY)) {
                band = BAND_6GHZ;
            } else if (!strncmp(hw_mode_str, "a", 1)) {
                band = BAND_5GHZ;
            } else {
                band = BAND_24GHZ;
            }
            /* Single wlan use ID 1 */
            bss_info.band = band;
            bss_info.identifier = 1;
            wlan = assign_wireless_interface_info(&bss_info);
        }
    }
    if (wlan) {
        printf("ifname %s hostapd conf file %s\n",
               wlan ? wlan->ifname : "n/a",
               wlan ? wlan->hapd_conf_file: "n/a"
               );
        len = generate_hostapd_config(buffer, sizeof(buffer), req, wlan);
        if (len)
        {
#if HOSTAPD_SUPPORT_MBSSID
            if (bss_info.mbssid_enable && !bss_info.transmitter) {
                if (band_transmitter[bss_info.band]) {
                    append_file(band_transmitter[bss_info.band]->hapd_conf_file, buffer, len);
                }
                memset(wlan->hapd_conf_file, 0, sizeof(wlan->hapd_conf_file));
            }
            else
#endif
                write_file(wlan->hapd_conf_file, buffer, len);
        }
    }
    show_wireless_interface_info();

    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, len > 0 ? TLV_VALUE_STATUS_OK : TLV_VALUE_STATUS_NOT_OK);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);

    return 0;
}

#ifdef HOSTAPD_SUPPORT_MBSSID_WAR
extern int use_openwrt_wpad;
#endif
// RESP: {<ResponseTLV.STATUS: 40961>: '0', <ResponseTLV.MESSAGE: 40960>: 'AP is up : Hostapd service is active'}
static int start_ap_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    char *message = TLV_VALUE_HOSTAPD_START_OK;
#ifdef CONTROLAPPC_LINUX_API
    char buffer[S_BUFFER_LEN];
#endif
    int len = 0;

    int swap_hostapd = 0;

#ifdef _WTS_OPENWRT_
    openwrt_apply_radio_config();
    // DFS wait again if set wlan params after hostapd starts
    iterate_all_wlan_interfaces(start_ap_set_wlan_params);
#endif
#ifdef CONTROLAPPC_LINUX_API
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s -B -t -P /var/run/hostapd.pid -g %s %s -f /var/log/hostapd.log %s",
        get_hapd_full_exec_path(),
        get_hapd_global_ctrl_path(),
        get_hostapd_debug_arguments(),
        get_all_hapd_conf_files(&swap_hostapd));
    len = system(buffer);
    sleep(1);
#endif
    /* Bring up VAPs with MBSSID disable using WFA hostapd */
    if (swap_hostapd) {
#ifdef HOSTAPD_SUPPORT_MBSSID_WAR
        indigo_logger(LOG_LEVEL_INFO, "Use WFA hostapd for MBSSID disable VAPs with RNR");
        system("cp /overlay/hostapd /usr/sbin/hostapd");
        use_openwrt_wpad = 0;
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%s -B -t -P /var/run/hostapd_1.pid %s -f /var/log/hostapd_1.log %s",
                get_hapd_full_exec_path(),
                get_hostapd_debug_arguments(),
                get_all_hapd_conf_files(&swap_hostapd));
        len = system(buffer);
        sleep(1);
#endif
    }

#ifndef _WTS_OPENWRT_
    iterate_all_wlan_interfaces(start_ap_set_wlan_params);
#endif

    bridge_init(BRIDGE_WLANS);

    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, len == 0 ? TLV_VALUE_STATUS_OK : TLV_VALUE_STATUS_NOT_OK);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);

    return 0;
}

/* deprecated */
static int create_bridge_network_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int err = 0;
    char static_ip[S_BUFFER_LEN];
    struct tlv_hdr *tlv;
    char *message = TLV_VALUE_CREATE_BRIDGE_OK;

    /* TLV: TLV_STATIC_IP */
    memset(static_ip, 0, sizeof(static_ip));
    tlv = find_wrapper_tlv_by_id(req, TLV_STATIC_IP);
    if (tlv) {
        memcpy(static_ip, tlv->value, tlv->len);
    } else {
        message = TLV_VALUE_CREATE_BRIDGE_NOT_OK;
        err = -1;
        goto response;
    }

    /* Create new bridge */
    create_bridge(BRIDGE_WLANS);

    add_all_wireless_interface_to_bridge(BRIDGE_WLANS);

    set_interface_ip(BRIDGE_WLANS, static_ip);

    response:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, err >= 0 ? TLV_VALUE_STATUS_OK : TLV_VALUE_STATUS_NOT_OK);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);

    return 0;
}

// Bytes to DUT : 01 50 06 00 ed ff ff 00 55 0c 31 39 32 2e 31 36 38 2e 31 30 2e 33
// RESP :{<ResponseTLV.STATUS: 40961>: '0', <ResponseTLV.MESSAGE: 40960>: 'Static Ip successfully assigned to wireless interface'}
static int assign_static_ip_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int len = 0;
    char buffer[64];
    struct tlv_hdr *tlv = NULL;
#ifndef BEKEN_API
    char *ifname = NULL;
#endif
    char *message = TLV_VALUE_ASSIGN_STATIC_IP_OK;

    memset(buffer, 0, sizeof(buffer));
    tlv = find_wrapper_tlv_by_id(req, TLV_STATIC_IP);
    if (tlv) {
        memcpy(buffer, tlv->value, tlv->len);
    } else {
        message = "Failed.";
        goto response;
    }

#ifdef BEKEN_API
	{
		IPStatusTypedef inNetpara;
		memset(&inNetpara, 0, sizeof(IPStatusTypedef));

		memcpy(inNetpara.ip, tlv->value, tlv->len);
		os_strcpy(inNetpara.mask, "255.255.255.0");
		os_strcpy(inNetpara.gate, "0.0.0.0");
		os_strcpy(inNetpara.dns, "0.0.0.0");

		len = ip_address_set(BK_STATION, inNetpara.dhcp, inNetpara.ip,
							 inNetpara.mask, inNetpara.gate, inNetpara.dns);
		if (len) {
			message = TLV_VALUE_ASSIGN_STATIC_IP_NOT_OK;
		}
		// bk_wlan_set_ip_status(&inNetpara , BK_STATION);
	}
#else
    if (is_bridge_created()) {
        ifname = BRIDGE_WLANS;
    } else {
        ifname = get_wireless_interface();
    }

    /* Release IP address from interface */
    reset_interface_ip(ifname);
    /* Bring up interface */
    control_interface(ifname, "up");
    /* Set IP address with network mask */
    strcat(buffer, "/24");
    len = set_interface_ip(ifname, buffer);
    if (len) {
        message = TLV_VALUE_ASSIGN_STATIC_IP_NOT_OK;
    }
#endif

response:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, len == 0 ? TLV_VALUE_STATUS_OK : TLV_VALUE_STATUS_NOT_OK);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);

    return 0;
}

// Bytes to DUT : 01 50 01 00 ee ff ff
// ACK:  Bytes from DUT : 01 00 01 00 ee ff ff a0 01 01 30 a0 00 15 41 43 4b 3a 20 43 6f 6d 6d 61 6e 64 20 72 65 63 65 69 76 65 64
// RESP: {<ResponseTLV.STATUS: 40961>: '0', <ResponseTLV.MESSAGE: 40960>: '9c:b6:d0:19:40:c7', <ResponseTLV.DUT_MAC_ADDR: 40963>: '9c:b6:d0:19:40:c7'}
static int get_mac_addr_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int status = TLV_VALUE_STATUS_NOT_OK;
    char *message = TLV_VALUE_NOT_OK;
    char mac_addr[32];
#if 0
    struct tlv_hdr *tlv;
    char band[S_BUFFER_LEN];
    char ssid[S_BUFFER_LEN];
    char role[S_BUFFER_LEN];
    char connected_freq[S_BUFFER_LEN];
    char connected_ssid[S_BUFFER_LEN];
    int bss_identifier = 0;
    char bss_identifier_str[16];
    struct bss_identifier_info bss_info;

#ifdef CONTROLAPPC_LINUX_API
	struct wpa_ctrl *w = NULL;
	size_t resp_len = 0;
	char cmd[16];
	char response[L_BUFFER_LEN];
	struct interface_info* wlan = NULL;
	char buff[S_BUFFER_LEN];
#endif
#endif

#ifdef BEKEN_API
	{
		unsigned char mac[6];
		bk_wifi_sta_get_mac((uint8_t *)mac);

		snprintf(mac_addr, sizeof(mac_addr),  "%02x:%02x:%02x:%02x:%02x:%02x",
			(char)mac[0]&0x00ff, (char)mac[1]&0x00ff, (char)mac[2]&0x00ff,
			(char)mac[3]&0x00ff, (char)mac[4]&0x00ff, (char)mac[5]&0x00ff);
	}
#endif



    if (req->tlv_num == 0) {
        get_mac_address(mac_addr, sizeof(mac_addr), get_wireless_interface());
        status = TLV_VALUE_STATUS_OK;
        message = TLV_VALUE_OK;

        goto done;
    } else {
#if 0
        /* TLV: TLV_ROLE */
        memset(role, 0, sizeof(role));
        tlv = find_wrapper_tlv_by_id(req, TLV_ROLE);
        if (tlv) {
            memcpy(role, tlv->value, tlv->len);
        }

        /* TLV: TLV_BAND */
        memset(band, 0, sizeof(band));
        tlv = find_wrapper_tlv_by_id(req, TLV_BAND);
        if (tlv) {
            memcpy(band, tlv->value, tlv->len);
        }

        /* TLV: TLV_SSID */
        memset(ssid, 0, sizeof(ssid));
        tlv = find_wrapper_tlv_by_id(req, TLV_SSID);
        if (tlv) {
            memcpy(ssid, tlv->value, tlv->len);
        }

        memset(&bss_info, 0, sizeof(bss_info));
        tlv = find_wrapper_tlv_by_id(req, TLV_BSS_IDENTIFIER);
        if (tlv) {
            memset(bss_identifier_str, 0, sizeof(bss_identifier_str));
            memcpy(bss_identifier_str, tlv->value, tlv->len);
            bss_identifier = atoi(bss_identifier_str);
            parse_bss_identifier(bss_identifier, &bss_info);

            printf("TLV_BSS_IDENTIFIER 0x%x identifier %d band %d\n",
                    bss_identifier,
                    bss_info.identifier,
                    bss_info.band
                    );
        } else {
            bss_info.identifier = -1;
        }
#endif
    }
#if 0
#ifdef CONTROLAPPC_LINUX_API
    if (atoi(role) == DUT_TYPE_STAUT) {
        w = wpa_ctrl_open(get_wpas_ctrl_path());
    } else {
        wlan = get_wireless_interface_info(bss_info.band, bss_info.identifier);
        w = wpa_ctrl_open(get_hapd_ctrl_path_by_id(wlan));
    }

    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to %s", atoi(role) == DUT_TYPE_STAUT ? "wpa_supplicant" : "hostapd");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_NOT_OK;
        goto done;
    }

    /* Assemble hostapd command */
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "STATUS");
    /* Send command to hostapd UDS socket */
    resp_len = sizeof(response) - 1;
    memset(response, 0, sizeof(response));
    wpa_ctrl_request(w, cmd, strlen(cmd), response, &resp_len, NULL);

    /* Check response */
    get_key_value(connected_freq, response, "freq");

    memset(mac_addr, 0, sizeof(mac_addr));
    if (atoi(role) == DUT_TYPE_STAUT) {
        get_key_value(connected_ssid, response, "ssid");
        get_key_value(mac_addr, response, "address");
    } else {
#if HOSTAPD_SUPPORT_MBSSID
        if(wlan && wlan->mbssid_enable) {
            sprintf(buff, "ssid[%d]", wlan->hapd_bss_id);
            get_key_value(connected_ssid, response, buff);
            sprintf(buff, "bssid[%d]", wlan->hapd_bss_id);
            get_key_value(mac_addr, response, buff);
        } else {
            get_key_value(connected_ssid, response, "ssid[0]");
            get_key_value(mac_addr, response, "bssid[0]");
        }
#else
        get_key_value(connected_ssid, response, "ssid[0]");
        get_key_value(mac_addr, response, "bssid[0]");
#endif
    }
#endif

    if (bss_info.identifier >= 0) {
        printf("Get mac_addr %s\n", mac_addr);
        status = TLV_VALUE_STATUS_OK;
        message = TLV_VALUE_OK;
        goto done;
    }

    /* Check band and connected freq*/
    if (strlen(band)) {
        int band_id = 0;

        if (strcmp(band, "2.4GHz") == 0)
            band_id = BAND_24GHZ;
        else if (strcmp(band, "5GHz") == 0)
            band_id = BAND_5GHZ;
        else if (strcmp(band, "6GHz") == 0)
            band_id = BAND_6GHZ;
        if (verify_band_from_freq(atoi(connected_freq), band_id) == 0) {
            status = TLV_VALUE_STATUS_OK;
            message = TLV_VALUE_OK;
        } else {
            status = TLV_VALUE_STATUS_NOT_OK;
            message = "Unable to get mac address associated with the given band";
            goto done;
        }
    }

    /* Check SSID and connected SSID */
    if (strlen(ssid)) {
        if (strcmp(ssid, connected_ssid) == 0) {
            status = TLV_VALUE_STATUS_OK;
            message = TLV_VALUE_OK;
        } else {
            status = TLV_VALUE_STATUS_NOT_OK;
            message = "Unable to get mac address associated with the given ssid";
            goto done;
        }
    }

    /* TODO: BSSID */
#endif

done:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    if (status == TLV_VALUE_STATUS_OK) {
        fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(mac_addr), mac_addr);
        fill_wrapper_tlv_bytes(resp, TLV_DUT_MAC_ADDR, strlen(mac_addr), mac_addr);
    } else {
        fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
    }
#ifdef CONTROLAPPC_LINUX_API
    if (w) {
        wpa_ctrl_close(w);
    }
#endif
    return 0;
}

static int start_loopback_server(struct packet_wrapper *req, struct packet_wrapper *resp) {
    char local_ip[256];
    int status = TLV_VALUE_STATUS_NOT_OK;
    char *message = TLV_VALUE_LOOPBACK_SVR_START_NOT_OK;
    char tool_udp_port[16];

    /* Find network interface. If BRIDGE_WLANS exists, then use it. Otherwise, it uses the initiation value. */
    memset(local_ip, 0, sizeof(local_ip));
#ifdef BEKEN_API
    {
		struct wlan_ip_config addr;
    	net_get_if_addr(&addr, net_get_sta_handle());
		os_strcpy(local_ip, wlan_ipv4_ntoa(&addr));
    }
#else
    if (find_interface_ip(local_ip, sizeof(local_ip), BRIDGE_WLANS)) {
        indigo_logger(LOG_LEVEL_DEBUG, "use %s", BRIDGE_WLANS);
    } else if (find_interface_ip(local_ip, sizeof(local_ip), get_wireless_interface())) {
        indigo_logger(LOG_LEVEL_DEBUG, "use %s", get_wireless_interface());
// #ifdef __TEST__
    } else if (find_interface_ip(local_ip, sizeof(local_ip), "eth0")) {
        indigo_logger(LOG_LEVEL_DEBUG, "use %s", "eth0");
// #endif /* __TEST__ */
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "No available interface");
        goto done;
    }
#endif
    /* Start loopback */
    if (!loopback_server_start(local_ip, tool_udp_port, LOOPBACK_TIMEOUT)) {
        status = TLV_VALUE_STATUS_OK;
        message = TLV_VALUE_LOOPBACK_SVR_START_OK;
    }
#ifndef BEKEN_API
done:
#endif
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
    fill_wrapper_tlv_bytes(resp, TLV_LOOP_BACK_SERVER_PORT, strlen(tool_udp_port), tool_udp_port);

    return 0;
}

// RESP: {<ResponseTLV.STATUS: 40961>: '0', <ResponseTLV.MESSAGE: 40960>: 'Loopback server in idle state'}
static int stop_loop_back_server_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    /* Stop loopback */
    if (loopback_server_status()) {
        loopback_server_stop();
    }
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, TLV_VALUE_STATUS_OK);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(TLV_VALUE_LOOP_BACK_STOP_OK), TLV_VALUE_LOOP_BACK_STOP_OK);

    return 0;
}

static int send_ap_disconnect_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int status = TLV_VALUE_STATUS_NOT_OK;
    char address[32];
    char *message = NULL;
    struct tlv_hdr *tlv = NULL;
#ifdef CONTROLAPPC_LINUX_API
	int len;
	char *parameter[] = {"pidof", get_hapd_exec_file(), NULL};
    struct wpa_ctrl *w = NULL;
	char buffer[S_BUFFER_LEN];
	char response[S_BUFFER_LEN];
    size_t resp_len;
#endif

#ifdef CONTROLAPPC_LINUX_API
    /* Check hostapd status. TODO: it may use UDS directly */
    memset(buffer, 0, sizeof(buffer));
    len = pipe_command(buffer, sizeof(buffer), "/bin/pidof", parameter);
    if (len == 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to find hostapd PID");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_HOSTAPD_NOT_OK;
        goto done;
    }
    /* Open hostapd UDS socket */
    w = wpa_ctrl_open(get_hapd_ctrl_path());
    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to hostapd");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_HOSTAPD_CTRL_NOT_OK;
        goto done;
    }
#endif
    /* ControlApp on DUT */
    /* TLV: TLV_ADDRESS */
    memset(address, 0, sizeof(address));
    tlv = find_wrapper_tlv_by_id(req, TLV_ADDRESS);
    if (tlv) {
        memcpy(address, tlv->value, tlv->len);
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "Missed TLV:Address");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_INSUFFICIENT_TLV;
        goto done;
    }
#ifdef CONTROLAPPC_LINUX_API
    /* Assemble hostapd command */
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "DISASSOCIATE %s reason=1", address);
    /* Send command to hostapd UDS socket */
    resp_len = sizeof(response) - 1;
    wpa_ctrl_request(w, buffer, strlen(buffer), response, &resp_len, NULL);
    /* Check response */
    if (strncmp(response, WPA_CTRL_OK, strlen(WPA_CTRL_OK)) != 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to execute the command. Response: %s", response);
        message = TLV_VALUE_HOSTAPD_RESP_NOT_OK;
        goto done;
    }
#endif
    status = TLV_VALUE_STATUS_OK;
    message = TLV_VALUE_HOSTAPD_STOP_OK;
done:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
#ifdef CONTROLAPPC_LINUX_API
    if (w) {
        wpa_ctrl_close(w);
    }
#endif
    return 0;
}

static int set_ap_parameter_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int status = TLV_VALUE_STATUS_NOT_OK;
    char *message = NULL;
    char param_name[32];
    char param_value[256];
    struct tlv_hdr *tlv = NULL;
#ifdef CONTROLAPPC_LINUX_API
    size_t resp_len;
    char buffer[8192];
    char response[1024];
    struct wpa_ctrl *w = NULL;
#endif

#ifdef CONTROLAPPC_LINUX_API
    /* Open hostapd UDS socket */
    w = wpa_ctrl_open(get_hapd_ctrl_path());
    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to hostapd");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_HOSTAPD_CTRL_NOT_OK;
        goto done;
    }
#endif
    /* ControlApp on DUT */
    /* TLV: MBO_ASSOC_DISALLOW or GAS_COMEBACK_DELAY */
    memset(param_value, 0, sizeof(param_value));
    tlv = find_wrapper_tlv_by_id(req, TLV_MBO_ASSOC_DISALLOW);
    if (!tlv) {
        tlv = find_wrapper_tlv_by_id(req, TLV_GAS_COMEBACK_DELAY);
    }
    if (tlv && find_tlv_config_name(tlv->id) != NULL) {
        strcpy(param_name, find_tlv_config_name(tlv->id));
        memcpy(param_value, tlv->value, tlv->len);
    } else {
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_INSUFFICIENT_TLV;
        indigo_logger(LOG_LEVEL_ERROR, "Missed TLV: TLV_MBO_ASSOC_DISALLOW or TLV_GAS_COMEBACK_DELAY");
        goto done;
    }
    /* Assemble hostapd command */
#ifdef CONTROLAPPC_LINUX_API
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "SET %s %s", param_name, param_value);
    /* Send command to hostapd UDS socket */
    resp_len = sizeof(response) - 1;
    wpa_ctrl_request(w, buffer, strlen(buffer), response, &resp_len, NULL);
    /* Check response */
    if (strncmp(response, WPA_CTRL_OK, strlen(WPA_CTRL_OK)) != 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to execute the command. Response: %s", response);
        message = TLV_VALUE_HOSTAPD_RESP_NOT_OK;
        goto done;
    }
#endif
    status = TLV_VALUE_STATUS_OK;
    message = TLV_VALUE_OK;
done:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
#ifdef CONTROLAPPC_LINUX_API
    if (w) {
        wpa_ctrl_close(w);
    }
#endif
    return 0;
}

static int send_ap_btm_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int status = TLV_VALUE_STATUS_NOT_OK;

    char *message = NULL;
    struct tlv_hdr *tlv = NULL;
    char request[4096];
    char buffer[1024];
    char bssid[256];
    char disassoc_imminent[256];
    char disassoc_timer[256];
    char candidate_list[256];
    char reassoc_retry_delay[256];
    char bss_term_bit[256];
    char bss_term_tsf[256];
    char bss_term_duration[256];
#ifdef CONTROLAPPC_LINUX_API
	size_t resp_len;
	struct wpa_ctrl *w = NULL;
	char response[4096];

#endif
    memset(bssid, 0, sizeof(bssid));
    memset(disassoc_imminent, 0, sizeof(disassoc_imminent));
    memset(disassoc_timer, 0, sizeof(disassoc_timer));
    memset(candidate_list, 0, sizeof(candidate_list));
    memset(reassoc_retry_delay, 0, sizeof(reassoc_retry_delay));
    memset(bss_term_bit, 0, sizeof(bss_term_bit));
    memset(bss_term_tsf, 0, sizeof(bss_term_tsf));
    memset(bss_term_duration, 0, sizeof(bss_term_duration));

    /* ControlApp on DUT */
    /* TLV: BSSID (required) */
    tlv = find_wrapper_tlv_by_id(req, TLV_BSSID);
    if (tlv) {
        memcpy(bssid, tlv->value, tlv->len);
    }
    /* DISASSOC_IMMINENT            disassoc_imminent=%s */
    tlv = find_wrapper_tlv_by_id(req, TLV_DISASSOC_IMMINENT);
    if (tlv) {
        memcpy(disassoc_imminent, tlv->value, tlv->len);
    }
    /* DISASSOC_TIMER               disassoc_timer=%s */
    tlv = find_wrapper_tlv_by_id(req, TLV_DISASSOC_TIMER);
    if (tlv) {
        memcpy(disassoc_timer, tlv->value, tlv->len);
    }
    /* REASSOCIAITION_RETRY_DELAY   mbo=0:{}:0 */
    tlv = find_wrapper_tlv_by_id(req, TLV_REASSOCIAITION_RETRY_DELAY);
    if (tlv) {
        memcpy(reassoc_retry_delay, tlv->value, tlv->len);
    }
    /* CANDIDATE_LIST              pref=1 */
    tlv = find_wrapper_tlv_by_id(req, TLV_CANDIDATE_LIST);
    if (tlv) {
        memcpy(candidate_list, tlv->value, tlv->len);
    }
    /* BSS_TERMINATION              bss_term_bit */
    tlv = find_wrapper_tlv_by_id(req, TLV_BSS_TERMINATION);
    if (tlv) {
        memcpy(bss_term_bit, tlv->value, tlv->len);
    }
    /* BSS_TERMINATION_TSF          bss_term_tsf */
    tlv = find_wrapper_tlv_by_id(req, TLV_BSS_TERMINATION_TSF);
    if (tlv) {
        memcpy(bss_term_tsf, tlv->value, tlv->len);
    }
    /* BSS_TERMINATION_DURATION     bss_term_duration */
    tlv = find_wrapper_tlv_by_id(req, TLV_BSS_TERMINATION_DURATION);
    if (tlv) {
        memcpy(bss_term_duration, tlv->value, tlv->len);
    }

    /* Assemble hostapd command for BSS_TM_REQ */
    memset(request, 0, sizeof(request));
    sprintf(request, "BSS_TM_REQ %s", bssid);
    /*  disassoc_imminent=%s */
    if (strlen(disassoc_imminent)) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, " disassoc_imminent=%s", disassoc_imminent);
        strcat(request, buffer);
    }
    /* disassoc_timer=%s */
    if (strlen(disassoc_timer)) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, " disassoc_timer=%s", disassoc_timer);
        strcat(request, buffer);
    }
    /* reassoc_retry_delay=%s */
    if (strlen(reassoc_retry_delay)) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, " mbo=0:%s:0", reassoc_retry_delay);
        strcat(request, buffer);
    }
    /* if bss_term_bit && bss_term_tsf && bss_term_duration, then bss_term={bss_term_tsf},{bss_term_duration} */
    if (strlen(bss_term_bit) && strlen(bss_term_tsf) && strlen(bss_term_duration) ) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, " bss_term=%s,%s", bss_term_tsf, bss_term_duration);
        strcat(request, buffer);
    }
    /* candidate_list */
    if (strlen(candidate_list) && atoi(candidate_list) == 1) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, " pref=1");
        strcat(request, buffer);
    }
    indigo_logger(LOG_LEVEL_DEBUG, "cmd:%s", request);

#ifdef CONTROLAPPC_LINUX_API
    /* Open hostapd UDS socket */
    w = wpa_ctrl_open(get_hapd_ctrl_path());
    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to hostapd");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_HOSTAPD_CTRL_NOT_OK;
        goto done;
    }
    resp_len = sizeof(response) - 1;
    wpa_ctrl_request(w, request, strlen(request), response, &resp_len, NULL);
    /* Check response */
    if (strncmp(response, WPA_CTRL_OK, strlen(WPA_CTRL_OK)) != 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to execute the command. Response: %s", response);
        message = TLV_VALUE_HOSTAPD_RESP_NOT_OK;
        goto done;
    }
#endif
    status = TLV_VALUE_STATUS_OK;
    message = TLV_VALUE_OK;
//done:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
#ifdef CONTROLAPPC_LINUX_API
    if (w) {
        wpa_ctrl_close(w);
    }
#endif
    return 0;
}

static int trigger_ap_channel_switch(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int status = TLV_VALUE_STATUS_NOT_OK;
    char *message = NULL;
    struct tlv_hdr *tlv = NULL;
    char request[S_BUFFER_LEN];
#ifdef CONTROLAPPC_LINUX_API
	size_t resp_len;
	struct wpa_ctrl *w = NULL;
	char response[S_BUFFER_LEN];
#endif
    char channel[64];
    char frequency[64];
    int freq, center_freq, offset;

    memset(channel, 0, sizeof(channel));
    memset(frequency, 0, sizeof(frequency));

    /* ControlApp on DUT */
    /* TLV: TLV_CHANNEL (required) */
    tlv = find_wrapper_tlv_by_id(req, TLV_CHANNEL);
    if (tlv) {
        memcpy(channel, tlv->value, tlv->len);
    } else {
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_INSUFFICIENT_TLV;
        indigo_logger(LOG_LEVEL_ERROR, "Missed TLV: TLV_CHANNEL");
        goto done;
    }
    /* TLV_FREQUENCY (required) */
    tlv = find_wrapper_tlv_by_id(req, TLV_FREQUENCY);
    if (tlv) {
        memcpy(frequency, tlv->value, tlv->len);
    } else {
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_INSUFFICIENT_TLV;
        indigo_logger(LOG_LEVEL_ERROR, "Missed TLV: TLV_FREQUENCY");
    }

    center_freq = 5000 + get_center_freq_index(atoi(channel), 1) * 5;
    freq = atoi(frequency);
    if ((center_freq == freq + 30) || (center_freq == freq - 10))
        offset = 1;
    else
        offset = -1;
    /* Assemble hostapd command for channel switch */
    memset(request, 0, sizeof(request));
    sprintf(request, "CHAN_SWITCH 10 %s center_freq1=%d sec_channel_offset=%d bandwidth=80 vht", frequency, center_freq, offset);
    indigo_logger(LOG_LEVEL_INFO, "%s", request);
#ifdef CONTROLAPPC_LINUX_API
    /* Open hostapd UDS socket */
    w = wpa_ctrl_open(get_hapd_ctrl_path());
    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to hostapd");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_HOSTAPD_CTRL_NOT_OK;
        goto done;
    }
    resp_len = sizeof(response) - 1;
    wpa_ctrl_request(w, request, strlen(request), response, &resp_len, NULL);
    /* Check response */
    if (strncmp(response, WPA_CTRL_OK, strlen(WPA_CTRL_OK)) != 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to execute the command. Response: %s", response);
        message = TLV_VALUE_HOSTAPD_RESP_NOT_OK;
        goto done;
    }
#endif
    status = TLV_VALUE_STATUS_OK;
    message = TLV_VALUE_OK;
done:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
#ifdef CONTROLAPPC_LINUX_API
    if (w) {
        wpa_ctrl_close(w);
    }
#endif
    return 0;
}

static int get_ip_addr_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int status = TLV_VALUE_STATUS_NOT_OK;
    char *message = NULL;
    char buffer[64];

#ifndef BEKEN_API
    if (find_interface_ip(buffer, sizeof(buffer), BRIDGE_WLANS)) {
        status = TLV_VALUE_STATUS_OK;
        message = TLV_VALUE_OK;
    } else if (find_interface_ip(buffer, sizeof(buffer), get_wireless_interface())) {
        status = TLV_VALUE_STATUS_OK;
        message = TLV_VALUE_OK;
    } else {
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_NOT_OK;
    }
#else
    {
		struct wlan_ip_config addr;
    	net_get_if_addr(&addr, net_get_sta_handle());
		os_strcpy(buffer, wlan_ipv4_ntoa(&addr));

		status = TLV_VALUE_STATUS_OK;
		message = TLV_VALUE_OK;
    }
#endif
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
    if (status == TLV_VALUE_STATUS_OK) {
        fill_wrapper_tlv_bytes(resp, TLV_DUT_WLAN_IP_ADDR, strlen(buffer), buffer);
    }
    return 0;
}

static int stop_sta_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int len = 0, reset = 0;
    char reset_type[16];

    char *message = TLV_VALUE_WPA_S_STOP_OK;
    struct tlv_hdr *tlv = NULL;

#ifdef BEKEN_API
	wlan_sta_disable();
	//wlan_sta_disconnect();
#endif

#ifdef CONTROLAPPC_LINUX_API
	char buffer[S_BUFFER_LEN];
	char *parameter[] = {"pidof", get_wpas_exec_file(), NULL};
#endif
    /* TLV: RESET_TYPE */
    tlv = find_wrapper_tlv_by_id(req, TLV_RESET_TYPE);
    memset(reset_type, 0, sizeof(reset_type));
    if (tlv) {
        memcpy(reset_type, tlv->value, tlv->len);
        reset = atoi(reset_type);
        indigo_logger(LOG_LEVEL_DEBUG, "Reset Type: %d", reset);
    }
#ifdef CONTROLAPPC_LINUX_API
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "killall %s 1>/dev/null 2>/dev/null", get_wpas_exec_file());
    system(buffer);
    sleep(2);
#endif
    /* Test case teardown case */
    if (reset == RESET_TYPE_TEARDOWN) {
    }

    if (reset == RESET_TYPE_INIT) {
#ifdef CONTROLAPPC_LINUX_API
        /* clean the log */
        system("rm -rf /var/log/supplicant.log >/dev/null 2>/dev/null");

        /* remove pac file if needed */
        if (strlen(pac_file_path)) {
            remove_pac_file(pac_file_path);
            memset(pac_file_path, 0, sizeof(pac_file_path));
        }
#endif
    }

    len = reset_interface_ip(get_wireless_interface());
    if (len) {
        indigo_logger(LOG_LEVEL_DEBUG, "Failed to free IP address");
    }
#ifdef CONTROLAPPC_LINUX_API
    sleep(1);

    len = pipe_command(buffer, sizeof(buffer), "/bin/pidof", parameter);
    if (len) {
        message = TLV_VALUE_WPA_S_STOP_NOT_OK;
    } else {
        message = TLV_VALUE_WPA_S_STOP_OK;
    }
#endif
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, len == 0 ? TLV_VALUE_STATUS_OK : TLV_VALUE_STATUS_NOT_OK);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);

    return 0;
}

#ifdef _RESERVED_
/* The function is reserved for the defeault wpas config */
#define WPAS_DEFAULT_CONFIG_SSID                    "QuickTrack"
#define WPAS_DEFAULT_CONFIG_WPA_KEY_MGMT            "WPA-PSK"
#define WPAS_DEFAULT_CONFIG_PROTO                   "RSN"
#define HOSTAPD_DEFAULT_CONFIG_RSN_PAIRWISE         "CCMP"
#define WPAS_DEFAULT_CONFIG_WPA_PASSPHRASE          "12345678"

static void append_wpas_network_default_config(struct packet_wrapper *wrapper) {
    if (find_wrapper_tlv_by_id(wrapper, TLV_SSID) == NULL) {
        add_wrapper_tlv(wrapper, TLV_SSID, strlen(WPAS_DEFAULT_CONFIG_SSID), WPAS_DEFAULT_CONFIG_SSID);
    }
    if (find_wrapper_tlv_by_id(wrapper, TLV_WPA_KEY_MGMT) == NULL) {
        add_wrapper_tlv(wrapper, TLV_WPA_KEY_MGMT, strlen(WPAS_DEFAULT_CONFIG_WPA_KEY_MGMT), WPAS_DEFAULT_CONFIG_WPA_KEY_MGMT);
    }
    if (find_wrapper_tlv_by_id(wrapper, TLV_PROTO) == NULL) {
        add_wrapper_tlv(wrapper, TLV_PROTO, strlen(WPAS_DEFAULT_CONFIG_PROTO), WPAS_DEFAULT_CONFIG_PROTO);
    }
    if (find_wrapper_tlv_by_id(wrapper, TLV_RSN_PAIRWISE) == NULL) {
        add_wrapper_tlv(wrapper, TLV_RSN_PAIRWISE, strlen(HOSTAPD_DEFAULT_CONFIG_RSN_PAIRWISE), HOSTAPD_DEFAULT_CONFIG_RSN_PAIRWISE);
    }
    if (find_wrapper_tlv_by_id(wrapper, TLV_WPA_PASSPHRASE) == NULL) {
        add_wrapper_tlv(wrapper, TLV_WPA_PASSPHRASE, strlen(WPAS_DEFAULT_CONFIG_WPA_PASSPHRASE), WPAS_DEFAULT_CONFIG_WPA_PASSPHRASE);
    }
}
#endif /* _RESERVED_ */

#ifdef BEKEN_API
void generate_wpas_config(struct packet_wrapper *wrapper) {
    int i;
    char value[128] = {0};
    struct tlv_to_config_name* cfg = NULL;
    int ieee80211w_configured = 0;
    int transition_mode_enabled = 0;
    int owe_configured = 0;
    int sae_only = 0;
	wlan_sta_config_t config;

#ifdef _RESERVED_
    /* The function is reserved for the defeault wpas config */
    append_wpas_network_default_config(wrapper);
#endif /* _RESERVED_ */

	os_memset(&quickTrackNetwork, 0, sizeof(quickTrackNetwork));

	/* int some fields */
	quickTrackNetwork.key_mgmt = WPA_KEY_MGMT_PSK | WPA_KEY_MGMT_SAE;
	quickTrackNetwork.pairwise_cipher = WPA_CIPHER_CCMP | WPA_CIPHER_TKIP;
	quickTrackNetwork.group_cipher = WPA_CIPHER_CCMP | WPA_CIPHER_TKIP;
	quickTrackNetwork.proto = WPA_PROTO_WPA | WPA_PROTO_RSN;
	quickTrackNetwork.ieee80211w = 1;

    for (i = 0; i < wrapper->tlv_num; i++) {
        cfg = find_tlv_config(wrapper->tlv[i]->id);
        if (cfg) {
            memset(value, 0, sizeof(value));
            memcpy(value, wrapper->tlv[i]->value, wrapper->tlv[i]->len);
			os_memset(&config, 0, sizeof(config));

			if (wrapper->tlv[i]->id == TLV_STA_SSID) {
				os_memcpy(quickTrackNetwork.wifi_ssid, wrapper->tlv[i]->value, wrapper->tlv[i]->len);
			} else if (wrapper->tlv[i]->id == TLV_KEY_MGMT) {
				if (strstr(value, "WPA-PSK") && strstr(value, "SAE")) {
					transition_mode_enabled = 1;
				}
				if (!strstr(value, "WPA-PSK") && strstr(value, "SAE")) {
					sae_only = 1;
				}
				if (strstr(value, "OWE")) {
					owe_configured = 1;
				}
				int key_mgmt = cmd_wpas_parse_key_mgmt(value);
				if (key_mgmt > 0) {
					config.field = WLAN_STA_FIELD_KEY_MGMT;
					config.u.key_mgmt = key_mgmt;
					//wlan_sta_set_config(&config);
				}
				quickTrackNetwork.key_mgmt = key_mgmt;
			} else if (wrapper->tlv[i]->id == TLV_STA_WEP_KEY0) {
				config.field = WLAN_STA_FIELD_WEP_KEY0;
				os_strlcpy((char *)config.u.wep_key, value, sizeof(config.u.wep_key));
				//wlan_sta_set_config(&config);
				os_memcpy(quickTrackNetwork.wifi_key, wrapper->tlv[i]->value, wrapper->tlv[i]->len);
			} else if (wrapper->tlv[i]->id == TLV_WEP_TX_KEYIDX) {
				config.field = WLAN_STA_FIELD_WEP_KEY_INDEX;
				config.u.wep_tx_keyidx = atoi(value);
				//wlan_sta_set_config(&config);
			} else if (wrapper->tlv[i]->id == TLV_GROUP) {
				int group_cipher = cmd_wpas_parse_cipher(value);
				if (group_cipher > 0) {
					config.field = WLAN_STA_FIELD_GROUP_CIPHER;
					config.u.group_cipher = group_cipher;
					//wlan_sta_set_config(&config);
				}
				quickTrackNetwork.group_cipher = group_cipher;
			} else if (wrapper->tlv[i]->id == TLV_PSK) {
				//config.field = WLAN_STA_FIELD_PSK;
				//os_strlcpy((char *)config.u.psk, value, wrapper->tlv[i]->len);
				//os_strlcpy((char*)(g_sta_param_ptr->key), value, wrapper->tlv[i]->len);
				//g_sta_param_ptr->key_len = wrapper->tlv[i]->len;
				//wlan_sta_set_config(&config);
				os_memcpy(quickTrackNetwork.wifi_key, wrapper->tlv[i]->value, wrapper->tlv[i]->len);
			} else if (wrapper->tlv[i]->id == TLV_PROTO) {
				int proto = cmd_wpas_parse_proto(value);
				if (proto >= 0) {
					config.field = WLAN_STA_FIELD_PROTO;
					config.u.proto = proto;
					//wlan_sta_set_config(&config);
				}
				quickTrackNetwork.proto = proto;
			} else if (wrapper->tlv[i]->id == TLV_STA_IEEE80211_W) {
				ieee80211w_configured = 1;
				quickTrackNetwork.ieee80211w = atoi((char *)wrapper->tlv[i]->value);
			} else if (wrapper->tlv[i]->id == TLV_PAIRWISE) {
				int pairwise_cipher = cmd_wpas_parse_cipher(value);
				if (pairwise_cipher > 0) {
					config.field = WLAN_STA_FIELD_PAIRWISE_CIPHER;
					config.u.pairwise_cipher = pairwise_cipher;
					//wlan_sta_set_config(&config);
				}
				quickTrackNetwork.pairwise_cipher = pairwise_cipher;
			}
        }
    }

	os_memset(&config, 0, sizeof(config));
    if (ieee80211w_configured == 0) {
		config.field = WLAN_STA_FIELD_MFP;
        if (transition_mode_enabled) {
			config.u.ieee80211w = 1;
			quickTrackNetwork.ieee80211w = 1;
        } else if (sae_only) {
			config.u.ieee80211w = 2;
			quickTrackNetwork.ieee80211w = 2;
        } else if (owe_configured) {
			config.u.ieee80211w = 2;
			quickTrackNetwork.ieee80211w = 2;
        } else {
			// defaults to MPF Optional
			quickTrackNetwork.ieee80211w = 1;
		}
    }

	//wlan_sta_set_config(&config);


	//bk_wlan_start_sta();
}

#else
static int generate_wpas_config(char *buffer, int buffer_size, struct packet_wrapper *wrapper) {
    int i;
    char value[S_BUFFER_LEN], cfg_item[2*S_BUFFER_LEN], buf[S_BUFFER_LEN];
    int ieee80211w_configured = 0;
    int transition_mode_enabled = 0;
    int owe_configured = 0;
    int sae_only = 0;

    struct tlv_to_config_name* cfg = NULL;

    sprintf(buffer, "ctrl_interface=%s\nap_scan=1\npmf=1\n", WPAS_CTRL_PATH_DEFAULT);

    for (i = 0; i < wrapper->tlv_num; i++) {
        cfg = find_wpas_global_config_name(wrapper->tlv[i]->id);
        if (cfg) {
            memset(value, 0, sizeof(value));
            memcpy(value, wrapper->tlv[i]->value, wrapper->tlv[i]->len);
            sprintf(cfg_item, "%s=%s\n", cfg->config_name, value);
            strcat(buffer, cfg_item);
        }
    }
    strcat(buffer, "network={\n");

#ifdef _RESERVED_
    /* The function is reserved for the defeault wpas config */
    append_wpas_network_default_config(wrapper);
#endif /* _RESERVED_ */

    for (i = 0; i < wrapper->tlv_num; i++) {
        cfg = find_tlv_config(wrapper->tlv[i]->id);
        if (cfg && find_wpas_global_config_name(wrapper->tlv[i]->id) == NULL) {
            memset(value, 0, sizeof(value));
            memcpy(value, wrapper->tlv[i]->value, wrapper->tlv[i]->len);

            if ((wrapper->tlv[i]->id == TLV_IEEE80211_W) || (wrapper->tlv[i]->id == TLV_STA_IEEE80211_W)) {
                ieee80211w_configured = 1;
            } else if (wrapper->tlv[i]->id == TLV_KEY_MGMT) {
                if (strstr(value, "WPA-PSK") && strstr(value, "SAE")) {
                    transition_mode_enabled = 1;
                }
                if (!strstr(value, "WPA-PSK") && strstr(value, "SAE")) {
                    sae_only = 1;
                }

                if (strstr(value, "OWE")) {
                    owe_configured = 1;
                }
            } else if ((wrapper->tlv[i]->id == TLV_CA_CERT) && strcmp("DEFAULT", value) == 0) {
                sprintf(value, "/etc/ssl/certs/ca-certificates.crt");
            } else if ((wrapper->tlv[i]->id == TLV_PAC_FILE)) {
                memset(pac_file_path, 0, sizeof(pac_file_path));
                snprintf(pac_file_path, sizeof(pac_file_path), "%s", value);
            } else if (wrapper->tlv[i]->id == TLV_SERVER_CERT) {
                memset(buf, 0, sizeof(buf));
                get_server_cert_hash(value, buf);
                memcpy(value, buf, sizeof(buf));
            }

            if (cfg->quoted) {
                sprintf(cfg_item, "%s=\"%s\"\n", cfg->config_name, value);
                strcat(buffer, cfg_item);
            } else {
                sprintf(cfg_item, "%s=%s\n", cfg->config_name, value);
                strcat(buffer, cfg_item);
            }
        }
    }

    if (ieee80211w_configured == 0) {
        if (transition_mode_enabled) {
            strcat(buffer, "ieee80211w=1\n");
        } else if (sae_only) {
            strcat(buffer, "ieee80211w=2\n");
        } else if (owe_configured) {
            strcat(buffer, "ieee80211w=2\n");
        }
    }

    /* TODO: merge another file */
    /* python source code:
        if merge_config_file:
        appended_supplicant_conf_str = ""
        existing_conf = StaCommandHelper.get_existing_supplicant_conf()
        wpa_supplicant_dict = StaCommandHelper.__convert_config_str_to_dict(config = wps_config)
        for each_key in existing_conf:
            if each_key not in wpa_supplicant_dict:
                wpa_supplicant_dict[each_key] = existing_conf[each_key]

        for each_supplicant_conf in wpa_supplicant_dict:
            appended_supplicant_conf_str += each_supplicant_conf + "=" + wpa_supplicant_dict[each_supplicant_conf] + "\n"
        wps_config = appended_supplicant_conf_str.rstrip()
    */

    strcat(buffer, "}\n");

    return strlen(buffer);
}
#endif
static int configure_sta_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
#if 0
    int len;
    char buffer[L_BUFFER_LEN];
#endif
    char *message = "DUT configured as STA : Configuration file created";

#if 0
    memset(buffer, 0, sizeof(buffer));
    len = generate_wpas_config(buffer, sizeof(buffer), req);
    if (len) {
        write_file(get_wpas_conf_file(), buffer, len);
    }
#endif

#ifdef BEKEN_API
	indigo_logger(LOG_LEVEL_ERROR, "generate wpas config");
	wlan_sta_enable();
	generate_wpas_config(req);
#endif

    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, TLV_VALUE_STATUS_OK);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);

    return 0;
}

static int associate_sta_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    char *message = TLV_VALUE_WPA_S_START_UP_NOT_OK;
    int len __maybe_unused, status = TLV_VALUE_STATUS_NOT_OK;
#ifdef CONTROLAPPC_LINUX_API
    char buffer[256];
#endif

	indigo_logger(LOG_LEVEL_ERROR, "associate_sta_handler");

#ifdef CONTROLAPPC_LINUX_API
#ifdef _OPENWRT_
#else
    system("rfkill unblock wlan");
    sleep(1);
#endif

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "killall %s 1>/dev/null 2>/dev/null", get_wpas_exec_file());
    system(buffer);
    sleep(3);

    /* Start WPA supplicant */
    memset(buffer, 0 ,sizeof(buffer));
    sprintf(buffer, "%s -B -t -c %s %s -i %s -f /var/log/supplicant.log",
        get_wpas_full_exec_path(),
        get_wpas_conf_file(),
        get_wpas_debug_arguments(),
        get_wireless_interface());
    len = system(buffer);
    sleep(2);
#endif

    status = TLV_VALUE_STATUS_OK;
    message = TLV_VALUE_WPA_S_START_UP_OK;

    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);

#ifdef BEKEN_API
//	wlan_sta_enable();
//	wlan_sta_connect(0);
	rtos_thread_sleep(2);	// to let server's sniffer start
	bk_wlan_start_sta(&quickTrackNetwork);
#endif

    return 0;
}

static int send_sta_disconnect_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    char *message = TLV_VALUE_WPA_S_DISCONNECT_NOT_OK;
    int status = TLV_VALUE_STATUS_NOT_OK;

#ifdef CONTROLAPPC_LINUX_API
    struct wpa_ctrl *w = NULL;
    char buffer[256], response[1024];
    size_t resp_len;
#endif

#ifdef BEKEN_API
	wlan_sta_disconnect();
#endif

#ifdef CONTROLAPPC_LINUX_API
    /* Open WPA supplicant UDS socket */
    w = wpa_ctrl_open(get_wpas_ctrl_path());
    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to wpa_supplicant");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_WPA_S_DISCONNECT_NOT_OK;
        goto done;
    }
    /* Send command to hostapd UDS socket */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "DISCONNECT");
    memset(response, 0, sizeof(response));
    resp_len = sizeof(response) - 1;
    wpa_ctrl_request(w, buffer, strlen(buffer), response, &resp_len, NULL);
    if (strncmp(response, WPA_CTRL_OK, strlen(WPA_CTRL_OK)) != 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to execute the command. Response: %s", response);
        goto done;
    }
#endif
    status = TLV_VALUE_STATUS_OK;
    message = TLV_VALUE_WPA_S_DISCONNECT_OK;

//done:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
#ifdef CONTROLAPPC_LINUX_API
    if (w) {
        wpa_ctrl_close(w);
    }
#endif
    return 0;
}

static int send_sta_reconnect_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    char *message = TLV_VALUE_WPA_S_RECONNECT_NOT_OK;
    int status = TLV_VALUE_STATUS_NOT_OK;

#ifdef CONTROLAPPC_LINUX_API
    struct wpa_ctrl *w = NULL;
    char buffer[256], response[1024];
    size_t resp_len;
#endif

#ifdef CONTROLAPPC_LINUX_API
    /* Open WPA supplicant UDS socket */
    w = wpa_ctrl_open(get_wpas_ctrl_path());
    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to wpa_supplicant");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_WPA_S_RECONNECT_NOT_OK;
        goto done;
    }
    /* Send command to hostapd UDS socket */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "RECONNECT");
    memset(response, 0, sizeof(response));
    resp_len = sizeof(response) - 1;
    wpa_ctrl_request(w, buffer, strlen(buffer), response, &resp_len, NULL);
    if (strncmp(response, WPA_CTRL_OK, strlen(WPA_CTRL_OK)) != 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to execute the command. Response: %s", response);
        goto done;
    }
#endif
    status = TLV_VALUE_STATUS_OK;
    message = TLV_VALUE_WPA_S_RECONNECT_OK;

//done:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
#ifdef CONTROLAPPC_LINUX_API
    if (w) {
        wpa_ctrl_close(w);
    }
#endif

#ifdef BEKEN_API
	rtos_thread_sleep(2);	// let sniffer start
	wlan_sta_connect(0);
	ip_address_set(BK_STATION, DHCP_CLIENT, NULL, NULL, NULL, NULL);
#endif

    return 0;
}

static int set_sta_parameter_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int status = TLV_VALUE_STATUS_NOT_OK;
    char *message = NULL;
    char param_name[32];
    char param_value[256];
    struct tlv_hdr *tlv = NULL;

#ifdef CONTROLAPPC_LINUX_API
    size_t resp_len;
    char buffer[BUFFER_LEN];
    char response[BUFFER_LEN];
    struct wpa_ctrl *w = NULL;
#endif
    /* Open wpa_supplicant UDS socket */
#ifdef CONTROLAPPC_LINUX_API
    w = wpa_ctrl_open(get_wpas_ctrl_path());
    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to wpa_supplicant");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_WPA_S_CTRL_NOT_OK;
        goto done;
    }
#endif
    /* Example: Use TLV_STA_IEEE80211_W. Change to corresponding TLV from Tool */
    memset(param_value, 0, sizeof(param_value));
    tlv = find_wrapper_tlv_by_id(req, TLV_STA_IEEE80211_W);
    if (tlv && find_tlv_config_name(tlv->id) != NULL) {
        strcpy(param_name, find_tlv_config_name(tlv->id));
        memcpy(param_value, tlv->value, tlv->len);
    } else {
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_INSUFFICIENT_TLV;
        indigo_logger(LOG_LEVEL_ERROR, "Missed TLV: STA_IEEE80211_W");
        goto done;
    }

#ifdef BEKEN_API
	{
        wlan_sta_config_t config;

		config.u.ieee80211w = atoi(param_value);
		wlan_sta_set_config(&config);
	}
#endif

#ifdef CONTROLAPPC_LINUX_API
    /* Assemble wpa_supplicant command */
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "SET %s %s", param_name, param_value);
    /* Send command to wpa_supplicant UDS socket */
    resp_len = sizeof(response) - 1;
    wpa_ctrl_request(w, buffer, strlen(buffer), response, &resp_len, NULL);
    /* Check response */
    if (strncmp(response, WPA_CTRL_OK, strlen(WPA_CTRL_OK)) != 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to execute the command. Response: %s", response);
        goto done;
    }
#endif
    status = TLV_VALUE_STATUS_OK;
    message = TLV_VALUE_OK;
done:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
#ifdef CONTROLAPPC_LINUX_API
    if (w) {
        wpa_ctrl_close(w);
    }
#endif
    return 0;
}

static int send_sta_btm_query_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int status = TLV_VALUE_STATUS_NOT_OK;
    char *message = TLV_VALUE_WPA_S_BTM_QUERY_NOT_OK;
    char reason_code[256];
    char candidate_list[256] = {0};
    struct tlv_hdr *tlv = NULL;

#ifdef CONTROLAPPC_LINUX_API
	size_t resp_len;
	char buffer[1024];
	char response[1024];
	struct wpa_ctrl *w = NULL;
#endif

#ifdef CONTROLAPPC_LINUX_API
    /* Open wpa_supplicant UDS socket */
    w = wpa_ctrl_open(get_wpas_ctrl_path());
    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to wpa_supplicant");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_WPA_S_CTRL_NOT_OK;
        goto done;
    }
#endif
    /* TLV: BTMQUERY_REASON_CODE */
    tlv = find_wrapper_tlv_by_id(req, TLV_BTMQUERY_REASON_CODE);
    if (tlv) {
        memcpy(reason_code, tlv->value, tlv->len);
    } else {
        goto done;
    }

    /* TLV: TLV_CANDIDATE_LIST */
    tlv = find_wrapper_tlv_by_id(req, TLV_CANDIDATE_LIST);
    if (tlv) {
        memcpy(candidate_list, tlv->value, tlv->len);
    }

#ifdef CONTROLAPPC_LINUX_API
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "WNM_BSS_QUERY %s", reason_code);
    if (strcmp(candidate_list, "1") == 0) {
        strcat(buffer, " list");
    }

    /* Send command to wpa_supplicant UDS socket */
    resp_len = sizeof(response) - 1;
    wpa_ctrl_request(w, buffer, strlen(buffer), response, &resp_len, NULL);
    /* Check response */
    if (strncmp(response, WPA_CTRL_OK, strlen(WPA_CTRL_OK)) != 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to execute the command. Response: %s", response);
        goto done;
    }
#endif
    status = TLV_VALUE_STATUS_OK;
    message = TLV_VALUE_OK;

done:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
#ifdef CONTROLAPPC_LINUX_API
    if (w) {
        wpa_ctrl_close(w);
    }
#endif
    return 0;
}

static int send_sta_anqp_query_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int status = TLV_VALUE_STATUS_NOT_OK;
    char *message = TLV_VALUE_WPA_S_BTM_QUERY_NOT_OK;
    char bssid[256];
    char anqp_info_id[256] = {0};
    struct tlv_hdr *tlv = NULL;

#ifdef CONTROLAPPC_LINUX_API
	int len;
	char buffer[1024];
	char response[1024];
	struct wpa_ctrl *w = NULL;
	size_t resp_len;
#endif
#ifdef CONTROLAPPC_LINUX_API
    /* It may need to check whether to just scan */
    memset(buffer, 0, sizeof(buffer));
    len = sprintf(buffer, "ctrl_interface=%s\nap_scan=1\n", WPAS_CTRL_PATH_DEFAULT);
    if (len) {
        write_file(get_wpas_conf_file(), buffer, len);
    }

    memset(buffer, 0 ,sizeof(buffer));
    sprintf(buffer, "%s -B -t -c %s -i %s -f /var/log/supplicant.log",
        get_wpas_full_exec_path(),
        get_wpas_conf_file(),
        get_wireless_interface());
    len = system(buffer);
    sleep(2);

    /* Open wpa_supplicant UDS socket */
    w = wpa_ctrl_open(get_wpas_ctrl_path());
    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to wpa_supplicant");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_WPA_S_CTRL_NOT_OK;
        goto done;
    }
    // SCAN
    memset(buffer, 0, sizeof(buffer));
    memset(response, 0, sizeof(response));
    sprintf(buffer, "SCAN");
    resp_len = sizeof(response) - 1;
    wpa_ctrl_request(w, buffer, strlen(buffer), response, &resp_len, NULL);
    /* Check response */
    if (strncmp(response, WPA_CTRL_OK, strlen(WPA_CTRL_OK)) != 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to execute the command. Response: %s", response);
        goto done;
    }
    sleep(10);
#endif
    /* TLV: BSSID */
    tlv = find_wrapper_tlv_by_id(req, TLV_BSSID);
    if (tlv) {
        memset(bssid, 0, sizeof(bssid));
        memcpy(bssid, tlv->value, tlv->len);
    } else {
        goto done;
    }

    /* TLV: ANQP_INFO_ID */
    tlv = find_wrapper_tlv_by_id(req, TLV_ANQP_INFO_ID);
    if (tlv) {
        memset(anqp_info_id, 0, sizeof(anqp_info_id));
        memcpy(anqp_info_id, tlv->value, tlv->len);
    }
#ifdef CONTROLAPPC_LINUX_API
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "ANQP_GET %s", bssid);
    if (strcmp(anqp_info_id, "NeighborReportReq") == 0) {
        strcat(buffer, " 272");
    } else if (strcmp(anqp_info_id, "QueryListWithCellPref") == 0) {
        strcat(buffer, " mbo:2");
    }

    /* Send command to wpa_supplicant UDS socket */
    resp_len = sizeof(response) - 1;
    wpa_ctrl_request(w, buffer, strlen(buffer), response, &resp_len, NULL);

    printf("%s -> resp: %s\n", buffer, response);
    /* Check response */
    if (strncmp(response, WPA_CTRL_OK, strlen(WPA_CTRL_OK)) != 0) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to execute the command. Response: %s", response);
        goto done;
    }
#endif
    status = TLV_VALUE_STATUS_OK;
    message = TLV_VALUE_OK;

done:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
#ifdef CONTROLAPPC_LINUX_API
    if (w) {
        wpa_ctrl_close(w);
    }
#endif
    return 0;
}
