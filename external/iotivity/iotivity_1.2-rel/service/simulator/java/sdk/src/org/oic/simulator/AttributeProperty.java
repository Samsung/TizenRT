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
 * This class represents the resource attribute's value property.
 */
public abstract class AttributeProperty {

    public enum Type {
        INTEGER, DOUBLE, BOOLEAN, STRING, MODEL, ARRAY
    }

    private Type mType;

    protected AttributeProperty(Type type) {
        mType = type;
    }

    public Type getType() {
        return mType;
    }

    public boolean isInteger() {
        return false;
    }

    public boolean isDouble() {
        return false;
    }

    public boolean isBoolean() {
        return false;
    }

    public boolean isString() {
        return false;
    }

    public boolean isArray() {
        return false;
    }

    public boolean isModel() {
        return false;
    }

    public IntegerProperty asInteger() {
        return null;
    }

    public DoubleProperty asDouble() {
        return null;
    }

    public BooleanProperty asBoolean() {
        return null;
    }

    public StringProperty asString() {
        return null;
    }

    public ArrayProperty asArray() {
        return null;
    }

    public ModelProperty asModel() {
        return null;
    }

    public abstract boolean validate(AttributeValue value);
}
