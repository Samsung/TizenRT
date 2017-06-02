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

import org.iotivity.base.OcException;
import org.iotivity.base.OcRepresentation;
import org.iotivity.service.easysetup.mediator.ESConstants;
import org.iotivity.service.easysetup.mediator.enums.ESErrorCode;
import org.iotivity.service.easysetup.mediator.enums.ProvStatus;

/**
 * This class stores Enrollee's status including provisioning status and last error code.
 */
public class EnrolleeStatus
{
    private static final String TAG = EnrolleeStatus.class.getName();
    private OcRepresentation mRep = null;
    /**
     * Constructor
     *
     * @param rep received properties in a form of OcRepresentation
     *
     */
    public EnrolleeStatus(OcRepresentation rep)
    {
        mRep = rep;
    }

    /**
     * Get a provisioning status property of Enrollee.
     *
     * @return a provisioning status property of Enrollee
     */
    public ProvStatus getProvStatus()
    {
        try
        {
            if(mRep != null && mRep.hasAttribute(ESConstants.OC_RSRVD_ES_PROVSTATUS)) {
                int provStatus = mRep.getValue(ESConstants.OC_RSRVD_ES_PROVSTATUS);
                return ProvStatus.fromInt(provStatus);
            }
        } catch (OcException e) {
            Log.e(TAG, "getProvStatus is failed.");
        }
        return ProvStatus.fromInt(0);
    }

    /**
     * Get a last error code property of Enrollee.
     *
     * @return a last error code property of Enrollee.
     */
    public ESErrorCode getLastErrCode()
    {
        try
        {
            if(mRep != null && mRep.hasAttribute(ESConstants.OC_RSRVD_ES_LAST_ERRORCODE)) {
                int errCode = mRep.getValue(ESConstants.OC_RSRVD_ES_LAST_ERRORCODE);
                return ESErrorCode.fromInt(errCode);
            }
        } catch (OcException e) {
            Log.e(TAG, "getLastErrCode is failed.");
        }
        return ESErrorCode.fromInt(0);
    }
}


