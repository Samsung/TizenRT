/*****************************************************************************
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

#include "hip.h"
#include "debug_scsc.h"
#include "hydra.h"
#include "sap.h"
#include "utils_scsc.h"
#include <tinyara/config.h>

#include <sys/types.h>
#include <sys/time.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <poll.h>
#include <errno.h>
#include <assert.h>
#include <debug_scsc.h>

#include <tinyara/irq.h>
#include <tinyara/kmalloc.h>
#include <tinyara/clock.h>
#include <tinyara/fs/fs.h>
#include <tinyara/spi/spi.h>
#include <arpa/inet.h>
/* SAP implementations container. Local and static to hip */
static struct hip_sap {
	struct sap_api *sap[SAP_TOTAL];
} hip_sap_cont;

/* Register SAP with HIP layer */
int slsi_hip_sap_register(struct sap_api *sap_api)
{
	u8 class = sap_api->sap_class;

	if (class >= SAP_TOTAL) {
		return -ENODEV;
	}

	hip_sap_cont.sap[class] = sap_api;

	return 0;
}

/* UNregister SAP with HIP layer */
int slsi_hip_sap_unregister(struct sap_api *sap_api)
{
	u8 class = sap_api->sap_class;

	if (class >= SAP_TOTAL) {
		return -ENODEV;
	}

	hip_sap_cont.sap[class] = NULL;

	return 0;
}

int slsi_hip_sap_setup(struct slsi_dev *sdev)
{
	/* Execute callbacks to intorm Supported version */
	u16 version = 0;
	u32 conf_hip4_ver = 0;

	conf_hip4_ver = scsc_wifi_get_hip_config_version(&sdev->hip4_inst.hip_control->init);
	/* We enforce that all the SAPs are registered at this point */
	if ((!hip_sap_cont.sap[SAP_MLME]) || (!hip_sap_cont.sap[SAP_MA]) || (!hip_sap_cont.sap[SAP_DBG]) || (!hip_sap_cont.sap[SAP_TST])) {
		return -ENODEV;
	}

	if (hip_sap_cont.sap[SAP_MLME]->sap_version_supported) {
		if (conf_hip4_ver == 4) {
			version = scsc_wifi_get_hip_config_version_4_u16(&sdev->hip4_inst.hip_control->config_v4, sap_mlme_ver);
		}
		if (conf_hip4_ver == 3) {
			version = scsc_wifi_get_hip_config_version_3_u16(&sdev->hip4_inst.hip_control->config_v3, sap_mlme_ver);
		}
		if (hip_sap_cont.sap[SAP_MLME]->sap_version_supported(version)) {
			return -ENODEV;
		}
	} else {
		return -ENODEV;
	}

	if (hip_sap_cont.sap[SAP_MA]->sap_version_supported) {
		if (conf_hip4_ver == 4) {
			version = scsc_wifi_get_hip_config_version_4_u16(&sdev->hip4_inst.hip_control->config_v4, sap_ma_ver);
		}
		if (conf_hip4_ver == 3) {
			version = scsc_wifi_get_hip_config_version_3_u16(&sdev->hip4_inst.hip_control->config_v3, sap_ma_ver);
		}
		if (hip_sap_cont.sap[SAP_MA]->sap_version_supported(version)) {
			return -ENODEV;
		}
	} else {
		return -ENODEV;
	}

	if (hip_sap_cont.sap[SAP_DBG]->sap_version_supported) {
		if (conf_hip4_ver == 4) {
			version = scsc_wifi_get_hip_config_version_4_u16(&sdev->hip4_inst.hip_control->config_v4, sap_debug_ver);
		}
		if (conf_hip4_ver == 3) {
			version = scsc_wifi_get_hip_config_version_3_u16(&sdev->hip4_inst.hip_control->config_v3, sap_debug_ver);
		}
		if (hip_sap_cont.sap[SAP_DBG]->sap_version_supported(version)) {
			return -ENODEV;
		}
	} else {
		return -ENODEV;
	}

	if (hip_sap_cont.sap[SAP_TST]->sap_version_supported) {
		if (conf_hip4_ver == 4) {
			version = scsc_wifi_get_hip_config_version_4_u16(&sdev->hip4_inst.hip_control->config_v4, sap_test_ver);
		}
		if (conf_hip4_ver == 3) {
			version = scsc_wifi_get_hip_config_version_3_u16(&sdev->hip4_inst.hip_control->config_v3, sap_test_ver);
		}
		if (hip_sap_cont.sap[SAP_TST]->sap_version_supported(version)) {
			return -ENODEV;
		}
	} else {
		return -ENODEV;
	}

	/* Success */
	return 0;
}

int slsi_hip_service_notifier(struct slsi_dev *sdev, unsigned long event)
{
	switch (event) {
	case SCSC_WIFI_STOP:
		SLSI_INFO(sdev, "Freeze hip4\n");
		hip4_freeze(&sdev->hip4_inst);
		break;

	case SCSC_WIFI_FAILURE_RESET:
		SLSI_INFO(sdev, "Set HIP4 up again\n");
		hip4_setup(&sdev->hip4_inst);
		break;

	default:
		SLSI_INFO(sdev, "Unknown event code %lu\n", event);
		break;
	}

	return 0;
}

int slsi_hip_init(struct slsi_dev *sdev)
{
	memset(&sdev->hip4_inst, 0, sizeof(sdev->hip4_inst));

	sdev->hip.sdev = sdev;
	sdev->hip.hip_state = SLSI_HIP_STATE_STOPPED;
	SLSI_MUTEX_INIT(sdev->hip.hip_mutex);
	mbulk_pool_init();

	return 0;
}

void slsi_hip_deinit(struct slsi_dev *sdev)
{
	pthread_mutex_destroy(&sdev->hip.hip_mutex);
}

int slsi_hip_start(struct slsi_dev *sdev, struct slsi_hip_card_params *hip_param)
{
	SLSI_UNUSED_PARAMETER(hip_param);

	if (!sdev->maxwell_core) {
		SLSI_ERR(sdev, "Maxwell core does not exist\n");
		return -EINVAL;
	}

	SLSI_DBG4(sdev, SLSI_HIP_INIT_DEINIT, "[1/3]. Update HIP state (SLSI_HIP_STATE_STARTING)\n");
	sdev->hip.hip_state = SLSI_HIP_STATE_STARTING;

	SLSI_DBG4(sdev, SLSI_HIP_INIT_DEINIT, "[2/3]. Initialise HIP\n");
	if (hip4_init(&sdev->hip4_inst)) {
		sdev->hip.hip_state = SLSI_HIP_STATE_STOPPED;
		SLSI_ERR(sdev, "hip4_init failed\n");
		return -EINVAL;
	}

	SLSI_DBG4(sdev, SLSI_HIP_INIT_DEINIT, "[3/3]. Update HIP state (SLSI_HIP_STATE_STARTED)\n");
	sdev->hip.hip_state = SLSI_HIP_STATE_STARTED;

	return 0;
}

/* SAP rx proxy */
int slsi_hip_rx(struct slsi_dev *sdev, struct max_buff *mbuf)
{
	u16 pid;

	/* We enforce that all the SAPs are registered at this point */
	if ((!hip_sap_cont.sap[SAP_MLME]) || (!hip_sap_cont.sap[SAP_MA]) || (!hip_sap_cont.sap[SAP_DBG]) || (!hip_sap_cont.sap[SAP_TST])) {
		return -ENODEV;
	}
	/* Udi test : If pid in UDI range then pass to UDI and ignore */
	slsi_log_clients_log_signal_fast(sdev, &sdev->log_clients, mbuf, SLSI_LOG_DIRECTION_TO_HOST);
	pid = fapi_get_u16(mbuf, receiver_pid);
	if (pid >= SLSI_TX_PROCESS_ID_UDI_MIN && pid <= SLSI_TX_PROCESS_ID_UDI_MAX) {
		slsi_kfree_mbuf(mbuf);
		return 0;
	}

	if (fapi_is_ma(mbuf)) {
		return hip_sap_cont.sap[SAP_MA]->sap_handler(sdev, mbuf);
	}

	if (fapi_is_mlme(mbuf)) {
		return hip_sap_cont.sap[SAP_MLME]->sap_handler(sdev, mbuf);
	}

	if (fapi_is_debug(mbuf)) {
		return hip_sap_cont.sap[SAP_DBG]->sap_handler(sdev, mbuf);
	}

	if (fapi_is_test(mbuf)) {
		return hip_sap_cont.sap[SAP_TST]->sap_handler(sdev, mbuf);
	}

	return -EIO;
}

/* Only DATA plane will look at the returning FB to account BoT */
int slsi_hip_tx_done(struct slsi_dev *sdev, u16 colour)
{
	return hip_sap_cont.sap[SAP_MA]->sap_txdone(sdev, colour);
}

int slsi_hip_setup(struct slsi_dev *sdev)
{
	/* Setup hip4 after initialization */
	return hip4_setup(&sdev->hip4_inst);
}

int slsi_hip_stop(struct slsi_dev *sdev)
{
	SLSI_DBG4(sdev, SLSI_HIP_INIT_DEINIT, "Update HIP state (SLSI_HIP_STATE_STOPPING)\n");
	sdev->hip.hip_state = SLSI_HIP_STATE_STOPPING;

	hip4_deinit(&sdev->hip4_inst);

	SLSI_DBG4(sdev, SLSI_HIP_INIT_DEINIT, "Update HIP state (SLSI_HIP_STATE_STOPPED)\n");
	sdev->hip.hip_state = SLSI_HIP_STATE_STOPPED;

	return 0;
}
