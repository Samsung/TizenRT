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

import org.iotivity.cloud.accountserver.db.CertificateTable;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;
import org.iotivity.cloud.accountserver.x509.cert.Utility;

import java.math.BigInteger;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static org.iotivity.cloud.accountserver.Constants.*;
import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.ACCOUNT_DB_MANAGER;


/**
 * This class is used as DB manager for CertificateTable.
 * With help of this class we can save certificate info to DB,
 * retrieve it from DB by specified device id,
 * update X509 certificate, also it helps us to get user Id from Token
 * Table by specified device id.
 */
final class CertificateManager {

    /**
     * Type casting manager for converting CertificateTable object tot map
     */
    private static final TypeCastingManager<CertificateTable> CERTIFICATE_TABLE_TYPE_CASTING_MANAGER =
            new TypeCastingManager<>();

    /**
     * Class attribute as payload for response.
     */
    private final Map<String, Object> payLoad;

    /**
     * Class attribute to store device id.
     */
    private final String deviceId;

    /**
     * Constructs certificateMananger with specified device id. Initialize payload
     * as new hash map instance.
     *
     * @param deviceId specified device identifier for this CertificateManager.
     */
    CertificateManager(String deviceId) {
        payLoad = new HashMap<>();
        this.deviceId = deviceId;
    }

    /**
     * Returns payload for response.
     */
    Map<String, Object> getPayLoad() {
        return payLoad;
    }

    /**
     * Puts specified value for specified key to payload;
     *
     * @param key   specified String key value
     * @param value specified Object value
     */
    public void put(String key, Object value) {
        payLoad.put(key, value);
    }

    /**
     * Saves certificate information: serial number, not after, to DB with specified columns.
     *
     * @param serialNumber specified certificate serial number
     * @param notAfter     validation date not after
     * @param notBefore    validation date not before
     */
    void save(BigInteger serialNumber, Date notAfter, Date notBefore) {
        ACCOUNT_DB_MANAGER.insertRecord(CERTIFICATE_TABLE,
                CERTIFICATE_TABLE_TYPE_CASTING_MANAGER.convertObjectToMap(
                        new CertificateTable(serialNumber.toString(), notAfter,
                                notBefore, deviceId, Utility.getUserID(deviceId), false)));
    }

    /**
     * Updates certificate table by revoked column.
     *
     * @param certificateTable certificate to be updated.
     * @param revoked          specified value for revoke
     */
    void update(CertificateTable certificateTable, boolean revoked) {
        certificateTable.setRevoked(revoked);
        ACCOUNT_DB_MANAGER.updateRecord(CERTIFICATE_TABLE,
                CERTIFICATE_TABLE_TYPE_CASTING_MANAGER.convertObjectToMap(certificateTable));
    }

    /**
     * Returns certificate from database for specified in constructor
     * device id.
     */
    public CertificateTable getCertificate() {
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(KEYFIELD_DID, deviceId);
        condition.put(KEYFIELD_REVOKED, false);
        List<HashMap<String, Object>> listMap = ACCOUNT_DB_MANAGER.selectRecord(
                CERTIFICATE_TABLE, condition);
        if (listMap != null && !listMap.isEmpty()) {
            return CERTIFICATE_TABLE_TYPE_CASTING_MANAGER
                    .convertMaptoObject(
                            listMap.get(0),
                            new CertificateTable());
        } else {
            return null;
        }
    }
}