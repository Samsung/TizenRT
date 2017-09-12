package org.iotivity.cloud.accountserver.resources.acl.group;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.ListIterator;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.db.AceTable;
import org.iotivity.cloud.accountserver.db.GroupTable;
import org.iotivity.cloud.accountserver.resources.acl.id.AceResource;
import org.iotivity.cloud.accountserver.resources.acl.id.AclResource;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;
import org.iotivity.cloud.base.exception.ServerException.InternalServerErrorException;

/**
 *
 * This class provides a set of APIs to manage between AclManager and
 * GroupManager
 * 
 *
 */

public class GroupAclManager {
    private static GroupAclManager          mGrAclManager    = new GroupAclManager();
    private TypeCastingManager<AceTable>    mTypeAceTable    = new TypeCastingManager<AceTable>();
    private TypeCastingManager<AceResource> mTypeAceResource = new TypeCastingManager<AceResource>();

    private GroupAclManager() {
    }

    public static GroupAclManager getInstance() {
        return mGrAclManager;
    }

    /**
     * API to add acl table by members
     * 
     * @param gid
     *            group id
     * @param permission
     *            resource permission of group acl in group
     * @param members
     *            member list which add into group
     */
    public void addAceByMembers(String gid, int permission,
            ArrayList<String> members) {

        // make default acelist for each new member
        for (String device : getGroupDevices(gid)) {
            addAceList(gid, device, makeAceList(members, permission, null));
        }

        // make acelist with specific resources for each new member
        addAceByResources(gid, permission, getGroupResources(gid), members);
    }

    /**
     * API to add acl table by devices
     * 
     * @param gid
     *            group id
     * @param permission
     *            resource permission of group acl in group
     * @param devices
     *            device list which add into group
     */
    public void addAceByDevices(String gid, int permission,
            ArrayList<String> devices) {

        // make default acelist for each existing member
        for (String device : devices) {
            addAceList(gid, device,
                    makeAceList(getGroupMembers(gid), permission, null));
        }
    }

    /**
     * API to add acl table by resources
     * 
     * @param gid
     *            group id
     * @param permission
     *            resource permission of group acl in group
     * @param resources
     *            resource list which add into group
     */
    public void addAceByResources(String gid, int permission,
            ArrayList<HashMap<String, Object>> resources) {

        addAceByResources(gid, permission, resources, getGroupMembers(gid));
    }

    /**
     * API to remove acl table by group
     * 
     * @param gid
     *            group id
     */
    public void removeAceByGroup(String gid) {

        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.KEYFIELD_GID, gid);

        ArrayList<AceTable> getAceTable = searchAceTableList(condition);
        if (getAceTable.isEmpty()) {
            return;
        }

        removeAceList(getAceTable);
        deleteAceRecords(condition);
    }

    /**
     * API to remove acl table by members
     * 
     * @param members
     *            member list which remove in group
     * @param gid
     *            group id
     */
    public void removeAceByMembers(ArrayList<String> members, String gid) {
        for (String member : members) {
            removeAceByMember(member, gid);
            removeAceByOwner(member, gid);
        }
    }

    /**
     * API to remove acl table by devices
     * 
     * @param devices
     *            device list which remove in group
     * @param gid
     *            group id
     */
    public void removeAceByDevices(ArrayList<String> devices, String gid) {
        for (String device : devices) {
            removeAceByDevice(device, gid);
        }
    }

    /**
     * API to remove acl table by resources
     * 
     * @param resources
     *            resource list which remove in group
     * @param gid
     *            group id
     */
    public void removeAceByResources(
            ArrayList<HashMap<String, Object>> resources, String gid) {
        for (HashMap<String, Object> href : resources) {
            removeAceByResource(
                    href.get(Constants.KEYFIELD_ACE_RESOURCE_HREF).toString(),
                    gid);
        }
    }

    /**
     * API to get device owner id
     * 
     * @param resources
     *            resource list which remove in group
     * @param gid
     *            group id
     */
    public String getDeviceOwnerId(String di) {

        String aclid = getAclid(di);

        HashMap<String, Object> acl = AclResource.getInstance().getAcl(aclid)
                .getInfo();

        if (acl == null || acl.containsKey(Constants.REQ_OWNER_ID) == false) {
            throw new BadRequestException(
                    "Error while getting device owner id for " + di);
        }

        return acl.get(Constants.REQ_OWNER_ID).toString();
    }

    private void removeAceByMember(String uid, String gid) {
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.KEYFIELD_UID, uid);
        condition.put(Constants.KEYFIELD_GID, gid);

        ArrayList<AceTable> getAceTable = searchAceTableList(condition);
        if (getAceTable.isEmpty()) {
            return;
        }

        removeAceList(getAceTable);
        deleteAceRecords(condition);
    }

    private void removeAceByOwner(String oid, String gid) {
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.KEYFIELD_OID, oid);
        condition.put(Constants.KEYFIELD_GID, gid);

        ArrayList<AceTable> getAceTable = searchAceTableList(condition);
        if (getAceTable.isEmpty()) {
            return;
        }

        removeAceList(getAceTable);
        deleteAceRecords(condition);
    }

    private void removeAceByDevice(String di, String gid) {
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.KEYFIELD_DI, di);
        condition.put(Constants.KEYFIELD_GID, gid);

        ArrayList<AceTable> getAceTable = searchAceTableList(condition);
        if (getAceTable.isEmpty()) {
            return;
        }

        removeAceList(getAceTable);
        deleteAceRecords(condition);
    }

    private void removeAceByResource(String href, String gid) {
        String removeHref = extractHref(href);
        String di = Arrays.asList(href.split("/")).get(2);
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.KEYFIELD_DI, di);
        condition.put(Constants.KEYFIELD_GID, gid);

        ArrayList<AceTable> aceTableList = searchAceTableList(condition);
        ArrayList<HashMap<String, Object>> aceMapList = searchAceMapList(
                aceTableList);

        for (HashMap<String, Object> aceMap : aceMapList) {
            ArrayList<HashMap<String, Object>> AceResourceList = (ArrayList) aceMap
                    .get(Constants.KEYFIELD_ACE_RESOURCE);
            ListIterator<HashMap<String, Object>> iterator = AceResourceList
                    .listIterator();
            while (iterator.hasNext()) {
                AceResource res = new AceResource();
                res = mTypeAceResource.convertMaptoObject(iterator.next(), res);

                if (res.getHref().contains(removeHref)) {
                    iterator.remove();
                    break;
                }
            }

            condition.put(Constants.KEYFIELD_UID,
                    aceMap.get(Constants.KEYFIELD_ACE_SUBJECT_ID));

            ArrayList<AceTable> getAceTable = searchAceTableList(condition);
            if (getAceTable.isEmpty()) {
                return;
            }

            if (AceResourceList.isEmpty()) {
                removeAceList(getAceTable);
                deleteAceRecords(condition);
            } else {
                updateAce(getAceTable.get(0), aceMap);
            }

        }
    }

    private ArrayList<AceTable> searchAceTableList(
            HashMap<String, Object> condition) {
        ArrayList<AceTable> getAceTableList = new ArrayList<>();
        ArrayList<HashMap<String, Object>> getAceList = AccountDBManager
                .getInstance().selectRecord(Constants.ACE_TABLE, condition);
        for (HashMap<String, Object> getAce : getAceList) {
            AceTable getAceTable = new AceTable();
            getAceTable = mTypeAceTable.convertMaptoObject(getAce, getAceTable);
            getAceTableList.add(getAceTable);
        }
        return getAceTableList;
    }

    private ArrayList<HashMap<String, Object>> searchAceMapList(
            ArrayList<AceTable> aceList) {
        ArrayList<HashMap<String, Object>> getAceList = new ArrayList<>();
        for (AceTable getAce : aceList) {
            getAceList.add(getAce(getAce));
        }
        return getAceList;
    }

    private void removeAceList(ArrayList<AceTable> aceTableList) {
        for (AceTable removeAce : aceTableList) {
            removeAce(removeAce);
        }
    }

    private void updateAce(AceTable updateAce, HashMap<String, Object> ace) {
        AclResource.getInstance().updateACE(getAclid(updateAce),
                getAceid(updateAce), ace);
    }

    private void removeAce(AceTable removeAce) {
        AclResource.getInstance().deleteAclACE(getAclid(removeAce),
                getAceid(removeAce));
    }

    private HashMap<String, Object> getAce(AceTable getAce) {
        return AclResource.getInstance().getAclACE(getAclid(getAce),
                getAceid(getAce));
    }

    private String extractHref(String href) {

        List<String> segment = new ArrayList<String>(
                Arrays.asList(href.split("/")));

        // Remove prefix path
        segment.remove(0);
        segment.remove(0);
        segment.remove(0);

        StringBuilder uriPath = new StringBuilder();
        for (String path : segment) {
            uriPath.append("/" + path);
        }

        return uriPath.toString();
    }

    private String getAceid(AceTable aceTable) {
        if (aceTable.getAceid() == null) {
            throw new InternalServerErrorException("aceid is invalid");
        }
        return aceTable.getAceid();
    }

    private String getAclid(AceTable aceTable) {
        String di = null;
        if (aceTable.getDi() == null) {
            throw new InternalServerErrorException("di is invalid");
        }
        di = aceTable.getDi();

        return getAclid(di);
    }

    private String getAclid(String di) {

        HashMap<String, Object> acl = AclResource.getInstance().getAclid(di);

        if (acl == null || !acl.containsKey(Constants.KEYFIELD_ACLID)) {
            throw new InternalServerErrorException("aclid is invalid");
        }

        return (String) acl.get(Constants.KEYFIELD_ACLID);
    }

    private void addAceList(String gid, String di,
            ArrayList<HashMap<String, Object>> acelist) {

        String aclid = getAclid(di);
        String oid = getDeviceOwnerId(di);

        acelist = (ArrayList<HashMap<String, Object>>) AclResource.getInstance()
                .addAclACE(aclid, acelist);

        insertAceRecords(gid, di, oid, acelist);
    }

    private void addAceByResources(String gid, int permission,
            ArrayList<HashMap<String, Object>> resources,
            ArrayList<String> members) {

        HashMap<String, ArrayList<HashMap<String, Object>>> sortedResources = sortResources(
                resources);

        for (String di : sortedResources.keySet()) {

            // query aceid using gid-di in GroupACE table
            HashMap<String, Object> condition = new HashMap<>();
            condition.put(Constants.KEYFIELD_GID, gid);
            condition.put(Constants.KEYFIELD_DI, di);
            ArrayList<AceTable> aceTables = searchAceTableList(condition);

            if (aceTables == null || aceTables.isEmpty()) {
                // add new ace
                addAceList(gid, di, makeAceList(members, permission,
                        sortedResources.get(di)));
                continue;
            } else {
                // update ace
                updateAceByResources(gid, permission, di, sortedResources,
                        aceTables);
            }
        }
    }

    private void updateAceByResources(String gid, int permission, String di,
            HashMap<String, ArrayList<HashMap<String, Object>>> sortedResources,
            ArrayList<AceTable> aceTables) {

        String aclid = getAclid(aceTables.get(0));
        String aceid = getAceid(aceTables.get(0));

        @SuppressWarnings("unchecked")
        ArrayList<HashMap<String, Object>> updatedAceResources = (ArrayList<HashMap<String, Object>>) AclResource
                .getInstance().getAclACE(aclid, aceid)
                .get(Constants.KEYFIELD_ACE_RESOURCE);

        if (updatedAceResources.isEmpty() == false && updatedAceResources.get(0)
                .get(Constants.KEYFIELD_ACE_RESOURCE_HREF).equals("*")) {

            updatedAceResources.clear();
        }

        updatedAceResources.addAll(sortedResources.get(di));

        for (AceTable acetable : aceTables) {
            updateAce(acetable, makeAce(acetable.getUid(), permission,
                    updatedAceResources));
        }
    }

    private ArrayList<HashMap<String, Object>> makeAceList(
            ArrayList<String> members, int permission,
            ArrayList<HashMap<String, Object>> resources) {

        ArrayList<HashMap<String, Object>> acelist = new ArrayList<>();

        for (String member : members) {
            acelist.add(makeAce(member, permission, resources));
        }

        return acelist;
    }

    private HashMap<String, Object> makeAce(String mid, int permission,
            ArrayList<HashMap<String, Object>> resources) {

        if (resources == null) {
            resources = new ArrayList<>();
            HashMap<String, Object> resource = new HashMap<>();
            resource.put(Constants.KEYFIELD_ACE_RESOURCE_HREF, "*");
            resource.put(Constants.KEYFIELD_ACE_RESOURCE_RT, Arrays.asList(""));
            resource.put(Constants.KEYFIELD_ACE_RESOURCE_IF, Arrays.asList(""));

            resources.add(resource);
        }

        HashMap<String, Object> newAce = new HashMap<>();
        newAce.put(Constants.KEYFIELD_ACE_SUBJECT_ID, mid);
        newAce.put(Constants.KEYFIELD_ACE_SUBJECT_TYPE, 1);
        newAce.put(Constants.KEYFIELD_ACE_PERMISSION, permission);
        newAce.put(Constants.KEYFIELD_ACE_RESOURCE, resources);

        return newAce;
    }

    private void insertAceRecords(String gid, String di, String oid,
            ArrayList<HashMap<String, Object>> acelist) {

        for (HashMap<String, Object> ace : acelist) {

            AceTable aceTable = new AceTable();
            aceTable.setAceid(ace.get(Constants.KEYFIELD_ACE_ID).toString());
            aceTable.setUid(
                    ace.get(Constants.KEYFIELD_ACE_SUBJECT_ID).toString());
            aceTable.setGid(gid);
            aceTable.setDi(di);
            aceTable.setOid(oid);

            AccountDBManager.getInstance().insertAndReplaceRecord(
                    Constants.ACE_TABLE,
                    mTypeAceTable.convertObjectToMap(aceTable));
        }
    }

    private void deleteAceRecords(HashMap<String, Object> condition) {

        AccountDBManager.getInstance().deleteRecord(Constants.ACE_TABLE,
                condition);
    }

    private ArrayList<String> getGroupMembers(String gid) {

        GroupTable groupTable = GroupManager.getInstance().getGroupTable(gid);
        ArrayList<String> members = new ArrayList<>();

        if (groupTable.getMembers() != null) {
            members = groupTable.getMembers();
        }
        return members;
    }

    private ArrayList<String> getGroupDevices(String gid) {

        GroupTable groupTable = GroupManager.getInstance().getGroupTable(gid);
        ArrayList<String> devices = new ArrayList<>();

        if (groupTable.getDevices() != null) {
            devices = groupTable.getDevices();
        }
        return devices;
    }

    private ArrayList<HashMap<String, Object>> getGroupResources(String gid) {

        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        GroupTable groupTable = GroupManager.getInstance().getGroupTable(gid);

        if (groupTable.getResources() == null) {
            return resources;
        }

        for (Object resource : groupTable.getResources()) {

            @SuppressWarnings("unchecked")
            HashMap<String, Object> resourceInfo = (HashMap<String, Object>) resource;
            resources.add(resourceInfo);
        }

        return resources;
    }

    // classify resources in group according to device id
    private HashMap<String, ArrayList<HashMap<String, Object>>> sortResources(
            ArrayList<HashMap<String, Object>> resources) {

        HashMap<String, ArrayList<HashMap<String, Object>>> sortedResources = new HashMap<>();

        for (HashMap<String, Object> resource : resources) {

            HashMap<String, Object> resourceInfo = new HashMap<>(resource);

            String href = resourceInfo.get(Constants.KEYFIELD_ACE_RESOURCE_HREF)
                    .toString();
            String di = Arrays.asList(href.split("/")).get(2);
            href = extractHref(href);

            resourceInfo.put(Constants.KEYFIELD_ACE_RESOURCE_HREF, href);

            if (sortedResources.containsKey(di) == false) {
                sortedResources.put(di, new ArrayList<>());
            }

            sortedResources.get(di).add(resourceInfo);
        }

        return sortedResources;
    }
}
