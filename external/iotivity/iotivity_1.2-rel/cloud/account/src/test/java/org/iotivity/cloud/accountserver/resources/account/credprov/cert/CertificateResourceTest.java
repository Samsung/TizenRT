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
package org.iotivity.cloud.accountserver.resources.account.credprov.cert;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.db.TokenTable;
import org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants;
import org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateResource;
import org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateStorage;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;
import org.iotivity.cloud.accountserver.x509.cert.Utility;
import org.iotivity.cloud.base.OICConstants;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.util.Cbor;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.security.cert.X509Certificate;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.CountDownLatch;

import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.CERTIFICATE_FACTORY;
import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.KEYSTORE_FILE;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.mock;

public class CertificateResourceTest {

    public static final String COMMON_NAME = "OU=OCF Device CA, O=Samsung, C=KR, CN=uuid:B371C481-38E6-4D47-8320-7688D8A5B58C";
    public static final String DEVICE_ID = "B371C481-38E6-4D47-8320-7688D8A5B58C";
    private static final String CERTIFICATE_URI = OICConstants.CREDPROV_CERT_FULL_URI;
    private static CertificateResource certificateResource = new CertificateResource();
    private static TypeCastingManager<TokenTable> castingManager = new TypeCastingManager<>();
    private CoapDevice mMockDevice = mock(CoapDevice.class);
    private Cbor<HashMap<String, Object>> mCbor = new Cbor<>();
    private IResponse mResponse = null;
    private CountDownLatch mLatch = new CountDownLatch(
            1);
    private byte[] csr;

    static void createToken() {
        TokenTable certificateTable = new TokenTable();
        certificateTable.setDid(DEVICE_ID);
        certificateTable.setUuid("uuid:" + DEVICE_ID);
        AccountDBManager.getInstance().insertRecord(Constants.TOKEN_TABLE,
                castingManager.convertObjectToMap(certificateTable));
    }

    @AfterClass
    public static void after() {
        KEYSTORE_FILE.delete();
        AccountDBManager.getInstance().deleteRecord(Constants.CERTIFICATE_TABLE, new HashMap<>());
    }

    @BeforeClass
    public static void setUpBefore() {
        createToken();
    }

    /**
     * DERSequence othernameSequence = new DERSequence(new ASN1Encodable[]{
     * new ASN1ObjectIdentifier("1.3.6.1.5.5.7.8.5"), new DERTaggedObject(true, 0, new DERUTF8String(subjectAltName))});
     * GeneralName othernameGN = new GeneralName(GeneralName.otherName, othernameSequence);
     * ASN1Encodable[] subjectAlternativeNames = new ASN1Encodable[]
     * {
     * othernameGN
     * <p>
     * <p>
     * };
     * DERSequence subjectAlternativeNamesExtension = new DERSequence(othernameGN);
     * return buildOne(Extension.subjectAlternativeName, true, subjectAlternativeNamesExtension, issuerNameBld.build());
     */
    @Test
    public void testLoad() {
        try {
            Method m = CertificateStorage.class.getDeclaredMethod("load");
            m.setAccessible(true);
            m.invoke(null);
        } catch (IllegalAccessException | InvocationTargetException | NoSuchMethodException e) {
            e.printStackTrace();
        }
    }

    @Before
    public void setUp() throws Exception {
        csr = GenerateCSR.generatePKCS10(COMMON_NAME, false);
        MockitoAnnotations.initMocks(this);
        // callback mock
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];
                mResponse = resp;
                mLatch.countDown();
                return null;
            }
        }).when(mMockDevice).sendResponse(Mockito.anyObject());
    }

    @Test(expected = AssertionError.class)
    public void testErrorConstruction() {
        Constructor<CertificateStorage> constructor;
        try {
            constructor = CertificateStorage.class.getDeclaredConstructor();
            constructor.setAccessible(true);
            constructor.newInstance();
        } catch (NoSuchMethodException | IllegalAccessException | InstantiationException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            throw new AssertionError();
        }

    }

    @Test(expected = AssertionError.class)
    public void testErrorConstructionUtility() {
        Constructor<Utility> constructor;
        try {
            constructor = Utility.class.getDeclaredConstructor();
            constructor.setAccessible(true);
            constructor.newInstance();
        } catch (NoSuchMethodException | IllegalAccessException | InstantiationException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            throw new AssertionError();
        }
    }

    @Test(expected = AssertionError.class)
    public void testErrorConstructionCertificateConstants() {
        Constructor<CertificateConstants> constructor;
        try {
            constructor = CertificateConstants.class.getDeclaredConstructor();
            constructor.setAccessible(true);
            constructor.newInstance();
        } catch (NoSuchMethodException | IllegalAccessException | InstantiationException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            throw new AssertionError();
        }

    }

    /**
     * DERUTF8String derDomainName = new DERUTF8String("hello.world");
     * DERTaggedObject derTaggedDomainName = new DERTaggedObject(0, derDomainName);
     * DLSequence otherName = new DLSequence(new ASN1Encodable[]{new ASN1ObjectIdentifier("1.3.6.1.5.5.7.9.3"), derTaggedDomainName});
     * GeneralNames generalNames = new GeneralNames(new GeneralName(GeneralName.otherName, otherName));
     *
     * @throws Exception
     */

    @Test
    public void testCSRIssueDER() throws Exception {
        getTestMethodName();
        IRequest request = csrRequest(DEVICE_ID, CertificateConstants.DER, csr, RequestMethod.POST, true);
        certificateResource.onDefaultRequestReceived(mMockDevice, request);
        Map<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(mResponse.getPayload(), HashMap.class);
        assertTrue(hashmapCheck(mResponse, Constants.REQ_DEVICE_ID));
        assertTrue(hashmapCheck(mResponse, Constants.CERT));
        assertTrue(hashmapCheck(mResponse, Constants.CERT_CHAIN));
        Map<String, Object> certMap = (Map<String, Object>) payloadData.get(Constants.CERT);
        InputStream in = new ByteArrayInputStream((byte[]) certMap.get(Constants.DATA));
        X509Certificate personaleCert = (X509Certificate) CERTIFICATE_FACTORY.generateCertificate(in);
        personaleCert.verify(CertificateStorage.ROOT_CERTIFICATE.getPublicKey());
        assertEquals(COMMON_NAME, personaleCert.getSubjectDN().getName());
        String encoding = certMap.get(Constants.ENCODING).toString();
        assertEquals(CertificateConstants.DER, encoding);
        assertEquals(personaleCert.getPublicKey(), GenerateCSR.getPublicKey());
        certMap = (Map<String, Object>) payloadData.get(Constants.CERT_CHAIN);
        encoding = certMap.get(Constants.ENCODING).toString();
        assertEquals(CertificateConstants.DER, encoding);
        in = new ByteArrayInputStream((byte[]) certMap.get(Constants.DATA));
        X509Certificate caCert = (X509Certificate) CERTIFICATE_FACTORY.generateCertificate(in);
        assertEquals(caCert, CertificateStorage.ROOT_CERTIFICATE);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
    }

    @Test
    public void testCSRIssueDERFailed() throws Exception {
        getTestMethodName();
        IRequest request = csrRequestFailed(DEVICE_ID, RequestMethod.POST);
        certificateResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.BAD_REQUEST));
    }

    @Test
    public void testCSRIssueDERFailedSignature() throws Exception {
        byte[] csr = GenerateCSR.generatePKCS10(COMMON_NAME, true);
        getTestMethodName();
        IRequest request = csrRequest(DEVICE_ID, CertificateConstants.DER, csr, RequestMethod.POST, true);
        certificateResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.BAD_REQUEST));
    }

    @Test
    public void testMethodNotAllowed() {
        IRequest request = csrRequest(DEVICE_ID, CertificateConstants.DER, csr, RequestMethod.GET, true);
        certificateResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.METHOD_NOT_ALLOWED));
    }

    @Test
    public void testBadRequest() {
        IRequest request = csrRequest(DEVICE_ID, CertificateConstants.DER, csr, RequestMethod.POST, false);
        certificateResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.BAD_REQUEST));
    }


    @Test
    public void testBadData() {
        IRequest request = csrRequest(DEVICE_ID, CertificateConstants.DER, null, RequestMethod.POST, true);
        certificateResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.BAD_REQUEST));
    }


    private void getTestMethodName() {
        StackTraceElement[] stacks = new Throwable().getStackTrace();
        StackTraceElement currentStack = stacks[1];
        System.out.println("\t---Test Name : " + currentStack.getMethodName());
    }

    private IRequest csrRequestFailed(String deviceId, RequestMethod method) {
        IRequest request;
        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put(Constants.REQ_DEVICE_ID, deviceId);
        payloadData.put("csr", "csr");
        request = MessageBuilder.createRequest(method, CERTIFICATE_URI,
                null, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return request;
    }

    private IRequest csrRequest(String deviceId, String encoding, byte[] data, RequestMethod method, boolean isEncoded) {
        IRequest request;
        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put(Constants.REQ_DEVICE_ID, deviceId);
        CSR csr = new CSR();
        if (isEncoded) {
            csr.setEncoding(encoding);
            csr.setData(data);
            payloadData.put("csr", csr);
        }
        request = MessageBuilder.createRequest(method, CERTIFICATE_URI,
                null, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return request;
    }

    private boolean methodCheck(IResponse response,
                                ResponseStatus responseStatus) {
        if (responseStatus == response.getStatus())
            return true;
        else
            return false;
    }

    private boolean hashmapCheck(IResponse response, String propertyName) {
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);
        if (payloadData.containsKey(propertyName))
            return true;
        else
            return false;
    }

    public static class CSR {
        String encoding;

        byte[] data;

        public String getEncoding() {
            return encoding;
        }

        public void setEncoding(String encoding) {
            this.encoding = encoding;
        }

        public byte[] getData() {
            return data;
        }

        public void setData(byte[] data) {
            this.data = data;
        }

        @Override
        public String toString() {
            return "CSR{" +
                    "encoding='" + encoding + '\'' +
                    ", data=" + Arrays.toString(data) +
                    '}';
        }
    }

}
