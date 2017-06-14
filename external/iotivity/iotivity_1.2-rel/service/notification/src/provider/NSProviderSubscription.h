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

#ifndef _NS_PROVIDER_SUBSCRIPTION_H_
#define _NS_PROVIDER_SUBSCRIPTION_H_

#include "logger.h"
#include "ocstack.h"
#include "ocpayload.h"
#include "NSCommon.h"
#include "NSConstants.h"
#include "NSProviderScheduler.h"
#include "NSProviderResource.h"
#include "NSProviderMemoryCache.h"
#include "NSProviderSystem.h"
#include "oic_string.h"
#include "oic_malloc.h"
#include <stdbool.h>

NSCacheList * consumerSubList;

NSResult NSInitSubscriptionList();
NSResult NSSetSubscriptionAccessPolicy(bool policy);
NSResult NSSendAccessPolicyResponse(OCEntityHandlerRequest *entityHandlerRequest);
void NSHandleSubscription(OCEntityHandlerRequest *entityHandlerRequest,
        NSResourceType resourceType);
void NSHandleUnsubscription(OCEntityHandlerRequest *entityHandlerRequest);
void NSAskAcceptanceToUser(OCEntityHandlerRequest *entityHandlerRequest);
NSResult NSSendConsumerSubResponse(OCEntityHandlerRequest *entityHandlerRequest);
NSResult NSSendResponse(const char * id, bool accepted);

#endif /* _NS_PROVIDER_SUBSCRIPTION_H_ */
