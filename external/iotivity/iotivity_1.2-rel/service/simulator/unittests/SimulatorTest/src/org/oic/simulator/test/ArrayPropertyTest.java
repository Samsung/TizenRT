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

import org.oic.simulator.ArrayProperty;
import org.oic.simulator.AttributeValue;
import org.oic.simulator.BooleanProperty;
import org.oic.simulator.DoubleProperty;
import org.oic.simulator.IntegerProperty;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.ModelProperty;
import org.oic.simulator.SimulatorResourceModel;
import org.oic.simulator.StringProperty;

import junit.framework.TestCase;

public class ArrayPropertyTest extends TestCase {

    protected void setUp() throws Exception {
        super.setUp();
    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }

    public void testArrayProperty_P01() {
        ArrayProperty.Builder builder = new ArrayProperty.Builder();
        builder.setElementProperty(new IntegerProperty.Builder().build());
        ArrayProperty property = builder.build();

        assertNotNull(property);
    }

    public void testArrayProperty_N01() {
        ArrayProperty.Builder builder = new ArrayProperty.Builder();
        ArrayProperty property = builder.build();

        assertNull(property);
    }

    public void testIsInteger_P01() {
        ArrayProperty.Builder builder = new ArrayProperty.Builder();
        builder.setElementProperty(new IntegerProperty.Builder().build());
        ArrayProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.isArray());
    }

    public void testHasRange_P01() {
        ArrayProperty.Builder builder = new ArrayProperty.Builder();
        builder.setElementProperty(new IntegerProperty.Builder().build());
        builder.setRange(1, 2);

        ArrayProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.hasRange());
    }

    public void testGetRange_P01() {
        ArrayProperty.Builder builder = new ArrayProperty.Builder();
        builder.setElementProperty(new IntegerProperty.Builder().build());
        builder.setRange(1, 2);

        ArrayProperty property = builder.build();

        assertNotNull(property);
        assertEquals(property.minItems(), 1);
        assertEquals(property.maxItems(), 2);
    }

    public void testIsVariable_P01() {
        ArrayProperty.Builder builder = new ArrayProperty.Builder();
        builder.setElementProperty(new IntegerProperty.Builder().build());
        builder.setVariableSize(true);

        ArrayProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.isVariable());
    }

    public void testIsUnique_P01() {
        ArrayProperty.Builder builder = new ArrayProperty.Builder();
        builder.setElementProperty(new IntegerProperty.Builder().build());
        builder.setUnique(true);

        ArrayProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.isUnique());
    }

    public void testValidateInt1DArray_P01() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();
        IntegerProperty.Builder intBuilder = new IntegerProperty.Builder();
        intBuilder.setRange(1, 10);
        arrayBuilder.setElementProperty(intBuilder.build());

        ArrayProperty property = arrayBuilder.build();

        int[] value = { 1, 5, 7 };

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(value)));
    }

    public void testValidateInt1DArray_N01() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();
        IntegerProperty.Builder intBuilder = new IntegerProperty.Builder();
        intBuilder.setRange(1, 10);
        arrayBuilder.setElementProperty(intBuilder.build());

        ArrayProperty property = arrayBuilder.build();

        int[] value = { 1, 15 };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateInt1DArray_N02() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();
        IntegerProperty.Builder intBuilder = new IntegerProperty.Builder();
        intBuilder.setRange(1, 10);
        arrayBuilder.setElementProperty(intBuilder.build());
        arrayBuilder.setRange(1, 2);

        ArrayProperty property = arrayBuilder.build();

        int[] value = { 1, 5, 7 };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateInt2DArray_P01() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        IntegerProperty.Builder intBuilder = new IntegerProperty.Builder();
        intBuilder.setRange(1, 10);
        arrayBuilder2.setElementProperty(intBuilder.build());
        arrayBuilder1.setElementProperty(arrayBuilder2.build());

        ArrayProperty property = arrayBuilder1.build();

        int[][] value = { { 1 } };

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(value)));
    }

    public void testValidateInt2DArray_N01() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        IntegerProperty.Builder intBuilder = new IntegerProperty.Builder();
        intBuilder.setRange(1, 10);
        arrayBuilder2.setElementProperty(intBuilder.build());
        arrayBuilder1.setElementProperty(arrayBuilder2.build());

        ArrayProperty property = arrayBuilder1.build();

        int[][] value = { { 15 } };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateInt2DArray_N02() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        IntegerProperty.Builder intBuilder = new IntegerProperty.Builder();
        intBuilder.setRange(1, 10);
        arrayBuilder2.setElementProperty(intBuilder.build());
        arrayBuilder2.setRange(1, 1);
        arrayBuilder1.setElementProperty(arrayBuilder2.build());
        arrayBuilder1.setRange(1, 1);

        ArrayProperty property = arrayBuilder1.build();

        int[][] value = { { 1, 2 } };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateInt2DArray_N03() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        IntegerProperty.Builder intBuilder = new IntegerProperty.Builder();
        intBuilder.setRange(1, 10);
        arrayBuilder2.setElementProperty(intBuilder.build());
        arrayBuilder2.setRange(1, 1);
        arrayBuilder1.setElementProperty(arrayBuilder2.build());
        arrayBuilder1.setRange(1, 1);

        ArrayProperty property = arrayBuilder1.build();

        int[][] value = { { 1 }, { 2 } };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateDouble1DArray_P01() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();
        DoubleProperty.Builder doubleBuilder = new DoubleProperty.Builder();
        doubleBuilder.setRange(1.2, 10.5);
        arrayBuilder.setElementProperty(doubleBuilder.build());

        ArrayProperty property = arrayBuilder.build();

        double[] value = { 1.3, 5.2, 10.4 };

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(value)));
    }

    public void testValidateDouble1DArray_N01() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();
        DoubleProperty.Builder doubleBuilder = new DoubleProperty.Builder();
        doubleBuilder.setRange(1.2, 10.5);
        arrayBuilder.setElementProperty(doubleBuilder.build());

        ArrayProperty property = arrayBuilder.build();

        double[] value = { 5.5, 10.9 };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateDouble1DArray_N02() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();
        DoubleProperty.Builder doubleBuilder = new DoubleProperty.Builder();
        doubleBuilder.setRange(1.2, 10.5);
        arrayBuilder.setElementProperty(doubleBuilder.build());
        arrayBuilder.setRange(1, 2);

        ArrayProperty property = arrayBuilder.build();

        double[] value = { 1.3, 5.2, 10.4 };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateDouble2DArray_P01() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        DoubleProperty.Builder doubleBuilder = new DoubleProperty.Builder();
        doubleBuilder.setRange(1.2, 10.5);
        arrayBuilder2.setElementProperty(doubleBuilder.build());
        arrayBuilder1.setElementProperty(arrayBuilder2.build());

        ArrayProperty property = arrayBuilder1.build();

        double[][] value = { { 5.5 } };

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(value)));
    }

    public void testValidateDouble2DArray_N01() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        DoubleProperty.Builder doubleBuilder = new DoubleProperty.Builder();
        doubleBuilder.setRange(1.2, 10.5);
        arrayBuilder2.setElementProperty(doubleBuilder.build());
        arrayBuilder1.setElementProperty(arrayBuilder2.build());

        ArrayProperty property = arrayBuilder1.build();

        double[][] value = { { 10.9 } };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateDouble2DArray_N02() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        DoubleProperty.Builder doubleBuilder = new DoubleProperty.Builder();
        doubleBuilder.setRange(1.2, 10.5);
        arrayBuilder2.setElementProperty(doubleBuilder.build());
        arrayBuilder2.setRange(1, 1);
        arrayBuilder1.setElementProperty(arrayBuilder2.build());
        arrayBuilder1.setRange(1, 1);

        ArrayProperty property = arrayBuilder1.build();

        double[][] value = { { 1.5, 2.5 } };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateDouble2DArray_N03() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        DoubleProperty.Builder doubleBuilder = new DoubleProperty.Builder();
        doubleBuilder.setRange(1.2, 10.5);
        arrayBuilder2.setElementProperty(doubleBuilder.build());
        arrayBuilder2.setRange(1, 1);
        arrayBuilder1.setElementProperty(arrayBuilder2.build());
        arrayBuilder1.setRange(1, 1);

        ArrayProperty property = arrayBuilder1.build();

        double[][] value = { { 1.5 }, { 2.5 } };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateBoolean1DArray_P01() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();
        BooleanProperty.Builder booleanBuilder = new BooleanProperty.Builder();
        arrayBuilder.setElementProperty(booleanBuilder.build());

        ArrayProperty property = arrayBuilder.build();

        boolean[] value = { true, false };

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(value)));
    }

    public void testValidateBoolean1DArray_N01() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();
        BooleanProperty.Builder booleanBuilder = new BooleanProperty.Builder();
        arrayBuilder.setElementProperty(booleanBuilder.build());
        arrayBuilder.setRange(1, 1);

        ArrayProperty property = arrayBuilder.build();

        boolean[] value = { true, false };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateBoolean2DArray_P01() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        BooleanProperty.Builder booleanBuilder = new BooleanProperty.Builder();
        arrayBuilder2.setElementProperty(booleanBuilder.build());
        arrayBuilder1.setElementProperty(arrayBuilder2.build());

        ArrayProperty property = arrayBuilder1.build();

        boolean[][] value = { { true, false } };

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(value)));
    }

    public void testValidateBoolean2DArray_N01() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        BooleanProperty.Builder booleanBuilder = new BooleanProperty.Builder();
        arrayBuilder2.setElementProperty(booleanBuilder.build());
        arrayBuilder2.setRange(1, 1);
        arrayBuilder1.setElementProperty(arrayBuilder2.build());

        ArrayProperty property = arrayBuilder1.build();

        boolean[][] value = { { true, false } };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateBoolean2DArray_N02() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        BooleanProperty.Builder booleanBuilder = new BooleanProperty.Builder();
        arrayBuilder2.setElementProperty(booleanBuilder.build());
        arrayBuilder2.setRange(1, 1);
        arrayBuilder1.setElementProperty(arrayBuilder2.build());
        arrayBuilder1.setRange(1, 1);

        ArrayProperty property = arrayBuilder1.build();

        boolean[][] value = { { true }, { false } };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateString1DArray_P01() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();
        StringProperty.Builder stringBuilder = new StringProperty.Builder();
        stringBuilder.setRange(1, 5);
        arrayBuilder.setElementProperty(stringBuilder.build());

        ArrayProperty property = arrayBuilder.build();

        String[] value = { "12345" };

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(value)));
    }

    public void testValidateString1DArray_N01() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();
        StringProperty.Builder stringBuilder = new StringProperty.Builder();
        stringBuilder.setRange(1, 5);
        arrayBuilder.setElementProperty(stringBuilder.build());

        ArrayProperty property = arrayBuilder.build();

        String[] value = { "123456" };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateString1DArray_N02() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();
        StringProperty.Builder stringBuilder = new StringProperty.Builder();
        stringBuilder.setRange(1, 5);
        arrayBuilder.setElementProperty(stringBuilder.build());
        arrayBuilder.setRange(1, 2);

        ArrayProperty property = arrayBuilder.build();

        String[] value = { "1", "2", "3", "4" };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateString2DArray_P01() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        StringProperty.Builder stringBuilder = new StringProperty.Builder();
        stringBuilder.setRange(1, 5);
        arrayBuilder2.setElementProperty(stringBuilder.build());
        arrayBuilder1.setElementProperty(arrayBuilder2.build());

        ArrayProperty property = arrayBuilder1.build();

        String[][] value = { { "12345" } };

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(value)));
    }

    public void testValidateString2DArray_N01() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        StringProperty.Builder stringBuilder = new StringProperty.Builder();
        stringBuilder.setRange(1, 5);
        arrayBuilder2.setElementProperty(stringBuilder.build());
        arrayBuilder1.setElementProperty(arrayBuilder2.build());

        ArrayProperty property = arrayBuilder1.build();

        String[][] value = { { "123456" } };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateString2DArray_N02() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        StringProperty.Builder stringBuilder = new StringProperty.Builder();
        stringBuilder.setRange(1, 5);
        arrayBuilder2.setElementProperty(stringBuilder.build());
        arrayBuilder2.setRange(1, 1);
        arrayBuilder1.setElementProperty(arrayBuilder2.build());
        arrayBuilder1.setRange(1, 1);

        ArrayProperty property = arrayBuilder1.build();

        String[][] value = { { "1", "2" } };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateString2DArray_N03() {
        ArrayProperty.Builder arrayBuilder1 = new ArrayProperty.Builder();
        ArrayProperty.Builder arrayBuilder2 = new ArrayProperty.Builder();
        StringProperty.Builder stringBuilder = new StringProperty.Builder();
        stringBuilder.setRange(1, 5);
        arrayBuilder2.setElementProperty(stringBuilder.build());
        arrayBuilder2.setRange(1, 1);
        arrayBuilder1.setElementProperty(arrayBuilder2.build());
        arrayBuilder1.setRange(1, 1);

        ArrayProperty property = arrayBuilder1.build();

        String[][] value = { { "1", "2" } };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(value)));
    }

    public void testValidateModel1DArray_P01() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();

        {
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

            arrayBuilder.setElementProperty(modelProperty);
        }

        ArrayProperty property = arrayBuilder.build();

        SimulatorResourceModel value = new SimulatorResourceModel();
        try {
            value.set("int", new AttributeValue(5));
            value.set("double", new AttributeValue(3.5));
            value.set("boolean", new AttributeValue(true));
            value.set("string", new AttributeValue("12345"));
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        SimulatorResourceModel[] values = { value };

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(values)));
    }

    public void testValidateModel1DArray_N01() {
        ArrayProperty.Builder arrayBuilder = new ArrayProperty.Builder();

        {
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

            arrayBuilder.setElementProperty(modelProperty);
        }

        ArrayProperty property = arrayBuilder.build();

        SimulatorResourceModel value = new SimulatorResourceModel();
        try {
            value.set("int", new AttributeValue(5));
            value.set("double", new AttributeValue(3.5));
            value.set("boolean", new AttributeValue(true));
            value.set("string", new AttributeValue("123456"));
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        SimulatorResourceModel[] values = { value };

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(values)));
    }
}
