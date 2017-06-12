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

import java.io.File;
import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import javax.net.ssl.SSLException;

import org.iotivity.cloud.base.OICConstants;
import org.iotivity.cloud.base.protocols.coap.CoapDecoder;
import org.iotivity.cloud.base.protocols.coap.CoapEncoder;
import org.iotivity.cloud.base.protocols.coap.CoapLogHandler;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;

import io.netty.bootstrap.Bootstrap;
import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandler.Sharable;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.ssl.SslContext;
import io.netty.handler.ssl.SslContextBuilder;
import io.netty.handler.ssl.SslProvider;

public class CoapConnector {

    public CoapConnector() {

        mBootstrap.group(mConnectorGroup);
        mBootstrap.channel(NioSocketChannel.class);
        mBootstrap.option(ChannelOption.TCP_NODELAY, true);
        mBootstrap.option(ChannelOption.SO_KEEPALIVE, true);
        mBootstrap.option(ChannelOption.SO_REUSEADDR, true);
    }

    @Sharable
    private class CoapPacketHandler
            extends SimpleChannelInboundHandler<CoapResponse> {

        @Override
        protected void channelRead0(ChannelHandlerContext ctx, CoapResponse msg)
                throws Exception {
            mChannelMap.get(ctx.channel()).onResponseReceived(msg);
        }
    }

    public static class CoapConnectorInitializer
            extends ChannelInitializer<SocketChannel> {

        private List<ChannelHandler> additionalHandlers = new ArrayList<>();

        private Boolean              mTlsMode           = false;
        InetSocketAddress            mInetSocketAddress = null;
        String                       mRootCertFiePath   = null;

        public void setTlsMode(Boolean tlsMode) {
            this.mTlsMode = tlsMode;
        }

        public void setInetSocketAddress(InetSocketAddress address) {
            this.mInetSocketAddress = address;
        }

        public void setRootCertFilePath(String path) {
            this.mRootCertFiePath = path;
        }

        public void addHandler(ChannelHandler handler) {
            additionalHandlers.add(handler);
        }

        @Override
        public void initChannel(SocketChannel ch) {
            ChannelPipeline p = ch.pipeline();

            SslContext sslContext = null;

            if (mTlsMode.equals(true)) {

                File rootCert = new File(mRootCertFiePath);

                try {
                    sslContext = SslContextBuilder.forClient()
                            .sslProvider(SslProvider.JDK).trustManager(rootCert)
                            .build();
                } catch (SSLException e) {
                    e.printStackTrace();
                }

                final SslContext sslCtx = sslContext;
                p.addLast(sslCtx.newHandler(ch.alloc(),
                        mInetSocketAddress.getHostString(),
                        mInetSocketAddress.getPort()));
            }

            p.addLast(new CoapDecoder());
            p.addLast(new CoapEncoder());
            p.addLast(new CoapLogHandler());
            for (ChannelHandler handler : additionalHandlers) {
                p.addLast(handler);
            }
        }
    }

    HashMap<Channel, CoapClient> mChannelMap     = new HashMap<>();

    Bootstrap                    mBootstrap      = new Bootstrap();
    EventLoopGroup               mConnectorGroup = new NioEventLoopGroup();

    public CoapClient connect(final InetSocketAddress inetSocketAddress,
            boolean tlsMode) throws InterruptedException {

        CoapConnectorInitializer initializer = new CoapConnectorInitializer();

        if (tlsMode == true) {
            initializer.setTlsMode(true);
            initializer.setInetSocketAddress(inetSocketAddress);
            initializer.setRootCertFilePath(OICConstants.ROOT_CERT_FILE);
        }

        initializer.addHandler(new CoapPacketHandler());
        mBootstrap.handler(initializer);

        ChannelFuture channelFuture = null;
        channelFuture = mBootstrap.connect(inetSocketAddress).sync();

        CoapClient coapClient = null;
        coapClient = new CoapClient(channelFuture.channel());
        mChannelMap.put(channelFuture.channel(), coapClient);

        return coapClient;
    }

    public void disconenct() throws Exception {
        mConnectorGroup.shutdownGracefully().await();
    }
}
