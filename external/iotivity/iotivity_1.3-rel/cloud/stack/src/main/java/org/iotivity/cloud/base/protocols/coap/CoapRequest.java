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

import org.iotivity.cloud.base.exception.ServerException.MethodNotAllowedException;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.protocols.enums.SignalingMethod;

public class CoapRequest extends CoapMessage {
    private RequestMethod mRequestMethod;

    public CoapRequest(RequestMethod requestMethod) {
        mRequestMethod = requestMethod;
    }

    public CoapRequest(int code) {
        switch (code) {
            case 1:
                mRequestMethod = RequestMethod.GET;
                break;
            case 2:
                mRequestMethod = RequestMethod.POST;
                break;
            case 3:
                mRequestMethod = RequestMethod.PUT;
                break;
            case 4:
                mRequestMethod = RequestMethod.DELETE;
                break;
            default:
                // unrecognized or unsupported Method Code MUST generate
                // a 4.05 (Method Not Allowed) piggybacked response. (RFC7252)
                throw new MethodNotAllowedException("Invalid CoapRequest code");
        }
    }

    @Override
    public int getCode() {
        switch (mRequestMethod) {
            case GET:
                return 1;
            case POST:
                return 2;
            case PUT:
                return 3;
            case DELETE:
                return 4;
            default:
                break;

        }
        return 0;
    }

    @Override
    public RequestMethod getMethod() {
        return mRequestMethod;
    }

    // This request object does not support response status
    @Override
    public ResponseStatus getStatus() {
        return ResponseStatus.METHOD_NOT_ALLOWED;
    }

    // This request object does not support signaling status
    @Override
    public SignalingMethod getSignalingMethod() {
        return null;
    }
}