/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stdio.h>
#include "CSIParser.h"
#include "CSIManager.h"
#include "PingGenerator.h"
#include "CSINetworkMonitor.h"
#include "CSIPacketReceiver.h"

static csifw_context_t *g_pcsifw_context = NULL;
static pthread_mutex_t g_api_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Private functions */
static int get_empty_idx(void);
static int all_services_stopped(void);
static void destroy_csifw_context(void);
static int get_service_idx(unsigned int id);
static void nw_state_notify_service(CONNECTION_STATE state);
static CSIFW_RES check_hnd_validity(csifw_service_handle hnd);
static void csifw_update_state_and_log(CSI_FRAMEWORK_STATE new_state);
static CSIFW_RES remove_service(csifw_context_t *p_csifw_ctx, csifw_service_info_t *p_svc_info);
static csifw_service_info_t *add_service(csifw_context_t *p_csifw_ctx, service_callbacks_t *p_svc_cb);

static const char *csi_framework_state_strings[] = 
{
    "UNINITIALIZED",                    /* CSI_FRAMEWORK_STATE_UNINITIALIZED */
    "STARTED",                          /* CSI_FRAMEWORK_STATE_STARTED */
    "STARTED_WAITING_FOR_NW",           /* CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW */
    "STOPPED",                          /* CSI_FRAMEWORK_STATE_STOPPED */
    "INITIALIZED",                      /* CSI_FRAMEWORK_STATE_INITIALIZED */
    "STARTED_WAITING_FOR_NW_RECONNECT"  /* CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW_RECONNECT */
};

static void csifw_update_state_and_log(CSI_FRAMEWORK_STATE new_state) 
{
  if (!g_pcsifw_context) {
    return;
  }
  const int max_state = sizeof(csi_framework_state_strings) / sizeof(csi_framework_state_strings[0]);
  if (new_state < CSI_FRAMEWORK_STATE_UNINITIALIZED || new_state >= max_state) {
    CSIFW_LOGE("Invalid state value: %d", new_state);
    return;
  }

  CSI_FRAMEWORK_STATE prev_state = g_pcsifw_context->csifw_state;
  g_pcsifw_context->csifw_state = new_state;

  const char *prev_state_str = (prev_state == CSI_FRAMEWORK_STATE_UNINITIALIZED) ? "UNINITIALIZED" : csi_framework_state_strings[prev_state + 1];
  const char *new_state_str = csi_framework_state_strings[new_state + 1];
  CSIFW_LOGI("CSIFW state changed from %s to %s", prev_state_str, new_state_str);
}

static void csi_network_state_listener(CONNECTION_STATE state)
{
  if (!g_pcsifw_context) {
    return;
  }
  CSIFW_MUTEX_LOCK(&g_api_mutex);
  CSIFW_LOGD("Network Status: %s", state ? "disconnected" : "connected");
  if (state == WIFI_DISCONNECTED) {
    if (g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STARTED) {
      CSIFW_LOGD("Stopping CSI Service due to WiFi disconnect");
      g_pcsifw_context->disable_required = false;
      if (stop_csi_framework(g_pcsifw_context) != 0) {
        CSIFW_LOGE("CSI Stop Service failed");
      }
      csifw_update_state_and_log(CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW_RECONNECT);
    }
  } else if (state == WIFI_CONNECTED) {
    if (g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW_RECONNECT) {
      g_pcsifw_context->disable_required = true;
    }
    if (g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW ||
        g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW_RECONNECT) {
      CSIFW_LOGD("Starting service on wifi reconnection");
      if (start_csi_framework(g_pcsifw_context) != 0) {
        CSIFW_LOGE("CSI Start service failed");
      }
      // The state is set to STARTED even if start_csi_framework fails
      // Because csi_network_state_listener is called only when network state
      // changes. If State is set to Stopped or Waiting_NW then Application will
      // never STOP FW properly. After Wifi Connect, if Data is not comming,
      // then Application has to Stop -> Start CSIFW.
      csifw_update_state_and_log(CSI_FRAMEWORK_STATE_STARTED);
    }
  }
  nw_state_notify_service(state);
  CSIFW_MUTEX_UNLOCK(&g_api_mutex);
}

static void CSIRawDataListener(CSIFW_RES res, int raw_csi_buff_len, unsigned char *raw_csi_buff, int raw_csi_data_len) 
{
  if (!g_pcsifw_context) {
    return;
  }

  if (g_pcsifw_context->parsed_data_cb_count > 0 && g_pcsifw_context->parsed_data_cb_started_count > 0) {
    getParsedData(raw_csi_buff, raw_csi_buff_len, 
					g_pcsifw_context->csi_config,
					g_pcsifw_context->parsed_buffptr,
					&g_pcsifw_context->ParsedDataBufferLen);
  }

  // we use this service state but if its called parelly this is will stuck
  CSIFW_MUTEX_LOCK(&g_pcsifw_context->data_reciever_mutex);
  for (int i = 0; i < CSIFW_MAX_NUM_APPS; i++) {
    // send raw
    if (g_pcsifw_context->csi_services[i].svc_id != 0 && g_pcsifw_context->csi_services[i].svc_state == CSI_SERVICE_START) {
      if (g_pcsifw_context->csi_services[i].raw_data_cb) {
        g_pcsifw_context->csi_services[i].raw_data_cb(res,raw_csi_buff_len, raw_csi_buff, g_pcsifw_context->csi_services[i].service_data);
      }
      // send parsed
      if (g_pcsifw_context->csi_services[i].parsed_data_cb) {
        g_pcsifw_context->csi_services[i].parsed_data_cb(res,
														g_pcsifw_context->ParsedDataBufferLen,
														g_pcsifw_context->parsed_buffptr,
														g_pcsifw_context->csi_services[i].service_data);
      }
    }
  }
  CSIFW_MUTEX_UNLOCK(&g_pcsifw_context->data_reciever_mutex);
}

CSIFW_RES csifw_registerService(csifw_service_handle *p_hnd, service_callbacks_t *p_svc_cb, csi_config_type_t config_type, unsigned int interval) 
{
  CSIFW_RES res = CSIFW_OK;
  csifw_service_info_t *p_svc_info = NULL;
  CSIFW_MUTEX_LOCK(&g_api_mutex);

  if (!p_hnd) {
    CSIFW_LOGE("Invalid handle argument, should not be a null pointer");
    res = CSIFW_INVALID_ARG;
    goto on_error;
  }

  if (!g_pcsifw_context) {
    CSIFW_LOGD("Allocating new CSIFW_Context");
    g_pcsifw_context = (csifw_context_t *)calloc(1, sizeof(csifw_context_t));
    if (!g_pcsifw_context) {
      CSIFW_LOGE("CSIFW_Context Memory allocation Failed");
      res = CSIFW_NO_MEM;
      goto on_error;
    }
    CSIFW_LOGD("Context Created Successfully");
    if (pthread_mutex_init(&g_pcsifw_context->data_reciever_mutex, NULL) != 0) {
      CSIFW_LOGE("Mutex init failed of data_reciever_mutex");
      destroy_csifw_context();
      res = CSIFW_ERROR;
      goto on_error;
    }
    g_pcsifw_context->csifw_state = CSI_FRAMEWORK_STATE_UNINITIALIZED;
  }

  if (g_pcsifw_context->service_count >= CSIFW_MAX_NUM_APPS) {
    CSIFW_LOGE("MAX NUM SERVICES ALREADY REGISTERED");
    res = CSIFW_ERROR_MAX_NUM_SERVICE_REGISTERED;
    goto on_error;
  }

  if (g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_UNINITIALIZED) {
    if (config_type <= MIN_CSI_CONFIG_TYPE || config_type >= MAX_CSI_CONFIG_TYPE || interval < CSIFW_MIN_INTERVAL_MS) {
      CSIFW_LOGE("Either invalid CSI config type: %d. (Valid config range: %d-%d)", config_type, HT_CSI_DATA, NON_HT_CSI_DATA_ACC1);
      CSIFW_LOGE("Or invalid interval: %d ms. (Minimum allowed: %d ms)", interval, CSIFW_MIN_INTERVAL_MS);
      destroy_csifw_context();
      res = CSIFW_INVALID_ARG;
      goto on_error;
    }

    g_pcsifw_context->csi_config = config_type;
    g_pcsifw_context->csi_interval = interval;
    if (config_type == HT_CSI_DATA_ACC1 || config_type == HT_CSI_DATA) {
      ping_generator_change_interval(g_pcsifw_context->csi_interval);
    } else {
      ping_generator_change_interval(0);
    }

    csi_packet_callback_register(CSIRawDataListener);
    if (!network_monitor_init(csi_network_state_listener)) {
      CSIFW_LOGE("Network_Monitor_Init Failed");
      destroy_csifw_context();
      res = CSIFW_ERROR;
      goto on_error;
    }
  } else {
    if (g_pcsifw_context->csi_config != config_type || g_pcsifw_context->csi_interval != interval) {
      CSIFW_LOGE("Service already inited with different configuration/interval."
          		 "Cannot register service with different configuration/interval.");
      res = CSIFW_ERROR_ALREADY_INIT_WITH_DIFFERENT_CONFIG;
      goto on_error;
    }
  }

  p_svc_info = add_service(g_pcsifw_context, p_svc_cb);
  if (!p_svc_info) {
    if (g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_UNINITIALIZED) {
      CSIFW_LOGE("Created context but failed to allocate memory.");
      destroy_csifw_context();
    }
    res = CSIFW_ERROR_MAX_NUM_SERVICE_REGISTERED;
    goto on_error;
  }

  *p_hnd = (csifw_service_handle)p_svc_info;

  if (g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_UNINITIALIZED) {
    csifw_update_state_and_log(CSI_FRAMEWORK_STATE_INITIALIZED);
    CSIFW_LOGI("CSIFW Service Registered Successfully.");
  }

on_error:
  CSIFW_MUTEX_UNLOCK(&g_api_mutex);
  return res;
}

CSIFW_RES csifw_start(csifw_service_handle hnd) 
{
  CSIFW_RES res = CSIFW_OK;
  csifw_service_info_t *p_svc_info = NULL;
  CSIFW_MUTEX_LOCK(&g_api_mutex);

  res = check_hnd_validity(hnd);
  if (res != CSIFW_OK) {
    goto on_error;
  }

  p_svc_info = (csifw_service_info_t *)hnd;

  if (g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW ||
      g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW_RECONNECT) {
    CSIFW_LOGI("Waiting for NW, CSI data will start when network will be connected");
    p_svc_info->svc_state = CSI_SERVICE_START;
    res = CSIFW_OK_WIFI_NOT_CONNECTED;
    goto on_error;
  }

  if (p_svc_info->svc_state == CSI_SERVICE_START) {
    CSIFW_LOGI("CSIFW Already Started");
    res = CSIFW_OK_ALREADY_STARTED;
    goto on_error;
  }

  if (g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STOPPED ||
      g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_INITIALIZED) {
    if (checkWifiConnection()) {
      if (start_csi_framework(g_pcsifw_context) != 0) {
        res = CSIFW_ERROR;
        goto on_error;
      }
      csifw_update_state_and_log(CSI_FRAMEWORK_STATE_STARTED);
    } else {
      csifw_update_state_and_log(CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW);
      CSIFW_LOGE("Waiting for NW, CSI data will start when network will be connected");
    }
  }

  CSIFW_MUTEX_LOCK(&g_pcsifw_context->data_reciever_mutex);
  p_svc_info->svc_state = CSI_SERVICE_START;
  if (p_svc_info->parsed_data_cb) {
    g_pcsifw_context->parsed_data_cb_started_count++;
  }
  CSIFW_MUTEX_UNLOCK(&g_pcsifw_context->data_reciever_mutex);
  CSIFW_LOGI("Service [%d] started successfully, services count is %d", p_svc_info->svc_id, g_pcsifw_context->service_count);

on_error:
  CSIFW_MUTEX_UNLOCK(&g_api_mutex);
  return res;
}

CSIFW_RES csifw_stop(csifw_service_handle hnd) 
{
  CSIFW_RES res = CSIFW_OK;
  csifw_service_info_t *p_svc_info = NULL;
  CSIFW_MUTEX_LOCK(&g_api_mutex);

  res = check_hnd_validity(hnd);
  if (res != CSIFW_OK) {
    goto on_error;
  }

  p_svc_info = (csifw_service_info_t *)hnd;

  if (g_pcsifw_context->csifw_state != CSI_FRAMEWORK_STATE_STARTED &&
      g_pcsifw_context->csifw_state != CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW &&
      g_pcsifw_context->csifw_state != CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW_RECONNECT) {
    CSIFW_LOGI("CSIFW service already stopped");
    res = CSIFW_OK_ALREADY_STOPPED;
    goto on_error;
  }

  CSIFW_MUTEX_LOCK(&g_pcsifw_context->data_reciever_mutex);
  p_svc_info->svc_state = CSI_SERVICE_STOP;
  if (p_svc_info->parsed_data_cb) {
    g_pcsifw_context->parsed_data_cb_started_count--;
  }
  CSIFW_MUTEX_UNLOCK(&g_pcsifw_context->data_reciever_mutex);

  if (!all_services_stopped()) {
    goto on_error;
  }

  // In case of waiting for Network no stopping required only state change to Stopped
  if (g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STARTED ||
      g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW_RECONNECT) {
    g_pcsifw_context->disable_required = true;
    if (g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STARTED) {
      stop_csi_framework(g_pcsifw_context);
    }
  }
  csifw_update_state_and_log(CSI_FRAMEWORK_STATE_STOPPED);
  CSIFW_LOGI("Service [%d] stopped successfully, remaining services count is %d", p_svc_info->svc_id, g_pcsifw_context->service_count);

on_error:
  CSIFW_MUTEX_UNLOCK(&g_api_mutex);
  return res;
}

CSIFW_RES csifw_unregisterService(csifw_service_handle hnd) 
{
  CSIFW_RES res = CSIFW_OK;
  csifw_service_info_t *p_svc_info = NULL;
  CSIFW_MUTEX_LOCK(&g_api_mutex);

  res = check_hnd_validity(hnd);
  if (res != CSIFW_OK) {
    goto on_error;
  }

  p_svc_info = (csifw_service_info_t *)hnd;
  if (p_svc_info->svc_state == CSI_SERVICE_START) {
    CSIFW_LOGE("Un-Register called in service Start state, call stop() first");
    res = CSIFW_ERROR_INVALID_SERVICE_STATE;
    goto on_error;
  }

  remove_service(g_pcsifw_context, p_svc_info);
  if (g_pcsifw_context->service_count == 0 && g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STOPPED) {
    CSIFW_LOGD("No service left, destroying framework context");
    res = csi_packet_receiver_deinit();
    if (res != CSIFW_OK) {
      CSIFW_LOGE("Packet Receiver deinit failed");
      res = CSIFW_ERROR;
    }
    if (!network_monitor_deinit()) {
      CSIFW_LOGE("Network monitor deinit failed");
      res = CSIFW_ERROR;
    }
    pthread_mutex_destroy(&g_pcsifw_context->data_reciever_mutex);
    destroy_csifw_context();
  }
  CSIFW_LOGI("CSIFW Service Un-Registered Successfully.");

on_error:
  CSIFW_MUTEX_UNLOCK(&g_api_mutex);
  return res;
}

CSIFW_RES csifw_set_interval(csifw_service_handle hnd, unsigned int interval) 
{
  CSIFW_RES res = CSIFW_OK;
  CSIFW_MUTEX_LOCK(&g_api_mutex);

  res = check_hnd_validity(hnd);
  if (res != CSIFW_OK) {
    goto on_error;
  }

  if (interval < CSIFW_MIN_INTERVAL_MS) {
    CSIFW_LOGE("Invalid interval %d ms. Minimum allowed: %d ms", interval, CSIFW_MIN_INTERVAL_MS);
    res = CSIFW_INVALID_ARG;
    goto on_error;
  }

  if (g_pcsifw_context->csi_interval == interval) {
    CSIFW_LOGI("Interval unchanged (current: %u)", g_pcsifw_context->csi_interval);
    goto on_error;
  }

  g_pcsifw_context->csi_interval = interval;
  if (g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STOPPED || g_pcsifw_context->csifw_state == CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW) {
    CSIFW_LOGI("Interval updated: %u", g_pcsifw_context->csi_interval);
    goto on_error;
  }

  if (g_pcsifw_context->csi_config == HT_CSI_DATA || g_pcsifw_context->csi_config == HT_CSI_DATA_ACC1) {
    ping_generator_change_interval(g_pcsifw_context->csi_interval);
  } else {
    res = csi_packet_receiver_change_interval();
  }
  CSIFW_LOGD("Interval updated : %u", g_pcsifw_context->csi_interval);

on_error:
  CSIFW_MUTEX_UNLOCK(&g_api_mutex);
  return res;
}

CSIFW_RES csifw_get_interval(csifw_service_handle hnd, unsigned int *p_interval) 
{
  CSIFW_RES res = CSIFW_OK;
  CSIFW_MUTEX_LOCK(&g_api_mutex);

  res = check_hnd_validity(hnd);
  if (res != CSIFW_OK) {
    goto on_error;
  }

  if (!p_interval) {
    CSIFW_LOGE("CSIFW Interval: Null argument passed");
    res = CSIFW_INVALID_ARG;
    goto on_error;
  }

  *p_interval = g_pcsifw_context->csi_interval;
  CSIFW_LOGD("Current Interval is :%d", g_pcsifw_context->csi_interval);

on_error:
  CSIFW_MUTEX_UNLOCK(&g_api_mutex);
  return res;
}

CSIFW_RES csifw_get_config(csifw_service_handle hnd, csi_config_type_t *config_type) 
{
  CSIFW_RES res = CSIFW_OK;
  CSIFW_MUTEX_LOCK(&g_api_mutex);

  res = check_hnd_validity(hnd);
  if (res != CSIFW_OK) {
    goto on_error;
  }

  if (!config_type) {
    CSIFW_LOGE("CSIFW Configuration: Null argument passed");
    res = CSIFW_INVALID_ARG;
    goto on_error;
  }

  *config_type = g_pcsifw_context->csi_config;
  CSIFW_LOGD("Current Config is :%d", g_pcsifw_context->csi_config);

on_error:
  CSIFW_MUTEX_UNLOCK(&g_api_mutex);
  return res;
}

CSIFW_RES csifw_get_ap_mac_addr(csifw_service_handle hnd, csifw_mac_info *p_mac_info) 
{
  CSIFW_RES res = CSIFW_OK;
  csifw_service_info_t *p_svc_info = NULL;
  CSIFW_MUTEX_LOCK(&g_api_mutex);

  res = check_hnd_validity(hnd);
  if (res != CSIFW_OK) {
    goto on_error;
  }

  if (!p_mac_info) {
    CSIFW_LOGE("CSIFW MAC_INFO: Null argument passed");
    res = CSIFW_INVALID_ARG;
    goto on_error;
  }

  p_svc_info = (csifw_service_info_t *)hnd;
  if (g_pcsifw_context->csifw_state != CSI_FRAMEWORK_STATE_STARTED && p_svc_info->svc_state != CSI_SERVICE_START) {
    CSIFW_LOGE("CSIFW should be started to get AP MAC.");
    res = CSIFW_ERROR_INVALID_SERVICE_STATE;
    goto on_error;
  }

  res = csi_packet_receiver_get_mac_addr(p_mac_info);

on_error:
  CSIFW_MUTEX_UNLOCK(&g_api_mutex);
  return res;
}

CSIFW_RES csifw_force_restart(csifw_service_handle hnd) 
{
  // THIS SERVICE WILL FORCE RESTART THE THREADS TO GET DATA ANYHOW
  // ONLY CHECHK IS INIT = TRUE.
  CSIFW_RES res = CSIFW_ERROR;
  return res; // NOT YET IMPLEMENTED
}

/* service handling functions */

static int get_service_idx(unsigned int id) 
{
  if ((!g_pcsifw_context) || (g_pcsifw_context->service_count == 0)) {
    return -1;
  }
  for (int i = 0; i < CSIFW_MAX_NUM_APPS; i++) {
    if (g_pcsifw_context->csi_services[i].svc_id == id) {
      return i;
    }
  }
  return -1;
}

static int get_empty_idx(void) 
{
  if (!g_pcsifw_context) {
    return CSIFW_ERROR_NOT_INITIALIZED;
  }

  for (int i = 0; i < CSIFW_MAX_NUM_APPS; i++) {
    if (g_pcsifw_context->csi_services[i].svc_id == 0) {
      return i;
    }
  }
  return -1;
}

/**
 * @brief Register a CSI service with the CSI service.
 *
 * @param csifw_context_t Callback function to receive csifw_context_t handle.
 * @param service_callbacks_t Callback functions to receive raw/parsed CSI data.
 *
 * @return Index of register service on success, -1 in case of failure.
 */
csifw_service_info_t *add_service(csifw_context_t *p_csifw_ctx, service_callbacks_t *p_svc_cb) 
{
  unsigned int cid = 0;
  int idx = get_empty_idx();
  if (idx < 0) {
    CSIFW_LOGE("Max Number of services already registered");
    return NULL;  // invalid id
  }

  if (!p_svc_cb->raw_data_cb && !p_svc_cb->parsed_data_cb) {
    CSIFW_LOGE("Invalid service register request, both Raw Data CB and Parse Data CB are NULL");
    return NULL;
  }

  if (p_svc_cb->parsed_data_cb) {
    cid = (unsigned int)p_svc_cb->parsed_data_cb;
    if (!p_csifw_ctx->parsed_buffptr) {
      p_csifw_ctx->ParsedDataBufferLen = CSIFW_MAX_RAW_BUFF_LEN;
      p_csifw_ctx->parsed_buffptr = (float *)malloc(sizeof(float) * p_csifw_ctx->ParsedDataBufferLen);
      if (!p_csifw_ctx->parsed_buffptr) {
        CSIFW_LOGE("Failed to allocate memory for parsed data buffer, size: %zu", sizeof(float) * p_csifw_ctx->ParsedDataBufferLen);
        return NULL;
      }
    }
  }
  if (p_svc_cb->raw_data_cb) {
    cid = (unsigned int)p_svc_cb->raw_data_cb;
  }

  if (get_service_idx(cid) != -1) {
    if (!p_csifw_ctx->parsed_buffptr) {
      free(p_csifw_ctx->parsed_buffptr);
      p_csifw_ctx->parsed_buffptr = NULL;
    }
    CSIFW_LOGE("Service with ID: %d already registered", cid);
    return NULL;
  }

  p_csifw_ctx->csi_services[idx].svc_id = cid;
  p_csifw_ctx->csi_services[idx].svc_state = CSI_SERVICE_REGISTERED;
  p_csifw_ctx->csi_services[idx].raw_data_cb = p_svc_cb->raw_data_cb;
  p_csifw_ctx->csi_services[idx].parsed_data_cb = p_svc_cb->parsed_data_cb;
  if (p_svc_cb->parsed_data_cb) {
    p_csifw_ctx->parsed_data_cb_count++;
  }
  p_csifw_ctx->csi_services[idx].service_data = p_svc_cb->user_data;
  p_csifw_ctx->service_count++;

  CSIFW_LOGI("Idx %d: Service(%d) registered with CSIFW. Total services: %d", idx, cid, p_csifw_ctx->service_count);
  return &p_csifw_ctx->csi_services[idx];
}

static CSIFW_RES remove_service(csifw_context_t *p_csifw_ctx, csifw_service_info_t *p_svc_info) 
{
  int idx = get_service_idx(p_svc_info->svc_id);
  if (idx == -1) {
    CSIFW_LOGE("CSI Service not registered [%d].", p_svc_info->svc_id);
    return CSIFW_ERROR_SERVICE_NOT_REGISTERED;
  }

  if (p_csifw_ctx->csi_services[idx].parsed_data_cb) {
    p_csifw_ctx->parsed_data_cb_count--;
  }

  if ((p_csifw_ctx->parsed_buffptr) &&
      (p_csifw_ctx->parsed_data_cb_count <= 0)) {
    p_csifw_ctx->ParsedDataBufferLen = 0;
    free(p_csifw_ctx->parsed_buffptr);
    p_csifw_ctx->parsed_buffptr = NULL;
  }

  memset(&p_csifw_ctx->csi_services[idx], 0, sizeof(csifw_service_info_t));
  p_csifw_ctx->service_count--;

  CSIFW_LOGI("Service [%d] un-registered successfully", p_svc_info->svc_id);
  CSIFW_LOGI("Remaining services count is %d", p_csifw_ctx->service_count);
  return CSIFW_OK;
}

static void nw_state_notify_service(CONNECTION_STATE state) 
{
  if (!g_pcsifw_context) {
    return;
  }
  CSIFW_RES res = CSIFW_OK_WIFI_CONNECTED;
  if (state == WIFI_DISCONNECTED) {
    res = CSIFW_ERROR_WIFI_DIS_CONNECTED;
  }
  for (int i = 0; i < CSIFW_MAX_NUM_APPS; i++) {
    // send raw data
    if (g_pcsifw_context->csi_services[i].svc_id) {
      if (g_pcsifw_context->csi_services[i].raw_data_cb) {
        g_pcsifw_context->csi_services[i].raw_data_cb(res, 0, NULL, g_pcsifw_context->csi_services[i].service_data);
		// send parsed data
      } else if (g_pcsifw_context->csi_services[i].parsed_data_cb) {
        g_pcsifw_context->csi_services[i].parsed_data_cb(res, 0, NULL, g_pcsifw_context->csi_services[i].service_data);
      }
    }
  }
}

csifw_context_t *get_csifw_context() { return g_pcsifw_context; }

static CSIFW_RES check_hnd_validity(csifw_service_handle hnd) 
{
  if (!hnd) {
    CSIFW_LOGE(
        "Invalid/NULL handle argument, should be same as csifw_service_handle "
        "returned from csifw_registerService()");
    return CSIFW_INVALID_ARG;
  }

  if (!g_pcsifw_context) {
    CSIFW_LOGE("Invalid context pointer (NULL)");
    return CSIFW_ERROR_NOT_INITIALIZED;
  }

  csifw_service_info_t *p_svc_info = (csifw_service_info_t *)hnd;

  if (((uintptr_t)p_svc_info - (uintptr_t)g_pcsifw_context->csi_services) % sizeof(csifw_service_info_t) != 0) {
    CSIFW_LOGE("Invalid handle alignment");
    return CSIFW_INVALID_ARG;
  }

  if (p_svc_info < &g_pcsifw_context->csi_services[0] ||
      p_svc_info > &g_pcsifw_context->csi_services[CSIFW_MAX_NUM_APPS - 1]) {
    CSIFW_LOGE("Handle out of bounds");
    return CSIFW_INVALID_ARG;
  }

  if (p_svc_info->svc_id == 0) {
    CSIFW_LOGE("Service not registered");
    return CSIFW_ERROR_SERVICE_NOT_REGISTERED;
  }

  return CSIFW_OK;
}

static void destroy_csifw_context(void) 
{
  if (!g_pcsifw_context) {
    return;
  }

  free(g_pcsifw_context);
  g_pcsifw_context = NULL;
}

static int all_services_stopped(void) 
{
  if (!g_pcsifw_context) {
    return 0;
  }

  if (g_pcsifw_context->service_count == 0) {
    CSIFW_LOGD("No services registered\n");
    return 1;
  }

  for (int i = 0; i < CSIFW_MAX_NUM_APPS; i++) {
    if ((g_pcsifw_context->csi_services[i].svc_id != 0) && (g_pcsifw_context->csi_services[i].svc_state != CSI_SERVICE_STOP)) {
      CSIFW_LOGD("Service %d is still running (state: %d)\n", i, g_pcsifw_context->csi_services[i].svc_state);
      return 0;
    }
  }

  CSIFW_LOGI("All services are stopped\n");
  return 1;
}
