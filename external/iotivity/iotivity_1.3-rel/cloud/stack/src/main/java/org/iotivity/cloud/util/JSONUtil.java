/*
 * //******************************************************************
 * //
 * // Copyright 2016 Samsung Electronics All Rights Reserved.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * //
 * // Licensed under the Apache License, Version 2.0 (the "License");
 * // you may not use this file except in compliance with the License.
 * // You may obtain a copy of the License at
 * //
 * //      http://www.apache.org/licenses/LICENSE-2.0
 * //
 * // Unless required by applicable law or agreed to in writing, software
 * // distributed under the License is distributed on an "AS IS" BASIS,
 * // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * // See the License for the specific language governing permissions and
 * // limitations under the License.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */
package org.iotivity.cloud.util;

import java.io.IOException;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

/**
 * 
 * This class provides utility for parsing JSON object and converting data to
 * JSON string.
 * 
 */
public class JSONUtil<T> {

    private static ObjectMapper mapper = new ObjectMapper();

    @SuppressWarnings("unchecked")
    public T parseJSON(byte[] data, Class<? extends Object> class1) {

        if (data == null)
            return null;

        T parsedData = null;

        try {
            parsedData = (T) mapper.readValue(data, class1);

        } catch (IOException ioe) {
            ioe.printStackTrace();
        }

        return parsedData;
    }

    public T parseJSON(String data, Class<? extends Object> class1) {

        if (data == null || data.equals(""))
            return null;

        T parsedData = null;

        try {
            parsedData = (T) mapper.readValue(data, class1);

        } catch (IOException ioe) {
            ioe.printStackTrace();
        }

        return parsedData;
    }

    public T writeJSON(Object data) {
        if (data == null)
            return null;

        T json = null;
        try {
            json = (T) mapper.writeValueAsString(data);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }

        if (json == null)
            json = (T) "{}";

        return json;
    }
}