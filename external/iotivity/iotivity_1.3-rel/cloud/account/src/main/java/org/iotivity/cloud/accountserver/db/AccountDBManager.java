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
package org.iotivity.cloud.accountserver.db;

import java.net.InetAddress;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.bson.Document;
import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.base.exception.ServerException.InternalServerErrorException;

/**
 *
 * This class provides a set of APIs managing database.
 *
 */
public class AccountDBManager {

    private static AccountDBManager            accountDBManager;
    private MongoDB                            mongoDB;
    private HashMap<String, ArrayList<String>> keyField        = new HashMap<String, ArrayList<String>>();

    private AccountDBManager(String dbHost) {
        createDatabase(dbHost);
        createTables();
        createIndexes();
    }

    private void createDatabase(String dbHost) {

        try {
            mongoDB = new MongoDB(dbHost, Constants.DB_NAME);
        } catch (Exception e) {
            e.printStackTrace();
            throw new InternalServerErrorException(
                    "Database or Table create failed!");
        }
    }

    private void createTables() {

        mongoDB.createTable(Constants.USER_TABLE);
        mongoDB.createTable(Constants.TOKEN_TABLE);
        mongoDB.createTable(Constants.GROUP_TABLE);
        mongoDB.createTable(Constants.INVITE_TABLE);
        mongoDB.createTable(Constants.DEVICE_TABLE);
        mongoDB.createTable(Constants.ACL_TABLE);
        mongoDB.createTable(Constants.ACE_TABLE);
        mongoDB.createTable(Constants.ACLTEMPLATE_TABLE);
        mongoDB.createTable(Constants.CERTIFICATE_TABLE);
        mongoDB.createTable(Constants.CRL_TABLE);
    }

    private void createIndexes() {

        ArrayList<String> keys = new ArrayList<>();
        keys.add(Constants.KEYFIELD_UUID);

        mongoDB.createIndex(Constants.USER_TABLE, keys);
        keyField.put(Constants.USER_TABLE, keys);

        keys = new ArrayList<>();
        keys.add(Constants.KEYFIELD_UUID);
        keys.add(Constants.KEYFIELD_DID);

        mongoDB.createIndex(Constants.TOKEN_TABLE, keys);
        keyField.put(Constants.TOKEN_TABLE, keys);

        keys = new ArrayList<>();
        keys.add(Constants.KEYFIELD_GID);

        mongoDB.createIndex(Constants.GROUP_TABLE, keys);
        keyField.put(Constants.GROUP_TABLE, keys);

        keys = new ArrayList<>();
        keys.add(Constants.KEYFIELD_INVITE_USER);
        keys.add(Constants.KEYFIELD_GID);
        keys.add(Constants.KEYFIELD_INVITED_USER);

        mongoDB.createIndex(Constants.INVITE_TABLE, keys);
        keyField.put(Constants.INVITE_TABLE, keys);

        keys = new ArrayList<>();
        keys.add(Constants.KEYFIELD_UUID);
        keys.add(Constants.KEYFIELD_DID);

        mongoDB.createIndex(Constants.DEVICE_TABLE, keys);
        keyField.put(Constants.DEVICE_TABLE, keys);

        keys = new ArrayList<>();
        keys.add(Constants.KEYFIELD_ACLID);

        mongoDB.createIndex(Constants.ACL_TABLE, keys);
        keyField.put(Constants.ACL_TABLE, keys);

        keys = new ArrayList<>();
        keys.add(Constants.KEYFIELD_GID);
        keys.add(Constants.KEYFIELD_DI);
        keys.add(Constants.KEYFIELD_UID);
        keys.add(Constants.KEYFIELD_OID);
        keys.add(Constants.KEYFIELD_ACE_RESOURCE_HREF);

        mongoDB.createIndex(Constants.ACE_TABLE, keys);
        keyField.put(Constants.ACE_TABLE, keys);

        mongoDB.createIndex(Constants.ACLTEMPLATE_TABLE, keys);
        keyField.put(Constants.ACLTEMPLATE_TABLE, keys);

        keys = new ArrayList<>();
        keys.add(Constants.KEYFIELD_SN);
        mongoDB.createIndex(Constants.CERTIFICATE_TABLE, keys);
        keyField.put(Constants.CERTIFICATE_TABLE, keys);

        keys = new ArrayList<>();
        keys.add(Constants.REQ_THIS_UPDATE);
        mongoDB.createIndex(Constants.CRL_TABLE, keys);
        keyField.put(Constants.CRL_TABLE, keys);

    }

    /**
     * API to implement singleton pattern based DB manager
     * 
     * @return account DB manager
     */
    public static AccountDBManager getInstance() {
        if (accountDBManager == null)
            accountDBManager = new AccountDBManager("127.0.0.1");
        return accountDBManager;
    }

    /**
     * API to create DBManager instance with specific host
     *
     * @return created DB manager
     */
    public static AccountDBManager createInstance(String dbHost) {
        if (accountDBManager == null)
            accountDBManager = new AccountDBManager(dbHost);
        return accountDBManager;
    }

    /**
     * API for inserting a record into DB table. the record will not be inserted
     * if duplicated one.
     * 
     * @param tableName
     *            table name to be inserted
     * @param record
     *            record to be inserted
     */
    public void insertRecord(String tableName, HashMap<String, Object> insert) {

        if (!_insertRecord(tableName, insert))
            throw new InternalServerErrorException(
                    "Database record insert failed");
    }

    /**
     * API for inserting a record into DB table. the record will be replaced if
     * duplicated one.
     * 
     * @param tableName
     *            table name to be inserted
     * @param replace
     *            record to be inserted
     */
    public void insertAndReplaceRecord(String tableName,
            HashMap<String, Object> replace) {

        if (!_insertAndReplaceRecord(tableName, replace))
            throw new InternalServerErrorException(
                    "Database record insert failed");
    }

    /**
     * API for selecting records from DB table.
     * 
     * @param tableName
     *            table name to be inserted
     * @param condition
     *            condition record to be selected
     * @return selected records
     */
    public ArrayList<HashMap<String, Object>> selectRecord(String tableName,
            HashMap<String, Object> condition) {

        return _selectRecord(tableName, condition);
    }

    /**
     * API for selecting records to primary key from DB table
     * 
     * @param tableName
     *            table name to be selected
     * 
     * @param condition
     *            condition record to be selected
     * @return selected record
     */

    public HashMap<String, Object> selectOneRecord(String tableName,
            HashMap<String, Object> condition) {

        ArrayList<HashMap<String, Object>> records = _selectRecord(tableName,
                condition);

        if (records.size() > 1) {
            throw new InternalServerErrorException(
                    "Database record select failed");
        }

        if (records.isEmpty()) {
            return new HashMap<String, Object>();
        } else {
            return records.get(0);
        }

    }

    /**
     * API for deleting records from DB table.
     * 
     * @param tableName
     *            table name to be inserted
     * @param condition
     *            condition record to be deleted
     */
    public void deleteRecord(String tableName,
            HashMap<String, Object> condition) {

        if (!_deleteRecord(tableName, condition))
            throw new InternalServerErrorException(
                    "Database record delete failed");
    }

    /**
     * API for updating a record into DB table.
     * 
     * @param tableName
     *            table name to be inserted
     * @param replace
     *            record to be updated
     */
    public void updateRecord(String tableName,
            HashMap<String, Object> replace) {

        if (!_updateRecord(tableName, replace))
            throw new InternalServerErrorException(
                    "Database record update failed");

    }

    private Boolean _insertRecord(String tableName,
            HashMap<String, Object> record) {

        Document doc = createDocument(record);

        return mongoDB.insertRecord(tableName, doc);
    }

    private Boolean _insertAndReplaceRecord(String tableName,
            HashMap<String, Object> record) {

        Document doc = createDocument(record);
        Document filter = getKeyFilter(tableName, record);

        return mongoDB.insertAndReplaceRecord(tableName, filter, doc);
    }

    private Boolean _deleteRecord(String tableName,
            HashMap<String, Object> condition) {

        Document doc = createDocument(condition);

        return mongoDB.deleteRecord(tableName, doc);
    }

    private Boolean _updateRecord(String tableName,
            HashMap<String, Object> record) {

        Document replace = createDocument(record);
        Document filter = getKeyFilter(tableName, record);

        return mongoDB.updateRecord(tableName, filter, replace);
    }

    private ArrayList<HashMap<String, Object>> _selectRecord(String tableName,
            HashMap<String, Object> record) {

        Document doc = createDocument(record);

        return mongoDB.selectRecord(tableName, doc);
    }

    private Document getKeyFilter(String tableName,
            HashMap<String, Object> record) {

        Document filterDoc = new Document();

        ArrayList<String> keys = keyField.get(tableName);

        for (String key : keys) {

            String value = (String) record.get(key);
            filterDoc.append(key, value);
        }

        return filterDoc;
    }

    private Document createDocument(HashMap<String, Object> record) {

        Document doc = new Document();
        Set<Entry<String, Object>> resEntrySet = record.entrySet();
        Iterator<Entry<String, Object>> entryIter = resEntrySet.iterator();

        while (entryIter.hasNext()) {
            Map.Entry<String, Object> entry = (Map.Entry<String, Object>) entryIter
                    .next();
            doc.append(entry.getKey().toString(), entry.getValue());
        }

        return doc;
    }

}
