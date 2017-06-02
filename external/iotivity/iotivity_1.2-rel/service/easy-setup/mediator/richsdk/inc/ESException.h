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


#ifndef EASY_SETUP_EXCEPTION_H
#define EASY_SETUP_EXCEPTION_H

#include <string>
#include <octypes.h>
/**
 * @file
 *
 * This file contains the declaration of exception classes for easy setup
 */
namespace OIC
{
    namespace Service
    {

        /**
         * The base exception class for resource encapsulation.
         *
         */
        class ESException: public std::exception
        {
        public:

            /**
             * Constructs an exception with an empty description.
             */
            ESException();

            /**
             * Constructs an exception with a description.
             *
             * @param what The description for the error.
             */
            explicit ESException(const std::string &what);

            /**
             * @overload
             */
            explicit ESException(std::string &&what);

            virtual ~ESException() noexcept;

            /**
             * Returns the exception description.
             *
             */
            virtual const char *what() const noexcept;

        private:
            /**
             *  Exception description
             */
            const std::string m_what;
        };

        /**
         * Thrown when OC layer returns an error.
         *
         */
        class ESPlatformException: public ESException
        {
        public:

            /**
             * Constructs an exception with a description.
             *
             * @param OCStackResult The description for the error.
             */
            explicit ESPlatformException(OCStackResult reason);

            /**
             * Returns the reason.
             *
             */
            OCStackResult getReasonCode() const;

            /**
             * This function returns description of the exception.
             *
             */
            std::string getReason() const;

        private:
            OCStackResult m_reason;
        };

        /**
         * Thrown when a request is not acceptable.
         *
         */
        class ESBadRequestException: public ESException
        {
        public:
            /**
             * Constructs an exception with a description.
             *
             * @param OCStackResult The description for the error.
             */
            explicit ESBadRequestException(const std::string& what);
            /**
             * @overload
             */
            explicit ESBadRequestException(std::string&& what);
        };

        /**
         * Thrown when a parameter is not valid.
         *
         */
        class ESInvalidParameterException: public ESException
        {
        public:
            /**
             * Constructs an exception with a description.
             *
             * @param OCStackResult The description for the error.
             */
            explicit ESInvalidParameterException(const std::string& what);
         /**
             * @overload
             */
            explicit ESInvalidParameterException(std::string&& what);
        };

        /**
         * Thrown when getting value with wrong template parameter.
         */
        class ESBadGetException: public ESException
        {
        public:
            /**
             * Constructs an exception with a description.
             *
             * @param OCStackResult The description for the error.
             */
            explicit ESBadGetException(const std::string& what);
           /**
             * @overload
             */
            explicit ESBadGetException(std::string&& what);
        };

        /**
         * Thrown when a key is invalid.
         *
         */
        class ESInvalidKeyException: public ESException
        {
        public:
            /**
             * Constructs an exception with a description.
             *
             * @param OCStackResult The description for the error.
             */
            explicit ESInvalidKeyException(const std::string& what);
             /**
             * @overload
             */
            explicit ESInvalidKeyException(std::string&& what);
        };
    }
}

#endif // EASY_SETUP_EXCEPTION_H
