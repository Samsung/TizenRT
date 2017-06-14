/*
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

import org.oic.simulator.AttributeValue;
import org.oic.simulator.BooleanProperty;
import org.oic.simulator.DoubleProperty;
import org.oic.simulator.IntegerProperty;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.ModelProperty;
import org.oic.simulator.SimulatorResourceModel;
import org.oic.simulator.StringProperty;

import junit.framework.TestCase;

public class ModelPropertyTest extends TestCase {

    protected void setUp() throws Exception {
        super.setUp();
    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }

    public void testModelProperty_P01() {
        ModelProperty property = new ModelProperty();

        assertNotNull(property);
    }

    public void testIsModel_P01() {
        ModelProperty property = new ModelProperty();

        assertNotNull(property);
        assertTrue(property.isModel());
    }

    public void testAdd_P01() {
        ModelProperty property = new ModelProperty();
        boolean result = property.add("int",
                new IntegerProperty.Builder().build(), true);

        assertTrue(result);
        assertNotNull(property.get("int"));
    }

    public void testAdd_N01() {
        ModelProperty property = new ModelProperty();
        boolean result = property.add(null,
                new IntegerProperty.Builder().build(), true);

        assertFalse(result);
        assertNull(property.get("int"));
    }

    public void testAdd_N02() {
        ModelProperty property = new ModelProperty();
        boolean result = property.add("int", null, true);

        assertFalse(result);
        assertNull(property.get("int"));
    }

    public void testRemove_P01() {
        ModelProperty property = new ModelProperty();
        boolean result = property.add("int",
                new IntegerProperty.Builder().build(), true);
        assertTrue(result);

        property.remove("int");
        assertNull(property.get("int"));
    }

    public void testRemove_N01() {
        ModelProperty property = new ModelProperty();
        boolean result = property.add("int",
                new IntegerProperty.Builder().build(), true);
        assertTrue(result);

        property.remove("wrong-key");
        assertNotNull(property.get("int"));
    }

    public void testRemove_N02() {
        ModelProperty property = new ModelProperty();
        boolean result = property.add("int",
                new IntegerProperty.Builder().build(), true);
        assertTrue(result);

        property.remove(null);
        assertNotNull(property.get("int"));
    }

    public void testGet_P01() {
        ModelProperty property = new ModelProperty();
        boolean result = property.add("int",
                new IntegerProperty.Builder().build(), true);
        assertTrue(result);

        assertNotNull(property.get("int"));
    }

    public void testGetChildProperties_P01() {
        ModelProperty property = new ModelProperty();

        boolean result = property.add("int",
                new IntegerProperty.Builder().build(), true);
        assertTrue(result);

        result = property.add("double", new DoubleProperty.Builder().build(),
                true);
        assertTrue(result);

        assertNotNull(property.getChildProperties());
        assertEquals(property.getChildProperties().size(), 2);
    }

    public void testIsRequired_P01() {
        ModelProperty property = new ModelProperty();

        boolean result = property.add("int",
                new IntegerProperty.Builder().build(), true);
        assertTrue(result);

        result = property.add("double", new DoubleProperty.Builder().build(),
                false);
        assertTrue(result);

        assertEquals(property.isRequired("int"), true);
        assertEquals(property.isRequired("double"), false);
    }

    public void testValidate_P01() {
        ModelProperty modelProperty = new ModelProperty();

        IntegerProperty.Builder intBuilder = new IntegerProperty.Builder();
        intBuilder.setRange(1, 10);
        modelProperty.add("int", intBuilder.build(), true);

        DoubleProperty.Builder doubleBuilder = new DoubleProperty.Builder();
        doubleBuilder.setRange(1.5, 5.5);
        modelProperty.add("double", doubleBuilder.build(), true);

        BooleanProperty.Builder booleanBuilder = new BooleanProperty.Builder();
        modelProperty.add("boolean", booleanBuilder.build(), true);

        StringProperty.Builder stringBuilder = new StringProperty.Builder();
        stringBuilder.setRange(1, 5);
        modelProperty.add("string", stringBuilder.build(), true);

        SimulatorResourceModel value = new SimulatorResourceModel();
        try {
            value.set("int", new AttributeValue(5));
            value.set("double", new AttributeValue(3.5));
            value.set("boolean", new AttributeValue(true));
            value.set("string", new AttributeValue("12345"));
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertTrue(modelProperty.validate(new AttributeValue(value)));
    }

    public void testValidate_P02() {
        ModelProperty modelProperty = new ModelProperty();

        IntegerProperty.Builder intBuilder = new IntegerProperty.Builder();
        intBuilder.setRange(1, 10);
        modelProperty.add("int", intBuilder.build(), true);

        DoubleProperty.Builder doubleBuilder = new DoubleProperty.Builder();
        doubleBuilder.setRange(1.5, 5.5);
        modelProperty.add("double", doubleBuilder.build(), true);

        BooleanProperty.Builder booleanBuilder = new BooleanProperty.Builder();
        modelProperty.add("boolean", booleanBuilder.build(), true);

        StringProperty.Builder stringBuilder = new StringProperty.Builder();
        stringBuilder.setRange(1, 5);
        modelProperty.add("string", stringBuilder.build(), true);

        SimulatorResourceModel value = new SimulatorResourceModel();
        try {
            value.set("int", new AttributeValue(5));
            value.set("double", new AttributeValue(3.5));
            value.set("boolean", new AttributeValue(true));
            value.set("string", new AttributeValue("12345"));
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertTrue(modelProperty.validate(value));
    }

    public void testValidate_N01() {
        ModelProperty modelProperty = new ModelProperty();

        IntegerProperty.Builder intBuilder = new IntegerProperty.Builder();
        intBuilder.setRange(1, 10);
        modelProperty.add("int", intBuilder.build(), true);

        DoubleProperty.Builder doubleBuilder = new DoubleProperty.Builder();
        doubleBuilder.setRange(1.5, 5.5);
        modelProperty.add("double", doubleBuilder.build(), true);

        BooleanProperty.Builder booleanBuilder = new BooleanProperty.Builder();
        modelProperty.add("boolean", booleanBuilder.build(), true);

        StringProperty.Builder stringBuilder = new StringProperty.Builder();
        stringBuilder.setRange(1, 5);
        modelProperty.add("string", stringBuilder.build(), true);

        SimulatorResourceModel value = new SimulatorResourceModel();
        try {
            value.set("int", new AttributeValue(5));
            value.set("double", new AttributeValue(3.5));
            value.set("boolean", new AttributeValue(true));
            value.set("string", new AttributeValue("123456"));
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        assertFalse(modelProperty.validate(new AttributeValue(value)));
    }
}
