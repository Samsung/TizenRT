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
package org.iotivity.cloud.testrdserver;

import static java.util.concurrent.TimeUnit.SECONDS;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.mock;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.Observe;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.rdserver.Constants;
import org.iotivity.cloud.rdserver.resources.presence.device.DevicePresenceResource;
import org.iotivity.cloud.util.Cbor;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

public class DevicePresenceResourceTest {
    private Cbor<HashMap<String, Object>> mCbor                       = new Cbor<>();
    private DevicePresenceResource        mMockDevicePresenceResource = null;
    private CoapDevice                    mMockDevice                 = null;
    private CountDownLatch                mLatch                      = null;
    private IResponse                     mResponse;

    @Before
    public void setUp() throws Exception {
        mResponse = null;
        mMockDevice = mock(CoapDevice.class);
        mLatch = new CountDownLatch(1);
        mMockDevicePresenceResource = new DevicePresenceResource();
        // callback mock
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                CoapResponse resp = (CoapResponse) invocation.getArguments()[0];
                mLatch.countDown();
                mResponse = resp;
                return null;
            }
        }).when(mMockDevice).sendResponse(Mockito.anyObject());
    }

    @After
    public void tearDown() throws Exception {
        RDServerTestUtils.resetRDDatabase();
    }

    private IRequest makePresenceGetRequest(Observe obs) {
        String query = Constants.DEVICE_ID + "=" + RDServerTestUtils.DI;
        IRequest request = null;
        if (obs.compareTo(Observe.SUBSCRIBE) == 0) {
            request = MessageBuilder.createRequest(RequestMethod.GET,
                    RDServerTestUtils.DEVICE_PRS_REQ_URI, query);
        } else if (obs.compareTo(Observe.UNSUBSCRIBE) == 0) {
            request = MessageBuilder.createRequest(RequestMethod.GET,
                    RDServerTestUtils.DEVICE_PRS_REQ_URI, query);
        }
        ((CoapRequest) request).setObserve(obs);
        return request;
    }

    @Test
    public void testSubscribeRequest() throws Exception {
        System.out.println("\t------testHandleGetSubscribeRequest");
        IRequest request = makePresenceGetRequest(Observe.SUBSCRIBE);
        mMockDevicePresenceResource.onDefaultRequestReceived(mMockDevice,
                request);
        // assertion: if the response status is "CONTENT"
        assertTrue(mLatch.await(2L, SECONDS));
        assertTrue(checkResponseCode(mResponse, ResponseStatus.CONTENT));
        assertTrue(checkPayloadProperty(mResponse, Constants.DEVICE_ID,
                RDServerTestUtils.DI));
        assertTrue(checkPayloadProperty(mResponse, Constants.PRESENCE_STATE,
                Constants.PRESENCE_OFF));
    }

    @Test
    public void testUnsubscribeRequest() throws Exception {
        System.out.println("\t------testHandleGetUnsubscribeRequest");
        IRequest request = makePresenceGetRequest(Observe.UNSUBSCRIBE);
        mMockDevicePresenceResource.onDefaultRequestReceived(mMockDevice,
                request);
        // assertion: if the response status is "CONTENT"
        assertTrue(mLatch.await(2L, SECONDS));
        assertTrue(checkResponseCode(mResponse, ResponseStatus.CONTENT));
        assertTrue(checkPayloadProperty(mResponse, Constants.DEVICE_ID,
                RDServerTestUtils.DI));
        assertTrue(checkPayloadProperty(mResponse, Constants.PRESENCE_STATE,
                Constants.PRESENCE_OFF));
    }

    @Test
    public void testSubscribeRequest_existDevice() throws Exception {
        System.out.println("\t------testSubscribeRequest_existDevice");
        CoapDevice observerDevice = mock(CoapDevice.class);
        CountDownLatch observerLatch = new CountDownLatch(2);
        // callback mock for observer Device
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                CoapResponse response = (CoapResponse) invocation
                        .getArguments()[0];
                observerLatch.countDown();
                // assertion for observer device (subscribe response)
                if (observerLatch.getCount() == 1) {
                    assertTrue(checkResponseCode(response,
                            ResponseStatus.CONTENT));
                }
                if (observerLatch.getCount() == 0) {
                    assertTrue(checkResponseCode(response,
                            ResponseStatus.CONTENT));
                    assertTrue(checkPayloadProperty(response,
                            Constants.DEVICE_ID, RDServerTestUtils.DI));
                    assertTrue(checkPayloadProperty(response,
                            Constants.PRESENCE_STATE, Constants.PRESENCE_ON));
                }

                return null;
            }

        }).when(observerDevice).sendResponse(Mockito.anyObject());
        // subscribe request (specific device)
        IRequest subRequest = makePresenceGetRequest(Observe.SUBSCRIBE);
        mMockDevicePresenceResource.onDefaultRequestReceived(observerDevice,
                subRequest);
        // POST device presence off
        HashMap<String, Object> payload = new HashMap<>();
        payload.put(Constants.DEVICE_ID, RDServerTestUtils.DI);
        payload.put(Constants.PRESENCE_STATE, Constants.PRESENCE_ON);
        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                RDServerTestUtils.DEVICE_PRS_REQ_URI, null,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payload));
        mMockDevicePresenceResource.onDefaultRequestReceived(mMockDevice,
                request);
        // assertion for resource server device : responseStatus is "CHANGED"
        assertTrue(mLatch.await(2L, SECONDS));
        assertTrue(observerLatch.await(2L, SECONDS));
        assertTrue(checkResponseCode(mResponse, ResponseStatus.CHANGED));
    }

    @Test
    public void testUnSubscribeRequest_existDevice() throws Exception {
        System.out.println("\t------testUnSubscribeRequest_existDevice");
        CoapDevice observerDevice = mock(CoapDevice.class);
        CountDownLatch observerLatch = new CountDownLatch(1);
        // callback mock for observer Device
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                CoapResponse response = (CoapResponse) invocation
                        .getArguments()[0];
                observerLatch.countDown();
                // assertion for observer device (subscribe response)
                if (observerLatch.getCount() == 0) {
                    assertTrue(checkResponseCode(response,
                            ResponseStatus.CONTENT));
                    assertTrue(checkPayloadProperty(response,
                            Constants.DEVICE_ID, RDServerTestUtils.DI));
                    assertTrue(checkPayloadProperty(response,
                            Constants.PRESENCE_STATE, Constants.PRESENCE_OFF));
                }

                return null;
            }

        }).when(observerDevice).sendResponse(Mockito.anyObject());
        // subscribe request (specific device)
        IRequest subRequest = makePresenceGetRequest(Observe.UNSUBSCRIBE);
        mMockDevicePresenceResource.onDefaultRequestReceived(observerDevice,
                subRequest);
        HashMap<String, Object> payload = new HashMap<>();
        payload.put(Constants.DEVICE_ID, RDServerTestUtils.DI);
        payload.put(Constants.PRESENCE_STATE, Constants.PRESENCE_OFF);
        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                RDServerTestUtils.DEVICE_PRS_REQ_URI, null,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payload));
        mMockDevicePresenceResource.onDefaultRequestReceived(mMockDevice,
                request);
        // assertion for resource server device : responseStatus is "CHANGED"
        assertTrue(mLatch.await(2L, SECONDS));
        assertTrue(observerLatch.await(2L, SECONDS));
        assertTrue(checkResponseCode(mResponse, ResponseStatus.CHANGED));
    }

    private boolean checkPayloadProperty(IResponse response,
            String propertyName, String propertyValue) {
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);

        ArrayList<HashMap<String, String>> prsList = (ArrayList<HashMap<String, String>>) payloadData
                .get(Constants.PRESENCE_LIST);

        HashMap<String, String> mapData = prsList.get(0);
        if (mapData.containsKey(propertyName)
                && mapData.get(propertyName).equals(propertyValue)) {
            return true;
        } else
            return false;
    }

    private boolean checkResponseCode(IResponse response,
            ResponseStatus responseStatus) {
        if (responseStatus == response.getStatus())
            return true;
        else
            return false;
    }

}