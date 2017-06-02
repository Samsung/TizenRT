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

import java.util.Arrays;

public class IntegerProperty extends AttributeProperty {
    private int     mDefaultValue;
    private int     mMin;
    private int     mMax;
    private int[]   mValues;
    private boolean mHasRange = false;

    private IntegerProperty(int value) {
        super(Type.INTEGER);
        mDefaultValue = value;
    }

    private IntegerProperty(int value, int min, int max) {
        super(Type.INTEGER);
        mDefaultValue = value;
        mMin = min;
        mMax = max;
        mHasRange = true;
    }

    private IntegerProperty(int value, int[] values) {
        super(Type.INTEGER);
        mDefaultValue = value;
        mValues = Arrays.copyOf(values, values.length);
        Arrays.sort(mValues);
    }

    @Override
    public boolean isInteger() {
        return true;
    }

    @Override
    public IntegerProperty asInteger() {
        return this;
    }

    public int getDefaultValue() {
        return mDefaultValue;
    }

    public boolean hasRange() {
        return mHasRange;
    }

    public int min() {
        return mMin;
    }

    public int max() {
        return mMax;
    }

    public boolean hasValues() {
        return (null != mValues && (mValues.length > 0));
    }

    public int[] getValues() {
        return mValues;
    }

    @Override
    public boolean validate(AttributeValue value) {
        return new IntegerValueValidator(this).validate(value);
    }

    public boolean validate(int value) {
        if (mHasRange && (value < mMin || value > mMax)) {
            return false;
        } else if (hasValues() && 0 > Arrays.binarySearch(mValues, value)) {
            return false;
        }

        return true;
    }

    public static class Builder {
        private int     mDefaultValue = 0;
        private int     mMin;
        private int     mMax;
        private int[]   mValues;
        private boolean mHasRange     = false;

        public void setDefaultValue(int value) {
            mDefaultValue = value;
        }

        public void setRange(int min, int max) {
            mMin = min;
            mMax = max;
            mHasRange = true;
        }

        public void setValues(int[] values) {
            mValues = values;
        }

        public IntegerProperty build() {
            if (mHasRange)
                return new IntegerProperty(mDefaultValue, mMin, mMax);
            if (null != mValues && (0 != mValues.length))
                return new IntegerProperty(mDefaultValue, mValues);
            return new IntegerProperty(mDefaultValue);
        }
    }
}
