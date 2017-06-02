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

public class DoubleProperty extends AttributeProperty {

    private double   mDefaultValue;
    private double   mMin;
    private double   mMax;
    private double[] mValues;
    private boolean  mHasRange = false;

    private DoubleProperty(double value) {
        super(Type.DOUBLE);
        mDefaultValue = value;
    }

    private DoubleProperty(double value, double min, double max) {
        super(Type.DOUBLE);
        mDefaultValue = value;
        mMin = min;
        mMax = max;
        mHasRange = true;
    }

    private DoubleProperty(double value, double[] values) {
        super(Type.DOUBLE);
        mDefaultValue = value;
        mValues = Arrays.copyOf(values, values.length);
        Arrays.sort(mValues);
    }

    @Override
    public boolean isDouble() {
        return true;
    }

    @Override
    public DoubleProperty asDouble() {
        return this;
    }

    public double getDefaultValue() {
        return mDefaultValue;
    }

    public boolean hasRange() {
        return mHasRange;
    }

    public double min() {
        return mMin;
    }

    public double max() {
        return mMax;
    }

    public boolean hasValues() {
        return (null != mValues && (mValues.length > 0));
    }

    public double[] getValues() {
        return mValues;
    }

    @Override
    public boolean validate(AttributeValue value) {
        return new DoubleValueValidator(this).validate(value);
    }

    public boolean validate(double value) {
        if (mHasRange && (value < mMin || value > mMax)) {
            return false;
        } else if (hasValues() && 0 > Arrays.binarySearch(mValues, value)) {
            return false;
        }

        return true;
    }

    public static class Builder {
        private double   mDefaultValue = 0;
        private double   mMin;
        private double   mMax;
        private double[] mValues;
        private boolean  mHasRange     = false;

        public void setDefaultValue(double value) {
            mDefaultValue = value;
        }

        public void setRange(double min, double max) {
            mMin = min;
            mMax = max;
            mHasRange = true;
        }

        public void setValues(double[] values) {
            mValues = values;
        }

        public DoubleProperty build() {
            if (mHasRange)
                return new DoubleProperty(mDefaultValue, mMin, mMax);
            if (null != mValues && (0 != mValues.length))
                return new DoubleProperty(mDefaultValue, mValues);
            return new DoubleProperty(mDefaultValue);
        }
    }
}
