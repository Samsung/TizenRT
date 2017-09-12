//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

///
///This sample demonstrates platform and device discovery feature
///The server sets the platform and device related info. which can
///be later retrieved by a client.
///

#include <mutex>
#include <condition_variable>

#include "OCPlatform.h"
#include "OCApi.h"
#include "ocpayload.h"

using namespace OC;

// Set of strings for each of platform Info fields
std::string gPlatformID = "0A3E0D6F-DBF5-404E-8719-D6880042463A";
std::string gManufacturerName = "myName";
std::string gManufacturerLink = "https://www.example.com";
std::string gModelNumber = "myModelNumber";
std::string gDateOfManufacture = "2016-01-15";
std::string gPlatformVersion = "platformVersion";
std::string gOperatingSystemVersion = "myOS";
std::string gHardwareVersion = "myHardwareVersion";
std::string gFirmwareVersion = "1.0";
std::string gSupportLink = "https://www.examplesupport.com";
std::string gSystemTime = "2016-01-15T11.01";

// Set of strings for each of device info fields
std::string  deviceName = "Bill's Battlestar";
std::string  specVersion = "ocf.1.1.0";
std::vector<std::string> dataModelVersions = {"ocf.res.1.1.0", "ocf.sh.1.1.0"};
std::string  protocolIndependentID = "4cae60c1-48cb-47dc-882e-dedec114f45c";

// Device type
std::string  deviceType = "oic.d.tv";

// OCPlatformInfo Contains all the platform info to be stored
OCPlatformInfo platformInfo;

void DeletePlatformInfo()
{
    delete[] platformInfo.platformID;
    delete[] platformInfo.manufacturerName;
    delete[] platformInfo.manufacturerUrl;
    delete[] platformInfo.modelNumber;
    delete[] platformInfo.dateOfManufacture;
    delete[] platformInfo.platformVersion;
    delete[] platformInfo.operatingSystemVersion;
    delete[] platformInfo.hardwareVersion;
    delete[] platformInfo.firmwareVersion;
    delete[] platformInfo.supportUrl;
    delete[] platformInfo.systemTime;
}

void DuplicateString(char ** targetString, std::string sourceString)
{
    *targetString = new char[sourceString.length() + 1];
    strncpy(*targetString, sourceString.c_str(), (sourceString.length() + 1));
}

OCStackResult SetPlatformInfo(std::string platformID, std::string manufacturerName,
    std::string manufacturerUrl, std::string modelNumber, std::string dateOfManufacture,
    std::string platformVersion, std::string operatingSystemVersion, std::string hardwareVersion,
    std::string firmwareVersion, std::string supportUrl, std::string systemTime)
{
    DuplicateString(&platformInfo.platformID, platformID);
    DuplicateString(&platformInfo.manufacturerName, manufacturerName);
    DuplicateString(&platformInfo.manufacturerUrl, manufacturerUrl);
    DuplicateString(&platformInfo.modelNumber, modelNumber);
    DuplicateString(&platformInfo.dateOfManufacture, dateOfManufacture);
    DuplicateString(&platformInfo.platformVersion, platformVersion);
    DuplicateString(&platformInfo.operatingSystemVersion, operatingSystemVersion);
    DuplicateString(&platformInfo.hardwareVersion, hardwareVersion);
    DuplicateString(&platformInfo.firmwareVersion, firmwareVersion);
    DuplicateString(&platformInfo.supportUrl, supportUrl);
    DuplicateString(&platformInfo.systemTime, systemTime);

    return OC_STACK_OK;
}

OCStackResult SetDeviceInfo()
{
    OCStackResult result = OC_STACK_ERROR;

    OCResourceHandle handle = OCGetResourceHandleAtUri(OC_RSRVD_DEVICE_URI);
    if (handle == NULL)
    {
        std::cout << "Failed to find resource " << OC_RSRVD_DEVICE_URI << std::endl;
        return result;
    }

    result = OCBindResourceTypeToResource(handle, deviceType.c_str());
    if (result != OC_STACK_OK)
    {
        std::cout << "Failed to add device type" << std::endl;
        return result;
    }

    result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME, deviceName);
    if (result != OC_STACK_OK)
    {
        std::cout << "Failed to set device name" << std::endl;
        return result;
    }

    result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DATA_MODEL_VERSION,
                                          dataModelVersions);
    if (result != OC_STACK_OK)
    {
        std::cout << "Failed to set data model versions" << std::endl;
        return result;
    }

    result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION, specVersion);
    if (result != OC_STACK_OK)
    {
        std::cout << "Failed to set spec version" << std::endl;
        return result;
    }

    result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_PROTOCOL_INDEPENDENT_ID,
                                          protocolIndependentID);
    if (result != OC_STACK_OK)
    {
        std::cout << "Failed to set piid" << std::endl;
        return result;
    }

    return OC_STACK_OK;
}

int main()
{
    // Create PlatformConfig object
    PlatformConfig cfg {
        OC::ServiceType::InProc,
        OC::ModeType::Server,
        nullptr
    };

    OCPlatform::Configure(cfg);
    OC_VERIFY(OCPlatform::start() == OC_STACK_OK);

    std::cout<<"Starting server & setting platform info\n";

    OCStackResult result = SetPlatformInfo(gPlatformID, gManufacturerName, gManufacturerLink,
            gModelNumber, gDateOfManufacture, gPlatformVersion,  gOperatingSystemVersion,
            gHardwareVersion, gFirmwareVersion,  gSupportLink, gSystemTime);

    result = OCPlatform::registerPlatformInfo(platformInfo);

    if (result != OC_STACK_OK)
    {
        std::cout << "Platform Registration failed\n";
        return -1;
    }

    result = SetDeviceInfo();

    if (result != OC_STACK_OK)
    {
        std::cout << "Device Registration failed\n";
        return -1;
    }

    DeletePlatformInfo();

    std::cout << "Waiting. Press \"Enter\" to quit." << std::endl;
    // Ignoring all input except for EOL.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Perform platform clean up.
    OC_VERIFY(OCPlatform::stop() == OC_STACK_OK);
    return 0;

}
