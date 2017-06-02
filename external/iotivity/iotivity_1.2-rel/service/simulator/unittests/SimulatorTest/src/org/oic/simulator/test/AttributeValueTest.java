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

import org.oic.simulator.AttributeValue;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.SimulatorResourceModel;

import junit.framework.TestCase;

public class AttributeValueTest extends TestCase {
    private static final String INT_KEY    = "Interger";
    private static final String DOUBLE_KEY = "Double";
    private static final String BOOL_KEY   = "Boolean";
    private static final String STRING_KEY = "String";

    protected void setUp() throws Exception {
        super.setUp();
    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }

    public void testAttributeValueInt_P01() {
        AttributeValue value = new AttributeValue(5);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.INTEGER
                && value.typeInfo().mBaseType == AttributeValue.ValueType.INTEGER
                && value.typeInfo().mDepth == 0);
    }

    public void testAttributeValueInt_P02() {
        AttributeValue attribute = new AttributeValue(new Integer(5));
        assertNotNull(attribute);
        assertTrue(attribute.typeInfo() != null
                && attribute
                        .typeInfo().mType == AttributeValue.ValueType.INTEGER
                && attribute
                        .typeInfo().mBaseType == AttributeValue.ValueType.INTEGER
                && attribute.typeInfo().mDepth == 0);
    }

    public void testAttributeValueDouble_P01() {
        AttributeValue value = new AttributeValue(5.00);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.DOUBLE
                && value.typeInfo().mBaseType == AttributeValue.ValueType.DOUBLE
                && value.typeInfo().mDepth == 0);
    }

    public void testAttributeValueDouble_P02() {
        AttributeValue value = new AttributeValue(new Double(5.00));

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.DOUBLE
                && value.typeInfo().mBaseType == AttributeValue.ValueType.DOUBLE
                && value.typeInfo().mDepth == 0);
    }

    public void testAttributeValueBoolean_P01() {
        AttributeValue value = new AttributeValue(true);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.BOOLEAN
                && value.typeInfo().mBaseType == AttributeValue.ValueType.BOOLEAN
                && value.typeInfo().mDepth == 0);
    }

    public void testAttributeValueBoolean_P02() {
        AttributeValue value = new AttributeValue(new Boolean(true));

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.BOOLEAN
                && value.typeInfo().mBaseType == AttributeValue.ValueType.BOOLEAN
                && value.typeInfo().mDepth == 0);
    }

    public void testAttributeValueString_P01() {
        AttributeValue value = new AttributeValue("String");

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.STRING
                && value.typeInfo().mBaseType == AttributeValue.ValueType.STRING
                && value.typeInfo().mDepth == 0);
    }

    public void testAttributeValueString_P02() {
        AttributeValue value = new AttributeValue(new String("String"));

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.STRING
                && value.typeInfo().mBaseType == AttributeValue.ValueType.STRING
                && value.typeInfo().mDepth == 0);
    }

    public void testAttributeValueSimulatorResourceModel() {
        SimulatorResourceModel model = new SimulatorResourceModel();
        try {
            model.set(INT_KEY, new AttributeValue(1));
            model.set(DOUBLE_KEY, new AttributeValue(1.00));
            model.set(BOOL_KEY, new AttributeValue(true));
            model.set(STRING_KEY, new AttributeValue("string"));
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        if (model.size() != 4)
            fail("Failed to construct model for continuing test!");

        AttributeValue value = new AttributeValue(model);
        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.RESOURCEMODEL
                && value.typeInfo().mBaseType == AttributeValue.ValueType.RESOURCEMODEL
                && value.typeInfo().mDepth == 0);
    }

    public void testAttributeValueIntArray_P01() {
        int[] array = { 1, 2, 3 };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.INTEGER
                && value.typeInfo().mDepth == 1);
    }

    public void testAttributeValueIntArray_P02() {
        Integer[] array = { new Integer(1), new Integer(2), new Integer(3) };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.INTEGER
                && value.typeInfo().mDepth == 1);
    }

    public void testAttributeValueDoubleArray_P01() {
        double[] array = { 1.00, 2.00, 3.00 };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.DOUBLE
                && value.typeInfo().mDepth == 1);
    }

    public void testAttributeValueDoubleArray_P02() {
        Double[] array = { new Double(1.00), new Double(2.00),
                new Double(3.00) };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.DOUBLE
                && value.typeInfo().mDepth == 1);
    }

    public void testAttributeValueBooleanArray_P01() {
        boolean[] array = { true };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.BOOLEAN
                && value.typeInfo().mDepth == 1);
    }

    public void testAttributeValueBooleanArray_P02() {
        boolean[] array = { new Boolean(true) };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.BOOLEAN
                && value.typeInfo().mDepth == 1);
    }

    public void testAttributeValueStringArray_P01() {
        String[] array = { "string1", "string2" };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.STRING
                && value.typeInfo().mDepth == 1);
    }

    public void testAttributeValueStringArray_P02() {
        String[] array = { new String("string1"), new String("string2") };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.STRING
                && value.typeInfo().mDepth == 1);
    }

    public void testAttributeValueSimulatorResourceModelArray() {
        SimulatorResourceModel childModel = new SimulatorResourceModel();
        try {
            childModel.set(INT_KEY, new AttributeValue(1));
            childModel.set(DOUBLE_KEY, new AttributeValue(1.00));
            childModel.set(BOOL_KEY, new AttributeValue(true));
            childModel.set(STRING_KEY, new AttributeValue("string"));
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        if (childModel.size() != 4)
            fail("Failed to construct model for continuing test!");

        SimulatorResourceModel[] array = { childModel };

        AttributeValue value = new AttributeValue(array);
        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.RESOURCEMODEL
                && value.typeInfo().mDepth == 1);
    }

    public void testAttributeValueIntArrayArray_P01() {
        int[][] array = { { 1, 2, 3 } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.INTEGER
                && value.typeInfo().mDepth == 2);
    }

    public void testAttributeValueIntArrayArray_P02() {
        Integer[][] array = {
                { new Integer(1), new Integer(2), new Integer(3) } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.INTEGER
                && value.typeInfo().mDepth == 2);
    }

    public void testAttributeValueDoubleArrayArray_P01() {
        double[][] array = { { 1.00, 2.00, 3.00 } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.DOUBLE
                && value.typeInfo().mDepth == 2);
    }

    public void testAttributeValueDoubleArrayArray_P02() {
        Double[][] array = {
                { new Double(1.00), new Double(2.00), new Double(3.00) } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.DOUBLE
                && value.typeInfo().mDepth == 2);
    }

    public void testAttributeValueBooleanArrayArray_P01() {
        boolean[][] array = { { true } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.BOOLEAN
                && value.typeInfo().mDepth == 2);
    }

    public void testAttributeValueBooleanArrayArray_P02() {
        Boolean[][] array = { { new Boolean(true) } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.BOOLEAN
                && value.typeInfo().mDepth == 2);
    }

    public void testAttributeValueStringArrayArray_P01() {
        String[][] array = { { "string" } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.STRING
                && value.typeInfo().mDepth == 2);
    }

    public void testAttributeValueStringArrayArray_P02() {
        String[][] array = { { new String("string") } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.STRING
                && value.typeInfo().mDepth == 2);
    }

    public void testAttributeValueSimulatorResourceModelArrayArray() {
        SimulatorResourceModel childModel = new SimulatorResourceModel();
        try {
            childModel.set(INT_KEY, new AttributeValue(1));
            childModel.set(DOUBLE_KEY, new AttributeValue(1.00));
            childModel.set(BOOL_KEY, new AttributeValue(true));
            childModel.set(STRING_KEY, new AttributeValue("string"));
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        if (childModel.size() != 4)
            fail("Failed to construct model for continuing test!");

        SimulatorResourceModel[][] array = { { childModel } };

        AttributeValue value = new AttributeValue(array);
        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.RESOURCEMODEL
                && value.typeInfo().mDepth == 2);
    }

    public void testAttributeValueIntArrayArrayArray_P01() {
        int[][][] array = { { { 1, 2, 3 } } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.INTEGER
                && value.typeInfo().mDepth == 3);
    }

    public void testAttributeValueIntArrayArrayArray_P02() {
        Integer[][][] array = {
                { { new Integer(1), new Integer(2), new Integer(3) } } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.INTEGER
                && value.typeInfo().mDepth == 3);
    }

    public void testAttributeValueDoubleArrayArrayArray_P01() {
        double[][][] array = { { { 1.00, 2.00, 3.00 } } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.DOUBLE
                && value.typeInfo().mDepth == 3);
    }

    public void testAttributeValueDoubleArrayArrayArray_P02() {
        Double[][][] array = {
                { { new Double(1.00), new Double(2.00), new Double(3.00) } } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.DOUBLE
                && value.typeInfo().mDepth == 3);
    }

    public void testAttributeValueBooleanArrayArrayArray_P01() {
        boolean[][][] array = { { { true } } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.BOOLEAN
                && value.typeInfo().mDepth == 3);
    }

    public void testAttributeValueBooleanArrayArrayArray_P02() {
        Boolean[][][] array = { { { new Boolean(true) } } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.BOOLEAN
                && value.typeInfo().mDepth == 3);
    }

    public void testAttributeValueStringArrayArrayArray_P01() {
        String[][][] array = { { { "string" } } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.STRING
                && value.typeInfo().mDepth == 3);
    }

    public void testAttributeValueStringArrayArrayArray_P02() {
        String[][][] array = { { { new String("string") } } };
        AttributeValue value = new AttributeValue(array);

        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.STRING
                && value.typeInfo().mDepth == 3);
    }

    public void testAttributeValueSimulatorResourceModelArrayArrayArray() {
        SimulatorResourceModel childModel = new SimulatorResourceModel();
        try {
            childModel.set(INT_KEY, new AttributeValue(1));
            childModel.set(DOUBLE_KEY, new AttributeValue(1.00));
            childModel.set(BOOL_KEY, new AttributeValue(true));
            childModel.set(STRING_KEY, new AttributeValue("string"));
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        }

        if (childModel.size() != 4)
            fail("Failed to construct model for continuing test!");

        SimulatorResourceModel[][][] array = { { { childModel } } };

        AttributeValue value = new AttributeValue(array);
        assertNotNull(value);
        assertTrue(value.typeInfo() != null
                && value.typeInfo().mType == AttributeValue.ValueType.ARRAY
                && value.typeInfo().mBaseType == AttributeValue.ValueType.RESOURCEMODEL
                && value.typeInfo().mDepth == 3);
    }
}
