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

public class ModelProperty extends AttributeProperty {

    private Map<String, Boolean>           mRequiredAttributes;
    private Map<String, AttributeProperty> mChildProperties;

    ModelProperty() {
        super(Type.MODEL);
        mRequiredAttributes = new HashMap<>();
        mChildProperties = new HashMap<>();
    }

    @Override
    public boolean isModel() {
        return true;
    }

    @Override
    public ModelProperty asModel() {
        return this;
    }

    public boolean add(String name, AttributeProperty property, boolean required) {
        if (null == name || null == property) {
            return false;
        }

        mChildProperties.put(name, property);
        mRequiredAttributes.put(name, required);
        return true;
    }

    public void remove(String name) {
        mChildProperties.remove(name);
        mRequiredAttributes.remove(name);
    }

    public AttributeProperty get(String name) {
        return mChildProperties.get(name);
    }

    public Map<String, AttributeProperty> getChildProperties() {
        return mChildProperties;
    }

    public boolean isRequired(String name) {
        return mRequiredAttributes.get(name);
    }

    @Override
    public boolean validate(AttributeValue value) {
        return new ModelValueValidator(this).validate(value);
    }

    public boolean validate(SimulatorResourceModel model) {
        for (Map.Entry<String, AttributeValue> attributeEntry : model.get()
                .entrySet()) {
            AttributeProperty childProperty = get(attributeEntry.getKey());
            if (null != childProperty
                    && !childProperty.validate(attributeEntry.getValue())) {
                return false;
            }
        }

        return true;
    }
}
