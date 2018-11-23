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

#ifndef ES_COMMON_H_
#define ES_COMMON_H_

#include "ocstack.h"
#include "octypes.h"
#include "things_types.h"

#define THINGS_STRING_MAX_HOSTNAME 256
#define THINGS_STRING_MAX_VALUE    100
#define IP_PORT                    22

/**
 * Attributes used to form a proper easysetup conforming JSON message.
 */
#define THINGS_RSRVD_ES_PROVSTATUS               "ps"
#define THINGS_RSRVD_ES_LATHINGS_ERRORCODE       "lec"
#define THINGS_RSRVD_ES_LINKS                    "links"
#define THINGS_RSRVD_ES_SUPPORTEDWIFIMODE        "swmt"
#define THINGS_RSRVD_ES_SUPPORTEDWIFIFREQ        "swf"
#define THINGS_RSRVD_ES_SSID                     "tnn"
#define THINGS_RSRVD_ES_CRED                     "cd"
#define THINGS_RSRVD_ES_AUTHTYPE                 "wat"
#define THINGS_RSRVD_ES_ENCTYPE                  "wet"
#define THINGS_RSRVD_ES_AUTHCODE                 "ac"
#define THINGS_RSRVD_ES_AUTHPROVIDER             "apn"
#define THINGS_RSRVD_ES_CISERVER                 "cis"
#define THINGS_RSRVD_ES_SERVERID                 "sid"
#define THINGS_RSRVD_ES_CLIENTID                 "clientid"
#define THINGS_RSRVD_ES_DEVNAME                  "dn"
#define THINGS_RSRVD_ES_LANGUAGE                 "lang"
#define THINGS_RSRVD_ES_COUNTRY                  "ctry"
#define THINGS_RSRVD_ES_VENDOR_DEVTYPE           "x.com.samsung.dt"
#ifdef CONFIG_ST_THINGS_SUPPORT_SUB_DEVICE
#define THINGS_RSRVD_ES_VENDOR_DEVSUBTYPE        "x.com.samsung.sdt"
#endif
#define THINGS_RSRVD_ES_VENDOR_DISCOVERYCHANNEL  "x.com.samsung.chn"
#define THINGS_RSRVD_ES_VENDOR_CLIENTID          "x.com.samsung.clientid"
#define THINGS_RSRVD_ES_VENDOR_ERRORCODE         "x.com.samsung.vd.errcode"
#define THINGS_RSRVD_ES_VENDOR_ACCESS_TOKEN      "at"
#define THINGS_RSRVD_ES_VENDOR_ACCESS_TOKEN_TYPE "att"
#define SC_RSRVD_ES_VENDOR_REFRESH_TOKEN         "x.com.samsung.refreshtoken"
#define THINGS_RSRVD_ES_VENDOR_USER_ID           "x.com.samsung.uid"
#define THINGS_RSRVD_ES_VENDOR_UTC_DATE_TIME     "x.com.samsung.datetime"

/**
 * Easysetup defined resoruce types and uris.
 */
#define THINGS_RSRVD_ES_RES_TYPE_PROV         "oic.r.easysetup"         // "oic.wk.prov"
#define THINGS_RSRVD_ES_URI_PROV              "/EasySetupResURI"        // "/ProvisioningResURI"
#define THINGS_RSRVD_ES_RES_TYPE_WIFI         "oic.r.wificonf"          // "oic.wk.wifi"
#define THINGS_RSRVD_ES_URI_WIFI              "/WiFiConfResURI"         // "/WiFiProvisioningResURI"
#define THINGS_RSRVD_ES_RES_TYPE_CLOUDSERVER  "oic.r.coapcloudconf"     // "oic.wk.cloudserver"
#define THINGS_RSRVD_ES_URI_CLOUDSERVER       "/CoapCloudConfResURI"    // "/CloudServerProvisioningResURI"
#define THINGS_RSRVD_ES_RES_TYPE_DEVCONF      "oic.r.devconf"           // "oic.wk.devconf"
#define THINGS_RSRVD_ES_URI_DEVCONF           "/DevConfResURI"          // "/DevConfProvisioningResURI"

#define NUM_WIFIMODE    10

#define MAX_DEVICELEN   100

typedef enum {
	ES_ERROR = -1,
	ES_OK = 0,
	ES_NETWORKFOUND = 1,
	ES_NETWORKCONNECTED,
	ES_NETWORKNOTCONNECTED,
	ES_RESOURCECREATED = 11,
	ES_RECVREQOFPROVRES = 21,
	ES_RECVREQOFNETRES,
	ES_RECVUPDATEOFPROVRES,
	ES_RECVTRIGGEROFPROVRES,
	ES_UNAUTHORIZED = 31
} es_result_e;

typedef enum {
	ES_WIFI_RESOURCE = 0x01,
	ES_CLOUD_RESOURCE = 0x02,
	ES_DEVCONF_RESOURCE = 0x04
} es_resource_mask_e;

typedef struct {
	struct {
		wifi_mode_e mode[NUM_WIFIMODE];
		wifi_freq_e freq;
	} WiFi;
	struct {
		char device_name[MAX_DEVICELEN];
		char device_type[THINGS_STRING_MAX_VALUE];
#ifdef CONFIG_ST_THINGS_SUPPORT_SUB_DEVICE
		char device_sub_type[THINGS_STRING_MAX_VALUE];
#endif
	} dev_conf_s;
} es_device_property;

/**
 * @brief Indicate last error code to describe a reason of error during easy setup.
 */
typedef enum {
	/**
	 * Initialize Error Code
	 */
	ES_ERRCODE_NO_ERROR = 0,

	/**
	 * Error Code that given WiFi's SSID is not found
	 */
	ES_ERRCODE_SSID_NOT_FOUND,

	/**
	 * Error Code that given WiFi's Password is wrong
	 */
	ES_ERRCODE_PW_WRONG,

	/**
	 * Error Code that IP address is not allocated
	 */
	ES_ERRCODE_IP_NOT_ALLOCATED,

	/**
	 * Error Code that there is no Internet connection
	 */
	ES_ERRCODE_NO_INTERNETCONNECTION,

	/**
	 * Error Code that Timeout occurred about wifi provisioning.
	 */
	ES_ERRCODE_TIMEOUT,

	/**
	 * Error Code that cloud server is not reachable due to wrong URL of cloud server, for example.
	 */
	ES_ERRCODE_FAILED_TO_ACCESS_CLOUD_SERVER,

	/**
	 * Error Code that no response is arrived from cloud server
	 */
	ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER,

	/**
	 * Error Code that a delivered authcode is not valid.
	 */
	ES_ERRCODE_INVALID_AUTHCODE,

	/**
	 * Error Code that a given access token is not valid due to its expiration, for example.
	 */
	ES_ERRCODE_INVALID_ACCESSTOKEN,

	/**
	 * Error Code that a refresh of expired access token is failed due to some reasons.
	 */
	ES_ERRCODE_FAILED_TO_REFRESH_ACCESSTOKEN,

	/**
	 * Error Code that a target device is not discovered in cloud server
	 */
	ES_ERRCODE_FAILED_TO_FIND_REGISTERED_DEVICE_IN_CLOUD,

	/**
	 * Error Code that a target user does not exist in cloud server.
	 */
	ES_ERRCODE_FAILED_TO_FIND_REGISTERED_USER_IN_CLOUD,

	/**
	 * Cloud Provisioning Error
	 */
	ER_ERRCODE_INVALID_PROV_PAYLOAD = 100,

	/**
	 * Cloud Data invalid in provisioning.json file.
	 */
	ER_ERRCODE_INVALID_SAVED_CLOUD_DATA = 101,

	/**
	 * Device System Error.
	 */
	ER_ERRCODE_SYSTEM_ERROR = 102,	// System Error of device.

	/**
	 * Error Code that Unknown error occurred
	 */
	ES_ERRCODE_UNKNOWN = 255
} es_error_code_e;

/*
 * * Used HTTP Status Code
 *      200 : OK
 *      201 : Created
 *      400 : Bad Request
 *      401 : Unauthorized
 *      403 : Forbidden
 *      404 : Not Found
 *      500 : Internal Server Error
 *
 *
 * * Serial Number
 *      00xx : Common
 *      01xx : User
 *      02xx : Device
 *      03xx : RD (ResourceDirectory)
 *      04xx : RD Link
 *      05xx : Group
 *      06xx : ACL
 *      07xx : CI (Cloud Interface)
 *      08xx : Push
 *
 * * Sub-Serial Number
 *      xx00 : Not Found.
 *      xx01 : Duplicated.
 *      xx02 : Invalid Param.
 *
 *  * Result Code is made by
 *      HTTP Status Code(xxx) + Serial Number(xxxx)
 */

typedef enum {
	// No error
	ERRCI_NO_ERROR = 0,

	/**
	 * Cloud Error Code
	 */
	// No error
	ERRCI_SUCCESS = 2000000,

	//COMMON
	ERRCI_SAMSUNG_ACCOUNT_AUTHORIZATION_FAILED = 4010001,	// Invalid authcode value.
	ERRCI_TOKEN_VALIDATION_FAILED = 4000002,
	ERRCI_FORBIDDEN = 4030003,
	ERRCI_TOKEN_EXPIRED = 4000004,
	ERRCI_AUTHORIZATION_FAILED = 4000005,
	ERRCI_UNSUPPORTED_REQ = 4000901,
	ERRCI_NOT_ACCEPTABLE = 4060001,

	//USER
	ERRCI_USER_DUPLICATE_PARAM = 4000101,
	ERRCI_USER_INVALID_PARAM = 4000102,
	ERRCI_USER_NOT_FOUND = 4040100,
	ERRCI_UNAUTHORIZED_TOKEN = 4010200,
	ERRCI_SAMSUNG_ACCOUNT_UNAUTHORIZED_TOKEN = 4010201,	// Already registered device for other user.

	//DEVICE
	ERRCI_DEVICE_DUPLICATE_PARAM = 4000201,
	ERRCI_DEVICE_INVALID_PARAM = 4000202,
	ERRCI_DEVICE_NOT_CONNECTED = 4010900,
	ERRCI_DEVICE_NOT_FOUND = 4040200,

	//RD
	ERRCI_RD_DUPLICATE_PARAM = 4000301,
	ERRCI_RD_INVALID_PARAM = 4000302,
	ERRCI_RD_NOT_FOUND = 4040300,

	//RD LINK
	ERRCI_RDLINK_DUPLICATE_PARAM = 4000401,
	ERRCI_RDLINK_INVALID_PARAM = 4000402,
	ERRCI_RDLINK_NOT_FOUND = 4040400,

	//GROUP
	ERRCI_GROUP_DUPLICATE_PARAM = 4000501,
	ERRCI_GROUP_INVALID_PARAM = 4000502,
	ERRCI_GROUP_NOT_FOUND = 4040500,

	//ACL
	ERRCI_ACL_DUPLICATE_PARAM = 4000601,
	ERRCI_ACL_INVALID_PARAM = 4000602,
	ERRCI_ACL_NOT_FOUND = 4040600,

	//CI
	ERRCI_CI_DUPLICATE_PARAM = 4000701,
	ERRCI_CI_INVALID_PARAM = 4000702,
	ERRCI_CI_NOT_FOUND = 4040700,

	//PUSH
	ERRCI_PUSH_DUPLICATE_PARAM = 4000801,
	ERRCI_PUSH_INVALID_PARAM = 4000802,
	ERRCI_PUSH_NOT_FOUND = 4040800,

	//COMMON
	ERRCI_INTERNAL_SERVER_ERROR = 5000000,
	ERRCI_DB_CONNECTION_FAIL = 5000002,

	/**
	* Error Code that Unknown error occurred
	*/
	ERRCI_UNKNOWN = 6000000
} ci_error_code_e;

#endif							//ES_COMMON_H_
