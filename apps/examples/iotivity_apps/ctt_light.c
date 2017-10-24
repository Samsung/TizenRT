/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
///////////////////////////////////////////////////////////////////////
//NOTE :  This sample server is generated based on ocserverbasicops.cpp
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"
#include "ocserver.h"

#include "iotivity_common.h"

#if defined(CONFIG_TLS_WITH_SSS)
#include "tls/see_api.h"
#endif

#define TAG "CTT LIGHT"

static int gObserveNotifyType = 3;

//Secure Virtual Resource database for Iotivity Server
//It contains Server's Identity and the PSK credentials
//of other devices which the server trusts

#define IOTIVITY_TEST_STACKSIZE   16384
#define IOTIVITY_TEST_PRI   100
#define IOTIVITY_TEST_SCHED_POLICIY SCHED_RR

/* Initialize oic_svr_db_server_justworks.dat file
*  Current implementation:  CRED_FILE data is loaded/hardcoded in byte_array(data_oic_svr_db_server_dat)
*/

int ctt_light_main_cb(int argc, char *argv[])
{
	int ret;
	struct timespec timeout;
	int isRemoveDB = 0;

#if defined(__TIZENRT__)
	if (argc >= 3) {
		isRemoveDB = (int)(*argv[2] - '0');
	}
#else
	if (argc >= 2) {
		isRemoveDB = (int)(*argv[1] - '0');
	}
#endif

	observeThreadStarted = false;
#if defined(__TIZENRT__) && defined(CONFIG_TLS_WITH_SSS)
	see_init();
#endif
	printf("[%s] OCServer is starting...\n", TAG);

	if (isRemoveDB == 1) {
		printf("Recovery json to db\n");
		ocf_mylight_remove_db();
		ocf_mylight_recovery_default();
	}

	ocf_mylight_security_init();

	/* Intialize OCStack */
#ifdef RA_ADAPTER
	OCSetRAInfo(&rainfo);
#endif
	if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK) {
		printf("[%s] OCStack init error \n", TAG);
		return -1;
	}

	/* Set Platform and device information */
	OCSetDefaultDeviceEntityHandler(OCDeviceEntityHandlerCb, NULL);
	OCStackResult registrationResult = SetPlatformInfo(platformID, manufacturerName, manufacturerLink, modelNumber,
									   dateOfManufacture, platformVersion, operatingSystemVersion, hardwareVersion,
									   firmwareVersion, supportLink, systemTime);
	if (registrationResult != OC_STACK_OK) {
		printf("Platform info setting failed locally!\n");
		exit(EXIT_FAILURE);
	}

	registrationResult = OCSetPlatformInfo(platformInfo);

	if (registrationResult != OC_STACK_OK) {
		printf("Platform Registration failed!");
		exit(EXIT_FAILURE);
	}
	//optional device info
	OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_PROTOCOL_INDEPENDENT_ID, piid);

	registrationResult = SetDeviceInfoLocal(deviceName, specVersion, dataModelVersions, deviceType);

	if (registrationResult != OC_STACK_OK) {
		printf("Device info setting failed locally!");
		exit(EXIT_FAILURE);
	}

	registrationResult = OCSetDeviceInfo(deviceInfo);

	if (registrationResult != OC_STACK_OK) {
		printf("Device Registration failed!");
		exit(EXIT_FAILURE);
	}

	/*
	 * Declare and create the example resource: LED
	 */
	initLightResource(gResourceUri, &LED, false, 0);


	ocf_mylight_notify_init();
	ocf_mylight_userinput_init();

	timeout.tv_sec = 0;
	timeout.tv_nsec = 100000000L;

	DeletePlatformInfoLocal();
	DeleteDeviceInfoLocal();

	// Break from loop with Ctrl-C
	printf("[%s] Entering ocserver main loop \n", TAG);
	while (!gQuitFlag) {
		if (OCProcess() != OC_STACK_OK) {
			printf("[%s] OCStack process error \n", TAG);
			return -1;
		}
		nanosleep(&timeout, NULL);
	}

	printf("[%s] Exiting ocserver main loop \n", TAG);

	if (observeThreadStarted) {
		pthread_cancel(threadId_observe);
		pthread_join(threadId_observe, NULL);
	}

	if (OCStop() != OC_STACK_OK) {
		printf("[%s] OCStack process error \n", TAG);
	}
#if defined(__TIZENRT__) && defined(CONFIG_TLS_WITH_SSS)
	see_free();
#endif

	return 0;
}
#ifndef __TIZENRT__
int main(int argc, char *argv[])
{
	ctt_light_main_cb(argc, argv);

	return 0;
}

#else
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ctt_light_main(int argc, char *argv[])
#endif
{
	task_create("iotivity_ctt_light", IOTIVITY_TEST_PRI, IOTIVITY_TEST_STACKSIZE, ctt_light_main_cb, (FAR char *const *)argv);
	return 0;
}
#endif
