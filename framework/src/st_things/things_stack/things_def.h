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

#define SEC_RTYPE_THINGS_DEVICE         "x.com.samsung.devcol"
#define SEC_RTYPE_PROV_INFO             "x.com.samsung.provisioninginfo"

#define SEC_ATTRIBUTE_PROV_TARGETS          "x.com.samsung.provisioning.targets"
#define SEC_ATTRIBUTE_PROV_EZSETDI          "x.com.samsung.provisioning.easysetupdi"
#define SEC_ATTRIBUTE_PROV_RESET            "x.com.samsung.provisioning.reset"
#define SEC_ATTRIBUTE_PROV_TARGET_OWNED     "x.com.samsung.provisioning.owned"
#define SEC_ATTRIBUTE_PROV_TERMINATE_AP     "x.com.samsung.provisioning.stopAP"
#define SEC_ATTRIBUTE_PROV_ABORT            "x.com.samsung.provisioning.abort"
#define SEC_ATTRIBUTE_PROV_TARGET_ID        "x.com.samsung.targetDi"
#define SEC_ATTRIBUTE_PROV_TARGET_RT        "x.com.samsung.targetRt"
#define SEC_ATTRIBUTE_PROV_TARGET_PUBED     "x.com.samsung.published"

/* Resource Attribute's names for AccessPoint List*/
#define SEC_ATTRIBUTE_AP_ITEMS              "x.com.samsung.accesspoint.items"
#define SEC_ATTRIBUTE_AP_MACADDR            "x.com.samsung.macAddress"
#define SEC_ATTRIBUTE_AP_RSSI               "x.com.samsung.rssi"
#define SEC_ATTRIBUTE_AP_SSID               "x.com.samsung.ssid"

/* Resource URI */
#define URI_SEC                         "/sec"
#define URI_PROVINFO                    "/provisioninginfo"
#define URI_DEVICE_COL                  "/device"
#define URI_FIRMWARE                    "/firmware"
#define URI_ACCESSPOINTLIST             "/accesspointlist"

#define DEVICE_OS_VERSION               "1.1"
#define DEVICE_PLATFORM_VERSION         "1.1"
#define DEVICE_HARDWARE_VERSION         "1.0"

#endif							// _THINGS_DEF_
