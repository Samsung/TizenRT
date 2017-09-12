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
package org.iotivity.cloud.base.protocols;

import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.StringTokenizer;

public abstract class Message implements IRequest, IResponse, ISignaling {
    protected List<byte[]> uri_path  = null;
    protected List<byte[]> uri_query = null;
    protected byte[]       payload   = null;

    // Message properties are intersection of HTTP and COAP with observe.

    @Override
    public List<String> getUriPathSegments() {
        if (uri_path == null) {
            return null;
        }
        List<String> segments = new ArrayList<>();
        for (byte[] pathSegment : uri_path) {
            segments.add(new String(pathSegment, StandardCharsets.UTF_8));
        }
        return segments;
    }

    @Override
    public String getUriPath() {
        if (uri_path == null) {
            return null;
        }
        StringBuilder path = new StringBuilder("/");
        int nItem = uri_path.size();
        for (byte[] pathSegment : uri_path) {
            path.append(new String(pathSegment, StandardCharsets.UTF_8));
            if (--nItem > 0) {
                path.append('/');
            }
        }
        return path.toString();
    }

    @Override
    public String getUriQuery() {
        if (uri_query == null) {
            return null;
        }
        StringBuilder uriQuery = new StringBuilder();
        int nItem = uri_query.size();
        for (byte[] querySegment : uri_query) {
            uriQuery.append(new String(querySegment, StandardCharsets.UTF_8));
            if (--nItem > 0) {
                uriQuery.append(';');
            }
        }
        return uriQuery.toString();
    }

    @Override
    public List<String> getUriQuerySegments() {
        if (uri_query == null) {
            return null;
        }
        List<String> segments = new ArrayList<>();
        for (byte[] querySegment : uri_query) {
            segments.add(new String(querySegment, StandardCharsets.UTF_8));
        }
        return segments;
    }

    @Override
    public HashMap<String, List<String>> getUriQueryMap() {

        List<String> segments = getUriQuerySegments();
        if (segments == null) {
            return null;
        }

        HashMap<String, List<String>> queryMap = new HashMap<>();
        for (String segment : segments) {
            StringTokenizer keyValuePair = new StringTokenizer(segment, "=");
            if (keyValuePair.countTokens() == 2) {
                String key = keyValuePair.nextToken();
                String value = keyValuePair.nextToken();
                if (queryMap.containsKey(key) == false) {
                    queryMap.put(key, new ArrayList<String>());
                }
                queryMap.get(key).add(value);
            }
        }

        return queryMap;
    }

    @Override
    public int getPayloadSize() {
        return payload == null ? 0 : payload.length;
    }

    @Override
    public byte[] getPayload() {
        return payload;
    }
}
