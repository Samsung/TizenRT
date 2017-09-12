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

#ifndef ACL_LOGGING_H_
#define ACL_LOGGING_H_

#include "logger.h"
#include "oic_malloc.h"
#include "ocrandom.h"
#include "utlist.h"
#include "experimental/securevirtualresourcetypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define ACL_TAG "ACL_LOG"

#ifdef TB_LOG
    #define OIC_LOG_ACL(level, acl) printACL((level),(acl))
    #define OIC_LOG_ACE(level, ace) printACE((level),(ace))

INLINE_API void printACE(LogLevel level, const OicSecAce_t *ace)
{
    OIC_LOG(level, ACL_TAG, "=================================================");
    OIC_LOG_V(level, ACL_TAG, "ACE @ %p", ace);

    if (NULL == ace)
    {
        return;
    }

    OIC_LOG_V(level, ACL_TAG, "    aceid = %d", ace->aceid);

    OIC_LOG_V(level, ACL_TAG, "    permission = %#x", (uint32_t)ace->permission);

    // Log the subject
    if (ace->subjectType == OicSecAceUuidSubject)
    {
        char uuidString[UUID_STRING_SIZE] = { 0 };
        bool convertedUUID = OCConvertUuidToString(ace->subjectuuid.id, uuidString);
        OIC_LOG_V(level, ACL_TAG, "    subject UUID = %s", convertedUUID ? uuidString : "incorrect format");
    }
    else if (ace->subjectType == OicSecAceRoleSubject)
    {
        OIC_LOG_V(level, ACL_TAG, "    role id = %s", ace->subjectRole.id);
        OIC_LOG_V(level, ACL_TAG, "    authority = %s", ace->subjectRole.authority);
    }
    else if (ace->subjectType == OicSecAceConntypeSubject)
    {
        const char *conntype;
        if (ANON_CLEAR == ace->subjectConn)
        {
            conntype = "ANON_CLEAR";
        }
        else if (AUTH_CRYPT == ace->subjectConn)
        {
            conntype = "AUTH_CRYPT";
        }
        else
        {
            conntype = "Unknown conntype in subjectConn";
        }
        OIC_LOG_V(level, ACL_TAG, "    conntype = %s", conntype);
    }
    else
    {
        OIC_LOG(level, ACL_TAG, "    subject = (subject of unknown type)");
    }

    // Log all resources this ACE applies to.
    OicSecRsrc_t *resource = NULL;
    size_t resourceCount = 0;
    LL_FOREACH(ace->resources, resource)
    {
        OIC_LOG_V(level, ACL_TAG, "    resources[%" PRIuPTR "]:", resourceCount);
        OIC_LOG_V(level, ACL_TAG, "        href = %s", resource->href ? resource->href : "null");

        for (size_t i = 0; i < resource->typeLen; i++)
        {
            OIC_LOG_V(level, ACL_TAG, "        types[%" PRIuPTR "] = %s", i,
                resource->types[i] ? resource->types[i] : "null");
        }

        for (size_t i = 0; i < resource->interfaceLen; i++)
        {
            OIC_LOG_V(level, ACL_TAG, "        interfaces[%" PRIuPTR "] = %s", i,
                resource->interfaces[i] ? resource->interfaces[i] : "null");
        }

        resourceCount++;
    }

    // Log the validities.
    OicSecValidity_t *validity = NULL;
    size_t validityCount = 0;
    LL_FOREACH(ace->validities, validity)
    {
        OIC_LOG_V(level, ACL_TAG, "    validities[%" PRIuPTR "]:", validityCount);
        OIC_LOG_V(level, ACL_TAG, "        period = %s", validity->period);
        for (size_t i = 0; i < validity->recurrenceLen; i++)
        {
            OIC_LOG_V(level, ACL_TAG, "    recurrences[%" PRIuPTR "] = %s", i,
                validity->recurrences[i] ? validity->recurrences[i] : "null");
        }
        validityCount++;
    }

    OIC_LOG(level, ACL_TAG, "=================================================");
}

INLINE_API void printACL(LogLevel level, const OicSecAcl_t* acl)
{
    OIC_LOG_V(level, ACL_TAG, "Print ACL @ %p:", acl);

    if (NULL == acl)
    {
        return;
    }

    char rowner[UUID_STRING_SIZE] = { 0 };
    if (OCConvertUuidToString(acl->rownerID.id, rowner))
    {
        OIC_LOG_V(level, ACL_TAG, "rowner id = %s", rowner);
    }
    else
    {
        OIC_LOG(ERROR, ACL_TAG, "Can't convert rowner uuid to string");
    }

    const OicSecAce_t *ace = acl->aces;
    size_t ace_count = 0;
    while (ace)
    {
        OIC_LOG_V(level, ACL_TAG, "Print ace[%" PRIuPTR "]:", ace_count);
        printACE(level, ace);
        ace = ace->next;
        ace_count++;
    }
}
#else
    #define OIC_LOG_ACL(level, acl)
    #define OIC_LOG_ACE(level, ace)
#endif

#ifdef __cplusplus
}
#endif

#endif
