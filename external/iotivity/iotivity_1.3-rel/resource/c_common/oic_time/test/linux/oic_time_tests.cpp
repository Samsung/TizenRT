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

#include "oic_time.h"
#include <gtest/gtest.h>
#include <stdint.h>

// Tests to get current time in milli seconds
TEST(TimeTests, GetCurrentTimeInMilliSec)
{
    uint64_t  currentMilliSecTime = OICGetCurrentTime(TIME_IN_MS);
    EXPECT_TRUE(0 != currentMilliSecTime);
}

//Test to get current time in micro seconds
TEST(TimeTests, GetCurrentTimeInMicroSec)
{
    uint64_t currentMicroSecTime = OICGetCurrentTime(TIME_IN_US);
    EXPECT_TRUE(0 != currentMicroSecTime);
}
