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

import org.bouncycastle.asn1.x500.X500Name;
import org.bouncycastle.cert.CertIOException;
import org.bouncycastle.cert.X509v3CertificateBuilder;
import org.bouncycastle.cert.jcajce.JcaX509CertificateConverter;
import org.bouncycastle.cert.jcajce.JcaX509v3CertificateBuilder;
import org.bouncycastle.operator.OperatorCreationException;
import org.bouncycastle.operator.jcajce.JcaContentSignerBuilder;
import org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateStorage;

import java.math.BigInteger;
import java.security.GeneralSecurityException;
import java.security.PublicKey;
import java.security.cert.X509Certificate;
import java.util.Calendar;
import java.util.Date;

import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.*;

/**
 * Class is used for generating X509 certificate with specified in properties signature and ley algorithms.
 */
public class CertificateBuilder {

    /**
     * SIGNER_BUILDER for building X509 certificates
     */
    public static final JcaContentSignerBuilder SIGNER_BUILDER = new JcaContentSignerBuilder(SIGNATURE_ALGORITHM)
            .setProvider(SECURITY_PROVIDER);

    /**
     * Serial number of last issued certificate.
     */
    private static BigInteger SERIAL_NUMBER = new BigInteger(PROPERTIES.getProperty("serialNumber"));

    /**
     * Attribute for X500Name subject.
     */
    private final X500Name subject;

    /**
     * Attribute for public key.
     */
    private final PublicKey publicKey;

    /**
     * Attribute for certificate extension.
     */
    private final CertificateExtension extension;

    /**
     * Constructs certificate builder with specified subject
     * public key and certificate extension.
     */
    public CertificateBuilder(X500Name subject, PublicKey publicKey, CertificateExtension extension) {
        this.subject = subject;
        this.publicKey = publicKey;
        this.extension = extension;
    }

    /**
     * Builds X509Certificate, issued by CA issuer, with specific subject and publick key
     * Adds extension during build.
     */
    public X509Certificate build() throws CertIOException, GeneralSecurityException, OperatorCreationException {
        X509v3CertificateBuilder certGen = new JcaX509v3CertificateBuilder(
                CA_ISSUER,
                getNextSerialNumber(), getNotBeforeDate(), getNotAfterDate(), subject, publicKey);
        certGen.addExtension(extension.getOid(), extension.isCritical(),
                extension.getValue());
        return new JcaX509CertificateConverter().setProvider(SECURITY_PROVIDER).
                getCertificate(certGen.build(
                        SIGNER_BUILDER.build(CertificateStorage.ROOT_PRIVATE_KEY)));
    }

    /**
     * Returns next serial number.
     */
    private static BigInteger getNextSerialNumber() {
        SERIAL_NUMBER = SERIAL_NUMBER.add(BigInteger.ONE);
        set("serialNumber", SERIAL_NUMBER.toString());
        return SERIAL_NUMBER;
    }

    /**
     * Returns date not before.
     */
    private Date getNotBeforeDate() {
        return Calendar.getInstance().getTime();
    }

    /**
     * Returns date not after.
     */
    private Date getNotAfterDate() {
        Calendar calendar = Calendar.getInstance();
        calendar.set(Calendar.YEAR, calendar.get(Calendar.YEAR) +
                Integer.parseInt(NOT_AFTER_INTERVAL));
        return calendar.getTime();
    }
}