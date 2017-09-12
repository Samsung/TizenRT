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

#include <string>
#include <iostream>
#include "ipca.h"
#include "OCFDevice.h"

OCFDevice::OCFDevice(IPCAAppHandle appHandle, std::string id) :
    m_ipcaAppHandle(appHandle),
    m_deviceId(id),
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
        return IPCAOpenDevice(m_ipcaAppHandle, m_deviceId.c_str(), &m_deviceHandle);
    }
}

IPCADeviceInfo* OCFDevice::GetDeviceInfo()
{
    IPCAStatus status;

    if (m_deviceInfo != nullptr)
    {
        return m_deviceInfo;
    }

    if (OpenDevice() != IPCA_OK)
    {
        return nullptr;
    }

    status = IPCAGetDeviceInfo(m_deviceHandle, &m_deviceInfo);

    if (IPCA_OK != status)
    {
        return nullptr;
    }

    return m_deviceInfo;
}

IPCAPlatformInfo* OCFDevice::GetPlatformInfo()
{
    IPCAStatus status;

    if (m_platformInfo != nullptr)
    {
        return m_platformInfo;
    }

    if (OpenDevice() != IPCA_OK)
    {
        return nullptr;
    }

    status = IPCAGetPlatformInfo(m_deviceHandle, &m_platformInfo);

    if (IPCA_OK != status)
    {
        return nullptr;
    }

    return m_platformInfo;
}

ResourceList OCFDevice::GetResourceInfo()
{
    IPCAStatus status;

    ResourceList emptyResourceList;
    if (OpenDevice() != IPCA_OK)
    {
        return emptyResourceList;
    }

    char** resourcePathList;
    size_t resourceListCount;
    status = IPCAGetResources(m_deviceHandle,
                nullptr, nullptr, &resourcePathList, &resourceListCount);
    if (IPCA_OK != status)
    {
        return emptyResourceList;
    }

    m_resourceList.clear();
    for (size_t i = 0 ; i < resourceListCount ; i++)
    {
        char** resourceTypes;
        size_t resourceTypeCount;
        status = IPCAGetResourceTypes(m_deviceHandle,
                    resourcePathList[i], &resourceTypes, &resourceTypeCount);
        if (IPCA_OK == status)
        {
            for (size_t j = 0 ; j < resourceTypeCount; j++)
            {
                m_resourceList[resourcePathList[i]].push_back(resourceTypes[j]);
            }
            IPCAFreeStringArray(resourceTypes, resourceTypeCount);
        }
        else
        {
            std::cout << "Failed IPCAGetResourceTypes() for resource: " << resourcePathList[i];
            std::cout << std::endl;
        }
    }

    IPCAFreeStringArray(resourcePathList, resourceListCount);
    return m_resourceList;
}
