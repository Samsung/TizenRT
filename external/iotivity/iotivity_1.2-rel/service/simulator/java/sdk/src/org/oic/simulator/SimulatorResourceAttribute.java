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

package org.oic.simulator;

/**
 * This class represents the resource attribute which contains attribute value
 * and its property.
 */
public class SimulatorResourceAttribute {

    private String            mName     = null;
    private AttributeValue    mValue    = null;
    private AttributeProperty mProperty = null;

    /**
     * Constructs {@link SimulatorResourceAttribute} with the given attribute
     * name, value and its property.
     *
     * @param name
     *            Name of the attribute.
     * @param value
     *            Value of the attribute.
     * @param property
     *            Property of attribute value.
     */
    public SimulatorResourceAttribute(String name, AttributeValue value,
            AttributeProperty property) {
        mName = name;
        mValue = value;
        mProperty = property;
    }

    /**
     * Constructs {@link SimulatorResourceAttribute} with the given attribute
     * name and value.
     *
     * @param name
     *            Name of the attribute.
     * @param value
     *            Value of the attribute.
     */
    public SimulatorResourceAttribute(String name, AttributeValue value) {
        mName = name;
        mValue = value;
        mProperty = null;
    }

    /**
     * API to get name of attribute.
     *
     * @return Attribute's name.
     */
    public String name() {
        return mName;
    }

    /**
     * API to get value of attribute.
     *
     * @return Attribute's value {@link AttributeValue}.
     */
    public AttributeValue value() {
        return mValue;
    }

    /**
     * API to get property of attribute's value.
     *
     * @return Attribute's value property {@link AttributeProperty
     * }.
     */
    public AttributeProperty property() {
        return mProperty;
    }

    /**
     * API to set the name of attribute.
     *
     * @param name
     *            Name of the attribute.
     */
    public void setName(String name) {
        this.mName = name;
    }

    /**
     * API to set the value of attribute.
     *
     * @param value
     *            Value of the attribute.
     */
    public void setValue(AttributeValue value) {
        this.mValue = value;
    }

    /**
     * API to set the property of attribute.
     *
     * @param property
     *            Property of the attribute.
     */
    public void setProperty(AttributeProperty property) {
        this.mProperty = property;
    }
}
