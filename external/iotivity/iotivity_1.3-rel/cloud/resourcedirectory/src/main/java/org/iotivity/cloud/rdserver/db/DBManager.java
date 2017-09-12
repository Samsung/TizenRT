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
package org.iotivity.cloud.rdserver.db;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.bson.Document;
import org.iotivity.cloud.base.exception.ServerException.InternalServerErrorException;
import org.iotivity.cloud.rdserver.Constants;

/**
 *
 * This class provides a set of APIs to access a DataBase.
 *
 */
public class DBManager {

    private static DBManager                   mDBManager;
    private MongoDB                            mMongoDB   = null;
    private HashMap<String, ArrayList<String>> mKeyField  = new HashMap<>();

    private DBManager(String dbHost) {
        createDatabase(dbHost);
        createTables();
        createIndexes();
    }

    /**
     * API to return DBManager object
     * 
     * @return DBManager DBManager object
     */
    public static DBManager getInstance() {
        if (mDBManager == null)
            mDBManager = new DBManager("127.0.0.1");
        return mDBManager;
    }

    /**
     * API to create DBManager instance with specific host
     *
     * @return created DB manager
     */
    public static DBManager createInstance(String dbHost) {
        if (mDBManager == null)
            mDBManager = new DBManager(dbHost);
        return mDBManager;
    }

    private void createDatabase(String dbHost) {
        try {
            mMongoDB = new MongoDB(dbHost, Constants.RD_DB_NAME);
        } catch (Exception e) {
            e.printStackTrace();
            throw new InternalServerErrorException("Database create failed!");
        }
    }

    private void createTables() {
        mMongoDB.createTable(Constants.RD_TABLE);
        mMongoDB.createTable(Constants.PRESENCE_TABLE);
    }

    private void createIndexes() {

        ArrayList<String> keys = new ArrayList<>();
        keys.add(Constants.DEVICE_ID);
        keys.add(Constants.INS);

        mMongoDB.createIndex(Constants.RD_TABLE, keys);
        mKeyField.put(Constants.RD_TABLE, keys);

        keys = new ArrayList<>();
        keys.add(Constants.DEVICE_ID);

        mMongoDB.createIndex(Constants.PRESENCE_TABLE, keys);
        mKeyField.put(Constants.PRESENCE_TABLE, keys);

    }

    /**
     * API for inserting a record into DB table. the record will not be inserted
     * if duplicated one.
     * 
     * @param tableName
     *            table name to be inserted
     * @param insert
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
     *            table name to be inserted or replaced
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
     *            table name to be selected
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

        if (records != null && records.size() > 1) {
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
     *            table name to be deleted
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
     *            table name to be updated
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

        return mMongoDB.insertRecord(tableName, doc);
    }

    private Boolean _insertAndReplaceRecord(String tableName,
            HashMap<String, Object> record) {

        Document doc = createDocument(record);
        Document filter = getKeyFilter(tableName, record);

        return mMongoDB.insertAndReplaceRecord(tableName, filter, doc);
    }

    private Boolean _deleteRecord(String tableName,
            HashMap<String, Object> condition) {

        Document doc = createDocument(condition);

        return mMongoDB.deleteRecord(tableName, doc);
    }

    private Boolean _updateRecord(String tableName,
            HashMap<String, Object> record) {

        Document replace = createDocument(record);
        Document filter = getKeyFilter(tableName, record);

        return mMongoDB.updateRecord(tableName, filter, replace);
    }

    private ArrayList<HashMap<String, Object>> _selectRecord(String tableName,
            HashMap<String, Object> record) {

        Document doc = createDocument(record);

        return mMongoDB.selectRecord(tableName, doc);
    }

    private Document getKeyFilter(String tableName,
            HashMap<String, Object> record) {

        Document filterDoc = new Document();

        ArrayList<String> keys = mKeyField.get(tableName);

        for (String key : keys) {

            Object value = record.get(key);
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