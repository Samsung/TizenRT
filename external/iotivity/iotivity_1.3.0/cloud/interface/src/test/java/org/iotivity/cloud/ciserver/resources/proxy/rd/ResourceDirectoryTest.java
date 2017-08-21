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
import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.device.IRequestChannel;
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
public class ResourceDirectoryTest {
    private static final String TEST_RD_URI         = Constants.RD_FULL_URI;
    public static final String  DEVICE_LIST_KEY     = "devices";

    private String              mDi                 = "B371C481-38E6-4D47-8320-7688D8A5B58C";
    String                      mUserId             = "testuser";
    private CoapDevice          mMockDevice         = null;
    IRequest                    mReqRDServer        = null;
    IRequest                    mReqASServer        = null;
    DeviceServerSystem          mDeviceServerSystem = new DeviceServerSystem();
    final CountDownLatch        mLatch              = new CountDownLatch(1);
    @Mock(name = "mRDServer")
    IRequestChannel             mRequestChannelRDServer;
    @Mock(name = "mASServer")
    IRequestChannel             mRequestChannelASServer;

    @InjectMocks
    ResourceDirectory           mRdHandler          = new ResourceDirectory();

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mDeviceServerSystem.addResource(mRdHandler);
        mMockDevice = mock(CoapDevice.class);
        Mockito.doReturn(mUserId).when(mMockDevice).getUserId();
        Mockito.doReturn(mDi).when(mMockDevice).getDeviceId();

        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapRequest answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapRequest request = (CoapRequest) args[0];
                System.out.println("\t----------mRDServer --------------");
                System.out.println(
                        "\t----------payload : " + request.getPayloadString());
                System.out.println("\t----------uripath : "
                        + request.getMethod() + " " + request.getUriPath());
                System.out.println(
                        "\t---------uriquery : " + request.getUriQuery());
                mReqRDServer = request;
                mLatch.countDown();
                return request;
            }
        }).when(mRequestChannelRDServer).sendRequest(
                Mockito.any(IRequest.class), Mockito.any(CoapDevice.class));

        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapRequest answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapRequest request = (CoapRequest) args[0];
                System.out.println("\t----------mASServer --------------");
                System.out.println(
                        "\t----------payload : " + request.getPayloadString());
                System.out.println("\t----------uripath : "
                        + request.getMethod() + " " + request.getUriPath());
                System.out.println(
                        "\t---------uriquery : " + request.getUriQuery());
                mReqASServer = request;
                mLatch.countDown();
                return request;
            }
        }).when(mRequestChannelASServer).sendRequest(
                Mockito.any(IRequest.class), Mockito.any(CoapDevice.class));

        PowerMockito.mockStatic(ConnectorPool.class);
        PowerMockito.when(ConnectorPool.getConnection("account")).thenReturn(mRequestChannelASServer);
        PowerMockito.when(ConnectorPool.getConnection("rd")).thenReturn(mRequestChannelRDServer);
    }

    @Test
    public void testRDResourcePublishOnRequestReceived() throws Exception {
        IRequest request = makeResourcePublishRequest();
        mDeviceServerSystem.onRequestReceived(mMockDevice, request);
        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(mReqASServer.getMethod().equals(RequestMethod.POST));
        assertTrue(mReqASServer.getUriPath()
                .contains(Constants.GROUP_FULL_URI + "/"));
    }

    @Test
    public void testRDResourceDeleteOnRequestReceived() throws Exception {
        IRequest request = makeResourceDeleteRequest();
        mDeviceServerSystem.onRequestReceived(mMockDevice, request);
        assertTrue(mLatch.await(1L, SECONDS));
        assertTrue(mReqRDServer.getMethod().equals(RequestMethod.DELETE));
        assertTrue(mReqRDServer.getUriPath().contains(TEST_RD_URI));
    }

    IRequest  rdPublishRequest  = makeResourcePublishRequest();
    IResponse rdPublishResponse = makeResourcePublishResponse();

    @Test
    public void testRDResourcePublishOnResponseReceived() throws Exception {

        ResourceDirectory.AccountReceiveHandler accountReceiveHandler = mRdHandler.new AccountReceiveHandler(
                rdPublishRequest, mMockDevice);

        IRequest request = makeResourcePublishRequest();
        accountReceiveHandler.onResponseReceived(
                MessageBuilder.createResponse(request, ResponseStatus.CHANGED));

        assertEquals(mReqRDServer, rdPublishRequest);
        assertTrue(mLatch.await(1L, SECONDS));
    }

    @Test
    public void testRDResourcePublishPayloadConverted() throws Exception {

        ResourceDirectory.AccountReceiveHandler accountReceiveHandler = mRdHandler.new AccountReceiveHandler(
                rdPublishRequest, mMockDevice);

        IRequest request = makeResourcePublishRequest();
        accountReceiveHandler.onResponseReceived(
                MessageBuilder.createResponse(request, ResponseStatus.CHANGED));

        assertEquals(getHrefInTestPublishPayload(mReqRDServer.getPayload()),
                "/oic/route/" + mDi + "/a/light");
    }

    @Test
    public void testRDResourcePublishResponse() throws Exception {

        ResourceDirectory.PublishResponseHandler publishResponseHandler = mRdHandler.new PublishResponseHandler(
                mMockDevice);

        CountDownLatch latch = new CountDownLatch(1);

        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];

                assertEquals(getHrefInTestPublishPayload(resp.getPayload()),
                        "/a/light");

                latch.countDown();
                return resp;
            }
        }).when(mMockDevice).sendResponse(Mockito.anyObject());

        publishResponseHandler
                .onResponseReceived(makeResourcePublishResponse());

        assertTrue(latch.await(1L, SECONDS));
    }

    private IRequest makeResourcePublishRequest() {
        HashMap<Object, Object> payload = new HashMap<>();
        payload.put(Constants.DEVICE_ID, mDi);
        ArrayList<HashMap<Object, Object>> publishLinks = new ArrayList<>();
        HashMap<Object, Object> link = new HashMap<>();
        link.put("href", "/a/light");
        ArrayList<String> rt = new ArrayList<String>();
        rt.add("core.light");
        ArrayList<String> itf = new ArrayList<String>();
        itf.add("oic.if.baseline");
        HashMap<String, Object> policy = new HashMap<>();
        policy.put("bm", 5);
        link.put("rt", rt);
        link.put("if", itf);
        link.put("p", policy);
        publishLinks.add(link);
        payload.put("links", publishLinks);
        Cbor<HashMap<Object, Object>> cbor = new Cbor<>();
        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                TEST_RD_URI, "rt" + "=" + "oic.wk.rdpub",
                ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(payload));
        return request;
    }

    private IResponse makeResourcePublishResponse() {
        HashMap<Object, Object> payload = new HashMap<>();
        payload.put(Constants.DEVICE_ID, mDi);
        ArrayList<HashMap<Object, Object>> publishLinks = new ArrayList<>();
        HashMap<Object, Object> link = new HashMap<>();
        link.put("href", "/oic/route/" + mDi + "/a/light");
        ArrayList<String> rt = new ArrayList<String>();
        rt.add("core.light");
        ArrayList<String> itf = new ArrayList<String>();
        itf.add("oic.if.baseline");
        HashMap<String, Object> policy = new HashMap<>();
        policy.put("bm", 5);
        link.put("rt", rt);
        link.put("if", itf);
        link.put("p", policy);
        publishLinks.add(link);
        payload.put("links", publishLinks);

        Cbor<HashMap<Object, Object>> cbor = new Cbor<>();
        return MessageBuilder.createResponse(makeResourcePublishRequest(),
                ResponseStatus.CHANGED, ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(payload));
    }

    private IRequest makeResourceDeleteRequest() {
        IRequest request = MessageBuilder.createRequest(RequestMethod.DELETE,
                TEST_RD_URI, "di" + "=" + mDi + ";" + "ins" + "=" + "1234",
                null, null);
        return request;
    }

    private String getHrefInTestPublishPayload(byte[] payload) {

        Cbor<HashMap<String, Object>> cbor = new Cbor<>();
        HashMap<String, Object> parsedPayload = cbor
                .parsePayloadFromCbor(payload, HashMap.class);

        @SuppressWarnings("unchecked")
        ArrayList<HashMap<String, Object>> links = (ArrayList<HashMap<String, Object>>) parsedPayload
                .get(Constants.REQ_LINKS);

        return (String) links.get(0).get(Constants.REQ_HREF);
    }

}
