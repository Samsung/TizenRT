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

#ifndef APP_H_
#define APP_H_

typedef std::shared_ptr<CallbackInfo> CallbackInfoPtr;

// Internal version of IPCAAppInfo.
struct IPCAAppInfoInternal
{
    IPCAUuid     appId;
    std::string  appIdString;
    std::string  appName;
    std::string  appSoftwareVersion;
    std::string  appCompanyName;
};

// Housekeeping for periodic discovery.
// The lastDiscoveryTime & discoveryCount are used to schedule more frequent discovery in
// the beginning.
typedef struct _DiscoveryDetails
{
    typedef std::shared_ptr<_DiscoveryDetails> Ptr;
    std::vector<std::string> resourceTypesToDiscover;  // list of resource types
    uint64_t lastDiscoveryTime;  // last time discovery request was sent for this DiscoveryDetails.
    size_t discoveryCount;  // and how many times already.
} DiscoveryDetails;

// One App object per IPCAOpen()
class App
{
    public:
        typedef std::shared_ptr<App> Ptr;

        App(const IPCAAppInfo* ipcaAppInfo, IPCAVersion ipcaVersion);
        ~App();

        // Application calls IPCAOpen()/IPCAClose().
        IPCAStatus Start(bool unitTestMode, App::Ptr thisSharedPtr);
        void Stop();

        // Returns application's ID.
        std::string GetAppId();

        // Application calls IPCADiscoverDevices().
        IPCAStatus DiscoverDevices(
                        IPCADiscoverDeviceCallback callback,
                        const void* context,
                        const char* const* resourceTypeList,
                        int resourceTypeCount,
                        IPCAHandle* handle);

        // Application calls IPCAOpenDevice().
        IPCAStatus OpenDevice(
                        App::Ptr thisApp,
                        const char* deviceId,
                        IPCADeviceHandle* deviceHandle);

        void CloseDevice(IPCADeviceHandle deviceHandle);

        // Application calls IPCAGetProperties().
        IPCAStatus GetProperties(
                           Device::Ptr device,
                           IPCAGetPropertiesComplete getPropertiesCb,
                           const void* context,
                           const char* resourcePath,
                           const char* resourceInterface,
                           const char* resourceType,
                           IPCAHandle* handle);

        // Application calls IPCASetProperties().
        IPCAStatus SetProperties(
                           Device::Ptr device,
                           IPCAGetPropertiesComplete getPropertiesCb,
                           const void* context,
                           const char* resourcePath,
                           const char* resourceInterface,
                           const char* resourceType,
                           OC::OCRepresentation* rep,
                           IPCAHandle* handle);

        // Application calls IPCAObserveResource().
        IPCAStatus ObserveResource(
                           Device::Ptr device,
                           IPCAResourceChangeCallback resourceChangeCb,
                           const void* context,
                           const char* resourcePath,
                           const char* resourceType,
                           IPCAHandle* handle);

        // Application calls IPCACreateResource().
        IPCAStatus CreateResource(
                            Device::Ptr device,
                            IPCACreateResourceComplete createResourceCb,
                            const void* context,
                            const char* resourcePath,
                            const char* resourceInterface,
                            const char* resourceType,
                            OC::OCRepresentation* rep,
                            IPCAHandle* handle);

        // Application calls IPCADeleteResource()
        IPCAStatus DeleteResource(
                            Device::Ptr device,
                            IPCADeleteResourceComplete deleteResourceCb,
                            const void* context,
                            const char* resourcePath,
                            IPCAHandle* handle);

        /* Security/Ownership */
        IPCAStatus SetPasswordCallbacks(
                            IPCAProvidePasswordCallback inputCallback,
                            IPCADisplayPasswordCallback displayCallback,
                            void* context);

        IPCAStatus RequestAccess(
                            Device::Ptr device,
                            const char* resourcePath,
                            IPCARequestAccessCompletionCallback completionCallback,
                            void* context,
                            IPCAHandle* handle);

        // Close handle returned in DiscoverDevices(), GetProperties(), SetProperties(),
        // ObserveResource(), CreateResource() or RequestAccess().
        // Return IPCA_OK if successful.  Or failure, for example, if handle is already closed.
        IPCAStatus CloseIPCAHandle(
                        IPCAHandle handle,
                        IPCACloseHandleComplete closeHandleComplete,
                        const void* context);
    private:
        std::mutex m_appMutex;
        App::Ptr m_thisSharedPtr;   // shared_ptr of this object.
        volatile bool m_isStopped;  // set to true when Stop() is called.

        IPCAAppInfoInternal m_ipcaAppInfo;
        std::string m_appId;        // String of m_ipcaAppInfo.appId.
        IPCAVersion m_ipcaVersion;  // IPCA version requested in the call to IPCAOpen().

        // Object that implements callbacks to the app.
        std::shared_ptr<Callback> m_callback;

        // Devices this app opened.
        std::map<DeviceWrapper*, DeviceWrapper*> m_openedDevices;

        // Thread that performs periodic discovery.
        std::thread m_appWorkerThread;
        std::condition_variable m_discoveryThreadCV;
        std::mutex m_appWorkerThreadMutex;

        // Create and register CallbackInfo with the Callback object.
        IPCAStatus CreateAndRegisterNewCallbackInfo(
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
                                    const char* resourceType);

        // Delete the CallbackInfo and unregister from Callback object list.
        IPCAStatus DeleteAndUnregisterCallbackInfo(
                                    size_t mapKey,
                                    IPCACloseHandleComplete closeHandleComplete = nullptr,
                                    const void* context = nullptr);

        // Thread performing periodic discovery.
        static void AppWorkerThread(App::Ptr app);

        // List of resource types to discover periodically.
        // Key is cbInfo->mapKey of each IPCADiscoverDevices() request.
        std::map<size_t, DiscoveryDetails::Ptr> m_discoveryList;

        // Password callback registration
        InputPinCallbackHandle m_passwordInputCallbackHandle;
        CallbackInfo::Ptr m_passwordInputCallbackInfo;
        DisplayPinCallbackHandle m_passwordDisplayCallbackHandle;
        CallbackInfo::Ptr m_passwordDisplayCallbackInfo;
};
#endif // APP_H_
