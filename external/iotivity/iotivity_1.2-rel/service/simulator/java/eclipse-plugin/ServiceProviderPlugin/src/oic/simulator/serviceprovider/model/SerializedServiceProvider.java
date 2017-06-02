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

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.List;
import java.util.Map;

public class SerializedServiceProvider implements Serializable {

    public static class SerializedAttribute implements Serializable {

        public String getName() {
            return m_name;
        }

        public void setName(String name) {
            this.m_name = name;
        }

        public Object getValue() {
            return m_value;
        }

        public void setValue(Object value) {
            this.m_value = value;
        }

        public String getType() {
            return m_type;
        }

        public void setType(String type) {
            this.m_type = type;
        }

        public int getMin() {
            return m_min;
        }

        public void setMin(int min) {
            this.m_min = min;
        }

        public int getMax() {
            return m_max;
        }

        public void setMax(int max) {
            this.m_max = max;
        }

        public Object getAllowedValues() {
            return m_AllowedValues;
        }

        public void setAllowedValues(Object allowedValues) {
            this.m_AllowedValues = allowedValues;
        }

        private String m_name          = null;
        private Object m_value         = null;
        private String m_type          = null;
        private int    m_min           = 0;
        private int    m_max           = 0;
        private Object m_AllowedValues = null;
    }

    public String getResourceName() {
        return m_resourceName;
    }

    public void setResourceName(String resourceName) {
        this.m_resourceName = resourceName;
    }

    public String getUri() {
        return m_uri;
    }

    public void setUri(String uri) {
        this.m_uri = uri;
    }

    public String getResourceType() {
        return m_resourceType;
    }

    public void setResourceType(String resourceType) {
        this.m_resourceType = resourceType;
    }

    public List<String> getInterfaceType() {
        return m_interfaceType;
    }

    public void setInterfaceType(List<String> interfaceType) {
        this.m_interfaceType = interfaceType;
    }

    public Map<String, SerializedAttribute> getResourceAttributesMap() {
        return m_resourceAttributesMap;
    }

    public void setResourceAttributesMap(
            Map<String, SerializedAttribute> resourceAttributesMap) {
        this.m_resourceAttributesMap = resourceAttributesMap;
    }

    public void serialize(String filePath) throws FileNotFoundException,
            IOException {
        FileOutputStream fileOut = null;
        ObjectOutputStream out = null;
        try {
            fileOut = new FileOutputStream(filePath);
            out = new ObjectOutputStream(fileOut);
            out.writeObject(this);
        } finally {
            if (null != fileOut)
                fileOut.close();

            if (null != out)
                out.close();
        }
    }

    public static SerializedServiceProvider deSerialize(String filePath)
            throws FileNotFoundException, IOException, ClassNotFoundException {
        FileInputStream fileIn = null;
        ObjectInputStream in = null;
        SerializedServiceProvider r;
        try {
            fileIn = new FileInputStream(filePath);
            in = new ObjectInputStream(fileIn);
            r = (SerializedServiceProvider) in.readObject();
        } finally {
            if (null != fileIn)
                fileIn.close();

            if (null != in)
                in.close();
        }
        return r;
    }

    private String                           m_resourceName;
    private String                           m_uri;
    private String                           m_resourceType;
    private List<String>                     m_interfaceType;
    private Map<String, SerializedAttribute> m_resourceAttributesMap;

}