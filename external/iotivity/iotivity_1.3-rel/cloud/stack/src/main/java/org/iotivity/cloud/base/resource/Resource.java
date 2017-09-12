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
package org.iotivity.cloud.base.resource;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map.Entry;

import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.device.IRequestEventHandler;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.exception.ServerException.NotFoundException;
import org.iotivity.cloud.base.exception.ServerException.PreconditionFailedException;
import org.iotivity.cloud.base.protocols.IRequest;

public class Resource implements IRequestEventHandler {

    private List<String> mPathSegments;

    public interface Functional {
        void queryHandler(Device srcDevice, IRequest request)
                throws ServerException;
    }

    private HashMap<List<String>, Functional> mQuerySetList = new HashMap<>();

    public void addQueryHandler(List<String> querySet,
            Functional queryHandler) {
        mQuerySetList.put(querySet, queryHandler);
    }

    public List<String> getUriPathSegments() {
        return mPathSegments;
    }

    public Resource(List<String> pathSegments) {
        mPathSegments = pathSegments;
    }

    @Override
    final public void onRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {
        // Handle pre-registered handlers
        List<String> reqQuerySet = request.getUriQuerySegments();

        if (reqQuerySet != null && mQuerySetList.size() > 0) {
            for (Entry<List<String>, Functional> entry : mQuerySetList
                    .entrySet()) {
                if (reqQuerySet.retainAll(entry.getKey()) == false) {
                    entry.getValue().queryHandler(srcDevice, request);
                    return;
                }
            }
        }

        // No packet handler found, pass to default handler
        onDefaultRequestReceived(srcDevice, request);
    }

    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {
        // No default handlers registered, send error
        throw new NotFoundException("No handlers registered");
    }

    public boolean checkQueryException(String property,
            HashMap<String, List<String>> queryData) {
        return checkQueryException(Arrays.asList(property), queryData);
    }

    public boolean checkQueryException(List<String> propertyList,
            HashMap<String, List<String>> queryData) {

        if (queryData == null)
            throw new BadRequestException("queryData is null");

        for (String property : propertyList) {
            if (!queryData.containsKey(property))
                throw new PreconditionFailedException(
                        "query does not contatin " + property + " property");
            if (queryData.get(property) == null)
                throw new PreconditionFailedException(
                        property + " param is null");
        }
        return true;
    }

    public boolean checkPayloadException(String property,
            HashMap<String, Object> payloadData) {
        return checkPayloadException(Arrays.asList(property), payloadData);
    }

    public boolean checkPayloadException(List<String> propertyList,
            HashMap<String, Object> payloadData) {

        if (payloadData == null)
            throw new BadRequestException("payloadData is null");

        for (String property : propertyList) {
            if (!payloadData.containsKey(property))
                throw new PreconditionFailedException(
                        "payload does not contain" + property + " property");
            if (payloadData.get(property) == null)
                throw new PreconditionFailedException(
                        property + " param is null in the payload");
        }
        return true;
    }
}
