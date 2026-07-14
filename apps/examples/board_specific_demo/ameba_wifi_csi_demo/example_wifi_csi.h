#ifndef __EXAMPLE_WIFI_CSI_H__
#define __EXAMPLE_WIFI_CSI_H__

/******************************************************************************
 *
 * Copyright(c) 2007 - 2015 Realtek Corporation. All rights reserved.
 *
 *
 ******************************************************************************/
#include "platform_autoconf.h"
#include "platform_stdlib.h"
#include "basic_types.h"
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include "lwip_netconf.h"
#include "wifi_api.h"
#include "rtw_autoconf.h"
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include "rtw_queue.h"

#ifdef CONFIG_PLATFORM_TIZENRT_OS
#include "rtw_wifi_common.h"
#include "wifi_api_event.h"
#include "wifi_api_types_csi.h"
#include "os_wrapper_task.h"
#include "os_wrapper_semaphore.h"
#include "os_wrapper_mutex.h"
#include "os_wrapper_misc.h"

#define NOTAG ""
#define RTK_LOGA(tag, fmt, ...)			printf(fmt __VA_OPT__(,) __VA_ARGS__)

/* From lwiP_netconf.h */
#define CONNECTION_VALID 0
#define CONNECTION_INVALID 1

enum {
#if defined(CONFIG_WLAN)
	NETIF_WLAN_STA_INDEX = STA_WLAN_INDEX,
	NETIF_WLAN_AP_INDEX = SOFTAP_WLAN_INDEX,
#endif
#if defined(CONFIG_NAN)
	NETIF_WLAN_NAN_INDEX,
#endif
#if defined(CONFIG_LWIP_USB_ETHERNET)
	NETIF_USB_ETH_INDEX,
#endif
#if defined(CONFIG_LWIP_ETHERNET)
	NETIF_ETH_INDEX,
#endif
	NET_IF_NUM
};
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

#ifdef CONFIG_PLATFORM_TIZENRT_OS
#define CSI_REPORT_BUF_NUM 5
#else
#define CSI_REPORT_BUF_NUM 1
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
#define CSI_REPORT_BUF_SIZE 1024

struct csi_report_data {
	struct list_head list;
	u8 *csi_buffer;
	u32 length;
};

struct csi_report_q_priv {
	struct csi_report_data csi_rpt_pkt[CSI_REPORT_BUF_NUM];
	struct list_head idle_q;
	struct list_head busy_q;
	rtos_mutex_t csi_rpt_q_mutex;  /* mutex */
	u32 idle_q_cnt;
	u32 busy_q_cnt;
};

void example_wifi_csi_report_cb(u8 *evt_info);
void wifi_csi_show(struct csi_report_data *csi_rpt_pkt);
void example_wifi_csi(void);

#endif //#ifndef __EXAMPLE_WIFI_CSI_H__
