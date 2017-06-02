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
 * This class visits the given attribute value and invokes the right method
 * based on the type of the attribute value.
 */
public class AttributeValueVisitor {
    private AttributeValue  mValue;
    private VisitingMethods mListener;

    public interface VisitingMethods<T extends Object> {
        public T visitingValue(Integer value);

        public T visitingValue(Double value);

        public T visitingValue(Boolean value);

        public T visitingValue(String value);

        public T visitingValue(SimulatorResourceModel value);

        public T visitingValue(Integer[] values);

        public T visitingValue(Double[] values);

        public T visitingValue(Boolean[] values);

        public T visitingValue(String[] values);

        public T visitingValue(SimulatorResourceModel[] values);

        public T visitingValue(Integer[][] values);

        public T visitingValue(Double[][] values);

        public T visitingValue(Boolean[][] values);

        public T visitingValue(String[][] values);

        public T visitingValue(SimulatorResourceModel[][] values);

        public T visitingValue(Integer[][][] values);

        public T visitingValue(Double[][][] values);

        public T visitingValue(Boolean[][][] values);

        public T visitingValue(String[][][] values);

        public T visitingValue(SimulatorResourceModel[][][] values);
    }

    /**
     * Constructs {@link AttributeValueVisitor} from the given
     * {@link AttributeValue} and the implementation of the
     * {@link VisitingMethods} interface.
     *
     * @param value
     *            {@link AttributeValue} object.
     * @param listener
     *            Implementation of {@link VisitingMethods} interface.
     */
    public AttributeValueVisitor(AttributeValue value, VisitingMethods listener) {
        mValue = value;
        mListener = listener;
    }

    /**
     * API which processes an {@link AttributeValue} to find its value type and
     * convert the Object to the appropriate type for invoking the right
     * visitor(method) in the {@link VisitingMethods} interface.
     *
     * @return Object which holds the value returned by the visitor method.
     */
    public Object visit() {
        if (null == mValue || null == mListener)
            return null;

        AttributeValue.TypeInfo typeInfo = mValue.typeInfo();
        if (AttributeValue.ValueType.INTEGER == typeInfo.mBaseType) {
            if (AttributeValue.ValueType.INTEGER == typeInfo.mType)
                return mListener.visitingValue((Integer) mValue.get());
            else if (AttributeValue.ValueType.ARRAY == typeInfo.mType) {
                if (1 == typeInfo.mDepth)
                    return mListener.visitingValue((Integer[]) mValue.get());
                if (2 == typeInfo.mDepth)
                    return mListener.visitingValue((Integer[][]) mValue.get());
                if (3 == typeInfo.mDepth)
                    return mListener
                            .visitingValue((Integer[][][]) mValue.get());
            }
        } else if (AttributeValue.ValueType.DOUBLE == typeInfo.mBaseType) {
            if (AttributeValue.ValueType.DOUBLE == typeInfo.mType)
                return mListener.visitingValue((Double) mValue.get());
            else if (AttributeValue.ValueType.ARRAY == typeInfo.mType) {
                if (1 == typeInfo.mDepth)
                    return mListener.visitingValue((Double[]) mValue.get());
                if (2 == typeInfo.mDepth)
                    return mListener.visitingValue((Double[][]) mValue.get());
                if (3 == typeInfo.mDepth)
                    return mListener.visitingValue((Double[][][]) mValue.get());
            }
        } else if (AttributeValue.ValueType.BOOLEAN == typeInfo.mBaseType) {
            if (AttributeValue.ValueType.BOOLEAN == typeInfo.mType)
                return mListener.visitingValue((Boolean) mValue.get());
            else if (AttributeValue.ValueType.ARRAY == typeInfo.mType) {
                if (1 == typeInfo.mDepth)
                    return mListener.visitingValue((Boolean[]) mValue.get());
                if (2 == typeInfo.mDepth)
                    return mListener.visitingValue((Boolean[][]) mValue.get());
                if (3 == typeInfo.mDepth)
                    return mListener
                            .visitingValue((Boolean[][][]) mValue.get());
            }
        } else if (AttributeValue.ValueType.STRING == typeInfo.mBaseType) {
            if (AttributeValue.ValueType.STRING == typeInfo.mType)
                return mListener.visitingValue((String) mValue.get());
            else if (AttributeValue.ValueType.ARRAY == typeInfo.mType) {
                if (1 == typeInfo.mDepth)
                    return mListener.visitingValue((String[]) mValue.get());
                if (2 == typeInfo.mDepth)
                    return mListener.visitingValue((String[][]) mValue.get());
                if (3 == typeInfo.mDepth)
                    return mListener.visitingValue((String[][][]) mValue.get());
            }
        } else if (AttributeValue.ValueType.RESOURCEMODEL == typeInfo.mBaseType) {
            if (AttributeValue.ValueType.RESOURCEMODEL == typeInfo.mType)
                return mListener.visitingValue((SimulatorResourceModel) mValue
                        .get());
            else if (AttributeValue.ValueType.ARRAY == typeInfo.mType) {
                if (1 == typeInfo.mDepth)
                    return mListener
                            .visitingValue((SimulatorResourceModel[]) mValue
                                    .get());
                if (2 == typeInfo.mDepth)
                    return mListener
                            .visitingValue((SimulatorResourceModel[][]) mValue
                                    .get());
                if (3 == typeInfo.mDepth)
                    return mListener
                            .visitingValue((SimulatorResourceModel[][][]) mValue
                                    .get());
            }
        }

        return null;
    }
}
