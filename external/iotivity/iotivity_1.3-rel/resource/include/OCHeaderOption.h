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

/**
 * @file
 *
 * This file contains the declaration of classes and its members related to
 * OCHeaderOption.
 */

#ifndef OC_HEADEROPTION_H_
#define OC_HEADEROPTION_H_

#include <OCException.h>
#include <StringConstants.h>
namespace OC
{
    namespace HeaderOption
    {
        /**
        *     @brief OCHeaderOption class allows to create instances which comprises optionID
        *            and optionData as members. These are used in setting Header options.
        *            After creating instances of OCHeaderOptions, use setHeaderOptions API
        *            (in OCResource.h) to set header Options.
        *            NOTE: HeaderOptionID  is an unsigned integer value which MUST be within
        *            range of 2048 to 3000 inclusive of lower and upper bound
        *            except for If-Match with empty(num : 1), If-None-Match(num : 5),
        *            Location-Path(num : 8), Location-Query(num : 20), Accept(num : 17) option.
        *            HeaderOptions instance creation fails if above condition is not satisfied.
        */
        const uint16_t MIN_HEADER_OPTIONID = 2048;
        const uint16_t MAX_HEADER_OPTIONID = 3000;
        const uint16_t IF_MATCH_OPTION_ID = 1;
        const uint16_t IF_NONE_MATCH_OPTION_ID = 5;
        const uint16_t LOCATION_PATH_OPTION_ID = 8;
        const uint16_t LOCATION_QUERY_OPTION_ID = 20;
        const uint16_t ACCEPT_OPTION_ID = 17;
        const uint16_t CONTENT_TYPE_OPTION_ID = 12;

        class OCHeaderOption
        {
        private:
            uint16_t m_optionID;
            std::string m_optionData;

        public:
            /**
            * OCHeaderOption constructor
            */
            OCHeaderOption(uint16_t optionID, std::string optionData):
                m_optionID(optionID),
                m_optionData(optionData)
            {
                if (!(optionID >= MIN_HEADER_OPTIONID && optionID <= MAX_HEADER_OPTIONID)
                        && optionID != IF_MATCH_OPTION_ID
                        && optionID != IF_NONE_MATCH_OPTION_ID
                        && optionID != LOCATION_PATH_OPTION_ID
                        && optionID != LOCATION_QUERY_OPTION_ID
                        && optionID != ACCEPT_OPTION_ID
                        && optionID != CONTENT_TYPE_OPTION_ID)
                {
                    throw OCException(OC::Exception::OPTION_ID_RANGE_INVALID);
                }
            }

            virtual ~OCHeaderOption(){}

            OCHeaderOption(const OCHeaderOption&) = default;

#if defined(_MSC_VER) && (_MSC_VER < 1900)
            OCHeaderOption(OCHeaderOption&& o)
            {
                std::memmove(this, &o, sizeof(o));
            }
#else
            OCHeaderOption(OCHeaderOption&&) = default;
#endif

            OCHeaderOption& operator=(const OCHeaderOption&) = default;

#if defined(_MSC_VER) && (_MSC_VER < 1900)
            OCHeaderOption& operator=(OCHeaderOption&& o)
            {
                std::memmove(this, &o, sizeof(o));
            }
#else
            OCHeaderOption& operator=(OCHeaderOption&&) = default;
#endif

            /**
            * API to get Option ID
            * @return unsigned integer option ID
            */
            uint16_t getOptionID() const
            {
                return m_optionID;
            }

            /*
            * API to get Option data
            * @return std::string of option data
            */
            std::string getOptionData() const
            {
                return m_optionData;
            }
        };
    } // namespace HeaderOption
} // namespace OC

#endif // OC_HEADEROPTION_H_
