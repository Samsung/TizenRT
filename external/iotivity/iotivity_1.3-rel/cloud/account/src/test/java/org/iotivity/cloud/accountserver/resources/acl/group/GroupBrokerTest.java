package org.iotivity.cloud.accountserver.resources.acl.group;

import static java.util.concurrent.TimeUnit.SECONDS;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.mock;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.MongoDB;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.exception.ServerException;
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
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

public class GroupBrokerTest {

    private String                        GROUP_URI         = Constants.GROUP_FULL_URI;
    private String                        UID_QUERY         = Constants.KEYFIELD_UID
            + "=";
    private String                        mGid1             = "g1";
    private String                        mGid2             = "g2";
    private String                        mGname1           = "myHome";
    private String                        mGname2           = "myroom";
    private String                        mDi1              = "d1";
    private String                        mDi2              = "d2";
    private String                        mDi3              = "d3";
    private String                        mUid1             = "u1";

    CountDownLatch                        mLatch            = new CountDownLatch(
            1);
    private CoapDevice                    mMockDevice       = Mockito
            .mock(CoapDevice.class);
    private Cbor<HashMap<String, Object>> mCbor             = new Cbor<>();
    private IResponse                     mResponse         = null;
    private IResponse                     mResponseObserver = null;
    private GroupResource                 mGroupResource    = new GroupResource();
    private HashMap<String, Object>       mPayload          = new HashMap<>();
    private GroupResource                 mGResource        = new GroupResource();

    @Before
    public void setUp() throws Exception {
        mResponse = null;
        mMockDevice = mock(CoapDevice.class);
        mLatch = new CountDownLatch(1);
        mGResource = new GroupResource();
        // callback mock
        resetDB();
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                CoapResponse resp = (CoapResponse) invocation.getArguments()[0];
                mLatch.countDown();
                mResponse = resp;
                return resp;
            }
        }).when(mMockDevice).sendResponse(Mockito.anyObject());
    }

    @After
    public void resetAccountDatabase() throws Exception {
        MongoDB mongoDB = new MongoDB("127.0.0.1", Constants.DB_NAME);
        mongoDB.createTable(Constants.USER_TABLE);
        mongoDB.createTable(Constants.TOKEN_TABLE);
        mongoDB.createTable(Constants.GROUP_TABLE);
        mongoDB.createTable(Constants.ACL_TABLE);
        // mongoDB.createTable(Constants.ACE_TABLE);
    }

    @Test
    public void testCreateGroup() throws Exception {
        getTestMethodName();
        sendCreateGroupRequest(mMockDevice, mUid1, mGname1, null);
        assertTrue(responseCodeCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(checkProperty(mResponse, Constants.KEYFIELD_GID));
        assertTrue(checkProperty(mResponse, Constants.KEYFIELD_GROUP_NAME));
        assertTrue(checkProperty(mResponse, Constants.KEYFIELD_GROUP_OWNER));
        assertTrue(checkProperty(mResponse, Constants.KEYFIELD_GROUP_MEMBERS));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testCreateSubGroup() throws Exception {
        getTestMethodName();
        sendCreateGroupRequest(mMockDevice, mUid1, mGname1, null);
        String gid = getProperty(mResponse, Constants.KEYFIELD_GID).toString();
        sendCreateGroupRequest(mMockDevice, mUid1, null, gid);
        assertTrue(responseCodeCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(checkProperty(mResponse, Constants.KEYFIELD_GID));
        assertTrue(checkProperty(mResponse, Constants.KEYFIELD_GROUP_NAME));
        assertTrue(checkProperty(mResponse, Constants.KEYFIELD_GROUP_OWNER));
        assertTrue(checkProperty(mResponse, Constants.KEYFIELD_GROUP_MEMBERS));
        assertTrue(checkProperty(mResponse, Constants.KEYFIELD_GROUP_PARENT));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testCreateSubGroupNotExistParentGroup() throws Exception {
        getTestMethodName();
        GroupBrokerManager.getInstance().createGroup(mUid1, mGid1, null, null);
        sendCreateGroupRequest(mMockDevice, mUid1, null, mGid2);
    }

    @Test
    public void testGetGroupList() throws Exception {
        getTestMethodName();
        sendCreateGroupRequest(mMockDevice, mUid1, mGname1, null);
        String gid = getProperty(mResponse, Constants.KEYFIELD_GID).toString();
        sendCreateGroupRequest(mMockDevice, mUid1, null, gid);
        sendGetGroupResquest(mMockDevice, mUid1);
        assertTrue(responseCodeCheck(mResponse, ResponseStatus.CONTENT));
    }

    @Test
    public void testGetSubscribe() throws Exception {
        getTestMethodName();
        sendGetGroupResquest(mMockDevice, mUid1, Observe.SUBSCRIBE);
        assertTrue(responseCodeCheck(mResponse, ResponseStatus.CONTENT));
        ArrayList<String> uidList = new ArrayList<>();
        uidList.add(mUid1);
        mResponse = null;
        GroupBrokerManager.getInstance().notifyToObservers(uidList);
        assertTrue(responseCodeCheck(mResponse, ResponseStatus.CONTENT));
    }

    @Test
    public void testUnsubscribe() throws Exception {
        getTestMethodName();
        sendGetGroupResquest(mMockDevice, mUid1, Observe.UNSUBSCRIBE);
        assertTrue(responseCodeCheck(mResponse, ResponseStatus.CONTENT));
        mResponse = null;
        ArrayList<String> uidList = new ArrayList<>();
        uidList.add(mUid1);
        GroupBrokerManager.getInstance().notifyToObservers(uidList);
        assertNull(mResponse);
    }

    private void sendCreateGroupRequest(CoapDevice device, String uid,
            String gname, String parent) {
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

    private void sendGetGroupResquest(CoapDevice device, String uid) {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.GET, GROUP_URI,
                UID_QUERY + uid + ";" + Constants.KEYFIELD_GROUP_MEMBERS + "="
                        + uid);
        mGroupResource.onDefaultRequestReceived(device, request);
    }

    private void sendGetGroupResquest(CoapDevice device, String uid,
            Observe obs) {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.GET, GROUP_URI,
                UID_QUERY + uid + ";" + Constants.KEYFIELD_GROUP_MEMBERS + "="
                        + uid);
        ((CoapRequest) request).setObserve(obs);
        mGroupResource.onDefaultRequestReceived(device, request);
    }

    private void resetDB() throws Exception {
        MongoDB mongoDB = new MongoDB("127.0.0.1", Constants.DB_NAME);
        mongoDB.deleteTable(Constants.GROUP_TABLE);
        mongoDB.createTable(Constants.GROUP_TABLE);
        mongoDB.deleteTable(Constants.USER_TABLE);
        mongoDB.createTable(Constants.USER_TABLE);
        mongoDB.deleteTable(Constants.TOKEN_TABLE);
        mongoDB.createTable(Constants.TOKEN_TABLE);
    }

    private void getTestMethodName() {
        StackTraceElement[] stacks = new Throwable().getStackTrace();
        StackTraceElement currentStack = stacks[1];
        System.out.println("\t---Test Name : " + currentStack.getMethodName());
    }

    private boolean responseCodeCheck(IResponse response,
            ResponseStatus responseStatus) {
        if (responseStatus == response.getStatus())
            return true;
        else
            return false;
    }

    private boolean checkProperty(IResponse response, String propertyName) {
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);
        if (payloadData.containsKey(propertyName))
            return true;
        else
            return false;
    }

    private Object getProperty(IResponse response, String propertyName) {
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);
        return payloadData.get(propertyName);
    }
}
