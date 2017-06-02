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

#include "BundleResource.h"

#include <list>
#include <string.h>
#include <iostream>
#include <boost/thread.hpp>
#include "NotificationReceiver.h"

#include "InternalTypes.h"

namespace OIC
{
    namespace Service
    {
        BundleResource::BundleResource() : m_pNotiReceiver(nullptr), m_resourceAttributes_mutex()
        {

        }

        BundleResource::~BundleResource()
        {
            m_pNotiReceiver = nullptr;
        }

        void BundleResource::registerObserver(NotificationReceiver *pNotiReceiver)
        {
            m_pNotiReceiver = pNotiReceiver;
        }

        std::list< std::string > BundleResource::getAttributeNames()
        {
            std::list< std::string > ret;

            for (auto &it : m_resourceAttributes)
            {
                ret.push_back(it.key());
            }

            return ret;
        }

        const RCSResourceAttributes BundleResource::getAttributes()
        {
            std::lock_guard<std::mutex> lock(m_resourceAttributes_mutex);
            return RCSResourceAttributes(m_resourceAttributes);
        }

        void BundleResource::setAttributes(const RCSResourceAttributes &attrs)
        {
            setAttributes(attrs, true);
        }

        void BundleResource::setAttributes(const RCSResourceAttributes &attrs, bool notify)
        {
            std::lock_guard<std::mutex> lock(m_resourceAttributes_mutex);

            for (auto &it : attrs)
            {
                OIC_LOG_V(INFO, CONTAINER_TAG, "set attribute \(%s)'",
                           std::string(it.key() + "\', with " + it.value().toString()).c_str());

                m_resourceAttributes[it.key()] = it.value();
            }

            if(notify)
            {
                // asynchronous notification
                auto notifyFunc = [](NotificationReceiver *notificationReceiver,
                                        std::string uri)
                {
                    if (notificationReceiver)
                    {
                        notificationReceiver->onNotificationReceived(uri);
                    }
                };
                auto f = std::bind(notifyFunc, m_pNotiReceiver, m_uri);
                boost::thread notifyThread(f);
            }

        }

        void BundleResource::setAttribute(const std::string &key,
                                          RCSResourceAttributes::Value &&value, bool notify)
        {
            OIC_LOG_V(INFO, CONTAINER_TAG, "set attribute \(%s)'", std::string(key + "\', with " +
                     value.toString()).c_str());
            std::lock_guard<std::mutex> lock(m_resourceAttributes_mutex);
            m_resourceAttributes[key] = std::move(value);

            if(notify)
            {
                // asynchronous notification
                auto notifyFunc = [](NotificationReceiver *notificationReceiver,
                                        std::string uri)
                {
                    if (notificationReceiver)
                    {
                        notificationReceiver->onNotificationReceived(uri);
                    }
                };
                auto f = std::bind(notifyFunc, m_pNotiReceiver, m_uri);
                boost::thread notifyThread(f);
            }

        }

        void BundleResource::setAttribute(const std::string &key,
                                                 RCSResourceAttributes::Value &value, bool notify)
        {
            setAttribute(key, RCSResourceAttributes::Value(value), notify);
        }

        void BundleResource::setAttribute(const std::string &key,
                RCSResourceAttributes::Value &&value)
        {
            setAttribute(key, std::move(value), true);
        }

        void BundleResource::setAttribute(const std::string &key,
                        RCSResourceAttributes::Value &value)
        {
            setAttribute(key, value, true);
        }

        RCSResourceAttributes::Value BundleResource::getAttribute(const std::string &key)
        {
            OIC_LOG_V(INFO, CONTAINER_TAG, "get attribute \'(%s)" , std::string(key + "\'").c_str());
            std::lock_guard<std::mutex> lock(m_resourceAttributes_mutex);
            return m_resourceAttributes.at(key);
        }
    }
}
