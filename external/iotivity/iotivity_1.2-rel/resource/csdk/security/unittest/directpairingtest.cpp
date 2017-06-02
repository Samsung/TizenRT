//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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
#include "directpairing.h"

using namespace std;

#define TAG  "DIRECTPAIRING-UT"

static OCDirectPairingDev_t peer;
static OicSecPrm_t pmSel;
static char pinNumber;

static void ResultCB(void *UNUSED1, OCDirectPairingDev_t *UNUSED2, OCStackResult UNUSED3)
{
    //dummy callback
    (void) UNUSED1;
    (void) UNUSED2;
    (void) UNUSED3;
}

TEST(DPDirectPairingTest, NullPeer)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DPDirectPairing(NULL, NULL, pmSel, &pinNumber, &ResultCB));
}

TEST(DPDirectPairingTest, NullPinNumber)
{
    EXPECT_EQ(OC_STACK_INVALID_PARAM, DPDirectPairing(NULL, &peer, pmSel, NULL, &ResultCB));
}

