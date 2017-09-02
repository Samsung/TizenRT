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

#include "CoapHttpHandler.h"

#include "oic_malloc.h"
#include "oic_string.h"
#include "logger.h"
#include "ocpayload.h"
#include "uarraylist.h"
#include "CoapHttpParser.h"
#include "CoapHttpMap.h"
#if defined (__TIZENRT__)
#include <json/cJSON.h>
#else
#include "cJSON.h"
#endif

#include <signal.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


typedef struct
{
    OCMethod method;
    OCRequestHandle requestHandle;
}CHPRequest_t; 

static int g_quitFlag = 0;
static int g_secureFlag = 0;

class CoApHttpTest: public TestWithMock
{
protected:
    void SetUp()
    {
        TestWithMock::SetUp();
    }
};

TEST_F(CoApHttpTest, OcInit)
{
    EXPECT_EQ(OC_STACK_OK, OCInit(NULL, 0, OC_SERVER));
}

TEST_F(CoApHttpTest, CHPInitialize)
{

    EXPECT_EQ(OC_STACK_OK, (CHPInitialize(g_secureFlag)));
}

TEST_F(CoApHttpTest, CHPIsInitialized)
{

    EXPECT_EQ(true, (CHPIsInitialized()));
}


TEST_F(CoApHttpTest, CHPTerminate)
{

    EXPECT_EQ(OC_STACK_OK, (CHPTerminate()));
}

TEST_F(CoApHttpTest, CHPGetOCCode)
{
    CHPRequest_t context;
    //enum HttpResponseResult_t httpCode = 200;
    static OCResourceHandle g_proxyHandle = NULL;
    OCEntityHandlerResponse response = { .requestHandle = context.requestHandle,
                                         .resourceHandle = g_proxyHandle};
    response.persistentBufferFlag = 0;

    EXPECT_EQ(OC_STACK_OK, (CHPGetOCCode((const HttpResponseResult_t)200,(const OCMethod)context.method,&response.ehResult)));
}

TEST_F(CoApHttpTest, CHPGetOCOption)
{
    int numOptions = 0;
    OCHeaderOption *ret = NULL;
    const HttpResponse_t *httpResponse = NULL;
    HttpHeaderOption_t *httpOption = NULL;
    EXPECT_NE(OC_STACK_OK, (CHPGetOCOption(httpOption,ret)));
}

TEST_F(CoApHttpTest, CHPGetOCContentType)
{
    HttpResponse_t httpResponse;
    //char *httpContentType = "OC_FORMAT_CBOR";
    char httpContentType[64];
    strcpy(httpContentType, "application/cbor");
    EXPECT_EQ(OC_FORMAT_CBOR, (CHPGetOCContentType(httpContentType)));
}

TEST_F(CoApHttpTest, CHPGetHttpMethod)
{
    CHPRequest_t context;
    HttpRequest_t httpRequest = { .httpMajor = 1,
                                  .httpMinor = 1};

    EXPECT_NE(OC_STACK_OK, (CHPGetHttpMethod((const OCMethod)context.method,&httpRequest.method)));
}

TEST_F(CoApHttpTest, CHPParserInitialize)
{
    EXPECT_EQ(OC_STACK_OK, (CHPParserInitialize()));
}

TEST_F(CoApHttpTest, CHPParserTerminate)
{
    EXPECT_EQ(OC_STACK_OK, (CHPParserTerminate()));
}

