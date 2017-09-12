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
import org.iotivity.cloud.util.Log;

import com.mongodb.MongoClient;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoCursor;
import com.mongodb.client.MongoDatabase;
import com.mongodb.client.model.IndexOptions;

/**
 *
 * This class provides a set of APIs to use MongoDB APIs.
 *
 */
public class MongoDB {

    private MongoClient   mongoClient = null;
    private MongoDatabase db          = null;

    /**
     * API creating MongoClient and initializing MongoDatabase
     *
     * @param host
     *            host of MongoDatabase
     * @param dbname
     *            database name to create MongoDatabase
     * @throws Exception
     */
    public MongoDB(String host, String dbname) throws Exception {

        mongoClient = new MongoClient(host);
        mongoClient.dropDatabase(dbname);
        db = mongoClient.getDatabase(dbname);
    }

    /**
     * API for creating collection
     *
     * @param tableName
     *            collection name
     */
    public void createTable(String tableName) {

        db.createCollection(tableName);
    }

    /**
     * API for creating index
     *
     * @param tableName
     *            collection name
     * @param keys
     *            key fields of collection
     */
    public void createIndex(String tablename, ArrayList<String> keys) {

        Document doc = new Document();

        for (String key : keys) {

            doc.append(key, 1);
        }

        IndexOptions options = new IndexOptions();
        options.unique(true);

        db.getCollection(tablename).createIndex(doc, options);
    }

    /**
     * API for deleting collection
     *
     * @param tableName
     *            collection name
     */
    public void deleteTable(String tableName) {

        db.getCollection(tableName).drop();
    }

    /**
     * API for getting database object
     *
     */
    public MongoDatabase getMongoDatabase() {

        return db;
    }

    /**
     * API for inserting a record into DB table. the record will not be inserted
     * if duplicated one.
     * 
     * @param tableName
     *            table name to be inserted
     * @param doc
     *            document to be inserted
     */
    public Boolean insertRecord(String tableName, Document doc) {

        if (tableName == null || doc == null)
            return false;

        MongoCollection<Document> collection = db.getCollection(tableName);

        try {

            if (collection.find(doc).first() == null) {

                collection.insertOne(doc);

            } else {

                Log.w("DB insert failed due to duplecated one.");
                return false;
            }

        } catch (Exception e) {

            e.printStackTrace();
            return false;
        }

        showRecord(tableName);

        return true;
    }

    /**
     * API for inserting a record into DB table. the record will be replaced if
     * duplicated one.
     * 
     * @param tableName
     *            table name to be inserted
     * @param filter
     *            document filter
     * @param doc
     *            document to be inserted
     * @return returns true if the record is inserted and replaced successfully,
     *         or returns false
     */
    public Boolean insertAndReplaceRecord(String tableName, Document filter,
            Document doc) {

        if (tableName == null || filter == null || doc == null)
            return false;

        MongoCollection<Document> collection = db.getCollection(tableName);

        try {

            if (collection.findOneAndReplace(filter, doc) == null) {

                collection.insertOne(doc);
            }

        } catch (Exception e) {

            e.printStackTrace();
            return false;
        }

        showRecord(tableName);

        return true;
    }

    /**
     * API for updating a record into DB table.
     * 
     * @param tableName
     *            table name to be updated
     * @param filter
     *            document filter
     * @param record
     *            record to be updated
     * @return returns true if the record is updated successfully, or returns
     *         false
     */
    public Boolean updateRecord(String tableName, Document filter,
            Document record) {

        if (tableName == null || filter == null || record == null)
            return false;

        MongoCollection<Document> collection = db.getCollection(tableName);

        if (collection.findOneAndReplace(filter, record) == null) {

            Log.w("DB update failed due to no matched record!");
            return false;
        }

        showRecord(tableName);

        return true;
    }

    /**
     * API for deleting records from DB table.
     * 
     * @param tableName
     *            table name for the record to be deleted
     * @param record
     *            record filter to be deleted
     * @return returns true if the record is deleted successfully, or returns
     *         false
     */
    public Boolean deleteRecord(String tableName, Document record) {

        if (tableName == null || record == null)
            return false;

        MongoCollection<Document> collection = db.getCollection(tableName);

        try {

            collection.deleteMany(record);

        } catch (Exception e) {

            e.printStackTrace();
            return false;
        }

        showRecord(tableName);

        return true;
    }

    /**
     * API for selecting records from DB table.
     * 
     * @param tableName
     *            table name for the record to be selected
     * @param doc
     *            document filter to be selected
     * @return record list according to the filter document
     */
    public ArrayList<HashMap<String, Object>> selectRecord(String tableName,
            Document doc) {

        if (tableName == null || doc == null)
            return new ArrayList<>();

        MongoCollection<Document> collection = db.getCollection(tableName);
        MongoCursor<Document> cursor = collection.find(doc).iterator();

        ArrayList<HashMap<String, Object>> recordList = new ArrayList<>();

        try {

            while (cursor.hasNext()) {
                Document selectedDoc = cursor.next();
                recordList.add(convertDocumentToHashMap(selectedDoc));
            }

        } finally {

            cursor.close();
        }

        return recordList;
    }

    private HashMap<String, Object> convertDocumentToHashMap(Document doc) {
        HashMap<String, Object> resourceMap = new HashMap<>();

        Set<Entry<String, Object>> entrySet = doc.entrySet();
        Iterator<Entry<String, Object>> entryIter = entrySet.iterator();

        while (entryIter.hasNext()) {

            Map.Entry<String, Object> entry = (Map.Entry<String, Object>) entryIter
                    .next();

            String entryKey = entry.getKey();

            // remove a mongoDB index
            if (entry.getValue() != null && !entryKey.equals("_id")) {

                resourceMap.put(entry.getKey(), entry.getValue());
            }
        }

        return resourceMap;
    }

    private void showRecord(String tableName) {

        MongoCollection<Document> collection = db.getCollection(tableName);
        MongoCursor<Document> cursor = collection.find().iterator();

        Log.i("<" + tableName + ">");

        HashMap<String, Object> records = null;
        int index = 0;
        while (cursor.hasNext()) {

            Document doc = cursor.next();
            records = convertDocumentToHashMap(doc);

            Log.i("[" + index + "] " + records.toString());
            index++;
        }

        cursor.close();
    }
}
