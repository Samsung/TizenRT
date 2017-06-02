/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
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

#include "simulator_manager.h"
#include "simulator_resource_factory.h"
#include "simulator_remote_resource_impl.h"
#include "simulator_utils.h"

SimulatorManager *SimulatorManager::getInstance()
{
    static SimulatorManager s_instance;
    return &s_instance;
}

SimulatorManager::SimulatorManager()
{
    OC::PlatformConfig conf
    {
        OC::ServiceType::InProc,
        OC::ModeType::Both,
        "0.0.0.0", // By setting to "0.0.0.0", it binds to all available interfaces
        0,         // Uses randomly available port
        OC::QualityOfService::LowQos
    };

    OC::OCPlatform::Configure(conf);
}

std::shared_ptr<SimulatorResource> SimulatorManager::createResource(
    const std::string &configPath)
{
    VALIDATE_INPUT(configPath.empty(), "Empty path!")

    std::shared_ptr<SimulatorResource> resource;
    try
    {
        resource = SimulatorResourceFactory::getInstance()->createResource(configPath);
        if (!resource)
            throw SimulatorException(SIMULATOR_ERROR, "Failed to create resource!");
    }
    catch (RAML::RamlException &e)
    {
        throw SimulatorException(SIMULATOR_ERROR, "Failed to create resource!");
    }

    return resource;
}

std::vector<std::shared_ptr<SimulatorResource>> SimulatorManager::createResource(
            const std::string &configPath, unsigned int count)
{
    VALIDATE_INPUT(configPath.empty(), "Empty path!")
    VALIDATE_INPUT(!count, "Count is zero!")

    std::vector<std::shared_ptr<SimulatorResource>> resources;
    try
    {
        resources = SimulatorResourceFactory::getInstance()->createResource(configPath, count);
        if (!resources.size())
            throw SimulatorException(SIMULATOR_ERROR, "Failed to create resource!");
    }
    catch (RAML::RamlException &e)
    {
        throw SimulatorException(SIMULATOR_ERROR, "Failed to create resource!");
    }

    return resources;
}

std::shared_ptr<SimulatorSingleResource> SimulatorManager::createSingleResource(
    const std::string &name, const std::string &uri, const std::string &resourceType)
{
    VALIDATE_INPUT(name.empty(), "Empty resource name!")
    VALIDATE_INPUT(uri.empty(), "Empty URI!")
    VALIDATE_INPUT(resourceType.empty(), "Empty resource type!")

    return SimulatorResourceFactory::getInstance()->createSingleResource(name, uri, resourceType);
}

std::shared_ptr<SimulatorCollectionResource> SimulatorManager::createCollectionResource(
    const std::string &name, const std::string &uri, const std::string &resourceType)
{
    VALIDATE_INPUT(name.empty(), "Empty resource name!")
    VALIDATE_INPUT(uri.empty(), "Empty URI!")
    VALIDATE_INPUT(resourceType.empty(), "Empty resource type!")

    return SimulatorResourceFactory::getInstance()->createCollectionResource(name, uri, resourceType);
}

void SimulatorManager::findResource(ResourceFindCallback callback)
{
    VALIDATE_CALLBACK(callback)

    OC::FindCallback findCallback = std::bind(
                                        [](std::shared_ptr<OC::OCResource> ocResource, ResourceFindCallback callback)
    {
        if (!ocResource)
            return;

        callback(std::make_shared<SimulatorRemoteResourceImpl>(ocResource));
    }, std::placeholders::_1, callback);

    typedef OCStackResult (*FindResource)(const std::string &, const std::string &,
                                          OCConnectivityType, OC::FindCallback);

    invokeocplatform(static_cast<FindResource>(OC::OCPlatform::findResource), "",
                     OC_MULTICAST_DISCOVERY_URI, CT_DEFAULT, findCallback);
}

void SimulatorManager::findResource(const std::string &resourceType,
                                    ResourceFindCallback callback)
{
    VALIDATE_INPUT(resourceType.empty(), "Empty resource type!")
    VALIDATE_CALLBACK(callback)

    OC::FindCallback findCallback = std::bind(
                                        [](std::shared_ptr<OC::OCResource> ocResource, ResourceFindCallback callback)
    {
        if (!ocResource)
            return;

        callback(std::make_shared<SimulatorRemoteResourceImpl>(ocResource));
    }, std::placeholders::_1, callback);

    std::ostringstream query;
    query << OC_MULTICAST_DISCOVERY_URI << "?rt=" << resourceType;

    typedef OCStackResult (*FindResource)(const std::string &, const std::string &,
                                          OCConnectivityType, OC::FindCallback);

    invokeocplatform(static_cast<FindResource>(OC::OCPlatform::findResource), "", query.str(),
                     CT_DEFAULT, findCallback);
}

void SimulatorManager::getDeviceInfo(const std::string &host, DeviceInfoCallback callback)
{
    VALIDATE_CALLBACK(callback)

    OC::FindDeviceCallback deviceCallback = std::bind(
            [](const OC::OCRepresentation & rep, const std::string & hostUri, DeviceInfoCallback callback)
    {
        std::string deviceName = rep.getValue<std::string>("n");
        std::string deviceID = rep.getValue<std::string>("di");
        std::string deviceSpecVersion = rep.getValue<std::string>("lcv");
        std::string deviceDMV = rep.getValue<std::string>("dmv");

        DeviceInfo deviceInfo(deviceName, deviceID, deviceSpecVersion, deviceDMV);
        callback(hostUri, deviceInfo);
    }, std::placeholders::_1, host, callback);

    typedef OCStackResult (*GetDeviceInfo)(const std::string &, const std::string &,
                                           OCConnectivityType, OC::FindDeviceCallback);

    invokeocplatform(static_cast<GetDeviceInfo>(OC::OCPlatform::getDeviceInfo), host.c_str(),
                     "/oic/d", CT_DEFAULT, deviceCallback);
}

void SimulatorManager::setDeviceInfo(const std::string &deviceName)
{
    VALIDATE_INPUT(deviceName.empty(), "Empty resource type!")

    typedef OCStackResult (*RegisterDeviceInfo)(const OCDeviceInfo);

    OCDeviceInfo ocDeviceInfo {nullptr, nullptr};
    ocDeviceInfo.deviceName = const_cast<char *>(deviceName.c_str());
    invokeocplatform(static_cast<RegisterDeviceInfo>(OC::OCPlatform::registerDeviceInfo),
                     ocDeviceInfo);
}

void SimulatorManager::getPlatformInfo(const std::string &host, PlatformInfoCallback callback)
{
    VALIDATE_CALLBACK(callback)

    OC::FindPlatformCallback platformCallback = std::bind(
                [](const OC::OCRepresentation & rep, const std::string & hostUri, PlatformInfoCallback callback)
    {
        PlatformInfo platformInfo;
        platformInfo.setPlatformID(rep.getValue<std::string>("pi"));
        platformInfo.setPlatformVersion(rep.getValue<std::string>("mnpv"));
        platformInfo.setManufacturerName(rep.getValue<std::string>("mnmn"));
        platformInfo.setManufacturerUrl(rep.getValue<std::string>("mnml"));
        platformInfo.setModelNumber(rep.getValue<std::string>("mnmo"));
        platformInfo.setDateOfManfacture(rep.getValue<std::string>("mndt"));
        platformInfo.setOSVersion(rep.getValue<std::string>("mnos"));
        platformInfo.setHardwareVersion(rep.getValue<std::string>("mnhw"));
        platformInfo.setFirmwareVersion(rep.getValue<std::string>("mnfv"));
        platformInfo.setSupportUrl(rep.getValue<std::string>("mnsl"));
        platformInfo.setSystemTime(rep.getValue<std::string>("st"));

        callback(hostUri, platformInfo);
    }, std::placeholders::_1, host, callback);

    typedef OCStackResult (*GetPlatformInfo)(const std::string &, const std::string &,
            OCConnectivityType, OC::FindPlatformCallback);

    invokeocplatform(static_cast<GetPlatformInfo>(OC::OCPlatform::getPlatformInfo), host.c_str(),
                     "/oic/p", CT_DEFAULT, platformCallback);
}

void SimulatorManager::setPlatformInfo(PlatformInfo &platformInfo)
{
    OCPlatformInfo ocPlatformInfo;
    ocPlatformInfo.platformID = const_cast<char *>(platformInfo.getPlatformID().c_str());
    ocPlatformInfo.manufacturerName = const_cast<char *>(platformInfo.getManufacturerName().c_str());
    ocPlatformInfo.manufacturerUrl = const_cast<char *>(platformInfo.getManufacturerUrl().c_str());
    ocPlatformInfo.modelNumber = const_cast<char *>(platformInfo.getModelNumber().c_str());
    ocPlatformInfo.dateOfManufacture = const_cast<char *>(platformInfo.getDateOfManfacture().c_str());
    ocPlatformInfo.platformVersion = const_cast<char *>(platformInfo.getPlatformVersion().c_str());
    ocPlatformInfo.operatingSystemVersion = const_cast<char *>(platformInfo.getOSVersion().c_str());
    ocPlatformInfo.hardwareVersion = const_cast<char *>(platformInfo.getHardwareVersion().c_str());
    ocPlatformInfo.firmwareVersion = const_cast<char *>(platformInfo.getFirmwareVersion().c_str());
    ocPlatformInfo.supportUrl = const_cast<char *>(platformInfo.getSupportUrl().c_str());
    ocPlatformInfo.systemTime = const_cast<char *>(platformInfo.getSystemTime().c_str());

    typedef OCStackResult (*RegisterPlatformInfo)(const OCPlatformInfo);
    invokeocplatform(static_cast<RegisterPlatformInfo>(OC::OCPlatform::registerPlatformInfo),
                     ocPlatformInfo);
}

void SimulatorManager::setLogger(const std::shared_ptr<ILogger> &logger)
{
    simLogger().setCustomTarget(logger);
}

bool SimulatorManager::setConsoleLogger()
{
    return simLogger().setDefaultConsoleTarget();
}

bool SimulatorManager::setFileLogger(const std::string &path)
{
    return simLogger().setDefaultFileTarget(path);
}