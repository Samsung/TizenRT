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

import java.net.InetSocketAddress;
import java.util.Enumeration;
import java.util.concurrent.ConcurrentHashMap;

import org.iotivity.cloud.base.device.HttpDevice;
import org.iotivity.cloud.base.protocols.http.HCProxyHandler;
import org.iotivity.cloud.base.protocols.http.HttpLogHandler;
import org.iotivity.cloud.util.Log;

import io.netty.channel.ChannelHandler;
import io.netty.handler.codec.http.HttpRequestDecoder;
import io.netty.handler.codec.http.HttpResponseEncoder;

public class HttpServer extends Server {

    public static ConcurrentHashMap<String, HttpDevice> httpDeviceMap
    = new ConcurrentHashMap<String, HttpDevice>(); //Key=SID, Value=HttpDevice

    public HttpServer(InetSocketAddress inetSocketAddress) {
        super(inetSocketAddress);
        Log.i("HTTP-to-CoAP Proxy is enabled.");

        Thread sessionTimeoutCheckThread
        = new Thread(new SessionTimeoutCheck());
        sessionTimeoutCheckThread.setDaemon(true);
        sessionTimeoutCheckThread.start();
    }

    protected class SessionTimeoutCheck implements Runnable {

        // Check the session registry(httpDeviceMap) everyday,
        // and if the session timeout is passed,
        // then remove the session.
        public void run() {

            while (true) {
                Log.v("HTTP Session registry check is started");

                Enumeration<String> sessionIdList = httpDeviceMap.keys();

                while (sessionIdList.hasMoreElements()) {
                    String sessionId = sessionIdList.nextElement();
                    HttpDevice httpDevice = httpDeviceMap.get(sessionId);
                    if (httpDevice.getSessionExpiredTime() <= System
                            .currentTimeMillis()) {
                        httpDeviceMap.remove(sessionId);
                    }
                }

                Log.v("HTTP Session registry check is ended");

                try { // Sleep for 1 day
                    Thread.sleep(1000L * 3600L * 24L);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    @Override
    protected ChannelHandler[] onQueryDefaultHandler() {

        /*
         * Channel handlers added to HttpServer:
         * 1. SslContext: for TLS connection
         * 2. HttpRequestDecoder: netty-http-codec
         * 3. HttpResponseEncoder: netty-http-codec
         * 4. HttpLogHandler: for Http log
         * 5. HCProxyHandler: to translate HTTP to CoAP
         * 6. HttpAuthHandler: for Authorization check
         * 7. NonPersistentPacketReceiver:
         *    to hand over a request msg to a proper resource (server)
         * Note: Resource-manager will broker the received request
         *       to correspoding resource according to url path.
         */
        return new ChannelHandler[] { new HttpRequestDecoder(),
                new HttpResponseEncoder(), new HttpLogHandler(),
                new HCProxyHandler() };
    }
}
