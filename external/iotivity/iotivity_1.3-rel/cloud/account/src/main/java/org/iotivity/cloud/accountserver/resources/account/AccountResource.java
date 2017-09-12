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
package org.iotivity.cloud.accountserver.resources.account;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.resources.account.AccountManager.SearchOperation;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.exception.ServerException.UnAuthorizedException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.resource.Resource;
import org.iotivity.cloud.util.Cbor;
import org.iotivity.cloud.util.Log;

/**
 *
 * This class provides a set of APIs to manage resources corresponding with user
 * account
 *
 */
public class AccountResource extends Resource {

    private Cbor<HashMap<String, Object>> mCbor      = new Cbor<>();

    private AccountManager                mAsManager = new AccountManager();

    public AccountResource() {
        super(Arrays.asList(Constants.PREFIX_OIC, Constants.ACCOUNT_URI));

    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {

        IResponse response = null;

        switch (request.getMethod()) {

            case POST:
                // make sign-up response message
                response = handlePostSignUp(request);
                break;

            case GET:
                response = handleGetSearch(request);
                break;

            case DELETE:
                response = handleDeleteDevice(request);
                break;

            default:
                throw new BadRequestException(
                        request.getMethod() + " request type is not support");
        }
        // send sign-up response to the source device
        srcDevice.sendResponse(response);
    }

    private IResponse handlePostSignUp(IRequest request)
            throws ServerException {

        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(request.getPayload(), HashMap.class);

        if (payloadData == null) {
            throw new BadRequestException("payload is null");
        }

        HashMap<String, Object> responsePayload = null;

        // payload verification if the mandatory properties are
        // included in the payload
        if (checkPayloadException(Arrays.asList(Constants.REQ_DEVICE_ID,
                Constants.REQ_AUTH_CODE, Constants.REQ_AUTH_PROVIDER),
                payloadData)) {

            String did = payloadData.get(Constants.REQ_DEVICE_ID).toString();
            String authCode = payloadData.get(Constants.REQ_AUTH_CODE)
                    .toString();
            String authProvider = payloadData.get(Constants.REQ_AUTH_PROVIDER)
                    .toString();

            Log.d("authCode: " + authCode);

            Object options = payloadData.get(Constants.REQ_AUTH_OPTIONS);

            responsePayload = mAsManager.signUp(did, authCode, authProvider,
                    options);
        }

        return MessageBuilder.createResponse(request, ResponseStatus.CHANGED,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(responsePayload));
    }

    private IResponse handleGetSearch(IRequest request) {

        HashMap<String, List<String>> queryData = request.getUriQueryMap();

        if (queryData == null) {
            throw new BadRequestException("query is null");
        }

        HashMap<String, Object> responsePayload = null;

        String uriQuery = request.getUriQuery();

        // AND or OR operation to find users
        if (uriQuery != null && uriQuery.contains(",")) {
            queryData = mAsManager.getQueryMap(uriQuery, ",");
            responsePayload = (mAsManager.searchUserUsingCriteria(queryData,
                    SearchOperation.AND));
        } else {
            responsePayload = (mAsManager.searchUserUsingCriteria(queryData,
                    SearchOperation.OR));
        }

        Log.d("Search criteria query : " + queryData);

        return MessageBuilder.createResponse(request, ResponseStatus.CONTENT,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(responsePayload));
    }

    private IResponse handleDeleteDevice(IRequest request) {

        HashMap<String, List<String>> queryMap = request.getUriQueryMap();

        if (checkQueryException(Arrays.asList(Constants.REQ_UUID_ID,
                Constants.REQ_DEVICE_ID, Constants.REQ_ACCESS_TOKEN),
                queryMap)) {

            String uid = queryMap.get(Constants.REQ_UUID_ID).get(0);
            String did = queryMap.get(Constants.REQ_DEVICE_ID).get(0);
            String accesstoken = queryMap.get(Constants.REQ_ACCESS_TOKEN)
                    .get(0);
            if (!mAsManager.deleteDevice(uid, did, accesstoken))
                throw new UnAuthorizedException("accesstoken is not valid");
        }

        return MessageBuilder.createResponse(request, ResponseStatus.DELETED);
    }
}
