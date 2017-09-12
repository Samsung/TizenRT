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

#include "iotivity_config.h"
#include "OCResource.h"
#include "OCUtilities.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "cacommon.h"
#include "cautilinterface.h"
#include "oic_string.h"

#include <boost/lexical_cast.hpp>
#include <sstream>
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_WS2TCPIP_H
#include <ws2tcpip.h>
#endif
#ifdef HAVE_IN6ADDR_H
#include <in6addr.h>
#endif
#include "ocstack.h"

namespace OC {

static const char COAP[] = "coap://";
static const char COAPS[] = "coaps://";
static const char COAP_TCP[] = "coap+tcp://";
static const char COAPS_TCP[] = "coaps+tcp://";
static const char COAP_GATT[] = "coap+gatt://";
static const char COAP_RFCOMM[] = "coap+rfcomm://";

using OC::nil_guard;
using OC::result_guard;
using OC::checked_guard;

OCResource::OCResource(std::weak_ptr<IClientWrapper> clientWrapper,
                        const OCDevAddr& devAddr, const std::string& uri,
                        const std::string& serverId, uint8_t property,
                        const std::vector<std::string>& resourceTypes,
                        const std::vector<std::string>& interfaces)
 :  m_clientWrapper(clientWrapper), m_uri(uri),
    m_resourceId(serverId, m_uri), m_devAddr(devAddr),
    m_isCollection(false), m_property(property),
    m_resourceTypes(resourceTypes), m_interfaces(interfaces),
    m_observeHandle(nullptr)
{
    m_isCollection = std::find(m_interfaces.begin(), m_interfaces.end(), LINK_INTERFACE)
                        != m_interfaces.end();

    if (m_uri.empty() ||
        resourceTypes.empty() ||
        interfaces.empty()||
        m_clientWrapper.expired())
    {
        throw ResourceInitException(m_uri.empty(), resourceTypes.empty(),
                interfaces.empty(), m_clientWrapper.expired(), false, false);
    }
}

OCResource::OCResource(std::weak_ptr<IClientWrapper> clientWrapper,
                        const OCDevAddr& devAddr, const std::string& uri,
                        const std::string& serverId, uint8_t property,
                        const std::vector<std::string>& resourceTypes,
                        const std::vector<std::string>& interfaces,
                        const std::vector<std::string>& endpoints,
                        const HeaderOptions& serverHeaderOptions)
 :  m_clientWrapper(clientWrapper), m_uri(uri),
    m_resourceId(serverId, m_uri), m_devAddr(devAddr),
    m_isCollection(false), m_property(property),
    m_resourceTypes(resourceTypes), m_interfaces(interfaces),
    m_endpoints(endpoints),
    m_serverHeaderOptions(serverHeaderOptions),
    m_observeHandle(nullptr)
{
    m_isCollection = std::find(m_interfaces.begin(), m_interfaces.end(), LINK_INTERFACE)
                        != m_interfaces.end();

    if (m_uri.empty() ||
        resourceTypes.empty() ||
        interfaces.empty()||
        m_clientWrapper.expired())
    {
        throw ResourceInitException(m_uri.empty(), resourceTypes.empty(),
                interfaces.empty(), m_clientWrapper.expired(), false, false);
    }
}

OCResource::OCResource(std::weak_ptr<IClientWrapper> clientWrapper,
                        const std::string& host, const std::string& uri,
                        const std::string& serverId,
                        OCConnectivityType connectivityType, uint8_t property,
                        const std::vector<std::string>& resourceTypes,
                        const std::vector<std::string>& interfaces,
                        const std::vector<std::string>& endpoints)
 :  m_clientWrapper(clientWrapper), m_uri(uri),
    m_resourceId(serverId, m_uri),
    m_isCollection(false), m_property(property),
    m_resourceTypes(resourceTypes), m_interfaces(interfaces),
    m_endpoints(endpoints),
    m_observeHandle(nullptr)
{
    m_devAddr = OCDevAddr{OC_DEFAULT_ADAPTER, OC_DEFAULT_FLAGS, 0, {0}, 0, {0}, {0}};
    m_isCollection = std::find(m_interfaces.begin(), m_interfaces.end(), LINK_INTERFACE)
                        != m_interfaces.end();

    if (m_uri.empty() ||
        resourceTypes.empty() ||
        interfaces.empty()||
        m_clientWrapper.expired())
    {
        throw ResourceInitException(m_uri.empty(), resourceTypes.empty(),
                interfaces.empty(), m_clientWrapper.expired(), false, false);
    }

    if (uri.length() == 1 && uri[0] == '/')
    {
        throw ResourceInitException(m_uri.empty(), resourceTypes.empty(),
                interfaces.empty(), m_clientWrapper.expired(), false, false);
    }

    if (uri[0] != '/')
    {
        throw ResourceInitException(m_uri.empty(), resourceTypes.empty(),
                interfaces.empty(), m_clientWrapper.expired(), false, false);
    }

    // construct the devAddr from the pieces we have
    m_devAddr.adapter = static_cast<OCTransportAdapter>(connectivityType >> CT_ADAPTER_SHIFT);
    m_devAddr.flags = static_cast<OCTransportFlags>(connectivityType & CT_MASK_FLAGS);

    this->setHost(host);
}

OCResource::OCResource(std::weak_ptr<IClientWrapper> clientWrapper,
                        const std::string& host, const std::string& uri,
                        const std::string& serverId,
                        OCConnectivityType connectivityType, uint8_t property,
                        const std::vector<std::string>& resourceTypes,
                        const std::vector<std::string>& interfaces)
:  m_clientWrapper(clientWrapper), m_uri(uri),
    m_resourceId(serverId, m_uri),
    m_isCollection(false), m_property(property),
    m_resourceTypes(resourceTypes), m_interfaces(interfaces),
    m_observeHandle(nullptr)
{
    m_devAddr = OCDevAddr{OC_DEFAULT_ADAPTER, OC_DEFAULT_FLAGS, 0, {0}, 0,
#if defined (ROUTING_GATEWAY) || defined (ROUTING_EP)
                          {0},
#endif
                          {0}};
    m_isCollection = std::find(m_interfaces.begin(), m_interfaces.end(), LINK_INTERFACE)
                        != m_interfaces.end();

    if (m_uri.empty() ||
        resourceTypes.empty() ||
        interfaces.empty()||
        m_clientWrapper.expired())
    {
        throw ResourceInitException(m_uri.empty(), resourceTypes.empty(),
                interfaces.empty(), m_clientWrapper.expired(), false, false);
    }

    if (uri.length() == 1 && uri[0] == '/')
    {
        throw ResourceInitException(m_uri.empty(), resourceTypes.empty(),
                interfaces.empty(), m_clientWrapper.expired(), false, false);
    }

    if (uri[0] != '/')
    {
        throw ResourceInitException(m_uri.empty(), resourceTypes.empty(),
                interfaces.empty(), m_clientWrapper.expired(), false, false);
    }

    // construct the devAddr from the pieces we have
    m_devAddr.adapter = static_cast<OCTransportAdapter>(connectivityType >> CT_ADAPTER_SHIFT);
    m_devAddr.flags = static_cast<OCTransportFlags>(connectivityType & CT_MASK_FLAGS);

    this->setHost(host);
}

OCResource::~OCResource()
{
}

std::string OCResource::setHost(const std::string& host)
{
    if (!host.length())
    {
        throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
        m_interfaces.empty(), m_clientWrapper.expired(), false, false);
    }

    size_t prefix_len;

    OCDevAddr new_devAddr;
    memset(&new_devAddr, 0, sizeof(new_devAddr));
    new_devAddr.adapter = OC_DEFAULT_ADAPTER;
    new_devAddr.flags = OC_DEFAULT_FLAGS;

    // init m_devAddr
    m_devAddr = new_devAddr;
    bool usingIpAddr = false;

    if (host.compare(0, sizeof(COAPS) - 1, COAPS) == 0)
    {
        if (!OC_SECURE)
        {
            throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
            m_interfaces.empty(), m_clientWrapper.expired(), false, false);
        }
        prefix_len = sizeof(COAPS) - 1;
        m_devAddr.flags = static_cast<OCTransportFlags>(m_devAddr.flags | OC_SECURE);
        m_devAddr.adapter = OC_ADAPTER_IP;
        usingIpAddr = true;
    }
    else if (host.compare(0, sizeof(COAP) - 1, COAP) == 0)
    {
        prefix_len = sizeof(COAP) - 1;
        m_devAddr.adapter = OC_ADAPTER_IP;
        usingIpAddr = true;
    }
    else if (host.compare(0, sizeof(COAP_TCP) - 1, COAP_TCP) == 0)
    {
        prefix_len = sizeof(COAP_TCP) - 1;
        m_devAddr.adapter = OC_ADAPTER_TCP;
        usingIpAddr = true;
    }
    else if (host.compare(0, sizeof(COAPS_TCP) - 1, COAPS_TCP) == 0)
    {
        if (!OC_SECURE)
        {
            throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
            m_interfaces.empty(), m_clientWrapper.expired(), false, false);
        }
        prefix_len = sizeof(COAPS_TCP) - 1;
        m_devAddr.flags = static_cast<OCTransportFlags>(m_devAddr.flags | OC_SECURE);
        m_devAddr.adapter = OC_ADAPTER_TCP;
        usingIpAddr = true;
    }
    else if (host.compare(0, sizeof(COAP_GATT) - 1, COAP_GATT) == 0)
    {
        prefix_len = sizeof(COAP_GATT) - 1;
        m_devAddr.adapter = OC_ADAPTER_GATT_BTLE;
    }
    else if (host.compare(0, sizeof(COAP_RFCOMM) - 1, COAP_RFCOMM) == 0)
    {
        prefix_len = sizeof(COAP_RFCOMM) - 1;
        m_devAddr.adapter = OC_ADAPTER_RFCOMM_BTEDR;
    }
    else
    {
        throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
            m_interfaces.empty(), m_clientWrapper.expired(), false, false);
    }

    // set flag
    if (usingIpAddr)
    {
        if (host.find('[') != std::string::npos)
        {
            // ipv6
            m_devAddr.flags = static_cast<OCTransportFlags>(m_devAddr.flags | OC_IP_USE_V6);
        }
        else
        {
            // ipv4
            m_devAddr.flags = static_cast<OCTransportFlags>(m_devAddr.flags | OC_IP_USE_V4);
        }
    }

    // remove 'coap://' or 'coaps://' or 'coap+tcp://' or 'coap+gatt://' or 'coap+rfcomm://'
    std::string host_token = host.substr(prefix_len);

    if (host_token[0] == '[') // IPv6
    {
        size_t bracket = host_token.find(']');

        if (std::string::npos == bracket || 0 == bracket)
        {
            throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                m_interfaces.empty(), m_clientWrapper.expired(), false, false);
        }
        // extract the ipv6 address
        std::string ip6Addr = host_token.substr(1, bracket - 1);

        // address validity check
        std::string ip6AddrToValidityCheck(ip6Addr);
        size_t percent = ip6AddrToValidityCheck.find('%');
        if (std::string::npos != percent)
        {
            ip6AddrToValidityCheck.resize(percent);
        }
        struct in6_addr buf;
        const char *cAddr = ip6AddrToValidityCheck.c_str();
        if (0 == inet_pton(AF_INET6, cAddr, &buf))
        {
            throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                m_interfaces.empty(), m_clientWrapper.expired(), false, false);
        }

        //skip ']' and ':' characters in host string
        host_token = host_token.substr(bracket + 2);
        int port = std::stoi(host_token);

        if (0 > port || UINT16_MAX < port)
        {
            throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                m_interfaces.empty(), m_clientWrapper.expired(), false, false);
        }

        if (std::string::npos != ip6Addr.find("%25"))
        {
            OCStackResult result = OCDecodeAddressForRFC6874(m_devAddr.addr,
                                   sizeof(m_devAddr.addr), ip6Addr.c_str(), nullptr);

            if (OC_STACK_OK != result)
            {
                throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                    m_interfaces.empty(), m_clientWrapper.expired(), false, false);
            }
        }
        else
        {
            // It means zone-id is missing, check ipv6Addr is link local
            OCTransportFlags scopeLevel;
            OCStackResult ocResult = OCGetIpv6AddrScope(ip6Addr.c_str(), &scopeLevel);

            if (OC_STACK_OK != ocResult)
            {
                throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                    m_interfaces.empty(), m_clientWrapper.expired(), false, false);
            }
            else
            {
                if (OC_SCOPE_LINK == scopeLevel)
                {
                    {
                        // Given ip address is link-local scope without zone-id.
                        throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                            m_interfaces.empty(), m_clientWrapper.expired(), false, false);
                    }
                }
                else
                {
                    if (!OICStrcpy(m_devAddr.addr, sizeof(m_devAddr.addr), ip6Addr.c_str()))
                    {
                        throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                            m_interfaces.empty(), m_clientWrapper.expired(), false, false);
                    }
                }
            }
        }

        m_devAddr.port = static_cast<uint16_t>(port);
        m_devAddr.flags = static_cast<OCTransportFlags>(m_devAddr.flags | OC_IP_USE_V6);
    }
    else if (host_token[0] == ':')
    {
        throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
            m_interfaces.empty(), m_clientWrapper.expired(), false, false);
    }
    else
    {
        size_t dot = host_token.find('.');
        if (std::string::npos == dot) // MAC
        {
            std::string macAddr = host_token;

            // address validity check
            if (MAC_ADDR_STR_SIZE != macAddr.length())
            {
                throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                    m_interfaces.empty(), m_clientWrapper.expired(), false, false);
            }

            for (size_t blockCnt = 0; blockCnt < MAC_ADDR_BLOCKS; blockCnt++)
            {
                std::string block = macAddr.substr(blockCnt * 3, 2);

                if (std::string::npos != block.find_first_not_of("0123456789ABCDEFabcdef"))
                {
                    throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                        m_interfaces.empty(), m_clientWrapper.expired(), false, false);
                }

                if (MAC_ADDR_BLOCKS - 1 > blockCnt)
                {
                    char delimiter = macAddr[blockCnt * 3 + 2];

                    if (':' != delimiter)
                    {
                        throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                            m_interfaces.empty(), m_clientWrapper.expired(), false, false);
                    }
                }
            }

            macAddr.copy(m_devAddr.addr, sizeof(m_devAddr.addr));
            m_devAddr.addr[MAC_ADDR_STR_SIZE] = '\0';
        }
        else // IPv4
        {
            size_t colon = host_token.find(':');

            if (colon == std::string::npos || colon == 0)
            {
                throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                    m_interfaces.empty(), m_clientWrapper.expired(), false, false);
            }

            // extract the ipv4 address
            std::string ip4Addr = host_token.substr(0, colon);

            // address validity check
            struct in_addr buf;
            const char *cAddr = ip4Addr.c_str();
            if (0 == inet_pton(AF_INET, cAddr, &buf))
            {
                throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                    m_interfaces.empty(), m_clientWrapper.expired(), false, false);
            }

            //skip ':' characters in host string
            host_token = host_token.substr(colon + 1);
            int port = std::stoi(host_token);

            if (0 > port || UINT16_MAX < port)
            {
                throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                    m_interfaces.empty(), m_clientWrapper.expired(), false, false);
            }

            ip4Addr.copy(m_devAddr.addr, sizeof(m_devAddr.addr));
            m_devAddr.addr[ip4Addr.length()] = '\0';
            m_devAddr.port = static_cast<uint16_t>(port);
        }
    }
    return this->host();
}

OCStackResult OCResource::get(const QueryParamsMap& queryParametersMap,
                              GetCallback attributeHandler, QualityOfService QoS)
{
    return checked_guard(m_clientWrapper.lock(),
                            &IClientWrapper::GetResourceRepresentation,
                            m_devAddr, m_uri,
                            queryParametersMap, m_headerOptions, CT_DEFAULT,
                            attributeHandler, QoS);
}

OCStackResult OCResource::get(const QueryParamsMap& queryParametersMap,
                              GetCallback attributeHandler)
{
    QualityOfService defaultQos = OC::QualityOfService::NaQos;
    checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetDefaultQos, defaultQos);
    return result_guard(get(queryParametersMap, attributeHandler, defaultQos));
}

OCStackResult OCResource::get(const std::string& resourceType,
                     const std::string& resourceInterface, const QueryParamsMap& queryParametersMap,
                     GetCallback attributeHandler)
{
    QualityOfService defaultQoS = OC::QualityOfService::NaQos;
    checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetDefaultQos, defaultQoS);

    return result_guard(get(resourceType, resourceInterface, queryParametersMap, attributeHandler, defaultQoS));
}

OCStackResult OCResource::get(const std::string& resourceType, const std::string& resourceInterface, const QueryParamsMap& queryParametersMap, GetCallback attributeHandler,
        QualityOfService QoS)
{
    QueryParamsMap mapCpy(queryParametersMap);

    if (!resourceType.empty())
    {
        mapCpy[OC::Key::RESOURCETYPESKEY]=resourceType;
    }

    if (!resourceInterface.empty())
    {
        mapCpy[OC::Key::INTERFACESKEY]= resourceInterface;
    }

    return result_guard(get(mapCpy, attributeHandler, QoS));
}

OCStackResult OCResource::put(const OCRepresentation& rep,
                              const QueryParamsMap& queryParametersMap, PutCallback attributeHandler,
                              QualityOfService QoS)
{
    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::PutResourceRepresentation,
                         m_devAddr, m_uri, rep, queryParametersMap,
                         m_headerOptions, attributeHandler, QoS);
}

OCStackResult OCResource::put(const OCRepresentation& rep,
                              const QueryParamsMap& queryParametersMap, PutCallback attributeHandler)
{
    QualityOfService defaultQos = OC::QualityOfService::NaQos;
    checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetDefaultQos, defaultQos);
    return result_guard(put(rep, queryParametersMap, attributeHandler, defaultQos));
}

OCStackResult OCResource::put(const std::string& resourceType,
                              const std::string& resourceInterface, const OCRepresentation& rep,
                              const QueryParamsMap& queryParametersMap,
                              PutCallback attributeHandler)
{
    QualityOfService defaultQos = OC::QualityOfService::NaQos;
    checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetDefaultQos, defaultQos);

    return result_guard(put(resourceType, resourceInterface, rep, queryParametersMap,
            attributeHandler, defaultQos));
}

OCStackResult OCResource::put(const std::string& resourceType,
                              const std::string& resourceInterface, const OCRepresentation& rep,
                              const QueryParamsMap& queryParametersMap,
                              PutCallback attributeHandler,
                              QualityOfService QoS)
{
    QueryParamsMap mapCpy(queryParametersMap);

    if (!resourceType.empty())
    {
        mapCpy[OC::Key::RESOURCETYPESKEY]=resourceType;
    }

    if (!resourceInterface.empty())
    {
        mapCpy[OC::Key::INTERFACESKEY]=resourceInterface;
    }

    return result_guard(put(rep, mapCpy, attributeHandler, QoS));
}

OCStackResult OCResource::post(const OCRepresentation& rep,
                               const QueryParamsMap& queryParametersMap, PostCallback attributeHandler,
                               QualityOfService QoS)
{
    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::PostResourceRepresentation,
                         m_devAddr, m_uri, rep, queryParametersMap,
                         m_headerOptions, CT_DEFAULT, attributeHandler, QoS);
}

OCStackResult OCResource::post(const OCRepresentation& rep,
                              const QueryParamsMap& queryParametersMap, PutCallback attributeHandler)
{
    QualityOfService defaultQos = OC::QualityOfService::NaQos;
    checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetDefaultQos, defaultQos);
    return result_guard(post(rep, queryParametersMap, attributeHandler, defaultQos));
}

OCStackResult OCResource::post(const std::string& resourceType,
                               const std::string& resourceInterface, const OCRepresentation& rep,
                               const QueryParamsMap& queryParametersMap,
                               PostCallback attributeHandler)
{
    QualityOfService defaultQoS = OC::QualityOfService::NaQos;
    checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetDefaultQos, defaultQoS);

    return result_guard(post(resourceType, resourceInterface, rep, queryParametersMap, attributeHandler,
            defaultQoS));
}

OCStackResult OCResource::post(const std::string& resourceType,
                               const std::string& resourceInterface, const OCRepresentation& rep,
                               const QueryParamsMap& queryParametersMap,
                               PostCallback attributeHandler,
                               QualityOfService QoS)
{
    QueryParamsMap mapCpy(queryParametersMap);

    if (!resourceType.empty())
    {
        mapCpy[OC::Key::RESOURCETYPESKEY]=resourceType;
    }

    if (!resourceInterface.empty())
    {
        mapCpy[OC::Key::INTERFACESKEY]=resourceInterface;
    }

    return result_guard(post(rep, mapCpy, attributeHandler, QoS));
}

OCStackResult OCResource::deleteResource(DeleteCallback deleteHandler, QualityOfService QoS)
{
    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::DeleteResource,
                         m_devAddr, m_uri, m_headerOptions, CT_DEFAULT, deleteHandler, QoS);
}

OCStackResult OCResource::deleteResource(DeleteCallback deleteHandler)
{
    QualityOfService defaultQos = OC::QualityOfService::NaQos;
    checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetDefaultQos, defaultQos);

    return result_guard(deleteResource(deleteHandler, defaultQos));
}

OCStackResult OCResource::observe(ObserveType observeType,
        const QueryParamsMap& queryParametersMap, ObserveCallback observeHandler,
        QualityOfService QoS)
{
    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::ObserveResource,
                         observeType, &m_observeHandle, m_devAddr,
                         m_uri, queryParametersMap, m_headerOptions,
                         observeHandler, QoS);
}

OCStackResult OCResource::observe(ObserveType observeType,
        const QueryParamsMap& queryParametersMap, ObserveCallback observeHandler)
{
    QualityOfService defaultQoS = OC::QualityOfService::NaQos;
    checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetDefaultQos, defaultQoS);

    return result_guard(observe(observeType, queryParametersMap, observeHandler, defaultQoS));
}

OCStackResult OCResource::cancelObserve()
{
    QualityOfService defaultQoS = OC::QualityOfService::NaQos;
    checked_guard(m_clientWrapper.lock(), &IClientWrapper::GetDefaultQos, defaultQoS);
    return result_guard(cancelObserve(defaultQoS));
}

OCStackResult OCResource::cancelObserve(QualityOfService QoS)
{
    if (m_observeHandle == nullptr)
    {
        return result_guard(OC_STACK_INVALID_PARAM);
    }

    OCStackResult result =  checked_guard(m_clientWrapper.lock(),
            &IClientWrapper::CancelObserveResource,
            m_observeHandle, (const char*)"", m_uri, m_headerOptions, QoS);

    if (result == OC_STACK_OK)
    {
        m_observeHandle = nullptr;
    }

    return result;
}

void OCResource::setHeaderOptions(const HeaderOptions& headerOptions)
{
    m_headerOptions = headerOptions;
}

void OCResource::unsetHeaderOptions()
{
    m_headerOptions.clear();
}

std::string OCResource::host() const
{
    std::ostringstream ss;

    if (m_devAddr.adapter & OC_ADAPTER_TCP)
    {
        if (m_devAddr.flags & OC_SECURE)
        {
            ss << COAPS_TCP;
        }
        else
        {
            ss << COAP_TCP;
        }
    }
    else if (m_devAddr.adapter & OC_ADAPTER_GATT_BTLE)
    {
        ss << COAP_GATT;
    }
    else if (m_devAddr.adapter & OC_ADAPTER_RFCOMM_BTEDR)
    {
        ss << COAP_RFCOMM;
    }
    else
    {
        if (m_devAddr.flags & OC_SECURE)
        {
            ss << COAPS;
        }
        else
        {
            ss << COAP;
        }
    }

    if (m_devAddr.flags & OC_IP_USE_V6)
    {
        char addressEncoded[128] = {0};

        OCStackResult result = OCEncodeAddressForRFC6874(addressEncoded,
                                                         sizeof(addressEncoded),
                                                         m_devAddr.addr);
        if (OC_STACK_OK != result)
        {
            throw ResourceInitException(m_uri.empty(), m_resourceTypes.empty(),
                m_interfaces.empty(), m_clientWrapper.expired(), false, false);
        }
        ss << '[' << addressEncoded << ']';
    }
    else
    {
        ss << m_devAddr.addr;
    }
    if (m_devAddr.port)
    {
        ss << ':' << m_devAddr.port;
    }
    return ss.str();
}

std::vector<std::string> OCResource::getAllHosts() const
{
    return m_endpoints;
}

HeaderOptions OCResource::getServerHeaderOptions() const
{
    return m_serverHeaderOptions;
}

std::string OCResource::uri() const
{
    return m_uri;
}

OCConnectivityType OCResource::connectivityType() const
{
    return static_cast<OCConnectivityType>(
           (m_devAddr.adapter << CT_ADAPTER_SHIFT) | (m_devAddr.flags & CT_MASK_FLAGS));
}

bool OCResource::isObservable() const
{
    return (m_property & OC_OBSERVABLE) == OC_OBSERVABLE;
}

#ifdef WITH_MQ
bool OCResource::isPublish() const
{
    return (m_property & OC_MQ_PUBLISHER) == OC_MQ_PUBLISHER;
}
#endif

std::vector<std::string> OCResource::getResourceTypes() const
{
    return m_resourceTypes;
}

std::vector<std::string> OCResource::getResourceInterfaces(void) const
{
    return m_interfaces;
}

OCResourceIdentifier OCResource::uniqueIdentifier() const
{
    return m_resourceId;
}

std::string OCResource::sid() const
{
    return this->uniqueIdentifier().m_representation;
}

#ifdef WITH_MQ
OCStackResult OCResource::discoveryMQTopics(const QueryParamsMap& queryParametersMap,
                                            MQTopicCallback attributeHandler,
                                            QualityOfService qos)
{
    return checked_guard(m_clientWrapper.lock(),
                            &IClientWrapper::ListenForMQTopic,
                            m_devAddr, m_uri,
                            queryParametersMap, m_headerOptions,
                            attributeHandler, qos);
}

OCStackResult OCResource::createMQTopic(const OCRepresentation& rep,
                                        const std::string& topicUri,
                                        const QueryParamsMap& queryParametersMap,
                                        MQTopicCallback attributeHandler,
                                        QualityOfService qos)
{
    return checked_guard(m_clientWrapper.lock(), &IClientWrapper::PutMQTopicRepresentation,
                         m_devAddr, topicUri, rep, queryParametersMap,
                         m_headerOptions, attributeHandler, qos);
}
#endif
#ifdef MQ_SUBSCRIBER
OCStackResult OCResource::subscribeMQTopic(ObserveType observeType,
                                           const QueryParamsMap& queryParametersMap,
                                           ObserveCallback observeHandler,
                                           QualityOfService qos)
{
    return result_guard(observe(observeType, queryParametersMap, observeHandler, qos));
}

OCStackResult OCResource::unsubscribeMQTopic(QualityOfService qos)
{
    return result_guard(cancelObserve(qos));
}

OCStackResult OCResource::requestMQPublish(const QueryParamsMap& queryParametersMap,
                                           PostCallback attributeHandler,
                                           QualityOfService qos)
{
    OCRepresentation rep;
    rep.setValue(std::string("req_pub"), std::string("true"));
    return result_guard(post(rep, queryParametersMap, attributeHandler, qos));
}
#endif
#ifdef MQ_PUBLISHER
OCStackResult OCResource::publishMQTopic(const OCRepresentation& rep,
                                         const QueryParamsMap& queryParametersMap,
                                         PostCallback attributeHandler,
                                         QualityOfService qos)
{
    return result_guard(post(rep, queryParametersMap, attributeHandler, qos));
}
#endif

bool OCResource::operator==(const OCResource &other) const
{
    return m_resourceId == other.m_resourceId;
}

bool OCResource::operator!=(const OCResource &other) const
{
    return m_resourceId != other.m_resourceId;
}

bool OCResource::operator<(const OCResource &other) const
{
    return m_resourceId < other.m_resourceId;
}

bool OCResource::operator>(const OCResource &other) const
{
    return m_resourceId > other.m_resourceId;
}

bool OCResource::operator<=(const OCResource &other) const
{
    return m_resourceId <= other.m_resourceId;
}

bool OCResource::operator>=(const OCResource &other) const
{
    return m_resourceId >= other.m_resourceId;
}

OCResourceIdentifier::OCResourceIdentifier(const std::string& wireServerIdentifier,
        const std::string& resourceUri)
    :m_representation(wireServerIdentifier), m_resourceUri(resourceUri)
{
}

std::ostream& operator <<(std::ostream& os, const OCResourceIdentifier& ri)
{
    os << ri.m_representation<<ri.m_resourceUri;

    return os;
}

bool OCResourceIdentifier::operator==(const OCResourceIdentifier &other) const
{
    return m_representation == other.m_representation
        && m_resourceUri == other.m_resourceUri;
}

bool OCResourceIdentifier::operator!=(const OCResourceIdentifier &other) const
{
    return !(*this == other);
}

bool OCResourceIdentifier::operator<(const OCResourceIdentifier &other) const
{
    return m_resourceUri < other.m_resourceUri
        || (m_resourceUri == other.m_resourceUri &&
                m_representation < other.m_representation);
}

bool OCResourceIdentifier::operator>(const OCResourceIdentifier &other) const
{
    return *this != other && !(*this<other);
}

bool OCResourceIdentifier::operator<=(const OCResourceIdentifier &other) const
{
    return !(*this > other);
}

bool OCResourceIdentifier::operator>=(const OCResourceIdentifier &other) const
{
    return !(*this < other);
}

} // namespace OC
