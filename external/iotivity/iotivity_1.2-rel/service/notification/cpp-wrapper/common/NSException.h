/******************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/**
 * @file   NSException.h
 *
 * @brief   This file provides exception handling for Notification Service.
 */

#ifndef _NS_EXCEPTION_H_
#define _NS_EXCEPTION_H_

#include <exception>

namespace OIC
{
    namespace Service
    {
        /**
         * @class   NSException
         * @brief   This is the base exception of all type of exception thrown from Notification service module.
         */
        class NSException : public std::exception
        {
            public:
                /**
                      * Constructor of NSException.
                      *
                      * @param message - String describing the error messsage.
                      */
                NSException(const std::string &message) : m_message(message) {}

                /**
                      * API to get error message describing exception reason.
                      *
                      * @return Null terminated string.
                      */
                virtual const char *what() const noexcept                
                {
                    return m_message.c_str();
                }

                virtual ~NSException() throw() {}

            private:
                std::string m_message;
        };
    }
}
#endif /* _NS_EXCEPTION_H_ */
