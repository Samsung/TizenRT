//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
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

#include <gtest/gtest.h>
#include "oic_malloc.h"
#include "ocstack.h"
#include <stdlib.h>
#include "cbor.h"
#include "platform_features.h"

#define STRINGIZE2(x) #x
#define STRINGIZE(x) STRINGIZE2(x)

// TODO: Remove this, once all cbor related are completed.
char* ReadFileToBuffer(const char* filename)
{

    FILE *fp = NULL;
    char *data = NULL;
    struct stat st;
    // TODO: Find the location of the executable and concatenate the SVR file name
    // before opening it.
    fp = fopen(filename, "r");
    if (fp)
    {
        if (stat(filename, &st) == 0)
        {
            data = (char*)OICMalloc(st.st_size);
            if (data)
            {
                if (fread(data, 1, st.st_size, fp) != (size_t)st.st_size)
                {
                    printf("Error in reading file %s", filename);
                }
            }
        }
        fclose(fp);
    }
    else
    {
        printf("Unable to open %s file", filename);
    }

    return data;
}

bool ReadCBORFile(const char* filename, const char* rsrcname, uint8_t **payload, size_t *pSize)
{
    bool status = false;
    if (!payload || !pSize)
    {
        printf("Passed parameter are INVALID \n");
        return status;
    }
    uint8_t *data = NULL;
    size_t size = 0;

#ifdef _MSC_VER
// The path SECURITY_BUILD_UNITTEST_DIR can contain '\u' which VS misinterprets as a universal 
// character name in the STRINGIZE macro and outputs warning C4429 'possible incomplete or 
// improperly formed universal-character-name' https://msdn.microsoft.com/en-us/library/z78hwa6k.aspx
#pragma warning(push)
#pragma warning(disable:4429)
#endif
    size_t len = strlen(STRINGIZE(SECURITY_BUILD_UNITTEST_DIR)) + strlen(filename) + 1;
    char *filepath = (char *)OICCalloc(1, len);
    if (!filepath)
    {
        printf("filepath memory allocation failed. \n");
        return false;
    }
    int ret = snprintf(filepath, len, "%s%s", STRINGIZE(SECURITY_BUILD_UNITTEST_DIR), filename);
    printf("Root build path: %s \n", filepath);

#ifdef _MSC_VER
#pragma warning(pop)
#endif

    if (ret == (int)(len - 1))
    {
        FILE *fp = fopen(filepath, "rb");
        if (fp)
        {
            struct stat st;
            if (stat(filepath, &st) == 0)
            {
                data = (uint8_t *)OICMalloc(st.st_size);
                if (data)
                {
                    if (fread(data, 1, st.st_size, fp) != (size_t)st.st_size)
                    {
                        printf("Error in reading file %s\n", filename);
                    }
                    else
                    {
                        size = (size_t)st.st_size;

                        CborValue cbor = {NULL, NULL, 0, 0, 0, 0};
                        CborParser parser = {NULL, 0};
                        cbor_parser_init(data, size, 0, &parser, &cbor);
                        CborError cborFindResult = CborNoError;

                        CborValue curVal = {NULL, NULL, 0, 0, 0, 0};
                        cborFindResult = cbor_value_map_find_value(&cbor, rsrcname, &curVal);
                        if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
                        {
                            cborFindResult = cbor_value_dup_byte_string(&curVal, payload, pSize, NULL);
                            if(CborNoError != cborFindResult)
                            {
                                printf("Failed to getting %s data\n", rsrcname);
                            }
                            else
                            {
                                status = true;
                            }
                        }
                        else
                        {
                            printf("Failed to finding %s data\n", rsrcname);
                        }

                    }
                    OICFree(data);
                }
            }
            fclose(fp);
        }
        else
        {
            printf("Unable to open %s file\n", filepath);
        }
    }
    else
    {
        printf("Filepath copy failed.\n");
    }
    OICFree(filepath);
    return status;
}

void SetPersistentHandler(OCPersistentStorage *ps, bool set)
{
    if (set)
    {
        ps->open = fopen;
        ps->read = fread;
        ps->write = fwrite;
        ps->close = fclose;
        ps->unlink = remove;
    }
    else
    {
        memset(ps, 0, sizeof(OCPersistentStorage));
    }
    EXPECT_EQ(OC_STACK_OK,
            OCRegisterPersistentStorageHandler(ps));
}
