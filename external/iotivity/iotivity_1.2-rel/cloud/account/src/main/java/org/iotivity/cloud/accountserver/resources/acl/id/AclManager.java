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
package org.iotivity.cloud.accountserver.resources.acl.id;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.UUID;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.db.AclTable;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;

import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.protocols.IRequest;

import org.iotivity.cloud.util.Log;

public class AclManager {
    public HashMap<String, Acl>          mAcls    = new HashMap<>();
    private TypeCastingManager<AclTable> mTypeAcl = new TypeCastingManager<AclTable>();

    public HashMap<String, Object> createAcl(String oid, String di) {
        HashMap<String, Object> responsePayload = new HashMap<>();
        String aclid = null;
        aclid = UUID.randomUUID().toString();

        AclTable newAclTable = new AclTable(aclid, oid, di, null, null);

        AccountDBManager.getInstance().insertRecord(Constants.ACL_TABLE,
                mTypeAcl.convertObjectToMap(newAclTable));
        mAcls.put(aclid, new Acl(aclid));
        responsePayload.put(Constants.REQ_ACL_ID, aclid);
        return responsePayload;
    }

    public Acl getAcl(String aclid) {
        return mAcls.get(aclid);
    }

    public HashMap<String, Object> getAclid(String di) {
        HashMap<String, Object> responsePayload = new HashMap<>();
        String aclid = null;
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.KEYFIELD_DI, di);
        ArrayList<HashMap<String, Object>> result = AccountDBManager
            .getInstance().selectRecord(Constants.ACL_TABLE, condition);
        if (result != null)
        {
            for (HashMap<String, Object> element : result) {
                AclTable getAclTable = new AclTable();
                getAclTable = Acl.convertMaptoAclObject(element);
                aclid = getAclTable.getAclid();
                responsePayload.put(Constants.KEYFIELD_ACLID, aclid);
                return responsePayload;
            }
        }
        return null;
    }

    public void deleteAcl(String aclid) {
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.REQ_ACL_ID, aclid);
        AccountDBManager.getInstance().deleteRecord(Constants.ACL_TABLE,
            condition);
        mAcls.remove(aclid);
    }

    public List<HashMap<String, Object>> addAclACE(String aclid, List<HashMap<String, Object>> aclist) {
        return getAcl(aclid).addACE(aclist);
    }

    public HashMap<String, Object> getAclACE(String aclid, String aceid) {
        return getAcl(aclid).getACE(aceid);
    }

    public void updateACE(String aclid, String aceid, HashMap<String, Object> ace) {
        if(getAcl(aclid).isValidAceId(aceid))
        {
            getAcl(aclid).updateACE(aceid, ace);
        }
        else
        {
            throw new BadRequestException("Invalid parameters");
        }
    }

    public void deleteAclACE(String aclid, String aceid) {
        if(getAcl(aclid).isValidAceId(aceid))
        {
            getAcl(aclid).deleteACE(aceid);
        }
        else
        {
            throw new BadRequestException("Invalid parameters");
        }
    }
    public void deleteAclAclist(String aclid) {
        getAcl(aclid).deleteAclist();
    }

    public HashMap<String, Object> addAclSubscriber(String aclid, String di,
        Device srcDevice, IRequest request) {
        return getAcl(aclid).addSubscriber(di, srcDevice, request);
    }

    public HashMap<String, Object> removeAclSubscriber(String aclid, String di) {
        return getAcl(aclid).removeSubscriber(di);
    }

    public HashMap<String, Object> getAclInfo(String aclid) {
        return getAcl(aclid).getInfo();
    }

}
