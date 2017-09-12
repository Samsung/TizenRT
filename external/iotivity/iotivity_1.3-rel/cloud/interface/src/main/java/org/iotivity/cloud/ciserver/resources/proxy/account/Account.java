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
package org.iotivity.cloud.ciserver.resources.proxy.account;

import java.util.Arrays;

import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.device.IResponseEventHandler;
import org.iotivity.cloud.base.exception.ClientException;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.resource.Resource;
import org.iotivity.cloud.ciserver.Constants;

/**
 *
 * This class provides a set of APIs to send requests about account to account
 *
 */

public class Account extends Resource {

    public Account() {
        super(Arrays.asList(Constants.PREFIX_OIC, Constants.ACCOUNT_URI));
    }

    class RDReceiveHandler implements IResponseEventHandler {

        private Device    mSrcDevice;
        private IResponse mResponse;
        private IRequest  mRequest;

        public RDReceiveHandler(IRequest request, IResponse response,
                Device srcDevice) {
            mSrcDevice = srcDevice;
            mRequest = request;
            mResponse = response;
        }

        @Override
        public void onResponseReceived(IResponse response)
                throws ClientException {
            switch (response.getStatus()) {
                case DELETED:
                    mSrcDevice.sendResponse(mResponse);
                    break;
                default:
                    mSrcDevice.sendResponse(MessageBuilder.createResponse(
                            mRequest, ResponseStatus.BAD_REQUEST));
            }
        }
    }

    class AccountReceiveHandler implements IResponseEventHandler {

        private Device   mSrcDevice;
        private IRequest mRequest;

        public AccountReceiveHandler(IRequest request, Device srcDevice) {
            mSrcDevice = srcDevice;
            mRequest = request;
        }

        @Override
        public void onResponseReceived(IResponse response)
                throws ClientException {
            switch (response.getStatus()) {
                case DELETED:
                    StringBuffer uriPath = new StringBuffer();
                    uriPath.append(Constants.PREFIX_OIC + "/");
                    uriPath.append(Constants.RD_URI);
                    ConnectorPool.getConnection("rd").sendRequest(
                            MessageBuilder.createRequest(RequestMethod.DELETE,
                                    uriPath.toString(), mRequest.getUriQuery()),
                            new RDReceiveHandler(mRequest, response,
                                    mSrcDevice));
                    break;
                default:
                    mSrcDevice.sendResponse(response);
            }
        }
    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {
        switch (request.getMethod()) {
            case GET:
                if (request.getUriQuery() == null) {
                    StringBuffer additionalQuery = new StringBuffer();
                    additionalQuery.append(
                            Constants.USER_ID + "=" + srcDevice.getUserId());
                    String uriQuery = additionalQuery.toString()
                            + (request.getUriQuery() != null
                                    ? (";" + request.getUriQuery()) : "");
                    request = MessageBuilder.modifyRequest(request, null,
                            uriQuery, null, null);
                }
                break;
            case DELETE:
                StringBuffer additionalQuery = new StringBuffer();
                additionalQuery.append(
                        Constants.USER_ID + "=" + srcDevice.getUserId());
                String uriQuery = additionalQuery.toString()
                        + (request.getUriQuery() != null
                                ? (";" + request.getUriQuery()) : "");
                request = MessageBuilder.modifyRequest(request, null, uriQuery,
                        null, null);
                break;
            default:
                break;
        }
        ConnectorPool.getConnection("account").sendRequest(request,
                new AccountReceiveHandler(request, srcDevice));
    }
}
