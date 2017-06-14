/* *****************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/

#ifndef OC_PROVISIONING_TYPES_H
#define OC_PROVISIONING_TYPES_H

#include <stdbool.h>
#include "securevirtualresourcetypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Node to construct list of UUID pair list.
 */
typedef struct OCPairList OCPairList_t;
struct OCPairList
{
    OicUuid_t dev;
    OicUuid_t dev2;
    OCPairList_t *next;
};

/**
 * Node to construct UUID linked list.
 */
typedef struct OCUuidList  OCUuidList_t;
struct OCUuidList
{
    OicUuid_t dev;
    OCUuidList_t *next;
};

/*
 * Device's power on/off state.
 */
typedef enum {
    DEV_STATUS_ON = (1 << 0),
    DEV_STATUS_OFF = (1 << 1)
}DeviceStatus;

/**
 * Device Information of discoverd unowned/owned device(s) for provisioning.
 */
typedef struct OCProvisionDev
{
    OCDevAddr       endpoint;        /**< target address **/
    OicSecPstat_t   *pstat;          /**< Pointer to target's pstat resource. **/
    OicSecDoxm_t    *doxm;           /**< Pointer to target's doxm resource. **/
    OCConnectivityType connType;     /**< Connectivity type of endpoint */
    uint16_t        securePort;      /**< secure port **/
#ifdef WITH_TCP
    uint16_t        tcpPort;         /**< tcp port **/
#endif
    char             secVer[OIC_SEC_MAX_VER_LEN];         /**< security version **/
    DeviceStatus    devStatus;       /**< status of device **/
    OCDoHandle    handle;
    struct OCProvisionDev  *next;    /**< Next pointer. **/
}OCProvisionDev_t;

/**
 * Device Information of discoverd direct pairing device(s).
 */
typedef struct OCDirectPairingDev
{
    OCDevAddr               endpoint;
    OCConnectivityType   connType;
    uint16_t                     securePort;
    bool              edp;
    OicSecPrm_t  *prm;
    size_t            prmLen;
    OicUuid_t       deviceID;
    OicUuid_t       rowner;
    struct OCDirectPairingDev *next;
} OCDirectPairingDev_t;

/**
 * Result information for each target device.
 */
typedef struct OCPMResult{
    OicUuid_t      deviceId;
    OCStackResult  res;
}OCProvisionResult_t;

/**
 * Owner device type
 */
typedef enum OwnerType{
    SUPER_OWNER = 0,
    SUB_OWNER = 1
}OwnerType_t;

/**
 * Index value to access OxM allow table
 */
typedef enum OxmAllowTableIdx {
    OXM_IDX_JUST_WORKS = 0,
    OXM_IDX_MV_JUST_WORKS,
#ifdef MULTIPLE_OWNER
    OXM_IDX_PRECONFIG_PIN,
#endif
    OXM_IDX_RANDOM_DEVICE_PIN,
    OXM_IDX_MANUFACTURER_CERTIFICATE,
    OXM_IDX_CON_MFG_CERT,
    OXM_IDX_DECENTRALIZED_PUBLIC_KEY,
    OXM_IDX_COUNT,
    OXM_IDX_UNKNOWN
}OxmAllowTableIdx_t;

/**
 * Callback function definition of provisioning API
 *
 * @param[OUT] ctx - If user set his/her context, it will be returned here.
 * @param[OUT] nOfRes - total number of results, it depends on using which provisioning API.
 * @param[OUT] arr - Array of OCPMResult_t, each OCPMResult_t contains result for target Device.
 * @param[OUT} hasError - If there is no error, it's returned with 'false' but if there is a single
 *                        or more error is/are occured during operation, it will be 'true'.
 */
typedef void (*OCProvisionResultCB)(void* ctx, int nOfRes, OCProvisionResult_t *arr, bool hasError);


/**
 * Callback function definition of direct-pairing
 *
 * @param[OUT] ctx - User context which will be returned wth callback
 * @param[OUT] peer - pairing device info.
 * @param[OUT} result - It's returned with 'OC_STACK_XXX'. It will return 'OC_STACK_OK'
 *                                   if D2D pairing is success without error
 */
typedef void (*OCDirectPairingResultCB)(void *ctx, OCDirectPairingDev_t *peer, OCStackResult result);


#ifdef __cplusplus
}
#endif

#endif //OC_PROVISIONING_TYPES_H
