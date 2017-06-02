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

#ifndef _NS_UTIL__H_
#define _NS_UTIL__H_

#include "logger.h"
#include <octypes.h>
#include <stdbool.h>
#include "ocstack.h"
#include "ocpayload.h"
#include "octypes.h"
#include "NSStructs.h"
#include "NSConstants.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocrandom.h"
#include "oic_time.h"
#include "NSProviderSystem.h"

OCEntityHandlerRequest *NSCopyOCEntityHandlerRequest(OCEntityHandlerRequest *);
NSResult NSFreeOCEntityHandlerRequest(OCEntityHandlerRequest *);

NSMessage * NSInitializeMessage();
NSResult NSFreeMessage(NSMessage *);
NSMessage * NSDuplicateMessage(NSMessage *);

NSResult NSFreeSync(NSSyncInfo *);
NSSyncInfo * NSDuplicateSync(NSSyncInfo *);
NSSyncInfo * NSGetSyncInfo(OCPayload * payload);

NSResult NSFreeConsumer(NSConsumer *);
NSConsumer * NSDuplicateConsumer(NSConsumer *);

NSTopicList * NSInitializeTopicList();

void NSDuplicateSetPropertyString(OCRepPayload **, const char *, const char *);
void NSDuplicateSetPropertyInt(OCRepPayload ** msgPayload, const char * name, int64_t value);
NSResult NSGenerateUUIDStr(char uuidStr[UUID_STRING_SIZE]);

char * NSGetValueFromQuery(char *query, char * compareKey);
NSResult NSFreeMalloc(char ** obj);

NSResult NSFreeMediaContents(NSMediaContents * obj);
NSMediaContents * NSDuplicateMediaContents(NSMediaContents * copyObj);

OCRepPayloadValue* NSPayloadFindValue(const OCRepPayload* payload, const char* name);

OCDevAddr * NSChangeAddress(const char * address);
bool NSOCResultToSuccess(OCStackResult ret);

#endif /* _NS_UTIL__H_ */
