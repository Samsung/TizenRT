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
package org.iotivity.cloud.accountserver.resources.credprov.cert;

import org.bouncycastle.asn1.ASN1Encodable;
import org.bouncycastle.asn1.DERSequence;
import org.bouncycastle.asn1.x509.Extension;
import org.bouncycastle.asn1.x509.GeneralName;
import org.bouncycastle.cert.CertIOException;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.operator.OperatorCreationException;
import org.bouncycastle.util.encoders.Base64;
import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.CertificateTable;
import org.iotivity.cloud.accountserver.resources.credprov.crl.CrlManager;
import org.iotivity.cloud.accountserver.x509.cert.CSRParser;
import org.iotivity.cloud.accountserver.x509.cert.CertificateBuilder;
import org.iotivity.cloud.accountserver.x509.cert.CertificateExtension;
import org.iotivity.cloud.accountserver.x509.cert.Utility;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.resource.Resource;
import org.iotivity.cloud.util.Cbor;
import org.iotivity.cloud.util.Log;

import java.io.IOException;
import java.security.GeneralSecurityException;
import java.security.PublicKey;
import java.security.Security;
import java.security.cert.CRLException;
import java.security.cert.X509Certificate;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static org.iotivity.cloud.accountserver.Constants.*;
import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.BASE_64;
import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.KEYSTORE_FILE;

/**
 * This class provides access for certificate resource.
 * Devices can send CSR requests in CBOR format to this resource
 * and get responses in the same format. Response contains device identifier,
 * personal certificate, issued by CA certificate and certificate chain.
 */
public class CertificateResource extends Resource {

    /**
     * This constant object is used for parsing cbor payload to Map object and to
     * encoding map object to cbor format.
     */
    private static final Cbor<Map<String, Object>> MAP_CBOR = new Cbor<>();

    /**
     * Inserts BouncyCastleProvider into 0 position in security provider list,
     * inits KeyStore, generates CA certificate and saves it to keyStore.
     */
    static {
        Security.insertProviderAt(new BouncyCastleProvider(), 0);
        try {
            if (!KEYSTORE_FILE.exists()) {
                CertificateStorage.init();
            } else {
                CertificateStorage.load();
            }
        } catch (GeneralSecurityException | IOException | OperatorCreationException e) {
            Log.e(e.getMessage());
        }
    }

    /**
     * Constructs certificate resource with specified prefixes.
     */
    public CertificateResource() {
        super(Arrays.asList(PREFIX_OIC, CREDPROV_URI, CERT_URI));
    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {
        IResponse response;
        switch (request.getMethod()) {
            case POST:
                response = handlePostRequest(request);
                break;
            default:
                response = MessageBuilder.createResponse(request, ResponseStatus.METHOD_NOT_ALLOWED);
        }
        srcDevice.sendResponse(response);
    }

    /**
     * Handles post requests to Certificate Resource.
     * Request should be with specified format
     * POST /oic/credprov/cert
     * {
     *      “di” : “11-22-xx”,
     *      “csr” : {
     *          “encoding” : “oic.sec.encoding.base64”,
     *          “data” : “<Base64 encoded CSR Binary>”
     *      }
     * }
     * Method checks encoding, and decodes data by specified encoding if needed.
     *
     * Method issus a certificate including User UUID in extension field,
     * stores issuing information (serial number, validity, device uuid, user uuid) for management (e.g. re-issue).
     * Response should be in next format for example:
     * 2.04 CHANGED
     * {
     *      “di” : “1111-22-xx”,
     *      “cert” : {
     *          “encoding” : “oic.sec.encoding.base64”,
     *          “data” : “<Base64 encoded Cert. Binary>”
     *       },
     *      “certchain” : {
     *          “encoding” : “oic.sec.encoding.base64”,
     *          “data” : “<Base64 encoded CA Cert. chain>”
     *       }
     * }
     * or returns BAD_REQUEST: 4.0.1 if any exceptions occured.
     *
     * @param request request with payload information.
     * @throws ServerException
     */
    private IResponse handlePostRequest(IRequest request)
            throws ServerException {
        byte[] requestPayload = request.getPayload();
        IResponse response = MessageBuilder.createResponse(request, ResponseStatus.BAD_REQUEST);
        if (requestPayload != null) {
            Map<String, Object> payloadData = MAP_CBOR
                    .parsePayloadFromCbor(requestPayload, HashMap.class);
            if (payloadData != null) {
                Object csr = payloadData.get(Constants.REQ_CSR);
                if (csr != null && csr instanceof Map) {
                    Object encoding = ((Map<String, Object>) csr).get(ENCODING);
                    Object data = ((Map<String, Object>) csr).get(DATA);
                    if (encoding != null && encoding instanceof String && data != null && data instanceof byte[]) {
                        byte[] csrData = (byte[]) data;
                        if (encoding.equals(BASE_64)) {
                            csrData = Base64.decode(csrData);
                        }
                        try {
                            CSRParser parser = new CSRParser(csrData);
                            String commonName = parser.getCommonName();
                            String pattern = "^uuid:(.*)$";
                            Pattern r = Pattern.compile(pattern);
                            Matcher m = r.matcher(commonName);
                            String deviceId = (String) payloadData.get(RESP_DEVICE_ID);
                            if (m.find() && m.group(1).equals(deviceId) && parser.isSignatureValid()) {
                                CertificateManager certificateManager = new CertificateManager(deviceId);
                                CertificateTable certificateTable = certificateManager.getCertificate();
                                if (certificateTable != null) {
                                    try {
                                        CrlManager.CRL_MANAGER.revoke(certificateTable.getSerialNumber());
                                    } catch (CRLException | OperatorCreationException e) {
                                        Log.e(e.getMessage() + e.getClass());
                                    }
                                    certificateManager.update(certificateTable, true);
                                }
                                PublicKey publicKey = parser.getPublicKey();
                                if (publicKey != null) {
                                    CertificateExtension extension = new CertificateExtension(Extension.subjectAlternativeName,
                                            false, new DERSequence(new ASN1Encodable[]
                                            {new GeneralName(GeneralName.dNSName, Constants.KEYFIELD_USERID + ":" +
                                                    Utility.getUserID(deviceId))}));
                                    CertificateBuilder certBuilder = new CertificateBuilder(parser.getSubject(),
                                            publicKey, extension);
                                    try {
                                        X509Certificate personal = certBuilder.build();
                                        byte[] encodedCert = personal.getEncoded();
                                        byte[] encodedCa = CertificateStorage.ROOT_CERTIFICATE.getEncoded();
                                        if (encoding.equals(CertificateConstants.BASE_64)) {
                                            encodedCert = Base64.encode(encodedCert);
                                            encodedCa = Base64.encode(encodedCa);
                                        }
                                        certificateManager.put(Constants.RESP_DEVICE_ID, deviceId);
                                        certificateManager.put(Constants.CERT, new CSR(encoding.toString(), encodedCert));
                                        certificateManager.put(Constants.CERT_CHAIN, new CSR(encoding.toString(), encodedCa));
                                        certificateManager.save(personal.getSerialNumber(), personal.getNotAfter(),
                                                personal.getNotBefore());
                                        response = MessageBuilder.createResponse(request, ResponseStatus.CHANGED,
                                                ContentFormat.APPLICATION_CBOR,
                                                MAP_CBOR.encodingPayloadToCbor(certificateManager.getPayLoad()));
                                    } catch (GeneralSecurityException | OperatorCreationException | CertIOException e) {
                                        Log.e(e.getMessage());
                                    }
                                }
                            }
                        } catch (IOException e) {
                            Log.e(e.getMessage());
                        }
                    }
                }
            }
        }
        return response;
    }

    /**
     * Response utility class.
     */
    private static final class CSR {

        private final String encoding;

        private final byte[] data;

        CSR(String encoding, byte[] data) {
            this.encoding = encoding;
            this.data = data;
        }

        /**
         * Return encoding.
         */
        public String getEncoding() {
            return encoding;
        }

        /**
         * Retrieves data.
         */
        public byte[] getData() {
            return data;
        }
    }
}
