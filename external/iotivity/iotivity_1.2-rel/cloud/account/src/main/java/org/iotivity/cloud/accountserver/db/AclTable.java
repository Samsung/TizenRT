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

import java.util.List;

import org.iotivity.cloud.accountserver.resources.acl.id.Ace;

public class AclTable {

    private String aclid;
    private String oid;
    private String di;
    private String rowneruuid;
    private List<Ace> aclist;

    public AclTable(String aclid, String oid, String di, String rowneruuid,
        List<Ace> aclist) {
        this.aclid = aclid;
        this.oid = oid;
        this.di = di;
        this.rowneruuid = rowneruuid;
        this.aclist = aclist;
    }

    public AclTable() {
    }

    public String getAclid() {
        return aclid;
    }

    public void setAclid(String aclid) {
        this.aclid = aclid;
    }

    public String getOid() {
        return oid;
    }

    public void setOid(String oid) {
        this.oid = oid;
    }

    public String getDi() {
        return di;
    }

    public void setDi(String di) {
        this.di = di;
    }

    public String getRowneruuid() {
        return rowneruuid;
    }

    public void setRowneruuid(String rowneruuid) {
        this.rowneruuid = rowneruuid;
    }

    public List<Ace> getAclist() {
        return aclist;
    }

    public void setAclist(List<Ace> aclist) {
        this.aclist = aclist;
    }

}
