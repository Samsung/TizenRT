//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
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
#include <string.h>

#include "utlist.h"
#include "oic_malloc.h"
#include "policyengine.h"
#include "amsmgr.h"
#include "resourcemanager.h"
#include "securevirtualresourcetypes.h"
#include "srmresourcestrings.h"
#include "logger.h"
#include "aclresource.h"
#include "srmutility.h"
#include "doxmresource.h"
#include "iotvticalendar.h"
#include "pstatresource.h"
#include "dpairingresource.h"
#include "pconfresource.h"
#include "amaclresource.h"
#include "credresource.h"

#define TAG "OIC_SRM_PE"

uint16_t GetPermissionFromCAMethod_t(const CAMethod_t method)
{
    uint16_t perm = 0;
    switch (method)
    {
        case CA_GET:
            perm = (uint16_t)PERMISSION_READ;
            break;
        case CA_POST: // Treat all POST as Write (Update) because
                      // we don't know if resource exists yet.
                      // This will be addressed in IoTivity impl of OCF 1.0
            perm = (uint16_t)PERMISSION_WRITE;
            break;
        case CA_PUT: // Per convention, OIC/OCF uses PUT only for Create,
                     // never for Update.
            perm = (uint16_t)PERMISSION_CREATE;
            break;
        case CA_DELETE:
            perm = (uint16_t)PERMISSION_DELETE;
            break;
        default: // if not recognized, must assume requesting full control
            perm = (uint16_t)PERMISSION_FULL_CONTROL;
            break;
    }
    return perm;
}

/**
 * Compares two OicUuid_t structs.
 *
 * @return true if the two OicUuid_t structs are equal, else false.
 */
static bool UuidCmp(OicUuid_t *firstId, OicUuid_t *secondId)
{
    // TODO use VERIFY macros to check for null when they are merged.
    if(NULL == firstId || NULL == secondId)
    {
        return false;
    }
    // Check empty uuid string
    if('\0' == firstId->id[0] || '\0' == secondId->id[0])
    {
        return false;
    }
    for(int i = 0; i < UUID_LENGTH; i++)
    {
        if(firstId->id[i] != secondId->id[i])
        {
            return false;
        }
    }
    return true;
}

void SetPolicyEngineState(PEContext_t *context, const PEState_t state)
{
    if (NULL == context)
    {
        return;
    }

    // Clear stateful context variables.
    memset(&context->subject, 0, sizeof(context->subject));
    memset(&context->resource, 0, sizeof(context->resource));
    context->permission = 0x0;
    context->amsProcessing = false;
    context->retVal = ACCESS_DENIED_POLICY_ENGINE_ERROR;

    if (context->amsMgrContext)
    {
        if (context->amsMgrContext->requestInfo)
        {
            FreeCARequestInfo(context->amsMgrContext->requestInfo);
        }
        OICFree(context->amsMgrContext->endpoint);
        memset(context->amsMgrContext, 0, sizeof(AmsMgrContext_t));
    }

    // Set state.
    context->state = state;
}

/**
 * Compare the request's subject to DevOwner.
 *
 * @return true if context->subjectId == GetDoxmDevOwner(), else false.
 */
static bool IsRequestFromDevOwner(PEContext_t *context)
{
    bool retVal = false;

    if(NULL == context)
    {
        return retVal;
    }

    /*
    if(OC_STACK_OK == GetDoxmDevOwnerId(&ownerid))
    {
        retVal = UuidCmp(&context->subject, &ownerid);
    }
    */

    // TODO: Added as workaround for CTT
    OicSecDoxm_t* doxm = (OicSecDoxm_t*) GetDoxmResourceData();
    if (doxm)
    {
        retVal = UuidCmp(&doxm->owner, &context->subject);
    }
    return retVal;
}


#ifdef MULTIPLE_OWNER
/**
 * Compare the request's subject to SubOwner.
 *
 * @return true if context->subjectId exist subowner list, else false.
 */
static bool IsRequestFromSubOwner(PEContext_t *context)
{
    bool retVal = false;

    if(NULL == context)
    {
        return retVal;
    }

    if(IsSubOwner(&context->subject))
    {
        retVal = true;
    }

    if(true == retVal)
    {
        OIC_LOG(INFO, TAG, "PE.IsRequestFromSubOwner(): returning true");
    }
    else
    {
        OIC_LOG(INFO, TAG, "PE.IsRequestFromSubOwner(): returning false");
    }

    return retVal;
}


/**
 * Verify the SubOwner's request.
 *
 * @return true if request is valid, else false.
 */
static bool IsValidRequestFromSubOwner(PEContext_t *context)
{
    bool isValidRequest = false;

    if(NULL == context)
    {
        return isValidRequest;
    }

    switch(context->resourceType)
    {
        case OIC_R_DOXM_TYPE:
            //SubOwner has READ permission only for DOXM
            if(PERMISSION_READ == context->permission)
            {
                isValidRequest = true;
            }
            break;
        case OIC_R_PSTAT_TYPE:
            //SubOwner has full permsion for PSTAT except RESET
            isValidRequest = IsValidPstatAccessForSubOwner(context->payload, context->payloadSize);
            break;
        case OIC_R_CRED_TYPE:
            //SubOwner can only access the credential which is registered as the eowner.
            isValidRequest = IsValidCredentialAccessForSubOwner(&context->subject, context->payload, context->payloadSize);
            break;
        case OIC_R_ACL_TYPE:
            //SubOwner can only access the ACL which is registered as the eowner.
            isValidRequest = IsValidAclAccessForSubOwner(&context->subject, context->payload, context->payloadSize);
            break;
        default:
            //SubOwner has full permission for all resource except the security resource
            isValidRequest = true;
            break;
    }

    if(isValidRequest)
    {
        OIC_LOG(INFO, TAG, "PE.IsValidRequestFromSubOwner(): returning true");
    }
    else
    {
        OIC_LOG(INFO, TAG, "PE.IsValidRequestFromSubOwner(): returning false");
    }

    return isValidRequest;
}
#endif //MULTIPLE_OWNER


// TODO - remove these function placeholders as they are implemented
// in the resource entity handler code.
// Note that because many SVRs do not have a rowner, in those cases we
// just return "OC_STACK_ERROR" which results in a "false" return by
// IsRequestFromResourceOwner().
// As these SVRs are revised to have a rowner, these functions should be
// replaced (see pstatresource.c for example of GetPstatRownerId).

OCStackResult GetCrlRownerId(OicUuid_t *rowner)
{
    OC_UNUSED(rowner);
    rowner = NULL;
    return OC_STACK_ERROR;
}

OCStackResult GetSaclRownerId(OicUuid_t *rowner)
{
    OC_UNUSED(rowner);
    rowner = NULL;
    return OC_STACK_ERROR;
}

OCStackResult GetSvcRownerId(OicUuid_t *rowner)
{
    OC_UNUSED(rowner);
    rowner = NULL;
    return OC_STACK_ERROR;
}

static GetSvrRownerId_t GetSvrRownerId[OIC_SEC_SVR_TYPE_COUNT] = {
    GetAclRownerId,
    GetAmaclRownerId,
    GetCredRownerId,
    GetCrlRownerId,
    GetDoxmRownerId,
    GetDpairingRownerId,
    GetPconfRownerId,
    GetPstatRownerId,
    GetSaclRownerId,
    GetSvcRownerId
};

/**
 * Compare the request's subject to resource.ROwner.
 *
 * @return true if context->subjectId equals SVR rowner id, else return false
 */
bool IsRequestFromResourceOwner(PEContext_t *context)
{
    bool retVal = false;
    OicUuid_t resourceOwner;

    if(NULL == context)
    {
        return false;
    }

    if((OIC_R_ACL_TYPE <= context->resourceType) && \
        (OIC_SEC_SVR_TYPE_COUNT > context->resourceType))
    {
        if(OC_STACK_OK == GetSvrRownerId[(int)context->resourceType](&resourceOwner))
        {
            retVal = UuidCmp(&context->subject, &resourceOwner);
        }
    }

    if(true == retVal)
    {
        OIC_LOG(INFO, TAG, "PE.IsRequestFromResourceOwner(): returning true");
    }
    else
    {
        OIC_LOG(INFO, TAG, "PE.IsRequestFromResourceOwner(): returning false");
    }

    return retVal;
}

INLINE_API bool IsRequestSubjectEmpty(PEContext_t *context)
{
    OicUuid_t emptySubject = {.id={0}};

    if(NULL == context)
    {
        return false;
    }

    return (memcmp(&context->subject, &emptySubject, sizeof(OicUuid_t)) == 0) ?
            true : false;
}

/**
 * Bitwise check to see if 'permission' contains 'request'.
 *
 * @param permission is the allowed CRUDN permission.
 * @param request is the CRUDN permission being requested.
 *
 * @return true if 'permission' bits include all 'request' bits.
 */
INLINE_API bool IsPermissionAllowingRequest(const uint16_t permission,
    const uint16_t request)
{
    if (request == (request & permission))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Compare the passed subject to the wildcard (aka anonymous) subjectId.
 *
 * @return true if 'subject' is the wildcard, false if it is not.
 */
INLINE_API bool IsWildCardSubject(OicUuid_t *subject)
{
    if(NULL == subject)
    {
        return false;
    }

    // Because always comparing to string literal, use strcmp()
    if(0 == memcmp(subject, &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Copy the subject, resource and permission into the context fields.
 */
static void CopyParamsToContext(PEContext_t     *context,
                                const OicUuid_t *subjectId,
                                const char      *resource,
                                const uint16_t  requestedPermission)
{
    size_t length = 0;

    if (NULL == context || NULL == subjectId || NULL == resource)
    {
        return;
    }

    memcpy(&context->subject, subjectId, sizeof(OicUuid_t));

    // Copy the resource string into context.
    length = sizeof(context->resource) - 1;
    strncpy(context->resource, resource, length);
    context->resource[length] = '\0';


    // Assign the permission field.
    context->permission = requestedPermission;
}

/**
 * Check whether 'resource' is getting accessed within the valid time period.
 *
 * @param acl is the ACL to check.
 *
 * @return true if access is within valid time period or if the period or recurrence is not present.
 * false if period and recurrence present and the access is not within valid time period.
 */
static bool IsAccessWithinValidTime(const OicSecAce_t *ace)
{
#ifndef WITH_ARDUINO //Period & Recurrence not supported on Arduino due
    //lack of absolute time
    if (NULL== ace || NULL == ace->validities)
    {
        return true;
    }

    //periods & recurrences rules are paired.
    if (NULL == ace->validities->recurrences)
    {
        return false;
    }

    OicSecValidity_t* validity =  NULL;
    LL_FOREACH(ace->validities, validity)
    {
        for(size_t i = 0; i < validity->recurrenceLen; i++)
        {
            if (IOTVTICAL_VALID_ACCESS ==  IsRequestWithinValidTime(validity->period,
                validity->recurrences[i]))
            {
                OIC_LOG(INFO, TAG, "Access request is in allowed time period");
                return true;
            }
        }
    }
    OIC_LOG(ERROR, TAG, "Access request is in invalid time period");
    return false;

#else
    return true;
#endif
}

/**
 * Check whether 'resource' is in the passed ACE.
 *
 * @param resource is the resource being searched.
 * @param ace is the ACE to check.
 *
 * @return true if 'resource' found, otherwise false.
 */
 static bool IsResourceInAce(const char *resource, const OicSecAce_t *ace)
{
    if (NULL== ace || NULL == resource)
    {
        return false;
    }

    OicSecRsrc_t* rsrc = NULL;
    LL_FOREACH(ace->resources, rsrc)
    {
         if (0 == strcmp(resource, rsrc->href) || // TODO null terms?
             0 == strcmp(WILDCARD_RESOURCE_URI, rsrc->href))
         {
             return true;
         }
    }
    return false;
}


/**
 * Find ACLs containing context->subject.
 * Search each ACL for requested resource.
 * If resource found, check for context->permission and period validity.
 * If the ACL is not found locally and AMACL for the resource is found
 * then sends the request to AMS service for the ACL.
 * Set context->retVal to result from first ACL found which contains
 * correct subject AND resource.
 */
static void ProcessAccessRequest(PEContext_t *context)
{
    OIC_LOG(DEBUG, TAG, "Entering ProcessAccessRequest()");
    if (NULL != context)
    {
        const OicSecAce_t *currentAce = NULL;
        OicSecAce_t *savePtr = NULL;

        // Start out assuming subject not found.
        context->retVal = ACCESS_DENIED_SUBJECT_NOT_FOUND;

        char *strUuid = NULL;
        if (OC_STACK_OK == ConvertUuidToStr(&context->subject, &strUuid))
        {
            OIC_LOG_V(DEBUG, TAG, "%s: subject : %s" ,__func__, strUuid);
            OICFree(strUuid);
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Can't convert subject uuid to string");
        }

        // Loop through all ACLs with a matching Subject searching for the right
        // ACL for this request.
        do
        {
            OIC_LOG_V(DEBUG, TAG, "%s: getting ACE..." ,__func__);
            currentAce = GetACLResourceData(&context->subject, &savePtr);

            if (NULL != currentAce)
            {
                // Found the subject, so how about resource?
                OIC_LOG_V(DEBUG, TAG, "%s:found ACE matching subject" ,__func__);

                // Subject was found, so err changes to Rsrc not found for now.
                context->retVal = ACCESS_DENIED_RESOURCE_NOT_FOUND;
                OIC_LOG_V(DEBUG, TAG, "%s:Searching for resource..." ,__func__);
                if (IsResourceInAce(context->resource, currentAce))
                {
                    OIC_LOG_V(INFO, TAG, "%s:found matching resource in ACE" ,__func__);

                    // Found the resource, so it's down to valid period & permission.
                    context->retVal = ACCESS_DENIED_INVALID_PERIOD;
                    if (IsAccessWithinValidTime(currentAce))
                    {
                        context->retVal = ACCESS_DENIED_INSUFFICIENT_PERMISSION;
                        if (IsPermissionAllowingRequest(currentAce->permission, context->permission))
                        {
                            context->retVal = ACCESS_GRANTED;
                        }
                    }
                }
            }
            else
            {
                OIC_LOG_V(INFO, TAG, "%s:no ACL found matching subject for resource %s",__func__, context->resource);
            }
        } while ((NULL != currentAce) && (ACCESS_GRANTED != context->retVal));

        if (IsAccessGranted(context->retVal))
        {
            OIC_LOG_V(INFO, TAG, "%s:Leaving ProcessAccessRequest(ACCESS_GRANTED)", __func__);
        }
        else
        {
            OIC_LOG_V(INFO, TAG, "%s:Leaving ProcessAccessRequest(ACCESS_DENIED)", __func__);
        }
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s:Leaving ProcessAccessRequest(context is NULL)", __func__);
    }
}

SRMAccessResponse_t CheckPermission(PEContext_t     *context,
                                    const OicUuid_t *subjectId,
                                    const char      *resource,
                                    const uint16_t  requestedPermission)
{
    SRMAccessResponse_t retVal = ACCESS_DENIED_POLICY_ENGINE_ERROR;

    VERIFY_NON_NULL(TAG, context, ERROR);
    VERIFY_NON_NULL(TAG, subjectId, ERROR);
    VERIFY_NON_NULL(TAG, resource, ERROR);

    // Each state machine context can only be processing one request at a time.
    // Therefore if the context is not in AWAITING_REQUEST or AWAITING_AMS_RESPONSE
    // state, return error. Otherwise, change to BUSY state and begin processing request.
    if (AWAITING_REQUEST == context->state || AWAITING_AMS_RESPONSE == context->state)
    {
        if (AWAITING_REQUEST == context->state)
        {
            SetPolicyEngineState(context, BUSY);
            CopyParamsToContext(context, subjectId, resource, requestedPermission);
        }

        // Before doing any ACL processing, check if request a) coming
        // from DevOwner AND b) the device is in Ready for OTM or Reset state
        // (which in IoTivity is equivalent to isOp == false && owned == false)
        // AND c) the request is for a SVR resource.
        // If all 3 conditions are met, grant request.
        bool isDeviceOwned = true; // default to value that will not grant access
        if (OC_STACK_OK != GetDoxmIsOwned(&isDeviceOwned)) // if runtime error, don't grant
        {
            context->retVal = ACCESS_DENIED_POLICY_ENGINE_ERROR;
        }
        // If we were able to get the value of doxm->isOwned, proceed with
        // test for implicit access...
        else if (IsRequestFromDevOwner(context) // if from DevOwner
        && (GetPstatIsop() == false) // AND if pstat->isOp == false
        && (isDeviceOwned == false) // AND if doxm->isOwned == false
        && (context->resourceType != NOT_A_SVR_RESOURCE)) // AND if SVR type
        {
            context->retVal = ACCESS_GRANTED;
        }
        // If not granted via DevOwner status and not a subowner,
        // then check if request is for a SVR and coming from rowner
        else if (IsRequestFromResourceOwner(context))
        {
            context->retVal = ACCESS_GRANTED;
        }
#ifdef MULTIPLE_OWNER
        //Then check if request from SubOwner
        else if(IsRequestFromSubOwner(context))
        {
            if(IsValidRequestFromSubOwner(context))
            {
                context->retVal = ACCESS_GRANTED;
            }
        }
#endif //MULTIPLE_OWNER
        // Else request is a "normal" request that must be tested against ACL
        else
        {
            OicUuid_t saveSubject = {.id={0}};
            bool isSubEmpty = IsRequestSubjectEmpty(context);

            ProcessAccessRequest(context);

            // If access not already granted, and requested subject != wildcard,
            // try looking for a wildcard ACE that grants access.
            if ((ACCESS_GRANTED != context->retVal) && \
              (false == IsWildCardSubject(&context->subject)))
            {
                //Saving subject for Amacl check
                memcpy(&saveSubject, &context->subject,sizeof(OicUuid_t));

                //Setting context subject to WILDCARD_SUBJECT_ID
                //TODO: change ProcessAccessRequest method signature to
                //ProcessAccessRequest(context, subject) so that context
                //subject is not tempered.
                memset(&context->subject, 0, sizeof(context->subject));
                memcpy(&context->subject, &WILDCARD_SUBJECT_ID,sizeof(OicUuid_t));
                ProcessAccessRequest(context); // TODO anonymous subj can result
                                               // in confusing err code return.
            }

            //No local ACE found for the request so checking Amacl resource
            if (ACCESS_GRANTED != context->retVal)
            {
                //If subject is not empty then restore the original subject
                //else keep the subject to WILDCARD_SUBJECT_ID
                if(!isSubEmpty)
                {
                    memcpy(&context->subject, &saveSubject, sizeof(OicUuid_t));
                }

                //FoundAmaclForRequest method checks for Amacl and fills up
                //context->amsMgrContext->amsDeviceId with the AMS deviceId
                //if Amacl was found for the requested resource.
                if(FoundAmaclForRequest(context))
                {
                    ProcessAMSRequest(context);
                }
            }
        }
    }
    else
    {
        context->retVal = ACCESS_DENIED_POLICY_ENGINE_ERROR;
    }

    // Capture retVal before resetting state for next request.
    retVal = context->retVal;

   if (!context->amsProcessing)
    {
        OIC_LOG(INFO, TAG, "Resetting PE context and PE State to AWAITING_REQUEST");
        SetPolicyEngineState(context, AWAITING_REQUEST);
    }

exit:
    return retVal;
}

OCStackResult InitPolicyEngine(PEContext_t *context)
{
    if(NULL == context)
    {
        return OC_STACK_ERROR;
    }

    context->amsMgrContext = (AmsMgrContext_t *)OICCalloc(1, sizeof(AmsMgrContext_t));
    if(NULL == context->amsMgrContext)
    {
        return OC_STACK_ERROR;
    }

    SetPolicyEngineState(context, AWAITING_REQUEST);
    return OC_STACK_OK;
}

void DeInitPolicyEngine(PEContext_t *context)
{
    if(NULL != context)
    {
        SetPolicyEngineState(context, STOPPED);
        OICFree(context->amsMgrContext);
    }
    return;
}
