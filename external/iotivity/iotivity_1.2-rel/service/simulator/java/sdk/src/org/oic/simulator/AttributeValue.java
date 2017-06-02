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
 * This class accommodate different type of values as Object.
 */
public class AttributeValue {

    private Object mValue = null;

    /**
     * Enum to represent value type.
     */
    public enum ValueType {
        UNKNOWN, INTEGER, DOUBLE, BOOLEAN, STRING, RESOURCEMODEL, ARRAY
    }

    /**
     * Class which provides the value type information in detail.
     */
    public static class TypeInfo {
        public ValueType mType;
        public ValueType mBaseType;
        public int       mDepth;

        /**
         * Constructs {@link AttributeValue.TypeInfo} with default values.
         */
        TypeInfo() {
            mType = ValueType.UNKNOWN;
            mBaseType = ValueType.UNKNOWN;
            mDepth = 0;
        }

        /**
         * Constructs {@link AttributeValue.TypeInfo} with given
         * {AttributeValue.ValueType}.
         *
         * @param type
         *            Value type.
         */
        TypeInfo(ValueType type) {
            mType = type;
            mBaseType = type;
            mDepth = 0;
        }

        /**
         * Constructs {@link AttributeValue.TypeInfo} with given
         * {AttributeValue.ValueType}s and depth information.
         *
         * @param type
         *            Value type.
         * @param baseType
         *            Value base type. This type is useful when value is of
         *            array type.
         * @param depth
         *            Depth of array type value. This is useful when value is of
         *            array type.
         */
        TypeInfo(ValueType type, ValueType baseType, int depth) {
            mType = type;
            mBaseType = baseType;
            mDepth = depth;
        }
    }

    /**
     * Constructs {@link AttributeValue} with int type value.
     *
     * @param value
     *            int type value.
     */
    public AttributeValue(int value) {
        mValue = new Integer(value);
    }

    /**
     * Constructs {@link AttributeValue} with double type value.
     *
     * @param value
     *            double type value.
     */
    public AttributeValue(double value) {
        mValue = new Double(value);
    }

    /**
     * Constructs {@link AttributeValue} with boolean type value.
     *
     * @param value
     *            boolean type value.
     */
    public AttributeValue(boolean value) {
        mValue = Boolean.valueOf(value);
    }

    /**
     * Constructs {@link AttributeValue} with string type value.
     *
     * @param value
     *            String type value.
     */
    public AttributeValue(String value) {
        mValue = value;
    }

    /**
     * Constructs {@link AttributeValue} with SimulatorResourceModel type value.
     *
     * @param value
     *            SimulatorResourceModel type value.
     */
    public AttributeValue(SimulatorResourceModel value) {
        mValue = value;
    }

    /**
     * Constructs {@link AttributeValue} with array of int type values.
     *
     * @param values
     *            Array of int type values.
     */
    public AttributeValue(int[] values) {
        Integer[] newValues = new Integer[values.length];
        for (int i = 0; i < values.length; i++)
            newValues[i] = Integer.valueOf(values[i]);
        mValue = newValues;
    }

    /**
     * Constructs {@link AttributeValue} with array of Integer type values.
     *
     * @param values
     *            Array of Integer type values.
     */
    public AttributeValue(Integer[] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with array of double type values.
     *
     * @param values
     *            Array of double type values.
     */
    public AttributeValue(double[] values) {
        Double[] newValues = new Double[values.length];
        for (int i = 0; i < values.length; i++)
            newValues[i] = Double.valueOf(values[i]);
        mValue = newValues;
    }

    /**
     * Constructs {@link AttributeValue} with array of Double type values.
     *
     * @param values
     *            Array of Double type values.
     */
    public AttributeValue(Double[] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with array of boolean type values.
     *
     * @param values
     *            Array of boolean type values.
     */
    public AttributeValue(boolean[] values) {
        Boolean[] newValues = new Boolean[values.length];
        for (int i = 0; i < values.length; i++)
            newValues[i] = Boolean.valueOf(values[i]);
        mValue = newValues;
    }

    /**
     * Constructs {@link AttributeValue} with array of Boolean type values.
     *
     * @param values
     *            Array of Boolean type values.
     */
    public AttributeValue(Boolean[] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with array of String type values.
     *
     * @param values
     *            Array of String type values.
     */
    public AttributeValue(String[] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with array of SimulatorResourceModel
     * type values.
     *
     * @param values
     *            Array of SimulatorResourceModel type values.
     */
    public AttributeValue(SimulatorResourceModel[] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with 2 dimensional array of int type
     * values.
     *
     * @param values
     *            2 dimensional array of int type values.
     */
    public AttributeValue(int[][] values) {
        Integer[][] newValues = new Integer[values.length][];
        for (int i = 0; i < values.length; i++) {
            newValues[i] = new Integer[values[i].length];
            for (int j = 0; j < values[i].length; j++) {
                newValues[i][j] = Integer.valueOf(values[i][j]);
            }
        }
        mValue = newValues;
    }

    /**
     * Constructs {@link AttributeValue} with 2 dimensional array of Integer
     * type values.
     *
     * @param values
     *            2 dimensional array of Integer type values.
     */
    public AttributeValue(Integer[][] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with 2 dimensional array of double type
     * values.
     *
     * @param values
     *            2 dimensional array of double type values.
     */
    public AttributeValue(double[][] values) {
        Double[][] newValues = new Double[values.length][];
        for (int i = 0; i < values.length; i++) {
            newValues[i] = new Double[values[i].length];
            for (int j = 0; j < values[i].length; j++) {
                newValues[i][j] = Double.valueOf(values[i][j]);
            }
        }
        mValue = newValues;
    }

    /**
     * Constructs {@link AttributeValue} with 2 dimensional array of Double type
     * values.
     *
     * @param values
     *            2 dimensional array of Double type values.
     */
    public AttributeValue(Double[][] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with 2 dimensional array of boolean
     * type values.
     *
     * @param values
     *            2 dimensional array of boolean type values.
     */
    public AttributeValue(boolean[][] values) {
        Boolean[][] newValues = new Boolean[values.length][];
        for (int i = 0; i < values.length; i++) {
            newValues[i] = new Boolean[values[i].length];
            for (int j = 0; j < values[i].length; j++) {
                newValues[i][j] = Boolean.valueOf(values[i][j]);
            }
        }
        mValue = newValues;
    }

    /**
     * Constructs {@link AttributeValue} with 2 dimensional array of Boolean
     * type values.
     *
     * @param values
     *            2 dimensional array of Boolean type values.
     */
    public AttributeValue(Boolean[][] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with 2 dimensional array of String type
     * values.
     *
     * @param values
     *            2 dimensional array of String type values.
     */
    public AttributeValue(String[][] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with 2 dimensional array of
     * SimulatorResourceModel type values.
     *
     * @param values
     *            2 dimensional array of SimulatorResourceModel type values.
     */
    public AttributeValue(SimulatorResourceModel[][] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with 3 dimensional array of int type
     * values.
     *
     * @param values
     *            3 dimensional array of int type values.
     */
    public AttributeValue(int[][][] values) {
        Integer[][][] newValues = new Integer[values.length][][];
        for (int i = 0; i < values.length; i++) {
            newValues[i] = new Integer[values[i].length][];
            for (int j = 0; j < values[i].length; j++) {
                newValues[i][j] = new Integer[values[i][j].length];
                for (int k = 0; k < values[j].length; k++) {
                    newValues[i][j][k] = Integer.valueOf(values[i][j][k]);
                }
            }
        }
        mValue = newValues;
    }

    /**
     * Constructs {@link AttributeValue} with 3 dimensional array of Integer
     * type values.
     *
     * @param values
     *            3 dimensional array of Integer type values.
     */
    public AttributeValue(Integer[][][] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with 3 dimensional array of double type
     * values.
     *
     * @param values
     *            3 dimensional array of double type values.
     */
    public AttributeValue(double[][][] values) {
        Double[][][] newValues = new Double[values.length][][];
        for (int i = 0; i < values.length; i++) {
            newValues[i] = new Double[values[i].length][];
            for (int j = 0; j < values[i].length; j++) {
                newValues[i][j] = new Double[values[i][j].length];
                for (int k = 0; k < values[j].length; k++) {
                    newValues[i][j][k] = Double.valueOf(values[i][j][k]);
                }
            }
        }
        mValue = newValues;
    }

    /**
     * Constructs {@link AttributeValue} with 3 dimensional array of Double type
     * values.
     *
     * @param values
     *            3 dimensional array of Double type values.
     */
    public AttributeValue(Double[][][] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with 3 dimensional array of boolean
     * type values.
     *
     * @param values
     *            3 dimensional array of boolean type values.
     */
    public AttributeValue(boolean[][][] values) {
        Boolean[][][] newValues = new Boolean[values.length][][];
        for (int i = 0; i < values.length; i++) {
            newValues[i] = new Boolean[values[i].length][];
            for (int j = 0; j < values[i].length; j++) {
                newValues[i][j] = new Boolean[values[i][j].length];
                for (int k = 0; k < values[j].length; k++) {
                    newValues[i][j][k] = Boolean.valueOf(values[i][j][k]);
                }
            }
        }
        mValue = newValues;
    }

    /**
     * Constructs {@link AttributeValue} with 3 dimensional array of Boolean
     * type values.
     *
     * @param values
     *            3 dimensional array of Boolean type values.
     */
    public AttributeValue(Boolean[][][] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with 3 dimensional array of String type
     * values.
     *
     * @param values
     *            3 dimensional array of String type values.
     */
    public AttributeValue(String[][][] values) {
        mValue = values;
    }

    /**
     * Constructs {@link AttributeValue} with 3 dimensional array of
     * SimulatorResourceModel type values.
     *
     * @param values
     *            3 dimensional array of SimulatorResourceModel type values.
     */
    public AttributeValue(SimulatorResourceModel[][][] values) {
        mValue = values;
    }

    /**
     * API to get value type information.
     *
     * @return {@link AttributeValue.TypeInfo}.
     */
    public TypeInfo typeInfo() {
        return createTypeInfo(mValue);
    }

    /**
     * API to get value as Object.
     *
     * @return Value as Object.
     */
    public Object get() {
        return mValue;
    }

    private AttributeValue(Object value) {
        mValue = value;
    }

    private TypeInfo createTypeInfo(Object value) {
        TypeInfo typeInfo = new TypeInfo();
        String className = value.getClass().getName();
        if (className.contains(Integer.class.getName())) {
            typeInfo.mBaseType = ValueType.INTEGER;
            typeInfo.mType = ValueType.INTEGER;
        } else if (className.contains(Double.class.getName())) {
            typeInfo.mBaseType = ValueType.DOUBLE;
            typeInfo.mType = ValueType.DOUBLE;
        } else if (className.contains(Boolean.class.getName())) {
            typeInfo.mBaseType = ValueType.BOOLEAN;
            typeInfo.mType = ValueType.BOOLEAN;
        } else if (className.contains(String.class.getName())) {
            typeInfo.mBaseType = ValueType.STRING;
            typeInfo.mType = ValueType.STRING;
        } else if (className.contains(SimulatorResourceModel.class.getName())) {
            typeInfo.mBaseType = ValueType.RESOURCEMODEL;
            typeInfo.mType = ValueType.RESOURCEMODEL;
        }

        // For array types
        if (value.getClass().isArray()) {
            typeInfo.mType = ValueType.ARRAY;
            for (char ch : className.toCharArray()) {
                if (ch == '[')
                    typeInfo.mDepth++;
            }
        }

        return typeInfo;
    }
}
