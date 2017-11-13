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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "sqlite3.h"
#include "logger.h"
#include "ocpayload.h"
#include "octypes.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "oic_time.h"
#include "ocstackinternal.h"

#ifdef RD_SERVER

#define TAG "OIC_RD_DATABASE"

static sqlite3 *gRDDB = NULL;

#define CHECK_DATABASE_INIT \
    if (!gRDDB) \
    { \
        OIC_LOG(ERROR, TAG, "Database is not initialized."); \
        return OC_STACK_ERROR; \
    }

#define VERIFY_SQLITE(arg) \
    if (SQLITE_OK != (res = (arg))) \
    { \
        OIC_LOG_V(ERROR, TAG, "Error in " #arg ", Error Message: %s",  sqlite3_errmsg(gRDDB)); \
        goto exit; \
    }

#define STR(a) #a
#define XSTR(a) STR(a)

#define RD_TABLE \
    "create table RD_DEVICE_LIST(ID INTEGER PRIMARY KEY AUTOINCREMENT, " \
    XSTR(OC_RSRVD_DEVICE_ID) " UNIQUE NOT NULL, " \
    XSTR(OC_RSRVD_TTL) " NOT NULL," \
    "EXTERNAL_HOST INTEGER NOT NULL);"

#define RD_LL_TABLE  \
    "create table RD_DEVICE_LINK_LIST("XSTR(OC_RSRVD_INS)" INTEGER PRIMARY KEY AUTOINCREMENT, " \
    XSTR(OC_RSRVD_HREF) "," \
    XSTR(OC_RSRVD_REL) ","  \
    XSTR(OC_RSRVD_URI) "," \
    XSTR(OC_RSRVD_BITMAP) "," \
    "DEVICE_ID INT NOT NULL, " \
    "FOREIGN KEY(DEVICE_ID) REFERENCES RD_DEVICE_LIST(ID) ON DELETE CASCADE);"

#define RD_RT_TABLE \
    "create table RD_LINK_RT(" XSTR(OC_RSRVD_RESOURCE_TYPE) " NOT NULL, " \
    "LINK_ID INT NOT NULL, " \
    "FOREIGN KEY("XSTR(LINK_ID)") REFERENCES RD_DEVICE_LINK_LIST("XSTR(OC_RSRVD_INS)") " \
    "ON DELETE CASCADE);"

#define RD_IF_TABLE \
    "create table RD_LINK_IF(" XSTR(OC_RSRVD_INTERFACE) " NOT NULL, " \
    "LINK_ID INT NOT NULL, "\
    "FOREIGN KEY("XSTR(LINK_ID)") REFERENCES RD_DEVICE_LINK_LIST("XSTR(OC_RSRVD_INS)") " \
    "ON DELETE CASCADE);"

#define RD_EP_TABLE \
    "create table RD_LINK_EP(" XSTR(OC_RSRVD_ENDPOINT) " NOT NULL, " \
    XSTR(OC_RSRVD_PRIORITY) " INT NOT NULL, " \
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

static bool stringArgumentNonNullAndWithinBounds(const char* argument)
{
    return ((NULL != argument) && (strlen(argument) <= INT_MAX));
}

static bool stringArgumentWithinBounds(const char* argument)
{
    return ((NULL == argument) || (strlen(argument) <= INT_MAX));
}

static bool stringArgumentsWithinBounds(const char** arguments, size_t count)
{
    for (size_t index = 0; index < count; index++)
    {
        if (!stringArgumentWithinBounds(arguments[index]))
        {
            return false;
        }
    }

    return true;
}

static int storeResourceTypes(const char **resourceTypes, size_t size, sqlite3_int64 rowid)
{
    int res = SQLITE_ERROR;
    sqlite3_stmt *stmt = NULL;
    if (!stringArgumentsWithinBounds(resourceTypes, size))
    {
        return res;
    }

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "SAVEPOINT storeResourceTypes", NULL, NULL, NULL));

    static const char deleteRT[] = "DELETE FROM RD_LINK_RT WHERE LINK_ID=@id";
    static const char insertRT[] = "INSERT INTO RD_LINK_RT VALUES(@resourceType, @id)";
    int deleteRTSize = (int)sizeof(deleteRT);
    int insertRTSize = (int)sizeof(insertRT);

    VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, deleteRT, deleteRTSize, &stmt, NULL));
    VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@id"), rowid));
    res = sqlite3_step(stmt);
    if (SQLITE_DONE != res)
    {
        goto exit;
    }
    VERIFY_SQLITE(sqlite3_finalize(stmt));
    stmt = NULL;

    for (size_t i = 0; i < size; i++)
    {
        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, insertRT, insertRTSize, &stmt, NULL));
        if (resourceTypes[i])
        {
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@resourceType"),
                            resourceTypes[i], (int)strlen(resourceTypes[i]), SQLITE_STATIC));
            VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@id"), rowid));
        }
        res = sqlite3_step(stmt);
        if (SQLITE_DONE != res)
        {
            goto exit;
        }
        VERIFY_SQLITE(sqlite3_finalize(stmt));
        stmt = NULL;
    }

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "RELEASE storeResourceTypes", NULL, NULL, NULL));
    res = SQLITE_OK;

exit:
    sqlite3_finalize(stmt);
    if (SQLITE_OK != res)
    {
        sqlite3_exec(gRDDB, "ROLLBACK TO storeResourceTypes", NULL, NULL, NULL);
    }
    return res;
}

static int storeInterfaces(const char **interfaces, size_t size, sqlite3_int64 rowid)
{
    int res = SQLITE_ERROR;
    sqlite3_stmt *stmt = NULL;
    if (!stringArgumentsWithinBounds(interfaces, size))
    {
        return res;
    }

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "SAVEPOINT storeInterfaces", NULL, NULL, NULL));

    static const char deleteIF[] = "DELETE FROM RD_LINK_IF WHERE LINK_ID=@id";
    static const char insertIF[] = "INSERT INTO RD_LINK_IF VALUES(@interfaceType, @id)";
    int deleteIFLength = (int)sizeof(deleteIF);
    int insertIFLength = (int)sizeof(insertIF);

    VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, deleteIF, deleteIFLength, &stmt, NULL));
    VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@id"), rowid));
    res = sqlite3_step(stmt);
    if (SQLITE_DONE != res)
    {
        goto exit;
    }
    VERIFY_SQLITE(sqlite3_finalize(stmt));
    stmt = NULL;

    for (size_t i = 0; i < size; i++)
    {
        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, insertIF, insertIFLength, &stmt, NULL));
        if (interfaces[i])
        {
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@interfaceType"),
                            interfaces[i], (int)strlen(interfaces[i]), SQLITE_STATIC));
            VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@id"), rowid));
        }
        res = sqlite3_step(stmt);
        if (SQLITE_DONE != res)
        {
            goto exit;
        }
        VERIFY_SQLITE(sqlite3_finalize(stmt));
        stmt = NULL;
    }

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "RELEASE storeInterfaces", NULL, NULL, NULL));
    res = SQLITE_OK;

exit:
    sqlite3_finalize(stmt);
    if (SQLITE_OK != res)
    {
        sqlite3_exec(gRDDB, "ROLLBACK TO storeInterfaces", NULL, NULL, NULL);
    }
    return res;
}

static int storeEndpoints(OCRepPayload **eps, size_t size, sqlite3_int64 rowid)
{
    int res;
    char *ep = NULL;
    sqlite3_stmt *stmt = NULL;

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "SAVEPOINT storeEndpoints", NULL, NULL, NULL));
    static const char deleteEp[] = "DELETE FROM RD_LINK_EP WHERE LINK_ID=@id";
    static const char insertEp[] = "INSERT INTO RD_LINK_EP VALUES(@ep, @pri, @id)";
    int deleteEpLength = (int)sizeof(deleteEp);
    int insertEpLength = (int)sizeof(insertEp);

    VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, deleteEp, deleteEpLength, &stmt, NULL));
    VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@id"), rowid));
    res = sqlite3_step(stmt);
    if (SQLITE_DONE != res)
    {
        goto exit;
    }
    VERIFY_SQLITE(sqlite3_finalize(stmt));
    stmt = NULL;

    for (size_t i = 0; i < size; i++)
    {
        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, insertEp, insertEpLength, &stmt, NULL));
        if (OCRepPayloadGetPropString(eps[i], OC_RSRVD_ENDPOINT, &ep))
        {
            if (!stringArgumentWithinBounds(ep))
            {
                goto exit;
            }
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@ep"),
                            ep, (int)strlen(ep), SQLITE_STATIC));
        }
        sqlite3_int64 pri = 1;
        OCRepPayloadGetPropInt(eps[i], OC_RSRVD_PRIORITY, (int64_t *) &pri);
        VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@pri"), pri));
        VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@id"), rowid));
        res = sqlite3_step(stmt);
        if (SQLITE_DONE != res)
        {
            goto exit;
        }
        VERIFY_SQLITE(sqlite3_finalize(stmt));
        stmt = NULL;
        OICFree(ep);
        ep = NULL;
    }

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "RELEASE storeEndpoints", NULL, NULL, NULL));
    res = SQLITE_OK;

exit:
    OICFree(ep);
    sqlite3_finalize(stmt);
    if (SQLITE_OK != res)
    {
        sqlite3_exec(gRDDB, "ROLLBACK TO storeInterfaces", NULL, NULL, NULL);
    }
    return res;
}

static OCRepPayloadValue *getLinks(const OCRepPayload *rdPayload)
{
    /*
     * Iterate over the properties manually rather than OCRepPayloadGetPropObjectArray to avoid
     * the clone since we want to insert the 'ins' values into the payload.
     */
    OCRepPayloadValue *links = NULL;
    if (rdPayload)
    {
        for (links = rdPayload->values; links; links = links->next)
        {
            if (0 == strcmp(links->name, OC_RSRVD_LINKS))
            {
                if (links->type != OCREP_PROP_ARRAY || links->arr.type != OCREP_PROP_OBJECT)
                {
                    links = NULL;
                }
                break;
            }
        }
    }
    return links;
}

static int storeLinkPayload(OCRepPayloadValue *links, sqlite3_int64 rowid)
{
    int res = SQLITE_OK;

    sqlite3_stmt *stmt = NULL;
    char *uri = NULL;
    char *anchor = NULL;
    OCRepPayload *p = NULL;
    char **rt = NULL;
    size_t rtDim[MAX_REP_ARRAY_DEPTH] = {0};
    char **itf = NULL;
    size_t itfDim[MAX_REP_ARRAY_DEPTH] = {0};
    OCRepPayload** eps = NULL;
    size_t epsDim[MAX_REP_ARRAY_DEPTH] = {0};

    static const char insertDeviceLLList[] = "INSERT OR IGNORE INTO RD_DEVICE_LINK_LIST (ins, href, DEVICE_ID) "
        "VALUES((SELECT ins FROM RD_DEVICE_LINK_LIST WHERE DEVICE_ID=@id AND href=@uri),@uri,@id)";
    static const char updateDeviceLLList[] = "UPDATE RD_DEVICE_LINK_LIST SET anchor=@anchor,bm=@bm "
        "WHERE DEVICE_ID=@id AND href=@uri";
    int insertDeviceLLListSize = (int)sizeof(insertDeviceLLList);
    int updateDeviceLLListSize = (int)sizeof(updateDeviceLLList);

    assert(links);
    for (size_t i = 0; (SQLITE_OK == res) && (i < links->arr.dimensions[0]); i++)
    {
        VERIFY_SQLITE(sqlite3_exec(gRDDB, "SAVEPOINT storeLinkPayload", NULL, NULL, NULL));

        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, insertDeviceLLList,
                        insertDeviceLLListSize, &stmt, NULL));

        OCRepPayload *link = links->arr.objArray[i];
        VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@id"), rowid));
        if (OCRepPayloadGetPropString(link, OC_RSRVD_HREF, &uri))
        {
            if (!stringArgumentWithinBounds(uri))
            {
                return SQLITE_ERROR;
            }
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@uri"),
                            uri, (int)strlen(uri), SQLITE_STATIC));
        }
        res = sqlite3_step(stmt);
        if (SQLITE_DONE != res)
        {
            goto exit;
        }
        VERIFY_SQLITE(sqlite3_finalize(stmt));
        stmt = NULL;

        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, updateDeviceLLList,
                        updateDeviceLLListSize, &stmt, NULL));
        VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@id"), rowid));
        if (uri)
        {
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@uri"),
                            uri, (int)strlen(uri), SQLITE_STATIC));
        }
        if (OCRepPayloadGetPropString(link, OC_RSRVD_URI, &anchor))
        {
            if (!stringArgumentWithinBounds(anchor))
            {
                goto exit;
            }
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@anchor"),
                            anchor, (int)strlen(anchor), SQLITE_STATIC));
        }
        if (OCRepPayloadGetPropObject(link, OC_RSRVD_POLICY, &p))
        {
            sqlite3_int64 bm = 0;
            if (OCRepPayloadGetPropInt(p, OC_RSRVD_BITMAP, (int64_t *) &bm))
            {
                VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@bm"), bm));
            }
        }
        res = sqlite3_step(stmt);
        if (SQLITE_DONE != res)
        {
            goto exit;
        }
        VERIFY_SQLITE(sqlite3_finalize(stmt));
        stmt = NULL;

        static const char input[] = "SELECT ins FROM RD_DEVICE_LINK_LIST WHERE DEVICE_ID=@id AND href=@uri";
        int inputSize = (int)sizeof(input);

        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, input, inputSize, &stmt, NULL));
        VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@id"), rowid));
        if (uri)
        {
            VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@uri"),
                            uri, (int)strlen(uri), SQLITE_STATIC));
        }
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW || res == SQLITE_DONE)
        {
            sqlite3_int64 ins = sqlite3_column_int64(stmt, 0);
            VERIFY_SQLITE(sqlite3_finalize(stmt));
            stmt = NULL;
            if (!OCRepPayloadSetPropInt(link, OC_RSRVD_INS, ins))
            {
                OIC_LOG_V(ERROR, TAG, "Error setting 'ins' value");
                return SQLITE_ERROR;
            }
            OCRepPayloadGetStringArray(link, OC_RSRVD_RESOURCE_TYPE, &rt, rtDim);
            OCRepPayloadGetStringArray(link, OC_RSRVD_INTERFACE, &itf, itfDim);
            OCRepPayloadGetPropObjectArray(link, OC_RSRVD_ENDPOINTS, &eps, epsDim);
            VERIFY_SQLITE(storeResourceTypes((const char **) rt, rtDim[0], ins));
            VERIFY_SQLITE(storeInterfaces((const char **) itf, itfDim[0], ins));
            VERIFY_SQLITE(storeEndpoints(eps, epsDim[0], ins));
        }
        else
        {
            VERIFY_SQLITE(sqlite3_finalize(stmt));
            stmt = NULL;
        }

        VERIFY_SQLITE(sqlite3_exec(gRDDB, "RELEASE storeLinkPayload", NULL, NULL, NULL));
        res = SQLITE_OK;

    exit:
        if (eps)
        {
            for (size_t j = 0; j < epsDim[0]; j++)
            {
                OCRepPayloadDestroy(eps[j]);
            }
            OICFree(eps);
            eps = NULL;
        }
        if (itf)
        {
            for (size_t j = 0; j < itfDim[0]; j++)
            {
                OICFree(itf[j]);
            }
            OICFree(itf);
            itf = NULL;
        }
        if (rt)
        {
            for (size_t j = 0; j < rtDim[0]; j++)
            {
                OICFree(rt[j]);
            }
            OICFree(rt);
            rt = NULL;
        }
        OCPayloadDestroy((OCPayload *)p);
        p = NULL;
        OICFree(anchor);
        anchor = NULL;
        OICFree(uri);
        uri = NULL;
        sqlite3_finalize(stmt);
        stmt = NULL;
        if (SQLITE_OK != res)
        {
            sqlite3_exec(gRDDB, "ROLLBACK TO storeLinkPayload", NULL, NULL, NULL);
        }
    }

    return res;
}

static int storeResources(const OCRepPayload *payload, bool externalHost)
{
    sqlite3_stmt *stmt = NULL;

    /* di is a required property */
    char *deviceId = NULL;
    OCRepPayloadGetPropString(payload, OC_RSRVD_DEVICE_ID, &deviceId);
    if (!stringArgumentNonNullAndWithinBounds(deviceId))
    {
        return SQLITE_ERROR;
    }

    /* ttl is a required property */
    int64_t tmp = 0;
    if (!OCRepPayloadGetPropInt(payload, OC_RSRVD_DEVICE_TTL, &tmp))
    {
        return SQLITE_ERROR;
    }
    /* Add current time in front of the seconds received from the Publishing Device */
    sqlite3_int64 ttl = (tmp * US_PER_SEC) + OICGetCurrentTime(TIME_IN_US);

    /* links is a required property */
    OCRepPayloadValue *links = getLinks(payload);
    if (!links)
    {
        return SQLITE_ERROR;
    }

    int res;
    VERIFY_SQLITE(sqlite3_exec(gRDDB, "BEGIN TRANSACTION", NULL, NULL, NULL));

    /* INSERT OR IGNORE then UPDATE to update or insert the row without triggering the cascading deletes */
    static const char insertDeviceList[] = "INSERT OR IGNORE INTO RD_DEVICE_LIST (ID, di, ttl, external_host) "
        "VALUES ((SELECT ID FROM RD_DEVICE_LIST WHERE di=@deviceId), @deviceId, @ttl, @external_host)";
    int insertDeviceListSize = (int)sizeof(insertDeviceList);
    VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, insertDeviceList, insertDeviceListSize,
                    &stmt, NULL));
    if (deviceId)
    {
        VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@deviceId"),
                        deviceId, (int)strlen(deviceId), SQLITE_STATIC));
    }
    if (ttl)
    {
        VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@ttl"), ttl));
    }
    VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@external_host"),
            externalHost));
    res = sqlite3_step(stmt);
    if (SQLITE_DONE != res)
    {
        goto exit;
    }
    VERIFY_SQLITE(sqlite3_finalize(stmt));
    stmt = NULL;

    static const char updateDeviceList[] = "UPDATE RD_DEVICE_LIST SET ttl=@ttl WHERE di=@deviceId";
    int updateDeviceListSize = (int)sizeof(updateDeviceList);
    VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, updateDeviceList, updateDeviceListSize,
                    &stmt, NULL));
    if (deviceId)
    {
        VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@deviceId"),
                        deviceId, (int)strlen(deviceId), SQLITE_STATIC));
    }
    if (ttl)
    {
        VERIFY_SQLITE(sqlite3_bind_int64(stmt, sqlite3_bind_parameter_index(stmt, "@ttl"), ttl));
    }
    res = sqlite3_step(stmt);
    if (SQLITE_DONE != res)
    {
        goto exit;
    }
    VERIFY_SQLITE(sqlite3_finalize(stmt));
    stmt = NULL;

    /* Store the rest of the payload */
    static const char input[] = "SELECT ID FROM RD_DEVICE_LIST WHERE di=@deviceId";
    int inputSize = (int)sizeof(input);

    VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, input, inputSize, &stmt, NULL));
    if (deviceId)
    {
        VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@deviceId"),
                        deviceId, (int)strlen(deviceId), SQLITE_STATIC));
    }
    res = sqlite3_step(stmt);
    if (res == SQLITE_ROW || res == SQLITE_DONE)
    {
        sqlite3_int64 rowid = sqlite3_column_int64(stmt, 0);
        VERIFY_SQLITE(sqlite3_finalize(stmt));
        stmt = NULL;
        VERIFY_SQLITE(storeLinkPayload(links, rowid));
    }
    else
    {
        VERIFY_SQLITE(sqlite3_finalize(stmt));
        stmt = NULL;
    }

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "COMMIT", NULL, NULL, NULL));
    res = SQLITE_OK;

exit:
    sqlite3_finalize(stmt);
    OICFree(deviceId);
    if (SQLITE_OK != res)
    {
        sqlite3_exec(gRDDB, "ROLLBACK", NULL, NULL, NULL);
    }
    return res;
}

static int deleteResources(const char *deviceId, const int64_t *instanceIds, uint16_t nInstanceIds)
{
    char *delResource = NULL;
    sqlite3_stmt *stmt = NULL;
    if (!stringArgumentWithinBounds(deviceId))
    {
        OIC_LOG_V(ERROR, TAG, "Query longer than %d: \n%s", INT_MAX, deviceId);
        return SQLITE_ERROR;
    }

    int res;
    VERIFY_SQLITE(sqlite3_exec(gRDDB, "BEGIN TRANSACTION", NULL, NULL, NULL));

    if (!instanceIds || !nInstanceIds)
    {
        static const char delDevice[] = "DELETE FROM RD_DEVICE_LIST WHERE di=@deviceId";
        int delDeviceSize = (int)sizeof(delDevice);

        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, delDevice, delDeviceSize, &stmt, NULL));
        VERIFY_SQLITE(sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@deviceId"),
                                        deviceId, (int)strlen(deviceId), SQLITE_STATIC));
    }
    else
    {
        static const char pre[] = "DELETE FROM RD_DEVICE_LINK_LIST "
            "WHERE ins IN ("
            "SELECT RD_DEVICE_LINK_LIST.ins FROM RD_DEVICE_LINK_LIST "
            "INNER JOIN RD_DEVICE_LIST ON RD_DEVICE_LINK_LIST.DEVICE_ID=RD_DEVICE_LIST.ID "
            "WHERE RD_DEVICE_LINK_LIST.ins IN (";
        size_t inLen = nInstanceIds + (nInstanceIds - 1);
        static const char post[] = "))";
        size_t delResourceSize = sizeof(pre) + inLen + (sizeof(post) - 1);
        delResource = OICCalloc(delResourceSize, 1);
        if (!delResource)
        {
            res = SQLITE_NOMEM;
            goto exit;
        }
        OICStrcat(delResource, delResourceSize, pre);
        OICStrcat(delResource, delResourceSize, "?");
        for (uint16_t i = 1; i < nInstanceIds; ++i)
        {
            OICStrcat(delResource, delResourceSize, ",?");
        }
        OICStrcat(delResource, delResourceSize, post);
        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, delResource, (int)delResourceSize,
                        &stmt, NULL));
        for (uint16_t i = 0; i < nInstanceIds; ++i)
        {
            VERIFY_SQLITE(sqlite3_bind_int64(stmt, 1 + i, instanceIds[i]));
        }
    }

    res = sqlite3_step(stmt);
    if (SQLITE_DONE != res)
    {
        goto exit;
    }
    VERIFY_SQLITE(sqlite3_finalize(stmt));
    stmt = NULL;

    VERIFY_SQLITE(sqlite3_exec(gRDDB, "COMMIT", NULL, NULL, NULL));
    res = SQLITE_OK;

exit:
    OICFree(delResource);
    sqlite3_finalize(stmt);
    if (SQLITE_OK != res)
    {
        sqlite3_exec(gRDDB, "ROLLBACK", NULL, NULL, NULL);
    }
    return res;
}

OCStackResult OC_CALL OCRDDatabaseInit()
{
    if (SQLITE_OK == sqlite3_config(SQLITE_CONFIG_LOG, errorCallback))
    {
        OIC_LOG_V(INFO, TAG, "SQLite debugging log initialized.");
    }

    sqlite3_stmt *stmt = NULL;
    int res;
    res = sqlite3_open_v2(OCRDDatabaseGetStorageFilename(), &gRDDB, SQLITE_OPEN_READWRITE, NULL);
    if (SQLITE_OK != res)
    {
        OIC_LOG(DEBUG, TAG, "RD database file did not open, as no table exists.");
        OIC_LOG(DEBUG, TAG, "RD creating new table.");
        VERIFY_SQLITE(sqlite3_open_v2(OCRDDatabaseGetStorageFilename(), &gRDDB,
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL));

        VERIFY_SQLITE(sqlite3_exec(gRDDB, RD_TABLE, NULL, NULL, NULL));
        OIC_LOG(DEBUG, TAG, "RD created RD_DEVICE_LIST table.");

        VERIFY_SQLITE(sqlite3_exec(gRDDB, RD_LL_TABLE, NULL, NULL, NULL));
        OIC_LOG(DEBUG, TAG, "RD created RD_DEVICE_LINK_LIST table.");

        VERIFY_SQLITE(sqlite3_exec(gRDDB, RD_RT_TABLE, NULL, NULL, NULL));
        OIC_LOG(DEBUG, TAG, "RD created RD_LINK_RT table.");

        VERIFY_SQLITE(sqlite3_exec(gRDDB, RD_IF_TABLE, NULL, NULL, NULL));
        OIC_LOG(DEBUG, TAG, "RD created RD_LINK_IF table.");

        VERIFY_SQLITE(sqlite3_exec(gRDDB, RD_EP_TABLE, NULL, NULL, NULL));
        OIC_LOG(DEBUG, TAG, "RD created RD_LINK_EP table.");

        res = SQLITE_OK;
    }

    if (SQLITE_OK == res)
    {
        VERIFY_SQLITE(sqlite3_prepare_v2(gRDDB, "PRAGMA foreign_keys = ON;", -1, &stmt, NULL));
        res = sqlite3_step(stmt);
        if (SQLITE_DONE != res)
        {
            goto exit;
        }
        VERIFY_SQLITE(sqlite3_finalize(stmt));
        stmt = NULL;
    }

exit:
    sqlite3_finalize(stmt);
    if (SQLITE_OK == res)
    {
        return OC_STACK_OK;
    }
    else
    {
        sqlite3_close(gRDDB);
        gRDDB = NULL;
        return OC_STACK_ERROR;
    }
}

OCStackResult OC_CALL OCRDDatabaseClose()
{
    CHECK_DATABASE_INIT;
    int res;
    VERIFY_SQLITE(sqlite3_close(gRDDB));
    gRDDB = NULL;

exit:
    return (SQLITE_OK == res) ? OC_STACK_OK : OC_STACK_ERROR;
}

OCStackResult OC_CALL OCRDDatabaseStoreResources(const OCRepPayload *payload)
{
    CHECK_DATABASE_INIT;
    int res;
    VERIFY_SQLITE(storeResources(payload, true));
exit:
    return (SQLITE_OK == res) ? OC_STACK_OK : OC_STACK_ERROR;
}

OCStackResult OC_CALL OCRDDatabaseStoreResourcesFromThisHost(const OCRepPayload *payload)
{
    CHECK_DATABASE_INIT;
    int res;
    VERIFY_SQLITE(storeResources(payload, false));
exit:
    return (SQLITE_OK == res) ? OC_STACK_OK : OC_STACK_ERROR;
}

OCStackResult OC_CALL OCRDDatabaseDeleteResources(const char *deviceId, const int64_t *instanceIds,
        uint16_t nInstanceIds)
{
    CHECK_DATABASE_INIT;
    int res;
    VERIFY_SQLITE(deleteResources(deviceId, instanceIds, nInstanceIds));
exit:
    return (SQLITE_OK == res) ? OC_STACK_OK : OC_STACK_ERROR;
}

#endif
