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
package org.iotivity.cloud.rdserver.resources.directory.rd;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.exception.ServerException.PreconditionFailedException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.resource.Resource;
import org.iotivity.cloud.rdserver.Constants;
import org.iotivity.cloud.rdserver.resources.directory.RDManager;
import org.iotivity.cloud.rdserver.resources.presence.PresenceManager;
import org.iotivity.cloud.util.Cbor;
import org.iotivity.cloud.util.Log;

/**
 * 
 * This class provides a set of APIs handle requests about resource directory
 *
 */
public class ResourceDirectoryResource extends Resource {

    private Cbor<HashMap<String, Object>> mCbor      = new Cbor<>();
    private RDManager                     mRdManager = new RDManager();

    public ResourceDirectoryResource() {
        super(Arrays.asList(Constants.PREFIX_OIC, Constants.RD_URI));
    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {

        IResponse response = null;

        switch (request.getMethod()) {
            case POST:
                response = handlePostRequest(request);
                break;

            case DELETE:
                response = handleDeleteRequest(request);
                break;

            default:
                throw new BadRequestException(
                        request.getMethod() + " request type is not supported");
        }

        srcDevice.sendResponse(response);

        PresenceManager.getInstance()
                .notifyToObservers(mRdManager.getmResourcePresence());

    }

    private void checkLinksProperty(HashMap<String, Object> payload) {

        Object links = payload.get(Constants.LINKS);
        if (links == null) {
            throw new BadRequestException("links property is null");
        }
        ArrayList<HashMap<String, Object>> linksList = (ArrayList<HashMap<String, Object>>) links;
        for (HashMap<String, Object> link : linksList) {
            checkPayloadException(
                    Arrays.asList(Constants.HREF, Constants.RESOURCE_TYPE,
                            Constants.INS, Constants.INTERFACE),
                    link);
        }
    }

    private IResponse handlePostRequest(IRequest request)
            throws ServerException {

        HashMap<String, Object> payload = mCbor
                .parsePayloadFromCbor(request.getPayload(), HashMap.class);

        Log.d("publish payload : " + payload);

        checkPayloadException(
                Arrays.asList(Constants.DEVICE_ID, Constants.LINKS), payload);

        // check mandatory property
        checkLinksProperty(payload);

        HashMap<String, Object> response = mRdManager.publishResource(payload);

        Log.d("publish response : " + response);

        return MessageBuilder.createResponse(request, ResponseStatus.CHANGED,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(response));

    }

    private IResponse handleDeleteRequest(IRequest request)
            throws ServerException {

        HashMap<String, List<String>> queryMap = request.getUriQueryMap();

        checkQueryException(Arrays.asList(Constants.DEVICE_ID), queryMap);

        List<String> diList = queryMap.get(Constants.DEVICE_ID);
        List<String> insList = queryMap.get(Constants.INS);

        mRdManager.deleteResource(diList.get(0), insList);

        IResponse response = MessageBuilder.createResponse(request,
                ResponseStatus.DELETED);

        return response;
    }

}
