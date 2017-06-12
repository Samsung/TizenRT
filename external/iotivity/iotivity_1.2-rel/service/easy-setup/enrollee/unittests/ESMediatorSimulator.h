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

#ifndef _ES_MEDIATOR_SIMULATOR_H_
#define _ES_MEDIATOR_SIMULATOR_H_

#include <iostream>

#include "OCPlatform.h"
#include "OCApi.h"
#include "oic_malloc.h"

#include "EasySetup.hpp"
#include "ESRichCommon.h"

#include "ocrandom.h"
#include "cainterface.h"
#include "OCPlatform.h"

using namespace OIC::Service;

class ESMediatorSimulator
{
private:
    std::shared_ptr<RemoteEnrollee> m_remoteEnrollee;
    std::function<void(std::shared_ptr<OC::OCResource> resource)> m_discoveryCb;
    std::function<void(std::shared_ptr< GetConfigurationStatus > status)> m_getConfigurationCb;
    std::function<void(std::shared_ptr< GetEnrolleeStatus >)> m_getStatusCb;
    std::function<void(std::shared_ptr< DevicePropProvisioningStatus >)> m_DevicePropProvisioningCb;
    std::function<void(std::shared_ptr< CloudPropProvisioningStatus >)> m_CloudPropProvisioningCb;
    std::function<void(const OCRepresentation&)> m_getWifiCb;
    std::function<void(const OCRepresentation&)> m_getCloudCb;
    std::function<void(const OCRepresentation&)> m_getDevConfCb;
    std::function<void(const OCRepresentation&)> m_provPutCb;

public:
    ESMediatorSimulator()
    : m_remoteEnrollee(), m_discoveryCb(), m_getConfigurationCb(), m_getStatusCb(),
    m_DevicePropProvisioningCb(), m_CloudPropProvisioningCb(),
    m_getWifiCb(), m_getCloudCb(), m_getDevConfCb(), m_provPutCb()
    {
    };
    ~ESMediatorSimulator() = default;

    ESMediatorSimulator(const ESMediatorSimulator &) = delete;
    ESMediatorSimulator & operator = (const ESMediatorSimulator &) = delete;

    ESMediatorSimulator(ESMediatorSimulator &&) = delete;
    ESMediatorSimulator & operator = (ESMediatorSimulator &&) = delete;

    void discoverRemoteEnrollee(std::function<void(std::shared_ptr<OC::OCResource> resource)> cb)
    {
        m_discoveryCb = cb;
        std::string uri = std::string("/oic/res?rt=") + OC_RSRVD_ES_RES_TYPE_EASYSETUP;
        OC::OCPlatform::findResource("", uri,
                OCConnectivityType::CT_DEFAULT,
                std::bind(&ESMediatorSimulator::discoverRemoteEnrolleeCb,
                                                                    this, std::placeholders::_1));

    }

    void getConfiguration(std::function<void(std::shared_ptr< GetConfigurationStatus >)> cb)
    {
        m_getConfigurationCb = cb;
        m_remoteEnrollee = NULL;
        std::string uri = std::string("/oic/res?rt=") + OC_RSRVD_ES_RES_TYPE_EASYSETUP;
        OC::OCPlatform::findResource("", uri,
                OCConnectivityType::CT_DEFAULT,
                std::bind(&ESMediatorSimulator::discoverRemoteEnrolleeCbToGetConfiguration,
                                                                    this, std::placeholders::_1));
    }

    void getWifiRsrc(std::function<void(const OCRepresentation& rep)> cb)
    {
        m_getWifiCb = cb;
        std::string uri = std::string("/oic/res?rt=") + OC_RSRVD_ES_RES_TYPE_WIFICONF;
        OC::OCPlatform::findResource("", uri,
                OCConnectivityType::CT_DEFAULT,
                std::bind(&ESMediatorSimulator::discoverRemoteEnrolleeCbToGetWifiRsrc,
                                                                    this, std::placeholders::_1));
    }

    void getCloudRsrc(std::function<void(const OCRepresentation& rep)> cb)
    {
        m_getCloudCb = cb;
        std::string uri = std::string("/oic/res?rt=") + OC_RSRVD_ES_RES_TYPE_COAPCLOUDCONF;
        OC::OCPlatform::findResource("", uri,
                OCConnectivityType::CT_DEFAULT,
                std::bind(&ESMediatorSimulator::discoverRemoteEnrolleeCbToGetCloudRsrc,
                                                                    this, std::placeholders::_1));
    }

    void getDevConfiguration(std::function<void(const OCRepresentation& rep)> cb)
    {
        m_getDevConfCb = cb;
        std::string uri = std::string("/oic/res?rt=") + OC_RSRVD_ES_RES_TYPE_DEVCONF;
        OC::OCPlatform::findResource("", uri,
                OCConnectivityType::CT_DEFAULT,
                std::bind(&ESMediatorSimulator::discoverRemoteEnrolleeCbToGetDevConf,
                                                                    this, std::placeholders::_1));
    }

    void getStatus(std::function<void(std::shared_ptr< GetEnrolleeStatus >)> cb)
    {
        m_getStatusCb = cb;
        m_remoteEnrollee = NULL;
        std::string uri = std::string("/oic/res?rt=") + OC_RSRVD_ES_RES_TYPE_EASYSETUP;
        OC::OCPlatform::findResource("", uri,
                OCConnectivityType::CT_DEFAULT,
                std::bind(&ESMediatorSimulator::discoverRemoteEnrolleeCbToGetStatus,
                                                                    this, std::placeholders::_1));
    }

    void provisionDeviceProperties(std::function<void(std::shared_ptr< DevicePropProvisioningStatus >)> cb)
    {
        m_DevicePropProvisioningCb = cb;
        m_remoteEnrollee = NULL;
        std::string uri = std::string("/oic/res?rt=") + OC_RSRVD_ES_RES_TYPE_EASYSETUP;
        OC::OCPlatform::findResource("", uri,
                OCConnectivityType::CT_DEFAULT,
                std::bind(&ESMediatorSimulator::discoverRemoteEnrolleeCbToProvisionDeviceProperties,
                                                                    this, std::placeholders::_1));
    }

    void provisionCloudProperties(std::function<void(std::shared_ptr< CloudPropProvisioningStatus >)> cb)
    {
        m_CloudPropProvisioningCb = cb;
        m_remoteEnrollee = NULL;
        std::string uri = std::string("/oic/res?rt=") + OC_RSRVD_ES_RES_TYPE_EASYSETUP;
        OC::OCPlatform::findResource("", uri,
                OCConnectivityType::CT_DEFAULT,
                std::bind(&ESMediatorSimulator::discoverRemoteEnrolleeCbToProvisionCloudProperties,
                                                                    this, std::placeholders::_1));
    }

    void putProvRsrc(std::function<void(const OCRepresentation& rep)> cb)
    {
        m_provPutCb = cb;
        m_remoteEnrollee = NULL;
        std::string uri = std::string("/oic/res?rt=") + OC_RSRVD_ES_RES_TYPE_EASYSETUP;
        OC::OCPlatform::findResource("", uri,
                OCConnectivityType::CT_DEFAULT,
                std::bind(&ESMediatorSimulator::discoverRemoteEnrolleeCbToPutProvRsrc,
                                                                    this, std::placeholders::_1));
    }

private:
    bool isValidResourceToTest(std::shared_ptr<OC::OCResource> resource)
    {
        if((resource->connectivityType() & CT_ADAPTER_TCP) == CT_ADAPTER_TCP)
        {
            return false;
        }

        CAEndpoint_t *tempInfo = NULL;
        uint32_t tempSize = 0;

        CAResult_t res = CAGetNetworkInformation(&tempInfo, &tempSize);
        if (CA_STATUS_OK != res || NULL == tempInfo || 0 >= tempSize)
        {
            free(tempInfo);
            return false;
        }

        for (uint32_t index = 0; index  < tempSize; index++)
        {
            if (CA_ADAPTER_IP == tempInfo[index].adapter)
            {
                if(resource->host().find(tempInfo[index].addr) != std::string::npos &&
                    resource->host().find(std::to_string(tempInfo[index].port).c_str()) != std::string::npos)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void discoverRemoteEnrolleeCb(std::shared_ptr<OC::OCResource> resource)
    {
        if(!isValidResourceToTest(resource))
        {
            return ;
        }

        if(!resource->getResourceTypes().at(0).compare(OC_RSRVD_ES_RES_TYPE_EASYSETUP) && m_discoveryCb)
        {
            m_discoveryCb(resource);
            m_discoveryCb = NULL;
        }
    }

    void getConfigurationCallback(std::shared_ptr< GetConfigurationStatus > getConfigurationStatus)
    {
        if(m_getConfigurationCb != NULL)
        {
            m_getConfigurationCb(getConfigurationStatus);
            m_getConfigurationCb = NULL;
        }
    }

    void discoverRemoteEnrolleeCbToGetConfiguration(std::shared_ptr<OC::OCResource> resource)
    {
        if(!isValidResourceToTest(resource))
        {
            return ;
        }

        if(!resource->getResourceTypes().at(0).compare(OC_RSRVD_ES_RES_TYPE_EASYSETUP) && m_getConfigurationCb
                                                                           && !m_remoteEnrollee)
        {
            m_remoteEnrollee = EasySetup::getInstance()->createRemoteEnrollee(resource);

            if(m_remoteEnrollee != NULL)
            {
              m_remoteEnrollee->getConfiguration(std::bind(
                    &ESMediatorSimulator::getConfigurationCallback, this, std::placeholders::_1));
            }
        }
    }

    void getWifiRsrcCallback(const HeaderOptions& , const OCRepresentation& rep, const int eCode)
    {
        try
        {
            if(eCode == OC_STACK_OK)
            {
                std::cout << "GET request was successful" << std::endl;
                std::cout << "Resource URI: " << rep.getUri() << std::endl;
                if(m_getWifiCb != NULL)
                {
                    m_getWifiCb(rep);
                    m_getWifiCb = NULL;
                }
            }
            else
            {
                std::cout << "onGET Response error: " << eCode << std::endl;
                std::exit(-1);
            }
        }
        catch(std::exception& e)
        {
            std::cout << "Exception: " << e.what() << " in onGet" << std::endl;
        }
    }

    void discoverRemoteEnrolleeCbToGetWifiRsrc(std::shared_ptr<OC::OCResource> resource)
    {

        if(!isValidResourceToTest(resource))
        {
            return ;
        }

        if(!resource->getResourceTypes().at(0).compare(OC_RSRVD_ES_RES_TYPE_WIFICONF) && m_getWifiCb)
        {
            QueryParamsMap test;
            resource->get(test, std::bind(
                    &ESMediatorSimulator::getWifiRsrcCallback, this, std::placeholders::_1,
                                            std::placeholders::_2, std::placeholders::_3));
        }
    }

    void getCloudRsrcCallback(const HeaderOptions& , const OCRepresentation& rep, const int eCode)
    {
        try
        {
            if(eCode == OC_STACK_OK)
            {
                std::cout << "GET request was successful" << std::endl;
                std::cout << "Resource URI: " << rep.getUri() << std::endl;
                if(m_getCloudCb != NULL)
                {
                    m_getCloudCb(rep);
                    m_getCloudCb = NULL;
                }
            }
            else
            {
                std::cout << "onGET Response error: " << eCode << std::endl;
                std::exit(-1);
            }
        }
        catch(std::exception& e)
        {
            std::cout << "Exception: " << e.what() << " in onGet" << std::endl;
        }
    }


    void discoverRemoteEnrolleeCbToGetCloudRsrc(std::shared_ptr<OC::OCResource> resource)
    {
        if(!isValidResourceToTest(resource))
        {
            return ;
        }

        if(!resource->getResourceTypes().at(0).compare(OC_RSRVD_ES_RES_TYPE_COAPCLOUDCONF) && m_getCloudCb)
        {
            QueryParamsMap test;
            resource->get(test, std::bind(
                    &ESMediatorSimulator::getCloudRsrcCallback, this, std::placeholders::_1,
                                            std::placeholders::_2, std::placeholders::_3));
        }
    }

    void getDevConfCallback(const HeaderOptions&, const OCRepresentation& rep, const int eCode)
    {
        try
        {
            if(eCode == OC_STACK_OK)
            {
                std::cout << "GET request was successful" << std::endl;
                std::cout << "Resource URI: " << rep.getUri() << std::endl;
                if(m_getDevConfCb != NULL)
                {
                    m_getDevConfCb(rep);
                    m_getDevConfCb = NULL;
                }
            }
            else
            {
                std::cout << "onGET Response error: " << eCode << std::endl;
                std::exit(-1);
            }
        }
        catch(std::exception& e)
        {
            std::cout << "Exception: " << e.what() << " in onGet" << std::endl;
        }
    }

    void discoverRemoteEnrolleeCbToGetDevConf(std::shared_ptr<OC::OCResource> resource)
    {
        if(!isValidResourceToTest(resource))
        {
            return ;
        }

        if(!resource->getResourceTypes().at(0).compare(OC_RSRVD_ES_RES_TYPE_DEVCONF) && m_getDevConfCb)
        {
            QueryParamsMap test;
            resource->get(test, std::bind(
                    &ESMediatorSimulator::getDevConfCallback, this, std::placeholders::_1,
                                            std::placeholders::_2, std::placeholders::_3));
        }
    }

    void getStatusCallback(std::shared_ptr< GetEnrolleeStatus > getEnrolleeStatus)
    {
        if(m_getStatusCb != NULL)
        {
            m_getStatusCb(getEnrolleeStatus);
            m_getStatusCb = NULL;
        }
    }

    void discoverRemoteEnrolleeCbToGetStatus(std::shared_ptr<OC::OCResource> resource)
    {
        if(!isValidResourceToTest(resource))
        {
            return ;
        }

        if(!resource->getResourceTypes().at(0).compare(OC_RSRVD_ES_RES_TYPE_EASYSETUP) && m_getStatusCb
                                                                            && !m_remoteEnrollee)
        {
            m_remoteEnrollee = EasySetup::getInstance()->createRemoteEnrollee(resource);

            if(m_remoteEnrollee != NULL)
            {
                m_remoteEnrollee->getStatus(std::bind(
                    &ESMediatorSimulator::getStatusCallback, this, std::placeholders::_1));
            }
        }
    }

    void deviceProvisioningStatusCallback(std::shared_ptr< DevicePropProvisioningStatus >
                                                                    devicePropProvisioningStatus)
    {
        if(m_DevicePropProvisioningCb != NULL)
        {
            m_DevicePropProvisioningCb(devicePropProvisioningStatus);
            m_DevicePropProvisioningCb = NULL;
        }
    }

    void discoverRemoteEnrolleeCbToProvisionDeviceProperties(std::shared_ptr<OC::OCResource> resource)
    {
        if(!isValidResourceToTest(resource))
        {
            return ;
        }

        if(!resource->getResourceTypes().at(0).compare(OC_RSRVD_ES_RES_TYPE_EASYSETUP) &&
                                                m_DevicePropProvisioningCb && !m_remoteEnrollee)
        {
            m_remoteEnrollee = EasySetup::getInstance()->createRemoteEnrollee(resource);

            if(m_remoteEnrollee != NULL)
            {
                DeviceProp devProp;
                devProp.setWiFiProp("Iotivity_SSID", "Iotivity_PWD", WPA2_PSK, TKIP_AES);
                devProp.setDevConfProp("korean", "Korea", "Location");

                m_remoteEnrollee->provisionDeviceProperties(devProp,
                    std::bind(&ESMediatorSimulator::deviceProvisioningStatusCallback,
                                                            this, std::placeholders::_1));
            }
        }
    }

    void cloudProvisioningStatusCallback(std::shared_ptr< CloudPropProvisioningStatus >
                                                                    cloudPropProvisioningStatus)
    {
        if(m_CloudPropProvisioningCb != NULL)
        {
            m_CloudPropProvisioningCb(cloudPropProvisioningStatus);
            if(cloudPropProvisioningStatus->getESResult() == ES_OK)
            {
                m_CloudPropProvisioningCb = NULL;
            }
        }
    }

    void discoverRemoteEnrolleeCbToProvisionCloudProperties(std::shared_ptr<OC::OCResource> resource)
    {
        if(!isValidResourceToTest(resource))
        {
            return ;
        }

        if(!resource->getResourceTypes().at(0).compare(OC_RSRVD_ES_RES_TYPE_EASYSETUP) &&
                                                m_CloudPropProvisioningCb && !m_remoteEnrollee)
        {
            m_remoteEnrollee = EasySetup::getInstance()->createRemoteEnrollee(resource);

            if(m_remoteEnrollee != NULL)
            {
                CloudProp cloudProp;
                cloudProp.setCloudProp("authCode", "authProvider", "ciServer");

                m_remoteEnrollee->provisionCloudProperties(cloudProp,
                    std::bind(&ESMediatorSimulator::cloudProvisioningStatusCallback,
                                                            this, std::placeholders::_1));
            }
        }
    }

    void putProvRsrcCallabck(const HeaderOptions&, const OCRepresentation& rep, const int )
    {
        cout << "putProvRsrcCallback is called" << endl;

        if(m_provPutCb != NULL){
            m_provPutCb(rep);
            m_provPutCb = NULL;
        }

    }

    void discoverRemoteEnrolleeCbToPutProvRsrc(std::shared_ptr<OC::OCResource> resource)
    {
        m_remoteEnrollee = EasySetup::getInstance()->createRemoteEnrollee(resource);

        if(m_remoteEnrollee != NULL)
        {
            if(m_provPutCb)
            {
                OCRepresentation rep;
                QueryParamsMap test;
                resource->put(rep, test, std::bind(
                        &ESMediatorSimulator::putProvRsrcCallabck, this, std::placeholders::_1,
                                                std::placeholders::_2, std::placeholders::_3));
                }

        }

    }

};


#endif //_NS_CONSUMER_SIMULATOR_H_

