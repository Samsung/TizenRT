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

#ifndef _THINGS_DEF_
#define _THINGS_DEF_

/**
 * Defines used in more than 2 devices are defined here
 * All the other device specific values are defined
 * in the "oicXXdef.h"
 */

#define ST_THINGS_STACK_VERSION      "ST_THINGS_1.0_18.1.30"

/* Resource Types */
#define OIC_INTERFACE_BASELINE          "oic.if.baseline"
#define OIC_INTERFACE_LINKEDLIST        "oic.if.ll"
#define OIC_INTERFACE_LINKEDBATCH       "oic.if.lb"
#define OIC_INTERFACE_BATCH             "oic.if.b"
#define OIC_INTERFACE_ACTUATOR          "oic.if.a"
#define OIC_INTERFACE_SENSOR            "oic.if.s"

#define OIC_RTYPE_COLLECTION_WK         "oic.wk.col"
#define OIC_RTYPE_MAINTENANCE_WK        "oic.wk.mnt"
#define OIC_RTYPE_BINARY_SWITCH         "oic.r.switch.binary"
#define OIC_RTYPE_MODE                  "oic.r.mode"
#define OIC_RTYPE_TEMPERATURE           "oic.r.temperature"
#define OIC_RTYPE_LIGHT_DIMMING         "oic.r.light.dimming"

#define SEC_RTYPE_THINGS_DEVICE         "x.com.samsung.devcol"
#define SEC_RTYPE_TEMPERATURE           "x.com.samsung.da.temperatures"
#define SEC_RTYPE_PROV_INFO             "x.com.samsung.provisioninginfo"

// Common Attribute Keys
#define SEC_ATTRIBUTE_COMMON_ITEMS          "x.com.samsung.items"
#define SEC_ATTRIBUTE_COMMON_ID             "x.com.samsung.id"
#define SEC_ATTRIBUTE_COMMON_TARGET         "x.com.samsung.target"
#define SEC_ATTRIBUTE_COMMON_VALUE          "x.com.samsung.value"
#define SEC_ATTRIBUTE_LOG_COLLECT           "x.com.samsung.collectlogs"
#define SEC_ATTRIBUTE_LOG_IDS               "x.com.samsung.logids"
#define SEC_ATTRIBUTE_FILE_BLOB             "x.com.samsung.blob"
#define SEC_ATTRIBUTE_NAME                  "x.com.samsung.name"

#define SEC_ATTRIBUTE_PROV_TARGETS          "x.com.samsung.provisioning.targets"
#define SEC_ATTRIBUTE_PROV_EZSETDI          "x.com.samsung.provisioning.easysetupdi"
#define SEC_ATTRIBUTE_PROV_RESET            "x.com.samsung.provisioning.reset"
#define SEC_ATTRIBUTE_PROV_TARGET_OWNED     "x.com.samsung.provisioning.owned"
#define SEC_ATTRIBUTE_PROV_TERMINATE_AP     "x.com.samsung.provisioning.stopAP"
#define SEC_ATTRIBUTE_PROV_ABORT            "x.com.samsung.provisioning.abort"
#define SEC_ATTRIBUTE_PROV_TARGET_ID        "x.com.samsung.targetDi"
#define SEC_ATTRIBUTE_PROV_TARGET_RT        "x.com.samsung.targetRt"
#define SEC_ATTRIBUTE_PROV_TARGET_PUBED     "x.com.samsung.published"

#define SEC_ATTRIBUTE_AP_ITEMS              "x.com.samsung.accesspoint.items"
#define SEC_ATTRIBUTE_AP_CHANNAL            "x.com.samsung.channel"
#define SEC_ATTRIBUTE_AP_ENCTYPE            "x.com.samsung.encryptionType"
#define SEC_ATTRIBUTE_AP_MACADDR            "x.com.samsung.macAddress"
#define SEC_ATTRIBUTE_AP_MAXRATE            "x.com.samsung.maxRate"
#define SEC_ATTRIBUTE_AP_RSSI               "x.com.samsung.rssi"
#define SEC_ATTRIBUTE_AP_SECTYPE            "x.com.samsung.securityType"
#define SEC_ATTRIBUTE_AP_SSID               "x.com.samsung.ssid"

#define SEC_ATTRIBUTE_CONTENTS              "x.com.samsung.contents"
#define SEC_ATTRIBUTE_CONTENTS_RT           "x.com.samsung.rt"
#define SEC_ATTRIBUTE_CONTENTS_PROVIDER     "x.com.samsung.provider"
#define SEC_ATTRIBUTE_CONTENTS_HOST         "x.com.samsung.host"
#define SEC_ATTRIBUTE_CONTENTS_PATH         "x.com.samsung.path"
#define SEC_ATTRIBUTE_CONTENTS_QUERY        "x.com.samsung.query"
#define SEC_ATTRIBUTE_CONTENTS_MIME_TYPE    "x.com.samsung.mimeType"
#define SEC_ATTRIBUTE_CONTENTS_TRANSPORT    "x.com.samsung.transport"
#define SEC_ATTRIBUTE_CONTENTS_ENCODING     "x.com.samsung.encoding"
#define SEC_ATTRIBUTE_CONTENTS_PAYLOAD      "x.com.samsung.payload"
#define SEC_ATTRIBUTE_CONTENTS_ACTION       "x.com.samsung.action"
#define SEC_ATTRIBUTE_CONTENTS_CAPA_TYPE    "x.com.samsung.capability.type"
#define SEC_ATTRIBUTE_CONTENTS_DATETIME     "x.com.samsung.datetime"

#define OIC_QUERY_KEY_OP                    "op"
#define OIC_QUERY_VALUE_UPDATE              "update"

/* Resource URI */
#define URI_OIC                         "/oic"
#define URI_SEC                         "/sec"
#define URI_WK_MAINTENANCE              "/mnt"
#define URI_OVEN                        "/oven"
#define URI_DEVICE_COL                  "/device"
#define URI_TEMPERATURE                 "/temperature"
#define URI_TEMPERATURES                "/temperatures"
#define URI_BIXBY                       "/bixby"
#define URI_ACTIONS                     "/actions"
#define URI_ALARMS                      "/alarms"
#define URI_INFORMATION                 "/information"
#define URI_SENSOR                      "/sensors"
#define URI_VENDER_SPECIFIC             "/vs"
#define URI_COOK                        "/cook"
#define URI_DOOR                        "/door"
#define URI_DOORS                       "/doors"
#define URI_FILE                        "/file"
#define URI_LIST                        "/list"
#define URI_TRANSFER                    "/transfer"
#define URI_CONTENTS                    "/contents"
#define URI_PROVIDER                    "/provider"
#define URI_RENDERER                    "/renderer"
#define URI_PROVINFO                    "/provisioninginfo"
#define URI_FIRMWARE                    "/firmware"

/* Temperature Type Resources */
#define URI_FRIDGE_COOLER               "/cooler"
#define URI_FRIDGE_FREEZER              "/freezer"
#define URI_FRIDGE_CVROOM               "/cvroom"
#define URI_OVEN_PROBE                  "/prob"
#define URI_ACCESSPOINTLIST             "/accesspointlist"

/* Notification */
#define URI_PUSHSERVICE                 "/service/pushservice"
#define KEY_MESSAGE                     "message"
#define KEY_EXTRA                       "x.com.samsung.da.notification"
#define KEY_PUSH_CODE                   "x.com.samsung.code"
#define KEY_PUSH_DATA                   "x.com.samsung.data"
#define KEY_PUSH_DATA_ET                "et"
#define KEY_PUSH_DATA_DT                "dt"	//Device Type
#define KEY_PUSH_DATA_RT                "rt"	//Resource Type
#define KEY_PUSH_DATA_DNAME             "dname"	//Device Name
#define KEY_PUSH_DATA_REP               "rep"	//Representation
#define KEY_PUSH_NS_MESSAGE_ID          "x.org.iotivity.ns.messageid"
#define KEY_PUSH_NS_PROVIDER_ID         "x.org.iotivity.ns.providerid"

#define MAX_LEN_FILE_ID                  (10)
#define MAX_NUM_OICINFO                 (200)
#define MAX_NUM_OICFILE                  (10)
#define MAX_NUM_OICCONTENT              (100)
#define MAX_NUM_SCHEDULE                 (15)
#define MAX_NUM_AP                       (50)
#define MAX_NUM_AP                      50

#endif							// _THINGS_DEF_
