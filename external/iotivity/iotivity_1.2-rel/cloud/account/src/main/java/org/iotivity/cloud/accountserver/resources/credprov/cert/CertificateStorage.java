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

import org.bouncycastle.asn1.x509.BasicConstraints;
import org.bouncycastle.asn1.x509.Extension;
import org.bouncycastle.jce.ECNamedCurveTable;
import org.bouncycastle.operator.OperatorCreationException;
import org.iotivity.cloud.accountserver.x509.cert.CertificateBuilder;
import org.iotivity.cloud.accountserver.x509.cert.CertificateExtension;
import org.iotivity.cloud.util.Log;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.security.*;
import java.security.cert.Certificate;
import java.security.cert.X509Certificate;

import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.*;

/**
 * This class is used for loading and storing key store.
 * Also it generates CA certificate and puts it to keystore.
 */
public final class CertificateStorage {

    /**
     * This attribute is used to get password to kestore, that stores CA certificate info.
     * Private key and certificate.
     */
    private static final String PASSWORD = PROPERTIES.getProperty("password");

    /**
     * Root private key is used for signing certificates ans CRLs.
     */
    public static PrivateKey ROOT_PRIVATE_KEY;

    /**
     * Root certificate, is used for isssuing low level certificates.
     */
    public static X509Certificate ROOT_CERTIFICATE;

    /**
     * Keystore object to save, retrieve ca private key and ca certificate from keystore.
     */
    private static KeyStore keyStore;

    /**
     * Private contructor to make this class non-instantiable.
     */
    private CertificateStorage() {
        throw new AssertionError();
    }

    /**
     * Loads keystore with null paramaters.
     * Stores it empty version. Generates CA certificate and private key and
     * saves it to key storage.
     */
    static void init() throws GeneralSecurityException, IOException, OperatorCreationException {
        Files.createDirectories(Paths.get(KEYSTORE_DIR));
        keyStore = KeyStore.getInstance(KEYSTORE_TYPE, SECURITY_PROVIDER);
        keyStore.load(null, null);
        generate();
    }

    /**
     * Loads KeyStore intance from created keystore file and with default password.
     *
     * @return KeyStore instance.
     */
    static void load() throws GeneralSecurityException, IOException {
        keyStore = KeyStore.getInstance(KEYSTORE_TYPE, SECURITY_PROVIDER);
        try (InputStream inputStream = new FileInputStream(KEYSTORE_FILE)) {
            keyStore.load(inputStream, PASSWORD.toCharArray());
        } catch (IOException ioException) {
            Log.e(ioException.getMessage());
        }
        ROOT_PRIVATE_KEY = (PrivateKey) keyStore.getKey(CA_ALIAS, PASSWORD.toCharArray());
        ROOT_CERTIFICATE = (X509Certificate) keyStore.getCertificate(CA_ALIAS);
    }

    /**
     * Generates CA X509Certificate and private key and stores it to key storage.
     *
     * @return certificate and private key
     */
    private static void generate() throws GeneralSecurityException,
            OperatorCreationException, IOException {
        if (ROOT_PRIVATE_KEY == null) {
            KeyPairGenerator g = KeyPairGenerator.getInstance(KEY_GENERATOR_ALGORITHM, SECURITY_PROVIDER);
            g.initialize(ECNamedCurveTable.getParameterSpec(CURVE), new SecureRandom());
            KeyPair pair = g.generateKeyPair();
            ROOT_PRIVATE_KEY = pair.getPrivate();
            ROOT_CERTIFICATE = new CertificateBuilder(CA_ISSUER, pair.getPublic(),
                    new CertificateExtension(Extension.basicConstraints, false,
                            new BasicConstraints(true))).build();
            keyStore.setCertificateEntry(CA_ALIAS, ROOT_CERTIFICATE);
            keyStore.setKeyEntry(CA_ALIAS, ROOT_PRIVATE_KEY, PASSWORD.toCharArray(),
                    new Certificate[]{ROOT_CERTIFICATE});
            store();
        }
    }

    /**
     * Stores keyStore instance to default keystore file with default password.
     */
    private static void store() throws IOException, GeneralSecurityException {
        try (FileOutputStream out = new FileOutputStream(KEYSTORE_FILE)) {
            keyStore.store(out, PASSWORD.toCharArray());
        } catch (IOException ioException) {
            Log.e(ioException.getMessage());
        }
    }
}
