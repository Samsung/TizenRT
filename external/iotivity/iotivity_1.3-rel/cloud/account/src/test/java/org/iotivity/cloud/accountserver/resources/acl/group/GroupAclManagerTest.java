package org.iotivity.cloud.accountserver.resources.acl.group;

import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.db.GroupTable;
import org.iotivity.cloud.accountserver.db.MongoDB;
import org.iotivity.cloud.accountserver.resources.acl.id.AclResource;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;
import org.iotivity.cloud.util.Log;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class GroupAclManagerTest {
    private String                  mGid1               = "g1";
    private String                  mGid2               = "g2";
    private String                  mDi1                = "d1";
    private String                  mDi2                = "d2";
    private String                  mUid1               = "u1";
    private String                  mUid2               = "u2";
    private String                  mUid3               = "u3";

    private HashMap<String, Object> mResourceLight1ofD1 = new HashMap<>();
    private HashMap<String, Object> mResourceLight2ofD1 = new HashMap<>();
    private HashMap<String, Object> mResourceLight1ofD2 = new HashMap<>();
    private HashMap<String, Object> mResourceLight2ofD2 = new HashMap<>();
    private HashMap<String, Object> mResourceAll        = new HashMap<>();
    private int                     mPermission         = 15;

    public GroupAclManager          mGroupAclManager    = GroupAclManager
            .getInstance();

    @Before
    public void setUp() throws Exception {
        AclResource.getInstance().createAcl(mUid1, mDi1);
        AclResource.getInstance().createAcl(mUid2, mDi2);

        mResourceLight1ofD1.put(Constants.KEYFIELD_ACE_RESOURCE_HREF,
                "/di/d1/a/light/1");
        mResourceLight1ofD1.put(Constants.KEYFIELD_ACE_RESOURCE_RT,
                Arrays.asList("core.light"));
        mResourceLight1ofD1.put(Constants.KEYFIELD_ACE_RESOURCE_IF,
                Arrays.asList("oic.if.baseline"));

        mResourceLight2ofD1.put(Constants.KEYFIELD_ACE_RESOURCE_HREF,
                "/di/d1/a/light/2");
        mResourceLight2ofD1.put(Constants.KEYFIELD_ACE_RESOURCE_RT,
                Arrays.asList("core.light"));
        mResourceLight2ofD1.put(Constants.KEYFIELD_ACE_RESOURCE_IF,
                Arrays.asList("oic.if.baseline"));

        mResourceLight1ofD2.put(Constants.KEYFIELD_ACE_RESOURCE_HREF,
                "/di/d2/a/light/1");
        mResourceLight1ofD2.put(Constants.KEYFIELD_ACE_RESOURCE_RT,
                Arrays.asList("core.light"));
        mResourceLight1ofD2.put(Constants.KEYFIELD_ACE_RESOURCE_IF,
                Arrays.asList("oic.if.baseline"));

        mResourceLight2ofD2.put(Constants.KEYFIELD_ACE_RESOURCE_HREF,
                "/di/d2/a/light/2");
        mResourceLight2ofD2.put(Constants.KEYFIELD_ACE_RESOURCE_RT,
                Arrays.asList("core.light"));
        mResourceLight2ofD2.put(Constants.KEYFIELD_ACE_RESOURCE_IF,
                Arrays.asList("oic.if.baseline"));

        mResourceAll.put(Constants.KEYFIELD_ACE_RESOURCE_HREF, "*");
        mResourceAll.put(Constants.KEYFIELD_ACE_RESOURCE_RT, Arrays.asList(""));
        mResourceAll.put(Constants.KEYFIELD_ACE_RESOURCE_IF, Arrays.asList(""));
    }

    @After
    public void resetAccountDatabase() throws Exception {
        AclResource.getInstance().deleteAcl((String) AclResource.getInstance()
                .getAclid(mDi1).get(Constants.KEYFIELD_ACLID));
        AclResource.getInstance().deleteAcl((String) AclResource.getInstance()
                .getAclid(mDi2).get(Constants.KEYFIELD_ACLID));

        MongoDB mongoDB = new MongoDB("127.0.0.1", Constants.DB_NAME);
        mongoDB.deleteTable(Constants.GROUP_TABLE);
        mongoDB.createTable(Constants.GROUP_TABLE);
        mongoDB.deleteTable(Constants.ACL_TABLE);
        mongoDB.createTable(Constants.ACL_TABLE);
        mongoDB.deleteTable(Constants.ACE_TABLE);
        mongoDB.createTable(Constants.ACE_TABLE);
    }

    @Test
    public void testAddAceByResourcesOnlyOwner() {
        Log.v("--------------testAddAceByResourcesOnlyOwner------------");

        // initialize group info --
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid1);

        HashMap<String, Object> groupInfo = new HashMap<>();
        groupInfo.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        setGroupInfo(mGid1, groupInfo);

        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        resources.add(mResourceLight1ofD1);
        resources.add(mResourceLight2ofD1);

        mGroupAclManager.addAceByResources(mGid1, mPermission, resources);

        // check ACE not updated for d1-u1(device owner)
        HashMap<String, Object> aceidCondition = new HashMap<>();
        aceidCondition.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition.put(Constants.KEYFIELD_DI, mDi1);
        aceidCondition.put(Constants.KEYFIELD_UID, mUid1);
        assertNull(getAceId(aceidCondition));
    }

    @Test
    public void testAddAceByResources() {
        Log.v("--------------testAddAceByResoruces------------");

        // initialize group info --
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid1);
        members.add(mUid2);

        HashMap<String, Object> groupInfo = new HashMap<>();
        groupInfo.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        setGroupInfo(mGid1, groupInfo);
        // -- initialize group info

        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        resources.add(mResourceLight1ofD1);
        resources.add(mResourceLight2ofD1);

        mGroupAclManager.addAceByResources(mGid1, mPermission, resources);

        // check ACE not updated for d1-u1(device owner)
        HashMap<String, Object> aceidCondition1 = new HashMap<>();
        aceidCondition1.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition1.put(Constants.KEYFIELD_DI, mDi1);
        aceidCondition1.put(Constants.KEYFIELD_UID, mUid1);
        assertNull(getAceId(aceidCondition1));

        // check ACETable and ACL for d1(light1, light2)-u2
        HashMap<String, Object> aceidCondition2 = new HashMap<>();
        aceidCondition2.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition2.put(Constants.KEYFIELD_DI, mDi1);
        aceidCondition2.put(Constants.KEYFIELD_UID, mUid2);
        String aceIdg1d1 = getAceId(aceidCondition2);

        HashMap<String, Object> aceCondition2 = new HashMap<>();
        aceCondition2.put(Constants.KEYFIELD_ACE_ID, aceIdg1d1);

        aceidCondition2.clear();
        aceidCondition2.put(Constants.KEYFIELD_OID, mUid1);
        assertTrue(checkAceTable(aceCondition2, aceidCondition2));

        // remove prefix in href
        for (HashMap<String, Object> resource : resources) {
            resource.put(Constants.KEYFIELD_ACE_RESOURCE_HREF,
                    extractHref(
                            resource.get(Constants.KEYFIELD_ACE_RESOURCE_HREF)
                                    .toString()));
        }

        assertTrue(checkAclTable(mDi1, aceIdg1d1, mUid2, resources));
    }

    @Test
    public void testAddAceByResourcesNoMembers() {
        Log.v("--------------testAddAceByResorucesNoMembers------------");
        setGroupInfo(mGid1, new HashMap<>());

        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        resources.add(mResourceLight1ofD1);
        resources.add(mResourceLight2ofD1);

        mGroupAclManager.addAceByResources(mGid1, mPermission, resources);

        // check ACE not updated for d1(light1, light2)
        HashMap<String, Object> aceidCondition = new HashMap<>();
        aceidCondition.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition.put(Constants.KEYFIELD_DI, mDi1);
        assertNull(getAceId(aceidCondition));
    }

    @Test
    public void testAddAceByDevices() {
        Log.v("--------------testAddAceByDevices------------");

        // initialize group info --
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid1);
        members.add(mUid2);

        HashMap<String, Object> groupInfo = new HashMap<>();
        groupInfo.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        setGroupInfo(mGid1, groupInfo);
        // -- initialize group info

        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);

        mGroupAclManager.addAceByDevices(mGid1, mPermission, devices);

        // check ACETable and ACL for d1-u2
        HashMap<String, Object> aceidCondition = new HashMap<>();
        aceidCondition.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition.put(Constants.KEYFIELD_DI, mDi1);
        aceidCondition.put(Constants.KEYFIELD_UID, mUid2);
        String aceIdg1d1 = getAceId(aceidCondition);

        HashMap<String, Object> aceCondition = new HashMap<>();
        aceCondition.put(Constants.KEYFIELD_ACE_ID, aceIdg1d1);

        aceidCondition.clear();
        aceidCondition.put(Constants.KEYFIELD_OID, mUid1);
        assertTrue(checkAceTable(aceCondition, aceidCondition));

        assertTrue(checkAclTable(mDi1, aceIdg1d1, mUid2,
                Arrays.asList(mResourceAll)));
    }

    @Test
    public void testAddAceByDevicesOnlyOwner() {
        Log.v("--------------testAddAceByDevicesOnlyOwner------------");

        // initialize group info --
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid1);

        HashMap<String, Object> groupInfo = new HashMap<>();
        groupInfo.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        setGroupInfo(mGid1, groupInfo);
        // -- initialize group info

        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);

        mGroupAclManager.addAceByDevices(mGid1, mPermission, devices);

        // check ACE not updated for d1-u1(device owner)
        HashMap<String, Object> aceidCondition = new HashMap<>();
        aceidCondition.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition.put(Constants.KEYFIELD_DI, mDi1);
        aceidCondition.put(Constants.KEYFIELD_UID, mUid1);
        assertNull(getAceId(aceidCondition));
    }

    @Test
    public void testAddAceByDevicesNoMembers() {
        Log.v("--------------testAddAceByDevicesNoMembers------------");
        setGroupInfo(mGid1, new HashMap<>());

        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);

        mGroupAclManager.addAceByDevices(mGid1, mPermission, devices);

        // check ACE not updated for d1
        HashMap<String, Object> aceidCondition = new HashMap<>();
        aceidCondition.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition.put(Constants.KEYFIELD_DI, mDi1);
        assertNull(getAceId(aceidCondition));
    }

    @Test
    public void testAddAceByMembersWithDevice() {
        Log.v("--------------testAddAceByMembersWithDevice------------");

        // initialize group info --
        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);
        devices.add(mDi2);

        HashMap<String, Object> groupInfo = new HashMap<>();
        groupInfo.put(Constants.KEYFIELD_GROUP_DEVICES, devices);
        setGroupInfo(mGid1, groupInfo);
        // -- initialize group info

        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);

        mGroupAclManager.addAceByMembers(mGid1, mPermission, members);

        // check ACETable and ACL for d1-u2
        HashMap<String, Object> aceidCondition1 = new HashMap<>();
        aceidCondition1.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition1.put(Constants.KEYFIELD_DI, mDi1);
        aceidCondition1.put(Constants.KEYFIELD_UID, mUid2);
        String aceIdg1d1 = getAceId(aceidCondition1);

        HashMap<String, Object> aceCondition = new HashMap<>();
        aceCondition.put(Constants.KEYFIELD_ACE_ID, aceIdg1d1);

        aceidCondition1.clear();
        aceidCondition1.put(Constants.KEYFIELD_OID, mUid1);
        assertTrue(checkAceTable(aceCondition, aceidCondition1));

        assertTrue(checkAclTable(mDi1, aceIdg1d1, mUid2,
                Arrays.asList(mResourceAll)));

        // check ACE not updated for d2-u2(device owner)
        HashMap<String, Object> aceidCondition2 = new HashMap<>();
        aceidCondition2.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition2.put(Constants.KEYFIELD_DI, mDi2);
        aceidCondition2.put(Constants.KEYFIELD_UID, mUid2);
        assertNull(getAceId(aceidCondition2));
    }

    @Test
    public void testAddAceByMembersWithDeviceAndResource() {
        Log.v("--------------testAddAceByMembersWithDeviceAndResource------------");

        // initialize group info --
        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);
        devices.add(mDi2);

        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        resources.add(mResourceLight1ofD1);

        HashMap<String, Object> groupInfo = new HashMap<>();
        groupInfo.put(Constants.KEYFIELD_GROUP_DEVICES, devices);
        groupInfo.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);
        setGroupInfo(mGid1, groupInfo);
        // -- initialize group info

        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);
        members.add(mUid3);

        mGroupAclManager.addAceByMembers(mGid1, mPermission, members);

        // remove prefix in href
        ArrayList<HashMap<String, Object>> resourcesWithExtractedHref = new ArrayList<>(
                resources);
        for (HashMap<String, Object> resource : resourcesWithExtractedHref) {
            resource.put(Constants.KEYFIELD_ACE_RESOURCE_HREF,
                    extractHref(
                            resource.get(Constants.KEYFIELD_ACE_RESOURCE_HREF)
                                    .toString()));
        }

        /* d1(light1) */
        // check ACETable and ACL for d1-u2
        HashMap<String, Object> aceidCondition1 = new HashMap<>();
        aceidCondition1.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition1.put(Constants.KEYFIELD_DI, mDi1);
        aceidCondition1.put(Constants.KEYFIELD_UID, mUid2);
        String aceIdg1d1 = getAceId(aceidCondition1);

        HashMap<String, Object> aceCondition1 = new HashMap<>();
        aceCondition1.put(Constants.KEYFIELD_ACE_ID, aceIdg1d1);

        aceidCondition1.clear();
        aceidCondition1.put(Constants.KEYFIELD_OID, mUid1);
        assertTrue(checkAceTable(aceCondition1, aceidCondition1));
        assertTrue(checkAclTable(mDi1, aceIdg1d1, mUid2,
                resourcesWithExtractedHref));

        // check ACETable and ACL for d1-u3
        aceidCondition1.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition1.put(Constants.KEYFIELD_DI, mDi1);
        aceidCondition1.put(Constants.KEYFIELD_UID, mUid3);
        aceIdg1d1 = getAceId(aceidCondition1);

        aceCondition1.put(Constants.KEYFIELD_ACE_ID, aceIdg1d1);

        aceidCondition1.clear();
        aceidCondition1.put(Constants.KEYFIELD_OID, mUid1);
        assertTrue(checkAceTable(aceCondition1, aceidCondition1));
        assertTrue(checkAclTable(mDi1, aceIdg1d1, mUid3,
                resourcesWithExtractedHref));

        /* d2(all resources) */
        // check ACE not updated for d2-u2(device owner)
        HashMap<String, Object> aceidCondition2 = new HashMap<>();
        aceidCondition2.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition2.put(Constants.KEYFIELD_DI, mDi2);
        aceidCondition2.put(Constants.KEYFIELD_UID, mUid2);
        assertNull(getAceId(aceidCondition2));

        // check ACETable and ACL for d2-u3
        aceidCondition2.put(Constants.KEYFIELD_UID, mUid3);
        String aceIdg1d2 = getAceId(aceidCondition2);

        HashMap<String, Object> aceCondition2 = new HashMap<>();
        aceCondition2.put(Constants.KEYFIELD_ACE_ID, aceIdg1d2);

        aceidCondition2.clear();
        aceidCondition2.put(Constants.KEYFIELD_OID, mUid2);
        assertTrue(checkAceTable(aceidCondition2, aceidCondition2));
        assertTrue(checkAclTable(mDi2, aceIdg1d2, mUid3,
                Arrays.asList(mResourceAll)));
    }

    @Test
    public void testAddAceByMembersNoDevice() {
        Log.v("--------------testAddAceByMembersNoDevice------------");
        setGroupInfo(mGid1, new HashMap<>());

        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);

        mGroupAclManager.addAceByMembers(mGid1, mPermission, members);

        // check ACE not updated
        HashMap<String, Object> aceidCondition = new HashMap<>();
        aceidCondition.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition.put(Constants.KEYFIELD_UID, mUid2);
        assertNull(getAceId(aceidCondition));
    }

    @Test
    public void testAddAceByMembersOwnerDevice() {
        Log.v("--------------testAddAceByMembersOwnerDevice------------");

        // initialize group info --
        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi2);

        HashMap<String, Object> groupInfo = new HashMap<>();
        groupInfo.put(Constants.KEYFIELD_GROUP_DEVICES, devices);
        setGroupInfo(mGid1, groupInfo);
        // -- initialize group info

        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);

        mGroupAclManager.addAceByMembers(mGid1, mPermission, members);

        // check ACE not updated for d2-u2(device owner)
        HashMap<String, Object> aceidCondition = new HashMap<>();
        aceidCondition.put(Constants.REQ_GROUP_ID, mGid1);
        aceidCondition.put(Constants.REQ_GROUP_ID, mDi2);
        aceidCondition.put(Constants.KEYFIELD_UID, mUid2);
        assertNull(getAceId(aceidCondition));
    }

    @Test
    public void testRemoveAceByGroup() {
        Log.v("--------------testRemoveAceByGroup------------");

        // initialize group info --
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid1);
        members.add(mUid2);

        HashMap<String, Object> groupInfo = new HashMap<>();
        groupInfo.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        setGroupInfo(mGid1, groupInfo);
        setGroupInfo(mGid2, groupInfo);
        // -- initialize group info

        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);
        devices.add(mDi2);

        mGroupAclManager.addAceByDevices(mGid2, mPermission, devices);

        mGroupAclManager.addAceByDevices(mGid1, mPermission - 11, devices);

        mGroupAclManager.removeAceByGroup(mGid1);

        String aceIdg1d1, aceIdg2d1 = null;
        // verify ACE Group1
        HashMap<String, Object> aceidCondition1 = new HashMap<>();
        aceidCondition1.put(Constants.REQ_GROUP_ID, mGid1);

        // verify AceId is Null Cases Group1
        assertNull(getAceId(aceidCondition1));

        // verify ACE Device1 in Group2
        HashMap<String, Object> aceCondition2 = new HashMap<>();
        HashMap<String, Object> aceidCondition2 = new HashMap<>();
        aceidCondition2.put(Constants.REQ_GROUP_ID, mGid2);
        aceidCondition2.put(Constants.KEYFIELD_DI, mDi1);
        aceidCondition2.put(Constants.KEYFIELD_UID, mUid2);

        aceCondition2.put(Constants.KEYFIELD_ACE_ID,
                aceIdg1d1 = getAceId(aceidCondition2));
        aceidCondition2.clear();
        aceidCondition2.put(Constants.KEYFIELD_OID, mUid1);

        // verify AceId is Null Cases Group2
        assertTrue(checkAceTable(aceCondition2, aceidCondition2));
        // verify ACL Device1 Group2
        checkAclTable(mDi1, aceIdg1d1, mUid2, Arrays.asList(mResourceAll));

        // verify ACE Device1 in Group2
        HashMap<String, Object> aceCondition3 = new HashMap<>();
        HashMap<String, Object> aceidCondition3 = new HashMap<>();
        aceidCondition3.put(Constants.REQ_GROUP_ID, mGid2);
        aceidCondition3.put(Constants.KEYFIELD_DI, mDi2);
        aceidCondition3.put(Constants.KEYFIELD_UID, mUid1);

        aceCondition3.put(Constants.KEYFIELD_ACE_ID,
                aceIdg2d1 = getAceId(aceidCondition3));
        aceidCondition3.clear();
        aceidCondition3.put(Constants.KEYFIELD_OID, mUid2);

        // verify AceId is true Cases Group2
        assertTrue(checkAceTable(aceCondition3, aceidCondition3));
        // verify ACL Device2 Group2
        assertTrue(checkAclTable(mDi2, aceIdg2d1, mUid1,
                Arrays.asList(mResourceAll)));
    }

    @Test
    public void testRemoveAceByMembers() {
        Log.v("--------------testRemoveAceByMembers------------");

        // initialize group info --
        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);
        devices.add(mDi2);
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        resources.add(mResourceLight1ofD2);

        HashMap<String, Object> groupInfo = new HashMap<>();
        groupInfo.put(Constants.KEYFIELD_GROUP_DEVICES, devices);
        groupInfo.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);
        setGroupInfo(mGid1, groupInfo);
        // -- initialize group info

        ArrayList<String> members = new ArrayList<>();
        members.add(mUid1);
        members.add(mUid2);
        members.add(mUid3);

        mGroupAclManager.addAceByMembers(mGid1, mPermission, members);

        members.remove(mUid2);
        members.remove(mUid3);
        mGroupAclManager.removeAceByMembers(members, mGid1);

        String aceIdg1d1 = null;
        // verify ACE User1
        HashMap<String, Object> aceidCondition1 = new HashMap<>();
        aceidCondition1.put(Constants.KEYFIELD_OID, mUid1);

        // verify AceId is Null Cases User1
        assertNull(getAceId(aceidCondition1));

        // verify ACE User2
        HashMap<String, Object> aceCondition2 = new HashMap<>();
        HashMap<String, Object> aceidCondition2 = new HashMap<>();
        aceidCondition2.put(Constants.KEYFIELD_UID, mUid3);
        aceidCondition2.put(Constants.KEYFIELD_DI, mDi2);

        aceCondition2.put(Constants.KEYFIELD_ACE_ID,
                aceIdg1d1 = getAceId(aceidCondition2));
        aceidCondition2.clear();
        aceidCondition2.put(Constants.KEYFIELD_OID, mUid2);

        // remove prefix in href
        for (HashMap<String, Object> resource : resources) {
            resource.put(Constants.KEYFIELD_ACE_RESOURCE_HREF,
                    extractHref(
                            resource.get(Constants.KEYFIELD_ACE_RESOURCE_HREF)
                                    .toString()));
        }
        // verify AceId True
        assertTrue(checkAceTable(aceCondition2, aceidCondition2));
        // verify ACL Device1 Group2
        assertTrue(checkAclTable(mDi2, aceIdg1d1, mUid3, resources));
    }

    @Test
    public void testRemoveAceByDevices() {
        Log.v("--------------testRemoveAceByDevices------------");

        // initialize group info --
        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);
        devices.add(mDi2);

        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        resources.add(mResourceLight1ofD2);

        HashMap<String, Object> groupInfo = new HashMap<>();
        groupInfo.put(Constants.KEYFIELD_GROUP_DEVICES, devices);
        groupInfo.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);
        setGroupInfo(mGid1, groupInfo);
        // -- initialize group info

        ArrayList<String> members = new ArrayList<>();
        members.add(mUid1);
        members.add(mUid2);
        members.add(mUid3);

        mGroupAclManager.addAceByMembers(mGid1, mPermission, members);

        devices.remove(mDi2);
        mGroupAclManager.removeAceByDevices(devices, mGid1);

        String aceIdu1d2 = null, aceIdu3d2 = null;
        // verify ACE Device1
        HashMap<String, Object> aceidCondition1 = new HashMap<>();
        aceidCondition1.put(Constants.KEYFIELD_DI, mDi1);

        // verify AceId is Null Cases User1
        assertNull(getAceId(aceidCondition1));

        // verify ACE ownerid=User2, subjectuuid=User1
        HashMap<String, Object> aceCondition2 = new HashMap<>();
        HashMap<String, Object> aceidCondition2 = new HashMap<>();
        aceidCondition2.put(Constants.KEYFIELD_UID, mUid1);
        aceidCondition2.put(Constants.KEYFIELD_DI, mDi2);

        aceCondition2.put(Constants.KEYFIELD_ACE_ID,
                aceIdu1d2 = getAceId(aceidCondition2));
        aceidCondition2.clear();
        aceidCondition2.put(Constants.KEYFIELD_OID, mUid2);

        // verify ACE ownerid=User2, subjectuuid=User3
        HashMap<String, Object> aceCondition3 = new HashMap<>();
        HashMap<String, Object> aceidCondition3 = new HashMap<>();
        aceidCondition3.put(Constants.KEYFIELD_UID, mUid3);
        aceidCondition3.put(Constants.KEYFIELD_DI, mDi2);

        aceCondition3.put(Constants.KEYFIELD_ACE_ID,
                aceIdu3d2 = getAceId(aceidCondition3));
        aceidCondition3.clear();
        aceidCondition3.put(Constants.KEYFIELD_OID, mUid2);

        // remove prefix in href
        for (HashMap<String, Object> resource : resources) {
            resource.put(Constants.KEYFIELD_ACE_RESOURCE_HREF,
                    extractHref(
                            resource.get(Constants.KEYFIELD_ACE_RESOURCE_HREF)
                                    .toString()));
        }

        // verify AceId ownerid=User2, subjectuuid=User1 True
        assertTrue(checkAceTable(aceCondition2, aceidCondition2));
        // verify ACL Device2 User1
        assertTrue(checkAclTable(mDi2, aceIdu1d2, mUid1,
                Arrays.asList(mResourceLight1ofD2)));

        // verify AceId ownerid=User2, subjectuuid=User3 True
        assertTrue(checkAceTable(aceCondition3, aceidCondition3));
        // verify ACL Device2 User1
        assertTrue(checkAclTable(mDi2, aceIdu3d2, mUid3,
                Arrays.asList(mResourceLight1ofD2)));

    }

    @Test
    public void testRemoveAceByResourcesDeleteAce() {
        Log.v("--------------testRemoveAceByResourcesDeleteAce------------");

        // initialize group info --
        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);
        devices.add(mDi2);
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid1);
        members.add(mUid2);

        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        resources.add(mResourceLight1ofD2);

        HashMap<String, Object> groupInfo = new HashMap<>();
        groupInfo.put(Constants.KEYFIELD_GROUP_DEVICES, devices);
        groupInfo.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);
        setGroupInfo(mGid1, groupInfo);
        // -- initialize group info

        mGroupAclManager.addAceByMembers(mGid1, mPermission, members);

        mGroupAclManager.removeAceByResources(
                new ArrayList<>(Arrays.asList(mResourceLight1ofD2)), mGid1);

        // verify ACE Device2
        HashMap<String, Object> aceidCondition1 = new HashMap<>();
        aceidCondition1.put(Constants.KEYFIELD_DI, mDi2);

        // verify AceId is Null Cases User1
        assertNull(getAceId(aceidCondition1));

        String aceIdu1d2 = null;
        // verify ACE ownerid=u1, subjectuuid=u2 Deivce=d1
        HashMap<String, Object> aceCondition2 = new HashMap<>();
        HashMap<String, Object> aceidCondition2 = new HashMap<>();
        aceidCondition2.put(Constants.KEYFIELD_UID, mUid2);
        aceidCondition2.put(Constants.KEYFIELD_DI, mDi1);

        aceCondition2.put(Constants.KEYFIELD_ACE_ID,
                aceIdu1d2 = getAceId(aceidCondition2));
        aceidCondition2.clear();
        aceidCondition2.put(Constants.KEYFIELD_OID, mUid1);

        // verify AceId ownerid=u1, subjectuuid=u2 True
        assertTrue(checkAceTable(aceCondition2, aceidCondition2));
        // verify ACL Device2 User1
        assertTrue(checkAclTable(mDi1, aceIdu1d2, mUid2,
                Arrays.asList(mResourceAll)));
    }

    @Test
    public void testRemoveAceByResourcesRemainAce() {

        // initialize group info --
        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);
        devices.add(mDi2);
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid1);
        members.add(mUid2);

        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        resources.add(mResourceLight1ofD2);
        resources.add(mResourceLight2ofD2);

        HashMap<String, Object> groupInfo = new HashMap<>();
        groupInfo.put(Constants.KEYFIELD_GROUP_DEVICES, devices);
        groupInfo.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);
        setGroupInfo(mGid1, groupInfo);
        // -- initialize group info

        mGroupAclManager.addAceByMembers(mGid1, mPermission, members);

        mGroupAclManager.removeAceByResources(
                new ArrayList<>(Arrays.asList(mResourceLight1ofD2)), mGid1);

        String aceIdu2d1 = null, aceIdu1d2 = null;
        // verify ACE ownerid=u1, subjectuuid=u1 Deivce=d2
        HashMap<String, Object> aceCondition1 = new HashMap<>();
        HashMap<String, Object> aceidCondition1 = new HashMap<>();
        aceidCondition1.put(Constants.KEYFIELD_UID, mUid1);
        aceidCondition1.put(Constants.KEYFIELD_DI, mDi2);

        aceCondition1.put(Constants.KEYFIELD_ACE_ID,
                aceIdu1d2 = getAceId(aceidCondition1));
        aceidCondition1.clear();
        aceidCondition1.put(Constants.KEYFIELD_OID, mUid2);

        // remove prefix in href
        for (HashMap<String, Object> resource : resources) {
            resource.put(Constants.KEYFIELD_ACE_RESOURCE_HREF,
                    extractHref(
                            resource.get(Constants.KEYFIELD_ACE_RESOURCE_HREF)
                                    .toString()));
        }
        // verify AceId ownerid=u2, subjectuuid=u1 True
        assertTrue(checkAceTable(aceCondition1, aceidCondition1));
        // verify ACL Device2 User1
        assertTrue(checkAclTable(mDi2, aceIdu1d2, mUid1,
                Arrays.asList(mResourceLight2ofD2)));

        // verify ACE ownerid=u1, subjectuuid=u2 Deivce=d1
        HashMap<String, Object> aceCondition2 = new HashMap<>();
        HashMap<String, Object> aceidCondition2 = new HashMap<>();
        aceidCondition2.put(Constants.KEYFIELD_UID, mUid2);
        aceidCondition2.put(Constants.KEYFIELD_DI, mDi1);

        aceCondition2.put(Constants.KEYFIELD_ACE_ID,
                aceIdu1d2 = getAceId(aceidCondition2));
        aceidCondition2.clear();
        aceidCondition2.put(Constants.KEYFIELD_OID, mUid1);

        // verify AceId ownerid=u1, subjectuuid=u2 True
        assertTrue(checkAceTable(aceCondition2, aceidCondition2));
        // verify ACL Device2 User1
        assertTrue(checkAclTable(mDi1, aceIdu1d2, mUid2,
                Arrays.asList(mResourceAll)));
    }

    private boolean checkAclTable(String di, String aceid, String subjectUuid,
            List<HashMap<String, Object>> resource) {
        Log.v("--------------checkAclTable : " + di + " ------------");
        HashMap<String, Object> getAclist = AclResource.getInstance()
                .getAclACE((String) AclResource.getInstance().getAclid(di)
                        .get(Constants.KEYFIELD_ACLID), aceid);
        Log.v("check result : " + getAclist);
        if (getAclist.isEmpty()) {
            return false;
        }

        // check subject uuid
        assertTrue(getAclist.containsKey(Constants.KEYFIELD_ACE_SUBJECT_ID));
        assertTrue(getAclist.get(Constants.KEYFIELD_ACE_SUBJECT_ID)
                .equals(subjectUuid));

        // check resource
        assertTrue(getAclist.containsKey(Constants.KEYFIELD_ACE_RESOURCE));
        for (HashMap<String, Object> targetRes : resource) {
            boolean isbreak = false;
            for (HashMap<String, Object> res : (ArrayList<HashMap<String, Object>>) getAclist
                    .get(Constants.KEYFIELD_ACE_RESOURCE)) {
                assertTrue(
                        res.containsKey(Constants.KEYFIELD_ACE_RESOURCE_HREF));
                if (res.get(Constants.KEYFIELD_ACE_RESOURCE_HREF).equals(
                        targetRes.get(Constants.KEYFIELD_ACE_RESOURCE_HREF))) {
                    isbreak = true;
                    break;
                }
            }
            if (isbreak) {
                continue;
            }
            return false;
        }
        return true;
    }

    private boolean checkAceTable(HashMap<String, Object> condition,
            HashMap<String, Object> key) {
        Log.v("--------------checkAceTable : " + condition.toString()
                + " ------------");
        HashMap<String, Object> getAce = getAceTable(condition).get(0);
        Log.v("check result : " + getAce);
        if (getAce.isEmpty()) {
            return false;
        }
        for (String keyelement : key.keySet()) {
            assertTrue(getAce.containsKey(keyelement));
            assertTrue(getAce.get(keyelement).equals(key.get(keyelement)));
        }
        return true;
    }

    private String getAceId(HashMap<String, Object> condition) {

        ArrayList<HashMap<String, Object>> aceTables = AccountDBManager
                .getInstance().selectRecord(Constants.ACE_TABLE, condition);

        if (aceTables == null || aceTables.isEmpty()) {
            return null;
        }

        return (String) aceTables.get(0).get(Constants.KEYFIELD_ACE_ID);
    }

    private ArrayList<HashMap<String, Object>> getAceTable(
            HashMap<String, Object> condition) {
        return AccountDBManager.getInstance().selectRecord(Constants.ACE_TABLE,
                condition);
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

    private void setGroupInfo(String gid, HashMap<String, Object> keyValue) {

        GroupTable groupTable = new GroupTable();
        groupTable.setGid(gid);
        for (String key : keyValue.keySet()) {
            groupTable.setPropertyValue(key, keyValue.get(key));
        }

        TypeCastingManager<GroupTable> typeGroup = new TypeCastingManager<GroupTable>();
        AccountDBManager.getInstance().insertAndReplaceRecord(
                Constants.GROUP_TABLE,
                typeGroup.convertObjectToMap(groupTable));
    }
}
