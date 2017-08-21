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
package org.iotivity.cloud.base.protocols.http;

import java.util.concurrent.ConcurrentHashMap;

import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.Message;
import org.iotivity.cloud.util.Log;

import io.netty.channel.ChannelDuplexHandler;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelPromise;
import io.netty.handler.codec.http.HttpContent;
import io.netty.handler.codec.http.HttpHeaderNames;
import io.netty.handler.codec.http.HttpHeaders;
import io.netty.handler.codec.http.HttpRequest;
import io.netty.handler.codec.http.HttpResponse;
import io.netty.handler.codec.http.LastHttpContent;
import io.netty.util.CharsetUtil;

/**
 * This channel handler intercepts HTTP request and CoAP response
 * and translate them into CoAP request and HTTP response accordingly.
 */
public class HCProxyHandler extends ChannelDuplexHandler {

    private static ConcurrentHashMap<String, HCProxyProcessor> hcProxyProcessorMap
    = new ConcurrentHashMap<String, HCProxyProcessor>();

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg)
            throws Exception {

        // Wait translating the request until the end of content is received
        if (msg instanceof HttpRequest) {

            StringBuilder contentStrBuilder = new StringBuilder();
            ctx.channel().attr(HCProxyProcessor.ctxStrContent)
                    .set(contentStrBuilder);

            HttpRequest httpRequest = (HttpRequest) msg;

            HCProxyProcessor hcProxyProcessor = new HCProxyProcessor(ctx);

            HCProxyHandler.hcProxyProcessorMap
                    .put(ctx.channel().id().asLongText(), hcProxyProcessor);

            // Set the values for required attributes in hcProxyProcessor
            hcProxyProcessor.setHttpMethod(httpRequest.method());
            hcProxyProcessor.setHostingHttpUri(httpRequest.uri());
            HttpHeaders httpHeaders = httpRequest.headers();
            hcProxyProcessor.setContentType(
                    httpHeaders.get(HttpHeaderNames.CONTENT_TYPE));

            // Check Session-ID for the already signed-in transaction
            hcProxyProcessor.checkSessionId(
                    httpHeaders.get(HttpHeaderNames.COOKIE));
        }

        // Do not hand over the message to next handler until the end of content
        if (msg instanceof HttpContent) {

            HttpContent content = (HttpContent) msg;

            StringBuilder contentStrBuilder = ctx.channel()
                    .attr(HCProxyProcessor.ctxStrContent).get();
            contentStrBuilder
                    .append(content.content().toString(CharsetUtil.UTF_8));

            if (content instanceof LastHttpContent) {

                HCProxyProcessor hcProxyProcessor = HCProxyHandler.hcProxyProcessorMap
                        .get(ctx.channel().id().asLongText());

                if (hcProxyProcessor != null
                        && hcProxyProcessor.getContentType() != null) {
                    hcProxyProcessor.setContent(contentStrBuilder.toString());
                }

                contentStrBuilder.setLength(0);

                // Check HTTP request whether there is an error or not
                String errorStatusCode = hcProxyProcessor.checkHttpRequest();

                if (errorStatusCode != null) {

                    Log.v("HTTP Error: " + errorStatusCode);

                    HttpResponse httpResponse = hcProxyProcessor
                            .getErrorResponse(errorStatusCode);

                    ctx.writeAndFlush(httpResponse);
                }

                // Create a message request from HTTP request
                Message message = null;

                if (hcProxyProcessor != null) {
                    message = hcProxyProcessor.getRequestMessage();
                }

                if (message != null) {
                    ctx.fireChannelRead(message);
                } else {
                    errorStatusCode = "500 Internal Server Error: "
                            + "HTTP-Method does not recognized.";

                    Log.v("HTTP Error: " + errorStatusCode);

                    HttpResponse httpResponse = hcProxyProcessor
                            .getErrorResponse(errorStatusCode);

                    ctx.writeAndFlush(httpResponse);
                }
            }
        }
    }

    @Override
    public void write(ChannelHandlerContext ctx, Object msg,
            ChannelPromise promise) {

        // Create HTTP response from the response
        HttpResponse httpResponse = null;

        if (msg instanceof IResponse) {

            IResponse response = (IResponse) msg;

            HCProxyProcessor hcProxyProcessor = HCProxyHandler.hcProxyProcessorMap
                    .get(ctx.channel().id().asLongText());

            if (hcProxyProcessor != null) {

                /*
                 * If HTTP request was sign-in and HTTP response is 200 OK,
                 * then set HTTP Cookie in the response for the session.
                 */
                httpResponse = hcProxyProcessor.getHttpResponse(response);
            }

            if (httpResponse == null) {
                String errorStatusCode = "500 Internal Server Error: "
                        + "HTTP response could not be generated.";

                Log.v("HTTP Error: " + errorStatusCode);

                httpResponse = hcProxyProcessor
                        .getErrorResponse(errorStatusCode);
            }

            msg = httpResponse;
        }

        ChannelFuture future = ctx.writeAndFlush(msg);

        // Close the http connection after sending response
        future.addListener(new ChannelFutureListener() {

            @Override
            public void operationComplete(ChannelFuture future)
                    throws Exception {

                if (future.isSuccess()) {
                    future.channel().close();
                } else {
                    Log.v(ctx.channel().id().asLongText().substring(26)
                            + " HTTP Disconnected (Unexpectedly), Address: "
                            + ctx.channel().remoteAddress().toString());
                }
            }
        });
    }
}
