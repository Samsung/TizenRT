/*
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.oic.simulator.utils;

import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorManager;
import org.oic.simulator.server.SimulatorResource;
import org.oic.simulator.server.SimulatorSingleResource;

public class SampleSingleResource {

    private SimulatorSingleResource mResource;
    public String                   mName         = "test-singleresource";
    public String                   mUri          = "/test/singleresource";
    public String                   mResourceType = "test.singleresource";

    public boolean start() {
        try {
            mResource = (SimulatorSingleResource) SimulatorManager
                    .createResource(SimulatorResource.Type.SINGLE, mName, mUri,
                            mResourceType);
            mResource.start();
            return true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        return false;
    }

    public boolean stop() {
        try {
            mResource.stop();
            mResource = null;
            return true;
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        return false;
    }
}
