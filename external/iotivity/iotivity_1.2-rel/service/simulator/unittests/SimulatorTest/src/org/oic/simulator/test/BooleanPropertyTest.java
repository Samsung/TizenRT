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

import junit.framework.TestCase;

public class BooleanPropertyTest extends TestCase {

    protected void setUp() throws Exception {
        super.setUp();
    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }

    public void testBooleanProperty_P01() {
        BooleanProperty property = new BooleanProperty.Builder().build();

        assertNotNull(property);
        assertEquals(property.getDefaultValue(), true);
    }

    public void testIsBoolean_P01() {
        BooleanProperty property = new BooleanProperty.Builder().build();

        assertNotNull(property);
        assertTrue(property.isBoolean());
    }

    public void testGetDefaultValue_P02() {
        BooleanProperty.Builder builder = new BooleanProperty.Builder();
        builder.setDefaultValue(true);

        BooleanProperty property = builder.build();

        assertNotNull(property);
        assertEquals(property.getDefaultValue(), true);
    }

    public void testValidate_P01() {
        BooleanProperty.Builder builder = new BooleanProperty.Builder();
        builder.setDefaultValue(true);

        BooleanProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate(new AttributeValue(false)));
    }

    public void testValidate_P02() {
        BooleanProperty.Builder builder = new BooleanProperty.Builder();
        builder.setDefaultValue(true);

        BooleanProperty property = builder.build();

        assertNotNull(property);
        assertTrue(property.validate(false));
    }
}
