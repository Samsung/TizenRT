//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License a
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

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#ifdef RD_SERVER
#include "sqlite3.h"
#endif

#include "octypes.h"
#include "ocstack.h"
#include "ocrandom.h"
#include "logger.h"
#include "ocpayload.h"
#include "ocendpoint.h"
#include "oic_malloc.h"
#include "oic_string.h"

#define TAG "OIC_RI_RESOURCEDIRECTORY"

#define VERIFY_NON_NULL(arg) \
if (!(arg)) \
{ \
    OIC_LOG(ERROR, TAG, #arg " is NULL"); \
    result = OC_STACK_NO_MEMORY; \
    goto exit; \
}

#ifdef RD_SERVER

static const char *gRDPath = "RD.db";

static sqlite3 *gRDDB = NULL;

/* Column indices of RD_DEVICE_LINK_LIST table */
static const uint8_t ins_index = 0;
static const uint8_t href_index = 1;
static const uint8_t rel_index = 2;
static const uint8_t anchor_index = 3;
static const uint8_t bm_index = 4;
static const uint8_t d_index = 5;

/* Column indices of RD_LINK_RT table */
static const uint8_t rt_value_index = 0;

/* Column indices of RD_LINK_IF table */
static const uint8_t if_value_index = 0;

/* Column indices of RD_LINK_EP table */
static const uint8_t ep_value_index = 0;
static const uint8_t pri_value_index = 1;

#define VERIFY_SQLITE(arg) \
if (SQLITE_OK != (arg)) \
{ \
    OIC_LOG_V(ERROR, TAG, "Error in " #arg ", Error Message: %s",  sqlite3_errmsg(gRDDB)); \
    result = OC_STACK_ERROR; \
    goto exit; \
}

OCStackResult OCRDDatabaseSetStorageFilename(const char *filename)
{
    if (!filename)
    {
        OIC_LOG(ERROR, TAG, "The persistent storage filename is invalid");
        return OC_STACK_INVALID_PARAM;
    }
    gRDPath = filename;
    return OC_STACK_OK;
}

const char *OCRDDatabaseGetStorageFilename()
{
    return gRDPath;
}

static void errorCallback(void *arg, int errCode, const char *errMsg)
{
    OC_UNUSED(arg);
    OC_UNUSED(errCode);
    OC_UNUSED(errMsg);
    OIC_LOG_V(ERROR, TAG, "SQLLite Error: %s : %d", errMsg, errCode);
}

static OCStackResult appendStringLL(OCStringLL **type, const unsigned char *value)
{
    OCStackResult result;
    OCStringLL *temp= (OCStringLL*)OICCalloc(1, sizeof(OCStringLL));
    VERIFY_NON_NULL(temp);
    temp->value = OICStrdup((char *)value);
    VERIFY_NON_NULL(temp->value);
    temp->next = NULL;

    if (!*type)
    {
        *type = temp;
    }
    else
    {
        OCStringLL *tmp = *type;
        for (; tmp->next; tmp = tmp->next);
        tmp->next = temp;
    }
    temp = NULL;
    result = OC_STACK_OK;

exit:
    if (temp)
    {
        OICFree(temp->value);
        OICFree(temp);
    }
    return result;
}

/* stmt is of form "SELECT * FROM RD_DEVICE_LINK_LIST ..." */
static OCStackResult ResourcePayloadCreate(sqlite3_stmt *stmt, OCDiscoveryPayload *discPayload)
{
    int res = sqlite3_step(stmt);
    if (SQLITE_ROW != res)
    {
        return OC_STACK_NO_RESOURCE;
    }

    OCStackResult result;
    OCResourcePayload *resourcePayload = NULL;
    OCEndpointPayload *epPayload = NULL;
    sqlite3_stmt *stmtRT = NULL;
    sqlite3_stmt *stmtIF = NULL;
    sqlite3_stmt *stmtEP = NULL;
    sqlite3_stmt *stmtDI = NULL;
    while (SQLITE_ROW == res)
    {
        resourcePayload = (OCResourcePayload *)OICCalloc(1, sizeof(OCResourcePayload));
        VERIFY_NON_NULL(resourcePayload);

        sqlite3_int64 id = sqlite3_column_int64(stmt, ins_index);
        const unsigned char *uri = sqlite3_column_text(stmt, href_index);
        const unsigned char *rel = sqlite3_column_text(stmt, rel_index);
        const unsigned char *anchor = sqlite3_column_text(stmt, anchor_index);
        sqlite3_int64 bitmap = sqlite3_column_int64(stmt, bm_index);
        sqlite3_int64 deviceId = sqlite3_column_int64(stmt, d_index);
        OIC_LOG_V(DEBUG, TAG, " %s %" PRId64, uri, (int64_t) deviceId);

        resourcePayload->uri = OICStrdup((char *)uri);
        VERIFY_NON_NULL(resourcePayload->uri)
        if (rel)
        {
            resourcePayload->rel = OICStrdup((char *)rel);
            VERIFY_NON_NULL(resourcePayload->rel);
        }
        if (anchor)
        {
            resourcePayload->anchor = OICStrdup((char *)anchor);
            VERIFY_NON_NULL(resourcePayload->anchor);
        }

        const char rt[] = "SELECT rt FROM RD_LINK_RT WHERE LINK_ID=@id";
        int rtSize = (int)sizeof(rt);
        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, rt, rtSize, &stmtRT, NULL));
        VERIFY_SQLITE(sqlite3_bind_int64(stmtRT, sqlite3_bind_parameter_index(stmtRT, "@id"), id));
        while (SQLITE_ROW == sqlite3_step(stmtRT))
        {
            const unsigned char *tempRt = sqlite3_column_text(stmtRT, rt_value_index);
            result = appendStringLL(&resourcePayload->types, tempRt);
            if (OC_STACK_OK != result)
            {
                goto exit;
            }
        }
        VERIFY_SQLITE(sqlite3_finalize(stmtRT));
        stmtRT = NULL;

        const char itf[] = "SELECT if FROM RD_LINK_IF WHERE LINK_ID=@id";
        int itfSize = (int)sizeof(itf);
        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, itf, itfSize, &stmtIF, NULL));
        VERIFY_SQLITE(sqlite3_bind_int64(stmtIF, sqlite3_bind_parameter_index(stmtIF, "@id"), id));
        while (SQLITE_ROW == sqlite3_step(stmtIF))
        {
            const unsigned char *tempItf = sqlite3_column_text(stmtIF, if_value_index);
            result = appendStringLL(&resourcePayload->interfaces, tempItf);
            if (OC_STACK_OK != result)
            {
                goto exit;
            }
        }
        VERIFY_SQLITE(sqlite3_finalize(stmtIF));
        stmtIF = NULL;

        resourcePayload->bitmap = (uint8_t)(bitmap & (OC_OBSERVABLE | OC_DISCOVERABLE));

        const char ep[] = "SELECT ep,pri FROM RD_LINK_EP WHERE LINK_ID=@id";
        int epSize = (int)sizeof(ep);
        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, ep, epSize, &stmtEP, NULL));
        VERIFY_SQLITE(sqlite3_bind_int64(stmtEP, sqlite3_bind_parameter_index(stmtEP, "@id"), id));
        while (SQLITE_ROW == sqlite3_step(stmtEP))
        {
            epPayload = (OCEndpointPayload *)OICCalloc(1, sizeof(OCEndpointPayload));
            VERIFY_NON_NULL(epPayload);
            const unsigned char *tempEp = sqlite3_column_text(stmtEP, ep_value_index);
            result = OCParseEndpointString((const char *)tempEp, epPayload);
            if (OC_STACK_OK != result)
            {
                goto exit;
            }
            sqlite3_int64 pri = sqlite3_column_int64(stmtEP, pri_value_index);
            epPayload->pri = (uint16_t)pri;
            OCEndpointPayload **tmp = &resourcePayload->eps;
            while (*tmp)
            {
                tmp = &(*tmp)->next;
            }
            *tmp = epPayload;
            epPayload = NULL;
        }
        VERIFY_SQLITE(sqlite3_finalize(stmtEP));
        stmtEP = NULL;

        const char di[] = "SELECT di FROM RD_DEVICE_LIST "
            "INNER JOIN RD_DEVICE_LINK_LIST ON RD_DEVICE_LINK_LIST.DEVICE_ID = RD_DEVICE_LIST.ID "
            "WHERE RD_DEVICE_LINK_LIST.DEVICE_ID=@deviceId";
        int diSize = (int)sizeof(di);
        const uint8_t di_index = 0;
        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, di, diSize, &stmtDI, NULL));
        VERIFY_SQLITE(sqlite3_bind_int64(stmtDI, sqlite3_bind_parameter_index(stmtDI, "@deviceId"), deviceId));
        res = sqlite3_step(stmtDI);
        if (SQLITE_ROW == res || SQLITE_DONE == res)
        {
            const unsigned char *tempDi = sqlite3_column_text(stmtDI, di_index);
            OIC_LOG_V(DEBUG, TAG, " %s", tempDi);
            discPayload->sid = OICStrdup((char *)tempDi);
            VERIFY_NON_NULL(discPayload->sid);
        }
        VERIFY_SQLITE(sqlite3_finalize(stmtDI));
        stmtDI = NULL;

        OCDiscoveryPayloadAddNewResource(discPayload, resourcePayload);
        resourcePayload = NULL;
        res = sqlite3_step(stmt);
    }
    result = OC_STACK_OK;

exit:
    sqlite3_finalize(stmtDI);
    sqlite3_finalize(stmtEP);
    sqlite3_finalize(stmtIF);
    sqlite3_finalize(stmtRT);
    OICFree(epPayload);
    OCDiscoveryResourceDestroy(resourcePayload);
    return result;
}

static OCStackResult CheckResources(const char *interfaceType, const char *resourceType,
        OCDiscoveryPayload *discPayload)
{
    if (!interfaceType && !resourceType)
    {
        return OC_STACK_INVALID_QUERY;
    }
    if (!discPayload || !discPayload->sid)
    {
        return OC_STACK_INTERNAL_SERVER_ERROR;
    }

    size_t sidLength = strlen(discPayload->sid);
    size_t resourceTypeLength = resourceType ? strlen(resourceType) : 0;
    size_t interfaceTypeLength = interfaceType ? strlen(interfaceType) : 0;

    if ((sidLength > INT_MAX) ||
        (resourceTypeLength > INT_MAX) ||
        (interfaceTypeLength > INT_MAX))
    {
        return OC_STACK_INVALID_QUERY;
    }

    OCStackResult result = OC_STACK_OK;
    sqlite3_stmt *stmt = NULL;
    if (resourceType)
    {
        if (!interfaceType || 0 == strcmp(interfaceType, OC_RSRVD_INTERFACE_LL) ||
                0 == strcmp(interfaceType, OC_RSRVD_INTERFACE_DEFAULT))
        {
            const char input[] = "SELECT * FROM RD_DEVICE_LINK_LIST "
                                "INNER JOIN RD_DEVICE_LIST ON RD_DEVICE_LINK_LIST.DEVICE_ID=RD_DEVICE_LIST.ID "
                                "INNER JOIN RD_LINK_RT ON RD_DEVICE_LINK_LIST.INS=RD_LINK_RT.LINK_ID "
                                "WHERE RD_DEVICE_LIST.di LIKE @di AND RD_LINK_RT.rt LIKE @resourceType";
            int inputSize = (int)sizeof(input);
            VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, input, inputSize, &stmt, NULL));
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@di"),
                            discPayload->sid, (int)sidLength, SQLITE_STATIC));
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@resourceType"),
                            resourceType, (int)resourceTypeLength, SQLITE_STATIC));
        }
        else
        {
            const char input[] = "SELECT * FROM RD_DEVICE_LINK_LIST "
                                "INNER JOIN RD_DEVICE_LIST ON RD_DEVICE_LINK_LIST.DEVICE_ID=RD_DEVICE_LIST.ID "
                                "INNER JOIN RD_LINK_RT ON RD_DEVICE_LINK_LIST.INS=RD_LINK_RT.LINK_ID "
                                "INNER JOIN RD_LINK_IF ON RD_DEVICE_LINK_LIST.INS=RD_LINK_IF.LINK_ID "
                                "WHERE RD_DEVICE_LIST.di LIKE @di "
                                "AND RD_LINK_RT.rt LIKE @resourceType "
                                "AND RD_LINK_IF.if LIKE @interfaceType";
            int inputSize = (int)sizeof(input);
            VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, input, inputSize, &stmt, NULL));
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@di"),
                            discPayload->sid, (int)sidLength, SQLITE_STATIC));
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@resourceType"),
                            resourceType, (int)resourceTypeLength, SQLITE_STATIC));
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@interfaceType"),
                            interfaceType, (int)interfaceTypeLength, SQLITE_STATIC));
        }
        result = ResourcePayloadCreate(stmt, discPayload);
    }
    else if (interfaceType)
    {
        if (0 == strcmp(interfaceType, OC_RSRVD_INTERFACE_LL) ||
                0 == strcmp(interfaceType, OC_RSRVD_INTERFACE_DEFAULT))
        {
            const char input[] = "SELECT * FROM RD_DEVICE_LINK_LIST "
                                "INNER JOIN RD_DEVICE_LIST ON RD_DEVICE_LINK_LIST.DEVICE_ID=RD_DEVICE_LIST.ID "
                                "WHERE RD_DEVICE_LIST.di LIKE @di";
            int inputSize = (int)sizeof(input);
            VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, input, inputSize, &stmt, NULL));
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@di"),
                            discPayload->sid, (int)sidLength, SQLITE_STATIC));
        }
        else
        {
            const char input[] = "SELECT * FROM RD_DEVICE_LINK_LIST "
                                "INNER JOIN RD_DEVICE_LIST ON RD_DEVICE_LINK_LIST.DEVICE_ID=RD_DEVICE_LIST.ID "
                                "INNER JOIN RD_LINK_IF ON RD_DEVICE_LINK_LIST.INS=RD_LINK_IF.LINK_ID "
                                "WHERE RD_DEVICE_LIST.di LIKE @di AND RD_LINK_IF.if LIKE @interfaceType";
            int inputSize = (int)sizeof(input);
            VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, input, inputSize, &stmt, NULL));
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@di"),
                            discPayload->sid, (int)sidLength, SQLITE_STATIC));
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@interfaceType"),
                            interfaceType, (int)interfaceTypeLength, SQLITE_STATIC));
        }
        result = ResourcePayloadCreate(stmt, discPayload);
    }

exit:
    sqlite3_finalize(stmt);
    return result;
}

OCStackResult OCRDDatabaseDiscoveryPayloadCreate(const char *interfaceType,
        const char *resourceType,
        OCDiscoveryPayload **payload)
{
    OCStackResult result;
    OCDiscoveryPayload *head = NULL;
    OCDiscoveryPayload **tail = &head;
    sqlite3_stmt *stmt = NULL;

    if (*payload)
    {
        /*
         * This is an error of the caller, return here instead of touching the
         * caller provided payload.
         */
        OIC_LOG_V(ERROR, TAG, "Payload is already allocated");
        result = OC_STACK_INTERNAL_SERVER_ERROR;
        goto exit;
    }

    if (SQLITE_OK == sqlite3_config(SQLITE_CONFIG_LOG, errorCallback))
    {
        OIC_LOG_V(INFO, TAG, "SQLite debugging log initialized.");
    }
    sqlite3_open_v2(OCRDDatabaseGetStorageFilename(), &gRDDB, SQLITE_OPEN_READONLY, NULL);
    if (!gRDDB)
    {
        result = OC_STACK_ERROR;
        goto exit;
    }

    const char *serverID = OCGetServerInstanceIDString();
    const char input[] = "SELECT di FROM RD_DEVICE_LIST";
    int inputSize = (int)sizeof(input);
    const uint8_t di_index = 0;
    VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, input, inputSize, &stmt, NULL));
    while (SQLITE_ROW == sqlite3_step(stmt))
    {
        const unsigned char *di = sqlite3_column_text(stmt, di_index);
        if (0 == strcmp((const char *)di, serverID))
        {
            continue;
        }
        *tail = OCDiscoveryPayloadCreate();
        result = OC_STACK_INTERNAL_SERVER_ERROR;
        VERIFY_NON_NULL(*tail);
        (*tail)->sid = (char *)OICCalloc(1, UUID_STRING_SIZE);
        VERIFY_NON_NULL((*tail)->sid);
        memcpy((*tail)->sid, di, UUID_STRING_SIZE);
        result = CheckResources(interfaceType, resourceType, *tail);
        if (OC_STACK_OK == result)
        {
            tail = &(*tail)->next;
        }
        else
        {
            OCPayloadDestroy((OCPayload *) *tail);
            *tail = NULL;
        }
    }
    result = head ? OC_STACK_OK : OC_STACK_NO_RESOURCE;

exit:
    if (OC_STACK_OK != result)
    {
        OCPayloadDestroy((OCPayload *) head);
        head = NULL;
    }
    *payload = head;
    sqlite3_finalize(stmt);
    sqlite3_close(gRDDB);
    return result;
}
#endif
