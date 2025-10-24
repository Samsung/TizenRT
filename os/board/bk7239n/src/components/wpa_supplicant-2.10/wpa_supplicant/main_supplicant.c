/*
 * WPA Supplicant / main() function for UNIX like OSes and MinGW
 * Copyright (c) 2003-2013, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */
#include "../src/utils/includes.h"
#include "../src/utils/common.h"
#include "wpa_supplicant_i.h"
#include "driver_i.h"
#include "../hostapd/main_none.h"
//#include "ps.h"
#include "sys_rtos.h"
#include "../src/utils/os.h"
#include <common/bk_kernel_err.h>
#include "../bk_patch/signal.h"
#include "../src/utils/eloop.h"
#include "config.h"
#if CFG_SUPPORT_BSSID_CONNECT
#include "bk_wifi_types.h"
#endif
#include "modules/wifi.h"
#include "bss.h"
#if CONFIG_LWIP
//#include "net.h"
#endif
#include "../src/common/wpa_psk_cache.h"
#include "rw_msg_rx.h"
#include "modules/wifi.h"
#include "bk_hostapd_intf.h"
#include "bk_private/bk_wifi.h"
#include "../bk_patch/fake_socket.h"

#include "../src/l2_packet/l2_packet.h"
#ifdef CONFIG_P2P
/* if SQRTMOD_USE_MOD_EXP is not enabled, enlarge stack size to 15K */
#define WPAS_STACK_SZ	4096*2
#elif defined(CONFIG_USE_MBEDTLS) && defined(CONFIG_OCV)
/*
 * FIXME: bk7236, crypto_ec_point_compute_y_sqr -> mbedtls_mpi_exp_mod ->
 *   mbedtls_mpi RR, T, W[ 2 << MBEDTLS_MPI_WINDOW_SIZE ] uses lots of stack
 * size. And wpa_supplicant_event_associnfo also eats lots of stack size.
 */
#define WPAS_STACK_SZ	5120
#else
/* if SQRTMOD_USE_MOD_EXP is not enabled, enlarge stack size to 15K */
#define WPAS_STACK_SZ	5120//4096
#endif
static struct wpa_global *wpa_global_ptr = NULL;
beken_thread_t wpas_thread_handle = NULL;
uint32_t wpas_stack_size = WPAS_STACK_SZ;
beken_semaphore_t wpas_sema = NULL;
struct wpa_ssid_value *wpas_connect_ssid = 0;
struct wpa_interface *wpas_ifaces = 0;

extern beken_queue_t wpah_queue;

extern void wpas_thread_start(void);
extern void wpas_thread_stop(void);
extern void wpa_handler_signal(void *arg, u8 vif_idx);

extern uint32_t sta_ip_start_flag;
extern uint32_t uap_ip_start_flag;

struct wpa_supplicant *wpa_suppliant_ctrl_get_wpas()
{
	if (!wpa_global_ptr)
		return NULL;
	return wpa_global_ptr->ifaces;
}

int wpa_get_psk(char *psk)
{
    struct wpa_config *conf = NULL;

    if(!psk)
    {
        return -1;
    }
    memset(psk, 0, 32);
    conf = wpa_global_ptr->ifaces->conf;
    memcpy(psk, conf->ssid->psk, 32);

    return 0;
}

int supplicant_main_exit(void)
{
	if (wpa_global_ptr == NULL)
		return 0;

	if (wpa_global_ptr) {
		wpa_supplicant_deinit(wpa_global_ptr);
		wpa_global_ptr = NULL;
	}

	if (wpas_ifaces) {
		os_free(wpas_ifaces);
		wpas_ifaces = 0;
	}

	if (wpas_connect_ssid) {
		os_free(wpas_connect_ssid);
		wpas_connect_ssid = 0;
	}

	return 0;
}

u8 supplicant_main_is_exit(void)
{
	return (wpa_global_ptr == NULL) ? 1 : 0;
}

int supplicant_main_entry(char *oob_ssid)
{
	int i;
	int iface_count, exitcode = -1;
	struct wpa_params params;
	struct wpa_supplicant *wpa_s = 0;
	struct wpa_interface *iface;

	if (os_program_init())
		return -1;

	os_memset(&params, 0, sizeof(params));
	params.wpa_debug_level = MSG_DEBUG;
	params.wpa_debug_show_keys = 1;

	if (0 == wpas_ifaces) {
		wpas_ifaces = os_zalloc(sizeof(struct wpa_interface));
		if (wpas_ifaces == NULL)
			return -1;
	}

	iface = wpas_ifaces;
	iface_count = 1;
	iface->ifname = bss_iface;
	exitcode = 0;

	WPA_LOGD("sizeof(wpa_supplicant)=%d\n", sizeof(*wpa_s));
	wpa_global_ptr = wpa_supplicant_init(&params);
	if (wpa_global_ptr == NULL) {
		wpa_printf(MSG_ERROR, "Failed to initialize wpa_supplicant");
		exitcode = -1;
		goto out;
	} else {
		wpa_printf(MSG_INFO, "Successfully initialized wpa_supplicant");
	}

	for (i = 0; exitcode == 0 && i < iface_count; i++) {
		if (wpas_ifaces[i].ctrl_interface == NULL &&
			wpas_ifaces[i].ifname == NULL) {
			if (iface_count == 1 /* && (params.ctrl_interface || params.dbus_ctrl_interface) */)
				break;

			exitcode = -1;
			break;
		}

		wpa_s = wpa_supplicant_add_iface(wpa_global_ptr, &wpas_ifaces[i], NULL);
		if (wpa_s == NULL) {
			exitcode = -1;
			break;
		}
	}

	if (exitcode) {
		wpa_supplicant_deinit(wpa_global_ptr);
		wpa_global_ptr = NULL;
	} else {
		wpa_supplicant_run(wpa_global_ptr);

		return 0;
	}

out:
	os_free(wpas_ifaces);
	wpas_ifaces = 0;

	return exitcode;
}

static void wpas_thread_main( void *arg )
{
#ifdef CONFIG_WPA_PSK_CACHE
	wpa_psk_cache_init();
#endif

    eloop_init();

    eloop_run();

	wpas_thread_handle = NULL;

    rtos_deinit_queue(&wpah_queue);
    wpah_queue = NULL;

	rtos_delete_thread(NULL);
}

void wpas_thread_start(void)
{
	bk_err_t ret;
	SOCKET_ENTITY *fsocket = get_fsocket_entity();

	struct l2_packet_head *l2_packet = get_l2_packet_entity();
	if (wpah_queue == NULL) {
		ret = rtos_init_queue(&wpah_queue,
							  "wpah_queue",
							  sizeof(wpah_msg_t),
							  64);
		BK_ASSERT(kNoErr == ret);
	}

	if (NULL == wpas_thread_handle) {
		ret = rtos_create_thread(&wpas_thread_handle,
								 CONFIG_TASK_WPAS_PRIO,
								 "wpas_thread",
								 (beken_thread_function_t)wpas_thread_main,
								 (unsigned short)wpas_stack_size,
								 (beken_thread_arg_t)NULLPTR);
		BK_ASSERT(kNoErr == ret);
	}

	rtos_init_mutex(&fsocket->fs_mutex);
	rtos_init_mutex(&l2_packet->l2_mutex);
}

void wpas_thread_stop(void)
{
	SOCKET_ENTITY *fsocket = get_fsocket_entity();

	struct l2_packet_head *l2_packet = get_l2_packet_entity();
	wpa_handler_signal((void*)SIGTERM, 0xff);

	while(wpas_thread_handle != NULL) {
		rtos_delay_milliseconds(10);
	}

	rtos_deinit_mutex(&fsocket->fs_mutex);
	rtos_deinit_mutex(&l2_packet->l2_mutex);
}

void wpa_supplicant_poll(void *param)
{
    bk_err_t ret;

	if(wpas_sema)
	{
    	ret = rtos_set_semaphore(&wpas_sema);
	}

	(void)ret;
}

int wpa_sem_wait(uint32_t ms)
{
	if(NULL == wpas_sema)
	{
		return kTimeoutErr;
	}

	return rtos_get_semaphore(&wpas_sema, ms);
}

u8* wpas_get_sta_psk(void)
{
	return wpa_global_ptr->ifaces->conf->ssid->psk;
}
// eof

