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

import org.bson.types.Binary;

import java.util.Date;

public class CRLTable {

    private Date thisUpdate;

    private Binary binaryData;

    public CRLTable() {
    }

    public CRLTable(Date thisUpdate, Binary binaryData) {
        this.thisUpdate = thisUpdate;
        this.binaryData = binaryData;
    }

    public Date getThisUpdate() {
        return thisUpdate;
    }

    public void setThisUpdate(Date thisUpdate) {
        this.thisUpdate = thisUpdate;
    }

    public Binary getBinaryData() {
        return binaryData;
    }

    public void setBinaryData(Binary binaryData) {
        this.binaryData = binaryData;
    }

    @Override
    public String toString() {
        return "CRLTable{" +
                "thisUpdate=" + thisUpdate +
                ", binaryData=" + binaryData +
                '}';
    }
}
