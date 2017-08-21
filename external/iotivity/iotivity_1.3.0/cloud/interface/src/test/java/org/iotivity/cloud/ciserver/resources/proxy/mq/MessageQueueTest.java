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

package org.iotivity.cloud.ciserver.resources.proxy.mq;

import static java.util.concurrent.TimeUnit.SECONDS;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.mock;

import java.util.HashMap;
import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
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
public class MessageQueueTest {
    private static final String TEST_MQ_BROKER_URI = Constants.MQ_BROKER_FULL_URI;
    private CoapDevice           mMockDevice        = null;
    private IRequest             mReq               = null;
    private DeviceServerSystem   mDeviceServerSystem;
    private final CountDownLatch mLatch             = new CountDownLatch(1);
    @Mock
    private IRequestChannel      mRequestChannel;
    @InjectMocks
    private MessageQueue         mMqHandler         = new MessageQueue();

    @Before
    public void setUp() throws Exception {
        mReq = null;
        MockitoAnnotations.initMocks(this);
        mDeviceServerSystem = new DeviceServerSystem();
        mDeviceServerSystem.addResource(mMqHandler);
        mMockDevice = mock(CoapDevice.class);
        // callback requestmsg mock from (IRequestChannel) server
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapRequest answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapRequest request = (CoapRequest) args[0];
                mReq = request;
                mLatch.countDown();
                return null;
            }
        }).when(mRequestChannel).sendRequest(Mockito.any(IRequest.class),
                Mockito.any(CoapDevice.class));

        PowerMockito.mockStatic(ConnectorPool.class);
        PowerMockito.when(ConnectorPool.getConnection(Mockito.anyString())).thenReturn(mRequestChannel);
    }

    @Test
    public void testMessageQueueResourceOnRequestReceived() throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(MQ) Topic Creation Test------------");
        IRequest request = null;
        HashMap<Object, Object> tags = new HashMap<Object, Object>();
        tags.put("topic", "DeviceServerSystemtestTopic");
        Cbor<HashMap<Object, Object>> cbor = new Cbor<HashMap<Object, Object>>();
        request = MessageBuilder.createRequest(RequestMethod.PUT,
                TEST_MQ_BROKER_URI, null, ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(tags));
        mDeviceServerSystem.onRequestReceived(mMockDevice, request);
        // assertion : request msg to the AS is identical to the request msg
        // from the client
        assertTrue(mLatch.await(1L, SECONDS));
        assertEquals(request, mReq);
    }

}
