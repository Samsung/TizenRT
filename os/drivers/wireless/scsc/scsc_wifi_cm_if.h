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

#ifndef __SCSC_WIFI_CM_IF_H__
#define __SCSC_WIFI_CM_IF_H__

#include <sys/types.h>
#include <scsc/scsc_mx.h>

struct slsi_dev;

/**
 * CM interface States:
 *     STOPPED  : (default) state,
 *     PROBING  :
 *     PROBED   :
 *     STARTING :
 *     STARTED  :
 *     STOPPING :
 *     REMOVING :
 *     REMOVED  :
 *     BLOCKED  :
 */
enum scsc_wifi_cm_if_state {
	SCSC_WIFI_CM_IF_STATE_STOPPED,
	SCSC_WIFI_CM_IF_STATE_PROBING,
	SCSC_WIFI_CM_IF_STATE_PROBED,
	SCSC_WIFI_CM_IF_STATE_STARTING,
	SCSC_WIFI_CM_IF_STATE_STARTED,
	SCSC_WIFI_CM_IF_STATE_STOPPING,
	SCSC_WIFI_CM_IF_STATE_REMOVING,
	SCSC_WIFI_CM_IF_STATE_REMOVED,
	SCSC_WIFI_CM_IF_STATE_BLOCKED
};

/**
 * Notification Events
 *     SCSC_WIFI_STOP : Wifi service should freeze
 *     SCSC_WIFI_FAILURE_RESET : Fail to reset
 *
 */
enum scsc_wifi_cm_if_notifier {
	SCSC_WIFI_STOP,
	SCSC_WIFI_FAILURE_RESET
};

struct scsc_wifi_cm_if {
	struct slsi_dev *sdev;
	/* a std mutex */
	pthread_mutex_t cm_if_mutex;

	/* refer to enum scsc_wifi_cm_if_state */
	int cm_if_state;
};

/*********************************** API ************************************/

/**
 * Driver's interface to cm_if
 */
struct slsi_dev *slsi_dev_attach(struct scsc_mx *core, struct scsc_service_client *mx_wlan_client);
void slsi_dev_detach(struct slsi_dev *sdev);

/**
 * cm_if's interface to driver
 */
int slsi_sm_service_driver_register(void);
void slsi_sm_service_driver_unregister(void);
void slsi_sm_service_failed(struct slsi_dev *sdev, const char *reason);
int slsi_sm_wlan_service_open(struct slsi_dev *sdev);
int slsi_sm_wlan_service_start(struct slsi_dev *sdev);
void slsi_sm_wlan_service_stop(struct slsi_dev *sdev);
//int slsi_wlan_service_notifier_register(struct notifier_block *nb);
//int slsi_wlan_service_notifier_unregister(struct notifier_block *nb);

#endif
