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

#ifndef DEVICE_H_
#define DEVICE_H_

class App;
class OCFFramework;

typedef std::shared_ptr<App> AppPtr;

/* Type of resource info requested in GetResourceInfo() */
enum class ResourceInfoType
{
    ResourceType,
    ResourceInterface
};

class Device
{
    public:
        typedef std::shared_ptr<Device> Ptr;

        Device(const char* deviceId, OCFFramework* ocf, AppPtr app);
        ~Device();

        AppPtr GetApp() { return m_app; }
        std::string GetDeviceId() { return m_deviceId; }

        // Make sure OCFFramework has found this device and let it know that it's opened.
        IPCAStatus Open();
        IPCAStatus Close();

        IPCAStatus GetDeviceInfo(IPCADeviceInfo** callerDeviceInfo);
        IPCAStatus GetPlatformInfo(IPCAPlatformInfo** callerPlatformInfo);

        // Return array of resource paths (examples of resource paths: "/oic/d", "/oem/elevator").
        IPCAStatus GetResourcePathList(const std::string& resourceInterface,
                            const std::string& resourceType,
                            char*** resourcePathList,
                            size_t* resourcePathCount);

        // Depending on resourceInfoType, return an array of:
        //    Resource types (examples of resource types: "oic.wk.d", "x.oem.elevator.status").
        //    Resource interfaces (e.g., "oic.if.baseline", "oic.if.r").
        IPCAStatus GetResourceInfo(const char* resourcePath,
                            ResourceInfoType resourceInfoType,
                            char*** stringArray,
                            size_t* stringCount);

        // Send Get request to device.
        IPCAStatus GetProperties(CallbackInfo::Ptr callbackInfo);

        // Send Post request to device.
        IPCAStatus SetProperties(CallbackInfo::Ptr callbackInfo, OC::OCRepresentation* rep);

        // Subscribe to resource change notifications.
        IPCAStatus ObserveResource(CallbackInfo::Ptr callbackInfo);
        void StopObserve(CallbackInfo::Ptr callbackInfo);
        void IsResourceObservable(const char* resourcePath, bool* isObservable);

        // Send Post for IPCA_RELATIVE_RESOURCE_PATH and Put for IPCA_EXPLICIT_RESOURCE_PATH.
        IPCAStatus CreateResource(CallbackInfo::Ptr callbackInfo, OC::OCRepresentation* rep);

        // Send Delete request to device.
        IPCAStatus DeleteResource(CallbackInfo::Ptr callbackInfo);

        // Ping device to determine that it's still there.
        // If device responds, OCFFramework will update the device's lastResponseTimeToDiscovery.
        IPCAStatus Ping();
        IPCAStatus GetLastPingTime(uint64_t& lastPingTime);

        // Request access to a device
        IPCAStatus RequestAccess(CallbackInfo::Ptr callbackInfo,
                        CallbackInfo::Ptr passwordInputCallbackInfo);

    private:
        IPCAStatus ToArrayOfCharPointers(std::vector<std::string>& stringList,
                        char*** callerPointer, size_t* arraySize);

    private:
        std::string m_deviceId;   // from IPCAOpenDevice()
        AppPtr m_app;
        OCFFramework* m_ocfFramework;
        bool m_isClosed;  // set to true after Close().
};

// returned as IPCAHandle to the app.
typedef struct
{
    AppPtr app;
    Device::Ptr device;
} DeviceWrapper;

#endif // DEVICE_H_
