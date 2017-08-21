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

import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.protocols.enums.SignalingMethod;

public class CoapResponse extends CoapMessage {
    private ResponseStatus mResponseStatus;

    public CoapResponse(ResponseStatus responseStatus) {
        mResponseStatus = responseStatus;
    }

    public CoapResponse(int code) {
        switch (code) {
            case 65:
                mResponseStatus = ResponseStatus.CREATED;
                break;
            case 66:
                mResponseStatus = ResponseStatus.DELETED;
                break;
            case 67:
                mResponseStatus = ResponseStatus.VALID;
                break;
            case 68:
                mResponseStatus = ResponseStatus.CHANGED;
                break;
            case 69:
                mResponseStatus = ResponseStatus.CONTENT;
                break;
            case 128:
                mResponseStatus = ResponseStatus.BAD_REQUEST;
                break;
            case 129:
                mResponseStatus = ResponseStatus.UNAUTHORIZED;
                break;
            case 130:
                mResponseStatus = ResponseStatus.BAD_OPTION;
                break;
            case 131:
                mResponseStatus = ResponseStatus.FORBIDDEN;
                break;
            case 132:
                mResponseStatus = ResponseStatus.NOT_FOUND;
                break;
            case 133:
                mResponseStatus = ResponseStatus.METHOD_NOT_ALLOWED;
                break;
            case 134:
                mResponseStatus = ResponseStatus.NOT_ACCEPTABLE;
                break;
            case 140:
                mResponseStatus = ResponseStatus.PRECONDITION_FAILED;
                break;
            case 141:
                mResponseStatus = ResponseStatus.REQUEST_ENTITY_TOO_LARGE;
                break;
            case 143:
                mResponseStatus = ResponseStatus.UNSUPPORTED_CONTENT_FORMAT;
                break;
            case 160:
                mResponseStatus = ResponseStatus.INTERNAL_SERVER_ERROR;
                break;
            case 161:
                mResponseStatus = ResponseStatus.NOT_IMPLEMENTED;
                break;
            case 162:
                mResponseStatus = ResponseStatus.BAD_GATEWAY;
                break;
            case 163:
                mResponseStatus = ResponseStatus.SERVICE_UNAVAILABLE;
                break;
            case 164:
                mResponseStatus = ResponseStatus.GATEWAY_TIMEOUT;
                break;
            case 165:
                mResponseStatus = ResponseStatus.PROXY_NOT_SUPPORTED;
                break;
            default:
                // unrecognized response code is treated as being equivalent to
                // the generic response code of 4.00 (RFC7252)
                mResponseStatus = ResponseStatus.BAD_REQUEST;
        }
    }

    @Override
    public int getCode() {
        // TODO Auto-generated method stub
        switch (mResponseStatus) {
            case BAD_GATEWAY:
                return 162;
            case BAD_OPTION:
                return 130;
            case BAD_REQUEST:
                return 128;
            case CHANGED:
                return 68;
            case CONTENT:
                return 69;
            case CREATED:
                return 65;
            case DELETED:
                return 66;
            case FORBIDDEN:
                return 131;
            case GATEWAY_TIMEOUT:
                return 164;
            case INTERNAL_SERVER_ERROR:
                return 160;
            case METHOD_NOT_ALLOWED:
                return 133;
            case NOT_ACCEPTABLE:
                return 134;
            case NOT_FOUND:
                return 132;
            case NOT_IMPLEMENTED:
                return 161;
            case PRECONDITION_FAILED:
                return 140;
            case PROXY_NOT_SUPPORTED:
                return 165;
            case REQUEST_ENTITY_TOO_LARGE:
                return 141;
            case SERVICE_UNAVAILABLE:
                return 163;
            case UNAUTHORIZED:
                return 129;
            case UNSUPPORTED_CONTENT_FORMAT:
                return 143;
            case VALID:
                return 67;
            default:
                break;
        }
        return 0;
    }

    // This response object does not support request method
    @Override
    public RequestMethod getMethod() {
        return RequestMethod.GET;
    }

    @Override
    public ResponseStatus getStatus() {
        return mResponseStatus;
    }

    // This response object does not support signaling method
    @Override
    public SignalingMethod getSignalingMethod() {
        return null;
    }
}
