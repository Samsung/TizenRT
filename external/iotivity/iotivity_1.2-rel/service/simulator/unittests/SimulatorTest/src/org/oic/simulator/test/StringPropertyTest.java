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
import org.oic.simulator.StringProperty;

import junit.framework.TestCase;

public class StringPropertyTest extends TestCase {

    protected void setUp() throws Exception {
        super.setUp();
    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }

    public void testStringProperty_P01() {
        StringProperty property = new StringProperty.Builder().build();

        assertNotNull(property);
    }

    public void testIsString_P01() {
        StringProperty property = new StringProperty.Builder().build();

        assertNotNull(property);
        assertTrue(property.isString());
    }

    public void testHasRange_P01() {
        StringProperty.Builder builder = new StringProperty.Builder();
        builder.setRange(1, 5);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.hasRange());
    }

    public void testGetRange_P01() {
        StringProperty.Builder builder = new StringProperty.Builder();
        builder.setRange(1, 5);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertEquals(property.min(), 1);
        assertEquals(property.max(), 5);
    }

    public void testHasValues_P01() {
        StringProperty.Builder builder = new StringProperty.Builder();
        String[] values = { "value1", "value2" };
        builder.setValues(values);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.hasValues());
    }

    public void testGetValues_P01() {
        StringProperty.Builder builder = new StringProperty.Builder();
        String[] values = { "value1", "value2" };
        builder.setValues(values);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertNotNull(property.getValues());
        assertEquals(property.getValues().length, 2);
    }

    public void testValidate_P01() {
        StringProperty.Builder builder = new StringProperty.Builder();
        builder.setRange(1, 5);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue("12345")));
    }

    public void testValidate_P02() {
        StringProperty.Builder builder = new StringProperty.Builder();
        builder.setRange(1, 5);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate("12345"));
    }

    public void testValidate_P03() {
        StringProperty.Builder builder = new StringProperty.Builder();
        String[] values = { "value1", "value2" };
        builder.setValues(values);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue("value1")));
    }

    public void testValidate_P04() {
        StringProperty.Builder builder = new StringProperty.Builder();
        String[] values = { "value1", "value2" };
        builder.setValues(values);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate("value1"));
    }

    public void testValidate_N01() {
        StringProperty.Builder builder = new StringProperty.Builder();
        builder.setRange(1, 5);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue("123456")));
    }

    public void testValidate_N02() {
        StringProperty.Builder builder = new StringProperty.Builder();
        builder.setRange(1, 5);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate("123456"));
    }

    public void testValidate_N03() {
        StringProperty.Builder builder = new StringProperty.Builder();
        builder.setRange(3, 5);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue("12")));
    }

    public void testValidate_N04() {
        StringProperty.Builder builder = new StringProperty.Builder();
        builder.setRange(3, 5);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate("12"));
    }

    public void testValidate_N05() {
        StringProperty.Builder builder = new StringProperty.Builder();
        String[] values = { "value1", "value2" };
        builder.setValues(values);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate(new AttributeValue("wrong-value")));
    }

    public void testValidate_N06() {
        StringProperty.Builder builder = new StringProperty.Builder();
        String[] values = { "value1", "value2" };
        builder.setValues(values);

        StringProperty property = builder.build();

        assertNotNull(property);
        assertFalse(property.validate("wrong-value"));
    }
}
