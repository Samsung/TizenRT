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

package oic.simulator.serviceprovider.utils;

import java.util.Vector;

import org.oic.simulator.AttributeValue;

public class AttributeValueBuilder {
    public static AttributeValue build(String valueString,
            AttributeValue.ValueType valueType) throws NullPointerException {
        int depth = findDepth(valueString);
        if (0 == depth) {
            return handleDepth0(valueString, valueType);
        } else if (1 == depth) {
            return handleDepth1(valueString, valueType);
        } else if (2 == depth) {
            return handleDepth2(valueString, valueType);
        } else if (3 == depth) {
            return handleDepth3(valueString, valueType);
        }

        return null;
    }

    private static int findDepth(String value) {
        int depth = 0;
        for (char ch : value.toCharArray()) {
            if (ch == '[')
                depth++;
            else
                break;
        }

        return depth;
    }

    private static boolean isValidSyntax(String value) {
        int count = 0;
        for (char ch : value.toCharArray()) {
            if (ch == '[')
                count++;
            if (ch == ']')
                count--;
        }

        if (count == 0)
            return true;
        return false;

    }

    private static AttributeValue handleDepth0(String valueString,
            AttributeValue.ValueType valueType) {
        valueString = valueString.trim();
        if (0 != findDepth(valueString))
            return null;

        try {
            if (valueType == AttributeValue.ValueType.INTEGER)
                return new AttributeValue(Integer.parseInt(valueString));
            else if (valueType == AttributeValue.ValueType.DOUBLE) {
                Double value = Double.parseDouble(valueString);
                if (!value.isInfinite()) {
                    return new AttributeValue(value);
                }
            } else if (valueType == AttributeValue.ValueType.BOOLEAN) {
                if (valueString.equalsIgnoreCase("true")
                        || valueString.equalsIgnoreCase("false"))
                    return new AttributeValue(Boolean.parseBoolean(valueString));
            } else if (valueType == AttributeValue.ValueType.STRING)
                return new AttributeValue(valueString);
        } catch (Exception e) {
            return null;
        }
        return null;
    }

    private static String[] splitIntoArrays(String valueString) {
        Vector<String> values = new Vector<String>();
        valueString = valueString.substring(valueString.indexOf('[') + 1,
                valueString.lastIndexOf(']'));

        int count = 0;
        int startPos = 0;
        char[] charArray = valueString.toCharArray();
        for (int index = 0; index < charArray.length; index++) {
            if (charArray[index] == '[' && 0 == count++) {
                startPos = index;
            }

            if (charArray[index] == ']' && 0 == --count) {
                values.add(valueString.substring(startPos, index + 1));
            }
        }

        String[] result = new String[values.size()];
        values.toArray(result);
        return result;
    }

    private static AttributeValue handleDepth1(String valueString,
            AttributeValue.ValueType valueType) {
        valueString = valueString.trim();
        if (1 != findDepth(valueString) || false == isValidSyntax(valueString))
            return null;

        valueString = valueString.substring(valueString.indexOf('[') + 1,
                valueString.lastIndexOf(']'));
        String[] valuesString = valueString.split(",");
        if (null == valuesString || 0 == valuesString.length)
            return null;

        if (valueType == AttributeValue.ValueType.INTEGER) {
            if (1 == valuesString.length && valuesString[0].isEmpty()) {
                return new AttributeValue(new Integer[0]);
            }

            Integer[] result = new Integer[valuesString.length];
            for (int index = 0; index < valuesString.length; index++) {
                if (null != valuesString[index]
                        && !valuesString[index].isEmpty()) {
                    AttributeValue attValue = handleDepth0(valuesString[index],
                            valueType);
                    if (null == attValue)
                        return null;

                    Integer value = (Integer) attValue.get();
                    if (null == value)
                        return null;
                    result[index] = value;
                }
            }
            return new AttributeValue(result);
        } else if (valueType == AttributeValue.ValueType.DOUBLE) {
            if (1 == valuesString.length && valuesString[0].isEmpty()) {
                return new AttributeValue(new Double[0]);
            }

            Double[] result = new Double[valuesString.length];
            for (int index = 0; index < valuesString.length; index++) {
                if (null != valuesString[index]
                        && !valuesString[index].isEmpty()) {
                    AttributeValue attValue = handleDepth0(valuesString[index],
                            valueType);
                    if (null == attValue)
                        return null;

                    Double value = (Double) attValue.get();
                    if (null == value)
                        return null;
                    result[index] = value;
                }
            }
            return new AttributeValue(result);
        } else if (valueType == AttributeValue.ValueType.BOOLEAN) {
            if (1 == valuesString.length && valuesString[0].isEmpty()) {
                return new AttributeValue(new Boolean[0]);
            }

            Boolean[] result = new Boolean[valuesString.length];
            for (int index = 0; index < valuesString.length; index++) {
                if (null != valuesString[index]
                        && !valuesString[index].isEmpty()) {
                    AttributeValue attValue = handleDepth0(valuesString[index],
                            valueType);
                    if (null == attValue)
                        return null;

                    Boolean value = (Boolean) attValue.get();
                    if (null == value)
                        return null;
                    result[index] = value;
                }
            }
            return new AttributeValue(result);
        } else if (valueType == AttributeValue.ValueType.STRING) {
            if (1 == valuesString.length && valuesString[0].isEmpty()) {
                return new AttributeValue(new String[0]);
            }

            for (int index = 0; index < valuesString.length; index++) {
                if (null != valuesString[index]
                        && !valuesString[index].isEmpty()) {
                    valuesString[index] = valuesString[index].trim();
                }
            }
            return new AttributeValue(valuesString);
        }

        return null;
    }

    private static AttributeValue handleDepth2(String valueString,
            AttributeValue.ValueType valueType) {
        valueString = valueString.trim();
        if (2 != findDepth(valueString) || false == isValidSyntax(valueString))
            return null;

        String[] valuesString = splitIntoArrays(valueString);
        if (null == valuesString || 0 == valuesString.length)
            return null;

        if (valueType == AttributeValue.ValueType.INTEGER) {
            Integer[][] result = new Integer[valuesString.length][];
            for (int index = 0; index < valuesString.length; index++) {
                AttributeValue attValue = handleDepth1(valuesString[index],
                        valueType);
                if (null == attValue)
                    return null;

                Integer[] value = (Integer[]) attValue.get();
                if (null == value)
                    return null;
                result[index] = value;
            }
            return new AttributeValue(result);
        } else if (valueType == AttributeValue.ValueType.DOUBLE) {
            Double[][] result = new Double[valuesString.length][];
            for (int index = 0; index < valuesString.length; index++) {
                AttributeValue attValue = handleDepth1(valuesString[index],
                        valueType);
                if (null == attValue)
                    return null;

                Double[] value = (Double[]) attValue.get();
                if (null == value)
                    return null;
                result[index] = value;
            }
            return new AttributeValue(result);
        } else if (valueType == AttributeValue.ValueType.BOOLEAN) {
            Boolean[][] result = new Boolean[valuesString.length][];
            for (int index = 0; index < valuesString.length; index++) {
                AttributeValue attValue = handleDepth1(valuesString[index],
                        valueType);
                if (null == attValue)
                    return null;

                Boolean[] value = (Boolean[]) attValue.get();
                if (null == value)
                    return null;
                result[index] = value;
            }
            return new AttributeValue(result);
        } else if (valueType == AttributeValue.ValueType.STRING) {
            String[][] result = new String[valuesString.length][];
            for (int index = 0; index < valuesString.length; index++) {
                AttributeValue attValue = handleDepth1(valuesString[index],
                        valueType);
                if (null == attValue)
                    return null;

                String[] value = (String[]) attValue.get();
                if (null == value)
                    return null;
                result[index] = value;
            }
            return new AttributeValue(result);
        }

        return null;
    }

    public static AttributeValue handleDepth3(String valueString,
            AttributeValue.ValueType valueType) {
        valueString = valueString.trim();
        if (3 != findDepth(valueString) || false == isValidSyntax(valueString))
            return null;

        String[] valuesString = splitIntoArrays(valueString);
        if (null == valuesString || 0 == valuesString.length)
            return null;

        if (valueType == AttributeValue.ValueType.INTEGER) {
            Integer[][][] result = new Integer[valuesString.length][][];
            for (int index = 0; index < valuesString.length; index++) {
                AttributeValue attValue = handleDepth2(valuesString[index],
                        valueType);
                if (null == attValue)
                    return null;

                Integer[][] value = (Integer[][]) attValue.get();
                if (null == value)
                    return null;
                result[index] = value;
            }
            return new AttributeValue(result);
        } else if (valueType == AttributeValue.ValueType.DOUBLE) {
            Double[][][] result = new Double[valuesString.length][][];
            for (int index = 0; index < valuesString.length; index++) {
                AttributeValue attValue = handleDepth2(valuesString[index],
                        valueType);
                if (null == attValue)
                    return null;

                Double[][] value = (Double[][]) attValue.get();
                if (null == value)
                    return null;
                result[index] = value;
            }
            return new AttributeValue(result);
        } else if (valueType == AttributeValue.ValueType.BOOLEAN) {
            Boolean[][][] result = new Boolean[valuesString.length][][];
            for (int index = 0; index < valuesString.length; index++) {
                AttributeValue attValue = handleDepth2(valuesString[index],
                        valueType);
                if (null == attValue)
                    return null;

                Boolean[][] value = (Boolean[][]) attValue.get();
                if (null == value)
                    return null;
                result[index] = value;
            }
            return new AttributeValue(result);
        } else if (valueType == AttributeValue.ValueType.STRING) {
            String[][][] result = new String[valuesString.length][][];
            for (int index = 0; index < valuesString.length; index++) {
                AttributeValue attValue = handleDepth2(valuesString[index],
                        valueType);
                if (null == attValue)
                    return null;

                String[][] value = (String[][]) attValue.get();
                if (null == value)
                    return null;
                result[index] = value;
            }
            return new AttributeValue(result);
        }

        return null;
    }
}
