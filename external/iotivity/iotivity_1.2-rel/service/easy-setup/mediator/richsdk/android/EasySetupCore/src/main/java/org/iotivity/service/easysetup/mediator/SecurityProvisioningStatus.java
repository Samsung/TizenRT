/**
 * ***************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ****************************************************************
 */

package org.iotivity.service.easysetup.mediator;

import android.util.Log;

import org.iotivity.service.easysetup.mediator.enums.ESResult;

/**
 * This class indicates a result if a provisionSecurity API succeed or fails and
 * a target Enrollee's device ID
 */
public class SecurityProvisioningStatus
{
    private ESResult mResult;
    private String mDevUUID;

    /**
     * Constructor
     *
     * @param result a result of security provisioning
     * @param uuid a device ID of a target Enrollee
     */
    public SecurityProvisioningStatus(int result, String uuid)
    {
        mResult = ESResult.fromInt(result);
        mDevUUID = uuid;
    }

    /**
     * Get a Result of security provisioning
     *
     * @return ESResult a result of security provisioning
     */
    public ESResult getESResult()
    {
        return mResult;
    }

    /**
     * Get a device ID of a target Enrollee
     *
     * @return String a device ID of a target Enrollee
     */
    public String getDevUUID()
    {
        return mDevUUID;
    }
};
