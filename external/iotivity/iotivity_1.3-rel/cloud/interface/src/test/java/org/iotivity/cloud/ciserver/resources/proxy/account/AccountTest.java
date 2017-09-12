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

package org.iotivity.cloud.ciserver.resources.proxy.account;

import static java.util.concurrent.TimeUnit.SECONDS;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.mock;

import java.util.HashMap;
import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.ciserver.Constants;
import org.iotivity.cloud.ciserver.DeviceServerSystem;
import org.iotivity.cloud.util.Cbor;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@PrepareForTest(ConnectorPool.class)
public class AccountTest {
    private String               mDi                 = "B371C481-38E6-4D47-8320-7688D8A5B58C";
    public static final String   ACCOUNT_URI         = Constants.ACCOUNT_FULL_URI;
    private String               mAuthProvider       = "github";
    private String               mAuthCode           = "f6b55d8d8a27e0d32ab8";
    private CoapDevice           mMockDevice         = mock(CoapDevice.class);
    private IRequest             mReqRDServer        = null;
    private IRequest             mReqASServer        = null;
    ConnectorPool                connectorPool       = null;
    private DeviceServerSystem   mDeviceServerSystem = new DeviceServerSystem();
    private final CountDownLatch mLatch              = new CountDownLatch(1);
    @Mock(name = "mRDServer")
    IRequestChannel              mRequestChannelRDServer;
    @Mock(name = "mASServer")
    IRequestChannel              mRequestChannelASServer;

    @InjectMocks
    private Account              mAcHandler          = new Account();

    @Before
    public void setUp() throws Exception {
        Mockito.doReturn("mockDeviceUser").when(mMockDevice).getUserId();
        MockitoAnnotations.initMocks(this);
        mDeviceServerSystem.addResource(mAcHandler);

        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapRequest answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapRequest request = (CoapRequest) args[0];
                mReqRDServer = request;
                mLatch.countDown();
                System.out.println(
                        "\t----------requestMethod : " + request.getMethod());
                System.out.println(
                        "\t----------payload : " + request.getPayloadString());
                System.out.println(
                        "\t----------uripath : " + request.getUriPath());
                System.out.println(
                        "\t---------uriquery : " + request.getUriQuery());
                return null;
            }
        }).when(mRequestChannelRDServer).sendRequest(
                Mockito.any(IRequest.class), Mockito.any(CoapDevice.class));

        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapRequest answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapRequest request = (CoapRequest) args[0];
                mReqASServer = request;
                mLatch.countDown();
                System.out.println(
                        "\t----------requestMethod : " + request.getMethod());
                System.out.println(
                        "\t----------payload : " + request.getPayloadString());
                System.out.println(
                        "\t----------uripath : " + request.getUriPath());
                System.out.println(
                        "\t---------uriquery : " + request.getUriQuery());
                return null;
            }
        }).when(mRequestChannelASServer).sendRequest(
                Mockito.any(IRequest.class), Mockito.any(CoapDevice.class));

        PowerMockito.mockStatic(ConnectorPool.class);
        PowerMockito.when(ConnectorPool.getConnection("account")).thenReturn(mRequestChannelASServer);
        PowerMockito.when(ConnectorPool.getConnection("rd")).thenReturn(mRequestChannelRDServer);
    }

    @Test
    public void testAccountDeviceDeleteOnRequestReceived() throws Exception {
        getTestMethodName();
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.DELETE,
                ACCOUNT_URI, "di=device1");
        mDeviceServerSystem.onRequestReceived(mMockDevice, request);
        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(mReqASServer.getUriQueryMap().get("uid")
                .contains("mockDeviceUser"));
        assertTrue(mReqASServer.getUriQueryMap().get("di").contains("device1"));
        assertTrue(mReqASServer.getUriPath().equals(ACCOUNT_URI));
    }

    IRequest                      asRequest             = MessageBuilder
            .createRequest(RequestMethod.DELETE, ACCOUNT_URI, "di=device1");

    @InjectMocks
    Account.AccountReceiveHandler accountReceiveHandler = mAcHandler.new AccountReceiveHandler(
            asRequest, mMockDevice);

    @Test
    public void testAccountDeviceDeleteOnResponseReceived() throws Exception {
        getTestMethodName();
        IRequest request = MessageBuilder.createRequest(RequestMethod.DELETE,
                ACCOUNT_URI, "di=device1");

        accountReceiveHandler.onResponseReceived(
                MessageBuilder.createResponse(request, ResponseStatus.DELETED));
        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(mReqRDServer.getUriQueryMap().get("di").contains("device1"));
        assertTrue(mReqRDServer.getUriPath()
                .contains(Constants.PREFIX_OIC + "/" + Constants.RD_URI));
    }

    @Test
    public void testAccountSearchSingleUserOnResponseReceived()
            throws Exception {
        getTestMethodName();
        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                Constants.ACCOUNT_SEARCH_FULL_URI,
                "search=userid:user1,userid:user2,userid:user3");

        mDeviceServerSystem.onRequestReceived(mMockDevice, request);
        assertTrue(mReqASServer.getUriPath()
                .equals(Constants.ACCOUNT_SEARCH_FULL_URI));
        assertFalse(
                mReqASServer.getUriQueryMap().containsKey(Constants.USER_ID));
    }

    @Test
    public void testAccountSearchNoQueryOnResponseReceived() throws Exception {
        getTestMethodName();
        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                Constants.ACCOUNT_SEARCH_FULL_URI, null);

        mDeviceServerSystem.onRequestReceived(mMockDevice, request);
        assertTrue(mReqASServer.getUriPath()
                .equals(Constants.ACCOUNT_SEARCH_FULL_URI));
        assertTrue(
                mReqASServer.getUriQueryMap().containsKey(Constants.USER_ID));
    }

    @Test
    public void testAccountResourceOnRequestReceived() throws Exception {
        getTestMethodName();
        // sign up request from the client
        IRequest request = null;
        HashMap<String, String> payloadData = new HashMap<String, String>();
        payloadData.put(Constants.AUTH_CODE, mAuthCode);
        payloadData.put(Constants.AUTH_PROVIDER, mAuthProvider);
        payloadData.put(Constants.DEVICE_ID, mDi);
        Cbor<HashMap<String, String>> cbor = new Cbor<HashMap<String, String>>();
        request = MessageBuilder.createRequest(RequestMethod.POST, ACCOUNT_URI,
                null, ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(payloadData));
        mDeviceServerSystem.onRequestReceived(mMockDevice, request);
        // assertion : request msg to the AS is identical to the request msg
        // from the client
        assertTrue(mLatch.await(1L, SECONDS));
        assertEquals(request, mReqASServer);
    }

    private void getTestMethodName() {
        StackTraceElement[] stacks = new Throwable().getStackTrace();
        StackTraceElement currentStack = stacks[1];
        System.out.println("\t---Test Name : " + currentStack.getMethodName());
    }
}
