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

package org.iotivity.cloud.ciserver;

import static java.util.concurrent.TimeUnit.SECONDS;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.mock;

import java.util.HashMap;
import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.base.OICConstants;
import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.ISignaling;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.coap.CoapSignaling;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.protocols.enums.SignalingMethod;
import org.iotivity.cloud.base.server.CoapServer;
import org.iotivity.cloud.base.server.HttpServer;
import org.iotivity.cloud.ciserver.DeviceServerSystem.CoapDevicePool;
import org.iotivity.cloud.ciserver.DeviceServerSystem.CoapSignalingHandler;
import org.iotivity.cloud.ciserver.resources.proxy.account.Account;
import org.iotivity.cloud.ciserver.resources.proxy.mq.MessageQueue;
import org.iotivity.cloud.ciserver.resources.proxy.rd.ResourceDirectory;
import org.iotivity.cloud.ciserver.resources.proxy.rd.ResourceFind;
import org.iotivity.cloud.ciserver.resources.proxy.rd.ResourcePresence;
import org.iotivity.cloud.util.Cbor;
import org.iotivity.cloud.util.Log;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelId;
import io.netty.channel.ChannelPromise;
import io.netty.util.Attribute;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@PrepareForTest(ConnectorPool.class)
public class DeviceServerSystemTest {
    private ChannelHandlerContext                   mCtx                  = null;
    private ChannelHandlerContext                   mCtxSignal            = null;
    private String                                  mDi                   = "B371C481-38E6-4D47-8320-7688D8A5B58C";
    private String                                  mUserId               = "testuser";
    private String                                  mAccessToken          = "1689c70ffa245effc563017fee36d250";
    private Device                                  mDevice               = mock(
            Device.class);
    private CoapDevice                              mMockDevice           = mock(
            CoapDevice.class);
    private IResponse                               mRes                  = null;
    private IRequest                                mReq                  = null;
    private ISignaling                              mSig                  = null;
    final CountDownLatch                            mLatch                = new CountDownLatch(
            1);
    private Cbor<HashMap<Object, Object>>           mCbor                 = new Cbor<>();
    @Mock
    private IRequestChannel                         mRequestChannel;

    @InjectMocks
    private DeviceServerSystem                      mDeviceServerSystem   = new DeviceServerSystem();
    @InjectMocks
    private DeviceServerSystem.CoapLifecycleHandler mCoapLifecycleHandler = mDeviceServerSystem.new CoapLifecycleHandler();
    @InjectMocks
    private DeviceServerSystem.CoapAuthHandler      mCoapAuthHandler      = mDeviceServerSystem.new CoapAuthHandler();
    @InjectMocks
    private CoapSignalingHandler                    mCoapSignalingHandler = mDeviceServerSystem.new CoapSignalingHandler();

    @Before
    public void setUp() throws Exception {
        Log.createfile();
        MockitoAnnotations.initMocks(this);
        mRes = null;
        mReq = null;
        mCtx = mock(ChannelHandlerContext.class);
        mCtxSignal = mock(ChannelHandlerContext.class);
        Cbor<HashMap<Object, Object>> cbor = new Cbor<>();
        Channel channel = mock(Channel.class);
        Attribute<Device> attribute = mock(Attribute.class);
        ChannelId channelId = mock(ChannelId.class);
        Mockito.doReturn(channel).when(mCtx).channel();
        Mockito.doReturn(attribute).when(channel).attr(Mockito.any());
        Mockito.doReturn(channelId).when(channel).id();
        Mockito.doReturn(
                "0000000141f3edcfc2c3000000000001d0000000000000000000000000000000000000")
                .when(channelId).asLongText();
        Mockito.doReturn(mMockDevice).when(attribute).get();
        Mockito.doReturn("sampleDeviceID").when(mMockDevice).getDeviceId();
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
        }).when(mCtx).fireChannelRead(Mockito.any());

        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapSignaling answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapSignaling signaling = (CoapSignaling) args[0];
                System.out.println("\t----------payload : "
                        + signaling.getPayloadString());
                System.out.println("\t----------signaling method : "
                        + signaling.getSignalingMethod());
                if (signaling.getSignalingMethod()
                        .equals(SignalingMethod.CSM)) {
                    System.out.println("\t----------CSM Blockwise Transfer : "
                            + signaling.getCsmBlockWiseTransfer());
                    System.out.println("\t----------CSM Max Message Size : "
                            + signaling.getCsmMaxMessageSize());
                    System.out.println("\t----------CSM Server Name : "
                            + signaling.getCsmServerName());
                }
                mSig = signaling;
                mLatch.countDown();
                return null;
            }
        }).when(mCtxSignal).fireChannelRead(Mockito.any());

        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse response = (CoapResponse) args[0];
                System.out.println(
                        "\t----------payload : " + response.getPayloadString());
                System.out.println(
                        "\t----------status : " + response.getStatus());
                mRes = response;
                mLatch.countDown();
                return null;
            }
        }).when(mCtx).writeAndFlush(Mockito.any());

        PowerMockito.mockStatic(ConnectorPool.class);
        PowerMockito.when(ConnectorPool.getConnection(Mockito.anyString())).thenReturn(mRequestChannel);
    }

    @Test
    public void testAddHttpServer() throws Exception {
        HttpServer httpServer = new HttpServer(null);
        mDeviceServerSystem.addServer(httpServer);
    }

    @Test
    public void testAddCoapServer() throws Exception {
        CoapServer coapServer = new CoapServer(null);
        mDeviceServerSystem.addServer(coapServer);
    }

    @Test
    public void testGetDevicePool() throws Exception {
        CoapDevicePool devicePool = mDeviceServerSystem.getDevicePool();
        if (devicePool != null) {
            System.out.println("devicePool returned :" + devicePool);
        }
    }

    @Test
    public void testAddDevice() throws Exception {
        CoapDevice coapDevice = new CoapDevice(null);
        coapDevice.updateDevice(mDi, mUserId, mAccessToken);
        CoapDevicePool devicePool = mDeviceServerSystem.getDevicePool();
        devicePool.addDevice(coapDevice);
    }

    @Test
    public void testRemoveNotRegisteredDevice() throws Exception {
        CoapDevice coapDevice = new CoapDevice(null);
        coapDevice.updateDevice(mDi, mUserId, mAccessToken);
        CoapDevicePool devicePool = mDeviceServerSystem.getDevicePool();
        devicePool.removeDevice(coapDevice);
    }

    @Test
    public void testRemoveDevice() throws Exception {
        CoapDevice coapDevice = new CoapDevice(null);
        coapDevice.updateDevice(mDi, mUserId, mAccessToken);
        CoapDevicePool devicePool = mDeviceServerSystem.getDevicePool();
        devicePool.addDevice(coapDevice);
        devicePool.removeDevice(coapDevice);
    }

    @Test
    public void testQueryDevice() throws Exception {
        CoapDevice coapDevice = new CoapDevice(null);
        coapDevice.updateDevice(mDi, mUserId, mAccessToken);
        CoapDevicePool devicePool = mDeviceServerSystem.getDevicePool();
        devicePool.addDevice(coapDevice);
        devicePool.queryDevice(mDi);
    }

    @Test
    public void testStopSystem() throws Exception {
        mDeviceServerSystem.stopSystem();
    }

    @Test
    public void testAddAccountResource() {
        Account acHandler = new Account();
        ResourceDirectory rdHandler = new ResourceDirectory();
        ResourceFind resHandler = new ResourceFind();
        ResourcePresence adHandler = new ResourcePresence();
        MessageQueue mqHandler = new MessageQueue();
        mDeviceServerSystem.addResource(acHandler);
        mDeviceServerSystem.addResource(rdHandler);
        mDeviceServerSystem.addResource(resHandler);
        mDeviceServerSystem.addResource(adHandler);
        mDeviceServerSystem.addResource(mqHandler);
    }

    @Test
    public void coapAuthHandlerAccountCtxNullChannelReadRequest()
            throws InterruptedException {
        System.out.println(
                "\t--------------coapAuthHandler Account ctx is null ChannelReadRequest Test------------");
        Channel channel = mock(Channel.class);
        Attribute<Device> attribute = mock(Attribute.class);
        Mockito.doReturn(channel).when(mCtx).channel();
        Mockito.doReturn(attribute).when(channel).attr(Mockito.any());
        Mockito.doReturn(null).when(attribute).get();
        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put(Constants.DEVICE_ID, "sampleDevice");

        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                OICConstants.ACCOUNT_FULL_URI, null,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        mCoapAuthHandler.channelRead(mCtx, request);

        assertEquals(mReq.getUriPath(), OICConstants.ACCOUNT_FULL_URI);
        assertTrue(mCbor.parsePayloadFromCbor(mReq.getPayload(), HashMap.class)
                .containsKey(Constants.DEVICE_ID));
    }

    @Test
    public void coapAuthHandlerPingChannelReadRequest()
            throws InterruptedException {
        System.out.println(
                "\t--------------coapAuthHandler Ping ChannelReadRequest Test------------");

        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                OICConstants.KEEP_ALIVE_FULL_URI, null, null, null);
        mCoapAuthHandler.channelRead(mCtx, request);

        assertEquals(mReq.getUriPath(), OICConstants.KEEP_ALIVE_FULL_URI);
    }

    @Test
    public void coapAuthHandlerAccountSessionChannelReadRequest()
            throws InterruptedException {
        System.out.println(
                "\t--------------coapAuthHandler AccountSession ChannelReadRequest Test------------");

        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put(Constants.DEVICE_ID, mDi);
        payloadData.put(Constants.USER_ID, mAccessToken);
        payloadData.put(Constants.ACCESS_TOKEN, mAccessToken);

        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                OICConstants.ACCOUNT_SESSION_FULL_URI, null,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        mCoapAuthHandler.channelRead(mCtx, request);

        assertEquals(mReq.getUriPath(), OICConstants.ACCOUNT_SESSION_FULL_URI);
        assertTrue(mCbor.parsePayloadFromCbor(mReq.getPayload(), HashMap.class)
                .containsKey(Constants.DEVICE_ID));
        assertTrue(mCbor.parsePayloadFromCbor(mReq.getPayload(), HashMap.class)
                .containsKey(Constants.USER_ID));
        assertTrue(mCbor.parsePayloadFromCbor(mReq.getPayload(), HashMap.class)
                .containsKey(Constants.ACCESS_TOKEN));
    }

    @Test
    public void coapAuthHandlerAccountSessionCtxNullChannelReadRequest()
            throws InterruptedException {
        System.out.println(
                "\t--------------coapAuthHandler AccountSession ctx is null ChannelReadRequest Test------------");
        Channel channel = mock(Channel.class);
        Attribute<Device> attribute = mock(Attribute.class);
        Mockito.doReturn(channel).when(mCtx).channel();
        Mockito.doReturn(attribute).when(channel).attr(Mockito.any());
        Mockito.doReturn(null).when(attribute).get();
        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put(Constants.DEVICE_ID, mDi);
        payloadData.put(Constants.USER_ID, mAccessToken);
        payloadData.put(Constants.ACCESS_TOKEN, mAccessToken);

        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                OICConstants.ACCOUNT_SESSION_FULL_URI, null,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        mCoapAuthHandler.channelRead(mCtx, request);

        assertEquals(mReq.getUriPath(), OICConstants.ACCOUNT_SESSION_FULL_URI);
        assertTrue(mCbor.parsePayloadFromCbor(mReq.getPayload(), HashMap.class)
                .containsKey(Constants.DEVICE_ID));
        assertTrue(mCbor.parsePayloadFromCbor(mReq.getPayload(), HashMap.class)
                .containsKey(Constants.USER_ID));
        assertTrue(mCbor.parsePayloadFromCbor(mReq.getPayload(), HashMap.class)
                .containsKey(Constants.ACCESS_TOKEN));
        mLatch.countDown();
    }

    @Test
    public void coapAuthHandlerUnAuthorizedExceptionChannelReadRequest()
            throws Exception {

        System.out.println(
                "\t--------------coapAuthHandler UnAuthorizedException ChannelReadRequest Test------------");

        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                OICConstants.ACCOUNT_SESSION_FULL_URI + "/" + "errorURI", null,
                null, null);
        mCoapAuthHandler.channelRead(mCtx, request);
        assertEquals(mRes.getStatus(), ResponseStatus.UNAUTHORIZED);
    }

    @Test
    public void coapAuthHandlerAccountSessionChannelReadResponse()
            throws Exception {
        System.out.println(
                "\t--------------coapAuthHandler AccountSession ChannelReadResponse Test------------");

        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                OICConstants.ACCOUNT_SESSION_FULL_URI, null, null, null);
        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put(Constants.EXPIRES_IN, 3300);
        IResponse response = MessageBuilder.createResponse(request,
                ResponseStatus.CHANGED, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        ChannelPromise channelPromise = null;
        mCoapAuthHandler.write(mCtx, response, channelPromise);
        assertEquals(((IRequest) mRes).getUriPath(),
                OICConstants.ACCOUNT_SESSION_FULL_URI);
        assertTrue(mCbor.parsePayloadFromCbor(mRes.getPayload(), HashMap.class)
                .containsKey(Constants.EXPIRES_IN));
    }

    @Test
    public void coapAuthHandlerUnAuthorizedExceptionChannelReadResponse()
            throws Exception {
        System.out.println(
                "\t--------------coapAuthHandler UnAuthorizedException ChannelReadResponse Test------------");

        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                OICConstants.ACCOUNT_SESSION_FULL_URI, null, null, null);
        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put(Constants.EXPIRES_IN, -1);
        IResponse response = MessageBuilder.createResponse(request,
                ResponseStatus.UNAUTHORIZED, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        ChannelPromise channelPromise = null;
        mCoapAuthHandler.write(mCtx, response, channelPromise);
        assertEquals(mRes.getStatus(), ResponseStatus.UNAUTHORIZED);
    }

    @Test
    public void coapAuthHandlerCSMSignaling() throws Exception {

        System.out.println(
                "\t--------------coapAuthHandler coapAuthHandlerCSMSignaling Test------------");
        ISignaling signal = MessageBuilder.createSignaling(SignalingMethod.CSM);
        CoapSignaling signaling = (CoapSignaling) signal;
        signaling.setCsmBlockWiseTransfer(true);
        signaling.setCsmMaxMessageSize(1124);
        signaling.setCsmServerName("default_server_name");
        mCoapSignalingHandler.channelRead(mCtxSignal, signal);
        assertEquals(mSig.getSignalingMethod(), SignalingMethod.CSM);
    }

    @Test
    public void CoapLifecycleHandlerChannelReadRequest()
            throws InterruptedException {
        System.out.println(
                "\t--------------CoapLifecycleHandler ChannelReadCoapRequest Test------------");

        mCoapLifecycleHandler.channelRead(mCtx,
                MessageBuilder.createRequest(RequestMethod.GET, null, null));
        assertTrue(mLatch.await(1L, SECONDS));
    }

    @Test
    public void CoapLifecycleHandlerUnAuthorizedExceptionChannelReadRequest()
            throws Exception {
        System.out.println(
                "\t--------------CoapLifecycleHandler ChannelReadCoapRequest Test------------");
        Channel channel = mock(Channel.class);
        Attribute<Device> attribute = mock(Attribute.class);
        Mockito.doReturn(channel).when(mCtx).channel();
        Mockito.doReturn(attribute).when(channel).attr(Mockito.any());
        Mockito.doReturn(mMockDevice).when(attribute).get();
        Mockito.doReturn(true).when(mMockDevice).isExpiredTime();
        ChannelId channelId = mock(ChannelId.class);
        Mockito.doReturn(channelId).when(channel).id();
        Mockito.doReturn(
                "0000000141f3edcfc2c3000000000001d0000000000000000000000000000000000000")
                .when(channelId).asLongText();
        mCoapLifecycleHandler.channelRead(mCtx,
                MessageBuilder.createRequest(RequestMethod.GET, null, null));
        assertEquals(mRes.getStatus(), ResponseStatus.UNAUTHORIZED);
    }

    @Test
    public void CoapLifecycleHandlerchannelActive() throws Exception {
        System.out.println(
                "\t--------------CoapLifecycleHandler ChannelReadCoapRequest    Test------------");
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapRequest answer(InvocationOnMock invocation)
                    throws Throwable {

                Object[] args = invocation.getArguments();
                CoapRequest req = (CoapRequest) args[0];
                assertTrue(req.getUriPath().contains(Constants.PREFIX_OIC + "/"
                        + Constants.DEVICE_PRESENCE_URI));
                assertTrue(mCbor
                        .parsePayloadFromCbor(req.getPayload(), HashMap.class)
                        .containsKey(Constants.PRESENCE_STATE));

                mLatch.countDown();
                return null;
            }

        }).when(mRequestChannel).sendRequest(Mockito.any(), Mockito.any());
        mCoapLifecycleHandler.channelActive(mCtx);
    }

    @Test
    public void CoapLifecycleHandlerchannelInActive() throws Exception {
        System.out.println(
                "\t--------------CoapLifecycleHandler ChannelReadCoapRequest    Test------------");

        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapRequest answer(InvocationOnMock invocation)
                    throws Throwable {

                Object[] args = invocation.getArguments();
                CoapRequest req = (CoapRequest) args[0];
                assertTrue(req.getUriPath().contains(Constants.PREFIX_OIC + "/"
                        + Constants.DEVICE_PRESENCE_URI));
                assertTrue(mCbor
                        .parsePayloadFromCbor(req.getPayload(), HashMap.class)
                        .containsKey(Constants.PRESENCE_STATE));

                mLatch.countDown();
                return null;
            }

        }).when(mRequestChannel).sendRequest(Mockito.any(), Mockito.any());
        mCoapLifecycleHandler.channelInactive(mCtx);
    }

}
