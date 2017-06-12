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

import java.util.HashSet;
import java.util.Set;

class ArrayValueValidator implements
        AttributeValueVisitor.VisitingMethods<Boolean> {

    private ArrayProperty mProperty = null;

    ArrayValueValidator(ArrayProperty property) {
        mProperty = property;
    }

    public boolean validate(AttributeValue value) {
        AttributeValueVisitor visitor = new AttributeValueVisitor(value, this);
        return (Boolean) visitor.visit();
    }

    @Override
    public Boolean visitingValue(Integer value) {
        return false;
    }

    @Override
    public Boolean visitingValue(Double value) {
        return false;
    }

    @Override
    public Boolean visitingValue(Boolean value) {
        return false;
    }

    @Override
    public Boolean visitingValue(String value) {
        return false;
    }

    @Override
    public Boolean visitingValue(SimulatorResourceModel value) {
        return false;
    }

    @Override
    public Boolean visitingValue(Integer[] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isInteger()) {
            return false;
        }

        Set<Integer> uniqueSet = new HashSet<Integer>();
        for (Integer value : values) {
            if (mProperty.isUnique() && uniqueSet.add(value) == false) {
                return false;
            }

            IntegerProperty intProperty = elementProperty.asInteger();
            if (null == intProperty) {
                return false;
            }

            if (false == intProperty.validate(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(Double[] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isDouble()) {
            return false;
        }

        Set<Double> uniqueSet = new HashSet<Double>();
        for (Double value : values) {
            if (mProperty.isUnique() && uniqueSet.add(value) == false) {
                return false;
            }

            DoubleProperty dblProperty = elementProperty.asDouble();
            if (null == dblProperty) {
                return false;
            }

            if (false == dblProperty.validate(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(Boolean[] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isBoolean()) {
            return false;
        }

        Set<Boolean> uniqueSet = new HashSet<Boolean>();
        for (Boolean value : values) {
            if (mProperty.isUnique() && uniqueSet.add(value) == false) {
                return false;
            }

            BooleanProperty boolProperty = elementProperty.asBoolean();
            if (null == boolProperty) {
                return false;
            }

            if (false == boolProperty.validate(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(String[] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isString()) {
            return false;
        }

        Set<String> uniqueSet = new HashSet<String>();
        for (String value : values) {
            if (mProperty.isUnique() && uniqueSet.add(value) == false) {
                return false;
            }

            StringProperty stringProperty = elementProperty.asString();
            if (null == stringProperty) {
                return false;
            }

            if (false == stringProperty.validate(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(SimulatorResourceModel[] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isInteger()) {
            return false;
        }

        for (SimulatorResourceModel value : values) {
            ModelProperty modelProperty = elementProperty.asModel();
            if (null == modelProperty) {
                return false;
            }

            if (false == modelProperty.validate(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(Integer[][] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isArray()) {
            return false;
        }

        ArrayValueValidator validator = new ArrayValueValidator(
                elementProperty.asArray());
        for (Integer[] value : values) {
            if (false == validator.visitingValue(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(Double[][] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isArray()) {
            return false;
        }

        ArrayValueValidator validator = new ArrayValueValidator(
                elementProperty.asArray());
        for (Double[] value : values) {
            if (false == validator.visitingValue(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(Boolean[][] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isArray()) {
            return false;
        }

        ArrayValueValidator validator = new ArrayValueValidator(
                elementProperty.asArray());
        for (Boolean[] value : values) {
            if (false == validator.visitingValue(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(String[][] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isArray()) {
            return false;
        }

        ArrayValueValidator validator = new ArrayValueValidator(
                elementProperty.asArray());
        for (String[] value : values) {
            if (false == validator.visitingValue(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(SimulatorResourceModel[][] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isArray()) {
            return false;
        }

        ArrayValueValidator validator = new ArrayValueValidator(
                elementProperty.asArray());
        for (SimulatorResourceModel[] value : values) {
            if (false == validator.visitingValue(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(Integer[][][] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isArray()) {
            return false;
        }

        ArrayValueValidator validator = new ArrayValueValidator(
                elementProperty.asArray());
        for (Integer[][] value : values) {
            if (false == validator.visitingValue(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(Double[][][] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isArray()) {
            return false;
        }

        ArrayValueValidator validator = new ArrayValueValidator(
                elementProperty.asArray());
        for (Double[][] value : values) {
            if (false == validator.visitingValue(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(Boolean[][][] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isArray()) {
            return false;
        }

        ArrayValueValidator validator = new ArrayValueValidator(
                elementProperty.asArray());
        for (Boolean[][] value : values) {
            if (false == validator.visitingValue(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(String[][][] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isArray()) {
            return false;
        }

        ArrayValueValidator validator = new ArrayValueValidator(
                elementProperty.asArray());
        for (String[][] value : values) {
            if (false == validator.visitingValue(value))
                return false;
        }

        return true;
    }

    @Override
    public Boolean visitingValue(SimulatorResourceModel[][][] values) {
        if (null == mProperty)
            return false;

        // Validating length of array
        if (mProperty.hasRange()) {
            if (values.length < mProperty.minItems()
                    || (values.length > mProperty.maxItems() && !mProperty
                            .isVariable())) {
                return false;
            }
        }

        // Validating elements of array
        AttributeProperty elementProperty = mProperty.getElementProperty();
        if (!elementProperty.isArray()) {
            return false;
        }

        ArrayValueValidator validator = new ArrayValueValidator(
                elementProperty.asArray());
        for (SimulatorResourceModel[][] value : values) {
            if (false == validator.visitingValue(value))
                return false;
        }

        return true;
    }
}
