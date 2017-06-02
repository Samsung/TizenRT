/******************************************************************
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 * <p>
 * <p>
 * <p>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************/

package org.iotivity.service.sample.client;

import org.iotivity.service.RcsException;
import org.iotivity.service.RcsResourceAttributes;
import org.iotivity.service.client.RcsRemoteResourceObject;

public class Utils {
    public static String resourceInfo(RcsRemoteResourceObject resourceObject)
            throws RcsException {
        StringBuilder sb = new StringBuilder();

        sb.append("URI : " + resourceObject.getUri() + "\n");
        sb.append("Host : " + resourceObject.getAddress() + "\n");
        for (String type : resourceObject.getTypes()) {
            sb.append("resourceType : " + type + "\n");
        }

        for (String itf : resourceObject.getInterfaces()) {
            sb.append("resourceInterfaces : " + itf + "\n");
        }

        sb.append("isObservable : " + resourceObject.isObservable() + "\n");

        return sb.toString();
    }

    public static String resourceAttributes(RcsResourceAttributes attr)
            throws RcsException {
        StringBuilder sb = new StringBuilder();

        for (String key : attr.keySet())
        {
            sb.append(key + " : " + attr.get(key) + "\n");
        }

        return sb.toString();
    }
}
