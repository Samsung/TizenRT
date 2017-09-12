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
package org.iotivity.cloud.accountserver.resources.credprov.crl;

import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.ACCOUNT_DB_MANAGER;
import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.CERTIFICATE_FACTORY;
import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.DER;
import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.NEXT_UPDATE_INTERVAL;
import static org.iotivity.cloud.accountserver.x509.crl.CrlIssuer.CRL_ISSUER;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.security.cert.CRLException;
import java.security.cert.X509CRL;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import org.bouncycastle.operator.OperatorCreationException;
import org.bson.types.Binary;
import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.CRLTable;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.util.Log;

/**
 * Class is used to manage CRLs. It helps to create, update CRLS, revoke
 * certificates.
 */
public final class CrlManager {

    /**
     * Casting manager for working with CRLTable in mongo db
     */
    private static TypeCastingManager<CRLTable> castingManager = new TypeCastingManager<>();

    /**
     * X509 CRL presentation.
     */
    private X509CRL                             x509CRL;

    /**
     * Static manager for CRLs.
     */
    public static final CrlManager              CRL_MANAGER    = new CrlManager();

    /**
     * Private constructor to make this class non-instantiable.
     */
    private CrlManager() {
        try {
            Calendar calendar = Calendar.getInstance();
            Date thisUpdate = calendar.getTime();
            calendar.add(Calendar.DAY_OF_MONTH,
                    Integer.parseInt(NEXT_UPDATE_INTERVAL));
            byte[] data = CRL_ISSUER.generate(thisUpdate, calendar.getTime(),
                    Collections.emptyList());
            ACCOUNT_DB_MANAGER.insertRecord(Constants.CRL_TABLE,
                    castingManager.convertObjectToMap(
                            new CRLTable(thisUpdate, new Binary(data))));
            setX509CRL(data);
        } catch (CRLException | IOException | OperatorCreationException e) {
            Log.e(e.getMessage());
        }
    }

    /**
     * Revokes specified serial numbers. Puts them to database.
     *
     * @param serialNumbers
     *            specified var args serial numbers from 0.
     */
    public void revoke(String... serialNumbers)
            throws CRLException, IOException, OperatorCreationException {
        if (x509CRL != null) {
            update(x509CRL.getThisUpdate(),
                    CRL_ISSUER.generate(x509CRL.getThisUpdate(),
                            x509CRL.getNextUpdate(),
                            x509CRL.getRevokedCertificates(), serialNumbers));
        }
    }

    /**
     * Checks last update less than crl this update and returns response
     * payload, including this update, next update, and CRL in DER encoding.
     */
    Map<String, Object> getPayload(String lastUpdate)
            throws ServerException.PreconditionFailedException, CRLException {
        if (checkLastUpdate(lastUpdate) && x509CRL != null) {
            Map<String, Object> responsePayload = new HashMap<>();
            responsePayload.put(Constants.REQ_THIS_UPDATE,
                    new SimpleDateFormat("yyyyMMddHHmmss")
                            .format(x509CRL.getThisUpdate()));
            responsePayload.put(Constants.REQ_NEXT_UPDATE,
                    new SimpleDateFormat("yyyyMMddHHmmss")
                            .format(x509CRL.getNextUpdate()));
            responsePayload.put(Constants.REQ_CRL,
                    new CRL(DER, x509CRL.getEncoded()));
            return responsePayload;
        }
        return Collections.emptyMap();
    }

    /**
     * Checks if last update is before CRL this update.
     *
     * @param lastUpdate
     *            specified last update;
     * @return true if before and false - otherwise.
     */
    private boolean checkLastUpdate(String lastUpdate) {
        boolean checkCondition = false;
        try {
            if (x509CRL != null) {
                checkCondition = new SimpleDateFormat("yyyyMMddHHmmss")
                        .parse(lastUpdate).before(x509CRL.getThisUpdate());
            }
        } catch (ParseException e) {
            Log.e(e.getMessage());
        }
        return checkCondition;
    }

    /**
     * Updates CRLTable with specified this update and binary CRL data.
     */
    void update(Date thisUpdate, byte[] data) throws CRLException {
        ArrayList<HashMap<String, Object>> crlList = ACCOUNT_DB_MANAGER
                .selectRecord(Constants.CRL_TABLE, new HashMap<>());
        if (crlList != null && !crlList.isEmpty()) {
            CRLTable crlTable = castingManager
                    .convertMaptoObject(crlList.get(0), new CRLTable());
            crlTable.setThisUpdate(thisUpdate);
            crlTable.setBinaryData(new Binary(data));
            ACCOUNT_DB_MANAGER.updateRecord(Constants.CRL_TABLE,
                    castingManager.convertObjectToMap(crlTable));
            setX509CRL(data);
        }
    }

    /**
     * Updates currect x509 CRL state by parsing specified data.
     */
    private void setX509CRL(byte[] data) throws CRLException {
        x509CRL = (X509CRL) CERTIFICATE_FACTORY
                .generateCRL(new ByteArrayInputStream(data));
    }

    /**
     * Utility class for CBOR Crl presentation.
     */
    private static final class CRL {

        private final String encoding;

        private final byte[] data;

        CRL(String encoding, byte[] data) {
            this.encoding = encoding;
            this.data = data;
        }

        public String getEncoding() {
            return encoding;
        }

        public byte[] getData() {
            return data;
        }
    }
}
