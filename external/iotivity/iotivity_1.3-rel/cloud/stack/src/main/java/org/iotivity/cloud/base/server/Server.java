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
package org.iotivity.cloud.base.server;

import java.io.File;
import java.net.InetSocketAddress;
import java.security.cert.CertificateException;
import java.util.ArrayList;
import java.util.List;

import javax.net.ssl.SSLException;

import org.iotivity.cloud.base.OICConstants;
import org.iotivity.cloud.util.Log;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.logging.LogLevel;
import io.netty.handler.logging.LoggingHandler;
import io.netty.handler.ssl.SslContext;
import io.netty.handler.ssl.SslContextBuilder;

public abstract class Server {

    EventLoopGroup    acceptorGroup      = new NioEventLoopGroup(1);

    EventLoopGroup    workerGroup        = new NioEventLoopGroup();

    ServerInitializer mServerInitializer = new ServerInitializer();

    InetSocketAddress mInetSocketAddress = null;

    SslContext        mSslContext        = null;

    private class ServerInitializer extends ChannelInitializer<SocketChannel> {
        private List<ChannelHandler> additionalHandlers = new ArrayList<>();

        public ServerInitializer() {
        }

        public void addHandler(ChannelHandler handler) {
            additionalHandlers.add(handler);
        }

        @Override
        public void initChannel(SocketChannel ch) throws Exception {
            ChannelPipeline p = ch.pipeline();

            if (mSslContext != null) {
                p.addLast(mSslContext.newHandler(ch.alloc()));
            }

            p.addLast(onQueryDefaultHandler());

            for (ChannelHandler handler : additionalHandlers) {
                p.addLast(handler);
            }
        }
    }

    public Server(InetSocketAddress inetSocketAddress) {
        mInetSocketAddress = inetSocketAddress;
    }

    public void startServer(boolean tlsMode)
            throws CertificateException, SSLException, InterruptedException {

        try {
            if (tlsMode)
                Log.i("Server starts with TLS!");

            if (tlsMode == true) {

                File serverCert = new File(OICConstants.CLOUD_CERT_FILE);

                File serverKey = new File(OICConstants.CLOUD_KEY_FILE);

                mSslContext = SslContextBuilder.forServer(serverCert, serverKey)
                        .build();
            }

            ServerBootstrap b = new ServerBootstrap();
            b.group(acceptorGroup, workerGroup);
            b.channel(NioServerSocketChannel.class);
            b.handler(new LoggingHandler(LogLevel.INFO));

            b.childHandler(mServerInitializer);

            b.bind(mInetSocketAddress).sync();
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
        }
    }

    public void stopServer() throws Exception {
        acceptorGroup.shutdownGracefully().await();
        workerGroup.shutdownGracefully().await();
    }

    public void addHandler(ChannelHandler handler) {
        mServerInitializer.addHandler(handler);
    }

    abstract protected ChannelHandler[] onQueryDefaultHandler();
}
