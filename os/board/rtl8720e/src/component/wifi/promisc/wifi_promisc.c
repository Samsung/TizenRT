#if !defined(CONFIG_MBED_ENABLED) && !defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#include "main.h"
#if CONFIG_LWIP_LAYER
#include "tcpip.h"
#endif
#endif
#include <osdep_service.h>
#include "wifi_conf.h"
#ifndef CONFIG_WLAN
#define CONFIG_WLAN 1
#endif

#if CONFIG_WLAN
#include <platform_stdlib.h>

#ifndef CONFIG_UNSUPPORT_PLCPHDR_RPT
#define CONFIG_UNSUPPORT_PLCPHDR_RPT 0
#endif

extern void _promisc_deinit(void *padapter);
extern int _promisc_recv_func(void *padapter, void *rframe);
extern int _promisc_set(rtw_rcr_level_t enabled, void (*callback)(unsigned char *, unsigned int, void *), unsigned char len_used);
extern unsigned char _is_promisc_enabled(void);
extern int _promisc_get_fixed_channel(void *fixed_bssid, u8 *ssid, int *ssid_length);
extern void _promisc_filter_by_ap_and_phone_mac(u8 enable, void *ap_mac, void *phone_mac);
extern int _promisc_recv_lens_func(void *padapter, u8 *payload, u8 plen);
extern int _promisc_filter_with_len(u16 len);
extern int _promisc_filter_retransmit_plcp_pkt(u8 enable, u8 filter_interval_ms);
extern int _promisc_set_mgntframe(u8 enable);
extern int _promisc_get_chnl_by_bssid(u8 *bssid);
extern void _promisc_update_candi_ap_rssi_avg(s8 rssi, u8 cnt);
extern void _promisc_stop_tx_beacn(void);
extern void _promisc_resume_tx_beacn(void);
extern void _promisc_issue_probersp(unsigned char *da);
// Add extra interfaces to make release sdk able to determine promisc API linking
void promisc_deinit(void *padapter)
{
#ifdef CONFIG_PROMISC
	_promisc_deinit(padapter);
#endif
}

int promisc_recv_func(void *padapter, void *rframe)
{
	// Never reach here if not define CONFIG_PROMISC
#ifdef CONFIG_PROMISC
	return _promisc_recv_func(padapter, rframe);
#else
	return 0;
#endif
}

int promisc_recv_lens_func(void *padapter, u8 *payload, u8 plen)
{
	/* To avoid gcc warnings */
	(void) padapter;
	(void) payload;
	(void) plen;

	// Never reach here if not define CONFIG_PROMISC
#ifdef CONFIG_PROMISC
#if defined(CONFIG_UNSUPPORT_PLCPHDR_RPT) && CONFIG_UNSUPPORT_PLCPHDR_RPT
	return _promisc_recv_lens_func(padapter, payload, plen);
#else
	return 0;
#endif
#else
	return 0;
#endif
}

int promisc_filter_retransmit_pkt(unsigned char enable, unsigned char filter_interval_ms)
{
#ifdef CONFIG_PROMISC
#if CONFIG_UNSUPPORT_PLCPHDR_RPT
	return _promisc_filter_retransmit_plcp_pkt((u8)enable, (u8)filter_interval_ms);
#else
	(void) enable;
	(void) filter_interval_ms;
	return -1;//_promisc_filter_retransmit_normal_pkt(enable);
#endif
#else
	(void) enable;
	(void) filter_interval_ms;
	return -1;
#endif
}

int promisc_filter_with_len(u16 len)
{
	// Never reach here if not define CONFIG_PROMISC
#ifdef CONFIG_PROMISC
#if defined(CONFIG_UNSUPPORT_PLCPHDR_RPT) && CONFIG_UNSUPPORT_PLCPHDR_RPT
	return _promisc_filter_with_len(len);
#else
	(void) len;
	return -1;
#endif
#else
	(void) len;
	return -1;
#endif
}

int promisc_set(rtw_rcr_level_t enabled, void (*callback)(unsigned char *, unsigned int, void *), unsigned char len_used)
{
#ifdef CONFIG_PROMISC
	return _promisc_set(enabled, callback, len_used);
#else
	return -1;
#endif
}

unsigned char is_promisc_enabled(void)
{
#ifdef CONFIG_PROMISC
	return _is_promisc_enabled();
#else
	return 0;
#endif
}

int promisc_get_fixed_channel(void *fixed_bssid, unsigned char *ssid, int *ssid_length)
{
#ifdef CONFIG_PROMISC
	return _promisc_get_fixed_channel(fixed_bssid, (u8 *)ssid, ssid_length);
#else
	return 0;
#endif
}

void promisc_filter_by_ap_and_phone_mac(unsigned char enable, void *ap_mac, void *phone_mac)
{
#ifdef CONFIG_PROMISC
	_promisc_filter_by_ap_and_phone_mac((u8)enable, ap_mac, phone_mac);
#endif
}

int promisc_set_mgntframe(unsigned char enable)
{
#ifdef CONFIG_PROMISC
	_promisc_set_mgntframe((u8)enable);
#endif
	return -1;
}

int promisc_get_chnl_by_bssid(unsigned char *bssid)
{
#ifdef CONFIG_PROMISC
	_promisc_get_chnl_by_bssid((u8 *)bssid);
#endif
	return 0;
}

void promisc_update_candi_ap_rssi_avg(signed char rssi, unsigned char cnt)
{
#ifdef CONFIG_PROMISC
	_promisc_update_candi_ap_rssi_avg((s8)rssi, (u8)cnt);
#endif
}

void promisc_stop_tx_beacn(void)
{
#ifdef CONFIG_PROMISC
	_promisc_stop_tx_beacn();
#endif
}

void promisc_resume_tx_beacn(void)
{
#ifdef CONFIG_PROMISC
	_promisc_resume_tx_beacn();
#endif
}

void promisc_issue_probersp(unsigned char *da)
{
#ifdef CONFIG_PROMISC
	_promisc_issue_probersp(da);
#endif
}

// End of Add extra interfaces

struct eth_frame {
	struct eth_frame *prev;
	struct eth_frame *next;
	unsigned char da[6];
	unsigned char sa[6];
	unsigned int len;
	unsigned char type;
	signed char rssi;
};

struct eth_buffer {
	struct eth_frame *head;
	struct eth_frame *tail;
};

static struct eth_buffer eth_buffer;

#ifdef CONFIG_PROMISC
#define MAX_PACKET_FILTER_INFO 5
#define FILTER_ID_INIT_VALUE 10
rtw_packet_filter_info_t paff_array[MAX_PACKET_FILTER_INFO] = {0};
static u8 packet_filter_enable_num = 0;

void promisc_init_packet_filter(void)
{
	int i = 0;
	for (i = 0; i < MAX_PACKET_FILTER_INFO; i++) {
		paff_array[i].filter_id = FILTER_ID_INIT_VALUE;
		paff_array[i].enable = 0;
		paff_array[i].patt.mask_size = 0;
		paff_array[i].rule = RTW_POSITIVE_MATCHING;
		paff_array[i].patt.mask = NULL;
		paff_array[i].patt.pattern = NULL;
	}
	packet_filter_enable_num = 0;
}

int promisc_add_packet_filter(u8 filter_id, rtw_packet_filter_pattern_t *patt, rtw_packet_filter_rule_t rule)
{
	int i = 0;
	while (i < MAX_PACKET_FILTER_INFO) {
		if (paff_array[i].filter_id == FILTER_ID_INIT_VALUE) {
			break;
		}
		i++;
	}

	if (i == MAX_PACKET_FILTER_INFO) {
		return -1;
	}

	paff_array[i].filter_id = filter_id;

	paff_array[i].patt.offset = patt->offset;
	paff_array[i].patt.mask_size = patt->mask_size;
	paff_array[i].patt.mask = rtw_malloc(patt->mask_size);
	memcpy(paff_array[i].patt.mask, patt->mask, patt->mask_size);
	paff_array[i].patt.pattern = rtw_malloc(patt->mask_size);
	memcpy(paff_array[i].patt.pattern, patt->pattern, patt->mask_size);

	paff_array[i].rule = rule;
	paff_array[i].enable = 0;

	return 0;
}

int promisc_enable_packet_filter(u8 filter_id)
{
	int i = 0;
	while (i < MAX_PACKET_FILTER_INFO) {
		if (paff_array[i].filter_id == filter_id) {
			break;
		}
		i++;
	}

	if (i == MAX_PACKET_FILTER_INFO) {
		return -1;
	}

	paff_array[i].enable = 1;
	packet_filter_enable_num++;
	return 0;
}

int promisc_disable_packet_filter(u8 filter_id)
{
	int i = 0;
	while (i < MAX_PACKET_FILTER_INFO) {
		if (paff_array[i].filter_id == filter_id) {
			break;
		}
		i++;
	}

	if (i == MAX_PACKET_FILTER_INFO) {
		return -1;
	}

	paff_array[i].enable = 0;
	packet_filter_enable_num--;
	return 0;
}

int promisc_remove_packet_filter(u8 filter_id)
{
	int i = 0;
	while (i < MAX_PACKET_FILTER_INFO) {
		if (paff_array[i].filter_id == filter_id) {
			break;
		}
		i++;
	}

	if (i == MAX_PACKET_FILTER_INFO) {
		return -1;
	}

	paff_array[i].filter_id = FILTER_ID_INIT_VALUE;
	paff_array[i].enable = 0;
	paff_array[i].patt.mask_size = 0;
	paff_array[i].rule = 0;
	if (paff_array[i].patt.mask) {
		rtw_free(paff_array[i].patt.mask);
		paff_array[i].patt.mask = NULL;
	}

	if (paff_array[i].patt.pattern) {
		rtw_free(paff_array[i].patt.pattern);
		paff_array[i].patt.pattern = NULL;
	}
	return 0;
}
#endif

/*	Make callback simple to prevent latency to wlan rx when promiscuous mode */
static void promisc_callback(unsigned char *buf, unsigned int len, void *userdata)
{
	struct eth_frame *frame = (struct eth_frame *) rtw_malloc(sizeof(struct eth_frame));
	_lock lock;
	_irqL irqL;

	if (frame) {
		frame->prev = NULL;
		frame->next = NULL;
		memcpy(frame->da, buf, 6);
		memcpy(frame->sa, buf + 6, 6);
		frame->len = len;
		frame->rssi = ((ieee80211_frame_info_t *)userdata)->rssi;

		rtw_enter_critical(&lock, &irqL);

		if (eth_buffer.tail) {
			eth_buffer.tail->next = frame;
			frame->prev = eth_buffer.tail;
			eth_buffer.tail = frame;
		} else {
			eth_buffer.head = frame;
			eth_buffer.tail = frame;
		}

		rtw_exit_critical(&lock, &irqL);
	}
}

struct eth_frame *retrieve_frame(void)
{
	struct eth_frame *frame = NULL;
	_lock lock;
	_irqL irqL;

	rtw_enter_critical(&lock, &irqL);

	if (eth_buffer.head) {
		frame = eth_buffer.head;

		if (eth_buffer.head->next) {
			eth_buffer.head = eth_buffer.head->next;
			eth_buffer.head->prev = NULL;
		} else {
			eth_buffer.head = NULL;
			eth_buffer.tail = NULL;
		}
	}

	rtw_exit_critical(&lock, &irqL);

	return frame;
}

static void promisc_test(int duration, unsigned char len_used)
{
	int ch;
	unsigned int start_time;
	struct eth_frame *frame;
	eth_buffer.head = NULL;
	eth_buffer.tail = NULL;

	wifi_enter_promisc_mode();
	wifi_set_promisc(RTW_PROMISC_ENABLE, promisc_callback, len_used);

	for (ch = 1; ch <= 13; ch ++) {
		if (wifi_set_channel(ch) == 0) {
			printf("\n\n\rSwitch to channel(%d)", ch);
		}

		start_time = rtw_get_current_time();

		while (1) {
			unsigned int current_time = rtw_get_current_time();

			if (rtw_systime_to_ms(current_time - start_time) < (unsigned int)duration) {
				frame = retrieve_frame();

				if (frame) {
					int i;
					printf("\n\rDA:");
					for (i = 0; i < 6; i ++) {
						printf(" %02x", frame->da[i]);
					}
					printf(", SA:");
					for (i = 0; i < 6; i ++) {
						printf(" %02x", frame->sa[i]);
					}
					printf(", len=%d", frame->len);
					printf(", RSSI=%d", frame->rssi);
					rtw_free((void *) frame);
				} else {
					rtw_mdelay_os(1);    //delay 1 tick
				}
			} else {
				break;
			}
		}
	}

	wifi_set_promisc(RTW_PROMISC_DISABLE, NULL, 0);

	while ((frame = retrieve_frame()) != NULL) {
		rtw_free((void *) frame);
	}
}

static void promisc_callback_all(unsigned char *buf, unsigned int len, void *userdata)
{
	struct eth_frame *frame = (struct eth_frame *) rtw_malloc(sizeof(struct eth_frame));
	_lock lock;
	_irqL irqL;

	if (frame) {
		frame->prev = NULL;
		frame->next = NULL;
#if defined(CONFIG_UNSUPPORT_PLCPHDR_RPT) && CONFIG_UNSUPPORT_PLCPHDR_RPT
		if (((ieee80211_frame_info_t *)userdata)->type == RTW_RX_UNSUPPORT) {
			//NOTICE: buf structure now is rtw_rx_info_t.
			frame->type = 0xFF;
			memset(frame->da, 0, 6);
			memset(frame->sa, 0, 6);
		} else
#endif
		{
			memcpy(frame->da, buf + 4, 6);
			memcpy(frame->sa, buf + 10, 6);
			frame->type = *buf;
		}

		frame->len = len;
		/*
		* type is the first byte of Frame Control Field of 802.11 frame
		* If the from/to ds information is needed, type could be reused as follows:
		* frame->type = ((((ieee80211_frame_info_t *)userdata)->i_fc & 0x0100) == 0x0100) ? 2 : 1;
		* 1: from ds; 2: to ds
		*/

		frame->rssi = ((ieee80211_frame_info_t *)userdata)->rssi;

		rtw_enter_critical(&lock, &irqL);

		if (eth_buffer.tail) {
			eth_buffer.tail->next = frame;
			frame->prev = eth_buffer.tail;
			eth_buffer.tail = frame;
		} else {
			eth_buffer.head = frame;
			eth_buffer.tail = frame;
		}

		rtw_exit_critical(&lock, &irqL);
	}
}
static void promisc_test_all(int duration, unsigned char len_used)
{
	int ch;
	unsigned int start_time;
	struct eth_frame *frame;
	eth_buffer.head = NULL;
	eth_buffer.tail = NULL;

	wifi_enter_promisc_mode();
	wifi_set_promisc(RTW_PROMISC_ENABLE_2, promisc_callback_all, len_used);

	for (ch = 1; ch <= 13; ch ++) {
		if (wifi_set_channel(ch) == 0) {
			printf("\n\n\rSwitch to channel(%d)", ch);
		}

		start_time = rtw_get_current_time();

		while (1) {
			unsigned int current_time = rtw_get_current_time();

			if (rtw_systime_to_ms(current_time - start_time) < (unsigned int)duration) {
				frame = retrieve_frame();

				if (frame) {
					int i;
					printf("\n\rTYPE: 0x%x, ", frame->type);
					printf("DA:");
					for (i = 0; i < 6; i ++) {
						printf(" %02x", frame->da[i]);
					}
					printf(", SA:");
					for (i = 0; i < 6; i ++) {
						printf(" %02x", frame->sa[i]);
					}
					printf(", len=%d", frame->len);
					printf(", RSSI=%d", frame->rssi);
					rtw_free((void *) frame);
				} else {
					rtw_mdelay_os(1);    //delay 1 tick
				}
			} else {
				break;
			}
		}
	}

	wifi_set_promisc(RTW_PROMISC_DISABLE, NULL, 0);

	while ((frame = retrieve_frame()) != NULL) {
		rtw_free((void *) frame);
	}
}

void cmd_promisc(int argc, char **argv)
{
	int duration;
#ifdef CONFIG_PROMISC
	wifi_init_packet_filter();
#endif
	if ((argc == 2) && ((duration = atoi(argv[1])) > 0))
		//promisc_test(duration, 0);
	{
		promisc_test_all(duration, 0);
	} else if ((argc == 3) && ((duration = atoi(argv[1])) > 0) && (strcmp(argv[2], "with_len") == 0)) {
		promisc_test(duration, 1);
	} else {
		printf("\n\rUsage: %s DURATION_SECONDS [with_len]", argv[0]);
	}
}
#endif	//#if CONFIG_WLAN
