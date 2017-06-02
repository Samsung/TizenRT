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
package org.iotivity.cloud.accountserver.x509.crl;


import org.bouncycastle.cert.X509v2CRLBuilder;
import org.bouncycastle.operator.OperatorCreationException;
import org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateStorage;
import org.iotivity.cloud.accountserver.x509.cert.CertificateBuilder;

import java.io.IOException;
import java.math.BigInteger;
import java.security.cert.X509CRLEntry;
import java.util.Collection;
import java.util.Date;

import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.CA_ISSUER;

/**
 * Class is used for generating CRLs with specified parameters.
 */
public final class CrlIssuer {
    /**
     * Creates static final reference for CRL issuer.
     */
    public static final CrlIssuer CRL_ISSUER = new CrlIssuer();

    /**
     * Private constructor to make class non-instantiable.
     */
    private CrlIssuer() {
    }

    /**
     * Generates new CRL with specified this update, next update, certs and serial numbers list.
     */
    public byte[] generate(Date thisUpdate, Date nextUpdate, Collection<? extends X509CRLEntry> certs,
                           String... serialNumbers) throws IOException, OperatorCreationException {
        X509v2CRLBuilder crlBuilder = new X509v2CRLBuilder(CA_ISSUER,
                thisUpdate);
        crlBuilder.setNextUpdate(nextUpdate);
        if (certs != null) {
            for (X509CRLEntry entryHolder : certs) {
                crlBuilder.addCRLEntry(entryHolder.getSerialNumber(), entryHolder.getRevocationDate(), 0);
            }
        }
        for (String serialNumber : serialNumbers) {
            crlBuilder.addCRLEntry(new BigInteger(serialNumber), new Date(), 0);
        }
        return crlBuilder.build(CertificateBuilder.SIGNER_BUILDER.
                build(CertificateStorage.ROOT_PRIVATE_KEY)).getEncoded();
    }

}