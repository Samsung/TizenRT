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
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.rdserver.resources.directory.rd.ResourceDirectoryResource;
import org.iotivity.cloud.util.Cbor;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

public class ResourceDirectoryResourceTest {
    private Cbor<HashMap<String, Object>> mCbor       = new Cbor<>();
    private ResourceDirectoryResource     mRDResource = null;
    private CoapDevice                    mockDevice  = null;
    CountDownLatch                        mLatch      = null;
    IResponse                             mResponse;

    @Before
    public void setUp() throws Exception {
        mResponse = null;
        mockDevice = mock(CoapDevice.class);
        mLatch = new CountDownLatch(1);
        mRDResource = new ResourceDirectoryResource();
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
    public void testPostRequest() throws Exception {
        System.out.println("\t------testPostRequest");
        mRDResource.onDefaultRequestReceived(mockDevice,
                RDServerTestUtils.makePublishRequest());
        // assertion: if the response status is "CHANGED" according to
        // the resource publication
        assertTrue(mLatch.await(2L, SECONDS));
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        // assertion : if the mandatory properties are received in the
        // response
        assertTrue(linkCheck(mResponse, "href"));
        assertTrue(linkCheck(mResponse, "rt"));
        assertTrue(linkCheck(mResponse, "if"));
        assertTrue(linkCheck(mResponse, "ins"));
        assertTrue(hashmapCheck(mResponse, "di"));
    }

    @Test
    public void testHandleDeleteRequestByDi_notExistVaule() throws Exception {
        System.out.println("\t------testHandleDeleteRequestByDi_notExistVaule");
        IRequest request = MessageBuilder.createRequest(RequestMethod.DELETE,
                RDServerTestUtils.RD_REQ_URI, "di=" + RDServerTestUtils.DI);
        mRDResource.onDefaultRequestReceived(mockDevice, request);
        // assertion: if the response status is "DELETED" according to the
        // resource publication
        assertTrue(mLatch.await(2L, SECONDS));
        assertTrue(methodCheck(mResponse, ResponseStatus.DELETED));
    }

    @Test
    public void testHandleDeleteRequestByDi_existVaule() throws Exception {
        System.out.println("\t------testHandleDeleteRequestByDi_existVaule");
        IRequest request = MessageBuilder.createRequest(RequestMethod.DELETE,
                RDServerTestUtils.RD_REQ_URI, "di=" + RDServerTestUtils.DI);
        mRDResource.onDefaultRequestReceived(mockDevice,
                RDServerTestUtils.makePublishRequest());
        mRDResource.onDefaultRequestReceived(mockDevice, request);
        // assertion: if the response status is "DELETED" according to the
        // resource publication
        assertTrue(mLatch.await(2L, SECONDS));
        assertTrue(methodCheck(mResponse, ResponseStatus.DELETED));
    }

    @Test
    public void testHandleDeleteRequestByIns_notExistVaule() throws Exception {
        System.out
                .println("\t------testHandleDeleteRequestByIns_notExistVaule");
        IRequest request = MessageBuilder.createRequest(RequestMethod.DELETE,
                RDServerTestUtils.RD_REQ_URI,
                "di=" + RDServerTestUtils.DI + ";ins=1");
        mRDResource.onDefaultRequestReceived(mockDevice, request);
        // assertion: if the response status is "DELETED" according to the
        // resource publication
        assertTrue(mLatch.await(2L, SECONDS));
        assertTrue(methodCheck(mResponse, ResponseStatus.DELETED));
    }

    @Test
    public void testHandleDeleteRequestByIns_existVaule() throws Exception {
        System.out.println("\t------testHandleDeleteRequestByIns_existVaule");
        IRequest request = MessageBuilder.createRequest(RequestMethod.DELETE,
                RDServerTestUtils.RD_REQ_URI,
                "di=" + RDServerTestUtils.DI + ";ins=1");
        mRDResource.onDefaultRequestReceived(mockDevice,
                RDServerTestUtils.makePublishRequest());
        mRDResource.onDefaultRequestReceived(mockDevice, request);
        // assertion: if the response status is "DELETED" according to the
        // resource publication
        assertTrue(mLatch.await(2L, SECONDS));
        assertTrue(methodCheck(mResponse, ResponseStatus.DELETED));
    }

    private boolean hashmapCheck(IResponse response, String propertyName) {
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);
        if (payloadData.get(propertyName) != null)
            return true;
        else
            return false;
    }

    private boolean linkCheck(IResponse response, String propertyName) {
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);
        ArrayList<HashMap<Object, Object>> linkData = (ArrayList<HashMap<Object, Object>>) payloadData
                .get("links");
        HashMap<Object, Object> linkMap = linkData.get(0);
        if (linkMap.get(propertyName) != null)
            return true;
        else
            return false;
    }

    private boolean methodCheck(IResponse response,
            ResponseStatus responseStatus) {
        if (responseStatus == response.getStatus())
            return true;
        else
            return false;
    }
}