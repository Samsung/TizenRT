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

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <windows.h>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include "logger.h"
#include "iotivity_debug.h"
#include "oic_platform.h"
#include "oic_string.h"
#include "oic_malloc.h"

#ifdef UWP_APP
#include <cstdint>
#include <Windows.Foundation.h>
#include <windows.storage.h>
#include <wrl.h>

using namespace ABI::Windows::Storage;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
#endif // UWP_APP

#define TAG "OIC_PLATFORM"

#define IOTIVITY_FOLDER_NAME "\\iotivity\\"

using namespace std;

#ifdef UWP_APP
// This function converts a wide char string to a standard char string.
static std::string ConvertWStrtoStr(PCWSTR wstr)
{
    std::string strRet;
    char* str = nullptr;

    int strLength = WideCharToMultiByte(
        CP_UTF8,
        WC_ERR_INVALID_CHARS,
        wstr,
        -1,
        nullptr,
        0,
        nullptr,
        nullptr);

    if (strLength == 0)
    {
        OIC_LOG_V(ERROR, TAG, "Failed WideCharToMultiByte(), GetLastError(): %u", GetLastError());
        goto exit;
    }

    // strLength includes null char
    str = static_cast<char*>(OICCalloc(1, strLength));
    if (str == nullptr)
    {
        OIC_LOG(ERROR, TAG, "Failed to create str buffer");
        goto exit;
    }

    int retLen = WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr,
        -1,
        str,
        strLength,
        nullptr,
        nullptr);

    if (retLen != strLength)
    {
        OIC_LOG_V(ERROR, TAG, "WideCharToMultiByte failed to convert WSTR, GetLastError(): %u",
            GetLastError());
        goto exit;
    }

    strRet = str;

exit:
    if (str != nullptr)
    {
        OICFree(str);
    }
    return strRet;
}
#endif // UWP_APP

static bool VerifyOrCreateDir(LPCSTR dir)
{
    if (!CreateDirectory(dir, nullptr))
    {
        DWORD err = GetLastError();
        if (err != ERROR_ALREADY_EXISTS)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to CreateDirectory, GetLastError(): %u", err);
            return false;
        }
    }

    return true;
}

static OICPlatformResult_t GetSysLocalAppDataPath(std::string &path, size_t &sysPathLen, bool getTempDir)
{
    OICPlatformResult_t ret = OIC_PLATFORM_OK;

#ifdef UWP_APP
    HRESULT hr = S_OK;

    // Since we are running in a UWP app, we don't need to initialize the Windows Runtime
    // with RoInitialize
    ComPtr<IApplicationDataStatics> appDataStatics;
    hr = RoGetActivationFactory(HStringReference(L"Windows.Storage.ApplicationData").Get(),
        __uuidof(appDataStatics), &appDataStatics);
    if (FAILED(hr))
    {
        OIC_LOG_V(ERROR, TAG, "Failed to get the ActivationFactory, hr: %#x", hr);
        return OIC_PLATFORM_ERROR;
    }

    ComPtr<IApplicationData> appData;
    hr = appDataStatics->get_Current(&appData);
    if (FAILED(hr))
    {
        OIC_LOG_V(ERROR, TAG, "Failed to get ApplicationData, hr: %#x", hr);
        return OIC_PLATFORM_ERROR;
    }

    ComPtr<IStorageFolder> folder;
    if (!getTempDir)
    {
        hr = appData->get_LocalFolder(&folder);
        if (FAILED(hr))
        {
            OIC_LOG_V(ERROR, TAG, "Failed to get Local App StorageFolder, hr: %#x", hr);
            return OIC_PLATFORM_ERROR;
        }
    }
    else
    {
        hr = appData->get_TemporaryFolder(&folder);
        if (FAILED(hr))
        {
            OIC_LOG_V(ERROR, TAG, "Failed to get Temp App StorageFolder, hr: %#x", hr);
            return OIC_PLATFORM_ERROR;
        }
    }

    ComPtr<IStorageItem> folderItem;
    hr = folder.As(&folderItem);
    if (FAILED(hr))
    {
        OIC_LOG_V(ERROR, TAG, "Failed to get StorageItem, hr: %#x", hr);
        return OIC_PLATFORM_ERROR;
    }

    HString folderPathHString;
    hr = folderItem->get_Path(folderPathHString.GetAddressOf());
    if (FAILED(hr))
    {
        OIC_LOG_V(ERROR, TAG, "Failed to get FolderPath, hr: %#x", hr);
        return OIC_PLATFORM_ERROR;
    }

    uint32_t wstrPathLength;
    PCWSTR folderPathWStr = folderPathHString.GetRawBuffer(&wstrPathLength);
    path = ConvertWStrtoStr(folderPathWStr);
    if (path.empty())
    {
        OIC_LOG(ERROR, TAG, "Failed to ConvertWStrtoStr");
        return OIC_PLATFORM_ERROR;
    }

    // The length of the string that the system returned. All the folders up to this point
    // should exist.
    sysPathLen = path.length();
#else // UWP_APP
    // Unsupported for win32 apps
    OIC_LOG(INFO, TAG, "GetSysLocalAppDataPath: Unsupported platform.");
    OC_UNUSED(path);
    OC_UNUSED(sysPathLen);
    OC_UNUSED(getTempDir);
    ret = OIC_PLATFORM_NOTIMPL;
#endif // UWP_APP

    return ret;
}

// This function returns the platform and application specific local or temp application data path.
// This path will be used to store the iotivity metadata.For example, the security databases.
// The path will contain the trailing backslash(\) and the null terminating character.
// To get the needed buffer size, call this function with buffer NULL.
static OICPlatformResult_t GetLocalAppDataPath(std::string &path, bool getTempDir, char* buffer, size_t* bufferLen)
{
    if (bufferLen == nullptr)
    {
        OIC_LOG(ERROR, TAG, "bufferLen is NULL");
        return OIC_PLATFORM_INVALID_PARAM;
    }

    if (path.empty())
    {
        // The length of the string that the system returned. All the folders up to this point
        // should exist.
        size_t sysPathLen;
        OICPlatformResult_t ret = GetSysLocalAppDataPath(path, sysPathLen, getTempDir);
        // Set path to the appropriate system local or temp application data path
        if (ret != OIC_PLATFORM_OK)
        {
            if (ret != OIC_PLATFORM_NOTIMPL)
            {
                OIC_LOG_V(ERROR, TAG, "Failed to GetSysLocalAppDataPath, ret: %" PRIuPTR,
                    static_cast<size_t>(ret));
            }
            // On failure, path should be cleared in GetSysLocalAppDataPath
            return ret;
        }

        // Append \iotivity\ for UWP and Win32
        path.append(IOTIVITY_FOLDER_NAME);

        if (path[path.length() - 1] != '\\')
        {
            path.append("\\");
        }

        // At this point, the path should end with backslash (\)
        // Verify/Create all the folders in the path.
        // Start searching from after the path retrieved from the system APIs
        size_t prevSlashIdx = 0;
        size_t slashIdx = path.find("\\", sysPathLen);
        while (slashIdx != string::npos)
        {
            std::string dir = path.substr(0, slashIdx);
            if (!VerifyOrCreateDir(dir.c_str()))
            {
                OIC_LOG_V(ERROR, TAG, "Failed to VerifyOrCreateDir %s", dir.c_str());
                // Revert path back to default state as we cannot create an
                // intermediate folder
                path.clear();
                return OIC_PLATFORM_ERROR;
            }
            prevSlashIdx = slashIdx;
            slashIdx = path.find("\\", slashIdx + 1);
        }

        if (prevSlashIdx != (path.length() - 1))
        {
            // Verify/Create the last folder
            if (!VerifyOrCreateDir(path.c_str()))
            {
                OIC_LOG_V(ERROR, TAG, "Failed to VerifyOrCreateDir %s", path.c_str());
                // Revert path back to default state as we cannot create an
                // intermediate folder
                path.clear();
                return OIC_PLATFORM_ERROR;
            }
        }
    }

    if (path.empty())
    {
        // Path shouldn't be empty at this point
        OIC_LOG(ERROR, TAG, "Path is empty");
        return OIC_PLATFORM_ERROR;
    }

    if (buffer != nullptr)
    {
        if (*bufferLen < (path.length() + 1))
        {
            // Insufficient buffer size for path string
            OIC_LOG(ERROR, TAG, "Insufficient buffer size for path string");
            *bufferLen = path.length() + 1; // + 1 for null-character
            return OIC_PLATFORM_INVALID_PARAM;
        }
        OICStrcpy(buffer, *bufferLen, path.c_str());
        *bufferLen = path.length() + 1; // + 1 for null-character
    }
    else
    {
        *bufferLen = path.length() + 1; // + 1 for null-character
    }

    return OIC_PLATFORM_OK;
}

// This function returns the platform and application specific local application data path.
// This path will be used to store the iotivity metadata.For example, the security databases.
// The path will contain the trailing backslash(\) and the null terminating character.
// To get the needed buffer size, call this function with buffer NULL.
OICPlatformResult_t OICGetLocalAppDataPath(char* buffer, size_t* bufferLen)
{
    static std::string localAppDataPath;
    return GetLocalAppDataPath(localAppDataPath, false, buffer, bufferLen);
}

// This function returns the platform and application specific temp application data path.
// This path will be used to store the iotivity metadata.For example, the security databases.
// The path will contain the trailing backslash(\) and the null terminating character.
// To get the needed buffer size, call this function with buffer NULL.
OICPlatformResult_t OICGetTempAppDataPath(char* buffer, size_t* bufferLen)
{
    static std::string tempAppDataPath;
    return GetLocalAppDataPath(tempAppDataPath, true, buffer, bufferLen);
}
