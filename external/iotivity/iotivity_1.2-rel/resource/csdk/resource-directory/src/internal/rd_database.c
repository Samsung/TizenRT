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

#include "sqlite3.h"
#include "logger.h"
#include "ocpayload.h"
#include "octypes.h"
#include "oic_malloc.h"
#include "oic_string.h"

#ifdef RD_SERVER

#define TAG "RD_DATABASE"
#define RD_PATH "RD.db"

static sqlite3 *gRDDB = NULL;

static const uint8_t device_index = 2;
static const uint8_t ttl_index = 3;
static const uint8_t address_index = 4;

static const uint8_t uri_index = 2;
static const uint8_t p_index = 5;
static const uint8_t mt_index = 7;
static const uint8_t d_index = 8;

static const uint8_t rt_value_index = 1;
static const uint8_t rt_link_id_index = 2;

static const uint8_t if_value_index = 1;
static const uint8_t if_link_id_index = 2;

static const uint8_t bind_index_value = 1;

#define VERIFY_SQLITE(arg) \
    if (SQLITE_OK != (arg)) \
    { \
        OIC_LOG_V(ERROR, TAG, "Error in " #arg ", Error Message: %s",  sqlite3_errmsg(gRDDB)); \
        sqlite3_exec(gRDDB, "ROLLBACK", NULL, NULL, NULL); \
        return OC_STACK_ERROR; \
    }

#define CHECK_DATABASE_INIT \
    if (!gRDDB) \
    { \
        OIC_LOG(ERROR, TAG, "Database is not initialized."); \
        return OC_STACK_ERROR; \
    }

#define STR(a) #a
#define XSTR(a) STR(a)

#define RD_TABLE \
    "create table RD_DEVICE_LIST(ID INTEGER PRIMARY KEY AUTOINCREMENT, " \
    XSTR(OC_RSRVD_DEVICE_ID) " UNIQUE NOT NULL, " \
    XSTR(OC_RSRVD_TTL) " NOT NULL, " \
    "ADDRESS NOT NULL);"

#define RD_LL_TABLE  \
    "create table RD_DEVICE_LINK_LIST("XSTR(OC_RSRVD_INS)" INTEGER PRIMARY KEY AUTOINCREMENT, " \
    XSTR(OC_RSRVD_HREF)  "," \
    XSTR(OC_RSRVD_REL)   ","  \
    XSTR(OC_RSRVD_TITLE) "," \
    XSTR(OC_RSRVD_BITMAP)"," \
    XSTR(OC_RSRVD_TTL)   "," \
    XSTR(OC_RSRVD_MEDIA_TYPE) "," \
    "DEVICE_ID INT NOT NULL, " \
    "FOREIGN KEY(DEVICE_ID) REFERENCES RD_DEVICE_LIST(ID) ON DELETE CASCADE );"

#define RD_RT_TABLE \
    "create table RD_LINK_RT(" XSTR(OC_RSRVD_RESOURCE_TYPE) " NOT NULL, " \
    "LINK_ID INT NOT NULL, "\
    "FOREIGN KEY("XSTR(LINK_ID)") REFERENCES RD_DEVICE_LINK_LIST("XSTR(OC_RSRVD_INS)") " \
    "ON DELETE CASCADE);"

#define RD_IF_TABLE \
    "create table RD_LINK_IF(" XSTR(OC_RSRVD_INTERFACE) " NOT NULL, " \
    "LINK_ID INT NOT NULL, "\
    "FOREIGN KEY("XSTR(LINK_ID)") REFERENCES RD_DEVICE_LINK_LIST("XSTR(OC_RSRVD_INS)") " \
    "ON DELETE CASCADE);"

static void errorCallback(void *arg, int errCode, const char *errMsg)
{
    OC_UNUSED(arg);
    OC_UNUSED(errCode);
    OC_UNUSED(errMsg);
    OIC_LOG_V(ERROR, TAG, "SQLLite Error: %s : %d", errMsg, errCode);
}

OCStackResult OCRDDatabaseInit(const char *path)
{
    if (SQLITE_OK == sqlite3_config(SQLITE_CONFIG_LOG, errorCallback))
    {
        OIC_LOG_V(INFO, TAG, "SQLite debugging log initialized.");
    }

    int sqlRet = sqlite3_open_v2(!path ? RD_PATH : path, &gRDDB, SQLITE_OPEN_READWRITE, NULL);
    if (SQLITE_OK != sqlRet)
    {
        OIC_LOG(DEBUG, TAG, "RD database file did not open, as no table exists.");
        OIC_LOG(DEBUG, TAG, "RD creating new table.");
        sqlRet = sqlite3_open_v2(!path ? RD_PATH : path, &gRDDB,
                                 SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        if (SQLITE_OK == sqlRet)
        {
            VERIFY_SQLITE(sqlite3_exec(gRDDB, RD_TABLE, NULL, NULL, NULL));
            OIC_LOG(DEBUG, TAG, "RD created RD_DEVICE_LIST table.");

            VERIFY_SQLITE(sqlite3_exec(gRDDB, RD_LL_TABLE, NULL, NULL, NULL));
            OIC_LOG(DEBUG, TAG, "RD created RD_DEVICE_LINK_LIST table.");

            VERIFY_SQLITE(sqlite3_exec(gRDDB, RD_RT_TABLE, NULL, NULL, NULL));
            OIC_LOG(DEBUG, TAG, "RD created RD_LINK_RT table.");

            VERIFY_SQLITE(sqlite3_exec(gRDDB, RD_IF_TABLE, NULL, NULL, NULL));
            OIC_LOG(DEBUG, TAG, "RD created RD_LINK_IF table.");
            sqlRet = SQLITE_OK;
        }
    }

    if (sqlRet == SQLITE_OK)
    {
        sqlite3_stmt *stmt = 0;
        VERIFY_SQLITE(sqlite3_prepare_v2 (gRDDB, "PRAGMA foreign_keys = ON;", -1, &stmt, NULL));

        if (SQLITE_DONE != sqlite3_step(stmt))
        {
            sqlite3_finalize(stmt);
            return OC_STACK_ERROR;
        }

        VERIFY_SQLITE(sqlite3_finalize(stmt));
    }

    return OC_STACK_OK;
}

OCStackResult OCRDDatabaseClose()
{
    CHECK_DATABASE_INIT;
    VERIFY_SQLITE(sqlite3_close_v2(gRDDB));
    return OC_STACK_OK;
}

static int storeResourceType(char **link, size_t size, uint8_t rowid)
{
    int res = 1;
    VERIFY_SQLITE(sqlite3_exec(gRDDB, "BEGIN TRANSACTION", NULL, NULL, NULL));

    const char *insertRT = "INSERT INTO RD_LINK_RT VALUES(?, ?)";
    sqlite3_stmt *stmtRT = 0;

    for (size_t i = 0; i < size; i++)
    {
        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, insertRT, strlen(insertRT) + 1, &stmtRT, NULL));
        if (link[i])
        {
            VERIFY_SQLITE(sqlite3_bind_text(stmtRT, rt_value_index, link[i],
                    strlen(link[i])+1, SQLITE_STATIC));

            VERIFY_SQLITE(sqlite3_bind_int(stmtRT, rt_link_id_index, rowid));
        }
        if (SQLITE_DONE != sqlite3_step(stmtRT))
        {
            sqlite3_finalize(stmtRT);
            return res;
        }
    }

    VERIFY_SQLITE(sqlite3_finalize(stmtRT));

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "COMMIT", NULL, NULL, NULL));
    res = SQLITE_OK;

    return res;
}


static int storeInterfaceType(char **link, size_t size, uint8_t rowid)
{
    int res = 1;

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "BEGIN TRANSACTION", NULL, NULL, NULL));

    const char *insertIF = "INSERT INTO RD_LINK_IF VALUES(?, ?)";
    sqlite3_stmt *stmtIF = 0;

    for (size_t i = 0; i < size; i++)
    {
        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, insertIF, strlen(insertIF) + 1, &stmtIF, NULL));

        if (link[i])
        {
            VERIFY_SQLITE(sqlite3_bind_text(stmtIF, if_value_index, link[i], strlen(link[i])+1, SQLITE_STATIC));
            VERIFY_SQLITE(sqlite3_bind_int(stmtIF, if_link_id_index, rowid));
        }
        if (SQLITE_DONE != sqlite3_step(stmtIF))
        {
            res = sqlite3_finalize(stmtIF);
            return res;
        }
    }
    VERIFY_SQLITE(sqlite3_finalize(stmtIF));

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "COMMIT", NULL, NULL, NULL));
    res = SQLITE_OK;

    return res;
}

static int storeLinkPayload(OCRepPayload *rdPayload, int64_t rowid)
{
    OCRepPayload **links = NULL;
    size_t dimensions[MAX_REP_ARRAY_DEPTH];
    int res = 1 ;
    size_t j = 0;

    if (OCRepPayloadGetPropObjectArray(rdPayload, OC_RSRVD_LINKS, &links, dimensions))
    {
        const char *insertDeviceLLList = "INSERT INTO RD_DEVICE_LINK_LIST VALUES(?,?,?,?,?,?,?,?)";
        sqlite3_stmt *stmt = 0;

        for (size_t i = 0; i < dimensions[0]; i++)
        {
            VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, insertDeviceLLList, strlen(insertDeviceLLList) + 1, &stmt, NULL));
            VERIFY_SQLITE(sqlite3_exec(gRDDB, "BEGIN TRANSACTION", NULL, NULL, NULL));

            OCRepPayload *link = links[i];
            char *uri = NULL;
            if (OCRepPayloadGetPropString(link, OC_RSRVD_HREF, &uri))
            {
                VERIFY_SQLITE(sqlite3_bind_text(stmt, uri_index, uri, strlen(uri), SQLITE_STATIC));
            }

            OCRepPayload *p = NULL;
            if (OCRepPayloadGetPropObject(link, OC_RSRVD_POLICY, &p))
            {
                int64_t bm = 0;
                if (OCRepPayloadGetPropInt(p, OC_RSRVD_BITMAP, &bm))
                {
                    VERIFY_SQLITE(sqlite3_bind_int(stmt, p_index, bm));
                }
            }

            size_t mtDim[MAX_REP_ARRAY_DEPTH] = {0};
            char **mediaType = NULL;
            if (OCRepPayloadGetStringArray(link, OC_RSRVD_MEDIA_TYPE, &mediaType, mtDim))
            {
                VERIFY_SQLITE(sqlite3_bind_text(stmt, mt_index, mediaType[0], mtDim[0], SQLITE_STATIC));
            }

            VERIFY_SQLITE(sqlite3_bind_int(stmt, d_index, rowid));

            size_t rtDim[MAX_REP_ARRAY_DEPTH] = {0};
            char **rt = NULL;
            OCRepPayloadGetStringArray(link, OC_RSRVD_RESOURCE_TYPE, &rt, rtDim);

            size_t itfDim[MAX_REP_ARRAY_DEPTH] = {0};
            char **itf = NULL;
            OCRepPayloadGetStringArray(link, OC_RSRVD_INTERFACE, &itf, itfDim);

            if (SQLITE_DONE != sqlite3_step(stmt))
            {
                sqlite3_finalize(stmt);
                return res;
            }
            VERIFY_SQLITE(sqlite3_exec(gRDDB, "COMMIT", NULL, NULL, NULL));

            int64_t ins = sqlite3_last_insert_rowid(gRDDB);
            VERIFY_SQLITE(storeResourceType(rt, rtDim[0], ins));
            VERIFY_SQLITE(storeInterfaceType(itf, itfDim[0], ins));
            OICFree(uri);
            OCPayloadDestroy((OCPayload *)p);
            for (j = 0; j < mtDim[0]; j++)
            {
                OICFree(mediaType[j]);
            }
            OICFree(mediaType);

            for (j = 0; j < rtDim[0]; j++)
            {
                OICFree(rt[j]);
            }
            OICFree(rt);

            for (j = 0; j < itfDim[0]; j++)
            {
                OICFree(itf[j]);
            }
            OICFree(itf);

        }

        VERIFY_SQLITE(sqlite3_finalize(stmt));
        res = SQLITE_OK;
    }
    return res;
}

OCStackResult OCRDDatabaseStoreResources(OCRepPayload *payload, const OCDevAddr *address)
{
    CHECK_DATABASE_INIT;

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "BEGIN TRANSACTION", NULL, NULL, NULL));
    const char *insertDeviceList = "INSERT INTO RD_DEVICE_LIST VALUES(?,?,?,?)";
    sqlite3_stmt *stmt = 0;
    VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, insertDeviceList, strlen(insertDeviceList) + 1, &stmt, NULL));

    char *deviceid = NULL;
    if (OCRepPayloadGetPropString(payload, OC_RSRVD_DEVICE_ID, &deviceid))
    {
        VERIFY_SQLITE(sqlite3_bind_text(stmt, device_index, deviceid, strlen(deviceid) + 1, SQLITE_STATIC));
    }

    int64_t ttl = 0;
    if (OCRepPayloadGetPropInt(payload, OC_RSRVD_DEVICE_TTL, &ttl))
    {
        VERIFY_SQLITE(sqlite3_bind_int(stmt, ttl_index, ttl));
    }

    char rdAddress[MAX_URI_LENGTH];
    snprintf(rdAddress, MAX_URI_LENGTH, "%s:%d", address->addr, address->port);
    OIC_LOG_V(DEBUG, TAG, "Address: %s", rdAddress);
    VERIFY_SQLITE(sqlite3_bind_text(stmt, address_index, rdAddress, strlen(rdAddress) + 1, SQLITE_STATIC));

    if (SQLITE_DONE != sqlite3_step(stmt))
    {
        sqlite3_finalize(stmt);
        return OC_STACK_ERROR;
    }
    VERIFY_SQLITE(sqlite3_finalize(stmt));

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "COMMIT", NULL, NULL, NULL));

    int64_t rowid = sqlite3_last_insert_rowid(gRDDB);
    if (rowid)
    {
        VERIFY_SQLITE(storeLinkPayload(payload, rowid));
    }
    OICFree(deviceid);
    return OC_STACK_OK;
}

OCStackResult OCRDDatabaseDeleteDevice(const char *deviceId)
{
    CHECK_DATABASE_INIT;
    VERIFY_SQLITE(sqlite3_exec(gRDDB, "BEGIN TRANSACTION", NULL, NULL, NULL));

    sqlite3_stmt *stmt = 0;
    char *delDevice = "DELETE FROM RD_DEVICE_LIST WHERE "XSTR(OC_RSRVD_DEVICE_ID)" = ?";
    VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, delDevice, strlen(delDevice) + 1, &stmt, NULL));

    VERIFY_SQLITE(sqlite3_bind_text(stmt, 1, deviceId, strlen(deviceId) + 1, SQLITE_STATIC));

    if (SQLITE_DONE != sqlite3_step(stmt))
    {
        sqlite3_exec(gRDDB, "ROLLBACK", NULL, NULL, NULL);
        sqlite3_finalize(stmt);
        return OC_STACK_ERROR;
    }
    VERIFY_SQLITE(sqlite3_finalize(stmt));
    VERIFY_SQLITE(sqlite3_exec(gRDDB, "COMMIT", NULL, NULL, NULL));

    return OC_STACK_OK;
}

#endif
