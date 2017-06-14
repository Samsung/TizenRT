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

package oic.simulator.clientcontroller.utils;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.oic.simulator.AttributeValue;
import org.oic.simulator.AttributeValue.TypeInfo;
import org.oic.simulator.AttributeValue.ValueType;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorResourceModel;

import oic.simulator.clientcontroller.remoteresource.AttributeElement;

/**
 * This class has common utility methods.
 */
public class Utility {
    public static List<String> convertSetToList(Set<String> typeSet) {
        if (null == typeSet) {
            return null;
        }
        List<String> list = new ArrayList<String>();
        Iterator<String> typeItr = typeSet.iterator();
        while (typeItr.hasNext()) {
            list.add(typeItr.next());
        }
        return list;
    }

    public static String getObservableInString(boolean observable) {
        if (observable) {
            return Constants.YES;
        } else {
            return Constants.NO;
        }
    }

    public static String[] convertListToString(List<String> valueList) {
        String[] strArr;
        if (null != valueList && valueList.size() > 0) {
            strArr = valueList.toArray(new String[1]);
        } else {
            strArr = new String[1];
        }
        return strArr;
    }

    public static Set<String> splitStringByComma(String text) {
        Set<String> tokenSet = null;
        if (null != text) {
            String[] token = text.split(",");
            if (null != token) {
                tokenSet = new HashSet<String>();
                for (String tok : token) {
                    tok = tok.trim();
                    if (tok.length() > 0) {
                        tokenSet.add(tok);
                    }
                }
            }
        }
        return tokenSet;
    }

    public static String getSimulatorErrorString(Exception e, String info) {
        if (null == e) {
            return null;
        }
        String detail;
        if (e instanceof SimulatorException) {
            SimulatorException simEx = (SimulatorException) e;
            detail = simEx.message() + "\n";
            detail += "Exception Type: " + simEx.getClass().getSimpleName()
                    + "\n";
            detail += "Error code: " + simEx.code().toString();
        } else {
            detail = info + "\n";
            detail += "Exception Type: " + e.getClass().getSimpleName() + "\n";
            detail += "Message: " + e.getMessage();
        }
        return detail;
    }

    public static Comparator<AttributeElement> attributeComparator = new Comparator<AttributeElement>() {
                                                                       public int compare(
                                                                               AttributeElement att1,
                                                                               AttributeElement att2) {
                                                                           String s1 = att1
                                                                                   .getSimulatorResourceAttribute()
                                                                                   .name();
                                                                           String s2 = att2
                                                                                   .getSimulatorResourceAttribute()
                                                                                   .name();

                                                                           String s1Part = s1
                                                                                   .replaceAll(
                                                                                           "\\d",
                                                                                           "");
                                                                           String s2Part = s2
                                                                                   .replaceAll(
                                                                                           "\\d",
                                                                                           "");

                                                                           if (s1Part
                                                                                   .equalsIgnoreCase(s2Part)) {
                                                                               return extractInt(s1)
                                                                                       - extractInt(s2);
                                                                           }
                                                                           return s1
                                                                                   .compareTo(s2);
                                                                       }

                                                                       int extractInt(
                                                                               String s) {
                                                                           String num = s
                                                                                   .replaceAll(
                                                                                           "\\D",
                                                                                           "");

                                                                           // Return
                                                                           // 0
                                                                           // if
                                                                           // no
                                                                           // digits
                                                                           // found
                                                                           return num
                                                                                   .isEmpty() ? 0
                                                                                   : Integer
                                                                                           .parseInt(num);
                                                                       }
                                                                   };

    // This method only works for attributes whose values are of type int,
    // double, bool, string and 1-D array of primitive types
    public static String getAttributeValueAsString(AttributeValue val) {
        if (null == val) {
            return null;
        }

        Object value = val.get();
        if (null == value) {
            return null;
        }

        TypeInfo type = val.typeInfo();
        if (type.mBaseType == ValueType.RESOURCEMODEL
                || (type.mType == ValueType.ARRAY && type.mDepth > 1)) {
            return null;
        }

        AttributeValueStringConverter converter = new AttributeValueStringConverter(
                val);
        return converter.toString();
    }

    public static Map<String, String> getResourceInterfaces() {
        Map<String, String> ifTypes = new HashMap<String, String>();
        ifTypes.put(Constants.BASELINE_INTERFACE, "Baseline");
        ifTypes.put(Constants.LINKS_LIST_INTERFACE, "Links List");
        ifTypes.put(Constants.BATCH_INTERFACE, "Batch");
        ifTypes.put(Constants.LINK_BATCH_INTERFACE, "Link Batch");
        ifTypes.put(Constants.READ_ONLY_INTERFACE, "Read-Only");
        ifTypes.put(Constants.READ_WRITE_INTERFACE, "Read-Write");
        ifTypes.put(Constants.ACTUATOR_INTERFACE, "Actuator");
        ifTypes.put(Constants.SENSOR_INTERFACE, "Sensor");
        return ifTypes;
    }

    public static String removeWhiteSpacesInArrayValues(String value) {
        if (null == value || value.isEmpty())
            return null;

        value = value.trim();

        String token[] = value.split(",");
        StringBuilder result = new StringBuilder();
        for (int i = 0; i < token.length; i++) {
            result.append(token[i].trim());
            if (i + 1 < token.length) {
                result.append(",");
            }
        }

        return result.toString();
    }

    public static AttributeValue cloneAttributeValue(AttributeValue value)
            throws InvalidArgsException, NullPointerException {
        AttributeValue clone = null;

        AttributeValue.TypeInfo typeInfo = value.typeInfo();

        if (typeInfo.mType == AttributeValue.ValueType.RESOURCEMODEL) {
            SimulatorResourceModel resModel = (SimulatorResourceModel) value
                    .get();
            SimulatorResourceModel modelCopy = new SimulatorResourceModel();

            for (Map.Entry<String, AttributeValue> entry : resModel.get()
                    .entrySet()) {
                String attName = entry.getKey();
                AttributeValue attValue = entry.getValue();
                modelCopy.set(attName, cloneAttributeValue(attValue));
            }
            clone = new AttributeValue(modelCopy);
        } else if (typeInfo.mType == AttributeValue.ValueType.ARRAY
                && typeInfo.mBaseType == AttributeValue.ValueType.RESOURCEMODEL) {
            if (typeInfo.mDepth == 1) {
                SimulatorResourceModel[] resModelArray = (SimulatorResourceModel[]) value
                        .get();
                SimulatorResourceModel[] modelArrayCopy = new SimulatorResourceModel[resModelArray.length];
                for (int i = 0; i < resModelArray.length; i++) {
                    AttributeValue attValue = cloneAttributeValue(new AttributeValue(
                            resModelArray[i]));
                    if (null != attValue) {
                        modelArrayCopy[i] = (SimulatorResourceModel) attValue
                                .get();
                    }
                }
                clone = new AttributeValue(modelArrayCopy);
            } else if (typeInfo.mDepth == 2) {
                SimulatorResourceModel[][] resModelArray = (SimulatorResourceModel[][]) value
                        .get();
                SimulatorResourceModel[][] modelArrayCopy = new SimulatorResourceModel[resModelArray.length][];
                for (int i = 0; i < resModelArray.length; i++) {
                    AttributeValue attValue = cloneAttributeValue(new AttributeValue(
                            resModelArray[i]));
                    if (null != attValue) {
                        modelArrayCopy[i] = (SimulatorResourceModel[]) attValue
                                .get();
                    }
                }
                clone = new AttributeValue(modelArrayCopy);
            } else if (typeInfo.mDepth == 3) {
                SimulatorResourceModel[][][] resModelArray = (SimulatorResourceModel[][][]) value
                        .get();
                SimulatorResourceModel[][][] modelArrayCopy = new SimulatorResourceModel[resModelArray.length][][];
                for (int i = 0; i < resModelArray.length; i++) {
                    AttributeValue attValue = cloneAttributeValue(new AttributeValue(
                            resModelArray[i]));
                    if (null != attValue) {
                        modelArrayCopy[i] = (SimulatorResourceModel[][]) attValue
                                .get();
                    }
                }
                clone = new AttributeValue(modelArrayCopy);
            }
        } else {
            String attValueInString = new AttributeValueStringConverter(value)
                    .toString();
            clone = AttributeValueBuilder.build(attValueInString,
                    typeInfo.mBaseType);
        }

        return clone;
    }
}