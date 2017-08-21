package org.iotivity.cloud.accountserver.resources.acl.group;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.ListIterator;
import java.util.UUID;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.db.GroupTable;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.util.Cbor;
import org.iotivity.cloud.util.Log;

/**
 * This class provides a set of APIs manage a group broker
 */
public class GroupBrokerManager {

    private static GroupBrokerManager                   mGroupBrokerMgr    = new GroupBrokerManager();
    private TypeCastingManager<GroupTable>              mTypeGroup         = new TypeCastingManager<>();
    private HashMap<String, Object>                     mGroupResourceInfo = new HashMap<>();

    private HashMap<String, ArrayList<GroupSubscriber>> mGroupSubscriber   = new HashMap<>();

    private Cbor<HashMap<String, Object>>               mCbor              = new Cbor<>();

    private GroupPolicyManager                          mGroupPolicyMgr    = new GroupPolicyManager();

    private GroupBrokerManager() {
        mGroupResourceInfo.put(Constants.RS_RESOURCE_TYPE,
                Constants.ACL_RESOURCE_TYPE);
        mGroupResourceInfo.put(Constants.RS_INTERFACE,
                Constants.DEFAULT_INTERFACE);
    }

    /**
     * Function to get GroupBrokerManager as a singleton
     * 
     * @return GroupBrokerManager as a singleton
     */
    public static GroupBrokerManager getInstance() {
        return mGroupBrokerMgr;
    }

    private static class GroupSubscriber {
        GroupSubscriber(Device subscriber, IRequest request) {
            mSubscriber = subscriber;
            mRequest = request;
        }

        public Device   mSubscriber;
        public IRequest mRequest;
    }

    /**
     * API to verify user's Authorization
     * 
     * @param uid
     *            unique user id
     * @param gid
     *            group id
     * @param properties
     *            payload properties
     * @param op
     *            operation
     */
    public void verifyAuthorization(String uid, String gid,
            ArrayList<String> properties, UserOperation op) {

        if (gid == null) {
            return;
        }

        mGroupPolicyMgr.verifyOperationAuthorization(gid, uid, op, properties);
    }

    /**
     * API to get Group information
     * 
     * @param uid
     *            unique user id
     * @return response message
     */
    public HashMap<String, Object> getGroupList(String uid) {

        return makeGetResponse(uid);

    }

    private HashMap<String, Object> makeGetResponse(String uid) {

        ArrayList<HashMap<String, Object>> records = readGroupInfo(
                Constants.KEYFIELD_GROUP_MEMBERS, uid);

        HashMap<String, Object> response = new HashMap<>();
        // put group info
        response.putAll(mGroupResourceInfo);
        if (!records.isEmpty()) {
            response.put(Constants.RESP_GROUPS, records);
        }

        Log.d("Group get response : " + response.toString());
        return response;
    }

    private ArrayList<HashMap<String, Object>> readGroupInfo(String key,
            String value) {
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(key, value);
        ArrayList<HashMap<String, Object>> records = AccountDBManager
                .getInstance().selectRecord(Constants.GROUP_TABLE, condition);
        return records;
    }

    /**
     * API to create a group
     * 
     * @param uid
     *            unique user id
     * @param gid
     *            group id
     * @param gname
     *            group name
     * @param parent
     *            parent group id
     * @return response message
     */
    public HashMap<String, Object> createGroup(String uid, String gid,
            String gname, String parent) {

        if (gname == null) {
            gname = "DEFAULT_GNAME";
        }

        if (gid == null) {
            gid = UUID.randomUUID().toString();
        }

        if (parent != null) {
            // parent update
            HashMap<String, Object> condition = new HashMap<>();
            condition.put(Constants.KEYFIELD_GID, parent);
            HashMap<String, Object> record = AccountDBManager.getInstance()
                    .selectOneRecord(Constants.GROUP_TABLE, condition);

            if (record.isEmpty()) {
                throw new BadRequestException("parent group is null");
            }
            ArrayList<String> subgroups = (ArrayList<String>) record
                    .get(Constants.KEYFIELD_GROUP_SUBGROUPS);
            if (subgroups == null) {
                subgroups = new ArrayList<String>();
            }
            subgroups.add(gid);
            record.put(Constants.KEYFIELD_GROUP_SUBGROUPS, subgroups);
            AccountDBManager.getInstance()
                    .insertAndReplaceRecord(Constants.GROUP_TABLE, record);
        }

        storeGroupInfo(uid, gid, gname, parent);
        return makePostResponse(uid, gid, gname, parent);
    }

    private HashMap<String, Object> makePostResponse(String uid, String gid,
            String gname, String parent) {

        HashMap<String, Object> response = new HashMap<>();
        response.put(Constants.KEYFIELD_GID, gid);
        response.put(Constants.KEYFIELD_GROUP_NAME, gname);
        response.put(Constants.KEYFIELD_GROUP_OWNER, uid);
        response.put(Constants.KEYFIELD_GROUP_MEMBERS, Arrays.asList(uid));
        if (parent != null) {
            response.put(Constants.KEYFIELD_GROUP_PARENT, parent);
        }

        Log.d("Group post response : " + response.toString());

        return response;
    }

    private void storeGroupInfo(String uid, String gid, String gname,
            String parent) {
        GroupTable groupTable = new GroupTable();
        groupTable.setGid(gid);
        groupTable.setGname(gname);
        groupTable.setOwner(uid);
        ArrayList<String> members = new ArrayList<>();
        members.add(uid);
        groupTable.setMembers(members);
        if (parent != null) {
            groupTable.setParent(parent);
        }

        HashMap<String, Object> groupMap = mTypeGroup
                .convertObjectToMap(groupTable);
        groupMap.putAll(mGroupResourceInfo);
        AccountDBManager.getInstance().insertRecord(Constants.GROUP_TABLE,
                groupMap);

    }

    /**
     * API to add a observer
     * 
     * @param uid
     *            unique user id
     * @param srcDevice
     *            channel information
     * @param request
     *            request message
     */
    public void addObserver(String uid, Device srcDevice, IRequest request) {
        ArrayList<GroupSubscriber> subscribers = mGroupSubscriber.get(uid);
        if (subscribers == null) {
            mGroupSubscriber.put(uid, new ArrayList<GroupSubscriber>(
                    Arrays.asList(new GroupSubscriber(srcDevice, request))));
            return;
        }
        // TODO control consecutive GET observe requests from single device
        // removeObserverFromSubscriberList(uid, srcDevice, request);
        GroupSubscriber subscriber = new GroupSubscriber(srcDevice, request);
        subscribers.add(subscriber);
        mGroupSubscriber.put(uid, subscribers);
    }

    /**
     * API to remove a observer
     * 
     * @param uid
     *            unique user id
     * @param srcDevice
     *            channel information
     * @param request
     *            request message
     */
    public void removeObserver(String uid, Device srcDevice, IRequest request) {
        removeObserverFromSubscriberList(uid, srcDevice, request);
    }

    private void removeObserverFromSubscriberList(String uid, Device srcDevice,
            IRequest request) {
        ArrayList<GroupSubscriber> subscribers = mGroupSubscriber.get(uid);
        if (subscribers != null) {
            ListIterator<GroupSubscriber> iterator = subscribers.listIterator();
            while (iterator.hasNext()) {
                GroupSubscriber subscriber = iterator.next();
                CoapRequest coapRequest = (CoapRequest) subscriber.mRequest;
                if (srcDevice.equals(subscriber.mSubscriber)
                        && coapRequest.getTokenString().equals(
                                ((CoapRequest) request).getTokenString())) {
                    iterator.remove();
                    Log.d("subscriber removed, "
                            + ((CoapRequest) request).getTokenString());
                }
            }
            mGroupSubscriber.put(uid, subscribers);
        }
    }

    /**
     * API to notify to observers
     * 
     * @param uidList
     *            unique user id list
     */
    public void notifyToObservers(ArrayList<String> uidList) {

        if (uidList == null) {
            return;
        }
        for (String uid : uidList) {
            ArrayList<GroupSubscriber> subscribers = mGroupSubscriber.get(uid);
            if (subscribers != null) {
                ArrayList<String> subscriberTokenList = new ArrayList<>();
                for (GroupSubscriber subscriber : subscribers) {
                    subscriberTokenList.add(((CoapRequest) subscriber.mRequest)
                            .getTokenString());
                    subscriber.mSubscriber.sendResponse(
                            MessageBuilder.createResponse(subscriber.mRequest,
                                    ResponseStatus.CONTENT,
                                    ContentFormat.APPLICATION_CBOR,
                                    mCbor.encodingPayloadToCbor(
                                            makeGetResponse(uid))));
                }
                Log.d("subscriber : " + uid
                        + " , subscriber internal Token list : "
                        + subscriberTokenList);
            }
        }
    }
}
