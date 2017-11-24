/* *****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/

#include <stdio.h>
#include <string.h>

#include "utlist.h"
#include "oic_malloc.h"
#include "oic_string.h"

#include "srmresourcestrings.h"
#include "experimental/securevirtualresourcetypes.h"
#include "srmutility.h"

#include "svrdbeditorcommon.h"

#define STR_UUID_LENGTH (UUID_LENGTH * 2 + 4 + 1) // length + dash length  + '\0'
#define STR_UUID_ZERO "0"

void PrintUuid(const OicUuid_t *uuid)
{
    char *strUuid = NULL;
    if (OC_STACK_OK == ConvertUuidToStr(uuid, &strUuid))
    {
        PRINT_DATA("%s\n", strUuid);
        OICFree(strUuid);
    }
    else
    {
        PRINT_ERR("Can not convert UUID to String");
    }
}

void PrintIntArray(const int *array, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        PRINT_DATA("%d ", array[i]);
    }
    PRINT_NL();
}

void PrintStringArray(const char **array, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        PRINT_DATA("%s ", array[i]);
    }
    PRINT_NL();
}

void PrintInt(int value)
{
    PRINT_DATA("%d\n", value);
}

void PrintString(const char *text)
{
    PRINT_DATA("%s\n", text);
}

void PrintBuffer(const uint8_t *buf, size_t bufLen)
{
    size_t i = 0;

    for (i = 0; i < bufLen; i++)
    {
        if (0 == (i + 1) % 20 || bufLen - 1 == i)
        {
            PRINT_DATA("%02X \n", buf[i]);
        }
        else
        {
            PRINT_DATA("%02X ", buf[i]);
        }
    }
}

int InputNumber(const char *infoText)
{
    int inputValue = 0;

    PRINT_PROG("%s", infoText);
    for (int ret = 0; 1 != ret; )
    {
        ret = scanf("%d", &inputValue);
        while ('\n' != getchar());
    }

    return inputValue;
}

char *InputString(const char *infoText)
{
    char tmpStr[SVR_DB_PATH_LENGTH] = {0};

    PRINT_PROG("%s", infoText);
    for (int ret = 0; 1 != ret; )
    {
        ret = scanf("%1024s", tmpStr);
        while ('\n' != getchar());
    }

    return OICStrdup(tmpStr);
}

int InputUuid(OicUuid_t *uuid)
{
    OCStackResult ocResult = OC_STACK_ERROR;
    char strUuid[STR_UUID_LENGTH] = {0};
    size_t strLen = 0;

    if (NULL == uuid)
    {
        PRINT_ERR("Invalid parameter");
        return -1;
    }

    if (NULL == fgets(strUuid, STR_UUID_LENGTH, stdin))
    {
        PRINT_ERR("Failed fgets");
        return -1;
    }
    strLen = strlen(strUuid);
    if ('\n' == strUuid[strLen - 1])
    {
        strUuid[strLen - 1] = '\0';
    }
    else
    {
        while ('\n' != getchar());
    }

    if (0 == strncmp(strUuid, STR_UUID_ZERO, sizeof(STR_UUID_ZERO)))
    {
        memset(uuid->id, 0x00, sizeof(uuid->id));
    }
    else if (0 == strncmp(strUuid, (char *)WILDCARD_SUBJECT_ID.id, sizeof(WILDCARD_SUBJECT_ID.id)))
    {
        memset(uuid->id, 0x00, sizeof(uuid->id));
        memcpy(uuid->id, WILDCARD_SUBJECT_ID.id, WILDCARD_SUBJECT_ID_LEN);
    }
    else
    {
        ocResult = ConvertStrToUuid(strUuid, uuid);
        if (OC_STACK_OK != ocResult)
        {
            PRINT_ERR("Failed ConvertStrToUuid");
            return -1;
        }
    }

    return 0;
}

