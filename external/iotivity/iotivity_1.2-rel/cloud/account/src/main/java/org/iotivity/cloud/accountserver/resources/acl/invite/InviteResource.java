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
package org.iotivity.cloud.accountserver.resources.acl.invite;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.exception.ServerException.PreconditionFailedException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.resource.Resource;
import org.iotivity.cloud.util.Cbor;

/**
 * 
 * This class provides a set of APIs to handle requests to Invite resource.
 *
 */
public class InviteResource extends Resource {

    private InviteManager                 mInviteManager = new InviteManager();

    private Cbor<HashMap<String, Object>> mCbor          = new Cbor<>();

    private enum ReqType {
        NONE, CANCEL_INVITATION, DELETE_INVITATION
    };

    public InviteResource() {
        super(Arrays.asList(Constants.PREFIX_OIC, Constants.ACL_URI,
                Constants.INVITE_URI));
    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {

        IResponse response = null;

        switch (request.getMethod()) {
            case GET:
                response = handleGetRequest(srcDevice, request);
                break;
            case POST:
                // handle send-invitation message
                response = handlePostRequest(request);
                break;
            case DELETE:
                response = handleDeleteRequest(request);
                break;
            default:
                throw new BadRequestException(
                        request.getMethod() + " request type is not supported");
        }

        srcDevice.sendResponse(response);
    }

    private IResponse handleGetRequest(Device srcDevice, IRequest request)
            throws ServerException {

        checkQueryException(Constants.REQ_UUID_ID, request.getUriQueryMap());

        String uid = request.getUriQueryMap().get(Constants.REQ_UUID_ID).get(0);
        HashMap<String, Object> responsePayload = null;

        switch (request.getObserve()) {
            case NOTHING:
                responsePayload = mInviteManager.getInvitationInfo(uid);
                break;
            case SUBSCRIBE:
                responsePayload = mInviteManager.addSubscriber(uid, srcDevice,
                        request);
                break;
            case UNSUBSCRIBE:
                responsePayload = mInviteManager.removeSubscriber(uid, request);
                break;
            default:
                break;
        }

        return MessageBuilder.createResponse(request, ResponseStatus.CONTENT,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(responsePayload));
    }

    @SuppressWarnings("unchecked")
    private IResponse handlePostRequest(IRequest request)
            throws ServerException {

        HashMap<String, Object> payload = mCbor
                .parsePayloadFromCbor(request.getPayload(), HashMap.class);

        // check properties in payload
        checkPayloadException(
                Arrays.asList(Constants.REQ_UUID_ID, Constants.REQ_INVITE),
                payload);

        // get user uid
        String uid = (String) payload.get(Constants.REQ_UUID_ID);
        ArrayList<HashMap<String, String>> inviteList = (ArrayList<HashMap<String, String>>) payload
                .get(Constants.REQ_INVITE);

        for (HashMap<String, String> invite : inviteList) {

            String gid = invite.get(Constants.REQ_GROUP_ID);
            String mid = invite.get(Constants.REQ_MEMBER);

            if (gid == null || mid == null) {

                throw new PreconditionFailedException(
                        "value of invite property is invalid");
            }

            mInviteManager.addInvitation(uid, gid, mid);
        }

        return MessageBuilder.createResponse(request, ResponseStatus.CHANGED);
    }

    private IResponse handleDeleteRequest(IRequest request)
            throws ServerException {

        HashMap<String, List<String>> queryParams = request.getUriQueryMap();

        checkQueryException(
                Arrays.asList(Constants.REQ_UUID_ID, Constants.REQ_GROUP_ID),
                queryParams);

        String gid = queryParams.get(Constants.REQ_GROUP_ID).get(0);
        String uid = queryParams.get(Constants.REQ_UUID_ID).get(0);

        ReqType reqType = ReqType.NONE;
        if (queryParams.get(Constants.REQ_MEMBER) == null) {
            reqType = ReqType.DELETE_INVITATION;
        } else {
            reqType = ReqType.CANCEL_INVITATION;
        }

        if (reqType.equals(ReqType.DELETE_INVITATION)) {

            String acceptStr = queryParams.get(Constants.REQ_INVITE_ACCEPT)
                    .get(0);

            boolean accepted = false;
            if (acceptStr.equals(Constants.INVITE_ACCEPT)) {
                accepted = true;
            }

            mInviteManager.deleteInvitation(uid, gid, accepted);

        } else if (reqType.equals(ReqType.CANCEL_INVITATION)) {

            String mid = queryParams.get(Constants.REQ_MEMBER).get(0);
            mInviteManager.cancelInvitation(uid, gid, mid);

        } else {

            throw new BadRequestException("queryData is not enough");
        }

        return MessageBuilder.createResponse(request, ResponseStatus.DELETED);
    }

}