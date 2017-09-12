/*
 * //******************************************************************
 * //
 * // Copyright 2017 Samsung Electronics All Rights Reserved.
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
package org.iotivity.cloud.base.protocols.coap.websocket;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.exception.ServerException.InternalServerErrorException;
import org.iotivity.cloud.base.protocols.coap.CoapDecoder;
import org.iotivity.cloud.base.protocols.coap.CoapEncoder;
import org.iotivity.cloud.base.protocols.coap.CoapMessage;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.util.Cbor;
import org.iotivity.cloud.util.JSONUtil;
import org.iotivity.cloud.util.Log;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelDuplexHandler;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelPromise;
import io.netty.handler.codec.http.DefaultFullHttpResponse;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpServerCodec;
import io.netty.handler.codec.http.websocketx.BinaryWebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketFrame;

public class WebSocketFrameHandler extends ChannelDuplexHandler {

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        Log.v(ctx.channel().id().asLongText().substring(26)
                + " WebSocket Connected, Address: "
                + ctx.channel().remoteAddress().toString());

        ctx.fireChannelActive();
    }

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        Log.v(ctx.channel().id().asLongText().substring(26)
                + " WebSocket Disconnected, Address: "
                + ctx.channel().remoteAddress().toString());

        ctx.fireChannelInactive();
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg)
            throws Exception {
        // TODO check ping pong

        if (msg instanceof BinaryWebSocketFrame) {

            List<Object> messages = new ArrayList<>();
            new CoapDecoder().decode(((BinaryWebSocketFrame) msg).content(),
                    messages,
                    ((BinaryWebSocketFrame) msg).content().readableBytes());

            for (Object message : messages) {
                if (message instanceof CoapMessage) {
                    CoapMessage coapMessage = (CoapMessage) message;

                    // convert content format to cbor if content format is json.
                    if (coapMessage.getPayloadSize() != 0
                            && coapMessage.getContentFormat()
                                    .equals(ContentFormat.APPLICATION_JSON)) {
                        byte[] payload = coapMessage.getPayload();
                        coapMessage.setPayload(convertJsonToCbor(payload));
                        coapMessage.setContentFormat(
                                ContentFormat.APPLICATION_CBOR);
                    }
                    ctx.fireChannelRead(coapMessage);
                }
            }
        } else {
            throw new BadRequestException("invalid request message type");
        }
    }

    @Override
    public void write(ChannelHandlerContext ctx, Object msg,
            ChannelPromise promise) throws Exception {
        Object newMsg = msg;

        if (msg instanceof DefaultFullHttpResponse) {

            ChannelFuture ch = ctx.writeAndFlush(newMsg);
            ch.addListener(new ChannelFutureListener() {

                @Override
                public void operationComplete(ChannelFuture future)
                        throws Exception {
                    Log.v(future.channel().id().asLongText().substring(26)
                            + " WebSocket Handshake done, Address: "
                            + future.channel().remoteAddress().toString());

                    // remove http encoder/decoder after handshake done.
                    future.channel().pipeline().remove(HttpServerCodec.class);
                    future.channel().pipeline()
                            .remove(HttpObjectAggregator.class);
                }
            });

            return;
        }
        if (msg instanceof CoapMessage) {

            CoapMessage coapMessage = (CoapMessage) msg;

            // covert content format to json.
            if (coapMessage.getPayloadSize() != 0) {
                byte[] payload = coapMessage.getPayload();
                coapMessage.setPayload(convertCborToJson(payload));
                coapMessage.setContentFormat(ContentFormat.APPLICATION_JSON);
            }

            ByteBuf encodedBytes = Unpooled.buffer();
            new CoapEncoder().encode((CoapMessage) msg, encodedBytes, true);
            WebSocketFrame frame = new BinaryWebSocketFrame(encodedBytes);
            newMsg = frame;
        } else {
            throw new InternalServerErrorException(
                    "invalid response message type");
        }

        ctx.writeAndFlush(newMsg);
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause)
            throws Exception {

        cause.printStackTrace();
    }

    private byte[] convertJsonToCbor(byte[] jsonData) {

        JSONUtil<HashMap<String, Object>> json = new JSONUtil<>();
        HashMap<String, Object> parsedData = json.parseJSON(jsonData,
                HashMap.class);

        Cbor<HashMap<String, Object>> cbor = new Cbor<>();
        return cbor.encodingPayloadToCbor(parsedData);
    }

    private byte[] convertCborToJson(byte[] cborData) {

        Cbor<Object> cbor = new Cbor<>();
        Object parsedData = cbor.parsePayloadFromCbor(cborData, Object.class);

        JSONUtil<String> json = new JSONUtil<>();
        return json.writeJSON(parsedData).getBytes();
    }
}