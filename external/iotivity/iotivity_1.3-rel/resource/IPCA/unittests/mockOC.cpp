/* *****************************************************************
 *
 * Copyright 2017 Microsoft
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

#include "OCPlatform_impl.h"
#include <OCPlatform.h>
#include <OCResource.h>
#include <string>
#include "ipcatestdata.h"

// This file implements subset of functions of the following classes that are required by IPCA unit
// tests.
//   OCResource
//   OCPlatform
//   OCResourceRequest (data structure sent to server app's entity handler)
//   OCResourceReponse (data structure sent to client app's callbacks)

using namespace OC;

typedef struct
{
    std::string deviceName;
    std::vector<std::string> types;
    std::string specVersion;
    std::string platformIndependentId;
    std::vector<std::string> dataModelVersions;
} Mock_OCDeviceInfo;

typedef struct
{
    std::string platformID;
    std::string manufacturerName;
    std::string manufacturerUrl;
    std::string modelNumber;
    std::string dateOfManufacture;
    std::string platformVersion;
    std::string operatingSystemVersion;
    std::string hardwareVersion;
    std::string firmwareVersion;
    std::string supportUrl;
    std::string systemTime;
} Mock_OCPlatformInfo;

class MockOCResource
{
   public:
       typedef std::shared_ptr<MockOCResource> Ptr;
       MockOCResource(const std::string& uri,
                      const std::string& resourceTypeName,
                      const std::string& resourceInterface,
                      EntityHandler entityHandler,
                      uint8_t resourceProperty,
                      std::shared_ptr<OCResource> ocResource) :
           m_uri(uri),
           m_resourceType(resourceTypeName),
           m_resourceInterface(resourceInterface),
           m_entityHandler(entityHandler),
           m_resourceProperty(resourceProperty),
           m_ocResource(ocResource)
       {
       };
       std::string m_uri;
       std::string m_resourceType;
       std::string m_resourceInterface;
       EntityHandler m_entityHandler;
       uint8_t m_resourceProperty;
       OCResource::Ptr m_ocResource;
};

size_t g_requestId = 0; // incremented per request.
OCObservationId  g_observationId = 0;  // incremented per observe request.
struct PendingRequest
{
    typedef std::shared_ptr<PendingRequest> Ptr;
    size_t requestNumber;
    MockOCResource::Ptr mockOCResource;
    OCMethod method;
    PutCallback postCallback;
    GetCallback getCallback;
    ObserveCallback observeCallback;
    OCObservationId observationId;
    DeleteCallback deleteCallback;
};

PlatformConfig g_platformConfig;
Mock_OCDeviceInfo g_deviceInfo; // C++ version of OCDeviceInfo.
Mock_OCPlatformInfo g_platformInfo; // C++ version of OCPlatformInfo.

std::recursive_mutex g_globalMutex; // the only lock.
std::map<std::string, MockOCResource::Ptr> g_resourceList;   // Key is resource uri.
std::map<size_t, PendingRequest::Ptr> g_requestList;    // Key is PendingRequest.requestNumber.

std::string g_mockHostAddress = "fe80::1%eth0";
uint16_t g_mockHostPort = 10000;
std::string g_mockCompleteAddress = "coap://[fe80::1%25eth0]:10000";
void SetMockOCDevAddr(OCDevAddr& addr)
{
    memset (&addr, 0, sizeof(addr));
    addr.adapter = OC_DEFAULT_ADAPTER;
    addr.flags= OC_IP_USE_V6;
    addr.port = g_mockHostPort;
    strcpy(addr.addr, g_mockHostAddress.c_str());
}

// Forward decl.
OCEntityHandlerResult MockEntityHandler(OCEntityHandlerFlag flag,
                            OCMethod method,
                            bool isCancelObserve,
                            const OCRepresentation& rep,
                            const QueryParamsMap& queryParametersMap,
                            PostCallback postCallback,
                            GetCallback getCallback,
                            ObserveCallback observeCallback,
                            DeleteCallback deleteCallback,
                            const std::string& uri);

// Called to configure platform config.
void OCPlatform::Configure(const PlatformConfig& config)
{
    g_platformConfig = config;
}

OCStackResult OCPlatform::start()
{
    return OC_STACK_OK;
}

OCStackResult OCPlatform::stop()
{
    return OC_STACK_OK;
}

OCStackResult OCPlatform::setPropertyValue(OCPayloadType type, const std::string& tag,
                                       const std::string& value)
{
    if ((type == PAYLOAD_TYPE_DEVICE) && (tag.compare(OC_RSRVD_PROTOCOL_INDEPENDENT_ID) == 0))
    {
        g_deviceInfo.platformIndependentId = value;
    }

    return OC_STACK_OK;
}

OCStackResult OCPlatform::setPropertyValue(OCPayloadType type, const std::string& tag,
                               const std::vector<std::string>& value)
{
    if ((type == PAYLOAD_TYPE_DEVICE) && (tag.compare(OC_RSRVD_DATA_MODEL_VERSION) == 0))
    {
        g_deviceInfo.dataModelVersions = value;
    }

    return OC_STACK_OK;
}

OCStackResult OCPlatform::getPropertyValue(OCPayloadType type, const std::string& tag,
                               std::string& value)
{
    if ((type == PAYLOAD_TYPE_DEVICE) && (tag.compare(OC_RSRVD_PROTOCOL_INDEPENDENT_ID) == 0))
    {
        value = g_deviceInfo.platformIndependentId;
    }

    return OC_STACK_OK;
}

OCStackResult OCPlatform::getPropertyValue(OCPayloadType type, const std::string& tag,
                               std::vector<std::string>& value)
{
    if ((type == PAYLOAD_TYPE_DEVICE) && (tag.compare(OC_RSRVD_DATA_MODEL_VERSION) == 0))
    {
        value = g_deviceInfo.dataModelVersions;
    }

    return OC_STACK_OK;
}

// Called by server app to register device info.
OCStackResult OCPlatform::registerDeviceInfo(const OCDeviceInfo deviceInfo)
{
    g_deviceInfo.deviceName = deviceInfo.deviceName;
    g_deviceInfo.specVersion = deviceInfo.specVersion;

    if (deviceInfo.types)
    {
        for (OCStringLL* temp = deviceInfo.types; temp; temp = temp->next)
        {
            if (temp->value)
            {
                g_deviceInfo.types.push_back(temp->value);
            }
        }
    }

    if (deviceInfo.dataModelVersions)
    {
        for (OCStringLL* temp = deviceInfo.dataModelVersions; temp; temp = temp->next)
        {
            if (temp->value)
            {
                g_deviceInfo.dataModelVersions.push_back(temp->value);
            }
        }
    }

    return OC_STACK_OK;
}

// Called by server app to register platform info.
OCStackResult OCPlatform::registerPlatformInfo(const OCPlatformInfo platformInfo)
{
    g_platformInfo.platformID = platformInfo.platformID;
    g_platformInfo.manufacturerName = platformInfo.manufacturerName;
    g_platformInfo.manufacturerUrl = platformInfo.manufacturerUrl;
    g_platformInfo.modelNumber = platformInfo.modelNumber;
    g_platformInfo.dateOfManufacture = platformInfo.dateOfManufacture;
    g_platformInfo.platformVersion = platformInfo.platformVersion;
    g_platformInfo.operatingSystemVersion = platformInfo.operatingSystemVersion;
    g_platformInfo.hardwareVersion = platformInfo.hardwareVersion;
    g_platformInfo.firmwareVersion = platformInfo.firmwareVersion;
    g_platformInfo.supportUrl = platformInfo.supportUrl;
    g_platformInfo.systemTime =  platformInfo.systemTime ? platformInfo.systemTime : "";
    return OC_STACK_OK;
}

// Called by server app to register resource. This mock creates an OCResource that can be used by
// client app.
// MockOCResource keeps track of OC::OCResource and callbacks to the server app.
OCStackResult OCPlatform::registerResource(
                                     OCResourceHandle& resourceHandle,
                                     std::string& resourceURI,
                                     const std::string& resourceTypeName,
                                     const std::string& resourceInterface,
                                     EntityHandler entityHandler,
                                     uint8_t resourceProperty)
{
    std::lock_guard<std::recursive_mutex> lock(g_globalMutex);

    if (g_resourceList.find(resourceURI) != g_resourceList.end())
    {
        // duplicate resource.
        return OC_STACK_ERROR;
    }

    OCConnectivityType connectivityType = CT_DEFAULT;
    std::vector<std::string> types = { resourceTypeName };
    std::vector<std::string> ifaces = { resourceInterface };
    OCResource::Ptr ocResource = OCPlatform::constructResourceObject(
                                                "", // host address is mocked in SetHost().
                                                resourceURI,
                                                connectivityType,
                                                (resourceProperty & OC_OBSERVABLE) ? true : false,
                                                types,
                                                ifaces);

    std::shared_ptr<MockOCResource> newMockResource = std::shared_ptr<MockOCResource>(
                                                                new MockOCResource(
                                                                        resourceURI,
                                                                        resourceTypeName,
                                                                        resourceInterface,
                                                                        entityHandler,
                                                                        resourceProperty,
                                                                        ocResource));
    if (newMockResource == nullptr)
    {
        return OC_STACK_ERROR;
    }

    g_resourceList[resourceURI] = newMockResource;
    resourceHandle = reinterpret_cast<OCResourceHandle>(newMockResource.get());
    return OC_STACK_OK;
}

// Called by server app to shut down a resource.
// This also deletes the MockOCResource.
OCStackResult OCPlatform::unregisterResource(const OCResourceHandle& resourceHandle)
{
    std::lock_guard<std::recursive_mutex> lock(g_globalMutex);

    MockOCResource* rawMockOCResource = reinterpret_cast<MockOCResource*>(resourceHandle);

    for (auto mockResource : g_resourceList)
    {
        if (mockResource.second.get() == rawMockOCResource)
        {
            g_resourceList.erase(mockResource.second->m_uri);
            break;
        }
    }

    return OC_STACK_OK;
}

// Called by client app to discover a resource.
OCStackResult OCPlatform::findResource(const std::string& host,
                         const std::string& resourceName,
                         OCConnectivityType connectivityType,
                         FindCallback resourceHandler)
{
    OC_UNUSED(connectivityType);
    OC_UNUSED(resourceName);
    OC_UNUSED(host);

    // Ideally: return only requested resource types embedded in resourceName.
    // This mock fires back everything that server app registered.
    std::lock_guard<std::recursive_mutex> lock(g_globalMutex);
    for (auto mockResource : g_resourceList)
    {
        std::thread findResourceCallbackThread(resourceHandler, mockResource.second->m_ocResource);
        findResourceCallbackThread.detach();
    }

    return OC_STACK_OK;
}

// Called by client app to get device info of device.
OCStackResult OCPlatform::getDeviceInfo(const std::string& host,
                             const std::string& deviceURI,
                             OCConnectivityType connectivityType,
                             FindDeviceCallback deviceInfoHandler)
{
    OC_UNUSED(host);
    OC_UNUSED(connectivityType);
    OC_UNUSED(deviceURI);

    OCDevAddr addr;
    SetMockOCDevAddr(addr);

    OCRepresentation ocRep;
    ocRep.setDevAddr(addr);
    ocRep.setValue(OC_RSRVD_DEVICE_NAME, g_deviceInfo.deviceName);
    ocRep.setValue(OC_RSRVD_SPEC_VERSION, g_deviceInfo.specVersion);
    ocRep.setValue(OC_RSRVD_PROTOCOL_INDEPENDENT_ID, g_deviceInfo.platformIndependentId);

    //  DMV is returned in CSV.
    std::ostringstream outputStream;
    bool firstEntry = true;
    for (auto dataModelVersion : g_deviceInfo.dataModelVersions)
    {
        if (!firstEntry)
        {
            outputStream << ',' << dataModelVersion;
        }
        else
        {
            outputStream << dataModelVersion;
            firstEntry = false;
        }
    }

    ocRep.setValue(OC_RSRVD_DATA_MODEL_VERSION, outputStream.str());

    std::thread getDeviceInfoCallbackThread(deviceInfoHandler, ocRep);
    getDeviceInfoCallbackThread.detach();
    return OC_STACK_OK;
}

// Called by client app to get platform info of device.
OCStackResult OCPlatform::getPlatformInfo(const std::string& host,
                             const std::string& platformURI,
                             OCConnectivityType connectivityType,
                             FindPlatformCallback platformInfoHandler)
{
    OC_UNUSED(host);
    OC_UNUSED(platformURI);
    OC_UNUSED(connectivityType);

    OCDevAddr addr;
    SetMockOCDevAddr(addr);

    OCRepresentation ocRep;
    ocRep.setDevAddr(addr);
    ocRep.setValue(OC_RSRVD_PLATFORM_ID, g_platformInfo.platformID);
    ocRep.setValue(OC_RSRVD_MFG_NAME, g_platformInfo.manufacturerName);
    ocRep.setValue(OC_RSRVD_MFG_URL, g_platformInfo.manufacturerUrl);
    ocRep.setValue(OC_RSRVD_MODEL_NUM, g_platformInfo.modelNumber);
    ocRep.setValue(OC_RSRVD_MFG_DATE, g_platformInfo.dateOfManufacture);
    ocRep.setValue(OC_RSRVD_PLATFORM_VERSION, g_platformInfo.platformVersion);
    ocRep.setValue(OC_RSRVD_OS_VERSION, g_platformInfo.operatingSystemVersion);
    ocRep.setValue(OC_RSRVD_HARDWARE_VERSION, g_platformInfo.hardwareVersion);
    ocRep.setValue(OC_RSRVD_FIRMWARE_VERSION, g_platformInfo.firmwareVersion);
    ocRep.setValue(OC_RSRVD_SUPPORT_URL, g_platformInfo.supportUrl);
    ocRep.setValue(OC_RSRVD_SYSTEM_TIME, g_platformInfo.systemTime);

    std::thread getPlatformInfoCallbackThread(platformInfoHandler, ocRep);
    getPlatformInfoCallbackThread.detach();
    return OC_STACK_OK;

}

// Called by server app to send notification to a list of observers that have called ObserveResource().
OCStackResult OCPlatform::notifyListOfObservers(OCResourceHandle resourceHandle,
                             ObservationIds& observationIds,
                             const std::shared_ptr<OCResourceResponse> pResponse)
{
    OC_UNUSED(resourceHandle);

    OCRepresentation ocRep = pResponse->getResourceRepresentation();
    OCEntityHandlerResult result = pResponse->getResponseResult();
    HeaderOptions serverHeaderOptions; /* not mocked */
    ObservationIds localCopy = observationIds;

    {
        std::lock_guard<std::recursive_mutex> lock(g_globalMutex);
        for (auto targetObserveId : localCopy)
        {
            for (auto request : g_requestList)
            {
                PendingRequest::Ptr pendingRequest = request.second;
                if ((pendingRequest->method == OC_REST_OBSERVE) ||
                    (pendingRequest->method == OC_REST_OBSERVE_ALL))
                {
                    if (pendingRequest->observationId == targetObserveId)
                    {
                        std::thread observeCallbackThread(pendingRequest->observeCallback,
                                        serverHeaderOptions,
                                        ocRep,
                                        (result == OC_EH_OK) ? OC_STACK_OK : OC_STACK_ERROR,
                                        1 /* sequence number, not mocked */);
                        observeCallbackThread.detach();
                    }

                }
            }
        }
    }

    return OC_STACK_OK;
}

// Every response from server app to client app is performed by this function.
OCStackResult OCPlatform::sendResponse(const std::shared_ptr<OCResourceResponse> pResponse)
{
    size_t requestNumber = reinterpret_cast<size_t>(pResponse->getRequestHandle());
    OCRepresentation ocRep = pResponse->getResourceRepresentation();
    OCEntityHandlerResult result = pResponse->getResponseResult();
    HeaderOptions serverHeaderOptions;

    PendingRequest::Ptr pendingRequest;
    {
        std::lock_guard<std::recursive_mutex> lock(g_globalMutex);
        if (g_requestList.find(requestNumber) == g_requestList.end())
        {
            return OC_STACK_ERROR;
        }

        pendingRequest = g_requestList[requestNumber];
    }

    switch (pendingRequest->method)
    {
        case OC_REST_POST:
        {
            OCStackResult ocResult = OC_STACK_ERROR;
            if ((result == OC_EH_OK) || (result == OC_EH_RESOURCE_CREATED))
            {
                ocResult = OC_STACK_OK;
            }

            // Add path of new resource in the header option.
            std::string newResourcePath = pResponse->getNewResourceUri();
            if (!newResourcePath.empty())
            {
                // MAX_URI_LENGTH is the URI limit of OCEntityHandlerResponse.resourceUri in
                // inProcServerWrapper.cpp.
                if (newResourcePath.length() > MAX_URI_LENGTH)
                {
                    return OC_STACK_ERROR;
                }

                HeaderOption::OCHeaderOption headerOption(
                                                HeaderOption::LOCATION_PATH_OPTION_ID,
                                                newResourcePath);
                serverHeaderOptions.push_back(headerOption);
            }

            std::thread postCallbackThread(pendingRequest->postCallback,
                            serverHeaderOptions,
                            ocRep,
                            ocResult);
            postCallbackThread.detach();
            break;
        }

        case OC_REST_GET:
        {
            std::thread getCallbackThread(pendingRequest->getCallback,
                            serverHeaderOptions,
                            ocRep,
                            (result == OC_EH_OK) ? OC_STACK_OK : OC_STACK_ERROR);
            getCallbackThread.detach();
            break;
        }

        case OC_REST_DELETE:
        {
            OCStackResult ocResult = OC_STACK_ERROR;
            if ((result == OC_EH_OK) || (result == OC_EH_RESOURCE_DELETED))
            {
                ocResult = OC_STACK_OK;
            }

            std::thread deleteCallbackThread(pendingRequest->deleteCallback,
                            serverHeaderOptions,
                            ocResult);
            deleteCallbackThread.detach();
            break;
        }

        default:
        {
            assert(false);
            return OC_STACK_ERROR;
        }
    }

    // One time request is responded, remove the pending request.
    if (pendingRequest->method & (OC_REST_POST | OC_REST_GET | OC_REST_DELETE))
    {
        std::lock_guard<std::recursive_mutex> lock(g_globalMutex);
        g_requestList.erase(pendingRequest->requestNumber);
    }

    return OC_STACK_OK;
}

// Constructor of OCResource.
OCResource::OCResource(std::weak_ptr<IClientWrapper> clientWrapper,
                    const std::string& host,
                    const std::string& uri,
                    const std::string& serverId,
                    OCConnectivityType connectivityType,
                    uint8_t property,
                    const std::vector<std::string>& resourceTypes,
                    const std::vector<std::string>& interfaces) :
    m_clientWrapper(clientWrapper),
    m_uri(uri),
    m_resourceId(serverId, m_uri),
    m_isCollection(false),
    m_property(property),
    m_resourceTypes(resourceTypes),
    m_interfaces(interfaces),
    m_observeHandle(nullptr)
{
    m_devAddr.adapter = static_cast<OCTransportAdapter>(connectivityType >> CT_ADAPTER_SHIFT);
    m_devAddr.flags = static_cast<OCTransportFlags>(connectivityType & CT_MASK_FLAGS);
    this->setHost(host);
}

OCResource::~OCResource()
{
}

void defaultCallback(const HeaderOptions&, const OCRepresentation&, const int)
{
}

void defaultDeleteCallback(const HeaderOptions&, const int)
{
}

void defaultObserveCallback(const HeaderOptions&, const OCRepresentation&, const int, const int)
{
}

// Client app's request to Delete a resource.
OCStackResult OCResource::deleteResource(DeleteCallback deleteHandler)
{
    OCRepresentation rep;
    QueryParamsMap queryParametersMap;
    OCEntityHandlerResult result = MockEntityHandler(
                                        OC_REQUEST_FLAG,
                                        OC_REST_DELETE,
                                        false,
                                        rep,
                                        queryParametersMap,
                                        &defaultCallback,
                                        &defaultCallback,
                                        &defaultObserveCallback,
                                        deleteHandler,
                                        uri());

    return (result == OC_EH_OK) ? OC_STACK_OK : OC_STACK_ERROR;
}

// Client app's request to Get a resource.
OCStackResult OCResource::get(const QueryParamsMap& queryParametersMap,
                              GetCallback attributeHandler)
{
    OC_UNUSED(queryParametersMap);

    OCRepresentation rep;
    OCEntityHandlerResult result = MockEntityHandler(
                                        OC_REQUEST_FLAG,
                                        OC_REST_GET,
                                        false,
                                        rep,
                                        queryParametersMap,
                                        &defaultCallback,
                                        attributeHandler,
                                        &defaultObserveCallback,
                                        &defaultDeleteCallback,
                                        uri());

    return (result == OC_EH_OK) ? OC_STACK_OK : OC_STACK_ERROR;
}

// Client app's request to Post to a resource.
OCStackResult OCResource::post(const OCRepresentation& rep,
                            const QueryParamsMap& queryParametersMap,
                            PostCallback attributeHandler)
{
    OC_UNUSED(queryParametersMap);

    OCEntityHandlerResult result = MockEntityHandler(
                                        OC_REQUEST_FLAG,
                                        OC_REST_POST,
                                        false,
                                        rep,
                                        queryParametersMap,
                                        attributeHandler,
                                        &defaultCallback,
                                        &defaultObserveCallback,
                                        &defaultDeleteCallback,
                                        uri());

    return (result == OC_EH_OK) ? OC_STACK_OK : OC_STACK_ERROR;
}

// Client app's request to observe a resource.
OCStackResult OCResource::observe(ObserveType observeType,
                            const QueryParamsMap& queryParametersMap,
                            ObserveCallback observeHandler)
{
    OC_UNUSED(queryParametersMap);

    OCRepresentation rep;
    OCEntityHandlerResult result = MockEntityHandler(
                                        OC_OBSERVE_FLAG,
                                        (observeType == ObserveType::Observe) ?
                                            OC_REST_OBSERVE : OC_REST_OBSERVE_ALL,
                                        false,
                                        rep,
                                        queryParametersMap,
                                        &defaultCallback,
                                        &defaultCallback,
                                        observeHandler,
                                        &defaultDeleteCallback,
                                        uri());

    return (result == OC_EH_OK) ? OC_STACK_OK : OC_STACK_ERROR;
}

// Client app's request to cancel observe request.
OCStackResult OCResource::cancelObserve()
{
    OCRepresentation rep;
    QueryParamsMap queryParametersMap;

    OCEntityHandlerResult result = MockEntityHandler(
                                        OC_OBSERVE_FLAG,
                                        OC_REST_OBSERVE,
                                        true,
                                        rep,
                                        queryParametersMap,
                                        &defaultCallback,
                                        &defaultCallback,
                                        &defaultObserveCallback,
                                        &defaultDeleteCallback,
                                        uri());

    return (result == OC_EH_OK) ? OC_STACK_OK : OC_STACK_ERROR;
}

// Is resource observable.
bool OCResource::isObservable() const
{
    return (m_property & OC_OBSERVABLE) == OC_OBSERVABLE;
}

// Host address of the resource.
std::string OCResource::host() const
{
    std::ostringstream ss;
    ss << m_devAddr.addr;
    return ss.str();
}

// Host address of the resource.
std::string OCResource::setHost(const std::string& host)
{
    OC_UNUSED(host);

    g_mockCompleteAddress.copy(m_devAddr.addr, sizeof(m_devAddr.addr));
    m_devAddr.addr[g_mockCompleteAddress.length()] = '\0';
    return host;
}

// Resource types implemented by resource.
std::vector<std::string> OCResource::getResourceTypes() const
{
    return m_resourceTypes;
}

// Resource interfaces implemented by resource.
std::vector<std::string> OCResource::getResourceInterfaces() const
{
    return m_interfaces;
}

// URI of the resource.
std::string OCResource::uri() const
{
    return m_uri;
}

// ID of reosurce.
std::string OCResource::sid() const
{
    return this->uniqueIdentifier().m_representation;
}

// See: OCResourceRequest.h declaration of this function prototype as friend function.
// Function prototype must match that.
void formResourceRequest(OCEntityHandlerFlag flag,
                         OCEntityHandlerRequest* entityHandlerRequest,
                         std::shared_ptr<OCResourceRequest> pRequest)
{
    if(pRequest && entityHandlerRequest)
    {
        pRequest->setRequestHandle(entityHandlerRequest->requestHandle);
        pRequest->setResourceHandle(entityHandlerRequest->resource);
        pRequest->setMessageID(entityHandlerRequest->messageID);
    }

    if(flag & OC_REQUEST_FLAG)
    {
        pRequest->setRequestHandlerFlag(OC::RequestHandlerFlag::RequestFlag);

        if(entityHandlerRequest)
        {
            if(OC_REST_GET == entityHandlerRequest->method)
            {
                pRequest->setRequestType(OC::PlatformCommands::GET);
            }
            else if(OC_REST_PUT == entityHandlerRequest->method)
            {
                pRequest->setRequestType(OC::PlatformCommands::PUT);
                pRequest->setPayload(entityHandlerRequest->payload);
            }
            else if(OC_REST_POST == entityHandlerRequest->method)
            {
                pRequest->setRequestType(OC::PlatformCommands::POST);
                pRequest->setPayload(entityHandlerRequest->payload);
            }
            else if(OC_REST_DELETE == entityHandlerRequest->method)
            {
                pRequest->setRequestType(OC::PlatformCommands::DELETE);
            }
        }
    }

    if(flag & OC_OBSERVE_FLAG)
    {
        pRequest->setRequestHandlerFlag(
                   OC::RequestHandlerFlag::RequestFlag | OC::RequestHandlerFlag::ObserverFlag);

        if(entityHandlerRequest)
        {
            OC::ObservationInfo observationInfo;
            observationInfo.action = (OC::ObserveAction) entityHandlerRequest->obsInfo.action;
            observationInfo.obsId = entityHandlerRequest->obsInfo.obsId;
            pRequest->setObservationInfo(observationInfo);
        }
    }

    QueryParamsMap* queryParams = reinterpret_cast<QueryParamsMap*>(entityHandlerRequest->query);
    pRequest->setQueryParams(*queryParams);

}

// Set payload to the OCResourceRequest sent to the server app.
void OCResourceRequest::setPayload(OCPayload* payload)
{
    MessageContainer info;

    if(payload == nullptr)
    {
        return;
    }

    if(payload->type != PAYLOAD_TYPE_REPRESENTATION)
    {
        throw std::logic_error("Wrong payload type");
        return;
    }

    info.setPayload(payload);

    const std::vector<OCRepresentation>& reps = info.representations();
    if(reps.size() > 0)
    {
        std::vector<OCRepresentation>::const_iterator itr = reps.begin();
        std::vector<OCRepresentation>::const_iterator back = reps.end();
        m_representation = *itr;
        ++itr;

        for(;itr != back; ++itr)
        {
            m_representation.addChild(*itr);
        }
    }
}

OCResourceIdentifier::OCResourceIdentifier(const std::string& wireServerIdentifier,
        const std::string& resourceUri)
    :m_representation(wireServerIdentifier), m_resourceUri(resourceUri)
{
}

OCResourceIdentifier OCResource::uniqueIdentifier() const
{
    return m_resourceId;
}


// Every callback to server app is performed by this function.
OCEntityHandlerResult MockEntityHandler(OCEntityHandlerFlag flag,
                            OCMethod method,
                            bool isCancelObserve,
                            const OCRepresentation& rep,
                            const QueryParamsMap& queryParametersMap,
                            PostCallback postCallback,
                            GetCallback getCallback,
                            ObserveCallback observeCallback,
                            DeleteCallback deleteCallback,
                            const std::string& uri)
{
    OCObservationId observationIdToCancel = 0;
    size_t requestNumber;
    MockOCResource::Ptr mockOCResource = nullptr;
    {
        std::lock_guard<std::recursive_mutex> lock(g_globalMutex);
        if (g_resourceList.find(uri) == g_resourceList.end())
        {
            return OC_EH_ERROR;
        }

        mockOCResource = g_resourceList[uri];
        requestNumber = ++g_requestId;
    }

    auto pendingRequest = std::make_shared<PendingRequest>();
    if (pendingRequest == nullptr)
    {
        return OC_EH_ERROR;
    }

    pendingRequest->requestNumber   = requestNumber;
    pendingRequest->mockOCResource  = mockOCResource;
    pendingRequest->method          = method;
    pendingRequest->postCallback    = postCallback;
    pendingRequest->getCallback     = getCallback;
    pendingRequest->observeCallback = observeCallback;
    pendingRequest->deleteCallback  = deleteCallback;

    if (!isCancelObserve)
    {
        // Store the request for response.
        std::lock_guard<std::recursive_mutex> lock(g_globalMutex);
        g_requestList[requestNumber] = pendingRequest;
    }
    else
    {
        // Observe request is canceled. Remove the request for observe.
        std::lock_guard<std::recursive_mutex> lock(g_globalMutex);
        for (auto request : g_requestList)
        {
            PendingRequest::Ptr localPendingRequest = request.second;
            if (localPendingRequest->method & (OC_REST_OBSERVE | OC_REST_OBSERVE_ALL))
            {
                if (localPendingRequest->mockOCResource->m_uri.compare(uri) == 0)
                {
                    observationIdToCancel = localPendingRequest->observationId;
                    g_requestList.erase(localPendingRequest->requestNumber);
                    break;
                }
            }
        }
    }

    auto pRequest = std::make_shared<OC::OCResourceRequest>();
    if (pRequest == nullptr)
    {
        return OC_EH_ERROR;
    }

    MessageContainer ocInfo;
    ocInfo.addRepresentation(rep);
    for(const OCRepresentation& r : rep.getChildren())
    {
       ocInfo.addRepresentation(r);
    }

    OCPayload* payload = reinterpret_cast<OCPayload*>(ocInfo.getPayload());

    OCEntityHandlerRequest  entityHandlerRequest;
    memset(&entityHandlerRequest, 0, sizeof(OCEntityHandlerRequest));

    entityHandlerRequest.requestHandle = reinterpret_cast<void*>(pendingRequest->requestNumber);
    entityHandlerRequest.resource = nullptr;
    entityHandlerRequest.messageID = 0;
    entityHandlerRequest.method = method;
    entityHandlerRequest.payload = payload;

    if (flag & OC_OBSERVE_FLAG)
    {
        std::lock_guard<std::recursive_mutex> lock(g_globalMutex);
        entityHandlerRequest.obsInfo.action = isCancelObserve ?
                                                    OC_OBSERVE_DEREGISTER : OC_OBSERVE_REGISTER;
        entityHandlerRequest.obsInfo.obsId = isCancelObserve ?
                                                    observationIdToCancel : ++g_observationId;
        pendingRequest->observationId = entityHandlerRequest.obsInfo.obsId;
    }

    // Pass QueryParamsMap directly to formResourceRequest() which has access
    // to pRequest->setQueryParams().
    QueryParamsMap localCopy = queryParametersMap;
    entityHandlerRequest.query = reinterpret_cast<char*>(&localCopy);

    formResourceRequest(flag, &entityHandlerRequest, pRequest);
    pRequest->setResourceUri(uri);

    std::thread callEntityHandlerThread(mockOCResource->m_entityHandler, pRequest);
    callEntityHandlerThread.detach();
    return OC_EH_OK;
}
