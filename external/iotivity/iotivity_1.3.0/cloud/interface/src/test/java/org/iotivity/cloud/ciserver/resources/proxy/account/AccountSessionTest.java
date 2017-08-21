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
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.mock;

import java.util.HashMap;
import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
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
public class AccountSessionTest {
    private String               mDi                 = "B371C481-38E6-4D47-8320-7688D8A5B58C";
    public static final String   SESSION_URI         = Constants.ACCOUNT_SESSION_FULL_URI;
    private CoapDevice           mMockDevice         = mock(CoapDevice.class);
    private IResponse            mRes                = null;
    private IRequest             mReq                = null;
    private ConnectorPool        mConnectorPool      = null;
    private DeviceServerSystem   mDeviceServerSystem = new DeviceServerSystem();
    private final CountDownLatch mLatch              = new CountDownLatch(1);
    @Mock
    private IRequestChannel      mRequestChannel;

    @InjectMocks
    private AccountSession       mAcSessionHandler   = new AccountSession();

    @Before
    public void setUp() throws Exception {
        mRes = null;
        mReq = null;
        Mockito.doReturn(mDi).when(mMockDevice).getDeviceId();
        Mockito.doReturn("mockDeviceUser").when(mMockDevice).getUserId();
        Mockito.doReturn("1689c70ffa245effc563017fee36d250").when(mMockDevice)
                .getAccessToken();
        MockitoAnnotations.initMocks(this);
        mDeviceServerSystem.addResource(mAcSessionHandler);
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapRequest answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapRequest request = (CoapRequest) args[0];
                mReq = request;
                mLatch.countDown();
                System.out.println(
                        "\t----------payload : " + request.getPayloadString());
                System.out.println(
                        "\t----------uripath : " + request.getUriPath());
                System.out.println(
                        "\t---------uriquery : " + request.getUriQuery());
                return null;
            }
        }).when(mRequestChannel).sendRequest(Mockito.any(IRequest.class),
                Mockito.any(CoapDevice.class));

        PowerMockito.mockStatic(ConnectorPool.class);
        PowerMockito.when(ConnectorPool.getConnection(Mockito.anyString())).thenReturn(mRequestChannel);
    }

    @Test
    public void testAccountSignInOnRequestReceived() throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived Sign In Test------------");
        IRequest request = makeSignInRequest();
        mDeviceServerSystem.onRequestReceived(mMockDevice, request);
        assertTrue(mLatch.await(1L, SECONDS));
        assertEquals(request, request);
    }

    @Test
    public void testAccountResourceOnRequestReceived() throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived Sign Out Test------------");
        // sign up request from the client
        IRequest request = makeSignOutRequest();
        mDeviceServerSystem.onRequestReceived(mMockDevice, request);
        // assertion : request msg to the AS is identical to the request msg
        // from the client
        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(queryCheck(mReq, Constants.USER_ID));
        assertTrue(payloadCheck(mReq, Constants.DEVICE_ID));
        assertTrue(payloadCheck(mReq, Constants.ACCESS_TOKEN));
        assertTrue(payloadCheck(mReq, Constants.REQ_LOGIN));
    }

    private IRequest makeSignInRequest() {
        Cbor<HashMap<String, Object>> cbor = new Cbor<HashMap<String, Object>>();
        IRequest request = null;
        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put(Constants.USER_ID, "u0001");
        payloadData.put(Constants.DEVICE_ID, mDi);
        payloadData.put(Constants.ACCESS_TOKEN,
                "1689c70ffa245effc563017fee36d250");
        payloadData.put(Constants.REQ_LOGIN, true);
        request = MessageBuilder.createRequest(RequestMethod.POST, SESSION_URI,
                null, ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(payloadData));
        return request;
    }

    private IRequest makeSignOutRequest() {
        Cbor<HashMap<String, Object>> cbor = new Cbor<HashMap<String, Object>>();
        IRequest request = null;
        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put(Constants.REQ_LOGIN, false);
        payloadData.put(Constants.DEVICE_ID, mDi);
        payloadData.put(Constants.ACCESS_TOKEN,
                "1689c70ffa245effc563017fee36d250");
        request = MessageBuilder.createRequest(RequestMethod.POST, SESSION_URI,
                null, ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(payloadData));
        return request;
    }

    private boolean payloadCheck(IRequest request, String propertyName) {
        Cbor<HashMap<String, Object>> mCbor = new Cbor<>();
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(request.getPayload(), HashMap.class);
        if (payloadData.get(propertyName) != null)
            return true;
        else
            return false;
    }

    private boolean queryCheck(IRequest request, String propertyName) {
        if (request.getUriQueryMap().get(propertyName) != null)
            return true;
        else
            return false;
    }
}
