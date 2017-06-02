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

public class ArrayProperty extends AttributeProperty {

    private int               mMin;
    private int               mMax;
    private boolean           mIsVariableSize;
    private boolean           mIsUnique;
    private AttributeProperty mElementProperty;
    private boolean           mHasRange = false;

    private ArrayProperty(int min, int max, boolean variableSize,
            boolean unique, AttributeProperty elementProperty) {
        super(Type.ARRAY);
        mMin = min;
        mMax = max;
        mIsVariableSize = variableSize;
        mIsUnique = unique;
        mElementProperty = elementProperty;
        mHasRange = true;
    }

    private ArrayProperty(boolean variableSize, boolean unique,
            AttributeProperty elementProperty) {
        super(Type.ARRAY);
        mIsVariableSize = variableSize;
        mIsUnique = unique;
        mElementProperty = elementProperty;
    }

    @Override
    public boolean isArray() {
        return true;
    }

    @Override
    public ArrayProperty asArray() {
        return this;
    }

    public boolean hasRange() {
        return mHasRange;
    }

    public int minItems() {
        return mMin;
    }

    public int maxItems() {
        return mMax;
    }

    public boolean isVariable() {
        return mIsVariableSize;
    }

    public boolean isUnique() {
        return mIsUnique;
    }

    public AttributeProperty getElementProperty() {
        return mElementProperty;
    }

    @Override
    public boolean validate(AttributeValue value) {
        return new ArrayValueValidator(this).validate(value);
    }

    public static class Builder {
        private int               mMin;
        private int               mMax;
        private boolean           mIsVariableSize  = false;
        private boolean           mIsUnique        = false;
        private AttributeProperty mElementProperty = null;
        private boolean           mHasRange        = false;

        public void setRange(int minItems, int maxItems) {
            mMin = minItems;
            mMax = maxItems;
            mHasRange = true;
        }

        public void setVariableSize(boolean state) {
            mIsVariableSize = state;
        }

        public void setUnique(boolean state) {
            mIsUnique = state;
        }

        public void setElementProperty(AttributeProperty property) {
            mElementProperty = property;
        }

        public ArrayProperty build() {
            if (null == mElementProperty)
                return null;

            if (mHasRange)
                return new ArrayProperty(mMin, mMax, mIsVariableSize,
                        mIsUnique, mElementProperty);
            return new ArrayProperty(mIsVariableSize, mIsUnique,
                    mElementProperty);
        }
    }
}
