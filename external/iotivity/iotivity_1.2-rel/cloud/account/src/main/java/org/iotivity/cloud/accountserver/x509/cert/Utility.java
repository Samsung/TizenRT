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
import org.bouncycastle.asn1.x500.X500NameBuilder;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

import static org.bouncycastle.asn1.x500.style.BCStyle.*;
import static org.iotivity.cloud.accountserver.Constants.*;
import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.ACCOUNT_DB_MANAGER;

/**
 * Utility class is for getting X500Name from common name, country, organization and organization unit name
 */
public final class Utility {

    /**
     * Forbid creation of new Utility object. This class is final
     * and non-instantiable
     */
    private Utility() {
        throw new AssertionError();
    }

    /**
     * Returns X500Name contructed with help of specified parameters commonName, country, organizational
     * and organizationUnit
     */
    public static X500Name getName(String commonName, String country, String organizational, String organizationUnit) {
        X500NameBuilder nameBuilder = new X500NameBuilder(INSTANCE);
        nameBuilder.addRDN(CN, commonName);
        nameBuilder.addRDN(C, country);
        nameBuilder.addRDN(O, organizational);
        nameBuilder.addRDN(OU, organizationUnit);
        return nameBuilder.build();
    }

    /**
     * Retrieves user id from Token Table by specified deviceId.
     */
    public static String getUserID(String deviceId) {
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(KEYFIELD_DID, deviceId);
        ArrayList<HashMap<String, Object>> recordList = ACCOUNT_DB_MANAGER.selectRecord(TOKEN_TABLE, condition);
        Iterator<HashMap<String, Object>> iterator = recordList.iterator();
        String result = null;
        if (iterator.hasNext()) {
            result = iterator.next().get(KEYFIELD_UUID).toString();
        }
        return result;
    }


}
