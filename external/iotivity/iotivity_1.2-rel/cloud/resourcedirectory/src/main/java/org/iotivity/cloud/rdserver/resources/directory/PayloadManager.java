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
package org.iotivity.cloud.rdserver.resources.directory;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.rdserver.Constants;

/**
 * 
 * This class provides a set of APIs handle a payload
 *
 */
public class PayloadManager {

    /** tag values of /oic/rd resource payload */
    public List<String> pubTagKey       = Arrays.asList("di", "n", "lt");

    /** links values of /oic/rd resource payload */
    public List<String> pubLinkKey      = Arrays.asList("href", "rel", "rt",
            "if", "p", "title", "anchor", "ins", "ttl", "type");

    /** tag values of /oic/res resource payload */
    public List<String> discoverTagKey  = Arrays.asList("di", "n");

    /** links values of /oic/res resource payload */
    public List<String> discoverLinkKey = Arrays.asList("href", "rt", "if",
            "p");

    /**
     * API for setting data of payload
     * 
     * @param payload
     *            payload data
     * @param keyType
     *            key type
     * @return data included keys of keyType
     */
    public HashMap<String, Object> setPayloadData(
            HashMap<String, Object> payload, List<String> keyType) {

        HashMap<String, Object> data = new HashMap<String, Object>();

        for (String key : keyType) {
            Object value = payload.get(key);
            if (value != null) {
                data.put(key, value);
            }
        }
        return data;
    }

    /**
     * API for changing type of policy property to store in DB
     * 
     * @param payload
     *            payload data
     */
    public void changePolicyTypeToStore(HashMap<String, Object> payload) {
        Object policy = payload.get(Constants.POLICY);
        if (policy != null) {
            HashMap<String, Object> bm = (HashMap<String, Object>) policy;
            payload.put(Constants.POLICY, (int) (bm.get(Constants.BITMAP)));
        }
    }

    /**
     * API for changing type of policy property to make a discovery response
     * 
     * @param payload
     *            payload data
     */
    public void changePolicyTypeToDiscover(HashMap<String, Object> payload) {
        Object policy = payload.get(Constants.POLICY);
        if (policy != null) {
            HashMap<Object, Object> bm = new HashMap<>();
            bm.put(Constants.BITMAP, policy);
            payload.put(Constants.POLICY, bm);
        }
    }

}
