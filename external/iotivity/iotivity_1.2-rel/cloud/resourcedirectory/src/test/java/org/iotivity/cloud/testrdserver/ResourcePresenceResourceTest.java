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

import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.rdserver.resources.directory.rd.ResourceDirectoryResource;
import org.iotivity.cloud.rdserver.resources.presence.resource.ResPresenceResource;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

public class ResourcePresenceResourceTest {
    private ResourceDirectoryResource mRDResource          = null;
    private ResPresenceResource       mResPresenceResource = null;
    private CoapDevice                mockDevice           = null;
    private CountDownLatch            mLatch               = null;
    private IResponse                 mResponse;

    @Before
    public void setUp() throws Exception {
        mRDResource = new ResourceDirectoryResource();
        mResPresenceResource = new ResPresenceResource();
        mResponse = null;
        mockDevice = mock(CoapDevice.class);
        mLatch = new CountDownLatch(1);
        // callback mock
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                CoapResponse resp = (CoapResponse) invocation.getArguments()[0];
                mLatch.countDown();
                mResponse = resp;
                return resp;
            }
        }).when(mockDevice).sendResponse(Mockito.anyObject());
    }

    @After
    public void tearDown() throws Exception {
        RDServerTestUtils.resetRDDatabase();
    }

    @Test
    public void testHandleGetObserveRequest_notExistValue() throws Exception {
        System.out.println("\t------testHandleGetObserveRequest_notExistValue");
        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                RDServerTestUtils.RES_PRS_URI, "di=" + RDServerTestUtils.DI);
        mResPresenceResource.onDefaultRequestReceived(mockDevice, request);
        assertTrue(mLatch.await(2L, SECONDS));
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
    }

    @Test
    public void testHandleGetObserveRequest_ExistValue() throws Exception {
        System.out.println("\t------testHandleGetObserveRequest_ExistValue");
        CoapDevice observerDevice = mock(CoapDevice.class);
        CountDownLatch observerLatch = new CountDownLatch(2);
        // callback mock for observer Device
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                CoapResponse resp = (CoapResponse) invocation.getArguments()[0];
                observerLatch.countDown();
                // assertion for observer device (resource presence response)
                if (observerLatch.getCount() == 1) {
                    assertTrue(methodCheck(resp, ResponseStatus.CONTENT));
                }
                // assertion for observer device (resource presence response
                // when the
                // resource is published)
                if (observerLatch.getCount() == 0) {
                    String payload = new String(resp.getPayload());
                    assertTrue(methodCheck(resp, ResponseStatus.CONTENT));
                    assertTrue(payload.contains("non"));
                    assertTrue(payload.contains("ttl"));
                    assertTrue(payload.contains("trg"));
                    assertTrue(payload.contains("rt"));
                    assertTrue(payload.contains("href"));
                }
                return null;
            }
        }).when(observerDevice).sendResponse(Mockito.anyObject());
        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                RDServerTestUtils.RES_PRS_URI, "di=" + RDServerTestUtils.DI);
        // observer : resource presence request (from CoapDevice observeDevice)
        // observe specific device
        mResPresenceResource.onDefaultRequestReceived(observerDevice, request);
        // resource publish (from CoapDevice mockDevice)
        mRDResource.onDefaultRequestReceived(mockDevice,
                RDServerTestUtils.makePublishRequest());
        // assertion: if the response status is "CHANGED"
        assertTrue(mLatch.await(2L, SECONDS));
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
    }

    private boolean methodCheck(IResponse response,
            ResponseStatus responseStatus) {
        if (responseStatus == response.getStatus())
            return true;
        else
            return false;
    }
}