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

import java.util.HashMap;
import java.util.Map;

/**
 * This class represents the resource model of a resource. A resource model
 * contains a set of attributes. It also represents the complex value type of an
 * attribute.
 */
public class SimulatorResourceModel {

    private Map<String, AttributeValue> mValues = null;

    /**
     * Constructs new {@link SimulatorResourceModel} object.
     */
    public SimulatorResourceModel() {
        mValues = new HashMap<>();
    }

    /**
     * API to set an attribute to resource model. If an attribute exists with
     * the given name, then it overwrites the existing value. Otherwise creates
     * a new attribute.
     *
     * @param attrName
     *            Name of the attribute.
     * @param value
     *            Value of the attribute.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown on invalid input.
     */
    public void set(String attrName, AttributeValue value)
            throws InvalidArgsException {
        if (null == attrName || attrName.isEmpty())
            throw new InvalidArgsException("Invalid attribute name!");

        if (null == value)
            throw new InvalidArgsException("Attribute value is null!");

        mValues.put(attrName, value);
    }

    /**
     * API to remove attribute from resource model.
     *
     * @param attrName
     *            Name of the attribute.
     *
     * @return true if resource model has an attribute with the given name,
     *         otherwise false.
     */
    public boolean remove(String attrName) {
        if (mValues.containsKey(attrName)) {
            mValues.remove(attrName);
            return true;
        }

        return false;
    }

    /**
     * API to get attribute by name.
     *
     * @param attrName
     *            Name of the attribute.
     *
     * @return {@link SimulatorResourceAttribute}.
     */
    public AttributeValue get(String attrName) {
        if (mValues.containsKey(attrName)) {
            return mValues.get(attrName);
        }

        return null;
    }

    /**
     * API to get all attributes of the model.
     *
     * @return A map of {@link AttributeValue} objects with attribute name as
     *         the key.
     */
    public Map<String, AttributeValue> get() {
        return mValues;
    }

    /**
     * API to check whether resource model has an attribute with given name.
     *
     * @param attrName
     *            Name of the attribute.
     *
     * @return true if resource model has an attribute with given name,
     *         otherwise false.
     */
    public boolean contains(String attrName) {
        if (mValues.containsKey(attrName))
            return true;
        return false;
    }

    /**
     * API to get value type information of attribute.
     *
     * @param attrName
     *            Name of the attribute.
     *
     * @return Attribute value type information {@link AttributeValue.TypeInfo
     *
     * }.
     */
    public AttributeValue.TypeInfo getType(String attrName) {
        if (mValues.containsKey(attrName))
            return mValues.get(attrName).typeInfo();
        return null;
    }

    /**
     * API to get number of attributes present in resource model.
     *
     * @return Number of attributes present in resource model.
     */
    public int size() {
        return mValues.size();
    }

    private SimulatorResourceModel(Map<String, AttributeValue> values) {
        mValues = values;
    }
}
