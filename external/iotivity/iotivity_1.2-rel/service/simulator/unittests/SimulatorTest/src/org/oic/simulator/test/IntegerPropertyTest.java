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
import org.oic.simulator.IntegerProperty;

import junit.framework.TestCase;

public class IntegerPropertyTest extends TestCase {

    protected void setUp() throws Exception {
        super.setUp();
    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }

    public void testIntegerProperty_P01() {
        IntegerProperty property = new IntegerProperty.Builder().build();

        assertNotNull(property);
        assertEquals(property.getDefaultValue(), 0);
    }

    public void testIsInteger_P01() {
        IntegerProperty property = new IntegerProperty.Builder().build();

        assertNotNull(property);
        assertTrue(property.isInteger());
    }

    public void testGetDefaultValue_P02() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        builder.setDefaultValue(10);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertEquals(property.getDefaultValue(), 10);
    }

    public void testHasRange_P01() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        builder.setRange(1, 10);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.hasRange());
    }

    public void testGetRange_P03() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        builder.setRange(1, 10);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertEquals(property.min(), 1);
        assertEquals(property.max(), 10);
    }

    public void testHasValues_P01() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        int[] values = { 1, 2, 3 };
        builder.setValues(values);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.hasValues());
    }

    public void testGetValues_P04() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        int[] values = { 1, 2, 3 };
        builder.setValues(values);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertEquals(property.getDefaultValue(), 0);
        assertNotNull(property.getValues());
        assertEquals(property.getValues().length, 3);
    }

    public void testValidate_P04() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        builder.setRange(1, 10);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(3)));
    }

    public void testValidate_P05() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        builder.setRange(1, 10);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate(3));
    }

    public void testValidate_P06() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        int[] values = { 1, 2, 3 };
        builder.setValues(values);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(3)));
    }

    public void testValidate_P07() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        int[] values = { 1, 2, 3 };
        builder.setValues(values);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate(3));
    }

    public void testValidate_N01() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        builder.setRange(1, 10);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(11)));
    }

    public void testValidate_N02() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        builder.setRange(1, 10);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate(11));
    }

    public void testValidate_N04() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        int[] values = { 1, 2, 3 };
        builder.setValues(values);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue(11)));
    }

    public void testValidate_N05() {
        IntegerProperty.Builder builder = new IntegerProperty.Builder();
        int[] values = { 1, 2, 3 };
        builder.setValues(values);

        IntegerProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate(11));
    }
}
