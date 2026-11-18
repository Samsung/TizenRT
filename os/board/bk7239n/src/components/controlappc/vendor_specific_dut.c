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
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include "vendor_specific.h"
#include "utils.h"

#ifdef HOSTAPD_SUPPORT_MBSSID_WAR
extern int use_openwrt_wpad;
#endif

#if defined(_OPENWRT_)
int detect_third_radio() {
    FILE *fp;
    char buffer[BUFFER_LEN];
    int third_radio = 0;

    fp = popen("iw dev", "r");
    if (fp) {
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            if (strstr(buffer, "phy#2"))
                third_radio = 1;
        }
        pclose(fp);
    }

    return third_radio;
}
#endif

void interfaces_init() {
#if defined(_OPENWRT_) && !defined(_WTS_OPENWRT_)
    char buffer[BUFFER_LEN];
    char mac_addr[S_BUFFER_LEN];
    int third_radio = 0;

    third_radio = detect_third_radio();

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "iw phy phy1 interface add ath1 type managed >/dev/null 2>/dev/null");
    system(buffer);
    sprintf(buffer, "iw phy phy1 interface add ath11 type managed >/dev/null 2>/dev/null");
    system(buffer);
    sprintf(buffer, "iw phy phy0 interface add ath0 type managed >/dev/null 2>/dev/null");
    system(buffer);
    sprintf(buffer, "iw phy phy0 interface add ath01 type managed >/dev/null 2>/dev/null");
    system(buffer);
    if (third_radio == 1) {
        sprintf(buffer, "iw phy phy2 interface add ath2 type managed >/dev/null 2>/dev/null");
        system(buffer);
        sprintf(buffer, "iw phy phy2 interface add ath21 type managed >/dev/null 2>/dev/null");
        system(buffer);
    }

    memset(mac_addr, 0, sizeof(mac_addr));
    get_mac_address(mac_addr, sizeof(mac_addr), "ath1");
    control_interface("ath1", "down");
    mac_addr[16] = (char)'0';
    set_mac_address("ath1", mac_addr);

    control_interface("ath11", "down");
    mac_addr[16] = (char)'1';
    set_mac_address("ath11", mac_addr);

    memset(mac_addr, 0, sizeof(mac_addr));
    get_mac_address(mac_addr, sizeof(mac_addr), "ath0");
    control_interface("ath0", "down");
    mac_addr[16] = (char)'0';
    set_mac_address("ath0", mac_addr);

    control_interface("ath01", "down");
    mac_addr[16] = (char)'1';
    set_mac_address("ath01", mac_addr);

    if (third_radio == 1) {
        memset(mac_addr, 0, sizeof(mac_addr));
        get_mac_address(mac_addr, sizeof(mac_addr), "ath2");
        control_interface("ath2", "down");
        mac_addr[16] = (char)'8';
        set_mac_address("ath2", mac_addr);

        control_interface("ath21", "down");
        mac_addr[16] = (char)'9';
        set_mac_address("ath21", mac_addr);
    }
    sleep(1);
#endif
}
/* Be invoked when start controlApp */
void vendor_init() {
#if defined(_OPENWRT_) && !defined(_WTS_OPENWRT_)
    char buffer[BUFFER_LEN];
    char mac_addr[S_BUFFER_LEN];

    /* Vendor: add codes to let ControlApp have full control of hostapd */
    /* Avoid hostapd being invoked by procd */
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "/etc/init.d/wpad stop >/dev/null 2>/dev/null");
    system(buffer);

    interfaces_init();
#if HOSTAPD_SUPPORT_MBSSID
#ifdef HOSTAPD_SUPPORT_MBSSID_WAR
        system("cp /overlay/hostapd /usr/sbin/hostapd");
        use_openwrt_wpad = 0;
#endif
#endif
#endif
}

/* Be invoked when terminate controlApp */
void vendor_deinit() {
    char buffer[S_BUFFER_LEN];
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "killall %s 1>/dev/null 2>/dev/null", get_hapd_exec_file());
    system(buffer);
    sprintf(buffer, "killall %s 1>/dev/null 2>/dev/null", get_wpas_exec_file());
    system(buffer);
}

/* Called by reset_device_hander() */
void vendor_device_reset() {
#ifdef _WTS_OPENWRT_
    char buffer[S_BUFFER_LEN];

    /* Reset the country code */
    snprintf(buffer, sizeof(buffer), "uci -q delete wireless.wifi0.country");
    system(buffer);

    snprintf(buffer, sizeof(buffer), "uci -q delete wireless.wifi1.country");
    system(buffer);
#endif
#if HOSTAPD_SUPPORT_MBSSID
    /* interfaces may be destroyed by hostapd after done the MBSSID testing */
    interfaces_init();
#ifdef HOSTAPD_SUPPORT_MBSSID_WAR
    if (use_openwrt_wpad > 0) {
        system("cp /overlay/hostapd /usr/sbin/hostapd");
        use_openwrt_wpad = 0;
    }
#endif
#endif
}

#ifdef _OPENWRT_
void openwrt_apply_radio_config(void) {
    char buffer[S_BUFFER_LEN];

#ifdef _WTS_OPENWRT_
    // Apply radio configurations
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s -g /var/run/hostapd/global -B -P /var/run/hostapd-global.pid",
        get_hapd_full_exec_path());
    system(buffer);
    sleep(1);
    system("wifi down >/dev/null 2>/dev/null");
    sleep(2);
    system("wifi up >/dev/null 2>/dev/null");
    sleep(3);

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "killall %s 1>/dev/null 2>/dev/null", get_hapd_exec_file());
    system(buffer);
    sleep(2);
#endif
}
#endif

/* Called by configure_ap_handler() */
void configure_ap_enable_mbssid() {
#ifdef _WTS_OPENWRT_
    /*
     * the following uci commands need to reboot openwrt
     *    so it can not be configured by controlApp
     *
     * Manually enable MBSSID on OpenWRT when need to test MBSSID
     *
    system("uci set wireless.qcawifi=qcawifi");
    system("uci set wireless.qcawifi.mbss_ie_enable=1");
    system("uci commit");
    */
#elif defined(_OPENWRT_)
#ifdef HOSTAPD_SUPPORT_MBSSID_WAR
    system("cp /rom/usr/sbin/wpad /usr/sbin/hostapd");
    use_openwrt_wpad = 1;
#endif
#endif
}

void configure_ap_radio_params(char *band, char *country, int channel, int chwidth) {
#ifdef _WTS_OPENWRT
char buffer[S_BUFFER_LEN], wifi_name[16];

    if (!strncmp(band, "a", 1)) {
        snprintf(wifi_name, sizeof(wifi_name), "wifi0");
    } else {
        snprintf(wifi_name, sizeof(wifi_name), "wifi1");
    }

    if (strlen(country) > 0) {
        snprintf(buffer, sizeof(buffer), "uci set wireless.%s.country=\'%s\'", wifi_name, country);
        system(buffer);
    }

    snprintf(buffer, sizeof(buffer), "uci set wireless.%s.channel=\'%d\'", wifi_name, channel);
    system(buffer);

    if (!strncmp(band, "a", 1)) {
        if (channel == 165) { // Force 20M for CH 165
            snprintf(buffer, sizeof(buffer), "uci set wireless.wifi0.htmode=\'HT20\'");
        } else if (chwidth == 2) { // 160M test cases only
            snprintf(buffer, sizeof(buffer), "uci set wireless.wifi0.htmode=\'HT160\'");
        } else if (chwidth == 0) { // 11N only
            snprintf(buffer, sizeof(buffer), "uci set wireless.wifi0.htmode=\'HT40\'");
        } else { // 11AC or 11AX
            snprintf(buffer, sizeof(buffer), "uci set wireless.wifi0.htmode=\'HT80\'");
        }
        system(buffer);
    }

    system("uci commit");
#endif
}

/* void (*callback_fn)(void *), callback of active wlans iterator
 *
 * Called by start_ap_handler() after invoking hostapd
 */
void start_ap_set_wlan_params(void *if_info) {
    char buffer[S_BUFFER_LEN];
    struct interface_info *wlan __maybe_unused = (struct interface_info *) if_info;

    memset(buffer, 0, sizeof(buffer));
#ifdef _WTS_OPENWRT_
    /* Workaround: openwrt has IOT issue with intel AX210 AX mode */
    sprintf(buffer, "cfg80211tool %s he_ul_ofdma 0", wlan->ifname);
    system(buffer);
    /* Avoid target assert during channel switch */
    sprintf(buffer, "cfg80211tool %s he_ul_mimo 0", wlan->ifname);
    system(buffer);
    sprintf(buffer, "cfg80211tool %s twt_responder 0", wlan->ifname);
    system(buffer);
#endif
    printf("set_wlan_params: %s\n", buffer);
}
