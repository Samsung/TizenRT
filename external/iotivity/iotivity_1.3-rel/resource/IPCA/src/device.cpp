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

#include "ipca.h"
#include "ipcainternal.h"
#include "oic_malloc.h"

Device::Device(const char* deviceId, OCFFramework* ocf, App::Ptr app) :
    m_deviceId(deviceId),
    m_app(app),
    m_ocfFramework(ocf),
    m_isClosed(false)
{
}

Device::~Device()
{
}

IPCAStatus Device::Open()
{
    return m_ocfFramework->IPCADeviceOpenCalled(m_deviceId);
}

IPCAStatus Device::Close()
{
    m_isClosed = true;
    return m_ocfFramework->IPCADeviceCloseCalled(m_deviceId);
}

IPCAStatus Device::GetDeviceInfo(IPCADeviceInfo** callerDeviceInfo)
{
    if (m_isClosed)
    {
        return IPCA_FAIL;
    }

    return m_ocfFramework->CopyDeviceInfo(m_deviceId, callerDeviceInfo);
}

IPCAStatus Device::GetPlatformInfo(IPCAPlatformInfo** callerPlatformInfo)
{
    if (m_isClosed)
    {
        return IPCA_FAIL;
    }

    return m_ocfFramework->CopyPlatformInfo(m_deviceId, callerPlatformInfo);
}

IPCAStatus Device::GetResourcePathList(const std::string& resourceInterface,
                        const std::string& resourceType,
                        char*** resourcePathList,
                        size_t* resourcePathCount)
{
    if (m_isClosed)
    {
        return IPCA_FAIL;
    }

    *resourcePathList = nullptr;
    *resourcePathCount = 0;

    std::vector<std::string> resourcePaths;
    IPCAStatus status = m_ocfFramework->CopyResourcePaths(
                            resourceInterface, resourceType, m_deviceId, resourcePaths);
    if (status != IPCA_OK)
    {
        return status;
    }

    return AllocateAndCopyStringVectorToArrayOfCharPointers(
                            resourcePaths, resourcePathList, resourcePathCount);
}

IPCAStatus Device::GetResourceInfo(const char* resourcePath,
                        ResourceInfoType resourceInfoType,
                        char*** stringArray,
                        size_t* stringArrayCount)
{
    if (m_isClosed)
    {
        return IPCA_FAIL;
    }

    *stringArray = nullptr;
    *stringArrayCount = 0;

    std::vector<std::string> resourceInfo;
    std::string resURI = "";
    if (resourcePath)
    {
        resURI = resourcePath;
    }

    IPCAStatus status = m_ocfFramework->CopyResourceInfo(
                                m_deviceId, resURI, resourceInfoType, resourceInfo);
    if (status != IPCA_OK)
    {
        return status;
    }

    return AllocateAndCopyStringVectorToArrayOfCharPointers(
                                resourceInfo, stringArray, stringArrayCount);
}

IPCAStatus Device::GetProperties(CallbackInfo::Ptr callbackInfo)
{
    if (m_isClosed)
    {
        return IPCA_FAIL;
    }

    return m_ocfFramework->SendCommandToDevice(m_deviceId, callbackInfo, nullptr);
}

IPCAStatus Device::SetProperties(CallbackInfo::Ptr callbackInfo, OC::OCRepresentation* rep)
{
    if (m_isClosed)
    {
        return IPCA_FAIL;
    }

    return m_ocfFramework->SendCommandToDevice(m_deviceId, callbackInfo, rep);
}

IPCAStatus Device::CreateResource(CallbackInfo::Ptr callbackInfo, OC::OCRepresentation* rep)
{
    if (m_isClosed)
    {
        return IPCA_FAIL;
    }

    return m_ocfFramework->SendCommandToDevice(m_deviceId, callbackInfo, rep);
}

IPCAStatus Device::DeleteResource(CallbackInfo::Ptr callbackInfo)
{
    if (m_isClosed)
    {
        return IPCA_FAIL;
    }

    return m_ocfFramework->SendCommandToDevice(m_deviceId, callbackInfo, nullptr);
}

IPCAStatus Device::ObserveResource(CallbackInfo::Ptr callbackInfo)
{
    if (m_isClosed)
    {
        return IPCA_FAIL;
    }

    return m_ocfFramework->SendCommandToDevice(m_deviceId, callbackInfo, nullptr);
}

void Device::StopObserve(CallbackInfo::Ptr cbInfo)
{
    return m_ocfFramework->StopObserve(cbInfo);
}

void Device::IsResourceObservable(const char* resourcePath, bool* isObservable)
{
    if (m_isClosed)
    {
        *isObservable = false;
        return;
    }

    m_ocfFramework->IsResourceObservable(m_deviceId, resourcePath, isObservable);
}

IPCAStatus Device::Ping()
{
    if (m_isClosed)
    {
        return IPCA_FAIL;
    }

    return m_ocfFramework->PingDevice(m_deviceId);
}

IPCAStatus Device::GetLastPingTime(uint64_t& lastPingTime)
{
    if (m_isClosed)
    {
        return IPCA_FAIL;
    }

    return m_ocfFramework->GetLastPingTime(m_deviceId, lastPingTime);
}

IPCAStatus Device::RequestAccess(CallbackInfo::Ptr callbackInfo,
                                 CallbackInfo::Ptr passwordInputCallbackInfo)
{
    if (m_isClosed)
    {
        return IPCA_FAIL;
    }

    return m_ocfFramework->RequestAccess(m_deviceId, callbackInfo, passwordInputCallbackInfo);
}
