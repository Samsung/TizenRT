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
public class ResourcePresenceTest {
    public static final String DEVICE_PRS_REQ_URI = Constants.DEVICE_PRESENCE_FULL_URI;
    public static final String RES_PRS_URI        = Constants.RESOURCE_PRESENCE_FULL_URI;
    private String             mDi                = "B371C481-38E6-4D47-8320-7688D8A5B58C";
    private CoapDevice         mMockDevice        = mock(CoapDevice.class);
    private IResponse          mRes               = null;
    private IRequest           mReq               = null;
    private DeviceServerSystem deviceServerSystem = new DeviceServerSystem();
    final CountDownLatch       latch              = new CountDownLatch(1);
    @Mock
    private IRequestChannel    mRequestChannel;
    @InjectMocks
    private ResourcePresence   adHandler          = new ResourcePresence();

    @Before
    public void setUp() throws Exception {
        mRes = null;
        mReq = null;
        Mockito.doReturn("mockDeviceId").when(mMockDevice).getDeviceId();
        Mockito.doReturn("mockUserId").when(mMockDevice).getUserId();
        MockitoAnnotations.initMocks(this);
        deviceServerSystem.addResource(adHandler);
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
                latch.countDown();
                return request;
            }
        }).when(mRequestChannel).sendRequest(Mockito.any(IRequest.class),
                Mockito.any(CoapDevice.class));

        PowerMockito.mockStatic(ConnectorPool.class);
        PowerMockito.when(ConnectorPool.getConnection(Mockito.anyString())).thenReturn(mRequestChannel);
    }

    @Test
    public void testResourcePresenceEntireResourcesOnRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(RD) Resource Presence (entire resource) Test------------");
        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                RES_PRS_URI, null);
        deviceServerSystem.onRequestReceived(mMockDevice, request);
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        // assertion: if the request packet from the CI contains the query
        // which includes the accesstoken and the di
        assertTrue(latch.await(1L, SECONDS));
        assertTrue(queryMap.containsKey("uid"));
        assertTrue(queryMap.containsKey("members"));
        assertEquals(mReq.getUriPath(),
                Constants.GROUP_FULL_URI + "/mockUserId");
    }

    @Test
    public void testResourcePresenceSpecificResourceOnRequestReceived()
            throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(RD) Resource Presence (specific resources) Test------------");
        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                RES_PRS_URI, "di=" + mDi);
        deviceServerSystem.onRequestReceived(mMockDevice, request);
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        assertTrue(latch.await(1L, SECONDS));
        assertTrue(queryMap.containsKey("uid"));
        assertTrue(queryMap.containsKey("members"));
        assertEquals(mReq.getUriPath(),
                Constants.GROUP_FULL_URI + "/mockUserId");
    }

    IRequest                               requestEntireDevices = MessageBuilder
            .createRequest(RequestMethod.GET, RES_PRS_URI, null);
    @InjectMocks
    ResourcePresence.AccountReceiveHandler entireDeviceHandler  = adHandler.new AccountReceiveHandler(
            requestEntireDevices, mMockDevice);

    @Test
    public void testEntireDeviceonResponseReceived() throws ClientException {
        System.out.println(
                "\t--------------onResponseReceived(RD) Resource Presence (entire deivces) Test------------");
        IResponse responseFromAccountServer = responseFromAccountServer();
        entireDeviceHandler.onResponseReceived(responseFromAccountServer);
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        // assertion : if query has pre-requested multiple di list given from
        // the AS
        assertTrue(mReq.getMethod() == RequestMethod.GET);
        assertTrue(queryMap.get("di").contains("device1"));
        assertTrue(queryMap.get("di").contains("device2"));
        assertTrue(queryMap.get("di").contains("device3"));
    }

    IRequest                               requestSpecificDevice = MessageBuilder
            .createRequest(RequestMethod.GET, RES_PRS_URI, "di=" + "device1");
    @InjectMocks
    ResourcePresence.AccountReceiveHandler specificDeviceHandler = adHandler.new AccountReceiveHandler(
            requestSpecificDevice, mMockDevice);

    @Test
    public void testSpecificDeviceonResponseReceived() throws ClientException {
        System.out.println(
                "\t--------------onResponseReceived(RD) Resource Presence (specific deivce) Test------------");
        IResponse response = responseFromAccountServer();
        specificDeviceHandler.onResponseReceived(response);
        HashMap<String, List<String>> queryMap = mReq.getUriQueryMap();
        // assertion : if query has pre-requested di
        assertTrue(queryMap.get("di").contains("device1"));
        assertTrue(mReq.getMethod() == RequestMethod.GET);
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
        ArrayList<String> midList = new ArrayList<String>();
        midList.add("u0001");
        responsePayload.put("members", midList);
        IResponse response = MessageBuilder.createResponse(requestEntireDevices,
                ResponseStatus.CONTENT, ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(responsePayload));
        return response;
    }
}
