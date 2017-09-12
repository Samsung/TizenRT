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
package org.iotivity.cloud.ciserver;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import org.iotivity.cloud.base.OICConstants;
import org.iotivity.cloud.base.ServerSystem;
import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.exception.ClientException;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.exception.ServerException.BadOptionException;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.exception.ServerException.InternalServerErrorException;
import org.iotivity.cloud.base.exception.ServerException.UnAuthorizedException;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.coap.CoapSignaling;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.protocols.enums.SignalingMethod;
import org.iotivity.cloud.base.server.CoapServer;
import org.iotivity.cloud.base.server.HttpServer;
import org.iotivity.cloud.base.server.Server;
import org.iotivity.cloud.base.server.WebSocketServer;
import org.iotivity.cloud.util.Bytes;
import org.iotivity.cloud.util.Cbor;
import org.iotivity.cloud.util.Log;

import io.netty.channel.ChannelDuplexHandler;
import io.netty.channel.ChannelHandler.Sharable;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import io.netty.channel.ChannelPromise;

/**
 *
 * This class provides a set of APIs to manage all of request
 *
 */

public class DeviceServerSystem extends ServerSystem {

    private Cbor<HashMap<String, Object>>                 mCbor   = new Cbor<HashMap<String, Object>>();
    private HashMap<ChannelHandlerContext, CoapSignaling> mCsmMap = new HashMap<>();

    /**
     *
     * This class provides a set of APIs to manage device pool.
     *
     */
    public static class CoapDevicePool {
        HashMap<String, Device> mMapDevice = new HashMap<>();

        /**
         * API for adding device information into pool.
         * 
         * @param device
         *            device to be added
         */
        public void addDevice(Device device) {
            String deviceId = ((CoapDevice) device).getDeviceId();
            synchronized (mMapDevice) {
                mMapDevice.put(deviceId, device);
            }
        }

        /**
         * API for removing device information into pool.
         * 
         * @param device
         *            device to be removed
         */
        public void removeDevice(Device device) throws ClientException {
            String deviceId = ((CoapDevice) device).getDeviceId();
            synchronized (mMapDevice) {
                if (mMapDevice.get(deviceId) == device) {
                    mMapDevice.remove(deviceId);
                }
            }
            removeObserveDevice(device);
        }

        private void removeObserveDevice(Device device) throws ClientException {
            synchronized (mMapDevice) {
                Iterator<String> iterator = mMapDevice.keySet().iterator();
                while (iterator.hasNext()) {
                    String deviceId = iterator.next();
                    CoapDevice getDevice = (CoapDevice) queryDevice(deviceId);
                    getDevice.removeObserveChannel(
                            ((CoapDevice) device).getRequestChannel());
                }
            }
        }

        /**
         * API for getting device information.
         * 
         * @param deviceId
         *            device id to get device
         */
        public Device queryDevice(String deviceId) {
            Device device = null;
            synchronized (mMapDevice) {
                device = mMapDevice.get(deviceId);
            }
            return device;
        }
    }

    CoapDevicePool mDevicePool = new CoapDevicePool();

    /**
     *
     * This class provides a set of APIs to manage life cycle of coap message.
     *
     */
    @Sharable
    class CoapLifecycleHandler extends ChannelDuplexHandler {
        @Override
        public void channelRead(ChannelHandlerContext ctx, Object msg) {

            if (msg instanceof CoapRequest) {
                try {
                    CoapDevice coapDevice = (CoapDevice) ctx.channel()
                            .attr(keyDevice).get();

                    if (coapDevice.isExpiredTime()) {
                        throw new UnAuthorizedException("token is expired");
                    }

                    CoapRequest coapRequest = (CoapRequest) msg;
                    IRequestChannel targetChannel = null;
                    String urlPath = coapRequest.getUriPath();

                    if (urlPath == null) {
                        throw new InternalServerErrorException(
                                "request uriPath is null");
                    }

                    if (urlPath.contains(Constants.ROUTE_FULL_URI)) {

                        int RouteResourcePathSize = Constants.ROUTE_FULL_URI
                                .split("/").length;
                        List<String> uriPath = coapRequest.getUriPathSegments();
                        if (uriPath != null && !uriPath.isEmpty()) {
                            CoapDevice targetDevice = (CoapDevice) mDevicePool
                                    .queryDevice(uriPath
                                            .get(RouteResourcePathSize - 1));
                            targetChannel = targetDevice.getRequestChannel();
                        }

                        switch (coapRequest.getObserve()) {
                            case SUBSCRIBE:
                                coapDevice.addObserveRequest(
                                        Bytes.bytesToLong(
                                                coapRequest.getToken()),
                                        coapRequest);
                                coapDevice.addObserveChannel(targetChannel);
                                break;
                            case UNSUBSCRIBE:
                                coapDevice.removeObserveChannel(targetChannel);
                                coapDevice.removeObserveRequest(Bytes
                                        .bytesToLong(coapRequest.getToken()));
                                break;
                            default:
                                break;
                        }
                    }
                } catch (Throwable t) {
                    Log.f(ctx.channel(), t);
                    ResponseStatus responseStatus = t instanceof ServerException
                            ? ((ServerException) t).getErrorResponse()
                            : ResponseStatus.INTERNAL_SERVER_ERROR;
                    ctx.writeAndFlush(MessageBuilder
                            .createResponse((CoapRequest) msg, responseStatus));
                    ctx.close();
                }
            }
            ctx.fireChannelRead(msg);
        }

        @Override
        public void write(ChannelHandlerContext ctx, Object msg,
                ChannelPromise promise) throws Exception {

            boolean bCloseConnection = false;

            if (msg instanceof CoapResponse) {
                // This is CoapResponse
                // Once the response is valid, add this to deviceList
                CoapResponse response = (CoapResponse) msg;

                String urlPath = response.getUriPath();

                if (urlPath == null) {
                    throw new InternalServerErrorException(
                            "request uriPath is null");
                }

                switch (urlPath) {
                    case OICConstants.ACCOUNT_SESSION_FULL_URI:
                        if (response.getStatus() != ResponseStatus.CHANGED) {
                            bCloseConnection = true;
                        }
                        break;
                    case OICConstants.ACCOUNT_FULL_URI:
                        if (response.getStatus() == ResponseStatus.DELETED) {
                            bCloseConnection = true;
                        }
                        break;
                }
            }

            ctx.writeAndFlush(msg);

            if (bCloseConnection == true) {
                ctx.close();
            }
        }

        @Override
        public void channelActive(ChannelHandlerContext ctx) {
            Device device = ctx.channel().attr(keyDevice).get();
            // Authenticated device connected

            sendDevicePresence(device.getDeviceId(), "on");
            mDevicePool.addDevice(device);

            device.onConnected();
        }

        @Override
        public void channelInactive(ChannelHandlerContext ctx)
                throws ClientException {
            Device device = ctx.channel().attr(keyDevice).get();

            // Some cases, this event occurs after new device connected using
            // same di.
            // So compare actual value, and remove if same.
            if (device != null) {
                sendDevicePresence(device.getDeviceId(), "off");

                device.onDisconnected();

                mDevicePool.removeDevice(device);
                ctx.channel().attr(keyDevice).remove();

            }
        }

        /**
         * API for sending state to resource directory
         * 
         * @param deviceId
         *            device id to be sent to resource directory
         * @param state
         *            device state to be sent to resource directory
         */
        public void sendDevicePresence(String deviceId, String state) {

            Cbor<HashMap<String, Object>> cbor = new Cbor<>();
            HashMap<String, Object> payload = new HashMap<String, Object>();
            payload.put(Constants.REQ_DEVICE_ID, deviceId);
            payload.put(Constants.PRESENCE_STATE, state);
            StringBuffer uriPath = new StringBuffer();
            uriPath.append("/" + Constants.PREFIX_OIC);
            uriPath.append("/" + Constants.DEVICE_PRESENCE_URI);
            ConnectorPool.getConnection("rd")
                    .sendRequest(MessageBuilder.createRequest(
                            RequestMethod.POST, uriPath.toString(), null,
                            ContentFormat.APPLICATION_CBOR,
                            cbor.encodingPayloadToCbor(payload)), null);
        }
    }

    CoapLifecycleHandler mLifeCycleHandler = new CoapLifecycleHandler();

    @Sharable
    class CoapAuthHandler extends ChannelDuplexHandler {

        @Override
        public void channelActive(ChannelHandlerContext ctx) {
            // Actual channel active should decided after authentication.
        }

        @Override
        public void write(ChannelHandlerContext ctx, Object msg,
                ChannelPromise promise) {
            try {

                if (!(msg instanceof CoapResponse)) {
                    // throw new BadRequestException(
                    // "this msg type is not CoapResponse");

                    // TODO check websocket handshake response
                    ctx.writeAndFlush(msg);
                    return;
                }
                // This is CoapResponse
                // Once the response is valid, add this to deviceList

                CoapResponse response = (CoapResponse) msg;

                String urlPath = response.getUriPath();

                if (urlPath == null) {
                    throw new InternalServerErrorException(
                            "request uriPath is null");
                }

                switch (urlPath) {
                    /*
                     * case OICConstants.ACCOUNT_FULL_URI:
                     * ctx.writeAndFlush(msg); ctx.close(); return;
                     */

                    case OICConstants.ACCOUNT_SESSION_FULL_URI:
                        HashMap<String, Object> payloadData = mCbor
                                .parsePayloadFromCbor(response.getPayload(),
                                        HashMap.class);

                        if (response.getStatus() != ResponseStatus.CHANGED) {
                            throw new UnAuthorizedException();
                        }

                        if (payloadData == null) {
                            throw new BadRequestException("payload is empty");
                        }
                        int remainTime = (int) payloadData
                                .get(Constants.EXPIRES_IN);

                        Device device = ctx.channel().attr(keyDevice).get();
                        ((CoapDevice) device).setExpiredPolicy(remainTime);

                        // Remove current auth handler and replace to
                        // LifeCycleHandle
                        ctx.channel().pipeline().replace(this,
                                "LifeCycleHandler", mLifeCycleHandler);

                        // Raise event that we have Authenticated device
                        ctx.fireChannelActive();

                        break;
                }

                ctx.writeAndFlush(msg);

            } catch (Throwable t) {
                Log.f(ctx.channel(), t);
                ctx.writeAndFlush(msg);
                ctx.close();
            }
        }

        @Override
        public void channelRead(ChannelHandlerContext ctx, Object msg) {
            try {
                if (!(msg instanceof CoapRequest)) {
                    throw new BadRequestException(
                            "this msg type is not CoapRequest");
                }

                // And check first response is VALID then add or cut
                CoapRequest request = (CoapRequest) msg;

                String urlPath = request.getUriPath();

                if (urlPath == null) {
                    throw new InternalServerErrorException(
                            "request uriPath is null");
                }

                switch (urlPath) {
                    // Check whether request is about account
                    case OICConstants.ACCOUNT_FULL_URI:
                    case OICConstants.ACCOUNT_TOKENREFRESH_FULL_URI:

                        if (ctx.channel().attr(keyDevice).get() == null) {
                            // Create device first and pass to upperlayer
                            Device device = new CoapDevice(ctx);
                            ctx.channel().attr(keyDevice).set(device);
                        }

                        break;

                    case OICConstants.ACCOUNT_SESSION_FULL_URI:

                        HashMap<String, Object> authPayload = mCbor
                                .parsePayloadFromCbor(request.getPayload(),
                                        HashMap.class);

                        Device device = ctx.channel().attr(keyDevice).get();

                        if (device == null) {
                            device = new CoapDevice(ctx);
                            ctx.channel().attr(keyDevice).set(device);
                        }

                        if (authPayload == null) {
                            throw new BadRequestException("payload is empty");
                        }

                        ((CoapDevice) device).updateDevice(
                                (String) authPayload.get(Constants.DEVICE_ID),
                                (String) authPayload.get(Constants.USER_ID),
                                (String) authPayload
                                        .get(Constants.ACCESS_TOKEN));

                        break;

                    case OICConstants.KEEP_ALIVE_FULL_URI:
                        // TODO: Pass ping request to upper layer
                        break;

                    default:
                        throw new UnAuthorizedException(
                                "authentication required first");
                }

                ctx.fireChannelRead(msg);

            } catch (Throwable t) {
                ResponseStatus responseStatus = t instanceof ServerException
                        ? ((ServerException) t).getErrorResponse()
                        : ResponseStatus.UNAUTHORIZED;
                ctx.writeAndFlush(MessageBuilder
                        .createResponse((CoapRequest) msg, responseStatus));
                Log.f(ctx.channel(), t);
            }
        }
    }

    @Sharable
    static class HttpAuthHandler extends ChannelDuplexHandler {
        @Override
        public void channelActive(ChannelHandlerContext ctx) throws Exception {
            // After current channel authenticated, raise to upper layer
        }
    }

    @Sharable
    class CoapSignalingHandler extends ChannelInboundHandlerAdapter {

        @Override
        public void channelInactive(ChannelHandlerContext ctx)
                throws Exception {
            // delete csm information from the map
            mCsmMap.remove(ctx);
            ctx.fireChannelInactive();
        }

        @Override
        public void channelRead(ChannelHandlerContext ctx, Object msg) {
            try {
                if (msg instanceof CoapSignaling) {
                    if (mCsmMap.get(ctx) == null) {
                        // In the server, the CSM message is sent to the device
                        // once
                        CoapSignaling inicialCsm = (CoapSignaling) MessageBuilder
                                .createSignaling(SignalingMethod.CSM);
                        inicialCsm.setCsmMaxMessageSize(4294967295L);
                        ctx.writeAndFlush(inicialCsm);
                    }
                    CoapSignaling signaling = (CoapSignaling) msg;
                    switch (signaling.getSignalingMethod()) {
                        case CSM:
                            // get existing CSM from the map
                            CoapSignaling existingCsm = mCsmMap.get(ctx);
                            if (existingCsm == null) {
                                existingCsm = signaling;
                            } else {
                                // replace and cumulate CSM options
                                existingCsm.setCsmBlockWiseTransfer(
                                        signaling.getCsmBlockWiseTransfer());
                                existingCsm.setCsmMaxMessageSize(
                                        signaling.getCsmMaxMessageSize());
                                existingCsm.setCsmServerName(
                                        signaling.getCsmServerName());
                            }
                            mCsmMap.put(ctx, existingCsm);
                            break;
                        case PING:
                            // TODO process PING signaling option
                            break;
                        case PONG:
                            // TODO process PONG signaling option
                            break;
                        case RELEASE:
                        case ABORT:
                            mCsmMap.remove(ctx);
                            ctx.close();
                            break;
                        default:
                            throw new BadOptionException(
                                    "unsupported CoAP Signaling option");
                    }

                    ctx.fireChannelRead(msg);
                } else {
                    ctx.fireChannelRead(msg);
                    // TODO annotated codes must be removed to follow
                    // the CSM specification of draft-ietf-core-coap-tcp-tls-05

                    // if (mCsmMap.get(ctx) != null) {
                    // ctx.fireChannelRead(msg);
                    // } else {
                    // // send ABORT signaling and close the connection
                    // ctx.writeAndFlush(MessageBuilder.createSignaling(
                    // SignalingMethod.ABORT,
                    // new String(
                    // "Capability and Settings message (CSM) is not received
                    // yet")
                    // .getBytes()));
                    // ctx.close();
                    // }
                }
            } catch (Throwable t) {
                ResponseStatus responseStatus = t instanceof ServerException
                        ? ((ServerException) t).getErrorResponse()
                        : ResponseStatus.BAD_OPTION;
                if (msg instanceof CoapRequest) {
                    ctx.writeAndFlush(MessageBuilder
                            .createResponse((CoapRequest) msg, responseStatus));
                } else if (msg instanceof CoapSignaling) {
                    ctx.writeAndFlush(MessageBuilder.createSignalingResponse(
                            (CoapSignaling) msg, responseStatus));
                }
                Log.f(ctx.channel(), t);
            }
        }

    }

    @Override
    public void addServer(Server server) {
        if (server instanceof CoapServer) {
            server.addHandler(new CoapSignalingHandler());
            server.addHandler(new CoapAuthHandler());
        }

        if (server instanceof WebSocketServer) {
            server.addHandler(new CoapAuthHandler());
        }

        if (server instanceof HttpServer) {
            server.addHandler(new HttpAuthHandler());
        }

        super.addServer(server);
    }

    public CoapDevicePool getDevicePool() {
        return mDevicePool;
    }
}
