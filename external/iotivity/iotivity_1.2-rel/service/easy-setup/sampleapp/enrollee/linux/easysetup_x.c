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


#include "easysetup_x.h"
#include "string.h"
#include "oic_malloc.h"
#include "logger.h"
#include "ocpayload.h"

/**
 * @var ESX_ENROLLEE_TAG
 * @brief Logging tag for module name.
 */
#define ESX_ENROLLEE_TAG "ESX"

UserProperties g_userProperties;

void SetUserProperties(const UserProperties *prop)
{
    if(prop != NULL)
    {
        strncpy(g_userProperties.userValue_str, prop->userValue_str, MAXLEN_STRING);
        g_userProperties.userValue_int = prop->userValue_int;
    }
}

void ReadUserdataCb(OCRepPayload* payload, char* resourceType, void** userdata)
{
    OIC_LOG(DEBUG, ESX_ENROLLEE_TAG, "ReadUserdataCb IN");

    if(payload != NULL)
    {
        if(strstr(resourceType, OC_RSRVD_ES_RES_TYPE_WIFICONF))
        {
            int64_t value = -1;
            if (OCRepPayloadGetPropInt(payload, USERPROPERTY_KEY_INT, &value))
            {
                if(*userdata != NULL)
                {
                    *userdata = (void*)OICMalloc(sizeof(UserProperties));
                    if( *userdata == NULL )
                    {
                        OIC_LOG(ERROR, ESX_ENROLLEE_TAG, "OICMalloc for UserProperties is failed");
                        return ;
                    }
                }

                ((UserProperties*)(*userdata))->userValue_int = value;
                OIC_LOG_V(INFO, ESX_ENROLLEE_TAG, "[User specific property] %s : %ld",
                                                                            USERPROPERTY_KEY_INT, value);
                g_userProperties.userValue_int = value;
            }
        }
    }

    OIC_LOG(DEBUG, ESX_ENROLLEE_TAG, "ReadUserdataCb OUT");
}

void WriteUserdataCb(OCRepPayload* payload, char* resourceType)
{
    OIC_LOG(DEBUG, ESX_ENROLLEE_TAG, "WriteUserdataCb IN");

    if(payload != NULL)
    {
        if(strstr(resourceType, OC_RSRVD_ES_RES_TYPE_WIFICONF))
        {
            OCRepPayloadSetPropInt(payload, USERPROPERTY_KEY_INT, g_userProperties.userValue_int);
        }

        if(strstr(resourceType, OC_RSRVD_ES_RES_TYPE_DEVCONF))
        {
            OCRepPayloadSetPropString(payload, USERPROPERTY_KEY_STR, g_userProperties.userValue_str);
        }
    }

    OIC_LOG(DEBUG, ESX_ENROLLEE_TAG, "WriteUserdataCb OUT");
}
