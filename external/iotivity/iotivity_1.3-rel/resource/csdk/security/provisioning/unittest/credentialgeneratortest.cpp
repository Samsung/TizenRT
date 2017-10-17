/* *****************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
#include <gtest/gtest.h>
#include "credentialgenerator.h"
#include "oic_malloc.h"


TEST(PMGeneratePairWiseCredentialsTest, InvalidFirstDevID)
{
    OicUuid_t *SecondDevID = (OicUuid_t*)OICMalloc(sizeof(OicUuid_t));
    if(SecondDevID)
    {
        SecondDevID->id[0] = 2;
    }
    OicSecCred_t *cred1 = NULL;
    OicSecCred_t *cred2 = NULL;
    size_t keySize = OWNER_PSK_LENGTH_128;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PMGeneratePairWiseCredentials(NO_SECURITY_MODE,
              keySize, NULL, SecondDevID, NULL, NULL, &cred1, &cred2));
    OICFree(SecondDevID);
}

TEST(PMGeneratePairWiseCredentialsTest, InvalidSecondDevID)
{
    OicUuid_t *firstDevID = (OicUuid_t*)OICMalloc(sizeof(OicUuid_t));
    if(firstDevID)
    {
        firstDevID->id[0] = 1;
    }
    OicUuid_t *provisioningDevID = (OicUuid_t*)OICMalloc(sizeof(OicUuid_t));
    if(provisioningDevID)
    {
        provisioningDevID->id[0] = 2;
    }
    OicSecCred_t *cred1 = NULL;
    OicSecCred_t *cred2 = NULL;
    size_t keySize = OWNER_PSK_LENGTH_128;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PMGeneratePairWiseCredentials(NO_SECURITY_MODE, keySize,
              firstDevID, NULL, NULL, NULL, &cred1, &cred2));
    OICFree(firstDevID);
    OICFree(provisioningDevID);
}

TEST(PMGeneratePairWiseCredentialsTest, InvalidCred)
{
    OicUuid_t *firstDevID = (OicUuid_t*)OICMalloc(sizeof(OicUuid_t));
    if(firstDevID)
    {
        firstDevID->id[0] = 1;
    }
    OicUuid_t *SecondDevID = (OicUuid_t*)OICMalloc(sizeof(OicUuid_t));
    if(SecondDevID)
    {
        SecondDevID->id[0] = 2;
    }
    OicUuid_t *provisioningDevID = (OicUuid_t*)OICMalloc(sizeof(OicUuid_t));
    if(provisioningDevID)
    {
        provisioningDevID->id[0] = 3;
    }
    size_t keySize = OWNER_PSK_LENGTH_128;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PMGeneratePairWiseCredentials(NO_SECURITY_MODE, keySize,
              firstDevID, SecondDevID, NULL, NULL, NULL, NULL));
    OICFree(firstDevID);
    OICFree(SecondDevID);
    OICFree(provisioningDevID);
}
