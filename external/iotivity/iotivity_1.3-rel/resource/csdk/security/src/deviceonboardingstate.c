//******************************************************************
//
// Copyright 2017 Intel OpenSource Technology Center All Rights Reserved.
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

#include "deviceonboardingstate.h"
#include "srmutility.h"
#include "octypes.h"
#include "logger.h"
#include "srmresourcestrings.h"
#include "aclresource.h"
#include "amaclresource.h"
#include "credresource.h"
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
#include "crlresource.h"
#endif /* (__WITH_DTLS__) || (__WITH_TLS__) */
#include "doxmresource.h"
#include "pstatresource.h"
#include "resourcemanager.h"

#define TAG "OIC_SRM_DOS"

/**
 * @return true if changing from oldState to newState is valid transition.
 */
static bool IsValidStateTransition(OicSecDeviceOnboardingState_t oldState,
    OicSecDeviceOnboardingState_t newState)
{
    bool ret = false;

    switch (newState)
    {
        case DOS_RESET:
        ret = true;
        break;

        case DOS_RFNOP:
        if (DOS_RFPRO == oldState)
        {
            ret = true;
        }
        break;

        case DOS_RFOTM:
        if (DOS_RESET == oldState)
        {
            ret = true;
        }
        break;

        case DOS_RFPRO:
        if (DOS_RFNOP == oldState
            || DOS_RFOTM == oldState
            || DOS_SRESET == oldState)
        {
            ret = true;
        }
        break;

        case DOS_SRESET:
        if (DOS_RFNOP == oldState
            || DOS_RFPRO == oldState)
        {
            ret = true;
        }
        break;
    }

    OIC_LOG_V(INFO, TAG, "%s: returning %s.", __func__, ret?"true":"false");
    return ret;
}

/**
 * @return true if Device meets requirements to enter RFNOP DOS.
 */
static bool IsReadyToEnterRFNOP()
{
    bool ret = false;
    bool tempBool = false;
    OicUuid_t tempUuid = {.id={0}};

    // Note: pstat.dos.p asserted by DoStateChange(), so not checked here.

    // Verify doxm.owned == TRUE.
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmIsOwned(&tempBool), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, tempBool, WARNING);

    // Verify doxm.devowneruuid != nil UUID.
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmDevOwnerId(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    // Verify doxm.deviceuuid != nil UUID.
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmDeviceID(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    // Verify oxmsel was the actual OTM used (no-op: CTT will verify this during
    // certification testing, as it requires OBT cooperation to verify).

    // Verify pstat.isop == false (Server sets isop on entry)
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetPstatIsop(&tempBool), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !tempBool, WARNING);

    // Verify implemented SVRs with rowneruuid Property have non-Nil rowneruuid
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetAclRownerId(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetCredRownerId(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmRownerId(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetPstatRownerId(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    // Verify each rowneruuid, devowneruuid has a corresponding /cred entry
    // TODO [IOT-2023]

    ret = true;

exit:
    OIC_LOG_V(DEBUG, TAG, "%s: returning %s.", __func__, ret?"true":"false");
    return ret;
}

/**
 * @return true if Device meets requirements to enter RFOTM DOS.
 */
static bool IsReadyToEnterRFOTM()
{
    bool ret = false;
    bool tempBool = false;
    OicUuid_t tempUuid = {.id={0}};

    // Note: pstat.dos.p asserted by DoStateChange(), so not checked here.

    // Verify doxm.owned == FALSE.
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmIsOwned(&tempBool), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !tempBool, WARNING);

    // Verify doxm.devowneruuid == nil UUID.
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmDevOwnerId(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, IsNilUuid(&tempUuid), WARNING);

    // Check and log whether doxm.deviceuuid == nil UUID ("may" reqt not "shall")
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmDeviceID(&tempUuid), ERROR);
    if (!IsNilUuid(&tempUuid))
    {
        OIC_LOG_V(INFO, TAG, "%s: doxm.deviceuuid != Nil UUID... allowed but noted.",
            __func__);
    }

    ret = true;

exit:
    OIC_LOG_V(DEBUG, TAG, "%s: returning %s.", __func__, ret?"true":"false");
    return ret;
}

/**
 * @return true if Device meets requirements to enter RFPRO DOS.
 */
static bool IsReadyToEnterRFPRO()
{
    bool ret = false;
    bool tempBool = false;
    OicUuid_t tempUuid = {.id={0}};

    // Note: pstat.dos.p asserted by DoStateChange(), so not checked here.

    // Verify doxm.owned == TRUE.
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmIsOwned(&tempBool), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, tempBool, WARNING);

    // Verify doxm.devowneruuid != nil UUID.
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmDevOwnerId(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    // Verify doxm.deviceuuid != nil UUID.
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmDeviceID(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    // doxm.sct and doxm.oxmsel retain previous values (checked by CTT)

    // Verify implemented SVRs with rowneruuid Property have non-Nil rowneruuid
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetAclRownerId(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetCredRownerId(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmRownerId(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetPstatRownerId(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    // Verify each rowneruuid, devowneruuid has a corresponding /cred entry
    // TODO [IOT-2023]

    ret = true;

exit:
    OIC_LOG_V(DEBUG, TAG, "%s: returning %s.", __func__, ret?"true":"false");
    return ret;
}

/**
 * @return true if Device meets requirements to set pstat.dos.s = SRESET.
 */
static bool IsReadyToEnterSRESET()
{
    bool ret = false;
    bool tempBool = false;
    OicUuid_t tempUuid = {.id={0}};

    // Note: pstat.dos.p set by DoStateChange(), so not checked here.

    // TODO [IOT-2023]: sanity check SVRs (optional)

    // Verify doxm.owned == TRUE.
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmIsOwned(&tempBool), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, tempBool, WARNING);

    // Verify doxm.devowneruuid != nil UUID.
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmDevOwnerId(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    // Verify doxm.deviceuuid != nil UUID.
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDoxmDeviceID(&tempUuid), ERROR);
    VERIFY_TRUE_OR_EXIT(TAG, !IsNilUuid(&tempUuid), WARNING);

    // doxm.sct and doxm.oxmsel retain previous values (checked by CTT)

    ret = true;

exit:
    OIC_LOG_V(DEBUG, TAG, "%s: returning %s.", __func__, ret?"true":"false");
    return ret;
}

/**
 * Generic ops performed on entering most states, coalesced to avoid repeat code.
 */
static bool EnterStateGeneric(bool isop,
                              bool cmReset,
                              bool cmTakeOwner,
                              bool tmReset,
                              bool tmTakeOwner,
                              OicSecDeviceOnboardingState_t state)
{
    bool ret = false;
    OicSecDpm_t cm = 0;
    OicSecDpm_t tm = 0;

    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetPstatCm(&cm), ERROR);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetPstatTm(&tm), ERROR);

    // Set pstat.isop
    VERIFY_SUCCESS(TAG, OC_STACK_OK == SetPstatIsop(isop), ERROR);

    // Set pstat.cm
    if (cmReset)
    {
        cm |= RESET; // set RESET bit
    }
    else
    {
        cm &= ~RESET; // clear RESET bit
    }
    if (cmTakeOwner)
    {
        cm |= TAKE_OWNER; // set TAKE_OWNER bit
    }
    else
    {
        cm &= ~TAKE_OWNER; // clear TAKE_OWNER bit
    }
    VERIFY_SUCCESS(TAG, OC_STACK_OK == SetPstatCm(cm), ERROR);

    // Set pstat.tm
    if (tmReset)
    {
        tm |= RESET; // set RESET bit
    }
    else
    {
        tm &= ~RESET; // clear RESET bit
    }
    if (tmTakeOwner)
    {
        tm |= TAKE_OWNER; // set TAKE_OWNER bit
    }
    else
    {
        tm &= ~TAKE_OWNER; // clear TAKE_OWNER bit
    }
    VERIFY_SUCCESS(TAG, OC_STACK_OK == SetPstatTm(tm), ERROR);

    // Set pstat.dos.s
    VERIFY_SUCCESS(TAG, OC_STACK_OK == SetPstatDosS(state), ERROR);

    ret = true;

exit:
    OIC_LOG_V(DEBUG, TAG, "%s: returning %s.", __func__, ret?"true":"false");
    return ret;
}

/**
 * Enter RFNOP state and set all Server-controlled SVR Property values.
 */
static bool EnterRFNOP()
{
    bool ret = false;

    // Set pstat.isop = TRUE
    // Unset pstat.cm RESET and TAKE_OWNER bits
    // Unset pstat.tm RESET and TAKE_OWNER bits
    // Set pstat.dos to RFNOP
    ret = EnterStateGeneric(true, false, false, false, false, DOS_RFNOP);

    OIC_LOG_V(DEBUG, TAG, "%s: returning %s.", __func__, ret?"true":"false");
    return ret;
}

/**
 * Enter RFOTM state and set all Server-controlled SVR Property values.
 */
static bool EnterRFOTM()
{
    bool ret = false;

    // Set pstat.isop = FALSE
    // Unset pstat.cm RESET bit, and set TAKE_OWNER bit
    // Unset pstat.tm RESET and TAKE_OWNER bits
    // Set pstat.dos to RFOTM
    ret = EnterStateGeneric(false, false, true, false, false, DOS_RFOTM);

    OIC_LOG_V(DEBUG, TAG, "%s: returning %s.", __func__, ret?"true":"false");
    return ret;
}

/**
 * Enter RFPRO state and set all Server-controlled SVR Property values.
 */
static bool EnterRFPRO()
{
    bool ret = false;

    // Set pstat.isop = FALSE
    // Unset pstat.cm RESET and TAKE_OWNER bits
    // Unset pstat.tm RESET and TAKE_OWNER bits
    // Set pstat.dos to RFPRO
    ret = EnterStateGeneric(false, false, false, false, false, DOS_RFPRO);

    OIC_LOG_V(DEBUG, TAG, "%s: returning %s.", __func__, ret?"true":"false");
    return ret;
}

/**
 * Enter RESET state and set all Server-controlled SVR Property values.
 */
static bool EnterRESET()
{
    bool ret = false;

    // Restore Mfr Defaults
    // "Mfr Defaults" is defined by manufacturer.  It could be "failsafe"
    // SVRs (e.g. the hard-coded SVRs in IoTivity) or it could be a backup
    // copy of the initally-provisioned SVRs (e.g. the ResetSecureResourceInPS
    // function in IoTivity).
    // TODO [IOT-2633]: VERIFY_SUCCESS(TAG, OC_STACK_OK == ResetSecureResources(), ERROR);

    // Set doxm.deviceuuid = Mfr Default (handled above)
    // Set doxm.sct = Mfr Default ("")
    // Set doxm.oxmsel = Mfr Default ("")
    // Set pstat.om = Mfr Default ("")
    // Set pstat.sm = Mfr Default ("")
    // Assert acl2, amacl, sacl, cred arrays = mfr defaults ("")

    // Set doxm.owned = FALSE
    VERIFY_SUCCESS(TAG, OC_STACK_OK == SetDoxmIsOwned(false), ERROR);

    // Set doxm.devowneruuid = Nil UUID
    VERIFY_SUCCESS(TAG, OC_STACK_OK == SetDoxmDevOwnerId(&THE_NIL_UUID), ERROR);

    // Set acl, doxm, cred and pstat rowneruuids = Nil UUID
    VERIFY_SUCCESS(TAG, OC_STACK_OK == SetAclRownerId(&THE_NIL_UUID), ERROR);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == SetDoxmRownerId(&THE_NIL_UUID), ERROR);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == SetPstatRownerId(&THE_NIL_UUID), ERROR);

    // Set pstat.isop = FALSE
    // Set pstat.cm RESET and unset TAKE_OWNER
    // Unset pstat.tm and set TAKE_OWNER
    // Set pstat.dos.s to RESET
    VERIFY_SUCCESS(TAG,
        EnterStateGeneric(false, true, false, false, true, DOS_RESET),
        ERROR);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    // Enable Anon DH cipher suite if appropriate
    bool isAnonEnabled = false;
    VERIFY_SUCCESS(TAG,
        OC_STACK_OK == EnableAnonCipherSuiteIfUnOwnedAndJustWorksSelected(&isAnonEnabled),
        ERROR);
    OIC_LOG_V(INFO, TAG, "%s: Anon Ciphersuite %sENABLED.", __func__,
        isAnonEnabled ? "" : "NOT ");
#endif // __WITH_DTLS__ or __WITH_TLS__

    ret = true;

exit:
    OIC_LOG_V(DEBUG, TAG, "%s: returning %s.", __func__, ret?"true":"false");
    return ret;
}

/**
 * Enter SRESET state and set all Server-controlled SVR Property values.
 */
static bool EnterSRESET()
{
    bool ret = false;

    // Set pstat.isop = FALSE
    // Set pstat.cm RESET and unset TAKE_OWNER
    // Unset pstat.tm and unset TAKE_OWNER
    // Set pstat.dos.s to RESET
    VERIFY_SUCCESS(TAG,
        EnterStateGeneric(false, true, false, false, false, DOS_SRESET),
        ERROR);

    ret = true;

exit:
    OIC_LOG_V(DEBUG, TAG, "%s: returning %s.", __func__, ret?"true":"false");
    return ret;
}

/**
 * Set pstat.dos.p (pending) to true, then verify device is ready to perform
 * the state change.  If so, perform the state change.  Finally, set
 * pstat.dos.p (pending) to false.
 * @return  OC_STACK_OK if successful change to newState
 *          OC_STACK_FORBIDDEN_REQ if state change preconditions not met
 *          OC_STACK_INTERNAL_SERVER_ERROR if SVRs left in potentially unstable state
 */
static OCStackResult DoStateChange(OicSecDeviceOnboardingState_t newState)
{
    OCStackResult ret = OC_STACK_INTERNAL_SERVER_ERROR;

    switch (newState)
    {
        case DOS_RESET:
        // No preconditions other than setting dos.p = true, which is done above
        if (EnterRESET())
        {
            // if RESET succeeds, immediately enter RFOTM
            if (IsReadyToEnterRFOTM())
            {
                if (EnterRFOTM())
                {
                    ret = OC_STACK_OK;
                }
                else
                {
                    ret = OC_STACK_INTERNAL_SERVER_ERROR;
                }
            }
            else
            {
                ret = OC_STACK_FORBIDDEN_REQ;
            }
        }
        else
        {
            ret = OC_STACK_INTERNAL_SERVER_ERROR;
        }
        break;

        case DOS_RFNOP:
        if (IsReadyToEnterRFNOP())
        {
            if (EnterRFNOP())
            {
                ret = OC_STACK_OK;
            }
            else
            {
                ret = OC_STACK_INTERNAL_SERVER_ERROR;
            }
        }
        else
        {
            ret = OC_STACK_FORBIDDEN_REQ;
        }
        break;

        case DOS_RFOTM:
        if (IsReadyToEnterRFOTM())
        {
            if (EnterRFOTM())
            {
                ret = OC_STACK_OK;
            }
            else
            {
                ret = OC_STACK_INTERNAL_SERVER_ERROR;
            }
        }
        else
        {
            ret = OC_STACK_FORBIDDEN_REQ;
        }
        break;

        case DOS_RFPRO:
        if (IsReadyToEnterRFPRO())
        {
            if (EnterRFPRO())
            {
                ret = OC_STACK_OK;
            }
            else
            {
                ret = OC_STACK_INTERNAL_SERVER_ERROR;
            }
        }
        else
        {
            ret = OC_STACK_FORBIDDEN_REQ;
        }
        break;

        case DOS_SRESET:
        if (IsReadyToEnterSRESET())
        {
            if (EnterSRESET())
            {
                ret = OC_STACK_OK;
            }
            else
            {
                ret = OC_STACK_INTERNAL_SERVER_ERROR;
            }
        }
        else
        {
            ret = OC_STACK_FORBIDDEN_REQ;
        }
        break;
    }

    OIC_LOG_V(DEBUG, TAG, "%s: returning %d.", __func__, ret);
    return ret;
}

OCStackResult GetDos(OicSecDostype_t *dos)
{
    if (dos)
    {
        VERIFY_SUCCESS(TAG, OC_STACK_OK == GetPstatDosS(&(dos->state)), ERROR);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == GetPstatDosP(&(dos->pending)), ERROR);
        return OC_STACK_OK;
    }
exit:
    return OC_STACK_ERROR;
}

OCStackResult SetDosState(const OicSecDeviceOnboardingState_t desiredState)
{
    OIC_LOG_V(INFO, TAG, "%s called for state %d.", __func__, desiredState);

    OCStackResult ret = OC_STACK_ERROR;
    bool pending = false;

    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetPstatDosP(&pending), ERROR);

    if (!pending)
    {

        VERIFY_SUCCESS(TAG, OC_STACK_OK == SetPstatDosP(true), ERROR);

        OicSecDeviceOnboardingState_t oldState = DOS_RESET;
        VERIFY_SUCCESS(TAG, OC_STACK_OK == GetPstatDosS(&oldState), ERROR);
        if (IsValidStateTransition(oldState, desiredState))
        {
            OCStackResult stateChangeResult = DoStateChange(desiredState);
            switch (stateChangeResult)
            {
                case OC_STACK_OK:
                OIC_LOG_V(INFO, TAG, "%s: DOS state changed SUCCESSFULLY from %d to %d.",
                    __func__, oldState, desiredState);
                ret = OC_STACK_OK;
                break;

                case OC_STACK_FORBIDDEN_REQ:
                OIC_LOG_V(WARNING, TAG, "%s: DOS state change change from %d to %d NOT ALLOWED.",
                    __func__, oldState, desiredState);
                ret = OC_STACK_FORBIDDEN_REQ;
                break;

                case OC_STACK_INTERNAL_SERVER_ERROR:
                default:
                OIC_LOG_V(ERROR, TAG, "%s: DOS state change change from %d to %d FAILED."
                    " Internal error - SVRs may be in bad state.",
                    __func__, oldState, desiredState);
                ret = OC_STACK_INTERNAL_SERVER_ERROR;
                break;
            }
        }
        else
        {
            OIC_LOG_V(INFO, TAG, "%s: Invalid transition; cannot go from %d to %d.", \
                __func__, oldState, desiredState);
            ret = OC_STACK_FORBIDDEN_REQ;
        }

        VERIFY_SUCCESS(TAG, OC_STACK_OK == SetPstatDosP(false), ERROR);

    }
    else
    {
        OIC_LOG_V(WARNING, TAG, "%s: cannot set pstat->dos.s, change \
            already pending.", __func__);
        ret = OC_STACK_FORBIDDEN_REQ;
    }

    // TODO [IOT-2023] if OC_STACK_OK, update all SVRs in Persistent Storage?

    return ret;

exit:
    return OC_STACK_INTERNAL_SERVER_ERROR;
}