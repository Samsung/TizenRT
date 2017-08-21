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
import java.util.HashMap;

import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.resource.Resource;
import org.iotivity.cloud.ciserver.Constants;
import org.iotivity.cloud.util.Cbor;

/**
 *
 * This class provides a set of APIs to send requests about session to account
 *
 */

public class AccountSession extends Resource {
    private Cbor<HashMap<String, Object>> mCbor       = new Cbor<>();

    public AccountSession() {
        super(Arrays.asList(Constants.PREFIX_OIC, Constants.ACCOUNT_URI,
                Constants.SESSION_URI));
    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(request.getPayload(), HashMap.class);

        checkPayloadException(Constants.REQ_LOGIN, payloadData);

        if (payloadData.get(Constants.REQ_LOGIN).toString().equals("false")) {
            StringBuffer additionalQuery = new StringBuffer();
            additionalQuery
                    .append(Constants.USER_ID + "=" + srcDevice.getUserId());
            String uriQuery = additionalQuery.toString()
                    + (request.getUriQuery() != null
                            ? (";" + request.getUriQuery()) : "");
            request = MessageBuilder.modifyRequest(request, null, uriQuery,
                    ContentFormat.APPLICATION_CBOR,
                    mCbor.encodingPayloadToCbor(payloadData));
        }
        ConnectorPool.getConnection("account").sendRequest(request, srcDevice);
    }
}