/******************************************************************
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

#ifndef IPCA_H_  // IoTivity Procedural Client API
#define IPCA_H_

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * A physical device (piece of plastic) is represented by a platform object with a UUID identifier
 *
 * A logical device is a virtual device that runs on the platform and is represented by a UUID
 * identifier. Multiple logical devices can run on a platform.
 *
 * A logical device is made up of one or more resources. Each resource is identified by a "/"
 * separated string which is unique within the device.
 *
 * A resource implements one or more resource types. A resource type is identified by a "."
 * separated string which is unique within the resource.
 *
 * Finally each resource type has a number of properties. Each property is represented by a pair of
 * key and value.
 *
 * Below is an example of a physical device with one logical device. The logical device implements
 * multiple resources as follows:
 *   1. Base platform resources (/oic/p, /oic/d) [note: refer to the OCF spec for a complete set of
 *      mandatory resources a device implements]
 *   2. OEM Elevator resource
 *   3. OEM Display screen resource
 *
 * Platform (id: UUID)
 * |
 * + Device (id: UUID)
 *     |
 *     + Resource (name: "/oic/d")
 *     |   |
 *     |   + Resource Type (name: "oic.wk.d", interface: "oic.if.r")
 *     |       |
 *     |       + Property (key: "piid", type: string,   access: read-only)
 *     |       + Property (key: "di",   type: string,   access: read-only)
 *     |       + Property (key: "n",    type: string,   access: read-only)
 *     |       + Property (key: "icv",  type: string,   access: read-only)
 *     |       + Property (key: "dmv",  type: string,   access: read-only)
 *     |
 *     + Resource (name: "/oic/p")
 *     |   |
 *     |   + Resource Type (name: "oic.wk.p", interface: "oic.if.r")
 *     |       |
 *     |       + Property (key: "pi",     type: string,   access: read-only)
 *     |       + Property (key: "mnmn",   type: string,   access: read-only)
 *     |       + Property (key: "mnmo",   type: string,   access: read-only)
 *     |       + Property (key: "mnos",   type: string,   access: read-only)
 *     |       + Property (key: "mnfv",   type: string,   access: read-only)
 *     |       + ...
 *     |
 *     + Resource ("/oem/elevator")
 *     |   |
 *     |   + Resource Type ("x.oem.elevator.status", interface: "oic.if.r")
 *     |   |   |
 *     |   |   + Property ("x.oem.CurrentFloor",  type: integer,  access: read-only)
 *     |   |   + Property ("x.oem.Direction",     type: integer,  access: read-only)
 *     |   |
 *     |   + Resource Type ("x.oem.elevator.operation", interface: "oic.if.a")
 *     |   |   |
 *     |   |   + Property ("x.oem.TargetFloor"    type: integer,  access: read-write)
 *     |   |
 *     |   + Resource Type ("x.oem.elevator.maintenance", interface: "oic.if.r")
 *     |       |
 *     |       + Property ("x.oem.MotorTemperature", type: integer,  access: read-only)
 *     |       + Property ("x.oem.PulleyTension",    type: integer,  access: read-only)
 *     |
 *     + Resource ("/oem/display")
 *         |
 *         + Resource Type ("x.oem.display.screen", interface: "oic.if.r, oic.if.w")
 *             |
 *             + Property ("x.oem.DisplayText",        type: string,   access: read-write)
 *             + Property ("x.oem.DisplayBackground",  type: string,   access: read-write)
 */

#ifdef OC_CALL
#   define IPCA_CALL    OC_CALL
#else
#   ifdef _WIN32
        /*
         * Set to __stdcall for Windows, consistent with WIN32 APIs.
         */
#       define IPCA_CALL    __stdcall
#   else
#       define IPCA_CALL
#   endif
#endif

/**
 * Handle types returned by IPCA.
 */
typedef struct IPCAAppHandleStruct* IPCAAppHandle;
typedef struct IPCACallbackHandleStruct* IPCACallbackHandle;
typedef struct IPCADeviceHandleStruct* IPCADeviceHandle;
typedef struct IPCAPropertyBagHandleStruct* IPCAPropertyBagHandle;
typedef struct IPCAHandleStruct* IPCAHandle;

struct IPCAUuid
{
    uint8_t uuid[16];
};

/**
 * Some information about the device from device discovery.
 * Use IPCAGetDeviceInfo() for detailed device information.
 *
 * deviceId         UUID of the device. The value is as defined in OCF 1.0 Security Specification
 *                  for DeviceID.
 *                  An example of UUID string: d99c2350-d95e-11e6-bf26-cec0c932ce01.
 * deviceUris       An array of device URIs (e.g., coap://[fe80::c0bd:f33:6783:78fe%2]:49555).
 *                  A device can have multiple endpoints.
 * deviceUriCount   Number of device URIs in the deviceUris array.
 * deviceName       Human friendly name of the device.
 */
typedef struct
{
    const char*   deviceId;
    const char**  deviceUris;
    size_t        deviceUriCount;
    const char*   deviceName;
} IPCADiscoveredDeviceInfo;

/**
 * The IPCA version.
 */
typedef enum
{
    IPCA_VERSION_1 = 1
} IPCAVersion;

/**
 * Device information.
 *
 * version                      The IPCA version of the IPCADeviceInfo structure matching version
 *                              requested in IPCAOpen.
 * protocolIndependentId        Protocol independent ID of the device. This is in UUID format.
 *                              An example of UUID string: d99c2350-d95e-11e6-bf26-cec0c932ce01.
 * deviceId                     UUID of the device. The value is as defined in OCF 1.0 Security
 *                              Specification for DeviceID.
 * deviceUris                   An array of of device URIs
 *                              (e.g., coap://[fe80::c0bd:f33:6783:78fe%2]:49555).
 *                              A device can have multiple endpoints.
 * deviceUriCount               Number of device URIs in the deviceUris array.
 * deviceName                   Human friendly name of the device.
 * deviceSpecVersion            Spec version of the the core specification this device is
 *                              implemented to.
 * dataModelVersions            Array of spec versions of the vertical specifications.
 * dataModelVersionCount        Number of spec versions in dataModelVersions array.
 * localizedDescriptions        Detailed description of the device in one or more languages. Each
 *                              object has a 'language' key field and 'value' field.
 * deviceSoftwareVersion        Version of device software.
 * manufacturerName             Name of manufacturer of device. Each object has a 'language' key
 *                              field and manufacturer name 'value' field.
 * modelNumber                  Model number as designated by manufacturer.
 */
typedef struct
{
    IPCAVersion             version;
    const char*             protocolIndependentId;
    const char*             deviceId;
    const char**            deviceUris;
    size_t                  deviceUriCount;
    const char*             deviceName;
    const char*             deviceSpecVersion;
    const char**            dataModelVersions;
    size_t                  dataModelVersionCount;
    IPCAPropertyBagHandle   localizedDescriptions;
    const char*             deviceSoftwareVersion;
    IPCAPropertyBagHandle   manufacturerNames;
    const char*             modelNumber;
} IPCADeviceInfo;

/**
 * Platform information.
 *
 * version                 The IPCA version of the IPCAPlatformInfo structure matching version
 *                         requested in IPCAOpen.
 * platformId              UUID of platform identifier.
 *                         An example of UUID string: d99c2350-d95e-11e6-bf26-cec0c932ce01.
 * manufacturerName        Name of the manufacturer of the platform.
 * manufacturerURL         Manufacturer's URL.
 * modelNumber             Model number as defined by the manufacturer.
 * manufacturingDate       Manufacturing date.
 * platformVersion         Platform version.
 * osVersion               Platform resident OS version.
 * hardwareVersion         Platform hardware version.
 * firmwareVersion         Platform firmware version.
 * manufacturerSupportURL  Manufacturer's support information URL.
 * referenceTime           Reference time for the device.
 */
typedef struct
{
    IPCAVersion      version;
    const char*      platformId;
    const char*      manufacturerName;
    const char*      manufacturerURL;
    const char*      modelNumber;
    const char*      manufacturingDate;
    const char*      platformVersion;
    const char*      osVersion;
    const char*      hardwareVersion;
    const char*      firmwareVersion;
    const char*      manufacturerSupportURL;
    const char*      referenceTime;
} IPCAPlatformInfo;

/**
 * Application's information.
 *
 * appId               UUID of the app.
 * appName             Friendly name of the app.
 * appSoftwareVersion  Software version of the app.
 * appCompanyName      Name of the software company that produced this app.
 *
 */
typedef struct
{
    IPCAUuid        appId;
    const char*     appName;
    const char*     appSoftwareVersion;
    const char*     appCompanyName;
} IPCAAppInfo;

/**
 * IPCA status.
 */
typedef enum
{
    IPCA_OK = 0,                            // Successful.
    IPCA_FAIL,                              // Generic error status.
    IPCA_ALREADY_OPENED,                    // IPCAOpen() is already called.
                                            // Use IPCAClose() to close the IPCA session.
    IPCA_INVALID_ARGUMENT,                  // Invalid parameters passed to IPCA methods.
    IPCA_INVALID_GUID,                      // Invalid GUID string.
    IPCA_OUT_OF_MEMORY,                     // Out of memory error.
    IPCA_INFO_VERSION_NOT_SUPPORTED,        // Requested info version is not supported.

    // Device related
    IPCA_DEVICE_APPEAR_OFFLINE = 0x1000,    // Device is either offline or not responding and
                                            // assumed offline.
    IPCA_DEVICE_NOT_DISCOVERED,             // Device is not discovered by IPCADiscoverDevices().
    IPCA_INFORMATION_NOT_AVAILABLE,         // Information is not available, caller can retry in a
                                            // few seconds.

    // Resource related
    IPCA_RESOURCE_NOT_FOUND = 0x2000,       // No matching resource.
    IPCA_RESOURCE_CREATED,                  // Device created new resource.
    IPCA_RESOURCE_DELETED,                  // Device deleted resource.
    IPCA_REQUEST_TIMEOUT,                   // No response from server.

    // Resource type related
    IPCA_PROPERTY_NOT_SUPPORTED = 0x3000,   // When property is not mandatory in a resource type,
                                            // e.g., Factory_Reset in oic.wk.mnt.

    // Security related
    IPCA_ACCESS_DENIED = 0x4000,            // The requested access to a device has failed due to
                                            // the current security settings.
                                            // The application can then call IPCARequestAccess, to
                                            // request updated settings.

    IPCA_SECURITY_UPDATE_REQUEST_FINISHED,  // Security settings have been updated. Re-attemping an
                                            // operation that previously failed with status
                                            // IPCA_ACCESS_DENIED might yield a different result.

    IPCA_SECURITY_UPDATE_REQUEST_INCORRECT_PASSWORD,    // Updating the security settings failed
                                                        // due to an incorrect password.

    IPCA_SECURITY_UPDATE_REQUEST_NOT_SUPPORTED, // Updating the security settings corresponding to
                                                // a device is not supported.

    IPCA_SECURITY_UPDATE_REQUEST_FAILED,        // Updating the security settings has been
                                                // attempted, but failed.
} IPCAStatus;

/**
 * Callback from IPCA to application when device sends a resource change notification.
 * To receive resource change notifications, application calls IPCAObserveResource() to set
 * observation on a resource.
 *
 * @param[in] result              Result of the call.
 * @param[in] context             Caller's context set in IPCAObserveResource().
 * @param[in] propertyBagHandle   Handle to an IPCAPropertyBag object.
 *                                Use IPCAPropertyBag APIs to retrieve the values indexed by keys.
 *                                propertyBagHandle is only valid during this callback.
 */
typedef void (IPCA_CALL *IPCAResourceChangeCallback)(
                                IPCAStatus result,
                                void* context,
                                IPCAPropertyBagHandle propertyBagHandle);

/**
 * Callback from IPCA to application when device responds to IPCAGetProperties().
 *
 * @param[in] result              Result of the call.
 * @param[in] context             Caller's context set in IPCAGetProperties().
 * @param[in] propertyBagHandle   Handle to an IPCAPropertyBag bag object.
 *                                Use IPCAPropertyBag APIs to retrieve the values indexed by keys.
 *                                propertyBagHandle is only valid during this callback.
 */
typedef void (IPCA_CALL *IPCAGetPropertiesComplete)(
                                IPCAStatus result,
                                void* context,
                                IPCAPropertyBagHandle propertyBagHandle);
/**
 * Callback from IPCA to application when device responds to IPCASetProperties().
 *
 * @param[in] result              Result of the call.
 * @param[in] context             Caller's context set in IPCASetProperties().
 * @param[in] propertyBagHandle   Handle to an IPCAPropertyBag object.
 *                                Use IPCAPropertyBag APIs to retrieve the values indexed by keys.
 *                                propertyBagHandle is only valid during this callback.
 */
typedef void (IPCA_CALL *IPCASetPropertiesComplete)(
                                IPCAStatus result,
                                void* context,
                                IPCAPropertyBagHandle propertyBagHandle);

/**
 * Callback from IPCA to application when device responds to IPCACreateResource().
 *
 * @param[in] result              Result of the call.
 * @param[in] context             Caller's context set in IPCACreateResource().
 * @param[in] newResourcePath     The relative path of the newly created resource in the collection.
 * @param[in] propertyBagHandle   Handle to an IPCAPropertyBag object.
 *                                Use IPCAPropertyBag APIs to retrieve the values indexed by keys.
 *                                propertyBagHandle is only valid during this callback.
 *
 */
typedef void (IPCA_CALL *IPCACreateResourceComplete)(
                                IPCAStatus result,
                                void* context,
                                const char* newResourcePath,
                                IPCAPropertyBagHandle propertyBagHandle);

/**
 * Callback from IPCA to application when device responds to IPCADeleteResource().
 *
 * @param[in] result              Result of the call.
 * @param[in] context             Caller's context set in IPCADeleteResource().
 *
 */
typedef void (IPCA_CALL *IPCADeleteResourceComplete)(IPCAStatus result, void* context);

/**
 * Callback from IPCA when a handle is completely closed.
 *
 * @param[in] context   Caller's context set in IPCACloseHandle().
 *
 */
typedef void (IPCA_CALL *IPCACloseHandleComplete)(void* context);


/**
 * Discovery status in IPCADiscoverDeviceCallback.
 */
typedef enum
{
    IPCA_DEVICE_DISCOVERED = 1,         /* Device responded to discovery request. */
    IPCA_DEVICE_UPDATED_INFO,           /* There's an update in discoveredDeviceInfo. */
    IPCA_DEVICE_STOPPED_RESPONDING,     /* Device stopped responding to discovery request. */
} IPCADeviceStatus;


/**
 * Callback when a device that implements resourceTypeList in IPCADiscoverDevices() is either
 * discovered or has stopped responding to discovery request.
 *
 * @param[in] context                Application's context in IPCADiscoverDevices().
 * @param[in] deviceStatus           See IPCADeviceStatus.
 * @param[in] discoveredDeviceInfo   Some information of device.
 */

typedef void (IPCA_CALL *IPCADiscoverDeviceCallback)(
                                void* context,
                                IPCADeviceStatus deviceStatus,
                                const IPCADiscoveredDeviceInfo* discoveredDeviceInfo);

/**
 * An application calls this method one time to register with IPCA.
 *
 * @param[in]  ipcaAppInfo     Application's information.  See IPCAAppInfo.
 * @param[in]  ipcaVersion     The IPCAVersion the application can work with.
 * @param[out] ipcaAppHandle   Handle returned by IPCA. Use IPCAClose() to close the handle.
 *
 * @return IPCA_OK if successful.
 */
IPCAStatus IPCA_CALL IPCAOpen(
                        const IPCAAppInfo* ipcaAppInfo,
                        IPCAVersion ipcaVersion,
                        IPCAAppHandle* ipcaAppHandle);

/**
 * Close the handle returned in IPCAOpen().
 * After this call returns, application should consider any outstanding calls to devices canceled
 * and opened handles closed.
 *
 * @param[in] ipcaAppHandle  Handle returned in IPCAOpen().
 */
void IPCA_CALL IPCAClose(IPCAAppHandle ipcaAppHandle);


/**
 * Application calls this method to start discovery of devices that implement resource types
 * specified in resourceTypeList.
 * The underlying framework triggers discovery request periodically until caller cancels the
 * request using IPCACloseHandle().
 *
 * @param[in]  ipcaAppHandle         Application handle returned in IPCAOpen().
 * @param[in]  callback              Callback from IPCA when devices are found.
 * @param[in]  context               Application's context that is passed back as argument in the
 *                                   callback.
 * @param[in]  resourceTypeList      A list of resource types, a device must implement all of them
 *                                   to be discovered. Empty string indicates any resource type.
 * @param[in]  resourceTypeCount     Number of resource types pointed by resourceTypeList.
 * @param[out] handle                Handle that can be used in IPCACloseHandle() to stop device
 *                                   discovery.
 *                                   Null handle pointer is not allowed.
 *
 * @return IPCA_OK if successful.
 */
IPCAStatus IPCA_CALL IPCADiscoverDevices(
                            IPCAAppHandle ipcaAppHandle,
                            IPCADiscoverDeviceCallback discoverDeviceCallback,
                            void* context,
                            const char* const* resourceTypeList,
                            int resourceTypeCount,
                            IPCAHandle* handle);

/**
 * Application indicates to IPCA the intention to start working with a device whose ID matches
 * deviceID.
 *
 * @param[in] ipcaAppHandle   Application handle returned in IPCAOpen().
 * @param[in] deviceId        UUID of device (see: IPCADeviceInfo).
 * @param[out] deviceHandle   Handle to the device. Use IPCACloseDevice() to close device handle.
 *
 * @return IPCA_OK if successful. IPCA_DEVICE_NOT_DISCOVERED if device is not yet discovered, call
 *         IPCADiscoverDevices().
 */
IPCAStatus IPCA_CALL IPCAOpenDevice(
                            IPCAAppHandle ipcaAppHandle,
                            const char* deviceId,
                            IPCADeviceHandle* deviceHandle);

/**
 * Closes device handle. The application will stop receiving notifications and callbacks from the
 * device.
 *
 * @param[in] deviceHandle  device handle returned in IPCAOpenDevice().
 */
void IPCA_CALL IPCACloseDevice(IPCADeviceHandle deviceHandle);

/**
 * This method returns a pointer to IPCADeviceInfo structure.
 *
 * @param[in]  deviceHandle       Device handle returned in IPCAOpenDevice().
 * @param[out] deviceInfo         IPCA returns a pointer to IPCADeviceInfo structure.
 *                                Use IPCAFreeDeviceInfo() to free the memory.
 *
 * @return
 *   IPCA_OK if successful.
 *   IPCA_INFORMATION_NOT_AVAILABLE if server has not returned the device info query.
 *   IPCA_INFO_VERSION_NOT_SUPPORTED when requested info version is not supported.
 */
IPCAStatus IPCA_CALL IPCAGetDeviceInfo(IPCADeviceHandle deviceHandle, IPCADeviceInfo** deviceInfo);

/**
 * Free the memory allocated in IPCAGetDeviceInfo().
 *
 * @param[in] deviceInfo   a Pointer to IPCADeviceInfo structure returned in IPCAGetDeviceInfo().
 */
void IPCA_CALL IPCAFreeDeviceInfo(IPCADeviceInfo* deviceInfo);

/**
 * This method returns a pointer to IPCAPlatformInfo structure.
 *
 * @param[in]  deviceHandle        Device handle returned in IPCAOpenDevice().
 * @param[out] platformInfo        IPCA returns a pointer to IPCAPlatformInfo structure.
 *                                 Use IPCAFreePlatformInfo() to free the memory.
 *
 * @return
 *   IPCA_OK if successful.
 *   IPCA_INFORMATION_NOT_AVAILABLE if server has not returned the platform info query.
 *   IPCA_INFO_VERSION_NOT_SUPPORTED when requested info version is not supported.
 */
IPCAStatus IPCA_CALL IPCAGetPlatformInfo(
                            IPCADeviceHandle deviceHandle,
                            IPCAPlatformInfo** platformInfo);

/**
 * Free the memory allocated in IPCAGetPlatformInfo().
 *
 * @param[in] platformInfo   A pointer to IPCAPlatformInfo structure returned
 *                           in IPCAGetPlatformInfo().
 */
void IPCA_CALL IPCAFreePlatformInfo(IPCAPlatformInfo* platformInfo);

/**
 * Get the resources that implement resource types in IPCADiscoverDevices().
 * Application must call IPCAFreeStringArray() to free memory allocated for resourcePathList.
 *
 * @param[in]  deviceHandle       Device handle returned in IPCAOpenDevice().
 * @param[in]  resourceInterface  Interface the resource must support. Use NULL or empty string
 *                                for any interface.
 * @param[in]  resourceType       Resource type the resource must implement. Use NULL or empty
 *                                string for any resource type.
 * @param[out] resourcePathList   A pointer to an array of resource path, a percent encoded URI
 *                                reference, that IPCA returns.
 * @param[out] resourcePathCount  Number of resource paths returned.
 *
 * @return IPCA_OK if successful.
 */
IPCAStatus IPCA_CALL IPCAGetResources(
                            IPCADeviceHandle deviceHandle,
                            const char* resourceInterface,
                            const char* resourceType,
                            char*** resourcePathList,
                            size_t* resourcePathCount);

/**
 * Get the list of resource types implemented by a resource.
 * Application must call IPCAFreeStringArray() to free memory allocated for resourceTypeList.
 *
 * @param[in] deviceHandle         Device handle returned in IPCAOpenDevice().
 * @param[in] resourcePath         Resource path, a percent encoded URI reference, e.g., "/oic/d".
 *                                 If resourcePath is equal to NULL or empty string, IPCA returns
 *                                 all resource types.
 * @param[out] resourceTypeList    A pointer to an array of resource types that IPCA returns.
 * @param[out] resourceTypeCount   Number of resource types returned.
 *
 * @return IPCA_OK if successful.
 */
IPCAStatus IPCA_CALL IPCAGetResourceTypes(
                            IPCADeviceHandle deviceHandle,
                            const char* resourcePath,
                            char*** resourceTypeList,
                            size_t* resourceTypeCount);

/**
 * Get the list of resource interfaces implemented by a resource.
 * Application must call IPCAFreeStringArray() to free memory allocated for resourceInterfaceList.
 *
 * @param[in] deviceHandle              Device handle returned in IPCAOpenDevice().
 * @param[in] resourcePath              Resource path, a percent encoded URI reference,
 *                                      e.g., "/oic/d".
 *                                      If resourcePath is equal to NULL or empty string,
 *                                      IPCA returns all interfaces the device supports.
 * @param[out] resourceInterfaceList    A pointer to an array of resource interfaces.
 * @param[out] resourceInterfaceCount   Number of resource interfaces returned.
 *
 * @return IPCA_OK if successful.
 */
IPCAStatus IPCA_CALL IPCAGetResourceInterfaces(
                            IPCADeviceHandle deviceHandle,
                            const char* resourcePath,
                            char*** resourceInterfaceList,
                            size_t* resourceInterfaceCount);

/**
 * Free string array returned in IPCAGetResources(), IPCAGetResourceTypes(),
 * IPCAGetResourceInterfaces().
 *
 * @param[in] stringArray  Array of strings.
 * @param[in] stringCount  Number of strings.
 *
 */
void IPCA_CALL IPCAFreeStringArray(char** stringArray, size_t stringCount);

/**
 * Get property values of a resource.
 *
 * @param[in]  deviceHandle             Device handle returned in IPCAOpenDevice().
 * @param[in]  getPropertiesComplete    Caller's callback when device responds to
 *                                      IPCAGetProperties()
 * @param[in]  context                  Application's context that is passed back as argument in the
 *                                      callback.
 * @param[in]  resourcePath             Resource which implements the resource type. E.g., "/oic/d".
 * @param[in]  resourceInterface        Resource interface to be used. Use NULL or empty string for
 *                                      default interface.
 * @param[in]  resourceType             The resource type which implements the properties.
 * @param[out] handle                   Handle that can be used in IPCACloseHandle() to cancel the
 *                                      callback.
 *                                      The reference count to the handle is handled by IPCA.
 *                                      Therefore, caller does not need to close the handle.
 *                                      Null handle pointer is allowed when caller does not plan to
 *                                      receive device's response.
 *
 * @return IPCA_OK if successful. IPCA calls getPropertiesComplete() when device responds.
 */
IPCAStatus IPCA_CALL IPCAGetProperties(
                            IPCADeviceHandle deviceHandle,
                            IPCAGetPropertiesComplete getPropertiesComplete,
                            void* context,
                            const char* resourcePath,
                            const char* resourceInterface,
                            const char* resourceType,
                            IPCAHandle* handle);

/**
 * Set property values of a resource.
 *
 * @param[in]  deviceHandle             Device handle returned in IPCAOpenDevice().
 * @param[in]  setPropertiesComplete    Caller's callback when device responds to
 *                                      IPCASetProperties()
 * @param[in]  context                  Application's context that is passed back as argument in the
 *                                      callback.
 * @param[in]  resourcePath             Resource path which implements the resource type.
 *                                      E.g., "/oic/d".
 * @param[in]  resourceInterface        Resource interface to be used. Use NULL or empty string for
 *                                      default interface.
 * @param[in]  resourceType             The resource type which implements the properties.
 * @param[in]  propertyBagHandle        Data to set (see IPCAPropertyBagCreate()).
 * @param[out] handle                   Handle that can be used in IPCACloseHandle() to cancel the
 *                                      callback.
 *                                      The reference count to the handle is handled by IPCA.
 *                                      Therefore, caller does not need to close the handle.
 *                                      Null handle pointer is allowed when caller does not plan to
 *                                      receive device's response.
 *
 * @return IPCA_OK if successful. IPCA calls setPropertiesComplete() when device reponds.
 */
IPCAStatus IPCA_CALL IPCASetProperties(
                            IPCADeviceHandle deviceHandle,
                            IPCASetPropertiesComplete setPropertiesComplete,
                            void* context,
                            const char* resourcePath,
                            const char* resourceInterface,
                            const char* resourceType,
                            IPCAPropertyBagHandle propertyBagHandle,
                            IPCAHandle* handle);

/**
 * Request device to create a new resource.
 *
 * @param[in]  deviceHandle             Device handle returned in IPCAOpenDevice().
 * @param[in]  createResourceComplete   Caller's callback when device responds to
 *                                      IPCACreateResource()
 * @param[in]  context                  Application's context that is passed back as argument in
 *                                      the callback.
 * @param[in]  resourceCollectionPath   Resource that handles resource creation.
 * @param[in]  resourceInterface        Resource interface to be used. Use NULL or empty string for
 *                                      default interface.
 * @param[in]  resourceType             The resource type which implements the properties.
 * @param[in]  propertyBagHandle        Data to set (see IPCAPropertyBagCreate()).
 * @param[out] handle                   Handle that can be used in IPCACloseHandle() to cancel the
 *                                      callback.
 *                                      The reference count to the handle is handled by IPCA.
 *                                      Therefore, caller does not need to close the handle.
 *                                      Null handle pointer is allowed when caller does not plan to
 *                                      receive device's response.
 *
 * @return IPCA_OK if successful. IPCA calls createResourceComplete() when device reponds.
 */
IPCAStatus IPCA_CALL IPCACreateResource(
                            IPCADeviceHandle deviceHandle,
                            IPCACreateResourceComplete createResourceComplete,
                            void* context,
                            const char* resourceCollectionPath,
                            const char* resourceInterface,
                            const char* resourceType,
                            IPCAPropertyBagHandle propertyBagHandle,
                            IPCAHandle* handle);

/**
 * Request device to delete a resource.
 *
 * @param[in]  deviceHandle             Device handle returned in IPCAOpenDevice().
 * @param[in]  deleteResourceComplete   Caller's callback when device responds to
 *                                      IPCADeleteResource()
 * @param[in]  context                  Application's context that is passed back as argument in
 *                                      the callback.
 * @param[in]  resourcePath             Resource path of resource to be deleted.
 * @param[out] handle                   Handle that can be used in IPCACloseHandle() to cancel the
 *                                      callback.
 *                                      The reference count to the handle is handled by IPCA.
 *                                      Therefore, caller does not need to close the handle.
 *                                      Null handle pointer is allowed when caller does not plan to
 *                                      receive device's response.
 *
 * @return IPCA_OK if successful. IPCA calls deleteResourceComplete() when device reponds.
 */

IPCAStatus IPCA_CALL IPCADeleteResource(
                            IPCADeviceHandle deviceHandle,
                            IPCADeleteResourceComplete deleteResourceComplete,
                            void* context,
                            const char* resourcePath,
                            IPCAHandle* handle);

/**
 * Function returns whether a resource is observable.
 *
 * @param[in]  deviceHandle    Device handle returned in IPCAOpenDevice().
 * @param[in]  resourcePath    Resource to check for observability.
 * @param[out] isObservable    Set to true if resourcePath is observable. Set to false otherwise.
 *
 * @return IPCA_OK if successful.
 */
void IPCA_CALL IPCAIsResourceObservable(
                            IPCADeviceHandle deviceHandle,
                            const char* resourcePath,
                            bool* isObservable);

/**
 * Function to start observing a resource. Use IPCACloseHandle() to stop receiving resource change
 * notifications from the device.
 *
 * @param[in]  deviceHandle             Device handle returned in IPCAOpenDevice().
 * @param[in]  resourceChangeCallback   Caller's callback when device sends a resource change
 *                                      notification.
 * @param[in]  context                  Application's context that is passed back as argument in the
 *                                      callback.
 * @param[in]  resourcePath             Resource to observe. E.g., "/oic/d".
 * @param[in]  resourceType             The resource type which implements the properties to
 *                                      observe.
 * @param[out] handle                   Handle that can be used in IPCACloseHandle() to stop
 *                                      observing the resource.
 *                                      Null handle pointer is not allowed.
 *
 * @return IPCA_OK if successful. IPCA calls resourceChangeCallback() when device sends
 *         notifications.
 */
IPCAStatus IPCA_CALL IPCAObserveResource(
                            IPCADeviceHandle deviceHandle,
                            IPCAResourceChangeCallback resourceChangeCallback,
                            void* context,
                            const char* resourcePath,
                            const char* resourceType,
                            IPCAHandle* handle);

/**
 * Stop receiving callbacks for the handle.
 * For handle returned by IPCAObserveResource(), IPCACloseHandle unsubscribes server's resource
 * change notification.
 *
 * @param[in] handle                Handle returned in the following methods:
 *                                  IPCAGetProperties(), IPCASetProperties(), IPCAObserveResource()
 *                                  IPCADiscoverDevices(), IPCACreateResource() and
 *                                  IPCARequestAccess().
 * @param[in] closeHandleComplete   Optional callback when handle is completely closed.
 *                                  Upon receiving this callback, an application can safely perform
 *                                  any needed cleanup for resources related to the handle closed.
 * @param[in] context               Application's context that is passed back as argument in the
 *                                  closeHandleComplete callback.
 *
 * @return IPCA_OK if successful.
 *         IPCA_FAIL if handle is already closed, closeHandleComplete will not be called.
 */
IPCAStatus IPCA_CALL IPCACloseHandle(IPCAHandle handle,
                                     IPCACloseHandleComplete closeHandleComplete,
                                     void* context);

/**
 * Perform factory reset.
 *
 * @param[in] deviceHandle   Device handle returned in IPCAOpenDevice().
 *
 * @return IPCA_OK if successful. IPCA_PROPERTY_NOT_SUPPORTED if device does not support factory
 *         reset.
 */
IPCAStatus IPCA_CALL IPCAFactoryReset(IPCADeviceHandle deviceHandle);

/**
 * Perform device warm reset.
 *
 * @param[in] deviceHandle    Device handle returned in IPCAOpenDevice().
 *
 * @return IPCA_OK if successful. IPCA_PROPERTY_NOT_SUPPORTED if device does not support reboot
 *         request.
 */
IPCAStatus IPCA_CALL IPCAReboot(IPCADeviceHandle deviceHandle);

/**
 * Create a handle to an IPCAPropertyBag object. Use IPCAPropertyBagDestroy() to delete the object.
 *
 * @param[out] propertyBagHandle   Handle to an IPCAPropertyBag object.
 *
 * @return IPCA_OK if successful.
 */
IPCAStatus IPCA_CALL IPCAPropertyBagCreate(IPCAPropertyBagHandle* propertyBagHandle);

/**
 * Delete an IPCPropertyBag object.
 *
 * @param[in] propertyBagHandle  Handle returned in IPCAPropertyBagCreate().
 */

void IPCA_CALL IPCAPropertyBagDestroy(IPCAPropertyBagHandle propertyBagHandle);

/**
 * Get the resource path of the property bag.
 * Use IPCAPropertyBagFreeString() to free string buffer returned in resourcePath.
 *
 * @param[in]  propertyBagHandle  IPCAPropertyBagHandle object returned by device.
 * @param[out] resourcePath       Buffer containing resource path, caller must use
 *                                IPCAPropertyBagFreeString() to free the buffer.
 *
 * @return IPCA_OK if successful.
 */
IPCAStatus IPCA_CALL IPCAPropertyBagGetResourcePath(
                                IPCAPropertyBagHandle propertyBagHandle,
                                char** resourcePath);

/**
 * Value type of a property in a property bag.
 */
typedef enum
{
    IPCA_INTEGER = 1,
    IPCA_DOUBLE,
    IPCA_BOOLEAN,
    IPCA_STRING,
    IPCA_ARRAY,
    IPCA_PROPERTY_BAG,
    IPCA_VALUE_TYPE_NOT_SUPPORTED
} IPCAValueType;

/**
 * Get the key and the value type of all the properties in a property bag.
 * Caller must free the memory allocated for the keys and valueTypes array returned.
 *
 * @param[in]  propertyBagHandle    IPCAPropertyBagHandle object returned by device.
 * @param[out] keys                 A pointer to an array of property names.
 * @param[out] valueTypes           A pointer to an array of property value types.
 *                                  Note: valueTypes[0] is the value type for property keys[0]
 *                                  and so on.
 * @param[out] count                Number of entries in keys and valueTypes array.
 *
 * Note:
 * Use IPCAPropertyBagFreeStringArray() to free keys buffer.
 * Use IPCAPropertyBagFreeIPCAValueTypeArray() to free valueTypes buffer.
 *
 * @return IPCA_OK if successful.
 */
IPCAStatus IPCA_CALL IPCAPropertyBagGetAllKeyValuePairs(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    char*** keys,
                                    IPCAValueType** valueTypes,
                                    size_t* count);

/**
 * Free memory allocated for IPCAValueType in IPCAPropertyBagGetAllKeyValuePairs().
 *
 * @param[in] valueArray    Array of IPCAValueType values returned in
 *                          IPCAPropertyBagGetAllKeyValuePairs().
 */
void IPCA_CALL IPCAPropertyBagFreeIPCAValueTypeArray(IPCAValueType* valueArray);

/**
 * Set property value.
 *
 * @param[in] propertyBagHandle   Handle returned in IPCAPropertyBagCreate.
 * @param[in] key                 The key of the key-value pair to be stored in the
 *                                propertyBagHandle.
 * @param[in] value               The value of the key-value pair to be stored in propertyBagHandle.
 *
 * @return IPCA_OK if successful.
 */
IPCAStatus IPCA_CALL IPCAPropertyBagSetValueInt(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    int value);

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueDouble(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    double value);

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueBool(
                                    IPCAPropertyBagHandle
                                    propertyBagHandle,
                                    const char* key,
                                    bool value);

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueString(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    const char* value);

IPCAStatus IPCA_CALL IPCAPropertyBagSetValuePropertyBag(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    const IPCAPropertyBagHandle value);

/**
 * Set array value.
 *
 * @param[in] propertyBagHandle   Handle returned in IPCAPropertyBagCreate.
 * @param[in] key                 The key of the key-value pair to be stored in the
 *                                propertyBagHandle.
 * @param[in] valueArray          The array value of the key-value pair to be stored in
 *                                propertyBagHandle.
 * @param[in] valueCount          Size of the valueArray.
 *
 * @return IPCA_OK if successful.
 */
IPCAStatus IPCA_CALL IPCAPropertyBagSetValueIntArray(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    const int* valueArray,
                                    size_t valueCount);

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueDoubleArray(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    const double* valueArray,
                                    size_t valueCount);

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueBoolArray(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    const bool* valueArray,
                                    size_t valueCount);

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueStringArray(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    const char** valueArray,
                                    size_t valueCount);

IPCAStatus IPCA_CALL IPCAPropertyBagSetValuePropertyBagArray(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    const IPCAPropertyBagHandle* valueArray,
                                    size_t valueCount);

/**
 * Get property value.
 *
 * @param[in]  propertyBagHandle   Handle returned in IPCAPropertyBagCreate or IPCAPropertyBagHandle
 *                                 object returned by device.
 * @param[in]  key                 The key of key-value pair of data stored in propertyBagHandle.
 * @param[out] value               The value of key-value pair of data stored in propertyBagHandle.
 *
 * Note:
 *
 * For IPCAPropertyBagGetValueString() caller must use IPCAPropertyBagFreeString() to free the
 * string buffer returned in value argument.
 *
 * For IPCAPropertyBagGetValuePropertyBag(), use IPCAPropertyBagDestroy() to free the
 * IPCAPropertyBagHandle returned in value argument.
 *
 * @return IPCA_OK if successful.
 */
IPCAStatus IPCA_CALL IPCAPropertyBagGetValueInt(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    int* value);

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueDouble(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    double* value);

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueBool(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    bool* value);

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueString(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    char** value);

IPCAStatus IPCA_CALL IPCAPropertyBagGetValuePropertyBag(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    IPCAPropertyBagHandle* value);

/**
 * Get array value.
 *
 * @param[in]  propertyBagHandle   Handle returned in IPCAPropertyBagCreate or IPCAPropertyBagHandle
 *                                 object returned by device.
 * @param[in]  key                 The key of the key-value pair to be stored in the
 *                                 propertyBagHandle.
 * @param[out] valueArray          The array value of the key-value pair to be stored in
 *                                 propertyBagHandle.
 * @param[out] valueCount          Size of the valueArray.
 *
 * Note:
 * Use corresponding IPCAPropertyBagFreeXXXXArray()to free the array of value type returned in value
 * buffer.
 *
 * @return IPCA_OK if successful.
 */
IPCAStatus IPCA_CALL IPCAPropertyBagGetValueIntArray(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    int** value,
                                    size_t* valueCount);

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueDoubleArray(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    double** value,
                                    size_t* valueCount);

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueBoolArray(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    bool** value,
                                    size_t* valueCount);

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueStringArray(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    char*** value,
                                    size_t* valueCount);

IPCAStatus IPCA_CALL IPCAPropertyBagGetValuePropertyBagArray(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    IPCAPropertyBagHandle** value,
                                    size_t* valueCount);

/**
 * Free array types.
 */
void IPCA_CALL IPCAPropertyBagFreeIntArray(int* valueArray);
void IPCA_CALL IPCAPropertyBagFreeDoubleArray(double* valueArray);
void IPCA_CALL IPCAPropertyBagFreeBoolArray(bool* valueArray);
void IPCA_CALL IPCAPropertyBagFreeStringArray(char** valueArray, size_t valueCount);
void IPCA_CALL IPCAPropertyBagFreePropertyBagArray(
                                    IPCAPropertyBagHandle* valueArray,
                                    size_t valueCount);

/**
 * Free stringBuffer allocated in IPCAPropertyBagGetValueString().
 *
 * @param[in] stringBuffer   String buffer returned in IPCAPropertyBagGetValueString().
 */
void IPCA_CALL IPCAPropertyBagFreeString(char* stringBuffer);

/**
 * Security authentication type used during Ownership Transfer communication.
 *
 * Ownership Transfer is currently supported in two different cases:
 * - Between an Onboarding Tool (OBT) and the application. The OBT becomes the owner of the
 *   application.
 * - Between the application and a device, if the device has Multiple Owner Transfer (MOT) enabled.
 *   The application becomes one of the sub-owners of the device.
 *
 */
typedef enum
{
    IPCA_OWNERSHIP_TRANSFER_JUST_WORKS = 0,     // No authentication is required.

    IPCA_OWNERSHIP_TRANSFER_RANDOM_PIN = 1,     // Either:
                                                // - The device displays a random password, and the
                                                //   user inputs the same password in the
                                                //   application, or:
                                                // - The application displays a random password, and
                                                //   the user inputs the same password in an
                                                //   onboarding tool.

    IPCA_OWNERSHIP_TRANSFER_MANUFACTURER_CERTIFICATE = 2,   // Authenticate using a pre-provisioned
                                                            // security certificate.

    IPCA_OWNERSHIP_TRANSFER_PRECONFIGURED_PIN = 0xFF00,     // The device owner configures a
                                                            // password using an onboarding tool.
                                                            // Then, the application obtains the
                                                            // password and passes it to IPCA.
} IPCAOwnershipTransferType;

/**
 * This callback will be called whenever a device requires that the app provide a password, in order
 * to complete the security provisioning of the application. This will be called when a device
 * implements the "Multiple Ownership Transfer" (MOT) method. The app is responsible for eliciting
 * the appropriate password from the user.
 *
 * @param[in] context               Applications's context set in IPCASetPasswordCallbacks().
 * @param[in] deviceInformation     Information about the device or onboarding tool requiring
 *                                  authentication.
 * @param[in] platformInformation   Information about the platform of the device or onboarding tool
 *                                  requiring authentication.
 * @param[in] type                  The type of authentication requiring the password entry. An
 *                                  application might display different messages to the user,
 *                                  depending on the value of authenticationType.
 *                                  The current version of IPCA supports these values for this
 *                                  parameter:
 *                                      - IPCA_OWNERSHIP_TRANSFER_RANDOM_PIN
 *                                      - IPCA_OWNERSHIP_TRANSFER_PRECONFIGURED_PIN
 * @param[out] passwordBuffer       Buffer to fill with a password, as a zero-terminated string. The
 *                                  application can obtain the password:
 *                                      - Directly from the MOT-enabled device, for
 *                                        IPCA_OWNERSHIP_TRANSFER_RANDOM_PIN
 *                                      - From the owner of the MOT-enabled device, for
 *                                        IPCA_OWNERSHIP_TRANSFER_PRECONFIGURED_PIN
 * @param[in] passwordBufferSize    Size of passwordBuffer.
 *
 * @return IPCA_OK if successful. IPCA_FAIL if the app did not provide a password, or if the
 *         passwordBufferSize was too small.
 */
typedef IPCAStatus (IPCA_CALL *IPCAProvidePasswordCallback)(
                                            void* context,
                                            const IPCADeviceInfo* deviceInformation,
                                            const IPCAPlatformInfo* platformInformation,
                                            IPCAOwnershipTransferType type,
                                            char* passwordBuffer,
                                            size_t passwordBufferSize);

/**
 * Callback used to display a password in the application. The user would typically read this
 * password and communicate it to the user of an onboarding tool.
 *
 * @param[in] context               Applications's context set in IPCASetPasswordCallbacks().
 * @param[in] deviceInformation     Information about the device or onboarding tool requiring
 *                                  authentication.
 * @param[in] platformInformation   Information about the platform of the device or onboarding tool
 *                                  requiring authentication.
 * @param[in] type                  The type of authentication that requires displaying the
 *                                  password.
 *                                  An application might display different messages to the user,
 *                                  depending on the value of authenticationType. The current
 *                                  version of IPCA supports the following value for this parameter:
 *                                  - IPCA_OWNERSHIP_TRANSFER_RANDOM_PIN - to authenticate with an
 *                                    onboarding tool.
 * @param[in] password             Password to be displayed, as a zero-terminated string.
 *
 * @return IPCA_OK if successful. IPCA_FAIL if the app or its user decided to cancel the
 *         authentication with the onboarding tool.
 */
typedef IPCAStatus (IPCA_CALL *IPCADisplayPasswordCallback)(
                                            void* context,
                                            const IPCADeviceInfo* deviceInformation,
                                            const IPCAPlatformInfo* platformInformation,
                                            IPCAOwnershipTransferType type,
                                            const char* password);

/**
 * An application calls this method one time to set up its password input and display callbacks.
 * The callbacks get automatically removed by IPCA, when the application calls IPCAClose for the
 * ipcaAppHandle.
 *
 * @param[in] ipcaAppHandle:            Handle returned by IPCAOpen().
 * @param[in] providePasswordCallback:  Callback from IPCA when the application must obtain a
 *                                      password and provide it to IPCA.
 * @param[in] displayPasswordCallback:  Callback from IPCA when the application must display a
 *                                      password to the user.
 * @param[in] context:                  Application's context that is passed back by IPCA as
 *                                      argument in the password callbacks.
 *
 */
IPCAStatus IPCA_CALL IPCASetPasswordCallbacks(
                                IPCAAppHandle ipcaAppHandle,
                                IPCAProvidePasswordCallback providePasswordCallback,
                                IPCADisplayPasswordCallback displayPasswordCallback,
                                void* context);

/**
 * Callback from IPCA when a request initiated by the application using IPCARequestAccess has been
 * completed.
 *
 * @param[in] completionStatus      One of IPCA_SECURITY_UPDATE_REQUEST_FINISHED,
 *                                  IPCA_SECURITY_UPDATE_REQUEST_INCORRECT_PASSWORD,
 *                                  IPCA_SECURITY_UPDATE_REQUEST_NOT_SUPPORTED,
 *                                  IPCA_SECURITY_UPDATE_REQUEST_FAILED.
 * @param[in] context               Applications's context set in IPCARequestAccess().
 */
typedef void (IPCA_CALL *IPCARequestAccessCompletionCallback)(
                                IPCAStatus completionStatus,
                                void* context);

/**
 * Initiate a request to access a device. Called by the application after IPCAGetProperties or
 * IPCASetProperties failed with status IPCA_ACCESS_DENIED. completionCallback gets called after the
 * access request has been processed by IPCA.
 *
 * @param[in] deviceHandle          Device handle returned in IPCAOpenDevice().
 * @param[in] resourcePath          E.g., "/oic/d". If NULL the application requests access to all
 *                                  vertical resources on the device.
 * @param[in] completionCallback    Callback from IPCA when access request has been completed.
 * @param[in] context               Application's context that is passed back as argument in the
 *                                  completion callback.
 * @param[out] handle               Handle that can be used in IPCACloseHandle() to cancel the
 *                                  request.
 *                                  The reference count to the handle is handled by IPCA. Therefore,
 *                                  the caller does not need to close the handle. Null handle
 *                                  pointer is allowed when the caller does not plan to cancel the
 *                                  request.
 * @return
 *   IPCA_OK if the request has been successfully initiated.
 */
IPCAStatus IPCA_CALL IPCARequestAccess(
                                IPCADeviceHandle deviceHandle,
                                const char* resourcePath,
                                IPCARequestAccessCompletionCallback completionCallback,
                                void* context,
                                IPCAHandle* handle);
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // IPCA_H_
