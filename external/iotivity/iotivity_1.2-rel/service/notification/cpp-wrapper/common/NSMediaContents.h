//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * This file contains Notification service media contents representation.
 */

#ifndef _NS_MEDIA_CONTENTS_H_
#define _NS_MEDIA_CONTENTS_H_


#include <string>
#include "NSCommon.h"

namespace OIC
{
    namespace Service
    {
        /**
         * @class   NSMediaContents
         * @brief   This class provides a set of APIs for Notification service Media Contents.
         */
        class NSMediaContents
        {
            public:
                /**
                     * Constructor of NSMediaContents.
                     */
                NSMediaContents() = default;

                /**
                     * Constructor of NSMediaContents.
                     *
                     * @param mediacontents - pointer to NSMediaContents struct to initialize.
                     */
                NSMediaContents(::NSMediaContents *mediacontents);

                /**
                     * Constructor of NSMediaContents.
                     *
                     * @param iconImage - iconImage of the Notification service MediaContents.
                     */
                NSMediaContents(const std::string &iconImage)
                    : m_iconImage(iconImage)
                {
                }


                /**
                     * Destructor of NSMediaContents.
                     */
                ~NSMediaContents() = default;

                /**
                      * This method is for getting icon image from the Notification service media contents.
                      *
                      * @return iconImage as string.
                      */
                std::string getIconImage() const;

                /**
                      * This method is for setting icon image for the Notification service media contents.
                      *
                      * @param iconImage - as string.
                      */
                void setIconImage(const std::string &iconImage);

            private:
                std::string m_iconImage;

        };
    }
}
#endif /* _NS_MEDIA_CONTENTS_H_ */
