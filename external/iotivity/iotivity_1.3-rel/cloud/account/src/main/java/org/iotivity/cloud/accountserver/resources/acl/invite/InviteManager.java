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
import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.db.InviteTable;
import org.iotivity.cloud.accountserver.resources.acl.group.GroupBrokerManager;
import org.iotivity.cloud.accountserver.resources.acl.group.GroupManager;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.util.Cbor;

/**
 *
 * This class provides a set of APIs to invite a user to a group
 *
 */
public class InviteManager {

    private TypeCastingManager<InviteTable> mTypeInvite = new TypeCastingManager<>();

    private static class InviteSubscriber {
        InviteSubscriber(Device subscriber, IRequest request) {
            mSubscriber = subscriber;
            mRequest = request;
        }

        public Device   mSubscriber;
        public IRequest mRequest;
    }

    // <uid, subscriber list>
    private HashMap<String, ArrayList<InviteSubscriber>> mSubscribers = new HashMap<>();

    /**
     * API to add invitation
     * 
     * @param uid
     *            id of user who sent invitation
     * @param gid
     *            id of group to invite member to
     * @param mid
     *            id of invited user
     */
    public void addInvitation(String uid, String gid, String mid) {

        // create invitation table
        InviteTable newInviteTable = new InviteTable(uid, gid, mid);

        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.KEYFIELD_INVITE_USER, uid);
        condition.put(Constants.KEYFIELD_GID, gid);
        condition.put(Constants.KEYFIELD_INVITED_USER, mid);

        if (AccountDBManager.getInstance()
                .selectRecord(Constants.INVITE_TABLE, condition).isEmpty()) {
            AccountDBManager.getInstance().insertRecord(Constants.INVITE_TABLE,
                    mTypeInvite.convertObjectToMap(newInviteTable));
            notifyToSubscriber(uid);
            notifyToSubscriber(mid);
        }
    }

    /**
     * API to delete invitation by invited user
     * 
     * @param mid
     *            id of invited user
     * @param gid
     *            id of group which the user was invited to
     * @param accepted
     *            value of invitation accept or deny
     */
    public void deleteInvitation(String mid, String gid, boolean accepted) {
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.REQ_GROUP_ID, gid);
        condition.put(Constants.KEYFIELD_INVITED_USER, mid);

        InviteTable getInviteTable = new InviteTable();

        List<HashMap<String, Object>> getInviteList = AccountDBManager
                .getInstance().selectRecord(Constants.INVITE_TABLE, condition);

        if (getInviteList == null || getInviteList.isEmpty()) {
            throw new BadRequestException("mid or gid is not valid");
        }

        ArrayList<String> uidList = new ArrayList<>();
        for (HashMap<String, Object> getInvite : getInviteList) {
            getInviteTable = mTypeInvite.convertMaptoObject(getInvite,
                    getInviteTable);
            uidList.add(getInviteTable.getInviteUser());
        }

        if (getInviteList == null || getInviteList.isEmpty()) {
            throw new BadRequestException("mid or gid is not valid");
        }

        AccountDBManager.getInstance().deleteRecord(Constants.INVITE_TABLE,
                condition);

        /* add user into group */
        if (accepted) {

            ArrayList<String> midlist = new ArrayList<>();
            midlist.add(mid);

            GroupManager.getInstance().addMembersToGroup(gid, midlist);
            GroupBrokerManager.getInstance().notifyToObservers(
                    GroupManager.getInstance().getGroupTable(gid).getMembers());
        }

        notifyToSubscriber(mid);
        for (String uid : uidList) {
            notifyToSubscriber(uid);
        }
    }

    /**
     * API to cancel invitation by user who invited member
     * 
     * @param uid
     *            id of user who sent invitation
     * @param gid
     *            id of group to invite member to
     * @param mid
     *            id of invited user
     */
    public void cancelInvitation(String uid, String gid, String mid) {

        HashMap<String, Object> condition = new HashMap<>();

        condition.put(Constants.REQ_GROUP_ID, gid);
        condition.put(Constants.KEYFIELD_INVITED_USER, mid);
        condition.put(Constants.KEYFIELD_INVITE_USER, uid);

        AccountDBManager.getInstance().deleteRecord(Constants.INVITE_TABLE,
                condition);

        notifyToSubscriber(uid);
        notifyToSubscriber(mid);
    }

    /**
     * API to get invitation information
     * 
     * @param uid
     *            user id
     * 
     * @return returns invite and invited information of the user
     */
    public HashMap<String, Object> getInvitationInfo(String uid) {
        HashMap<String, Object> responsePayload = new HashMap<>();

        ArrayList<Object> invitePayloadData = null;
        ArrayList<Object> invitedPayloadData = null;

        List<InviteTable> inviteList = getInviteTableList(
                Constants.KEYFIELD_INVITE_USER, uid);
        if (!inviteList.isEmpty()) {
            invitePayloadData = new ArrayList<>();
            for (InviteTable invite : inviteList) {
                HashMap<String, String> inviteElement = new HashMap<>();
                inviteElement.put(Constants.REQ_GROUP_ID, invite.getGid());
                inviteElement.put(Constants.REQ_MEMBER,
                        invite.getInvitedUser());
                invitePayloadData.add(inviteElement);
            }
        }

        List<InviteTable> invitedList = getInviteTableList(
                Constants.KEYFIELD_INVITED_USER, uid);
        if (!invitedList.isEmpty()) {
            invitedPayloadData = new ArrayList<>();
            for (InviteTable invited : invitedList) {
                HashMap<String, String> invitedElement = new HashMap<>();
                invitedElement.put(Constants.REQ_GROUP_ID, invited.getGid());
                invitedElement.put(Constants.REQ_MEMBER,
                        invited.getInviteUser());
                invitedPayloadData.add(invitedElement);
            }
        }

        responsePayload.put(Constants.RESP_INVITE, invitePayloadData);
        responsePayload.put(Constants.RESP_INVITED, invitedPayloadData);

        return responsePayload;
    }

    /**
     * API to add subscriber of invite resource
     * 
     * @param uid
     *            user id
     * @param subscriber
     *            device that sent request for subscription
     * @param request
     *            received request for subscription
     * 
     * @return returns invite and invited information of the user
     */
    public HashMap<String, Object> addSubscriber(String uid, Device subscriber,
            IRequest request) {

        InviteSubscriber newSubscriber = new InviteSubscriber(subscriber,
                request);

        synchronized (mSubscribers) {
            ArrayList<InviteSubscriber> subscriberList = mSubscribers.get(uid);

            if (subscriberList == null) {
                subscriberList = new ArrayList<>();
            }

            subscriberList.add(newSubscriber);
            mSubscribers.put(uid, subscriberList);
        }

        return getInvitationInfo(uid);
    }

    /**
     * API to remove subscriber of invite resource
     * 
     * @param uid
     *            user id
     * @param request
     *            received request for unsubscription
     * 
     * @return returns invite and invited information of the user
     */
    public HashMap<String, Object> removeSubscriber(String uid,
            IRequest request) {

        synchronized (mSubscribers) {
            if (mSubscribers.containsKey(uid)) {

                mSubscribers.get(uid).removeIf(subscriber -> subscriber.mRequest
                        .getRequestId().equals(request.getRequestId()));
            }
        }

        return getInvitationInfo(uid);
    }

    private void notifyToSubscriber(String id) {

        synchronized (mSubscribers) {
            if (!mSubscribers.containsKey(id)) {
                return;
            }

            Cbor<HashMap<String, Object>> cbor = new Cbor<>();
            byte[] payload = cbor.encodingPayloadToCbor(getInvitationInfo(id));

            for (InviteSubscriber subscriber : mSubscribers.get(id)) {

                subscriber.mSubscriber.sendResponse(
                        MessageBuilder.createResponse(subscriber.mRequest,
                                ResponseStatus.CONTENT,
                                ContentFormat.APPLICATION_CBOR, payload));
            }
        }
    }

    private List<InviteTable> getInviteTableList(String property, String uid) {

        InviteTable getInviteTable = new InviteTable();
        ArrayList<InviteTable> inviteList = new ArrayList<>();

        HashMap<String, Object> condition = new HashMap<>();
        condition.put(property, uid);
        ArrayList<HashMap<String, Object>> mapInviteList = AccountDBManager
                .getInstance().selectRecord(Constants.INVITE_TABLE, condition);
        if (mapInviteList == null) {
            throw new BadRequestException("uid is invalid");
        }
        for (HashMap<String, Object> mapInviteTable : mapInviteList) {

            getInviteTable = mTypeInvite.convertMaptoObject(mapInviteTable,
                    getInviteTable);
            inviteList.add(getInviteTable);
        }
        return inviteList;

    }

}