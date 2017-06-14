//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "base64.h"

#include "EnrolleeSecurity.h"
#include "oxmjustworks.h"
#include "oxmrandompin.h"
#include "EnrolleeResource.h"
#include "logger.h"
#include "ESException.h"
#include "oic_malloc.h"
#include "provisioningdatabasemanager.h"
#include "oic_string.h"
#include "utlist.h"
#include "srmutility.h"
#include "aclresource.h"
#include "internal/doxmresource.h"
#include "ocrandom.h"

namespace OIC
{
    namespace Service
    {
        namespace
        {
            static const char COAP[] = "coap://";
            static const char COAPS[] = "coaps://";
            static const char COAP_TCP[] = "coap+tcp://";
            static const char COAP_GATT[] = "coap+gatt://";
            static const char COAP_RFCOMM[] = "coap+rfcomm://";
        }

        #define MAX_PERMISSION_LENGTH (5)
        #define CREATE (1)
        #define READ (2)
        #define UPDATE (4)
        #define DELETE (8)
        #define NOTIFY (16)
        #define DASH '-'

        // TODO : Currently discovery timeout for owned and unowned devices is fixed as 5
        // The value should be accepted from the application as a parameter during ocplatform
        // config call

#ifdef __APPLE__
        #define ES_SEC_DISCOVERY_TIMEOUT 15
#else
        #define ES_SEC_DISCOVERY_TIMEOUT 5
#endif

        EnrolleeSecurity::EnrolleeSecurity(std::shared_ptr< OC::OCResource > resource)
        {
            m_ocResource = resource;
        }

        void EnrolleeSecurity::onEnrolleeSecuritySafetyCB(OC::PMResultList_t *result,
                                                        int hasError,
                                                        ESSecurityCb cb,
                                                        std::weak_ptr<EnrolleeSecurity> this_ptr)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "onEnrolleeSecuritySafetyCB");
            std::shared_ptr<EnrolleeSecurity> Ptr = this_ptr.lock();
            if(Ptr)
            {
                cb(result, hasError);
            }
        }

        void EnrolleeSecurity::convertUUIDToString(const uint8_t uuid[UUID_SIZE],
                                                              std::string& uuidString)
        {
            char uuidArray[UUID_STRING_SIZE] = {'\0',};
            int ret = snprintf(uuidArray, UUID_STRING_SIZE,
                    "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                    uuid[0], uuid[1], uuid[2], uuid[3],
                    uuid[4], uuid[5], uuid[6], uuid[7],
                    uuid[8], uuid[9], uuid[10], uuid[11],
                    uuid[12], uuid[13], uuid[14], uuid[15]
                    );

            if (ret != UUID_STRING_SIZE - 1)
            {
                return;
            }

            uuidString = uuidArray;
        }

        std::string EnrolleeSecurity::getResourceDeviceAddress(const std::string& host)
        {
            size_t prefix_len = 0;

            if (host.compare(0, sizeof(COAP) - 1, COAP) == 0)
            {
                prefix_len = sizeof(COAP) - 1;
            }
            else if (host.compare(0, sizeof(COAPS) - 1, COAPS) == 0)
            {
                prefix_len = sizeof(COAPS) - 1;
            }
            else if (host.compare(0, sizeof(COAP_TCP) - 1, COAP_TCP) == 0)
            {
                prefix_len = sizeof(COAP_TCP) - 1;
            }
            else if (host.compare(0, sizeof(COAP_GATT) - 1, COAP_GATT) == 0)
            {
                prefix_len = sizeof(COAP_GATT) - 1;
            }
            else if (host.compare(0, sizeof(COAP_RFCOMM) - 1, COAP_RFCOMM) == 0)
            {
                prefix_len = sizeof(COAP_RFCOMM) - 1;
            }
            else
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG,
                    "Well-known prefix for connectivity is not found. Please check OCResource::setHost");
                return {};
            }

            // remove prefix
            std::string host_token = host.substr(prefix_len);

            if (host_token[0] == '[') // IPv6
            {
                size_t bracket = host_token.find(']');

                // extract the ipv6 address
                return host_token.substr(0, bracket + 1);
            }
            else
            {
                size_t dot = host_token.find('.');
                if (std::string::npos == dot) // MAC
                {
                    size_t semi_count = std::count(host_token.begin(), host_token.end(), ':');
                    if (semi_count > 5)
                    {
                        size_t found_semi = host_token.find_last_of(':');
                        host_token = host_token.substr(0, found_semi);
                    }
                    return host_token;
                }
                else // IPv4
                {
                    size_t colon = host_token.find(':');

                    // extract the ipv4 address
                    return host_token.substr(0, colon);
                }
            }
        }

        bool EnrolleeSecurity::isOwnerIDMatched(std::shared_ptr< OC::OCSecureResource > foundDevice)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "isOwnerIDMatched IN");

            if(foundDevice.get() == nullptr)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "foundDevice is NULL ptr");
                return false;
            }

            bool ret = false;
            std::string ownerID;
            char uuidString[UUID_STRING_SIZE] = {};
            if(RAND_UUID_OK == OCConvertUuidToString(foundDevice->getDevPtr()->doxm->owner.id, uuidString))
            {
                ownerID = uuidString;
            }
            else
            {
                ownerID = {};
            }

            OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "Mediator ID %s", m_mediatorID.c_str());
            OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "Enrollee's Owner ID %s", ownerID.c_str());

            if(ownerID == m_mediatorID)
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG,
                    "The found device's first owner ID is matched with Mediator's ID");
                ret = true;
            }
            else
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG,
                    "The found device's first owner ID is NOT matched with Mediator's ID");
            }

            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "isOwnerIDMatched OUT");

            return ret;
        }

#ifdef MULTIPLE_OWNER
        bool EnrolleeSecurity::isSubOwnerIDMatched(std::shared_ptr< OC::OCSecureResource > foundDevice)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "isSubOwnerIDMatched IN");

            if(foundDevice.get() == nullptr)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,
                    "The found device's sub owner ID is NOT matched with Mediator's ID");

                return false;
            }

            bool ret = false;
            std::string subOwnerID;
            char uuidString[UUID_STRING_SIZE] = {};

            OicSecSubOwner_t* subOwnerList = foundDevice->getDevPtr()->doxm->subOwners;

            while(subOwnerList)
            {
                if(RAND_UUID_OK == OCConvertUuidToString(subOwnerList->uuid.id, uuidString))
                {
                    subOwnerID = uuidString;
                }
                else
                {
                    subOwnerID = {};
                }

                if(subOwnerID == m_mediatorID)
                {
                    OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG,
                    "The found device's owner ID is matched with Mediator's ID as a second owner");
                    ret = true;
                    break;
                }
                subOwnerList = subOwnerList->next;
            }

            if(!ret)
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG,
                    "The found device's sub owner ID is NOT matched with Mediator's ID");
            }

            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "isSubOwnerIDMatched OUT");
            return ret;
        }

        void EnrolleeSecurity::changeMOTMethodCB(PMResultList_t *result, int hasError)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "changeMOTMethodCB IN");
            if (hasError)
            {
                OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG,
                                "requestEnableMOTMode API is failed with error %d", hasError);
                enableMOTModeResult = false;
            }
            else
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "requestEnableMOTMode API is succeeded");
                enableMOTModeResult = true;
            }

            delete result;
            m_cond.notify_all();
        }


        void EnrolleeSecurity::selectMOTMethodCB(PMResultList_t *result, int hasError)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "selectMOTMethodCB IN");
            if (hasError)
            {
                OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG,
                                "selectMOTMethod API is failed with error %d", hasError);
                motMethodProvResult = false;
            }
            else
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "selectMOTMethod API is succeeded");
                motMethodProvResult = true;
            }

            delete result;
            m_cond.notify_all();
        }

        void EnrolleeSecurity::preconfigPinProvCB(PMResultList_t *result, int hasError)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "preconfigPinProvCB IN");
            if (hasError)
            {
                OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG,
                                "provisionPreconfPin API is failed with error %d", hasError);
                preConfigPinProvResult = false;
            }
            else
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "provisionPreconfPin API is succeeded");
                preConfigPinProvResult = true;
            }

            delete result;
            m_cond.notify_all();
        }

        void EnrolleeSecurity::multipleOwnershipTransferCb(OC::PMResultList_t *result, int hasError)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "multipleOwnershipTransferCb IN");

            otmResult = false;

            if (hasError)
            {
                OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG, "multipleOwnershipTransferCb is failed with code(%d)", hasError);
                otmResult = false;
                m_cond.notify_all();
            }
            else
            {
                for (unsigned int i = 0; i < result->size(); i++)
                {
                    std::string uuid;
                    convertUUIDToString(result->at(i).deviceId.id, uuid);

                    if(m_ocResource != NULL && m_ocResource->sid() == uuid)
                    {
                        if( OC_STACK_OK == result->at(i).res )
                        {
                            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "multipleOwnershipTransferCb is succeeded");
                            OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "Result is = %d", result->at(i).res);
                            OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "device uuid : %s", uuid.c_str());

                            otmResult = true;
                        }
                        else
                        {
                            OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG, "multipleOwnershipTransfer is failed with code(%d)", hasError);
                            otmResult = false;
                        }
                    }
                }
                delete result;
                m_cond.notify_all();
            }
        }

        ESResult EnrolleeSecurity::requestSetPreconfPinData(const ESOwnershipTransferData& MOTData)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "requestSetPreconfPinData IN");

            ESResult res = ESResult::ES_ERROR;

            OC::ResultCallBack preconfigPinProvCB = std::bind(
                    &EnrolleeSecurity::onEnrolleeSecuritySafetyCB,
                    std::placeholders::_1, std::placeholders::_2,
                    static_cast<ESSecurityCb>(std::bind(&EnrolleeSecurity::preconfigPinProvCB,
                    this, std::placeholders::_1, std::placeholders::_2)),
                    shared_from_this());

            std::string pin = MOTData.getPreConfiguredPin();

            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "provisionPreconfPin is called.");
            if(OC_STACK_OK != m_securedResource->provisionPreconfPin(
                                    pin.c_str(), pin.length(), preconfigPinProvCB))
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "provisionPreconfPin API error");
                res = ESResult:: ES_PRE_CONFIG_PIN_PROVISIONING_FAILURE;
                return res;
            }

            std::unique_lock<std::mutex> lck(m_mtx);
            m_cond.wait(lck);

            if(!preConfigPinProvResult)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "provisionPreconfPin is failed.");
                res = ESResult:: ES_PRE_CONFIG_PIN_PROVISIONING_FAILURE;
                return res;
            }

            return ESResult::ES_OK;
        }

        ESResult EnrolleeSecurity::requestSetMOTMethod(const ESOwnershipTransferData& MOTData)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "requestSetMOTMethod IN");

            ESResult res = ESResult::ES_ERROR;

            OC::ResultCallBack selectMOTMethodCB = std::bind(
                    &EnrolleeSecurity::onEnrolleeSecuritySafetyCB,
                    std::placeholders::_1, std::placeholders::_2,
                    static_cast<ESSecurityCb>(std::bind(&EnrolleeSecurity::selectMOTMethodCB,
                    this, std::placeholders::_1, std::placeholders::_2)),
                    shared_from_this());

            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "selectMOTMethod is called.");
            if(OC_STACK_OK != m_securedResource->selectMOTMethod(
                                    MOTData.getMOTMethod(),
                                    selectMOTMethodCB))
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "selectMOTMethod API error");
                res = ESResult:: ES_MOT_METHOD_SELECTION_FAILURE;
                return res;
            }

            std::unique_lock<std::mutex> lck(m_mtx);
            m_cond.wait(lck);

            if(!motMethodProvResult)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "selectMOTMethod is failed.");
                res = ESResult:: ES_MOT_METHOD_SELECTION_FAILURE;
                return res;
            }

            return ESResult::ES_OK;
        }

        ESResult EnrolleeSecurity::requestEnableMOTMode()
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "requestEnableMOTMode IN");
            ESResult res = ESResult:: ES_ERROR;

            OC::ResultCallBack changeMOTMethodCB = std::bind(
                    &EnrolleeSecurity::onEnrolleeSecuritySafetyCB,
                    std::placeholders::_1, std::placeholders::_2,
                    static_cast<ESSecurityCb>(std::bind(&EnrolleeSecurity::changeMOTMethodCB,
                    this, std::placeholders::_1, std::placeholders::_2)),
                    shared_from_this());

            if(OC_STACK_OK !=
                m_securedResource->changeMOTMode(OIC_MULTIPLE_OWNER_ENABLE, changeMOTMethodCB))
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "changeMOTMode is failed.");
                return ESResult:: ES_MOT_ENABLING_FAILURE;
            }

            std::unique_lock<std::mutex> lck(m_mtx);
            m_cond.wait(lck);

            if(!enableMOTModeResult)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "requestEnableMOTMode is failed.");
                res = ESResult:: ES_MOT_ENABLING_FAILURE;
                return res;
            }

            return ESResult::ES_OK;
        }

        ESResult EnrolleeSecurity::provisionMOTConfig(const ESOwnershipTransferData& MOTData)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "provisionMOTConfig IN");
            ESResult res = ESResult:: ES_ERROR;

            if(!m_securedResource->isMOTEnabled())
            {
                res = requestEnableMOTMode();
                if(res != ESResult::ES_OK)
                {
                    OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "requestEnableMOTMode is failed.");
                    return res;
                }
            }

            if( OIC_PRECONFIG_PIN == MOTData.getMOTMethod() &&
                !MOTData.getPreConfiguredPin().empty())
            {
                res = requestSetPreconfPinData(MOTData);

                if(res != ESResult::ES_OK)
                {
                    OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "RequestSetPreconfPinData is failed.");
                    return res;
                }
            }
            if(OIC_PRECONFIG_PIN == MOTData.getMOTMethod() ||
               OIC_RANDOM_DEVICE_PIN == MOTData.getMOTMethod())
            {
                res = requestSetMOTMethod(MOTData);

                if(res != ESResult::ES_OK)
                {
                    OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "RequestSetMOTMethod is failed.");
                    return res;
                }
            }
            return res;
        }
#endif

        void EnrolleeSecurity::ownershipTransferCb(OC::PMResultList_t *result, int hasError
                                                   , ESResult& res)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "ownershipTransferCb IN");

            otmResult = false;

            if (hasError)
            {
                for (unsigned int i = 0; i < result->size(); i++)
                {
                    std::string uuid;
                    convertUUIDToString(result->at(i).deviceId.id, uuid);

                    if(m_ocResource != NULL && m_ocResource->sid() == uuid)
                    {
                        if(OC_STACK_USER_DENIED_REQ == result->at(i).res)
                        {
                            res = ESResult::ES_USER_DENIED_CONFIRMATION_REQ;
                        }
                        else if(OC_STACK_AUTHENTICATION_FAILURE  == result->at(i).res)
                        {
                            OicSecOxm_t oxm = OIC_OXM_COUNT;
                            if(OC_STACK_OK != m_securedResource->getOTMethod(&oxm))
                            {
                                otmResult = false;
                                return;
                            }

                            if(OIC_MANUFACTURER_CERTIFICATE == oxm)
                            {
                                res = ESResult::ES_AUTHENTICATION_FAILURE_WITH_WRONG_CERT;
                            }
                            else if(OIC_CON_MFG_CERT == oxm)
                            {
                                res = ESResult::ES_AUTHENTICATION_FAILURE_WITH_WRONG_CERT;
                            }
                            else if(OIC_RANDOM_DEVICE_PIN == oxm)
                            {
                                res = ESResult::ES_AUTHENTICATION_FAILURE_WITH_WRONG_PIN;
                            }
                        }
                    }
                }
                OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG, "OwnershipTransfer is failed with ESResult(%d)", res);

                otmResult = false;
            }
            else
            {
                for (unsigned int i = 0; i < result->size(); i++)
                {
                    std::string uuid;
                    convertUUIDToString(result->at(i).deviceId.id, uuid);

                    if(m_ocResource != NULL && m_ocResource->sid() == uuid)
                    {
                        if( OC_STACK_OK == result->at(i).res )
                        {
                            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "OwnershipTransfer is succeeded");
                            OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "Result is = %d", result->at(i).res);
                            OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "device uuid : %s", uuid.c_str());

                            otmResult = true;
                        }
                        else
                        {
                            OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG, "OwnershipTransfer is failed with code(%d)", hasError);
                            otmResult = false;
                        }
                    }
                }
            }

            delete result;
            m_cond.notify_all();
        }

        ESResult EnrolleeSecurity::discoverTargetSecureResource()
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "discoverTargetSecureResource IN");

            OCStackResult result = OC_STACK_ERROR;

            OicUuid_t uuid;
            if(OC_STACK_OK != ConvertStrToUuid(m_ocResource->sid().c_str(), &uuid))
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "Convert to uuid from deviceID failed.");
                return ES_ERROR;
            }

            // If a discovered resource uses BLE transport, unicast for secure resource discovery is
            // used.
            if( m_ocResource->connectivityType() & CT_ADAPTER_GATT_BTLE )
            {
                std::string GattAddress = getResourceDeviceAddress(m_ocResource->host());
                if(!GattAddress.empty())
                {
                    result = OCSecure::discoverSingleDeviceInUnicast(ES_SEC_DISCOVERY_TIMEOUT,
                                                            &uuid,
                                                            GattAddress,
                                                            m_ocResource->connectivityType(),
                                                            m_securedResource);
                    if(result != OC_STACK_OK)
                    {
                        return ES_SECURE_RESOURCE_DISCOVERY_FAILURE;
                    }
                }
                else
                {
                    OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "GATT BTLE address format is wrong.");
                    return ES_ERROR;
                }
            }
            else
            {
                result = OCSecure::discoverSingleDevice(ES_SEC_DISCOVERY_TIMEOUT,
                                                        &uuid,
                                                        m_securedResource);
                if(result != OC_STACK_OK)
                {
                    return ES_SECURE_RESOURCE_DISCOVERY_FAILURE;
                }
            }

            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "Secured resource is found.");
            OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "HOST: %s", m_securedResource->getDevAddr().c_str());
            OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "SID: %s", m_securedResource->getDeviceID().c_str());
            OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "Owned status: %d", m_securedResource->getOwnedStatus());

            OicSecOxm_t selectedOTMethod = OIC_OXM_COUNT;
            if( OC_STACK_OK != m_securedResource->getOTMethod(&selectedOTMethod) )
            {
                selectedOTMethod = OIC_OXM_COUNT; // Out-of-range
            }
            OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "Selected OT Method: %d", (int)(selectedOTMethod));
#ifdef MULTIPLE_OWNER
            OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "MOT Supported: %d", (int)(m_securedResource->isMOTSupported()));
            OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "MOT Enabled: %d", (int)(m_securedResource->isMOTEnabled()));
#endif
            if(m_securedResource->getOwnedStatus())
            {
                char uuidString[UUID_STRING_SIZE] = {};
                if(RAND_UUID_OK == OCConvertUuidToString(m_securedResource->getDevPtr()->doxm->owner.id, uuidString))
                {
                    OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "Owner ID: %s", uuidString);
                }
                else
                {
                    OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "OCConvertUuidToString is failed");
                }
            }

            return ES_OK;
        }

        ESOwnershipTransferData EnrolleeSecurity::getOwnershipTransferDataFromUser
                                            (SecurityProvStatusCbWithOption callback)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "getOwnershipTransferDataFromUser IN");
            ESOwnershipTransferData ownershipTransferData;

            std::shared_ptr< SecProvisioningStatus > securityProvisioningStatus =
                             std::make_shared< SecProvisioningStatus >
                                           (m_securedResource,
                                           ESResult::ES_SECURE_RESOURCE_IS_DISCOVERED);
            ownershipTransferData = callback(securityProvisioningStatus);
#ifdef MULTIPLE_OWNER
            if(OIC_RANDOM_DEVICE_PIN == ownershipTransferData.getMOTMethod())
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "Selected MOT Method: OIC_RANDOM_DEVICE_PIN");
            }
            else if(OIC_PRECONFIG_PIN == ownershipTransferData.getMOTMethod())
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "Selected MOT Method: OIC_PRECONFIG_PIN");
                OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "Pre-configured PIN: %s",
                                    ownershipTransferData.getPreConfiguredPin().c_str());
            }
#endif
            return ownershipTransferData;
        }

        ESResult EnrolleeSecurity::syncUpWithMediatorDB()
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "syncUpWithMediatorDB IN");

            OCStackResult result = OC_STACK_ERROR;
            ESResult res = ESResult::ES_ERROR;

            OC::ResultCallBack removeDeviceWithUuidCB = std::bind(
                    &EnrolleeSecurity::onEnrolleeSecuritySafetyCB,
                    std::placeholders::_1, std::placeholders::_2,
                    static_cast<ESSecurityCb>(std::bind(&EnrolleeSecurity::removeDeviceWithUuidCB,
                    this, std::placeholders::_1, std::placeholders::_2)),
                    shared_from_this());

            result = OCSecure::removeDeviceWithUuid(ES_SEC_DISCOVERY_TIMEOUT,
                                                    m_ocResource->sid(),
                                                    removeDeviceWithUuidCB);
            if(result != OC_STACK_OK)
            {
                OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG, "removeDeviceWithUuid failed. (%d)", result);
                res = ESResult::ES_OWNERSHIP_IS_NOT_SYNCHRONIZED;
                return res;
            }

            std::unique_lock<std::mutex> lck(m_mtx);
            m_cond.wait_for(lck, std::chrono::seconds(ES_SEC_DISCOVERY_TIMEOUT));

            if(!removeDeviceResult)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "Removing device is failed.");
                res = ESResult::ES_OWNERSHIP_IS_NOT_SYNCHRONIZED;
                return res;
            }
            return ESResult::ES_OK;
        }

        std::string EnrolleeSecurity::getMediatorDevID()
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "getMediatorDevID IN");
            OCUUIdentity* mediatorDevId = (OCUUIdentity* )OICMalloc(sizeof(OCUUIdentity));
            if(!mediatorDevId)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "provisionOwnership: OICMalloc error return");
                return {};
            }

            if(OC::OCPlatform::getDeviceId(mediatorDevId) != OC_STACK_OK)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "getDeviceId is failed.");
                OICFree(mediatorDevId);
                return {};
            }

            char uuidString[UUID_STRING_SIZE] = {};
            if(RAND_UUID_OK == OCConvertUuidToString(mediatorDevId->id, uuidString))
            {
                OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "Mediator UUID : %s", uuidString);
                OICFree(mediatorDevId);
                return std::string(uuidString);
            }

            OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "OCConvertUuidToString is failed.");
            OICFree(mediatorDevId);
            return {};
        }

        ESResult EnrolleeSecurity::provisionOwnership(SecurityProvStatusCbWithOption callback)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "provisionOwnership IN");

            ESResult res = ESResult:: ES_ERROR;

            ESOwnershipTransferData ownershipTransferData;

            std::string mediatorDevIdStr = getMediatorDevID();
            if(mediatorDevIdStr.empty())
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "getMediatorDevID is failed.");
                return res;
            }
            else
            {
                m_mediatorID = mediatorDevIdStr;
            }

            res = discoverTargetSecureResource();

            if (res != ES_OK)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "Secure Resource Discovery failed.");
                res = ESResult:: ES_SECURE_RESOURCE_DISCOVERY_FAILURE;
                return res;
            }
            else if (m_securedResource)
            {
                if(callback != NULL)
                {
                    ownershipTransferData = getOwnershipTransferDataFromUser(callback);
                }

                if(m_securedResource->getOwnedStatus())
                {
                    if(isOwnedDeviceRegisteredInDB())
                    {
#ifdef MULTIPLE_OWNER
                        if(isSubOwnerIDMatched(m_securedResource))
                        {
                            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG,
                                "The found device is already owned by Mediator.(SUCCESS)");
                            res = ESResult::ES_OK;
                            return res;
                        }

                        if(isOwnerIDMatched(m_securedResource))
                        {
                            if((OIC_PRECONFIG_PIN == ownershipTransferData.getMOTMethod() ||
                                OIC_RANDOM_DEVICE_PIN == ownershipTransferData.getMOTMethod()))
                            {
                                if(m_securedResource->isMOTSupported())
                                {
                                    res = provisionMOTConfig(ownershipTransferData);
                                    if(res != ES_OK)
                                    {
                                        OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG,
                                            "provisionMOTConfig is failed.");
                                        return res;
                                    }
                                    OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG,
                                        "The found device is already owned by Mediator and enabled MOT mode.");
                                    return res;
                                }
                                else
                                {
                                    OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG,
                                        "The found device is not supported MOT");
                                    return ESResult:: ES_MOT_NOT_SUPPORTED;
                                }
                            }
                            else
                            {
                                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG,
                                    "The found device is already owned by Mediator.(SUCCESS)");
                                res = ESResult::ES_OK;
                                return res;
                            }
                        }

                        OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,
                            "An ownership transfer knowledge is not synchronized"
                            "between mediator and found enrollee.(FAILED)");
                        res = ESResult::ES_OWNERSHIP_IS_NOT_SYNCHRONIZED;
                        return res;
#else
                        if(isOwnerIDMatched(m_securedResource))
                        {
                            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG,
                                "The found device is already owned by Mediator.(SUCCESS)");
                            res = ESResult::ES_OK;
                            return res;
                        }
                        else
                        {
                            OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,
                                "An ownership transfer knowledge is not synchronized"
                                "between mediator and found enrollee.(FAILED)");
                            res = ESResult::ES_OWNERSHIP_IS_NOT_SYNCHRONIZED;
                            return res;
                        }
#endif
                    }
#ifdef MULTIPLE_OWNER
                    else if( !isOwnedDeviceRegisteredInDB() &&
                             !isOwnerIDMatched(m_securedResource) &&
                             !isSubOwnerIDMatched(m_securedResource) &&
                             m_securedResource->isMOTEnabled() &&
                             (OIC_PRECONFIG_PIN == ownershipTransferData.getMOTMethod() ||
                              OIC_RANDOM_DEVICE_PIN == ownershipTransferData.getMOTMethod()))
                    {
                        // MOT case;
                        res = performMultipleOwnershipTransfer(ownershipTransferData);

                        if(res != ESResult::ES_OK)
                        {
                            OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG,
                                            "Multiple Ownership-Transfer failed. (%d)", res);
                            return res;
                        }
                    }
                    else if( !isOwnedDeviceRegisteredInDB() &&
                             (isOwnerIDMatched(m_securedResource) ||
                             isSubOwnerIDMatched(m_securedResource)))
#else
                    else if( !isOwnedDeviceRegisteredInDB() && isOwnerIDMatched(m_securedResource))
#endif
                    {
                        OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,
                        "An ownership transfer knowledge is not synchronized between mediator and found enrollee.(FAILED)");
                        res = ESResult::ES_OWNERSHIP_IS_NOT_SYNCHRONIZED;
                        return res;
                    }
                    else
                    {
                        OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,
                            "The found device is already owned by Other Mediator.(FAILED)");
                        res = ESResult::ES_OWNERSHIP_TRANSFER_FAILURE;
                        return res;
                    }
                }
                else
                {
                    if(isOwnedDeviceRegisteredInDB())
                    {
                        res = syncUpWithMediatorDB();

                        if(res != ESResult::ES_OK)
                        {
                            OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG,
                                            "syncUpWithMediatorDB failed. (%d)", res);
                            return res;
                        }
                        OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "Removing device is succeeded.");
                    }
                    res = performOwnershipTransfer();

                    if(res != ESResult::ES_OK)
                    {
                        OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG, "Ownership-Transfer failed. (%d)", res);
                        return res;
                    }
#ifdef MULTIPLE_OWNER
                    if( m_securedResource->isMOTSupported() &&
                        ownershipTransferData.getMOTMethod() != OIC_OXM_COUNT)
                    {
                        res = provisionMOTConfig(ownershipTransferData);
                    }
                    return res;
#endif
                }
            }
            else
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "No secure resource is found.");
                res = ESResult:: ES_SECURE_RESOURCE_DISCOVERY_FAILURE;
            }
            return res;
        }

        ESResult EnrolleeSecurity::performOwnershipTransfer()
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "performOwnershipTransfer IN.");

            OCStackResult result = OC_STACK_ERROR;
            ESResult res = ESResult::ES_ERROR;

            OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "Transfering ownership for : %s ",
                    m_securedResource->getDeviceID().c_str());

            OC::ResultCallBack ownershipTransferCb =
                std::bind(&EnrolleeSecurity::onEnrolleeSecuritySafetyCB,
                          std::placeholders::_1, std::placeholders::_2,
                          static_cast<ESSecurityCb>(std::bind(&EnrolleeSecurity::ownershipTransferCb,
                          this, std::placeholders::_1, std::placeholders::_2, std::ref(res))),
                          shared_from_this());


            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "doOwnershipTransfer is excuted");
            result = m_securedResource->doOwnershipTransfer(ownershipTransferCb);

            if (result != OC_STACK_OK)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "doOwnershipTransfer is failed");
                return ESResult::ES_ERROR;
            }

            std::unique_lock<std::mutex> lck(m_mtx);
            m_cond.wait(lck);

            if(!otmResult)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "Ownership-Transfer failed.");
                return res;
            }

            return ESResult::ES_OK;
        }

#ifdef MULTIPLE_OWNER
        ESResult EnrolleeSecurity::performMultipleOwnershipTransfer(const ESOwnershipTransferData& MOTdata)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "performMultipleOwnershipTransfer IN.");

            OCStackResult result = OC_STACK_ERROR;

            OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "Transfering sub-ownership for : %s ",
                    m_securedResource->getDeviceID().c_str());

            if(OIC_PRECONFIG_PIN == MOTdata.getMOTMethod() &&
               !MOTdata.getPreConfiguredPin().empty())
            {
                std::string pin = MOTdata.getPreConfiguredPin();

                result = m_securedResource->addPreconfigPIN(pin.c_str(), pin.length());
                if(OC_STACK_OK != result)
                {
                    OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "addPreconfigPIN is failed");
                    return ESResult::ES_ERROR;
                }

                OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "Preconfig PIN : %s", pin.c_str());
            }

            OC::ResultCallBack multipleOwnershipTransferCb =
                std::bind(&EnrolleeSecurity::onEnrolleeSecuritySafetyCB,
                          std::placeholders::_1, std::placeholders::_2,
                          static_cast<ESSecurityCb>(std::bind(&EnrolleeSecurity::multipleOwnershipTransferCb,
                          this, std::placeholders::_1, std::placeholders::_2)),
                          shared_from_this());

            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "doMultipleOwnershipTransfer is excuted");

            result = m_securedResource->doMultipleOwnershipTransfer(multipleOwnershipTransferCb);
            if(OC_STACK_OK != result)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "doMultipleOwnershipTransfer is failed");
                return ESResult::ES_ERROR;
            }

            std::unique_lock<std::mutex> lck(m_mtx);
            m_cond.wait(lck);

            if(!otmResult)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "Multiple Ownership-Transfer failed.");
                return ESResult::ES_OWNERSHIP_TRANSFER_FAILURE;
            }

            return ESResult::ES_OK;
        }
#endif

        void EnrolleeSecurity::removeDeviceWithUuidCB(OC::PMResultList_t *result, int hasError)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "removeDeviceWithUuidCB IN");

            if (hasError)
            {
               OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG, "removeDeviceWithUuid is failed with code (%d)", hasError);
               removeDeviceResult = false;
            }

            else
            {
               OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "removeDeviceWithUuid is succeeded");

               for (unsigned int i = 0; i < result->size(); i++)
               {
                    std::string uuid;
                    convertUUIDToString(result->at(i).deviceId.id, uuid);

                    OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "Result is = %d", result->at(i).res);
                    OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "device uuid : %s", uuid.c_str());
               }
               removeDeviceResult = true;
            }
            m_cond.notify_all();
        }

        bool EnrolleeSecurity::isOwnedDeviceRegisteredInDB()
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "isOwnedDeviceRegisteredInDB IN");

            OCStackResult res = OC_STACK_ERROR;

            OCUuidList_t *uuidList = NULL;
            size_t numOfDevices = 0;

            res = PDMGetOwnedDevices(&uuidList, &numOfDevices);
            if (OC_STACK_OK != res)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "Error while getting info from DB");
                OICFree(uuidList);
                return false;
            }

            OCUuidList_t *pUuidList = uuidList;
            while (pUuidList)
            {
                std::string uuid;
                convertUUIDToString(pUuidList->dev.id, uuid);
                OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG,
                    "m_ocResource->sid(): %s, cur DB UUID %s",
                    m_ocResource->sid().c_str(), uuid.c_str());
                if(m_ocResource->sid() == uuid.c_str())
                {
                    OICFree(uuidList);
                    return true;
                }
                pUuidList = pUuidList->next;
            }
            OICFree(uuidList);
            return false;
        }


        std::string EnrolleeSecurity::getUUID() const
        {
            return m_ocResource->sid();
        };

#if defined(__WITH_DTLS__) && defined(__WITH_TLS__)
        ESResult EnrolleeSecurity::provisionSecurityForCloudServer(
            std::string cloudUuid, int credId)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "provisionSecurityForCloudServer IN");

            ESResult res = ESResult::ES_ERROR;

            // Need to discover Owned device in a given network, again
            std::shared_ptr< OC::OCSecureResource > ownedDevice = NULL;

            OCStackResult result = OC_STACK_ERROR;
            OicUuid_t uuid;
            if(OC_STACK_OK != ConvertStrToUuid(m_ocResource->sid().c_str(), &uuid))
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "Convert to uuid from deviceID failed.");
                return res;
            }

            // If a discovered resource uses BLE transport, unicast for secure resource discovery is
            // used.
            if( m_ocResource->connectivityType() & CT_ADAPTER_GATT_BTLE )
            {
                std::string GattAddress = getResourceDeviceAddress(m_ocResource->host());
                if(!GattAddress.empty())
                {
                    result = OCSecure::discoverSingleDeviceInUnicast(ES_SEC_DISCOVERY_TIMEOUT,
                                                            &uuid,
                                                            GattAddress,
                                                            m_ocResource->connectivityType(),
                                                            ownedDevice);
                }
                else
                {
                    OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "GATT BTLE address format is wrong.");
                    res = ESResult:: ES_ERROR;
                    return res;
                }
            }
            else
            {
                result = OCSecure::discoverSingleDevice(ES_SEC_DISCOVERY_TIMEOUT,
                                                        &uuid,
                                                        ownedDevice);
            }

            if (result != OC_STACK_OK)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "secureResource Discovery failed.");
                res = ESResult::ES_SECURE_RESOURCE_DISCOVERY_FAILURE;
                return res;
            }
            else if (ownedDevice)
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "Secured resource is found.");
                OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "HOST: %s", ownedDevice->getDevAddr().c_str());
                OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "SID: %s", ownedDevice->getDeviceID().c_str());
                OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "Owned status: %d", ownedDevice->getOwnedStatus());

                if (ownedDevice->getOwnedStatus())
                {
                    if(!isOwnedDeviceRegisteredInDB())
                    {
                        OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "The found device is not one in SVR DB");
                        res = ESResult::ES_SECURE_RESOURCE_DISCOVERY_FAILURE;
                        return res;
                    }
                }
                else
                {
                    OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "The found device is unowned.");
                    OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "Ownerthip transfer is required.");

                    res = ESResult::ES_SECURE_RESOURCE_DISCOVERY_FAILURE;
                    return res;
                }
            }
            else
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "No secure resource is found");
                res = ESResult::ES_SECURE_RESOURCE_DISCOVERY_FAILURE;
                return res;
            }

            if(cloudUuid.empty())
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,
                         "ACL provisioning is skipped due to empty UUID of cloud server");
            }
            else
            {
                res = performACLProvisioningForCloudServer(ownedDevice, cloudUuid);
                if(res != ESResult::ES_OK)
                {
                    OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "error performACLProvisioningForCloudServer");
                    return res;
                }
            }

            if(credId < 1)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,
                         "Cert. provisioning is skipped due to wrong cred ID (<1)");
            }
            else
            {
                res = performCertProvisioningForCloudServer(ownedDevice, credId);
                if(res != ESResult::ES_OK)
                {
                    OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "error performCertProvisioningForCloudServer");
                    return res;
                }
            }

            return res;
        }

        ESResult EnrolleeSecurity::performCertProvisioningForCloudServer(
            std::shared_ptr< OC::OCSecureResource > ownedDevice, int credId)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "performCertProvisioningForCloudServer IN");

            ESResult res = ESResult::ES_CERT_PROVISIONING_FAILURE;

            if(!ownedDevice)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "Given ownedDevice is null");
                return res;
            }

            OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "Given CredId: %d", credId);

            OC::ResultCallBack CertProvisioningCb =
                std::bind(&EnrolleeSecurity::onEnrolleeSecuritySafetyCB,
                          std::placeholders::_1, std::placeholders::_2,
                          static_cast<ESSecurityCb>(std::bind(&EnrolleeSecurity::certProvisioningCb,
                          this, std::placeholders::_1, std::placeholders::_2)),
                          shared_from_this());

            OCStackResult rst = ownedDevice->provisionTrustCertChain(SIGNED_ASYMMETRIC_KEY,
                                                                    static_cast<uint16_t>(credId),
                                                                    CertProvisioningCb);
            if(OC_STACK_OK != rst)
            {
                OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG, "provisionTrustCertChain error: %d", rst);
                return res;
            }

            std::unique_lock<std::mutex> lck(m_mtx);
            m_cond.wait(lck);

            if(certResult)
            {
                res = ESResult::ES_OK;
            }

            return res;
        }

        ESResult EnrolleeSecurity::performACLProvisioningForCloudServer(
            std::shared_ptr< OC::OCSecureResource > ownedDevice, std::string& cloudUuid)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "performACLProvisioningForCloudServer IN");

            ESResult res = ESResult::ES_ACL_PROVISIONING_FAILURE;

            if(!ownedDevice)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "Given ownedDevice is null");
                return res;
            }

            OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "Given cloudUuid: %s", cloudUuid.c_str());

            OicUuid_t uuid;
            if(OC_STACK_OK != ConvertStrToUuid(cloudUuid.c_str(), &uuid))
            {
                OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "Convert to uuid from deviceID failed.");
                return res;
            }


            // Create Acl for Cloud Server to be provisioned to Enrollee
            OicSecAcl_t* acl = createAcl(uuid);
            if(!acl)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "createAcl error return");
                return res;
            }

            OC::ResultCallBack aclProvisioningCb =
                std::bind(&EnrolleeSecurity::onEnrolleeSecuritySafetyCB,
                          std::placeholders::_1, std::placeholders::_2,
                          static_cast<ESSecurityCb>(std::bind(&EnrolleeSecurity::aclProvisioningCb,
                          this, std::placeholders::_1, std::placeholders::_2)),
                          shared_from_this());

            // ACL provisioning to Enrollee
            OCStackResult rst = ownedDevice->provisionACL(acl, aclProvisioningCb);
            if(OC_STACK_OK != rst)
            {
                OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG, "OCProvisionACL API error: %d", rst);
                return res;
            }

            std::unique_lock<std::mutex> lck(m_mtx);
            m_cond.wait(lck);

            if(aclResult)
            {
                res = ESResult::ES_OK;
            }
            OCDeleteACLList(acl);
            return res;
        }

        OicSecAcl_t* EnrolleeSecurity::createAcl(const OicUuid_t cloudUuid)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "creatAcl IN");

            // allocate memory for |acl| struct
            OicSecAcl_t* acl = (OicSecAcl_t*) OICCalloc(1, sizeof(OicSecAcl_t));
            if(!acl)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "createAcl: OICCalloc error return");
                return NULL;  // not need to 'goto' |ERROR| before allocating |acl|
            }
            OicSecAce_t* ace = (OicSecAce_t*) OICCalloc(1, sizeof(OicSecAce_t));
            if(!ace)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,  "createAcl: OICCalloc error return");
                return NULL;  // not need to 'goto' |ERROR| before allocating |acl|
            }
            LL_APPEND(acl->aces, ace);

            memcpy(&ace->subjectuuid, &cloudUuid, UUID_LENGTH);

            OicSecRsrc_t* rsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
            if(!rsrc)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG, "createAcl: OICCalloc error return");
                OCDeleteACLList(acl);
                return NULL;
            }

            char href[] = "*";
            size_t len = strlen(href)+1;  // '1' for null termination
            rsrc->href = (char*) OICCalloc(len, sizeof(char));
            if(!rsrc->href)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,  "createAcl: OICCalloc error return");
                FreeRsrc(rsrc);
                OCDeleteACLList(acl);
                return NULL;
            }
            OICStrcpy(rsrc->href, len, href);

            size_t arrLen = 1;
            rsrc->typeLen = arrLen;
            rsrc->types = (char**)OICCalloc(arrLen, sizeof(char*));
            if(!rsrc->types)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,  "createAcl: OICCalloc error return");
                FreeRsrc(rsrc);
                OCDeleteACLList(acl);
                return NULL;
            }
            rsrc->types[0] = OICStrdup("rt");   // ignore
            if(!rsrc->types[0])
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,  "createAcl: OICStrdup error return");
                FreeRsrc(rsrc);
                OCDeleteACLList(acl);
                return NULL;
            }

            rsrc->interfaceLen = 1;
            rsrc->interfaces = (char**)OICCalloc(arrLen, sizeof(char*));
            if(!rsrc->interfaces)
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,  "createAcl: OICCalloc error return");
                FreeRsrc(rsrc);
                OCDeleteACLList(acl);
                return NULL;
            }
            rsrc->interfaces[0] = OICStrdup("if");  // ignore
            if(!rsrc->interfaces[0])
            {
                OIC_LOG(ERROR, ENROLEE_SECURITY_TAG,  "createAcl: OICStrdup error return");
                FreeRsrc(rsrc);
                OCDeleteACLList(acl);
                return NULL;
            }

            LL_APPEND(ace->resources, rsrc);

            ace->permission = 31;   // R/W/U/D

            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "creatAcl OUT");

            return acl;
        }

        void EnrolleeSecurity::aclProvisioningCb(PMResultList_t *result, int hasError)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "aclProvisioningCb IN");

            if (hasError)
            {
               OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG, "ACL provisioning is failed with code (%d)", hasError);
               aclResult = false;
            }
            else
            {
               OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "Received ACL provisioning results: ");

               std::string devUuid;
               for (unsigned int i = 0; i < result->size(); i++)
               {
                   convertUUIDToString(result->at(i).deviceId.id, devUuid);

                   OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "Result is = %d", result->at(i).res);
                   OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "device uuid : %s", devUuid.c_str());
               }
               delete result;
               aclResult = true;
            }
            m_cond.notify_all();
        }

        void EnrolleeSecurity::certProvisioningCb(PMResultList_t *result, int hasError)
        {
            OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "certProvisioningCb IN");

            if (hasError)
            {
               OIC_LOG_V(ERROR, ENROLEE_SECURITY_TAG, "Cert provisioning is failed with code (%d)", hasError);
               certResult = false;
            }
            else
            {
               OIC_LOG(DEBUG, ENROLEE_SECURITY_TAG, "Received Cert. provisioning results: ");

               std::string devUuid;
               for (unsigned int i = 0; i < result->size(); i++)
               {
                   convertUUIDToString(result->at(i).deviceId.id, devUuid);

                   OIC_LOG_V(DEBUG, ENROLEE_SECURITY_TAG, "Result is = %d", result->at(i).res);
                   OIC_LOG_V(INFO_PRIVATE, ENROLEE_SECURITY_TAG, "device uuid : %s", devUuid.c_str());
               }
               delete result;
               certResult= true;
            }
            m_cond.notify_all();
        }
#endif //defined(__WITH_DTLS__) && defined(__WITH_TLS__)
    }
}
