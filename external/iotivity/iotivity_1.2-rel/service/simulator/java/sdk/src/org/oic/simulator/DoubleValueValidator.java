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

class DoubleValueValidator implements
        AttributeValueVisitor.VisitingMethods<Boolean> {

    private DoubleProperty mProperty = null;

    DoubleValueValidator(DoubleProperty property) {
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
        if (null != mProperty)
            return mProperty.validate(value);
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
    public Boolean visitingValue(Integer[] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(Double[] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(Boolean[] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(String[] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(SimulatorResourceModel[] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(Integer[][] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(Double[][] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(Boolean[][] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(String[][] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(SimulatorResourceModel[][] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(Integer[][][] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(Double[][][] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(Boolean[][][] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(String[][][] value) {
        return false;
    }

    @Override
    public Boolean visitingValue(SimulatorResourceModel[][][] value) {
        return false;
    }
}
