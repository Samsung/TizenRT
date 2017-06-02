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

#include "RCSRepresentation.h"

#include "ResourceAttributesConverter.h"

#include "OCRepresentation.h"

namespace OIC
{
    namespace Service
    {

        RCSRepresentation::RCSRepresentation() { }

        RCSRepresentation::RCSRepresentation(const std::string& uri) :
                m_uri{ uri }
        {
        }

        RCSRepresentation::RCSRepresentation(const RCSResourceAttributes& attrs) :
                m_uri{ },
                m_interfaces{ },
                m_resourceTypes{ },
                m_attributes{ attrs }
        {
        }

        RCSRepresentation::RCSRepresentation(const std::string& uri,
                const std::vector< std::string >& interfaces,
                const std::vector< std::string >& resourceTypes) :
                m_uri{ uri },
                m_interfaces{ interfaces },
                m_resourceTypes{ resourceTypes }
        {
        }

        RCSRepresentation::RCSRepresentation(const std::string& uri,
                const std::vector< std::string >& interfaces,
                const std::vector< std::string >& resourceTypes,
                const RCSResourceAttributes& attrs) :
                m_uri{ uri },
                m_interfaces{ interfaces },
                m_resourceTypes{ resourceTypes },
                m_attributes{ attrs }
        {
        }

        std::string RCSRepresentation::getUri() const
        {
            return m_uri;
        }

        void RCSRepresentation::setUri(std::string uri)
        {
            m_uri = std::move(uri);
        }

        const std::vector< std::string >& RCSRepresentation::getInterfaces() const
        {
            return m_interfaces;
        }

        void RCSRepresentation::addInterface(std::string interface)
        {
            m_interfaces.push_back(std::move(interface));
        }

        void RCSRepresentation::clearInterfaces()
        {
            m_interfaces.clear();
        }

        const std::vector< std::string >& RCSRepresentation::getResourceTypes() const
        {
            return m_resourceTypes;
        }

        void RCSRepresentation::addResourceType(std::string resourceType)
        {
            m_resourceTypes.push_back(std::move(resourceType));
        }

        void RCSRepresentation::clearResourceTypes()
        {
            m_resourceTypes.clear();
        }

        const RCSResourceAttributes& RCSRepresentation::getAttributes() const
        {
            return m_attributes;
        }

        RCSResourceAttributes& RCSRepresentation::getAttributes()
        {
            return m_attributes;
        }

        void RCSRepresentation::setAttributes(const RCSResourceAttributes& attrs)
        {
            m_attributes = attrs;
        }

        void RCSRepresentation::setAttributes(RCSResourceAttributes&& attrs)
        {
            m_attributes = std::move(attrs);
        }

        void RCSRepresentation::addChild(RCSRepresentation child)
        {
            m_children.push_back(std::move(child));
        }

        void RCSRepresentation::setChildren(std::vector< RCSRepresentation > children)
        {
            m_children = std::move(children);
        }

        const std::vector< RCSRepresentation >& RCSRepresentation::getChildren() const
        {
            return m_children;
        }

        void RCSRepresentation::clearChildren()
        {
            m_children.clear();
        }


        RCSRepresentation RCSRepresentation::fromOCRepresentation(const OC::OCRepresentation& ocRep)
        {
            RCSRepresentation rcsRep(ocRep.getUri(), ocRep.getResourceInterfaces(),
                    ocRep.getResourceTypes(),
                    ResourceAttributesConverter::fromOCRepresentation(ocRep));

            // Convert child representations
            for (auto &childOCRep : ocRep.getChildren())
            {
                rcsRep.addChild(fromOCRepresentation(childOCRep));
            }

            return rcsRep;
        }

        OC::OCRepresentation RCSRepresentation::toOCRepresentation(const RCSRepresentation& rcsRep)
        {
            return toOCRepresentation(RCSRepresentation{ rcsRep });
        }

        OC::OCRepresentation RCSRepresentation::toOCRepresentation(RCSRepresentation&& rcsRep)
        {
            auto ocRep =
                    ResourceAttributesConverter::toOCRepresentation(std::move(rcsRep.m_attributes));

            ocRep.setUri(std::move(rcsRep.m_uri));
            ocRep.setResourceInterfaces(std::move(rcsRep.m_interfaces));
            ocRep.setResourceTypes(std::move(rcsRep.m_resourceTypes));

            for (auto& child : rcsRep.m_children)
            {
                ocRep.addChild(toOCRepresentation(std::move(child)));
            }

            return ocRep;
        }
    }
}
