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
package org.iotivity.cloud.rdserver.resources.directory.rd;

import java.util.ArrayList;
import java.util.HashMap;

import org.iotivity.cloud.rdserver.Constants;
import org.iotivity.cloud.rdserver.db.DBManager;

/**
 *
 * This class provides a set of APIs to handle ins(unique value of resource)
 *
 */
public class InsManager {

    private HashMap<String, Integer> mNextIns      = new HashMap<>();
    private int                      mInitialValue = 1;

    /**
     * API for getting ins from DB
     * 
     * @param di
     *            device id
     * @param href
     *            resource uri
     * @return ins
     */
    public int getIns(String di, String href) {

        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.DEVICE_ID, di);
        condition.put(Constants.HREF, href);
        ArrayList<HashMap<String, Object>> records = DBManager.getInstance()
                .selectRecord(Constants.RD_TABLE, condition);

        if (records.isEmpty()) {
            return -1;
        } else {
            return (int) (records.get(0).get(Constants.INS));
        }
    }

    /**
     * API for creating ins
     * 
     * @param di
     *            device id
     * @return created ins
     */
    public int createIns(String di) {
        Object objectIns;

        objectIns = mNextIns.get(di);
        if (objectIns == null) {
            mNextIns.put(di, mInitialValue + 1);
            return mInitialValue;
        } else {
            int ins = (int) objectIns;
            mNextIns.put(di, ins + 1);
            return ins;
        }
    }
}
