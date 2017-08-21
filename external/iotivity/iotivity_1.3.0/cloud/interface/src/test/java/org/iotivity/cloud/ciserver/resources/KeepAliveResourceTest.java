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

import static org.junit.Assert.assertEquals;
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
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.util.Cbor;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

public class KeepAliveResourceTest {

    private KeepAliveResource                         keepAliveResource;
    private CoapDevice                                mockDevice;
    private Cbor<HashMap<String, ArrayList<Integer>>> mCbor     = new Cbor<>();
    private final CountDownLatch                      mLatch    = new CountDownLatch(
            1);
    private IResponse                                 mRes;
    private int[]                                     mInterval = { 1, 2, 4,
            8 };

    @Before
    public void setUp() throws Exception {
        keepAliveResource = new KeepAliveResource(mInterval);
        MockitoAnnotations.initMocks(this);
        mockDevice = mock(CoapDevice.class);
        mRes = null;
        // callback mock
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];
                System.out
                        .println("\t----payload : " + resp.getPayloadString());
                System.out
                        .println("\t----responsestatus : " + resp.getStatus());
                mRes = resp;
                return null;
            }
        }).when(mockDevice).sendResponse(Mockito.anyObject());
        keepAliveResource.startSessionChecker(0, 3000);
    }

    @After
    public void Endup() throws Exception {
        keepAliveResource.stopSessionChecker();
    }

    @Test
    public void testOnDefaultGetRequestReceived() {
        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                "/oic/ping", null);
        keepAliveResource.onDefaultRequestReceived(mockDevice, request);
        HashMap<String, ArrayList<Integer>> payloadData = mCbor
                .parsePayloadFromCbor(mRes.getPayload(), HashMap.class);
        HashMap<String, ArrayList<Integer>> comparisonData = new HashMap<>();
        ArrayList<Integer> comparisonArray = new ArrayList<>();
        comparisonArray.add(1);
        comparisonArray.add(2);
        comparisonArray.add(4);
        comparisonArray.add(8);
        comparisonData.put("inarray", comparisonArray);
        assertEquals(payloadData, comparisonData);
        assertTrue(methodCheck(mRes, ResponseStatus.CONTENT));
    }

    @Test
    public void testOnDefaultPutRequestReceived() {
        HashMap<String, Integer> payloadData = new HashMap<>();
        Cbor<HashMap<String, Object>> cbor = new Cbor<>();
        payloadData.put("in", 8);
        IRequest request = MessageBuilder.createRequest(RequestMethod.PUT,
                "/oic/ping", null, ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(payloadData));
        keepAliveResource.onDefaultRequestReceived(mockDevice, request);
        assertTrue(methodCheck(mRes, ResponseStatus.VALID));
    }

    private boolean methodCheck(IResponse response,
            ResponseStatus responseStatus) {
        if (responseStatus == response.getStatus())
            return true;
        else
            return false;
    }
}
