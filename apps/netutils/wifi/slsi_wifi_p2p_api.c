/**
 * @file apps/netutils/wifi/slsi_wifi_api.c
 *
 *   Copyright (c) 2016 Samsung Electronics, Inc.
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>

#include "utils/includes.h"
#include "utils/common.h"
#include "common/wpa_ctrl.h"
#include "common/ieee802_11_defs.h"
#include "common/ieee802_11_common.h"
#include "common/defs.h"
#include "wps/wps.h"

#include <slsi_wifi_api.h>
#include <slsi_wifi_nvmap.h>
#include "fsm.h"
#include "slsi_wifi_p2p_fsm_definitions.h"
#include "slsi_wifi_private.h"

// P2P define should be on outer scope of this file
#ifdef CONFIG_SLSI_WIFI_P2P_API

#ifdef CONFIG_SLSI_WIFI_P2P_POSTFIX
#define SLSI_WIFI_P2P_POSTFIX           CONFIG_SLSI_WIFI_P2P_POSTFIX
#else
#define SLSI_WIFI_P2P_POSTFIX           ""
#endif

/*
 * Defines
 */
#define LOCKUNLOCK_CRITICAL { \
    EPRINT("LOCKUNLOCK_CRITICAL - %s\n", __func__); \
    if(SLSI_API_VERBOSE) fflush(stdout); \
    pthread_mutex_trylock(&mutex_state); \
    pthread_mutex_unlock(&mutex_state); \
}

#define ENTER_CRITICAL { \
    VPRINT("ENTER_CRITICAL in API - %s\n", __func__); \
    if(!g_mutex_initialized){ \
        VPRINT("Initializing the mutex\n"); \
        if(pthread_mutex_init(&mutex_state, NULL) != OK){ \
            EPRINT("Could not initialize mutex\n"); \
        }else { \
            VPRINT("Mutex ready\n"); \
            g_mutex_initialized = TRUE; \
        } \
    } \
    if(SLSI_API_VERBOSE) fflush(stdout); \
    int err = pthread_mutex_lock(&mutex_state); \
    if(err != OK){ \
        EPRINT("mutex lock failed with error %d",err); \
    } \
}

#define LEAVE_CRITICAL { \
    VPRINT("LEAVE_CRITICAL in API - %s\n", __func__); \
    if(SLSI_API_VERBOSE) fflush(stdout); \
    pthread_mutex_unlock(&mutex_state); \
}

#define CTRL_IFNAME_MAX_LEN     10

/*
 * Type definitions
 */
typedef struct p2p_fsm_generic_instance {
	int8_t result;
} p2p_fsm_generic_instance_t;

typedef struct p2p_fsm_group_instance {
	int8_t result;
	uint8_t channel;
	bool persistent;
} p2p_fsm_group_instance_t;

typedef struct p2p_fsm_event_instance {
	const char *event_str;
} p2p_fsm_event_instance_t;

typedef struct p2p_fsm_connect_instance {
	slsi_peer_info_t device_connecting;
	slsi_config_method_t config_method;
	slsi_p2p_connect_response_t response;
	char passphrase[SLSI_PASSPHRASE_LEN + 1];
	uint8_t go_intent;
} p2p_fsm_connect_instance_t;

typedef struct p2p_fsm_connect_out_instance {
	int8_t result;
	uint32_t timeout;
	slsi_config_method_t config_method;
	bool persistent;
	bool join;
	bool auto_mode;
	bool auth;
	bool provdisc;
} p2p_fsm_connect_out_instance_t;

typedef struct p2p_fsm_find_instance {
	int8_t result;
	char *device_address;
	uint32_t timeout;
} p2p_fsm_find_instance_t;

/*
 * Local parameter definitions
 */
#ifdef CONFIG_SCSC_WLAN_AUTO_RECOVERY

typedef struct slsi_recovery_data {
	slsi_api_states_t old_state;
	slsi_api_states_t recovering_state;
	WiFi_InterFace_ID_t old_interface_type;
	slsi_network_link_callback_t link_up;	// callback function
	slsi_network_link_callback_t link_down;	// callback function
	slsi_p2p_find_handler_t find_result_handler;	// callback function
	char remote_bssid[18];		// char string e.g. xx:xx:xx:xx:xx:xx
	slsi_ap_config_t ap_config;	// contains security pointer!
	uint8_t ssid[SLSI_SSID_LEN + 1];	// 802.11 spec defined unspecified or uint8
	int8_t ssid_len;			// length of ssid - # of valid octets
	uint8_t bssid[18];			// char string e.g. xx:xx:xx:xx:xx:xx
	slsi_security_config_t *security;
} slsi_recovery_data_t;

static slsi_recovery_data_t g_recovery_data;
static pthread_t g_recovery_thread;
static int g_recovery_running = 0;
static sem_t g_sem_recover;
#endif							// CONFIG_SCSC_WLAN_AUTO_RECOVERY
// END Recovery

#define FSM(a,b,c,d,e) fsm_32bit_run(a,b,c,d,e)
static fsm32_state_t p2p_fsm_state = P2P_ST_NULL;
static pthread_mutex_t mutex_state;
static bool g_mutex_initialized = FALSE;
static slsi_p2p_connect_ind_t g_connect_handler;
static sem_t g_connect_find_sem;
static sem_t g_connect_neg_sem;
static sem_t g_terminating;
static sem_t g_disconnect;

static p2p_fsm_connect_instance_t g_connect_inst;
static p2p_fsm_connect_out_instance_t g_connect_out_inst;
static bool g_is_group_owner = FALSE;
static char g_third_interface[CTRL_IFNAME_MAX_LEN];
static uint8_t g_connect_failed_reason = SLSI_STATUS_ERROR;
/*
 * Local function prototypes
 */
static fsm_result_t slsi_p2p_fsm(fsm_event_t event, void *arg1, void *arg2);
static int8_t slsi_p2p_find(uint32_t timeout, char *dev_addr);
static int8_t slsi_p2p_listen(uint8_t channel);
static int8_t slsi_p2p_stop_find(void);
static int8_t slsi_p2p_set_device_name(uint8_t *name, uint8_t name_len);
static int8_t slsi_p2p_set_postfix(uint8_t *device_name, uint8_t device_name_len);

extern const char *const slsi_wifi_p2p_state_names[];
extern const char *const slsi_wifi_p2p_event_names[];

/*
 * Public API function definitions declared in slsi_wifi_api.h
 */
int8_t WiFiP2PSetDeviceName(uint8_t *device_name, uint8_t device_name_len)
{
	ENTER_CRITICAL;
	int8_t result = SLSI_STATUS_ERROR;
	if (device_name && device_name_len > 0) {
		VPRINT("Device name: %s length: %d\n", device_name, device_name_len);
		result = slsi_p2p_set_device_name(device_name, device_name_len);
		if (result == SLSI_STATUS_SUCCESS) {
			result = slsi_p2p_set_postfix(device_name, device_name_len);
		}
	} else {
		EPRINT("Device name parameters wrong\n");
		result = SLSI_STATUS_PARAM_FAILED;
	}
	LEAVE_CRITICAL;
	return result;
}

int8_t WiFiP2PListen(void)
{
	ENTER_CRITICAL;
	p2p_fsm_generic_instance_t inst;
	inst.result = SLSI_STATUS_ERROR;
	fsm_result_t res = slsi_p2p_fsm(P2P_EV_LISTEN, &inst, NULL);
	UNUSED(res);
	LEAVE_CRITICAL;
	return inst.result;
}

int8_t WiFiP2PStopListen(void)
{
	ENTER_CRITICAL;
	p2p_fsm_generic_instance_t inst;
	inst.result = SLSI_STATUS_ERROR;
	fsm_result_t res = slsi_p2p_fsm(P2P_EV_STOP_LISTEN, &inst, NULL);
	UNUSED(res);
	LEAVE_CRITICAL;
	return inst.result;
}

int8_t WiFiP2PFind(uint32_t timeout, char device_address[SLSI_MACADDR_STR_LEN])
{
	ENTER_CRITICAL;
	p2p_fsm_find_instance_t inst;
	inst.timeout = timeout;
	inst.device_address = device_address;
	inst.result = SLSI_STATUS_ERROR;
	fsm_result_t res = slsi_p2p_fsm(P2P_EV_FIND, &inst, NULL);
	UNUSED(res);
	LEAVE_CRITICAL;
	return inst.result;
}

int8_t WiFiP2PStopFind(void)
{
	ENTER_CRITICAL;
	p2p_fsm_generic_instance_t inst;
	inst.result = SLSI_STATUS_ERROR;
	fsm_result_t res = slsi_p2p_fsm(P2P_EV_STOP_FIND, &inst, NULL);
	UNUSED(res);
	LEAVE_CRITICAL;
	return inst.result;
}

int8_t WiFiP2PRegisterFindCallback(slsi_p2p_find_callback_t find_handler)
{
	ENTER_CRITICAL;
	g_find_results_handler = find_handler;
#ifdef CONFIG_SCSC_WLAN_AUTO_RECOVERY
	g_recovery_data.find_result_handler = find_handler;
#endif
	LEAVE_CRITICAL;
	return SLSI_STATUS_SUCCESS;
}

int8_t WiFiP2PCreateGroup(uint8_t channel, bool persistent)
{
	p2p_fsm_group_instance_t inst;
	inst.result = SLSI_STATUS_ERROR;
	inst.persistent = persistent;
	if (!persistent) {			//persistance is currently not supported. included for future use
		inst.channel = channel;
		fsm_result_t res = slsi_p2p_fsm(P2P_EV_ADD_GROUP, &inst, NULL);
		UNUSED(res);
	} else {
		inst.result = SLSI_STATUS_NOT_SUPPORTED;
	}
	return inst.result;
}

int8_t WiFiP2PRemoveGroup(void)
{
	p2p_fsm_generic_instance_t inst;
	inst.result = SLSI_STATUS_ERROR;
	fsm_result_t res = slsi_p2p_fsm(P2P_EV_REMOVE_GROUP, &inst, NULL);
	UNUSED(res);
	return inst.result;
}

int8_t WiFiP2PConnect(char device_address[SLSI_MACADDR_STR_LEN], slsi_config_method_t config_method)
{
	ENTER_CRITICAL;
	memset(&g_connect_out_inst, 0, sizeof(p2p_fsm_connect_out_instance_t));
	g_connect_out_inst.result = SLSI_STATUS_ERROR;
	g_connect_out_inst.timeout = 30;	//FIXME make it a define or similar - find good value
	g_connect_out_inst.config_method = config_method;
	g_connect_out_inst.persistent = FALSE;	//FIXME hardcoded value
	g_connect_out_inst.auto_mode = FALSE;	// FIXME hardcoded value
	g_connect_out_inst.join = FALSE;	// FIXME hardcoded value
	g_connect_out_inst.auth = FALSE;	// FIXME hardcoded value
	g_connect_out_inst.provdisc = FALSE;	// FIXME hardcoded value

	memset(&g_connect_inst, 0, sizeof(p2p_fsm_connect_instance_t));
	strncpy((char *)g_connect_inst.device_connecting.device_address, (char *)device_address, SLSI_MACADDR_STR_LEN - 1);
	g_connect_inst.config_method = config_method;

	fsm_result_t res = slsi_p2p_fsm(P2P_EV_CONNECT, &g_connect_out_inst, &g_connect_inst);
	UNUSED(res);
	if (g_connect_out_inst.result == SLSI_STATUS_SUCCESS) {
		g_is_group_owner = FALSE;
		//wait on semaphore until we have a find result
		if (sem_wait(&g_connect_find_sem)) {
			DPRINT("sem_wait failed\n");
		}
		DPRINT("Semaphore unlocked - entering FSM to send P2P_CONNECT\n");
		res = slsi_p2p_fsm(P2P_EV_CONNECT, &g_connect_out_inst, &g_connect_inst);
	}
	LEAVE_CRITICAL;
	return g_connect_out_inst.result;
}

int8_t WiFiP2PDisconnect(void)
{
	p2p_fsm_generic_instance_t inst;
	inst.result = SLSI_STATUS_ERROR;
	fsm_result_t res = slsi_p2p_fsm(P2P_EV_DISCONNECT, &inst, NULL);
	UNUSED(res);
	return inst.result;
}

int8_t WiFiP2PRegisterConnectionHandler(slsi_p2p_connect_ind_t connect_handler)
{
	ENTER_CRITICAL;
	g_connect_handler = connect_handler;
#ifdef CONFIG_SCSC_WLAN_AUTO_RECOVERY
	g_recovery_data.connect_handler = connect_handler;
#endif
	LEAVE_CRITICAL;
	return SLSI_STATUS_SUCCESS;
}

int8_t WiFiP2PRegisterLinkCallback(slsi_network_link_callback_t link_up, slsi_network_link_callback_t link_down)
{
	ENTER_CRITICAL;
	g_p2p_link_up = link_up;
	g_p2p_link_down = link_down;
#ifdef CONFIG_SCSC_WLAN_AUTO_RECOVERY
	g_recovery_data.link_up = link_up;
	g_recovery_data.link_down = link_down;
#endif
	LEAVE_CRITICAL;

	return SLSI_STATUS_SUCCESS;
}

/*
 * Internal module - function definitions - declared in slsi_wifi_privat.h
 */
int8_t slsi_p2p_start(void)
{
	// TODO: include parameters from start API
	fsm_result_t res = slsi_p2p_fsm(P2P_EV_START, NULL, NULL);
	UNUSED(res);
	return SLSI_STATUS_SUCCESS;
}

int8_t slsi_p2p_stop(void)
{
	p2p_fsm_generic_instance_t inst;
	inst.result = SLSI_STATUS_ERROR;

	fsm_result_t res = slsi_p2p_fsm(P2P_EV_STOP, &inst, NULL);
	UNUSED(res);
	/* Destroy semaphores used */
	sem_post(&g_connect_find_sem);
	sem_destroy(&g_connect_find_sem);
	sem_post(&g_connect_neg_sem);
	sem_destroy(&g_connect_neg_sem);
	sem_post(&g_terminating);
	sem_destroy(&g_terminating);
	sem_post(&g_disconnect);
	sem_destroy(&g_disconnect);

	return inst.result;
}

void slsi_p2p_event_handle(const char *str)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_event_instance_t inst;

	VPRINT("P2P Event received: %s\n", str);
	if (slsi_event_received(str, P2P_EVENT_DEVICE_FOUND)) {
		event = P2P_EV_FOUND;
	}
	if (slsi_event_received(str, P2P_EVENT_FIND_STOPPED)) {
		event = P2P_EV_TIMEOUT;
	}
	if (slsi_event_received(str, P2P_EVENT_PROV_DISC_SHOW_PIN) || slsi_event_received(str, P2P_EVENT_PROV_DISC_ENTER_PIN) || slsi_event_received(str, P2P_EVENT_PROV_DISC_PBC_REQ) || slsi_event_received(str, P2P_EVENT_PROV_DISC_PBC_RESP)) {
		event = P2P_EV_PROV_DISC;
	}
	if (slsi_event_received(str, P2P_EVENT_GO_NEG_REQUEST)) {
		event = P2P_EV_NEG_REQ;
	}

	if (slsi_event_received(str, P2P_EVENT_GO_NEG_SUCCESS)) {
		event = P2P_EV_NEG_SUCCESS;
	}

	if (slsi_event_received(str, P2P_EVENT_GROUP_STARTED)) {
		event = P2P_EV_GROUP_STARTED;
	}

	if (slsi_event_received(str, P2P_EVENT_GROUP_REMOVED)) {
		event = P2P_EV_GROUP_REMOVED;
	}

	if (slsi_event_received(str, P2P_EVENT_GO_NEG_FAILURE)) {
		g_connect_failed_reason = SLSI_REASON_GO_NEG_FAILED;
		event = P2P_EV_CONNECT_FAIL;
	}

	if (slsi_event_received(str, P2P_EVENT_GROUP_FORMATION_FAILURE)) {
		g_connect_failed_reason = SLSI_REASON_GROUP_FORMATION_FAILED;
		event = P2P_EV_GROUP_REMOVED;
	}

	if (slsi_event_received(str, WPA_EVENT_CONNECTED) || slsi_event_received(str, AP_STA_CONNECTED)) {
		event = P2P_EV_CONNECTED;
	}
	if (slsi_event_received(str, WPA_EVENT_DISCONNECTED) || slsi_event_received(str, AP_STA_DISCONNECTED)) {
		event = P2P_EV_DISCONNECTED;
	}

	if (event != FSM_EVENT_NULL) {
		inst.event_str = str;
		fsm_result_t res = slsi_p2p_fsm(event, &inst, NULL);
		UNUSED(res);
	} else {
		DPRINT("Event not handled! \n");
	}

}

int8_t slsi_p2p_connect(char device_address[SLSI_MACADDR_STR_LEN], slsi_config_method_t config_method, char passphrase[SLSI_PASSPHRASE_LEN], uint8_t go_intent, bool persistent, bool join, bool auto_mode, bool auth, bool provdisc, bool reject)
{
	int8_t result = SLSI_STATUS_ERROR;
	char *arg1_str = WPA_EMPTY;	// config method or pin
	char *arg2_str = WPA_EMPTY;
	char *authjoin_str = WPA_EMPTY;
	char *automode_str = WPA_EMPTY;
	char *persist_str = WPA_EMPTY;
	char *provdisc_str = WPA_EMPTY;
	char *reject_str = WPA_EMPTY;
	char *delim = WPA_EMPTY;
	switch (config_method) {
	case SLSI_P2P_PBC:
		arg1_str = WPA_SPACE WPA_PARAM_PBC;
		break;
	case SLSI_P2P_DISPLAY:
		arg1_str = (char *)&passphrase[0];
		delim = WPA_SPACE;
		arg2_str = WPA_SPACE WPA_PARAM_DISPLAY;
		break;
	case SLSI_P2P_KEYPAD:
		arg1_str = (char *)&passphrase[0];
		delim = WPA_SPACE;
		arg2_str = WPA_SPACE WPA_PARAM_KEYPAD;
		break;
	default:
		EPRINT("Unsupported config method used (0x%02X)\n", config_method);
		return result;
		break;
	}
	if (auth) {
		authjoin_str = WPA_SPACE WPA_PARAM_AUTH;
	} else if (join) {
		authjoin_str = WPA_SPACE WPA_PARAM_JOIN;
	} else {
		authjoin_str = WPA_EMPTY;
	}
	if (auto_mode) {
		automode_str = WPA_SPACE WPA_PARAM_AUTO;
	} else {
		automode_str = WPA_EMPTY;
	}
	if (persistent) {
		persist_str = WPA_SPACE WPA_PARAM_PERSISTENT;
	} else {
		persist_str = WPA_EMPTY;
	}
	if (provdisc) {
		provdisc_str = WPA_SPACE WPA_PARAM_PROVDISC;
	} else {
		provdisc_str = WPA_EMPTY;
	}
	if (reject) {
		reject_str = WPA_SPACE WPA_PARAM_REJCT;
	} else {
		reject_str = WPA_EMPTY;
	}
	slsi_send_command_str(NULL, &result, "%s%s%s%s%s%s%s %s%d%s%s%s", WPA_COMMAND_P2P_CONNECT, device_address, delim, arg1_str, arg2_str, persist_str, authjoin_str, WPA_PARAM_GO_INTENT, go_intent, automode_str, provdisc_str, reject_str);
	return result;
}

int8_t slsi_p2p_flush(void)
{
	int8_t result = SLSI_STATUS_ERROR;
	DPRINT("Send P2P_FLUSH \n");
	slsi_send_command_str(NULL, &result, "%s", WPA_COMMAND_P2P_FLUSH);
	return result;
}

int8_t slsi_p2p_cancel(void)
{
	int8_t result = SLSI_STATUS_ERROR;
	DPRINT("Send P2P_CANCEL \n");
	slsi_send_command_str(NULL, &result, "%s", WPA_COMMAND_P2P_CANCEL);
	return result;
}

int8_t slsi_p2p_prov_disc(char device_address[SLSI_MACADDR_STR_LEN], slsi_config_method_t config_method, bool join, bool auto_mode)
{
	int8_t result = SLSI_STATUS_ERROR;
	DPRINT("Send P2P_PROV_DISC \n");
	char *method_str = WPA_EMPTY;
	char *join_str = WPA_EMPTY;
	char *automode_str = WPA_EMPTY;

	switch (config_method) {
	case SLSI_P2P_PBC:
		method_str = WPA_SPACE WPA_PARAM_PBC;
		break;
	case SLSI_P2P_DISPLAY:
		method_str = WPA_SPACE WPA_PARAM_DISPLAY;
		break;
	case SLSI_P2P_KEYPAD:
		method_str = WPA_SPACE WPA_PARAM_KEYPAD;
		break;
	default:
		EPRINT("Unsupported config method used (0x%02X)\n", config_method);
		return result;
		break;
	}

	if (join) {
		join_str = WPA_SPACE WPA_PARAM_JOIN;
	}
	if (auto_mode) {
		automode_str = WPA_SPACE WPA_PARAM_AUTO;
	}
	//p2p_prov_disc <peer device address> <display|keypad|pbc> [join|auto]
	slsi_send_command_str(NULL, &result, "%s%s%s%s%s", WPA_COMMAND_P2P_PROV_DISC, device_address, method_str, join_str, automode_str);
	return result;
}

int8_t slsi_p2p_set(char *field, char *value)
{
	int8_t result = SLSI_STATUS_ERROR;
	DPRINT("Send P2P_SET \n");
	//p2p_set <field> <value>
	slsi_send_command_str(NULL, &result, "%s%s %s", WPA_COMMAND_P2P_SET, field, value);
	return result;

}

int8_t slsi_p2p_peer(char device_address[SLSI_MACADDR_STR_LEN])
{
	int8_t result = SLSI_STATUS_ERROR;
	DPRINT("Send P2P_PEER \n");
	//p2p_peer <P2P Device Address>
	slsi_send_command_str(NULL, &result, "%s%s", WPA_COMMAND_P2P_PEER, device_address);
	return result;
}

/*
 * Local (static) P2P functions definitions
 */

static fsm_result_t slsi_p2p_fsm(fsm_event_t event, void *arg1, void *arg2)
{
	fsm32_state_t origstate = p2p_fsm_state;
	fsm_result_t res = FSM(&slsi_wifi_p2p_fsm, &p2p_fsm_state, event, arg1, arg2);
	DPRINT("##### FSM State change [%s => %s] via event %s #####\n", slsi_wifi_p2p_state_names[origstate], slsi_wifi_p2p_state_names[p2p_fsm_state], slsi_wifi_p2p_event_names[event]);
	UNUSED(origstate);			//needed when debug is not enabled
	return res;
}

static int8_t slsi_p2p_set_device_name(uint8_t *name, uint8_t name_len)
{
	int8_t result = SLSI_STATUS_ERROR;
	if (name && (name_len <= SLSI_SSID_LEN)) {
		DPRINT("Send SET device_name to: %s \n", name);
		/*set device_name <device name>
		   Set WPS Device Name (also included in some P2P messages). */
		slsi_send_command_str(NULL, &result, "%s%s %s", WPA_COMMAND_SET, WPA_PARAM_DEVICE_NAME, name);
	}
	return result;
}

static int8_t slsi_p2p_set_postfix(uint8_t *device_name, uint8_t device_name_len)
{
	int8_t result = SLSI_STATUS_ERROR;
	DPRINT("Send P2P_SET ssid_postfix \n");

	char full_postfix_name[SLSI_P2P_MAX_DEVICE_NAME_LEN + 2];	// make room for '-' and null
	memset(full_postfix_name, 0, SLSI_P2P_MAX_DEVICE_NAME_LEN + 2);
	uint8_t full_postfix_name_length = 0;
	uint8_t *tmp_postfix_name = NULL;
	uint8_t tmp_postfix_name_len = 0;
	uint8_t mac_name[4 + 2];
	memset(&mac_name, 0, 4 + 2);
	uint8_t postfix_name[device_name_len + 2];
	memset(&postfix_name, 0, device_name_len + 2);

	/* First add name from CONFIG if any */
	uint8_t config_postfix_length = strlen(SLSI_WIFI_P2P_POSTFIX);
	if (config_postfix_length > 0) {
		snprintf(full_postfix_name, SLSI_P2P_MAX_DEVICE_NAME_LEN + 2, "-%s", SLSI_WIFI_P2P_POSTFIX);
		/* Only store SLSI_P2P_MAX_DEVICE_NAME_LEN of characters in full name
		 * - ensure last character is null */
		full_postfix_name[SLSI_P2P_MAX_DEVICE_NAME_LEN + 1] = '\0';
	}

	if (config_postfix_length <= SLSI_P2P_MAX_DEVICE_NAME_LEN) {
		/* If more room in postfix then add received device name or MAC address digits */
		if (device_name) {
			tmp_postfix_name_len = device_name_len + 1;
			snprintf((char *)postfix_name, tmp_postfix_name_len + 1, "-%s", device_name);
			tmp_postfix_name = postfix_name;
		} else {
			uint8_t mac_value[SLSI_MACADDR_LEN];
			memset(mac_value, 0, SLSI_MACADDR_LEN);
			uint8_t mac_value5, mac_value6;
			if (slsi_get_mac(mac_value) == SLSI_STATUS_SUCCESS) {
				mac_value5 = mac_value[4];
				mac_value6 = mac_value[5];
				tmp_postfix_name_len = 4 + 1;
				snprintf((char *)mac_name, tmp_postfix_name_len + 1, "-%02x%02x", mac_value5, mac_value6);
				VPRINT("Last two uint8's 0x%02x 0x%02x\n", mac_value5, mac_value6);
				tmp_postfix_name = mac_name;
			} else {
				goto errout;
			}
		}

		if (tmp_postfix_name_len > 0) {
			full_postfix_name_length = tmp_postfix_name_len + config_postfix_length;
			if (full_postfix_name_length <= SLSI_P2P_MAX_DEVICE_NAME_LEN) {
				(void)strncat(full_postfix_name, (char *)tmp_postfix_name, (size_t) tmp_postfix_name_len);
			} else {
				uint8_t cat_size = SLSI_P2P_MAX_DEVICE_NAME_LEN - config_postfix_length;
				(void)strncat(full_postfix_name, (char *)tmp_postfix_name, (size_t) cat_size);
				/* Only store SLSI_P2P_MAX_DEVICE_NAME_LEN of characters in full name
				 * - ensure last character is null */
				full_postfix_name[SLSI_P2P_MAX_DEVICE_NAME_LEN + 1] = '\0';
				VPRINT("Concatenated device name cropped\n");
			}
		}
	} else {
		VPRINT("Device name cropped\n");
	}

	/*p2p_set ssid_postfix <postfix>
	   Set postfix string to be added to the automatically generated P2P SSID
	   (DIRECT-<two random characters>). For example, postfix of "-testing"
	   could result in the SSID becoming DIRECT-ab-testing. */
	VPRINT("Adding name: %s lenght: %d \n", full_postfix_name, full_postfix_name_length);
	slsi_send_command_str(NULL, &result, "%s%s %s", WPA_COMMAND_P2P_SET, WPA_PARAM_SSID_POSTFIX, full_postfix_name);

errout:
	return result;
}

static int8_t slsi_p2p_listen(uint8_t channel)
{
	int8_t result = SLSI_STATUS_ERROR;
	/*
	   p2p_listen channel
	 */
	if (channel == 1 || channel == 6 || channel == 11) {
		slsi_send_command_str(NULL, &result, "P2P_SET listen_channel %d", channel);
		DPRINT("Setting listening channel to %d\n", channel);
		if (result == SLSI_STATUS_SUCCESS) {
			slsi_send_command_str_upto_4(NULL, WPA_COMMAND_P2P_LISTEN, NULL, NULL, NULL, &result);
			if (result == SLSI_STATUS_SUCCESS) {
				DPRINT("listen initiated\n");
			}
		}
	} else if (channel == 0) {	// default is used
		result = SLSI_STATUS_SUCCESS;
	} else {
		DPRINT("Trying to set a wrong listen channel %d. Should be 1/6/11\n", channel);
		result = SLSI_STATUS_ERROR;
	}
	if (result == SLSI_STATUS_SUCCESS) {
		slsi_send_command_str_upto_4(NULL, WPA_COMMAND_P2P_LISTEN, NULL, NULL, NULL, &result);
		if (result == SLSI_STATUS_SUCCESS) {
			DPRINT("listen initiated\n");
		}
	}
	return result;
}

static int8_t slsi_p2p_find(uint32_t timeout, char *dev_addr)
{
	int8_t result = SLSI_STATUS_ERROR;
	/*
	   p2p_find [timeout in seconds] [type=<social|progressive>] \
	   [dev_id=<addr>] [dev_type=<device type>] \
	   [delay=<search delay in ms>] [seek=<service name>] [freq=<MHz>]
	 */
	if (timeout == 0) {
		if (dev_addr) {
			slsi_send_command_str_upto_4(NULL, WPA_COMMAND_P2P_FIND, WPA_PARAM_DEV_ID, (char *)dev_addr, NULL, &result);
		} else {
			slsi_send_command_str_upto_4(NULL, WPA_COMMAND_P2P_FIND, NULL, NULL, NULL, &result);
		}
	} else {
		if (dev_addr) {
			char command[WPA_COMMAND_MAX_SIZE] = { 0 };
			char *pbuf = NULL;
			memset(command, 0, WPA_COMMAND_MAX_SIZE);
			snprintf(command, WPA_COMMAND_MAX_SIZE, "%s%d %s%s", WPA_COMMAND_P2P_FIND, timeout, WPA_PARAM_DEV_ID, dev_addr);
			pbuf = slsi_send_request(NULL, command, &result);
			if (pbuf) {
				free(pbuf);
			}
		} else {
			slsi_send_command_str_digit(NULL, WPA_COMMAND_P2P_FIND, timeout, &result);
		}
	}
	if (result == SLSI_STATUS_SUCCESS) {
		DPRINT("find initiated - awaiting response\n");
	}
	return result;
}

static int8_t slsi_p2p_stop_find(void)
{
	int8_t result = SLSI_STATUS_ERROR;
	slsi_send_command_str_upto_4(NULL, WPA_COMMAND_P2P_STOP_FIND, NULL, NULL, NULL, &result);
	if (result == SLSI_STATUS_SUCCESS) {
		DPRINT("stop find/listen/connect\n");
	}
	return result;
}

static int8_t slsi_p2p_send_wps(slsi_config_method_t method, char *pin)
{
	int8_t result = SLSI_STATUS_SUCCESS;
	char *ifname;
	char *cmd = "";
	if (method == SLSI_P2P_PBC) {
		cmd = WPA_COMMAND_P2P_WPS_PBC;
		ifname = os_malloc(sizeof(CTRL_P2P_GROUP_IFNAME));
		os_memcpy(ifname, CTRL_P2P_GROUP_IFNAME, sizeof(CTRL_P2P_GROUP_IFNAME));
		slsi_send_command_str(ifname, &result, "%s %s", cmd, g_connect_inst.device_connecting.device_address);
	} else if (method == SLSI_P2P_KEYPAD) {
		cmd = WPA_COMMAND_P2P_WPS_PIN;
		slsi_send_command_str(NULL, &result, "%s%s %s", cmd, WPA_VALUE_P2P_WPS_ANY, pin);
	} else {
		result = SLSI_STATUS_ERROR;
	}
	return result;

}

static int8_t slsi_p2p_reject(char device_address[SLSI_MACADDR_STR_LEN])
{
	int8_t result = SLSI_STATUS_ERROR;
	slsi_send_command_str(NULL, &result, "%s%s", WPA_COMMAND_P2P_REJECT, device_address);
	return result;
}

static int8_t slsi_p2p_remove_group(char interface[CTRL_IFNAME_MAX_LEN])
{
	int8_t result = SLSI_STATUS_ERROR;
	slsi_send_command_str(NULL, &result, "%s%s", WPA_COMMAND_P2P_GROUP_REMOVE, interface);
	return result;
}

static int8_t slsi_p2p_group_add(uint8_t channel, bool persistent)
{
	int8_t result = SLSI_STATUS_NOT_STARTED;
	int localFreq = 0;
	// convert channel to frequency according to ieee
	char *persist_str = WPA_EMPTY;
	char ccode[3];
	memset(ccode, 0, 3);
	if (slsi_get_country_code(ccode) == SLSI_STATUS_SUCCESS) {
		localFreq = 2407 + 5 * channel;
		if (channel == 14) {
			localFreq = 2484;
		}
	}

	if (persistent) {
		persist_str = WPA_SPACE WPA_PARAM_PERSISTENT;
	}
	if (localFreq) {
		slsi_send_command_str(NULL, &result, "%s%s %s%d", WPA_COMMAND_P2P_GROUP_ADD, persist_str, WPA_VALUE_FREQ, localFreq);
	} else {
		slsi_send_command_str(NULL, &result, "%s%s", WPA_COMMAND_P2P_GROUP_ADD, persist_str);
	}

	return result;
}

static int8_t slsi_p2p_disconnect(void)
{
	int8_t result = SLSI_STATUS_ERROR;
	if (g_is_group_owner) {
		result = slsi_p2p_remove_group(CTRL_P2P_GROUP_IFNAME);
	} else {
		/* Disconnect */
		result = slsi_leave_network(strdup(CTRL_P2P_GROUP_IFNAME));

		if (result == SLSI_STATUS_SUCCESS) {
			VPRINT("Before wait sem count %d\n", g_sem_disconnect.semcount);
			int res = sem_wait(&g_disconnect);
			if (res) {
				EPRINT("g_sem_disconnect sem_wait error %d\n", errno);
			}
		} else {
			EPRINT("%s Error: disconnect. \n", __func__);
		}

	}
	return result;
}

static int8_t slsi_device_found(const char *str, slsi_peer_info_t *device)
{
	char *pos, *end;
	int8_t result = SLSI_STATUS_ERROR;
	/*
	   slsi_monitor_thread_handler: SLSI API DEBUG: SLSI_API slsi_monitor_thread_handler event -
	   <3>
	   P2P-DEVICE-FOUND c2:97:27:52:2a:5c
	   p2p_dev_addr=c2:97:27:52:2a:5c
	   pri_dev_type=7-0050F204-1
	   name='[TV] Samsung 7 Series (60)'
	   config_methods=0x188
	   dev_capab=0x25
	   group_capab=0x0
	   vendor_elems=1
	   new=1

	   <3>
	   P2P-DEVICE-FOUND 46:d4:e0:60:bc:16
	   p2p_dev_addr=46:d4:e0:60:bc:16
	   pri_dev_type=10-0050F204-5
	   name='Xperia Z3 Compact_3f03'
	   config_methods=0x188
	   dev_capab=0x25
	   group_capab=0x0
	   vendor_elems=1
	   new=1
	 */

	// Check state of network possibly already connected
	pos = strstr(str, WPA_PARAM_P2P_DEV_ADDR);
	if (pos != NULL) {
		pos += strlen(WPA_PARAM_P2P_DEV_ADDR);
		end = strchr(pos, ' ');
		if (end == NULL) {
			goto errout;
		}
		*end = '\0';
		strncpy(&(device->device_address[0]), pos, 17);
		pos += strlen(pos) + 1;
	} else {
		EPRINT("p2p device address not found\n");
		goto errout;
	}
	pos = strstr(pos, WPA_PARAM_PRI_DEV_TYPE);
	if (pos != NULL) {
		pos += strlen(WPA_PARAM_PRI_DEV_TYPE);
		end = strchr(pos, ' ');
		if (end == NULL) {
			goto errout;
		}
		*end = '\0';
		wps_dev_type_str2bin(pos, device->primary_device_type);
		pos += strlen(pos) + 1;
	} else {
		EPRINT("primary device type not found\n");
		goto errout;
	}
	pos = strstr(pos, WPA_PARAM_NAME);
	if (pos != NULL) {
		pos += strlen(WPA_PARAM_NAME) + 1;	// count starting '
		end = strchr(pos, '\'');	//Find ending '
		if (end == NULL) {
			goto errout;
		}
		*end = '\0';
		(void)printf_decode(device->device_name, SLSI_SSID_LEN + 1, pos);
		pos += strlen(pos) + 1;
	} else {
		EPRINT("name not found\n");
		goto errout;
	}
	pos = strstr(pos, WPA_PARAM_CONFIG_METHODS);
	if (pos != NULL) {
		uint16_t tmp_method;
		pos += strlen(WPA_PARAM_CONFIG_METHODS);
		end = strchr(pos, ' ');
		if (end == NULL) {
			goto errout;
		}
		*end = '\0';
		tmp_method = strtol(pos, &pos, 0);
		pos += strlen(pos) + 1;
		device->config_methods = 0;

		if (tmp_method == WPS_CONFIG_PUSHBUTTON) {
			device->config_methods = SLSI_P2P_PBC;
		}
		if (tmp_method == WPS_CONFIG_DISPLAY) {
			device->config_methods |= SLSI_P2P_DISPLAY;
		}
		if (tmp_method == WPS_CONFIG_KEYPAD) {
			device->config_methods |= SLSI_P2P_KEYPAD;
		}
		if (tmp_method == WPS_CONFIG_P2PS) {
			device->config_methods |= SLSI_P2P_P2PS;
		}

	} else {
		EPRINT("config method not found\n");
		goto errout;
	}
	pos = strstr(pos, WPA_PARAM_DEV_CAPAB);
	if (pos != NULL) {
		pos += strlen(WPA_PARAM_DEV_CAPAB);
		end = strchr(pos, ' ');
		if (end == NULL) {
			goto errout;
		}
		*end = '\0';
		device->device_capability = atoi(pos);
		pos += strlen(pos) + 1;
	} else {
		EPRINT("device capabilities not found\n");
		goto errout;
	}
	pos = strstr(pos, WPA_PARAM_GROUP_CAPAB);
	if (pos != NULL) {
		pos += strlen(WPA_PARAM_GROUP_CAPAB);
		end = strchr(pos, ' ');
		if (end) {			// Only in DEVICE-FOUND not PROV-DISC
			*end = '\0';
		}
		device->group_capability = atoi(pos);
	} else {
		EPRINT("group capabilities not found\n");
		goto errout;
	}
	result = SLSI_STATUS_SUCCESS;
errout:
	return result;
}

static int8_t slsi_config_method(const char *str, slsi_config_method_t *method, slsi_peer_info_t *device, char **passkey)
{
	int8_t result = SLSI_STATUS_ERROR;
	char *pos, *end;
	/*
	 * P2P-PROV-DISC-SHOW-PIN 96:b1:0a:cc:6b:ba 00279673
	 * or
	 * P2P-PROV-DISC-SHOW-PIN 46:d4:e0:60:bc:16 00133852
	 * p2p_dev_addr=46:d4:e0:60:bc:16
	 * pri_dev_type=10-0050F204-5
	 * name='Xperia Z3 Compact_3f03'
	 * config_methods=0x188
	 * dev_capab=0x25
	 * group_capab=0x0
	 *
	 * P2P-PROV-DISC-ENTER-PIN 96:b1:0a:cc:6b:ba
	 * or
	 * P2P-PROV-DISC-ENTER-PIN 46:d4:e0:60:bc:16
	 * p2p_dev_addr=46:d4:e0:60:bc:16
	 * pri_dev_type=10-0050F204-5
	 * name='Xperia Z3 Compact_3f03'
	 * config_methods=0x188
	 * dev_capab=0x25
	 * group_capab=0x0
	 *
	 * P2P-PROV-DISC-PBC-REQ 46:d4:e0:60:bc:16
	 * p2p_dev_addr=46:d4:e0:60:bc:16
	 * pri_dev_type=10-0050F204-5
	 * name='Xperia Z3 Compact_3f03'
	 * config_methods=0x188
	 * dev_capab=0x25
	 * group_capab=0x0
	 *
	 * P2P-PROV-DISC-PBC-RESP 96:b1:0a:cc:6b:ba
	 */
	VPRINT("%s\n", str);
	if (strstr(str, P2P_EVENT_PROV_DISC_SHOW_PIN) != NULL) {
		pos = strchr(str, ' ');
		if (pos == NULL) {
			EPRINT("parsing failed \n");
			goto errout;
		}
		pos += 1;
		// Find address
		end = strchr(pos, ' ');
		if (end == NULL) {
			EPRINT("parsing failed address\n");
			goto errout;
		}
		*end = '\0';
		memcpy(&device->device_address, pos, 17);
		pos += 17 + 1;
		/* Find pwd */
		if (strlen(pos) <= 12) {
			end = strchr(pos, '\0');
		} else {
			end = strchr(pos, ' ');
		}
		if (end == NULL) {
			EPRINT("parsing failed pin\n");
			goto errout;
		}
		size_t len = end - pos;
		memcpy(*passkey, pos, len);
		pos = strstr(pos, WPA_PARAM_NAME);
		if (pos != NULL) {
			pos += strlen(WPA_PARAM_NAME) + 1;	// count starting '
			end = strchr(pos, '\'');	//Find ending '
			if (end == NULL) {
				goto errout;
			}
			*end = '\0';
			(void)printf_decode(device->device_name, SLSI_SSID_LEN + 1, pos);
			pos += strlen(pos) + 1;
		} else {
			VPRINT("Might be outgoing connect then name not found\n");
		}
		*method = SLSI_P2P_DISPLAY;
	} else if (strstr(str, P2P_EVENT_PROV_DISC_ENTER_PIN) != NULL) {
		pos = strchr(str, ' ');
		if (pos == NULL) {
			EPRINT("parsing failed \n");
			goto errout;
		}
		pos += 1;
		// Find address
		if (strlen(pos) == 17) {
			end = strchr(pos, '\0');
		} else {
			end = strchr(pos, ' ');
		}
		if (end == NULL) {
			EPRINT("parsing failed address\n");
			goto errout;
		}
		memcpy(&device->device_address, pos, 17);
		pos = strstr(pos, WPA_PARAM_NAME);
		if (pos != NULL) {
			pos += strlen(WPA_PARAM_NAME) + 1;	// count starting '
			end = strchr(pos, '\'');	//Find ending '
			if (end == NULL) {
				goto errout;
			}
			*end = '\0';
			(void)printf_decode(device->device_name, SLSI_SSID_LEN + 1, pos);
			pos += strlen(pos) + 1;
		} else {
			VPRINT("Might be outgoing connect then name not found\n");
		}
		*method = SLSI_P2P_KEYPAD;
	} else if (strstr(str, P2P_EVENT_PROV_DISC_PBC_REQ) != NULL) {
		pos = strchr(str, ' ');
		if (pos == NULL) {
			EPRINT("parsing failed \n");
			goto errout;
		}
		pos += 1;
		// Find address
		memcpy(&device->device_address, pos, 17);
		pos = strstr(pos, WPA_PARAM_NAME);
		if (pos != NULL) {
			pos += strlen(WPA_PARAM_NAME) + 1;	// count starting '
			end = strchr(pos, '\'');	//Find ending '
			if (end == NULL) {
				goto errout;
			}
			*end = '\0';
			(void)printf_decode(device->device_name, SLSI_SSID_LEN + 1, pos);
			pos += strlen(pos) + 1;
		} else {
			EPRINT("name not found\n");
			goto errout;
		}
		*method = SLSI_P2P_PBC;
	} else if (strstr(str, P2P_EVENT_PROV_DISC_PBC_RESP) != NULL) {
		pos = strchr(str, ' ');
		if (pos == NULL) {
			EPRINT("parsing failed \n");
			goto errout;
		}
		pos += 1;
		// Find address
		memcpy(&device->device_address, pos, 17);
		*method = SLSI_P2P_PBC;
	} else {
		EPRINT("parsing failed PROV-DISC method unknown - should not happen\n");
		goto errout;
	}
	result = SLSI_STATUS_SUCCESS;
errout:
	return result;
}

/********************************************/
/*        fsm handler functions             */
/********************************************/

fsm_event_t p2p_fsm_handle_start(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	DPRINT("p2p_fsm_start_p2p\n");
	/* Ensure to initialize prefix for GO */

	(void)slsi_p2p_set_postfix(NULL, 0);
	/* Initialize semaphores used */
	sem_init(&g_connect_find_sem, 0, 0);
	sem_init(&g_connect_neg_sem, 0, 0);
	sem_init(&g_terminating, 0, 0);
	sem_init(&g_disconnect, 0, 0);
	return event;
}

fsm_event_t p2p_fsm_handle_stop(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;

	inst->result = SLSI_STATUS_SUCCESS;

	return event;
}

fsm_event_t p2p_fsm_handle_listen(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;

	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;
	inst->result = slsi_p2p_listen(0);
	DPRINT("result %d\n", inst->result);

	return event;
}

fsm_event_t p2p_fsm_handle_stop_listen(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;
	inst->result = slsi_p2p_stop_find();
	return event;
}

fsm_event_t p2p_fsm_handle_stop_find(void *arg1, void *arg2)
{
	/* expect to be called from FINDING state
	 * just send stop, change state to RUNNING and return NULL event
	 * */
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;
	inst->result = slsi_p2p_stop_find();
	return event;
}

fsm_event_t p2p_fsm_handle_find(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_find_instance_t *inst = (p2p_fsm_find_instance_t *) arg1;
	inst->result = slsi_p2p_find(inst->timeout, inst->device_address);
	return event;
}

fsm_event_t p2p_fsm_handle_found(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_event_instance_t *inst = (p2p_fsm_event_instance_t *) arg1;

	if (g_find_results_handler) {
		slsi_find_info_t found_info;
		memset(&found_info.peer_info, 0, sizeof(slsi_peer_info_t));
		if (slsi_device_found(inst->event_str, &found_info.peer_info) == SLSI_STATUS_SUCCESS) {
			found_info.reason_code = SLSI_STATUS_SUCCESS;
			slsi_msg_callback_t tmp;
			tmp.find_info = found_info;
			slsi_send_mqueue(SLSI_CALLBACK_P2P_FOUND, &tmp);
		} else {
			EPRINT("Device found failed\n");
		}
	}
	return event;
}

fsm_event_t p2p_fsm_handle_find_timeout(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	DPRINT("Find stopped by timeout!\n");
	slsi_find_info_t found_info = { 0 };
	found_info.reason_code = SLSI_REASON_TIMEOUT;
	if (g_find_results_handler != NULL) {
		slsi_msg_callback_t tmp;
		tmp.find_info = found_info;
		slsi_send_mqueue(SLSI_CALLBACK_P2P_FOUND, &tmp);
	}
	return event;
}

/* handlers for outgoing connect */
fsm_event_t p2p_fsm_handle_connect_find_init(void *arg1, void *arg2)
{
	/* Expects to be called from CONNECT_OUT state with the FIND event
	 * start the find and then return NULL event
	 * The flow will be continued by event coming from lower layers (found success/failure/timeout)
	 * */
	p2p_fsm_connect_out_instance_t *inst = (p2p_fsm_connect_out_instance_t *) arg1;
	p2p_fsm_connect_instance_t *inst2 = (p2p_fsm_connect_instance_t *) arg2;
	fsm_event_t event = FSM_EVENT_NULL;
	inst->result = slsi_p2p_find(inst->timeout, inst2->device_connecting.device_address);
	if (inst->result != SLSI_STATUS_SUCCESS) {
		event = P2P_EV_STOP;	// if we cannot even send the find request then we have to stop.
	}
	// if success then we return to initial caller and wait in a semaphore there until find results
	// come it via monitor thread.
	return event;
}

fsm_event_t p2p_fsm_handle_connect_find_fail(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	/**
	 * Expects to be called in CONNECT_OUT state from the upgoing event handler
	 * when a timeout has occured.
	 */
	slsi_reason_t reason = { 0 };
	reason.reason_code = g_connect_failed_reason;
	reason.locally_generated = 1;
	DPRINT("Find failed. Did not find the device\n");
	if (g_p2p_link_up != NULL) {
		slsi_msg_callback_t tmp;
		tmp.reason = reason;
		slsi_send_mqueue(SLSI_CALLBACK_P2P_LINK_UP, &tmp);
	}
	sem_post(&g_connect_find_sem);
	return event;
}

fsm_event_t p2p_fsm_handle_connect_find_success(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	DPRINT("Posting to Semaphore with value %d\n", g_connect_find_sem.semcount);
	sem_post(&g_connect_find_sem);
	return event;
}

fsm_event_t p2p_fsm_handle_connect_find_check(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	slsi_peer_info_t found_device;
	memset(&found_device, 0, sizeof(slsi_peer_info_t));
	p2p_fsm_event_instance_t *inst = (p2p_fsm_event_instance_t *) arg1;
	VPRINT("Found device\n");
	// found our device. Stop search and continue with the connect procedure
	if (slsi_device_found(inst->event_str, &found_device) == SLSI_STATUS_SUCCESS) {
		if (strncmp(g_connect_inst.device_connecting.device_address, found_device.device_address, SLSI_MACADDR_STR_LEN - 1) == 0) {
			DPRINT("Found the right device. Now lets stop the search and continue to connect\n");
			if (slsi_p2p_stop_find() != SLSI_STATUS_SUCCESS) {
				g_connect_failed_reason = SLSI_REASON_OPERATION_FAILED;
				event = P2P_EV_CONNECT_FAIL;
			} else {
				memset(&g_connect_inst.device_connecting, 0, sizeof(slsi_peer_info_t));
				memcpy(&g_connect_inst.device_connecting, &found_device, sizeof(slsi_peer_info_t));
				event = P2P_EV_FOUND_CORRECT;
			}
		} else {
			DPRINT("Found a wrong device %s (looking for %s). Will keep looking\n", found_device.device_address, g_connect_inst.device_connecting.device_address);
		}
	} else {
		g_connect_failed_reason = SLSI_REASON_NETWORK_CONFIGURATION_NOT_FOUND;
		event = P2P_EV_CONNECT_FAIL;
	}
	return event;
}

fsm_event_t p2p_fsm_handle_connect_connect(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	DPRINT("Sending actual connect\n");
	p2p_fsm_connect_out_instance_t *inst = (p2p_fsm_connect_out_instance_t *) arg1;
	p2p_fsm_connect_instance_t *inst2 = (p2p_fsm_connect_instance_t *) arg2;

	/* send down the actual connect request */
	inst->result = slsi_p2p_prov_disc(inst2->device_connecting.device_address, inst->config_method, inst->join, inst->auto_mode);
	return event;
}

fsm_event_t p2p_fsm_handle_connect_fail(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	DPRINT("Failed to connect\n");
	slsi_reason_t reason = { 0 };
	reason.reason_code = g_connect_failed_reason;
	reason.locally_generated = 1;
	if (g_p2p_link_up != NULL) {
		slsi_msg_callback_t tmp;
		tmp.reason = reason;
		slsi_send_mqueue(SLSI_CALLBACK_P2P_LINK_UP, &tmp);
	}

	return event;
}

fsm_event_t p2p_fsm_handle_connect_success(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	if (g_is_group_owner == FALSE) {
		slsi_reason_t reason = { 0 };
		DPRINT("Connect Success.\n");
		strncpy((char *)reason.ssid, (char *)g_connect_inst.device_connecting.device_name, SLSI_SSID_LEN);
		reason.ssid_len = strlen((char *)reason.ssid);
		strncpy((char *)reason.bssid, (char *)g_connect_inst.device_connecting.device_address, SLSI_MACADDR_STR_LEN - 1);

		DPRINT("ssid: %s bssid: %s length: %d\n", reason.ssid, reason.bssid, reason.ssid_len);
		// send linkup event
		if (g_p2p_link_up != NULL) {
			slsi_msg_callback_t tmp;
			tmp.reason = reason;
			slsi_send_mqueue(SLSI_CALLBACK_P2P_LINK_UP, &tmp);
		}
	} else {
		DPRINT("Ignore CTRL-EVENT-CONNECTED event as we are GO\n");
	}
	return event;
}

fsm_event_t p2p_fsm_handle_connected(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_event_instance_t *inst = (p2p_fsm_event_instance_t *) arg1;

	if (g_is_group_owner == FALSE) {
		//CTRL-EVENT-CONNECTED
		if (slsi_event_received(inst->event_str, WPA_EVENT_CONNECTED)) {
			event = P2P_EV_LINK_UP;
		} else {
			EPRINT("Ignore AP-STA-CONNECTED event as we are client - should not happen!\n");
		}
	} else {
		//AP-STA-CONNECTED 96:b1:0a:cc:6b:ba p2p_dev_addr=96:b1:0a:cc:6b:ba
		if (slsi_event_received(inst->event_str, AP_STA_CONNECTED)) {
			event = P2P_EV_LINK_UP;
		} else {
			VPRINT("Ignore CTRL-EVENT-CONNECTED event as we are GO\n");
		}
	}

	return event;
}

fsm_event_t p2p_fsm_handle_link_up(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;

	slsi_reason_t reason = { 0 };
	strncpy((char *)reason.ssid, (char *)g_connect_inst.device_connecting.device_name, SLSI_SSID_LEN);
	reason.ssid_len = strlen((char *)reason.ssid);
	strncpy((char *)reason.bssid, (char *)g_connect_inst.device_connecting.device_address, SLSI_MACADDR_STR_LEN - 1);

	DPRINT("device_address: %s name: %s length: %d\n", g_connect_inst.device_connecting.device_address, reason.ssid, reason.ssid_len);

	slsi_msg_callback_t tmp;
	tmp.reason = reason;
	slsi_send_mqueue(SLSI_CALLBACK_P2P_LINK_UP, &tmp);

	return event;
}

fsm_event_t p2p_fsm_handle_connect_in_fail(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	DPRINT("Failed to complete incoming connect\n");
	p2p_fsm_event_instance_t *inst = (p2p_fsm_event_instance_t *) arg1;

	slsi_reason_t reason = { 0 };
	if (g_connect_inst.response == SLSI_P2P_CONN_REJECT) {
		DPRINT("Ignore as we have rejected locally - return to find state\n");
	} else {
		// TODO don't know what how to set locally_generated to specify
		reason.locally_generated = 1;
		reason.reason_code = SLSI_REASON_CONNECT_ATTEMPT_FAILED;	// Default internal error
		/* Parse out error received */
		if (strstr(inst->event_str, P2P_EVENT_GO_NEG_FAILURE) != NULL) {
			reason.reason_code = SLSI_REASON_GO_NEG_FAILED;
		} else if (strstr(inst->event_str, P2P_EVENT_GROUP_FORMATION_FAILURE) != NULL) {
			reason.reason_code = SLSI_REASON_GROUP_FORMATION_FAILED;
		} else if (strstr(inst->event_str, WPS_EVENT_FAIL) != NULL) {
			reason.reason_code = SLSI_REASON_WPS_EVENT_FAILED;
		}

		if (g_connect_inst.device_connecting.device_name[0] != '\0') {
			strncpy((char *)reason.ssid, (char *)g_connect_inst.device_connecting.device_name, SLSI_SSID_LEN);
			reason.ssid_len = strlen((char *)reason.ssid);
		}
		if (g_connect_inst.device_connecting.device_address[0] != '\0') {
			strncpy((char *)reason.bssid, (char *)g_connect_inst.device_connecting.device_address, SLSI_MACADDR_STR_LEN - 1);
		}

		if (g_p2p_link_up != NULL) {
			slsi_msg_callback_t tmp;
			tmp.reason = reason;
			slsi_send_mqueue(SLSI_CALLBACK_P2P_LINK_UP, &tmp);
		}
	}

	return event;
}

/* handlers for incoming connect */
fsm_event_t p2p_fsm_handle_prov_disc(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_event_instance_t *inst = (p2p_fsm_event_instance_t *) arg1;

	/* New incoming request */
	char *tmp_pwd = g_connect_inst.passphrase;
	/* Parse config methods */
	if (slsi_config_method(inst->event_str, &g_connect_inst.config_method, &g_connect_inst.device_connecting, &tmp_pwd)
		!= SLSI_STATUS_SUCCESS) {
		g_connect_failed_reason = SLSI_REASON_PROV_DISC_FAILED;
		event = P2P_EV_CONNECT_FAIL;
	} else {
		DPRINT("device_name: %s device_address: %s config_method: %d\n", g_connect_inst.device_connecting.device_name, g_connect_inst.device_connecting.device_address, g_connect_inst.config_method);
		event = P2P_EV_CONTINUE;
	}
	return event;
}

fsm_event_t p2p_fsm_handle_prov_disc_in_cont(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;

	/* Call app callback for reply */
	char *tmpPwd = g_connect_inst.passphrase;
	g_connect_handler(&g_connect_inst.device_connecting, g_connect_inst.config_method, &g_connect_inst.response, &g_connect_inst.go_intent, &tmpPwd);

	if (g_connect_inst.response == SLSI_P2P_CONN_REJECT) {
		DPRINT("REJECTED from app\n");
	} else if (g_connect_inst.response == SLSI_P2P_CONN_ACCEPT) {
		DPRINT("connection ACCEPTED\n");
	}
	sem_post(&g_connect_neg_sem);
	return event;
}

fsm_event_t p2p_fsm_handle_prov_disc_cont(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;

	/* Call app callback for reply */
	char *tmpPwd = g_connect_inst.passphrase;

	g_connect_handler(&g_connect_inst.device_connecting, g_connect_inst.config_method, &g_connect_inst.response, &g_connect_inst.go_intent, &tmpPwd);

	if (g_connect_inst.response == SLSI_P2P_CONN_REJECT) {
		DPRINT("REJECTED from app\n");
		event = P2P_EV_REJECT;
	} else if (g_connect_inst.response == SLSI_P2P_CONN_ACCEPT) {
		DPRINT("connection ACCEPTED\n");
		event = P2P_EV_ACCEPT;
	}
	return event;
}

fsm_event_t p2p_fsm_handle_accept(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	int8_t result = slsi_p2p_connect(g_connect_inst.device_connecting.device_address,
									 g_connect_inst.config_method, g_connect_inst.passphrase,
									 g_connect_inst.go_intent, g_connect_out_inst.persistent,
									 g_connect_out_inst.join, g_connect_out_inst.auto_mode,
									 g_connect_out_inst.auth, g_connect_out_inst.provdisc,
									 FALSE);
	if (result == SLSI_STATUS_SUCCESS) {
		DPRINT("connect initiate successfully \n");
	} else {
		DPRINT("connect attempt failed \n");
		g_connect_failed_reason = SLSI_REASON_CONNECT_ATTEMPT_FAILED;
		event = P2P_EV_CONNECT_FAIL;
	}

	return event;
}

fsm_event_t p2p_fsm_handle_user_accept(void *arg1, void *arg2)
{
	//    p2p_fsm_event_instance_t *inst = (p2p_fsm_event_instance_t*)arg1;
	DPRINT("user accepted. Send down WPS command\n");
	/*TODO get args from inst->event_str */
	slsi_p2p_send_wps(/*FIXME right value? */ g_connect_inst.config_method,
			/*FIXME right value? */ &g_connect_inst.passphrase[0]);
	return FSM_EVENT_NULL;
}

fsm_event_t p2p_fsm_handle_user_reject(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	DPRINT("User rejected\n");
	return event;
}

fsm_event_t p2p_fsm_handle_neg_req(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_event_instance_t *inst = (p2p_fsm_event_instance_t *) arg1;
	VPRINT("Negotiation req\n");
	if (g_connect_handler) {
		char *pos, *end;
		/* P2P-GO-NEG-REQUEST 32:07:4d:1e:2c:b3
		 * dev_passwd_id=4
		 * go_intent=6
		 *
		 * P2P-GO-NEG-REQUEST 96:b1:0a:cc:6b:ba
		 * dev_passwd_id=1
		 * go_intent=6*/
		pos = strstr(inst->event_str, P2P_EVENT_GO_NEG_REQUEST);
		if (pos != NULL) {
			pos = strchr(pos, ' ');
			if (pos == NULL) {
				EPRINT("Parsing of PROV-DISC failed\n");
				event = P2P_EV_CONNECT_FAIL;
				goto errout;
			}
			pos += 1;
			end = strchr(pos, ' ');
			if (end == NULL) {
				EPRINT("Parsing of PROV-DISC failed\n");
				event = P2P_EV_CONNECT_FAIL;
				goto errout;
			}
			*end = '\0';
			if (strncmp(g_connect_inst.device_connecting.device_address, pos, SLSI_MACADDR_STR_LEN) != 0) {
				EPRINT("device not same as PROV-DISC\n");
				event = P2P_EV_CONNECT_FAIL;
				goto errout;
			}
		} else {
			EPRINT("p2p device address not found\n");
			event = P2P_EV_CONNECT_FAIL;
			goto errout;
		}

		/* Call app callback for reply */
		if (sem_wait(&g_connect_neg_sem)) {
			DPRINT("sem_wait failed\n");
		}
		if (g_connect_inst.response == SLSI_P2P_CONN_ACCEPT) {
			if (g_connect_inst.passphrase[0] != '\0') {
				DPRINT("ACCEPTED from app pwd: %s\n", g_connect_inst.passphrase);
			}
			int8_t result = slsi_p2p_connect(g_connect_inst.device_connecting.device_address,
											 g_connect_inst.config_method, g_connect_inst.passphrase, g_connect_inst.go_intent,
											 FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);
			if (result == SLSI_STATUS_SUCCESS) {
				DPRINT("connect initiate successfully \n");
			} else {
				EPRINT("connect attempt failed \n");
				event = P2P_EV_CONNECT_FAIL;
			}
		} else if (g_connect_inst.response == SLSI_P2P_CONN_REJECT) {
			DPRINT("REJECTED from app\n");
			int8_t result = slsi_p2p_connect(g_connect_inst.device_connecting.device_address,
											 g_connect_inst.config_method, g_connect_inst.passphrase, g_connect_inst.go_intent,
											 FALSE, FALSE, FALSE, FALSE, FALSE, TRUE);
			if (result == SLSI_STATUS_SUCCESS) {
				DPRINT("connection rejected successfully \n");
			} else {
				EPRINT("connect rejection failed \n");
			}
			event = P2P_EV_CONNECT_FAIL;
		}
	}
errout:
	g_connect_failed_reason = SLSI_REASON_GO_NEG_FAILED;
	return event;
}

fsm_event_t p2p_fsm_handle_neg_success(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_event_instance_t *inst = (p2p_fsm_event_instance_t *) arg1;

	char *pos, *end;
	/* P2P-GO-NEG-SUCCESS
	 * role=GO
	 * freq=2437
	 * ht40=0
	 * peer_dev=96:b1:0a:cc:6b:ba
	 * peer_iface=96:b1:0a:cc:6b:ba
	 * wps_method=PBC */
	pos = strstr(inst->event_str, P2P_EVENT_GO_NEG_SUCCESS);
	if (pos != NULL) {
		pos = strchr(pos, ' ');
		if (pos == NULL) {
			EPRINT("Parsing of P2P_EVENT_GO_NEG_SUCCESS failed\n");
			return P2P_EV_CONNECT_FAIL;
		}
		pos += 1;
		pos = strstr(pos, WPA_PARAM_ROLE);
		if (pos == NULL) {
			EPRINT("Parsing of WPA_PARAM_ROLE failed\n");
			return P2P_EV_CONNECT_FAIL;
		}
		pos += strlen(WPA_PARAM_ROLE);
		end = strchr(pos, ' ');
		if (end == NULL) {
			EPRINT("Parsing of P2P_EVENT_GROUP_STARTED failed - role\n");
			return P2P_EV_CONNECT_FAIL;
		}
		*end = '\0';
		if (strcmp(pos, WPA_VALUE_CLIENT) == 0) {
			g_is_group_owner = FALSE;
		} else if (strcmp(pos, WPA_VALUE_GO) == 0) {
			g_is_group_owner = TRUE;
		}
	} else {
		EPRINT("p2p device role not found\n");
	}
	DPRINT("group_owner=%d \n", g_is_group_owner);
	return event;
}

fsm_event_t p2p_fsm_handle_group_started(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;

	p2p_fsm_event_instance_t *inst = (p2p_fsm_event_instance_t *) arg1;
	char *pos, *end;
	/* P2P-GROUP-STARTED wl3 GO
	 * ssid="DIRECT-jE"
	 * freq=2437
	 * passphrase="UjAAYTAA"
	 * go_dev_addr=72:2c:1f:26:df:70
	 */
	/* P2P-GROUP-STARTED wl3 client
	 * ssid="DIRECT-Oz-Galaxy J3"
	 * freq=2437
	 * psk=d09e1f920746f4816287c82b1f81564a8969508b51d5c35c2aa70c37664f07cf
	 * go_dev_addr=96:b1:0a:cc:6b:ba
	 * ip_addr=192.168.49.200
	 * ip_mask=255.255.255.0
	 * go_ip_addr=192.168.49.1
	 */
	/* P2P-GROUP-STARTED wl3 client
	 * ssid="DIRECT-xwSM-G950F"
	 * freq=2437
	 * go_dev_addr=32:07:4d:9b:60:04
	 * ip_addr=192.168.49.200
	 * ip_mask=255.255.255.0
	 * go_ip_addr=192.168.49.1 */

	pos = strstr(inst->event_str, P2P_EVENT_GROUP_STARTED);
	if (pos != NULL) {
		DPRINT("Group started\n");
		pos = strchr(pos, ' ');
		if (pos == NULL) {
			EPRINT("Parsing of P2P_EVENT_GROUP_STARTED failed\n");
			event = P2P_EV_CONNECT_FAIL;
			goto errout;
		}
		pos += 1;
		/* extract interface name */
		end = strchr(pos, ' ');
		if (end == NULL) {
			EPRINT("Parsing of P2P_EVENT_GROUP_STARTED failed - ifname\n");
			event = P2P_EV_CONNECT_FAIL;
			goto errout;
		}
		*end = '\0';
		strncpy(&g_third_interface[0], pos, CTRL_IFNAME_MAX_LEN);
		pos += strlen(pos);
		pos += 1;
		end = strchr(pos, ' ');
		/* extract role */
		if (end == NULL) {
			EPRINT("Parsing of P2P_EVENT_GROUP_STARTED failed - role\n");
			event = P2P_EV_CONNECT_FAIL;
			goto errout;
		}
		*end = '\0';
		if (strcmp(pos, WPA_VALUE_CLIENT) == 0) {
			g_is_group_owner = FALSE;
		} else if (strcmp(pos, WPA_VALUE_GO) == 0) {
			g_is_group_owner = TRUE;
		}
		pos = end + 1;
		pos = strstr(pos, WPA_PARAM_SSID);
		if (pos != NULL) {
			pos += strlen(WPA_PARAM_SSID) + 1;	// count starting '
			end = strchr(pos, '"');	//Find ending '
			if (end == NULL) {
				EPRINT("name parsing error \n");
				event = P2P_EV_CONNECT_FAIL;
				goto errout;
			}
			*end = '\0';
			(void)printf_decode(g_connect_inst.device_connecting.device_name, SLSI_SSID_LEN + 1, pos);
		} else {
			EPRINT("name not found\n");
			event = P2P_EV_CONNECT_FAIL;
			goto errout;
		}
		DPRINT("ifname=%s group_owner=%d ssid=%s \n", g_third_interface, g_is_group_owner, g_connect_inst.device_connecting.device_name);

	} else {
		EPRINT("should not get here\n");
	}
	return P2P_EV_LINK_UP;
errout:
	g_connect_failed_reason = SLSI_REASON_GROUP_FORMATION_FAILED;
	return event;
}

fsm_event_t p2p_fsm_handle_remove_group(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;
	inst->result = slsi_p2p_remove_group(CTRL_P2P_GROUP_IFNAME);
	return event;
}

fsm_event_t p2p_fsm_handle_group_removed(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_event_instance_t *inst = (p2p_fsm_event_instance_t *) arg1;
	char *pos, *end;
	//P2P-GROUP-REMOVED wl3 client reason=IDLE
	//P2P-GROUP-REMOVED wl3 GO reason=REQUESTED

	pos = strstr(inst->event_str, P2P_EVENT_GROUP_REMOVED);
	if (pos != NULL) {
		DPRINT("group removed\n");
		pos = strchr(pos, ' ');
		if (pos == NULL) {
			EPRINT("Parsing of P2P_EVENT_GROUP_REMOVED failed\n");
			//TODO: do we need to use ERROR event
			goto errout;
		}
		pos += 1;
		end = strchr(pos, ' ');
		if (end == NULL) {
			EPRINT("Parsing of P2P_EVENT_GROUP_REMOVED failed - space\n");
			//TODO: do we need to use ERROR event
			goto errout;
		}
		*end = '\0';
		if (strncmp(g_third_interface, pos, strlen(g_third_interface)) == 0) {
			memset(g_third_interface, 0, CTRL_IFNAME_MAX_LEN);
		}

		if (g_is_group_owner) {
			g_is_group_owner = FALSE;
		}

	} else {
		EPRINT("should not get here\n");
	}
	return event;
errout:
	g_connect_failed_reason = SLSI_REASON_GROUP_FORMATION_FAILED;
	return P2P_EV_CONNECT_FAIL;

}

fsm_event_t p2p_fsm_handle_add_group(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_group_instance_t *inst = (p2p_fsm_group_instance_t *) arg1;
	DPRINT("Adding autonomous group\n");
	inst->result = slsi_p2p_group_add(inst->channel, inst->persistent);
	return event;
}

fsm_event_t p2p_fsm_handle_group_created(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	g_is_group_owner = TRUE;
	DPRINT("Group Created\n");
	return event;
}

/* handlers for disconnect */
fsm_event_t p2p_fsm_handle_disconnect(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;
	DPRINT("Disconnect network\n");
	inst->result = slsi_p2p_disconnect();
	return event;
}

fsm_event_t p2p_fsm_handle_disconnect_go(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;
	DPRINT("Disconnect not allowed - please remove group\n");
	inst->result = SLSI_STATUS_NOT_ALLOWED;
	return event;
}

fsm_event_t p2p_fsm_handle_disconnected(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	DPRINT("Remote disconnected\n");
	//CTRL-EVENT-DISCONNECTED bssid=72:2c:1f:26:5f:70 reason=3 locally_generated=1
	//AP-STA-DISCONNECTED 96:b1:0a:cc:6b:ba p2p_dev_addr=96:b1:0a:cc:6b:ba reason_code 1

	if (g_is_group_owner) {
		//event = P2P_EV_GO;
		(void)slsi_p2p_remove_group(CTRL_P2P_GROUP_IFNAME);
	}
	return event;
}

fsm_event_t p2p_fsm_handle_disconnected_go(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	DPRINT("Remote disconnected\n");
	if (g_p2p_link_down != NULL) {
		// TODO: Need to add reason code
		slsi_send_mqueue(SLSI_CALLBACK_P2P_LINK_DOWN, NULL);
	}
	return event;
}

fsm_event_t p2p_fsm_handle_link_down(void *arg1, void *arg2)
{
	fsm_event_t event = FSM_EVENT_NULL;
	DPRINT("Both disconnected and group is removed\n");

	sem_post(&g_disconnect);
	if (g_p2p_link_down != NULL) {
		// TODO: Need to add reason code
		slsi_send_mqueue(SLSI_CALLBACK_P2P_LINK_DOWN, NULL);
	}
	return event;
}

fsm_event_t p2p_fsm_handle_terminate_stop_find(void *arg1, void *arg2)
{
	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;
	DPRINT("stop find \n");
	inst->result = slsi_p2p_stop_find();
	DPRINT("result %d\n", inst->result);
	return P2P_EV_CANCEL;
}

fsm_event_t p2p_fsm_handle_terminate_remove(void *arg1, void *arg2)
{
	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;
	DPRINT("remove group \n");
	inst->result = slsi_p2p_remove_group(CTRL_P2P_GROUP_IFNAME);
	DPRINT("result %d\n", inst->result);
	return P2P_EV_CANCEL;
}

fsm_event_t p2p_fsm_handle_terminate_cancel(void *arg1, void *arg2)
{
	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;
	DPRINT("cancel p2p operations\n");
	inst->result = slsi_p2p_cancel();
	DPRINT("result %d\n", inst->result);
	return P2P_EV_CANCEL;
}

fsm_event_t p2p_fsm_handle_terminate_disconnect(void *arg1, void *arg2)
{
	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;
	DPRINT("disconnect \n");
	inst->result = slsi_p2p_disconnect();
	//if(sem_wait(&g_terminating)) DPRINT("sem_wait failed\n");
	DPRINT("result %d\n", inst->result);
	return FSM_EVENT_NULL;
}

fsm_event_t p2p_fsm_handle_terminate_disconnected(void *arg1, void *arg2)
{
	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;
	inst->result = SLSI_STATUS_SUCCESS;
	//sem_post(&g_terminating);
	DPRINT("post for semaphore\n");
	sem_post(&g_disconnect);
	DPRINT("result %d\n", inst->result);
	return FSM_EVENT_NULL;
}

fsm_event_t p2p_fsm_handle_terminate(void *arg1, void *arg2)
{
	p2p_fsm_generic_instance_t *inst = (p2p_fsm_generic_instance_t *) arg1;
	DPRINT("flush p2p handler\n");
	inst->result = slsi_p2p_flush();
	DPRINT("result %d\n", inst->result);
	return FSM_EVENT_NULL;
}

fsm_event_t p2p_fsm_handle_ignore(void *arg1, void *arg2)
{
	DPRINT("Event handled and ignored in this state\n");
	return FSM_EVENT_NULL;
}
#endif
