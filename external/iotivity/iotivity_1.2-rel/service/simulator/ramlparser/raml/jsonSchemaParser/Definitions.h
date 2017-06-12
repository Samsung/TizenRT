/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
 * @file   Definitions.h
 *
 * @brief   This file provides data Model for Json Schema Definitions.
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <string>
#include <vector>
#include <map>
#include "Properties.h"
#include <memory>

namespace RAML
{
    /**
     * @class   Definitions
     * @brief   This class provides data Model for Json Schema Definitions.
     */
    class Definitions
    {
        public:

            /**
                  * Constructor of Definitions.
                  */
            Definitions() = default;
            /**
                  * Copy Constructor of Definitions.
                  *
                  * @param Definitions
                  */
            Definitions(const Definitions &) = default;
            /**
                  * Assignment operator for Definitions.
                  *
                  * @param Definitions
                  */
            Definitions &operator=(const Definitions &) = default;
            /**
                  * Copy Constructor of Definitions.
                  *
                  * @param Definitions
                  */
            Definitions(Definitions &&) = default;
            /**
                  * Assignment operator for Definitions.
                  *
                  * @param Definitions
                  */
            Definitions &operator=(Definitions &&) = default;

            /**
                  * Constructor of Definitions.
                  *
                  * @param name - Definitions name as string.
                  */
            Definitions(const std::string &name) : m_defName(name) {}

            /**
                 * This method is for getting Name from Definitions.
                 *
                 * @return Definitions name as string
                 */
            std::string getName() const;
            /**
                 * This method is for setting name to Definitions
                 *
                 * @param name - Definitions name as string.
                 */
            void setName(const std::string &name);

            /**
                 * This method is for getting Type from Definitions.
                 *
                 * @return Definitions Type as string
                 */
            std::string getType() const;

            /**
                 * This method is for setting Type to Definitions
                 *
                 * @param type - Definitions Type as string.
                 */
            void setType(const std::string &type);
            /**
                 * This method is for getting RequiredValue from Definitions.
                 *
                 * @return list of RequiredValue as string
                 */
            std::vector<std::string> const &getRequiredValues() const;

            /**
                 * This method is for setting RequiredValue to Definitions
                 *
                 * @param reqValue - RequiredValue as string.
                 */
            void setRequiredValue(const std::string &reqValue);

            /**
                 * This method is for getting size of Properties from Definitions.
                 *
                 * @return size of Properties map
                 */
            int propertiesSize() const { return m_properties.size(); }

            /**
                 * This method is for getting Properties from Definitions.
                 *
                 * @param propName - name of property as string.
                 *
                 * @return pointer to Properties.
                 */
            PropertiesPtr getproperty(const std::string &propName );

            /**
                 * This method is for getting Properties from Definitions.
                 *
                 * @return map of Property name and pointer to Properties
                 */
            std::map<std::string, PropertiesPtr> const  &getProperties();

            /**
                 * This method is for setting Properties to Definitions
                 *
                 * @param propName - Definitions Type as string.
                 * @param property - pointer to Properties.
                 */
            void addProperty(const std::string &propName, const PropertiesPtr &property);

        private:
            std::map<std::string, PropertiesPtr > m_properties;
            std::string m_defName;
            std::string m_type;
            std::vector<std::string> m_required;

    };

    /** DefinitionsPtr - shared Ptr to Definitions.*/
    typedef std::shared_ptr<Definitions> DefinitionsPtr;

}
#endif
