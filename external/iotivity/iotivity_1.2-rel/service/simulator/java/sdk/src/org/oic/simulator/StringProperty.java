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

public class StringProperty extends AttributeProperty {

    private String   mDefaultValue;
    private int      mMin;
    private int      mMax;
    private String[] mValues;
    private boolean  mHasRange = false;

    private StringProperty(String value) {
        super(Type.STRING);
        mDefaultValue = value;
    }

    private StringProperty(String value, int min, int max) {
        super(Type.STRING);
        mDefaultValue = value;
        mMin = min;
        mMax = max;
        mHasRange = true;
    }

    private StringProperty(String value, String[] values) {
        super(Type.STRING);
        mDefaultValue = value;
        mValues = Arrays.copyOf(values, values.length);
        Arrays.sort(mValues);
    }

    @Override
    public boolean isString() {
        return true;
    }

    @Override
    public StringProperty asString() {
        return this;
    }

    public String getDefaultValue() {
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

    public String[] getValues() {
        return mValues;
    }

    @Override
    public boolean validate(AttributeValue value) {
        return new StringValueValidator(this).validate(value);
    }

    public boolean validate(String value) {
        int length = value.length();
        if (mHasRange && (length < mMin || length > mMax)) {
            return false;
        } else if (hasValues() && 0 > Arrays.binarySearch(mValues, value)) {
            return false;
        }

        return true;
    }

    public static class Builder {
        private String   mDefaultValue;
        private int      mMin;
        private int      mMax;
        private String[] mValues;
        private boolean  mHasRange = false;

        public void setDefaultValue(String value) {
            if (null != value)
                mDefaultValue = value;
        }

        public void setRange(int min, int max) {
            mMin = min;
            mMax = max;
            mHasRange = true;
        }

        public void setValues(String[] values) {
            mValues = values;
        }

        public StringProperty build() {
            if (mHasRange)
                return new StringProperty(mDefaultValue, mMin, mMax);
            if (null != mValues && (0 != mValues.length))
                return new StringProperty(mDefaultValue, mValues);
            return new StringProperty(mDefaultValue);
        }
    }
}
