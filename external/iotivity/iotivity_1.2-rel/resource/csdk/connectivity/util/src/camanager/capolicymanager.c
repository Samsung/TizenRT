/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cathreadpool.h"
#include "octhread.h"
#include "uarraylist.h"
#include "cacommon.h"
#include "logger.h"

#include "caconnectionmanager.h"
#include "capolicymanager.h"
#include "oic_string.h"

#define TAG "OIC_CM_POLICY"

static CMConfigureInfo_t g_configure = {.addr = NULL,
                                        .adapter = CA_ADAPTER_IP,
                                        .level = NORMAL_SPEED};

void CMSetConfigure(CMConfigureInfo_t info)
{
    OIC_LOG(DEBUG, TAG, "CMSetConfigurePolicy");
    OICStrcpy(g_configure.addr, sizeof(g_configure.addr), info.addr);
    g_configure.adapter = info.adapter;
    g_configure.level = info.level;
}

const char* CMGetTargetAddress()
{
    OIC_LOG(DEBUG, TAG, "CMGetTargetAddress");
    return g_configure.addr;
}

CATransportAdapter_t CMGetAdapterType()
{
    OIC_LOG(DEBUG, TAG, "CMGetAdapterType");
    return g_configure.adapter;
}

CMSpeedLevel_t CMGetSpeedOfResponseLevel()
{
    OIC_LOG(DEBUG, TAG, "CMGetSpeedOfResponseLevel");
    return g_configure.level;
}
