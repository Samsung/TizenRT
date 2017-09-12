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

#ifndef OCF_FRAMEWORK_H_
#define OCF_FRAMEWORK_H_

using namespace OC;

struct IPCAAppInfoInternal;

// Map's key is resource path obtained from resource->uri() (e.g.: /oic/d).
typedef std::map<std::string, std::shared_ptr<OCResource>> ResourceMap;

// Some information about an OCF device.
typedef struct DeviceDetails
{
    typedef std::shared_ptr<DeviceDetails> Ptr;

    // Value is set to value returned by OICGetCurrentTime(TIME_IN_MS).
    uint64_t lastResponseTimeToDiscovery;

    // Indicate device not responding 1 time.
    bool deviceNotRespondingIndicated;

    // How many IPCAOpenDevice() on this device.
    int deviceOpenCount;

    // Timestamp of final close.
    uint64_t lastCloseDeviceTime;

    // Timestamp of last ping call to device.
    uint64_t lastPingTime;

    // Device ID in OnResourceFound().
    std::string deviceId;

    // Device Uri in OnResourceFound().  A device can have multiple device uris
    // (e.g. ipv4 and ipv6).
    std::vector<std::string> deviceUris;

    // Device info
    size_t deviceInfoRequestCount; // Number of requests sent for /oic/d
    bool deviceInfoAvailable;
    InternalDeviceInfo deviceInfo;

    // Platform info
    size_t platformInfoRequestCount;   // Number of requests sent for /oic/p
    bool platformInfoAvailable;
    InternalPlatformInfo platformInfo;

    // Maintenance resource.
    size_t maintenanceResourceRequestCount; // Number of requests sent for /oic/mnt
    bool maintenanceResourceAvailable; // Set to true if device returns resource for rt: oic.wk.mnt.

    // Security Info
    bool securityInfoAvailable;
    InternalSecurityInfo securityInfo;

    // List of resources, not necessarily complete list, depending on the resource type requested
    // in the discovery.
    ResourceMap resourceMap;

    // List of resource types, not necessarily a complete list, depending on the resource type
    // in discovery.
    std::vector<std::string> discoveredResourceTypes;

    // List of resource interfaces, not necessarily a complete list, depending on the resource
    // type in discovery.
    std::vector<std::string> discoveredResourceInterfaces;
} DeviceDetails;

typedef struct RequestAccessContext
{
    std::string deviceId;
    OCFFramework* ocfFramework;
    CallbackInfo::Ptr callbackInfo;
    CallbackInfo::Ptr passwordInputCallbackInfo;
} RequestAccessContext;

// Implements OCF related functions.
class OCFFramework
{
    public:
        OCFFramework();
        ~OCFFramework();

        // When app calls IPCAOpen()/IPCAClose().
        IPCAStatus Start(const IPCAAppInfoInternal& ipcaAppInfo, bool isUnitTestMode);
        IPCAStatus Stop(InputPinCallbackHandle passwordInputCallbackHandle,
                        DisplayPinCallbackHandle passwordDisplayCallbackHandle);

        IPCAStatus RegisterAppCallbackObject(Callback::Ptr cb);
        void UnregisterAppCallbackObject(Callback::Ptr cb);

        // Discover on network, resources that match resource type.
        IPCAStatus DiscoverResources(std::vector<std::string>& resourceTypeList);

        // Discover all the resources for specific device.
        IPCAStatus DiscoverAllResourcesGivenHost(std::string deviceUri);

        // App calls IPCAOpenDevice() on deviceId.
        IPCAStatus IPCADeviceOpenCalled(std::string& deviceId);
        IPCAStatus IPCADeviceCloseCalled(std::string& deviceId);

        // Information pertaining to the device.
        IPCAStatus CopyDeviceInfo(std::string& deviceId, IPCADeviceInfo** callerDeviceInfo);
        IPCAStatus CopyPlatformInfo(std::string& deviceId, IPCAPlatformInfo** callerPlatformInfo);
        static void FreeDeviceInfo(IPCADeviceInfo* deviceInfo);
        static void FreePlatformInfo(IPCAPlatformInfo* platformInfo);

        // Resource and resource information.
        IPCAStatus CopyResourcePaths(
                        const std::string& resourceInterface,
                        const std::string& resourceType,
                        std::string& deviceId,
                        std::vector<std::string>& resourcePathList);

        IPCAStatus CopyResourceInfo(
                        const std::string& deviceId,
                        const std::string& resourcePath,
                        ResourceInfoType resourceInfoType,
                        std::vector<std::string>& resourceInfo);

        // Get/Set/Observe requests.
        IPCAStatus SendCommandToDevice(
                        std::string& deviceId,
                        CallbackInfo::Ptr callbackInfo,
                        OCRepresentation* rep);

        IPCAStatus GetProperties(std::string& deviceId, CallbackInfo::Ptr callbackInfo);

        IPCAStatus SetProperties(std::string& deviceId,
                        CallbackInfo::Ptr callbackInfo,
                        OCRepresentation* rep);

        IPCAStatus CreateResource(std::string& deviceId,
                        CallbackInfo::Ptr callbackInfo,
                        OCRepresentation* rep);

        IPCAStatus StartObserve(std::string& deviceId, CallbackInfo::Ptr callbackInfo);
        void StopObserve(CallbackInfo::Ptr callbackInfo);

        void IsResourceObservable(std::string& deviceId,
                        const char* resourcePath,
                        bool* isObservable);

        // Ping specific device for the given resource type.
        // The function sends oic/res request with rt = oic.wk.d.
        IPCAStatus PingDevice(std::string& deviceId);
        IPCAStatus GetLastPingTime(std::string& deviceId, uint64_t& lastPingTime);

        // Security requests.
        IPCAStatus SetInputPasswordCallback(CallbackInfo::Ptr callbackInfo,
                                            InputPinCallbackHandle* passwordInputCallbackHandle);

        IPCAStatus SetDisplayPasswordCallback(CallbackInfo::Ptr callbackInfo,
                        DisplayPinCallbackHandle* passwordDisplayCallbackHandle);

        IPCAStatus RequestAccess(std::string& deviceId,
                        CallbackInfo::Ptr callbackInfo,
                        CallbackInfo::Ptr passwordInputCallbackInfo);

    private:
        // Callback from OCF for OCResource->get()
        void OnObserve(const HeaderOptions headerOptions,
                const OCRepresentation &rep,
                const int &eCode,
                const int &sequenceNumber,
                CallbackInfo::Ptr callbackInfo);

        void OnGet(const HeaderOptions& headerOptions,
                const OCRepresentation& rep,
                const int eCode,
                CallbackInfo::Ptr callbackInfo);

        void OnDelete(const HeaderOptions& headerOptions,
                const int eCode,
                CallbackInfo::Ptr callbackInfo);

        void OnPostPut(const HeaderOptions& headerOptions,
                const OCRepresentation& rep,
                const int eCode,
                CallbackInfo::Ptr callbackInfo);

        // Callback from OCF when a resource is found.
        void OnResourceFound(std::shared_ptr<OCResource> resource);

        // Callback from OCF for requested device info.
        void OnDeviceInfoCallback(const OCRepresentation& rep);

        // Callback from OCF for requested platform info.
        void OnPlatformInfoCallback(const OCRepresentation& rep);

        // Callback from OCF for MOT completion
        void OnMultipleOwnershipTransferCompleteCallback(PMResultList_t* result,
                    int error,
                    std::string deviceId,
                    CallbackInfo::Ptr callbackInfo);

        // Callback from OCF for password input
        void OnPasswordInputCallback(OicUuid_t deviceId,
                    char* passwordBuffer,
                    size_t passwordBufferSize,
                    CallbackInfo::Ptr callbackInfo);

        // Callback from OCF to display a password
        void OnPasswordDisplayCallback(char* passwordBuffer,
                    size_t passwordBufferSize,
                    CallbackInfo::Ptr callbackInfo);

        // A device is formed when all its device info and platform info are known.
        IPCAStatus GetCommonResources(DeviceDetails::Ptr deviceDetails);

        // See m_workerThread variable below.
        static void WorkerThread(OCFFramework* ocfFramework);

        // Entry point for the thread that will request access to a device.
        static void RequestAccessWorkerThread(RequestAccessContext* requestContext);

        // Get DeviceDetails for deviceId.
        IPCAStatus FindDeviceDetails(const std::string& deviceId,
                    DeviceDetails::Ptr& deviceDetails);

        // Get the first OCResource that implements the resource type.
        std::shared_ptr<OCResource> FindOCResource(const DeviceDetails::Ptr& deviceDetails,
                    const std::string& resourcePath,
                    const std::string& resourceType);

        // Cleanup RequestAccess calls
        void CleanupRequestAccessDevices();

        // Some helper functions to help debugging.
        void DebugOutputOCFDevices();
        void DebugOutputOCRep(const OCRepresentation& rep);

        // Thread safe copy.
        template <typename _T>
        void ThreadSafeCopy(const _T& source, _T& dest);

        // Helper functions to set platform & device info.
        IPCAStatus SetPlatformInfo(const OCPlatformInfo* platformInfo);
        IPCAStatus SetDeviceInfo(const OCDeviceInfo* deviceInfo);

    private:
        // Lock for sync access to protected members in OCFFramework.
        std::recursive_mutex m_OCFFrameworkMutex;

        // A list of devices and their resources.
        // Key to the map is device ID (which is a UUID)
        std::map<std::string, DeviceDetails::Ptr> m_OCFDevices;

        // Fast look up for DeviceDetails given device's URI.
        // Key to the map is device URI (e.g., "coap://[fe80::5828:93a8:d53e:4222%7]:62744").
        // A device may have multiple URIs (e.g., ipv4 and ipv6).
        std::map<std::string, DeviceDetails::Ptr> m_OCFDevicesIndexedByDeviceURI;

        // A list of RequestAccess contexts.
        // Key to the map is the device ID (which is a UUID)
        std::map<std::string, RequestAccessContext*> m_OCFRequestAccessContexts;

        // One Callback per App. One App per IPCAOpen().
        std::vector<Callback::Ptr> m_callbacks;
        std::thread m_workerThread;
        std::condition_variable m_workerThreadCV;
        std::mutex m_workerThreadMutex;

        // Synchronize Start()/Stop()
        std::mutex m_startStopMutex;
        bool m_isStarted;
        bool m_isStopping;
};
#endif // OCF_FRAMEWORK_H_
