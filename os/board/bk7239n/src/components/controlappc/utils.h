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

#ifndef _INDIGO_UTILS_
#define _INDIGO_UTILS_  1

#include <stdbool.h>
#include "linux_defines.h"

#ifndef __maybe_unused
#define __maybe_unused __attribute__((unused))
#endif

#define S_BUFFER_LEN              512
#define BUFFER_LEN                1024
#define L_BUFFER_LEN              8192

#define TOOL_POST_PORT 8080
#define HAPD_UPLOAD_API "/upload-platform-hapd-log"
#define WPAS_UPLOAD_API "/upload-platform-wpas-log"

/* Log */
enum {
    LOG_LEVEL_DEBUG_VERBOSE = 0,
    LOG_LEVEL_DEBUG = 1,
    LOG_LEVEL_INFO = 2,
    LOG_LEVEL_NOTICE = 3,
    LOG_LEVEL_WARNING = 4,
    LOG_LEVEL_ERROR = 5
};

enum {
    BAND_24GHZ = 0,
    BAND_5GHZ = 1,
    BAND_6GHZ = 2
};

enum {
    PHYMODE_AUTO = 0,
    PHYMODE_11B = 1,
    PHYMODE_11BG = 2,
    PHYMODE_11BGN = 3,
    PHYMODE_11A = 4,
    PHYMODE_11NA = 5,
    PHYMODE_11AC = 6,
    PHYMODE_11AXG = 7,
    PHYMODE_11AXA = 8
};

enum {
    CHWIDTH_AUTO = 0,
    CHWIDTH_20 = 1,
    CHWIDTH_40 = 2,
    CHWIDTH_80 = 3,
    CHWIDTH_80PLUS80 = 4,
    CHWIDTH_160 = 5
};

enum {
    DATA_TYPE_UDP = 0,
    DATA_TYPE_ICMP = 1
};

enum {
    OP_CLASS_6G_20 = 131,
    OP_CLASS_6G_40 = 132,
    OP_CLASS_6G_80 = 133,
    OP_CLASS_6G_160 = 134
};

struct sta_platform_config {
    int phymode;
    int chwidth;
    bool phymode_isset;
    bool chwidth_isset;
};

struct channel_info {
    int channel;
    int freq;
};

#define UNUSED_IDENTIFIER -1
struct interface_info {
    int identifier; // valid only for multiple VAPs case
    int band;
    int bssid;
    char ifname[16];
    char ssid[64];
    int mbssid_enable;
    int transmitter;
    int hapd_bss_id;
    char hapd_conf_file[64];
};

struct bss_identifier_info {
    int identifier;
    int band;
    int mbssid_enable;
    int transmitter;
};

struct loopback_info {
    int sock;
    double rate;
    int pkt_sent;
    int pkt_rcv;
    int pkt_type;
    int pkt_size;
    char target_ip[64];
    char message[1600];
};

/* log and file API */
void indigo_logger(int level, const char *fmt, ...);
int pipe_command(char *buffer, int buffer_size, char *cmd, char *parameter[]);
char* read_file(char *fn);
int write_file(char *fn, char *buffer, int len);
int append_file(char *fn, char *buffer, int len);

/* network interface and loopback API */
int get_mac_address(char *buffer, int size, char *interface);
int set_mac_address(char *ifname, char *mac);
int find_interface_ip(char *ipaddr, int ipaddr_len, char *name);
int loopback_server_start(char *local_ip, char *local_port, int timeout);
int loopback_server_stop();
int loopback_server_status();
int send_udp_data(char *target_ip, int target_port, int packet_count, int packet_size, double rate);
int stop_loopback_data(int *pkt_sent);
int send_broadcast_arp(char *target_ip, int *send_count, int rate);
int send_icmp_data(char *target_ip, int packet_count, int packet_size, double rate);
int is_bridge_created();
int create_bridge(char *br);
int add_interface_to_bridge(char *br, char *interface);
int reset_bridge(char *br);
int control_interface(char *ifname, char *op);
int set_interface_ip(char *ifname, char *ip);
int reset_interface_ip(char *ifname);
int add_wireless_interface(char *ifname);
int delete_wireless_interface(char *ifname);
void bridge_init(char *br);
void detect_del_arp_entry(char *ip);

#define DEBUG_LEVEL_DISABLE             0
#define DEBUG_LEVEL_BASIC               1
#define DEBUG_LEVEL_ADVANCED            2
int get_debug_level(int value);

/* hostapd API */
char* get_hapd_exec_file();
int set_hapd_exec_file(char* path);
char* get_hapd_full_exec_path();
int set_hapd_full_exec_path(char* path);
char* get_hapd_ctrl_path_by_id(struct interface_info* wlan);
char* get_hapd_ctrl_path();
int set_hapd_ctrl_path(char* path);
char* get_hapd_global_ctrl_path();
int set_hapd_global_ctrl_path(char* path);
char* get_hapd_conf_file();
int set_hapd_conf_file(char* path);
void set_hostapd_debug_level(int level);
char* get_hostapd_debug_arguments();

/* wpa_supplicant API */
char* get_wpas_exec_file();
int set_wpas_exec_file(char* path);
char* get_wpas_full_exec_path();
int set_wpas_full_exec_path(char* path);
char* get_wpas_ctrl_path();
int set_wpas_ctrl_path(char* path);
char* get_wpas_global_ctrl_path();
int set_wpas_global_ctrl_path(char* path);
char* get_wpas_conf_file();
int set_wpas_conf_file(char* path);
void set_wpas_debug_level(int level);
char* get_wpas_debug_arguments();

/* service and environment API */
char* get_wireless_interface();
int set_wireless_interface(char *name);
int get_service_port();
int set_service_port(int port);
char* get_default_wireless_interface_info();
int clear_interfaces_resource();
char* get_all_hapd_conf_files(int *swap_hostapd);

void parse_bss_identifier(int bss_identifier, struct bss_identifier_info* bss);
struct interface_info* assign_wireless_interface_info(struct bss_identifier_info *bss);
struct interface_info* get_wireless_interface_info(int band, int identifier);
int add_all_wireless_interface_to_bridge(char *br);
void set_default_wireless_interface_info(int channel);
int show_wireless_interface_info();
void iterate_all_wlan_interfaces(void (*callback_fn)(void *));
void get_server_cert_hash(char *pem_file, char *buffer);
int insert_wpa_network_config(char *config);
void remove_pac_file(char *path);
int is_band_enabled(int band);

/* misc */
size_t strlcpy(char *dest, const char *src, size_t siz);
int get_key_value(char *value, char *buffer, char *token);
int verify_band_from_freq(int freq, int band);
int get_center_freq_index(int channel, int width);
int get_6g_center_freq_index(int channel, int width);
int is_ht40plus_chan(int chan);
int is_ht40minus_chan(int chan);
int http_file_post(char *host, int port, char *path, char *file_name);
#endif
