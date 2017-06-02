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

package oic.simulator.serviceprovider.model;

import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import org.oic.simulator.AttributeProperty;
import org.oic.simulator.AttributeValue;
import org.oic.simulator.AttributeValue.ValueType;
import org.oic.simulator.BooleanProperty;
import org.oic.simulator.DoubleProperty;
import org.oic.simulator.IntegerProperty;
import org.oic.simulator.SimulatorResourceAttribute;
import org.oic.simulator.StringProperty;

import oic.simulator.serviceprovider.utils.Constants;
import oic.simulator.serviceprovider.utils.Utility;

public class AttributeHelper {
    private String      attributeName;
    private String      attributeType;
    private String      min;
    private String      max;
    private Set<String> allowedValues;
    private String      attributeDflValue;

    public enum ValidValuesType {
        RANGE, VALUESET, UNKNOWN
    }

    private ValidValuesType validValuesType;

    public AttributeHelper() {
        allowedValues = new HashSet<String>();
        setValidValuesType(ValidValuesType.UNKNOWN);
        min = max = "";
    }

    public AttributeHelper clone() {
        AttributeHelper att = new AttributeHelper();
        att.setAttributeName(attributeName);
        att.setAttributeType(attributeType);
        att.setMin(min);
        att.setMax(max);
        if (null != allowedValues) {
            Set<String> values = new HashSet<String>();
            values.addAll(allowedValues);
            att.setAllowedValues(values);
        }
        att.setAttributeDflValue(attributeDflValue);
        att.setValidValuesType(validValuesType);
        return att;
    }

    public String toString() {
        return attributeName + "," + attributeType + "," + attributeDflValue
                + "," + validValuesType + ",(" + min + max + "),("
                + allowedValues + ")";
    }

    public String getAttributeName() {
        return attributeName;
    }

    public void setAttributeName(String attributeName) {
        this.attributeName = attributeName;
    }

    public String getAttributeType() {
        return attributeType;
    }

    public void setAttributeType(String attributeType) {
        this.attributeType = attributeType;
    }

    public String getMin() {
        return min;
    }

    public void setMin(String min) {
        this.min = min;
    }

    public String getMax() {
        return max;
    }

    public void setMax(String max) {
        this.max = max;
    }

    public Set<String> getAllowedValues() {
        return allowedValues;
    }

    public void setAllowedValues(Set<String> allowedValues) {
        this.allowedValues = allowedValues;
    }

    public void addValueToAllowedValues(String value) {
        if (null != value && value.trim().length() > 0) {
            if (null == allowedValues) {
                allowedValues = new HashSet<String>();
            }
            allowedValues.add(value);
        }
    }

    public void removeValueFromAllowedValues(String value) {
        if (null != allowedValues && null != value && value.trim().length() > 0) {
            allowedValues.remove(value);
        }
    }

    public void removeAllCustomValues() {
        allowedValues.clear();
    }

    public String getAttributeDflValue() {
        return attributeDflValue;
    }

    public void setAttributeDflValue(String attributeDflValue) {
        this.attributeDflValue = attributeDflValue;
    }

    public ValidValuesType getValidValuesType() {
        return validValuesType;
    }

    public void setValidValuesType(ValidValuesType validValuesType) {
        this.validValuesType = validValuesType;
    }

    public boolean isValueValid(String value, String type) {
        if (null == value || value.trim().length() < 1 || null == type
                || type.trim().length() < 1)
            return false;
        if (type.equalsIgnoreCase(Constants.INT)) {
            return isIntValue(value);
        } else if (type.equalsIgnoreCase(Constants.DOUBLE)) {
            return isDoubleValue(value);
        } else if (type.equalsIgnoreCase(Constants.BOOL)) {
            return isBoolValue(value);
        }
        return true;
    }

    public boolean isBoolValue(String value) {
        boolean isBool = false;
        if (null != value
                && (value.equalsIgnoreCase("true") || value
                        .equalsIgnoreCase("false"))) {
            isBool = true;
        }
        return isBool;
    }

    public boolean isIntValue(String value) {
        boolean isInt = true;
        try {
            Integer.parseInt(value);
        } catch (Exception e) {
            isInt = false;
        }
        return isInt;
    }

    public boolean isDoubleValue(String value) {
        boolean isDouble = true;
        try {
            Double.parseDouble(value);
        } catch (Exception e) {
            isDouble = false;
        }
        return isDouble;
    }

    public boolean isRangeValid(String minStr, String maxStr, String type) {
        if (null == type || type.trim().isEmpty()) {
            return false;
        }
        boolean result = true;
        if (type.equals(Constants.INT)) {
            int min, max;
            try {
                min = Integer.parseInt(minStr);
                max = Integer.parseInt(maxStr);
                if (max <= min) {
                    result = false;
                }
            } catch (Exception e) {
                result = false;
            }
        } else if (type.equals(Constants.DOUBLE)) {
            double min, max;
            try {
                min = Double.parseDouble(minStr);
                max = Double.parseDouble(maxStr);
                if (max <= min) {
                    result = false;
                }
            } catch (Exception e) {
                result = false;
            }
        }
        return result;
    }

    public boolean isDefaultValueValid(String value) {
        if (null == attributeType || attributeType.trim().isEmpty()) {
            return false;
        }
        boolean result = true;
        if (attributeType.equals(Constants.STRING)) {
            if (validValuesType == ValidValuesType.VALUESET
                    && null != allowedValues) {
                result = allowedValues.contains(value);
            } else {
                result = true;
            }
        } else if (attributeType.equals(Constants.BOOL)) {
            String val = value.toLowerCase();
            if (!(val.equals("true") || val.equals("false"))) {
                result = false;
            }
        } else {
            if (validValuesType == ValidValuesType.RANGE) {
                if (attributeType.equals(Constants.INT)) {
                    int min, max, dflValue;
                    try {
                        min = Integer.parseInt(this.min);
                        max = Integer.parseInt(this.max);
                        dflValue = Integer.parseInt(value);
                        if (dflValue < min || dflValue > max) {
                            result = false;
                        }
                    } catch (Exception e) {
                        result = false;
                    }
                } else if (attributeType.equals(Constants.DOUBLE)) {
                    double min, max, dflValue;
                    try {
                        min = Double.parseDouble(this.min);
                        max = Double.parseDouble(this.max);
                        dflValue = Double.parseDouble(value);
                        boolean found = false;
                        for (double val = min; val <= max; val += 1.0) {
                            if (Double.compare(dflValue, val) == 0) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            result = false;
                        }
                    } catch (Exception e) {
                        result = false;
                    }
                }
            } else if (validValuesType == ValidValuesType.VALUESET
                    && null != allowedValues && !allowedValues.isEmpty()) {
                boolean found = false;
                if (attributeType.equals(Constants.INT)) {
                    Iterator<String> itr = allowedValues.iterator();
                    while (itr.hasNext()) {
                        String val = itr.next();
                        int v1 = Integer.parseInt(val);
                        int v2 = Integer.parseInt(value);
                        if (v1 == v2) {
                            found = true;
                            break;
                        }
                    }
                } else {
                    Iterator<String> itr = allowedValues.iterator();
                    while (itr.hasNext()) {
                        String val = itr.next();
                        double v1 = Double.parseDouble(val);
                        double v2 = Double.parseDouble(value);
                        if (Double.compare(v1, v2) == 0) {
                            found = true;
                            break;
                        }
                    }
                }
                if (!found) {
                    result = false;
                }
            }
        }
        return result;
    }

    public void setAllowedValuesByArray(String[] cusItems) {
        if (null == cusItems) {
            return;
        }
        for (String item : cusItems) {
            addValueToAllowedValues(item);
        }
    }

    public boolean isAllowedValueExist(String[] items, String value) {
        if (null == items || items.length < 1 || null == value
                || value.isEmpty())
            return false;
        for (String item : items) {
            if (value.equalsIgnoreCase(item)) {
                return true;
            }
        }
        return false;
    }

    public SimulatorResourceAttribute convertToSimulatorResourceAttribute() {
        AttributeValue attValue = null;
        AttributeProperty attProperty = null;
        SimulatorResourceAttribute simResAtt;

        ValueType valueType = Utility.getAttributeTypeEnum(attributeType);
        switch (valueType) {
            case INTEGER:
                IntegerProperty.Builder intPropertyBuilder = new IntegerProperty.Builder();
                attValue = new AttributeValue(
                        Integer.parseInt(attributeDflValue));

                // Set default value.
                intPropertyBuilder.setDefaultValue(Integer
                        .parseInt(attributeDflValue));

                if (validValuesType == ValidValuesType.VALUESET) {
                    // Set allowed values.
                    intPropertyBuilder.setValues(Utility
                            .convertSetToArrayInt(Utility
                                    .convertSetStringToSetObject(allowedValues,
                                            valueType)));
                } else if (validValuesType == ValidValuesType.RANGE) {
                    // Set Range.
                    intPropertyBuilder.setRange(Integer.parseInt(min),
                            Integer.parseInt(max));
                }
                attProperty = intPropertyBuilder.build();
                break;
            case DOUBLE:
                DoubleProperty.Builder dblPropertyBuilder = new DoubleProperty.Builder();
                attValue = new AttributeValue(
                        Double.parseDouble(attributeDflValue));

                // Set default value.
                dblPropertyBuilder.setDefaultValue(Double
                        .parseDouble(attributeDflValue));

                if (validValuesType == ValidValuesType.VALUESET) {
                    // Set allowed values.
                    dblPropertyBuilder.setValues(Utility
                            .convertSetToArrayDouble(Utility
                                    .convertSetStringToSetObject(allowedValues,
                                            valueType)));
                } else if (validValuesType == ValidValuesType.RANGE) {
                    // Set Range.
                    dblPropertyBuilder.setRange(Double.parseDouble(min),
                            Double.parseDouble(max));
                }
                attProperty = dblPropertyBuilder.build();
                break;
            case BOOLEAN:
                attValue = new AttributeValue(
                        Boolean.parseBoolean(attributeDflValue));
                BooleanProperty.Builder boolPropertyBuilder = new BooleanProperty.Builder();
                // Set Default Value.
                boolPropertyBuilder.setDefaultValue(Boolean
                        .parseBoolean(attributeDflValue));

                attProperty = boolPropertyBuilder.build();
                break;
            case STRING:
                attValue = new AttributeValue(attributeDflValue);
                StringProperty.Builder stringPropertyBuilder = new StringProperty.Builder();

                // Set Default Value.
                stringPropertyBuilder.setDefaultValue(attributeDflValue);

                if (validValuesType == ValidValuesType.VALUESET) {
                    // Set Allowed Values.
                    stringPropertyBuilder.setValues(Utility
                            .convertSetToArrayString(Utility
                                    .convertSetStringToSetObject(allowedValues,
                                            valueType)));
                }
                attProperty = stringPropertyBuilder.build();
                break;
            default:
                break;
        }
        simResAtt = new SimulatorResourceAttribute(attributeName, attValue,
                attProperty);
        return simResAtt;
    }
}