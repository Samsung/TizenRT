//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

#ifndef OC_RESOURCE_INIT_EXCEPTION_H_
#define OC_RESOURCE_INIT_EXCEPTION_H_

#include <stdexcept>
#include "StringConstants.h"

namespace OC
{
    class ResourceInitException : public std::exception
    {
    public:
        ResourceInitException(
                bool missingUri,
                bool missingType,
                bool missingInterface,
                bool missingClientWrapper,
                bool invalidPort,
                bool invalidIp)
        : m_whatMessage(),
          m_missingUri(missingUri),
          m_missingType(missingType),
          m_missingInterface(missingInterface),
          m_missingClientWrapper(missingClientWrapper),
          m_invalidPort(invalidPort),
          m_invalidIp(invalidIp)
        {
            if(isUriMissing())
            {
                m_whatMessage += OC::InitException::MISSING_URI;
            }

            if(isTypeMissing())
            {
                m_whatMessage += OC::InitException::MISSING_TYPE;
            }

            if(isInterfaceMissing())
            {
                m_whatMessage += OC::InitException::MISSING_INTERFACE;
            }

            if(isClientWrapperMissing())
            {
                m_whatMessage += OC::InitException::MISSING_CLIENT_WRAPPER;
            }

            if(isInvalidPort())
            {
                m_whatMessage += OC::InitException::INVALID_PORT;
            }

            if(isInvalidIp())
            {
                m_whatMessage += OC::InitException::INVALID_IP;
            }
        }

        virtual ~ResourceInitException() throw() {}

        bool isInvalidPort() const
        {
            return m_invalidPort;
        }

        bool isInvalidIp() const
        {
            return m_invalidIp;
        }

        bool isClientWrapperMissing() const
        {
            return m_missingClientWrapper;
        }

        bool isUriMissing() const
        {
            return m_missingUri;
        }

        bool isTypeMissing() const
        {
            return m_missingType;
        }

        bool isInterfaceMissing() const
        {
            return m_missingInterface;
        }

        virtual const char* what() const BOOST_NOEXCEPT
        {
            return m_whatMessage.c_str();
        }

    private:
        std::string m_whatMessage;
        bool m_missingUri;
        bool m_missingType;
        bool m_missingInterface;
        bool m_missingClientWrapper;
        bool m_invalidPort;
        bool m_invalidIp;
    };
}

#endif
