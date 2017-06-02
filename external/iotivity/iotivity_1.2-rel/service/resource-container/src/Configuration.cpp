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

#include "Configuration.h"

#include <stdexcept>
#include <utility>
#include "InternalTypes.h"
#include <string.h>

#define CONTAINER_TAG "RESOURCE_CONTAINER"
#define DISCOVER_TAG "DISCOVER_RESOURCE_UNIT"

namespace OIC
{
    namespace Service
    {
        static inline std::string trim_both(const std::string &str)
        {
            size_t npos = str.find_first_not_of(" \t\v\n\r");

            if (npos == std::string::npos)
            {
                return "";
            }

            std::string tempString = str.substr(npos, str.length());
            npos = tempString.find_last_not_of(" \t\v\n\r");

            return npos == std::string::npos ? tempString : tempString.substr(0, npos + 1);
        }

        Configuration::Configuration()
        {
            m_loaded = false;
        }

        Configuration::Configuration(string configFile)
        {
            m_loaded = false;

            m_pathConfigFile.append(configFile);

            getConfigDocument(m_pathConfigFile);
        }

        Configuration::~Configuration()
        {
        }

        bool Configuration::isLoaded() const
        {
            return m_loaded;
        }

        bool Configuration::isHasInput(std::string &bundleId) const
        {

            try
            {
                OIC_LOG_V(INFO, CONTAINER_TAG, "isHasInput: (%d) %s",m_mapisHasInput.at(bundleId), bundleId.c_str() );
                return m_mapisHasInput.at(bundleId);
            }
            catch (std::out_of_range &e)
            {
                OIC_LOG_V(INFO, CONTAINER_TAG, "isHasInput out of range %s.", bundleId.c_str());
                return false;
            }
        }

        void Configuration::getConfiguredBundles(configInfo *configOutput)
        {
            rapidxml::xml_node< char > *bundle = nullptr;
            rapidxml::xml_node< char > *subItem = nullptr;

            string strKey, strValue;

            if (m_loaded)
            {
                try
                {
                    bundle = m_xmlDoc.first_node();
                    if (bundle)
                    {
                        for (bundle = bundle->first_node(BUNDLE_TAG); bundle; bundle =
                                 bundle->next_sibling())
                        {
                            std::map< std::string, std::string > bundleMap;
                            for (subItem = bundle->first_node(); subItem;
                                 subItem = subItem->next_sibling())
                            {
                                strKey = subItem->name();
                                strValue = subItem->value();

                                if (strlen(subItem->value()) > 0)
                                {
                                    bundleMap.insert(
                                        std::make_pair(trim_both(strKey), trim_both(strValue)));
                                }
                            }
                            configOutput->push_back(bundleMap);
                        }
                    }

                }
                catch (rapidxml::parse_error &e)
                {
                    OIC_LOG(ERROR, CONTAINER_TAG, "xml parsing failed !!");
                    OIC_LOG_V(ERROR, CONTAINER_TAG, "Exception : (%s)", e.what());
                }
            }
        }

        void Configuration::getBundleConfiguration(string bundleId, configInfo *configOutput)
        {
            rapidxml::xml_node< char > *bundle = nullptr;

            string strBundleId, strPath, strVersion;

            if (m_loaded)
            {
                try
                {
                    std::map< std::string, std::string > bundleConfigMap;

                    // <bundle>
                    bundle = m_xmlDoc.first_node();
                    if (bundle)
                    {
                        for (bundle = bundle->first_node(BUNDLE_TAG); bundle; bundle =
                                 bundle->next_sibling())
                        {
                            // <id>
                            if (bundle->first_node(BUNDLE_ID))
                            {
                                strBundleId = bundle->first_node(BUNDLE_ID)->value();
                            }
                            else
                            {
                                strBundleId = "";
                            }

                            if (!strBundleId.compare(bundleId))
                            {
                                bundleConfigMap.insert(std::make_pair(BUNDLE_ID, trim_both(strBundleId)));

                                // <path>
                                if (bundle->first_node(BUNDLE_PATH))
                                {
                                    strPath = bundle->first_node(BUNDLE_PATH)->value();
                                }
                                else
                                {
                                    strPath = "";
                                }
                                bundleConfigMap.insert(std::make_pair(BUNDLE_PATH, trim_both(strPath)));

                                // <version>
                                if (bundle->first_node(BUNDLE_VERSION))
                                {
                                    strVersion = bundle->first_node(BUNDLE_VERSION)->value();
                                }
                                else
                                {
                                    strVersion = "";
                                }
                                bundleConfigMap.insert(
                                    std::make_pair(BUNDLE_VERSION, trim_both(strVersion)));

                                configOutput->push_back(bundleConfigMap);

                                break;
                            }
                        }
                    }
                }
                catch (rapidxml::parse_error &e)
                {
                    OIC_LOG(ERROR, CONTAINER_TAG, "xml parsing failed !!");
                    OIC_LOG_V(ERROR, CONTAINER_TAG, "Exception (%s)", e.what());
                }
            }
        }

        void Configuration::getResourceConfiguration(std::string bundleId, std::string resourceUri,
                        resourceInfo *resourceInfoOut)
        {
            rapidxml::xml_node< char > *bundle = nullptr;
            rapidxml::xml_node< char > *resource = nullptr;
            rapidxml::xml_node< char > *item = nullptr;
            rapidxml::xml_node< char > *subItem = nullptr;
            rapidxml::xml_node< char > *subItem2 = nullptr;

            string strBundleId;
            string strKey, strValue;
            OIC_LOG_V(INFO, CONTAINER_TAG, "Loading resource configuration for %s %s!",
                bundleId.c_str(), resourceUri.c_str());

            if (m_loaded)
            {
                try
                {
                    // <bundle>
                    bundle = m_xmlDoc.first_node();
                    if (bundle)
                    {
                        for (bundle = bundle->first_node(BUNDLE_TAG); bundle;
                            bundle = bundle->next_sibling())
                        {
                            // <id>
                            strBundleId = bundle->first_node(BUNDLE_ID)->value();

                            OIC_LOG_V(INFO, CONTAINER_TAG, "Comparing bundle id %s - %s !",
                                    strBundleId.c_str(), bundleId.c_str());

                            if (!strBundleId.compare(bundleId))
                            {
                                OIC_LOG_V(INFO, CONTAINER_TAG, "Inspecting");
                                // <resourceInfo>
                                bundle = bundle->first_node(OUTPUT_RESOURCES_TAG);
                                if (bundle)
                                {
                                    for (resource = bundle->first_node(OUTPUT_RESOURCE_INFO);
                                         resource; resource = resource->next_sibling())
                                    {

                                        for (item = resource->first_node(); item; item =
                                                 item->next_sibling())
                                        {
                                            strKey = item->name();
                                            strValue = item->value();

                                            if (!strKey.compare(OUTPUT_RESOURCE_NAME))
                                            {
                                                
                                                resourceInfoOut->name = trim_both(strValue);
                                            }

                                            else if (!strKey.compare(OUTPUT_RESOURCE_URI))
                                            {
                                                if (trim_both(strValue).compare(resourceUri) != 0)
                                                {
                                                    break;
                                                }
                                                resourceInfoOut->uri = trim_both(strValue);
                                            }

                                            else if (!strKey.compare(OUTPUT_RESOURCE_ADDR))
                                            {
                                                resourceInfoOut->address = trim_both(strValue);
                                            }

                                            else if (!strKey.compare(OUTPUT_RESOURCE_TYPE))
                                            {
                                                resourceInfoOut->resourceType = trim_both(strValue);
                                            }

                                            else
                                            {
                                                for (subItem = item->first_node(); subItem; subItem =
                                                         subItem->next_sibling())
                                                {
                                                    map< string, string > propertyMap;

                                                    strKey = subItem->name();

                                                    if (strKey.compare(INPUT_RESOURCE))
                                                    {
                                                        m_mapisHasInput[strBundleId] = true;
                                                        OIC_LOG_V(INFO, CONTAINER_TAG,
                                                                "Bundle has input (%s)",
                                                                strBundleId.c_str());
                                                    }

                                                    for (subItem2 = subItem->first_node(); subItem2;
                                                         subItem2 = subItem2->next_sibling())
                                                    {
                                                        string newStrKey = subItem2->name();
                                                        string newStrValue = subItem2->value();
                                                        OIC_LOG_V(INFO, CONTAINER_TAG,
                                                                "key: %s, value %s",
                                                                newStrKey.c_str(), newStrValue.c_str());

                                                        propertyMap[trim_both(newStrKey)] =
                                                                trim_both(newStrValue);
                                                    }

                                                    resourceInfoOut->resourceProperty[trim_both(strKey)].push_back(
                                                        propertyMap);
                                                }
                                            }
                                        }

                                    }
                                }
                            }
                        }
                    }
                }
                catch (rapidxml::parse_error &e)
                {
                    OIC_LOG(ERROR, CONTAINER_TAG, "xml parsing failed !!");
                    OIC_LOG_V(ERROR, CONTAINER_TAG, "Exception (%s)", e.what());
                }
            }
            else
            {
                OIC_LOG(INFO, CONTAINER_TAG, "config is not loaded yet !!");
            }
        }

        void Configuration::getResourceConfiguration(std::string bundleId,
                std::vector< resourceInfo > *configOutput)
        {
            rapidxml::xml_node< char > *bundle = nullptr;
            rapidxml::xml_node< char > *resource = nullptr;
            rapidxml::xml_node< char > *item = nullptr;
            rapidxml::xml_node< char > *subItem = nullptr;
            rapidxml::xml_node< char > *subItem2 = nullptr;

            string strBundleId;
            string strKey, strValue;
            OIC_LOG(INFO, CONTAINER_TAG, "Loading resource configuration!");

            if (m_loaded)
            {
                try
                {
                    // <bundle>
                    bundle = m_xmlDoc.first_node();
                    if (bundle)
                    {
                        for (bundle = bundle->first_node(BUNDLE_TAG); bundle; bundle =
                                 bundle->next_sibling())
                        {
                            // <id>
                            if (bundle->first_node(BUNDLE_ID))
                            {
                                strBundleId = bundle->first_node(BUNDLE_ID)->value();
                            }

                            OIC_LOG_V(INFO, CONTAINER_TAG, "Comparing bundle ids %s - %s !",
                                    strBundleId.c_str(), bundleId.c_str());

                            if (!strBundleId.compare(bundleId))
                            {
                                OIC_LOG_V(INFO, CONTAINER_TAG, "Inspecting");
                                // <resourceInfo>
                                bundle = bundle->first_node(OUTPUT_RESOURCES_TAG);
                                if (bundle)
                                {
                                    for (resource = bundle->
                                            first_node(OUTPUT_RESOURCE_INFO);
                                         resource; resource = resource->next_sibling())
                                    {
                                        resourceInfo tempResourceInfo;

                                        for (item = resource->first_node(); item; item =
                                                 item->next_sibling())
                                        {
                                            strKey = item->name();
                                            strValue = item->value();

                                            if (!strKey.compare(OUTPUT_RESOURCE_NAME))
                                            {
                                                tempResourceInfo.name = trim_both(strValue);
                                            }

                                            else if (!strKey.compare(OUTPUT_RESOURCE_URI))
                                            {
                                                tempResourceInfo.uri = trim_both(strValue);
                                            }

                                            else if (!strKey.compare(OUTPUT_RESOURCE_ADDR))
                                            {
                                                tempResourceInfo.address = trim_both(strValue);
                                            }

                                            else if (!strKey.compare(OUTPUT_RESOURCE_TYPE))
                                            {
                                                tempResourceInfo.resourceType = trim_both(strValue);
                                            }

                                            else
                                            {
                                                for (subItem = item->first_node(); subItem; subItem =
                                                         subItem->next_sibling())
                                                {
                                                    map< string, string > propertyMap;

                                                    strKey = subItem->name();

                                                    if (strKey.compare(INPUT_RESOURCE))
                                                    {
                                                        m_mapisHasInput[strBundleId] = true;
                                                        OIC_LOG_V(INFO, CONTAINER_TAG,
                                                                "Bundle has input (%s)",
                                                                strBundleId.c_str());
                                                    }

                                                    for (subItem2 = subItem->first_node(); subItem2;
                                                         subItem2 = subItem2->next_sibling())
                                                    {
                                                        string newStrKey = subItem2->name();
                                                        string newStrValue = subItem2->value();
                                                        OIC_LOG_V(INFO, CONTAINER_TAG,
                                                                "key: %s, value %s",
                                                                newStrKey.c_str(),
                                                                newStrValue.c_str());

                                                        propertyMap[trim_both(newStrKey)] =
                                                                trim_both(newStrValue);
                                                    }

                                                    tempResourceInfo.resourceProperty[trim_both(strKey)].
                                                    push_back(propertyMap);
                                                }
                                            }
                                        }
                                        configOutput->push_back(tempResourceInfo);
                                    }
                                }
                            }
                        }
                    }
                }
                catch (rapidxml::parse_error &e)
                {
                    OIC_LOG(ERROR, CONTAINER_TAG, "xml parsing failed !!");
                    OIC_LOG_V(ERROR, CONTAINER_TAG, "Exception (%s)", e.what());
                }
            }
            else
            {
                OIC_LOG(INFO, CONTAINER_TAG, "config is not loaded yet !!");
            }
        }

        void Configuration::getConfigDocument(std::string pathConfigFile)
        {
            std::basic_ifstream< char > xmlFile(pathConfigFile.c_str());

            if (!xmlFile.fail())
            {
                xmlFile.seekg(0, std::ios::end);
                unsigned int size = (unsigned int) xmlFile.tellg();
                xmlFile.seekg(0);

                std::vector< char > xmlData(size + 1);
                xmlData[size] = 0;

                xmlFile.read(&xmlData.front(), (std::streamsize) size);
                xmlFile.close();
                m_strConfigData = std::string(xmlData.data());

                try
                {
                    m_xmlDoc.parse< 0 >((char *) m_strConfigData.c_str());
                    m_loaded = true;
                }
                catch (rapidxml::parse_error &e)
                {
                    OIC_LOG(ERROR, CONTAINER_TAG, "xml parsing failed !!");
                    OIC_LOG_V(ERROR, CONTAINER_TAG, "Exception (%s)", e.what());
                }
            }
            else
            {
                OIC_LOG(ERROR, CONTAINER_TAG, "Configuration File load failed !!");
            }
        }
    }
}
