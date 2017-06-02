/*
 * //******************************************************************
 * //
 * // Copyright 2016 Samsung Electronics All Rights Reserved.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * //
 * // Licensed under the Apache License, Version 2.0 (the "License");
 * // you may not use this file except in compliance with the License.
 * // You may obtain a copy of the License at
 * //
 * //      http://www.apache.org/licenses/LICENSE-2.0
 * //
 * // Unless required by applicable law or agreed to in writing, software
 * // distributed under the License is distributed on an "AS IS" BASIS,
 * // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * // See the License for the specific language governing permissions and
 * // limitations under the License.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

#include "logger.h"
#include "occloudprovisioning.h"
#include "OCCloudProvisioning.hpp"
#include "oic_malloc.h"
#include "oic_string.h"
#include "utils.h"

#define TAG "CLOUD-WRAPPER"

#define MAX_ID_LENGTH       (64)
#define MAX_STRING_LENGTH   (256)

#define UUID_EXAMPLE_1 "9cfbeb8e-5a1e-4d1c-9d01-2ae6fdb"
#define UUID_EXAMPLE_2 "123e4567-e89b-12d3-a456-4266554"
#define UUID_EXAMPLE_3 "987e6543-e21b-12d3-a456-4266554"
#define SUBJECT_ID_EXAMPLE "72616E64-5069-6E44-6576-557569643030"

#define ACL_ID_EXAMPLE "0f3d9f7fe5491d54077d"
#define ACE_ID_EXAMPLE "a0001"

#define ID_EXAMPLE_1   "78f98b4f25f21e2487e8"
#define ID_EXAMPLE_2   "6caa7009386290fd3681"

#define RESOURCE_URI_EXAMPLE "/a/light/0"
#define RESOURCE_TYPE_EXAMPLE "core.light"
#define INTERFACE_EXAMPLE "oic.if.baseline"

//in case of optional parameters absence should be sent NULL
#define OPTIONAL(str) (str[0] ? str : NULL)

using namespace OC;

static bool readOptional(const char* description)
{
    if (NULL == description)
    {
        return false;
    }

    printf("Do you want to Enter %s (y/n):\n", description);
    char choice = 0;

    while(1)
    {
        scanf("%c", &choice);
        getchar();

        switch (choice)
        {
            case 'y': return true;
            case 'n': return false;
            default: printf("Wrong value entered. Please press 'y' or 'n'\n");
        }
    }
    return false;
}

/**
 * Read user input (expect string value)
 *
 * @param[out] item           string item to fill
 * @param[in] length          max allowed string length
 * @param[in] description     item description
 * @param[in] example         item example
 */
void readString(char* item, int length, const char* description, const char* example)
{
    printf("Enter %s (f.e. %s):\n", description, example);
    char temp[8] = { 0 };
    snprintf(temp, sizeof(temp), "%%%ds", length - 1);
    scanf(temp, item);
    getchar();
}

/**
 * Read user input (expect string value, but it is optional and can be skipped by user)
 *
 * @param[out] item           string item to fill
 * @param[in] length          max allowed string length
 * @param[in] description     item description
 * @param[in] example         item example
 */
static void readOptionalString(char* item, int length, const char* description, const char* example)
{
    if (readOptional(description))
    {
        readString(item, length, description, example);
    }
}

/**
 * Read user input (expect integer value)
 *
 * @param[out] item           integer item to fill
 * @param[in] description     item description
 * @param[in] example         item example
 */
void readInteger(int* item, const char* description, const char* example)
{
    printf("Enter %s (f.e. %s):\n", description, example);
    scanf("%d", item);
    getchar();
}

/**
 * Read user input (expect array of strings)
 *
 * @param[out] list           array of strings structure
 * @param[in] length          max allowed array item length
 * @param[in] description     whole array description
 * @param[in] example         array item example
 */
static void readStringArray(stringArray_t *list, int length, const char* description, const char* example)
{
    int i = 0;
    int count = 0;
    char hint[MAX_STRING_LENGTH] = { 0 };

    snprintf(hint, sizeof(hint), "%s items count", description);
    readInteger(&count, hint, "2");

    char **item = NULL;

    if (0 >= count)
    {
        return;
    }

    item = (char**)OICCalloc(count, sizeof(char*));

    if (NULL == item)
    {
        goto no_memory;
    }

    for (i = 0; i < count; i++)
    {
        item[i] = (char*)OICCalloc(length, sizeof(char));

        if (NULL == item[i])
        {
            goto no_memory;
        }

        snprintf(hint, sizeof(hint), "%s %d item", description, i + 1);
        readString(item[i], length, hint, example);
    }
    list->array  = item;
    list->length = (size_t)count;
    return;

no_memory:
    //free already allocated memory here
    for (int k = 0; k < i; k++)
    {
        OICFree(item[k]);
    }
    OICFree(item);
}

/**
 * Read user input (expect array of strings)
 * It is optional and can be skipped by user.
 *
 * @param[out] list           array of strings structure
 * @param[in] length          max allowed array item length
 * @param[in] description     whole array description
 * @param[in] example         array item example
 */
static void readOptionalStringArray(stringArray_t *list, int length, const char* description, const char* example)
{
    if (readOptional(description))
    {
        readStringArray(list, length, description, example);
    }
}

/**
 * Copies whole binary file to crl variable
 *
 * @param[in] list           array of strings structure
 * @param[out] crl           byte array to fill
 * @return                   negative error code
 * */
static int ReadFile(const char *name, OCByteString *crl)
{
    FILE *file = NULL;
    int length = 0;
    uint8_t *buffer = NULL;
    int result = 1;
    size_t realLen = 0;

    //Open file
    file = fopen(name, "rb");
    if (!file)
    {
        OIC_LOG_V(ERROR, TAG, "Unable to open file %s", name);
        return result;
    }

    //Get file length
    result = fseek(file, 0, SEEK_END);
    if (result)
    {
        OIC_LOG(ERROR, TAG, "Failed to SEEK_END");
        goto exit;
    }

    length = ftell(file);
    if (length < 0)
    {
        OIC_LOG(ERROR, TAG, "Failed to ftell");
        goto exit;
    }

    result = fseek(file, 0, SEEK_SET);
    if (result)
    {
        OIC_LOG(ERROR, TAG, "Failed to SEEK_SET");
        goto exit;
    }

    //Allocate memory
    buffer = (uint8_t *)malloc(length);
    if (!buffer)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate buffer");
        goto exit;
    }

    //Read file contents into buffer
    realLen = fread(buffer, length, 1, file);
    if (realLen != (size_t)length)
    {
        OIC_LOG_V(ERROR, TAG, "Length mismatch: read %zu instead of %d bytes", realLen, length);
        goto exit;
    }

    crl->bytes = buffer;
    crl->len   = (size_t)length;

    result = 0;
exit:
    fclose(file);
    return 0;
}

OCStackResult OCWrapperCertificateIssueRequest(OCCloudProvisioning& ptr, ResponseCallBack callback)
{
    return ptr.requestCertificate(callback);
}

OCStackResult OCWrapperGetCRL(OCCloudProvisioning& ptr, ResponseCallBack callback)
{
    return ptr.getCRL(callback);
}

OCStackResult OCWrapperPostCRL(OCCloudProvisioning& ptr, ResponseCallBack callback)
{
    OCStackResult result = OC_STACK_ERROR;
    OCByteString crlData = {0, 0};
    char filename[64] = {0};
    char thisUpdate[16] = { 0 };
    char nextUpdate[16] = { 0 };
    stringArray_t serialNumbers = {0, 0};
    stringArray_t *rcsn = 0;
    OCByteString *crl = 0;

    readString(thisUpdate, sizeof(thisUpdate), "Crl's thisUpdate value", "20160727000000");
    readString(nextUpdate, sizeof(nextUpdate), "Crl's nextUpdate value", "20161027000000");
    readOptionalStringArray(&serialNumbers, 16, "Revoked serial numbers", "1234");

    if (NULL == serialNumbers.array)
    {
        readString(filename, sizeof(filename),
                   "filename from which binary Crl in DER format will be read", "crl");

        if (ReadFile(filename, &crlData))
        {
            printf("Can't read crl from file %s\n", filename);
            goto exit;
        }
    }
    rcsn = serialNumbers.array? &serialNumbers : NULL;
    crl = crlData.bytes? &crlData : NULL;

    result = ptr.postCRL(thisUpdate, nextUpdate, crl, rcsn,
                             callback);
exit:
    clearStringArray(&serialNumbers);
    OICFree(crlData.bytes);

    return result;
}

OCStackResult OCWrapperAclIdGetByDevice(OCCloudProvisioning& ptr, AclIdResponseCallBack callback)
{
    char di[MAX_ID_LENGTH] = { 0 };

    readString(di, sizeof(di), "device id", UUID_EXAMPLE_1);

    return ptr.getAclIdByDevice(di, callback);
}

OCStackResult OCWrapperAclIdCreate(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char oid[MAX_ID_LENGTH]  = { 0 };
    char di[MAX_ID_LENGTH]   = { 0 };

    readString(oid, sizeof(oid), "owner id", UUID_EXAMPLE_2);
    readString(di, sizeof(di), "device id", UUID_EXAMPLE_1);

    return OCCloudAclIdCreate(NULL, oid, di, endPoint, callback);
}

OCStackResult OCWrapperAclIdDelete(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char aclid[MAX_ID_LENGTH] = { 0 };

    readString(aclid, sizeof(aclid), "acl id", ACL_ID_EXAMPLE);

    return OCCloudAclIdDelete(NULL, aclid, endPoint, callback);
}

OCStackResult OCWrapperAclIndividualGetInfo(OCCloudProvisioning& ptr, ResponseCallBack callback)
{
    char aclid[MAX_ID_LENGTH] = { 0 };

    readString(aclid, sizeof(aclid), "acl id", ACL_ID_EXAMPLE);

    return ptr.getIndividualAclInfo(aclid, callback);
}

OCStackResult OCWrapperAclIndividualUpdateAce(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    OCStackResult result = OC_STACK_NO_MEMORY;
    int i = 0, j = 0;
    cloudAce_t *aces = NULL;

    char aclid[MAX_ID_LENGTH] = { 0 };
    readString(aclid, sizeof(aclid), "ace id", ACL_ID_EXAMPLE);

    int acllist_count = 0;
    readInteger(&acllist_count, "acl list count", "1");
    if (0 >= acllist_count)
    {
        OIC_LOG(ERROR, TAG, "Wrong number of aclList");
        goto exit;
    }

    aces = (cloudAce_t*)OICCalloc(acllist_count, sizeof(cloudAce_t));
    if (!aces)
    {
        OIC_LOG(ERROR, TAG, "Can't allocate memory for aces");
        goto exit;
    }

    for (i = 0; i < acllist_count; i++)
    {
        cloudAce_t *ace = &aces[i];
        if (i != acllist_count - 1) ace->next = &aces[i + 1];

        char aceid[MAX_ID_LENGTH] = { 0 };
        char subjectuuid[MAX_ID_LENGTH] = { 0 };
        int stype = 0;
        int permission = 0;

        readString(aceid, sizeof(aceid), "ace id", ACE_ID_EXAMPLE);
        readString(subjectuuid, sizeof(subjectuuid), "subjectuuid", SUBJECT_ID_EXAMPLE);
        readInteger(&stype, "subject type", "0 – Device, 1 – User, 2 - Group");
        readInteger(&permission, "permission", "6");

        ace->aceId = OICStrdup(aceid);
        ace->stype = stype;
        ace->permission = permission;
        memcpy(&ace->subjectuuid, subjectuuid, sizeof(OicUuid_t));

        int reslist_count = 0;
        readInteger(&reslist_count, "resources list count", "1");

        ace->resources = (OicSecRsrc_t*)OICCalloc(reslist_count, sizeof(OicSecRsrc_t));
        if (!ace->resources)
        {
            OIC_LOG(ERROR, TAG, "Can't allocate memory for resources");
            goto exit;
        }

        for (j = 0; j < reslist_count; j++)
        {
            OicSecRsrc_t *res = &ace->resources[j];
            if (j != reslist_count - 1) res->next = &ace->resources[j + 1];

            char href[32] = { 0 };
            readString(href, sizeof(href), "href", RESOURCE_URI_EXAMPLE);

            stringArray_t rt = {0, 0};
            readStringArray(&rt, MAX_ID_LENGTH, "resource type", RESOURCE_TYPE_EXAMPLE);

            stringArray_t _if = {0, 0};
            readStringArray(&_if, MAX_ID_LENGTH, "interface", INTERFACE_EXAMPLE);

            res->href = OICStrdup(href);
            res->types = rt.array;
            res->typeLen = rt.length;
            res->interfaces = _if.array;
            res->interfaceLen = _if.length;
        }
    }

    result = OCCloudAclIndividualUpdateAce(NULL, aclid, aces, endPoint, callback);
exit:
    if (aces)
    {
        for (int k = 0; k < i; k++)
        {
            cloudAce_t *ace = &aces[k];
            OICFree(ace->aceId);

            if (ace->resources)
            {
                for (int l = 0; l < j; l++)
                {
                    OicSecRsrc_t *res = &ace->resources[l];
                    OICFree(res->href);

                    stringArray_t rt { res->types, res->typeLen};
                    clearStringArray(&rt);

                    stringArray_t _if {res->interfaces, res->interfaceLen};
                    clearStringArray(&_if);
                }
            }

        }
    }
    return result;
}

OCStackResult OCWrapperAclIndividualDelete(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char aclid[MAX_ID_LENGTH] = { 0 };

    readString(aclid, sizeof(aclid), "acl id", ACL_ID_EXAMPLE);

    return OCCloudAclIndividualDelete(NULL, aclid, endPoint, callback);
}

OCStackResult OCWrapperAclCreateGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char gtype[16] = { 0 };
    char gmid[MAX_ID_LENGTH] = { 0 };

    readString(gtype, sizeof(gtype), "Group type value", "Public");
    readOptionalString(gmid, sizeof(gmid), "group member id value", UUID_EXAMPLE_2);

    return OCCloudAclCreateGroup(NULL, gtype, OPTIONAL(gmid), endPoint, callback);
}

OCStackResult OCWrapperAclFindMyGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char mid[MAX_ID_LENGTH]  = { 0 };

    readOptionalString(mid, sizeof(mid), "member id value", UUID_EXAMPLE_2);

    return OCCloudAclFindMyGroup(NULL, OPTIONAL(mid), endPoint, callback);
}

OCStackResult OCWrapperAclDeleteGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char gid[MAX_ID_LENGTH]  = { 0 };
    char gmid[MAX_ID_LENGTH] = { 0 };

    readString(gid, sizeof(gid), "Group id value", ID_EXAMPLE_1);

    readOptionalString(gmid, sizeof(gmid), "group member id value", UUID_EXAMPLE_2);

    return OCCloudAclDeleteGroup(NULL, gid, OPTIONAL(gmid), endPoint, callback);
}

OCStackResult OCWrapperAclJoinToInvitedGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char gid[MAX_ID_LENGTH]  = { 0 };

    readString(gid, sizeof(gid), "Group id value", ID_EXAMPLE_1);

    return OCCloudAclJoinToInvitedGroup(NULL, gid, endPoint, callback);
}

OCStackResult OCWrapperAclObserveGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char gid[MAX_ID_LENGTH]  = { 0 };

    readString(gid, sizeof(gid), "Group id value", ID_EXAMPLE_1);

    return OCCloudAclObserveGroup(NULL, gid, endPoint, callback);
}

OCStackResult OCWrapperAclShareDeviceIntoGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    OCStackResult result = OC_STACK_NO_MEMORY;
    char gid[MAX_ID_LENGTH]  = { 0 };
    stringArray_t midlist = {0,0};
    stringArray_t dilist = {0,0};

    readString(gid, sizeof(gid), "Group id value", ID_EXAMPLE_1);

    readStringArray(&midlist, MAX_ID_LENGTH, "member id list", UUID_EXAMPLE_2);

    readStringArray(&dilist, MAX_ID_LENGTH, "device list", UUID_EXAMPLE_1);

    result = OCCloudAclShareDeviceIntoGroup(NULL, gid, &midlist, &dilist, endPoint, callback);

    clearStringArray(&midlist);
    clearStringArray(&dilist);

    return result;
}

OCStackResult OCWrapperAclDeleteDeviceFromGroup(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    OCStackResult result = OC_STACK_NO_MEMORY;
    char gid[MAX_ID_LENGTH]  = { 0 };
    stringArray_t midlist = {0,0};
    stringArray_t dilist = {0,0};

    readString(gid, sizeof(gid), "Group id value", ID_EXAMPLE_1);

    readStringArray(&midlist, MAX_ID_LENGTH, "member id list", UUID_EXAMPLE_2);

    readStringArray(&dilist, MAX_ID_LENGTH, "device list", UUID_EXAMPLE_1);

    result = OCCloudAclDeleteDeviceFromGroup(NULL, gid, &midlist, &dilist, endPoint, callback);

    clearStringArray(&midlist);
    clearStringArray(&dilist);

    return result;
}

OCStackResult OCWrapperAclGroupGetInfo(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char gid[MAX_ID_LENGTH]  = { 0 };
    char mid[MAX_ID_LENGTH]  = { 0 };

    readString(gid, sizeof(gid), "Group id value", ID_EXAMPLE_1);

    readOptionalString(mid, sizeof(mid), "member id value", UUID_EXAMPLE_2);

    return OCCloudAclGroupGetInfo(NULL, gid, OPTIONAL(mid), endPoint, callback);
}

OCStackResult OCWrapperAclInviteUser(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    OCStackResult result = OC_STACK_NO_MEMORY;
    char uid[MAX_ID_LENGTH]  = { 0 };
    stringArray_t midlist = {0,0};
    stringArray_t gidlist = {0,0};

    readOptionalString(uid, sizeof(uid), "user id value", UUID_EXAMPLE_2);

    readStringArray(&gidlist, MAX_ID_LENGTH, "group id list", UUID_EXAMPLE_1);

    readStringArray(&midlist, MAX_ID_LENGTH, "member id list", UUID_EXAMPLE_2);

    result = OCCloudAclInviteUser(NULL, OPTIONAL(uid), &gidlist, &midlist, endPoint, callback);

    clearStringArray(&midlist);
    clearStringArray(&gidlist);

    return result;
}

OCStackResult OCWrapperAclGetInvitation(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char uid[MAX_ID_LENGTH]  = { 0 };

    readOptionalString(uid, sizeof(uid), "user uuid value", UUID_EXAMPLE_2);

    return OCCloudAclGetInvitation(NULL, OPTIONAL(uid), endPoint, callback);
}

OCStackResult OCWrapperAclDeleteInvitation(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char uid[MAX_ID_LENGTH]  = { 0 };
    char gid[MAX_ID_LENGTH]  = { 0 };

    readOptionalString(uid, sizeof(uid), "user uuid value", UUID_EXAMPLE_2);
    readString(gid, sizeof(gid), "Group id value", ID_EXAMPLE_1);

    return OCCloudAclDeleteInvitation(NULL, OPTIONAL(uid), gid, endPoint, callback);
}

OCStackResult OCWrapperAclCancelInvitation(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char uid[MAX_ID_LENGTH]  = { 0 };
    char gid[MAX_ID_LENGTH]  = { 0 };
    char mid[MAX_ID_LENGTH]  = { 0 };

    readOptionalString(uid, sizeof(uid), "user uuid value", UUID_EXAMPLE_2);

    readString(gid, sizeof(gid), "Group id value", ID_EXAMPLE_1);
    readString(mid, sizeof(mid), "member id value", ID_EXAMPLE_1);

    return OCCloudAclCancelInvitation(NULL, OPTIONAL(uid), gid, mid, endPoint, callback);
}

OCStackResult OCWrapperAclPolicyCheck(const OCDevAddr *endPoint, OCCloudResponseCB callback)
{
    char sid[MAX_ID_LENGTH] = { 0 };
    char di[MAX_ID_LENGTH]  = { 0 };
    char rm[16]  = { 0 };
    char user_uri[32] = { 0 };

    readString(sid, sizeof(sid), "subject id", UUID_EXAMPLE_1);
    readString(di, sizeof(di), "device id", UUID_EXAMPLE_2);
    readString(rm, sizeof(rm), "request method", "GET or POST or DELETE");
    readString(user_uri, sizeof(user_uri), "request uri", RESOURCE_URI_EXAMPLE);

    return OCCloudAclPolicyCheck(NULL, sid, di, rm, user_uri, endPoint, callback);
}
