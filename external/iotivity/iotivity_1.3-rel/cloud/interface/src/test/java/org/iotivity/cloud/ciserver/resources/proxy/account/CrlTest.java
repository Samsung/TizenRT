package org.iotivity.cloud.ciserver.resources.proxy.account;

import static java.util.concurrent.TimeUnit.SECONDS;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.mock;

import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.base.OICConstants;
import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.ciserver.Constants;
import org.iotivity.cloud.ciserver.DeviceServerSystem;
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
public class CrlTest {

    private static final String TEST_RESOURCE_CRL_URI = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.CREDPROV_URI + "/"
            + Constants.REQ_CRL;
    private CoapDevice          mMockDevice           = mock(CoapDevice.class);
    private IRequest            mReq                  = null;
    private DeviceServerSystem  mDeviceServerSystem   = new DeviceServerSystem();
    final CountDownLatch        mLatch                = new CountDownLatch(1);

    @Mock
    private IRequestChannel     mRequestChannel;

    @InjectMocks
    private Crl                 mCertHandler          = new Crl();

    @Before
    public void setUp() throws Exception {
        mReq = null;
        MockitoAnnotations.initMocks(this);
        mDeviceServerSystem.addResource(mCertHandler);
        // callback mock
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
        }).when(mRequestChannel).sendRequest(Mockito.any(IRequest.class),
                Mockito.any(CoapDevice.class));

        PowerMockito.mockStatic(ConnectorPool.class);
        PowerMockito.when(ConnectorPool.getConnection(Mockito.anyString())).thenReturn(mRequestChannel);
    }

    @Test
    public void testOnDefaultRequestReceived() throws Exception {
        System.out.println(
                "\t--------------OnRequestReceived(AS) Test------------");

        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                TEST_RESOURCE_CRL_URI, null, null, null);
        mCertHandler.onRequestReceived(mMockDevice, request);

        assertTrue(mLatch.await(1L, SECONDS));
        assertEquals(mReq.getUriPath(), TEST_RESOURCE_CRL_URI);
    }

}
