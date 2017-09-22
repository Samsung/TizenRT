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
#include "experimental/securevirtualresourcetypes.h"

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

/**
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
    uint16_t        tcpSecurePort;   /**< secure tcp port **/
#endif
    char            specVer[SPEC_MAX_VER_LEN];         /**< spec version **/
    DeviceStatus    devStatus;       /**< status of device **/
    OCDoHandle      handle;
    bool            ownerAclUnauthorizedRequest;        /**< true if the provisioning client has already re-tried posting the Owner ACE **/
    struct OCProvisionDev  *next;    /**< Next pointer. **/
}OCProvisionDev_t;

/**
 * Result information for each target device.
 */
typedef struct OCPMResult{
    OicUuid_t      deviceId;
    OCStackResult  res;
}OCProvisionResult_t;

typedef struct OCPMGetCsrResult
{
    OicUuid_t           deviceId;
    OCStackResult       res;
    uint8_t             *csr;
    size_t              csrLen;
    OicEncodingType_t   encoding; /* Determines contents of csr; either OIC_ENCODING_DER or OIC_ENCODING_PEM */
} OCPMGetCsrResult_t;

typedef struct OCPMRoleCertChain
{
    uint64_t            credId;         /**< credential ID */
    OicSecKey_t         certificate;    /**< certificate chain including leaf and intermediate CA certificates */
} OCPMRoleCertChain_t;

typedef struct OCPMGetRolesResult
{
    OicUuid_t           deviceId;       /**< responding device ID */
    OCStackResult       res;            /**< result for this device */
    OCPMRoleCertChain_t *chains;        /**< cert chains (if res is OC_STACK_OK) */
    size_t              chainsLength;   /**< length of chains array (if res is OC_STACK_OK */
} OCPMGetRolesResult_t;

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
 * @param[in] ctx - If user set his/her context, it will be returned here.
 * @param[in] nOfRes - total number of results, it depends on using which provisioning API.
 * @param[in] arr - Array of OCPMResult_t, each OCPMResult_t contains result for target Device.
 * @param[in] hasError - If there is no error, it's returned with 'false' but if there is a single
 *                        or more error is/are occured during operation, it will be 'true'.
 */
typedef void (*OCProvisionResultCB)(void* ctx, size_t nOfRes, OCProvisionResult_t *arr, bool hasError);

/**
 * Callback function definition of CSR retrieve API
 *
 * @param[OUT] ctx - If user set a context, it will be returned here.
 * @param[OUT] nOfRes - total number of results
 * @param[OUT] arr - Array of OCPMGetCsrResult_t, containing one entry for each target device. If an entry's res
 *                   member is OC_STACK_OK, then csr and csrLen are valid; otherwise they should not be used.
 *                   This memory is only valid while the callback is executing; callers must make copies if the data
 *                   needs to be kept longer.
 * @param[OUT] hasError - If all calls succeded, this will be false. One or more errors, and this will
 *                        be true. Examine the elements of arr to discover which failed.
 */
typedef void (*OCGetCSRResultCB)(void* ctx, size_t nOfRes, OCPMGetCsrResult_t *arr, bool hasError);

/**
 * Callback function definition of roles retrieve API
 *
 * @param[OUT] ctx - If user set a context, it will be returned here.
 * @param[OUT] nOfRes - total number of results
 * @param[OUT] arr - Array of OCPMGetRolesResult_t, containing one entry for each target device. If an entry's res
 *                   member is OC_STACK_OK, then chains and chainsLength are valid; otherwise they should not be used.
 *                   This memory is only valid while the callback is executing; callers must make copies if the data
 *                   needs to be kept longer.
 * @param[OUT] hasError - If all calls succeeded, this will be false. One or more errors, and this will
 *                        be true. Examine the elements of arr to discover which failed.
 */
typedef void (*OCGetRolesResultCB)(void* ctx, size_t nOfRes, OCPMGetRolesResult_t *arr, bool hasError);

#ifdef __cplusplus
}
#endif

#endif //OC_PROVISIONING_TYPES_H
