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

#ifndef _OXM_VERIFY_COMMON_
#define _OXM_VERIFY_COMMON_

#include "experimental/securevirtualresourcetypes.h"
#include "casecurityinterface.h"

#ifdef __cplusplus
 extern "C" {
#endif // __cplusplus

/** Verification Number Length */
#define MUTUAL_VERIF_NUM_LEN (3)

/** Label Length */
#define LABEL_LEN (30)

/** Verification Method Option definition
 * This type supports multiple bit set.
 */
 typedef enum VerifyOptionBitmask
{
    NOT_APPLICABLE              = 0x0,
    DISPLAY_NUM                 = (0x1 << 0),
    USER_CONFIRM                = (0x1 << 1)
} VerifyOptionBitmask_t;

/**
 * Function pointer to display verification PIN
 */
typedef OCStackResult (OC_CALL *DisplayNumCallback)(void * ctx, uint8_t verifNum[MUTUAL_VERIF_NUM_LEN]);

/**
 * Function pointer to get user confirmation
 */
typedef OCStackResult (OC_CALL *UserConfirmCallback)(void * ctx);

/**
 * Context for displaying verification PIN
 */
typedef struct DisplayNumContext
{
    DisplayNumCallback callback;
    void * context;
} DisplayNumContext_t;

/**
 * Context for getting user confirmation
 */
typedef struct UserConfirmContext
{
    UserConfirmCallback callback;
    void * context;
} UserConfirmContext_t;

/**
 * Set Callback for displaying verification PIN
 */
void OC_CALL SetDisplayNumCB(void * ptr, DisplayNumCallback displayNumCB);

/**
 * Unset Callback for displaying verification PIN
 */
void* OC_CALL UnsetDisplayNumCB();

/**
 * Set Callback for getting user confirmation
 */
void OC_CALL SetUserConfirmCB(void * ptr, UserConfirmCallback userConfirmCB);

/**
 * Unset Callback for getting user confirmation
 */
void* OC_CALL UnsetUserConfirmCB();

/**
 * Set verification method option.
 * The default is both display PIN and get user confirmation.
 */
void OC_CALL SetVerifyOption(VerifyOptionBitmask_t verifyOption);

/**
 * Call the Callback for Verifying Ownership Transfer process.
 */
OCStackResult OC_CALL VerifyOwnershipTransfer(uint8_t mutualVerifNum[MUTUAL_VERIF_NUM_LEN],
                                VerifyOptionBitmask_t verifyOption);

#ifdef __cplusplus
}
#endif

#endif

