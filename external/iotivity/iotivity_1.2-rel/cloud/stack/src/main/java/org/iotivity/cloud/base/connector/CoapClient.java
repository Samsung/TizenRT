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
package org.iotivity.cloud.base.connector;

import java.util.HashMap;

import org.iotivity.cloud.base.OICConstants;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.device.IResponseEventHandler;
import org.iotivity.cloud.base.exception.ClientException;
import org.iotivity.cloud.base.exception.ClientException.RequesterGoneException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.enums.Observe;
import org.iotivity.cloud.util.Bytes;
import org.iotivity.cloud.util.Log;

import io.netty.channel.Channel;

public class CoapClient implements IRequestChannel, IResponseEventHandler {

    private class RequestInfo {
        private byte[]                originToken     = null;
        private IRequest              originRequest   = null;
        private IResponseEventHandler responseHandler = null;
        private Observe               observe         = Observe.NOTHING;

        public RequestInfo(byte[] originToken, IRequest originRequest,
                IResponseEventHandler responseHandler, Observe observe) {
            this.originToken = originToken;
            this.originRequest = originRequest;
            this.responseHandler = responseHandler;
            this.observe = observe;
        }
    }

    private HashMap<Long, RequestInfo> mTokenExchanger = new HashMap<>();
    private Long                       mToken          = 0L;
    private Channel                    mChannel        = null;

    private HashMap<Long, Long>        mSubscription   = new HashMap<>();

    public CoapClient(Channel channel) {
        mChannel = channel;
    }

    @Override
    public void sendRequest(IRequest request,
            IResponseEventHandler responseEvent) {
        // Exchange request token to internal token and
        // add token with responseHandler to map
        try {
            byte[] token = null;
            long newToken;
            synchronized (mToken) {
                newToken = mToken;
            }

            CoapRequest coapRequest = (CoapRequest) request;

            token = coapRequest.getToken();

            Observe observe = request.getObserve();

            switch (request.getObserve()) {
                case UNSUBSCRIBE:
                    newToken = removeObserve(Bytes.bytesToLong(token));
                    break;

                case SUBSCRIBE:
                    addObserve(Bytes.bytesToLong(token), newToken);
                default:
                    // We create temp token
                    // TODO: temporal handling
                    if (request.getUriPath()
                            .equals(OICConstants.RESOURCE_PRESENCE_FULL_URI)) {
                        addObserve(Bytes.bytesToLong(token), newToken);
                        observe = Observe.SUBSCRIBE;
                    }
                    synchronized (mToken) {
                        newToken = mToken++;
                    }
                    break;
            }

            coapRequest.setToken(Bytes.longTo8Bytes(newToken));
            mTokenExchanger.put(newToken,
                    new RequestInfo(token, request, responseEvent, observe));

            mChannel.writeAndFlush(request);

        } catch (Exception e) {
            Log.f(mChannel, e);
        }
    }

    @Override
    public void onResponseReceived(IResponse response) throws ClientException {
        // Response received from this device.
        // Exchange internal token to request token
        // And call actual requester device

        // Response is always CoapResponse
        CoapResponse coapResponse = (CoapResponse) response;

        RequestInfo reqInfo = mTokenExchanger
                .get(Bytes.bytesToLong(coapResponse.getToken()));

        if (reqInfo == null) {
            throw new RequesterGoneException("Unable to find "
                    + Bytes.bytesToLong(coapResponse.getToken()));
        }

        ((CoapRequest) reqInfo.originRequest).setToken(reqInfo.originToken);

        // Subscription response should stored
        if (reqInfo.observe != Observe.SUBSCRIBE) {
            mTokenExchanger.remove(Bytes.bytesToLong(coapResponse.getToken()));
            if (mSubscription
                    .containsKey(Bytes.bytesToLong(reqInfo.originToken))) {
                mSubscription.remove(Bytes.bytesToLong(reqInfo.originToken));
            }
        }

        if (reqInfo.responseHandler != null) {
            coapResponse.setToken(reqInfo.originToken);
            reqInfo.responseHandler.onResponseReceived(coapResponse);
        }
    }

    public void addObserve(long token, long newtoken) {

        mSubscription.put(token, newtoken);
    }

    public Long removeObserve(long token) {

        Long getToken = mSubscription.remove(token);
        return getToken;
    }

    public Long isObserveRequest(Long token) {
        Long getToken = null;
        getToken = mSubscription.get(token);

        return getToken;
    }
}
