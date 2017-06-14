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

#include <stdlib.h>
#include <string.h>

#ifdef RD_SERVER
#include "sqlite3.h"
#endif

#include "octypes.h"
#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"
#include "oic_malloc.h"
#include "oic_string.h"

#define TAG "OIC_RI_RESOURCEDIRECTORY"

#define VERIFY_NON_NULL(arg, logLevel, retVal) { if (!(arg)) { OIC_LOG((logLevel), \
             TAG, #arg " is NULL"); return (retVal); } }

#ifdef RD_SERVER

#define RD_PATH "RD.db"

static sqlite3 *gRDDB = NULL;

static const uint8_t uri_index = 2;
static const uint8_t p_index = 5;
static const uint8_t mt_index = 7;
static const uint8_t d_index = 8;

static const uint8_t rt_value_index = 1;
static const uint8_t rt_link_id_index = 2;

static const uint8_t if_value_index = 1;
static const uint8_t if_link_id_index = 2;

#define VERIFY_SQLITE(arg) \
if (SQLITE_OK != (arg)) \
{ \
    OIC_LOG_V(ERROR, TAG, "Error in " #arg ", Error Message: %s",  sqlite3_errmsg(gRDDB)); \
    sqlite3_exec(gRDDB, "ROLLBACK", NULL, NULL, NULL); \
    return OC_STACK_ERROR; \
}

static void errorCallback(void *arg, int errCode, const char *errMsg)
{
    OC_UNUSED(arg);
    OC_UNUSED(errCode);
    OC_UNUSED(errMsg);
    OIC_LOG_V(ERROR, TAG, "SQLLite Error: %s : %d", errMsg, errCode);
}

static OCStackResult initializeDatabase(const char *path)
{
    if (SQLITE_OK == sqlite3_config(SQLITE_CONFIG_LOG, errorCallback))
    {
        OIC_LOG_V(INFO, TAG, "SQLite debugging log initialized.");
    }

    sqlite3_open_v2(!path ? RD_PATH : path, &gRDDB, SQLITE_OPEN_READONLY, NULL);
    if (!gRDDB)
    {
        return OC_STACK_ERROR;
    }
    return OC_STACK_OK;
}

static OCStackResult appendStringLL(OCStringLL **type, const unsigned char *value)
{
    OCStringLL *temp= (OCStringLL*)OICCalloc(1, sizeof(OCStringLL));
    if (!temp)
    {
        return OC_STACK_NO_MEMORY;
    }
    temp->value = OICStrdup((char *)value);
    if (!temp->value)
    {
        return OC_STACK_NO_MEMORY;
    }
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
    return OC_STACK_OK;
}

OCStackResult OCRDDatabaseCheckResources(const char *interfaceType, const char *resourceType,
    OCDiscoveryPayload *discPayload)
{
    if (initializeDatabase(NULL) != OC_STACK_OK)
    {
        return OC_STACK_INTERNAL_SERVER_ERROR;
    }
    if (!interfaceType && !resourceType)
    {
        return OC_STACK_INVALID_QUERY;
    }
    OCResourcePayload *resourcePayload = (OCResourcePayload *)OICCalloc(1, sizeof(OCResourcePayload));
    if (!resourcePayload)
    {
        return OC_STACK_NO_MEMORY;
    }

    if (resourceType)
    {
        sqlite3_stmt *stmt = 0;
        const char *input = "SELECT * FROM RD_DEVICE_LINK_LIST INNER JOIN RD_LINK_RT ON " \
        "RD_DEVICE_LINK_LIST.INS=RD_LINK_RT.LINK_ID WHERE RD_LINK_RT.rt LIKE ? ";

        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, input, -1, &stmt, NULL));
        VERIFY_SQLITE(sqlite3_bind_text(stmt, 1, resourceType, strlen(resourceType) + 1, SQLITE_STATIC));

        int res = sqlite3_step (stmt);
        if (res == SQLITE_ROW || res == SQLITE_DONE)
        {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *uri = sqlite3_column_text(stmt, uri_index - 1);
            int bitmap = sqlite3_column_int(stmt, p_index - 1);
            int deviceId = sqlite3_column_int(stmt, d_index - 1);
            OIC_LOG_V(DEBUG, TAG, " %s %d", uri, deviceId);
            resourcePayload->uri = OICStrdup((char *)uri);
            if (!resourcePayload->uri)
            {
                OCDiscoveryResourceDestroy(resourcePayload);
                return OC_STACK_NO_MEMORY;
            }
            res = sqlite3_reset(stmt);
            VERIFY_SQLITE(res);

            sqlite3_stmt *stmtRT = 0;
            const char *rt = "SELECT rt FROM RD_LINK_RT WHERE LINK_ID=?";
            VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, rt, -1, &stmtRT, NULL));
            VERIFY_SQLITE(sqlite3_bind_int(stmtRT, 1, id));
            while (SQLITE_ROW == sqlite3_step(stmtRT))
            {
                const unsigned char *rt1 = sqlite3_column_text(stmtRT, (rt_value_index - 1));
                appendStringLL(&resourcePayload->types, rt1);
            }

            sqlite3_stmt *stmtIF = 0;
            const char *itf = "SELECT if FROM RD_LINK_IF WHERE LINK_ID=?";
            VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, itf, -1, &stmtIF, NULL));
            VERIFY_SQLITE(sqlite3_bind_int(stmtIF, 1, id));
            while (SQLITE_ROW == sqlite3_step(stmtIF))
            {
                const unsigned char *itf = sqlite3_column_text(stmtIF, (if_value_index - 1));
                appendStringLL(&resourcePayload->interfaces, itf);
            }

            resourcePayload->bitmap = bitmap & (OC_OBSERVABLE | OC_DISCOVERABLE);
            resourcePayload->secure = (bitmap & OC_SECURE) != 0;

            const char *address = "SELECT di, address FROM RD_DEVICE_LIST INNER JOIN RD_DEVICE_LINK_LIST ON " \
            "RD_DEVICE_LINK_LIST.DEVICE_ID = RD_DEVICE_LIST.ID WHERE RD_DEVICE_LINK_LIST.DEVICE_ID=?";

            sqlite3_stmt *stmt1 = 0;
            VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, address, -1, &stmt1, NULL));
            VERIFY_SQLITE(sqlite3_bind_int(stmt1, 1, deviceId));
            // TODO: Right now, we have a bug where discovery payload can only send one device information.
            res = sqlite3_step(stmt1);
            if (res == SQLITE_ROW || res == SQLITE_DONE)
            {
                const unsigned char *di = sqlite3_column_text(stmt1, 0);
                const unsigned char *address = sqlite3_column_text(stmt1, 1);
                OIC_LOG_V(DEBUG, TAG, " %s %s", di, address);
                (discPayload)->baseURI = OICStrdup((char *)address);
                (discPayload)->sid = OICStrdup((char *)di);
            }
            OCDiscoveryPayloadAddNewResource(discPayload, resourcePayload);
        }
    }
    if (interfaceType)
    {
        sqlite3_stmt *stmt = 0;
        const char *input = "SELECT * FROM RD_DEVICE_LINK_LIST INNER JOIN RD_LINK_IF ON " \
        "RD_DEVICE_LINK_LIST.INS=RD_LINK_IF.LINK_ID WHERE RD_LINK_IF.if LIKE ? ";

        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, input, -1, &stmt, NULL));
        VERIFY_SQLITE(sqlite3_bind_text(stmt, 1, interfaceType, strlen(interfaceType) + 1, SQLITE_STATIC));

        int res = sqlite3_step (stmt);
        if (res == SQLITE_ROW || res == SQLITE_DONE)
        {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *uri = sqlite3_column_text(stmt, uri_index - 1);
            int bitmap = sqlite3_column_int(stmt, p_index - 1);
            int deviceId = sqlite3_column_int(stmt, d_index - 1);
            OIC_LOG_V(DEBUG, TAG, " %s %d", uri, deviceId);
            resourcePayload->uri = OICStrdup((char *)uri);
            if (!resourcePayload->uri)
            {
                OCDiscoveryResourceDestroy(resourcePayload);
                return OC_STACK_NO_MEMORY;
            }
            VERIFY_SQLITE(sqlite3_reset(stmt));

            sqlite3_stmt *stmtRT = 0;
            const char *rt = "SELECT rt FROM RD_LINK_RT WHERE LINK_ID=?";
            VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, rt, -1, &stmtRT, NULL));
            VERIFY_SQLITE(sqlite3_bind_int(stmtRT, 1, id));
            while (SQLITE_ROW == sqlite3_step(stmtRT))
            {
                const unsigned char *rt1 = sqlite3_column_text(stmtRT, (rt_value_index - 1));
                appendStringLL(&resourcePayload->types, rt1);
            }

            sqlite3_stmt *stmtIF = 0;
            const char *itf = "SELECT if FROM RD_LINK_IF WHERE LINK_ID=?";
            VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, itf, -1, &stmtIF, NULL));
            VERIFY_SQLITE(sqlite3_bind_int(stmtIF, 1, id));
            while (SQLITE_ROW == sqlite3_step (stmtIF))
            {
                const unsigned char *itf = sqlite3_column_text(stmtIF, (if_value_index - 1));
                appendStringLL(&resourcePayload->interfaces, itf);
            }

            resourcePayload->bitmap = bitmap & (OC_OBSERVABLE | OC_DISCOVERABLE);
            resourcePayload->secure = ((bitmap & OC_SECURE) != 0);

            const char *address = "SELECT di, address FROM RD_DEVICE_LIST INNER JOIN RD_DEVICE_LINK_LIST ON " \
            "RD_DEVICE_LINK_LIST.DEVICE_ID = RD_DEVICE_LIST.ID WHERE RD_DEVICE_LINK_LIST.DEVICE_ID=?";

            sqlite3_stmt *stmt1 = 0;
            VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, address, -1, &stmt1, NULL));
            VERIFY_SQLITE(sqlite3_bind_int(stmt1, 1, deviceId));

            res = sqlite3_step(stmt1);
            if (res == SQLITE_ROW || res == SQLITE_DONE)
            {
                const unsigned char *di = sqlite3_column_text(stmt1, 0);
                const unsigned char *address = sqlite3_column_text(stmt1, 1);
                OIC_LOG_V(DEBUG, TAG, " %s %s", di, address);
                (discPayload)->baseURI = OICStrdup((char *)address);
                (discPayload)->sid = OICStrdup((char *)di);
            }
            OCDiscoveryPayloadAddNewResource(discPayload, resourcePayload);
        }
    }
    return OC_STACK_OK;
}
#endif
