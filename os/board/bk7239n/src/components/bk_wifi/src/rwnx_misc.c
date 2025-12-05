// move some tcp/ip packets handlers from umac's sm to here.
// Please refactor me:
// move some application layer protocol handlers to their right place, such mqtt, etc.

#include <common/bk_include.h>
#include <os/str.h>
#include <os/mem.h>
#include "bk_list.h"
#include "rw_msdu.h"
#include "bk_rw.h"
#include "rwnx_version.h"

#if NX_VERSION > NX_VERSION_PACK(6, 22, 0, 0)
#include "rwnx_defs.h"
#include "sm_task.h"
#include "rwnx_tx.h"
#include "rwnx_misc.h"

#include "ps.h"


enum dhcp_state_e {
	DHCP_STATE_NULL = 0,
	DHCP_STATE_CONNECTING,
	DHCP_STATE_DONE,
	DHCP_STATE_RELET_START,
	DHCP_STATE_CONNECTED,
};

struct bootp_option {
	uint8_t type;
	uint8_t length;
	char	value[0];
} __attribute__((packed));

///////////////////////    DHCP_STATE  /////////////////////////////////
static uint8_t dhcp_state;
#if CONFIG_DHCP_PROTECT_IN_CONNECTED
/// DHCP_STATE life
static uint8_t dhcp_relet_life;
#endif
/// for getting ipv6 dhcp, after this timeout, dtim can use longer value
#define SM_DHCP_PROTECT_TIMEOUT      15000000 /// 15s

///////////////////////     MQTT      /////////////////////////////////
#if CONFIG_MQTT_ARP_ALIGN_EN
/// MQTT state and period
static struct mm_mqtt_tag mqtt_info;
#endif // CONFIG_MQTT_ARP_ALIGN_EN

/////////////////////// Multimedia    /////////////////////////////////
#if CONFIG_NX_MEDIA_DYNC_DTIM
static bool gpio_media_is_on;
static uint8_t gpio_media_dtim_cnt;
#endif

///////////////////////    OTA        /////////////////////////////////
#if CONFIG_NX_OTA_DYNC_DTIM
static bool ota_is_on;
#endif

///////////////////////     ARP       /////////////////////////////////
/// when set listen interval,last send arp time
static uint32_t last_arp_time;
static bool notify_arp_ind;

///////////////////////     TCP      //////////////////////////////////
/// reduce dtim interval if have TCP packets
#if CONFIG_TCP_PROTECT_IN_CONNECTED
static uint8_t tcp_protect_cnt;
static bool tcp_send_flag;
#endif

/////////////////////// Forward declaration ///////////////////////////
static bool txu_is_special_packet(uint8_t vif_idx, struct sk_buff *skb);
static bool sm_dhcp_check(uint8_t vif_index, void *frameptr, bool tx);
static void sm_connected_dhcp_protect_check(uint8_t vif_index);
static void mm_check_tcp_send(void);
static void mm_send_arp_ind(struct vif_info_tag *vif, bool is_send_arp);
static void mm_check_need_send_arp(struct vif_info_tag *vif, uint8_t dtim_period, bool is_mqtt);
static void mm_check_need_send_mqttping(uint32_t time_margin);
static int mm_set_mqtt_keepalive_info_handler(ke_msg_id_t const msgid,
	struct mm_mqtt_keepalive_info_ind *param,
    ke_task_id_t const dest_id,
    ke_task_id_t const src_id);
static void sta_dhcp_tx_cb(void *env, uint32_t status);

/**
 * preprocess pbuf before pass it to TCP/IP protocol stack
 */
void rwnx_rx_preprocess(uint8_t vif_idx, struct pbuf *p)
{
#if CONFIG_DHCP_PROTECT_IN_CONNECTED
	struct ip_hdr *ipv4hdr;
	struct eth_hdr *ethhdr;
	struct vif_info_tag *vif = &vif_info_tab[vif_idx];
	uint8_t *buf = p->payload;
	uint16_t ip_total_len;

	if (unlikely(vif->type != VIF_STA))
		return;

	// Forward to Ethernet header
	ethhdr = (struct eth_hdr *)p->payload;

	// If frame is not a IP frame
	if (ntohs(ethhdr->type) != ETHTYPE_IP)
		return;

	ipv4hdr = (struct ip_hdr *)(ethhdr + 1);

	ip_total_len = IPH_HL(ipv4hdr) * 4;
	buf += ip_total_len;

	// Check DHCP packets
	if (IPH_PROTO(ipv4hdr) == IP_PROTO_UDP) {
		struct udp_hdr *udphdr = (struct udp_hdr *)buf;
		if (lwip_htons(udphdr->dest) == 68) {
			/* DHCP frame */
			sm_dhcp_check(vif_idx, udphdr + 1, false);  // FIXME: bk7236 add length argment
		}
	}
#endif
}

void rwnx_tx_postprocess(uint8_t vif_idx, struct sk_buff *skb)
{
	// 1. txu_is_special_packet
	txu_is_special_packet(vif_idx, skb);
}

// receive beacon timeout event, called by vif_mgmt_bcn_to_evt
void on_bcn_to_evt(struct vif_info_tag *vif)
{
#if CONFIG_SPECIAL_TX_TYPE
	// mm_keep_alive_handler(vif, mm_env.record_dtim_period);
	if (vif->u.sta.listen_int_dyna > mm_env.record_dtim_period) {
#if CONFIG_MQTT_ARP_ALIGN_EN
		mm_check_need_send_mqttping(MM_MQTT_ARP_MARGIN);
#endif
		// vif->u.sta.mon_last_tx = hal_machw_time();

		// notify host send arp and update last_arp_time
		mm_send_arp_ind(vif, true);

		// RWNX_LOGI("mm_keep_alive_handler\n");
		// mm_keep_alive_null_cfm(vif,FRAME_SUCCESSFUL_TX_BIT);
	}
#endif

#if NX_HW_PARSER_TIM_ELEMENT
	// check whether ARP needs to be send
	mm_check_need_send_arp(vif, mm_env.record_dtim_period, false);
#endif

#if CONFIG_MQTT_ARP_ALIGN_EN
	// check whether needs to send mqtt ping request
	mm_check_need_send_mqttping(MM_MQTT_KEEPALIVE_MARGIN);
#endif // CONFIG_MQTT_ARP_ALIGN_EN
}

// called by mm_sta_tbtt
void on_mm_sta_tbtt(struct vif_info_tag *vif)
{
#if CONFIG_DHCP_PROTECT_IN_CONNECTED
	sm_connected_dhcp_protect_check(vif->index);
#endif

#if CONFIG_TCP_PROTECT_IN_CONNECTED
	mm_check_tcp_send();
#endif

#if CONFIG_NX_MEDIA_DYNC_DTIM
	// when multimedia is enable,we need Xs duration to receive key frames
	// dtim will be 1 when multimedia,so X*10 Beacon can measure the duration
	if (gpio_media_is_on) {
		gpio_media_dtim_cnt++;
		if (gpio_media_dtim_cnt > MM_DYNC_DTIM_DUR) {
			gpio_media_is_on = false;
			gpio_media_dtim_cnt = 0;
		}
	}
#endif // CONFIG_NX_MEDIA_DYNC_DTIM
}

// called by mm_check_beacon
void on_beacon_receive(struct vif_info_tag *vif, void *frame, uint16_t len, uint32_t tim)
{
	uint8_t dtim_period = co_read8p(tim + MAC_TIM_PERIOD_OFT);

#if CONFIG_SPECIAL_TX_TYPE
	// mm_keep_alive_handler(vif,dtim_period);
	if (vif->u.sta.listen_int_dyna > dtim_period) {
#if CONFIG_MQTT_ARP_ALIGN_EN
		mm_check_need_send_mqttping(MM_MQTT_ARP_MARGIN);
#endif
		// vif->u.sta.mon_last_tx = hal_machw_time();
		///notify host send arp and update last_arp_time
		mm_send_arp_ind(vif, true);
		//RWNX_LOGI("mm_keep_alive_handler\n");
		// mm_keep_alive_null_cfm(vif,FRAME_SUCCESSFUL_TX_BIT);
	}
#endif

	///check whether ARP needs to be send
	mm_check_need_send_arp(vif, dtim_period, false);
#if CONFIG_MQTT_ARP_ALIGN_EN
	///check whether needs to send mqtt ping request
	mm_check_need_send_mqttping(MM_MQTT_KEEPALIVE_MARGIN);
#endif //CONFIG_MQTT_ARP_ALIGN_EN
}

// called by mm_tim_bcn_evt
void on_hw_tim_event(struct vif_info_tag *vif)
{
#if CONFIG_MQTT_ARP_ALIGN_EN
	///check whether needs to send mqtt ping request
	mm_check_need_send_mqttping(MM_MQTT_KEEPALIVE_MARGIN);
#endif
}

void rwnx_misc_connect_ind(struct sm_connect_ind *ind)
{
#if CONFIG_NX_MEDIA_DYNC_DTIM
	gpio_media_is_on = false;
	gpio_media_dtim_cnt = 0;
#endif
#if !CONFIG_DHCP_PROTECT_IN_CONNECTED
	last_arp_time = hal_machw_time();
#endif
}

void rwnx_misc_disconnect_ind(struct sm_disconnect_ind *ind)
{
	dhcp_state = DHCP_STATE_NULL;
}

static bool txu_is_special_packet(uint8_t vif_idx, struct sk_buff *skb)
{
	struct ip_hdr *ipv4hdr;
	struct eth_hdr *ethhdr;
	bool ret = false;
	struct vif_info_tag *vif = &vif_info_tab[vif_idx];
	struct pbuf *p = skb->p;
	uint8_t *buf = p->payload;
	uint16_t ip_total_len;
	struct txdesc *txdesc;
	bool use_lowest_rate = false;

	if (unlikely(vif->type != VIF_STA))
		return false;

	txdesc = &skb->ftxdesc->txdesc;
#if CONFIG_MAC_SFRAME_SOFTWARE_RETRY
	if (skb->sframe_type == S_FRAME_DHCP)
		return true;
#endif

	// Forward to Ethernet header
	ethhdr = (struct eth_hdr *)p->payload;

	if (ntohs(ethhdr->type) == ETHTYPE_EAPOL ||
		ntohs(ethhdr->type) == ETHTYPE_ARP) {
		use_lowest_rate = true;
		goto out;
	}

	// If frame is not a IP frame
	if (ntohs(ethhdr->type) != ETHTYPE_IP)
		return false;

	buf += sizeof(struct eth_hdr);
	ipv4hdr = (struct ip_hdr *)(ethhdr + 1);

	ip_total_len = IPH_HL(ipv4hdr) * 4;
	buf += ip_total_len;

	if (IPH_PROTO(ipv4hdr) == IP_PROTO_UDP) {
		/* UDP frame */
		struct udp_hdr *udphdr = (struct udp_hdr *)buf;

		if (lwip_htons(udphdr->src) == 68) {
			/* DHCP frame */
			skb->sframe_type = S_FRAME_DHCP;
			use_lowest_rate = true;

#if CONFIG_DHCP_PROTECT_IN_CONNECTED
			if (skb->cb)
				RWNX_LOGD("Overwritten skb cb/args: %p/%p\n", skb->cb, skb->args);
			skb->cb = sta_dhcp_tx_cb;
			skb->args = skb;
			sm_dhcp_check(txdesc->host.vif_idx, udphdr + 1, true);  // FIXME: bk7236 add length argment
#endif
			ret = true;
		}
#if CONFIG_TCP_PROTECT_IN_CONNECTED
	} else if (IPH_PROTO(ipv4hdr) == IP_PROTO_TCP) {
		// If TCP packet has data, set flag
		struct tcp_hdr *tcphdr = (struct tcp_hdr *)buf;
		uint16_t tcp_hdr_len;
		uint16_t tcp_data_len;

		tcp_hdr_len = TCPH_HDRLEN(tcphdr);
		tcp_data_len = IPH_LEN(ipv4hdr) - tcp_hdr_len - ip_total_len;

		/* TCP ACK without Data */
		if (TCPH_FLAGS(tcphdr) == TCP_ACK && tcp_data_len == 0) {
			// RWNX_LOGI("tcp ack!\n");
			tcp_protect_cnt = 0;
			tcp_send_flag = 0;
		} else {
			// RWNX_LOGI("tcp flag: 0x%x, data len=%d.\n", TCPH_FLAGS(tcphdr), tcp_data_len);
			tcp_send_flag = 1;
			ret = true;
		}
#endif
	}

out:
	if (use_lowest_rate)
		txdesc->host.flags |= TXU_CNTRL_LOWEST_RATE;

	return ret;
}

#if CONFIG_DHCP_PROTECT_IN_CONNECTED
/**
 ****************************************************************************************
 * @brief check tx/rx dhcp frame
 *
 * @param[in] frameptr frame payload point
 * @param[in] mac_head_len rx mac header length
 * @param[in] check_type   rx/tx
 * @param[out] null
 ****************************************************************************************
 */
static bool sm_dhcp_check(uint8_t vif_index, void *frameptr, bool tx)
{
	bool check_result = false;
	struct vif_info_tag *vif;
	struct dhcp_msg *dhcphdr = frameptr;
	struct bootp_option *opt;

	if (vif_index >= NX_VIRT_DEV_MAX)
		return false;

	// if not STA, return.
	vif = &vif_info_tab[vif_index];
	if (vif->type != VIF_STA)
		return false;

	// Check the following case:
	//   1. DHCP completed and check TX, or
	//   2. DHCP Request successfuly sent, and check RX
	if (!((dhcp_state == DHCP_STATE_DONE && tx) ||
		  (dhcp_state == DHCP_STATE_CONNECTED && !tx))) {
		//RWNX_LOGI("sm_dhcp_check: state %d, %s\n", dhcp_state, tx ? "TX" : "RX");
		return check_result;
	}

	// redudant ??
	if (tx && dhcp_state < DHCP_STATE_DONE)
		return false;

	if (dhcphdr->op != DHCP_BOOTREQUEST && dhcphdr->op != DHCP_BOOTREPLY)
		//RWNX_LOGI("bootp Operation is not right.\n");
		return check_result;

	opt = (struct bootp_option *)dhcphdr->options;

	/* FIXME: bk7236, dhcp server may not put dhcp message(53) first */
	if (opt->type != 53)
		return false;

	check_result = true;
	uint8_t dhcp_tpye = opt->value[0];

	switch (dhcp_tpye) {
	case SM_DHCP_FRAME_DISCOVER:
		//RWNX_LOGI("sm %s DHCP_DISCOVER\n", tx ? "tx" : "rx");
		break;
	case SM_DHCP_FRAME_OFFER:
		//RWNX_LOGI("sm %s DHCP_OFFER\n", tx ? "tx" : "rx");
		break;
	case SM_DHCP_FRAME_REQUEST:
		//RWNX_LOGI("sm %s DHCP_REQUEST\n", tx ? "tx" : "rx");
		/// dhcp relet will send request first
		if (tx)
			//RWNX_LOGI("DHCP RELET\n");
			dhcp_state = DHCP_STATE_RELET_START;
		break;
	case SM_DHCP_FRAME_RELET:
		//RWNX_LOGI("sm %s DHCP_DECLINE\n", tx ? "tx" : "rx");
		break;
	case SM_DHCP_FRAME_ACK:
		//RWNX_LOGI("sm %s DHCP_ACK\n", tx ? "tx" : "rx");
		if (!tx) {
			RWNX_LOGI("DHCP RELET END\n");
			dhcp_state = DHCP_STATE_DONE;
			dhcp_relet_life = 0;
		}
		break;
	case SM_DHCP_FRAME_NAK:
		//RWNX_LOGI("sm %s DHCP_NAK\n", tx ? "tx" : "rx");
		break;
	case SM_DHCP_FRAME_RELEASE:
		//RWNX_LOGI("sm %s DHCP_RELEASE\n", tx ? "tx" : "rx");
		break;
	case SM_DHCP_FRAME_INFORM:
		//RWNX_LOGI("sm %s DHCP_INFORM\n", tx ? "tx" : "rx");
		break;
	default:
		//RWNX_LOGI("sm %s DHCP NULL type\n", tx ? "tx" : "rx");
		check_result = false;
		break;
	}

	return check_result;
}

/**
 ****************************************************************************************
 * @brief check if dhcp sended
 *
 * @param[in] vif
 * @param[in] type
 * @param[out] void
 ****************************************************************************************
 */
void sm_is_dhcp_relet_send(uint8_t vif_index, uint16_t type)
{
	if (type == S_FRAME_DHCP) {
		if (vif_index >= NX_VIRT_DEV_MAX) {
			RWNX_LOGI("DHCP vif err\n");
			return;
		}

		if (dhcp_state == DHCP_STATE_RELET_START) {
			RWNX_LOGI("DHCP RELET START\n");
			dhcp_state = DHCP_STATE_CONNECTED;
		}
	}
}

/**
 ****************************************************************************************
 * @brief indicate all dtim are needed received or not
 *
 * @param[in] void
 * @param[out] indicate all dtim are needed received or not
 ****************************************************************************************
 */
bool sm_is_connected_dhcp(uint8_t vif_index)
{
	return dhcp_state == DHCP_STATE_CONNECTED;
}

/**
 ****************************************************************************************
 * @brief check protect dhcp relet time
 *
 * @param[in] vif_index
 * @param[out] void
 ****************************************************************************************
 */
static void sm_connected_dhcp_protect_check(uint8_t vif_index)
{
	if (dhcp_state == DHCP_STATE_CONNECTED) {
		dhcp_relet_life++;
		if (dhcp_relet_life >= SM_DHCP_RELET_PERIOD) {
			dhcp_state = DHCP_STATE_DONE;
			dhcp_relet_life = 0;
		}
		RWNX_LOGI("dhcp relet:%d\n", dhcp_relet_life);
	}
}
#endif //CONFIG_DHCP_PROTECT_IN_CONNECTED

/**
 ****************************************************************************************
 * @brief sm_check_dhcp_is_done
 *
 * @param[in] vif_index
 * @param[out] true or false
 ****************************************************************************************
 */
bool sm_check_is_dhcp_done(uint8_t vif_index)
{
	bool is_dhcp_done = false;

	if (dhcp_state >= DHCP_STATE_DONE)
		is_dhcp_done = true;

	return is_dhcp_done;
}

bool sm_check_is_dhcpv6_done(uint8_t vif_index)
{
	/* after STA connected to AP, wait 15s to set long listen interval, else set minimal  */
	return true;
}


#if CONFIG_TCP_PROTECT_IN_CONNECTED
static void mm_check_tcp_send(void)
{
	if (tcp_send_flag) {
		if (tcp_protect_cnt < MM_TCP_PROTECT_PERIOD) {
			tcp_protect_cnt++;
			// RWNX_LOGI("dtim1 for protect tcp %d.\n", tcp_protect_cnt);
		} else {
			tcp_protect_cnt = 0;
			tcp_send_flag = 0;
		}
	}
}
#endif

// reset listen interval to dtim counter, or listen interval when assoc, or dtim period if
// dtim counter equals to zero.
bool mm_is_reset_listen_interval(uint8_t vif_idx)
{
	if (vif_idx >= NX_VIRT_DEV_MAX) {
		RWNX_LOGW("check resert vif null\n");
		return false;
	}

	// check dhcp6
	if (!sm_check_is_dhcpv6_done(vif_idx))
		return true;

	// check dhcp4
#if CONFIG_DHCP_PROTECT_IN_CONNECTED
	if (sm_is_connected_dhcp(vif_idx))
		return true;
#endif

	// check tcp:
#if CONFIG_TCP_PROTECT_IN_CONNECTED
	if (tcp_send_flag)
		return true;
#endif

	// check media
#if CONFIG_NX_MEDIA_DYNC_DTIM
	if (gpio_media_is_on)
		return true;
#endif

	// check ota
#if CONFIG_NX_OTA_DYNC_DTIM
	if (ota_is_on)
		return true;
#endif

	return false;
}

/**
 ****************************************************************************************
 * @brief mm_send_arp_ind
 * @param[in] vif, is_send_arp
 * @param[out] null
 ****************************************************************************************
 */
static void mm_send_arp_ind(struct vif_info_tag *vif, bool is_send_arp)
{
	struct mm_indicate_send_arp_ind *ind = NULL;

	last_arp_time = hal_machw_time();

	///notify host send arp
	ind = KE_MSG_ALLOC(MM_INDICATE_SEND_ARP_IND, TASK_API, TASK_MM, mm_indicate_send_arp_ind);

	if (ind) {
		ind->is_send_arp = is_send_arp;
		ind->is_allow_send_req = (!is_send_arp);
		ke_msg_send(ind);
	}
	BK_CLOG_MM("%s: is_send_arp %d, is_allow_send_req %d, arp_time %d\n", __func__,
			   is_send_arp, ind->is_allow_send_req, last_arp_time);
}

/**
 ****************************************************************************************
 * @brief mm_check_need_send_arp
 * @param[in] vif,receive_bcn_dtim
 * @param[out] null
 ****************************************************************************************
 */
static void mm_check_need_send_arp(struct vif_info_tag *vif, uint8_t dtim_period, bool is_mqtt)
{
	if (!sm_check_is_dhcp_done(vif->index))
		return;

	if (vif->u.sta.listen_int_dyna > dtim_period) {
		notify_arp_ind = false;

		if (hal_machw_time_past(last_arp_time + MM_SEND_ARP_PERIOD) || (is_mqtt)) {
#if CONFIG_MQTT_ARP_ALIGN_EN
			if (!is_mqtt)
				mm_check_need_send_mqttping(MM_MQTT_ARP_MARGIN);

#endif //CONFIG_MQTT_ARP_ALIGN_EN

			mm_send_arp_ind(vif, true);
			// mm_keep_alive_null_cfm(vif, FRAME_SUCCESSFUL_TX_BIT);
		}
	} else {
		last_arp_time = hal_machw_time();
		if (!notify_arp_ind && (vif->u.sta.listen_int_dyna <= dtim_period)) {
			notify_arp_ind = true;

			///notify host allow send arp req
			mm_send_arp_ind(vif, false);
		}
	}
}

#if CONFIG_MQTT_ARP_ALIGN_EN
/**
 ****************************************************************************************
 * @brief Check if need send mqtt ping request tx
 *
 * @param[in] time_margin Margin of mqtt ping before timeout
 ****************************************************************************************
 */
static void mm_check_need_send_mqttping(uint32_t time_margin)
{
	if ((mqtt_info.mqtt_valid) &&
		(hal_machw_time_cmp(mqtt_info.next_ping_time, (hal_machw_time() + time_margin)))) {
		mqtt_info.next_ping_time = hal_machw_time() + mqtt_info.ping_period;
		///notify host trigger mqtt sending mqtt ping request
		ke_msg_send_basic(MM_SEND_MQTT_PINGREQ_IND, TASK_API, TASK_MM);

		BK_CLOG_MM("mm_check_need_send_mqttping\n");
	}
}
#endif //CONFIG_MQTT_ARP_ALIGN_EN

#if CONFIG_MQTT_ARP_ALIGN_EN
/**
 ****************************************************************************************
 * @brief MM module @ref MM_MQTT_KEEPALIVE_PERIOD_IND message handler.
 *
 * Sent by Host.\n It provides mqtt information.
 *
 * @param[in] msgid Id of the message received.
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id TaskId of the receiving task.
 * @param[in] src_id TaskId of the sending task.
 * @return Whether the message was consumed or not.
 ****************************************************************************************
 */
static int mm_set_mqtt_keepalive_info_handler(ke_msg_id_t const	msgid,
		struct mm_mqtt_keepalive_info_ind *param,
		ke_task_id_t const dest_id,
		ke_task_id_t const src_id)
{
	bool mqtt_conn_pre = mqtt_info.mqtt_conn;
	struct vif_info_tag *vif;

	mqtt_info.ping_period = param->period_s * 1000000;
	mqtt_info.mqtt_conn = param->mqtt_conn;
	mqtt_info.mqtt_valid = false;

	if (!mqtt_info.mqtt_conn) {
		mqtt_info.mqtt_num_align_arp = 0;
		mqtt_info.mqtt_cnt_align_arp = 0;
	}

	for_each_vif_entry(vif) {
		if ((VIF_STA == vif->type) && (vif->u.sta.listen_int_dyna > mm_env.record_dtim_period)) {
			mqtt_info.mqtt_valid = param->mqtt_conn;
			mqtt_info.next_ping_time = hal_machw_time() + mqtt_info.ping_period;
			bool first_mqtt_ping = ((!mqtt_conn_pre) && (param->mqtt_conn) && (param->send_pingreq)) ? true : false;
			bool force_send_arp = false;
			if (first_mqtt_ping) {
				mqtt_info.mqtt_num_align_arp = 0;
				mqtt_info.mqtt_cnt_align_arp = 0;

				uint32_t arp_period_s = MM_SEND_ARP_PERIOD / 1000000;
				if ((0 == param->period_s % arp_period_s) || (0 == arp_period_s % param->period_s)) {
					mqtt_info.mqtt_num_align_arp = (param->period_s > arp_period_s) ? 1 : (arp_period_s / param->period_s);
					force_send_arp = true;
				}
			} else if ((param->mqtt_conn) && (param->send_pingreq) && (mqtt_info.mqtt_num_align_arp > 0)) {
				mqtt_info.mqtt_cnt_align_arp++;
				if (mqtt_info.mqtt_num_align_arp == mqtt_info.mqtt_cnt_align_arp) {
					force_send_arp = true;
					mqtt_info.mqtt_cnt_align_arp = 0;
				}
			}

			if (force_send_arp)
				mm_check_need_send_arp(vif, mm_env.record_dtim_period, true);

			break;
		}
	}

	return KE_MSG_CONSUMED;
}
#endif //CONFIG_MQTT_ARP_ALIGN_EN

#if CONFIG_DHCP_PROTECT_IN_CONNECTED
// called when DHCP frame sent by STA is confirmed
static void sta_dhcp_tx_cb(void *env, uint32_t status)
{
	struct sk_buff *skb = env;
	struct txdesc *txdesc = &skb->ftxdesc->txdesc;

	if (status & TX_STATUS_ACKNOWLEDGED)
		sm_is_dhcp_relet_send(txdesc->host.vif_idx, skb->sframe_type);
}
#endif

#if CONFIG_NX_MEDIA_DYNC_DTIM
void mm_media_dtim_check()
{
	struct vif_info_tag *vif;

	if (!ps_env.ps_on)
		return;

	vif = (struct vif_info_tag *)co_list_pick(&vif_mgmt_env.used_list);
	while (vif != NULL) {
		if ((vif->type == VIF_STA) && (vif->active) && (gpio_media_is_on == false))
			gpio_media_is_on = true;

		vif = (struct vif_info_tag *)co_list_next(&vif->list_hdr);
	}
}
#endif

#if CONFIG_NX_OTA_DYNC_DTIM
void mm_ota_dtim_open()
{
	struct vif_info_tag *vif;

	if (!ps_env.ps_on)
		return;

	vif = (struct vif_info_tag *)co_list_pick(&vif_mgmt_env.used_list);
	while (vif != NULL) {
		if ((vif->type == VIF_STA) && (vif->active) && (ota_is_on == false))
			ota_is_on = true;

		vif = (struct vif_info_tag *)co_list_next(&vif->list_hdr);
	}
}

void mm_ota_dtim_close()
{
	struct vif_info_tag *vif;

	if (!ps_env.ps_on)
		return;

	vif = (struct vif_info_tag *)co_list_pick(&vif_mgmt_env.used_list);
	while (vif != NULL) {
		if ((vif->type == VIF_STA) && (vif->active) && (ota_is_on == true))
			ota_is_on = false;

		vif = (struct vif_info_tag *)co_list_next(&vif->list_hdr);
	}
}
#endif

/**
 ****************************************************************************************
 * @brief sm_dhcp_done_handler
 *
 * @param[in] vif index
 * @param[out] void
 ****************************************************************************************
 */
void rwnx_dhcp_done_handler(uint8_t vif_idx)
{
	ASSERT_ERR(vif_idx < NX_VIRT_DEV_MAX);

	dhcp_state = DHCP_STATE_DONE;
#if CONFIG_DHCP_PROTECT_IN_CONNECTED
	dhcp_relet_life = 0;
	// dhcp done, set last_arp_time
	last_arp_time = hal_machw_time();
#endif
}

void rwnx_connecting_handler()
{
	dhcp_state = DHCP_STATE_CONNECTING;
}

#endif

