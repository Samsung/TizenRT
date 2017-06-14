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

#include "gtest/gtest.h"
#include <coap/utlist.h>
#include <sys/stat.h>
#include "ocstack.h"
#include "psinterface.h"
#include "ocpayload.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "cainterface.h"
#include "secureresourcemanager.h"
#include "securevirtualresourcetypes.h"
#include "srmresourcestrings.h"
#include "aclresource.h"
#include "srmtestcommon.h"
#include "srmutility.h"
#include "logger.h"
#include "doxmresource.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "payload_logging.h"
#include "security_internals.h"

using namespace std;

#define TAG  "SRM-ACL-UT"

// These paths match jenkins build configuration.
const char* DEFAULT_ACL_FILE_NAME = "oic_unittest_default_acl.dat";
const char* ACL1_FILE_NAME = "oic_unittest_acl1.dat";

#define NUM_ACE_FOR_WILDCARD_IN_ACL1_DAT (1)

static bool AddResourceToACE(OicSecAce_t* ace, const char* rsrcName,
                             const char* typeName, const char* interfaceName)
{
    OicSecRsrc_t* rsrc = NULL;

    VERIFY_NON_NULL(TAG, ace, ERROR);
    VERIFY_NON_NULL(TAG, rsrcName, ERROR);
    VERIFY_NON_NULL(TAG, interfaceName, ERROR);
    VERIFY_NON_NULL(TAG, typeName, ERROR);

    rsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NON_NULL(TAG, rsrc, ERROR);
    rsrc->href = OICStrdup(rsrcName);
    VERIFY_NON_NULL(TAG, rsrc->href, ERROR);

    rsrc->typeLen = 1;
    rsrc->types = (char**)OICCalloc(1, sizeof(char*));
    VERIFY_NON_NULL(TAG, rsrc->types, ERROR);
    rsrc->types[0] = OICStrdup(typeName);
    VERIFY_NON_NULL(TAG, rsrc->types[0], ERROR);

    rsrc->interfaceLen = 1;
    rsrc->interfaces = (char**)OICCalloc(1, sizeof(char*));
    VERIFY_NON_NULL(TAG, rsrc->interfaces, ERROR);
    rsrc->interfaces[0] = OICStrdup(interfaceName);
    VERIFY_NON_NULL(TAG, rsrc->interfaces[0], ERROR);

    LL_APPEND(ace->resources, rsrc);
    return true;
exit:
    if(rsrc)
    {
        if(rsrc->href)
        {
            OICFree(rsrc->href);
            OICFree(rsrc->types[0]);
            OICFree(rsrc->types);
            OICFree(rsrc->interfaces[0]);
            OICFree(rsrc->interfaces);
        }
        OICFree(rsrc);
    }
    return false;
}

static int GetNumberOfResource(const OicSecAce_t* ace)
{
    int ret = 0;
    OicSecRsrc_t* rsrc = NULL;
    LL_FOREACH(ace->resources, rsrc)
    {
        ret++;
    }

    return ret;
}

TEST(ACLResourceTest, CBORDefaultACLConversion)
{
    uint8_t defaultAclSub[] = { 0x2a };
    uint8_t defaultAclOwnrs[] = {0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32};

    OicSecAcl_t *defaultAcl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    ASSERT_TRUE(NULL != defaultAcl);
    OicSecAce_t *ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    ASSERT_TRUE(NULL != ace);
    ace->permission = 2;
    memcpy(ace->subjectuuid.id, defaultAclSub, sizeof(defaultAclSub));
    LL_APPEND(defaultAcl->aces, ace);

    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/res", "oic.wk.res", "oic.if.ll"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/d", "oic.wk.d", "oic.if.r"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/p", "oic.wk.p", "oic.if.r"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/res/types/d", "oic.wk.unknow", "oic.if.r"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/ad", "oic.wk.ad", "oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/acl", "oic.r.acl", "oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/doxm", "oic.r.doxm" ,"oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/pstat", "oic.r.pstat" ,"oic.if.baseline"));

    memcpy(defaultAcl->rownerID.id, defaultAclOwnrs, sizeof(defaultAclOwnrs));

    size_t defaultAclSize = 0;
    uint8_t *defaultPsStorage = NULL;
    OCStackResult convRet = AclToCBORPayload(defaultAcl, &defaultPsStorage, &defaultAclSize);
    EXPECT_EQ(OC_STACK_OK, convRet);
    ASSERT_TRUE(NULL != defaultPsStorage);
    EXPECT_NE(0, defaultAclSize);

    OicSecAcl_t* convertedAcl = CBORPayloadToAcl(defaultPsStorage, defaultAclSize);
    ASSERT_TRUE(NULL != convertedAcl);

    size_t rsrcCnt1 = 0;
    size_t rsrcCnt2 = 0;
    OicSecAce_t* tempAce = NULL;
    LL_FOREACH(defaultAcl->aces, tempAce)
    {
        rsrcCnt1 += GetNumberOfResource(tempAce);
    }
    tempAce = NULL;
    LL_FOREACH(convertedAcl->aces, tempAce)
    {
        rsrcCnt2 += GetNumberOfResource(tempAce);
    }
    EXPECT_EQ(rsrcCnt1, rsrcCnt2);

    DeleteACLList(convertedAcl);
    DeleteACLList(defaultAcl);
    OICFree(defaultPsStorage);
}

TEST(ACLResourceTest, CBORACLConversion)
{
    uint8_t ownrs[] = {0x32, 0x32, 0x32, 0x32,
                       0x32, 0x32, 0x32, 0x32,
                       0x32, 0x32, 0x32, 0x32,
                       0x32, 0x32, 0x32, 0x32};
    const char* subjectUuid[3] = {"0000000000000000",
                                  "1111111111111111",
                                  "2222222222222222"};
    const uint16_t permission[3] = {2, 6, 31};
    const size_t numOfRsrc[3] = {6, 2, 2};

    OicSecAcl_t *secAcl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    ASSERT_TRUE(secAcl != NULL);
    memcpy(secAcl->rownerID.id, ownrs, sizeof(ownrs));

    OicSecAce_t *ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    ASSERT_TRUE(NULL != ace);
    ace->permission = permission[0];
    memcpy(ace->subjectuuid.id, subjectUuid[0], strlen(subjectUuid[0]));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/res", "oic.wk.res", "oic.if.ll"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/d", "oic.wk.d", "oic.if.r"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/p", "oic.wk.p", "oic.if.r"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/res/types/d", "oic.wk.unknow", "oic.if.r"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/ad", "oic.wk.ad", "oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace, "/oic/sec/acl", "oic.r.acl", "oic.if.baseline"));
    LL_APPEND(secAcl->aces, ace);

    OicSecAce_t *ace1 = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    ASSERT_TRUE(NULL != ace1);
    ace1->permission = permission[1];
    memcpy(ace1->subjectuuid.id, subjectUuid[1], strlen(subjectUuid[1]));
    EXPECT_EQ(true, AddResourceToACE(ace1, "/oic/sec/doxm", "oic.r.doxm" ,"oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace1, "/oic/sec/pstat", "oic.r.pstat" ,"oic.if.baseline"));
    LL_APPEND(secAcl->aces, ace1);

    OicSecAce_t *ace2 = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    ASSERT_TRUE(NULL != ace2);
    ace2->permission = permission[2];
    memcpy(ace2->subjectuuid.id, subjectUuid[2], strlen(subjectUuid[2]));
    EXPECT_EQ(true, AddResourceToACE(ace2, "/oic/light", "oic.core", "oic.if.baseline"));
    EXPECT_EQ(true, AddResourceToACE(ace2, "/oic/garage", "oic.core", "oic.if.baseline"));
    LL_APPEND(secAcl->aces, ace2);

    size_t size = 0;
    uint8_t *psStorage = NULL;
    EXPECT_EQ(OC_STACK_OK, AclToCBORPayload(secAcl, &psStorage, &size));
    ASSERT_TRUE(NULL != psStorage);
    OicSecAcl_t *acl = CBORPayloadToAcl(psStorage, size);
    ASSERT_TRUE(NULL != acl);

    size_t numberOfCheckedAce = 0;
    OicSecAce_t* tempAce = NULL;
    LL_FOREACH(acl->aces, tempAce)
    {
        if(memcmp(tempAce->subjectuuid.id, subjectUuid[0], strlen(subjectUuid[0])) == 0)
        {
            EXPECT_EQ(numOfRsrc[0], GetNumberOfResource(tempAce));
            EXPECT_EQ(permission[0], tempAce->permission);
            numberOfCheckedAce++;
        }
        if(memcmp(tempAce->subjectuuid.id, subjectUuid[1], strlen(subjectUuid[1])) == 0)
        {
            EXPECT_EQ(numOfRsrc[1], GetNumberOfResource(tempAce));
            EXPECT_EQ(permission[1], tempAce->permission);
            numberOfCheckedAce++;
        }
        if(memcmp(tempAce->subjectuuid.id, subjectUuid[2], strlen(subjectUuid[2])) == 0)
        {
            EXPECT_EQ(numOfRsrc[2], GetNumberOfResource(tempAce));
            EXPECT_EQ(permission[2], tempAce->permission);
            numberOfCheckedAce++;
        }
    }
    EXPECT_EQ(3, numberOfCheckedAce);

    DeleteACLList(acl);
    DeleteACLList(secAcl);
    OICFree(psStorage);
}

//InitResource Tests
TEST(ACLResourceTest, InitAclResource)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, InitACLResource());
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DeInitACLResource());
}

// Default ACL tests
TEST(ACLResourceTest, GetDefaultACLTests)
{
    uint8_t *payload = NULL;
    size_t size = 0;

    ASSERT_TRUE(ReadCBORFile(DEFAULT_ACL_FILE_NAME, OIC_JSON_ACL_NAME, &payload, &size));
    ASSERT_TRUE(NULL != payload);

    OicSecAcl_t *psAcl = CBORPayloadToAcl(payload, size);
    ASSERT_TRUE(NULL != psAcl);

    OicSecAcl_t *acl = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&acl));
    ASSERT_TRUE(NULL != acl);

    // Verify if the SRM generated default ACL matches with unit test default
    if (acl && psAcl)
    {
        OicSecAce_t* tempAce1 = NULL;
        OicSecAce_t* tempAce2 = NULL;

        for(tempAce1 = acl->aces, tempAce2 = psAcl->aces;
            tempAce1 && tempAce2; tempAce1 = tempAce1->next,
            tempAce2 = tempAce2->next)
        {
            EXPECT_TRUE(memcmp(tempAce1->subjectuuid.id, tempAce2->subjectuuid.id, sizeof(tempAce1->subjectuuid.id)) == 0);
            EXPECT_EQ(tempAce1->permission, tempAce2->permission);
            EXPECT_EQ(GetNumberOfResource(tempAce1), GetNumberOfResource(tempAce2));
        }
    }

    DeleteACLList(psAcl);
    DeleteACLList(acl);
    DeInitACLResource();
    OICFree(payload);
}

// 'POST' ACL tests
TEST(ACLResourceTest, ACLPostTest)
{
    // Read an ACL from the file
    uint8_t *payload = NULL;
    size_t size = 0;

    static OCPersistentStorage ps = OCPersistentStorage();
    SetPersistentHandler(&ps, true);

    OicSecAcl_t *defaultAcl = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&defaultAcl));
    ASSERT_TRUE(defaultAcl != NULL);
    EXPECT_EQ(OC_STACK_OK, SetDefaultACL(defaultAcl));

    ASSERT_TRUE(ReadCBORFile(ACL1_FILE_NAME, OIC_JSON_ACL_NAME, &payload, &size));
    ASSERT_TRUE(NULL != payload);

    OCSecurityPayload *securityPayload = OCSecurityPayloadCreate(payload, size);
    ASSERT_TRUE(NULL != securityPayload);

    // Create Entity Handler POST request payload
    OCEntityHandlerRequest ehReq = OCEntityHandlerRequest();
    ehReq.method = OC_REST_POST;
    ehReq.payload = (OCPayload *) securityPayload;

    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    OicSecAcl_t *acl = CBORPayloadToAcl(payload, size);
    ASSERT_TRUE(NULL != acl);

    // Verify if SRM contains ACL for the subject
    OicSecAce_t *savePtr = NULL;
    const OicSecAce_t* subjectAcl = GetACLResourceData(&(acl->aces->subjectuuid), &savePtr);
    ASSERT_TRUE(NULL != subjectAcl);

    // Perform cleanup
    OICFree(payload);
    OCPayloadDestroy((OCPayload *) securityPayload);
    DeInitACLResource();
    DeleteACLList(acl);
}

extern "C" {
    // gAcl is a pointer to the the global ACL used by SRM
    extern OicSecAcl_t  *gAcl;
}

// GetACLResource tests
TEST(ACLResourceTest, GetACLResourceTests)
{
    // Read an ACL from the file
    static OCPersistentStorage ps = OCPersistentStorage();
    SetPersistentHandler(&ps, true);

    uint8_t *payload = NULL;
    size_t size = 0;

    ASSERT_TRUE(ReadCBORFile(ACL1_FILE_NAME, OIC_JSON_ACL_NAME, &payload, &size));
    ASSERT_TRUE(payload != NULL);

    OicSecAcl_t *defaultPsAcl = CBORPayloadToAcl(payload, size);
    ASSERT_TRUE(defaultPsAcl != NULL);

    OicSecAcl_t *acl1 = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&acl1));
    ASSERT_TRUE(acl1 != NULL);
    EXPECT_EQ(OC_STACK_OK, SetDefaultACL(acl1));

    // Verify that ACL file contains 2 ACE entries for 'WILDCARD' subject
    const OicSecAce_t *ace = NULL;
    OicSecAce_t *savePtr = NULL;
    OicUuid_t subject = WILDCARD_SUBJECT_ID;
    int count = 0;

    do
    {
        ace = GetACLResourceData(&subject, &savePtr);
        count = (NULL != ace) ? count + 1 : count;
    } while (ace != NULL);

    EXPECT_EQ(count, NUM_ACE_FOR_WILDCARD_IN_ACL1_DAT);

    /* Perform cleanup */
    OICFree(payload);
    DeleteACLList(defaultPsAcl);
    DeInitACLResource();
}

static OCStackResult  populateAcl(OicSecAcl_t *acl,  int numRsrc)
{
    OCStackResult ret = OC_STACK_ERROR;
    OicSecAce_t* ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    VERIFY_NON_NULL(TAG, ace, ERROR);

    memcpy(ace->subjectuuid.id, "2222222222222222", sizeof(ace->subjectuuid.id));
    EXPECT_EQ(true, AddResourceToACE(ace, "/a/led", "oic.core", "oic.if.r"));
    if(2 == numRsrc)
    {
        EXPECT_EQ(true, AddResourceToACE(ace, "/a/fan", "oic.core", "oic.if.r"));
    }
    ace->permission = 6;
    LL_APPEND(acl->aces, ace);

    memcpy(acl->rownerID.id, "1111111111111111", sizeof(acl->rownerID.id));

    ret = OC_STACK_OK;

    return ret;
exit:
    DeleteACLList(acl);
    return ret;
}

//'DELETE' ACL test
TEST(ACLResourceTest, ACLDeleteWithSingleResourceTest)
{
    static OCPersistentStorage ps = OCPersistentStorage();
    SetPersistentHandler(&ps, true);

    OicSecAcl_t *defaultAcl = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&defaultAcl));
    ASSERT_TRUE(defaultAcl != NULL);
    EXPECT_EQ(OC_STACK_OK, SetDefaultACL(defaultAcl));

    //Populate ACL
    OicSecAcl_t acl = OicSecAcl_t();
    EXPECT_EQ(OC_STACK_OK, populateAcl(&acl, 1));

    //GET CBOR POST payload
    size_t size = 0;
    uint8_t  *payload = NULL;
    EXPECT_EQ(OC_STACK_OK, AclToCBORPayload(&acl, &payload, &size));
    ASSERT_TRUE(NULL != payload);

    // Security Payload
    OCSecurityPayload *securityPayload = OCSecurityPayloadCreate(payload, size);
    ASSERT_TRUE(NULL != securityPayload);

    // Create Entity Handler POST request payload
    OCEntityHandlerRequest ehReq = OCEntityHandlerRequest();
    ehReq.payload = (OCPayload *) securityPayload;
    ehReq.method = OC_REST_POST;
    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    // Verify if SRM contains ACE for the subject
    OicSecAce_t* savePtr = NULL;
    const OicSecAce_t* subjectAce1 = GetACLResourceData(&acl.aces->subjectuuid, &savePtr);
    ASSERT_TRUE(NULL != subjectAce1);

    // Create Entity Handler DELETE request
    ehReq.method = OC_REST_DELETE;
    char query[] = "subjectuuid=32323232-3232-3232-3232-323232323232;resources=/a/led";
    ehReq.query = (char *)OICMalloc(strlen(query)+1);
    ASSERT_TRUE(NULL !=  ehReq.query);
    OICStrcpy(ehReq.query, strlen(query)+1, query);
    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    // Verify if SRM has deleted ACE for the subject
    savePtr = NULL;
    const OicSecAce_t* subjectAce2 = GetACLResourceData(&acl.aces->subjectuuid, &savePtr);
    ASSERT_TRUE(NULL == subjectAce2);

    // Perform cleanup
    DeInitACLResource();
    OICFree(ehReq.query);
    OCPayloadDestroy((OCPayload *)securityPayload);
    OICFree(payload);
}

TEST(ACLResourceTest, ACLDeleteWithMultiResourceTest)
{
    static OCPersistentStorage ps = OCPersistentStorage();
    SetPersistentHandler(&ps, true);

    OicSecAcl_t *defaultAcl = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&defaultAcl));
    ASSERT_TRUE(defaultAcl != NULL);
    EXPECT_EQ(OC_STACK_OK, SetDefaultACL(defaultAcl));

    //Populate ACL
    OicSecAcl_t acl = OicSecAcl_t();
    EXPECT_EQ(OC_STACK_OK, populateAcl(&acl, 2));

    //GET CBOR POST payload
    size_t size = 0;
    uint8_t *payload = NULL;
    EXPECT_EQ(OC_STACK_OK, AclToCBORPayload(&acl, &payload, &size));
    ASSERT_TRUE(NULL != payload);

    // Security Payload
    OCSecurityPayload *securityPayload = OCSecurityPayloadCreate(payload, size);
    ASSERT_TRUE(NULL!= securityPayload);

    // Create Entity Handler POST request payload
    OCEntityHandlerRequest ehReq = OCEntityHandlerRequest();
    ehReq.method = OC_REST_POST;
    ehReq.payload = (OCPayload *)securityPayload;
    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    // Verify if SRM contains ACE for the subject with two resources
    OicSecAce_t* savePtr = NULL;
    const OicSecAce_t* subjectAce1 = GetACLResourceData(&acl.aces->subjectuuid, &savePtr);
    ASSERT_TRUE(NULL != subjectAce1);
    EXPECT_EQ(2, GetNumberOfResource(subjectAce1));

    printf("\n\n");
    OicSecRsrc_t* rsrc = NULL;
    LL_FOREACH(subjectAce1->resources, rsrc)
    {
        printf("%s\n", rsrc->href);
    }
    printf("\n\n");

    // Create Entity Handler DELETE request
    ehReq.method = OC_REST_DELETE;
    char query[] = "subjectuuid=32323232-3232-3232-3232-323232323232;resources=/a/led";
    ehReq.query = (char *)OICMalloc(strlen(query)+1);
    ASSERT_TRUE(NULL != ehReq.query);
    OICStrcpy(ehReq.query, strlen(query)+1, query);

    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    // Verify if SRM contains ACL for the subject but only with one resource
    savePtr = NULL;
    const OicSecAce_t* subjectAce2 = GetACLResourceData(&acl.aces->subjectuuid, &savePtr);
    ASSERT_TRUE(NULL != subjectAce2);
    EXPECT_EQ(1, GetNumberOfResource(subjectAce2));

    // Perform cleanup
    OCPayloadDestroy((OCPayload *)securityPayload);
    DeInitACLResource();
    OICFree(ehReq.query);
    OICFree(payload);
}

//'GET' with query ACL test
TEST(ACLResourceTest, ACLGetWithQueryTest)
{
    static OCPersistentStorage ps = OCPersistentStorage();
    SetPersistentHandler(&ps, true);

    OicSecAcl_t *defaultAcl = NULL;
    EXPECT_EQ(OC_STACK_OK, GetDefaultACL(&defaultAcl));
    ASSERT_TRUE(defaultAcl != NULL);
    EXPECT_EQ(OC_STACK_OK, SetDefaultACL(defaultAcl));

    //Populate ACL
    OicSecAcl_t acl = OicSecAcl_t();
    EXPECT_EQ(OC_STACK_OK, populateAcl(&acl, 1));

    //GET CBOR POST payload
    size_t size = 0;
    uint8_t *payload = NULL;
    EXPECT_EQ(OC_STACK_OK, AclToCBORPayload(&acl, &payload, &size));
    ASSERT_TRUE(NULL != payload);

    // Security Payload
    OCSecurityPayload *securityPayload = OCSecurityPayloadCreate(payload, size);
    ASSERT_TRUE(NULL != securityPayload);

    //Create Entity Handler POST request payload
    OCEntityHandlerRequest ehReq = OCEntityHandlerRequest();
    ehReq.method = OC_REST_POST;
    ehReq.payload = (OCPayload *)securityPayload;
    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    //Create Entity Handler GET request wit query
    ehReq.method = OC_REST_GET;
    char query[] = "subjectuuid=32323232-3232-3232-3232-323232323232;resources=/a/led";
    ehReq.query = (char*)OICMalloc(strlen(query)+1);
    ASSERT_TRUE(NULL != ehReq.query);
    OICStrcpy(ehReq.query, strlen(query)+1, query);

    ACLEntityHandler(OC_REQUEST_FLAG, &ehReq, NULL);

    // Perform cleanup
    OCPayloadDestroy((OCPayload *)securityPayload);
    DeInitACLResource();
    OICFree(ehReq.query);
    OICFree(payload);
}
