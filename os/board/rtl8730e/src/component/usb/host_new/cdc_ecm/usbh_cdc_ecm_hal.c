/**
  ******************************************************************************
  * @file    usbh_cdc_ecm_hal.c
  * @author  Realsil WLAN5 Team
  * @version V1.0.0
  * @date    2023-01-13
  * @brief   This file provides the apis for USB CDC ECM host class
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include <platform_opts.h>
#include <FreeRTOS.h>
#include <task.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include "usbh_cdc_ecm.h"
#include "usbh_cdc_ecm_hal.h"


/* Private defines -----------------------------------------------------------*/
#define CONFIG_USBH_CDC_ECM_HOT_PLUG_TEST   1     /* Hot plug test */

#define USBH_CDC_ECM_INTR_BUF_SIZE          256   /* Buffer size for INTR loopback test, which should match with device INTR loopback buffer size */
#define USBH_CDC_ECM_LOOPBACK_CNT           100   /* Loopback test round */

#define USBH_CORE_INIT_FAIL                 (1)
#define USBH_CLASS_INIT_FAIL                (2)


/* Private types -------------------------------------------------------------*/
#pragma pack(push)
#pragma pack(1)
typedef struct {
	usb_report_usbdata			report_data;  			//usb rx callback function

	volatile u8 				send_success;			//usb tx send flag
	volatile u8 				cdc_ecm_is_ready;		//ecm attached status
	volatile u8 				ecm_hw_connect;		//ecm ethernet connect status:0 disconnect,1 connect
	volatile u8 				ecm_init_success;		//usb init success
} usbh_cdc_ecm_host_hal_t;
#pragma pack(pop)

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static u8 cdc_ecm_cb_init(void);
static u8 cdc_ecm_cb_deinit(void);
static u8 cdc_ecm_cb_attach(void);
static u8 cdc_ecm_cb_detach(void);
static u8 cdc_ecm_cb_setup(void);
static u8 cdc_ecm_cb_process(usb_host_t *host, u8 id);
static u8 cdc_ecm_cb_bulk_send(usbh_urb_state_t state);
static u8 cdc_ecm_cb_bulk_receive(u8 *pbuf, u32 Len);
static u8 cdc_ecm_cb_intr_receive(u8 *buf, u32 length);
static u8 set_usbh_cdc_ecm_bulk_send_flag(u8 flag);
static u8 usbh_cdc_ecm_doinit(void);

/* Private variables ---------------------------------------------------------*/
static u8  cdc_ecm_intr_rx_buf[USBH_CDC_ECM_INTR_BUF_SIZE];

static _sema cdc_ecm_detach_sema;
static _sema cdc_ecm_attach_sema;
static _sema cdc_ecm_intr_start_sema;
static _sema cdc_ecm_bulk_start_sema;

static usbh_cdc_ecm_host_hal_t usbh_cdc_ecm_host_user;

static usbh_config_t usbh_ecm_cfg = {
	.pipes = 5U,
	.speed = USB_SPEED_HIGH,
	.dma_enable = FALSE,
	.main_task_priority = 3U,
	.isr_task_priority = 4U,
	.rx_fifo_size = 0x200U,
	.nptx_fifo_size = 0x100U,
	.ptx_fifo_size = 0x100U,
};

static usbh_cdc_ecm_state_cb_t cdc_ecm_usb_cb = {
	.init = cdc_ecm_cb_init,
	.deinit = cdc_ecm_cb_deinit,
	.attach = cdc_ecm_cb_attach,
	.detach = cdc_ecm_cb_detach,
	.setup = cdc_ecm_cb_setup,
	.bulk_send = cdc_ecm_cb_bulk_send,
	.bulk_receive = cdc_ecm_cb_bulk_receive,
	.intr_received = cdc_ecm_cb_intr_receive,
};

static usbh_user_cb_t usbh_ecm_usr_cb = {
	.process = cdc_ecm_cb_process
};

/* Private functions ---------------------------------------------------------*/
static u8 usbh_cdc_ecm_get_usb_status(void)//
{
	return usbh_cdc_ecm_host_user.cdc_ecm_is_ready ;
}
static u8 set_usbh_cdc_ecm_bulk_send_flag(u8 flag)
{
	u8 ret =  usbh_cdc_ecm_host_user.send_success;
	usbh_cdc_ecm_host_user.send_success = flag;
	return ret;
}
static u8 cdc_ecm_cb_init(void)
{
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[CDC_ECM] CDC ECM INIT\n");
	return HAL_OK;
}
static u8 cdc_ecm_cb_deinit(void)
{
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[CDC_ECM] CDC ECM DEINIT\n");
	return HAL_OK;
}
static u8 cdc_ecm_cb_attach(void)
{
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[CDC_ECM] ATTACH\n");
	rtw_up_sema(&cdc_ecm_attach_sema);
	return HAL_OK;
}
static u8 cdc_ecm_cb_detach(void)
{
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[CDC_ECM] DETACH\n");
	usbh_cdc_ecm_host_user.cdc_ecm_is_ready = 0;
#if CONFIG_USBH_CDC_ECM_HOT_PLUG_TEST
	rtw_up_sema(&cdc_ecm_detach_sema);
#endif

	return HAL_OK;
}

static u8 cdc_ecm_cb_setup(void)
{
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[CDC_ECM] SETUP\n");
	usbh_cdc_ecm_host_user.cdc_ecm_is_ready = 1;
	rtw_up_sema(&cdc_ecm_intr_start_sema);
	rtw_up_sema(&cdc_ecm_bulk_start_sema);

	return HAL_OK;
}

static u8 cdc_ecm_cb_bulk_receive(u8 *buf, u32 length)
{
	//DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "[CDC_ECM] bulk receive \n");
	if (usbh_cdc_ecm_host_user.report_data != NULL) {
		usbh_cdc_ecm_host_user.report_data(buf, length);
	}

	return HAL_OK;
}
static u8 cdc_ecm_cb_bulk_send(usbh_urb_state_t state)
{
	set_usbh_cdc_ecm_bulk_send_flag(1);
	if (state != USBH_URB_DONE) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "[CDC_ECM] Transmit fail: %d\n", state);
	}

	return HAL_OK;
}
//todo
//add parse to get the ethernet status
static u8 cdc_ecm_cb_intr_receive(u8 *buf, u32 length)
{
	//DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "[USBH] INTR RX len=%d\n", length);
	if (buf && length >= 8) {
		/*A1 00 00 00 01 00 00 00 */
		if (length == 8 && buf[0] == 0xA1 && buf[1] == CDC_ECM_NOTIFY_NETWORK_CONNECTION) {
			usbh_cdc_ecm_host_user.ecm_hw_connect = buf[2];
		}
		/*A1 2A 00 00 01 00 00 08 	00 00 00 00 00 00 00 00 */
		else if (length == 16 && buf[0] == 0xA1 && buf[1] == CDC_ECM_NOTIFY_CONNECTION_SPEED_CHANGE) {
			//usbh_cdc_ecm_speed_change_t
		}
	}

	return HAL_OK;
}


static u8 cdc_ecm_cb_process(usb_host_t *host, u8 id)
{
	UNUSED(host);
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_TRACE, "[CDC_ECM] Process ,id=%d\n", id);
	switch (id) {
	case USBH_MSG_USER_SET_CONFIG:
		usbh_cdc_ecm_setconfig(host, 1);	//choose config id=1
		break;
	case USBH_MSG_DISCONNECTED:
		usbh_cdc_ecm_host_user.cdc_ecm_is_ready = 0;
		break;

	case USBH_MSG_CONNECTED:
		break;

	default:
		break;
	}

	return HAL_OK;
}

static void ecm_intr_rx_thread(void *param)
{
	int i;
	UNUSED(param);
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[USBH] INTR test pending, wait for device ready\n");

	if (rtw_down_sema(&cdc_ecm_intr_start_sema)) {
		rtw_msleep_os(500);
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[USBH] intr send task started \n");

		do {
			i++;
			if (!usbh_cdc_ecm_get_usb_status()) { //18s
				if ((i % 10 == 0)) {
					DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "[USBH] Device disconnected, intr test aborted[%d]\n", usbh_cdc_ecm_get_usb_status());
				}
				rtw_msleep_os(1000);
				continue;
			}
			if (!usbh_cdc_ecm_intr_in_processing()) {
				usbh_cdc_ecm_intr_receive(cdc_ecm_intr_rx_buf, USBH_CDC_ECM_INTR_BUF_SIZE);
			}
			rtw_msleep_os(1 * 50); //should sleep by bInterval value
		} while (1);
	}

	rtw_thread_exit();
}

static void ecm_bulk_rx_thread(void *param)
{
	u32 i;
	UNUSED(param);
	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[USBH] bulk test pending, wait for device ready\n");

	if (rtw_down_sema(&cdc_ecm_bulk_start_sema)) {
		rtw_msleep_os(500);
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[USBH] bulk send task started \n");

		do {
			i++;
			if (!usbh_cdc_ecm_get_usb_status()) {
				if ((i % 10 == 0)) {
					DBG_PRINTF(MODULE_USB_CLASS, LEVEL_WARN, "[USBH] Device disconnected, bulk test aborted[%d]\n", usbh_cdc_ecm_get_usb_status());
				}
				rtw_msleep_os(1000);
				continue;
			}
			if (!usbh_cdc_ecm_bulk_in_processing()) {
				usbh_cdc_ecm_bulk_receive();
				//rtw_msleep_os(1*1);
			} else {
				rtw_msleep_os(1 * 1);
			}
		} while (1);
	}

	rtw_thread_exit();
}

#if CONFIG_USBH_CDC_ECM_HOT_PLUG_TEST
static void ecm_hotplug_thread(void *param)
{
	int ret = 0;

	UNUSED(param);

	for (;;) {
		rtw_down_sema(&cdc_ecm_detach_sema);
		rtw_msleep_os(2000);
		usbh_cdc_ecm_deinit();
		usbh_deinit();
		rtw_msleep_os(10);
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[CDC_ECM] Free heap size: 0x%lx\n", xPortGetFreeHeapSize());

		ret = usbh_cdc_ecm_doinit();
		if (ret != 0) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "[CDC_ECM] Fail to init USB host controller: %d\n", ret);
			break;
		}
	}
	rtw_thread_exit();
}
#endif

static u8 usbh_cdc_ecm_doinit(void)
{
	int status;

	status = usbh_init(&usbh_ecm_cfg, &usbh_ecm_usr_cb);
	if (status != HAL_OK) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "[CDC_ECM] Fail to init USB host controller: %d\n", status);
		return USBH_CORE_INIT_FAIL;
	}

	status = usbh_cdc_ecm_init(&cdc_ecm_usb_cb);  /*0 means use default transfer size, and it can not exceed 65536*/
	if (status != HAL_OK) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "[CDC_ECM] Fail to init USB host cdc_ecm driver: %d\n", status);
		return USBH_CLASS_INIT_FAIL;
	}

	if (rtw_down_sema(&cdc_ecm_attach_sema)) {
		usbh_cdc_ecm_state_t step = CDC_ECM_STATE_ALT_SETTING;
		u8 loop = 1;
		u8 mac_str[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
		do {
			switch (step) {
			case CDC_ECM_STATE_ALT_SETTING: //choose alt
				status = usbh_cdc_ecm_alt_setting();
				if (HAL_OK != status) {
					rtw_msleep_os(100);
				} else {
					step = CDC_ECM_STATE_SWITCH_TO_TRANSFER;
				}
				break;

			case CDC_ECM_STATE_SET_ETHERNET_MULTICAST_FILTER://set the multi list
				status = usbh_cdc_ecm_set_ethernet_multicast_filter(mac_str, 6);
				if (HAL_OK != status) {
					rtw_msleep_os(100);
				} else {
					step = CDC_ECM_STATE_SET_ETHERNET_PACKET_FILTER;
				}
				break;

			case CDC_ECM_STATE_SET_ETHERNET_PACKET_FILTER://set the filter
				status = usbh_cdc_ecm_set_ethernet_packetfilter(CDC_ECM_ETH_PACKET_TYPE_DIRECTED);
				if (HAL_OK != status) {
					rtw_msleep_os(100);
				} else {
					step = CDC_ECM_STATE_SWITCH_TO_TRANSFER;
				}
				break;

			case CDC_ECM_STATE_SWITCH_TO_TRANSFER://switch to transfer
			default:
				status = usbh_cdc_ecm_set_ethernet_start_transfer();
				if (HAL_OK != status) {
					rtw_msleep_os(100);
				} else {
					loop = 0;
				}
				break;
			}
		} while (loop);
	}

	return HAL_OK;
}
static void usbh_cdc_ecm_init_thread(void *param)
{
	int status;
	struct task_struct intr_task;
	struct task_struct bulk_task;
#if CONFIG_USBH_CDC_ECM_HOT_PLUG_TEST
	struct task_struct hotplug_task;
#endif

	UNUSED(param);

	rtw_init_sema(&cdc_ecm_detach_sema, 0);
	rtw_init_sema(&cdc_ecm_attach_sema, 0);
	rtw_init_sema(&cdc_ecm_intr_start_sema, 0);
	rtw_init_sema(&cdc_ecm_bulk_start_sema, 0);

	status = usbh_cdc_ecm_doinit();
	if (USBH_CORE_INIT_FAIL == status) {
		goto free_sema_exit;
	} else if (USBH_CLASS_INIT_FAIL == status) {
		goto usb_deinit_exit;
	}

#if CONFIG_USBH_CDC_ECM_HOT_PLUG_TEST
	status = rtw_create_task(&hotplug_task, "ecm_hotplug_thread", 512, tskIDLE_PRIORITY + 2, (thread_func_t)ecm_hotplug_thread, NULL);
	if (status != pdPASS) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "[CDC_ECM] Fail to create USBH cdc_ecm hotplug check thread\n");
		goto usb_ecm_deinit_exit;
	}
#endif

	status = rtw_create_task(&intr_task, "ecm_intr_rx_thread", 512, tskIDLE_PRIORITY + 2, (thread_func_t)ecm_intr_rx_thread, NULL);
	if (status != pdPASS) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "[USBH] Fail to create USBH INTR intr_cdc_ecm_test_task thread\n");
		goto usbh_ecm_cdc_deinit_exit;
	}

	status = rtw_create_task(&bulk_task, "ecm_bulk_rx_thread", 512, tskIDLE_PRIORITY + 2, (thread_func_t)ecm_bulk_rx_thread, NULL);
	if (status != pdPASS) {
		DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "[USBH] Fail to create USBH BULK ecm_bulk_rx_thread thread\n");
		goto delete_intr_task_exit;
	}

	goto example_exit;

delete_intr_task_exit:
	rtw_delete_task(&intr_task);

usbh_ecm_cdc_deinit_exit:
#if CONFIG_USBH_CDC_ECM_HOT_PLUG_TEST
	rtw_delete_task(&hotplug_task);
#endif

usb_ecm_deinit_exit:
	usbh_cdc_ecm_deinit();

usb_deinit_exit:
	usbh_deinit();

free_sema_exit:
	rtw_free_sema(&cdc_ecm_detach_sema);
	rtw_free_sema(&cdc_ecm_attach_sema);
	rtw_free_sema(&cdc_ecm_intr_start_sema);
	rtw_free_sema(&cdc_ecm_bulk_start_sema);

example_exit:
	rtw_thread_exit();
}

/* Exported functions --------------------------------------------------------*/
u8 usbh_cdc_ecm_do_deinit(void)//todo destory all usb task
{
	return 0 ;
}
u8 usbh_cdc_ecm_do_init(usb_report_usbdata cb_handle)
{
	int status;
	struct task_struct task;

	usbh_cdc_ecm_host_user.report_data = cb_handle ;

#if 0
	DBG_ERR_MSG_ON(MODULE_USB_OTG);
	DBG_WARN_MSG_ON(MODULE_USB_OTG);
	DBG_INFO_MSG_ON(MODULE_USB_OTG);
	//ConfigDebug[LEVEL_TRACE] |= BIT(MODULE_USB_OTG);

	DBG_ERR_MSG_ON(MODULE_USB_CLASS);
	DBG_WARN_MSG_ON(MODULE_USB_CLASS);
	DBG_INFO_MSG_ON(MODULE_USB_CLASS);
	//ConfigDebug[LEVEL_TRACE] |= BIT(MODULE_USB_CLASS);
#endif

	DBG_PRINTF(MODULE_USB_CLASS, LEVEL_INFO, "[CDC_ECM] enter USB host init...\n");
	if (0 == usbh_cdc_ecm_host_user.ecm_init_success) {
		status = rtw_create_task(&task, "ecm_init_thread", 512, tskIDLE_PRIORITY + 1, usbh_cdc_ecm_init_thread, NULL);
		if (status != pdPASS) {
			DBG_PRINTF(MODULE_USB_CLASS, LEVEL_ERROR, "[CDC_ECM] Fail to create USB host cdc_ecm init thread: %d\n", status);
			return 1 ;
		}
		usbh_cdc_ecm_host_user.ecm_init_success = 1;
	}

	return 0;
}

u16 usbh_cdc_ecm_get_receive_mps(void)
{
	return usbh_cdc_ecm_get_usbin_mps();
}
u8 usbh_cdc_ecm_get_sendflag(void)
{
	return usbh_cdc_ecm_host_user.send_success;
}
u8 usbh_cdc_ecm_senddata(u8 *buf, u32 len)
{
	set_usbh_cdc_ecm_bulk_send_flag(0);
	return usbh_cdc_ecm_bulk_send(buf, len);
}
u8 usbh_cdc_ecm_get_connect_status(void)//1 up
{
	u8 ret = usbh_cdc_ecm_host_user.cdc_ecm_is_ready && usbh_cdc_ecm_host_user.ecm_hw_connect;
	return ret ;
}

