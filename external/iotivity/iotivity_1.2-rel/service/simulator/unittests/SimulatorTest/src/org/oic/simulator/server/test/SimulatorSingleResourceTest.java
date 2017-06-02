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

package org.oic.simulator.server.test;

import java.util.Map;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import org.oic.simulator.AttributeValue;
import org.oic.simulator.BooleanProperty;
import org.oic.simulator.DoubleProperty;
import org.oic.simulator.IntegerProperty;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorManager;
import org.oic.simulator.SimulatorResourceAttribute;
import org.oic.simulator.StringProperty;
import org.oic.simulator.server.SimulatorResource;
import org.oic.simulator.server.SimulatorResource.AutoUpdateListener;
import org.oic.simulator.server.SimulatorSingleResource;
import org.oic.simulator.test.ExceptionType;
import org.oic.simulator.utils.ObjectHolder;

import junit.framework.TestCase;

public class SimulatorSingleResourceTest extends TestCase {
    private static final String     SINGLE_RES_RAML = "./ramls/oic.r.light.raml";
    private static final String     INT_KEY         = "Integer";
    private static final String     DOUBLE_KEY      = "Double";
    private static final String     BOOL_KEY        = "Boolean";
    private static final String     STRING_KEY      = "String";
    private SimulatorSingleResource singleResource  = null;

    protected void setUp() throws Exception {
        super.setUp();
        singleResource = (SimulatorSingleResource) SimulatorManager
                .createResource(SimulatorResource.Type.SINGLE, "test-resource",
                        "/test/resource", "test.resource");
    }

    protected void tearDown() throws Exception {
        super.tearDown();
        singleResource = null;
    }

    public void testGetAttribute_P01() {
        SimulatorResourceAttribute attribute = null;

        try {
            IntegerProperty property = new IntegerProperty.Builder().build();
            SimulatorResourceAttribute intAttribute = new SimulatorResourceAttribute(
                    INT_KEY, new AttributeValue(2), property);

            assertTrue(singleResource.addAttribute(intAttribute));
            attribute = singleResource.getAttribute(INT_KEY);
            assertTrue(singleResource.removeAttribute(INT_KEY));
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(attribute);
    }

    public void testGetAttribute_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            singleResource.getAttribute(null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testGetAttribute_N02() {
        SimulatorResourceAttribute att = null;

        try {
            att = singleResource.getAttribute("PQRS");
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNull(att);
    }

    public void testAddAttributeInteger_P01() {
        int result = -1;

        try {
            IntegerProperty property = new IntegerProperty.Builder().build();
            SimulatorResourceAttribute intAttribute = new SimulatorResourceAttribute(
                    INT_KEY, new AttributeValue(2), property);

            assertTrue(singleResource.addAttribute(intAttribute));

            SimulatorResourceAttribute attribute = singleResource
                    .getAttribute(INT_KEY);
            assertNotNull(attribute);

            result = ((Integer) attribute.value().get()).intValue();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(2, result);
    }

    public void testAddAttributeDouble_P01() {
        double result = 0.0;

        try {
            DoubleProperty property = new DoubleProperty.Builder().build();
            SimulatorResourceAttribute doubleAttribute = new SimulatorResourceAttribute(
                    DOUBLE_KEY, new AttributeValue(4.0), property);

            assertTrue(singleResource.addAttribute(doubleAttribute));

            SimulatorResourceAttribute attribute = singleResource
                    .getAttribute(DOUBLE_KEY);
            assertNotNull(attribute);

            result = ((Double) attribute.value().get()).doubleValue();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(4.0, result);
    }

    public void testAddAttributeBoolean_P01() {
        boolean result = false;

        try {
            BooleanProperty property = new BooleanProperty.Builder().build();
            SimulatorResourceAttribute boolAttribute = new SimulatorResourceAttribute(
                    BOOL_KEY, new AttributeValue(true), property);

            assertTrue(singleResource.addAttribute(boolAttribute));

            SimulatorResourceAttribute attribute = singleResource
                    .getAttribute(BOOL_KEY);
            assertNotNull(attribute);

            result = ((Boolean) attribute.value().get()).booleanValue();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(true, result);
    }

    public void testAddAttributeString_P01() {
        String result = null;

        try {
            StringProperty property = new StringProperty.Builder().build();
            SimulatorResourceAttribute stringAttribute = new SimulatorResourceAttribute(
                    STRING_KEY, new AttributeValue("string-value"), property);

            assertTrue(singleResource.addAttribute(stringAttribute));

            SimulatorResourceAttribute attribute = singleResource
                    .getAttribute(STRING_KEY);
            assertNotNull(attribute);

            result = (String) attribute.value().get();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals("string-value", result);
    }

    public void testRemoveAttribute_P01() {
        SimulatorResourceAttribute result = null;

        try {
            StringProperty property = new StringProperty.Builder().build();
            SimulatorResourceAttribute stringAttribute = new SimulatorResourceAttribute(
                    STRING_KEY, new AttributeValue("friday"), property);

            assertTrue(singleResource.addAttribute(stringAttribute));
            assertTrue(singleResource.removeAttribute(STRING_KEY));

            result = singleResource.getAttribute(STRING_KEY);
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNull(result);
    }

    public void testRemoveAttribute_N02() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            StringProperty property = new StringProperty.Builder().build();
            SimulatorResourceAttribute stringAttribute = new SimulatorResourceAttribute(
                    STRING_KEY, new AttributeValue("friday"), property);

            assertTrue(singleResource.addAttribute(stringAttribute));
            singleResource.removeAttribute(null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testRemoveAttribute_N03() {
        boolean result = false;
        try {
            result = singleResource.removeAttribute("");
        } catch (InvalidArgsException e) {
        } catch (SimulatorException e) {
        }

        assertFalse(result);
    }

    public void testUpdateAttributeInteger_P01() {
        int result = -1;

        try {
            IntegerProperty property = new IntegerProperty.Builder().build();
            SimulatorResourceAttribute intAttribute = new SimulatorResourceAttribute(
                    INT_KEY, new AttributeValue(10), property);

            assertTrue(singleResource.addAttribute(intAttribute));
            assertTrue(singleResource.updateAttribute(INT_KEY,
                    new AttributeValue(12)));

            SimulatorResourceAttribute attribute = singleResource
                    .getAttribute(INT_KEY);
            assertNotNull(attribute);

            result = ((Integer) attribute.value().get()).intValue();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(12, result);
    }

    public void testUpdateAttributeDouble_P01() {
        double result = 0.0;

        try {
            DoubleProperty property = new DoubleProperty.Builder().build();
            SimulatorResourceAttribute doubleAttribute = new SimulatorResourceAttribute(
                    DOUBLE_KEY, new AttributeValue(22.0), property);

            assertTrue(singleResource.addAttribute(doubleAttribute));
            assertTrue(singleResource.updateAttribute(DOUBLE_KEY,
                    new AttributeValue(25.3)));

            SimulatorResourceAttribute attribute = singleResource
                    .getAttribute(DOUBLE_KEY);
            assertNotNull(attribute);

            result = ((Double) attribute.value().get()).doubleValue();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(25.3, result);
    }

    public void testUpdateAttributeBoolean_P01() {
        boolean result = true;

        try {
            BooleanProperty property = new BooleanProperty.Builder().build();
            SimulatorResourceAttribute boolAttribute = new SimulatorResourceAttribute(
                    BOOL_KEY, new AttributeValue(true), property);

            assertTrue(singleResource.addAttribute(boolAttribute));
            assertTrue(singleResource.updateAttribute(BOOL_KEY,
                    new AttributeValue(false)));

            SimulatorResourceAttribute attribute = singleResource
                    .getAttribute(BOOL_KEY);
            assertNotNull(attribute);

            result = ((Boolean) attribute.value().get()).booleanValue();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(false, result);
    }

    public void testupdateAttributeString_P01() {
        String result = null;

        try {
            StringProperty property = new StringProperty.Builder().build();
            SimulatorResourceAttribute stringAttribute = new SimulatorResourceAttribute(
                    STRING_KEY, new AttributeValue("old-value"), property);

            assertTrue(singleResource.addAttribute(stringAttribute));
            assertTrue(singleResource.updateAttribute(STRING_KEY,
                    new AttributeValue("new-value")));

            SimulatorResourceAttribute attribute = singleResource
                    .getAttribute(STRING_KEY);
            assertNotNull(attribute);

            result = (String) attribute.value().get();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals("new-value", result);
    }

    public void testStartResourceAutomation_P01() {
        SimulatorSingleResource resource = createResourceFromRAML();
        if (null == resource)
            return;

        CountDownLatch lockObject = new CountDownLatch(1);
        ObjectHolder<AutoUpdateInfo> autoUpdateHolder = new ObjectHolder<>();
        AutoUpdateCompleteListener automationListener = new AutoUpdateCompleteListener(
                lockObject, autoUpdateHolder);
        int id = -1;

        try {
            resource.start();
            id = resource.startResourceUpdation(
                    SimulatorResource.AutoUpdateType.REPEAT, 1000,
                    automationListener);

            try {
                lockObject.await(3, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            resource.stopUpdation(id);
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        try {
            resource.stop();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(autoUpdateHolder.get());
        assertEquals(id, autoUpdateHolder.get().getId());
    }

    public void testStartResourceAutomation_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            singleResource.startResourceUpdation(
                    SimulatorResource.AutoUpdateType.ONE_TIME, 500, null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testStartAttributeAutomation_P01() {
        SimulatorSingleResource resource = createResourceFromRAML();
        if (null == resource)
            return;

        String attributeName = null;
        try {
            for (Map.Entry<String, AttributeValue> entry : resource
                    .getResourceModel().get().entrySet()) {
                attributeName = entry.getKey();
                if (null != attributeName && !attributeName.isEmpty()) {
                    break;
                }
            }
        } catch (SimulatorException e1) {
            e1.printStackTrace();
        }

        if (null == attributeName)
            return;

        CountDownLatch lockObject = new CountDownLatch(1);
        ObjectHolder<AutoUpdateInfo> autoUpdateHolder = new ObjectHolder<>();
        AutoUpdateCompleteListener automationListener = new AutoUpdateCompleteListener(
                lockObject, autoUpdateHolder);
        int id = -1;

        try {
            resource.start();
            id = resource.startAttributeUpdation(attributeName,
                    SimulatorResource.AutoUpdateType.REPEAT, 1000,
                    automationListener);

            try {
                lockObject.await(3, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            resource.stopUpdation(id);
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        try {
            resource.stop();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(autoUpdateHolder.get());
        assertEquals(id, autoUpdateHolder.get().getId());
    }

    public void testStartAttributeAutomation_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            singleResource.startAttributeUpdation("intensity",
                    SimulatorResource.AutoUpdateType.ONE_TIME, 1000, null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testStopUpdation_P01() {
        SimulatorSingleResource resource = createResourceFromRAML();
        if (null == resource)
            return;

        CountDownLatch lockObject = new CountDownLatch(1);
        ObjectHolder<AutoUpdateInfo> autoUpdateHolder = new ObjectHolder<>();
        AutoUpdateCompleteListener automationListener = new AutoUpdateCompleteListener(
                lockObject, autoUpdateHolder);
        boolean result = false;
        try {
            resource.start();
            int id = resource.startResourceUpdation(
                    SimulatorResource.AutoUpdateType.REPEAT, 1000,
                    automationListener);
            resource.stopUpdation(id);
            result = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        try {
            resource.stop();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(result);
    }

    private SimulatorSingleResource createResourceFromRAML() {
        try {
            return (SimulatorSingleResource) SimulatorManager
                    .createResource(SINGLE_RES_RAML);
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        return null;
    }
}

class AutoUpdateInfo {
    private String uri = null;
    private int    id  = -1;

    AutoUpdateInfo(String uri, int id) {
        this.uri = uri;
        this.id = id;
    }

    public String getUri() {
        return uri;
    }

    public int getId() {
        return id;
    }
}

class AutoUpdateCompleteListener implements AutoUpdateListener {
    private CountDownLatch               lock;
    private ObjectHolder<AutoUpdateInfo> autoUpdateHolder;

    public AutoUpdateCompleteListener(CountDownLatch lock,
            ObjectHolder<AutoUpdateInfo> autoUpdateHolder) {
        this.lock = lock;
        this.autoUpdateHolder = autoUpdateHolder;
    }

    @Override
    public void onUpdateComplete(String uri, int id) {
        autoUpdateHolder.set(new AutoUpdateInfo(uri, id));
        lock.countDown();
    }
}