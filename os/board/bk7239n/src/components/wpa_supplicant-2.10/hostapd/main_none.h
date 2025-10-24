#ifndef _MAIN_NONE_H_
#define _MAIN_NONE_H_

#include "../src/utils/eloop.h"

#define CFG_CONFIG_FNAME          "beken_cfg_fname"
#define CFG_BSS_CONFIG            "wangzhilei_config:bss_fname"
#define CFG_AP_IFACE_CONFIG       "bss_config= wlan0"
#define WEP40_KEY_LENGTH           10

struct hapd_global {
	void **drv_priv;
	size_t drv_count;
};

extern const char *bss_iface;

void hostapd_thread_start(void);
void hostapd_thread_stop(void);
extern int hostapd_main_entry(int argc, char *argv[]);
extern int hostapd_channel_switch(int new_freq);
extern int supplicant_main_entry(char *oob_ssid);
extern int supplicant_main_exit(void);
extern void wpa_supplicant_poll(void *param);
extern int wpas_calculate_ap_pmk(const char *passphrase, const uint8_t *ssid, uint8_t *pmk);
int wpa_hostapd_queue_command(wpah_msg_t *msg);
int hostapd_main_exit(void);
int hostapd_main_entry(int argc, char *argv[]);
struct wpa_supplicant *wpa_suppliant_ctrl_get_wpas(void);
struct hapd_interfaces *hostapd_ctrl_get_interfaces(void);
bool hostapd_is_init_completed(void);
uint32_t wpa_hostapd_queue_poll(uint32_t param);
bool is_wpah_queue_full(void);
#endif // _MAIN_NONE_H_
// eof

