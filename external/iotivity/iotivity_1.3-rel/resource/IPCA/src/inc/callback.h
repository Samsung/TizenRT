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

#ifndef CALLBACK_H_
#define CALLBACK_H_

typedef enum
{
    CallbackType_Discovery = 1,
    CallbackType_ResourceChange,
    CallbackType_GetPropertiesComplete,
    CallbackType_SetPropertiesComplete,
    CallbackType_CreateResourceComplete,
    CallbackType_DeleteResourceComplete,
    CallbackType_PasswordInputCallback,
    CallbackType_PasswordDisplayCallback,
    Callbacktype_RequestAccessCompletionCallback,
    CallbackType_InvalidType
} CallbackType;

namespace OC {
    class OCResource;
}
class App;
class Device;
typedef std::shared_ptr<Device> DevicePtr;
typedef std::shared_ptr<App> AppPtr;

// Structure contains information to make callbacks to app.
struct CallbackInfo
{
    CallbackInfo() { /* std::cout << "+ CallbackInfo " << this << std::endl; */ }
    ~CallbackInfo() { /* std::cout << "- CallbackInfo " << this << " mapKey: ";
                         std::cout << mapKey << std::endl; */ }
    typedef std::shared_ptr<CallbackInfo> Ptr;
    size_t mapKey;          // key to m_callbackInfoList map.  Initialized in AddCallbackInfo().
    AppPtr app;             // the app that creates this callback.
    DevicePtr device;       // the device expected to respond to the callback.
    CallbackType type;
    union
    {
        IPCADiscoverDeviceCallback discoveryCallback;
        IPCAResourceChangeCallback resourceChangeCallback;
        IPCAGetPropertiesComplete getCallback;
        IPCASetPropertiesComplete setCallback;
        IPCACreateResourceComplete createResourceCallback;
        IPCADeleteResourceComplete deleteResourceCallback;
        IPCAProvidePasswordCallback passwordInputCallback;
        IPCADisplayPasswordCallback passwordDisplayCallback;
        IPCARequestAccessCompletionCallback requestAccessCompletionCallback;
    };
    const void* callbackContext;    // app's callback context.
    std::vector<std::string> resourceTypeList;  // Parameter for Discovery.
    std::string resourcePath;       // Parameters for for Get, Set, Observe request.
    std::string resourceInterface;
    std::string resourceType;

    size_t callbackInProgressCount; // Non zero if callback is in progress.
    bool markedToBeRemoved; // Set to true when this object can't be removed in
                            // RemoveCallbackInfo(). It'll be removed opportunistically.

    // closeHandleCompleteCallback is from argument in IPCACloseHandle().
    // It will be set in this structure if IPCACloseHandle() is called when the callback
    // for this CallbackInfo is in progress.
    IPCACloseHandleComplete closeHandleCompleteCallback;
    const void* closeHandleCompletecontext;

    std::vector<std::string> discoveredDevicesList; // List of device ids that were indicated to
                                                    // app with IPCA_DEVICE_DISCOVERED.
    std::shared_ptr<OC::OCResource> ocResource; // The OCResource this callback works with.

    uint64_t requestSentTimestamp; // when the request was sent to the server.

    bool inObserve; // set to true when observe request is sent for this callback.
};

// Represent IPCAResourceChangeCallback, IPCAGetPropertiesComplete, IPCASetPropertiesComplete.
typedef void (IPCA_CALL *GenericAppCallback)(
                                    IPCAStatus result,
                                    void* context,
                                    IPCAPropertyBagHandle propertyBagHandle);

// One Callback object per App.  One app per IPCAOpen().
class Callback
{
    public:
        typedef std::shared_ptr<Callback> Ptr;
        Callback(AppPtr app);
        ~Callback();

        // Preparation to shut down.
        // Function returns when there's no pending callback.
        // No callback is allowed after returning from this call.
        void Stop();

        CallbackInfo::Ptr CreateCallbackInfo(
                                        DevicePtr device,
                                        CallbackType cbType,
                                        const void* context,
                                        IPCADiscoverDeviceCallback discoveryCallback,/* discovery */
                                                                                     /* callback  */
                                        const char* const* resourceTypeList,
                                        int resourceTypeCount,
                                        GenericAppCallback callback, /* get, set, observe, create &
                                                                        delete resource callbacks */
                                        const char* resourcePath,
                                        const char* resourceInterface,
                                        const char* resourceType);

        // Password display or password input callback.
        CallbackInfo::Ptr CreatePasswordCallbackInfo(
                                        CallbackType cbType,
                                        const void* context,
                                        IPCAProvidePasswordCallback passwordInputCallback,
                                        IPCADisplayPasswordCallback passwordDisplayCallback);

        // Auth completion callback.
        CallbackInfo::Ptr CreateRequestAccessCompletionCallbackInfo(
                                        DevicePtr device,
                                        const void* context,
                                        const char* resourceURI,
                                        IPCARequestAccessCompletionCallback completionCallback);

        // Add CallbackInfo to the list of pending callbacks.
        IPCAStatus AddCallbackInfo(CallbackInfo::Ptr cbInfo);

        // Return pointer to CallbackInfo matching mapKey.
        CallbackInfo::Ptr GetCallbackInfo(size_t mapKey);

        // Return a pointer to PasswordInputCallback CallbackInfo
        CallbackInfo::Ptr GetPasswordInputCallbackInfo();

        // Remove the CallbackInfo matching mapKey. This function sets markedToBeRemoved if the
        // callback is in the middle of calling back.
        // Function returns IPCA_FAIL if callback is already closed.
        IPCAStatus RemoveCallbackInfo(size_t mapKey,
                        IPCACloseHandleComplete closeHandleComplete = nullptr,
                        const void* context = nullptr);

        // Complete the callback for expired CallbackInfo and remove them from the
        // m_callbackInfoList. Caller receives a list of them.
        void CompleteAndRemoveExpiredCallbackInfo(std::vector<CallbackInfo::Ptr>& cbInfoList);

        // Return a list of CallbackInfo object matching the type.
        void GetCallbackInfoList(CallbackType type, std::vector<CallbackInfo::Ptr>& cbInfoList);

        // Device discovery related.
        void DeviceDiscoveryCallback(bool deviceResponding,
                bool newInfoLearntAboutDevice,
                const InternalDeviceInfo& deviceInfo,
                const std::vector<std::string>& resourceTypes);

        // resource->get() callback.
        void GetCallback(IPCAStatus status,
                const OC::OCRepresentation& rep,
                CallbackInfo::Ptr cbInfo);

        // resource->put() callback.
        void SetCallback(IPCAStatus status,
                const OC::OCRepresentation& rep,
                CallbackInfo::Ptr cbInfo,
                std::string newResourcePath);

        // resource->observe() callback.
        void ObserveCallback(IPCAStatus status,
                const OC::OCRepresentation& rep,
                CallbackInfo::Ptr cbInfo);

        // resource->deleteResource() callback.
        void DeleteResourceCallback(IPCAStatus status, CallbackInfo::Ptr cbInfo);

        // Security/Auth
        void PasswordInputCallback(
                std::string deviceId,
                IPCAOwnershipTransferType type,
                char* passwordBuffer,
                size_t passwordBufferSize,
                CallbackInfo::Ptr cbInfo);

        void PasswordDisplayCallback(
                std::string deviceId,
                IPCAOwnershipTransferType type,
                const char* password,
                CallbackInfo::Ptr cbInfo);

        void RequestAccessCompletionCallback(IPCAStatus status, CallbackInfo::Ptr cbInfo);

    private:
        bool MatchAllRequiredResourceTypes(const std::vector<std::string>& requiredResourceTypes,
                                           const std::vector<std::string>& deviceResourceTypes);

        // Callback to app's closeHandleComplete() that is set in IPCACloseHandle().
        void CallCloseHandleComplete(IPCACloseHandleComplete closeHandleComplete,
                                     const void* context);

        // Common initialization for new CallbackInfo object.
        void CommonInitializeCallbackInfo(CallbackInfo::Ptr callbackInfo);

    private:
        // Mutex for synchronization use.
        std::mutex m_callbackMutex;

        // Mutex used for synchronizing discovery callback to app.
        std::mutex m_discoverDeviceCallbackMutex;

        // Table of CallbackInfo.  Key is autogenerated.
        std::map<size_t, CallbackInfo::Ptr> m_callbackInfoList;  // List of expected callbacks.
        AppPtr m_app; // Callback object is per app.
        volatile bool m_stopCalled;    // Set to true when Stop() is called.

        // Number of expired callbacks in progress.
        size_t m_expiredCallbacksInProgress;

        // Indicate that callback is in progress for callbackInfo matching mapKey.
        // Return false if the callback is already cancelled by app.
        bool SetCallbackInProgress(size_t mapKey);

        // Indicate that callback is not in progress.
        bool ClearCallbackInProgress(size_t mapKey);
};

#endif // CALLBACK_H_
