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

#include "pch.h"
#include "Util.h"

using namespace ElevatorClientUWP;
using namespace Platform;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;

String^ Util::ConvertStrtoPlatformStr(PCSTR str)
{
    String^ wstrRet = nullptr;
    PWSTR wstr = nullptr;

    int wstrLength = MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        str,
        -1,
        nullptr,
        0);

    if (wstrLength == 0)
    {
        goto exit;
    }

    // wstrLength includes null char
    wstr = new WCHAR[wstrLength];
    if (wstr == nullptr)
    {
        goto exit;
    }

    int retLen = MultiByteToWideChar(
        CP_UTF8,
        0,
        str,
        -1,
        wstr,
        wstrLength);

    if (retLen != wstrLength)
    {
        goto exit;
    }

    wstrRet = ref new String(wstr);

exit:
    if (wstr != nullptr)
    {
        delete[] wstr;
    }
    return wstrRet;
}

// This function converts a wide char string to a standard char string.
std::string Util::ConvertWStrtoStr(PCWSTR wstr)
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
        goto exit;
    }

    // strLength includes null char
    str = new char[strLength];
    if (str == nullptr)
    {
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
        goto exit;
    }

    strRet = str;

exit:
    if (str != nullptr)
    {
        delete[] str;
    }
    return strRet;
}

void Util::ShowErrorMsg(CoreDispatcher^ dispatcher, String^ msg)
{
    Platform::Agile<MessageDialog^> msgDlg(ref new MessageDialog(msg, "Error"));
    if (dispatcher)
    {
        dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([msgDlg]
        {
            msgDlg->ShowAsync();
        }));
    }
    else
    {
        msgDlg->ShowAsync();
    }
}
