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


package org.iotivity.service.resourcecontainer;

import java.util.HashMap;
import java.util.Set;
import java.util.Vector;
import android.content.Context;
import android.util.Log;

/**
 * Every resource bundle has to provide a bundle activator that can be called
 * by the resource container on bundle startup.
 */
public abstract class BundleSoftSensorResource extends BundleResource {
    protected HashMap<String, RcsValue> m_mapInputData;
    
    public BundleSoftSensorResource(Context context) {
        super(context);
    }
    /**
     * Initialize the internal attribute structure.
     */
    protected abstract void onUpdatedInputResource(String attributeName,  Vector<RcsValue> values);

    /**
     * SoftSensor logic. Has to be provided by the soft sensor developer.
     * This function will be executed if an input attribute is updated.
     *
     * @return void
     */
    protected abstract void executeLogic();
}