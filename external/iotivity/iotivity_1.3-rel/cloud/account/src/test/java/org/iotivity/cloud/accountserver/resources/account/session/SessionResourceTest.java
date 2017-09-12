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

package org.iotivity.cloud.accountserver.resources.account.session;

import static org.junit.Assert.assertTrue;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.db.MongoDB;
import org.iotivity.cloud.accountserver.db.TokenTable;
import org.iotivity.cloud.accountserver.db.UserTable;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
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

public class SessionResourceTest {
    private static final String            DEFAULT_AUTH_LOGIN        = Constants.ACCOUNT_SESSION_FULL_URI;
    private static final String            DEFAULT_AUTH_LOGOUT       = Constants.ACCOUNT_SESSION_FULL_URI;
    private static final String            DEVICE_ID                 = "d0001";
    private static final String            ACCESS_TOKEN              = "5f5536c896da7dd437316585b86ef9dd03441c40";
    private static final String            REFRESH_TOKEN             = "rt0001";
    private static final String            USER_UUID                 = "bc38f243-aab5-44d3-8eb9-4a54ebbaf359";
    private static final String            AUTH_PROVIDER             = "Github";
    private TypeCastingManager<UserTable>  mUserTableCastingManager  = new TypeCastingManager<>();
    private TypeCastingManager<TokenTable> mTokenTableCastingManager = new TypeCastingManager<>();
    private CountDownLatch                 mLatch                    = new CountDownLatch(
            1);
    private CoapDevice                     mMockDevice               = Mockito
            .mock(CoapDevice.class);
    private Cbor<HashMap<String, Object>>  mCbor                     = new Cbor<>();
    private IResponse                      mResponse                 = null;
    private SessionResource                mSessionResource          = new SessionResource();

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        // reset data base
        resetDB();
        // register the token table and user table to the DB
        HashMap<String, Object> tokenInfo = mTokenTableCastingManager
                .convertObjectToMap(makeTokenTable());
        HashMap<String, Object> userInfo = mUserTableCastingManager
                .convertObjectToMap(makeUserTable());
        AccountDBManager.getInstance()
                .insertAndReplaceRecord(Constants.TOKEN_TABLE, tokenInfo);
        AccountDBManager.getInstance().insertRecord(Constants.USER_TABLE,
                userInfo);
        // response handler mock
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
                mLatch.countDown();
                return null;
            }
        }).when(mMockDevice).sendResponse(Mockito.anyObject());
    }

    @After
    public void resetAccountDatabase() throws Exception {
        MongoDB mongoDB = new MongoDB("127.0.0.1", Constants.DB_NAME);
        mongoDB.createTable(Constants.USER_TABLE);
        mongoDB.createTable(Constants.TOKEN_TABLE);
        mongoDB.createTable(Constants.GROUP_TABLE);
    }

    @Test
    public void testSignInOnDefaultRequestReceived() throws Exception {
        getTestMethodName();
        SignIn(DEVICE_ID, ACCESS_TOKEN);
        assertTrue(hashmapCheck(mResponse, "expiresin"));
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
    }

    @Test(expected = ServerException.UnAuthorizedException.class)
    public void testSignInExpiredTokenOnDefaultRequestReceived()
            throws Exception {
        getTestMethodName();
        TokenTable tokenTable = makeTokenTable();
        tokenTable.setExpiredtime(0);
        HashMap<String, Object> tokenInfo = mTokenTableCastingManager
                .convertObjectToMap(tokenTable);
        Thread.sleep(500);
        AccountDBManager.getInstance()
                .insertAndReplaceRecord(Constants.TOKEN_TABLE, tokenInfo);
        SignIn(DEVICE_ID, ACCESS_TOKEN);
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testSignInWithInvalidMethodOnDefaultRequestReceived()
            throws Exception {
        getTestMethodName();
        SignInWithInvalidMethod(DEVICE_ID, ACCESS_TOKEN);
    }

    @Test(expected = ServerException.UnAuthorizedException.class)
    public void testWrongAccessTokenSignInOnDefaultRequestReceived()
            throws Exception {
        getTestMethodName();
        String accessTokenNotStored = "5689c70ffa245effc563017fee36d250";
        // sign in request
        SignIn(DEVICE_ID, accessTokenNotStored);
    }

    @Test(expected = ServerException.UnAuthorizedException.class)
    public void testInvalidDeviceIdSignInOnDefaultRequestReceived()
            throws Exception {
        getTestMethodName();
        String diNotStored = "F371C481-38E6-4D47-8320-7688D8A5B58C";
        SignIn(diNotStored, ACCESS_TOKEN);
    }

    @Test
    public void testSignOutOnDefaultRequestReceived() throws Exception {
        getTestMethodName();
        SignOut(DEVICE_ID, ACCESS_TOKEN);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
    }

    public void getTestMethodName() {
        StackTraceElement[] stacks = new Throwable().getStackTrace();
        StackTraceElement currentStack = stacks[1];
        System.out.println("\t---Test Name : " + currentStack.getMethodName());
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testSignInWithNullPayloadOnDefaultRequestReceived()
            throws Exception {
        getTestMethodName();
        SignInWithNullPayload();
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testSignOutWithNullPayloadOnDefaultRequestReceived()
            throws Exception {
        getTestMethodName();
        SignOutWithNullPayload();
    }

    @Test(expected = ServerException.PreconditionFailedException.class)
    public void testSignInWithBlankHashMapPayloadOnDefaultRequestReceived()
            throws Exception {
        getTestMethodName();
        SignInWithBlankHashMapPayload();
    }

    @Test(expected = ServerException.PreconditionFailedException.class)
    public void testSignOutWithBlankHashMapPayloadOnDefaultRequestReceived()
            throws Exception {
        getTestMethodName();
        SignOutWithBlankHashMapPayload();
    }

    private void SignInWithInvalidMethod(String di, String accessToken)
            throws Exception {
        System.out.println("-----Sign In With Invalid Method (GET)");
        IRequest request = null;
        request = SignInWithInvalidMethodRequest(di, accessToken);
        mSessionResource.onDefaultRequestReceived(mMockDevice, request);
    }

    private void SignIn(String di, String accessToken) throws Exception {
        System.out.println("-----Sign In");
        IRequest request = null;
        request = SignInRequest(di, accessToken);
        mSessionResource.onDefaultRequestReceived(mMockDevice, request);
    }

    private void SignInWithBlankHashMapPayload() throws Exception {
        System.out.println("-----Sign In With Blank Hashmap Payload");
        IRequest request = null;
        request = SignInBlankHashmapPayloadRequest();
        mSessionResource.onDefaultRequestReceived(mMockDevice, request);
    }

    private void SignInWithNullPayload() throws Exception {
        System.out.println("-----Sign In With Null Payload");
        IRequest request = null;
        request = SignInNullPayloadRequest();
        mSessionResource.onDefaultRequestReceived(mMockDevice, request);
    }

    private void SignOut(String di, String accessToken) throws Exception {
        System.out.println("-----Sign Out");
        IRequest request = null;
        request = SignOutRequest(di, accessToken);
        mSessionResource.onDefaultRequestReceived(mMockDevice, request);
    }

    private void SignOutWithBlankHashMapPayload() throws Exception {
        System.out.println("-----Sign Out With Blank Hashmap Payload");
        IRequest request = null;
        request = SignOutBlankHashmapPayloadRequest();
        mSessionResource.onDefaultRequestReceived(mMockDevice, request);
    }

    private void SignOutWithNullPayload() throws Exception {
        System.out.println("-----Sign Out With Null Payload");
        IRequest request = null;
        request = SignOutNullPayloadRequest();
        mSessionResource.onDefaultRequestReceived(mMockDevice, request);
    }

    private IRequest SignInNullPayloadRequest() {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.POST,
                DEFAULT_AUTH_LOGIN, null);
        return request;
    }

    private IRequest SignInBlankHashmapPayloadRequest() {
        IRequest request = null;
        HashMap<String, Object> payloadData = new HashMap<String, Object>();
        request = MessageBuilder.createRequest(RequestMethod.POST,
                DEFAULT_AUTH_LOGIN, null, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return request;
    }

    private IRequest SignOutNullPayloadRequest() {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.POST,
                DEFAULT_AUTH_LOGOUT, null);
        return request;
    }

    private IRequest SignOutBlankHashmapPayloadRequest() {
        IRequest request = null;
        HashMap<String, Object> payloadData = new HashMap<String, Object>();
        request = MessageBuilder.createRequest(RequestMethod.POST,
                DEFAULT_AUTH_LOGOUT, null, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return request;
    }

    private IRequest SignInWithInvalidMethodRequest(String deviceId,
            String accessToken) {
        IRequest request = null;
        HashMap<String, Object> payloadData = new HashMap<String, Object>();
        payloadData.put("accesstoken", accessToken);
        payloadData.put("status", true);
        payloadData.put("di", deviceId);
        request = MessageBuilder.createRequest(RequestMethod.GET,
                DEFAULT_AUTH_LOGIN, null, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return request;
    }

    private IRequest SignInRequest(String deviceId, String accessToken) {
        IRequest request = null;
        HashMap<String, Object> payloadData = new HashMap<String, Object>();
        payloadData.put("accesstoken", accessToken);
        payloadData.put("login", true);
        payloadData.put("di", deviceId);
        payloadData.put("uid", USER_UUID);
        request = MessageBuilder.createRequest(RequestMethod.POST,
                DEFAULT_AUTH_LOGIN, null, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return request;
    }

    private IRequest SignOutRequest(String deviceId, String accessToken) {
        IRequest request = null;
        HashMap<String, Object> payloadData = new HashMap<String, Object>();
        payloadData.put("accesstoken", accessToken);
        payloadData.put("login", false);
        payloadData.put("di", deviceId);
        StringBuffer query = new StringBuffer();
        query.append("uid=" + USER_UUID);
        request = MessageBuilder.createRequest(RequestMethod.POST,
                DEFAULT_AUTH_LOGOUT, query.toString(),
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return request;
    }

    private boolean hashmapCheck(IResponse response, String propertyName) {
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);
        if (payloadData.get(propertyName) != null)
            return true;
        else
            return false;
    }

    private boolean methodCheck(IResponse response,
            ResponseStatus responseStatus) {
        if (responseStatus == response.getStatus())
            return true;
        else
            return false;
    }

    private TokenTable makeTokenTable() {
        TokenTable tokenInfo = new TokenTable();
        tokenInfo.setUuid(USER_UUID);
        tokenInfo.setDid(DEVICE_ID);
        tokenInfo.setAccesstoken(ACCESS_TOKEN);
        tokenInfo.setRefreshtoken(REFRESH_TOKEN);
        tokenInfo.setProvider(AUTH_PROVIDER);
        tokenInfo.setExpiredtime(-1);
        Date currentTime = new Date();
        DateFormat transFormat = new SimpleDateFormat("yyyyMMddkkmm");
        tokenInfo.setIssuedtime(transFormat.format(currentTime));
        return tokenInfo;
    }

    private UserTable makeUserTable() {
        UserTable userInfo = new UserTable();
        userInfo.setUuid(USER_UUID);
        userInfo.setProvider(AUTH_PROVIDER);
        userInfo.setUserid("userId");
        return userInfo;
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
}
