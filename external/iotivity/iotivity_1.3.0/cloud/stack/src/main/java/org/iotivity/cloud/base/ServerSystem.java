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
package org.iotivity.cloud.base;

import io.netty.channel.ChannelHandler.Sharable;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.util.AttributeKey;

import java.util.ArrayList;
import java.util.List;

import org.iotivity.cloud.base.connector.CoapClient;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.exception.ClientException;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.exception.ServerException.InternalServerErrorException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapMessage;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.resource.ResourceManager;
import org.iotivity.cloud.base.server.CoapServer;
import org.iotivity.cloud.base.server.HttpServer;
import org.iotivity.cloud.base.server.Server;
import org.iotivity.cloud.base.server.WebSocketServer;
import org.iotivity.cloud.util.Log;

import io.netty.channel.ChannelHandler.Sharable;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.util.AttributeKey;

public class ServerSystem extends ResourceManager {
    private List<Server>               mServerList = new ArrayList<>();
    public static AttributeKey<Device> keyDevice   = AttributeKey
            .newInstance("device");

    @Sharable
    public class PersistentPacketReceiver
            extends SimpleChannelInboundHandler<CoapMessage> {
        @Override
        public void channelActive(ChannelHandlerContext ctx) {
            StringBuilder deviceId = new StringBuilder(
                    ctx.channel().id().asLongText().substring(26));
            deviceId.deleteCharAt(25);
            deviceId.insert(13, '-');
            deviceId.insert(18, '-');
            deviceId.insert(23, '-');
            CoapDevice device = new CoapDevice(ctx);
            device.updateDevice(deviceId.toString(), null, null);
            ctx.channel().attr(ServerSystem.keyDevice).set(device);

            device.onConnected();
        }

        @Override
        protected void channelRead0(ChannelHandlerContext ctx,
                CoapMessage msg) {
            try {
                // Find proper device and raise event.
                Device targetDevice = ctx.channel().attr(ServerSystem.keyDevice)
                        .get();

                if (targetDevice == null) {
                    throw new InternalServerErrorException(
                            "Unable to find device");
                }

                if (msg instanceof CoapRequest) {
                    onRequestReceived(targetDevice, (CoapRequest) msg);
                } else if (msg instanceof CoapResponse) {
                    // TODO: Re-architecturing required
                    IRequestChannel reqChannel = ((CoapDevice) targetDevice)
                            .getRequestChannel();
                    CoapClient coapClient = (CoapClient) reqChannel;
                    coapClient.onResponseReceived(msg);
                }

            } catch (ServerException e) {
                ctx.writeAndFlush(MessageBuilder.createResponse(msg,
                        e.getErrorResponse()));
                Log.f(ctx.channel(), e);
            } catch (ClientException e) {
                Log.f(ctx.channel(), e);
            } catch (Throwable t) {
                Log.f(ctx.channel(), t);
                if (msg instanceof CoapRequest) {
                    ctx.writeAndFlush(MessageBuilder.createResponse(msg,
                            ResponseStatus.INTERNAL_SERVER_ERROR));
                }
            }
        }

        @Override
        public void channelInactive(ChannelHandlerContext ctx)
                throws Exception {
            Device device = ctx.channel().attr(ServerSystem.keyDevice).get();
            device.onDisconnected();
            ctx.channel().attr(ServerSystem.keyDevice).remove();
        }
    }

    @Sharable
    public class NonPersistentPacketReceiver
            extends SimpleChannelInboundHandler<IRequest> {

        @Override
        public void channelRead0(ChannelHandlerContext ctx, IRequest msg)
                throws Exception {

            try {
                // Find proper device and raise event.
                Device targetDevice = ctx.channel().attr(ServerSystem.keyDevice)
                        .get();

                onRequestReceived(targetDevice, msg);

            } catch (ServerException e) {
                Log.f(ctx.channel(), e);
                ctx.writeAndFlush(MessageBuilder.createResponse(msg,
                        e.getErrorResponse()));
            } catch (Throwable t) {
                Log.f(ctx.channel(), t);
                ctx.writeAndFlush(MessageBuilder.createResponse(msg,
                        ResponseStatus.INTERNAL_SERVER_ERROR));
            }
        }
    }

    public void addServer(Server server) {
        if (server instanceof CoapServer || server instanceof WebSocketServer) {
            server.addHandler(new PersistentPacketReceiver());
        } else if (server instanceof HttpServer) {
            server.addHandler(new NonPersistentPacketReceiver());
        }

        mServerList.add(server);
    }

    public void startSystem(boolean tlsMode) throws Exception {
        for (Server server : mServerList) {
            server.startServer(tlsMode);
        }
    }

    public void stopSystem() throws Exception {
        for (Server server : mServerList) {
            server.stopServer();
        }
    }
}
