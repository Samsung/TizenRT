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
package org.iotivity.cloud.accountserver.resources.acl.group;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.db.GroupTable;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.exception.ServerException.PreconditionFailedException;
import org.iotivity.cloud.util.Log;

/**
 *
 * This class provides a set of APIs to manage group
 *
 */

public class GroupManager {
    private static GroupManager            mGrManager          = new GroupManager();
    private TypeCastingManager<GroupTable> mTypeGroup          = new TypeCastingManager<GroupTable>();
    private GroupPolicyManager             mGroupPolicyManager = new GroupPolicyManager();
    private GroupAclManager                mGroupAclManager    = GroupAclManager
            .getInstance();

    /**
     * Function to get GroupManager as a singleton
     * 
     * @return GroupManager as a singleton
     */
    public static GroupManager getInstance() {
        return mGrManager;
    }

    /**
     * API to replace group name to the group
     * 
     * @param gid
     *            group id
     * @param gname
     *            group name
     */
    public void replaceGnameToGroup(String gid, String gname) {
        replaceProperties(gid, Constants.KEYFIELD_GROUP_NAME, gname);
    }

    /**
     * API to replace owner id to the group
     * 
     * @param gid
     *            group id
     * @param owner
     *            owner id
     */
    public void replaceOwnerToGroup(String gid, String owner) {
        replaceProperties(gid, Constants.KEYFIELD_GROUP_OWNER, owner);
    }

    /**
     * API to add members to the group
     * 
     * @param gid
     *            group id
     * @param values
     *            user uuid list
     */
    public void addMembersToGroup(String gid, ArrayList<String> members) {
        addProperties(gid, Constants.KEYFIELD_GROUP_MEMBERS, members);
    }

    /**
     * API to add masters to the group
     * 
     * @param gid
     *            group id
     * @param values
     *            user uuid list
     */
    public void addMastersToGroup(String gid, ArrayList<String> masters) {
        addProperties(gid, Constants.KEYFIELD_GROUP_MASTERS, masters);
        addProperties(gid, Constants.KEYFIELD_GROUP_MEMBERS, masters);
    }

    /**
     * API to add resources to the group
     * 
     * @param gid
     *            group id
     * @param values
     *            resource list
     */
    public void addResourcesToGroup(String gid, ArrayList<Object> resources) {
        ArrayList<Object> addedResources = new ArrayList<>();
        // filter added resource list : if the device is already to the
        // group, the resource is not added to the group
        for (Object resource : resources) {
            String deviceId = getDeviceIdFromResource(
                    (HashMap<String, Object>) resource);
            ArrayList<String> devices = getGroupTable(gid).getDevices();
            if (devices == null) {
                addedResources.add(resource);
            } else {
                if (!devices.contains(deviceId)) {
                    addedResources.add(resource);
                }
            }
        }
        addProperties(gid, Constants.KEYFIELD_GROUP_RESOURCES, addedResources);
    }

    /**
     * API to add devices to the group
     * 
     * @param gid
     *            group id
     * @param values
     *            device list
     */
    public void addDevicesToGroup(String gid, ArrayList<String> devices) {
        // if resources regarding to the device is already registered, delete
        // resources in advance
        deleteResourcesOfDevices(gid, devices);
        addProperties(gid, Constants.KEYFIELD_GROUP_DEVICES, devices);
    }

    /**
     * API to delete member list from the group
     * 
     * @param gid
     *            group id
     * @param values
     *            member uuid list
     */
    public void deleteMembersFromGroup(String gid, ArrayList<String> members) {
        GroupTable groupTable = getGroupTable(gid);
        ArrayList<String> devices = groupTable.getDevices();
        if (devices != null) {
            // delete devices owned by deleted members
            ArrayList<String> deletedDevices = new ArrayList<String>();
            for (String device : devices) {
                if (members.contains(findDeviceOwner(device))) {
                    deletedDevices.add(device);
                }
            }
            deleteDevicesFromGroup(gid, deletedDevices);
        }
        deleteProperties(gid, Constants.KEYFIELD_GROUP_MEMBERS, members);
        deleteProperties(gid, Constants.KEYFIELD_GROUP_MASTERS, members);
    }

    /**
     * API to delete device and resources of each device from all groups
     * 
     * @param device
     *            device id to be deleted from all groups
     */
    public void deleteDevicesFromAllGroup(String device) {
        ArrayList<HashMap<String, Object>> groupList = readGroupList(
                Constants.KEYFIELD_GROUP_MEMBERS, findDeviceOwner(device));
        if (groupList == null) {
            return;
        }
        ArrayList<String> devices = new ArrayList<>();
        devices.add(device);
        for (HashMap<String, Object> group : groupList) {
            // deleteProperties((String) group.get(Constants.REQ_GROUP_ID),
            // Constants.KEYFIELD_GROUP_DEVICES, devices);
            deleteDevicesFromGroup((String) group.get(Constants.REQ_GROUP_ID),
                    devices);
        }
    }

    private ArrayList<HashMap<String, Object>> readGroupList(String key,
            String value) {
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(key, value);
        ArrayList<HashMap<String, Object>> records = AccountDBManager
                .getInstance().selectRecord(Constants.GROUP_TABLE, condition);
        return records;
    }

    /**
     * API to delete master list from the group
     * 
     * @param gid
     *            group id
     * @param values
     *            master uuid list
     */
    public void deleteMastersFromGroup(String gid, ArrayList<String> masters) {
        deleteProperties(gid, Constants.KEYFIELD_GROUP_MASTERS, masters);
    }

    /**
     * API to delete resource list from the group
     * 
     * @param gid
     *            group id
     * @param values
     *            resource list
     */
    public void deleteResourcesFromGroup(String gid,
            ArrayList<Object> deletedResources) {
        deleteProperties(gid, Constants.KEYFIELD_GROUP_RESOURCES,
                deletedResources);
    }

    /**
     * API to delete device list from the group
     * 
     * @param gid
     *            group id
     * @param values
     *            device list
     */
    public void deleteDevicesFromGroup(String gid, ArrayList<String> devices) {
        // delete resources owned by deleted members
        deleteResourcesOfDevices(gid, devices);
        deleteProperties(gid, Constants.KEYFIELD_GROUP_DEVICES, devices);
    }

    /**
     * API to verify if the member user is eligible to add/delete/replace the
     * requested property values
     * 
     * @param gid
     *            group id
     * @param mid
     *            user uuid
     * @param properties
     *            property key/value map to check
     * @param operation
     *            user operation
     */
    public void verifyPostRequestAuthz(String gid, String mid,
            HashMap<String, Object> properties, UserOperation operation) {
        ArrayList<String> keySet = new ArrayList<String>();
        keySet.addAll(properties.keySet());
        mGroupPolicyManager.verifyOperationAuthorization(gid, mid, operation,
                keySet);
        switch (operation) {
            case ADD:
                verifyPostAddPolicy(gid, mid, properties);
                break;
            case DELETE:
                verifyPostDeletePolicy(gid, mid, properties);
                break;
            case REPLACE:
                verifyPostReplacePolicy(gid, mid, properties);
                break;
            default:
                throw new PreconditionFailedException(
                        operation + " is not supported");
        }
    }

    /**
     * API to get added property value list to the group among the entire value
     * list to update
     * 
     * @param gid
     *            group id
     * @param property
     *            property to update
     * @param values
     *            value list to update
     * @return property value list to be added to the group
     */
    public <T> ArrayList<T> getAddPropertyValues(String gid, String property,
            ArrayList<T> values) {
        GroupTable groupTable = getGroupTable(gid);
        if (groupTable == null) {
            throw new BadRequestException("group " + gid + " does not exist");
        }
        ArrayList<T> propertyValues = groupTable.getPropertyValue(property);
        ArrayList<T> addedValues = new ArrayList<>();
        for (int i = 0; i < values.size(); i++) {
            if (!propertyValues.contains(values.get(i))) {
                addedValues.add(values.get(i));
            }
        }
        return addedValues;
    }

    /**
     * API to get deleted property value list from the group
     * 
     * @param gid
     *            group id
     * @param property
     *            property to update
     * @param values
     *            value list to update
     * @return property value list to be deleted from the group
     */
    public <T> ArrayList<T> getDeletePropertyValues(String gid, String property,
            ArrayList<T> values) {
        GroupTable groupTable = getGroupTable(gid);
        if (groupTable == null) {
            throw new BadRequestException("group " + gid + " does not exist");
        }
        ArrayList<T> propertyValues = groupTable.getPropertyValue(property);
        ArrayList<T> deletedValues = new ArrayList<>();
        for (int i = 0; i < propertyValues.size(); i++) {
            if (!values.contains(propertyValues.get(i))) {
                deletedValues.add(propertyValues.get(i));
            }
        }
        return deletedValues;
    }

    /**
     * API to verify if the user is eligible to get the group information
     * 
     * @param gid
     *            group id
     * @param mid
     *            user uuid
     */
    public void verifyGetRequestAuthz(String gid, String mid) {
        verifyMemberExistenceInGroup(gid, mid);
    }

    /**
     * API to verify if the user is eligible to delete the group
     * 
     * @param gid
     *            group id
     * @param mid
     *            user uuid
     */
    public void verifyDeleteRequestAuthz(String gid, String mid) {
        ArrayList<String> property = new ArrayList<>();
        property.add(Constants.KEYFIELD_GROUP);
        mGroupPolicyManager.verifyOperationAuthorization(gid, mid,
                UserOperation.DELETE, property);
    }

    /**
     * API to delete a group
     * 
     * @param gmid
     *            An unique identifier of member who must be a group master.
     *            Group master can be user or resource client.
     * @param gid
     *            An unique identifier of the group created under user entity
     *            who requested for group creation.
     */
    public void deleteGroup(String gid) {
        GroupTable groupTable = getGroupTable(gid);
        String parentGid = groupTable.getParent();
        // delete subgroup ID of the parent group
        if (parentGid != null && !parentGid.isEmpty()) {
            ArrayList<Object> gidList = new ArrayList<Object>(
                    Arrays.asList(gid));
            deleteProperties(parentGid, Constants.KEYFIELD_GROUP_SUBGROUPS,
                    gidList);
        }
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.KEYFIELD_GID, gid);
        // delete group from the table
        AccountDBManager.getInstance().deleteRecord(Constants.GROUP_TABLE,
                condition);
        ArrayList<String> subgroups = (ArrayList<String>) groupTable
                .getSubgroups();
        // delete subgroups
        if (subgroups != null) {
            for (String subgroup : subgroups) {
                deleteGroup(subgroup);
                mGroupAclManager.removeAceByGroup(subgroup);
            }
        }
    }

    /**
     * API to get the group information from the db
     * 
     * @param gid
     *            group id
     * @return group information payload
     */
    public HashMap<String, Object> getGroupInfo(String gid) {
        GroupTable grouptable = getGroupTable(gid);
        return mTypeGroup.convertObjectToMap(grouptable);
    }

    /**
     * API to add property value list to the group
     * 
     * @param gid
     *            group id
     * @param property
     *            property
     * @param values
     *            value list
     */
    private <T> void addProperties(String gid, String property,
            ArrayList<T> values) {
        Log.d("added property name: " + property + ", values : " + values
                + " , to group : " + gid);
        if (values == null || values.isEmpty()) {
            return;
        }
        GroupTable groupTable = getGroupTable(gid);
        ArrayList<T> propertyValues = groupTable.getPropertyValue(property);
        if (propertyValues == null) {
            propertyValues = new ArrayList<T>();
        }
        values.removeAll(propertyValues);
        propertyValues.addAll(values);
        groupTable.setPropertyValue(property, propertyValues);
        AccountDBManager.getInstance().updateRecord(Constants.GROUP_TABLE,
                mTypeGroup.convertObjectToMap(groupTable));
        updateAclist(property, values, UserOperation.ADD, groupTable);
    }

    /**
     * API to delete property value list from the group
     * 
     * @param gid
     *            group id
     * @param property
     *            property
     * @param values
     *            value list
     */
    private <T> void deleteProperties(String gid, String property,
            ArrayList<T> values) {
        Log.d("deleted property name: " + property + ", values : " + values
                + " , from group : " + gid);
        GroupTable groupTable = getGroupTable(gid);
        if (groupTable == null || values == null || values.isEmpty()) {
            return;
        }
        ArrayList<T> propertyValues = groupTable.getPropertyValue(property);
        if (propertyValues != null) {
            if (propertyValues.removeAll(values) == false) {
                return;
            }
        }
        groupTable.setPropertyValue(property, propertyValues);
        AccountDBManager.getInstance().updateRecord(Constants.GROUP_TABLE,
                mTypeGroup.convertObjectToMap(groupTable));
        ArrayList<String> subgroups = (ArrayList<String>) groupTable
                .getSubgroups();
        updateAclist(property, values, UserOperation.DELETE, groupTable);
        if (subgroups != null) {
            for (int i = 0; i < subgroups.size(); i++) {
                deleteProperties(subgroups.get(i), property, values);
            }
        }
    }

    /**
     * API to replace property value list to the group
     * 
     * @param gid
     *            group id
     * @param property
     *            property
     * @param value
     *            value string
     */
    private void replaceProperties(String gid, String property, String value) {
        Log.d("replaced property name: " + property + ", value : " + value
                + ", to group : " + gid);
        if (value == null || value.isEmpty()) {
            return;
        }
        GroupTable groupTable = getGroupTable(gid);
        groupTable.setPropertyValue(property, value);
        AccountDBManager.getInstance().updateRecord(Constants.GROUP_TABLE,
                mTypeGroup.convertObjectToMap(groupTable));
    }

    /**
     * API to get group table as an instance of GroupTable class
     * 
     * @param gid
     *            group id
     * @return group table
     */
    public GroupTable getGroupTable(String gid) {
        GroupTable getGroupTable = new GroupTable();
        ArrayList<HashMap<String, Object>> groupList = AccountDBManager
                .getInstance().selectRecord(Constants.GROUP_TABLE,
                        getCondition(Constants.REQ_GROUP_ID, gid));
        if (groupList.isEmpty()) {
            return null;
        }
        getGroupTable = mTypeGroup.convertMaptoObject(groupList.get(0),
                getGroupTable);
        return getGroupTable;

    }

    private void deleteResourcesOfDevices(String gid,
            ArrayList<String> devices) {
        GroupTable groupTable = getGroupTable(gid);
        ArrayList<Object> resources = groupTable.getResources();
        if (resources == null) {
            return;
        }
        ArrayList<Object> deletedResources = new ArrayList<>();
        for (Object object : resources) {
            HashMap<String, Object> resource = (HashMap<String, Object>) object;
            String resourceHref = (String) resource
                    .get(Constants.KEYFIELD_ACE_RESOURCE_HREF);
            String splitHref[] = resourceHref.split("/");

            String deviceId = new String();
            for (int i = 0; i < splitHref.length; i++) {
                if (splitHref[i].equals(Constants.REQ_DEVICE_ID)
                        && (i + 1) < splitHref.length) {
                    deviceId = splitHref[i + 1];

                    break;
                }
            }
            if (devices.contains(deviceId)) {
                deletedResources.add(resource);
            }
        }
        deleteResourcesFromGroup(gid, deletedResources);
    }

    private void verifyPostReplacePolicy(String gid, String mid,
            HashMap<String, Object> properties) {
        ArrayList<String> updatedKeySet = new ArrayList<String>();
        updatedKeySet.addAll(properties.keySet());
        mGroupPolicyManager.verifyOperationAuthorization(gid, mid,
                UserOperation.REPLACE, updatedKeySet);
        for (String key : properties.keySet()) {
            if (!(properties.get(key) instanceof String)) {
                throw new BadRequestException(
                        "replace property value should be an instance of String");
            }
        }
    }

    private void verifyMemberExistenceInGroup(String gid, String mid) {
        GroupTable groupTable = getGroupTable(gid);
        if (groupTable == null) {
            throw new BadRequestException("group " + gid + " does not exist");
        }
        if (groupTable.getMembers() == null) {
            throw new BadRequestException("there are no members in the group");
        }
        if (!groupTable.getMembers().contains(mid)) {
            throw new BadRequestException("uid is not a member of the group");
        }
    }

    private void verifyPostAddPolicy(String gid, String mid,
            HashMap<String, Object> properties) {
        for (String key : properties.keySet()) {
            if (!(properties.get(key) instanceof List)) {
                throw new BadRequestException(
                        "add property value should be an instance of Array");
            }
            switch (key) {
                case Constants.KEYFIELD_GROUP_DEVICES:
                    verifyDeviceOwner(mid,
                            (ArrayList<String>) properties.get(key));
                    verifyExistenceInParentGroup(gid, key,
                            (ArrayList<Object>) properties.get(key));
                    break;
                case Constants.KEYFIELD_GROUP_RESOURCES:
                    verifyResourceFormat(
                            Arrays.asList(Constants.KEYFIELD_RESOURCE_RT,
                                    Constants.KEYFIELD_RESOURCE_IF,
                                    Constants.KEYFIELD_ACE_RESOURCE_HREF),
                            (ArrayList<HashMap<String, Object>>) properties
                                    .get(key));
                    verifyResourceOwner(mid,
                            (ArrayList<HashMap<String, Object>>) properties
                                    .get(key));
                    verifyExistenceInParentGroup(gid, key,
                            filterResourceExistenceInParentGroupDeviceProperty(
                                    gid,
                                    (ArrayList<HashMap<String, Object>>) properties
                                            .get(key)));
                    break;
                case Constants.KEYFIELD_GROUP_MEMBERS:
                case Constants.KEYFIELD_GROUP_MASTERS:
                    // TODO verify if members are registered to the Account user
                    // DB
                    verifyExistenceInParentGroup(gid,
                            Constants.KEYFIELD_GROUP_MEMBERS,
                            (ArrayList<Object>) properties.get(key));
                    break;
                default:
                    throw new PreconditionFailedException(
                            key + " is not supported");
            }
        }
    }

    private void verifyResourceFormat(List<String> propertyList,
            ArrayList<HashMap<String, Object>> resources) {
        for (HashMap<String, Object> resource : resources) {
            for (String property : propertyList) {
                if (!resource.containsKey(property))
                    throw new PreconditionFailedException(
                            property + " property is not included");
                switch (property) {
                    case Constants.KEYFIELD_RESOURCE_RT:
                    case Constants.KEYFIELD_RESOURCE_IF:
                        if (!(resource.get(property) instanceof List)) {
                            throw new BadRequestException(property
                                    + " property values should be an instance of array");
                        }
                        break;
                    case Constants.KEYFIELD_ACE_RESOURCE_HREF:
                        if (resource.get(property) == null) {
                            throw new BadRequestException(
                                    property + " property is null");
                        }
                        break;
                }
            }
        }
    }

    private void verifyPostDeletePolicy(String gid, String mid,
            HashMap<String, Object> properties) {
        for (String key : properties.keySet()) {
            if (!(properties.get(key) instanceof List)) {
                throw new BadRequestException(
                        "delete property value should be an instance of Array");
            }
            switch (key) {
                case Constants.REQ_UUID_ID:
                    break;
                case Constants.KEYFIELD_GROUP_DEVICES:
                case Constants.KEYFIELD_GROUP_RESOURCES:
                case Constants.KEYFIELD_GROUP_MEMBERS:
                    if ((boolean) ((ArrayList<String>) properties.get(key))
                            .contains(getGroupTable(gid).getOwner())) {
                        throw new BadRequestException("cannot remove owner Id");
                    }
                case Constants.KEYFIELD_GROUP_MASTERS:
                    verifyExistenceInParentGroup(gid, key,
                            (ArrayList<Object>) properties.get(key));
                    break;
                default:
                    throw new BadRequestException(
                            key + " property is not supported to ");
            }
        }
    }

    private ArrayList<HashMap<String, Object>> filterResourceExistenceInParentGroupDeviceProperty(
            String gid, ArrayList<HashMap<String, Object>> resources) {
        GroupTable parentGroupTable = getParentGroupTable(gid);
        if (parentGroupTable == null) {
            return resources;
        }
        ArrayList<String> devices = parentGroupTable.getDevices();
        if (devices == null) {
            return resources;
        }
        for (HashMap<String, Object> resource : resources) {
            // if the device is registered to the parent group, filter the
            // resource list
            String deviceId = getDeviceIdFromResource(resource);
            if (devices.contains(deviceId)) {
                resources.remove(resource);
            }
        }
        return resources;
    }

    private GroupTable getParentGroupTable(String gid) {
        try {
            return getGroupTable(getGroupTable(gid).getParent());
        } catch (Exception e) {
            return null;
        }
    }

    private <T> void verifyExistenceInParentGroup(String gid, String property,
            ArrayList<T> values) {
        GroupTable parentGroupTable = getParentGroupTable(gid);
        if (parentGroupTable == null) {
            return;
        }

        ArrayList<Object> groupValues = parentGroupTable
                .getPropertyValue(property);
        if (groupValues == null) {
            throw new BadRequestException(
                    "verifying parent group Existence failed");
        }
        if (!groupValues.containsAll(values)) {
            throw new BadRequestException(
                    "verifying parent group Existence failed");

        }
    }

    private void verifyDeviceOwner(String mid, ArrayList<String> values) {
        for (String deviceId : values) {
            if (!findDeviceOwner(deviceId).equals(mid)) {
                throw new BadRequestException("verifying device owner failed");
            }
        }
    }

    private void verifyResourceOwner(String mid,
            ArrayList<HashMap<String, Object>> resources) {
        for (HashMap<String, Object> resource : resources) {
            String deviceId = getDeviceIdFromResource(resource);
            if (!findDeviceOwner(deviceId).equals(mid)) {
                throw new BadRequestException(
                        "verifying resource owner failed");
            }
        }
    }

    private String getDeviceIdFromResource(HashMap<String, Object> resource) {
        String resourceHref = (String) resource
                .get(Constants.KEYFIELD_ACE_RESOURCE_HREF);
        String splitHref[] = resourceHref.split("/");
        for (int i = 0; i < splitHref.length; i++) {
            if (splitHref[i].equals(Constants.REQ_DEVICE_ID)
                    && (i + 1) < splitHref.length) {
                return splitHref[i + 1];
            }
        }
        return null;
    }

    private String findDeviceOwner(String deviceId) {
        return mGroupAclManager.getDeviceOwnerId(deviceId);
    }

    private String findResourceOwner(String resourceHref) {
        String splitHref[] = resourceHref.split("/");
        for (int i = 0; i < splitHref.length; i++) {
            if (splitHref[i].equals(Constants.REQ_DEVICE_ID)
                    && (i + 1) < splitHref.length) {
                return findDeviceOwner(splitHref[i + 1]);
            }
        }
        return null;
    }

    private HashMap<String, Object> getCondition(String property,
            String value) {
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(property, value);
        return condition;
    }

    private <T> void updateAclist(String property, ArrayList<T> values,
            UserOperation operation, GroupTable groupTable) {
        switch (operation) {
            case ADD:
                addAclist(property, values, groupTable);
                break;
            case DELETE:
                removeAclist(property, values, groupTable);
                break;
            default:
                throw new BadRequestException(
                        operation + " is not supported operation in the group");
        }
    }

    private <T> void addAclist(String property, ArrayList<T> values,
            GroupTable groupTable) {
        String gid = groupTable.getGid();
        int permission = 0;
        for (Object gaclObject : groupTable.getGacl()) {
            HashMap<String, Object> gacl = (HashMap<String, Object>) gaclObject;
            permission = (int) gacl.get(Constants.KEYFIELD_ACE_PERMISSION);
        }
        switch (property) {
            case Constants.KEYFIELD_GROUP_MEMBERS:
                mGroupAclManager.addAceByMembers(gid, permission,
                        (ArrayList<String>) values);
                break;
            case Constants.KEYFIELD_GROUP_DEVICES:
                mGroupAclManager.addAceByDevices(gid, permission,
                        (ArrayList<String>) values);
                break;
            case Constants.KEYFIELD_GROUP_RESOURCES:
                mGroupAclManager.addAceByResources(gid, permission,
                        (ArrayList<HashMap<String, Object>>) values);
                break;
            case Constants.KEYFIELD_GROUP_OWNER:
            case Constants.KEYFIELD_GROUP_MASTERS:
            case Constants.KEYFIELD_GROUP_SUBGROUPS:
            case Constants.KEYFIELD_GROUP_GACL:
                return;
            default:
                throw new BadRequestException(
                        property + " is not supported property in the group");
        }
    }

    private <T> void removeAclist(String property, ArrayList<T> values,
            GroupTable groupTable) {
        String gid = groupTable.getGid();
        switch (property) {
            case Constants.KEYFIELD_GROUP_MEMBERS:
                mGroupAclManager.removeAceByMembers((ArrayList<String>) values,
                        gid);
                break;
            case Constants.KEYFIELD_GROUP_DEVICES:
                mGroupAclManager.removeAceByDevices((ArrayList<String>) values,
                        gid);
                break;
            case Constants.KEYFIELD_GROUP_RESOURCES:
                mGroupAclManager.removeAceByResources(
                        (ArrayList<HashMap<String, Object>>) values, gid);
                break;
            case Constants.KEYFIELD_GROUP_OWNER:
            case Constants.KEYFIELD_GROUP_MASTERS:
            case Constants.KEYFIELD_GROUP_SUBGROUPS:
            case Constants.KEYFIELD_GROUP_GACL:
                return;
            default:
                throw new BadRequestException(
                        property + " is not supported property in the group");
        }
    }
}
