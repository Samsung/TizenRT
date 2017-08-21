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

#include "ipca.h"

// Key of map is resource path. Value is array of resource types.
typedef std::map<std::string, std::vector<std::string>> ResourceList;

/**
 * Each OCFDevice object represents a device discovered by IPCA.
 */
class OCFDevice
{
public:
    typedef std::shared_ptr<OCFDevice> Ptr;
    OCFDevice(IPCAAppHandle appHandle, std::string id);
    ~OCFDevice();
    IPCADeviceInfo* GetDeviceInfo();
    IPCAPlatformInfo* GetPlatformInfo();
    ResourceList GetResourceInfo();

private:
    IPCAStatus OpenDevice();

private:
    IPCAAppHandle m_ipcaAppHandle;
    std::string m_deviceId;
    std::string m_hostAddress;
    IPCADeviceHandle m_deviceHandle;      // from IPCAOpenDevice();
    IPCADeviceInfo* m_deviceInfo;         // valid between IPCAOpenDevice() and IPCACloseDevice().
    IPCAPlatformInfo* m_platformInfo;     // valid between IPCAOpenDevice() and IPCACloseDevice().
    ResourceList m_resourceList;
};
