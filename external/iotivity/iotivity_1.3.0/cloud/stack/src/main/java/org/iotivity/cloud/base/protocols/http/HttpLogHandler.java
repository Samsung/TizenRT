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

import org.iotivity.cloud.util.Log;

import io.netty.channel.ChannelDuplexHandler;
import io.netty.channel.ChannelHandler.Sharable;
import io.netty.handler.codec.http.HttpContent;
import io.netty.handler.codec.http.HttpObject;
import io.netty.handler.codec.http.HttpRequest;
import io.netty.handler.codec.http.HttpResponse;
import io.netty.handler.codec.http.HttpUtil;
import io.netty.handler.codec.http.LastHttpContent;
import io.netty.util.CharsetUtil;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelPromise;

/**
 * This class provides a set of APIs to print out logs
 * for HTTP request and response.
 */
@Sharable
public class HttpLogHandler extends ChannelDuplexHandler {

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        Log.v(ctx.channel().id().asLongText().substring(26)
                + " HTTP Connected, Address: "
                + ctx.channel().remoteAddress().toString());

        ctx.fireChannelActive();
    }

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        Log.v(ctx.channel().id().asLongText().substring(26)
                + " HTTP Disconnected, Address: "
                + ctx.channel().remoteAddress().toString());

        ctx.fireChannelInactive();
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg)
            throws Exception {

        if (!(msg instanceof HttpObject)) {
            Log.v("Non-HTTP message has been received to the HC proxy:\n"
                    + msg.toString());
        }

        if (msg instanceof HttpRequest) {

            StringBuilder contentStrBuilder = new StringBuilder();
            ctx.channel().attr(HCProxyProcessor.ctxStrContent)
                    .set(contentStrBuilder);

            HttpRequest httpRequest = (HttpRequest) msg;

            Log.v(httpRequest.toString());

            if (HttpUtil.isTransferEncodingChunked(httpRequest)) {
                Log.v("BEGINNING OF HTTP CHUNKED CONTENT");
            } else {
                Log.v("BEGINNING OF HTTP CONTENT");
            }
        }

        if (msg instanceof HttpContent) {

            HttpContent content = (HttpContent) msg;

            StringBuilder contentStrBuilder = ctx.channel()
                    .attr(HCProxyProcessor.ctxStrContent).get();
            contentStrBuilder
                    .append(content.content().toString(CharsetUtil.UTF_8));

            if (content instanceof LastHttpContent) {

                Log.v(contentStrBuilder.toString());

                Log.v("END OF HTTP CONTENT");

                contentStrBuilder.setLength(0);
            }
        }

        ctx.fireChannelRead(msg);
    }

    @Override
    public void write(ChannelHandlerContext ctx, Object msg,
            ChannelPromise promise) {

        if (!(msg instanceof HttpObject)) {
            Log.v("Non-HTTP message has been sent from the HC proxy:\n"
                    + msg.toString());
        }

        if (msg instanceof HttpResponse) {

            StringBuilder contentStrBuilder = ctx.channel()
                    .attr(HCProxyProcessor.ctxStrContent).get();

            HttpResponse httpResponse = (HttpResponse) msg;

            if (contentStrBuilder.length() > 0) {
                Log.v(httpResponse.toString() + "\n\n"
                        + contentStrBuilder.toString());
            } else {
                Log.v(httpResponse.toString());
            }
        }

        ctx.writeAndFlush(msg);
    }
}
