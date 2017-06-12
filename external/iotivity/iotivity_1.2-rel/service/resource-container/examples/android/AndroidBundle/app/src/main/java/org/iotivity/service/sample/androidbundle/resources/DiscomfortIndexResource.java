//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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

package org.iotivity.service.sample.androidbundle.resources;

import android.app.Notification;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import org.iotivity.service.resourcecontainer.BundleSoftSensorResource;
import org.iotivity.service.resourcecontainer.RcsResourceAttributes;
import org.iotivity.service.resourcecontainer.RcsValue;

import java.util.HashMap;
import java.util.Vector;

import org.iotivity.service.sample.androidbundle.DummyActivity;
import org.iotivity.service.sample.androidbundle.R;

public class DiscomfortIndexResource extends BundleSoftSensorResource {
    private static final String LOG_TAG = DiscomfortIndexResource.class.getSimpleName();

    public DiscomfortIndexResource(Context context){
        super(context);
        this.setResourceType("oic.r.discomfortindex");
        this.setName("discomfortIndex");
        m_mapInputData = new HashMap<String, RcsValue>();
    }

    @Override
    protected void initAttributes() {
        this.m_attributes.put("discomfortIndex", 0);
        this.m_attributes.put("humidity", 0);
        this.m_attributes.put("temperature", 0);
    }

    @Override
    protected void onUpdatedInputResource(String attributeName, Vector<RcsValue> values) {
        m_mapInputData.put(attributeName, values.get(0));
        executeLogic();
    }

    @Override
    protected void executeLogic() {
        if(m_mapInputData.get("humidity") != null && m_mapInputData.get("temperature") != null){
            double dDI = 0.0;
            Vector v = new Vector();

            int t = m_mapInputData.get("temperature").asInt();
            int h = m_mapInputData.get("humidity").asInt();
            double F = (9.0 * (double) t) / 5.0 + 32.0;

            // calculation of discomfortIndex
            dDI = F - (F - 58.0) * (double)((100 - h) * 55) / 10000.0;

            this.setAttribute("temperature", new RcsValue(t));
            this.setAttribute("humidity", new RcsValue(h));
            this.setAttribute("discomfortIndex", new RcsValue(dDI));

            setAttribute("discomfortIndex",new RcsValue(dDI), true ); // notify observers
            Log.i(LOG_TAG, "Discomfort Index" + dDI);
            showNotification(" " + dDI, this.m_context);
        }
        else{
            Log.i(LOG_TAG, "Discomfort Index input data - humidity:  " +
                    m_mapInputData.get("humidity") + " temp: " +
                    m_mapInputData.get("temperature") );
        }
    }

    @Override
    public void handleSetAttributesRequest(RcsResourceAttributes rcsResourceAttributes) {
        this.setAttributes(rcsResourceAttributes);
        executeLogic();
    }

    @Override
    public RcsResourceAttributes handleGetAttributesRequest() {
        return this.getAttributes();
    }

    private void showNotification(String eventtext, Context ctx) {
        // Set the icon, scrolling text and timestamp
        Log.i(LOG_TAG, "*************************************** ");
        Log.i(LOG_TAG, "*************************************** ");
        Log.i(LOG_TAG, "*************************************** ");
        Log.i(LOG_TAG, eventtext);
        Log.i(LOG_TAG, "*************************************** ");
        Log.i(LOG_TAG, "*************************************** ");
        Log.i(LOG_TAG, "*************************************** ");
    }
}
