#ifndef __RWNX_MISC_H_
#define __RWNX_MISC_H_

#include "rwnx_version.h"
#include "rw_msdu.h"

#if NX_VERSION > NX_VERSION_PACK(6, 22, 0, 0)

#if CONFIG_NX_MEDIA_DYNC_DTIM
#define MM_DYNC_DTIM_DUR        200
#endif // CONFIG_NX_MEDIA_DYNC_DTIM

#if CONFIG_MQTT_ARP_ALIGN_EN
#define MM_MQTT_KEEPALIVE_MARGIN (1 * 1000000)   ///< 1s
#define MM_MQTT_ARP_MARGIN (2 * 1000000)   ///<2s
#endif //CONFIG_MQTT_ARP_ALIGN_EN

enum SPECIAL_FRAME_TYPE
{
    S_FRAME_EAPOL = 0,
    S_FRAME_DHCP,
    S_FRAME_ARP,
    S_FRAME_DARP,
    S_FRAME_CNT_MAX,
    S_FRAME_INVALID = 0xff,
};

#if CONFIG_MQTT_ARP_ALIGN_EN
///Mqtt structure
struct mm_mqtt_tag
{
    /// True indicates that mqtt is valid
    bool mqtt_valid;
    /// True indicates that mqtt is connected
    bool mqtt_conn;
    /// The number of mqtt ping to align arp reply
    uint8_t mqtt_num_align_arp;
    /// The count of mqtt ping after last arp reply alignment
    uint8_t mqtt_cnt_align_arp;
    /// Mqtt keepalive period (in us)
    uint32_t ping_period;
    /// mqtt next ping time (in us)
    uint32_t next_ping_time;
};
#endif // CONFIG_MQTT_ARP_ALIGN_EN

#if CONFIG_DHCP_PROTECT_IN_CONNECTED
enum SM_DHCP_FRAME_TYPE{
    SM_DHCP_FRAME_NULL        = 0,
    SM_DHCP_FRAME_DISCOVER,     // 1
    SM_DHCP_FRAME_OFFER,        // 2
    SM_DHCP_FRAME_REQUEST,      // 3
    SM_DHCP_FRAME_RELET,        // 4
    SM_DHCP_FRAME_ACK,          // 5
    SM_DHCP_FRAME_NAK,          // 6
    SM_DHCP_FRAME_RELEASE,      // 7
    SM_DHCP_FRAME_INFORM,       // 8
};

#define SM_DHCP_RELET_PERIOD 10
#endif //CONFIG_DHCP_PROTECT_IN_CONNECTED

#if CONFIG_NX_MEDIA_DYNC_DTIM
void mm_media_dtim_check(void);
#endif

#if CONFIG_NX_OTA_DYNC_DTIM
void mm_ota_dtim_open(void);
void mm_ota_dtim_close(void);
#endif
#endif // NX_VERSION

struct sm_disconnect_ind;
struct sm_connect_ind;

void rwnx_misc_disconnect_ind(struct sm_disconnect_ind *ind);
void rwnx_misc_connect_ind(struct sm_connect_ind *ind);
void rwnx_dhcp_done_handler(uint8_t vif_idx);
void rwnx_connecting_handler(void);

/**
 * preprocess pbuf before pass it to TCP/IP protocol stack
 */
void rwnx_rx_preprocess(uint8_t vif_idx, struct pbuf *p);
/**
 * postprocess pbuf after TCP/IP protocol stack passes it to umac
 */
void rwnx_tx_postprocess(uint8_t vif_idx, struct sk_buff *skb);

#endif // __RWNX_MISC_H_
