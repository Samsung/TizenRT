//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include "UnitTestHelper.h"

#include "RCSResponse.h"
#include "RCSSeparateResponse.h"
#include "RCSException.h"

using namespace OIC::Service;

TEST(RCSResponseTest, DefaultSetResponseHasDefaultMethod)
{
    EXPECT_EQ(RCSSetResponse::AcceptanceMethod::DEFAULT,
            RCSSetResponse::defaultAction().getAcceptanceMethod());
}

TEST(RCSResponseTest, AcceptSetResponseHasAcceptMethod)
{
    EXPECT_EQ(RCSSetResponse::AcceptanceMethod::ACCEPT,
            RCSSetResponse::accept().getAcceptanceMethod());
}

TEST(RCSResponseTest, IgnoreSetResponseHasIgnoreMethod)
{
    EXPECT_EQ(RCSSetResponse::AcceptanceMethod::IGNORE,
            RCSSetResponse::ignore().getAcceptanceMethod());
}

TEST(RCSResponseTest, SetResponseHasMethodSetBySetter)
{
    RCSSetResponse::AcceptanceMethod method = RCSSetResponse::AcceptanceMethod::ACCEPT;
    RCSSetResponse response =
            RCSSetResponse::defaultAction().setAcceptanceMethod(method);

    EXPECT_EQ(method, response.getAcceptanceMethod());
}

TEST(RCSResponseTest, SeparateResponseHasNoHandler)
{
    RCSGetResponse response = RCSGetResponse::separate();
    EXPECT_EQ(nullptr, response.getHandler());
}

TEST(RCSResponseTest, ThrowIfRequestIsInvalidWhenConstructingSeparateResponse)
{
    RCSRequest aRequest;

    EXPECT_THROW(RCSSeparateResponse resp(aRequest), RCSInvalidParameterException);
}
