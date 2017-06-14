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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;

public class GroupTable {
    private String            gid       = null;
    private String            gname     = null;
    private String            owner     = null;
    private ArrayList<String> members   = null;
    private ArrayList<String> masters   = null;
    private ArrayList<String> devices   = null;
    private ArrayList<Object> resources = null;
    private ArrayList<String> subgroups = null;
    private String            parent    = null;
    private ArrayList<Object> gacl      = null;

    public GroupTable() {
        setGacl(makeGAcl());
    }

    private ArrayList<Object> makeGAcl() {
        HashMap<String, Object> publicGAcl = new HashMap<>();
        publicGAcl.put(Constants.KEYFIELD_ACE_PERMISSION, (int) 31);
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        HashMap<String, Object> resource = new HashMap<>();
        resource.put(Constants.KEYFIELD_ACE_RESOURCE_HREF, "*");
        resource.put(Constants.KEYFIELD_ACE_RESOURCE_RT,
                new ArrayList<String>());
        resource.put(Constants.KEYFIELD_ACE_RESOURCE_IF,
                Arrays.asList(Constants.DEFAULT_INTERFACE));
        resources.add(resource);
        publicGAcl.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);

        ArrayList<Object> gaclList = new ArrayList<>();
        gaclList.add(publicGAcl);

        return gaclList;
    }

    public GroupTable(String gid, String gname, String owner,
            ArrayList<String> masters, ArrayList<String> members,
            ArrayList<String> devices, ArrayList<Object> resources,
            ArrayList<String> subgroups, String parent,
            ArrayList<Object> gacl) {
        this.gid = gid;
        this.gname = gname;
        this.owner = owner;
        this.members = members;
        this.masters = masters;
        this.devices = devices;
        this.resources = resources;
        this.subgroups = subgroups;
        this.parent = parent;
        this.gacl = gacl;
    }

    /**
     * API to get group ID
     * 
     * @return group ID
     */
    public String getGid() {
        return gid;
    }

    /**
     * API to set group ID
     * 
     * @param gid
     *            group ID to be set
     */
    public void setGid(String gid) {
        this.gid = gid.toString();
    }

    public String getGname() {
        return gname;
    }

    public void setGname(String gname) {
        this.gname = gname.toString();
    }

    public String getOwner() {
        return owner;
    }

    public void setOwner(String owner) {
        this.owner = owner.toString();
    }

    public ArrayList<String> getMasters() {
        return masters;
    }

    public void setMasters(ArrayList<String> masters) {
        this.masters = masters;
    }

    public ArrayList<String> getMembers() {
        return members;
    }

    public void setMembers(ArrayList<String> members) {
        this.members = members;
    }

    public ArrayList<String> getDevices() {
        return devices;
    }

    public void setDevices(ArrayList<String> devices) {
        this.devices = devices;
    }

    public ArrayList<Object> getResources() {
        return resources;
    }

    public void setResources(ArrayList<Object> resources) {
        this.resources = resources;
    }

    public ArrayList<String> getSubgroups() {
        return subgroups;
    }

    public void setSubgroups(ArrayList<String> subgroups) {
        this.subgroups = subgroups;
    }

    public String getParent() {
        return parent;
    }

    public void setParent(String parentGid) {
        this.parent = parentGid;
    }

    /**
     * API to get group ACL
     * 
     * @return group ACL
     */
    public ArrayList<Object> getGacl() {
        return gacl;
    }

    /**
     * API to set group ACL of the group
     * 
     * @param gacl
     *            group ACL to be set
     */
    public void setGacl(ArrayList<Object> gacl) {
        this.gacl = gacl;
    }

    public <T> T getPropertyValue(String property) {
        switch (property) {
            case Constants.REQ_GROUP_ID:
                return (T) gid;
            case Constants.KEYFIELD_GROUP_NAME:
                return (T) gname;
            case Constants.KEYFIELD_GROUP_OWNER:
                return (T) owner;
            case Constants.KEYFIELD_GROUP_MEMBERS:
                return (T) members;
            case Constants.KEYFIELD_GROUP_MASTERS:
                return (T) masters;
            case Constants.KEYFIELD_GROUP_DEVICES:
                return (T) devices;
            case Constants.KEYFIELD_GROUP_RESOURCES:
                return (T) resources;
            case Constants.KEYFIELD_GROUP_SUBGROUPS:
                return (T) subgroups;
            case Constants.KEYFIELD_GROUP_GACL:
                return (T) gacl;
            default:
                throw new BadRequestException(
                        property + " is not supported in the group");
        }
    }

    public void setPropertyValue(String property, Object value) {
        switch (property) {
            case Constants.REQ_GROUP_ID:
                this.gid = (String) value;
                break;
            case Constants.KEYFIELD_GROUP_NAME:
                this.gname = (String) value;
                break;
            case Constants.KEYFIELD_GROUP_OWNER:
                this.owner = (String) value;
                break;
            case Constants.KEYFIELD_GROUP_MEMBERS:
                this.members = (ArrayList<String>) value;
                break;
            case Constants.KEYFIELD_GROUP_MASTERS:
                this.masters = (ArrayList<String>) value;
                break;
            case Constants.KEYFIELD_GROUP_DEVICES:
                this.devices = (ArrayList<String>) value;
                break;
            case Constants.KEYFIELD_GROUP_RESOURCES:
                this.resources = (ArrayList<Object>) value;
                break;
            case Constants.KEYFIELD_GROUP_SUBGROUPS:
                this.subgroups = (ArrayList<String>) value;
                break;
            case Constants.KEYFIELD_GROUP_GACL:
                this.gacl = (ArrayList<Object>) value;
                break;
            default:
                throw new BadRequestException(
                        property + " is not supported in the group");
        }
    }
}
