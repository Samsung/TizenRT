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

import java.util.ArrayList;
import java.util.Arrays;
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
public class AclInviteTest {
    private static final String  TEST_RESOURCE_INVITE_URI = Constants.INVITE_FULL_URI;

    private CoapDevice           mMockDevice              = mock(
            CoapDevice.class);
    private IRequest             mReq                     = null;
    private DeviceServerSystem   mDeviceServerSystem      = new DeviceServerSystem();
    private final CountDownLatch mLatch                   = new CountDownLatch(
            1);

    @Mock
    private IRequestChannel      mRequestChannel;

    @InjectMocks
    private AclInvite            mAclInviteHandler        = new AclInvite();

    @Before
    public void setUp() throws Exception {
        mReq = null;
        Mockito.doReturn("mockUserId").when(mMockDevice).getUserId();
        MockitoAnnotations.initMocks(this);
        mDeviceServerSystem.addResource(mAclInviteHandler);
        // callback mock
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapRequest answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapRequest request = (CoapRequest) args[0];
                System.out.println(
                        "\t----------payload : " + request.getPayloadString());
                System.out.println(
                        "\t----------uripath : " + request.getUriPath());
                System.out.println(
                        "\t---------uriquery : " + request.getUriQuery());
                mReq = request;
                mLatch.countDown();
                return null;
            }
        }).when(mRequestChannel).sendRequest(Mockito.any(IRequest.class),
                Mockito.any(CoapDevice.class));

        PowerMockito.mockStatic(ConnectorPool.class);
        PowerMockito.when(ConnectorPool.getConnection(Mockito.anyString())).thenReturn(mRequestChannel);
    }

    @Test
    public void testPostRequestReceived() throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(AS) Post Message Test------------");

        HashMap<String, Object> payloadData = new HashMap<>();
        HashMap<String, Object> invite = new HashMap<>();
        invite.put("gid", "g0001");
        invite.put("mid", "u0001");
        payloadData.put("uid", "u0001");
        payloadData.put("invite", Arrays.asList(invite));
        Cbor<HashMap<Object, Object>> cbor = new Cbor<>();
        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                TEST_RESOURCE_INVITE_URI, null, ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(payloadData));
        mAclInviteHandler.onRequestReceived(mMockDevice, request);

        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(mReq.getUriQueryMap().containsKey("uid"));
        assertTrue(cbor.parsePayloadFromCbor(mReq.getPayload(), HashMap.class)
                .containsKey("uid"));
        assertTrue(cbor.parsePayloadFromCbor(mReq.getPayload(), HashMap.class)
                .containsKey("invite"));
        ArrayList<HashMap<String, Object>> getinvite = (ArrayList<HashMap<String, Object>>) cbor
                .parsePayloadFromCbor(mReq.getPayload(), HashMap.class)
                .get("invite");
        assertTrue(getinvite.get(0).containsKey("gid"));
        assertTrue(getinvite.get(0).containsKey("mid"));
        assertEquals(mReq.getUriPath(), TEST_RESOURCE_INVITE_URI);
    }

    @Test
    public void testGetRequestReceivedAccept() throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(AS) Get Message Test------------");

        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                TEST_RESOURCE_INVITE_URI, "gid=g0001;accept=1", null, null);
        mAclInviteHandler.onRequestReceived(mMockDevice, request);

        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(mReq.getUriQueryMap().containsKey("uid"));
        assertTrue(mReq.getUriQueryMap().containsKey("gid"));
        assertTrue(mReq.getUriQueryMap().containsKey("accept"));
        assertTrue(mReq.getUriQueryMap().get("accept").get(0).equals("1"));
        assertEquals(mReq.getUriPath(), TEST_RESOURCE_INVITE_URI);
    }

    @Test
    public void testGetRequestReceivedDeny() throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(AS) Get Message Test------------");

        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                TEST_RESOURCE_INVITE_URI, "gid=g0001;accept=0", null, null);
        mAclInviteHandler.onRequestReceived(mMockDevice, request);

        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(mReq.getUriQueryMap().containsKey("uid"));
        assertTrue(mReq.getUriQueryMap().containsKey("gid"));
        assertTrue(mReq.getUriQueryMap().containsKey("accept"));
        assertTrue(mReq.getUriQueryMap().get("accept").get(0).equals("0"));
        assertEquals(mReq.getUriPath(), TEST_RESOURCE_INVITE_URI);
    }

    @Test
    public void testDeleteRequestReceived() throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(AS) Delete Message Test------------");

        IRequest request = MessageBuilder.createRequest(RequestMethod.DELETE,
                TEST_RESOURCE_INVITE_URI, "gid=g0001;mid=u0002", null, null);
        mAclInviteHandler.onRequestReceived(mMockDevice, request);

        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(mReq.getUriQueryMap().containsKey("gid"));
        assertTrue(mReq.getUriQueryMap().containsKey("mid"));
        assertTrue(mReq.getUriQueryMap().containsKey("uid"));
        assertEquals(mReq.getUriPath(), TEST_RESOURCE_INVITE_URI);
    }
}
