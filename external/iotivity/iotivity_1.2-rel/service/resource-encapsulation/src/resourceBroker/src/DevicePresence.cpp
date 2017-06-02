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

#include "DevicePresence.h"
#include "RCSException.h"

namespace OIC
{
    namespace Service
    {
        DevicePresence::DevicePresence()
        {
            setDeviceState(DEVICE_STATE::REQUESTED);

            presenceTimerHandle = 0;
            isRunningTimeOut = false;

            pSubscribeRequestCB = std::bind(&DevicePresence::subscribeCB, this,
                        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            pTimeoutCB = std::bind(&DevicePresence::timeOutCB, this, std::placeholders::_1);
        }

        DevicePresence::~DevicePresence()
        {
            if(presenceSubscriber.isSubscribing())
            {
                OIC_LOG_V(DEBUG,BROKER_TAG,"unsubscribed presence.");
                try
                {
                    presenceSubscriber.unsubscribe();
                } catch (std::exception & e)
                {
                    OIC_LOG_V(DEBUG,BROKER_TAG,"unsubscribed presence : %s", e.what());
                }
            }
            resourcePresenceList.clear();
            OIC_LOG_V(DEBUG,BROKER_TAG,"destroy Timer.");
        }

        void DevicePresence::initializeDevicePresence(PrimitiveResourcePtr pResource)
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "initializeDevicePresence()");
            address = pResource->getHost();

            OIC_LOG_V(DEBUG, BROKER_TAG, "%s",address.c_str());

            try
            {
                OIC_LOG_V(DEBUG, BROKER_TAG, "subscribe Presence");
                presenceSubscriber
                = PresenceSubscriber(address, BROKER_TRANSPORT, pSubscribeRequestCB);
            } catch(RCSPlatformException &e)
            {
                OIC_LOG_V(DEBUG, BROKER_TAG,
                        "exception in subscribe Presence %s", e.getReason().c_str());
                throw;
            }
            presenceTimerHandle
            = presenceTimer.post(BROKER_DEVICE_PRESENCE_TIMEROUT, pTimeoutCB);
        }

        DEVICE_STATE DevicePresence::getDeviceState() const noexcept
        {
            return static_cast< DEVICE_STATE >(state.load());
        }

        void DevicePresence::setDeviceState(DEVICE_STATE newState)
        {
            state = static_cast< int >(newState);
        }

        const std::string DevicePresence::getAddress() const
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "getAddress()");
            return address;
        }

        void DevicePresence::addPresenceResource(ResourcePresence * rPresence)
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "addPresenceResource()");
            resourcePresenceList.push_back(rPresence);
        }

        void DevicePresence::removePresenceResource(ResourcePresence * rPresence)
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "removePresenceResource()");
            resourcePresenceList.remove(rPresence);
        }

        void DevicePresence::changeAllPresenceMode(BROKER_MODE mode)
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "changeAllPresenceMode()");
            if(!resourcePresenceList.empty())
            {
                for(auto it : resourcePresenceList)
                {
                    it->changePresenceMode(mode);
                }
            }
        }

        bool DevicePresence::isEmptyResourcePresence() const
        {
            OIC_LOG_V(DEBUG, BROKER_TAG, "isEmptyResourcePresence()");
            return resourcePresenceList.empty();
        }

        void DevicePresence::subscribeCB(OCStackResult ret,
                const unsigned int seq, const std::string & hostAddress)
        {
            OC_UNUSED(seq);
            OC_UNUSED(hostAddress);

            OIC_LOG_V(DEBUG, BROKER_TAG, "subscribeCB()");
            OIC_LOG_V(DEBUG, BROKER_TAG, "Received presence CB from: %s",hostAddress.c_str());
            OIC_LOG_V(DEBUG, BROKER_TAG, "In subscribeCB: %d",ret);

            if(isRunningTimeOut)
            {
                std::unique_lock<std::mutex> lock(timeoutMutex);
                condition.wait(lock);
            }
            presenceTimer.cancel(presenceTimerHandle);

            switch(ret)
            {
                case OC_STACK_OK:
                case OC_STACK_RESOURCE_CREATED:
                case OC_STACK_CONTINUE:
                {
                    OIC_LOG_V(DEBUG, BROKER_TAG, "SEQ# %d",seq);
                    setDeviceState(DEVICE_STATE::ALIVE);
                    OIC_LOG_V(DEBUG, BROKER_TAG, "device state : %d",
                            (int)getDeviceState());
                    changeAllPresenceMode(BROKER_MODE::DEVICE_PRESENCE_MODE);
                    presenceTimerHandle
                    = presenceTimer.post(BROKER_DEVICE_PRESENCE_TIMEROUT, pTimeoutCB);
                    break;
                }
                case OC_STACK_INVALID_REQUEST_HANDLE:
                case OC_STACK_RESOURCE_DELETED:
                case OC_STACK_TIMEOUT:
                case OC_STACK_COMM_ERROR:
                case OC_STACK_PRESENCE_STOPPED:
                case OC_STACK_PRESENCE_TIMEOUT:
                case OC_STACK_PRESENCE_DO_NOT_HANDLE:
                {
                    setDeviceState(DEVICE_STATE::LOST_SIGNAL);
                    changeAllPresenceMode(BROKER_MODE::NON_PRESENCE_MODE);
                    break;
                }
                default:
                {
                    OIC_LOG_V(DEBUG, BROKER_TAG, "Presence Lost Signal because unknown type");
                    setDeviceState(DEVICE_STATE::LOST_SIGNAL);
                    changeAllPresenceMode(BROKER_MODE::NON_PRESENCE_MODE);
                    break;
                }
            }
        }

        void DevicePresence::timeOutCB(TimerID /*id*/)
        {
            OIC_LOG_V(DEBUG,BROKER_TAG,"timeOutCB()");
            std::unique_lock<std::mutex> lock(timeoutMutex);
            isRunningTimeOut = true;

            OIC_LOG_V(DEBUG, BROKER_TAG,
                    "Timeout execution. will be discard after receiving cb message");
            setDeviceState(DEVICE_STATE::LOST_SIGNAL);
            changeAllPresenceMode(BROKER_MODE::NON_PRESENCE_MODE);

            isRunningTimeOut = false;
            condition.notify_all();
        }
    } // namespace Service
} // namespace OIC
