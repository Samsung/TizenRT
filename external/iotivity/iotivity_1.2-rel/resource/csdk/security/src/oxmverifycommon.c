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

#include "ocstack.h"
#include "logger.h"
#include "base64.h"
#include "securevirtualresourcetypes.h"
#include "srmresourcestrings.h"
#include "cainterface.h"
#include "oxmverifycommon.h"

#define TAG "OIC_VERIFY_COMMON"

static VerifyOptionBitmask_t gVerifyOption = (DISPLAY_NUM | USER_CONFIRM);

static DisplayNumContext_t gDisplayNumContext = { .callback = NULL, .context = NULL };
static UserConfirmContext_t gUserConfirmContext = { .callback = NULL, .context = NULL };

void SetDisplayNumCB(void * ptr, DisplayNumCallback displayNumCB)
{
    OIC_LOG(DEBUG, TAG, "IN SetDisplayNumCB");
    if (NULL == displayNumCB)
    {
        OIC_LOG(ERROR, TAG, "Failed to set callback for displaying mutualVerifNum");
        return;
    }
    gDisplayNumContext.callback = displayNumCB;
    gDisplayNumContext.context = ptr;
    OIC_LOG(DEBUG, TAG, "OUT SetDisplayNumCB");
}

void* UnsetDisplayNumCB()
{
    OIC_LOG(DEBUG, TAG, "IN UnsetDisplayNumCB");
    void *prevctx = gDisplayNumContext.context;
    gDisplayNumContext.callback = NULL;
    gDisplayNumContext.context= NULL;
    OIC_LOG(DEBUG, TAG, "OUT UnsetDisplayNumCB");
    return prevctx;
}

void SetUserConfirmCB(void * ptr, UserConfirmCallback userConfirmCB)
{
    OIC_LOG(DEBUG, TAG, "IN SetUserConfirmCB");
    if (NULL == userConfirmCB)
    {
        OIC_LOG(ERROR, TAG, "Failed to set callback to confirm mutualVerifNum");
        return;
    }
    gUserConfirmContext.callback = userConfirmCB;
    gUserConfirmContext.context = ptr;
    OIC_LOG(DEBUG, TAG, "OUT SetUserConfirmCB");
}

void* UnsetUserConfirmCB()
{
    OIC_LOG(DEBUG, TAG, "IN UnsetUserConfirmCB");
    void *prevctx = gUserConfirmContext.context;
    gUserConfirmContext.callback = NULL;
    gUserConfirmContext.context = NULL;
    OIC_LOG(DEBUG, TAG, "OUT UnsetUserConfirmCB");
    return prevctx;
}

void SetVerifyOption(VerifyOptionBitmask_t verifyOption)
{
    OIC_LOG(DEBUG, TAG, "IN SetVerifyOption");
    gVerifyOption = verifyOption;
    OIC_LOG_V(DEBUG, TAG, "VerifyOption set to %d", (int) gVerifyOption);
    OIC_LOG(DEBUG, TAG, "OUT SetVerifyOption");
}

OCStackResult VerifyOwnershipTransfer(uint8_t mutualVerifNum [MUTUAL_VERIF_NUM_LEN],
                                    VerifyOptionBitmask_t verifyOption)
{
    OIC_LOG(DEBUG, TAG, "IN VerifyOwnershipTransfer");
    OIC_LOG_V(DEBUG, TAG, "gVerifyOption: %d", (int) gVerifyOption);
    verifyOption = (VerifyOptionBitmask_t)(verifyOption & gVerifyOption);
    if (verifyOption & DISPLAY_NUM)
    {
        if (!gDisplayNumContext.callback)
        {
            OIC_LOG(ERROR, TAG, "Callback for displaying verification PIN not registered");
            return OC_STACK_ERROR;
        }
        OIC_LOG(DEBUG, TAG, "calling displayNumCallback");
        if (OC_STACK_OK != gDisplayNumContext.callback(gDisplayNumContext.context, mutualVerifNum))
        {
            OIC_LOG(ERROR, TAG, "Failed to display verification PIN");
            return OC_STACK_ERROR;
        }
    }
    if (verifyOption & USER_CONFIRM)
    {
        if (!gUserConfirmContext.callback)
        {
            OIC_LOG(ERROR, TAG, "Callback to get user confirmation not registered");
            return OC_STACK_ERROR;
        }
        OIC_LOG(DEBUG, TAG, "calling userConfirmCallback");
        if (OC_STACK_OK != gUserConfirmContext.callback(gUserConfirmContext.context))
        {
            OIC_LOG(ERROR, TAG, "Failed to get user confirmation");
            return OC_STACK_USER_DENIED_REQ;
        }
    }
    OIC_LOG(DEBUG, TAG, "OUT VerifyOwnershipTransfer");
    return OC_STACK_OK;
}
