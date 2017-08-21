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
package org.iotivity.cloud.accountserver.resources.acl.group;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Objects;

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

public class GroupResource extends Resource {
    private Cbor<HashMap<String, Object>> mCbor = new Cbor<>();

    public GroupResource() {
        super(Arrays.asList(Constants.PREFIX_OIC, Constants.ACL_URI,
                Constants.GROUP_URI));
    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {

        IResponse response = null;

        if (getUriPathSegments().containsAll(request.getUriPathSegments())) {
            response = groupBrokerManagement(srcDevice, request);
        }

        else {
            response = individualGroupManagement(request);
        }
        srcDevice.sendResponse(response);
    }

    private IResponse groupBrokerManagement(Device srcDevice,
            IRequest request) {
        IResponse response = null;
        switch (request.getMethod()) {
            case POST:
                response = handleBrokerPostRequest(request);
                break;
            case GET:
                response = handleBrokerGetRequest(srcDevice, request);
                break;
            default:
                throw new BadRequestException(
                        request.getMethod() + " request type is not support");
        }
        return response;
    }

    private IResponse handleBrokerPostRequest(IRequest request) {
        byte[] payload = request.getPayload();

        HashMap<String, Object> parsedPayload = mCbor
                .parsePayloadFromCbor(payload, HashMap.class);

        checkQueryException(Arrays.asList(Constants.KEYFIELD_UID),
                request.getUriQueryMap());

        checkPayloadException(Arrays.asList(Constants.KEYFIELD_GROUP_MEMBERS),
                parsedPayload);

        // get user id
        String uid = request.getUriQueryMap().get(Constants.KEYFIELD_UID)
                .get(0);

        if (uid == null || uid.isEmpty()) {
            throw new BadRequestException(
                    Constants.REQ_UUID_ID + " is null or empty");
        }

        String gname = getProperty(parsedPayload,
                Constants.KEYFIELD_GROUP_NAME);
        String parent = getProperty(parsedPayload,
                Constants.KEYFIELD_GROUP_PARENT);

        if (parent != null) {
            ArrayList<String> properties = new ArrayList<>(
                    Arrays.asList(Constants.KEYFIELD_GROUP));
            GroupBrokerManager.getInstance().verifyAuthorization(uid, parent,
                    properties, UserOperation.ADD);
        }

        HashMap<String, Object> responsePayload = GroupBrokerManager
                .getInstance().createGroup(uid, null, gname, parent);

        if (parent != null) {
            GroupBrokerManager.getInstance().notifyToObservers(GroupManager
                    .getInstance().getGroupTable(parent).getMembers());
        }

        return MessageBuilder.createResponse(request, ResponseStatus.CHANGED,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(responsePayload));
    }

    private IResponse handleBrokerGetRequest(Device srcDevice,
            IRequest request) {
        HashMap<String, List<String>> queryMap = request.getUriQueryMap();

        checkQueryException(Arrays.asList(Constants.REQ_UUID_ID,
                Constants.KEYFIELD_GROUP_MEMBERS), queryMap);

        String uid = queryMap.get(Constants.REQ_UUID_ID).get(0);
        if (!uid.equals(
                queryMap.get(Constants.KEYFIELD_GROUP_MEMBERS).get(0))) {
            throw new BadRequestException(
                    Constants.REQ_UUID_ID + "query value should be equal to "
                            + Constants.KEYFIELD_GROUP_MEMBERS + "query value");
        }
        switch (request.getObserve()) {
            case SUBSCRIBE:
                GroupBrokerManager.getInstance().addObserver(uid, srcDevice,
                        request);
                break;
            case UNSUBSCRIBE:
                GroupBrokerManager.getInstance().removeObserver(uid, srcDevice,
                        request);
                break;
            default:
        }

        HashMap<String, Object> responsePayload = GroupBrokerManager
                .getInstance().getGroupList(uid);

        return MessageBuilder.createResponse(request, ResponseStatus.CONTENT,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(responsePayload));
    }

    private String getProperty(HashMap<String, Object> properties, String key) {
        String property = null;
        Object obj = properties.get(key);
        if (obj != null) {
            property = obj.toString();
        }
        return property;
    }

    private IResponse individualGroupManagement(IRequest request) {
        IResponse response = null;
        switch (request.getMethod()) {
            case POST:
                response = handleGroupPostRequest(request);
                break;
            case GET:
                response = handleGroupGetRequest(request);
                break;
            case DELETE:
                response = handleGroupDeleteRequest(request);
                break;
            default:
                throw new BadRequestException(
                        request.getMethod() + " request type is not support");
        }
        return response;
    }

    private IResponse handleGroupPostRequest(IRequest request)
            throws ServerException {
        if (request.getPayload() == null) {
            throw new BadRequestException("payload is null");
        }
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(request.getPayload(), HashMap.class);

        // TODO to be deleted
        if (payloadData.containsKey(Constants.KEYFIELD_UID)) {
            throw new BadRequestException(
                    " uid key is not supported in the payload");
        }

        checkQueryException(Arrays.asList(Constants.KEYFIELD_UID),
                request.getUriQueryMap());

        // get user id
        String uid = request.getUriQueryMap().get(Constants.KEYFIELD_UID)
                .get(0);

        if (uid == null || uid.isEmpty()) {
            throw new BadRequestException(
                    Constants.REQ_UUID_ID + " is null or empty");
        }

        String gid = request.getUriPathSegments()
                .get(getUriPathSegments().size());

        // process POST oic/acl/group/<gid> to update group info
        if (!request.getUriQueryMap()
                .containsKey(Constants.REQ_GROUP_QUERY_OPERATION)) {
            handlePostUpdateRequest(gid, uid, payloadData);
        } else {
            checkQueryException(Constants.REQ_GROUP_QUERY_OPERATION,
                    request.getUriQueryMap());

            String postOption = request.getUriQueryMap()
                    .get(Constants.REQ_GROUP_QUERY_OPERATION).get(0);

            switch (postOption) {
                case Constants.REQ_GROUP_QUERY_ADD:
                    handlePostAddRequest(gid, uid, payloadData);
                    break;
                case Constants.REQ_GROUP_QUERY_DELETE:
                    handlePostDeleteRequest(gid, uid, payloadData);
                    break;
                default:
                    throw new PreconditionFailedException(
                            postOption + " query option is not supported");
            }
        }
        GroupBrokerManager.getInstance().notifyToObservers(
                GroupManager.getInstance().getGroupTable(gid).getMembers());
        return MessageBuilder.createResponse(request, ResponseStatus.CHANGED);
    }

    private void handlePostAddRequest(String gid, String mid,
            HashMap<String, Object> properties) {
        GroupManager.getInstance().verifyPostRequestAuthz(gid, mid, properties,
                UserOperation.ADD);
        addToGroupProcess(gid, properties);

    }

    private void handlePostDeleteRequest(String gid, String mid,
            HashMap<String, Object> properties) {
        GroupManager.getInstance().verifyPostRequestAuthz(gid, mid, properties,
                UserOperation.DELETE);
        deleteFromGroupProcess(gid, properties);
    }

    private void handlePostUpdateRequest(String gid, String mid,
            HashMap<String, Object> properties) {
        HashMap<String, Object> addedProperties = new HashMap<>();
        HashMap<String, Object> deletedProperties = new HashMap<>();
        HashMap<String, Object> replacedProperties = new HashMap<>();

        Iterator<String> keys = properties.keySet().iterator();

        while (keys.hasNext()) {
            String key = keys.next();
            if (properties.get(key) instanceof Collection) {
                ArrayList<Object> addedValues = GroupManager.getInstance()
                        .getAddPropertyValues(gid, key,
                                (ArrayList<Object>) properties.get(key));
                ArrayList<Object> deletedValues = GroupManager.getInstance()
                        .getDeletePropertyValues(gid, key,
                                (ArrayList<Object>) properties.get(key));
                if (!addedValues.isEmpty()) {
                    addedProperties.put(key, addedValues);
                }
                if (!deletedValues.isEmpty()) {
                    deletedProperties.put(key, deletedValues);
                }
            } else if (properties.get(key) instanceof String) {
                replacedProperties.put(key, properties.get(key));
            } else {
                throw new PreconditionFailedException(
                        "payload instance type is not supported");
            }
        }

        GroupManager.getInstance().verifyPostRequestAuthz(gid, mid,
                replacedProperties, UserOperation.REPLACE);

        GroupManager.getInstance().verifyPostRequestAuthz(gid, mid,
                addedProperties, UserOperation.ADD);

        GroupManager.getInstance().verifyPostRequestAuthz(gid, mid,
                deletedProperties, UserOperation.DELETE);

        deleteFromGroupProcess(gid, deletedProperties);

        addToGroupProcess(gid, addedProperties);

        replaceToGroupProcess(gid, replacedProperties);
    }

    private void addToGroupProcess(String gid,
            HashMap<String, Object> properties) {
        Iterator<String> keys = properties.keySet().iterator();
        while (keys.hasNext()) {
            String key = keys.next();
            switch (key) {
                case Constants.KEYFIELD_GROUP_MEMBERS:
                    GroupManager.getInstance().addMembersToGroup(gid,
                            (convertArrayObjectToString(
                                    (ArrayList<Object>) properties.get(key))));
                    break;
                case Constants.KEYFIELD_GROUP_MASTERS:
                    GroupManager.getInstance().addMastersToGroup(gid,
                            (convertArrayObjectToString(
                                    (ArrayList<Object>) properties.get(key))));
                    break;
                case Constants.KEYFIELD_GROUP_DEVICES:
                    GroupManager.getInstance().addDevicesToGroup(gid,
                            (convertArrayObjectToString(
                                    (ArrayList<Object>) properties.get(key))));
                    break;
                case Constants.KEYFIELD_GROUP_RESOURCES:
                    GroupManager.getInstance().addResourcesToGroup(gid,
                            (ArrayList<Object>) properties.get(key));
                    break;
                default:
                    throw new BadRequestException(key
                            + " property is not supported to add values to the group");
            }
        }
    }

    private void deleteFromGroupProcess(String gid,
            HashMap<String, Object> properties) {
        Iterator<String> keys = properties.keySet().iterator();
        while (keys.hasNext()) {
            String key = keys.next();
            switch (key) {
                case Constants.KEYFIELD_GROUP_MEMBERS:
                    GroupManager.getInstance().deleteMembersFromGroup(gid,
                            (convertArrayObjectToString(
                                    (ArrayList<Object>) properties.get(key))));
                    break;
                case Constants.KEYFIELD_GROUP_MASTERS:
                    GroupManager.getInstance().deleteMastersFromGroup(gid,
                            (convertArrayObjectToString(
                                    (ArrayList<Object>) properties.get(key))));
                    break;
                case Constants.KEYFIELD_GROUP_DEVICES:
                    GroupManager.getInstance().deleteDevicesFromGroup(gid,
                            (convertArrayObjectToString(
                                    (ArrayList<Object>) properties.get(key))));
                    break;
                case Constants.KEYFIELD_GROUP_RESOURCES:
                    GroupManager.getInstance().deleteResourcesFromGroup(gid,
                            (ArrayList<Object>) properties.get(key));
                    break;
                default:
                    throw new BadRequestException(key
                            + " property is not supported to delete values from the group");
            }
        }
    }

    private void replaceToGroupProcess(String gid,
            HashMap<String, Object> properties) {
        Iterator<String> keys = properties.keySet().iterator();
        while (keys.hasNext()) {
            String key = keys.next();
            switch (key) {
                case Constants.KEYFIELD_GROUP_NAME:
                    GroupManager.getInstance().replaceGnameToGroup(gid,
                            (String) properties.get(key));
                    break;
                case Constants.KEYFIELD_GROUP_OWNER:
                    GroupManager.getInstance().replaceOwnerToGroup(gid,
                            (String) properties.get(key));
                    break;
                default:
                    throw new BadRequestException(key
                            + " property is not supported to replace the value to the group");
            }
        }
    }

    private IResponse handleGroupGetRequest(IRequest request)
            throws ServerException {
        HashMap<String, List<String>> queryMap = request.getUriQueryMap();

        checkQueryException(Arrays.asList(Constants.REQ_UUID_ID,
                Constants.KEYFIELD_GROUP_MEMBERS), queryMap);

        String uid = queryMap.get(Constants.REQ_UUID_ID).get(0);

        if (uid == null || uid.isEmpty()) {
            throw new BadRequestException(
                    Constants.REQ_UUID_ID + " is null or empty");
        }

        if (!uid.equals(
                queryMap.get(Constants.KEYFIELD_GROUP_MEMBERS).get(0))) {
            throw new BadRequestException(
                    Constants.REQ_UUID_ID + "query value should be equal to "
                            + Constants.KEYFIELD_GROUP_MEMBERS + "query value");
        }

        String gid = request.getUriPathSegments()
                .get(getUriPathSegments().size());

        GroupManager.getInstance().verifyGetRequestAuthz(gid, uid);

        HashMap<String, Object> responsePayload = null;

        switch (request.getObserve()) {
            case NOTHING:
                responsePayload = GroupManager.getInstance().getGroupInfo(gid);
                break;
            case SUBSCRIBE:
            case UNSUBSCRIBE:
            default:
                throw new BadRequestException(
                        request.getObserve() + " observe type is not support");
        }

        return MessageBuilder.createResponse(request, ResponseStatus.CONTENT,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(responsePayload));
    }

    private IResponse handleGroupDeleteRequest(IRequest request)
            throws ServerException {

        HashMap<String, List<String>> queryMap = request.getUriQueryMap();

        checkQueryException(Arrays.asList(Constants.REQ_UUID_ID,
                Constants.KEYFIELD_GROUP_OWNER), queryMap);

        String uid = queryMap.get(Constants.REQ_UUID_ID).get(0);

        if (uid == null || uid.isEmpty()) {
            throw new BadRequestException(
                    Constants.REQ_UUID_ID + " is null or empty");
        }

        if (!uid.equals(queryMap.get(Constants.KEYFIELD_GROUP_OWNER).get(0))) {
            throw new BadRequestException(
                    Constants.REQ_UUID_ID + "query value should be equal to "
                            + Constants.KEYFIELD_GROUP_OWNER
                            + "query value to delete group");
        }

        String gid = request.getUriPathSegments()
                .get(getUriPathSegments().size());

        GroupManager.getInstance().verifyDeleteRequestAuthz(gid, uid);

        String parent = GroupManager.getInstance().getGroupTable(gid)
                .getParent();
        ArrayList<String> deletegGroupMembers = GroupManager.getInstance()
                .getGroupTable(gid).getMembers();

        GroupManager.getInstance().deleteGroup(gid);

        if (parent != null) {
            GroupBrokerManager.getInstance().notifyToObservers(GroupManager
                    .getInstance().getGroupTable(parent).getMembers());
        } else {
            GroupBrokerManager.getInstance()
                    .notifyToObservers(deletegGroupMembers);
        }

        GroupAclManager.getInstance().removeAceByGroup(gid);

        return MessageBuilder.createResponse(request, ResponseStatus.DELETED);
    }

    private ArrayList<String> convertArrayObjectToString(
            ArrayList<Object> objectArray) {
        ArrayList<String> strings = new ArrayList<>();
        for (Object object : objectArray) {
            strings.add(Objects.toString(object, null));
        }
        return strings;
    }
}
