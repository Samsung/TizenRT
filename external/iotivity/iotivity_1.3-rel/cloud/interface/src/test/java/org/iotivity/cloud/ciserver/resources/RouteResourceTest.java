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

package org.iotivity.cloud.ciserver.resources;

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
import org.iotivity.cloud.ciserver.DeviceServerSystem.CoapDevicePool;
import org.iotivity.cloud.ciserver.resources.RouteResource.AccountReceiveHandler;
import org.iotivity.cloud.ciserver.resources.RouteResource.DefaultResponseHandler;
import org.iotivity.cloud.ciserver.resources.RouteResource.LinkInterfaceHandler;
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
public class RouteResourceTest {
    private static final String           RELAY_URI                          = "/di";
    private static final String           RESOURCE_URI                       = "/a/light/0";
    private static final String           VERIFY_URI                         = "/oic/acl/verify";
    private String                        mDiServer                          = "resourceServerId";
    private CoapDevice                    mSourceDevice                      = mock(
            CoapDevice.class);
    private CoapDevice                    mTargetDevice                      = mock(
            CoapDevice.class);
    private IResponse                     mRes                               = null;
    private IRequest                      mReq                               = null;
    private DeviceServerSystem            mDeviceServerSystem                = new DeviceServerSystem();
    private final CountDownLatch          mLatch                             = new CountDownLatch(
            1);
    private Cbor<HashMap<String, Object>> mCbor                              = new Cbor<>();
    private IRequestChannel               mTargetChannel                     = mock(
            IRequestChannel.class);

    @Mock(name = "mASServer")
    IRequestChannel                       mRequestChannelASServer;

    @Mock
    CoapDevicePool                        coapDevicePool;

    @Mock
    IRequestChannel                       requestChannel;

    @InjectMocks
    RouteResource                            diHandler                          = new RouteResource(
            coapDevicePool);

    IRequest                              requestDefault                     = makePutRequest();
    IRequest                              requestLinkInterface               = makePutLinkInterfaceRequest();

    @InjectMocks
    LinkInterfaceHandler                  linkInterfaceHandler               = diHandler.new LinkInterfaceHandler(
            "targetDeviceId", mSourceDevice, requestLinkInterface);

    @InjectMocks
    DefaultResponseHandler                defaultResponseHandler             = diHandler.new DefaultResponseHandler(
            "targetDeviceId", mSourceDevice);

    @InjectMocks
    AccountReceiveHandler                 accountDefaultReceiveHandler       = diHandler.new AccountReceiveHandler(
            mSourceDevice, requestDefault);

    @InjectMocks
    AccountReceiveHandler                 accountLinkInterfaceReceiveHandler = diHandler.new AccountReceiveHandler(
            mSourceDevice, requestLinkInterface);

    @Before
    public void setUp() throws Exception {
        mRes = null;
        mReq = null;
        Mockito.doReturn("sourceDeviceId").when(mSourceDevice).getDeviceId();
        Mockito.doReturn("targetDeviceId").when(mTargetDevice).getDeviceId();
        MockitoAnnotations.initMocks(this);
        mDeviceServerSystem.addResource(diHandler);
        Mockito.doReturn(mTargetDevice).when(coapDevicePool)
                .queryDevice(Mockito.anyString());
        Mockito.doReturn(mTargetChannel).when(mTargetDevice)
                .getRequestChannel();

        // callback mock
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];
                mRes = resp;
                mLatch.countDown();
                return null;
            }
        }).when(mSourceDevice).sendResponse(Mockito.anyObject());
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
        }).when(mTargetChannel).sendRequest(Mockito.any(IRequest.class),
                Mockito.any(CoapDevice.class));
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
        PowerMockito.when(ConnectorPool.getConnection(Mockito.anyString())).thenReturn(mRequestChannelASServer);
    }

    @Test
    public void testOnDefaultRequestReceived() throws InterruptedException {
        IRequest request = makePutRequest();
        diHandler.onDefaultRequestReceived(mSourceDevice, request);
        assertTrue(mReq.getMethod().equals(RequestMethod.GET));
        assertTrue(mReq.getUriPath().equals(VERIFY_URI));
        assertTrue(mLatch.await(1L, SECONDS));
    }

    @Test
    public void testOnDefaultResponseHandleronResponseReceived()
            throws InterruptedException {
        IResponse response = makeContentResponse();
        defaultResponseHandler.onResponseReceived(response);
        assertEquals(mRes, response);
        assertTrue(mLatch.await(1L, SECONDS));
    }

    @Test
    public void testOnAccountReceiveHandlerDeniedonResponseReceived()
            throws InterruptedException {
        IResponse response = makeVerifyDeniedContentResponse();
        accountDefaultReceiveHandler.onResponseReceived(response);
    }

    @Test
    public void testOnAccountReceiveHandlerDefaultonResponseReceived()
            throws InterruptedException {
        IResponse response = makeVerifyAllowedContentResponse();
        accountDefaultReceiveHandler.onResponseReceived(response);
        assertEquals(mReq, requestDefault);
        assertTrue(mLatch.await(1L, SECONDS));
    }

    @Test
    public void testOnLinkInterfaceResponseHandleronResponseReceived()
            throws InterruptedException {
        IResponse response = makeVerifyAllowedContentResponse();
        accountLinkInterfaceReceiveHandler.onResponseReceived(response);
        assertEquals(mReq, requestLinkInterface);
        assertTrue(mLatch.await(1L, SECONDS));
    }

    @Test
    public void testOnAccountReceiveHandlerLinkInterfaceonResponseReceived()
            throws InterruptedException {
        IResponse response = makeContentLinkResponse();
        linkInterfaceHandler.onResponseReceived(response);
        assertEquals(mRes, response);
        assertTrue(mLatch.await(1L, SECONDS));
    }

    private IRequest makePutRequest() {
        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put("state", true);
        payloadData.put("power", 6);
        IRequest request = MessageBuilder.createRequest(RequestMethod.PUT,
                RELAY_URI + "/" + mDiServer + RESOURCE_URI, null,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return request;
    }

    private IRequest makePutLinkInterfaceRequest() {
        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put("state", true);
        payloadData.put("power", 6);
        IRequest request = MessageBuilder.createRequest(RequestMethod.PUT,
                RELAY_URI + "/" + mDiServer + RESOURCE_URI,
                Constants.RS_INTERFACE + "=" + Constants.LINK_INTERFACE,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return request;
    }

    private IResponse makeContentResponse() {

        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put("state", true);
        payloadData.put("power", 6);
        IResponse response = MessageBuilder.createResponse(makeGetRequest(),
                ResponseStatus.CONTENT, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return response;
    }

    private IResponse makeVerifyAllowedContentResponse() {

        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put("gp", "Allowed");
        IResponse response = MessageBuilder.createResponse(makeGetRequest(),
                ResponseStatus.CONTENT, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return response;
    }

    private IResponse makeVerifyDeniedContentResponse() {

        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put("gp", "Denied");
        IResponse response = MessageBuilder.createResponse(makeGetRequest(),
                ResponseStatus.CONTENT, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return response;
    }

    private IResponse makeContentLinkResponse() {
        HashMap<String, Object> payloadData = new HashMap<>();
        ArrayList<HashMap<String, Object>> linkPayload = new ArrayList<>();

        payloadData.put("href", "hrefsample1");
        linkPayload.add(payloadData);
        linkPayload.add(payloadData);
        linkPayload.add(payloadData);

        IResponse response = MessageBuilder.createResponse(makeGetRequest(),
                ResponseStatus.CONTENT, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(linkPayload));
        return response;
    }

    private IRequest makeGetRequest() {
        IRequest request = MessageBuilder.createRequest(RequestMethod.PUT,
                RELAY_URI + "/" + mDiServer + RESOURCE_URI, null);
        return request;
    }

}
