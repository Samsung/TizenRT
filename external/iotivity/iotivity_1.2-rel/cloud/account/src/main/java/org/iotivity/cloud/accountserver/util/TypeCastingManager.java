package org.iotivity.cloud.accountserver.util;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Iterator;

import org.iotivity.cloud.base.exception.ServerException.InternalServerErrorException;

public class TypeCastingManager<T> {
    public TypeCastingManager() {

    }

    public HashMap<String, Object> convertObjectToMap(T objClass) {

        try {
            Field[] fieldList = objClass.getClass().getDeclaredFields();
            HashMap<String, Object> map = new HashMap<>();

            for (Field field : fieldList) {
                field.setAccessible(true);
                Object value = field.get(objClass);
                if (value != null) {
                    String fieldName = field.getName();
                    map.put(fieldName, value);
                }
            }
            return map;
        } catch (Exception e) {
            throw new InternalServerErrorException(
                    "Object to Map casting error");
        }
    }

    public T convertMaptoObject(HashMap<String, Object> map, T objClass) {

        String keyAttribute = null;
        String methodName = null;
        Iterator<String> iter = map.keySet().iterator();
        String prefixName = "set";
        while (iter.hasNext()) {
            keyAttribute = iter.next().toString();
            methodName = makeMethodName(keyAttribute, prefixName);

            Method[] methodList = objClass.getClass().getDeclaredMethods();

            for (Method method : methodList) {
                if (methodName.equals(method.getName())) {
                    try {
                        method.invoke(objClass, map.get(keyAttribute));
                    } catch (Exception e) {
                        e.printStackTrace();
                        throw new InternalServerErrorException(
                                "Map to Object casting error");
                    }
                }
            }
        }
        return objClass;
    }

    private String makeMethodName(String keyAttribute, String prefixName) {

        String methodName = null;

        methodName = prefixName + keyAttribute.substring(0, 1).toUpperCase()
                + keyAttribute.substring(1);

        return methodName;
    }
}
