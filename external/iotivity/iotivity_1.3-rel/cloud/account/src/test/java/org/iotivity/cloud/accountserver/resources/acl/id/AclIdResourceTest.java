
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

import static java.util.concurrent.TimeUnit.SECONDS;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.MongoDB;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.Observe;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.util.Cbor;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

public class AclIdResourceTest {
    private static final String           ACL_ID_URI         = Constants.ACL_ID_FULL_URI;
    final CountDownLatch                  mLatch             = new CountDownLatch(
            1);
    private final String                  mDeviceUuid        = "9cfbeb8e-5a1e-4d1c-9d01-2ae6fdb";
    private final String                  mOwnerUuid         = "123e4567-e89b-12d3-a456-4266554";
    private final String                  mSubjectUuid       = "72616E64-5069-6E44-6576-5575696";
    private final int                     mSubjectType       = 0;
    private final int                     mPermission        = 6;
    private final int                     mSubjectTypeUpdate = 1;
    private final int                     mPermissionUpdate  = 7;
    private final String                  mResourceUri       = "/a/light/0";
    private final String                  mResourceType      = "core.light";
    private final String                  mResourceIf        = "oic.if.baseline";
    private String                        mAceId             = "a0001";
    private String                        mAclId             = "987e6543-e21b-12d3-a456-4266554";
    private CoapDevice                    mMockDevice        = Mockito
            .mock(CoapDevice.class);
    private Cbor<HashMap<String, Object>> mCbor              = new Cbor<>();
    private IResponse                     mResponse          = null;
    private AclResource                   mAclResource       = new AclResource();

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        // reset data base
        resetDB();
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];
                System.out.println(
                        "\t----------payload : " + resp.getPayloadString());
                System.out.println("\t---------method : " + resp.getStatus());
                mResponse = resp;
                if (mAclId == null) {
                    HashMap<String, Object> payloadData = mCbor
                            .parsePayloadFromCbor(resp.getPayload(),
                                    HashMap.class);
                    if (payloadData.containsKey("aclid")) {
                        mAclId = (String) payloadData.get("aclid");
                    }
                }
                mLatch.countDown();
                return null;
            }
        }).when(mMockDevice).sendResponse(Mockito.anyObject());
    }

    @After
    public void resetAccountDatabase() throws Exception {
        MongoDB mongoDB = new MongoDB("127.0.0.1", Constants.DB_NAME);
        mongoDB.createTable(Constants.USER_TABLE);
        mongoDB.createTable(Constants.ACL_TABLE);
        mongoDB.createTable(Constants.TOKEN_TABLE);
        mongoDB.createTable(Constants.GROUP_TABLE);
    }

    @Test
    public void testCreateAcl() throws Exception {
        getTestMethodName();
        createAclId(mMockDevice, mDeviceUuid, mOwnerUuid);
        assertTrue(methodCheck(mResponse, ResponseStatus.CREATED));
        assertTrue(hashmapCheck(mResponse, "aclid"));
        hashmapGetAclId(mResponse, "aclid");
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testGetAclId() throws Exception {
        getTestMethodName();
        createAclId(mMockDevice, mDeviceUuid, mOwnerUuid);
        hashmapGetAclId(mResponse, "aclid");
        getAclId(mMockDevice, mDeviceUuid);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        assertTrue(hashmapCheck(mResponse, "aclid"));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testDeleteAclId() throws Exception {
        getTestMethodName();
        createAclId(mMockDevice, mDeviceUuid, mOwnerUuid);
        hashmapGetAclId(mResponse, "aclid");
        deleteAclId(mMockDevice, mAclId);
        assertTrue(methodCheck(mResponse, ResponseStatus.DELETED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testGetAclInfo() throws Exception {
        getTestMethodName();
        createAclId(mMockDevice, mDeviceUuid, mOwnerUuid);
        hashmapGetAclId(mResponse, "aclid");
        addIndividualAce(mMockDevice, mAclId);
        getAclInfo(mMockDevice, mAclId);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        assertTrue(hashmapCheck(mResponse, "aclid"));
        assertTrue(hashmapCheck(mResponse, "aclist"));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testAddIndividualAce() throws Exception {
        getTestMethodName();
        createAclId(mMockDevice, mDeviceUuid, mOwnerUuid);
        hashmapGetAclId(mResponse, "aclid");
        addIndividualAce(mMockDevice, mAclId);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testUpdateIndividualAce() throws Exception {
        getTestMethodName();
        createAclId(mMockDevice, mDeviceUuid, mOwnerUuid);
        hashmapGetAclId(mResponse, "aclid");
        addIndividualAce(mMockDevice, mAclId);
        hashmapGetAceId(mResponse, "aceid");
        updateIndividualAce(mMockDevice, mAclId, mAceId);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testDeleteIndividualAclAce() throws Exception {
        getTestMethodName();
        createAclId(mMockDevice, mDeviceUuid, mOwnerUuid);
        hashmapGetAclId(mResponse, "aclid");
        addIndividualAce(mMockDevice, mAclId);
        hashmapGetAceId(mResponse, "aceid");
        deleteIndividualAclAce(mMockDevice, mAclId, mAceId);
        assertTrue(methodCheck(mResponse, ResponseStatus.DELETED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    private void createAclId(CoapDevice device, String di, String oid)
            throws Exception {
        System.out.println("-----Create Acl Id");
        IRequest request = null;
        request = createAclIdRequest(oid, di);
        mAclResource.onDefaultRequestReceived(device, request);
    }

    private void getAclId(CoapDevice device, String di) throws Exception {
        System.out.println("-----Get Acl Id");
        IRequest request = null;
        request = getAclIdRequest(di);
        mAclResource.onDefaultRequestReceived(device, request);
    }

    private void deleteAclId(CoapDevice device, String aclId) throws Exception {
        System.out.println("-----Delete Acl Id");
        IRequest request = null;
        request = deleteAclIdRequest(aclId);
        mAclResource.onDefaultRequestReceived(device, request);
    }

    private void getAclInfo(CoapDevice device, String aclId) throws Exception {
        System.out.println("-----Get Acl Info");
        IRequest request = null;
        request = getIndividualAclRequest(aclId);
        mAclResource.onDefaultRequestReceived(device, request);
    }

    private void addIndividualAce(CoapDevice device, String aclId)
            throws Exception {
        System.out.println("-----Add Individual Ace");
        IRequest request = null;
        request = addIndividualAceRequest(aclId);
        mAclResource.onDefaultRequestReceived(device, request);
    }

    private void updateIndividualAce(CoapDevice device, String aclId,
            String aceId) throws Exception {
        System.out.println("-----Update Individual Acl Ace");
        IRequest request = null;
        request = updateIndividualAceRequest(aclId, aceId);
        mAclResource.onDefaultRequestReceived(device, request);
    }

    private void deleteIndividualAclAce(CoapDevice device, String aclId,
            String aceId) throws Exception {
        System.out.println("-----Delete Individual Acl Ace");
        IRequest request = null;
        request = deleteIndividualAclAceRequest(aclId, aceId);
        mAclResource.onDefaultRequestReceived(device, request);
    }

    private IRequest getAclIdRequest(String deviceUuid) {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.GET, ACL_ID_URI,
                "di=" + deviceUuid);
        ((CoapRequest) request).setObserve(Observe.SUBSCRIBE);
        return request;
    }

    private IRequest createAclIdRequest(String oid, String di) {
        IRequest request = null;
        String uriQuery = Constants.REQ_OWNER_ID + "=" + oid + ";"
                + Constants.REQ_DEVICE_ID + "=" + di;
        request = MessageBuilder.createRequest(RequestMethod.PUT, ACL_ID_URI,
                uriQuery);
        return request;
    }

    private IRequest deleteAclIdRequest(String aclId) {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.DELETE, ACL_ID_URI,
                "aclid=" + aclId);
        return request;
    }

    private IRequest getIndividualAclRequest(String aclId) {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.GET,
                ACL_ID_URI + "/" + aclId, null);
        ((CoapRequest) request).setObserve(Observe.NOTHING);
        return request;
    }

    private IRequest addIndividualAceRequest(String aclId) {

        HashMap<String, Object> resource = new HashMap<>();

        ArrayList<String> rtype = new ArrayList<>();
        rtype.add(mResourceType);

        ArrayList<String> rif = new ArrayList<>();
        rif.add(mResourceIf);

        resource.put(Constants.KEYFIELD_ACE_RESOURCE_HREF, mResourceUri);
        resource.put(Constants.KEYFIELD_ACE_RESOURCE_RT, rtype);
        resource.put(Constants.KEYFIELD_ACE_RESOURCE_IF, rif);

        HashMap<String, Object> acelist = new HashMap<>();
        acelist.put(Constants.KEYFIELD_ACE_SUBJECT_ID, mSubjectUuid);
        acelist.put(Constants.KEYFIELD_ACE_SUBJECT_TYPE, mSubjectType);
        acelist.put(Constants.KEYFIELD_ACE_PERMISSION, mPermission);
        acelist.put(Constants.KEYFIELD_ACE_RESOURCE, Arrays.asList(resource));

        HashMap<String, Object> payload = new HashMap<>();
        payload.put(Constants.REQ_ACL_LIST, Arrays.asList(acelist));

        return MessageBuilder.createRequest(RequestMethod.POST,
                ACL_ID_URI + "/" + aclId, null, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payload));
    }

    private IRequest updateIndividualAceRequest(String aclId, String aceId) {

        HashMap<String, Object> resource = new HashMap<>();

        ArrayList<String> rtype = new ArrayList<>();
        rtype.add(mResourceType);

        ArrayList<String> rif = new ArrayList<>();
        rif.add(mResourceIf);

        resource.put(Constants.KEYFIELD_ACE_RESOURCE_HREF, "/a/light/1");
        resource.put(Constants.KEYFIELD_ACE_RESOURCE_RT, rtype);
        resource.put(Constants.KEYFIELD_ACE_RESOURCE_IF, rif);

        HashMap<String, Object> acelist = new HashMap<>();
        acelist.put(Constants.KEYFIELD_ACE_SUBJECT_ID, mSubjectUuid);
        acelist.put(Constants.KEYFIELD_ACE_SUBJECT_TYPE, mSubjectTypeUpdate);
        acelist.put(Constants.KEYFIELD_ACE_PERMISSION, mPermissionUpdate);
        acelist.put(Constants.KEYFIELD_ACE_RESOURCE, Arrays.asList(resource));

        HashMap<String, Object> payload = new HashMap<>();
        payload.put(Constants.REQ_ACL_LIST, Arrays.asList(acelist));

        return MessageBuilder.createRequest(RequestMethod.POST,
                ACL_ID_URI + "/" + aclId, "aceid=" + aceId,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payload));
    }

    private IRequest deleteIndividualAclAceRequest(String aclId, String aceId) {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.DELETE,
                ACL_ID_URI + "/" + aclId, "aceid=" + aceId);
        return request;
    }

    private void hashmapGetAclId(IResponse response, String propertyName) {
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);
        if (payloadData.containsKey(propertyName)) {

            mAclId = (String) payloadData.get(propertyName);
            System.out.println("mAclId hashmap getacl " + mAclId);
        }
    }

    private void hashmapGetAceId(IResponse response, String propertyName) {
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);
        if (payloadData.containsKey("aclist")) {
            List<HashMap<String, Object>> aclist = (List<HashMap<String, Object>>) payloadData
                    .get("aclist");
            mAceId = (String) aclist.get(0).get(propertyName);
            System.out.println("hashmapGetAceId getace " + mAceId);
        }
    }

    private boolean hashmapCheck(IResponse response, String propertyName) {
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);
        if (payloadData.containsKey(propertyName)) {
            return true;
        } else {
            return false;
        }
    }

    private boolean methodCheck(IResponse response,
            ResponseStatus responseStatus) {
        if (responseStatus == response.getStatus()) {
            return true;
        } else {
            return false;
        }
    }

    private void getTestMethodName() {
        StackTraceElement[] stacks = new Throwable().getStackTrace();
        StackTraceElement currentStack = stacks[1];
        System.out.println("\t---Test Name : " + currentStack.getMethodName());
    }

    public void resetDB() throws Exception {
        MongoDB mongoDB = new MongoDB("127.0.0.1", Constants.DB_NAME);
        mongoDB.deleteTable(Constants.GROUP_TABLE);
        mongoDB.createTable(Constants.GROUP_TABLE);
        mongoDB.deleteTable(Constants.ACL_TABLE);
        mongoDB.createTable(Constants.ACL_TABLE);
        mongoDB.deleteTable(Constants.USER_TABLE);
        mongoDB.createTable(Constants.USER_TABLE);
        mongoDB.deleteTable(Constants.TOKEN_TABLE);
        mongoDB.createTable(Constants.TOKEN_TABLE);
    }
}
