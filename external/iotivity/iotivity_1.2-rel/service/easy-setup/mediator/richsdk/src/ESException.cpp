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

#include "ESException.h"

#include "OCException.h"

namespace OIC
{
    namespace Service
    {

        ESException::ESException()
        {
        }

        ESException::ESException(const std::string& what) :
                m_what{ what }
        {
        }

        ESException::ESException(std::string&& what) :
                m_what{ std::move(what) }
        {
        }

        ESException::~ESException() noexcept
        {
        }

        const char* ESException::what() const noexcept
        {
            return m_what.c_str();
        }


        ESPlatformException::ESPlatformException(OCStackResult reason) :
                ESException{ "Failed : " + OC::OCException::reason(reason) },
                m_reason { reason }
        {
        }

        OCStackResult ESPlatformException::getReasonCode() const
        {
            return m_reason;
        }

        std::string ESPlatformException::getReason() const
        {
            return  OC::OCException::reason(m_reason);
        }


        ESBadRequestException::ESBadRequestException(const std::string& what) :
                ESException{ what }
        {
        }

        ESBadRequestException::ESBadRequestException(std::string&& what) :
                ESException{ std::move(what) }
        {
        }


        ESInvalidParameterException::ESInvalidParameterException(const std::string& what) :
                ESException{ what }
        {
        }

        ESInvalidParameterException::ESInvalidParameterException(std::string&& what) :
                ESException{ std::move(what) }
        {
        }


        ESBadGetException::ESBadGetException(const std::string& what) :
                ESException{ what }
        {
        }

        ESBadGetException::ESBadGetException(std::string&& what) :
                ESException{ std::move(what) }
        {
        }


        ESInvalidKeyException::ESInvalidKeyException(const std::string& what) :
                ESException{ what }
        {
        }

        ESInvalidKeyException::ESInvalidKeyException(std::string&& what) :
                ESException{ std::move(what) }
        {
        }
    }
}
