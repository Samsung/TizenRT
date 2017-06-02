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

import org.bouncycastle.jce.ECNamedCurveTable;
import org.bouncycastle.jce.spec.ECParameterSpec;
import org.bouncycastle.operator.ContentSigner;
import org.bouncycastle.operator.jcajce.JcaContentSignerBuilder;
import org.bouncycastle.pkcs.PKCS10CertificationRequest;
import org.bouncycastle.pkcs.PKCS10CertificationRequestBuilder;
import org.bouncycastle.pkcs.jcajce.JcaPKCS10CertificationRequestBuilder;
import org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants;

import javax.security.auth.x500.X500Principal;
import java.security.*;

public class GenerateCSR {

    public static final String SIGNATURE_ALGORITHM = CertificateConstants.PROPERTIES.getProperty("signatureAlgorithm");

    private static final String CURVE = CertificateConstants.PROPERTIES.getProperty("ellipticCurve");

    private static final String KEY_GENERATOR_ALGORITHM = CertificateConstants.PROPERTIES.getProperty("keyGeneratorAlgorithm");

    private static PublicKey publicKey = null;
    private static PrivateKey privateKey = null;
    private static PublicKey publicKey1 = null;
    private static PrivateKey privateKey1= null;
    private static KeyPairGenerator keyGen = null;
    private static GenerateCSR gcsr = null;

    /**
     * @return
     * @throws Exception
     */
    public static byte[] generatePKCS10(String commonName, boolean falseKey) throws Exception {
        ECParameterSpec ecSpec = ECNamedCurveTable.getParameterSpec(CURVE);
        KeyPairGenerator g = KeyPairGenerator.getInstance(KEY_GENERATOR_ALGORITHM, CertificateConstants.SECURITY_PROVIDER);
        g.initialize(ecSpec, new SecureRandom());
        KeyPair pair = g.generateKeyPair();
        privateKey = pair.getPrivate();
        publicKey = pair.getPublic();
        pair = g.generateKeyPair();
        privateKey1 = pair.getPrivate();
        publicKey1 = pair.getPublic();
        PKCS10CertificationRequestBuilder p10Builder = new JcaPKCS10CertificationRequestBuilder(
                new X500Principal(commonName), publicKey);
        JcaContentSignerBuilder csBuilder = new JcaContentSignerBuilder(SIGNATURE_ALGORITHM);
        ContentSigner signer;
        if (falseKey) {
            signer = csBuilder.build(privateKey1);
        } else {
            signer = csBuilder.build(privateKey);
        }

        PKCS10CertificationRequest csr = p10Builder.build(signer);
        return csr.getEncoded();
    }

    public static PublicKey getPublicKey() {
        return publicKey;
    }


}