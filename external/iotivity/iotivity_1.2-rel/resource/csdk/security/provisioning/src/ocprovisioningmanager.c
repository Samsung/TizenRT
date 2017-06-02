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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ocprovisioningmanager.h"
#include "pmutility.h"
#include "srmutility.h"
#include "ownershiptransfermanager.h"
#ifdef MULTIPLE_OWNER
#include "multipleownershiptransfermanager.h"
#endif //MULTIPLE_OWNER
#include "oic_malloc.h"
#include "logger.h"
#include "secureresourceprovider.h"
#include "provisioningdatabasemanager.h"
#include "credresource.h"
#include "utlist.h"
#include "aclresource.h" //Note: SRM internal header
#include "pconfresource.h"
#include "psinterface.h"
#include "srmresourcestrings.h"

#define TAG "OIC_OCPMAPI"

typedef struct Linkdata Linkdata_t;
struct Linkdata
{
    void *ctx;
    const OCProvisionDev_t *pDev1;
    OicSecAcl_t *pDev1Acl;
    const OCProvisionDev_t *pDev2;
    OicSecAcl_t *pDev2Acl;
    OCProvisionResult_t *resArr;
    int numOfResults;
    int currentCountResults;
    OCProvisionResultCB resultCallback;

};

#ifdef MULTIPLE_OWNER
typedef struct ProvPreconfPINCtx ProvPreconfPINCtx_t;
struct ProvPreconfPINCtx
{
    void *ctx;
    const OCProvisionDev_t *devInfo;
    const char* pin;
    size_t pinLen;
    OCProvisionResultCB resultCallback;
};
#endif //MULTIPLE_OWNER

/**
 * The function is responsible for initializaton of the provisioning manager. It will load
 * provisioning database which have owned device's list and their linked status.
 * TODO: In addition, if there is a device(s) which has not up-to-date credentials, this function will
 * automatically try to update the deivce(s).
 *
 * @param[in] dbPath file path of the sqlite3 db
 *
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCInitPM(const char* dbPath)
{
    return PDMInit(dbPath);
}

void OCTerminatePM()
{
    OTMTerminate();
}

OCStackResult OCPDMCleanupForTimeout()
{
    return PDMDeleteDeviceWithState(PDM_DEVICE_INIT);
}

/**
 * The function is responsible for discovery of owned/unowned device is specified endpoint/deviceID.
 * And this function will only return the specified device's response.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the device.
 * @param[in] deviceID         deviceID of target device.
 * @param[out] ppFoundDevice     OCProvisionDev_t of found device
 * @return OTM_SUCCESS in case of success and other value otherwise.
 */
OCStackResult OCDiscoverSingleDevice(unsigned short timeout, const OicUuid_t* deviceID,
                             OCProvisionDev_t **ppFoundDevice)
{
    if( NULL == ppFoundDevice || NULL != *ppFoundDevice || 0 == timeout || NULL == deviceID)
    {
        return OC_STACK_INVALID_PARAM;
    }

    return PMSingleDeviceDiscovery(timeout, deviceID, ppFoundDevice);
}

/**
 * The function is responsible for discovery of owned/unowned device is specified endpoint/deviceID.
 * And this function will only return the specified device's response.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the device.
 * @param[in] deviceID         deviceID of target device.
 * @param[in] hostAddress       MAC address of target device.
 * @param[in] connType       ConnectivityType for discovery.
 * @param[out] ppFoundDevice     OCProvisionDev_t of found device.
 * @return OTM_SUCCESS in case of success and other value otherwise.
 */
OCStackResult OCDiscoverSingleDeviceInUnicast(unsigned short timeout, const OicUuid_t* deviceID,
                             const char* hostAddress, OCConnectivityType connType,
                             OCProvisionDev_t **ppFoundDevice)
{
    if( NULL == ppFoundDevice || NULL != *ppFoundDevice || 0 == timeout || NULL == deviceID ||
            NULL == hostAddress)
    {
        OIC_LOG(ERROR, TAG, "OCDiscoverSingleDeviceInUnicast : Invalid Parameter");
        return OC_STACK_INVALID_PARAM;
    }

    return PMSingleDeviceDiscoveryInUnicast(timeout, deviceID, hostAddress, connType,
            ppFoundDevice);
}

/**
 * The function is responsible for discovery of device is current subnet. It will list
 * all the device in subnet which are not yet owned. Please call OCInit with OC_CLIENT_SERVER as
 * OCMode.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the list of devices.
 * @param[out] ppList List of candidate devices to be provisioned
 * @return OTM_SUCCESS in case of success and other value otherwise.
 */
OCStackResult OCDiscoverUnownedDevices(unsigned short timeout, OCProvisionDev_t **ppList)
{
    if( ppList == NULL || *ppList != NULL || 0 == timeout)
    {
        return OC_STACK_INVALID_PARAM;
    }

    return PMDeviceDiscovery(timeout, false, ppList);
}

/**
 * The function is responsible for discovery of owned device is current subnet. It will list
 * all the device in subnet which are owned by calling provisioning client.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the list of devices.
 * @param[out] ppList List of device owned by provisioning tool.
 * @return OTM_SUCCESS in case of success and other value otherwise.
 */
OCStackResult OCDiscoverOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList)
{
    if( ppList == NULL || *ppList != NULL || 0 == timeout)
    {
        return OC_STACK_INVALID_PARAM;
    }

    return PMDeviceDiscovery(timeout, true, ppList);
}

#ifdef MULTIPLE_OWNER
/**
 * The function is responsible for discovery of MOT enabled device is current subnet.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the list of devices.
 * @param[out] ppList List of MOT enabled devices.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCDiscoverMultipleOwnerEnabledDevices(unsigned short timeout, OCProvisionDev_t **ppList)
{
    if( ppList == NULL || *ppList != NULL || 0 == timeout)
    {
        return OC_STACK_INVALID_PARAM;
    }

    return PMMultipleOwnerDeviceDiscovery(timeout, false, ppList);
}

/**
 * The function is responsible for discovery of Multiple Owned device is current subnet.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the list of devices.
 * @param[out] ppList List of Multiple Owned devices.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCDiscoverMultipleOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList)
{
    if( ppList == NULL || *ppList != NULL || 0 == timeout)
    {
        return OC_STACK_INVALID_PARAM;
    }

    return PMMultipleOwnerDeviceDiscovery(timeout, true, ppList);
}


/**
 * API to add preconfigured PIN to local SVR DB.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] preconfigPin Preconfig PIN which is used while multiple owner authentication
 * @param[in] preconfigPinLen Byte length of preconfigPin
 *
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCAddPreconfigPin(const OCProvisionDev_t *targetDeviceInfo,
                                const char *preconfigPin,
                                size_t preconfigPinLen)
{
    return MOTAddPreconfigPIN(targetDeviceInfo, preconfigPin, preconfigPinLen);
}


OCStackResult OCDoMultipleOwnershipTransfer(void* ctx,
                                      OCProvisionDev_t *targetDevices,
                                      OCProvisionResultCB resultCallback)
{
    if( NULL == targetDevices )
    {
        return OC_STACK_INVALID_PARAM;
    }
    if (NULL == resultCallback)
    {
        OIC_LOG(INFO, TAG, "OCDoOwnershipTransfer : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }
    return MOTDoOwnershipTransfer(ctx, targetDevices, resultCallback);
}

OCStackResult OCRemoveSubOwner(void* ctx,
                                const OCProvisionDev_t *targetDeviceInfo,
                                const OicUuid_t* subOwner,
                                OCProvisionResultCB resultCallback)
{
    if (NULL == targetDeviceInfo || NULL == subOwner)
    {
        OIC_LOG_V(ERROR, TAG, "%s : NULL Param", __func__);
        return OC_STACK_INVALID_PARAM;
    }
    if (NULL == resultCallback)
    {
        OIC_LOG_V(ERROR, TAG, "%s : NULL Callback", __func__);
        return OC_STACK_INVALID_CALLBACK;
    }

    return MOTRemoveSubOwner(ctx, targetDeviceInfo, subOwner, resultCallback);
}

OCStackResult OCRemoveAllSubOwner(void* ctx,
                                const OCProvisionDev_t *targetDeviceInfo,
                                OCProvisionResultCB resultCallback)
{
    if (NULL == targetDeviceInfo)
    {
        OIC_LOG_V(ERROR, TAG, "%s : NULL Param", __func__);
        return OC_STACK_INVALID_PARAM;
    }
    if (NULL == resultCallback)
    {
        OIC_LOG_V(ERROR, TAG, "%s : NULL Callback", __func__);
        return OC_STACK_INVALID_CALLBACK;
    }

    return MOTRemoveSubOwner(ctx, targetDeviceInfo, &WILDCARD_SUBJECT_ID, resultCallback);
}


#endif //MULTIPLE_OWNER

/**
 * API to register for particular OxM.
 *
 * @param[in] Ownership transfer method.
 * @param[in] Implementation of callback functions for owership transfer.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCSetOwnerTransferCallbackData(OicSecOxm_t oxm, OTMCallbackData_t* callbackData)
{
    if(NULL == callbackData)
    {
        return OC_STACK_INVALID_CALLBACK ;
    }

    return OTMSetOwnershipTransferCallbackData(oxm, callbackData);
}

/**
 * API to set a allow status of OxM
 *
 * @param[in] oxm Owership transfer method (ref. OicSecOxm_t)
 * @param[in] allowStatus allow status (true = allow, false = not allow)
 *
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCSetOxmAllowStatus(const OicSecOxm_t oxm, const bool allowStatus)
{
    return OTMSetOxmAllowStatus(oxm, allowStatus);
}

OCStackResult OCDoOwnershipTransfer(void* ctx,
                                      OCProvisionDev_t *targetDevices,
                                      OCProvisionResultCB resultCallback)
{
    if( NULL == targetDevices )
    {
        return OC_STACK_INVALID_PARAM;
    }
    if (!resultCallback)
    {
        OIC_LOG(INFO, TAG, "OCDoOwnershipTransfer : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }
    return OTMDoOwnershipTransfer(ctx, targetDevices, resultCallback);
}

/**
 * This function deletes memory allocated to linked list created by OCDiscover_XXX_Devices API.
 *
 * @param[in] pList Pointer to OCProvisionDev_t which should be deleted.
 */
void OCDeleteDiscoveredDevices(OCProvisionDev_t *pList)
{
    PMDeleteDeviceList(pList);
}

/**
 * this function sends ACL information to resource.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] acl ACL to provision.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCProvisionACL(void* ctx, const OCProvisionDev_t *selectedDeviceInfo, OicSecAcl_t *acl,
                             OCProvisionResultCB resultCallback)
{
    return SRPProvisionACL(ctx, selectedDeviceInfo, acl, resultCallback);
}

/**
 * function to save ACL which has several ACE into Acl of SVR.
 *
 * @param acl ACL to be saved in Acl of SVR.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCSaveACL(const OicSecAcl_t* acl)
{
    return SRPSaveACL(acl);
}

/**
 * this function requests CRED information to resource.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCGetCredResource(void* ctx, const OCProvisionDev_t *selectedDeviceInfo,
                             OCProvisionResultCB resultCallback)
{
    return SRPGetCredResource(ctx, selectedDeviceInfo, resultCallback);
}

/**
 * this function requests ACL information to resource.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCGetACLResource(void* ctx, const OCProvisionDev_t *selectedDeviceInfo,
                             OCProvisionResultCB resultCallback)
{
    return SRPGetACLResource(ctx, selectedDeviceInfo, resultCallback);
}


OCStackResult OCReadTrustCertChain(uint16_t credId, uint8_t **trustCertChain,
                                     size_t *chainSize)
{
    return SRPReadTrustCertChain(credId, trustCertChain, chainSize);
}
/**
 * function to provision credential to devices.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] pDev1 Pointer to OCProvisionDev_t instance,respresenting resource to be provsioned.
   @param[in] pDev2 Pointer to OCProvisionDev_t instance,respresenting resource to be provsioned.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCProvisionCredentials(void *ctx, OicSecCredType_t type, size_t keySize,
                                      const OCProvisionDev_t *pDev1,
                                      const OCProvisionDev_t *pDev2,
                                      OCProvisionResultCB resultCallback)
{
    return SRPProvisionCredentials(ctx, type, keySize,
                                      pDev1, pDev2, resultCallback);

}

/**
 * this function sends Direct-Pairing Configuration to a device.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] pconf PCONF pointer.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCProvisionDirectPairing(void* ctx, const OCProvisionDev_t *selectedDeviceInfo, OicSecPconf_t *pconf,
                             OCProvisionResultCB resultCallback)
{
    return SRPProvisionDirectPairing(ctx, selectedDeviceInfo, pconf, resultCallback);
}

#ifdef MULTIPLE_OWNER
static void AddPreconfPinOxMCB(void* ctx, int nOfRes, OCProvisionResult_t *arr, bool hasError)
{
    ProvPreconfPINCtx_t* provCtx = (ProvPreconfPINCtx_t*)ctx;
    if(provCtx)
    {
        OCStackResult res = MOTProvisionPreconfigPIN(provCtx->ctx, provCtx->devInfo, provCtx->pin, provCtx->pinLen, provCtx->resultCallback);
        if(OC_STACK_OK != res)
        {
            arr->res = res;
            provCtx->resultCallback(provCtx->ctx, nOfRes, arr, true);
        }
    }
}

OCStackResult OCProvisionPreconfigPin(void *ctx,
                                      OCProvisionDev_t *targetDeviceInfo,
                                      const char *preconfigPin,
                                      size_t preconfigPinLen,
                                      OCProvisionResultCB resultCallback)
{
    if( NULL == targetDeviceInfo || NULL == preconfigPin || 0 == preconfigPinLen )
    {
        return OC_STACK_INVALID_PARAM;
    }
    if (NULL == resultCallback)
    {
        OIC_LOG(INFO, TAG, "OCProvisionPreconfigPinCredential : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }

    ProvPreconfPINCtx_t* provCtx = (ProvPreconfPINCtx_t*)OICCalloc(1, sizeof(ProvPreconfPINCtx_t));
    if(NULL == provCtx)
    {
        return OC_STACK_NO_MEMORY;
    }
    provCtx->ctx = ctx;
    provCtx->devInfo = targetDeviceInfo;
    provCtx->pin = preconfigPin;
    provCtx->pinLen = preconfigPinLen;
    provCtx->resultCallback = resultCallback;
    /*
     * First of all, update OxMs to support preconfigured PIN OxM.
     * In case of Preconfigured PIN OxM already supported on the server side,
     * MOTAddMOTMethod API will be send POST Cred request.
     * In case of Preconfigure PIN OxM not exist on the server side,
     * the MOTAddMOTMethod API will be send POST doxm request to update OxMs and then send POST Cred request.
     */
    return MOTAddMOTMethod((void*)provCtx, targetDeviceInfo, OIC_PRECONFIG_PIN, AddPreconfPinOxMCB);
}
#endif //MULTIPLE_OWNER

/*
* Function to unlink devices.
* This function will remove the credential & relationship between the two devices.
*
* @param[in] ctx Application context would be returned in result callback
* @param[in] pTargetDev1 first device information to be unlinked.
* @param[in] pTargetDev2 second device information to be unlinked.
* @param[in] resultCallback callback provided by API user, callback will be called when
*            device unlink is finished.
 * @return  OC_STACK_OK in case of success and other value otherwise.
*/
OCStackResult OCUnlinkDevices(void* ctx,
                              const OCProvisionDev_t* pTargetDev1,
                              const OCProvisionDev_t* pTargetDev2,
                              OCProvisionResultCB resultCallback)
{
    OIC_LOG(INFO, TAG, "IN OCUnlinkDevices");
    OCUuidList_t* idList = NULL;
    size_t numOfDev = 0;

    if (!pTargetDev1 || !pTargetDev2 || !pTargetDev1->doxm || !pTargetDev2->doxm)
    {
        OIC_LOG(ERROR, TAG, "OCUnlinkDevices : NULL parameters");
        return OC_STACK_INVALID_PARAM;
    }
    if (!resultCallback)
    {
        OIC_LOG(INFO, TAG, "OCUnlinkDevices : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }
    if (0 == memcmp(&pTargetDev1->doxm->deviceID, &pTargetDev2->doxm->deviceID, sizeof(OicUuid_t)))
    {
        OIC_LOG(INFO, TAG, "OCUnlinkDevices : Same device ID");
        return OC_STACK_INVALID_PARAM;
    }

    // Get linked devices with the first device.
    OCStackResult res = PDMGetLinkedDevices(&(pTargetDev1->doxm->deviceID), &idList, &numOfDev);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "OCUnlinkDevices : PDMgetOwnedDevices failed");
        goto error;
    }
    if (1 > numOfDev)
    {
        OIC_LOG(DEBUG, TAG, "OCUnlinkDevices : Can not find linked devices");
        res = OC_STACK_INVALID_PARAM; // Input devices are not linked, No request is made
        goto error;
    }

    // Check the linked devices contains the second device. If yes send credential DELETE request.
    OCUuidList_t* curDev = idList;
    while (NULL != curDev)
    {
        if (memcmp(pTargetDev2->doxm->deviceID.id, curDev->dev.id, sizeof(curDev->dev.id)) == 0)
        {
            res = SRPUnlinkDevices(ctx, pTargetDev1, pTargetDev2, resultCallback);
            if (OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, TAG, "OCUnlinkDevices : Failed to unlink devices.");
            }
            goto error;
        }
        curDev = curDev->next;
    }
    OIC_LOG(DEBUG, TAG, "No matched pair found from provisioning database");
    res = OC_STACK_INVALID_PARAM; // Input devices are not linked, No request is made

error:
    OIC_LOG(INFO, TAG, "OUT OCUnlinkDevices");

    PDMDestoryOicUuidLinkList(idList);
    return res;
}

static OCStackResult RemoveDeviceInfoFromLocal(const OCProvisionDev_t* pTargetDev)
{
    // Remove credential of revoked device from SVR database
    OCStackResult res = OC_STACK_ERROR;
    const OicSecCred_t *cred = NULL;

    OIC_LOG(DEBUG, TAG, "IN RemoveDeviceInfoFromLocal");
    cred = GetCredResourceData(&pTargetDev->doxm->deviceID);
    if (NULL != cred)
    {
        res = RemoveCredential(&cred->subject);
        if (res != OC_STACK_RESOURCE_DELETED)
        {
            OIC_LOG(ERROR, TAG, "RemoveDeviceInfoFromLocal : Failed to remove credential.");
            goto error;
        }
    }
    /**
     * Change the device status as stale status.
     * If all request are successed, this device information will be deleted.
     */
    res = PDMSetDeviceState(&pTargetDev->doxm->deviceID, PDM_DEVICE_STALE);
    if (res != OC_STACK_OK)
    {
        OIC_LOG(WARNING, TAG, "OCRemoveDevice : Failed to set device status as stale");
    }

    // TODO: We need to add new mechanism to clean up the stale state of the device.

    // Close the DTLS session of the removed device.
    CAResult_t caResult = CAcloseSslConnectionUsingUuid(pTargetDev->doxm->deviceID.id
                                                        , sizeof(pTargetDev->doxm->deviceID.id));
    if(CA_STATUS_OK != caResult)
    {
        OIC_LOG_V(WARNING, TAG, "OCRemoveDevice : Failed to close (D)TLS session : %d", caResult);
    }
    OIC_LOG(DEBUG, TAG, "OUT RemoveDeviceInfoFromLocal");
error:
    return res;
}

/*
* Function to device revocation
* This function will remove credential of target device from all devices in subnet.
*
* @param[in] ctx Application context would be returned in result callback
* @param[in] waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device discovery.(seconds)
* @param[in] pTargetDev Device information to be revoked.
* @param[in] resultCallback callback provided by API user, callback will be called when
*            credential revocation is finished.
 * @return  OC_STACK_OK in case of success and other value otherwise.
*/
OCStackResult OCRemoveDevice(void* ctx, unsigned short waitTimeForOwnedDeviceDiscovery,
                            const OCProvisionDev_t* pTargetDev,
                            OCProvisionResultCB resultCallback)
{
    OIC_LOG(INFO, TAG, "IN OCRemoveDevice");
    OCStackResult res = OC_STACK_ERROR;
    if (!pTargetDev || 0 == waitTimeForOwnedDeviceDiscovery)
    {
        OIC_LOG(INFO, TAG, "OCRemoveDevice : Invalied parameters");
        return OC_STACK_INVALID_PARAM;
    }
    if (!resultCallback)
    {
        OIC_LOG(INFO, TAG, "OCRemoveDevice : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }

    // Send DELETE requests to linked devices
    OCStackResult resReq = OC_STACK_ERROR; // Check that we have to wait callback or not.
    resReq = SRPRemoveDevice(ctx, waitTimeForOwnedDeviceDiscovery, pTargetDev, resultCallback);
    if (OC_STACK_OK != resReq)
    {
        if (OC_STACK_CONTINUE == resReq)
        {
            OIC_LOG(DEBUG, TAG, "OCRemoveDevice : Revoked device has no linked device except PT.");
        }
        else
        {
            OIC_LOG(ERROR, TAG, "OCRemoveDevice : Failed to invoke SRPRemoveDevice");
            res = resReq;
            goto error;
        }
    }

    res = RemoveDeviceInfoFromLocal(pTargetDev);
    if(OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Filed to remove the device information from local.");
        goto error;
    }

    if(OC_STACK_CONTINUE == resReq)
    {
        /**
          * If there is no linked device, PM does not send any request.
          * So we should directly invoke the result callback to inform the result of OCRemoveDevice.
          */
        if(resultCallback)
        {
            resultCallback(ctx, 0, NULL, false);
        }
        res = OC_STACK_OK;
    }

error:
    OIC_LOG(INFO, TAG, "OUT OCRemoveDevice");
    return res;
}

/*
* Function to device revocation
* This function will remove credential of target device from all devices in subnet.
*
* @param[in] ctx Application context would be returned in result callback
* @param[in] waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device discovery.(seconds)
* @param[in] pTargetDev Device information to be revoked.
* @param[in] resultCallback callback provided by API user, callback will be called when
*            credential revocation is finished.
 * @return  OC_STACK_OK in case of success and other value otherwise.
*/
OCStackResult OCRemoveDeviceWithUuid(void* ctx, unsigned short waitTimeForOwnedDeviceDiscovery,
                            const OicUuid_t* pTargetUuid,
                            OCProvisionResultCB resultCallback)
{
    OIC_LOG(INFO, TAG, "IN OCRemoveDeviceWithUuid");

    OCStackResult res = OC_STACK_ERROR;
    OCProvisionDev_t* pTargetDev = NULL;
    bool discoverdFlag = false;
    OCProvisionDev_t* pOwnedDevList = NULL;
    OCStackResult resReq = OC_STACK_CONTINUE;

    if (!pTargetUuid || 0 == waitTimeForOwnedDeviceDiscovery)
    {
        OIC_LOG(INFO, TAG, "OCRemoveDeviceWithUuid : Invalied parameters");
        return OC_STACK_INVALID_PARAM;
    }
    if (!resultCallback)
    {
        OIC_LOG(INFO, TAG, "OCRemoveDeviceWithUuid : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }

    char* strUuid = NULL;
    if(OC_STACK_OK != ConvertUuidToStr(pTargetUuid, &strUuid))
    {
        OIC_LOG(WARNING, TAG, "Failed to covert UUID to String.");
        goto error;
    }

    //Generate OCProvisionDev_t instance to use when target device not found on the network.
    //In this case, the device id required only.
    pTargetDev = (OCProvisionDev_t*)OICCalloc(1, sizeof(OCProvisionDev_t));
    if(NULL == pTargetDev)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate memory.");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }
    pTargetDev->doxm = (OicSecDoxm_t*)OICCalloc(1, sizeof(OicSecDoxm_t));
    if(NULL == pTargetDev->doxm)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate memory.");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }
    memcpy(pTargetDev->doxm->deviceID.id, pTargetUuid->id, sizeof(pTargetUuid->id));

    OCUuidList_t* linkedDevices = NULL;
    size_t numOfLinkedDevices = 0;
    res = PDMGetLinkedDevices(pTargetUuid, &linkedDevices, &numOfLinkedDevices);
    if(OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Error in PDMGetLinkedDevices");
        goto error;
    }
    PDMDestoryOicUuidLinkList(linkedDevices);

    //If there is no linked devices, device revocation step can be skipped.
    if(0 != numOfLinkedDevices)
    {
        OIC_LOG_V(INFO, TAG, "[%s] linked with other devices.", strUuid);
        //2. Find owned device from the network
        res = PMDeviceDiscovery(waitTimeForOwnedDeviceDiscovery, true, &pOwnedDevList);
        if (OC_STACK_OK != res)
        {
            OIC_LOG(ERROR, TAG, "OCRemoveDeviceWithUuid : Failed to PMDeviceDiscovery");
            goto error;
        }

        OCProvisionDev_t* tempDev = NULL;
        LL_FOREACH(pOwnedDevList, tempDev)
        {
            if(memcmp(&tempDev->doxm->deviceID.id, pTargetUuid->id, sizeof(pTargetUuid->id)) == 0)
            {
                break;
            }
        }

        if(NULL == tempDev)
        {
            OIC_LOG_V(WARNING, TAG, "Can not find [%s] on the network.", strUuid);
            OIC_LOG_V(WARNING, TAG, "[%s]'s information will be deleted from local and other devices.", strUuid);
        }
        else
        {
            OICFree(pTargetDev->doxm);
            OICFree(pTargetDev);
            pTargetDev = tempDev;
            discoverdFlag = true;
            OIC_LOG_V(INFO, TAG, "[%s] is dectected on the network.", strUuid);
        }

        OIC_LOG_V(INFO, TAG, "Trying [%s] revocation.", strUuid);

        // Send DELETE requests to linked devices
        resReq = SRPRemoveDeviceWithoutDiscovery(ctx, pOwnedDevList, pTargetDev, resultCallback);
        if (OC_STACK_OK != resReq)
        {
            if (OC_STACK_CONTINUE == resReq)
            {
                OIC_LOG(INFO, TAG, "OCRemoveDeviceWithUuid : Revoked device has no linked device except PT.");
            }
            else
            {
                OIC_LOG(ERROR, TAG, "OCRemoveDeviceWithUuid : Failed to invoke SRPRemoveDevice");
                res = resReq;
                goto error;
            }
        }
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "There is no linked devices with [%s]", strUuid);
        OIC_LOG(INFO, TAG, "Device discovery and SRPRemoveDevice will be skipped.");
    }

    res = RemoveDeviceInfoFromLocal(pTargetDev);
    if(OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "OCRemoveDeviceWithUuid : Filed to remove the device information from local.");
        goto error;
    }

    if(OC_STACK_CONTINUE == resReq)
    {
        /**
          * If there is no linked device, PM does not send any request.
          * So we should directly invoke the result callback to inform the result of OCRemoveDevice.
          */
        if(resultCallback)
        {
            resultCallback(ctx, 0, NULL, false);
        }
        res = OC_STACK_OK;
    }

error:
    OICFree(strUuid);
    PMDeleteDeviceList(pOwnedDevList);
    if(pTargetDev && false == discoverdFlag)
    {
        OICFree(pTargetDev->doxm);
        OICFree(pTargetDev);
    }
    OIC_LOG(INFO, TAG, "OUT OCRemoveDeviceWithUuid");
    return res;
}

/*
 * Function to reset the target device.
 * This function will remove credential and ACL of target device from all devices in subnet.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device discovery.(seconds)
 * @param[in] pTargetDev Device information to be revoked.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            credential revocation is finished.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCResetDevice(void* ctx, unsigned short waitTimeForOwnedDeviceDiscovery,
                            const OCProvisionDev_t* pTargetDev,
                            OCProvisionResultCB resultCallback)
{
    OIC_LOG(INFO, TAG, "IN OCResetDevice");
    OCStackResult res = OC_STACK_ERROR;
    if (!pTargetDev || 0 == waitTimeForOwnedDeviceDiscovery)
    {
        OIC_LOG(INFO, TAG, "OCResetDevice : Invalid parameters");
        return OC_STACK_INVALID_PARAM;
    }
    if (!resultCallback)
    {
        OIC_LOG(INFO, TAG, "OCResetDevice : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }

    // Send DELETE requests to linked devices
    res = SRPSyncDevice(ctx, waitTimeForOwnedDeviceDiscovery, pTargetDev, resultCallback);
    if (OC_STACK_CONTINUE == res)
    {
        OIC_LOG(DEBUG, TAG, "OCResetDevice : Target device has no linked device except PT.");
        if(resultCallback)
        {
            resultCallback(ctx, 0, NULL, false);
        }
        SRPResetDevice(pTargetDev, resultCallback);
        res = OC_STACK_OK;
    }
    else if(OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "OCResetDevice : Failed to invoke SRPSyncDevice");
    }
    OIC_LOG(INFO, TAG, "OUT OCResetDevice");
    return res;
}

/**
 * This function resets SVR DB to its factory setting.
 *
 * @return OC_STACK_OK in case of successful reset and other value otherwise.
 */
OCStackResult OCResetSVRDB(void)
{
    return ResetSecureResourceInPS();
}

/**
 * This function configures SVR DB as self-ownership.
 *
 *@return OC_STACK_OK in case of successful configue and other value otherwise.
 */
OCStackResult OCConfigSelfOwnership(void)
{
    return ConfigSelfOwnership();
}

/**
 * Internal Function to update result in link result array.
 */
static void UpdateLinkResults(Linkdata_t *link, int device, OCStackResult stackresult)
{

    OIC_LOG_V(INFO,TAG,"value of link->currentCountResults is %d",link->currentCountResults);
    if (1 == device)
    {
        memcpy(link->resArr[(link->currentCountResults)].deviceId.id, link->pDev1->doxm->deviceID.id,UUID_LENGTH);
    }
    else
    {
        memcpy(link->resArr[(link->currentCountResults)].deviceId.id, link->pDev2->doxm->deviceID.id,UUID_LENGTH);
    }
    link->resArr[(link->currentCountResults)].res = stackresult;
    ++(link->currentCountResults);

}

/**
 * Callback to handle ACL provisioning for device 2.
 */
static void AclProv2CB(void* ctx, int nOfRes, OCProvisionResult_t *arr, bool hasError)
{

    if (NULL == ctx)
    {
        OIC_LOG(ERROR,TAG,"Context is Null in ACLProv 2");
        return;
    }
    (void)nOfRes;
    Linkdata_t *link = (Linkdata_t*)ctx;
    OCProvisionResultCB resultCallback = link->resultCallback;


    if (hasError)
    {
        UpdateLinkResults(link, 2,arr[0].res);
        OIC_LOG(ERROR,TAG,"Error occured while ACL provisioning device 1");
        ((OCProvisionResultCB)(resultCallback))(link->ctx, link->currentCountResults,
                                                link->resArr,
                                                true);
        OICFree(link->resArr);
        OICFree(link) ;
        return;
    }
    UpdateLinkResults(link, 2, arr[0].res);
   ((OCProvisionResultCB)(resultCallback))(link->ctx, link->currentCountResults,
                                           link->resArr,
                                           false);
    OICFree(link->resArr);
    OICFree(link);
    return;
}

/**
 * Callback to handle ACL provisioning for device 1
 */
static void AclProv1CB(void* ctx, int nOfRes, OCProvisionResult_t *arr, bool hasError)
{

    if (NULL == ctx)
    {
        OIC_LOG(ERROR,TAG,"Context is Null in ACLProv1");
        return;
    }
    (void)nOfRes;
    Linkdata_t *link = (Linkdata_t*)ctx;
    OCProvisionResultCB resultCallback = link->resultCallback;

    if (hasError)
    {
        OIC_LOG(ERROR,TAG,"Error occured while ACL provisioning device 1");
        UpdateLinkResults(link, 1, arr[0].res);
        ((OCProvisionResultCB)(resultCallback))(link->ctx, link->currentCountResults,
                                                link->resArr,
                                                true);
        OICFree(link->resArr);
        OICFree(link);
        return;
    }
    UpdateLinkResults(link, 1, arr[0].res);
    if (NULL != link->pDev2Acl)
    {
        OCStackResult res =  SRPProvisionACL(ctx, link->pDev2, link->pDev2Acl, &AclProv2CB);
        if (OC_STACK_OK!=res)
        {
             UpdateLinkResults(link, 2, res);
             ((OCProvisionResultCB)(resultCallback))(link->ctx, link->currentCountResults,
                                                     link->resArr,
                                                     true);

        }
    }
    else
    {
        ((OCProvisionResultCB)(resultCallback))(link->ctx, link->currentCountResults,
                                                link->resArr,
                                                false);
        OICFree(link->resArr);
        OICFree(link);
    }

    return;
}

/**
 * Callback to handle credential provisioning.
 */
static void ProvisionCredsCB(void* ctx, int nOfRes, OCProvisionResult_t *arr, bool hasError)
{
    if (NULL == ctx)
    {
        OIC_LOG(ERROR,TAG,"Error occured while credential provisioning");
        return;
    }
    Linkdata_t *link = (Linkdata_t*)ctx;
    OCProvisionResultCB resultCallback = link->resultCallback;
    OIC_LOG_V(INFO, TAG, "has error returned %d",hasError);
    UpdateLinkResults(link, 1, arr[0].res);
    UpdateLinkResults(link, 2, arr[1].res);
    if (hasError)
    {
        OIC_LOG(ERROR,TAG,"Error occured while credential provisioning");
        ((OCProvisionResultCB)(resultCallback))(link->ctx, nOfRes,
                                                link->resArr,
                                                true);
         OICFree(link->resArr);
         OICFree(link);
         return;
    }
    if (NULL != link->pDev1Acl)
    {

        OCStackResult res =  SRPProvisionACL(ctx, link->pDev1, link->pDev1Acl, &AclProv1CB);
        if (OC_STACK_OK!=res)
        {
             OIC_LOG(ERROR, TAG, "Error while provisioning ACL for device 1");
             UpdateLinkResults(link, 1, res);
             ((OCProvisionResultCB)(resultCallback))(link->ctx, link->currentCountResults,
                                                     link->resArr,
                                                     true);
              OICFree(link->resArr);
              OICFree(link);
        }
    }
    else if (NULL!=link->pDev2Acl)
    {
        OIC_LOG(ERROR, TAG, "ACL for device 1 is NULL");
        OCStackResult res =  SRPProvisionACL(ctx, link->pDev2, link->pDev2Acl, &AclProv2CB);
        if (OC_STACK_OK!=res)
        {
             OIC_LOG(ERROR, TAG, "Error while provisioning ACL for device 2");
              UpdateLinkResults(link, 2, res);
             ((OCProvisionResultCB)(resultCallback))(link->ctx, link->currentCountResults,
                                                     link->resArr,
                                                     true);
              OICFree(link->resArr);
              OICFree(link);
        }
    }
    else
    {
        OIC_LOG(INFO, TAG, "ACLs of both devices are NULL");
        ((OCProvisionResultCB)(resultCallback))(link->ctx, link->currentCountResults,
                                                link->resArr,
                                                false);
        OICFree(link->resArr);
        OICFree(link);
    }
    return;
}
/**
 * function to provision credentials between two devices and ACLs for the devices who act as a server.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] pDev1 Pointer to OCProvisionDev_t instance,respresenting resource to be provsioned.
 * @param[in] acl ACL for device 1. If this is not required set NULL.
 * @param[in] pDev2 Pointer to OCProvisionDev_t instance,respresenting resource to be provsioned.
 * @param[in] acl ACL for device 2. If this is not required set NULL.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCProvisionPairwiseDevices(void* ctx, OicSecCredType_t type, size_t keySize,
                                         const OCProvisionDev_t *pDev1, OicSecAcl_t *pDev1Acl,
                                         const OCProvisionDev_t *pDev2, OicSecAcl_t *pDev2Acl,
                                         OCProvisionResultCB resultCallback)
{

    if (!pDev1 || !pDev2 || !pDev1->doxm || !pDev2->doxm)
    {
        OIC_LOG(ERROR, TAG, "OCProvisionPairwiseDevices : Invalid parameters");
        return OC_STACK_INVALID_PARAM;
    }
    if (!resultCallback)
    {
        OIC_LOG(INFO, TAG, "OCProvisionPairwiseDevices : NULL Callback");
        return OC_STACK_INVALID_CALLBACK;
    }
    if (!(keySize == OWNER_PSK_LENGTH_128 || keySize == OWNER_PSK_LENGTH_256))
    {
        OIC_LOG(INFO, TAG, "OCProvisionPairwiseDevices : Invalid key size");
        return OC_STACK_INVALID_PARAM;
    }
    if (0 == memcmp(&pDev1->doxm->deviceID, &pDev2->doxm->deviceID, sizeof(OicUuid_t)))
    {
        OIC_LOG(INFO, TAG, "OCProvisionPairwiseDevices : Same device ID");
        return OC_STACK_INVALID_PARAM;
    }

    OIC_LOG(DEBUG, TAG, "Checking link in DB");
    bool linkExists = true;
    OCStackResult res = PDMIsLinkExists(&pDev1->doxm->deviceID, &pDev2->doxm->deviceID, &linkExists);
    if(res != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "Internal Error Occured");
        return res;
    }
    if (linkExists)
    {
        OIC_LOG(ERROR, TAG, "Link already exists");
        return OC_STACK_INVALID_PARAM;
    }

    int noOfResults = 2; // Initial Value
    if (NULL != pDev1Acl)
    {
        ++noOfResults;
    }
    if (NULL != pDev2Acl)
    {
       ++noOfResults;
    }
    Linkdata_t *link = (Linkdata_t*) OICMalloc(sizeof(Linkdata_t));
    if (!link)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        return OC_STACK_NO_MEMORY;
    }
    OIC_LOG_V(INFO,TAG, "Maximum no od results %d",noOfResults);

    link->pDev1 = pDev1;
    link->pDev1Acl = pDev1Acl;
    link->pDev2 = pDev2;
    link->pDev2Acl = pDev2Acl;
    link->ctx = ctx;
    // 1 call for each device for credential provisioning. implict call by SRPProvisioning credential
    // 1 call for ACL provisioning for device 1 and 1 call for ACL provisioning for device 2.
    link->numOfResults = noOfResults;
    link->resultCallback = resultCallback;
    link->currentCountResults = 0;
    link->resArr = (OCProvisionResult_t*) OICMalloc(sizeof(OCProvisionResult_t)*noOfResults);
    res = SRPProvisionCredentials(link, type, keySize,
                                     pDev1, pDev2, &ProvisionCredsCB);
    if (res != OC_STACK_OK)
    {
        OICFree(link->resArr);
        OICFree(link);
    }
    return res;

}

OCStackResult OCGetDevInfoFromNetwork(unsigned short waittime,
                                       OCProvisionDev_t** pOwnedDevList,
                                       OCProvisionDev_t** pUnownedDevList)
{
    //TODO will be replaced by more efficient logic
    if (pOwnedDevList == NULL || *pOwnedDevList != NULL || pUnownedDevList == NULL
         || *pUnownedDevList != NULL || 0 == waittime)
    {
        return OC_STACK_INVALID_PARAM;
    }

    // Code for unowned discovery
    OCProvisionDev_t *unownedDevice = NULL;
    OCStackResult res =  OCDiscoverUnownedDevices(waittime/2, &unownedDevice);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR,TAG, "Error in unowned discovery");
        return res;
    }

    // Code for owned discovery
    OCProvisionDev_t *ownedDevice = NULL;
    res =  OCDiscoverOwnedDevices(waittime/2, &ownedDevice);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR,TAG, "Error in owned discovery");
        PMDeleteDeviceList(unownedDevice);
        return res;
    }

    // Code to get list of all the owned devices.
    OCUuidList_t *uuidList = NULL;
    size_t numOfDevices = 0;
    res =  PDMGetOwnedDevices(&uuidList, &numOfDevices);
    if (OC_STACK_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Error while getting info from DB");
        PMDeleteDeviceList(unownedDevice);
        PMDeleteDeviceList(ownedDevice);
        return res;
    }

    // Code to compare devices in unowned list and deviceid from DB
    // (In case of hard reset of the device)
    OCProvisionDev_t* pUnownedList = unownedDevice;
    while (pUnownedList && uuidList)
    {
        OCUuidList_t *tmp1 = NULL,*tmp2=NULL;
        LL_FOREACH_SAFE(uuidList, tmp1, tmp2)
        {
            if(0 == memcmp(tmp1->dev.id, pUnownedList->doxm->deviceID.id,
                            sizeof(pUnownedList->doxm->deviceID.id)))
            {
                OIC_LOG_V(INFO, TAG, "OCGetDevInfoFromNetwork : \
                            Removing device id = %s in PDM and dat.", pUnownedList->doxm->deviceID.id);
                if (OC_STACK_OK != PDMDeleteDevice(&pUnownedList->doxm->deviceID))
                {
                    OIC_LOG(ERROR, TAG, "OCGetDevInfoFromNetwork : \
                            Failed to remove device in PDM.");
                }
                //remove the cred entry from dat file
                if (OC_STACK_OK != RemoveDeviceInfoFromLocal(pUnownedList))
                {
                    OIC_LOG(ERROR, TAG, "OCGetDevInfoFromNetwork : \
                            Failed to remove cred entry device in dat file.");
                }
                LL_DELETE(uuidList, tmp1);
                OICFree(tmp1);
            }
        }
        pUnownedList = pUnownedList->next;
    }
    // Code to compare devices in owned list and deviceid from DB.
    OCProvisionDev_t* pCurDev = ownedDevice;
    size_t deleteCnt = 0;
    while (pCurDev)
    {
        if(true == PMDeleteFromUUIDList(&uuidList, &pCurDev->doxm->deviceID))
        {
            deleteCnt++;
        }
        pCurDev = pCurDev->next;
    }
    // If there is no remaind device in uuidList, we have to assign NULL to prevent free.
    if (deleteCnt == numOfDevices)
    {
        uuidList = NULL;
    }
    // Code to add information of the devices which are currently off in owned list.
    OCUuidList_t *powerOffDeviceList = uuidList;
    while (powerOffDeviceList)
    {
        OCProvisionDev_t *ptr = (OCProvisionDev_t *)OICCalloc(1, sizeof (OCProvisionDev_t));
        if (NULL == ptr)
        {
            OIC_LOG(ERROR,TAG,"Fail to allocate memory");
            PMDeleteDeviceList(unownedDevice);
            PMDeleteDeviceList(ownedDevice);
            OCDeleteUuidList(uuidList);
            return OC_STACK_NO_MEMORY;
        }

        ptr->doxm = (OicSecDoxm_t*)OICCalloc(1, sizeof(OicSecDoxm_t));
        if (NULL == ptr->doxm)
        {
            OIC_LOG(ERROR,TAG,"Fail to allocate memory");
            PMDeleteDeviceList(unownedDevice);
            PMDeleteDeviceList(ownedDevice);
            OCDeleteUuidList(uuidList);
            OICFree(ptr);
            return OC_STACK_NO_MEMORY;
        }

        memcpy(ptr->doxm->deviceID.id, powerOffDeviceList->dev.id, sizeof(ptr->doxm->deviceID.id));

        ptr->devStatus = DEV_STATUS_OFF;
        LL_PREPEND(ownedDevice, ptr);
        powerOffDeviceList = powerOffDeviceList->next;

    }
    OCDeleteUuidList(uuidList);
    *pOwnedDevList = ownedDevice;
    *pUnownedDevList = unownedDevice;
    return OC_STACK_OK;
}

OCStackResult OCGetLinkedStatus(const OicUuid_t* uuidOfDevice, OCUuidList_t** uuidList,
                                 size_t* numOfDevices)
{
    return PDMGetLinkedDevices(uuidOfDevice, uuidList, numOfDevices);
}

void OCDeleteUuidList(OCUuidList_t* pList)
{
    PDMDestoryOicUuidLinkList(pList);
}

/**
 * This function deletes ACL data.
 *
 * @param pAcl Pointer to OicSecAcl_t structure.
 */
void OCDeleteACLList(OicSecAcl_t* pAcl)
{
    DeleteACLList(pAcl);
}

/**
 * This function deletes PDACL data.
 *
 * @param pPdAcl Pointer to OicSecPdAcl_t structure.
 */
void OCDeletePdAclList(OicSecPdAcl_t* pPdAcl)
{
    FreePdAclList(pPdAcl);
}

#ifdef MULTIPLE_OWNER
/**
 * API to update 'doxm.mom' to resource server.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] momType Mode of multiple ownership transfer (ref. oic.sec.mom)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'mom' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCChangeMOTMode(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
                            const OicSecMomType_t momType, OCProvisionResultCB resultCallback)
{
    return MOTChangeMode(ctx, targetDeviceInfo, momType, resultCallback);
}

/**
 * API to update 'doxm.oxmsel' to resource server.
 *
 * @param[in] targetDeviceInfo Selected target device.
  * @param[in] oxmSelValue Method of multiple ownership transfer (ref. oic.sec.oxm)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'oxmsel' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCSelectMOTMethod(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
                                 const OicSecOxm_t oxmSelValue, OCProvisionResultCB resultCallback)
{
    return MOTSelectMOTMethod(ctx, targetDeviceInfo, oxmSelValue, resultCallback);
}
#endif //MULTIPLE_OWNER

/**
 * Function to select appropriate security provisioning method.
 *
 * @param[in] supportedMethods   Array of supported methods
 * @param[in] numberOfMethods   number of supported methods
 * @param[out]  selectedMethod         Selected methods
 * @param[in] ownerType type of owner device (SUPER_OWNER or SUB_OWNER)
 * @return  OC_STACK_OK on success
 */
OCStackResult OCSelectOwnershipTransferMethod(const OicSecOxm_t *supportedMethods,
        size_t numberOfMethods, OicSecOxm_t *selectedMethod, OwnerType_t ownerType)
{
    return OTMSelectOwnershipTransferMethod(supportedMethods, numberOfMethods,
                                            selectedMethod, ownerType);
}

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
/**
 * function to provision Trust certificate chain to devices.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] credId CredId of trust certificate chain to be provisioned to the device.
 * @param[in] selectedDeviceInfo Pointer to OCProvisionDev_t instance,respresenting resource to be provsioned.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCProvisionTrustCertChain(void *ctx, OicSecCredType_t type, uint16_t credId,
                                      const OCProvisionDev_t *selectedDeviceInfo,
                                      OCProvisionResultCB resultCallback)
{
    return SRPProvisionTrustCertChain(ctx, type, credId,
                                      selectedDeviceInfo, resultCallback);
}

/**
 * function to save Trust certificate chain into Cred of SVR.
 *
 * @param[in] trustCertChain Trust certificate chain to be saved in Cred of SVR.
 * @param[in] chainSize Size of trust certificate chain to be saved in Cred of SVR
 * @param[in] encodingType Encoding type of trust certificate chain to be saved in Cred of SVR
 * @param[out] credId CredId of saved trust certificate chain in Cred of SVR.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize,
                                    OicEncodingType_t encodingType, uint16_t *credId)
{
    return SRPSaveTrustCertChain(trustCertChain, chainSize, encodingType, credId);
}

/**
 * function to register notifier for Trustcertchain change.
 *
 * @param[in] ctx user context.
 * @param[in] TrustCertChainChangeCB notification callback fucntion.
 * @return    OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCRegisterTrustCertChainNotifier(void *ctx, TrustCertChainChangeCB Callback)
{
    return SRPRegisterTrustCertChainNotifier(ctx, Callback);
}

/**
 * function to de-register notifier for Trustcertchain change.
 */
void OCRemoveTrustCertChainNotifier()
{
    SRPRemoveTrustCertChainNotifier();
}
#endif // __WITH_DTLS__ || __WITH_TLS__

