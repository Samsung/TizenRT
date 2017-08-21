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
package org.iotivity.cloud.base.protocols.coap;

import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.iotivity.cloud.base.exception.ServerException.BadOptionException;
import org.iotivity.cloud.base.protocols.Message;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.Observe;
import org.iotivity.cloud.util.Bytes;

public abstract class CoapMessage extends Message {

    protected byte[]       mToken          = null;
    protected int          mObserve        = -1;

    // Option fields
    protected List<byte[]> if_match        = null;
    protected byte[]       uri_host        = null;
    protected List<byte[]> etag            = null;
    protected boolean      if_none_match   = false;
    protected byte[]       uri_port        = null;
    protected List<byte[]> location_path   = null;

    protected byte[]       max_age         = null;
    protected byte[]       accept          = null;
    protected List<byte[]> location_query  = null;
    protected byte[]       proxy_uri       = null;
    protected byte[]       proxy_scheme    = null;
    protected byte[]       size1           = null;
    protected byte[]       content_format  = null;

    // OCF spec Option fields
    protected byte[]       accept_version  = null;
    protected byte[]       content_version = null;

    public CoapMessage() {
    }

    public abstract int getCode();

    public void addOption(int optnum, byte[] value) {
        switch (optnum) {
            // IF_MATCH
            case 1:
                if (if_match == null) {
                    if_match = new ArrayList<>();
                }
                if_match.add(value);
                break;

            // URI_HOST
            case 3:
                uri_host = value;
                break;

            // ETAG
            case 4:
                if (etag == null) {
                    etag = new ArrayList<>();
                }
                etag.add(value);
                break;

            // IF_NONE_MATCH
            case 5:
                if_none_match = true;
                break;

            // URI_PORT
            case 7:
                uri_port = value;
                break;

            // LOCATION_PATH
            case 8:
                if (location_path == null) {
                    location_path = new ArrayList<>();
                }
                location_path.add(value);
                break;

            // URI_PATH
            case 11:
                if (uri_path == null) {
                    uri_path = new ArrayList<>();
                }
                uri_path.add(value);
                break;

            // CONTENT_FORMAT
            case 12:
                content_format = value;
                break;

            // MAX_AGE
            case 14:
                max_age = value;
                break;

            // URI_QUERY
            case 15:
                if (uri_query == null) {
                    uri_query = new ArrayList<>();
                }
                uri_query.add(value);
                break;

            // ACCEPT
            case 17:
                accept = value;
                break;

            // LOCATION_QUERY
            case 20:
                if (location_query == null) {
                    location_query = new ArrayList<>();
                }
                location_query.add(value);
                break;

            // PROXY_URI
            case 35:
                proxy_uri = value;
                break;

            // PROXY_SCHEME
            case 39:
                proxy_scheme = value;
                break;

            // SIZE1
            case 60:
                size1 = value;
                break;

            // OBSERVE
            case 6:
                mObserve = Bytes.bytesToInt(value);
                break;

            // ACCEPT_VERSION
            case 2049:
                accept_version = value;
                break;

            // CONTENT_VERSION
            case 2053:
                content_version = value;
                break;

            default: {
                if (optnum % 2 == 1) {
                    throw new BadOptionException(
                            "unrecognized critical option # " + optnum
                                    + " received");
                }
            }
        }
    }

    public List<byte[]> getOption(int optnum) {
        switch (optnum) {
            // IF_MATCH
            case 1:
                return if_match;

            // URI_HOST
            case 3:
                return uri_host != null ? Arrays.asList(uri_host) : null;

            // ETAG
            case 4:
                return etag;

            // IF_NONE_MATCH
            case 5:
                return if_none_match == true ? new ArrayList<byte[]>() : null;

            // URI_PORT
            case 7:
                return uri_port != null ? Arrays.asList(uri_port) : null;

            // LOCATION_PATH
            case 8:
                return location_path;

            // URI_PATH
            case 11:
                return uri_path;

            // CONTENT_FORMAT
            case 12:
                return content_format != null ? Arrays.asList(content_format)
                        : null;

            // MAX_AGE
            case 14:
                return max_age != null ? Arrays.asList(max_age) : null;

            // URI_QUERY
            case 15:
                return uri_query;

            // ACCEPT
            case 17:
                return accept != null ? Arrays.asList(accept) : null;

            // LOCATION_QUERY
            case 20:
                return location_query;

            // PROXY_URI
            case 35:
                return proxy_uri != null ? Arrays.asList(proxy_uri) : null;

            // PROXY_SCHEME
            case 39:
                return proxy_scheme != null ? Arrays.asList(proxy_scheme)
                        : null;

            // SIZE1
            case 60:
                return size1 != null ? Arrays.asList(size1) : null;

            // OBSERVE
            case 6:
                return mObserve != -1
                        ? Arrays.asList(Bytes.intToMax4Bytes(mObserve)) : null;

            // ACCEPT_VERSION
            case 2049:
                return accept_version != null ? Arrays.asList(accept_version)
                        : null;

            // CONTENT_VERSION
            case 2053:
                return content_version != null ? Arrays.asList(content_version)
                        : null;
        }

        return null;
    }

    public void setPayload(byte[] payload) {
        this.payload = payload;
    }

    public void setUriPath(String path) {
        if (uri_path == null) {
            uri_path = new ArrayList<>();
        } else {
            uri_path.clear();
        }

        if (path == null || path.length() == 0) {
            uri_path.clear();
            uri_path = null;
            return;
        }

        String[] pathSegments = path.split("/");
        for (String pathSegment : pathSegments) {
            if (pathSegment.length() == 0)
                continue;
            uri_path.add(pathSegment.getBytes(StandardCharsets.UTF_8));
        }
    }

    public void setUriQuery(String query) {
        if (uri_query == null) {
            uri_query = new ArrayList<>();
        } else {
            uri_query.clear();
        }

        if (query == null || query.length() == 0) {
            uri_query.clear();
            uri_query = null;
            return;
        }

        String[] querySegments = query.split(";");
        for (String querySegment : querySegments) {
            uri_query.add(querySegment.getBytes(StandardCharsets.UTF_8));
        }
    }

    public String getPayloadString() {
        if (payload == null)
            return "";
        return new String(payload, Charset.forName("UTF-8"));
    }

    public void setToken(byte[] token) {
        mToken = token;
    }

    public byte[] getToken() {
        return mToken;
    }

    public int getTokenLength() {
        return mToken == null ? 0 : mToken.length;
    }

    public String getTokenString() {
        StringBuffer strBuffer = new StringBuffer(mToken == null ? "null" : "");
        if (mToken != null)
            for (byte b : mToken)
                strBuffer.append(String.format("%02x", b & 0xff)); // hexadecimal(16)
        return strBuffer.toString();
    }

    public String getRequestId() {
        return getTokenString();
    }

    public void setSequenceNumber(int seqNumber) {
        mObserve = seqNumber;
    }

    public int getSequenceNumber() {
        return mObserve;
    }

    public int getNextSequenceNumber() {
        int ret = mObserve++;

        if (mObserve == 1 || mObserve == 16777216) {
            mObserve = 2;
        }

        return ret;
    }

    public void setObserve(Observe observe) {
        switch (observe) {
            case SUBSCRIBE:
                mObserve = 0;
                break;

            case UNSUBSCRIBE:
                mObserve = 1;
                break;

            case NOTHING:
                mObserve = -1;
                break;

            default:
        }
    }

    @Override
    public Observe getObserve() {
        switch (mObserve) {
            case -1:
                return Observe.NOTHING;

            case 0:
                return Observe.SUBSCRIBE;

            case 1:
                return Observe.UNSUBSCRIBE;
        }

        return Observe.SEQUENCE_NUMBER;
    }

    public void setContentFormat(ContentFormat format) {
        byte value = 0;
        switch (format) {
            case NO_CONTENT:
                break;
            case APPLICATION_LINK_FORMAT:
                value = 40;
                break;
            case APPLICATION_XML:
                value = 41;
                break;
            case APPLICATION_OCTET_STREAM:
                value = 42;
                break;
            case APPLICATION_EXI:
                value = 47;
                break;
            case APPLICATION_JSON:
                value = 50;
                break;
            case APPLICATION_CBOR:
                value = 60;
                break;
        }
        content_format = new byte[] { value };
    }

    public ContentFormat getContentFormat() {

        if (content_format == null) {
            return ContentFormat.NO_CONTENT;
        }

        int contentFormat = Bytes.bytesToInt(content_format);

        switch (contentFormat) {
            case 40:
                return ContentFormat.APPLICATION_LINK_FORMAT;

            case 41:
                return ContentFormat.APPLICATION_XML;

            case 42:
                return ContentFormat.APPLICATION_OCTET_STREAM;

            case 47:
                return ContentFormat.APPLICATION_EXI;

            case 50:
                return ContentFormat.APPLICATION_JSON;

            case 60:
            case 10000:
                return ContentFormat.APPLICATION_CBOR;
        }

        return ContentFormat.NO_CONTENT;
    }

    public int getContentFormatValue() {
        if (content_format == null) {
            return 0;
        } else {
            return Bytes.bytesToInt(content_format);
        }
    }

    public int getAcceptVersionValue() {
        if (accept_version == null) {
            return 0;
        } else {
            return Bytes.bytesToInt(accept_version);
        }
    }

    public void setVersionValue(int value) {
        setAcceptVersionValue(value);
        setContentVersionValue(value);
    }

    public void setAcceptVersionValue(int value) {
        accept_version = new byte[] { (byte) ((value >> 8) & 0xFF),
                (byte) (value & 0xFF) };
    }

    public int getContentVersionValue() {
        if (content_version == null) {
            return 0;
        } else {
            return Bytes.bytesToInt(content_version);
        }
    }

    public void setContentVersionValue(int value) {
        content_version = new byte[] { (byte) ((value >> 8) & 0xFF),
                (byte) (value & 0xFF) };
    }

    @Override
    public void setLocationPath(String locationPath) {
        if (location_path == null) {
            location_path = new ArrayList<>();
        } else {
            location_path.clear();
        }

        if (locationPath == null || locationPath.length() == 0) {
            location_path.clear();
            location_path = null;
            return;
        }

        String[] pathSegments = locationPath.split("/");
        for (String pathSegment : pathSegments) {
            if (pathSegment.length() == 0)
                continue;
            location_path.add(pathSegment.getBytes(StandardCharsets.UTF_8));
        }
    }
}
