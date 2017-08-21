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

import org.bouncycastle.asn1.x500.X500Name;
import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.x509.cert.Utility;
import org.iotivity.cloud.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.text.DateFormat;
import java.text.MessageFormat;
import java.text.SimpleDateFormat;
import java.util.Properties;

import static java.security.cert.CertificateFactory.getInstance;

/**
 * This utility class is used for getting all properties from properties file.
 */
public final class CertificateConstants {

    /**
     * Properties object is used for loading pre-defined configurations: algorithm names and so on.
     */
    public static final Properties PROPERTIES = new Properties();

    /**
     * Load properties from specified properties file.
     */
    static {
        try (FileInputStream inputStream = new FileInputStream(Constants.PROPERTIES_FILE_NAME)) {
            PROPERTIES.load(inputStream);
        } catch (IOException e) {
            Log.e(e.getMessage());
        }
    }

    /**
     * Base 64 encoding constant for comparing request encoding.
     */
    public static final String BASE_64 = "oic.sec.encoding.base64";

    /**
     * Der encoding constant for comparing request encoding.
     */
    public static final String DER = "oic.sec.encoding.der";

    /**
     * Name of security provider, load from properties.
     */
    public static final String SECURITY_PROVIDER = PROPERTIES.getProperty("securityProvider");

    /**
     * Not after interval for X509Certificate generation, load from properties.
     */
    public static final String NOT_AFTER_INTERVAL = PROPERTIES.getProperty("notAfterInterval");

    /**
     * Next Update interval. is used for generation X509CRL, load from properties.
     */
    public static final String NEXT_UPDATE_INTERVAL = PROPERTIES.getProperty("nextUpdateInterval");

    /**
     * Signature algorithm, is used for signing certificates and CRLs, load from properties.
     */
    public static final String SIGNATURE_ALGORITHM = PROPERTIES.getProperty("signatureAlgorithm");

    /**
     * KeyStore type, is used for private key storage and for root certificate storage
     */
    static final String KEYSTORE_TYPE = PROPERTIES.getProperty("keystoreType");

    /**
     * CA Alias name, is used for storing ca certificate to key storage.
     */
    static final String CA_ALIAS = PROPERTIES.getProperty("caAlias");

    /**
     * Curve name, is used in signing algorithm for X509 certificate generation.
     */
    static final String CURVE = PROPERTIES.getProperty("ellipticCurve");

    /**
     * Key generation algorithm is used for generation private and public keys.
     */
    static final String KEY_GENERATOR_ALGORITHM = PROPERTIES.getProperty("keyGeneratorAlgorithm");

    /**
     * Date format, is used to parse date with UTC TIME format for certificate and CRL generation.
     */
    public static final DateFormat DATE_FORMAT = new SimpleDateFormat("yyyyMMddHHmmss");

    /**
     * String constant is used to create keystore directory.
     */
    public static final String KEYSTORE_DIR = PROPERTIES.getProperty("keystoreDir");

    /**
     * X500Name for ca issuer, is used for issuing personal certificates and CRLs.
     */
    public static final X500Name CA_ISSUER = Utility.getName(PROPERTIES.getProperty("subjectName"),
            PROPERTIES.getProperty("rootC"), PROPERTIES.getProperty("rootO"), PROPERTIES.getProperty("rootOU"));

    /**
     * Shared AccountDBManager to get rid of repeatable links on the same object.
     */
    public static final AccountDBManager ACCOUNT_DB_MANAGER = AccountDBManager.getInstance();

    /**
     * Certificate factory is used during certificate generation process.
     */
    public static CertificateFactory CERTIFICATE_FACTORY;

    static {
        try {
            CERTIFICATE_FACTORY = getInstance("X509");
        } catch (CertificateException e) {
            Log.e(e.getMessage());
        }
    }

    /**
     * Path to keystore file, retrieved from properties file.
     */
    public static final File KEYSTORE_FILE = new File(MessageFormat.
            format(PROPERTIES.getProperty("keyStoreLocation"), File.separator));

    /**
     * Set specified value for specified property.
     *
     * @param property specified property
     * @param value    specified property value.
     */
    public static void set(String property, String value) {
        PROPERTIES.setProperty(property, value);
        try (FileOutputStream outputStream = new FileOutputStream(Constants.PROPERTIES_FILE_NAME)) {
            PROPERTIES.store(outputStream, "New Serial number");
        } catch (IOException e) {
            Log.e(e.getMessage());
        }
    }

    /**
     * Private constructor makes class non-instantiable utility class.
     */
    private CertificateConstants() {
        throw new AssertionError();
    }
}
