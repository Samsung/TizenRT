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

#include "ResourcePresence.h"
#if (defined(__APPLE__) && defined(TARGET_OS_IPHONE))
#include <atomic>
#else
#include <bits/atomic_base.h>
#include <bits/shared_ptr_base.h>
#endif
#include <time.h>
#include <unistd.h>
#include <cstdbool>
#include <exception>
#include <iostream>
#include <memory>

#include "PrimitiveResource.h"
#include "DeviceAssociation.h"
#include "DevicePresence.h"

namespace
{
using namespace OIC::Service;

    void getCallback(const HeaderOptions &hos, const ResponseStatement& rep,
            int eCode, std::weak_ptr<ResourcePresence> this_ptr)
    {
        OIC_LOG_V(DEBUG,BROKER_TAG,"getCallback().\n");
        std::shared_ptr<ResourcePresence> Ptr = this_ptr.lock();
        if(Ptr)
        {
            Ptr->getCB(hos, rep, eCode);
        }
    }
    void timeOutCallback(unsigned int msg, std::weak_ptr<ResourcePresence> this_ptr)
    {
        OIC_LOG_V(DEBUG,BROKER_TAG,"timeOutCallback().\n");
        std::shared_ptr<ResourcePresence> Ptr = this_ptr.lock();
        if(Ptr)
        {
            Ptr->timeOutCB(msg);
        }
    }
}

namespace OIC
{
    namespace Service
    {
        ResourcePresence::ResourcePresence()
        : requesterList(nullptr), primitiveResource(nullptr),
          state(BROKER_STATE::REQUESTED), mode(BROKER_MODE::NON_PRESENCE_MODE),
          isWithinTime(true), receivedTime(0L), timeoutHandle(0)
        {
        }

        void ResourcePresence::initializeResourcePresence(PrimitiveResourcePtr pResource)
        {
            OIC_LOG_V(DEBUG,BROKER_TAG,"initializeResourcePresence().\n");
            pGetCB = std::bind(getCallback, std::placeholders::_1, std::placeholders::_2,
                    std::placeholders::_3, std::weak_ptr<ResourcePresence>(shared_from_this()));
            pTimeoutCB = std::bind(timeOutCallback, std::placeholders::_1,
                    std::weak_ptr<ResourcePresence>(shared_from_this()));
            pPollingCB = std::bind(&ResourcePresence::pollingCB, this, std::placeholders::_1);

            primitiveResource = pResource;
            requesterList
            = std::unique_ptr<std::list<BrokerRequesterInfoPtr>>
            (new std::list<BrokerRequesterInfoPtr>);

            timeoutHandle = expiryTimer.post(BROKER_SAFE_MILLISECOND, pTimeoutCB);
            OIC_LOG_V(DEBUG,BROKER_TAG,"initializeResourcePresence::requestGet.\n");
            primitiveResource->requestGet(pGetCB);

            registerDevicePresence();
        }


        ResourcePresence::~ResourcePresence()
        {
            std::string deviceAddress = primitiveResource->getHost();

            DevicePresencePtr foundDevice
            = DeviceAssociation::getInstance()->findDevice(deviceAddress);

            if(foundDevice != nullptr)
            {
                foundDevice->removePresenceResource(this);

                if(foundDevice->isEmptyResourcePresence())
                {
                    DeviceAssociation::getInstance()->removeDevice(foundDevice);
                }
            }

            requesterList->clear();

            state = BROKER_STATE::DESTROYED;
        }

        void ResourcePresence::addBrokerRequester(BrokerID _id, BrokerCB _cb)
        {
            OIC_LOG_V(DEBUG,BROKER_TAG,"addBrokerRequester().\n");
            requesterList->push_back(
                    std::make_shared<BrokerRequesterInfo>(BrokerRequesterInfo(_id, _cb)));
        }

        void ResourcePresence::removeAllBrokerRequester()
        {
            OIC_LOG_V(DEBUG,BROKER_TAG,"removeAllBrokerRequester().\n");
            if(requesterList != nullptr)
            {
                requesterList->erase(requesterList->begin(), requesterList->end());
            }
        }

        void ResourcePresence::removeBrokerRequester(BrokerID _id)
        {
            OIC_LOG_V(DEBUG,BROKER_TAG,"removeBrokerRequester().\n");
            std::list<BrokerRequesterInfoPtr>::iterator iter = requesterList->begin();
            for(; iter != requesterList->end(); ++iter)
            {
                if(iter->get()->brokerId == _id)
                {
                    OIC_LOG_V(DEBUG,BROKER_TAG,"find broker-id in requesterList.\n");
                    requesterList->erase(iter);
                    break;
                }
            }
        }

        bool ResourcePresence::isEmptyRequester() const
        {
            OIC_LOG_V(DEBUG,BROKER_TAG,"isEmptyRequester().\n");
            return (requesterList!=nullptr)?requesterList->empty():true;
        }

        int ResourcePresence::requesterListSize() const {
            OIC_LOG_V(DEBUG,BROKER_TAG,"requesterListSize().\n");
            return (requesterList!=nullptr)?requesterList->size():0;
        }

        void ResourcePresence::requestResourceState() const
        {
            OIC_LOG_V(DEBUG,BROKER_TAG,"requestResourceState().\n");
            primitiveResource->requestGet(pGetCB);
            OIC_LOG_V(DEBUG, BROKER_TAG, "Request Get\n");
        }

        void ResourcePresence::registerDevicePresence()
        {
            OIC_LOG_V(DEBUG,BROKER_TAG,"registerDevicePresence().\n");
            std::string deviceAddress = primitiveResource->getHost();

            DevicePresencePtr foundDevice
            = DeviceAssociation::getInstance()->findDevice(deviceAddress);

            if(foundDevice == nullptr)
            {
                try
                {
                    foundDevice.reset(new DevicePresence());
                    foundDevice->initializeDevicePresence(primitiveResource);
                }catch(...)
                {
                    throw;
                }
                DeviceAssociation::getInstance()->addDevice(foundDevice);
            }
            foundDevice->addPresenceResource(this);
        }

        void ResourcePresence::executeAllBrokerCB(BROKER_STATE changedState)
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "executeAllBrokerCB().\n");
            if(state != changedState)
            {
                setResourcestate(changedState);
                if(requesterList->empty() != true)
                {
                    std::list<BrokerRequesterInfoPtr> list = * requesterList;
                    for(BrokerRequesterInfoPtr item : list)
                    {
                        item->brokerCB(state);
                    }
                }
            }
        }

        void ResourcePresence::setResourcestate(BROKER_STATE _state)
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "setResourcestate().\n");
            this->state = _state;
        }

        void ResourcePresence::timeOutCB(unsigned int /*msg*/)
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "timeOutCB()");
            OIC_LOG_V(DEBUG, BROKER_TAG, "waiting for terminate getCB\n");
            std::unique_lock<std::mutex> lock(cbMutex);

            time_t currentTime;
            time(&currentTime);
            currentTime += 0L;

            if((receivedTime == 0) || ((receivedTime + BROKER_SAFE_SECOND) > currentTime ))
            {
                this->isWithinTime = true;
                return;
            }
            this->isWithinTime = false;
            OIC_LOG_V(DEBUG, BROKER_TAG,
                    "Timeout execution. will be discard after receiving cb message.\n");

            executeAllBrokerCB(BROKER_STATE::LOST_SIGNAL);
            pollingCB();
        }

        void ResourcePresence::pollingCB(unsigned int /*msg*/)
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "pollingCB().\n");
            if(this->requesterList->size() != 0)
            {
                this->requestResourceState();
                timeoutHandle = expiryTimer.post(BROKER_SAFE_MILLISECOND,pTimeoutCB);
            }
        }

        void ResourcePresence::getCB(const HeaderOptions & /*hos*/,
                const ResponseStatement & /*rep*/, int eCode)
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "getCB().\n");
            OIC_LOG_V(DEBUG, BROKER_TAG, "waiting for terminate TimeoutCB.\n");
            std::unique_lock<std::mutex> lock(cbMutex);

            time_t currentTime;
            time(&currentTime);
            receivedTime = currentTime;

            verifiedGetResponse(eCode);

            if(isWithinTime)
            {
                expiryTimer.cancel(timeoutHandle);
                isWithinTime = true;
            }

            if(mode == BROKER_MODE::NON_PRESENCE_MODE)
            {
                expiryTimer.post(BROKER_SAFE_MILLISECOND,pPollingCB);
            }

        }

        void ResourcePresence::verifiedGetResponse(int eCode)
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "verifiedGetResponse().\n");
            BROKER_STATE verifiedState = BROKER_STATE::NONE;
            switch(eCode)
            {
            case OC_STACK_OK:
            case OC_STACK_CONTINUE:
                verifiedState = BROKER_STATE::ALIVE;
                break;

            case OC_STACK_RESOURCE_DELETED:
                verifiedState = BROKER_STATE::DESTROYED;
                break;

            case OC_STACK_INVALID_REQUEST_HANDLE:
            case OC_STACK_TIMEOUT:
            case OC_STACK_COMM_ERROR:
            case OC_STACK_PRESENCE_STOPPED:
            case OC_STACK_PRESENCE_TIMEOUT:
            default:
                verifiedState = BROKER_STATE::LOST_SIGNAL;
                break;
            }

            executeAllBrokerCB(verifiedState);
            OIC_LOG_V(DEBUG, BROKER_TAG, "resource state : %d",(int)state);
        }

        const PrimitiveResourcePtr ResourcePresence::getPrimitiveResource() const
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "getPrimitiveResource()\n");
            return primitiveResource;
        }

        BROKER_STATE ResourcePresence::getResourceState() const
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "getResourceState()\n");
            return state;
        }

        void ResourcePresence::changePresenceMode(BROKER_MODE newMode)
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "changePresenceMode()\n");
            if(newMode != mode)
            {
                expiryTimer.cancel(timeoutHandle);
                if(newMode == BROKER_MODE::NON_PRESENCE_MODE)
                {
                    timeoutHandle = expiryTimer.post(BROKER_SAFE_MILLISECOND,pTimeoutCB);
                    requestResourceState();
                }
                mode = newMode;
            }
        }
    } // namespace Service
} // namespace OIC
