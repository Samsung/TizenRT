/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef _THINGS_SECURITY_MANAGER_H_
#define _THINGS_SECURITY_MANAGER_H_

#include "srmutility.h"

#ifdef __SECURED__
/**
 * @brief   Authentication type for ownership transfer
 *              b00000001  (1):  Just works authentication
 *              b00000010  (2):  Random PIN based authentication
 *              b00000100  (4):  Certificate based authentication
 *              b00001000  (8):  Decentralized public key based authentication
 *              b00010000 (16):  Preconfigured PIN based authentication
 *              b00100000 (32):  Mutual Verification Just works authentication
 */
typedef enum things_auth_type_e {
	AUTH_UNKNOW = (0x0),
	AUTH_JUST_WORKS = (0x1 << 0),
	AUTH_RANDOM_PIN = (0x1 << 1),
	AUTH_CERTIFICATE = (0x1 << 2),
	AUTH_DECENTRALIZED_PUB_KEY = (0x1 << 3),
	AUTH_PRECONF_PIN = (0x1 << 4),
	AUTH_JUST_WORKS_MUTUAL_VERIFIED = (0x1 << 5),
	AUTH_CERTIFICATE_CONFIRM = (0x1 << 6),
} things_auth_type_e;

/**
 * Access policy in least significant bits (from Spec):
 *              b00000001  (1):  CREATE
 *              b00000010  (2):  READ
 *              b00000100  (4):  WRITE
 *              b00001000  (8):  DELETE
 *              b00010000 (16):  NOTIFY
 *              b00011111 (31) Full permission
 */
#define ACL_PERMISSION_CREATE       (0x1 << 0)
#define ACL_PERMISSION_READ         (0x1 << 1)
#define ACL_PERMISSION_WRITE        (0x1 << 2)
#define ACL_PERMISSION_DELETE       (0x1 << 3)
#define ACL_PERMISSION_NOTIFY       (0x1 << 4)
#define ACL_PERMISSION_FULL (ACL_PERMISSION_CREATE | \
								ACL_PERMISSION_READ | \
								ACL_PERMISSION_WRITE | \
								ACL_PERMISSION_DELETE | \
								ACL_PERMISSION_NOTIFY)

/**
 * Function to reset the SVR DB(security provisioning data)
 *
 * @param authTypes authentication type for ownership transfer
 * @ret 0 is success, otherwise fail.
 */
extern int sm_init_things_security(int auth_type, const char *db_path);

/**
 * Function to init SVR DB(security provisioning data)
 * If SVR DB not exist, SVR DB will be generated.
 *
 * @ret 0 is success, otherwise fail.
 */
extern int sm_init_svrdb();

/**
 * Function to reset SVR DB
 *
 * @ret 0 is success, otherwise fail.
 */
extern int sm_reset_svrdb();

/**
 * Function to generate the device UUID.
 * NOTE : device uuid will be changed to the MD5 hash value of MAC address
 *
 * @param is_forced if true, Device UUID will be generated.
 * @ret 0 is success, otherwise fail.
 */
extern int sm_generate_device_id();

/**
 * Function to save the Cloud ACL
 *
 * @param cloud_uuid UUID for cloud
 * @ret 0 is success, otherwise fail.
 */
extern int sm_save_cloud_acl(const char *cloud_uuid);

/**
 * Function to save the ACL
 *
 * @param uuid                 UUID of controller or wildcard("*")
 * @param resourceUri          resource URI to be added into access control list.
 * @param resourceType         Type name of resource
 * @param interfaceName        Interface name of resource
 * @param permission           Access permission value
 *               Access policy in least significant bits (from Spec):
 *                   b00000001  (1):  CREATE
 *                   b00000010  (2):  READ
 *                   b00000100  (4):  WRITE
 *                   b00001000  (8):  DELETE
 *                   b00010000 (16):  NOTIFY
 *                   b00011111 (31) Full permission
 * @ret 0 is success, otherwise fail.
 */
extern int sm_save_acl(const char *uuid, const char *resource_uri, const char *resource_type, const char *interface_name, uint16_t permission);

/**
 * API to register OTM event handler
 *
 * @param otmEventHandler implementation of OTM event handler (@ref OicSecOtmEventHandler_t)
 */
extern void sm_set_otm_event_handler(OicSecOtmEventHandler_t otm_event_handler);

/**
 * Function to set a MOT status
 *
 * @param enable whether the MOT is enabled. (true=enable, false=disable)
 *
 * @ret 0 is success, otherwise fail.
 */
extern int sm_set_mot_status(bool enable);

#endif							//__SECURED__

#endif							//_THINGS_SECURITY_MANAGER_H_
