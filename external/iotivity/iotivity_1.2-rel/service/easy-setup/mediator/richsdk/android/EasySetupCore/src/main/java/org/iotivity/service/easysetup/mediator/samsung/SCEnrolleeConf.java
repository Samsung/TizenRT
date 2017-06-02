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

package org.iotivity.service.easysetup.mediator.samsung;

import android.util.Log;

import org.iotivity.base.OcException;
import org.iotivity.base.OcRepresentation;
import org.iotivity.service.easysetup.mediator.ESConstants;
import org.iotivity.service.easysetup.mediator.EnrolleeConf;

import java.util.List;

/**
 * This class stores Enrollee's configuration including WiFi and Device configuration
 * including supported WiFi frequency and device name
 */
public class SCEnrolleeConf extends EnrolleeConf
{
    private static final String TAG = SCEnrolleeConf.class.getName();

    /**
     * Constructor
     *
     * @param rep received properties in a form of OcRepresentation
     */
    public SCEnrolleeConf(EnrolleeConf enrolleeConf) {
        super(enrolleeConf);
    }

    /**
     * Get Device type property in DevConf resource
     *
     * @return device type
     */
    public String getDeviceType()
    {
        List<OcRepresentation> children = mEasySetupRep.getChildren();

        for (OcRepresentation child : children) {
            if(child.getUri().indexOf(ESConstants.OC_RSRVD_ES_URI_DEVCONF) != -1)
            {
                try
                {
                    if(child.hasAttribute(ESConstants.OC_RSRVD_ES_VENDOR_DEVTYPE)) {
                        return (String) child.getValue(ESConstants.OC_RSRVD_ES_VENDOR_DEVTYPE);
                    }
                } catch (OcException e) {
                    Log.e(TAG, "getDeviceType is failed.");
                }
            }
        }
        return new String("");
    }

    /**
     * Get Device sub-type property in DevConf resource
     *
     * @return device sub-type
     */
    public String getDeviceSubType()
    {
        List<OcRepresentation> children = mEasySetupRep.getChildren();

        for (OcRepresentation child : children) {
            if(child.getUri().indexOf(ESConstants.OC_RSRVD_ES_URI_DEVCONF) != -1)
            {
                try
                {
                    if(child.hasAttribute(ESConstants.OC_RSRVD_ES_VENDOR_DEVSUBTYPE)) {
                        return (String) child.getValue(ESConstants.OC_RSRVD_ES_VENDOR_DEVSUBTYPE);
                    }
                } catch (OcException e) {
                    Log.e(TAG, "getDeviceType is failed.");
                }
            }
        }
        return new String("");
    }
}


