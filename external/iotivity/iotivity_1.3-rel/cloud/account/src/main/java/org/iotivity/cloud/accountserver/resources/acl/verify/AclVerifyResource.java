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
package org.iotivity.cloud.accountserver.resources.acl.verify;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.db.AclTable;
import org.iotivity.cloud.accountserver.resources.acl.id.Ace;
import org.iotivity.cloud.accountserver.resources.acl.id.AceResource;
import org.iotivity.cloud.accountserver.resources.acl.id.Acl;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.resource.Resource;
import org.iotivity.cloud.util.Cbor;

public class AclVerifyResource extends Resource {

    private Cbor<HashMap<String, Object>> mCbor = new Cbor<>();

    public AclVerifyResource() {
        super(Arrays.asList(Constants.PREFIX_OIC, Constants.ACL_URI,
                Constants.VERIFY_URI));
    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {

        IResponse response = null;

        switch (request.getMethod()) {
            case GET:
                response = handleGetRequest(request);
                break;
            default:
                throw new BadRequestException(
                        request.getMethod() + " request type is not supported");
        }

        srcDevice.sendResponse(response);
    }

    private boolean checkPermission(int permissionValue, String rme)
            throws ServerException {
        Permission per = null;
        int rm_value = 0;
        if (rme.equals("get")) {
            per = Permission.Read;
        } else if (rme.equals("post")) {
            per = Permission.Update;
        } else if (rme.equals("delete")) {
            per = Permission.Delete;
        }
        if (per != null) {
            rm_value = per.getValue();
        }
        // bit and operation
        return ((permissionValue & rm_value) == rm_value);
    }

    private boolean checkResourceUri(List<AceResource> aceResources, String uri)
            throws ServerException {
        for (AceResource aceResource : aceResources) {
            if (aceResource.getHref().trim().equals("*")
                    || aceResource.getHref().equals(uri)) {
                return true;
            }
        }
        return false;
    }

    private boolean verifyAcl(String sid, String di, String rm, String uri)
            throws ServerException {

        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.KEYFIELD_DI, di);

        // Query AclTable with condition deviceId(di)
        ArrayList<HashMap<String, Object>> aclResult = AccountDBManager
                .getInstance().selectRecord(Constants.ACL_TABLE, condition);

        // if aclResult size is zero then (di) does not exist
        if (aclResult == null || aclResult.size() == 0) {
            return false;
        }

        for (HashMap<String, Object> eachAclMap : aclResult) {

            AclTable aclTable = Acl.convertMaptoAclObject(eachAclMap);
            if (aclTable.getOid().equals(sid)) {
                return true;
            }
            if (aclTable.getAclist() == null) {
                return false;
            }

            for (Ace ace : aclTable.getAclist()) {
                if (ace.getSubjectuuid().equals(sid)) {
                    // check permission matches
                    if (checkPermission(ace.getPermission(),
                            rm.toLowerCase())) {
                        // check resource uri matches
                        if (checkResourceUri(ace.getResources(), uri)) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    private IResponse handleGetRequest(IRequest request)
            throws ServerException {

        String sid = null;
        String di = null;
        String rm = null;
        String uri = null;

        HashMap<String, List<String>> queryMap = request.getUriQueryMap();

        checkQueryException(Arrays.asList(Constants.REQ_SEARCH_USER_ID,
                Constants.REQ_DEVICE_ID, Constants.REQ_REQUEST_METHOD,
                Constants.REQ_REQUEST_URI), queryMap);

        sid = queryMap.get(Constants.REQ_SEARCH_USER_ID).get(0);
        di = queryMap.get(Constants.REQ_DEVICE_ID).get(0);
        rm = queryMap.get(Constants.REQ_REQUEST_METHOD).get(0);
        uri = queryMap.get(Constants.REQ_REQUEST_URI).get(0);

        HashMap<String, Object> responsePayload = new HashMap<>();
        if (verifyAcl(sid, di, rm, uri)) {
            responsePayload.put(Constants.RESP_GROUP_PERMISSION,
                    Constants.RESP_ACL_ALLOWED);
        } else {
            responsePayload.put(Constants.RESP_GROUP_PERMISSION,
                    Constants.RESP_ACL_DENIED);
        }

        return MessageBuilder.createResponse(request, ResponseStatus.CONTENT,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(responsePayload));
    }

}
