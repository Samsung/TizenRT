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

package org.iotivity.cloud.ciserver.resources.proxy.rd;

import static java.util.concurrent.TimeUnit.SECONDS;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.mock;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.exception.ClientException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.Observe;
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
public class DevicePresenceTest {
    private static final String DEVICE_PRS_REQ_URI  = Constants.DEVICE_PRESENCE_FULL_URI;
    private String              mDi                 = "B371C481-38E6-4D47-8320-7688D8A5B58C";
    private CoapDevice          mMockDevice         = mock(CoapDevice.class);
    private IRequest            mReq                = null;
    private DeviceServerSystem  mDeviceServerSystem = new DeviceServerSystem();
    final CountDownLatch        mLatch              = new CountDownLatch(1);
    @Mock
    private IRequestChannel     mRequestChannel;
    @InjectMocks
    private DevicePresence      mPrsHandler         = new DevicePresence();

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mDeviceServerSystem.addResource(mPrsHandler);
        Mockito.doReturn("mockDeviceId").when(mMockDevice).getDeviceId();
        Mockito.doReturn("mockUserId").when(mMockDevice).getUserId();
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
                return request;
            }
        }).when(mRequestChannel).sendRequest(Mockito.any(IRequest.class),
                Mockito.any(CoapDevice.class));

        PowerMockito.mockStatic(ConnectorPool.class);
        PowerMockito.when(ConnectorPool.getConnection(Mockito.anyString())).thenReturn(mRequestChannel);
    }

    // @InjectMocks for testSpecificDeviceonResponseReceived
    IRequest                             requestSpecificDevice = makePresenceSpecificDevice(
            "device1");
    @InjectMocks
    DevicePresence.AccountReceiveHandler SpecificDeviceHandler = mPrsHandler.new AccountReceiveHandler(
            requestSpecificDevice, mMockDevice);

    @Test
    public void testSpecificDeviceonResponseReceived() throws ClientException {
        System.out.println(
                "\t--------------onResponseReceived(RD) Device Presence (specific deivce) Test------------");
        IResponse response = responseFromAccountServer();
        SpecificDeviceHandler.onResponseReceived(response);
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        assertTrue(mReq.getObserve() == Observe.SUBSCRIBE);
        assertTrue(queryMap.get("di").contains("device1"));
        assertTrue(mReq.getMethod() == RequestMethod.GET);
    }

    // @InjectMocks for testEntireDeviceonResponseReceived
    IRequest                             requestEntireDevices = makePresenceEntireDevice();
    @InjectMocks
    DevicePresence.AccountReceiveHandler EntireDeviceHandler  = mPrsHandler.new AccountReceiveHandler(
            requestEntireDevices, mMockDevice);

    @Test
    public void testEntireDeviceonResponseReceived() throws ClientException {
        System.out.println(
                "\t--------------onResponseReceived(RD) Device Presence (entire deivces) Test------------");
        IResponse response = responseFromAccountServer();
        EntireDeviceHandler.onResponseReceived(response);
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        assertTrue(mReq.getMethod() == RequestMethod.GET);
        assertTrue(queryMap.get("di").contains("device1"));
        assertTrue(queryMap.get("di").contains("device2"));
        assertTrue(queryMap.get("di").contains("device3"));
        assertTrue(mReq.getObserve() == Observe.SUBSCRIBE);
    }

    @Test
    public void testDevicePresenceObserverSpecificDeviceOnRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(RD) Device Presence (observe specific device) Test------------");
        IRequest request = makePresenceSpecificDevice(mDi);
        mDeviceServerSystem.onRequestReceived(mMockDevice, request);
        assertTrue(mLatch.await(1L, SECONDS));
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        assertTrue(queryMap.containsKey("uid"));
        assertTrue(queryMap.containsKey("members"));
        assertTrue(mReq.getMethod().equals(RequestMethod.GET));
        assertEquals(mReq.getUriPath(),
                Constants.GROUP_FULL_URI + "/mockUserId");
    }

    // @InjectMocks for testPresenceDeregisterSpecificDeviceOnRequestReceived
    IRequest                             requestDeregisterSpecificDevice = makeDeregisterSpecificDevice(
            "device1");
    @InjectMocks
    DevicePresence.AccountReceiveHandler DeregisterHandler               = mPrsHandler.new AccountReceiveHandler(
            requestDeregisterSpecificDevice, mMockDevice);

    @Test
    public void testPresenceDeregisterSpecificDeviceOnRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(RD) Device Presence (deregister specific device) Test------------");
        IResponse response = responseFromAccountServer();
        DeregisterHandler.onResponseReceived(response);
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        assertTrue(mReq.getMethod() == RequestMethod.GET);
        assertTrue(queryMap.get("di").contains("device1"));
        assertTrue(mReq.getObserve() == Observe.UNSUBSCRIBE);
    }

    @Test
    public void testDevicePresenceObserverEntireDevicesOnRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(RD) Device Presence (observe entire devices) Test------------");
        IRequest request = makePresenceEntireDevice();
        mDeviceServerSystem.onRequestReceived(mMockDevice, request);
        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(mReq.getMethod().equals(RequestMethod.GET));
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        assertTrue(queryMap.containsKey("uid"));
        assertTrue(queryMap.containsKey("members"));
        assertEquals(mReq.getUriPath(),
                Constants.GROUP_FULL_URI + "/mockUserId");
    }

    public IRequest makePresenceEntireDevice() {
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.GET,
                DEVICE_PRS_REQ_URI, null);
        ((CoapRequest) request).setObserve(Observe.SUBSCRIBE);
        return request;
    }

    private IRequest makePresenceSpecificDevice(String di) {
        String query = "di=" + di;
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.GET,
                DEVICE_PRS_REQ_URI, query);
        ((CoapRequest) request).setObserve(Observe.SUBSCRIBE);
        return request;
    }

    private IRequest makeDeregisterSpecificDevice(String di) {
        String query = "di=" + di;
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.GET,
                DEVICE_PRS_REQ_URI, query);
        ((CoapRequest) request).setObserve(Observe.UNSUBSCRIBE);
        return request;
    }

    private IResponse responseFromAccountServer() {
        // make response which has "CONTENT" status
        Cbor<HashMap<String, Object>> cbor = new Cbor<>();
        HashMap<String, Object> responsePayload = new HashMap<String, Object>();
        ArrayList<String> deviceList = new ArrayList<String>();
        // assuming that there are three devices in the response msg from
        // the AS
        deviceList.add("device1");
        deviceList.add("device2");
        deviceList.add("device3");
        responsePayload.put("devices", deviceList);
        responsePayload.put("gid", "g0001");
        responsePayload.put("gmid", "u0001");
        ArrayList<String> midList = new ArrayList<String>();
        midList.add("u0001");
        responsePayload.put("members", midList);
        IRequest requestFromCitoAs = MessageBuilder.createRequest(
                RequestMethod.GET, Constants.GROUP_FULL_URI + "/g0001",
                "uid=mockUserId;members=mockUserId");
        IResponse response = MessageBuilder.createResponse(requestFromCitoAs,
                ResponseStatus.CONTENT, ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(responsePayload));
        return response;
    }
}
