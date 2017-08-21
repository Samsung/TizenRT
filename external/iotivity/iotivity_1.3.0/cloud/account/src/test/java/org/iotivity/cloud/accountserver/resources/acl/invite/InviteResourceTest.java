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

import static java.util.concurrent.TimeUnit.SECONDS;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.mock;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.MongoDB;
import org.iotivity.cloud.accountserver.resources.acl.group.GroupResource;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.exception.ServerException.PreconditionFailedException;
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
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

public class InviteResourceTest {
    private static final String           INVITE_URI        = Constants.INVITE_FULL_URI;
    private static final String           GROUP_URI         = Constants.GROUP_FULL_URI;

    private static final String           TEST_INVITE_USER  = "u0001";
    private static final String           TEST_INVITED_USER = "u0002";
    private static final String           TEST_GROUP_ID     = "g0001";

    private String                        mInvitedGroupId   = null;

    private InviteResource                mInviteResource   = null;

    private GroupResource                 mGroupResource    = new GroupResource();

    private CountDownLatch                mLatch            = null;

    private CoapDevice                    mMockDevice       = null;
    private IResponse                     mResponse         = null;

    private Cbor<HashMap<String, Object>> mCbor             = new Cbor<>();

    @Before
    public void setUp() throws Exception {

        mInviteResource = new InviteResource();

        mLatch = new CountDownLatch(1);

        mMockDevice = Mockito.mock(CoapDevice.class);

        mResponse = null;

        // reset data base
        MongoDB mongoDB = new MongoDB("127.0.0.1", Constants.DB_NAME);
        mongoDB.deleteTable(Constants.INVITE_TABLE);
        mongoDB.createTable(Constants.INVITE_TABLE);

        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];
                mResponse = resp;

                if (resp.getPayloadSize() != 0) {

                    HashMap<String, Object> payloadData = mCbor
                            .parsePayloadFromCbor(resp.getPayload(),
                                    HashMap.class);

                    if (payloadData.containsKey(Constants.REQ_GROUP_ID)) {
                        mInvitedGroupId = (String) payloadData
                                .get(Constants.REQ_GROUP_ID);
                    }
                }

                mLatch.countDown();

                return resp;
            }
        }).when(mMockDevice).sendResponse(Mockito.anyObject());

    }

    @Test
    public void testSendInvitation() throws Exception {

        sendInvitation(TEST_GROUP_ID, TEST_INVITED_USER);

        assertTrue(mLatch.await(1L, SECONDS));
        assertEquals(mResponse.getStatus(), ResponseStatus.CHANGED);
    }

    @Test(expected = PreconditionFailedException.class)
    public void testSendInvitationWithoutGid() throws Exception {

        sendInvitation(null, TEST_INVITED_USER);
    }

    @Test(expected = PreconditionFailedException.class)
    public void testSendInvitationWithoutMid() throws Exception {

        sendInvitation(TEST_GROUP_ID, null);
    }

    @Test
    public void testInviteUserObserveInvition() throws Exception {

        CountDownLatch latchInviteUser = new CountDownLatch(2);

        CoapDevice mockInviteUser = mock(CoapDevice.class);

        // invite user callback
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];

                latchInviteUser.countDown();

                // check observe notification response
                if (latchInviteUser.getCount() == 0) {

                    assertEquals(resp.getStatus(), ResponseStatus.CONTENT);
                    assertTrue(
                            checkResponsePayload(resp, Constants.RESP_INVITE));
                    assertTrue(checkInviteInfo(resp.getPayload(),
                            Constants.RESP_INVITE, TEST_GROUP_ID,
                            TEST_INVITED_USER));
                }

                return resp;
            }
        }).when(mockInviteUser).sendResponse(Mockito.anyObject());

        getInvitation(TEST_INVITE_USER, mockInviteUser, Observe.SUBSCRIBE);
        sendInvitation(TEST_GROUP_ID, TEST_INVITED_USER);

        assertTrue(latchInviteUser.await(2L, SECONDS));
    }

    @Test
    public void testInvitedUserObserveInvitation() throws Exception {

        CountDownLatch latchInvitedUser = new CountDownLatch(2);

        CoapDevice mockInvitedUser = mock(CoapDevice.class);

        // invited user callback
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];

                latchInvitedUser.countDown();

                // check observe notification response
                if (latchInvitedUser.getCount() == 0) {

                    assertEquals(resp.getStatus(), ResponseStatus.CONTENT);
                    assertTrue(
                            checkResponsePayload(resp, Constants.RESP_INVITED));
                    assertTrue(checkInviteInfo(resp.getPayload(),
                            Constants.RESP_INVITED, TEST_GROUP_ID,
                            TEST_INVITE_USER));
                }

                return resp;
            }
        }).when(mockInvitedUser).sendResponse(Mockito.anyObject());

        getInvitation(TEST_INVITED_USER, mockInvitedUser, Observe.SUBSCRIBE);
        sendInvitation(TEST_GROUP_ID, TEST_INVITED_USER);

        assertTrue(latchInvitedUser.await(2L, SECONDS));
    }

    @Test
    public void testInviteUserCancelObserveInvitation() throws Exception {

        getInvitation(TEST_INVITE_USER, mMockDevice, Observe.SUBSCRIBE);
        sendInvitation(TEST_GROUP_ID, TEST_INVITED_USER);

        getInvitation(TEST_INVITE_USER, mMockDevice, Observe.UNSUBSCRIBE);

        assertEquals(mResponse.getStatus(), ResponseStatus.CONTENT);
        assertTrue(checkInviteInfo(mResponse.getPayload(),
                Constants.RESP_INVITE, TEST_GROUP_ID, TEST_INVITED_USER));
    }

    @Test
    public void testInvitedUserCancelObserveInvitation() throws Exception {

        getInvitation(TEST_INVITED_USER, mMockDevice, Observe.SUBSCRIBE);
        sendInvitation(TEST_GROUP_ID, TEST_INVITED_USER);

        getInvitation(TEST_INVITED_USER, mMockDevice, Observe.UNSUBSCRIBE);

        assertEquals(mResponse.getStatus(), ResponseStatus.CONTENT);
        assertTrue(checkInviteInfo(mResponse.getPayload(),
                Constants.RESP_INVITED, TEST_GROUP_ID, TEST_INVITE_USER));
    }

    @Test
    public void testInviteUserGetInvitation() throws Exception {

        sendInvitation(TEST_GROUP_ID, TEST_INVITED_USER);

        getInvitation(TEST_INVITE_USER, mMockDevice, Observe.NOTHING);

        assertEquals(mResponse.getStatus(), ResponseStatus.CONTENT);
        assertTrue(checkInviteInfo(mResponse.getPayload(),
                Constants.RESP_INVITE, TEST_GROUP_ID, TEST_INVITED_USER));
    }

    @Test
    public void testInvitedUserGetInvitation() throws Exception {

        sendInvitation(TEST_GROUP_ID, TEST_INVITED_USER);

        getInvitation(TEST_INVITED_USER, mMockDevice, Observe.NOTHING);

        assertEquals(mResponse.getStatus(), ResponseStatus.CONTENT);
        assertTrue(checkInviteInfo(mResponse.getPayload(),
                Constants.RESP_INVITED, TEST_GROUP_ID, TEST_INVITE_USER));
    }

    @Test
    public void testDeleteInvitationForAccept() throws Exception {

        createGroup(mMockDevice, TEST_INVITE_USER, "Public", null);
        sendInvitation(mInvitedGroupId, TEST_INVITED_USER);

        deleteInvitationWithQuery(mInvitedGroupId, TEST_INVITED_USER, true);

        assertTrue(mLatch.await(1L, SECONDS));
        assertEquals(mResponse.getStatus(), ResponseStatus.DELETED);

    }

    @Test
    public void testDeleteInvitationForDeny() throws Exception {

        createGroup(mMockDevice, TEST_INVITE_USER, "Public", null);
        sendInvitation(mInvitedGroupId, TEST_INVITED_USER);

        deleteInvitationWithQuery(mInvitedGroupId, TEST_INVITED_USER, false);

        assertTrue(mLatch.await(1L, SECONDS));
        assertEquals(mResponse.getStatus(), ResponseStatus.DELETED);
    }

    @Test
    public void testCancelInvitation() throws Exception {

        sendInvitation(TEST_GROUP_ID, TEST_INVITED_USER);

        deleteInvitation(TEST_GROUP_ID, TEST_INVITE_USER, TEST_INVITED_USER);

        assertTrue(mLatch.await(1L, SECONDS));
        assertEquals(mResponse.getStatus(), ResponseStatus.DELETED);
    }

    private void sendInvitation(String gid, String mid) {

        IRequest request = makeSendInvitationRequest(gid, mid);

        mInviteResource.onDefaultRequestReceived(mMockDevice, request);
    }

    private void deleteInvitation(String gid, String uid, String mid) {

        String uriQuery = Constants.REQ_GROUP_ID + "=" + gid + ";"
                + Constants.REQ_UUID_ID + "=" + uid + ";" + Constants.REQ_MEMBER
                + "=" + mid;

        IRequest request = MessageBuilder.createRequest(RequestMethod.DELETE,
                INVITE_URI, uriQuery);

        mInviteResource.onDefaultRequestReceived(mMockDevice, request);
    }

    private void getInvitation(String uid, CoapDevice device,
            Observe observeOpt) {

        String uriQuery = Constants.REQ_UUID_ID + "=" + uid;
        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                INVITE_URI, uriQuery);

        if (observeOpt.equals(Observe.NOTHING) == false) {
            ((CoapRequest) request).setObserve(observeOpt);
        }

        mInviteResource.onDefaultRequestReceived(device, request);
    }

    private IRequest makeSendInvitationRequest(String gid, String mid) {

        HashMap<String, Object> invitation = new HashMap<>();
        invitation.put(Constants.REQ_GROUP_ID, gid);
        invitation.put(Constants.REQ_MEMBER, mid);

        HashMap<String, Object> payload = new HashMap<>();
        payload.put(Constants.REQ_UUID_ID, TEST_INVITE_USER);
        payload.put(Constants.REQ_INVITE, Arrays.asList(invitation));

        return MessageBuilder.createRequest(RequestMethod.POST, INVITE_URI,
                null, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payload));
    }

    private boolean checkResponsePayload(IResponse response,
            String propertyName) {

        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);

        return payloadData.get(propertyName) != null ? true : false;
    }

    private boolean checkInviteInfo(byte[] payload, String propertyKey,
            String gid, String mid) {

        HashMap<String, Object> paredPayload = mCbor
                .parsePayloadFromCbor(payload, HashMap.class);

        @SuppressWarnings("unchecked")
        ArrayList<HashMap<String, Object>> invitationList = (ArrayList<HashMap<String, Object>>) paredPayload
                .get(propertyKey);

        for (HashMap<String, Object> invitation : invitationList) {

            if (invitation.get(Constants.REQ_GROUP_ID).equals(gid)
                    && invitation.get(Constants.REQ_MEMBER).equals(mid)) {

                return true;
            }
        }

        return false;
    }

    private void deleteInvitationWithQuery(String gid, String uid,
            boolean accept) {

        int acceptInt = 0;

        if (accept)
            acceptInt = 1;

        String uriQuery = Constants.REQ_GROUP_ID + "=" + gid + ";"
                + Constants.REQ_UUID_ID + "=" + uid + ";"
                + Constants.REQ_INVITE_ACCEPT + "=" + acceptInt;

        IRequest request = MessageBuilder.createRequest(RequestMethod.DELETE,
                INVITE_URI, uriQuery);

        mInviteResource.onDefaultRequestReceived(mMockDevice, request);
    }

    private void createGroup(CoapDevice device, String uid, String gname,
            String parent) throws Exception {
        System.out.println("-----Create Group");
        IRequest request = null;
        HashMap<String, Object> payloadData = new HashMap<String, Object>();
        payloadData.put(Constants.KEYFIELD_GROUP_NAME, gname);
        payloadData.put(Constants.KEYFIELD_GROUP_PARENT, parent);
        payloadData.put(Constants.KEYFIELD_GROUP_MEMBERS,
                new ArrayList<String>(Arrays.asList(uid)));
        payloadData.put(Constants.KEYFIELD_OID, uid);
        request = MessageBuilder.createRequest(RequestMethod.POST, GROUP_URI,
                Constants.KEYFIELD_UID + "=" + uid,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        mGroupResource.onDefaultRequestReceived(device, request);
    }
}