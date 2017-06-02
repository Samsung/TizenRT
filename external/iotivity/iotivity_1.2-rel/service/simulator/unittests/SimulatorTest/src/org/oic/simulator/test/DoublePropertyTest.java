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
import org.oic.simulator.DoubleProperty;

import junit.framework.TestCase;

public class DoublePropertyTest extends TestCase {

    protected void setUp() throws Exception {
        super.setUp();
    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }

    public void testDoubleProperty_P01() {
        DoubleProperty property = new DoubleProperty.Builder().build();

        assertNotNull(property);
        assertEquals(property.getDefaultValue(), 0.0);
    }

    public void testIsDouble_P01() {
        DoubleProperty property = new DoubleProperty.Builder().build();

        assertNotNull(property);
        assertTrue(property.isDouble());
    }

    public void testGetDefaultValue_P02() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        builder.setDefaultValue(3.142);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertEquals(property.getDefaultValue(), 3.142);
    }

    public void testHasRange_P01() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        builder.setRange(1.0, 10.25);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.hasRange());
    }

    public void testGetRange_P03() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        builder.setRange(1.0, 10.25);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertEquals(property.min(), 1.0);
        assertEquals(property.max(), 10.25);
    }

    public void testHasValues_P01() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        double[] values = { 1.5, 2, 3.2 };
        builder.setValues(values);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.hasValues());
    }

    public void testGetValues_P04() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        double[] values = { 1.5, 2, 3.2 };
        builder.setValues(values);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertNotNull(property.getValues());
        assertEquals(property.getValues().length, 3);
    }

    public void testValidate_P04() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        builder.setRange(1.0, 10.25);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(3.25)));
    }

    public void testValidate_P05() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        builder.setRange(1.0, 10.25);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate(3.25));
    }

    public void testValidate_P06() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        double[] values = { 1.5, 2, 3.2 };
        builder.setValues(values);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(3.2)));
    }

    public void testValidate_P07() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        double[] values = { 1.5, 2, 3.2 };
        builder.setValues(values);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate(3.2));
    }

    public void testValidate_N01() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        builder.setRange(1.0, 10.25);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(11.5)));
    }

    public void testValidate_N02() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        builder.setRange(1.0, 10.25);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate(11.5));
    }

    public void testValidate_N04() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        double[] values = { 1.5, 2, 3.2 };
        builder.setValues(values);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(11.25)));
    }

    public void testValidate_N05() {
        DoubleProperty.Builder builder = new DoubleProperty.Builder();
        double[] values = { 1.5, 2, 3.2 };
        builder.setValues(values);

        DoubleProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate(11.25));
    }
}
