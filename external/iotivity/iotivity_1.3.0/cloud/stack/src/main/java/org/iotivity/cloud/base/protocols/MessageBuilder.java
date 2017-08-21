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

import org.iotivity.cloud.base.OICConstants;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.coap.CoapSignaling;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.protocols.enums.SignalingMethod;

public class MessageBuilder {

    public static IResponse createResponse(IRequest request,
            ResponseStatus responseStatus) {
        return createResponse(request, responseStatus, ContentFormat.NO_CONTENT,
                null);
    }

    public static IResponse createSignalingResponse(ISignaling signaling,
            ResponseStatus responseStatus) {
        return createSignalingResponse(signaling, responseStatus, null);
    }

    public static IResponse createSignalingResponse(ISignaling signaling,
            ResponseStatus responseStatus, byte[] payload) {
        IResponse response = null;

        if (signaling instanceof CoapSignaling) {
            CoapSignaling coapSignaling = (CoapSignaling) signaling;
            CoapResponse coapResponse = new CoapResponse(responseStatus);
            coapResponse.setToken(coapSignaling.getToken());
            if (payload != null) {
                coapResponse.setPayload(payload);
            }

            response = coapResponse;
        }

        return response;
    }

    public static IResponse createResponse(IRequest request,
            ResponseStatus responseStatus, ContentFormat format,
            byte[] payload) {

        IResponse response = null;

        if (request instanceof CoapRequest) {
            CoapRequest coapRequest = (CoapRequest) request;
            CoapResponse coapResponse = new CoapResponse(responseStatus);
            coapResponse.setUriPath(coapRequest.getUriPath());
            coapResponse.setToken(coapRequest.getToken());
            if (payload != null) {
                coapResponse.setContentFormat(format);
                coapResponse.setPayload(payload);
            }

            if (coapRequest.getMethod() == RequestMethod.GET
                    && responseStatus == ResponseStatus.CONTENT) {
                switch (coapRequest.getObserve()) {
                    case SUBSCRIBE:
                    case SEQUENCE_NUMBER:
                        coapResponse.setSequenceNumber(
                                coapRequest.getNextSequenceNumber());
                        break;
                    default:
                }
            }

            coapResponse.setVersionValue(OICConstants.DEFAULT_VERSION_VALUE);

            response = coapResponse;
        }

        return response;
    }

    public static IRequest createRequest(RequestMethod requestMethod,
            String uriPath, String uriQuery) {
        return createRequest(requestMethod, uriPath, uriQuery,
                ContentFormat.NO_CONTENT, null);
    }

    public static ISignaling createSignaling(SignalingMethod signalingMethod,
            byte[] diagnosticPayload) {

        CoapSignaling coapSignaling = null;

        coapSignaling = new CoapSignaling(signalingMethod);
        // TODO: Random token generation required
        coapSignaling.setToken("tmptoken".getBytes());

        if (diagnosticPayload != null) {
            coapSignaling.setPayload(diagnosticPayload);
        }

        return coapSignaling;
    }

    public static ISignaling createSignaling(SignalingMethod signalingMethod) {
        return createSignaling(signalingMethod, null);
    }

    public static IRequest createRequest(RequestMethod requestMethod,
            String uriPath, String uriQuery, ContentFormat contentFormat,
            byte[] payload) {

        CoapRequest coapRequest = null;

        coapRequest = new CoapRequest(requestMethod);
        // TODO: Random token generation required
        coapRequest.setToken("tmptoken".getBytes());
        coapRequest.setUriPath(uriPath);
        if (uriQuery != null) {
            coapRequest.setUriQuery(uriQuery);
        }
        if (payload != null) {
            coapRequest.setContentFormat(contentFormat);
            coapRequest.setPayload(payload);
        }

        coapRequest.setVersionValue(OICConstants.DEFAULT_VERSION_VALUE);

        return coapRequest;
    }

    public static IRequest modifyRequest(IRequest orgRequest, String uriPath,
            String uriQuery, ContentFormat contentFormat, byte[] payload) {

        CoapRequest coapRequest = (CoapRequest) orgRequest;

        if (uriPath != null) {
            coapRequest.setUriPath(uriPath);
        }
        if (uriQuery != null) {
            coapRequest.setUriQuery(uriQuery);
        }
        if (payload != null) {
            coapRequest.setContentFormat(contentFormat);
            coapRequest.setPayload(payload);
        }

        coapRequest.setVersionValue(OICConstants.DEFAULT_VERSION_VALUE);

        return coapRequest;
    }

    public static IResponse modifyResponse(IResponse orgResponse,
            ContentFormat contentFormat, byte[] payload) {

        return modifyResponse(orgResponse, null, contentFormat, payload);
    }

    public static IResponse modifyResponse(IResponse orgResponse,
            String uriPath, ContentFormat contentFormat, byte[] payload) {

        CoapResponse coapResponse = (CoapResponse) orgResponse;

        if (uriPath != null) {
            coapResponse.setUriPath(uriPath);
        }
        if (payload != null) {
            coapResponse.setContentFormat(contentFormat);
            coapResponse.setPayload(payload);
        }

        coapResponse.setVersionValue(OICConstants.DEFAULT_VERSION_VALUE);

        return coapResponse;
    }
}
