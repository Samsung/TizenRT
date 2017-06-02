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

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

import org.iotivity.service.resourcecontainer.BundleResource;
import org.iotivity.service.resourcecontainer.BundleResource;
import org.iotivity.service.resourcecontainer.RcsResourceAttributes;
import org.iotivity.service.resourcecontainer.RcsValue;

public class GyroscopeResource extends BundleResource implements SensorEventListener {
    private static final String LOG_TAG = GyroscopeResource.class.getSimpleName();
    private final SensorManager mSensorManager;
    private final Sensor humiditySensor;

    public GyroscopeResource(Context context){
        super(context);
        this.setResourceType("oic.r.gyroscope");
        this.setName("gyroscopeSensor");
        mSensorManager = (SensorManager) context.getApplicationContext().
                getSystemService(Context.SENSOR_SERVICE);
        humiditySensor = mSensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);
        mSensorManager.registerListener(this, humiditySensor, SensorManager.SENSOR_DELAY_NORMAL);
        Log.d(LOG_TAG, "Created new gyroscope instance");
    }

    @Override
    protected void initAttributes() {
        this.m_attributes.put("x", 0d);
        this.m_attributes.put("y", 0d);
        this.m_attributes.put("z", 0d);
    }

    @Override
    public void handleSetAttributesRequest(RcsResourceAttributes attrs) {
        Log.i(LOG_TAG, "Set Attributes called with ");
        for(String key: attrs.keySet()){
            Log.i(LOG_TAG, " " + key + ": " + attrs.get(key));
        }
    }

    @Override
    public RcsResourceAttributes handleGetAttributesRequest() {
        Log.i(LOG_TAG, "Get Attributes called");
        Log.i(LOG_TAG, "Returning: ");
        for(String key:m_attributes.keySet()){
            Log.i(LOG_TAG, " " + key + ": " + m_attributes.get(key));
        }
        return this.m_attributes;
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        Log.i(LOG_TAG, "Sensor event " + sensorEvent.values[0] + ", " + sensorEvent.values[1]
            + ", " + sensorEvent.values[2]);
        setAttribute("x", new RcsValue( (int) (sensorEvent.values[0])), true);
        setAttribute("y", new RcsValue( (int) (sensorEvent.values[1]) ) , true);
        setAttribute("z", new RcsValue( (int) (sensorEvent.values[2]) ) , true);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }
}
