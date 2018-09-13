/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef FMWUP_UTIL_DATA_H_
#define FMWUP_UTIL_DATA_H_

#include <stdbool.h>

#define FIRMWARE_URI                            "/firmware"

/* save to key manager(ckmc) with update */
#define FIRMWARE_PROPERTY_UPDATE                "update"
#define FIRMWARE_PROPERTY_UPDATE_TIME           "updatetime"
#define FIRMWARE_PROPERTY_DESCRIPTION           "description"
#define FIRMWARE_PROPERTY_STATE                 "state"
#define FIRMWARE_PROPERTY_RESULT                "result"
#define FIRMWARE_PROPERTY_PROGRESS              "progress"

/* save to key manager(ckmc) with pacakage */
#define FIRMWARE_PROPERTY_NEW_VERSION           "newversion"
#define FIRMWARE_PROPERTY_PACKAGE_URI           "packageuri"

/* platform information */
#define FIRMWARE_PROPERTY_CURRENT_VERSION       "version"
#define FIRMWARE_PROPERTY_VENDER                "vender"
#define FIRMWARE_PROPERTY_MODEL                 "model"

#define KEY_MANAGER_MAX_DATA_LENGTH             21
#define KEY_MANAGER_INT_DEFAULT_DATA            "0"
#define KEY_MANAGER_STR_DEFAULT_DATA            " "
#define KEY_MANAGER_BOL_DEFAULT_DATA            "0"

#define FIRMWARE_PROPERTY_UPDATE_INIT           "Init"
#define FIRMWARE_PROPERTY_UPDATE_CHECK          "Check"
#define FIRMWARE_PROPERTY_UPDATE_DOWNLOAD       "Download"
#define FIRMWARE_PROPERTY_UPDATE_UPDATE         "Update"
#define FIRMWARE_PROPERTY_UPDATE_DOWNLOADUPDATE "DownloadUpdate"

typedef struct {
	/* Update Property */
	int64_t state;				//[R][M] fmwup_state_e - 0: Idle, 1: Downloading, 2: Downloaded, 3: Updating
	int64_t result;				//[R][M] 0: Initial, 1: Success, 2: Not enough space, 3: Out of ram, 4: Connection lost, 5: Invalid binary, 6: Invalid uri, 7: Update failed, 8: Unsupported protocol
	int64_t update;				//[RW][M] 0: Initial, 1: Download Image, 2: Upgrade Image, 3:Dwonload and Upgrade, 4 Scheduled Upgrade
	char *update_time;			//[RW][O] case of (4 – update) (scheduled)  TODO: ISO 8601

	/* Package Information */
	char *new_version;			//[RW][M] New Version of the firmware package
	char *package_uri;			//[RW][M] Firmware package URI where the package located

	/* Device Information */
	char *manufacturer;			//[R][O] Device vendor identifier
	char *model_name;			//[R][O] Device model identifier
	char *current_version;		//[R][M] Current firmware version

} fmwup_data_s;

int64_t fmwup_data_get_update_int64(const char *);
char *fmwup_data_get_update_string(int64_t);

fmwup_data_s *fmwup_data_get_properties();
int fmwup_data_set_property_int64(const char *name, int64_t data);
int fmwup_data_set_property(const char *name, char *data);
char *fmwup_data_get_property(const char *name);
int key_manager_init();
int key_manager_save_data();

char *key_manager_set_default_data(const char *name);
int key_manager_reset_data();

#endif							/* FMWUP_UTIL_DATA_H_ */
