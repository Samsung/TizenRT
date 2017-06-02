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

#include "easysetup.h"
#include "escommon.h"

#ifndef EASYSETUPX_ENROLLEE_H__
#define EASYSETUPX_ENROLLEE_H__

#define MAXLEN_STRING 100

typedef struct UserProperties_t
{
    int userValue_int;                 /**< User-specific property in WiFi resource **/
    char userValue_str[MAXLEN_STRING]; /**< User-specific property in DevConf resource **/
} UserProperties;

#define USERPROPERTY_KEY_INT "x.user.property.int"
#define USERPROPERTY_KEY_STR "x.user.property.str"

void ReadUserdataCb(OCRepPayload* payload, char* resourceType, void** userdata);
void WriteUserdataCb(OCRepPayload* payload, char* resourceType);

void SetUserProperties(const UserProperties *prop);


#endif /* EASYSETUPX_ENROLLEE_H__ */
