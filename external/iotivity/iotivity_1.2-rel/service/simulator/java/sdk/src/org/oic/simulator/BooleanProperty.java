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

public class BooleanProperty extends AttributeProperty {
    private boolean mDefaultValue;

    private BooleanProperty(boolean value) {
        super(Type.BOOLEAN);
        mDefaultValue = value;
    }

    @Override
    public boolean isBoolean() {
        return true;
    }

    @Override
    public BooleanProperty asBoolean() {
        return this;
    }

    public boolean getDefaultValue() {
        return mDefaultValue;
    }

    @Override
    public boolean validate(AttributeValue value) {
        if (value.typeInfo().mType == AttributeValue.ValueType.BOOLEAN)
            return true;
        return false;
    }

    public boolean validate(boolean value) {
        return true;
    }

    public static class Builder {
        private boolean mDefaultValue = true;

        public void setDefaultValue(boolean value) {
            mDefaultValue = value;
        }

        public BooleanProperty build() {
            return new BooleanProperty(mDefaultValue);
        }
    }
}