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

import org.bouncycastle.asn1.ASN1Encodable;
import org.bouncycastle.asn1.ASN1ObjectIdentifier;

/**
 * Class for representing certificate extension.
 */
public class CertificateExtension {

    /**
     * Attribute for ASN1 object identifier.
     */
    private final ASN1ObjectIdentifier oid;

    /**
     * Attribute checks critical or non critical type of extension.
     */
    private final boolean isCritical;

    /**
     * Attribute stores ASN1 Encodable value
     */
    private final ASN1Encodable value;

    /**
     * Constructs certificate extension with specified oid, isCritical, value parameters.
     */
    public CertificateExtension(ASN1ObjectIdentifier oid, boolean isCritical, ASN1Encodable value) {
        this.oid = oid;
        this.isCritical = isCritical;
        this.value = value;
    }

    /**
     * Returns attribute value for oid
     */
    ASN1ObjectIdentifier getOid() {
        return oid;
    }

    /**
     * Returns isCritical attribute value.
     */
    boolean isCritical() {
        return isCritical;
    }

    /**
     * Returns ASN1Encodable attribute value.
     */
    ASN1Encodable getValue() {
        return value;
    }
}
