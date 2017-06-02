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

import java.util.ArrayList;
import java.util.Comparator;
import java.util.Date;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Vector;

import org.oic.simulator.AttributeValue;
import org.oic.simulator.AttributeValue.TypeInfo;
import org.oic.simulator.AttributeValue.ValueType;
import org.oic.simulator.ILogger.Level;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorResourceModel;

import oic.simulator.serviceprovider.Activator;
import oic.simulator.serviceprovider.model.AttributeElement;
import oic.simulator.serviceprovider.model.Resource;
import oic.simulator.serviceprovider.model.SingleResource;

/**
 * This class has common utility methods.
 */
public class Utility {

    public static String getAutomationStatus(boolean status) {
        if (status) {
            return Constants.ENABLED;
        } else {
            return Constants.DISABLED;
        }
    }

    public static String getAutomationString(boolean status) {
        if (status) {
            return Constants.ENABLE;
        } else {
            return Constants.DISABLE;
        }
    }

    public static boolean getAutomationBoolean(String status) {
        if (null != status) {
            if (status.equals(Constants.ENABLE)) {
                return true;
            }
        }
        return false;
    }

    public static int getUpdateIntervalFromString(String value) {
        int result = Constants.DEFAULT_AUTOMATION_INTERVAL;
        if (null != value) {
            try {
                result = Integer.parseInt(value);
            } catch (NumberFormatException e) {
                Activator
                        .getDefault()
                        .getLogManager()
                        .log(Level.ERROR.ordinal(),
                                new Date(),
                                getSimulatorErrorString(
                                        e,
                                        "Update interval convertion failed."
                                                + "Taking the default value("
                                                + Constants.DEFAULT_AUTOMATION_INTERVAL
                                                + ")"));
            }
        }
        return result;
    }

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

    public static List<SingleResource> getSingleResourceListFromSet(
            Set<SingleResource> resources) {
        if (null == resources) {
            return null;
        }
        List<SingleResource> list = new ArrayList<SingleResource>();
        Iterator<SingleResource> typeItr = resources.iterator();
        while (typeItr.hasNext()) {
            list.add(typeItr.next());
        }
        return list;
    }

    public static Set<String> convertVectorToSet(Vector<String> vector) {
        if (null == vector || vector.isEmpty()) {
            return null;
        }
        Set<String> resultSet = new HashSet<String>();
        Enumeration<String> e = vector.elements();
        while (e.hasMoreElements()) {
            resultSet.add(e.nextElement());
        }
        return resultSet;
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
            if (null != info && !info.isEmpty())
                detail = info + "\n";
            else
                detail = "Description not available\n";
            detail += "Exception Type: " + e.getClass().getSimpleName() + "\n";
            String msg = e.getMessage();
            if (null != msg && !msg.isEmpty()) {
                detail += "Message: " + e.getMessage();
            }
        }
        return detail;
    }

    public static Set<String> getAttributeTypes() {
        Set<String> attTypes = new HashSet<String>();
        ValueType[] types = ValueType.values();
        if (null != types) {
            attTypes.add(Constants.INT);
            attTypes.add(Constants.DOUBLE);
            attTypes.add(Constants.BOOL);
            attTypes.add(Constants.STRING);
        }
        return attTypes;
    }

    public static ValueType getAttributeTypeEnum(String type) {
        if (null != type && type.trim().length() > 0) {
            if (type.equalsIgnoreCase(Constants.INT)) {
                return ValueType.INTEGER;
            }
            if (type.equalsIgnoreCase(Constants.DOUBLE)) {
                return ValueType.DOUBLE;
            }
            if (type.equalsIgnoreCase(Constants.BOOL)) {
                return ValueType.BOOLEAN;
            }
            if (type.equalsIgnoreCase(Constants.STRING)) {
                return ValueType.STRING;
            }
        }
        return ValueType.UNKNOWN;
    }

    public static int[] convertSetToArrayInt(Set<Object> allowedValues) {
        if (null == allowedValues || allowedValues.size() < 1) {
            return null;
        }
        int[] arr = new int[allowedValues.size()];
        Iterator<Object> itr = allowedValues.iterator();
        try {
            int i = 0;
            while (itr.hasNext()) {
                arr[i++] = (int) itr.next();
            }
        } catch (Exception e) {
            return null;
        }
        return arr;
    }

    public static double[] convertSetToArrayDouble(Set<Object> allowedValues) {
        if (null == allowedValues || allowedValues.size() < 1) {
            return null;
        }
        double[] arr = new double[allowedValues.size()];
        Iterator<Object> itr = allowedValues.iterator();
        try {
            int i = 0;
            while (itr.hasNext()) {
                arr[i++] = (double) itr.next();
            }
        } catch (Exception e) {
            return null;
        }
        return arr;
    }

    public static boolean[] convertSetToArrayBoolean(Set<Object> allowedValues) {
        if (null == allowedValues || allowedValues.size() < 1) {
            return null;
        }
        boolean[] arr = new boolean[allowedValues.size()];
        Iterator<Object> itr = allowedValues.iterator();
        try {
            int i = 0;
            while (itr.hasNext()) {
                arr[i++] = (boolean) itr.next();
            }
        } catch (Exception e) {
            return null;
        }
        return arr;
    }

    public static String[] convertSetToArrayString(Set<Object> allowedValues) {
        if (null == allowedValues || allowedValues.size() < 1) {
            return null;
        }
        String[] arr = new String[allowedValues.size()];
        Iterator<Object> itr = allowedValues.iterator();
        try {
            int i = 0;
            while (itr.hasNext()) {
                arr[i++] = (String) itr.next();
            }
        } catch (Exception e) {
            return null;
        }
        return arr;
    }

    public static Vector<Integer> convertSetToVectorInt(
            Set<Object> allowedValues) {
        if (null == allowedValues || allowedValues.size() < 1) {
            return null;
        }
        Vector<Integer> resultVec = new Vector<Integer>();
        Iterator<Object> itr = allowedValues.iterator();
        try {
            while (itr.hasNext()) {
                resultVec.add((Integer) itr.next());
            }
        } catch (Exception e) {
            return null;
        }
        return resultVec;
    }

    public static Vector<Double> convertSetToVectorDouble(
            Set<Object> allowedValues) {
        if (null == allowedValues || allowedValues.size() < 1) {
            return null;
        }
        Vector<Double> resultVec = new Vector<Double>();
        Iterator<Object> itr = allowedValues.iterator();
        try {
            while (itr.hasNext()) {
                resultVec.add((Double) itr.next());
            }
        } catch (Exception e) {
            return null;
        }
        return resultVec;
    }

    public static Vector<String> convertSetToVectorString(
            Set<String> allowedValues) {
        if (null == allowedValues || allowedValues.size() < 1) {
            return null;
        }
        Vector<String> resultVec = new Vector<String>();
        Iterator<String> itr = allowedValues.iterator();
        try {
            while (itr.hasNext()) {
                resultVec.add((String) itr.next());
            }
        } catch (Exception e) {
            return null;
        }
        return resultVec;
    }

    public static Set<Object> convertSetStringToSetObject(Set<String> values,
            ValueType type) {
        if (null == values || values.isEmpty()) {
            return null;
        }
        Set<Object> resultSet = new HashSet<Object>();
        if (AttributeValue.ValueType.INTEGER == type) {
            int val;
            Iterator<String> itr = values.iterator();
            while (itr.hasNext()) {
                try {
                    val = Integer.parseInt(itr.next());
                    resultSet.add(val);
                } catch (NumberFormatException nfe) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(),
                                    new Date(),
                                    "There is an error while doing internal convertion(string set to object set).\n"
                                            + Utility.getSimulatorErrorString(
                                                    nfe, null));
                }
            }
        } else if (AttributeValue.ValueType.DOUBLE == type) {
            double val;
            Iterator<String> itr = values.iterator();
            while (itr.hasNext()) {
                try {
                    val = Double.parseDouble(itr.next());
                    resultSet.add(val);
                } catch (NumberFormatException nfe) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(),
                                    new Date(),
                                    "There is an error while doing internal convertion(string set to object set).\n"
                                            + Utility.getSimulatorErrorString(
                                                    nfe, null));
                }
            }
        } else if (AttributeValue.ValueType.BOOLEAN == type) {
            resultSet.add(true);
            resultSet.add(false);
        } else if (AttributeValue.ValueType.STRING == type) {
            Iterator<String> itr = values.iterator();
            while (itr.hasNext()) {
                resultSet.add(itr.next());
            }
        }
        return resultSet;
    }

    public static List<Resource> convertSingleTypeResourceListToBaseType(
            List<SingleResource> resources) {
        if (null == resources || resources.isEmpty()) {
            return null;
        }
        List<Resource> resultSet = new ArrayList<Resource>();
        Iterator<SingleResource> itr = resources.iterator();
        while (itr.hasNext()) {
            resultSet.add(itr.next());
        }
        return resultSet;
    }

    public static String[] convertListToStringArray(List<String> valueList) {
        String[] strArr;
        if (null != valueList && valueList.size() > 0) {
            strArr = valueList.toArray(new String[1]);
        } else {
            strArr = new String[1];
        }
        return strArr;
    }

    public static Comparator<Resource>         resourceComparator       = new Comparator<Resource>() {
                                                                            public int compare(
                                                                                    Resource res1,
                                                                                    Resource res2) {
                                                                                String s1 = res1
                                                                                        .getResourceName();
                                                                                String s2 = res2
                                                                                        .getResourceName();

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
                                                                                // return
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

    public static Comparator<SingleResource>   singleResourceComparator = new Comparator<SingleResource>() {
                                                                            public int compare(
                                                                                    SingleResource res1,
                                                                                    SingleResource res2) {
                                                                                String s1 = res1
                                                                                        .getResourceName();
                                                                                String s2 = res2
                                                                                        .getResourceName();

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
                                                                                // return
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

    public static Comparator<AttributeElement> attributeComparator      = new Comparator<AttributeElement>() {
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

    public static boolean isUriValid(String resURI) {
        if (null == resURI || resURI.length() < 2 || resURI.length() > 63
                || !resURI.startsWith("/") || resURI.endsWith("/")
                || resURI.contains("/..") || resURI.contains("//")
                || resURI.contains("/./") || resURI.contains("?")
                || resURI.contains("#") || resURI.contains("%")) {
            return false;
        }
        return true;
    }

    public static boolean isResourceTypeValid(String resType) {
        if (null == resType || resType.isEmpty()) {
            return false;
        }

        char[] ch = resType.toCharArray();
        for (char c : ch) {
            if (c != '.' && c != '-' && (c < 'a' || c > 'z')
                    && (c < '0' || c > '9')) {
                return false;
            }
        }

        return true;
    }

    public static Map<String, String> getResourceInterfaces(
            Class<? extends Resource> resourceClass) {
        Map<String, String> ifTypes = null;
        if (resourceClass == SingleResource.class) {
            ifTypes = new HashMap<String, String>();
            ifTypes.put(Constants.READ_ONLY_INTERFACE, "Read-Only");
            ifTypes.put(Constants.READ_WRITE_INTERFACE, "Read-Write");
            ifTypes.put(Constants.ACTUATOR_INTERFACE, "Actuator");
            ifTypes.put(Constants.SENSOR_INTERFACE, "Sensor");
        }
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