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
package org.iotivity.cloud.base.protocols.coap;

import java.util.ArrayList;

import org.iotivity.cloud.base.OICConstants;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.util.Cbor;
import org.iotivity.cloud.util.Log;

import com.google.gson.Gson;

import io.netty.channel.ChannelDuplexHandler;
import io.netty.channel.ChannelHandler.Sharable;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelPromise;

/**
 *
 * This class provides a set of APIs to print out logs for CoAP request and
 * response.
 *
 */
@Sharable
public class CoapLogHandler extends ChannelDuplexHandler {

    private Cbor<Object> mCbor      = new Cbor<>();
    private Gson         mGson      = new Gson();

    static final int     MAX_LOGLEN = 100;

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        Log.v(ctx.channel().id().asLongText().substring(26)
                + " Connected, Address: "
                + ctx.channel().remoteAddress().toString());

        ctx.fireChannelActive();
    }

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        Log.v(ctx.channel().id().asLongText().substring(26)
                + " Disconnected, Address: "
                + ctx.channel().remoteAddress().toString());

        ctx.fireChannelInactive();
    }

    @Override
    public void write(ChannelHandlerContext ctx, Object msg,
            ChannelPromise promise) {

        String log = null;

        if (msg instanceof CoapRequest) {
            log = composeCoapRequest(
                    ctx.channel().id().asLongText().substring(26),
                    (CoapRequest) msg);
        } else if (msg instanceof CoapResponse) {
            log = composeCoapResponse(ctx.channel().id().asLongText()
                    .substring(26), (CoapResponse) msg);
        }

        if (log != null) {
            Log.v(log);
        }

        ctx.writeAndFlush(msg);
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg)
            throws Exception {

        String log = null;

        if (msg instanceof CoapRequest) {
            log = composeCoapRequest(
                    ctx.channel().id().asLongText().substring(26),
                    (CoapRequest) msg);
        } else if (msg instanceof CoapResponse) {
            log = composeCoapResponse(ctx.channel().id().asLongText()
                    .substring(26), (CoapResponse) msg);
        }

        if (log != null) {
            Log.v(log);
        }

        ctx.fireChannelRead(msg);
    }

    private String composeCoapRequest(String channelId, CoapRequest request) {
        StringBuilder strBuilder = new StringBuilder();

        strBuilder.append(channelId);
        strBuilder.append(" " + request.getTokenString());

        switch (request.getMethod()) {
            case DELETE:
                strBuilder.append(" DELETE ");
                break;
            case GET:
                switch (request.getObserve()) {
                    case SUBSCRIBE:
                        strBuilder.append(" GET OBSERVE ");
                        break;
                    case UNSUBSCRIBE:
                        strBuilder.append(" GET OBSERVE CANCEL ");
                        break;
                    default:
                        strBuilder.append(" GET ");
                        break;
                }
                break;
            case POST:
                strBuilder.append(" POST ");
                break;
            case PUT:
                strBuilder.append(" PUT ");
                break;
        }

        strBuilder.append(request.getUriPath());
        String query = request.getUriQuery();
        if (query != null) {
            strBuilder.append("/?" + query);
        }

        if (request.getPayloadSize() > 0) {
            strBuilder.append(" CT:" + request.getContentFormat());
            strBuilder.append(" SZ:" + request.getPayloadSize() + " P:"
                    + getJsonPayloadString(request));
        }

        return strBuilder.toString();
    }

    private String composeCoapResponse(String channelId,
            CoapResponse response) {
        StringBuilder strBuilder = new StringBuilder();

        strBuilder.append(channelId);
        strBuilder.append(" " + response.getTokenString());

        switch (response.getStatus()) {
            case BAD_GATEWAY:
                strBuilder.append(" 5.02 Bad Gateway");
                break;
            case BAD_OPTION:
                strBuilder.append(" 4.02 Bad Option");
                break;
            case BAD_REQUEST:
                strBuilder.append(" 4.00 Bad Request");
                break;
            case CHANGED:
                strBuilder.append(" 2.04 Changed");
                break;
            case CONTENT:
                strBuilder.append(" 2.05 Content");
                break;
            case CREATED:
                strBuilder.append(" 2.01 Created");
                break;
            case DELETED:
                strBuilder.append(" 2.02 Deleted");
                break;
            case FORBIDDEN:
                strBuilder.append(" 4.03 Forbidden");
                break;
            case GATEWAY_TIMEOUT:
                strBuilder.append(" 5.04 Gateway Timeout");
                break;
            case INTERNAL_SERVER_ERROR:
                strBuilder.append(" 5.00 Internal Server Error");
                break;
            case METHOD_NOT_ALLOWED:
                strBuilder.append(" 4.05 Method Not Allowed");
                break;
            case NOT_ACCEPTABLE:
                strBuilder.append(" 4.06 Not Acceptable");
                break;
            case NOT_FOUND:
                strBuilder.append(" 4.04 Not Found");
                break;
            case NOT_IMPLEMENTED:
                strBuilder.append(" 5.01 Not Implemented");
                break;
            case PRECONDITION_FAILED:
                strBuilder.append(" 4.12 Precondition Failed");
                break;
            case PROXY_NOT_SUPPORTED:
                strBuilder.append(" 5.05 Proxying Not Supported");
                break;
            case REQUEST_ENTITY_TOO_LARGE:
                strBuilder.append(" 4.13 Request Entity Too Large");
                break;
            case SERVICE_UNAVAILABLE:
                strBuilder.append(" 5.03 Service Unavailable");
                break;
            case UNAUTHORIZED:
                strBuilder.append(" 4.01 Unauthorized");
                break;
            case UNSUPPORTED_CONTENT_FORMAT:
                strBuilder.append(" 4.15 Unsupported Content-Format");
                break;
            case VALID:
                strBuilder.append(" 2.03 Valid");
                break;
            default:
                break;
        }

        switch (response.getObserve()) {
            case SUBSCRIBE:
                strBuilder.append(" OBSERVE");
                break;
            case UNSUBSCRIBE:
                strBuilder.append(" OBSERVE CANCEL");
                break;
            case SEQUENCE_NUMBER:
                strBuilder.append(" OBSERVE SEQ:");
                strBuilder.append(response.getSequenceNumber());
                break;
            default:
                break;
        }

        if (response.getPayloadSize() > 0) {
            strBuilder.append(" CT:" + response.getContentFormat());
            strBuilder.append(" SZ:" + response.getPayloadSize() + " P:"
                    + getJsonPayloadString(response));
        }

        return strBuilder.toString();
    }

    private String getJsonPayloadString(CoapMessage coapMessage) {
        String jsonPayload = null;
        if (coapMessage.getContentFormat() == ContentFormat.APPLICATION_CBOR) {

            Object mapPayload = mCbor.parsePayloadFromCbor(
                    coapMessage.getPayload(), Object.class);
            if (coapMessage.getUriPath()
                    .contains(OICConstants.WELL_KNOWN_FULL_URI)) {
                jsonPayload = mGson.toJson((ArrayList<Object>) mapPayload);
                return jsonPayload.length() <= MAX_LOGLEN ? jsonPayload
                        : jsonPayload.substring(0, MAX_LOGLEN);
            }
            jsonPayload = mGson.toJson(mapPayload);
            return jsonPayload.length() <= MAX_LOGLEN ? jsonPayload
                    : jsonPayload.substring(0, MAX_LOGLEN);
        }
        return jsonPayload;
    }
}
