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

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.UUID;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.regex.Pattern;

import org.iotivity.cloud.base.ServerSystem;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.device.HttpDevice;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.Message;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.server.HttpServer;
import org.iotivity.cloud.util.Cbor;
import org.iotivity.cloud.util.Log;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.http.DefaultFullHttpResponse;
import io.netty.handler.codec.http.DefaultHttpResponse;
import io.netty.handler.codec.http.HttpHeaderNames;
import io.netty.handler.codec.http.HttpHeaderValues;
import io.netty.handler.codec.http.HttpMethod;
import io.netty.handler.codec.http.HttpResponse;
import io.netty.handler.codec.http.HttpResponseStatus;
import io.netty.handler.codec.http.HttpVersion;
import io.netty.util.AttributeKey;

/**
 * This class actually translates HTTP request to CoAP request
 * and CoAP response to HTTP response.
 * The translation complyes Default Mapping (scheme omission) Technique
 * described in https://tools.ietf.org/html/draft-ietf-core-http-mapping-13.
 */
public class HCProxyProcessor {

    public final static String COAP_FORMAT_TYPE = "application/coap-payload;cf=";
    public final static String APPLICATION_JSON = "application/json";
    public final static String APPLICATION_CBOR = "application/cbor";
    public final static String DISCOVER_URI     = "/oic/res";

    public final static String HC_PROXY_PATH    = "/hc/";

    public final static String SIGNIN_URI       = "/oic/account/session";
    public final static String COOKIE_SESSION   = "SID";
    public final static String COOKIE_MAX_AGE   = "Max-Age";

    public final static long SESSION_TIMEOUT    = 3600L * 24L; //1 Day

    public static AttributeKey<StringBuilder> ctxStrContent = AttributeKey
            .newInstance("strContent");

    public static AttributeKey<String> ctxSessionId = AttributeKey
            .newInstance("sessionId");

    // To encode/decode JSON with one root element
    private Cbor<HashMap<String, Object>> mStackCborMap = new Cbor<>();

    // To encode/decode JSON with array of root elements: Discover response
    private Cbor<ArrayList<Object>> mStackCborArray = new Cbor<>();

    private static int mRandomSeed = (int)(System.currentTimeMillis() % 10000);
    private static AtomicInteger mTokenCounter = new AtomicInteger(mRandomSeed);

    private ChannelHandlerContext mCtx = null;

    /**
     * This function is the constructor of this class.
     * 
     * @param ctx The message can be sent/received through this channel.
     */
    public HCProxyProcessor(ChannelHandlerContext ctx) {
        this.mCtx = ctx;
    }

    /**
     * This function returns a channel of the HTTP connection.
     * 
     * @return ctx The message can be sent/received through this channel.
     */
    public ChannelHandlerContext getCtx() {
        return this.mCtx;
    }

    /*
     * HC Proxy Path: A path addressing the HC proxy function, normally
     * following HC Proxy host address
     */
    private String     mHCProxyPath     = HC_PROXY_PATH;

    /*
     * Target CoAP Path: A path addressing the cloud-interface function,
     * normally CoAP host address is CI server address
     */
    private String     mTargetCoapPath  = null;
    private String     mTargetCoapQuery = null;

    /*
     * Hosting HTTP URI: A default map-able URI refers to a HC proxy, whereas
     * path (and query) component(s) embed the information used by a HC proxy to
     * extract the Target CoAP URI
     */
    private String     mHostingHttpUri  = null;

    /*
     * HTTP Request Content
     */
    private String     mContentType     = null;
    private String     mContent         = null;

    /*
     * CoAP Request Content
     */
    private String     mContentFormat   = null;
    private byte[]     mCborContent     = null;

    private HttpMethod mHttpMethod      = null;

    private String     mSessionId       = null;

    /**
     * This function returns Session-ID of the request.
     * 
     * @return sessionId
     */
    public String getSessionId() {
        return this.mSessionId;
    }

    /**
     * This function returns HTTP Session ID if exists,
     * and put session id value in the channel attribute.
     * 
     * @return sessionId, null if there is no session id
     */
    public String checkSessionId(String cookie) {
        if (cookie == null) { return null; }
        // e.g. SID=31d4d96e407aad42
        String[] sidKeyValue = cookie.split(Pattern.quote("="), 2);
        if (sidKeyValue.length > 1) {
            mSessionId = sidKeyValue[1];
        }
        if (mSessionId != null) {
            mCtx.channel().attr(HCProxyProcessor.ctxSessionId).set(mSessionId);
        }
        return mSessionId;
    }

    /**
     * This function returns HC Proxy Path string.
     * 
     * @return hcProxyPath A path addressing the HC proxy function, normally
     *         following HC Proxy host address
     */
    public String getHcProxyPath() {
        return this.mHCProxyPath;
    }

    /**
     * This function returns Target CoAP Path string.
     * 
     * @return A path addressing the cloud-interface function, normally CoAP
     *         host address is CI server address
     */
    public String getTargetCoapPath() {
        return this.mTargetCoapPath;
    }

    /**
     * This function returns Hosting HTTP URI string.
     * Hosting HTTP URI complyes Default Mapping (scheme omission) Technique
     * described in https://tools.ietf.org/html/draft-ietf-core-http-mapping-13.
     * 
     * @return hostingHttpUri A default map-able URI refers to a HC proxy,
     *         whereas path (and query) component(s) embed the information used
     *         by a HC proxy to extract the Target CoAP URI
     */
    public String getHostingHttpUri() {
        return this.mHostingHttpUri;
    }

    /**
     * This function sets Hosting HTTP URI and Target CoAP Path/Query strings.
     * Hosting HTTP URI complyes Default Mapping (scheme omission) Technique
     * described in https://tools.ietf.org/html/draft-ietf-core-http-mapping-13.
     * 
     * @param hostingHttpUri
     */
    public void setHostingHttpUri(String hostingHttpUri) {
        this.mHostingHttpUri = hostingHttpUri;
        // hostingHttpUri = /hc/<ip>:<port>/<path>[?]<query>
        String[] tempUriArray = hostingHttpUri
                .split(Pattern.quote(mHCProxyPath));
        if (tempUriArray.length > 1) {
            // tempUriArray[1] = <ip>:<port>/<path>[?]<query>
            String[] tempPathArray = tempUriArray[1]
                    .split(Pattern.quote("/"), 2);
            if (tempPathArray.length > 1) {
                // tempPathArray[1] = <path>[?]<query>
                String[] tempPathQuery = tempPathArray[1]
                        .split(Pattern.quote("?"), 2);
                this.mTargetCoapPath = "/" + tempPathQuery[0];
                if (tempPathQuery.length > 1) {
                    this.mTargetCoapQuery = tempPathQuery[1];
                }
            }
        }
    }

    /**
     * This function returns Content-Type of HTTP request.
     * 
     * @return contentType
     */
    public String getContentType() {
        return this.mContentType;
    }

    /**
     * This function sets HTTP Content-Type and CoAP Content-Format.
     * 
     * @param contentType
     */
    public void setContentType(String contentType) {
        this.mContentType = contentType;
        if (contentType != null) {
            if (contentType.equalsIgnoreCase(APPLICATION_JSON)) {
                this.mContentFormat = APPLICATION_CBOR;
            }
        }
    }

    /**
     * This function returns content of HTTP request.
     * 
     * @return content HTTP request
     */
    public String getContent() {
        return this.mContent;
    }

    /**
     * This function sets HTTP Content and CoAP Cbor Content.
     * 
     * @param content HTTP request
     */
    public void setContent(String content) {
        this.mContent = content;
        if (mContentType != null && !content.trim().equals("")) {
            if (mContentType.equalsIgnoreCase(APPLICATION_JSON)) {
                byte[] cborData = null;
                try { // Cbor: JSON string to Map
                    Log.v("Cbor encoding using HashMap.class");
                    ObjectMapper cborMapper = new ObjectMapper();
                    HashMap<String, Object> cborMap
                    = cborMapper.readValue(content,
                            new TypeReference<HashMap<String, Object>>() {});
                    cborData = mStackCborMap.encodingPayloadToCbor(cborMap);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                this.mCborContent = cborData;
            }
        }
    }

    /**
     * This function returns HTTP request method.
     * 
     * @return httpMethod
     */
    public HttpMethod getHttpMethod() {
        return this.mHttpMethod;
    }

    /**
     * This function sets HTTP request method.
     * 
     * @param httpMethod
     */
    public void setHttpMethod(HttpMethod httpMethod) {
        this.mHttpMethod = httpMethod;
    }

    /**
     * This function returns a message created by the request,
     * which implements IRequest(used in the cloud)
     * translated from the HTTP request.
     * 
     * @return requestMessage
     */
    public Message getRequestMessage() {

        Message requestMessage = null;

        String coapUriPath = mTargetCoapPath;
        String coapUriQuery = mTargetCoapQuery;
        ContentFormat coapContentFormat = null;
        if (mContentFormat != null) {
            if (mContentFormat.equalsIgnoreCase(APPLICATION_CBOR)) {
                coapContentFormat = ContentFormat.APPLICATION_CBOR;
            }
        }
        byte[] coapPayload = mCborContent;

        if (mHttpMethod == HttpMethod.POST) {

            CoapRequest coapRequest = new CoapRequest(RequestMethod.POST);
            coapRequest.setToken(createToken());
            coapRequest.setUriPath(coapUriPath);
            if (coapUriQuery != null) {
                coapRequest.setUriQuery(coapUriQuery);
            }
            if (coapPayload != null) {
                coapRequest.setContentFormat(coapContentFormat);
                coapRequest.setPayload(coapPayload);
            }
            requestMessage = coapRequest;

        } else if (mHttpMethod == HttpMethod.PUT) {

            CoapRequest coapRequest = new CoapRequest(RequestMethod.PUT);
            coapRequest.setToken(createToken());
            coapRequest.setUriPath(coapUriPath);
            if (coapUriQuery != null) {
                coapRequest.setUriQuery(coapUriQuery);
            }
            if (coapPayload != null) {
                coapRequest.setContentFormat(coapContentFormat);
                coapRequest.setPayload(coapPayload);
            }
            requestMessage = coapRequest;

        } else if (mHttpMethod == HttpMethod.GET) {

            CoapRequest coapRequest = new CoapRequest(RequestMethod.GET);
            coapRequest.setToken(createToken());
            coapRequest.setUriPath(coapUriPath);
            if (coapUriQuery != null) {
                coapRequest.setUriQuery(coapUriQuery);
            }
            requestMessage = coapRequest;

        } else if (mHttpMethod == HttpMethod.DELETE) {

            CoapRequest coapRequest = new CoapRequest(RequestMethod.DELETE);
            coapRequest.setToken(createToken());
            coapRequest.setUriPath(coapUriPath);
            if (coapUriQuery != null) {
                coapRequest.setUriQuery(coapUriQuery);
            }
            requestMessage = coapRequest;
        }

        return requestMessage;
    }

    private byte[] createToken() {
        int token = mTokenCounter.incrementAndGet();
        return new byte[] {
                (byte) (token >>> 24),
                (byte) (token >>> 16),
                (byte) (token >>> 8),
                (byte) token
        };
    }

    /**
     * This function returns an error information
     * (HTTP status code and reason phrase) if there is an error.
     * 
     * @return errorStatusCode
     */
    public String checkHttpRequest() {

        String errorStatusCode = null;

        if (mHttpMethod == HttpMethod.POST) {
            if (mContentFormat == null) {
                errorStatusCode = "500 Internal Server Error: "
                        + "Content-Type does not recognized.";
            } else if (mCborContent == null) {
                errorStatusCode = "400 Bad Request: Payload is empty.";
            }
        }

        if (mHostingHttpUri == null || mTargetCoapPath == null) {
            errorStatusCode = "400 Bad Request: "
                    + "Please use proper URI Path "
                    + HC_PROXY_PATH + "<Host, e.g. ip:port>/<Path>.";
        }

        return errorStatusCode;
    }

    /**
     * This function returns a created HTTP error response
     * created by an error information.
     * 
     * @param errorStatusCode HTTP status code and reason phrase
     * @return httpResponse
     */
    public HttpResponse getErrorResponse(String errorStatusCode) {

        String[] responseStatus = errorStatusCode.split(Pattern.quote(" "), 2);
        int httpCode = Integer.valueOf(responseStatus[0]);
        String reasonPhrase = responseStatus[1];

        HttpVersion httpVersion = HttpVersion.HTTP_1_1;
        HttpResponseStatus httpStatus = new HttpResponseStatus(httpCode,
                reasonPhrase);
        HttpResponse httpResponse = new DefaultHttpResponse(httpVersion,
                httpStatus);

        // Default: Non-persistent connection
        httpResponse.headers().set(HttpHeaderNames.CONNECTION,
                HttpHeaderValues.CLOSE);

        // Some clients requires content-length=0
        // to decide a proper size of one response message.
        httpResponse.headers().set(HttpHeaderNames.CONTENT_TYPE,
                HttpHeaderValues.NONE);
        httpResponse.headers().set(HttpHeaderNames.CONTENT_LENGTH,
                HttpHeaderValues.ZERO);

        return httpResponse;
    }

    /**
     * This function returns a created HTTP response
     * translated from the CoAP response.
     * 
     * @param response
     * @return httpResponse
     */
    public HttpResponse getHttpResponse(IResponse response) {

        HttpResponse httpResponse = null;

        ResponseStatus coapResponseStatus = response.getStatus();
        ContentFormat coapContentFormat = response.getContentFormat();
        byte[] coapPayload = response.getPayload();
        boolean isPayload = false;

        HttpVersion httpVersion = HttpVersion.HTTP_1_1;

        if (coapPayload != null && coapPayload.length > 0) {

            isPayload = true;

            HttpResponseStatus httpStatus = getHttpResponseStatus(
                    coapResponseStatus, isPayload);

            String httpContentType = null;

            ByteBuf httpContent = Unpooled.EMPTY_BUFFER;

            if (coapContentFormat == ContentFormat.APPLICATION_CBOR) {

                // Content-Type: application/json
                httpContentType = APPLICATION_JSON;

                String payloadString = null;
                try { // Cbor: Map to JSON string

                    if (getTargetCoapPath().contains(DISCOVER_URI)) {
                        Log.v("Cbor decoding using ArrayList.class");
                        ArrayList<Object> cborMap = mStackCborArray
                                .parsePayloadFromCbor(coapPayload, ArrayList.class);
                        ObjectMapper cborMapper = new ObjectMapper();
                        payloadString = cborMapper.writerWithDefaultPrettyPrinter()
                                .writeValueAsString(cborMap);
                    } else {
                        Log.v("Cbor decoding using HashMap.class");
                        HashMap<String, Object> cborMap = mStackCborMap
                                .parsePayloadFromCbor(coapPayload, HashMap.class);
                        ObjectMapper cborMapper = new ObjectMapper();
                        payloadString = cborMapper.writerWithDefaultPrettyPrinter()
                                .writeValueAsString(cborMap);
                    }

                } catch (JsonProcessingException e) {
                    e.printStackTrace();
                }

                if (payloadString == null
                        || payloadString.trim().equalsIgnoreCase("null")) {
                    payloadString = "";
                }

                byte[] payloadBytes = payloadString.getBytes();

                httpContent = Unpooled.wrappedBuffer(payloadBytes);

            } else {

                if (response instanceof CoapResponse) {
                    // Content-Type:
                    // application/coap-payload;cf=<Content-Format value>
                    httpContentType = COAP_FORMAT_TYPE
                            + ((CoapResponse) response)
                                    .getContentFormatValue();
                } else {
                    httpContentType = coapContentFormat.toString();
                }

                httpContent = Unpooled.wrappedBuffer(coapPayload);
            }

            httpResponse = new DefaultFullHttpResponse(httpVersion, httpStatus,
                    httpContent);

            // Default: Non-persistent connection
            httpResponse.headers().set(HttpHeaderNames.CONNECTION,
                    HttpHeaderValues.CLOSE);

            httpResponse.headers().set(HttpHeaderNames.CONTENT_TYPE,
                    httpContentType);
            httpResponse.headers().set(HttpHeaderNames.CONTENT_LENGTH,
                    httpContent.readableBytes());

            byte[] bytes = new byte[httpContent.readableBytes()];
            httpContent.getBytes(httpContent.readerIndex(), bytes);

            StringBuilder contentStrBuilder = new StringBuilder(
                    new String(bytes));
            mCtx.channel().attr(HCProxyProcessor.ctxStrContent)
                    .set(contentStrBuilder);

        } else {

            isPayload = false;

            HttpResponseStatus httpStatus = getHttpResponseStatus(
                    coapResponseStatus, isPayload);

            httpResponse = new DefaultFullHttpResponse(httpVersion, httpStatus);

            // Default: Non-persistent connection
            httpResponse.headers().set(HttpHeaderNames.CONNECTION,
                    HttpHeaderValues.CLOSE);

            // Some clients requires content-length=0
            // to decide a proper size of one response message.
            httpResponse.headers().set(HttpHeaderNames.CONTENT_TYPE,
                    HttpHeaderValues.NONE);
            httpResponse.headers().set(HttpHeaderNames.CONTENT_LENGTH,
                    HttpHeaderValues.ZERO);
        }

        /*
         * If HTTP request was sign-in and HTTP response is 200 OK,
         * then set HTTP Cookie in the response for the session.
         */
        if (mTargetCoapPath != null && mTargetCoapPath.contains(SIGNIN_URI)
                && httpResponse.status() == HttpResponseStatus.OK) {
            long timeoutSeconds = SESSION_TIMEOUT;
            String createdSid = setSessionId(httpResponse, timeoutSeconds);

            // Set SessionExpiredTime in HttpDevice
            // and put it into httpDeviceMap using the session id as a key
            Device device = mCtx.channel().attr(ServerSystem.keyDevice).get();
            if (device != null) {
                ((HttpDevice) device).setSessionExpiredTime(timeoutSeconds);
                HttpServer.httpDeviceMap.put(createdSid, (HttpDevice) device);
                mCtx.channel().attr(ServerSystem.keyDevice).set(device);
            }
        }

        // Put HttpDevice into httpDeviceMap using the session id as a key
        HttpDevice httpDevice = (HttpDevice) mCtx.channel()
                .attr(ServerSystem.keyDevice).get();
        if (httpDevice != null && mSessionId != null) {
            HttpServer.httpDeviceMap.put(mSessionId, httpDevice);
        }

        return httpResponse;
    }

    private HttpResponseStatus getHttpResponseStatus(
            ResponseStatus coapResponseStatus, boolean isPayload) {

        HttpResponseStatus httpStatusCode = HttpResponseStatus.NOT_IMPLEMENTED;

        if (coapResponseStatus == ResponseStatus.CREATED) {

            httpStatusCode = HttpResponseStatus.CREATED;

        } else if (coapResponseStatus == ResponseStatus.DELETED
                || coapResponseStatus == ResponseStatus.CHANGED
                || coapResponseStatus == ResponseStatus.CONTENT) {

            if (isPayload) {
                httpStatusCode = HttpResponseStatus.OK;
            } else {
                httpStatusCode = HttpResponseStatus.NO_CONTENT;
            }

        } else if (coapResponseStatus == ResponseStatus.VALID) {

            if (isPayload) {
                httpStatusCode = HttpResponseStatus.OK;
            } else {
                httpStatusCode = HttpResponseStatus.NOT_MODIFIED;
            }

        } else if (coapResponseStatus == ResponseStatus.UNAUTHORIZED
                || coapResponseStatus == ResponseStatus.FORBIDDEN) {

            httpStatusCode = HttpResponseStatus.FORBIDDEN;

        } else if (coapResponseStatus == ResponseStatus.BAD_REQUEST
                || coapResponseStatus == ResponseStatus.BAD_OPTION) {

            httpStatusCode = HttpResponseStatus.BAD_REQUEST;

        } else if (coapResponseStatus == ResponseStatus.METHOD_NOT_ALLOWED) {

            // The HC Proxy should return a HTTP reason-phrase
            // in the HTTP status line that starts with the string
            // "CoAP server returned 4.05"
            // in order to facilitate troubleshooting.
            httpStatusCode = new HttpResponseStatus(400,
                    "CoAP server returned 4.05");

        } else if (coapResponseStatus == ResponseStatus.NOT_ACCEPTABLE) {

            httpStatusCode = HttpResponseStatus.NOT_ACCEPTABLE;

        } else if (coapResponseStatus == ResponseStatus.PRECONDITION_FAILED) {

            httpStatusCode = HttpResponseStatus.PRECONDITION_FAILED;

        } else if (coapResponseStatus == ResponseStatus.UNSUPPORTED_CONTENT_FORMAT) {

            httpStatusCode = HttpResponseStatus.UNSUPPORTED_MEDIA_TYPE;

        } else if (coapResponseStatus == ResponseStatus.BAD_GATEWAY
                || coapResponseStatus == ResponseStatus.PROXY_NOT_SUPPORTED) {

            httpStatusCode = HttpResponseStatus.BAD_GATEWAY;

        } else if (coapResponseStatus == ResponseStatus.SERVICE_UNAVAILABLE) {

            httpStatusCode = HttpResponseStatus.SERVICE_UNAVAILABLE;

        } else if (coapResponseStatus == ResponseStatus.GATEWAY_TIMEOUT) {

            httpStatusCode = HttpResponseStatus.GATEWAY_TIMEOUT;

        } else if (coapResponseStatus == ResponseStatus.NOT_FOUND) {

            httpStatusCode = HttpResponseStatus.NOT_FOUND;

        } else if (coapResponseStatus == ResponseStatus.INTERNAL_SERVER_ERROR) {

            httpStatusCode = HttpResponseStatus.INTERNAL_SERVER_ERROR;
        }

        return httpStatusCode;
    }

    private String setSessionId(HttpResponse httpResponse, long timeoutSeconds) {

        String randomSid = UUID.randomUUID().toString().replaceAll("-", "");

        // e.g. SID=31d4d96e407aad42; Max-Age=86400
        StringBuilder sessionBuilder = new StringBuilder();
        sessionBuilder.append(COOKIE_SESSION);
        sessionBuilder.append("=");
        sessionBuilder.append(randomSid);
        sessionBuilder.append("; ");
        sessionBuilder.append(COOKIE_MAX_AGE);
        sessionBuilder.append("=");
        sessionBuilder.append(timeoutSeconds);

        httpResponse.headers().set(HttpHeaderNames.SET_COOKIE,
                sessionBuilder.toString());

        return randomSid;
    }
}
