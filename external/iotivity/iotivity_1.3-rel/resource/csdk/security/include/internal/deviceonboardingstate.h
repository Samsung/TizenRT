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

#ifndef DEVICEONBOARDINGSTATE_H_
#define DEVICEONBOARDINGSTATE_H_

#include "octypes.h"
#include "experimental/securevirtualresourcetypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get the current Device Onboarding State (pstat.dos).
 *
 * @param[out] dos Pointer to contain the values of current state (pstat.dos)
 *
 * @return  ::OC_STACK_OK If no errors and dos is filled in successfully.
            ::OC_STACK_ERROR If dos is NOT filled in successfully.
 */
OCStackResult GetDos(OicSecDostype_t *dos);

/**
 * Set the Device Onboarding State (pstat.dos) to a new state.
 *
 * Note that all requirements for entering new state should typically be set
 * prior to making this call, typically by the Onboarding Tool (OBT).  The
 * exceptions are Properties that are set by the Server itself.  See
 * OCF Security Specification, Security Resources chapters for details.
 *
 * @param[in] state Value of new desired state (pstat.dos.state)
 *
 * @return  ::OC_STACK_OK if successful change to newState
 *          ::OC_STACK_FORBIDDEN_REQ if state change preconditions not met
 *          ::OC_STACK_INTERNAL_SERVER_ERROR if SVRs left in potentially unstable state
 */
OCStackResult SetDosState(const OicSecDeviceOnboardingState_t state);

#ifdef __cplusplus
}
#endif

#endif /* DEVICEONBOARDINGSTATE_H_ */
