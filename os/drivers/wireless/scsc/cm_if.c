/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#include "dev.h"
#include "debug_scsc.h"
#include "scsc_wifi_cm_if.h"

#include "scsc_mx.h"
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t slsi_start_mutex;
pthread_mutex_t slsi_open_mutex;
static int recovery_in_progress;
static u16 latest_scsc_panic_code;
#define SLSI_SM_WLAN_SERVICE_STOP_RECOVERY_TIMEOUT 20000

static void slsi_hip_block_bh(struct slsi_dev *sdev);

static void wlan_stop_on_failure(struct scsc_service_client *client)
{
	int state;
	struct slsi_dev *sdev = container_of(client, struct slsi_dev, mx_wlan_client);

	SLSI_INFO_NODEV("\n");

	pthread_mutex_lock(&slsi_start_mutex);
	recovery_in_progress = 1;
	sdev->recovery_status = 1;
	state = sdev->cm_if.cm_if_state;
	if (state != SCSC_WIFI_CM_IF_STATE_STOPPED) {
		sdev->cm_if.cm_if_state = SCSC_WIFI_CM_IF_STATE_BLOCKED;
		slsi_hip_block_bh(sdev);
		sdev->fail_reported = true;

		/* Stop wlan operations. Send event to registered parties */
		pthread_mutex_unlock(&slsi_start_mutex);
		SLSI_INFO_NODEV("Nofity registered functions\n");
		sap_mlme_notifier(sdev, SCSC_WIFI_STOP);
		sap_ma_notifier(sdev, SCSC_WIFI_STOP);
		slsi_hip_service_notifier(sdev, SCSC_WIFI_STOP);
		pthread_mutex_lock(&slsi_start_mutex);
	} else {
		SLSI_INFO_NODEV("Wi-Fi service driver not started\n");
	}

	init_completion(&sdev->recovery_remove_completion);
	init_completion(&sdev->recovery_stop_completion);
	pthread_mutex_unlock(&slsi_start_mutex);
}

static void wlan_failure_reset(struct scsc_service_client *client, u16 scsc_panic_code)
{
	SLSI_INFO_NODEV("Enter\n");
	latest_scsc_panic_code = scsc_panic_code;
}

/* WLAN service driver registration
 * ================================
 */
void slsi_wlan_service_probe(struct scsc_mx_module_client *module_client, struct scsc_mx *mx, enum scsc_module_client_reason reason)
{
	struct slsi_dev *sdev;
	int ret = 0;
	struct scsc_service_client mx_wlan_client;

	UNUSED(module_client);

	SLSI_INFO_NODEV("WLAN service probe\n");

	pthread_mutex_lock(&slsi_start_mutex);
	if (reason == SCSC_MODULE_CLIENT_REASON_RECOVERY && !recovery_in_progress) {
		goto done;
	}

	if (reason == SCSC_MODULE_CLIENT_REASON_RECOVERY) {
		SLSI_INFO_NODEV("Probe recovery\n");
		sdev = cm_ctx.sdev;
		recovery_in_progress = 0;
		sdev->fail_reported = false;
	} else {
		/* Register callbacks */
		mx_wlan_client.stop_on_failure = wlan_stop_on_failure;
		mx_wlan_client.failure_reset = wlan_failure_reset;

		sdev = slsi_dev_attach(mx, &mx_wlan_client);
		if (sdev == NULL) {
			SLSI_ERR_NODEV("WLAN attach failed - slsi_dev_attach\n");
			ret = -ENOMEM;
			goto done;
		}

		cm_ctx.sdev = sdev;

		sdev->cm_if.cm_if_state = SCSC_WIFI_CM_IF_STATE_PROBING;
	}
	//get_device(dev);
	sdev->cm_if.cm_if_state = SCSC_WIFI_CM_IF_STATE_PROBED;

done:
	pthread_mutex_unlock(&slsi_start_mutex);
}

static void slsi_wlan_service_remove(struct scsc_mx_module_client *module_client, struct scsc_mx *mx, enum scsc_module_client_reason reason)
{
	struct slsi_dev *sdev;
	int state;

	UNUSED(mx);
	UNUSED(module_client);

	SLSI_INFO_NODEV("WLAN service remove\n");

	sdev = cm_ctx.sdev;
	if (!sdev) {
		SLSI_INFO_NODEV("no sdev\n");
		return;
	}
	if (reason == SCSC_MODULE_CLIENT_REASON_RECOVERY && !recovery_in_progress) {
		SLSI_INFO_NODEV("WLAN service remove - recovery. Service not active.\n");
	} else if (reason == SCSC_MODULE_CLIENT_REASON_RECOVERY && recovery_in_progress) {
		int r;

		SLSI_INFO_NODEV("WLAN service remove - recovery\n");
		SLSI_INFO_NODEV("Nofity registered functions\n");

		sap_mlme_notifier(sdev, SCSC_WIFI_FAILURE_RESET);
		sap_ma_notifier(sdev, SCSC_WIFI_FAILURE_RESET);
		slsi_hip_service_notifier(sdev, SCSC_WIFI_FAILURE_RESET);

		pthread_mutex_lock(&slsi_start_mutex);
		/**
		 * If there was a request to stop during the recovery, then do
		 * not sent a hang - just stop here. The Wi-Fi service driver is
		 * ready to be turned on again. Let the service_stop complete.
		 */
		complete_all(&sdev->recovery_remove_completion);
		if (sdev->recovery_next_state == SCSC_WIFI_CM_IF_STATE_STOPPING) {
			SLSI_INFO_NODEV("Recovery - next state stopping\n");
		} else {
			SLSI_INFO_NODEV("Calling slsi_send_hanged_vendor_event with latest_scsc_panic_code=0x%x\n", latest_scsc_panic_code);
			if (slsi_send_hanged_vendor_event(sdev, latest_scsc_panic_code) < 0) {
				SLSI_ERR(sdev, "Failed to send hang event\n");
			}

			/* Complete any pending ctrl signals, which will prevent
			 * the hang event from being processed.
			 */
			complete_all(&sdev->sig_wait.completion);
		}

		pthread_mutex_unlock(&slsi_start_mutex);
		r = wait_for_completion_timeout(&sdev->recovery_stop_completion, SLSI_SM_WLAN_SERVICE_STOP_RECOVERY_TIMEOUT);
		if (r == 0) {
			SLSI_INFO(sdev, "recovery_stop_completion timeout\n");
		}
	} else {
		pthread_mutex_lock(&slsi_start_mutex);
		state = sdev->cm_if.cm_if_state;
		if (state != SCSC_WIFI_CM_IF_STATE_STARTED && state != SCSC_WIFI_CM_IF_STATE_PROBED && state != SCSC_WIFI_CM_IF_STATE_STOPPED) {
			pthread_mutex_unlock(&slsi_start_mutex);
			SLSI_INFO_NODEV("state-event error %d\n", state);
			return;
		}

		sdev->cm_if.cm_if_state = SCSC_WIFI_CM_IF_STATE_REMOVING;
		//put_device(sdev->dev);
		slsi_dev_detach(sdev);
		sdev->maxwell_core = NULL;
		sdev->cm_if.cm_if_state = SCSC_WIFI_CM_IF_STATE_REMOVED;

		pthread_mutex_unlock(&slsi_start_mutex);
	}
}

/* Block future HIP runs through the hip_switch */
static void slsi_hip_block_bh(struct slsi_dev *sdev)
{
	SLSI_WARN(sdev, "HIP state set to #SLSI_HIP_STATE_BLOCKED#\n");
	SLSI_MUTEX_LOCK(sdev->hip.hip_mutex);
	sdev->hip.hip_state = SLSI_HIP_STATE_BLOCKED;
	SLSI_MUTEX_UNLOCK(sdev->hip.hip_mutex);
}

struct scsc_mx_module_client wlan_driver = {
	.name = "WLAN driver",
	.probe = slsi_wlan_service_probe,
	.remove = slsi_wlan_service_remove,
};

int slsi_sm_service_driver_register(void)
{
	struct slsi_cm_ctx *ctx = &cm_ctx;

	memset(ctx, 0, sizeof(*ctx));
	pthread_mutex_init(&slsi_start_mutex, NULL);
	pthread_mutex_init(&slsi_open_mutex, NULL);
	scsc_mx_module_register_client_module(&wlan_driver);

	return 0;
}

void slsi_sm_service_driver_unregister(void)
{
	scsc_mx_module_unregister_client_module(&wlan_driver);
}

/* start/stop wlan service
 * =======================
 */
void slsi_sm_service_failed(struct slsi_dev *sdev, const char *reason)
{
	int state;

	pthread_mutex_lock(&slsi_start_mutex);

	//state = atomic_read(&sdev->cm_if.cm_if_state);
	state = sdev->cm_if.cm_if_state;
	if (state != SCSC_WIFI_CM_IF_STATE_STARTED) {
		pthread_mutex_unlock(&slsi_start_mutex);
		SLSI_INFO(sdev, "State-event error %d\n", state);
		return;
	}

	/* Limit the volume of error reports to the core */
	if (!sdev->fail_reported) {
		/* This log may be scraped by test systems */
		pr_err("scsc_wifibt: FATAL ERROR: %s\n", reason);

		sdev->cm_if.cm_if_state = SCSC_WIFI_CM_IF_STATE_BLOCKED;
		slsi_hip_block_bh(sdev);
		scsc_mx_service_service_failed(sdev->service);
		sdev->fail_reported = true;
	}

	pthread_mutex_unlock(&slsi_start_mutex);
}

/* Is production test mode enabled? */
bool slsi_is_test_mode_enabled(void)
{
#ifdef CONFIG_SCSC_WLANLITE
	return true;
#else
	return false;
#endif
}

int slsi_sm_wlan_service_open(struct slsi_dev *sdev)
{
	int err = 0;
	int state;

	pthread_mutex_lock(&slsi_open_mutex);

	state = sdev->cm_if.cm_if_state;
	if (state != SCSC_WIFI_CM_IF_STATE_PROBED && state != SCSC_WIFI_CM_IF_STATE_STOPPED) {
		SLSI_INFO(sdev, "State-event error %d\n", state);
		err = -EINVAL;
		goto exit;
	}

	/* Open service - will download FW - will set MBOX0 with Starting address */
	SLSI_INFO(sdev, "Open WLAN service\n");
	sdev->service = scsc_mx_service_open(sdev->maxwell_core, SCSC_SERVICE_ID_WLAN, &sdev->mx_wlan_client);
	if (!sdev->service) {
		sdev->cm_if.cm_if_state = SCSC_WIFI_CM_IF_STATE_PROBED;
		SLSI_WARN(sdev, "Service open failed\n");
		err = -EINVAL;
		goto exit;
	}

exit:
	pthread_mutex_unlock(&slsi_open_mutex);
	return err;
}

int slsi_sm_wlan_service_start(struct slsi_dev *sdev)
{
	struct slsi_hip4 *hip = &sdev->hip4_inst;
	scsc_mifram_ref ref;
	int err = 0;
	int state;

	//pthread_mutex_lock(&slsi_start_mutex);
	state = sdev->cm_if.cm_if_state;
	if (state != SCSC_WIFI_CM_IF_STATE_PROBED && state != SCSC_WIFI_CM_IF_STATE_STOPPED) {
		SLSI_INFO(sdev, "State-event error %d\n", state);
		err = -EINVAL;
		goto exit;
	}

	sdev->cm_if.cm_if_state = SCSC_WIFI_CM_IF_STATE_STARTING;
	SLSI_INFO(sdev, "Start HIP\n");
	SLSI_MUTEX_LOCK(sdev->hip.hip_mutex);
	err = slsi_hip_start(sdev, NULL);
	SLSI_MUTEX_UNLOCK(sdev->hip.hip_mutex);
	if (err) {
		goto exit;
	}

	err = scsc_mx_service_mif_ptr_to_addr(sdev->service, hip->hip_control, &ref);
	if (err) {
		goto exit;
	}

	SLSI_INFO(sdev, "Starting WLAN service\n");
	err = scsc_mx_service_start(sdev->service, ref);
	if (err) {
		goto exit;
	}

	SLSI_INFO(sdev, "Starting HIP SETUP\n");
	err = slsi_hip_setup(sdev);
	if (err) {
		goto exit;
	}
	SLSI_INFO(sdev, "Starting HIP SAP SETUP\n");
	/* Service has started, inform SAP versions to the registered SAPs */
	err = slsi_hip_sap_setup(sdev);
	if (err) {
		goto exit;
	}
	SLSI_INFO(sdev, "CM STARTED\n");

	sdev->cm_if.cm_if_state = SCSC_WIFI_CM_IF_STATE_STARTED;

exit:
	//pthread_mutex_unlock(&slsi_start_mutex);
	return err;
}

void slsi_sm_wlan_service_stop(struct slsi_dev *sdev)
{
	int cm_if_state;
	int err;

	pthread_mutex_lock(&slsi_start_mutex);
	cm_if_state = sdev->cm_if.cm_if_state;
	SLSI_INFO_NODEV("Stopping WLAN service state =%d\n", cm_if_state);
	if (cm_if_state == SCSC_WIFI_CM_IF_STATE_BLOCKED) {
		sdev->recovery_next_state = SCSC_WIFI_CM_IF_STATE_STOPPED;

		/* If the wait hasn't timed out, then the cm_if_state will be
		 * set to probed here. If the probe hasn't fired for some reason
		 * try and do a service_stop regardless, since that's all we can
		 * do in this situation; hence skip the state check.
		 */
		goto skip_state_check;
	}

	if (cm_if_state != SCSC_WIFI_CM_IF_STATE_STARTED && cm_if_state != SCSC_WIFI_CM_IF_STATE_REMOVED && cm_if_state != SCSC_WIFI_CM_IF_STATE_PROBED) {
		SLSI_INFO(sdev, "Service not started or incorrect state %d\n", cm_if_state);
		goto exit;
	}

skip_state_check:
	sdev->cm_if.cm_if_state = SCSC_WIFI_CM_IF_STATE_STOPPING;
	SLSI_INFO_NODEV("Stopping WLAN service\n");
	err = scsc_mx_service_stop(sdev->service);
	if (err) {
		SLSI_INFO(sdev, "scsc_mx_service_stop failed err: %d\n", err);
	}

	scsc_mx_service_close(sdev->service);
	sdev->cm_if.cm_if_state = SCSC_WIFI_CM_IF_STATE_STOPPED;
	if (recovery_in_progress) {
		complete_all(&sdev->recovery_stop_completion);
	}
exit:
	pthread_mutex_unlock(&slsi_start_mutex);
}

void slsi_sm_wlan_service_close(struct slsi_dev *sdev)
{
	int cm_if_state;

	pthread_mutex_lock(&slsi_start_mutex);
	cm_if_state = sdev->cm_if.cm_if_state;
	if (cm_if_state != SCSC_WIFI_CM_IF_STATE_STOPPED) {
		SLSI_INFO(sdev, "Service not stopped\n");
		goto exit;
	}

	SLSI_INFO_NODEV("Closing WLAN service\n");
	scsc_mx_service_close(sdev->service);
	if (recovery_in_progress) {
		complete_all(&sdev->recovery_stop_completion);
	}
exit:
	pthread_mutex_unlock(&slsi_start_mutex);
}

void slsi_hydra_get_chip_info(struct hydra_service_info *chip_info)
{
	if (chip_info) {
		memset(chip_info, 0, sizeof(*chip_info));
	}
}
