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
// package org.iotivity.cloud.base;
//
// import java.net.URI;
// import java.net.URISyntaxException;
//
// import javax.net.ssl.SSLException;
//
// import io.netty.bootstrap.Bootstrap;
// import io.netty.channel.Channel;
// import io.netty.channel.ChannelHandlerContext;
// import io.netty.channel.ChannelInitializer;
// import io.netty.channel.ChannelPipeline;
// import io.netty.channel.EventLoopGroup;
// import io.netty.channel.SimpleChannelInboundHandler;
// import io.netty.channel.nio.NioEventLoopGroup;
// import io.netty.channel.socket.SocketChannel;
// import io.netty.channel.socket.nio.NioSocketChannel;
// import io.netty.handler.codec.http.ClientCookieEncoder;
// import io.netty.handler.codec.http.DefaultCookie;
// import io.netty.handler.codec.http.DefaultFullHttpRequest;
// import io.netty.handler.codec.http.HttpClientCodec;
// import io.netty.handler.codec.http.HttpContent;
// import io.netty.handler.codec.http.HttpContentDecompressor;
// import io.netty.handler.codec.http.HttpHeaders;
// import io.netty.handler.codec.http.HttpMethod;
// import io.netty.handler.codec.http.HttpObject;
// import io.netty.handler.codec.http.HttpRequest;
// import io.netty.handler.codec.http.HttpResponse;
// import io.netty.handler.codec.http.HttpVersion;
// import io.netty.handler.codec.http.LastHttpContent;
// import io.netty.handler.ssl.SslContext;
// import io.netty.handler.ssl.SslContextBuilder;
// import io.netty.handler.ssl.util.InsecureTrustManagerFactory;
// import io.netty.util.CharsetUtil;
//
// public class HttpClient {
//
// private static class HttpClientInitializer
// extends ChannelInitializer<SocketChannel> {
//
// public static class HttpSnoopClientHandler
// extends SimpleChannelInboundHandler<HttpObject> {
//
// @Override
// public void channelRead0(ChannelHandlerContext ctx,
// HttpObject msg) {
// if (msg instanceof HttpResponse) {
// HttpResponse response = (HttpResponse) msg;
//
// System.err.println("STATUS: " + response.getStatus());
// System.err.println(
// "VERSION: " + response.getProtocolVersion());
// System.err.println();
//
// if (!response.headers().isEmpty()) {
// for (String name : response.headers().names()) {
// for (String value : response.headers()
// .getAll(name)) {
// System.err.println(
// "HEADER: " + name + " = " + value);
// }
// }
// System.err.println();
// }
//
// if (HttpHeaders.isTransferEncodingChunked(response)) {
// System.err.println("CHUNKED CONTENT {");
// } else {
// System.err.println("CONTENT {");
// }
// }
// if (msg instanceof HttpContent) {
// HttpContent content = (HttpContent) msg;
//
// System.err.print(
// content.content().toString(CharsetUtil.UTF_8));
// System.err.flush();
//
// if (content instanceof LastHttpContent) {
// System.err.println("} END OF CONTENT");
// ctx.close();
// }
// }
// }
//
// @Override
// public void exceptionCaught(ChannelHandlerContext ctx,
// Throwable cause) {
// cause.printStackTrace();
// ctx.close();
// }
// }
//
// private final SslContext sslCtx;
//
// public HttpClientInitializer(SslContext sslCtx) {
// this.sslCtx = sslCtx;
// }
//
// @Override
// public void initChannel(SocketChannel ch) {
// ChannelPipeline p = ch.pipeline();
//
// // Enable HTTPS if necessary.
// if (sslCtx != null) {
// p.addLast(sslCtx.newHandler(ch.alloc()));
// }
//
// p.addLast(new HttpClientCodec());
//
// // Remove the following line if you don't want automatic content
// // decompression.
// p.addLast(new HttpContentDecompressor());
//
// // Uncomment the following line if you don't want to handle
// // HttpContents.
// // p.addLast(new HttpObjectAggregator(1048576));
//
// p.addLast(new HttpSnoopClientHandler());
// }
// }
//
// public void connect(String strUrl)
// throws URISyntaxException, InterruptedException, SSLException {
// URI uri = new URI(strUrl);
//
// String scheme = uri.getScheme() == null ? "http" : uri.getScheme();
// String host = uri.getHost() == null ? "127.0.0.1" : uri.getHost();
//
// int port = uri.getPort();
//
// if (port == -1) {
// if ("http".equalsIgnoreCase(scheme)) {
// port = 80;
// } else if ("https".equalsIgnoreCase(scheme)) {
// port = 443;
// }
// }
//
// if (!"http".equalsIgnoreCase(scheme)
// && !"https".equalsIgnoreCase(scheme)) {
// return;
// }
//
// final boolean ssl = "https".equalsIgnoreCase(scheme);
// final SslContext sslCtx;
//
// if (ssl) {
// sslCtx = SslContextBuilder.forClient()
// .trustManager(InsecureTrustManagerFactory.INSTANCE).build();
// } else {
// sslCtx = null;
// }
//
// EventLoopGroup group = new NioEventLoopGroup();
//
// try {
// Bootstrap b = new Bootstrap();
// b.group(group);
// b.channel(NioSocketChannel.class);
// b.handler(new HttpClientInitializer(sslCtx));
//
// Channel ch = b.connect(host, port).sync().channel();
//
// HttpRequest request = new DefaultFullHttpRequest(
// HttpVersion.HTTP_1_1, HttpMethod.GET, uri.getRawPath());
// request.headers().set(HttpHeaders.Names.HOST, host);
// request.headers().set(HttpHeaders.Names.CONNECTION,
// HttpHeaders.Values.CLOSE);
// request.headers().set(HttpHeaders.Names.ACCEPT_ENCODING,
// HttpHeaders.Values.GZIP);
//
// request.headers().set(HttpHeaders.Names.COOKIE,
// ClientCookieEncoder.encode(
// new DefaultCookie("my-cookie", "foo"),
// new DefaultCookie("another-cookie", "bar")));
//
// ch.writeAndFlush(request);
//
// ch.closeFuture().sync();
// } finally {
// group.shutdownGracefully();
// }
// }
//
// }
