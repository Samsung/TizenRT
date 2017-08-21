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

#include "iotivity_config.h"
#include "oic_malloc.h"
#include "ipca.h"
#include "ipcainternal.h"

#define OCFFactoryReset     "fr"
#define OCFReboot           "rb"

// As in proc there's no reason for multiple IPCAOpen().
// There's no ref count for g_app.  Therefore must hold g_ipcaAppMutex when using g_app.
App* g_app = nullptr;
std::mutex g_ipcaAppMutex;

bool g_unitTestMode = false;

IPCAStatus IPCA_CALL IPCAOpen(
                        const IPCAAppInfo* ipcaAppInfo,
                        IPCAVersion ipcaVersion,
                        IPCAAppHandle* ipcaAppHandle)
{
    std::lock_guard<std::mutex> lock(g_ipcaAppMutex);

    if (ipcaVersion != IPCA_VERSION_1)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    if (g_app != nullptr)
    {
        return IPCA_ALREADY_OPENED;
    }

    if (ipcaAppInfo == nullptr ||
        ipcaAppInfo->appName == nullptr ||
        ipcaAppInfo->appSoftwareVersion == nullptr ||
        ipcaAppInfo->appCompanyName == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    g_app= new App(ipcaAppInfo, ipcaVersion);

    if (g_app == nullptr)
    {
        return IPCA_OUT_OF_MEMORY;
    }

    IPCAStatus status = g_app->Start(g_unitTestMode);
    if (status != IPCA_OK)
    {
        delete g_app;
        g_app = nullptr;
        return status;
    }

    *ipcaAppHandle = reinterpret_cast<IPCAAppHandle>(g_app);
    return IPCA_OK;
}

void IPCA_CALL IPCAClose(IPCAAppHandle ipcaAppHandle)
{
    App* app = reinterpret_cast<App*>(ipcaAppHandle);
    std::lock_guard<std::mutex> lock(g_ipcaAppMutex);
    if (g_app == nullptr)
    {
        return;
    }

    app->Stop();
    delete app;
    g_app = nullptr;
}

IPCAStatus IPCA_CALL IPCADiscoverDevices(
                                    IPCAAppHandle ipcaAppHandle,
                                    IPCADiscoverDeviceCallback callback,
                                    void* context,
                                    const char* const* resourceTypeList,
                                    int resourceTypeCount,
                                    IPCAHandle* handle)
{
    App* app = reinterpret_cast<App*>(ipcaAppHandle);
    return app->DiscoverDevices(callback, context, resourceTypeList, resourceTypeCount, handle);
}

IPCAStatus IPCA_CALL IPCAOpenDevice(
                                    IPCAAppHandle ipcaAppHandle,
                                    const char* deviceId,
                                    IPCADeviceHandle* deviceHandle)
{
    App* app = reinterpret_cast<App*>(ipcaAppHandle);
    return app->OpenDevice(deviceId, deviceHandle);
}

void IPCA_CALL IPCACloseDevice(IPCADeviceHandle deviceHandle)
{
    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    App* app = deviceWrapper->device->GetApp();
    app->CloseDevice(deviceHandle);
}

IPCAStatus IPCA_CALL IPCAGetDeviceInfo(IPCADeviceHandle deviceHandle, IPCADeviceInfo** deviceInfo)
{
    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    return deviceWrapper->device->GetDeviceInfo(deviceInfo);
}

void IPCA_CALL IPCAFreeDeviceInfo(IPCADeviceInfo* deviceInfo)
{
    OCFFramework::FreeDeviceInfo(deviceInfo);
}

IPCAStatus IPCA_CALL IPCAGetPlatformInfo(IPCADeviceHandle deviceHandle,
                                         IPCAPlatformInfo** platformInfo)
{
    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    return deviceWrapper->device->GetPlatformInfo(platformInfo);
}

void IPCA_CALL IPCAFreePlatformInfo(IPCAPlatformInfo* platformInfo)
{
    OCFFramework::FreePlatformInfo(platformInfo);
}

IPCAStatus IPCA_CALL IPCAGetResources(
                            IPCADeviceHandle deviceHandle,
                            const char* resourceInterface,
                            const char* resourceType,
                            char*** resourcePathList,
                            size_t* resourcePathCount)
{
    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    return deviceWrapper->device->GetResourcePathList(
                                    std::string(resourceInterface ? resourceInterface : ""),
                                    std::string(resourceType ? resourceType : ""),
                                    resourcePathList,
                                    resourcePathCount);
}

IPCAStatus IPCA_CALL IPCAGetResourceTypes(IPCADeviceHandle deviceHandle,
                            const char* resourcePath,
                            char*** resourceTypeList,
                            size_t* resourceTypeCount)
{
    DeviceWrapper* deviceWrapper = (DeviceWrapper*)deviceHandle;
    return deviceWrapper->device->GetResourceInfo(resourcePath,
                ResourceInfoType::ResourceType, resourceTypeList, resourceTypeCount);
}

IPCAStatus IPCA_CALL IPCAGetResourceInterfaces(IPCADeviceHandle deviceHandle,
                            const char* resourcePath,
                            char*** resourceInterfaceList,
                            size_t* resourceInterfaceCount)
{
    DeviceWrapper* deviceWrapper = (DeviceWrapper*)deviceHandle;
    return deviceWrapper->device->GetResourceInfo(resourcePath,
                ResourceInfoType::ResourceInterface, resourceInterfaceList, resourceInterfaceCount);
}

void IPCA_CALL IPCAFreeStringArray(char** stringArray, size_t stringCount)
{
    FreeArrayOfCharPointers(stringArray, stringCount);
}

IPCAStatus IPCA_CALL IPCAGetProperties(IPCADeviceHandle deviceHandle,
                                       IPCAGetPropertiesComplete getPropertiesCb,
                                       void* context,
                                       const char* resourcePath,
                                       const char* resourceInterface,
                                       const char* resourceType,
                                       IPCAHandle* handle)
{
    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    return deviceWrapper->app->GetProperties(
                deviceWrapper->device,
                getPropertiesCb,
                context,
                resourcePath,
                resourceInterface,
                resourceType,
                handle);
}

IPCAStatus IPCA_CALL IPCASetProperties(IPCADeviceHandle deviceHandle,
                                        IPCASetPropertiesComplete setPropertiesCb,
                                        void* context,
                                        const char* resourcePath,
                                        const char* resourceInterface,
                                        const char* resourceType,
                                        IPCAPropertyBagHandle propertyBagHandle,
                                        IPCAHandle* handle)
{
    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    return deviceWrapper->app->SetProperties(
                deviceWrapper->device,
                setPropertiesCb,
                context,
                resourcePath,
                resourceInterface,
                resourceType,
                (OC::OCRepresentation*)propertyBagHandle,
                handle);
}

void IPCA_CALL IPCAIsResourceObservable(IPCADeviceHandle deviceHandle,
                    const char* resourcePath,
                    bool* isObservable)
{
    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    deviceWrapper->device->IsResourceObservable(resourcePath, isObservable);
    return;
}

IPCAStatus IPCA_CALL IPCAObserveResource(IPCADeviceHandle deviceHandle,
                                         IPCAResourceChangeCallback resourceChangeCb,
                                         void* context,
                                         const char* resourcePath,
                                         const char* resourceType,
                                         IPCAHandle* handle)
{
    if (handle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    return deviceWrapper->app->ObserveResource(
                    deviceWrapper->device,
                    resourceChangeCb,
                    context,
                    resourcePath,
                    resourceType,
                    handle);
}

IPCAStatus IPCA_CALL IPCACreateResource(
                                    IPCADeviceHandle deviceHandle,
                                    IPCACreateResourceComplete createResourceCb,
                                    void* context,
                                    const char* resourcePath,
                                    const char* resourceInterface,
                                    const char* resourceType,
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    IPCAHandle* handle)
{
    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    return deviceWrapper->app->CreateResource(
                deviceWrapper->device,
                createResourceCb,
                context,
                resourcePath,
                resourceInterface,
                resourceType,
                (OC::OCRepresentation*)propertyBagHandle,
                handle);
}

IPCAStatus IPCA_CALL IPCADeleteResource(
                                    IPCADeviceHandle deviceHandle,
                                    IPCADeleteResourceComplete deleteResourceCb,
                                    void* context,
                                    const char* resourcePath,
                                    IPCAHandle* handle)
{
    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    return deviceWrapper->app->DeleteResource(
                    deviceWrapper->device,
                    deleteResourceCb,
                    context,
                    resourcePath,
                    handle);
}

IPCAStatus IPCA_CALL IPCACloseHandle(IPCAHandle handle,
                        IPCACloseHandleComplete closeHandleComplete,
                        void* context)
{
    std::lock_guard<std::mutex> lock(g_ipcaAppMutex);
    if (g_app != nullptr)
    {
        return (g_app->CloseIPCAHandle(handle, closeHandleComplete, context));
    }

    return IPCA_FAIL;
}

typedef struct
{
    std::mutex syncMutex;
    std::condition_variable condVar;
    IPCAStatus result;
} AsyncContext;

void IPCA_CALL AsyncCallback(
                    IPCAStatus result,
                    void* context,
                    IPCAPropertyBagHandle propertyBagHandle)
{
    OC_UNUSED(propertyBagHandle);

    AsyncContext* async = reinterpret_cast<AsyncContext*>(context);
    async->result = result;
    async->condVar.notify_all();
}

IPCAStatus IPCA_CALL IPCAFactoryReset(IPCADeviceHandle deviceHandle)
{
    IPCAStatus status;

    AsyncContext factoryResetContext;
    std::unique_lock<std::mutex> lock { factoryResetContext.syncMutex };

    IPCAPropertyBagHandle propertyBagHandle;
    status = IPCAPropertyBagCreate(&propertyBagHandle);
    if (status != IPCA_OK)
    {
        return status;
    }

    status = IPCAPropertyBagSetValueBool(propertyBagHandle, OCFFactoryReset, true);
    if (status != IPCA_OK)
    {
        IPCAPropertyBagDestroy(propertyBagHandle);
        return status;
    }

    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    status = deviceWrapper->app->SetProperties(
                                    deviceWrapper->device,
                                    &AsyncCallback,
                                    &factoryResetContext,
                                    nullptr,
                                    nullptr,
                                    OC_RSRVD_RESOURCE_TYPE_MAINTENANCE,
                                    (OC::OCRepresentation*)propertyBagHandle,
                                    nullptr);

    if (status == IPCA_OK)
    {
        factoryResetContext.condVar.wait_for(lock, std::chrono::milliseconds{ INT_MAX });
        IPCAPropertyBagDestroy(propertyBagHandle);
        return factoryResetContext.result;
    }

    IPCAPropertyBagDestroy(propertyBagHandle);
    return status;
}

IPCAStatus IPCA_CALL IPCAReboot(IPCADeviceHandle deviceHandle)
{
    IPCAStatus status;

    AsyncContext rebootContext;
    std::unique_lock<std::mutex> lock { rebootContext.syncMutex };

    IPCAPropertyBagHandle propertyBagHandle;
    status = IPCAPropertyBagCreate(&propertyBagHandle);
    if (status != IPCA_OK)
    {
        return status;
    }

    status = IPCAPropertyBagSetValueBool(propertyBagHandle, OCFReboot, true);
    if (status != IPCA_OK)
    {
        IPCAPropertyBagDestroy(propertyBagHandle);
        return status;
    }

    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    status = deviceWrapper->app->SetProperties(
                                    deviceWrapper->device,
                                    &AsyncCallback,
                                    &rebootContext,
                                    nullptr,
                                    nullptr,
                                    OC_RSRVD_RESOURCE_TYPE_MAINTENANCE,
                                    (OC::OCRepresentation*)propertyBagHandle,
                                    nullptr);

    if (status == IPCA_OK)
    {
        rebootContext.condVar.wait_for(lock, std::chrono::milliseconds{ INT_MAX });
        IPCAPropertyBagDestroy(propertyBagHandle);
        return rebootContext.result;
    }

    IPCAPropertyBagDestroy(propertyBagHandle);
    return status;
}

IPCAStatus IPCA_CALL IPCASetPasswordCallbacks(
                                IPCAAppHandle ipcaAppHandle,
                                IPCAProvidePasswordCallback inputCallback,
                                IPCADisplayPasswordCallback displayCallback,
                                void* context)
{
    App* app = reinterpret_cast<App*>(ipcaAppHandle);
    return app->SetPasswordCallbacks(inputCallback, displayCallback, context);
}

IPCAStatus IPCA_CALL IPCARequestAccess(
                                IPCADeviceHandle deviceHandle,
                                const char* resourcePath,
                                IPCARequestAccessCompletionCallback completionCallback,
                                void* context,
                                IPCAHandle* handle)
{
    DeviceWrapper* deviceWrapper = reinterpret_cast<DeviceWrapper*>(deviceHandle);
    return deviceWrapper->app->RequestAccess(
                                    deviceWrapper->device,
                                    resourcePath,
                                    completionCallback,
                                    context,
                                    handle);
}

/*********************/
/* Utility functions */
/*********************/

void IPCA_CALL IPCASetUnitTestMode()
{
    g_unitTestMode = true;
}
