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

#ifndef COMMON_H_
#define COMMON_H_

#if defined(_MSC_VER)
#define ARRAY_SIZE(a)               _countof(a)
#else
#define ARRAY_SIZE(a)               (sizeof(a) / sizeof(a[0]))
#endif

// Copy the source std string to dest.
bool CopyStringToBufferAllowTruncate(const std::string& source, char* dest, size_t destSize);
bool CopyStringToFlatBuffer(const std::string& source, char* dest, size_t* destBufferSize);

// Allocate the dest buffer and copy the source std string to dest.
IPCAStatus AllocateAndCopyStringToFlatBuffer(const std::string& source, char** dest);

// Allocate the dest buffer of string and copy from source std string to dest.
IPCAStatus AllocateAndCopyStringVectorToArrayOfCharPointers(
                    const std::vector<std::string>& source,
                    char*** dest,
                    size_t* count);

// Free array of string.
void FreeArrayOfCharPointers(char** array, size_t count);

// Return true if string is in list.
bool IsStringInList(const std::string& string, const std::vector<std::string>& list);

// Add strings in newList that is not in targetList.  Return true if there's new entry added.
bool AddNewStringsToTargetList(const std::vector<std::string>& newList,
                               std::vector<std::string>& targetList);

// Same information as IPCADeviceInfo in ipca.h but with std::string.
typedef struct
{
    std::string deviceId;
    std::string platformIndependentId;
    std::vector<std::string> deviceUris;
    std::string deviceName;
    std::string deviceSoftwareVersion;
    std::vector<std::string> dataModelVersions;
} InternalDeviceInfo;

// Same information as IPCAPlatformInfo in ipca.h, but with std::string.
typedef struct
{
    std::string platformId;
    std::string manufacturerName;
    std::string manufacturerURL;
    std::string modelNumber;
    std::string manufacturingDate;
    std::string platformVersion;
    std::string osVersion;
    std::string hardwareVersion;
    std::string firmwareVersion;
    std::string manufacturerSupportURL;
    std::string referenceTime;
} InternalPlatformInfo;

// Security information
typedef struct
{
    bool subowner;
    bool isStarted;
    std::shared_ptr<OC::OCSecureResource> device;
    std::thread requestAccessThread;
    std::mutex requestAccessThreadMutex;
    std::condition_variable requestAccessThreadCV;
} InternalSecurityInfo;

// Debug helpers
void PrintOCRep(const OC::OCRepresentation& rep, size_t marginDepth = 0);
#endif // COMMON_H_
