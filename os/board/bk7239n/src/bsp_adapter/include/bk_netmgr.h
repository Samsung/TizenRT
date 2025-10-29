/******************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/
#ifndef __BK_NETMGR_H__
#define __BK_NETMGR_H__

#include <include/modules/wifi.h>
/* beken return values */
#define BK_STATUS_SUCCESS                             0 // Successfully completed
#define BK_STATUS_ERROR                               1 // Error  - unspecified
#define BK_STATUS_COMMAND_FAILED                      2 // Failed - command failed
#define BK_STATUS_COMMAND_UNKNOWN                     3 // Failed - command unknown
#define BK_STATUS_NOT_STARTED                         4 // Failed - mode never initiated
#define BK_STATUS_ALREADY_STARTED                     5 // Failed - mode already started
#define BK_STATUS_SUPPLICANT_START_FAILED             6 // Failed - start up of wpa_supplicant failed
#define BK_STATUS_PARAM_FAILED                        7 // Failed - parameter specified not valid
#define BK_STATUS_ALREADY_CONNECTED                   8 // Failed - WiFi already connected
#define BK_STATUS_NOT_CONNECTED                       9 // Failed - WiFi not connected
#define BK_STATUS_SECURITY_FAILED                     10    // Failed - security setup failed
#define BK_STATUS_NOT_ALLOWED                         11    // Failed - not allowed
#define BK_STATUS_NOT_SUPPORTED                       12    // Failed - function not supported (maybe due to missing dependencies to filesystem)

/* slsi wifi network interface - api parameter defines */
typedef enum WiFi_InterFace_ID {
    BK_WIFI_NONE,
    BK_WIFI_STATION_IF, // Station mode (turns on wpa_supplicant)
    BK_WIFI_SOFTAP_IF, // Soft AP mode (turns on hostapd)
    BK_WIFI_P2P_IF      // P2P mode (turns on wpa_supplicant)
} WiFi_InterFace_ID_t;

typedef struct bk_reason {
    uint32_t reason_code;           // Reason codes - 0 for success - error code see 'SLSI reason codes' above
    uint8_t locally_generated;      // Which side cause link down, 1 = locally, 0 = remotely - valid for STA mode only
    int8_t ssid_len;                // length of ssid - # of valid octets
    uint8_t ssid[33];               // 802.11 spec defined up to 32 octets of data
    char bssid[17];                 // BSS identification, char string e.g. xx:xx:xx:xx:xx:xx
    WiFi_InterFace_ID_t if_id;      // In which interface the reason was set
} bk_reason_t;

enum wpa_states {
    /**
     * WPA_DISCONNECTED - Disconnected state
     *
     * This state indicates that client is not associated, but is likely to
     * start looking for an access point. This state is entered when a
     * connection is lost.
     */
    WPA_DISCONNECTED,

    /**
     * WPA_INTERFACE_DISABLED - Interface disabled
     *
     * This state is entered if the network interface is disabled, e.g.,
     * due to rfkill. wpa_supplicant refuses any new operations that would
     * use the radio until the interface has been enabled.
     */
    WPA_INTERFACE_DISABLED,

    /**
     * WPA_INACTIVE - Inactive state (wpa_supplicant disabled)
     *
     * This state is entered if there are no enabled networks in the
     * configuration. wpa_supplicant is not trying to associate with a new
     * network and external interaction (e.g., ctrl_iface call to add or
     * enable a network) is needed to start association.
     */
    WPA_INACTIVE,

    /**
     * WPA_SCANNING - Scanning for a network
     *
     * This state is entered when wpa_supplicant starts scanning for a
     * network.
     */
    WPA_SCANNING,

    /**
     * WPA_AUTHENTICATING - Trying to authenticate with a BSS/SSID
     *
     * This state is entered when wpa_supplicant has found a suitable BSS
     * to authenticate with and the driver is configured to try to
     * authenticate with this BSS. This state is used only with drivers
     * that use wpa_supplicant as the SME.
     */
    WPA_AUTHENTICATING,

    /**
     * WPA_ASSOCIATING - Trying to associate with a BSS/SSID
     *
     * This state is entered when wpa_supplicant has found a suitable BSS
     * to associate with and the driver is configured to try to associate
     * with this BSS in ap_scan=1 mode. When using ap_scan=2 mode, this
     * state is entered when the driver is configured to try to associate
     * with a network using the configured SSID and security policy.
     */
    WPA_ASSOCIATING,

    /**
     * WPA_ASSOCIATED - Association completed
     *
     * This state is entered when the driver reports that association has
     * been successfully completed with an AP. If IEEE 802.1X is used
     * (with or without WPA/WPA2), wpa_supplicant remains in this state
     * until the IEEE 802.1X/EAPOL authentication has been completed.
     */
    WPA_ASSOCIATED,

    /**
     * WPA_4WAY_HANDSHAKE - WPA 4-Way Key Handshake in progress
     *
     * This state is entered when WPA/WPA2 4-Way Handshake is started. In
     * case of WPA-PSK, this happens when receiving the first EAPOL-Key
     * frame after association. In case of WPA-EAP, this state is entered
     * when the IEEE 802.1X/EAPOL authentication has been completed.
     */
    WPA_4WAY_HANDSHAKE,

    /**
     * WPA_GROUP_HANDSHAKE - WPA Group Key Handshake in progress
     *
     * This state is entered when 4-Way Key Handshake has been completed
     * (i.e., when the supplicant sends out message 4/4) and when Group
     * Key rekeying is started by the AP (i.e., when supplicant receives
     * message 1/2).
     */
    WPA_GROUP_HANDSHAKE,

    /**
     * WPA_COMPLETED - All authentication completed
     *
     * This state is entered when the full authentication process is
     * completed. In case of WPA2, this happens when the 4-Way Handshake is
     * successfully completed. With WPA, this state is entered after the
     * Group Key Handshake; with IEEE 802.1X (non-WPA) connection is
     * completed after dynamic keys are received (or if not used, after
     * the EAP authentication has been completed). With static WEP keys and
     * plaintext connections, this state is entered when an association
     * has been completed.
     *
     * This state indicates that the supplicant has completed its
     * processing for the association phase and that data connection is
     * fully configured.
     */
    WPA_COMPLETED
};

#endif