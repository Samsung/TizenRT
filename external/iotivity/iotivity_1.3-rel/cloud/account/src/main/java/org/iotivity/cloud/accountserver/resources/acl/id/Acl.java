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
package org.iotivity.cloud.accountserver.resources.acl.id;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;
import java.util.UUID;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.db.AclTable;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.exception.ServerException.InternalServerErrorException;
import org.iotivity.cloud.base.exception.ServerException.UnAuthorizedException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.util.Cbor;
import org.iotivity.cloud.util.Log;

public class Acl {

    private Cbor<HashMap<String, Object>> mCbor    = new Cbor<>();
    private String                        mAclid   = null;
    private String                        mOid     = null;
    private String                        mDi      = null;
    private TypeCastingManager<AclTable>  mTypeAcl = new TypeCastingManager<>();

    public Acl(String aclid) {
        mAclid = aclid;
    }

    public Acl(String aclid, String oid, String di) {
        mAclid = aclid;
        mOid = oid;
        mDi = di;
    }

    private static class AclSubscriber {
        AclSubscriber(Device subscriber, IRequest request) {
            mSubscriber = subscriber;
            mRequest = request;
        }

        public Device   mSubscriber;
        public IRequest mRequest;
    }

    private HashMap<String, AclSubscriber> mSubscribers = new HashMap<>();

    public static String valueOf(Object object) {
        return (object == null) ? "" : object.toString();
    }

    @SuppressWarnings("unchecked")
    public static AclTable convertMaptoAclObject(
            HashMap<String, Object> aclMap) {
        AclTable aclTable = new AclTable();
        try {
            aclTable.setAclid(valueOf(aclMap.get(Constants.KEYFIELD_ACLID)));
            aclTable.setDi(valueOf(aclMap.get(Constants.KEYFIELD_DI)));
            aclTable.setOid(valueOf(aclMap.get(Constants.REQ_OWNER_ID)));
            aclTable.setRowneruuid(
                    valueOf(aclMap.get(Constants.REQ_ROWNER_ID)));

            List<Ace> aceList = new ArrayList<Ace>();

            List<HashMap<String, Object>> aclist = (List<HashMap<String, Object>>) aclMap
                    .get(Constants.REQ_ACL_LIST);

            if (aclist == null) {
                return aclTable;
            }

            for (HashMap<String, Object> eachAce : aclist) {

                Ace ace = new Ace();

                ace.setAceid(valueOf(eachAce.get(Constants.REQ_ACE_ID)));
                ace.setSubjectuuid(valueOf(
                        eachAce.get(Constants.KEYFIELD_ACE_SUBJECT_ID)));
                ace.setStype(Integer.valueOf(eachAce
                        .get(Constants.KEYFIELD_ACE_SUBJECT_TYPE).toString()));
                ace.setPermission(Integer.valueOf(eachAce
                        .get(Constants.KEYFIELD_ACE_PERMISSION).toString()));

                Object validity = eachAce.get(Constants.KEYFIELD_ACE_VALIDITY);
                if (validity != null) {
                    ace.setValidity((List<String>) validity);
                }

                List<AceResource> resourceLst = new ArrayList<AceResource>();
                List<HashMap<String, Object>> resourceList = (List<HashMap<String, Object>>) eachAce
                        .get(Constants.KEYFIELD_ACE_RESOURCE);
                for (HashMap<String, Object> resrouce : resourceList) {

                    AceResource aceResource = new AceResource();
                    aceResource.setHref(valueOf(resrouce
                            .get(Constants.KEYFIELD_ACE_RESOURCE_HREF)));
                    List<String> rtList = (List<String>) resrouce
                            .get(Constants.KEYFIELD_ACE_RESOURCE_RT);
                    aceResource.setRt(rtList);
                    List<String> ifList = (List<String>) resrouce
                            .get(Constants.KEYFIELD_ACE_RESOURCE_IF);
                    aceResource.setOicif(ifList);

                    resourceLst.add(aceResource);
                }
                ace.setResources(resourceLst);

                aceList.add(ace);

            }
            aclTable.setAclist(aceList);
        } catch (Exception e) {
            throw new InternalServerErrorException(
                    "Map to Acl Object casting error " + e.getMessage());
        }
        return aclTable;

    }

    @SuppressWarnings("unchecked")

    public List<HashMap<String, Object>> addACE(
            List<HashMap<String, Object>> aclist) {
        Log.v("IN addACE");
        HashMap<String, Object> hashmap = AccountDBManager.getInstance()
                .selectRecord(Constants.ACL_TABLE, getCondition()).get(0);
        if (hashmap == null) {
            throw new BadRequestException("aclid is invalid");
        }
        List<HashMap<String, Object>> aclDbList = (List<HashMap<String, Object>>) hashmap
                .get(Constants.REQ_ACL_LIST);

        ListIterator<HashMap<String, Object>> iterator = aclist.listIterator();
        while (iterator.hasNext()) {
            HashMap<String, Object> aceMap = iterator.next();
            if (aceMap.get(Constants.KEYFIELD_ACE_SUBJECT_ID)
                    .equals(hashmap.get(Constants.REQ_OWNER_ID))) {
                // remove current iterator
                iterator.remove();
                continue;
            }
            aceMap.put(Constants.REQ_ACE_ID, UUID.randomUUID().toString());
        }

        List<HashMap<String, Object>> newAcList = new ArrayList<HashMap<String, Object>>(
                aclist);

        if (aclDbList != null) {
            newAcList.addAll(aclDbList);
        }
        hashmap.put(Constants.REQ_ACL_LIST, newAcList);
        AccountDBManager.getInstance().updateRecord(Constants.ACL_TABLE,
                hashmap);
        notifyToSubscriber(getResponsePayload(true));
        Log.v("OUT addACE");
        return aclist;
    }

    public HashMap<String, Object> getACE(String aceid) {
        HashMap<String, Object> hashmap = AccountDBManager.getInstance()
                .selectRecord(Constants.ACL_TABLE, getCondition()).get(0);
        if (hashmap == null) {
            throw new BadRequestException("aclid is invalid");
        }

        List<HashMap<String, Object>> aclDbList = (List<HashMap<String, Object>>) hashmap
                .get(Constants.REQ_ACL_LIST);
        ListIterator<HashMap<String, Object>> iterator = aclDbList
                .listIterator();
        while (iterator.hasNext()) {
            HashMap<String, Object> aceMap = iterator.next();
            if (aceMap.get(Constants.REQ_ACE_ID).equals(aceid)) {
                // Return the current element from the iterator
                return aceMap;
            }
        }
        throw new BadRequestException("aceid is invalid");
    }

    public boolean isValidAceId(String aceid) {
        HashMap<String, Object> hashmap = AccountDBManager.getInstance()
                .selectRecord(Constants.ACL_TABLE, getCondition()).get(0);
        if (hashmap == null) {
            return false;
        }

        List<HashMap<String, Object>> aclDbList = (List<HashMap<String, Object>>) hashmap
                .get(Constants.REQ_ACL_LIST);
        ListIterator<HashMap<String, Object>> iterator = aclDbList
                .listIterator();
        while (iterator.hasNext()) {
            HashMap<String, Object> aceMap = iterator.next();
            if (aceMap.get(Constants.REQ_ACE_ID).equals(aceid)) {
                return true;
            }
        }
        return false;
    }

    public void updateACE(String aceid, HashMap<String, Object> ace) {
        Log.v("IN updateACE");

        HashMap<String, Object> hashmap = AccountDBManager.getInstance()
                .selectRecord(Constants.ACL_TABLE, getCondition()).get(0);

        List<HashMap<String, Object>> aclDbList = (List<HashMap<String, Object>>) hashmap
                .get(Constants.REQ_ACL_LIST);

        ace.put(Constants.REQ_ACE_ID, aceid);

        ListIterator<HashMap<String, Object>> iterator = aclDbList
                .listIterator();
        while (iterator.hasNext()) {
            HashMap<String, Object> aceMap = iterator.next();
            if (aceMap.get(Constants.REQ_ACE_ID).equals(aceid)) {
                // replace current iterator with new element
                iterator.set(ace);
                break;
            }
        }

        hashmap.put(Constants.REQ_ACL_LIST, aclDbList);
        AccountDBManager.getInstance().updateRecord(Constants.ACL_TABLE,
                hashmap);
        notifyToSubscriber(getResponsePayload(true));
        Log.v("OUT updateACE");

    }

    public void deleteACE(String aceid) {

        HashMap<String, Object> hashmap = AccountDBManager.getInstance()
                .selectRecord(Constants.ACL_TABLE, getCondition()).get(0);

        List<HashMap<String, Object>> aclDbList = (List<HashMap<String, Object>>) hashmap
                .get(Constants.REQ_ACL_LIST);

        ListIterator<HashMap<String, Object>> iterator = aclDbList
                .listIterator();
        while (iterator.hasNext()) {
            HashMap<String, Object> aceMap = iterator.next();
            if (aceMap.get(Constants.REQ_ACE_ID).equals(aceid)) {
                // Remove the current element from the iterator
                iterator.remove();
                break;
            }
        }

        hashmap.put(Constants.REQ_ACL_LIST, aclDbList);
        AccountDBManager.getInstance().updateRecord(Constants.ACL_TABLE,
                hashmap);
        notifyToSubscriber(getResponsePayload(true));
    }

    public void deleteAclist() {
        AclTable aclTable = getAclTable();
        aclTable.setAclist(null);
        AccountDBManager.getInstance().updateRecord(Constants.ACL_TABLE,
                mTypeAcl.convertObjectToMap(aclTable));
        notifyToSubscriber(getResponsePayload(true));
    }

    public HashMap<String, Object> getInfo() {
        return getResponsePayload(true);
    }

    public HashMap<String, Object> addSubscriber(String di, Device subscriber,
            IRequest request) {

        verifyAclTableDi(di);
        AclSubscriber newSubscriber = new AclSubscriber(subscriber, request);
        mSubscribers.put(di, newSubscriber);
        return getInfo();
    }

    public HashMap<String, Object> removeSubscriber(String di) {

        HashMap<String, Object> responsePayload = getResponsePayload(true);
        if (mSubscribers.containsKey(di)) {
            mSubscribers.remove(di);
        }
        return responsePayload;
    }

    private void verifyAclTableDi(String di) {
        AclTable aclTable = getAclTable();
        if (aclTable.getDi() == null) {
            throw new BadRequestException("di is invalid in Acl");
        }
        String mDi = aclTable.getDi();
        if (!mDi.equals(di)) {
            throw new UnAuthorizedException(
                    di + "is not Device ID of this ACL");
        }
    }

    private void notifyToSubscriber(
            HashMap<String, Object> notifyBytePayloadData) {
        synchronized (mSubscribers) {

            Iterator<String> iterator = mSubscribers.keySet().iterator();
            while (iterator.hasNext()) {
                String key = iterator.next();
                AclSubscriber aclSubscriber = mSubscribers.get(key);
                aclSubscriber.mSubscriber.sendResponse(
                        MessageBuilder.createResponse(aclSubscriber.mRequest,
                                ResponseStatus.CONTENT,
                                ContentFormat.APPLICATION_CBOR,
                                mCbor.encodingPayloadToCbor(
                                        notifyBytePayloadData)));
            }
        }
    }

    private AclTable getAclTable() {
        AclTable getAclTable = new AclTable();
        getAclTable = convertMaptoAclObject(AccountDBManager.getInstance()
                .selectRecord(Constants.ACL_TABLE, getCondition()).get(0));
        return getAclTable;
    }

    private HashMap<String, Object> getAclTablePayLoad() {
        HashMap<String, Object> aclPayload = new HashMap<>();
        aclPayload = AccountDBManager.getInstance()
                .selectRecord(Constants.ACL_TABLE, getCondition()).get(0);
        return aclPayload;
    }

    private HashMap<String, Object> getResponsePayload(boolean isAliveAcl) {
        return isAliveAcl ? getAclTablePayLoad() : null;
    }

    private HashMap<String, Object> getCondition() {
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.REQ_ACL_ID, mAclid);
        return condition;
    }

}
