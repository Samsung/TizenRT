/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
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
 *
 ******************************************************************/

package org.iotivity.service.sample.container;

import android.test.InstrumentationTestCase;
import android.util.Log;

import org.iotivity.service.resourcecontainer.RcsResourceContainer;

import java.util.HashMap;

public class ResourceContainerTest extends InstrumentationTestCase {
    private RcsResourceContainer resourceContainerInstance;
    private final String sdCardDir =
            "/data/local/tmp/org.iotivity.service.sample.resourcecontainer.test/files/";

    @Override
    public void setUp(){
        resourceContainerInstance = new RcsResourceContainer(getInstrumentation().getContext());
    }

    public void testStartContainer() {
        resourceContainerInstance.startContainer("");
        assertTrue(resourceContainerInstance.listBundles().size() == 0);
        resourceContainerInstance.stopContainer();
    }

    public void testAddConfig(){
        resourceContainerInstance.startContainer("");
        //resourceContainerInstance.addBundle("");
        assertTrue(resourceContainerInstance.listBundles().size() == 0);
        resourceContainerInstance.stopContainer();
    }

    public void testStartBundle(){
        resourceContainerInstance.startBundle("not.existing");
        assertEquals(0, resourceContainerInstance.listBundles().size());
    }

    public void testStopBundle(){
        resourceContainerInstance.startBundle("not.existing");
        assertEquals(0, resourceContainerInstance.listBundles().size());
    }

    public void testAddBundle(){
        resourceContainerInstance.startContainer(
                "");

        resourceContainerInstance.addBundle("oic.bundle.test", "",
                sdCardDir + "libSoftSensorBundle.so",
                "disensor", new HashMap<String, String>());
        resourceContainerInstance.startBundle("oic.bundle.test");
        resourceContainerInstance.stopContainer();
    }
}