/****************************************************************************
 *
 * Copyright (c) 2012 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

#ifndef __SCSC_WIFI_CM_IF_H__
#define __SCSC_WIFI_CM_IF_H__

#include <scsc/scsc_mx.h>
#include <pthread.h>
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
