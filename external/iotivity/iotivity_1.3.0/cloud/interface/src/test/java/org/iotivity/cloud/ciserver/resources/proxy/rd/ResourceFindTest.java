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
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
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
public class ResourceFindTest {
    private static final String TEST_RESOURCE_FIND_URI = Constants.WELL_KNOWN_FULL_URI;
    private String              di                     = "B371C481-38E6-4D47-8320-7688D8A5B58C";
    private CoapDevice          mMockDevice            = mock(CoapDevice.class);
    private IResponse           mRes                   = null;
    private IRequest            mReq                   = null;
    private DeviceServerSystem  mDeviceServerSystem    = new DeviceServerSystem();
    final CountDownLatch        mLatch                 = new CountDownLatch(1);

    @Mock(name = "mRDServer")
    IRequestChannel             mRequestChannelRDServer;
    @Mock(name = "mASServer")
    IRequestChannel             mRequestChannelASServer;

    @InjectMocks
    private ResourceFind        mResHandler            = new ResourceFind();

    @Before
    public void setUp() throws Exception {
        mRes = null;
        mReq = null;
        Mockito.doReturn("mockDeviceId").when(mMockDevice).getDeviceId();
        Mockito.doReturn("mockUserId").when(mMockDevice).getUserId();
        MockitoAnnotations.initMocks(this);
        mDeviceServerSystem.addResource(mResHandler);
        // callback mock
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];
                mRes = resp;
                return resp;
            }
        }).when(mMockDevice).sendResponse(Mockito.anyObject());
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
        }).when(mRequestChannelRDServer).sendRequest(
                Mockito.any(IRequest.class), Mockito.any(CoapDevice.class));

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
        }).when(mRequestChannelASServer).sendRequest(
                Mockito.any(IRequest.class), Mockito.any(CoapDevice.class));

        PowerMockito.mockStatic(ConnectorPool.class);
        PowerMockito.when(ConnectorPool.getConnection("account")).thenReturn(mRequestChannelASServer);
        PowerMockito.when(ConnectorPool.getConnection("rd")).thenReturn(mRequestChannelRDServer);
    }

    // @InjectMocks for testSpecificDeviceonResponseReceived
    IRequest                           requestSpecificDevice = MessageBuilder
            .createRequest(RequestMethod.GET, TEST_RESOURCE_FIND_URI,
                    "rt=core.light;di=" + "device1");
    @InjectMocks
    ResourceFind.AccountReceiveHandler specificDeviceHandler = mResHandler.new AccountReceiveHandler(
            requestSpecificDevice, mMockDevice);

    @Test
    public void testSpecificDeviceonResponseReceived()
            throws ClientException, InterruptedException {
        System.out.println(
                "\t--------------onResponseReceived(RD) Resource Find (specific deivce) Test------------");
        IResponse response = responseFromAccountServer();
        specificDeviceHandler.onResponseReceived(response);
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        assertTrue(mReq.getMethod() == RequestMethod.GET);
        assertTrue(queryMap.get("rt").contains("core.light"));
        assertTrue(queryMap.get("di").contains("device1"));
    }

    // @InjectMocks for testEntireDeviceonResponseReceived
    IRequest                           requestEntireDevices = MessageBuilder
            .createRequest(RequestMethod.GET, TEST_RESOURCE_FIND_URI,
                    "rt=core.light");
    @InjectMocks
    ResourceFind.AccountReceiveHandler entireDevicehandler  = mResHandler.new AccountReceiveHandler(
            requestEntireDevices, mMockDevice);

    @Test
    public void testEntireDeviceonResponseReceived() throws ClientException {
        System.out.println(
                "\t--------------onResponseReceived(RD) Resource Find (entire deivces) Test------------");
        IResponse response = responseFromAccountServer();
        entireDevicehandler.onResponseReceived(response);
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        assertTrue(mReq.getMethod() == RequestMethod.GET);
        assertTrue(queryMap.get("rt").contains("core.light"));
        assertTrue(queryMap.get("di").contains("device1"));
        assertTrue(queryMap.get("di").contains("device2"));
        assertTrue(queryMap.get("di").contains("device3"));
    }

    // @InjectMocks for testEntireDeviceonResponseReceived
    IRequest                           requestEntireDevicesNoQuery = MessageBuilder
            .createRequest(RequestMethod.GET, TEST_RESOURCE_FIND_URI, null);
    @InjectMocks
    ResourceFind.AccountReceiveHandler entireDevicesNoQueryHandler = mResHandler.new AccountReceiveHandler(
            requestEntireDevicesNoQuery, mMockDevice);

    @Test
    public void testEntireDeviceNoQueryonResponseReceived()
            throws ClientException {
        System.out.println(
                "\t--------------onResponseReceived(RD) Resource Find (entire deivces, No Query) Test------------");
        IResponse response = responseFromAccountServer();
        entireDevicesNoQueryHandler.onResponseReceived(response);
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        assertTrue(mReq.getMethod() == RequestMethod.GET);
        assertTrue(queryMap.get("di").contains("device1"));
        assertTrue(queryMap.get("di").contains("device2"));
        assertTrue(queryMap.get("di").contains("device3"));
    }

    @Test
    public void testResourceFindEntireDevicesOnRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(RD) Resource Find (entire deivces) Test------------");
        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                TEST_RESOURCE_FIND_URI, "rt=core.light");
        mResHandler.onRequestReceived(mMockDevice, request);
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(queryMap.containsKey("uid"));
        assertTrue(queryMap.containsKey("members"));
        assertEquals(mReq.getUriPath(),
                Constants.GROUP_FULL_URI + "/mockUserId");
    }

    @Test
    public void testResourceFindSpecificDeviceOnRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(RD) Resource Find (specific deivce) Test------------");
        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                TEST_RESOURCE_FIND_URI, "rt=core.light;di=" + di);
        mResHandler.onRequestReceived(mMockDevice, request);
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        // assertion: if the request packet from the CI contains the query
        // which includes device ID and the accesstoken
        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(queryMap.containsKey("di"));
        assertEquals(mReq.getUriPath(), Constants.WELL_KNOWN_FULL_URI);
    }

    private IResponse responseFromAccountServer() {
        // make response which has "CONTENT" status
        Cbor<HashMap<String, Object>> cbor = new Cbor<>();
        HashMap<String, Object> responsePayload = new HashMap<String, Object>();
        ArrayList<String> deviceList = new ArrayList<String>();
        // assuming that there are three devices in the response msg from the AS
        deviceList.add("device1");
        deviceList.add("device2");
        deviceList.add("device3");
        responsePayload.put("devices", deviceList);
        responsePayload.put("gid", "g0001");
        responsePayload.put("gmid", "u0001");
        ArrayList<String> midList = new ArrayList<String>();
        midList.add("u0001");
        responsePayload.put("memebers", midList);
        IResponse response = MessageBuilder.createResponse(requestEntireDevices,
                ResponseStatus.CONTENT, ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(responsePayload));
        return response;
    }
}
