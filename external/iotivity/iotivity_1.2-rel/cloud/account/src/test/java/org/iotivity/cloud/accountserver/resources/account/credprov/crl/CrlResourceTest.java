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
package org.iotivity.cloud.accountserver.resources.account.credprov.crl;

import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.operator.OperatorCreationException;
import org.bouncycastle.util.encoders.Base64;
import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.db.CRLTable;
import org.iotivity.cloud.accountserver.resources.account.credprov.cert.CertificateResourceTest;
import org.iotivity.cloud.accountserver.resources.account.credprov.cert.GenerateCSR;
import org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants;
import org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateResource;
import org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateStorage;
import org.iotivity.cloud.accountserver.resources.credprov.crl.CrlManager;
import org.iotivity.cloud.accountserver.resources.credprov.crl.CrlResource;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;
import org.iotivity.cloud.accountserver.x509.crl.CrlIssuer;
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
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.math.BigInteger;
import java.security.PublicKey;
import java.security.Security;
import java.security.cert.CertificateFactory;
import java.security.cert.X509CRL;
import java.security.cert.X509CRLEntry;
import java.security.cert.X509Certificate;
import java.text.ParseException;
import java.util.*;
import java.util.concurrent.CountDownLatch;

import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.*;
import static org.junit.Assert.*;
import static org.mockito.Mockito.mock;


public class CrlResourceTest {

    private CoapDevice mMockDevice = mock(CoapDevice.class);
    private Cbor<Map<String, Object>> mCbor = new Cbor<>();
    private IResponse mResponse = null;
    private CountDownLatch mLatch = new CountDownLatch(
            1);
    public static final String[] FOR_SERIAL_NUMBER = {Constants.REQ_THIS_UPDATE, Constants.REQ_NEXT_UPDATE, Constants.REQ_SERIAL_NUMBER};
    public static final String CRL_URI = Constants.PREFIX_OIC + "/" + Constants.CREDPROV_URI + "/" +
            Constants.REQ_CRL;
    public static final String CRL_URI_QUERY = "lu=20160726210000";
    public static final String CRL_URI_QUERY_CONDITION_FALSE = "lu=21160726210000";
    public static final String CRL_URI_QUERY_PARSE_ERROR = "lu=1231212asdzfg4123123123123123";
    public static final String[] FOR_FULL_CRL = {Constants.REQ_THIS_UPDATE, Constants.REQ_NEXT_UPDATE, Constants.REQ_CRL};
    public static PublicKey key;
    static CertificateResource certificateResource;

    private static TypeCastingManager<CRLTable> castingManager = new TypeCastingManager<>();

    @AfterClass
    public static void after() {
        AccountDBManager.getInstance().deleteRecord(Constants.CERTIFICATE_TABLE, new HashMap<>());
        AccountDBManager.getInstance().deleteRecord(Constants.CRL_TABLE, new HashMap<>());
    }

    @BeforeClass
    public static void setUpBefore() {
        Security.insertProviderAt(new BouncyCastleProvider(), 0);
        certificateResource = new CertificateResource();
        key = CertificateStorage.ROOT_CERTIFICATE.getPublicKey();
    }

    /**
     * Serial number of last issued certificate.
     */
    private static BigInteger SERIAL_NUMBER = new BigInteger(PROPERTIES.getProperty("serialNumber"));

    Map<String, Object> payloadData;

    Map<String, Object> crlMap;

    byte[] data;

    X509CRL crlX509;


    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];
                mResponse = resp;
                if (mResponse.getPayload() != null) {
                    payloadData = mCbor
                            .parsePayloadFromCbor(mResponse.getPayload(), HashMap.class);
                    crlMap = (Map<String, Object>) payloadData.get(Constants.REQ_CRL);
                    if (crlMap != null) {
                        data = (byte[]) crlMap.get(Constants.DATA);
                        CertificateFactory factory = CertificateFactory.getInstance("X509");
                        crlX509 = (X509CRL) factory.generateCRL(new ByteArrayInputStream(data));
                    }
                }
                mLatch.countDown();
                return null;
            }
        }).when(mMockDevice).sendResponse(Mockito.anyObject());
    }


    CRLTable getCRLTime() {

        List<HashMap<String, Object>> listMap = AccountDBManager.getInstance().selectRecord(
                Constants.CRL_TABLE, new HashMap<>());
        if (!listMap.isEmpty()) {
            return castingManager
                    .convertMaptoObject(
                            listMap.get(0),
                            new CRLTable());
        } else {
            return null;
        }
    }

    private CrlResource crlResource = new CrlResource();

    /**
     * Returns next serial number.
     */
    private static BigInteger getNextSerialNumber() {
        SERIAL_NUMBER = SERIAL_NUMBER.add(BigInteger.ONE);
        set("serialNumber", SERIAL_NUMBER.toString());
        return SERIAL_NUMBER;
    }


    @Test
    public void testAeIssueBase64() throws Exception {
        byte[] csr = GenerateCSR.generatePKCS10(CertificateResourceTest.COMMON_NAME, false);
        IRequest request = csrRequest(CertificateResourceTest.DEVICE_ID, CertificateConstants.BASE_64, Base64.encode(csr), RequestMethod.POST, true);
        certificateResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        Map<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(mResponse.getPayload(), HashMap.class);
        List<BigInteger> serialNumbers = new ArrayList<>();
        Map<String, Object> certMap = (Map<String, Object>) payloadData.get(Constants.CERT);
        InputStream in = new ByteArrayInputStream(Base64.decode((byte[]) certMap.get(Constants.DATA)));
        X509Certificate personaleCert = (X509Certificate) CERTIFICATE_FACTORY.generateCertificate(in);
        serialNumbers.add(personaleCert.getSerialNumber());
        serialNumbers.add(personaleCert.getSerialNumber().subtract(BigInteger.ONE));
        request = csrRequest(CertificateResourceTest.DEVICE_ID, CertificateConstants.BASE_64, Base64.encode(csr), RequestMethod.POST, true);
        certificateResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        payloadData = mCbor
                .parsePayloadFromCbor(mResponse.getPayload(), HashMap.class);
        certMap = (Map<String, Object>) payloadData.get(Constants.CERT);
        in = new ByteArrayInputStream(Base64.decode((byte[]) certMap.get(Constants.DATA)));
        personaleCert = (X509Certificate) CERTIFICATE_FACTORY.generateCertificate(in);
        serialNumbers.add(personaleCert.getSerialNumber());
        request = csrRequest(CertificateResourceTest.DEVICE_ID, CertificateConstants.BASE_64, Base64.encode(csr), RequestMethod.POST, true);
        certificateResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        getTestMethodName();
        request = CrlResourceTest.crlRequest(RequestMethod.GET, CrlResourceTest.CRL_URI, CrlResourceTest.CRL_URI_QUERY);
        CrlResource crlResource = new CrlResource();
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        hashmapCheck(mResponse, Constants.ENCODING);
        hashmapCheck(mResponse, Constants.DATA);
        if (mResponse.getPayload() != null) {
            payloadData = mCbor
                    .parsePayloadFromCbor(mResponse.getPayload(), HashMap.class);
            crlMap = (Map<String, Object>) payloadData.get(Constants.REQ_CRL);
            data = (byte[]) crlMap.get(Constants.DATA);
            crlX509 = (X509CRL) CERTIFICATE_FACTORY.generateCRL(new ByteArrayInputStream(data));
        }
        assertEquals(DER, crlMap.get(Constants.ENCODING));
        assertNotNull(data);
        Set<? extends X509CRLEntry> entries = crlX509.getRevokedCertificates();
        Iterator<? extends X509CRLEntry> iterator = entries.iterator();
        while (iterator.hasNext()) {
            assertTrue(serialNumbers.contains(iterator.next().getSerialNumber()));
        }
    }

    private IRequest csrRequest(String deviceId, String encoding, byte[] data, RequestMethod method, boolean isEncoded) {
        IRequest request;
        HashMap<String, Object> payloadData = new HashMap<>();
        payloadData.put(Constants.REQ_DEVICE_ID, deviceId);
        CertificateResourceTest.CSR csr = new CertificateResourceTest.CSR();
        if (isEncoded) {
            csr.setEncoding(encoding);
            csr.setData(data);
            payloadData.put("csr", csr);
        }
        request = MessageBuilder.createRequest(method, OICConstants.CREDPROV_CERT_FULL_URI,
                null, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return request;
    }


    @Test
    public void testCrlGetContent() throws Exception {
        getTestMethodName();
        IRequest request = crlRequest(RequestMethod.GET, CRL_URI, CRL_URI_QUERY);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        hashmapCheck(mResponse, Constants.ENCODING);
        hashmapCheck(mResponse, Constants.DATA);
        assertEquals(DER, crlMap.get(Constants.ENCODING));
        assertNotNull(data);
        crlX509.verify(key);
    }

    @Test
    public void testCrlGetNotFoundLastUpdateConditionFalse() throws Exception {
        getTestMethodName();
        IRequest request = crlRequest(RequestMethod.GET, CRL_URI, CRL_URI_QUERY_CONDITION_FALSE);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.NOT_FOUND));
    }

    @Test
    public void testCrlGetNotFoundNullPointer() throws Exception {
        getTestMethodName();
        IRequest request = crlRequest(RequestMethod.GET, CRL_URI, null);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.NOT_FOUND));
    }

    @Test
    public void testCrlGetNotFoundParseError() throws Exception {
        getTestMethodName();
        IRequest request = crlRequest(RequestMethod.GET, CRL_URI, CRL_URI_QUERY_PARSE_ERROR);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.NOT_FOUND));
    }

    @Test(expected = InvocationTargetException.class)
    public void testNullX509CRL() throws InvocationTargetException, NoSuchMethodException, IllegalAccessException {
        IRequest request = crlRequest(RequestMethod.GET, CRL_URI, CRL_URI_QUERY);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        Method m = null;
        m = CrlManager.class.getDeclaredMethod("setX509CRL", byte[].class);
        m.setAccessible(true);
        System.out.println(m);
        m.invoke(CrlManager.CRL_MANAGER, "Hello".getBytes());
    }

    @Test
    public void testCrlPreconditionFailedException() throws ParseException, IOException, OperatorCreationException {
        getTestMethodName();
        getTestMethodName();
        String thisUpdate = "20160823000000";
        String nextUpdate = "20161123000000";
        byte[] crl = CrlIssuer.CRL_ISSUER.generate(DATE_FORMAT.parse(thisUpdate),
                DATE_FORMAT.parse(nextUpdate), Collections.emptyList());
        IRequest request = crlRequest(RequestMethod.POST, thisUpdate, nextUpdate, Base64.encode(Arrays.copyOf(crl, crl.length - 2)), BASE_64);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.PRECONDITION_FAILED));
        crl = new byte[8];
        request = crlRequest(RequestMethod.POST, thisUpdate, nextUpdate, Base64.encode(crl), BASE_64);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.PRECONDITION_FAILED));
        crl = new byte[8];
        request = crlRequest(RequestMethod.POST, thisUpdate, nextUpdate, crl, BASE_64);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.PRECONDITION_FAILED));
    }

    @Test
    public void testCrlPostSerialNumberFailedData() throws Exception {
        getTestMethodName();
        IRequest request = crlRequest(RequestMethod.GET, CRL_URI, CRL_URI_QUERY);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        System.out.println("MRESPONSE" + mResponse.getStatus());
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        String thisUpdate = CertificateConstants.DATE_FORMAT.format(crlX509.getThisUpdate());
        String nextUpdate = CertificateConstants.DATE_FORMAT.format(crlX509.getNextUpdate());
        String next = getNextSerialNumber().subtract(BigInteger.valueOf(3l)).toString();
        String[] serialNUmber = new String[]{"123456", "1234", "12312", next};
        request = crlRequestFailedCRL(RequestMethod.POST, thisUpdate, nextUpdate, serialNUmber);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.PRECONDITION_FAILED));
    }

    @Test
    public void testCrlPostSerialNumber() throws Exception {
        getTestMethodName();
        testCrlPostEncodingBase64();
        IRequest request = crlRequest(RequestMethod.GET, CRL_URI, CRL_URI_QUERY);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        String thisUpdate = CertificateConstants.DATE_FORMAT.format(crlX509.getThisUpdate());
        String nextUpdate = CertificateConstants.DATE_FORMAT.format(crlX509.getNextUpdate());
        String next = getNextSerialNumber().subtract(BigInteger.valueOf(3l)).toString();
        String nextNext = getNextSerialNumber().subtract(BigInteger.valueOf(3l)).toString();

        String[] serialNUmber = new String[]{"123456", "1234", "12312", next, nextNext};
        request = crlRequest(RequestMethod.POST, thisUpdate, nextUpdate, serialNUmber);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        serialNUmber = new String[]{"12345612312", "12341231", "12312231"};
        request = crlRequest(RequestMethod.POST, thisUpdate, nextUpdate, serialNUmber);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        serialNUmber = new String[]{"12345612312", "12341231", "12312231", "123456", "1234", "12312", next, nextNext};
        request = crlRequest(RequestMethod.GET, CRL_URI, CRL_URI_QUERY);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        assertEquals(DER, crlMap.get(Constants.ENCODING));
        Set<? extends X509CRLEntry> entries = crlX509.getRevokedCertificates();
        Iterator<? extends X509CRLEntry> iterator = entries.iterator();

        while (iterator.hasNext()) {
            assertTrue(Arrays.asList(serialNUmber).contains(iterator.next().getSerialNumber().toString()));
        }
        CRLTable table = getCRLTime();
        assertEquals(DATE_FORMAT.format(table.getThisUpdate()), thisUpdate);

    }

    @Test
    public void testSerialNumberNull() {
        IRequest request = crlRequest(RequestMethod.GET, CRL_URI, CRL_URI_QUERY);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        String thisUpdate = CertificateConstants.DATE_FORMAT.format(crlX509.getThisUpdate());
        String nextUpdate = CertificateConstants.DATE_FORMAT.format(crlX509.getNextUpdate());
        String[] serialNUmber = null;
        request = crlRequest(RequestMethod.POST, thisUpdate, nextUpdate, serialNUmber);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.PRECONDITION_FAILED));
    }

    @Test
    public void testCrlUnParseableDate() throws Exception {
        getTestMethodName();
        String thisUpdate = "l;dkfjg;ls";
        String nextUpdate = "sdfgsdfg";
        String[] serialNUmber = new String[]{"123456"};
        IRequest request = crlRequest(RequestMethod.POST, thisUpdate, nextUpdate, serialNUmber);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.PRECONDITION_FAILED));
    }

    @Test
    public void testCrlPostNullDate() throws Exception {
        getTestMethodName();
        String thisUpdate = null;
        String nextUpdate = null;
        String[] serialNUmber = new String[]{"123456"};
        IRequest request = crlRequest(RequestMethod.POST, thisUpdate, nextUpdate, serialNUmber);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
    }


    @Test
    public void testCrlPostEncodingBase64() throws Exception {
        getTestMethodName();
        String thisUpdate = "20160831000000";
        String nextUpdate = "20161123000000";
        byte[] crl = CrlIssuer.CRL_ISSUER.generate(DATE_FORMAT.parse(thisUpdate),
                DATE_FORMAT.parse(nextUpdate), Collections.emptyList());
        IRequest request = crlRequest(RequestMethod.POST, thisUpdate, nextUpdate, Base64.encode(crl), BASE_64);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        request = crlRequest(RequestMethod.GET, CRL_URI, CRL_URI_QUERY);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        assertEquals(DER, crlMap.get(Constants.ENCODING));
    }

    @Test
    public void testCrlPostEncodingBase64FailedCRL() throws Exception {
        getTestMethodName();
        String thisUpdate = "20160831000000";
        String nextUpdate = "20161123000000";
        IRequest request = crlRequestFailedCRL(RequestMethod.POST, thisUpdate, nextUpdate, BASE_64);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.PRECONDITION_FAILED));
    }

    @Test
    public void testCrlPostEncodingBase64FailedDate() throws Exception {
        getTestMethodName();
        String thisUpdate = "20160831000000";
        String nextUpdate = "20161123000000";
        byte[] crl = CrlIssuer.CRL_ISSUER.generate(DATE_FORMAT.parse(thisUpdate),
                DATE_FORMAT.parse(nextUpdate), Collections.emptyList());
        IRequest request = crlRequestFailedDate(RequestMethod.POST, thisUpdate, nextUpdate, BASE_64);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.PRECONDITION_FAILED));
    }

    @Test
    public void testCrlPostEncodingBase64FailedPayload() throws Exception {
        getTestMethodName();
        String thisUpdate = "20160831000000";
        String nextUpdate = "20161123000000";
        byte[] crl = CrlIssuer.CRL_ISSUER.generate(DATE_FORMAT.parse(thisUpdate),
                DATE_FORMAT.parse(nextUpdate), Collections.emptyList());
        IRequest request = crlRequestFailedPayload(RequestMethod.POST, BASE_64);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.PRECONDITION_FAILED));
    }

    @Test
    public void testCrlPostEncodingDER() throws Exception {
        getTestMethodName();
        String thisUpdate = "20160831000000";
        String nextUpdate = "20161123000000";
        byte[] crl = CrlIssuer.CRL_ISSUER.generate(DATE_FORMAT.parse(thisUpdate),
                DATE_FORMAT.parse(nextUpdate), Collections.emptyList());
        IRequest request = crlRequest(RequestMethod.POST, thisUpdate, nextUpdate, crl, DER);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        request = crlRequest(RequestMethod.GET, CRL_URI, CRL_URI_QUERY);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        assertEquals(DER, crlMap.get(Constants.ENCODING));
        assertTrue(Arrays.equals(crl, data));
        assertTrue(Arrays.equals(crl, crlX509.getEncoded()));
    }

    @Test
    public void testCrlBadRequest() throws Exception {
        getTestMethodName();
        IRequest request = crlRequest(RequestMethod.PUT, CRL_URI, null);
        crlResource.onDefaultRequestReceived(mMockDevice, request);
        assertTrue(mResponse.getStatus().equals(ResponseStatus.BAD_REQUEST));
    }

    public static IRequest crlRequest(RequestMethod method, String uri, String query) {
        IRequest request = MessageBuilder.createRequest(method, uri, query);
        return request;
    }

    private IRequest crlRequestSN(RequestMethod method, String[] keys, Object[] values) {
        HashMap<String, Object> payloadData = new HashMap<>();
        for (int i = 0; i < keys.length; i++) {
            payloadData.put(keys[i], values[i]);
        }
        IRequest request = MessageBuilder.createRequest(method, CRL_URI,
                null, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return request;
    }

    private IRequest crlRequestFull(RequestMethod method, String[] keys, Object[] values, String encoding) {
        HashMap<String, Object> payloadData = new HashMap<>();
        if (keys.length != 0 && values.length != 0) {
            for (int i = 0; i < keys.length - 1; i++) {
                payloadData.put(keys[i], values[i]);
            }
            Object value = values[values.length - 1];
            if (value instanceof byte[]) {
                payloadData.put(keys[keys.length - 1], new CRL(encoding, (byte[]) value));
            } else {
                payloadData.put(keys[keys.length - 1], value);
            }

        }
        IRequest request = MessageBuilder.createRequest(method, CRL_URI,
                null, ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        return request;
    }


    private IRequest crlRequest(RequestMethod method, String thisUpdate, String nextUpdate, String[] rcsn) {
        return crlRequestSN(method, FOR_SERIAL_NUMBER, new Object[]{thisUpdate, nextUpdate, rcsn});
    }

    private IRequest crlRequestFailedCRL(RequestMethod method, String thisUpdate, String nextUpdate, String[] rcsn) {
        return crlRequestSN(method, FOR_SERIAL_NUMBER, new Object[]{thisUpdate, nextUpdate, rcsn.length});
    }

    private IRequest crlRequest(RequestMethod method, String thisUpdate, String nextUpdate, byte[] crl, String encoding) {
        return crlRequestFull(method, FOR_FULL_CRL, new Object[]{thisUpdate, nextUpdate, crl}, encoding);
    }

    private IRequest crlRequestFailedCRL(RequestMethod method, String thisUpdate, String nextUpdate,
                                         String encoding) {
        return crlRequestFull(method, FOR_FULL_CRL, new Object[]{thisUpdate, nextUpdate, thisUpdate}, encoding);
    }

    private IRequest crlRequestFailedDate(RequestMethod method, String thisUpdate, String nextUpdate,
                                          String encoding) {
        return crlRequestFull(method, FOR_FULL_CRL, new Object[]{thisUpdate.getBytes(), nextUpdate, thisUpdate}, encoding);
    }

    private IRequest crlRequestFailedPayload(RequestMethod method, String encoding) {
        return crlRequestFull(method, new String[]{}, new Object[]{}, encoding);
    }

    private static final class CRL {

        private final String encoding;

        private final byte[] data;

        public CRL(String encoding, byte[] data) {
            this.encoding = encoding;
            this.data = data;
        }

        @Override
        public String toString() {
            return "CRL{" +
                    "encoding='" + encoding + '\'' +
                    ", data=" + Arrays.toString(data) +
                    '}';
        }

        public String getEncoding() {
            return encoding;
        }

        public byte[] getData() {
            return data;
        }
    }


    private void getTestMethodName() {
        StackTraceElement[] stacks = new Throwable().getStackTrace();
        StackTraceElement currentStack = stacks[1];
        System.out.println("\t---Test Name : " + currentStack.getMethodName());
    }

    private boolean methodCheck(IResponse response,
                                ResponseStatus responseStatus) {
        if (responseStatus == response.getStatus())
            return true;
        else
            return false;
    }

    private boolean hashmapCheck(IResponse response, String propertyName) {
        Map<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);
        if (payloadData.containsKey(propertyName))
            return true;
        else
            return false;
    }
}
