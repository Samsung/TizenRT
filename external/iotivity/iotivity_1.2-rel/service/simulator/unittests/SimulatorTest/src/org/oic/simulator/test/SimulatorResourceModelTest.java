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

package org.oic.simulator.test;

import java.util.Map;

import junit.framework.TestCase;

import org.oic.simulator.AttributeValue;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.SimulatorResourceModel;

/**
 * This class tests the APIs of SimulatorResourceModel class.
 */
public class SimulatorResourceModelTest extends TestCase {
    private static final String INT_KEY    = "Interger";
    private static final String DOUBLE_KEY = "Double";
    private static final String BOOL_KEY   = "Boolean";
    private static final String STRING_KEY = "String";

    @Override
    protected void setUp() throws Exception {
        super.setUp();
    }

    @Override
    protected void tearDown() throws Exception {
        super.tearDown();
    }

    public void testSimulatorResourceModel_P01() {
        SimulatorResourceModel resModel = new SimulatorResourceModel();
        assertNotNull(resModel);
    }

    public void testSetInt_P01() {
        int result = -1;

        try {
            SimulatorResourceModel resModel = new SimulatorResourceModel();
            resModel.set(INT_KEY, new AttributeValue(10));
            result = ((Integer) resModel.get(INT_KEY).get()).intValue();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertEquals(10, result);
    }

    public void testSetInt_P02() {
        int result = -1;

        try {
            SimulatorResourceModel resModel = new SimulatorResourceModel();
            resModel.set(INT_KEY, new AttributeValue(-10));
            result = ((Integer) resModel.get(INT_KEY).get()).intValue();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertEquals(-10, result);
    }

    public void testSetDouble_P01() {
        double result = 0.0;

        try {
            SimulatorResourceModel resModel = new SimulatorResourceModel();
            resModel.set(DOUBLE_KEY, new AttributeValue(4.0));
            result = ((Double) resModel.get(DOUBLE_KEY).get()).doubleValue();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertEquals(4.0, result);
    }

    public void testSetDouble_P02() {
        double result = 0.0;

        try {
            SimulatorResourceModel resModel = new SimulatorResourceModel();
            resModel.set(DOUBLE_KEY, new AttributeValue(-4.0));
            result = ((Double) resModel.get(DOUBLE_KEY).get()).doubleValue();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertEquals(-4.0, result);
    }

    public void testSetBoolean_P01() {
        boolean result = false;

        try {
            SimulatorResourceModel resModel = new SimulatorResourceModel();
            resModel.set(BOOL_KEY, new AttributeValue(true));
            result = ((Boolean) resModel.get(BOOL_KEY).get()).booleanValue();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertEquals(true, result);
    }

    public void testSetString_P01() {
        String result = null;

        try {
            SimulatorResourceModel resModel = new SimulatorResourceModel();
            resModel.set(STRING_KEY, new AttributeValue("string-value"));
            result = (String) resModel.get(STRING_KEY).get();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertEquals("string-value", result);
    }

    public void testSetString_P02() {
        String result = null;

        try {
            SimulatorResourceModel resModel = new SimulatorResourceModel();
            resModel.set(STRING_KEY, new AttributeValue(""));
            result = (String) resModel.get(STRING_KEY).get();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertEquals("", result);
    }

    public void testSetString_P03() {
        String result = null;

        try {
            SimulatorResourceModel resModel = new SimulatorResourceModel();
            resModel.set(STRING_KEY, new AttributeValue("@#$$&^*^(*^&"));
            result = (String) resModel.get(STRING_KEY).get();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertEquals("@#$$&^*^(*^&", result);
    }

    public void testSize_P01() {
        int result = -1;

        SimulatorResourceModel resModel = new SimulatorResourceModel();
        result = resModel.size();

        assertEquals(0, result);
    }

    public void testSize_P02() {
        int result = -1;

        SimulatorResourceModel resModel = new SimulatorResourceModel();
        try {
            resModel.set(INT_KEY, new AttributeValue(1234));
            resModel.set(DOUBLE_KEY, new AttributeValue(1.234));
            resModel.set(BOOL_KEY, new AttributeValue(true));
            resModel.set(STRING_KEY, new AttributeValue("string-value"));
            result = resModel.size();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertEquals(4, result);
    }

    public void testGet_P01() {
        AttributeValue result = null;

        SimulatorResourceModel resModel = new SimulatorResourceModel();
        try {
            resModel.set(INT_KEY, new AttributeValue(10));
            result = resModel.get(INT_KEY);
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertNotNull(result);
    }

    public void testGet_P02() {
        Map<String, AttributeValue> result = null;

        SimulatorResourceModel resModel = new SimulatorResourceModel();
        try {
            resModel.set(INT_KEY, new AttributeValue(1234));
            resModel.set(DOUBLE_KEY, new AttributeValue(1.234));
            resModel.set(BOOL_KEY, new AttributeValue(true));
            resModel.set(STRING_KEY, new AttributeValue("string-value"));
            result = resModel.get();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertNotNull(result);
        assertEquals(4, result.size());
    }
}
