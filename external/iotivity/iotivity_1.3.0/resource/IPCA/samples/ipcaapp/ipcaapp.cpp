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

#include <mutex>
#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <thread>
#include <chrono>
#include <memory>
#include <functional>
#include <algorithm>

#include "ipca.h"

#define UNUSED_PARAMETER(P)       (void)(P)

IPCAAppHandle g_ipcaAppHandle;
std::recursive_mutex g_globalMutex;

// Key is resource URI. Value is array of resource types.
typedef std::map<std::string, std::vector<std::string>> ResourceTypeList;

// Key is resource URI. Value is array of resource interfaces.
typedef std::map<std::string, std::vector<std::string>> ResourceInterfaceList;

// Key is resource URI, Values is array of property name & property value type pair.
typedef std::map<std::string, std::map<std::string, IPCAValueType>> ResourceProperties;

size_t g_idPool = 1;
size_t GenerateUniqueId()
{
    std::lock_guard<std::recursive_mutex> lock(g_globalMutex);
    return g_idPool++;
}

class OCFDevice
{
public:
    typedef std::shared_ptr<OCFDevice> Ptr;
    size_t m_localId;   // used as context to async IPCA call so there's no need to take a
                        // ref count.

    OCFDevice(std::string id);
    ~OCFDevice();
    void GetDeviceDetails(std::string deviceName, const char** deviceUris, size_t count);
    void DisplayDevice();
    void GetPropertiesCallback(IPCAStatus result, const void* context,
            IPCAPropertyBagHandle propertyBagHandle);

private:
    IPCADeviceInfo* GetDeviceInfo();
    IPCAPlatformInfo* GetPlatformInfo();
    ResourceTypeList GetResourceTypeInfo();
    ResourceInterfaceList GetResourceInterfaceInfo();

private:
    IPCAStatus OpenDevice();
    std::string m_deviceId;
    std::string m_deviceName;
    IPCADeviceHandle m_deviceHandle;    // from IPCAOpenDevice();
    IPCADeviceInfo* m_deviceInfo;
    IPCAPlatformInfo* m_platformInfo;
    std::vector<std::string> m_deviceUris;    // Uris of device.
    ResourceTypeList m_resourceTypeList;
    ResourceInterfaceList m_resourceInterfaceList;
    ResourceProperties m_resourceProperties;
};

OCFDevice::OCFDevice(std::string id) :
    m_localId(GenerateUniqueId()),
    m_deviceId (id),
    m_deviceHandle(nullptr),
    m_deviceInfo(nullptr),
    m_platformInfo(nullptr)
{
}

OCFDevice::~OCFDevice()
{
    if (m_deviceHandle != nullptr)
    {
        IPCACloseDevice(m_deviceHandle);
    }

    if (m_deviceInfo != nullptr)
    {
        IPCAFreeDeviceInfo(m_deviceInfo);
    }

    if (m_platformInfo != nullptr)
    {
        IPCAFreePlatformInfo(m_platformInfo);
    }
}

IPCAStatus OCFDevice::OpenDevice()
{
    if (m_deviceHandle != nullptr)
    {
        return IPCA_OK;
    }
    else
    {
        return IPCAOpenDevice(g_ipcaAppHandle, m_deviceId.c_str(), &m_deviceHandle);
    }
}

IPCADeviceInfo* OCFDevice::GetDeviceInfo()
{
    return m_deviceInfo;
}

IPCAPlatformInfo* OCFDevice::GetPlatformInfo()
{
    return m_platformInfo;
}

void IPCA_CALL C_GetPropertiesCallback(
                            IPCAStatus result,
                            void* context,
                            IPCAPropertyBagHandle propertyBagHandle)
{
    std::lock_guard<std::recursive_mutex> lock(g_globalMutex);

    size_t localId = reinterpret_cast<size_t>(context);
    OCFDevice::Ptr device = nullptr;

    extern std::map<std::string, OCFDevice::Ptr> g_OCFDeviceList;

    // Find the device.
    for (const auto& ocfDevice : g_OCFDeviceList)
    {
        if (ocfDevice.second->m_localId == localId)
        {
            device = ocfDevice.second;
            break;
        }
    }

    if (device)
    {
        device->GetPropertiesCallback(result, context, propertyBagHandle);
    }
}

void OCFDevice::GetPropertiesCallback(IPCAStatus result,
                    const void* context,
                    IPCAPropertyBagHandle propertyBagHandle)
{
    UNUSED_PARAMETER(context);
    UNUSED_PARAMETER(result);

    std::lock_guard<std::recursive_mutex> lock(g_globalMutex);

    IPCAStatus status;
    char* resourcePath;
    char** allKeys;
    IPCAValueType* allValueTypes;
    size_t count;

    status = IPCAPropertyBagGetResourcePath(propertyBagHandle, &resourcePath);
    if ((status != IPCA_OK) || (resourcePath == nullptr))
    {
        return;
    }

    status = IPCAPropertyBagGetAllKeyValuePairs(propertyBagHandle,
                            &allKeys, &allValueTypes, &count);
    if (status != IPCA_OK)
    {
        IPCAPropertyBagFreeString(resourcePath);
        return;
    }

    std::map<std::string, IPCAValueType> properties;
    for (size_t i = 0 ; i < count ; i++)
    {
        properties[allKeys[i]] = allValueTypes[i];
    }

    // Trigger new display if there's any new info.
    std::map<std::string, IPCAValueType> knownProperties = m_resourceProperties[resourcePath];

    for (auto& newProperty : properties)
    {
        if ((knownProperties.size() == 0) ||
            (knownProperties.find(newProperty.first) == knownProperties.end()))
        {
            // At least one new property is not in known properties.
            // Replace known properties & display device.
            m_resourceProperties[resourcePath] = properties;
            std::cout << "=== Updated info on device properties: === " << std::endl << std::endl;
            DisplayDevice();
            break;
        }
    }

    IPCAPropertyBagFreeString(resourcePath);
    IPCAPropertyBagFreeStringArray(allKeys, count);
    IPCAPropertyBagFreeIPCAValueTypeArray(allValueTypes);
}

ResourceTypeList OCFDevice::GetResourceTypeInfo()
{
    return m_resourceTypeList;
}

ResourceInterfaceList OCFDevice::GetResourceInterfaceInfo()
{
    return m_resourceInterfaceList;
}

std::string MapIPCAValueTypeToString(IPCAValueType type)
{
    switch(type)
    {
        case IPCA_INTEGER:
            return "integer";

        case IPCA_DOUBLE:
            return "double";

        case IPCA_BOOLEAN:
            return "boolean";

        case IPCA_STRING:
            return "string";

        case IPCA_ARRAY:
            return "array";

        case IPCA_PROPERTY_BAG:
            return "property bag";

        case IPCA_VALUE_TYPE_NOT_SUPPORTED:
            return "not supported";

        default:
            return "unknown";
    }
}

void OCFDevice::DisplayDevice()
{
    IPCADeviceInfo* di = GetDeviceInfo();

    if (di != nullptr)
    {
        std::cout << "Device Info: "  << std::endl;
        std::cout << std::endl;
        if (m_deviceUris.size() != 0)
        {
            int i = 0;
            for (auto& uri : m_deviceUris)
            {
                if (i++ == 0)
                {

                    std::cout << "   Device URI . . . . . . . :  " << uri << std::endl;
                }
                else
                {
                    std::cout << "                               " << uri << std::endl;
                }
            }
        }

        std::cout << "   Device ID  . . . . . . . :  " << (di->deviceId ? di->deviceId : "");
        std::cout << std::endl;
        std::cout << "   Device Name  . . . . . . :  " << (di ->deviceName ? di->deviceName : "");
        std::cout << std::endl;
        std::cout << "   Device Software Version  :  ";
        std::cout << (di->deviceSoftwareVersion ? di->deviceSoftwareVersion : "") << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Device Info: Not available." << std::endl << std::endl;
    }

    IPCAPlatformInfo* pi = GetPlatformInfo();

    if (pi != nullptr)
    {
        std::cout << "Platform Info:"  << std::endl;
        std::cout << std::endl;
        std::cout << "   Plaform ID . . . . . . . :  ";
        std::cout << (pi->platformId ? pi->platformId : "") << std::endl;
        std::cout << "   Manufacturer Name  . . . :  ";
        std::cout << (pi->manufacturerName ? pi->manufacturerName : "")  << std::endl;
        std::cout << "   Manufacturer URL . . . . :  ";
        std::cout << (pi->manufacturerURL ? pi->manufacturerURL : "")  << std::endl;
        std::cout << "   Model Number . . . . . . :  ";
        std::cout << (pi->modelNumber ? pi->modelNumber : "")  << std::endl;
        std::cout << "   Manufacturing Date . . . :  ";
        std::cout << (pi->manufacturingDate ? pi->manufacturingDate : "") << std::endl;
        std::cout << "   Platform Version . . . . :  ";
        std::cout << (pi->platformVersion ? pi->platformVersion : "")  << std::endl;
        std::cout << "   OS Version . . . . . . . :  ";
        std::cout << (pi->osVersion ? pi->osVersion : "")  << std::endl;
        std::cout << "   Hardware Version . . . . :  ";
        std::cout << (pi->hardwareVersion ? pi->hardwareVersion : "")  << std::endl;
        std::cout << "   Firmware Version . . . . :  ";
        std::cout << (pi->firmwareVersion ? pi->firmwareVersion : "")  << std::endl;
        std::cout << "   Manufacturer Support URL :  ";
        std::cout << (pi->manufacturerSupportURL ? pi->manufacturerSupportURL : "")  << std::endl;
        std::cout << "   Reference Time . . . . . :  ";
        std::cout << (pi->referenceTime ? pi->referenceTime : "") << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Platform Info: Not available." << std::endl << std::endl;
    }

    std::cout << "Resource List:" << std::endl;
    ResourceTypeList resourceTypeList = GetResourceTypeInfo();
    ResourceInterfaceList resourceInterfaceList = GetResourceInterfaceInfo();
    for (auto const& resourceType : resourceTypeList)
    {
        bool firstEntry = true;
        std::cout << "   Resource URI   . . . . . :  " << resourceType.first << std::endl;
        std::cout << "      Resource Types  . . . :  ";
        for (auto const& rt : resourceType.second)
        {
            if (firstEntry == true)
            {
                std::cout << rt << std::endl;
                firstEntry = false;
            }
            else
            {
                std::cout << "                               " << rt << std::endl;
            }
        }

        firstEntry = true;
        std::cout << "      Resource Interfaces . :  ";
        for (auto const& resourceInterface : resourceInterfaceList[resourceType.first])
        {
            if (firstEntry == true)
            {
                std::cout << resourceInterface << std::endl;
                firstEntry = false;
            }
            else
            {
                std::cout << "                               " << resourceInterface << std::endl;
            }
        }

        std::map<std::string, IPCAValueType> properties = m_resourceProperties[resourceType.first];
        if (properties.size() != 0)
        {
            std::cout << "      Properties  . . . . . :  ";
            firstEntry = true;
            for (auto const& property : properties)
            {
                if (firstEntry == true)
                {
                    firstEntry = false;
                }
                else
                {
                    std::cout << "                               ";
                }

                std::cout << property.first << " (";
                std::cout << MapIPCAValueTypeToString(property.second) << ")" << std::endl;
            }
        }

        std::cout << std::endl;
    }

    std::cout << std::endl << std::endl;
}

void OCFDevice::GetDeviceDetails(std::string deviceName, const char** deviceUris, size_t count)
{
    std::lock_guard<std::recursive_mutex> lock(g_globalMutex);

    m_deviceName = deviceName;

    for (size_t i = 0; i < count; i++)
    {
       if (std::find(m_deviceUris.begin(), m_deviceUris.end(), deviceUris[i]) == m_deviceUris.end())
       {
           m_deviceUris.push_back(deviceUris[i]);
       }
    }

    IPCAStatus status;

    if (OpenDevice() != IPCA_OK)
    {
        return;
    }

    // Get device info, platform info, and resources incl. resource types & property types.
    if (m_deviceInfo == nullptr)
    {
        status = IPCAGetDeviceInfo(m_deviceHandle, &m_deviceInfo);
        if ((status != IPCA_INFORMATION_NOT_AVAILABLE) &&  (status != IPCA_OK))
        {
            std::cout << "Failed IPCAGetDeviceInfo() status: " << status << std::endl;
        }
    }

    if (m_platformInfo == nullptr)
    {
        status = IPCAGetPlatformInfo(m_deviceHandle, &m_platformInfo);
        if ((status != IPCA_INFORMATION_NOT_AVAILABLE) && (status != IPCA_OK))
        {
            std::cout << "Failed IPCAGetPlatformInfo() status: " << status << std::endl;
        }
    }

    char** resourcePathList;
    size_t resourceListCount;
    status = IPCAGetResources(m_deviceHandle,
                nullptr, nullptr, &resourcePathList, &resourceListCount);
    if (IPCA_OK != status)
    {
        std::cout << "Failed IPCAGetResourceURIs() status: " << status << std::endl;
    }

    m_resourceTypeList.clear();
    m_resourceInterfaceList.clear();

    for (size_t i = 0 ; i < resourceListCount ; i++)
    {
        // Get resource types for each resource.
        char** resourceTypes;
        size_t resourceTypeCount;
        status = IPCAGetResourceTypes(m_deviceHandle,
                    resourcePathList[i], &resourceTypes, &resourceTypeCount);
        if (IPCA_OK == status)
        {
            for (size_t j = 0 ; j < resourceTypeCount; j++)
            {
                m_resourceTypeList[resourcePathList[i]].push_back(resourceTypes[j]);
            }
            IPCAFreeStringArray(resourceTypes, resourceTypeCount);
        }
        else
        {
            std::cout << "Failed IPCAGetResourceTypes() for resource: ";
            std::cout << resourcePathList[i] << std::endl;
        }

        // Get resource interfaces for each resource.
        char** resourceInterfaces;
        size_t resourceInterfaceCount;
        status = IPCAGetResourceInterfaces(m_deviceHandle,
                    resourcePathList[i], &resourceInterfaces, &resourceInterfaceCount);
        if (IPCA_OK == status)
        {
            for (size_t j = 0 ; j < resourceInterfaceCount; j++)
            {
                m_resourceInterfaceList[resourcePathList[i]].push_back(resourceInterfaces[j]);
            }
            IPCAFreeStringArray(resourceInterfaces, resourceInterfaceCount);
        }
        else
        {
            std::cout << "Failed IPCAGetResourceInterfaces() for resource: ";
            std::cout << resourcePathList[i] << std::endl;
        }

        // Get the resource properties.
        status = IPCAGetProperties(m_deviceHandle,
                        &C_GetPropertiesCallback,
                        reinterpret_cast<void*>(m_localId),
                        resourcePathList[i],
                        nullptr,
                        nullptr,
                        nullptr);

        if (status != IPCA_OK)
        {
            std::cout << "Failed IPCAGetProperties() status: " << status << std::endl;
        }
    }

    IPCAFreeStringArray(resourcePathList, resourceListCount);
}

// Key is device id.  Value is pointer to OCFDevice.
std::map<std::string, OCFDevice::Ptr> g_OCFDeviceList;

// Callback when device is discovered.
void IPCA_CALL DiscoverDevicesCallback(
                            void* context,
                            IPCADeviceStatus deviceStatus,
                            const IPCADiscoveredDeviceInfo* deviceInfo)
{
    UNUSED_PARAMETER(context);

    std::lock_guard<std::recursive_mutex> lock(g_globalMutex);

    std::string deviceId = deviceInfo->deviceId;
    std::string deviceName = deviceInfo->deviceName;

    if (g_OCFDeviceList.find(deviceId) == g_OCFDeviceList.end())
    {
        OCFDevice::Ptr ocfDevice = std::shared_ptr<OCFDevice>(new OCFDevice(deviceId));
        if (ocfDevice == nullptr)
        {
            std::cout << "Out of memory" << std::endl;
            return;
        }
        g_OCFDeviceList[deviceId] = ocfDevice;
    }

    OCFDevice::Ptr ocfDevice = g_OCFDeviceList[deviceId];

    ocfDevice->GetDeviceDetails(deviceName, deviceInfo->deviceUris, deviceInfo->deviceUriCount);

    if (deviceStatus == IPCA_DEVICE_DISCOVERED)
    {
        std::cout << "*** New Device. Device ID: [" << deviceId << "] ***";
        std::cout << std::endl << std::endl;
    }
    else
    if (deviceStatus == IPCA_DEVICE_UPDATED_INFO)
    {
        std::cout << "+++ Updated Info. Device ID: [" << deviceId << "] +++";
        std::cout << std::endl << std::endl;
    }
    else
    {
        std::cout << "--- Device no longer discoverable. Device ID: [" << deviceId << "] ---";
        std::cout << std::endl << std::endl;
        g_OCFDeviceList.erase(deviceId);
        return;
    }

    ocfDevice->DisplayDevice();
}

IPCAStatus IPCA_CALL PasswordInputCallback(
                                    void* context,
                                    const IPCADeviceInfo* deviceInformation,
                                    const IPCAPlatformInfo* platformInformation,
                                    IPCAOwnershipTransferType type,
                                    char* passwordBuffer,
                                    size_t passwordBufferSize)
{
    UNUSED_PARAMETER(context);
    UNUSED_PARAMETER(deviceInformation);
    UNUSED_PARAMETER(platformInformation);
    UNUSED_PARAMETER(type);
    UNUSED_PARAMETER(passwordBuffer);
    UNUSED_PARAMETER(passwordBufferSize);

    // @todo: collect the password from the user

    // Refuse authentication for ownership transfer.
    return IPCA_FAIL;
}

IPCAStatus IPCA_CALL PasswordDisplayCallback(
                                    void* context,
                                    const IPCADeviceInfo* deviceInformation,
                                    const IPCAPlatformInfo* platformInformation,
                                    IPCAOwnershipTransferType type,
                                    const char* password)
{
    UNUSED_PARAMETER(context);
    UNUSED_PARAMETER(deviceInformation);
    UNUSED_PARAMETER(platformInformation);
    UNUSED_PARAMETER(type);
    UNUSED_PARAMETER(password);

    // @todo: display the password and ask for confirmation from the user

    // Refuse authentication for ownership transfer.
    return IPCA_FAIL;
}

int main()
{
    // @future:
    // possible options:
    // ipcapp [-o] | [-rt] | [-g] | [-s]
    // -r <resource URI>
    // -rt <resource type name>
    // -g <dataName>
    // -s <dataName> <value>

    IPCAHandle discoverDeviceHandle;
    IPCAStatus status;

    // Initialize IPCA.
    IPCAUuid appId = {
                        {0x37, 0x9d, 0xf2, 0xf2, 0x7e, 0xf7, 0x11, 0xe6,
                         0xae, 0x22, 0x56, 0xb6, 0xb6, 0x49, 0x96, 0x11}
                     };
    IPCAAppInfo ipcaAppInfo = { appId, "IPCAAPP", "1.0.0", "Microsoft" };

    status = IPCAOpen(&ipcaAppInfo, IPCA_VERSION_1, &g_ipcaAppHandle);
    if (status != IPCA_OK)
    {
        std::cout << "IPCAOpen failed, status = " << status << std::endl;
    }

    // Get ready for Ownership Transfer.
    IPCASetPasswordCallbacks(g_ipcaAppHandle,
        PasswordInputCallback, PasswordDisplayCallback, nullptr);

    // Start discovering devices.
    const char* resourceTypes[] = {
        "" /* any resource type */
    };

    const int ResourceTypeCount = sizeof(resourceTypes)/sizeof(char*);

    status = IPCADiscoverDevices(
                g_ipcaAppHandle,
                &DiscoverDevicesCallback,
                nullptr,
                resourceTypes,
                ResourceTypeCount,
                &discoverDeviceHandle);


    int userInput;
    std::cin >> userInput;

    g_OCFDeviceList.clear();

    IPCACloseHandle(discoverDeviceHandle, nullptr, 0);
    IPCAClose(g_ipcaAppHandle);
}
