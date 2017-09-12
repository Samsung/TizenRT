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
package org.iotivity.cloud.ciserver.resources.proxy.rd;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.device.IResponseEventHandler;
import org.iotivity.cloud.base.exception.ClientException;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.resource.Resource;
import org.iotivity.cloud.ciserver.Constants;
import org.iotivity.cloud.util.Cbor;

/**
 *
 * This class provides a set of APIs to send requests about publish resource to
 * resource directory
 *
 */

public class ResourceDirectory extends Resource {
    private Cbor<HashMap<String, Object>> mCbor     = new Cbor<>();

    public ResourceDirectory() {
        super(Arrays.asList(Constants.PREFIX_OIC, Constants.RD_URI));
    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {
        switch (request.getMethod()) {
            case POST:
                HashMap<String, Object> payloadData = mCbor
                        .parsePayloadFromCbor(request.getPayload(),
                                HashMap.class);
                StringBuffer query = new StringBuffer();
                query.append("op=add;");
                query.append(Constants.USER_ID + "=" + srcDevice.getUserId());

                StringBuffer uriPath = new StringBuffer();
                uriPath.append(Constants.PREFIX_OIC + "/");
                uriPath.append(Constants.ACL_URI + "/");
                uriPath.append(Constants.GROUP_URI + "/");
                uriPath.append(srcDevice.getUserId());

                String di = payloadData.get(Constants.REQ_DEVICE_ID).toString();
                HashMap<String, Object> requestPayload = new HashMap<>();

                requestPayload.put(Constants.REQ_GROUP_DEVICES,
                        Arrays.asList(di));
                IRequest requestToAS = MessageBuilder.createRequest(
                        RequestMethod.POST, uriPath.toString(),
                        query.toString(), ContentFormat.APPLICATION_CBOR,
                        mCbor.encodingPayloadToCbor(requestPayload));

                ConnectorPool.getConnection("account").sendRequest(requestToAS,
                        new AccountReceiveHandler(request, srcDevice));
                break;

            case DELETE:
                ConnectorPool.getConnection("rd").sendRequest(request, srcDevice);
                break;

            default:
                throw new BadRequestException(
                        request.getMethod() + " request type is not support");
        }
    }

    class AccountReceiveHandler implements IResponseEventHandler {
        private Device   mSrcDevice;
        private IRequest mRequest;

        public AccountReceiveHandler(IRequest request, Device srcDevice) {
            mSrcDevice = srcDevice;
            mRequest = request;
        }

        @Override
        public void onResponseReceived(IResponse response)
                throws ClientException {

            switch (response.getStatus()) {
                case CHANGED:
                    byte[] convertedPayload = convertPublishHref(mRequest,
                            mSrcDevice);

                    mRequest = MessageBuilder.modifyRequest(mRequest, null,
                            null, ContentFormat.APPLICATION_CBOR,
                            convertedPayload);

                    ConnectorPool.getConnection("rd").sendRequest(mRequest,
                            new PublishResponseHandler(mSrcDevice));
                    break;

                default:
                    mSrcDevice.sendResponse(response);
            }
        }

        @SuppressWarnings("unchecked")
        private byte[] convertPublishHref(IRequest request, Device device) {

            Cbor<HashMap<String, Object>> cbor = new Cbor<>();
            HashMap<String, Object> payload = cbor
                    .parsePayloadFromCbor(request.getPayload(), HashMap.class);

            ArrayList<HashMap<String, Object>> links = (ArrayList<HashMap<String, Object>>) payload
                    .get(Constants.REQ_LINKS);

            for (HashMap<String, Object> link : links) {

                String href = (String) link.get(Constants.REQ_HREF);
                href = Constants.ROUTE_FULL_URI + "/" + device.getDeviceId()
                        + href;

                link.put(Constants.REQ_HREF, href);
            }

            payload.put(Constants.REQ_LINKS, links);

            return cbor.encodingPayloadToCbor(payload);
        }
    }

    // handles response to convert href in response payload into original href
    // when resource published or updated
    class PublishResponseHandler implements IResponseEventHandler {

        private Device mSrcDevice;

        public PublishResponseHandler(Device srcDevice) {
            mSrcDevice = srcDevice;
        }

        @Override
        public void onResponseReceived(IResponse response)
                throws ClientException {

            switch (response.getStatus()) {
                case CHANGED:
                    response = MessageBuilder.modifyResponse(response,
                            ContentFormat.APPLICATION_CBOR,
                            convertResponseHref(response));

                default:
                    mSrcDevice.sendResponse(response);
            }
        }

        @SuppressWarnings("unchecked")
        private byte[] convertResponseHref(IResponse response) {

            Cbor<HashMap<String, Object>> cbor = new Cbor<>();
            HashMap<String, Object> payload = cbor
                    .parsePayloadFromCbor(response.getPayload(), HashMap.class);

            ArrayList<HashMap<String, Object>> links = (ArrayList<HashMap<String, Object>>) payload
                    .get(Constants.REQ_LINKS);

            for (HashMap<String, Object> link : links) {

                String href = (String) link.get(Constants.REQ_HREF);

                // remove prefix
                ArrayList<String> hrefSegments = new ArrayList<>(
                        Arrays.asList(href.split("/")));

                ArrayList<String> RouteResourceSegment = new ArrayList<>(
                        Arrays.asList(Constants.ROUTE_FULL_URI.split("/")));

                for (String path : RouteResourceSegment) {
                    hrefSegments.remove(path);
                }
                hrefSegments.remove(0);

                StringBuilder newHref = new StringBuilder();
                for (String path : hrefSegments) {
                    newHref.append("/" + path);
                }

                link.put(Constants.REQ_HREF, newHref.toString());
            }

            payload.put(Constants.REQ_LINKS, links);

            return cbor.encodingPayloadToCbor(payload);
        }
    }

}