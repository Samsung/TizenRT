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
package org.iotivity.cloud.base.server;

import io.netty.channel.ChannelHandler;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpServerCodec;
import io.netty.handler.codec.http.websocketx.WebSocketServerProtocolHandler;
import io.netty.handler.codec.http.websocketx.extensions.compression.WebSocketServerCompressionHandler;

import java.net.InetSocketAddress;

import org.iotivity.cloud.base.protocols.coap.CoapLogHandler;
import org.iotivity.cloud.base.protocols.coap.websocket.WebSocketFrameHandler;

public class WebSocketServer extends Server {

    private final int    MAX_CONTENT_LENGTH = 65536;
    private final String PATH               = "/.well-known/coap";
    private final String SUBPROTOCOL        = "coap";

    public WebSocketServer(InetSocketAddress inetSocketAddress) {
        super(inetSocketAddress);
    }

    @Override
    protected ChannelHandler[] onQueryDefaultHandler() {

        return new ChannelHandler[] { new HttpServerCodec(),
                new HttpObjectAggregator(MAX_CONTENT_LENGTH),
                new WebSocketServerCompressionHandler(),
                new WebSocketServerProtocolHandler(PATH, SUBPROTOCOL, true),
                new WebSocketFrameHandler(), new CoapLogHandler() };
    }
}