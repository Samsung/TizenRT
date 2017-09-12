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

#include "oic_time.h"
#include "ipcainternal.h"
#include "ocrandom.h"

// Object that implements interface to IoTivity.
// @future: Consider having an instance of this per app when there's mechanism to unregister
//          from IoTivity and IoTivity supports multiple apps in same process.
OCFFramework ocfFramework;

#define TAG "IPCA_App"

App::App(const IPCAAppInfo* ipcaAppInfo, IPCAVersion ipcaVersion) :
    m_isStopped(false),
    m_ipcaVersion(ipcaVersion),
    m_passwordInputCallbackHandle(nullptr),
    m_passwordInputCallbackInfo(nullptr),
    m_passwordDisplayCallbackHandle(nullptr),
    m_passwordDisplayCallbackInfo(nullptr)
{
    m_ipcaAppInfo.appId = ipcaAppInfo->appId;
    m_ipcaAppInfo.appName = ipcaAppInfo->appName;
    m_ipcaAppInfo.appSoftwareVersion = ipcaAppInfo->appSoftwareVersion;
    m_ipcaAppInfo.appCompanyName = ipcaAppInfo->appCompanyName;
}

App::~App()
{
}

IPCAStatus App::Start(bool unitTestMode, App::Ptr thisSharedPtr)
{
    char appId[UUID_STRING_SIZE];
    if (!OCConvertUuidToString(m_ipcaAppInfo.appId.uuid, appId))
    {
        return IPCA_FAIL;
    }

    m_appId = appId;
    m_thisSharedPtr = thisSharedPtr;

    m_callback = std::make_shared<Callback>(m_thisSharedPtr);
    if (m_callback == nullptr)
    {
        return IPCA_OUT_OF_MEMORY;
    }

    // Start OCFFramework
    IPCAStatus status = ocfFramework.Start(m_ipcaAppInfo, unitTestMode);
    if (status != IPCA_OK)
    {
        m_callback = nullptr;
        return status;
    }

    // Register app's callback with OCFFramework.
    if (ocfFramework.RegisterAppCallbackObject(m_callback) != IPCA_OK)
    {
        ocfFramework.Stop(m_passwordInputCallbackHandle, m_passwordDisplayCallbackHandle);
        m_callback = nullptr;
        return IPCA_FAIL;
    }

    // Start periodic discovery thread.
    m_appWorkerThread = std::thread(&App::AppWorkerThread, m_thisSharedPtr);
    return IPCA_OK;
}

void App::Stop()
{
    ocfFramework.UnregisterAppCallbackObject(m_callback);

    if (m_isStopped)
    {
        assert(false);  // code fault in ipca.cpp if this happens.
        return;
    }

    // Stop the discovery thread.
    m_isStopped = true;
    m_discoveryThreadCV.notify_all();   // Wake discovery thread and wait for it to quit.
    if (m_appWorkerThread.joinable())
    {
        m_appWorkerThread.join();
    }

    // Wait for all in progress callbacks are completed.
    m_callback->Stop();
    m_callback = nullptr;

    // Force close devices that are still open.  Caller continues to own the deviceWrapper memory
    // but with device handle closed.
    for(auto& it : m_openedDevices)
    {
        it.second->device->Close();
        it.second->device = nullptr; // releases reference to device.
    }

    // Stop the OCFFramework.
    ocfFramework.Stop(m_passwordInputCallbackHandle, m_passwordDisplayCallbackHandle);
    m_passwordInputCallbackHandle = nullptr;
    m_passwordDisplayCallbackHandle = nullptr;

    // Deregister the callback info.
    if (m_passwordInputCallbackInfo != nullptr)
    {
        DeleteAndUnregisterCallbackInfo(m_passwordInputCallbackInfo->mapKey);
        m_passwordInputCallbackInfo = nullptr;
    }

    if (m_passwordDisplayCallbackInfo != nullptr)
    {
        DeleteAndUnregisterCallbackInfo(m_passwordDisplayCallbackInfo->mapKey);
        m_passwordDisplayCallbackInfo = nullptr;
    }

    m_thisSharedPtr = nullptr;  // Release reference to self.
}

std::string App::GetAppId()
{
    return m_appId;
}

void App::AppWorkerThread(App::Ptr app)
{
    const uint64_t FastDiscoveryCount = 4;  // First 4 periodic discovery requests use fast period.
    const uint64_t SlowDiscoveryPeriodMs = 30000;
    const uint64_t FastDiscoveryPeriodMs = 2000;

    const uint64_t PingPeriodMS = 30000;  // Do device ping for Observed devices every 30 seconds.

    // Outstanding requests should time out in 247 seconds (EXCHANGE_LIFETIME) per rfc 7252.
    // Wake up every second to check.
    const size_t AppThreadSleepTimeSeconds = 1;
    std::chrono::seconds appThreadSleepTime(AppThreadSleepTimeSeconds);

    std::unique_lock<std::mutex> appWorkerLock(app->m_appWorkerThreadMutex);

    OIC_LOG_V(INFO, TAG, "+AppWorkerThread started.");

    while (false == app->m_isStopped)
    {
        uint64_t currentTime = OICGetCurrentTime(TIME_IN_MS);

        // Do periodic discovery for active IPCADiscoverDevices() requests.
        std::map<size_t, std::vector<std::string>> resourceTypesToDiscover;
        {
            std::lock_guard<std::mutex> lock(app->m_appMutex);
            for (auto& entry : app->m_discoveryList)
            {
                DiscoveryDetails::Ptr discoveryDetails = entry.second;

                if (discoveryDetails->discoveryCount < FastDiscoveryCount)
                {
                    if (currentTime - discoveryDetails->lastDiscoveryTime > FastDiscoveryPeriodMs)
                    {
                        resourceTypesToDiscover[entry.first] =
                            discoveryDetails->resourceTypesToDiscover;

                        discoveryDetails->lastDiscoveryTime = currentTime;
                        discoveryDetails->discoveryCount++;
                    }
                }
                else
                {
                    if (currentTime - discoveryDetails->lastDiscoveryTime > SlowDiscoveryPeriodMs)
                    {
                        resourceTypesToDiscover[entry.first] =
                            discoveryDetails->resourceTypesToDiscover;

                        discoveryDetails->lastDiscoveryTime = currentTime;
                        discoveryDetails->discoveryCount++;
                    }
                }
            }
        }

        for (auto& resourceTypes : resourceTypesToDiscover)
        {
            ocfFramework.DiscoverResources(resourceTypes.second);
        }

        // Do callbacks for expired outstanding requests.
        std::vector<CallbackInfo::Ptr> expiredCallbacks;
        app->m_callback->CompleteAndRemoveExpiredCallbackInfo(expiredCallbacks);
        expiredCallbacks.clear();   // no use of the expired callbacks.

        // Get oustanding Observe requests and ping the device every PingPeriodMS.
        std::vector<CallbackInfo::Ptr> observeCallbacks;
        app->m_callback->GetCallbackInfoList(CallbackType_ResourceChange, observeCallbacks);
        for (auto& cbInfo : observeCallbacks)
        {
            uint64_t lastPingTime;
            if ((IPCA_OK == cbInfo->device->GetLastPingTime(lastPingTime)) &&
                (currentTime - lastPingTime > PingPeriodMS))
            {
                cbInfo->device->Ping();
            }
        }

        app->m_discoveryThreadCV.wait_for(appWorkerLock,
                        appThreadSleepTime,
                        [app]()
                        {
                            return app->m_isStopped;
                        });
    }

    OIC_LOG_V(INFO, TAG, "-AppWorkerThread exit.");
}

IPCAStatus App::OpenDevice(App::Ptr thisApp, const char* deviceId, IPCADeviceHandle* deviceHandle)
{
    *deviceHandle = nullptr;

    std::unique_ptr<DeviceWrapper> deviceWrapper(new DeviceWrapper);
    if (deviceWrapper == nullptr)
    {
        return IPCA_OUT_OF_MEMORY;
    }

    Device::Ptr device = std::shared_ptr<Device>(new Device(deviceId, &ocfFramework, thisApp));
    if (device == nullptr)
    {
        return IPCA_OUT_OF_MEMORY;
    }

    IPCAStatus status = device->Open();
    if (status != IPCA_OK)
    {
        return status;
    }

    deviceWrapper->app = thisApp;
    deviceWrapper->device = device;  // Take a device reference.
    *deviceHandle =  reinterpret_cast<IPCADeviceHandle>(deviceWrapper.get());
    m_openedDevices[deviceWrapper.get()] = deviceWrapper.get();
    deviceWrapper.release();
    return IPCA_OK;
}

void App::CloseDevice(IPCADeviceHandle deviceHandle)
{
    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    if (m_openedDevices.find(deviceWrapper) == m_openedDevices.end())
    {
        return;
    }

    if (deviceWrapper->device != nullptr)
    {
        deviceWrapper->device->Close();
        deviceWrapper->device = nullptr; // Release reference to device.
    }

    m_openedDevices.erase(deviceWrapper);
    delete deviceWrapper;
}

IPCAStatus App::DiscoverDevices(
                        IPCADiscoverDeviceCallback callback,
                        const void* context,
                        const char* const* resourceTypeList,
                        int resourceTypeCount,
                        IPCAHandle* handle)
{
    CallbackInfo::Ptr cbInfo = nullptr;

    // Discovery must have a callback.
    if (callback == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    DiscoveryDetails::Ptr discoveryDetails = std::shared_ptr<DiscoveryDetails>
                                                            (new(DiscoveryDetails));

    if (discoveryDetails == nullptr)
    {
        return IPCA_OUT_OF_MEMORY;
    }

    IPCAStatus status = CreateAndRegisterNewCallbackInfo(
                                           handle,
                                           nullptr, // No device handle, discovery is a multicast.
                                           &cbInfo,
                                           CallbackType_Discovery,
                                           context,
                                           callback,
                                           resourceTypeList,
                                           resourceTypeCount,
                                           nullptr,  // Not Get/Set/Create/Observe request.
                                           nullptr,  // Resource interface is not needed.
                                           nullptr,  // Resource path is not needed.
                                           nullptr); // Similarly resource type is also not needed.

    if (status != IPCA_OK)
    {
        return status;
    }

    // Start discovery.
    discoveryDetails->lastDiscoveryTime = OICGetCurrentTime(TIME_IN_MS);
    discoveryDetails->discoveryCount = 1;
    discoveryDetails->resourceTypesToDiscover = cbInfo->resourceTypeList;
    status = ocfFramework.DiscoverResources(cbInfo->resourceTypeList);

    if (status == IPCA_OK)
    {
        // Add it to the periodic discovery list.
        std::lock_guard<std::mutex> lock(m_appMutex);
        m_discoveryList[cbInfo->mapKey] = discoveryDetails;
    }
    else
    {
        if (handle != nullptr)
        {
            *handle = nullptr;
        }

        DeleteAndUnregisterCallbackInfo(cbInfo->mapKey);
    }

   return status;
}

IPCAStatus App::SetPasswordCallbacks(
        IPCAProvidePasswordCallback inputCallback,
        IPCADisplayPasswordCallback displayCallback,
        void* context)
{
    if ((inputCallback == nullptr) || (displayCallback == nullptr))
    {
        return IPCA_INVALID_ARGUMENT;
    }

    CallbackInfo::Ptr inputCallbackInfo = nullptr;
    CallbackInfo::Ptr displayCallbackInfo = nullptr;

    inputCallbackInfo = m_callback->CreatePasswordCallbackInfo(
                                            CallbackType_PasswordInputCallback,
                                            context,
                                            inputCallback,
                                            nullptr);

    if (inputCallbackInfo == nullptr)
    {
        return IPCA_OUT_OF_MEMORY;
    }

    IPCAStatus status = m_callback->AddCallbackInfo(inputCallbackInfo);
    if (status != IPCA_OK)
    {
        return status;
    }

    displayCallbackInfo = m_callback->CreatePasswordCallbackInfo(
                                            CallbackType_PasswordDisplayCallback,
                                            context,
                                            nullptr,
                                            displayCallback);

    if (displayCallbackInfo == nullptr)
    {
        DeleteAndUnregisterCallbackInfo(inputCallbackInfo->mapKey);
        return IPCA_OUT_OF_MEMORY;
    }

    status = m_callback->AddCallbackInfo(displayCallbackInfo);
    if (status != IPCA_OK)
    {
        // Failed to add displayCallbackInfo to the callback list, deregister inputCallbackInfo.
        // Note that: Failure to add displayCallbackInfo to the list means no ref count is taken on
        //            displayCallbackInfo. Therefore it will automatically be deleted outside the
        //            scope of this function.
        DeleteAndUnregisterCallbackInfo(inputCallbackInfo->mapKey);
        return status;
    }

    ocfFramework.SetInputPasswordCallback(inputCallbackInfo, &m_passwordInputCallbackHandle);
    ocfFramework.SetDisplayPasswordCallback(displayCallbackInfo, &m_passwordDisplayCallbackHandle);

    // The CallbackInfo to be deregistered in Stop().
    m_passwordInputCallbackInfo = inputCallbackInfo;
    m_passwordDisplayCallbackInfo = displayCallbackInfo;

    return IPCA_OK;
}

IPCAStatus App::RequestAccess(
                    Device::Ptr device,
                    const char* resourcePath,
                    IPCARequestAccessCompletionCallback completionCallback,
                    void* context,
                    IPCAHandle* handle)
{
    CallbackInfo::Ptr cbInfo;
    CallbackInfo::Ptr passwordInputCbInfo;

    if (handle != nullptr)
    {
        *handle = nullptr;
    }

    cbInfo = m_callback->CreateRequestAccessCompletionCallbackInfo(
                                device,
                                context,
                                resourcePath,
                                completionCallback);

    if (cbInfo == nullptr)
    {
        return IPCA_OUT_OF_MEMORY;
    }

    IPCAStatus status = m_callback->AddCallbackInfo(cbInfo);
    if (status == IPCA_OK)
    {
        if (handle != nullptr)
        {
            *handle = reinterpret_cast<IPCAHandle>(cbInfo->mapKey);
        }
    }
    else
    {
        return status;
    }

    passwordInputCbInfo = m_callback->GetPasswordInputCallbackInfo();
    if (passwordInputCbInfo == nullptr)
    {
        // App has not registered for password callback.
        // Delete the request access callback completion.
        DeleteAndUnregisterCallbackInfo(cbInfo->mapKey);
        return IPCA_FAIL;
    }

    status = device->RequestAccess(cbInfo, passwordInputCbInfo);
    if (status != IPCA_OK)
    {
        if (handle != nullptr)
        {
            *handle = nullptr;
        }

        DeleteAndUnregisterCallbackInfo(cbInfo->mapKey);
    }

   return status;
}

IPCAStatus App::GetProperties(
                           Device::Ptr device,
                           IPCAGetPropertiesComplete callback,
                           const void* context,
                           const char* resourcePath,
                           const char* resourceInterface,
                           const char* resourceType,
                           IPCAHandle* handle)
{
    CallbackInfo::Ptr cbInfo = nullptr;

    IPCAStatus status = CreateAndRegisterNewCallbackInfo(
                                   handle,
                                   device,
                                   &cbInfo,
                                   CallbackType_GetPropertiesComplete,
                                   context,
                                   nullptr,     //  Not discovery request.
                                   nullptr,     //  Therefore resourceTypeList is not needed.
                                   0,
                                   callback,
                                   resourcePath,
                                   resourceInterface,
                                   resourceType);

    if (status != IPCA_OK)
    {
        return status;
    }

    status = device->GetProperties(cbInfo);

    if (status != IPCA_OK)
    {
        if (handle != nullptr)
        {
            *handle = nullptr;
        }

        DeleteAndUnregisterCallbackInfo(cbInfo->mapKey);
    }

   return status;
}

IPCAStatus App::SetProperties(
                           Device::Ptr device,
                           IPCASetPropertiesComplete callback,
                           const void* context,
                           const char* resourcePath,
                           const char* resourceInterface,
                           const char* resourceType,
                           OC::OCRepresentation* rep,
                           IPCAHandle* handle)
{
    CallbackInfo::Ptr cbInfo = nullptr;

    IPCAStatus status = CreateAndRegisterNewCallbackInfo(
                                       handle,
                                       device,
                                       &cbInfo,
                                       CallbackType_SetPropertiesComplete,
                                       context,
                                       nullptr,     //  Not discovery request.
                                       nullptr,     //  Therefore resourceTypeList is not needed.
                                       0,
                                       callback,
                                       resourcePath,
                                       resourceInterface,
                                       resourceType);

    if (status != IPCA_OK)
    {
        return status;
    }

    status = device->SetProperties(cbInfo, rep);

    if ((status != IPCA_OK) && (cbInfo != nullptr))
    {
        if (handle != nullptr)
        {
            *handle = nullptr;
        }

        DeleteAndUnregisterCallbackInfo(cbInfo->mapKey);
    }

    return status;
}

IPCAStatus App::ObserveResource(
                           Device::Ptr device,
                           IPCAResourceChangeCallback callback,
                           const void* context,
                           const char* resourcePath,
                           const char* resourceType,
                           IPCAHandle* handle)
{
    CallbackInfo::Ptr cbInfo = nullptr;

    // ObserveResource must have a callback.
    if (callback == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    IPCAStatus status = CreateAndRegisterNewCallbackInfo(
                                       handle,
                                       device,
                                       &cbInfo,
                                       CallbackType_ResourceChange,
                                       context,
                                       nullptr,     //  Not a discovery request.
                                       nullptr,     //  Therefore resourceTypeList is not needed.
                                       0,
                                       callback,
                                       resourcePath,
                                       nullptr,
                                       resourceType);

    if (status != IPCA_OK)
    {
        return status;
    }

    status = device->ObserveResource(cbInfo);

    if (status == IPCA_OK)
    {
        cbInfo->inObserve = true;
    }

    if ((status != IPCA_OK) && (cbInfo != nullptr))
    {
        if (handle != nullptr)
        {
            *handle = nullptr;
        }

        DeleteAndUnregisterCallbackInfo(cbInfo->mapKey);
    }

    return status;
}

IPCAStatus App::CreateResource(
                    Device::Ptr device,
                    IPCACreateResourceComplete createResourceCb,
                    const void* context,
                    const char* resourcePath,
                    const char* resourceInterface,
                    const char* resourceType,
                    OC::OCRepresentation* rep,
                    IPCAHandle* handle)
{
    CallbackInfo::Ptr cbInfo = nullptr;

    IPCAStatus status = CreateAndRegisterNewCallbackInfo(
                                       handle,
                                       device,
                                       &cbInfo,
                                       CallbackType_CreateResourceComplete,
                                       context,
                                       nullptr,     //  Not a discovery request.
                                       nullptr,     //  Therefore resourceTypeList is not needed.
                                       0,
                                       reinterpret_cast<GenericAppCallback>(createResourceCb),
                                       resourcePath,
                                       resourceInterface,
                                       resourceType);

    if (status != IPCA_OK)
    {
        return status;
    }

    status = device->CreateResource(cbInfo, rep);

    if ((status != IPCA_OK) && (cbInfo != nullptr))
    {
        if (handle != nullptr)
        {
            *handle = nullptr;
        }

        DeleteAndUnregisterCallbackInfo(cbInfo->mapKey);
    }

    return status;
}

IPCAStatus App::DeleteResource(
                    Device::Ptr device,
                    IPCADeleteResourceComplete deleteResourceCb,
                    const void* context,
                    const char* resourcePath,
                    IPCAHandle* handle)
{
    CallbackInfo::Ptr cbInfo = nullptr;

    IPCAStatus status = CreateAndRegisterNewCallbackInfo(
                                       handle,
                                       device,
                                       &cbInfo,
                                       CallbackType_DeleteResourceComplete,
                                       context,
                                       nullptr,     // Not a discovery request.
                                       nullptr,     // Therefore resourceTypeList is not needed.
                                       0,
                                       reinterpret_cast<GenericAppCallback>(deleteResourceCb),
                                       resourcePath,
                                       nullptr,
                                       nullptr);

    if (status != IPCA_OK)
    {
        return status;
    }

    status = device->DeleteResource(cbInfo);

    if ((status != IPCA_OK) && (cbInfo != nullptr))
    {
        if (handle != nullptr)
        {
            *handle = nullptr;
        }

        DeleteAndUnregisterCallbackInfo(cbInfo->mapKey);
    }

    return status;
}

IPCAStatus App::CloseIPCAHandle(IPCAHandle handle,
                    IPCACloseHandleComplete closeHandleComplete,
                    const void* context)
{
    size_t mapKey = reinterpret_cast<size_t>(handle);

    CallbackInfo::Ptr cbInfo = m_callback->GetCallbackInfo(mapKey);

    if (cbInfo != nullptr)
    {
        if (cbInfo->type == CallbackType_Discovery)
        {
            // Stop periodic discovery of these resource types.
            std::lock_guard<std::mutex> lock(m_appMutex);
            m_discoveryList.erase(cbInfo->mapKey);
        }
        else
        if ((cbInfo->type == CallbackType_ResourceChange) && cbInfo->inObserve)
        {
            cbInfo->device->StopObserve(cbInfo);
            cbInfo->inObserve = false;
        }
    }

    return DeleteAndUnregisterCallbackInfo(mapKey, closeHandleComplete, context);
}

IPCAStatus App::CreateAndRegisterNewCallbackInfo(
                                IPCAHandle* handle,
                                Device::Ptr device,
                                CallbackInfo::Ptr* cbInfo,
                                CallbackType cbType,
                                const void* context,
                                IPCADiscoverDeviceCallback discoverDeviceCallback,
                                const char* const* resourceTypeList,
                                int resourceTypeCount,
                                GenericAppCallback appCallback,
                                const char* resourcePath,
                                const char* resourceInterface,
                                const char* resourceType)
{
    if (handle != nullptr)
    {
        *handle = nullptr;
    }

    *cbInfo = m_callback->CreateCallbackInfo(
                                device,
                                cbType,
                                context,
                                discoverDeviceCallback,
                                resourceTypeList,
                                resourceTypeCount,
                                appCallback,
                                resourcePath,
                                resourceInterface,
                                resourceType);

    if (*cbInfo == nullptr)
    {
        return IPCA_OUT_OF_MEMORY;
    }

    IPCAStatus status = m_callback->AddCallbackInfo(*cbInfo);

    if (status == IPCA_OK)
    {
        if (handle != nullptr)
        {
            *handle = reinterpret_cast<IPCAHandle>((*cbInfo)->mapKey);
        }
    }

    return status;
}

IPCAStatus App::DeleteAndUnregisterCallbackInfo(
                                size_t mapKey,
                                IPCACloseHandleComplete closeHandleComplete,
                                const void* context)
{
    return m_callback->RemoveCallbackInfo(mapKey, closeHandleComplete, context);
}
