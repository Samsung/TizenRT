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
package org.iotivity.cloud.accountserver.x509.cert;

import org.bouncycastle.asn1.x500.AttributeTypeAndValue;
import org.bouncycastle.asn1.x500.X500Name;
import org.bouncycastle.openssl.PEMException;
import org.bouncycastle.openssl.jcajce.JcaPEMKeyConverter;
import org.bouncycastle.operator.OperatorCreationException;
import org.bouncycastle.operator.jcajce.JcaContentVerifierProviderBuilder;
import org.bouncycastle.pkcs.PKCS10CertificationRequest;
import org.bouncycastle.pkcs.PKCSException;
import org.iotivity.cloud.util.Log;

import java.io.IOException;
import java.security.PublicKey;

import static org.bouncycastle.asn1.x500.style.BCStyle.CN;
import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.SECURITY_PROVIDER;

/**
 * Class is used for parsing CSR requests.
 */
public class CSRParser {

    /**
     * PKCS10CertificationRequest attribute.
     */
    private final PKCS10CertificationRequest mCsr;

    /**
     * Certificate subject.
     */
    private final X500Name subject;

    /**
     * Creates CSRParser instance with specified csrDer byte array.
     *
     * @param csrDer specified byte array.
     */
    public CSRParser(byte[] csrDer) throws IOException {
        mCsr = new PKCS10CertificationRequest(csrDer);
        subject = mCsr.getSubject();
    }

    /**
     * Returns public key
     */
    public PublicKey getPublicKey() {
        PublicKey publicKey = null;
        try {
            publicKey = new JcaPEMKeyConverter().setProvider(SECURITY_PROVIDER).
                    getPublicKey(mCsr.getSubjectPublicKeyInfo());
        } catch (PEMException e) {
            Log.e(e.getMessage());
        }
        return publicKey;
    }

    /**
     * Verifies signature from this CSR request with help of
     *
     * @return true if signature is correct and false otherwise.
     */
    public boolean isSignatureValid() {
        boolean condition = false;
        try {
            condition = mCsr.isSignatureValid(new JcaContentVerifierProviderBuilder()
                    .setProvider(SECURITY_PROVIDER).build(mCsr.getSubjectPublicKeyInfo()));
        } catch (OperatorCreationException | PKCSException e) {
            Log.e(e.getMessage());
        }
        return condition;
    }

    /**
     * Returns common name from csr subject.
     */
    public String getCommonName() {
        AttributeTypeAndValue rdn = subject.getRDNs(CN)[0].getFirst();
        if (rdn != null) {
            return rdn.getValue().toString();
        }
        return null;
    }

    /**
     * Returns subject as X500Name.
     */
    public X500Name getSubject() {
        return subject;
    }
}
