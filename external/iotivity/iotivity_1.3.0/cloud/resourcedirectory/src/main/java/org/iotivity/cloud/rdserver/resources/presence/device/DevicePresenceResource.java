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
package org.iotivity.cloud.rdserver.resources.presence.device;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.resource.Resource;
import org.iotivity.cloud.rdserver.Constants;
import org.iotivity.cloud.rdserver.resources.presence.PresenceManager;
import org.iotivity.cloud.util.Cbor;

/**
 * 
 * This class provides a set of APIs handle requests about device presence
 *
 */
public class DevicePresenceResource extends Resource {

    private Cbor<HashMap<String, Object>> mCbor = new Cbor<>();

    public DevicePresenceResource() {
        super(Arrays.asList(Constants.PREFIX_OIC,
                Constants.DEVICE_PRESENCE_URI));

    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {

        IResponse response = null;

        switch (request.getMethod()) {
            case GET:
                response = handleGetRequest(srcDevice, request);
                break;

            case POST:
                response = handlePostRequest(request);
                break;

            default:
                throw new BadRequestException(
                        request.getMethod() + " request type is not supported");
        }

        srcDevice.sendResponse(response);
    }

    private IResponse handleGetRequest(Device srcDevice, IRequest request)
            throws ServerException {
        HashMap<String, List<String>> queryMap = request.getUriQueryMap();

        checkQueryException(Arrays.asList(Constants.DEVICE_ID), queryMap);

        List<String> deviceList = queryMap.get(Constants.DEVICE_ID);

        switch (request.getObserve()) {
            case SUBSCRIBE:
                PresenceManager.getInstance().subscribePresence(srcDevice,
                        request, deviceList, Constants.DEVICE_PRESENCE);
                break;
            case UNSUBSCRIBE:
                PresenceManager.getInstance().unsubscribePresence(request,
                        deviceList, Constants.DEVICE_PRESENCE);
                break;
            default:
        }

        byte[] payload = PresenceManager.getInstance()
                .makeResponsePayload(deviceList);

        return MessageBuilder.createResponse(request, ResponseStatus.CONTENT,
                ContentFormat.APPLICATION_CBOR, payload);

    }

    private IResponse handlePostRequest(IRequest request)
            throws ServerException {
        // check payload
        byte[] payload = request.getPayload();

        HashMap<String, Object> parsedPayload = mCbor
                .parsePayloadFromCbor(payload, HashMap.class);

        checkPayloadException(
                Arrays.asList(Constants.DEVICE_ID, Constants.PRESENCE_STATE),
                parsedPayload);

        // store db
        PresenceManager.getInstance().updateDevicePresence(parsedPayload);

        // notification to observers
        String di = parsedPayload.get(Constants.DEVICE_ID).toString();
        PresenceManager.getInstance().notifyToObservers(di);

        return MessageBuilder.createResponse(request, ResponseStatus.CHANGED);

    }
}
