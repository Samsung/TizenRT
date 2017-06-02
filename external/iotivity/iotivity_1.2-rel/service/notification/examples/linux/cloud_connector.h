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

#include "ocstack.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define DEFAULT_CONTEXT_VALUE 0x99
#define DEFAULT_AUTH_SIGNUP "/oic/account"
#define DEFAULT_AUTH_SESSION "/oic/account/session"
#define DEFAULT_AUTH_REFRESH "/oic/account/tokenrefresh"

OCStackResult OCCloudSignup(const char *host, const char *deviceId,
        const char *authprovider, const char *authcode,
        OCClientResponseHandler response);

OCStackResult OCCloudSession(const char *host, const char *query, const char *uId,
        const char *deviceId, const char *accesstoken, bool isLogin,
        OCClientResponseHandler response);

OCStackResult OCCloudRefresh(const char *host, const char *query, const char *uId,
        const char *deviceId, const char *refreshtoken, OCClientResponseHandler response);

OCStackResult OCCloudLogin(const char *host, const char *uId, const char *deviceId,
        const char *accesstoken, OCClientResponseHandler response);

OCStackResult OCCloudLogout(const char *host, OCClientResponseHandler response);

OCStackApplicationResult CloudLoginoutCallback(void *ctx,
        OCDoHandle handle, OCClientResponse *clientResponse);

OCStackApplicationResult CloudSignupCallback(void *ctx,
        OCDoHandle handle, OCClientResponse *clientResponse);

bool IsCloudLoggedin();

#ifdef __cplusplus
}
#endif // __cplusplus

