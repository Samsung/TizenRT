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

#ifndef _NS_PROVIDER_LISTENER__H_
#define _NS_PROVIDER_LISTENER__H_

#include <octypes.h>
#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"
#include "NSStructs.h"
#include "NSConstants.h"
#include "NSProviderSystem.h"
#include "NSProviderScheduler.h"
#include "cautilinterface.h"
#include "oic_string.h"
#include "oic_malloc.h"
#include "NSUtil.h"
#include "NSProviderMemoryCache.h"

OCEntityHandlerResult NSEntityHandlerNotificationCb(OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *entityHandlerRequest, void* callback);

OCEntityHandlerResult NSEntityHandlerMessageCb(OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *entityHandlerRequest, void* callback);

OCEntityHandlerResult NSEntityHandlerSyncCb(OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *entityHandlerRequest, void* callback);

OCEntityHandlerResult NSEntityHandlerTopicCb(OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *entityHandlerRequest, void* callback);

void NSProviderConnectionStateListener(const CAEndpoint_t * info, bool isConnected);

void NSProviderAdapterStateListener(CATransportAdapter_t adapter, bool enabled);

#ifdef WITH_MQ
OCStackApplicationResult NSProviderGetMQResponseCB(void * ctx, OCDoHandle handle,
        OCClientResponse * clientResponse);

OCStackApplicationResult NSProviderPublishMQResponseCB(void *ctx, OCDoHandle handle,
        OCClientResponse *clientResponse);
#endif

#endif /* _NS_PROVIDER_LISTENER__H_ */
