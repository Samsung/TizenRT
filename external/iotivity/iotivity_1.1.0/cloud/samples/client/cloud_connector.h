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

#ifndef _CLOUD_CONNECTOR_H_
#define _CLOUD_CONNECTOR_H_

#include "ocstack.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

OCStackResult OCCloudRegisterLogin(const char *host, const char *auth_provider,
                                   const char *auth_code, OCClientResponseHandler response);
OCStackResult OCCloudLogin(const char *host, const char *auth_session,
                           OCClientResponseHandler response);
OCStackResult OCCloudLogout(const char *host, const char *auth_session,
                            OCClientResponseHandler response);
OCStackResult OCCloudPublish(const char *host, const char *query,
                             OCClientResponseHandler response, int numArg, ...);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
