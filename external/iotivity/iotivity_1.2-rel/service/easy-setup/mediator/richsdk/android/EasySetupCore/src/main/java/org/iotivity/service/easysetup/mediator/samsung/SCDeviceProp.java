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

import java.util.ArrayList;
import java.util.Arrays;

import android.util.Log;

import org.iotivity.service.easysetup.mediator.DeviceProp;
import org.iotivity.service.easysetup.mediator.ESConstants;
import org.iotivity.service.easysetup.mediator.enums.WIFI_AUTHTYPE;
import org.iotivity.service.easysetup.mediator.enums.WIFI_ENCTYPE;

import org.iotivity.base.OcException;
import org.iotivity.base.OcRepresentation;

/**
 * This class contains device properties to be delivered to Enrollee
 */
public class SCDeviceProp extends DeviceProp {
    private static final String TAG = SCDeviceProp.class.getName();
    final int INVALID_DISCOVERY_CHANNEL = -1;

    /**
     * Set Channel of Enroller for fast discover
     */
    public void setDiscoveryChannel(int discoveryChannel)
    {
        try
        {
            mRep.setValue(ESConstants.OC_RSRVD_ES_VENDOR_DISCOVERYCHANNEL, discoveryChannel);
        } catch (OcException e) {
            Log.e(TAG, "setDiscoveryChannel is failed.");
        }
    }

    /**
     * Get Channel of Enroller for fast discover
     *
     * @return int Enroller's DiscoveryChannel
     */
    public int getDiscoveryChannel()
    {
        try {
            if(mRep.hasAttribute(ESConstants.OC_RSRVD_ES_VENDOR_DISCOVERYCHANNEL))
                return mRep.getValue(ESConstants.OC_RSRVD_ES_VENDOR_DISCOVERYCHANNEL);
        } catch (OcException e) {
            Log.e(TAG, "getDiscoveryChannel is failed.");
        }
        return INVALID_DISCOVERY_CHANNEL;
    }

     /**
     * Set samsung-specific location property to be delivered to Enrollee
     *
     * @param locations a set of location information
     */
    public void setSCLocation(ArrayList<String> locations)
    {
        try
        {
            mRep.setValue(ESConstants.SC_RSRVD_ES_VENDOR_LOCATION, locations.toArray(new
            String[locations.size()]));
        } catch (OcException e) {
            Log.e(TAG, "setSCLocation is failed.");
        }
    }
}
