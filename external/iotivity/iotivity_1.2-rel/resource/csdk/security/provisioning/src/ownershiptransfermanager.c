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

// Defining _POSIX_C_SOURCE macro with 199309L (or greater) as value
// causes header files to expose definitions
// corresponding to the POSIX.1b, Real-time extensions
// (IEEE Std 1003.1b-1993) specification
//
// For this specific file, see use of clock_gettime,
// Refer to http://pubs.opengroup.org/stage7tc1/functions/clock_gettime.html
// and to http://man7.org/linux/man-pages/man2/clock_gettime.2.html
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include "iotivity_config.h"
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <stdbool.h>
#include <string.h>

#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "cacommon.h"
#include "cainterface.h"
#include "base64.h"
#if defined (__TIZENRT__)
#include <json/cJSON.h>
#else
#include "cJSON.h"
#endif
#include "utlist.h"
#include "srmresourcestrings.h"
#include "doxmresource.h"
#include "pstatresource.h"
#include "credresource.h"
#include "aclresource.h"
#include "ownershiptransfermanager.h"
#include "securevirtualresourcetypes.h"
#include "oxmjustworks.h"
#include "oxmrandompin.h"
#include "oxmmanufacturercert.h"
#include "secureresourceprovider.h"

#ifdef MULTIPLE_OWNER
#include "oxmpreconfpin.h"
#endif //MULTIPLE_OWNER
#include "otmcontextlist.h"
#include "pmtypes.h"
#include "pmutility.h"
#include "srmutility.h"
#include "provisioningdatabasemanager.h"
#include "ocpayload.h"
#include "payload_logging.h"
#include "pkix_interface.h"
#include "oxmverifycommon.h"
#include "psinterface.h"

#define TAG "OIC_OTM"


#define ALLOWED_OXM         1
#define NOT_ALLOWED_OXM     0

/**
 * List of allowed oxm list.
 * All oxm methods are allowed as default.
 */
#ifdef MULTIPLE_OWNER
static uint8_t g_OxmAllowStatus[OXM_IDX_COUNT] = {ALLOWED_OXM, ALLOWED_OXM, ALLOWED_OXM,
                                                  ALLOWED_OXM, ALLOWED_OXM, ALLOWED_OXM,
                                                  NOT_ALLOWED_OXM};
#else
static uint8_t g_OxmAllowStatus[OXM_IDX_COUNT] = {ALLOWED_OXM, ALLOWED_OXM, ALLOWED_OXM,
                                                  ALLOWED_OXM, ALLOWED_OXM, NOT_ALLOWED_OXM};
#endif

OCStackResult OTMSetOTCallback(OicSecOxm_t oxm, OTMCallbackData_t* callbacks)
{
    OCStackResult res = OC_STACK_INVALID_PARAM;

    OIC_LOG(INFO, TAG, "IN OTMSetOTCallback");

    VERIFY_NON_NULL(TAG, callbacks, ERROR);

#ifdef MULTIPLE_OWNER
    VERIFY_SUCCESS(TAG, (OIC_OXM_COUNT > oxm || OIC_PRECONFIG_PIN == oxm || OIC_MV_JUST_WORKS == oxm
                    || OIC_CON_MFG_CERT == oxm), ERROR);
#else
    VERIFY_SUCCESS(TAG, (OIC_OXM_COUNT > oxm || OIC_MV_JUST_WORKS == oxm || OIC_CON_MFG_CERT == oxm), ERROR);
#endif // MULTIPLE_OWNER

    switch(oxm)
    {
    case OIC_JUST_WORKS:
        callbacks->loadSecretCB = LoadSecretJustWorksCallback;
        callbacks->createSecureSessionCB = CreateSecureSessionJustWorksCallback;
        callbacks->createSelectOxmPayloadCB = CreateJustWorksSelectOxmPayload;
        callbacks->createOwnerTransferPayloadCB = CreateJustWorksOwnerTransferPayload;
        break;
    case OIC_RANDOM_DEVICE_PIN:
        callbacks->loadSecretCB = InputPinCodeCallback;
        callbacks->createSecureSessionCB = CreateSecureSessionRandomPinCallback;
        callbacks->createSelectOxmPayloadCB = CreatePinBasedSelectOxmPayload;
        callbacks->createOwnerTransferPayloadCB = CreatePinBasedOwnerTransferPayload;
        break;
    case OIC_MANUFACTURER_CERTIFICATE:
        callbacks->loadSecretCB = PrepareMCertificateCallback;
        callbacks->createSecureSessionCB = CreateSecureSessionMCertificateCallback;
        callbacks->createSelectOxmPayloadCB = CreateMCertificateBasedSelectOxmPayload;
        callbacks->createOwnerTransferPayloadCB = CreateMCertificateBasedOwnerTransferPayload;
        break;
    case OIC_DECENTRALIZED_PUBLIC_KEY:
        OIC_LOG(ERROR, TAG, "OIC_DECENTRALIZED_PUBLIC_KEY not supported yet.");
        return OC_STACK_INVALID_METHOD;
#ifdef MULTIPLE_OWNER
    case OIC_PRECONFIG_PIN:
        callbacks->loadSecretCB = LoadPreconfigPinCodeCallback;
        callbacks->createSecureSessionCB = CreateSecureSessionPreconfigPinCallback;
        callbacks->createSelectOxmPayloadCB = CreatePreconfigPinBasedSelectOxmPayload;
        callbacks->createOwnerTransferPayloadCB = CreatePreconfigPinBasedOwnerTransferPayload;
        break;
#endif //MULTIPLE_OWNER
    case OIC_MV_JUST_WORKS:
        callbacks->loadSecretCB = LoadSecretJustWorksCallback;
        callbacks->createSecureSessionCB = CreateSecureSessionJustWorksCallback;
        callbacks->createSelectOxmPayloadCB = CreateMVJustWorksSelectOxmPayload;
        callbacks->createOwnerTransferPayloadCB = CreateJustWorksOwnerTransferPayload;
        break;
    case OIC_CON_MFG_CERT:
        callbacks->loadSecretCB = PrepareMCertificateCallback;
        callbacks->createSecureSessionCB = CreateSecureSessionMCertificateCallback;
        callbacks->createSelectOxmPayloadCB = CreateConMCertificateBasedSelectOxmPayload;
        callbacks->createOwnerTransferPayloadCB = CreateMCertificateBasedOwnerTransferPayload;
        break;
    default:
        OIC_LOG_V(ERROR, TAG, "Unknown OxM : %d", (int)oxm);
        return OC_STACK_INVALID_PARAM;
        break;
    }

    res = OC_STACK_OK;
exit:
    OIC_LOG(INFO, TAG, "OUT OTMSetOTCallback");
    return res;
}

/**
 * Internal API to convert OxM value to index of oxm allow table.
 */
static OxmAllowTableIdx_t GetOxmAllowTableIdx(OicSecOxm_t oxm)
{
    switch(oxm)
    {
        case OIC_JUST_WORKS:
            return OXM_IDX_JUST_WORKS;
        case OIC_RANDOM_DEVICE_PIN:
            return OXM_IDX_RANDOM_DEVICE_PIN;
        case OIC_MANUFACTURER_CERTIFICATE:
            return OXM_IDX_MANUFACTURER_CERTIFICATE;
        case OIC_DECENTRALIZED_PUBLIC_KEY:
            return OXM_IDX_DECENTRALIZED_PUBLIC_KEY;
        case OIC_MV_JUST_WORKS:
            return OXM_IDX_MV_JUST_WORKS;
        case OIC_CON_MFG_CERT:
            return OXM_IDX_CON_MFG_CERT;
#ifdef MULTIPLE_OWNER
        case OIC_PRECONFIG_PIN:
            return OXM_IDX_PRECONFIG_PIN;
#endif
        default:
            return OXM_IDX_UNKNOWN;
    }
}

/**
 * Function to select appropriate  provisioning method.
 *
 * @param[in] supportedMethods   Array of supported methods
 * @param[in] numberOfMethods   number of supported methods
 * @param[out]  selectedMethod         Selected methods
 * @param[in] ownerType type of owner device (SUPER_OWNER or SUB_OWNER)
 * @return  OC_STACK_OK on success
 */
OCStackResult OTMSelectOwnershipTransferMethod(const OicSecOxm_t *supportedMethods,
        size_t numberOfMethods, OicSecOxm_t *selectedMethod, OwnerType_t ownerType)
{
    bool isOxmSelected = false;
    OxmAllowTableIdx_t selectedOxmIdx = OXM_IDX_UNKNOWN;

    OIC_LOG(DEBUG, TAG, "IN SelectProvisioningMethod");

    if (numberOfMethods == 0 || !supportedMethods)
    {
        OIC_LOG(WARNING, TAG, "Could not find a supported OxM.");
        return OC_STACK_ERROR;
    }

    switch(ownerType)
    {
        case SUPER_OWNER:
        {
            for (size_t i = 0; i < numberOfMethods; i++)
            {
                selectedOxmIdx = GetOxmAllowTableIdx(supportedMethods[i]);
                if (OXM_IDX_COUNT <= selectedOxmIdx)
                {
                    OIC_LOG(WARNING, TAG, "Invalid oxm index to access OxM allow table");
                    continue;
                }

#ifdef MULTIPLE_OWNER
                if (ALLOWED_OXM == g_OxmAllowStatus[selectedOxmIdx] &&
                   OXM_IDX_PRECONFIG_PIN != selectedOxmIdx)
#else
                if (ALLOWED_OXM == g_OxmAllowStatus[selectedOxmIdx])
#endif //MULTIPLE_OWNER
                {
                    *selectedMethod  = supportedMethods[i];
                    isOxmSelected = true;
                }
            }
        }
        break;
#ifdef MULTIPLE_OWNER
        case SUB_OWNER:
        {
            for (size_t i = 0; i < numberOfMethods; i++)
            {
                selectedOxmIdx = GetOxmAllowTableIdx(supportedMethods[i]);
                if (OXM_IDX_COUNT <= selectedOxmIdx)
                {
                    OIC_LOG(WARNING, TAG, "Invalid oxm index to access OxM allow table");
                    continue;
                }

                //in case of MOT, only Random PIN & Preconfigured PIN based OxM is allowed
                if (ALLOWED_OXM == g_OxmAllowStatus[selectedOxmIdx] &&
                    (OXM_IDX_RANDOM_DEVICE_PIN == selectedOxmIdx ||
                     OXM_IDX_PRECONFIG_PIN == selectedOxmIdx))
                {
                    *selectedMethod  = supportedMethods[i];
                    isOxmSelected = true;
                }
            }
        }
        break;
#endif
        default:
        {
            OIC_LOG_V(ERROR, TAG, "Unknown owner type or Not supported owner type : %d", ownerType);
            return OC_STACK_INVALID_PARAM;
        }
    }

    if (!isOxmSelected)
    {
        OIC_LOG(ERROR, TAG, "Can not find the allowed OxM.");
        return OC_STACK_NOT_ALLOWED_OXM;
    }

    OIC_LOG(DEBUG, TAG, "OUT SelectProvisioningMethod");

    return OC_STACK_OK;
}

/**
 * Function to select operation mode.This function will return most secure common operation mode.
 *
 * @param[in] selectedDeviceInfo   selected device information to performing provisioning.
 * @param[out]   selectedMode   selected operation mode
 * @return  OC_STACK_OK on success
 */
static void SelectOperationMode(const OCProvisionDev_t *selectedDeviceInfo,
                                OicSecDpom_t *selectedMode)
{
    OIC_LOG(DEBUG, TAG, "IN SelectOperationMode");
    *selectedMode = selectedDeviceInfo->pstat->sm[0];
    OIC_LOG_V(DEBUG, TAG, "Selected Operation Mode = %d", *selectedMode);
}

/**
 * Function to start ownership transfer.
 * This function will send the first request for provisioning,
 * The next request message is sent from the response handler for this request.
 *
 * @param[in] ctx   context value passed to callback from calling function.
 * @param[in] selectedDevice   selected device information to performing provisioning.
 * @return  OC_STACK_OK on success
 */
static OCStackResult StartOwnershipTransfer(void* ctx, OCProvisionDev_t* selectedDevice);

/*
 * Internal function to setup & cleanup PDM to performing provisioning.
 *
 * @param[in] selectedDevice   selected device information to performing provisioning.
 * @return  OC_STACK_OK on success
 */
static OCStackResult SetupPDM(const OCProvisionDev_t* selectedDevice);

/**
 * Function to update owner transfer mode
 *
 * @param[in]  otmCtx  Context value of ownership transfer.
 * @return  OC_STACK_OK on success
 */
static OCStackResult PostOwnerTransferModeToResource(OTMContext_t* otmCtx);

/**
 * Function to send request to resource to get its pstat resource information.
 *
 * @param[in]  otmCtx  Context value of ownership transfer.
 * @return  OC_STACK_OK on success
 */
static OCStackResult GetProvisioningStatusResource(OTMContext_t* otmCtx);


/**
 * Function to send  uuid of owner device to new device.
 * This function would update 'owner of doxm' as UUID for provisioning tool.
 *
 * @param[in]  otmCtx  Context value of ownership transfer.
 * @return  OC_STACK_OK on success
 */
static OCStackResult PostOwnerUuid(OTMContext_t* otmCtx);

/**
 * Function to update the operation mode. As per the spec. Operation mode in client driven
 * single service provisioning it will be updated to 0x3
 *
 * @param[in]  otmCtx  Context value of ownership transfer.
 * @return  OC_STACK_OK on success
 */
static OCStackResult PostUpdateOperationMode(OTMContext_t* otmCtx);

/**
 * Function to update the owner credential to new device
 *
 * @param[in]  otmCtx  Context value of ownership transfer.
 * @param[in] selectedOperationMode selected operation mode
 * @return  OC_STACK_OK on success
 */
static OCStackResult PostOwnerCredential(OTMContext_t* otmCtx);

/**
 * Function to update the owner ACL to new device.
 *
 * @param[in]  otmCtx  Context value of ownership transfer.
 * @return  OC_STACK_OK on success
 */
static OCStackResult PostOwnerAcl(OTMContext_t* otmCtx);

/**
 * Function to send ownerShip info.
 * This function would update 'owned of doxm' as true.
 *
 * @param[in]  otmCtx  Context value of ownership transfer.
 * @return  OC_STACK_OK on success
 */
static OCStackResult PostOwnershipInformation(OTMContext_t* otmCtx);

/**
 * Function to update pstat as Ready for provisioning.
 * This function would update 'cm' from bx0000,0010 to bx0000,0000.
 *
 * @param[in] ctx   context value passed to callback from calling function.
 * @param[in] selectedDevice   selected device information to performing provisioning.
 * @return  OC_STACK_OK on success
 */
static OCStackResult PostProvisioningStatus(OTMContext_t* otmCtx);

/**
 * Function to update pstat as Ready for Normal Operation.
 * This function would update 'isop' from false to true.
 *
 * @param[in] ctx   context value passed to callback from calling function.
 * @param[in] selectedDevice   selected device information to performing provisioning.
 * @return  OC_STACK_OK on success
 */
static OCStackResult PostNormalOperationStatus(OTMContext_t* otmCtx);

static bool IsComplete(OTMContext_t* otmCtx)
{
    for(size_t i = 0; i < otmCtx->ctxResultArraySize; i++)
    {
        if(OC_STACK_CONTINUE == otmCtx->ctxResultArray[i].res)
        {
            return false;
        }
    }

    return true;
}

/**
 * Function to save the result of provisioning.
 *
 * @param[in,out] otmCtx   Context value of ownership transfer.
 * @param[in] res   result of provisioning
 */
static void SetResult(OTMContext_t* otmCtx, const OCStackResult res)
{
    OIC_LOG_V(DEBUG, TAG, "IN SetResult : %d ", res);

    if(NULL == otmCtx || NULL == otmCtx->selectedDeviceInfo
            || NULL == otmCtx->selectedDeviceInfo->doxm)
    {
        OIC_LOG(WARNING, TAG, "OTMContext is NULL");
        return;
    }

    //If OTM Context was removed from previous response handler, just exit the current OTM process.
    if(NULL == GetOTMContext(otmCtx->selectedDeviceInfo->endpoint.addr,
                             otmCtx->selectedDeviceInfo->securePort))
    {
        OIC_LOG(WARNING, TAG, "Current OTM Process has already ended.");
    }

    //Revert psk_info callback and new deivce uuid in case of random PIN OxM
    if(OIC_RANDOM_DEVICE_PIN == otmCtx->selectedDeviceInfo->doxm->oxmSel)
    {
        if(CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskCredentials))
        {
            OIC_LOG(WARNING, TAG, "Failed to revert  is DTLS credential handler.");
        }
        OicUuid_t emptyUuid = { .id={0}};
        SetUuidForPinBasedOxm(&emptyUuid);
    }
    else if(OIC_MANUFACTURER_CERTIFICATE == otmCtx->selectedDeviceInfo->doxm->oxmSel ||
                        OIC_CON_MFG_CERT == otmCtx->selectedDeviceInfo->doxm->oxmSel)
    {
        //Revert back certificate related callbacks.
        if(CA_STATUS_OK != CAregisterPkixInfoHandler(GetPkixInfo))
        {
            OIC_LOG(WARNING, TAG, "Failed to revert PkixInfoHandler.");
        }
        if(CA_STATUS_OK != CAregisterGetCredentialTypesHandler(InitCipherSuiteList))
        {
            OIC_LOG(WARNING, TAG, "Failed to revert CredentialTypesHandler.");
        }
    }

    for(size_t i = 0; i < otmCtx->ctxResultArraySize; i++)
    {
        if(memcmp(otmCtx->selectedDeviceInfo->doxm->deviceID.id,
                  otmCtx->ctxResultArray[i].deviceId.id, UUID_LENGTH) == 0)
        {
            otmCtx->ctxResultArray[i].res = res;
            if(OC_STACK_OK != res && OC_STACK_CONTINUE != res && OC_STACK_DUPLICATE_REQUEST != res)
            {
                otmCtx->ctxHasError = true;
                if (OC_STACK_OK != PDMDeleteDevice(&otmCtx->ctxResultArray[i].deviceId))
                {
                    OIC_LOG(WARNING, TAG, "Internal error in PDMDeleteDevice");
                }
                CAEndpoint_t endpoint;
                memcpy(&endpoint, &(otmCtx->selectedDeviceInfo->endpoint), sizeof(CAEndpoint_t));
                endpoint.port = otmCtx->selectedDeviceInfo->securePort;
                if (CA_STATUS_OK != CAcloseSslConnection(&endpoint))
                {
                    OIC_LOG(WARNING, TAG, "Failed to close Secure session");
                }
            }
        }
    }

    //In case of duplicated OTM process, OTMContext and OCDoHandle should not be removed.
    if(OC_STACK_DUPLICATE_REQUEST != res)
    {
        //Remove the current OTM Context from OTM queue
        RemoveOTMContext(otmCtx->selectedDeviceInfo->endpoint.addr,
                         otmCtx->selectedDeviceInfo->securePort);

        //If there is a request being performed, cancel it to prevent retransmission.
        if(otmCtx->ocDoHandle)
        {
            OIC_LOG_V(DEBUG, TAG, "OCCancel - %s : %d",
                    otmCtx->selectedDeviceInfo->endpoint.addr,
                    otmCtx->selectedDeviceInfo->securePort);
            if(OC_STACK_OK != OCCancel(otmCtx->ocDoHandle, OC_HIGH_QOS, NULL, 0))
            {
                OIC_LOG(WARNING, TAG, "Failed to remove registered callback");
            }
            else
            {
                otmCtx->ocDoHandle = NULL;
            }
        }
    }

    //If all OTM process is complete, invoke the user callback.
    if(IsComplete(otmCtx))
    {
        if(OC_STACK_OK != res && OC_STACK_CONTINUE != res && OC_STACK_DUPLICATE_REQUEST != res)
        {
            // Reset doxm and pstat properties to pre-Ownership Transfer state
            OIC_LOG(DEBUG, TAG, "Resetting doxm and pstat properties");
            if(otmCtx->selectedDeviceInfo->doxm)
            {
                OicUuid_t emptyUuid = {.id = {0}};
                memcpy(&(otmCtx->selectedDeviceInfo->doxm->owner), &emptyUuid, sizeof(OicUuid_t));
                otmCtx->selectedDeviceInfo->doxm->owned = false;
            }
            if(otmCtx->selectedDeviceInfo->pstat)
            {
                otmCtx->selectedDeviceInfo->pstat->isOp = false;
                otmCtx->selectedDeviceInfo->pstat->cm |= TAKE_OWNER;
            }
        }

        otmCtx->ctxResultCallback(otmCtx->userCtx, otmCtx->ctxResultArraySize,
                                   otmCtx->ctxResultArray, otmCtx->ctxHasError);
        OICFree(otmCtx->ctxResultArray);
        OICFree(otmCtx);
    }
    else
    {
        if(OC_STACK_OK != StartOwnershipTransfer(otmCtx,
                                                 otmCtx->selectedDeviceInfo->next))
        {
            OIC_LOG(ERROR, TAG, "Failed to StartOwnershipTransfer");
        }
    }

    OIC_LOG(DEBUG, TAG, "OUT SetResult");
}

/**
 * Function to handle the handshake result in OTM.
 * This function will be invoked after DTLS handshake
 * @param   endPoint  [IN] The remote endpoint.
 * @param   errorInfo [IN] Error information from the endpoint.
 * @return  NONE
 */
void DTLSHandshakeCB(const CAEndpoint_t *endpoint, const CAErrorInfo_t *info)
{
    OIC_LOG(DEBUG, TAG, "IN DTLSHandshakeCB");
    if(NULL != endpoint && NULL != info)
    {
        OIC_LOG_V(INFO, TAG, "Received status from remote device(%s:%d) : %d",
                 endpoint->addr, endpoint->port, info->result);

        OTMContext_t* otmCtx = GetOTMContext(endpoint->addr, endpoint->port);
        if(otmCtx)
        {
            OicSecDoxm_t* newDevDoxm = otmCtx->selectedDeviceInfo->doxm;
            if(NULL != newDevDoxm)
            {
                OicUuid_t emptyUuid = {.id={0}};

                //Make sure the address matches.
                if(strncmp(otmCtx->selectedDeviceInfo->endpoint.addr,
                   endpoint->addr,
                   sizeof(endpoint->addr)) == 0 &&
                   otmCtx->selectedDeviceInfo->securePort == endpoint->port)
                {
                    OCStackResult res = OC_STACK_ERROR;

                    //If temporal secure sesstion established successfully
                    if(CA_STATUS_OK == info->result &&
                       false == newDevDoxm->owned &&
                       memcmp(&(newDevDoxm->owner), &emptyUuid, sizeof(OicUuid_t)) == 0)
                    {
                        //In case of Mutual Verified Just-Works, display mutualVerifNum
                        if (OIC_MV_JUST_WORKS == newDevDoxm->oxmSel)
                        {
                            uint8_t preMutualVerifNum[OWNER_PSK_LENGTH_128] = {0};
                            uint8_t mutualVerifNum[MUTUAL_VERIF_NUM_LEN] = {0};
                            OicUuid_t deviceID = {.id = {0}};

                            //Generate mutualVerifNum
                            char label[LABEL_LEN] = {0};
                            snprintf(label, LABEL_LEN, "%s%s", MUTUAL_VERIF_NUM, OXM_MV_JUST_WORKS);
                            res = GetDoxmDeviceID(&deviceID);
                            if (OC_STACK_OK != res)
                            {
                                OIC_LOG(ERROR, TAG, "Error while retrieving Owner's device ID");
                                SetResult(otmCtx, res);
                                return;
                            }

                            CAResult_t pskRet = CAGenerateOwnerPSK(endpoint,
                                    (uint8_t *)label,
                                    strlen(label),
                                    deviceID.id, sizeof(deviceID.id),
                                    newDevDoxm->deviceID.id, sizeof(newDevDoxm->deviceID.id),
                                    preMutualVerifNum, OWNER_PSK_LENGTH_128);
                            if (CA_STATUS_OK != pskRet)
                            {
                                OIC_LOG(WARNING, TAG, "Failed to remove the invaild owner credential");
                                SetResult(otmCtx, OC_STACK_ERROR);
                                return;
                            }

                            memcpy(mutualVerifNum, preMutualVerifNum + OWNER_PSK_LENGTH_128 - sizeof(mutualVerifNum),
                                    sizeof(mutualVerifNum));
                            res = VerifyOwnershipTransfer(mutualVerifNum, DISPLAY_NUM);
                            if (OC_STACK_OK != res)
                            {
                                OIC_LOG(ERROR, TAG, "Error while displaying mutualVerifNum");
                                SetResult(otmCtx, res);
                                return;
                            }
                        }
                        //In case of confirmed manufacturer cert, display message
                        else if (OIC_MANUFACTURER_CERTIFICATE == newDevDoxm->oxmSel || OIC_CON_MFG_CERT == newDevDoxm->oxmSel)
                        {
                            res = VerifyOwnershipTransfer(NULL, DISPLAY_NUM);
                            if (OC_STACK_OK != res)
                            {
                                OIC_LOG(ERROR, TAG, "Error while displaying message");
                                SetResult(otmCtx, res);
                                return;
                            }
                        }

                        //Send request : POST /oic/sec/doxm [{... , "devowner":"PT's UUID"}]
                        res = PostOwnerUuid(otmCtx);
                        if(OC_STACK_OK != res)
                        {
                            OIC_LOG(ERROR, TAG, "OperationModeUpdate : Failed to send owner information");
                            SetResult(otmCtx, res);
                        }
                    }
                    //In case of authentication failure
                    else if(CA_DTLS_AUTHENTICATION_FAILURE == info->result)
                    {
                        //in case of error from owner credential
                        if(memcmp(&(newDevDoxm->owner), &emptyUuid, sizeof(OicUuid_t)) != 0 &&
                            true == newDevDoxm->owned)
                        {
                            OIC_LOG(ERROR, TAG, "The owner credential may incorrect.");

                            if(OC_STACK_OK != RemoveCredential(&(newDevDoxm->deviceID)))
                            {
                                OIC_LOG(WARNING, TAG, "Failed to remove the invaild owner credential");
                            }
                            SetResult(otmCtx, OC_STACK_AUTHENTICATION_FAILURE);
                        }
                        //in case of error from wrong PIN, re-start the ownership transfer
                        else if(OIC_RANDOM_DEVICE_PIN == newDevDoxm->oxmSel)
                        {
                            OIC_LOG(ERROR, TAG, "The PIN number may incorrect.");

                            memcpy(&(newDevDoxm->owner), &emptyUuid, sizeof(OicUuid_t));
                            newDevDoxm->owned = false;
                            otmCtx->attemptCnt++;

                            RemoveOTMContext(otmCtx->selectedDeviceInfo->endpoint.addr,
                                             otmCtx->selectedDeviceInfo->securePort);

                            // In order to re-start ownership transfer, device information should be deleted from PDM.
                            res = PDMDeleteDevice(&(otmCtx->selectedDeviceInfo->doxm->deviceID));
                            if (OC_STACK_OK != res)
                            {
                                SetResult(otmCtx, res);
                                OIC_LOG(ERROR, TAG, "Failed to PDMDeleteDevice");
                            }
                            else
                            {
                                if(WRONG_PIN_MAX_ATTEMP > otmCtx->attemptCnt)
                                {
                                    res = StartOwnershipTransfer(otmCtx, otmCtx->selectedDeviceInfo);
                                    if(OC_STACK_OK != res)
                                    {
                                        SetResult(otmCtx, res);
                                        OIC_LOG(ERROR, TAG, "Failed to Re-StartOwnershipTransfer");
                                    }
                                }
                                else
                                {
                                    OIC_LOG(ERROR, TAG, "User has exceeded the number of authentication attempts.");
                                    SetResult(otmCtx, OC_STACK_AUTHENTICATION_FAILURE);
                                }
                            }
                        }
                        else
                        {
                            OIC_LOG(ERROR, TAG, "Failed to establish secure session.");
                            SetResult(otmCtx, OC_STACK_AUTHENTICATION_FAILURE);
                        }
                    }
                }
            }
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Can not find the OTM Context.");
        }
    }
    OIC_LOG(DEBUG, TAG, "OUT DTLSHandshakeCB");
}

/**
 * Function to save the Owner/SubOwner PSK.
 *
 * @param[in] selectedDeviceInfo   selected device information to performing provisioning.
 * @return  OC_STACK_OK on success
 */
static OCStackResult SaveOwnerPSK(OCProvisionDev_t *selectedDeviceInfo)
{
    OIC_LOG(DEBUG, TAG, "IN SaveOwnerPSK");

    OCStackResult res = OC_STACK_ERROR;

    CAEndpoint_t endpoint;
    memset(&endpoint, 0x00, sizeof(CAEndpoint_t));
    OICStrcpy(endpoint.addr, MAX_ADDR_STR_SIZE_CA, selectedDeviceInfo->endpoint.addr);
    endpoint.addr[MAX_ADDR_STR_SIZE_CA - 1] = '\0';
    endpoint.port = selectedDeviceInfo->securePort;
    endpoint.adapter = selectedDeviceInfo->endpoint.adapter;

    OicUuid_t ownerDeviceID = {.id={0}};
    if (OC_STACK_OK != GetDoxmDeviceID(&ownerDeviceID))
    {
        OIC_LOG(ERROR, TAG, "Error while retrieving Owner's device ID");
        return res;
    }

    uint8_t ownerPSK[OWNER_PSK_LENGTH_128] = {0};
    OicSecKey_t ownerKey = {.data=ownerPSK, .len=OWNER_PSK_LENGTH_128, .encoding=OIC_ENCODING_RAW};

    //Generating OwnerPSK
    CAResult_t pskRet = CAGenerateOwnerPSK(&endpoint,
            (uint8_t *)GetOxmString(selectedDeviceInfo->doxm->oxmSel),
            strlen(GetOxmString(selectedDeviceInfo->doxm->oxmSel)),
            ownerDeviceID.id, sizeof(ownerDeviceID.id),
            selectedDeviceInfo->doxm->deviceID.id, sizeof(selectedDeviceInfo->doxm->deviceID.id),
            ownerPSK, OWNER_PSK_LENGTH_128);

    if (CA_STATUS_OK == pskRet)
    {
        OIC_LOG(DEBUG, TAG,"Owner PSK dump:\n");
        OIC_LOG_BUFFER(DEBUG, TAG,ownerPSK, OWNER_PSK_LENGTH_128);
        //Generating new credential for provisioning tool
        OicSecCred_t *cred = GenerateCredential(&selectedDeviceInfo->doxm->deviceID,
                                  SYMMETRIC_PAIR_WISE_KEY, NULL,
                                  &ownerKey, &ownerDeviceID, NULL);
        OICClearMemory(ownerPSK, sizeof(ownerPSK));
        VERIFY_NON_NULL(TAG, cred, ERROR);

        uint32_t outSize = 0;
        size_t b64BufSize = B64ENCODE_OUT_SAFESIZE((OWNER_PSK_LENGTH_128 + 1));
        char* b64Buf = (char *)OICCalloc(1, b64BufSize);
        VERIFY_NON_NULL(TAG, b64Buf, ERROR);
        b64Encode(cred->privateData.data, cred->privateData.len, b64Buf, b64BufSize, &outSize);

        OICFree( cred->privateData.data );
        cred->privateData.data = (uint8_t *)OICCalloc(1, outSize + 1);
        VERIFY_NON_NULL(TAG, cred->privateData.data, ERROR);

        strncpy((char*)(cred->privateData.data), b64Buf, outSize);
        cred->privateData.data[outSize] = '\0';
        cred->privateData.encoding = OIC_ENCODING_BASE64;
        cred->privateData.len = outSize;
        OICFree(b64Buf);

        //Finding previous ownerPSK.
        const OicSecCred_t* credList = GetCredList();
        const OicSecCred_t* prevCred = NULL;
        uint16_t credId = 0;
        LL_FOREACH(credList, prevCred)
        {
            //OwnerPSK's type is SYMMETRIC_PAIR_WISE_KEY
            if (SYMMETRIC_PAIR_WISE_KEY == prevCred->credType &&
                0 == memcmp(prevCred->subject.id, cred->subject.id, sizeof(cred->subject.id)))
            {
                credId = prevCred->credId;
                break;
            }
        }

        //If duplicate owner PSK is exists, remove it.
        if(0 < credId)
        {
            OIC_LOG(WARNING, TAG, "Duplicate OwnerPSK was detected.");
            OIC_LOG(WARNING, TAG, "[Subject] : ");
            OIC_LOG_BUFFER(WARNING, TAG, prevCred->subject.id, sizeof(prevCred->subject.id));
            OIC_LOG_V(WARNING, TAG, "[Encoding Type] : %d", prevCred->privateData.encoding);
            OIC_LOG(DEBUG, TAG, "[Private Data] : ");
            OIC_LOG_BUFFER(DEBUG, TAG, prevCred->privateData.data, prevCred->privateData.len);
            OIC_LOG(WARNING, TAG, "Previous OwnerPSK will be removed.");

            res = RemoveCredentialByCredId(credId);
            if(OC_STACK_RESOURCE_DELETED != res)
            {
                OIC_LOG(ERROR, TAG, "Failed to remove the previous OwnerPSK");
                DeleteCredList(cred);
                goto exit;
            }
        }

        res = AddCredential(cred);
        if(res != OC_STACK_OK)
        {
            DeleteCredList(cred);
            return res;
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "CAGenerateOwnerPSK failed");
    }

    OIC_LOG(DEBUG, TAG, "OUT SaveOwnerPSK");
exit:
    return res;
}

/**
 * Callback handler for OwnerShipTransferModeHandler API.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult OwnerTransferModeHandler(void *ctx, OCDoHandle UNUSED,
                                                         OCClientResponse *clientResponse)
{
    OIC_LOG(DEBUG, TAG, "IN OwnerTransferModeHandler");

    VERIFY_NON_NULL(TAG, clientResponse, WARNING);
    VERIFY_NON_NULL(TAG, ctx, WARNING);

    OTMContext_t* otmCtx = (OTMContext_t*)ctx;
    otmCtx->ocDoHandle = NULL;
    (void)UNUSED;
    if (OC_STACK_RESOURCE_CHANGED == clientResponse->result)
    {
        OIC_LOG(INFO, TAG, "OwnerTransferModeHandler : response result = OC_STACK_OK");
        //Send request : GET /oic/sec/pstat
        OCStackResult res = GetProvisioningStatusResource(otmCtx);
        if(OC_STACK_OK != res)
        {
            OIC_LOG(WARNING, TAG, "Failed to get pstat information");
            SetResult(otmCtx, res);
        }
    }
    else
    {
        OIC_LOG_V(WARNING, TAG, "OwnerTransferModeHandler : Client response is incorrect : %d",
        clientResponse->result);
        SetResult(otmCtx, clientResponse->result);
    }

    OIC_LOG(DEBUG, TAG, "OUT OwnerTransferModeHandler");

exit:
    return  OC_STACK_DELETE_TRANSACTION;
}

/**
 * Callback handler for ProvisioningStatusResouceHandler API.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult ListMethodsHandler(void *ctx, OCDoHandle UNUSED,
                                                    OCClientResponse *clientResponse)
{
    OIC_LOG(DEBUG, TAG, "IN ListMethodsHandler");

    VERIFY_NON_NULL(TAG, clientResponse, WARNING);
    VERIFY_NON_NULL(TAG, ctx, WARNING);

    OTMContext_t* otmCtx = (OTMContext_t*)ctx;
    otmCtx->ocDoHandle = NULL;
    (void)UNUSED;
    if  (OC_STACK_OK == clientResponse->result)
    {
        if  (NULL == clientResponse->payload)
        {
            OIC_LOG(INFO, TAG, "Skiping Null payload");
            SetResult(otmCtx, OC_STACK_ERROR);
            return OC_STACK_DELETE_TRANSACTION;
        }

        if (PAYLOAD_TYPE_SECURITY != clientResponse->payload->type)
        {
            OIC_LOG(INFO, TAG, "Unknown payload type");
            SetResult(otmCtx, OC_STACK_ERROR);
            return OC_STACK_DELETE_TRANSACTION;
        }
        OicSecPstat_t* pstat = NULL;
        OCStackResult result = CBORPayloadToPstat(
                ((OCSecurityPayload*)clientResponse->payload)->securityData,
                ((OCSecurityPayload*)clientResponse->payload)->payloadSize,
                &pstat);
        if(NULL == pstat || result != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "Error while converting cbor to pstat.");
            SetResult(otmCtx, OC_STACK_ERROR);
            return OC_STACK_DELETE_TRANSACTION;
        }
        if(false == (TAKE_OWNER & pstat->cm))
        {
            OIC_LOG(ERROR, TAG, "Device pairing mode enabling owner transfer operations is disabled");
            SetResult(otmCtx, OC_STACK_ERROR);
            return OC_STACK_DELETE_TRANSACTION;
        }
        otmCtx->selectedDeviceInfo->pstat = pstat;

        //Select operation mode (Currently supported SINGLE_SERVICE_CLIENT_DRIVEN only)
        SelectOperationMode(otmCtx->selectedDeviceInfo, &(otmCtx->selectedDeviceInfo->pstat->om));

        //Send request : POST /oic/sec/pstat [{"om":"bx11", .. }]
        OCStackResult res = PostUpdateOperationMode(otmCtx);
        if (OC_STACK_OK != res)
        {
            OIC_LOG(ERROR, TAG, "Error while updating operation mode.");
            SetResult(otmCtx, res);
        }
    }
    else
    {
        OIC_LOG_V(WARNING, TAG, "ListMethodsHandler : Client response is incorrect : %d",
            clientResponse->result);
        SetResult(otmCtx, clientResponse->result);
    }

    OIC_LOG(DEBUG, TAG, "OUT ListMethodsHandler");
exit:
    return  OC_STACK_DELETE_TRANSACTION;
}

/**
 * Response handler for update owner uuid request.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult OwnerUuidUpdateHandler(void *ctx, OCDoHandle UNUSED,
                                OCClientResponse *clientResponse)
{
    VERIFY_NON_NULL(TAG, clientResponse, WARNING);
    VERIFY_NON_NULL(TAG, ctx, WARNING);

    OIC_LOG(DEBUG, TAG, "IN OwnerUuidUpdateHandler");
    (void)UNUSED;
    OCStackResult res = OC_STACK_OK;
    OTMContext_t* otmCtx = (OTMContext_t*)ctx;
    otmCtx->ocDoHandle = NULL;

    if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
    {
        if(otmCtx && otmCtx->selectedDeviceInfo)
        {
            //In case of Mutual Verified Just-Works, wait for user confirmation
            if (OIC_MV_JUST_WORKS == otmCtx->selectedDeviceInfo->doxm->oxmSel)
            {
                res = VerifyOwnershipTransfer(NULL, USER_CONFIRM);
                if (OC_STACK_OK != res)
                {
                    if (OC_STACK_OK != SRPResetDevice(otmCtx->selectedDeviceInfo, otmCtx->ctxResultCallback))
                    {
                        OIC_LOG(WARNING, TAG, "OwnerUuidUpdateHandler : SRPResetDevice error");
                    }
                    OIC_LOG(ERROR, TAG, "OwnerUuidUpdateHandler:Failed to verify user confirm");
                    SetResult(otmCtx, res);
                    return OC_STACK_DELETE_TRANSACTION;
                }
            }

            res = SaveOwnerPSK(otmCtx->selectedDeviceInfo);
            if(OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG, "OwnerUuidUpdateHandler:Failed to owner PSK generation");
                SetResult(otmCtx, res);
                return OC_STACK_DELETE_TRANSACTION;
            }

            //POST owner credential to new device according to security spec B.
            res = PostOwnerCredential(otmCtx);
            if(OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG,
                        "OwnerUuidUpdateHandler:Failed to send PosT request for onwer credential");
                SetResult(otmCtx, res);
                return OC_STACK_DELETE_TRANSACTION;
            }
        }
    }
    else
    {
        if (((OIC_MANUFACTURER_CERTIFICATE == otmCtx->selectedDeviceInfo->doxm->oxmSel) ||
            (OIC_CON_MFG_CERT == otmCtx->selectedDeviceInfo->doxm->oxmSel)) &&
                    OC_STACK_NOT_ACCEPTABLE == clientResponse->result)
        {
            res = OC_STACK_USER_DENIED_REQ;
        }
        else
        {
            res = clientResponse->result;
        }
        OIC_LOG_V(ERROR, TAG, "OwnerUuidHandler : Unexpected result %d", res);
        SetResult(otmCtx, res);
    }

    OIC_LOG(DEBUG, TAG, "OUT OwnerUuidUpdateHandler");

exit:
    return  OC_STACK_DELETE_TRANSACTION;
}

/**
 * Response handler for update operation mode.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult OperationModeUpdateHandler(void *ctx, OCDoHandle UNUSED,
                                OCClientResponse *clientResponse)
{
    OIC_LOG(DEBUG, TAG, "IN OperationModeUpdateHandler");

    VERIFY_NON_NULL(TAG, clientResponse, WARNING);
    VERIFY_NON_NULL(TAG, ctx, WARNING);

    OTMContext_t* otmCtx = (OTMContext_t*)ctx;
    otmCtx->ocDoHandle = NULL;
    (void) UNUSED;
    if  (OC_STACK_RESOURCE_CHANGED == clientResponse->result)
    {
        OCStackResult res = OC_STACK_ERROR;

        //DTLS Handshake
        //Load secret for temporal secure session.
        if(otmCtx->otmCallback.loadSecretCB)
        {
            res = otmCtx->otmCallback.loadSecretCB(otmCtx);
            if(OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG, "OperationModeUpdate : Failed to load secret");
                SetResult(otmCtx, res);
                return  OC_STACK_DELETE_TRANSACTION;
            }
        }

        //Save the current context instance to use on the dtls handshake callback
        if(OC_STACK_OK != AddOTMContext(otmCtx,
                                         otmCtx->selectedDeviceInfo->endpoint.addr,
                                         otmCtx->selectedDeviceInfo->securePort))
        {
            OIC_LOG(ERROR, TAG, "OperationModeUpdate : Failed to add OTM Context into OTM List.");
            SetResult(otmCtx, res);
            return OC_STACK_DELETE_TRANSACTION;
        }

        //Try DTLS handshake to generate secure session
        if(otmCtx->otmCallback.createSecureSessionCB)
        {
            res = otmCtx->otmCallback.createSecureSessionCB(otmCtx);
            if(OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG, "OperationModeUpdate : Failed to create DTLS session");
                SetResult(otmCtx, res);
                return OC_STACK_DELETE_TRANSACTION;
            }
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Error while update operation mode");
        SetResult(otmCtx, clientResponse->result);
    }

    OIC_LOG(DEBUG, TAG, "OUT OperationModeUpdateHandler");

exit:
    return  OC_STACK_DELETE_TRANSACTION;
}

/**
 * Response handler for update owner crendetial request.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult OwnerCredentialHandler(void *ctx, OCDoHandle UNUSED,
                                OCClientResponse *clientResponse)
{
    VERIFY_NON_NULL(TAG, clientResponse, WARNING);
    VERIFY_NON_NULL(TAG, ctx, WARNING);

    OIC_LOG(DEBUG, TAG, "IN OwnerCredentialHandler");
    (void)UNUSED;
    OCStackResult res = OC_STACK_OK;
    OTMContext_t* otmCtx = (OTMContext_t*)ctx;
    otmCtx->ocDoHandle = NULL;

    if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
    {
        if(otmCtx && otmCtx->selectedDeviceInfo)
        {
            //Close the temporal secure session to verify the owner credential
            CAEndpoint_t* endpoint = (CAEndpoint_t *)&otmCtx->selectedDeviceInfo->endpoint;
            endpoint->port = otmCtx->selectedDeviceInfo->securePort;
            CAResult_t caResult = CA_STATUS_OK;
            caResult = CAcloseSslConnection(endpoint);

            if(CA_STATUS_OK != caResult)
            {
                OIC_LOG(ERROR, TAG, "Failed to close DTLS session");
                SetResult(otmCtx, caResult);
                return OC_STACK_DELETE_TRANSACTION;
            }

            /**
             * If we select NULL cipher,
             * client will select appropriate cipher suite according to server's cipher-suite list.
             */
            // TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA_256 = 0xC037, /**< see RFC 5489 */
            caResult = CASelectCipherSuite(0xC037, endpoint->adapter);
            if(CA_STATUS_OK != caResult)
            {
                OIC_LOG(ERROR, TAG, "Failed to select TLS_NULL_WITH_NULL_NULL");
                SetResult(otmCtx, caResult);
                return OC_STACK_DELETE_TRANSACTION;
            }

            /**
             * in case of random PIN based OxM,
             * revert get_psk_info callback of tinyDTLS to use owner credential.
             */
            if(OIC_RANDOM_DEVICE_PIN == otmCtx->selectedDeviceInfo->doxm->oxmSel)
            {
                OicUuid_t emptyUuid = { .id={0}};
                SetUuidForPinBasedOxm(&emptyUuid);

                caResult = CAregisterPskCredentialsHandler(GetDtlsPskCredentials);

                if(CA_STATUS_OK != caResult)
                {
                    OIC_LOG(ERROR, TAG, "Failed to revert DTLS credential handler.");
                    SetResult(otmCtx, OC_STACK_INVALID_CALLBACK);
                    return OC_STACK_DELETE_TRANSACTION;
                }
            }
#ifdef __WITH_TLS__
           otmCtx->selectedDeviceInfo->connType |= CT_FLAG_SECURE;
#endif
            res = PostOwnerAcl(otmCtx);
            if(OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG, "Failed to update owner ACL to new device");
                SetResult(otmCtx, res);
                return OC_STACK_DELETE_TRANSACTION;
            }
        }
    }
    else
    {
        res = clientResponse->result;
        OIC_LOG_V(ERROR, TAG, "OwnerCredentialHandler : Unexpected result %d", res);
        SetResult(otmCtx, res);
    }

    OIC_LOG(DEBUG, TAG, "OUT OwnerCredentialHandler");

exit:
    return  OC_STACK_DELETE_TRANSACTION;
}

/**
 * Response handler for update owner ACL request.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult OwnerAclHandler(void *ctx, OCDoHandle UNUSED,
                                OCClientResponse *clientResponse)
{
    VERIFY_NON_NULL(TAG, clientResponse, WARNING);
    VERIFY_NON_NULL(TAG, ctx, WARNING);

    OIC_LOG(DEBUG, TAG, "IN OwnerAclHandler");
    (void)UNUSED;
    OCStackResult res = OC_STACK_OK;
    OTMContext_t* otmCtx = (OTMContext_t*)ctx;
    otmCtx->ocDoHandle = NULL;

    if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
    {
        if(otmCtx && otmCtx->selectedDeviceInfo)
        {
            //POST /oic/sec/doxm [{ ..., "owned":"TRUE" }]
            res = PostOwnershipInformation(otmCtx);
            if(OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG, "Failed to update ownership information to new device");
                SetResult(otmCtx, res);
            }
        }
    }
    else
    {
        res = clientResponse->result;
        OIC_LOG_V(ERROR, TAG, "OwnerAclHandler : Unexpected result %d", res);
        SetResult(otmCtx, res);
    }

    OIC_LOG(DEBUG, TAG, "OUT OwnerAclHandler");

exit:
    return  OC_STACK_DELETE_TRANSACTION;
}


/**
 * Response handler for update owner information request.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and  OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult OwnershipInformationHandler(void *ctx, OCDoHandle UNUSED,
                                OCClientResponse *clientResponse)
{
    VERIFY_NON_NULL(TAG, clientResponse, WARNING);
    VERIFY_NON_NULL(TAG, ctx, WARNING);

    OIC_LOG(DEBUG, TAG, "IN OwnershipInformationHandler");
    (void)UNUSED;
    OCStackResult res = OC_STACK_OK;
    OTMContext_t* otmCtx = (OTMContext_t*)ctx;
    otmCtx->ocDoHandle = NULL;

    if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
    {
        if(otmCtx && otmCtx->selectedDeviceInfo)
        {
            OIC_LOG(INFO, TAG, "Ownership transfer was successfully completed.");
            OIC_LOG(INFO, TAG, "Set Ready for provisioning state .");

            res = PostProvisioningStatus(otmCtx);
            if(OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG, "Failed to update pstat");
                SetResult(otmCtx, res);
            }
        }
    }
    else
    {
        res = clientResponse->result;
        OIC_LOG_V(ERROR, TAG, "OwnershipInformationHandler : Unexpected result %d", res);
        SetResult(otmCtx, res);
    }

    OIC_LOG(DEBUG, TAG, "OUT OwnershipInformationHandler");

exit:
    return  OC_STACK_DELETE_TRANSACTION;
}

/**
 * Response handler of update provisioning status.
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult ProvisioningStatusHandler(void *ctx, OCDoHandle UNUSED,
                                                       OCClientResponse *clientResponse)
{
    OIC_LOG_V(INFO, TAG, "IN ProvisioningStatusHandler.");

    VERIFY_NON_NULL(TAG, clientResponse, ERROR);
    VERIFY_NON_NULL(TAG, ctx, ERROR);

    OTMContext_t* otmCtx = (OTMContext_t*) ctx;
    otmCtx->ocDoHandle = NULL;
    (void)UNUSED;
    OCStackResult res = OC_STACK_OK;

    if(OC_STACK_RESOURCE_CHANGED == clientResponse->result)
    {
        if(otmCtx && otmCtx->selectedDeviceInfo)
        {
            OIC_LOG(INFO, TAG, "Device state is in Ready for Provisionig.");

            res = PostNormalOperationStatus(otmCtx);
            if(OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG, "Failed to update pstat");
                SetResult(otmCtx, res);
            }
        }
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "Error occured in provisionDefaultACLCB :: %d\n",
                            clientResponse->result);
        SetResult(otmCtx, clientResponse->result);
    }

exit:
    OIC_LOG_V(INFO, TAG, "OUT ProvisioningStatusHandler.");
    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * Response handler of update provisioning status to Ready for Normal..
 *
 * @param[in] ctx             ctx value passed to callback from calling function.
 * @param[in] UNUSED          handle to an invocation
 * @param[in] clientResponse  Response from queries to remote servers.
 * @return  OC_STACK_DELETE_TRANSACTION to delete the transaction
 *          and OC_STACK_KEEP_TRANSACTION to keep it.
 */
static OCStackApplicationResult ReadyForNomalStatusHandler(void *ctx, OCDoHandle UNUSED,
                                                       OCClientResponse *clientResponse)
{
    OIC_LOG_V(INFO, TAG, "IN ReadyForNomalStatusHandler.");

    VERIFY_NON_NULL(TAG, clientResponse, ERROR);
    VERIFY_NON_NULL(TAG, ctx, ERROR);

    OTMContext_t* otmCtx = (OTMContext_t*) ctx;
    otmCtx->ocDoHandle = NULL;
    (void)UNUSED;

    if (OC_STACK_RESOURCE_CHANGED == clientResponse->result)
    {
        OIC_LOG(INFO, TAG, "Device state is in Ready for Normal Operation.");
        OCStackResult res = PDMSetDeviceState(&otmCtx->selectedDeviceInfo->doxm->deviceID,
                                              PDM_DEVICE_ACTIVE);
         if (OC_STACK_OK == res)
         {
                OIC_LOG_V(INFO, TAG, "Add device's UUID in PDM_DB");
                SetResult(otmCtx, OC_STACK_OK);
                return OC_STACK_DELETE_TRANSACTION;
         }
          else
         {
              OIC_LOG(ERROR, TAG, "Ownership transfer is complete but adding information to DB is failed.");
         }
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "Error occured in provisionDefaultACLCB :: %d\n",
                            clientResponse->result);
        SetResult(otmCtx, clientResponse->result);
    }

exit:
    OIC_LOG_V(INFO, TAG, "OUT ReadyForNomalStatusHandler.");
    return OC_STACK_DELETE_TRANSACTION;
}

static OCStackResult PostOwnerCredential(OTMContext_t* otmCtx)
{
    OIC_LOG(DEBUG, TAG, "IN PostOwnerCredential");

    if(!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        OIC_LOG(ERROR, TAG, "Invalid parameters");
        return OC_STACK_INVALID_PARAM;
    }

    OCProvisionDev_t* deviceInfo = otmCtx->selectedDeviceInfo;
    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};

    if(!PMGenerateQuery(true,
                        deviceInfo->endpoint.addr, deviceInfo->securePort,
                        deviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_CRED_URI))
    {
        OIC_LOG(ERROR, TAG, "PostOwnerCredential : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);
    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if(!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }

    //Generate owner credential for new device
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;
    const OicSecCred_t* ownerCredential = GetCredResourceData(&(deviceInfo->doxm->deviceID));
    if(!ownerCredential)
    {
        OIC_LOG(ERROR, TAG, "Can not find OwnerPSK.");
        return OC_STACK_NO_RESOURCE;
    }

    OicUuid_t credSubjectId = {.id={0}};
    if(OC_STACK_OK == GetDoxmDeviceID(&credSubjectId))
    {
        OicSecCred_t newCredential;
        memcpy(&newCredential, ownerCredential, sizeof(OicSecCred_t));
        newCredential.next = NULL;

        //Set subject ID as PT's ID
        memcpy(&(newCredential.subject), &credSubjectId, sizeof(OicUuid_t));

        //Fill private data as empty string
        newCredential.privateData.data = (uint8_t*)"";
        newCredential.privateData.len = 0;
        newCredential.privateData.encoding = ownerCredential->privateData.encoding;

        newCredential.publicData.data = NULL;
        newCredential.publicData.len = 0;
        newCredential.publicData.encoding = ownerCredential->publicData.encoding;

        int secureFlag = 0;
        //Send owner credential to new device : POST /oic/sec/cred [ owner credential ]
        if (OC_STACK_OK != CredToCBORPayload(&newCredential, &secPayload->securityData,
                                        &secPayload->payloadSize, secureFlag))
        {
            OICFree(secPayload);
            OIC_LOG(ERROR, TAG, "Error while converting bin to cbor.");
            return OC_STACK_ERROR;
        }
        OIC_LOG(DEBUG, TAG, "Cred Payload:");
        OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

        OCCallbackData cbData;
        cbData.cb = &OwnerCredentialHandler;
        cbData.context = (void *)otmCtx;
        cbData.cd = NULL;
        OCStackResult res = OCDoResource(&otmCtx->ocDoHandle, OC_REST_POST, query,
                                         &deviceInfo->endpoint, (OCPayload*)secPayload,
                                         deviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
        if (res != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "OCStack resource error");
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Failed to read DOXM device ID.");
        return OC_STACK_NO_RESOURCE;
    }

    OIC_LOG(DEBUG, TAG, "OUT PostOwnerCredential");

    return OC_STACK_OK;
}

static OicSecAcl_t* GenerateOwnerAcl(const OicUuid_t* owner)
{
    OicSecAcl_t* ownerAcl = (OicSecAcl_t*)OICCalloc(1, sizeof(OicSecAcl_t));
    OicSecAce_t* ownerAce = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    OicSecRsrc_t* wildcardRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    if(NULL == ownerAcl || NULL == ownerAce || NULL == wildcardRsrc)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        goto error;
    }
    LL_APPEND(ownerAcl->aces, ownerAce);
    LL_APPEND(ownerAce->resources, wildcardRsrc);

    //Set resource owner as PT
    memcpy(ownerAcl->rownerID.id, owner->id, sizeof(owner->id));

    //PT has full permission.
    ownerAce->permission = PERMISSION_FULL_CONTROL;

    //Set subject as PT's UUID
    memcpy(ownerAce->subjectuuid.id, owner->id, sizeof(owner->id));

    wildcardRsrc->href = OICStrdup(WILDCARD_RESOURCE_URI);
    if(NULL == wildcardRsrc->href)
    {
        goto error;
    }

    wildcardRsrc->interfaceLen = 1;
    wildcardRsrc->interfaces = (char**)OICMalloc(wildcardRsrc->interfaceLen * sizeof(char*));
    if(NULL == wildcardRsrc->interfaces)
    {
        goto error;
    }
    wildcardRsrc->interfaces[0] = OICStrdup(WILDCARD_RESOURCE_URI);
    if(NULL == wildcardRsrc->interfaces[0])
    {
        goto error;
    }

    wildcardRsrc->typeLen = 1;
    wildcardRsrc->types = (char**)OICMalloc(wildcardRsrc->typeLen * sizeof(char*));
    if(NULL == wildcardRsrc->types)
    {
        goto error;
    }
    wildcardRsrc->types[0] = OICStrdup(WILDCARD_RESOURCE_URI);
    if(NULL == wildcardRsrc->types[0])
    {
        goto error;
    }

    return ownerAcl;

error:
    //in case of memory allocation failed, each resource should be removed individually.
    if(NULL == ownerAcl || NULL == ownerAce || NULL == wildcardRsrc)
    {
        OICFree(ownerAcl);
        OICFree(ownerAce);
        OICFree(wildcardRsrc);
    }
    else
    {
        DeleteACLList(ownerAcl);
    }
    return NULL;
}

/**
 * Function to update the owner ACL to new device.
 *
 * @param[in]  otmCtx  Context value of ownership transfer.
 * @return  OC_STACK_OK on success
 */
static OCStackResult PostOwnerAcl(OTMContext_t* otmCtx)
{
    OCStackResult res = OC_STACK_ERROR;

    OIC_LOG(DEBUG, TAG, "IN PostOwnerAcl");

    if(!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        OIC_LOG(ERROR, TAG, "Invalid parameters");
        return OC_STACK_INVALID_PARAM;
    }

    OCProvisionDev_t* deviceInfo = otmCtx->selectedDeviceInfo;
    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    OicSecAcl_t* ownerAcl = NULL;

    if(!PMGenerateQuery(true,
                        deviceInfo->endpoint.addr, deviceInfo->securePort,
                        deviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_ACL_URI))
    {
        OIC_LOG(ERROR, TAG, "Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OicUuid_t ownerID;
    res = GetDoxmDeviceID(&ownerID);
    if(OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Failed to generate owner ACL");
        return res;
    }

    //Generate owner ACL for new device
    ownerAcl = GenerateOwnerAcl(&ownerID);
    if(NULL == ownerAcl)
    {
        OIC_LOG(ERROR, TAG, "Failed to generate owner ACL");
        return OC_STACK_NO_MEMORY;
    }

    //Generate ACL payload
    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if(!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    res = AclToCBORPayload(ownerAcl, &secPayload->securityData, &secPayload->payloadSize);
    if (OC_STACK_OK != res)
    {
        OICFree(secPayload);
        OIC_LOG(ERROR, TAG, "Error while converting bin to cbor.");
        goto error;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;

    OIC_LOG(DEBUG, TAG, "Owner ACL Payload:");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    //Send owner ACL to new device : POST /oic/sec/cred [ owner credential ]
    OCCallbackData cbData;
    cbData.cb = &OwnerAclHandler;
    cbData.context = (void *)otmCtx;
    cbData.cd = NULL;
    res = OCDoResource(&otmCtx->ocDoHandle, OC_REST_POST, query,
                                     &deviceInfo->endpoint, (OCPayload*)secPayload,
                                     deviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    if (res != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
        goto error;
    }

    OIC_LOG(DEBUG, TAG, "OUT PostOwnerAcl");

error:
    DeleteACLList(ownerAcl);

    return OC_STACK_OK;
}

static OCStackResult PostOwnerTransferModeToResource(OTMContext_t* otmCtx)
{
    OIC_LOG(DEBUG, TAG, "IN PostOwnerTransferModeToResource");

    if(!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        OIC_LOG(ERROR, TAG, "Invalid parameters");
        return OC_STACK_INVALID_PARAM;
    }

    OCProvisionDev_t* deviceInfo = otmCtx->selectedDeviceInfo;
    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};

    if(!PMGenerateQuery(false,
                        deviceInfo->endpoint.addr, deviceInfo->endpoint.port,
                        deviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_DOXM_URI))
    {
        OIC_LOG(ERROR, TAG, "PostOwnerTransferModeToResource : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if(!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }

    secPayload->base.type = PAYLOAD_TYPE_SECURITY;
    OCStackResult res = otmCtx->otmCallback.createSelectOxmPayloadCB(otmCtx,
            &secPayload->securityData, &secPayload->payloadSize);
    if (OC_STACK_OK != res && NULL == secPayload->securityData)
    {
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG(ERROR, TAG, "Error while converting bin to cbor");
        return OC_STACK_ERROR;
    }

    OCCallbackData cbData;
    cbData.cb = &OwnerTransferModeHandler;
    cbData.context = (void *)otmCtx;
    cbData.cd = NULL;
    res = OCDoResource(&otmCtx->ocDoHandle, OC_REST_POST, query,
                       &deviceInfo->endpoint, (OCPayload *)secPayload,
                       deviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    if (res != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
    }

    OIC_LOG(DEBUG, TAG, "OUT PostOwnerTransferModeToResource");

    return res;
}

static OCStackResult GetProvisioningStatusResource(OTMContext_t* otmCtx)
{
    OIC_LOG(DEBUG, TAG, "IN GetProvisioningStatusResource");

    if(!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        OIC_LOG(ERROR, TAG, "Invailed parameters");
        return OC_STACK_INVALID_PARAM;
    }

    OCProvisionDev_t* deviceInfo = otmCtx->selectedDeviceInfo;
    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!PMGenerateQuery(false,
                        deviceInfo->endpoint.addr, deviceInfo->endpoint.port,
                        deviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_PSTAT_URI))
    {
        OIC_LOG(ERROR, TAG, "GetProvisioningStatusResource : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCCallbackData cbData;
    cbData.cb = &ListMethodsHandler;
    cbData.context = (void *)otmCtx;
    cbData.cd = NULL;
    OCStackResult res = OCDoResource(&otmCtx->ocDoHandle, OC_REST_GET, query, NULL, NULL,
                                     deviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    if (res != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
    }

    OIC_LOG(DEBUG, TAG, "OUT GetProvisioningStatusResource");

    return res;
}

static OCStackResult PostOwnerUuid(OTMContext_t* otmCtx)
{
    OIC_LOG(DEBUG, TAG, "IN PostOwnerUuid");

    if(!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        OIC_LOG(ERROR, TAG, "Invailed parameters");
        return OC_STACK_INVALID_PARAM;
    }

    OCProvisionDev_t* deviceInfo = otmCtx->selectedDeviceInfo;
    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!PMGenerateQuery(true,
                        deviceInfo->endpoint.addr, deviceInfo->securePort,
                        deviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_DOXM_URI))
    {
        OIC_LOG(ERROR, TAG, "PostOwnerUuid : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    //Post PT's uuid to new device
    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if(!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;
    OCStackResult res = otmCtx->otmCallback.createOwnerTransferPayloadCB(
            otmCtx, &secPayload->securityData, &secPayload->payloadSize);
    if (OC_STACK_OK != res && NULL == secPayload->securityData)
    {
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG(ERROR, TAG, "Error while converting doxm bin to cbor.");
        return OC_STACK_INVALID_PARAM;
    }
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    OCCallbackData cbData;
    cbData.cb = &OwnerUuidUpdateHandler;
    cbData.context = (void *)otmCtx;
    cbData.cd = NULL;

    // This is a workaround solution to avoid retransmission - will be removed later
    OCQualityOfService qs = OC_HIGH_QOS;
    if (OIC_CON_MFG_CERT == deviceInfo->doxm->oxmSel || OIC_MV_JUST_WORKS == deviceInfo->doxm->oxmSel)
    {
        OIC_LOG(DEBUG, TAG, "Switching to non-confirmable message");
        qs = OC_LOW_QOS;
    }
    res = OCDoResource(&otmCtx->ocDoHandle, OC_REST_POST, query, 0, (OCPayload *)secPayload,
            deviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    if (res != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
    }

    OIC_LOG(DEBUG, TAG, "OUT PostOwnerUuid");

    return res;
}

static OCStackResult PostOwnershipInformation(OTMContext_t* otmCtx)
{
    OIC_LOG(DEBUG, TAG, "IN PostOwnershipInformation");

    if(!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        OIC_LOG(ERROR, TAG, "Invailed parameters");
        return OC_STACK_INVALID_PARAM;
    }

    OCProvisionDev_t* deviceInfo = otmCtx->selectedDeviceInfo;
    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!PMGenerateQuery(true,
                        deviceInfo->endpoint.addr, deviceInfo->securePort,
                        deviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_DOXM_URI))
    {
        OIC_LOG(ERROR, TAG, "PostOwnershipInformation : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    //OwnershipInformationHandler
    OCSecurityPayload *secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if (!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }

    otmCtx->selectedDeviceInfo->doxm->owned = true;

    secPayload->base.type = PAYLOAD_TYPE_SECURITY;
    OCStackResult res = DoxmToCBORPayload(otmCtx->selectedDeviceInfo->doxm,
            &secPayload->securityData, &secPayload->payloadSize, true);
    if (OC_STACK_OK != res && NULL == secPayload->securityData)
    {
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG(ERROR, TAG, "Error while converting doxm bin to json");
        return OC_STACK_INVALID_PARAM;
    }

    OCCallbackData cbData;
    cbData.cb = &OwnershipInformationHandler;
    cbData.context = (void *)otmCtx;
    cbData.cd = NULL;

    res = OCDoResource(&otmCtx->ocDoHandle, OC_REST_POST, query, 0, (OCPayload*)secPayload,
                       deviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    if (res != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
    }

    OIC_LOG(DEBUG, TAG, "OUT PostOwnershipInformation");

    return res;
}

static OCStackResult PostUpdateOperationMode(OTMContext_t* otmCtx)
{
    OIC_LOG(DEBUG, TAG, "IN PostUpdateOperationMode");

    if(!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCProvisionDev_t* deviceInfo = otmCtx->selectedDeviceInfo;
    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!PMGenerateQuery(false,
                        deviceInfo->endpoint.addr, deviceInfo->endpoint.port,
                        deviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_PSTAT_URI))
    {
        OIC_LOG(ERROR, TAG, "PostUpdateOperationMode : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCSecurityPayload* secPayload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));
    if(!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;
    OCStackResult res = PstatToCBORPayload(deviceInfo->pstat, &secPayload->securityData,
                                           &secPayload->payloadSize, true);
   if (OC_STACK_OK != res)
    {
        OCPayloadDestroy((OCPayload *)secPayload);
        OIC_LOG(ERROR, TAG, "Error while converting pstat to cbor.");
        return OC_STACK_INVALID_PARAM;
    }

    OCCallbackData cbData;
    cbData.cb = &OperationModeUpdateHandler;
    cbData.context = (void *)otmCtx;
    cbData.cd = NULL;
    res = OCDoResource(&otmCtx->ocDoHandle, OC_REST_POST, query, 0, (OCPayload *)secPayload,
                       deviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    if (res != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
    }

    OIC_LOG(DEBUG, TAG, "OUT PostUpdateOperationMode");

    return res;
}

static OCStackResult SetupPDM(const OCProvisionDev_t* selectedDevice)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    PdmDeviceState_t pdmState = PDM_DEVICE_UNKNOWN;
    OCStackResult res = PDMGetDeviceState(&selectedDevice->doxm->deviceID, &pdmState);
    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Internal error in PDMGetDeviceState : %d", res);
        return res;
    }

    char* strUuid = NULL;
    bool removeCredReq = false;
    if (OC_STACK_OK != ConvertUuidToStr(&selectedDevice->doxm->deviceID, &strUuid))
    {
        OIC_LOG(WARNING, TAG, "Failed to covert uuid to string");
        return OC_STACK_NO_MEMORY;
    }

    if (PDM_DEVICE_UNKNOWN == pdmState && !selectedDevice->doxm->owned)
    {
        removeCredReq = true;
    }
    else if (PDM_DEVICE_ACTIVE == pdmState && !selectedDevice->doxm->owned)
    {
        OIC_LOG_V(WARNING, TAG, "Unowned device[%s] dectected from PDM.", strUuid);
        OIC_LOG_V(WARNING, TAG, "[%s] will be removed from PDM.", strUuid);
        res = PDMDeleteDevice(&selectedDevice->doxm->deviceID);
        if(OC_STACK_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to remove [%s] information from PDM.", strUuid);
            goto exit;
        }

        removeCredReq = true;
    }

    if (removeCredReq)
    {
        OIC_LOG_V(WARNING, TAG, "[%s]'s credential will be removed.", strUuid);
        res = RemoveCredential(&selectedDevice->doxm->deviceID);
        if (OC_STACK_RESOURCE_DELETED != res)
        {
            OIC_LOG_V(WARNING, TAG, "Can not find [%s]'s credential.", strUuid);
        }
    }

    //Checking duplication of Device ID.
    bool isDuplicate = true;
    res = PDMIsDuplicateDevice(&selectedDevice->doxm->deviceID, &isDuplicate);
    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Internal error in PDMIsDuplicateDevice : %d", res);
        goto exit;
    }

    if (isDuplicate)
    {
        char* strUuid = NULL;
        res = ConvertUuidToStr(&selectedDevice->doxm->deviceID, &strUuid);
        if (OC_STACK_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to convert UUID to str : %d", res);
            goto exit;
        }

        if (PDM_DEVICE_STALE == pdmState)
        {
            OIC_LOG(INFO, TAG, "Detected duplicated UUID in stale status, "
                               "device status will revert back to initial status.");
            res = PDMSetDeviceState(&selectedDevice->doxm->deviceID, PDM_DEVICE_INIT);
            if (OC_STACK_OK != res)
            {
                OIC_LOG_V(ERROR, TAG, "Internal error in PDMSetDeviceState : %d", res);
                goto exit;
            }
        }
        else if (PDM_DEVICE_INIT == pdmState)
        {
            OIC_LOG_V(ERROR, TAG, "[%s]'s ownership transfer process is already started.", strUuid);
            OICFree(strUuid);
            res = OC_STACK_DUPLICATE_REQUEST;
            goto exit;
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Unknow device status while OTM.");
            OICFree(strUuid);
            res = OC_STACK_ERROR;
            goto exit;
        }
    }
    else
    {
        res = PDMAddDevice(&selectedDevice->doxm->deviceID);
        if (OC_STACK_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "Internal error in PDMAddDevice : %d", res);
            goto exit;
        }
    }

exit:
    OICFree(strUuid);
    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);
    return res;
}

static OCStackResult StartOwnershipTransfer(void* ctx, OCProvisionDev_t* selectedDevice)
{
    OIC_LOG(INFO, TAG, "IN StartOwnershipTransfer");
    OCStackResult res = OC_STACK_INVALID_PARAM;
    OicUuid_t emptyOwner = {.id = {0} };

    VERIFY_NON_NULL(TAG, selectedDevice, ERROR);
    VERIFY_NON_NULL(TAG, selectedDevice->doxm, ERROR);

    OTMContext_t* otmCtx = (OTMContext_t*)ctx;
    otmCtx->selectedDeviceInfo = selectedDevice;

    //If devowneruuid of selectedDevice is not emtry, PostOwnerUuid does not triggered in DTLSHandshakeCB
    if (memcmp(&(selectedDevice->doxm->owner), &emptyOwner, sizeof(OicUuid_t)) != 0)
    {
        OIC_LOG(DEBUG, TAG, "Set devowneruuid of selectedDevice to empty for OwnershipTransfer");
        memcpy(&(selectedDevice->doxm->owner), &emptyOwner, sizeof(OicUuid_t));
    }

    //Setup PDM to perform the OTM, PDM will be cleanup if necessary.
    res = SetupPDM(selectedDevice);
    if(OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "SetupPDM error : %d", res);
        SetResult(otmCtx, res);
        return res;
    }

    //Select the OxM to performing ownership transfer
    res = OTMSelectOwnershipTransferMethod(selectedDevice->doxm->oxm,
                                          selectedDevice->doxm->oxmLen,
                                          &selectedDevice->doxm->oxmSel,
                                          SUPER_OWNER);
    if(OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to select the provisioning method : %d", res);
        SetResult(otmCtx, res);
        return res;
    }
    OIC_LOG_V(DEBUG, TAG, "Selected provisoning method = %d", selectedDevice->doxm->oxmSel);

    res = OTMSetOTCallback(selectedDevice->doxm->oxmSel, &otmCtx->otmCallback);
    if(OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Error in OTMSetOTCallback : %d", res);
        return res;
    }

    //Send Req: POST /oic/sec/doxm [{..."OxmSel" :g_OTMCbDatas[Index of Selected OxM].OXMString,...}]
    res = PostOwnerTransferModeToResource(otmCtx);
    if(OC_STACK_OK != res)
    {
        OIC_LOG_V(WARNING, TAG, "Failed to select the provisioning method : %d", res);
        SetResult(otmCtx, res);
        return res;
    }

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    //Register TLS event handler to catch the tls event while handshake
    if(CA_STATUS_OK != CAregisterSslHandshakeCallback(DTLSHandshakeCB))
    {
        OIC_LOG(WARNING, TAG, "StartOwnershipTransfer : Failed to register TLS handshake callback.");
    }
#endif // __WITH_DTLS__ or __WITH_TLS__
    OIC_LOG(INFO, TAG, "OUT StartOwnershipTransfer");

exit:
    return res;
}

OCStackResult OTMSetOwnershipTransferCallbackData(OicSecOxm_t oxmType, OTMCallbackData_t* data)
{
    OIC_LOG(DEBUG, TAG, "IN OTMSetOwnerTransferCallbackData");

    if(!data)
    {
        OIC_LOG(ERROR, TAG, "OTMSetOwnershipTransferCallbackData : Invalid parameters");
        return OC_STACK_INVALID_PARAM;
    }
    if(oxmType >= OIC_OXM_COUNT)
    {
        OIC_LOG(INFO, TAG, "Unknow ownership transfer method");
        return OC_STACK_INVALID_PARAM;
    }

    // TODO: Remove this API, Please see the jira ticket IOT-1484

    OIC_LOG(DEBUG, TAG, "OUT OTMSetOwnerTransferCallbackData");

    return OC_STACK_OK;
}

/**
 * NOTE : Unowned discovery should be done before performing OTMDoOwnershipTransfer
 */
OCStackResult OTMDoOwnershipTransfer(void* ctx,
                                     OCProvisionDev_t *selectedDevicelist,
                                     OCProvisionResultCB resultCallback)
{
    OIC_LOG(DEBUG, TAG, "IN OTMDoOwnershipTransfer");

    if (NULL == selectedDevicelist)
    {
        return OC_STACK_INVALID_PARAM;
    }
    if (NULL == resultCallback)
    {
        return OC_STACK_INVALID_CALLBACK;
    }

    OTMContext_t* otmCtx = (OTMContext_t*)OICCalloc(1,sizeof(OTMContext_t));
    if(!otmCtx)
    {
        OIC_LOG(ERROR, TAG, "Failed to create OTM Context");
        return OC_STACK_NO_MEMORY;
    }
    otmCtx->ctxResultCallback = resultCallback;
    otmCtx->ctxHasError = false;
    otmCtx->userCtx = ctx;
    OCProvisionDev_t* pCurDev = selectedDevicelist;

    //Counting number of selected devices.
    otmCtx->ctxResultArraySize = 0;
    while(NULL != pCurDev)
    {
        otmCtx->ctxResultArraySize++;
        pCurDev = pCurDev->next;
    }

    otmCtx->ctxResultArray =
        (OCProvisionResult_t*)OICCalloc(otmCtx->ctxResultArraySize, sizeof(OCProvisionResult_t));
    if(NULL == otmCtx->ctxResultArray)
    {
        OIC_LOG(ERROR, TAG, "OTMDoOwnershipTransfer : Failed to memory allocation");
        OICFree(otmCtx);
        return OC_STACK_NO_MEMORY;
    }
    pCurDev = selectedDevicelist;

    //Fill the device UUID for result array.
    for(size_t devIdx = 0; devIdx < otmCtx->ctxResultArraySize; devIdx++)
    {
        memcpy(otmCtx->ctxResultArray[devIdx].deviceId.id,
               pCurDev->doxm->deviceID.id,
               UUID_LENGTH);
        otmCtx->ctxResultArray[devIdx].res = OC_STACK_CONTINUE;
        pCurDev = pCurDev->next;
    }

    OCStackResult res = StartOwnershipTransfer(otmCtx, selectedDevicelist);

    OIC_LOG(DEBUG, TAG, "OUT OTMDoOwnershipTransfer");

    return res;
}

OCStackResult OTMSetOxmAllowStatus(const OicSecOxm_t oxm, const bool allowStatus)
{
    OIC_LOG_V(INFO, TAG, "IN %s : oxm=%d, allow status=%s",
              __func__, oxm, (allowStatus ? "true" : "false"));

#ifdef MULTIPLE_OWNER
    if(OIC_OXM_COUNT <= oxm && OIC_MV_JUST_WORKS != oxm && OIC_PRECONFIG_PIN != oxm && OIC_CON_MFG_CERT != oxm)
#else
    if(OIC_OXM_COUNT <= oxm && OIC_MV_JUST_WORKS != oxm && OIC_CON_MFG_CERT != oxm)
#endif
    {
        return OC_STACK_INVALID_PARAM;
    }

    OxmAllowTableIdx_t oxmIdx = GetOxmAllowTableIdx(oxm);
    if(OXM_IDX_COUNT <= oxmIdx)
    {
        OIC_LOG(ERROR, TAG, "Invalid oxm index to access oxm allow table.");
        return OC_STACK_ERROR;
    }
    g_OxmAllowStatus[oxmIdx] = (allowStatus ? ALLOWED_OXM : NOT_ALLOWED_OXM);

    OIC_LOG_V(INFO, TAG, "OUT %s", __func__);

    return OC_STACK_OK;
}

OCStackResult PostProvisioningStatus(OTMContext_t* otmCtx)
{
    OIC_LOG(INFO, TAG, "IN PostProvisioningStatus");

    if(!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        OIC_LOG(ERROR, TAG, "OTMContext is NULL");
        return OC_STACK_INVALID_PARAM;
    }

    //Change the TAKE_OWNER bit of CM to 0.
    otmCtx->selectedDeviceInfo->pstat->cm &= (~TAKE_OWNER);

    OCSecurityPayload *secPayload = (OCSecurityPayload *)OICCalloc(1, sizeof(OCSecurityPayload));
    if (!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;
    if (OC_STACK_OK != PstatToCBORPayload(otmCtx->selectedDeviceInfo->pstat,
            &secPayload->securityData, &secPayload->payloadSize, true))
    {
        OCPayloadDestroy((OCPayload *)secPayload);
        return OC_STACK_INVALID_JSON;
    }
    OIC_LOG(DEBUG, TAG, "Created payload for chage to Provisiong state");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!PMGenerateQuery(true,
                        otmCtx->selectedDeviceInfo->endpoint.addr,
                        otmCtx->selectedDeviceInfo->securePort,
                        otmCtx->selectedDeviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_PSTAT_URI))
    {
        OIC_LOG(ERROR, TAG, "PostProvisioningStatus : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCCallbackData cbData = {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = &ProvisioningStatusHandler;
    cbData.context = (void*)otmCtx;
    cbData.cd = NULL;
    OCStackResult ret = OCDoResource(&otmCtx->ocDoHandle, OC_REST_POST, query, 0, (OCPayload*)secPayload,
            otmCtx->selectedDeviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    OIC_LOG_V(INFO, TAG, "OCDoResource returned: %d",ret);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
    }

    OIC_LOG(INFO, TAG, "OUT PostProvisioningStatus");

    return ret;
}

OCStackResult PostNormalOperationStatus(OTMContext_t* otmCtx)
{
    OIC_LOG(INFO, TAG, "IN PostNormalOperationStatus");

    if(!otmCtx || !otmCtx->selectedDeviceInfo)
    {
        OIC_LOG(ERROR, TAG, "OTMContext is NULL");
        return OC_STACK_INVALID_PARAM;
    }

    //Set isop to true.
    otmCtx->selectedDeviceInfo->pstat->isOp = true;

    OCSecurityPayload *secPayload = (OCSecurityPayload *)OICCalloc(1, sizeof(OCSecurityPayload));
    if (!secPayload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }
    secPayload->base.type = PAYLOAD_TYPE_SECURITY;
    if (OC_STACK_OK != PstatToCBORPayload(otmCtx->selectedDeviceInfo->pstat,
            &secPayload->securityData, &secPayload->payloadSize, true))
    {
        OCPayloadDestroy((OCPayload *)secPayload);
        return OC_STACK_INVALID_JSON;
    }
    OIC_LOG(DEBUG, TAG, "Created payload for chage to Provisiong state");
    OIC_LOG_BUFFER(DEBUG, TAG, secPayload->securityData, secPayload->payloadSize);

    char query[MAX_URI_LENGTH + MAX_QUERY_LENGTH] = {0};
    if(!PMGenerateQuery(true,
                        otmCtx->selectedDeviceInfo->endpoint.addr,
                        otmCtx->selectedDeviceInfo->securePort,
                        otmCtx->selectedDeviceInfo->connType,
                        query, sizeof(query), OIC_RSRC_PSTAT_URI))
    {
        OIC_LOG(ERROR, TAG, "PostNormalOperationStatus : Failed to generate query");
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(DEBUG, TAG, "Query=%s", query);

    OCCallbackData cbData = {.context=NULL, .cb=NULL, .cd=NULL};
    cbData.cb = &ReadyForNomalStatusHandler;
    cbData.context = (void*)otmCtx;
    cbData.cd = NULL;
    OCStackResult ret = OCDoResource(&otmCtx->ocDoHandle, OC_REST_POST, query, 0, (OCPayload*)secPayload,
            otmCtx->selectedDeviceInfo->connType, OC_HIGH_QOS, &cbData, NULL, 0);
    OIC_LOG_V(INFO, TAG, "OCDoResource returned: %d",ret);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
    }

    OIC_LOG(INFO, TAG, "OUT PostNormalOperationStatus");

    return ret;
}

OCStackResult ConfigSelfOwnership(void)
{
    OIC_LOG(INFO, TAG, "IN ConfigSelfOwnership");

    bool isDeviceOwned = true;
    if (OC_STACK_OK != GetDoxmIsOwned(&isDeviceOwned))
    {
        OIC_LOG (ERROR, TAG, "Unable to retrieve doxm owned state");
        return OC_STACK_ERROR;
    }
    if( (true == isDeviceOwned) ||(true == GetPstatIsop()) )
    {
        OIC_LOG(ERROR, TAG, "The state of device is not Ready for Ownership transfer.");
        return OC_STACK_ERROR;
    }
    OicUuid_t deviceID = {.id={0}};
    if ( OC_STACK_OK != GetDoxmDeviceID(&deviceID) )
    {
        OIC_LOG (ERROR, TAG, "Unable to retrieve doxm Device ID");
        return OC_STACK_ERROR;
    }

    OCStackResult ret = OC_STACK_OK;
    //Update the pstat resource as Normal Operation.
    ret = SetPstatSelfOwnership(&deviceID);
    if(OC_STACK_OK != ret)
    {
        OIC_LOG (ERROR, TAG, "Unable to update pstat resource as Normal Operation");
        goto exit;
    }
    //Update the doxm resource as Normal Operation.
    ret = SetDoxmSelfOwnership(&deviceID);
    if(OC_STACK_OK != ret)
    {
        OIC_LOG (ERROR, TAG, "Unable to update doxm resource as Normal Operation");
        goto exit;
    }
    //Update default ACE of security resource to prevent anonymous user access.
    ret = UpdateDefaultSecProvACE();
    if(OC_STACK_OK != ret)
    {
        OIC_LOG (ERROR, TAG, "Unable to update default ace in ConfigSelfOwnership");
        goto exit;
    }
    //Update the acl resource owner as owner device.
    ret = SetAclRownerId(&deviceID);
    if(OC_STACK_OK != ret)
    {
        OIC_LOG (ERROR, TAG, "Unable to update acl resource in ConfigSelfOwnership");
        goto exit;
    }
    //Update the cred resource owner as owner device.
    ret = SetCredRownerId(&deviceID);
    if(OC_STACK_OK != ret)
    {
        // Cred resouce may be empty in Ready for Ownership transfer state.
        if (OC_STACK_NO_RESOURCE == ret)
        {
            OIC_LOG (INFO, TAG, "Cred resource is empty");
            ret = OC_STACK_OK;
            goto exit;
        }
        OIC_LOG (ERROR, TAG, "Unable to update cred resource in ConfigSelfOwnership");
    }

exit:
    if(OC_STACK_OK != ret)
    {
        /*
         * If some error is occured while configure self-ownership,
         * ownership related resource should be revert back to initial status.
        */
        ResetSecureResourceInPS();
    }

    return ret;
}


void OTMTerminate()
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    DeleteOTMContextList();

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    if(CA_STATUS_OK != CAregisterSslHandshakeCallback(NULL))
    {
        OIC_LOG(WARNING, TAG, "Failed to register (D)TLS handshake callback.");
    }
#endif // __WITH_DTLS__ or __WITH_TLS__

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}
