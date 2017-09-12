//******************************************************************
//
// Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef CRL_LOGGING_H_
#define CRL_LOGGING_H_

#include "logger.h"
#include "experimental/securevirtualresourcetypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CRL_TAG "CRL_LOG"

#ifdef TB_LOG
    #define OIC_LOG_CRL(level, crl) printCRL((level),(crl))

INLINE_API void printCRL(LogLevel level, const OicSecCrl_t *crl)
{
    OIC_LOG_V(level, CRL_TAG, "Print CRL @ %p:", crl);

    if (NULL == crl)
    {
        return;
    }

    OIC_LOG(level, CRL_TAG, "CRL object contains:");
    OIC_LOG_V(level, CRL_TAG, "id = %d", crl->CrlId);
    OIC_LOG_BUFFER(level, CRL_TAG, crl->ThisUpdate.data, crl->ThisUpdate.len);

    OIC_LOG(level, CRL_TAG, "crl:");
    OIC_LOG_V(level, CRL_TAG, "encoding = %d", crl->CrlData.encoding);
    OIC_LOG_V(level, CRL_TAG, "data (length = %" PRIuPTR "):", crl->CrlData.len);
    OIC_LOG_BUFFER(level, CRL_TAG, crl->CrlData.data, crl->CrlData.len);
}
#else
    #define OIC_LOG_CRL(level, crl)
#endif

#ifdef __cplusplus
}
#endif

#endif
