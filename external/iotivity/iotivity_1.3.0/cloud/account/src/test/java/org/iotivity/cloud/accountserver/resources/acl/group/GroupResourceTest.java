
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

import static java.util.concurrent.TimeUnit.SECONDS;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.CountDownLatch;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.MongoDB;
import org.iotivity.cloud.accountserver.resources.acl.id.AclResource;
import org.iotivity.cloud.base.device.CoapDevice;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.util.Cbor;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

public class GroupResourceTest {
    private static final String           GROUP_URI      = Constants.GROUP_FULL_URI;
    private String                        mGid1          = "g1";
    private String                        mGid2          = "g2";
    private String                        mDi1           = "d1";
    private String                        mDi2           = "d2";
    private String                        mDi3           = "d3";
    private String                        mDi4           = "d4";
    private String                        mUid1          = "u1";
    private String                        mUid2          = "u2";
    private String                        mUid3          = "u3";
    private String                        mUid4          = "u4";
    private String                        mGroupId       = null;
    final CountDownLatch                  mLatch         = new CountDownLatch(
            1);
    private CoapDevice                    mMockDevice    = Mockito
            .mock(CoapDevice.class);
    private Cbor<HashMap<String, Object>> mCbor          = new Cbor<>();
    private IResponse                     mResponse      = null;
    private GroupResource                 mGroupResource = new GroupResource();
    private HashMap<String, Object>       mPayload       = new HashMap<>();

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        // reset data base
        resetDB();
        Mockito.doAnswer(new Answer<Object>() {
            @Override
            public CoapResponse answer(InvocationOnMock invocation)
                    throws Throwable {
                Object[] args = invocation.getArguments();
                CoapResponse resp = (CoapResponse) args[0];
                System.out.println("\t---------method : " + resp.getStatus());
                mResponse = resp;
                if (mGroupId == null && !resp.getPayloadString().isEmpty()) {
                    HashMap<String, Object> payloadData = mCbor
                            .parsePayloadFromCbor(resp.getPayload(),
                                    HashMap.class);
                    System.out.println("\t----------payload : " + payloadData);
                    mPayload = payloadData;
                    if (payloadData.containsKey(Constants.KEYFIELD_GID)) {
                        mGroupId = (String) payloadData
                                .get(Constants.KEYFIELD_GID);
                    }
                }
                mLatch.countDown();
                return null;
            }
        }).when(mMockDevice).sendResponse(Mockito.anyObject());

        makeExampleGroup();
    }

    @After
    public void resetAccountDatabase() throws Exception {
        MongoDB mongoDB = new MongoDB("127.0.0.1", Constants.DB_NAME);
        mongoDB.deleteTable(Constants.USER_TABLE);
        mongoDB.createTable(Constants.USER_TABLE);
        mongoDB.deleteTable(Constants.TOKEN_TABLE);
        mongoDB.createTable(Constants.TOKEN_TABLE);
        mongoDB.deleteTable(Constants.GROUP_TABLE);
        mongoDB.createTable(Constants.GROUP_TABLE);
        mongoDB.deleteTable(Constants.ACL_TABLE);
        mongoDB.createTable(Constants.ACL_TABLE);
        mongoDB.deleteTable(Constants.ACE_TABLE);
        mongoDB.createTable(Constants.ACE_TABLE);
    }

    @Test
    public void testCreateGroup() throws Exception {
        getTestMethodName();
        createGroup(mMockDevice, mUid1, "myhome", null);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(hashmapCheck(mResponse, "gid"));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testCreateSubGroup() throws Exception {
        getTestMethodName();
        createGroup(mMockDevice, mUid1, "myroom2", mGid1);
        createGroup(mMockDevice, mUid1, "mybalcony", mGid1);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(hashmapCheck(mResponse, "gid"));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testCreateSubGroupNotExistParentGroup() throws Exception {
        getTestMethodName();
        createGroup(mMockDevice, mUid1, "myhome", "NOT");
    }

    @Test
    public void testAddDeviceToGroup() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);
        devices.add(mDi2);
        devices.add(mDi3);
        properties.put(Constants.KEYFIELD_GROUP_DEVICES, devices);
        addProperties(mMockDevice, mGid1, mUid1, properties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testAddNotSupportedPropertyToGroup() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> devices = new ArrayList<>();
        devices.add("d1");
        devices.add("d2");
        devices.add("d3");
        properties.put("NOT_SUPPORTED_PROPERTY", devices);
        addProperties(mMockDevice, mGid1, mUid1, properties);
    }

    @Test
    public void testAddResourceToGroup() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        // add resource 1
        resources.add(makeResources("/di/" + mDi1 + "/a/light/0",
                Arrays.asList("core.light"), Arrays.asList("oic.if.baseline")));
        resources.add(makeResources("/di/" + mDi1 + "/a/switch/1",
                Arrays.asList("core.switch"),
                Arrays.asList("oic.if.baseline")));
        properties.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);
        addProperties(mMockDevice, mGid1, mUid1, properties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testAddDeviceToSubGroup() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);
        devices.add(mDi2);
        properties.put(Constants.KEYFIELD_GROUP_DEVICES, devices);
        addProperties(mMockDevice, mGid1, mUid1, properties);
        addProperties(mMockDevice, mGid2, mUid1, properties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testAddDeviceToSubGroupNotByDeviceOwner() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);
        devices.add(mDi2);
        devices.add(mDi3);
        GroupManager.getInstance().addDevicesToGroup(mGid1, devices);
        properties.put(Constants.KEYFIELD_GROUP_DEVICES, devices);
        addProperties(mMockDevice, mGid2, mUid2, properties);
    }

    @Test
    public void testAddMembersToSubGroup() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);
        members.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        // add members to the parent group
        GroupManager.getInstance().addMembersToGroup(mGid1, members);
        addProperties(mMockDevice, mGid2, mUid1, properties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testAddMastersToSubGroup() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);
        members.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        // add members to the parent group
        GroupManager.getInstance().addMembersToGroup(mGid1, members);
        HashMap<String, Object> propertiesSubgroup = new HashMap<>();
        ArrayList<String> masters = new ArrayList<>();
        masters.add(mUid2);
        masters.add(mUid3);
        propertiesSubgroup.put(Constants.KEYFIELD_GROUP_MASTERS, masters);
        addProperties(mMockDevice, mGid2, mUid1, propertiesSubgroup);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testAddMastersToSubGroupByMaster() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);
        GroupManager.getInstance().addMastersToGroup(mGid1, members);
        GroupManager.getInstance().addMastersToGroup(mGid2, members);
        members.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        // add members to the parent group
        GroupManager.getInstance().addMembersToGroup(mGid1, members);
        HashMap<String, Object> propertiesSubgroup = new HashMap<>();
        ArrayList<String> masters = new ArrayList<>();
        masters.add(mUid3);
        propertiesSubgroup.put(Constants.KEYFIELD_GROUP_MASTERS, masters);
        addProperties(mMockDevice, mGid2, mUid2, propertiesSubgroup);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testAddResourceToSubgroup() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        // add resource 1
        resources.add(makeResources("/di/" + mDi1 + "/a/light/0",
                Arrays.asList("core.light"), Arrays.asList("oic.if.baseline")));
        resources.add(makeResources("/di/" + mDi1 + "/a/switch/1",
                Arrays.asList("core.switch"),
                Arrays.asList("oic.if.baseline")));
        properties.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);
        addProperties(mMockDevice, mGid1, mUid1, properties);

        HashMap<String, Object> subgroupProperties = new HashMap<>();
        ArrayList<HashMap<String, Object>> subgroupResources = new ArrayList<>();
        subgroupResources.add(makeResources("/di/" + mDi1 + "/a/light/0",
                Arrays.asList("core.light"), Arrays.asList("oic.if.baseline")));
        subgroupProperties.put(Constants.KEYFIELD_GROUP_RESOURCES,
                subgroupResources);
        addProperties(mMockDevice, mGid2, mUid1, subgroupProperties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testAddResourceDiffRtToSubgroup() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        // add resource 1
        resources.add(makeResources("/di/" + mDi1 + "/a/light/0",
                Arrays.asList("core.light"), Arrays.asList("oic.if.baseline")));
        resources.add(makeResources("/di/" + mDi1 + "/a/switch/1",
                Arrays.asList("core.switch"),
                Arrays.asList("oic.if.baseline")));
        properties.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);
        addProperties(mMockDevice, mGid1, mUid1, properties);

        HashMap<String, Object> subgroupProperties = new HashMap<>();
        ArrayList<HashMap<String, Object>> subgroupResources = new ArrayList<>();
        subgroupResources.add(makeResources("/di/" + mDi1 + "/a/light/0",
                Arrays.asList("core.none"), Arrays.asList("oic.if.baseline")));
        subgroupProperties.put(Constants.KEYFIELD_GROUP_RESOURCES,
                subgroupResources);
        addProperties(mMockDevice, mGid2, mUid1, subgroupProperties);
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testAddResourceGroupInvalidFormat() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        // add resource 1
        resources.add(makeResources("/di/" + mDi3 + "/a/light/0", "core.light",
                "oic.if.baseline"));
        properties.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);
        addProperties(mMockDevice, mGid1, mUid1, properties);
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testAddMembersToSubGroupUnauthorized() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);
        members.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        // add members to the parent group
        GroupManager.getInstance().addMembersToGroup(mGid1, members);
        addProperties(mMockDevice, mGid2, mUid3, properties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testAddNotExistingDeviceToSubGroup() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi1);
        devices.add(mDi2);
        properties.put(Constants.KEYFIELD_GROUP_DEVICES, devices);
        addProperties(mMockDevice, mGid1, mUid1, properties);
        devices.add(mDi3);
        addProperties(mMockDevice, mGid2, mUid1, properties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testPromoteMemberToMaster() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);
        members.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        // add members to the parent group
        GroupManager.getInstance().addMembersToGroup(mGid1, members);

        HashMap<String, Object> propertiesMaster = new HashMap<>();
        ArrayList<String> masters = new ArrayList<>();
        masters.add(mUid2);
        masters.add(mUid3);
        propertiesMaster.put(Constants.KEYFIELD_GROUP_MASTERS, masters);
        addProperties(mMockDevice, mGid1, mUid1, propertiesMaster);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testGetGroup() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);
        members.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        // add members to the parent group
        GroupManager.getInstance().addMembersToGroup(mGid1, members);
        getGroupInfo(mMockDevice, mGid1, mUid1);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testGetNonExistongGroup() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);
        members.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        // add members to the parent group
        GroupManager.getInstance().addMembersToGroup(mGid1, members);
        getGroupInfo(mMockDevice, mGid1 + "notExist", mUid1);
        assertTrue(methodCheck(mResponse, ResponseStatus.CONTENT));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testGetGroupByNonExistingUser() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);
        members.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);
        // add members to the parent group
        GroupManager.getInstance().addMembersToGroup(mGid1, members);
        getGroupInfo(mMockDevice, mGid1, "NON_EXIST_USER");
    }

    @Test
    public void testAddMultipleProperties() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        // add resource 1
        resources.add(makeResources("/di/" + mDi1 + "/a/light/0",
                Arrays.asList("core.light"), Arrays.asList("oic.if.baseline")));
        resources.add(makeResources("/di/" + mDi1 + "/a/switch/1",
                Arrays.asList("core.switch"),
                Arrays.asList("oic.if.baseline")));
        properties.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);

        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);

        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);

        ArrayList<String> masters = new ArrayList<>();
        masters.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MASTERS, masters);

        addProperties(mMockDevice, mGid1, mUid1, properties);

        HashMap<String, Object> subgroupProperties = new HashMap<>();
        ArrayList<HashMap<String, Object>> subgroupResources = new ArrayList<>();
        subgroupResources.add(makeResources("/di/" + mDi1 + "/a/light/0",
                Arrays.asList("core.light"), Arrays.asList("oic.if.baseline")));
        subgroupProperties.put(Constants.KEYFIELD_GROUP_RESOURCES,
                subgroupResources);
        addProperties(mMockDevice, mGid2, mUid1, subgroupProperties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testDeleteMultipleProperties() throws Exception {
        getTestMethodName();

        HashMap<String, Object> properties = setExampleGroup();

        properties.remove(Constants.KEYFIELD_GROUP_MEMBERS);

        deleteProperties(mMockDevice, mGid1, mUid1, properties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testDeleteUnregisteredDevices() throws Exception {
        getTestMethodName();

        setExampleGroup();

        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<String> devices = new ArrayList<String>();
        devices.addAll(Arrays.asList(mDi3, mDi2));

        properties.put(Constants.KEYFIELD_GROUP_DEVICES, devices);

        deleteProperties(mMockDevice, mGid1, mUid1, properties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testAddResources() throws Exception {
        getTestMethodName();
        setExampleGroup();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        resources.add(makeResources("/di/" + mDi3 + "/a/fan",
                Arrays.asList("core.fan"), Arrays.asList("oic.if.baseline")));
        resources.add(makeResources("/di/" + mDi4 + "/a/fan",
                Arrays.asList("core.fan"), Arrays.asList("oic.if.baseline")));
        properties.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);
        addProperties(mMockDevice, mGid1, mUid1, properties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testDeleteGroup() throws Exception {
        getTestMethodName();
        setExampleGroup();
        deleteGroup(mMockDevice, mGid1, mUid1);
        assertTrue(methodCheck(mResponse, ResponseStatus.DELETED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testDeleteNonExistingGroup() throws Exception {
        getTestMethodName();
        setExampleGroup();
        deleteGroup(mMockDevice, mGid1 + "notExist", mUid1);
        assertTrue(methodCheck(mResponse, ResponseStatus.DELETED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testDeleteGroupByMaster() throws Exception {
        getTestMethodName();
        setExampleGroup();
        deleteGroup(mMockDevice, mGid1, mUid2);
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testDeleteGroupByUnregisteredUser() throws Exception {
        getTestMethodName();
        setExampleGroup();
        deleteGroup(mMockDevice, mGid1, "UNREGISTERED_USER");
    }

    @Test
    public void testAddDeviceAndDeleteResources() throws Exception {
        getTestMethodName();
        setExampleGroup();
        HashMap<String, Object> properties = new HashMap<>();
        properties.put(Constants.KEYFIELD_GROUP_DEVICES, Arrays.asList(mDi1));
        addProperties(mMockDevice, mGid1, mUid1, properties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testUpdateResources() throws Exception {
        getTestMethodName();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        // add resource 1
        resources.add(makeResources("/di/" + mDi1 + "/a/light/0",
                Arrays.asList("core.light"), Arrays.asList("oic.if.baseline")));
        resources.add(makeResources("/di/" + mDi1 + "/a/switch/1",
                Arrays.asList("core.switch"),
                Arrays.asList("oic.if.baseline")));
        properties.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);

        ArrayList<String> members = new ArrayList<>();
        members.add(mUid2);

        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);

        ArrayList<String> masters = new ArrayList<>();
        masters.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MASTERS, masters);

        addProperties(mMockDevice, mGid1, mUid1, properties);

        HashMap<String, Object> subgroupProperties = new HashMap<>();
        ArrayList<HashMap<String, Object>> subgroupResources = new ArrayList<>();
        subgroupResources.add(makeResources("/di/" + mDi1 + "/a/light/0",
                Arrays.asList("core.light"), Arrays.asList("oic.if.baseline")));
        subgroupProperties.put(Constants.KEYFIELD_GROUP_RESOURCES,
                subgroupResources);
        addProperties(mMockDevice, mGid2, mUid1, subgroupProperties);

        HashMap<String, Object> updatedProperties = new HashMap<>();
        ArrayList<HashMap<String, Object>> updatedResources = new ArrayList<>();
        updatedResources.add(makeResources("/di/" + mDi1 + "/a/switch/1",
                Arrays.asList("core.switch"),
                Arrays.asList("oic.if.baseline")));
        updatedProperties.put(Constants.KEYFIELD_GROUP_NAME, "mypark");
        ArrayList<String> updatedMasters = new ArrayList<>();
        updatedMasters.add(mUid4);
        updatedProperties.put(Constants.KEYFIELD_GROUP_MASTERS, updatedMasters);
        updatedProperties.put(Constants.KEYFIELD_GROUP_RESOURCES,
                updatedResources);
        updateProperties(mMockDevice, mGid1, mUid1, updatedProperties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test
    public void testUpdateProperties() throws Exception {
        getTestMethodName();
        setExampleGroup();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        // add resource 1
        resources.add(makeResources("/di/" + mDi1 + "/a/switch/1",
                Arrays.asList("core.switch"),
                Arrays.asList("oic.if.baseline")));
        properties.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);

        ArrayList<String> members = new ArrayList<>();
        members.add(mUid1);
        members.add(mUid2);

        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);

        ArrayList<String> masters = new ArrayList<>();
        masters.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MASTERS, masters);

        updateProperties(mMockDevice, mGid1, mUid1, properties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    @Test(expected = ServerException.BadRequestException.class)
    public void testUpdatePropertiesToNonExistingGroup() throws Exception {
        getTestMethodName();
        setExampleGroup();
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        // add resource 1
        resources.add(makeResources("/di/" + mDi1 + "/a/switch/1",
                Arrays.asList("core.switch"),
                Arrays.asList("oic.if.baseline")));
        properties.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);

        ArrayList<String> members = new ArrayList<>();
        members.add(mUid1);
        members.add(mUid2);

        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);

        ArrayList<String> masters = new ArrayList<>();
        masters.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MASTERS, masters);

        updateProperties(mMockDevice, mGid1 + "nonExists", mUid1, properties);
        assertTrue(methodCheck(mResponse, ResponseStatus.CHANGED));
        assertTrue(mLatch.await(2L, SECONDS));
    }

    private HashMap<String, Object> setExampleGroup() throws Exception {
        HashMap<String, Object> properties = new HashMap<>();
        ArrayList<HashMap<String, Object>> resources = new ArrayList<>();
        // add resource 1
        resources.add(makeResources("/di/" + mDi1 + "/a/light/0",
                Arrays.asList("core.light"), Arrays.asList("oic.if.baseline")));
        resources.add(makeResources("/di/" + mDi1 + "/a/switch/1",
                Arrays.asList("core.switch"),
                Arrays.asList("oic.if.baseline")));
        resources.add(makeResources("/di/" + mDi1 + "/a/fan/1",
                Arrays.asList("core.fan"), Arrays.asList("oic.if.baseline")));
        properties.put(Constants.KEYFIELD_GROUP_RESOURCES, resources);

        ArrayList<String> members = new ArrayList<>();
        members.addAll(Arrays.asList(mUid2, mUid3, mUid4));
        properties.put(Constants.KEYFIELD_GROUP_MEMBERS, members);

        ArrayList<String> masters = new ArrayList<>();
        masters.add(mUid3);
        properties.put(Constants.KEYFIELD_GROUP_MASTERS, masters);

        ArrayList<String> devices = new ArrayList<>();
        devices.add(mDi3);
        properties.put(Constants.KEYFIELD_GROUP_DEVICES, devices);

        addProperties(mMockDevice, mGid1, mUid1, properties);

        HashMap<String, Object> subgroupProperties = new HashMap<>();
        ArrayList<HashMap<String, Object>> subgroupResources = new ArrayList<>();
        subgroupResources.add(makeResources("/di/" + mDi1 + "/a/light/0",
                Arrays.asList("core.light"), Arrays.asList("oic.if.baseline")));
        subgroupProperties.put(Constants.KEYFIELD_GROUP_RESOURCES,
                subgroupResources);
        addProperties(mMockDevice, mGid2, mUid1, subgroupProperties);
        System.out.println("---------- set Example Group END");
        return properties;
    }

    private void deleteGroup(CoapDevice device, String gid, String uid)
            throws Exception {
        System.out.println("-----Get group, gid : " + gid);
        IRequest request = MessageBuilder.createRequest(RequestMethod.DELETE,
                GROUP_URI + "/" + gid, Constants.REQ_UUID_ID + "=" + uid + ";"
                        + Constants.KEYFIELD_GROUP_OWNER + "=" + uid);
        mGroupResource.onDefaultRequestReceived(device, request);
    }

    private void makeExampleGroup() {
        GroupBrokerManager.getInstance().createGroup(mUid1, mGid1, "myhome",
                null);
        GroupBrokerManager.getInstance().createGroup(mUid1, mGid2, "myroom",
                mGid1);
        AclResource.getInstance().createAcl(mUid1, mDi1);
        AclResource.getInstance().createAcl(mUid1, mDi2);
        AclResource.getInstance().createAcl(mUid1, mDi3);
        AclResource.getInstance().createAcl(mUid1, mDi4);
    }

    private <T> HashMap<String, Object> makePropertyMap(String property,
            T value) {
        HashMap<String, Object> map = new HashMap<String, Object>();
        map.put(property, value);
        return map;
    }

    private HashMap<String, Object> makeResources(String href, String rt,
            String itf) {
        HashMap<String, Object> resource = new HashMap<String, Object>();
        resource.put(Constants.KEYFIELD_ACE_RESOURCE_HREF, href);
        resource.put(Constants.RS_INTERFACE, itf);
        resource.put(Constants.KEYFIELD_RESOURCE_RT, rt);
        return resource;
    }

    private HashMap<String, Object> makeResources(String href, List<String> rt,
            List<String> itf) {
        HashMap<String, Object> resource = new HashMap<String, Object>();
        resource.put(Constants.KEYFIELD_ACE_RESOURCE_HREF, href);
        resource.put(Constants.RS_INTERFACE, itf);
        resource.put(Constants.KEYFIELD_RESOURCE_RT, rt);
        return resource;
    }

    private void getGroupInfo(CoapDevice device, String gid, String uid)
            throws Exception {
        System.out.println("-----Get group, gid : " + gid);
        IRequest request = MessageBuilder.createRequest(RequestMethod.GET,
                GROUP_URI + "/" + gid, Constants.REQ_UUID_ID + "=" + uid + ";"
                        + Constants.KEYFIELD_GROUP_MEMBERS + "=" + uid);
        mGroupResource.onDefaultRequestReceived(device, request);
    }

    private void addProperties(CoapDevice device, String gid, String uid,
            HashMap<String, Object> properties) throws Exception {
        System.out.println("-----Add properties");
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.POST,
                GROUP_URI + "/" + gid,
                Constants.KEYFIELD_UID + "=" + uid + ";"
                        + Constants.REQ_GROUP_QUERY_OPERATION + "="
                        + Constants.REQ_GROUP_QUERY_ADD,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(properties));
        mGroupResource.onDefaultRequestReceived(device, request);
    }

    private void updateProperties(CoapDevice device, String gid, String uid,
            HashMap<String, Object> properties) throws Exception {
        System.out.println("-----Update properties");
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.POST,
                GROUP_URI + "/" + gid, Constants.KEYFIELD_UID + "=" + uid,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(properties));
        mGroupResource.onDefaultRequestReceived(device, request);
    }

    private void deleteProperties(CoapDevice device, String gid, String uid,
            HashMap<String, Object> properties) throws Exception {
        System.out.println("-----Delete properties");
        IRequest request = null;
        request = MessageBuilder.createRequest(RequestMethod.POST,
                GROUP_URI + "/" + gid,
                Constants.KEYFIELD_UID + "=" + uid + ";"
                        + Constants.REQ_GROUP_QUERY_OPERATION + "="
                        + Constants.REQ_GROUP_QUERY_DELETE,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(properties));
        mGroupResource.onDefaultRequestReceived(device, request);
    }

    private void createGroup(CoapDevice device, String uid, String gname,
            String parent) throws Exception {
        System.out.println("-----Create Group");
        IRequest request = null;
        HashMap<String, Object> payloadData = new HashMap<String, Object>();
        payloadData.put(Constants.KEYFIELD_GROUP_NAME, gname);
        payloadData.put(Constants.KEYFIELD_GROUP_PARENT, parent);
        payloadData.put(Constants.KEYFIELD_GROUP_MEMBERS,
                new ArrayList<String>(Arrays.asList(uid)));
        payloadData.put(Constants.KEYFIELD_OID, uid);
        request = MessageBuilder.createRequest(RequestMethod.POST, GROUP_URI,
                Constants.KEYFIELD_UID + "=" + uid,
                ContentFormat.APPLICATION_CBOR,
                mCbor.encodingPayloadToCbor(payloadData));
        mGroupResource.onDefaultRequestReceived(device, request);
    }

    private boolean hashmapCheck(IResponse response, String propertyName) {
        HashMap<String, Object> payloadData = mCbor
                .parsePayloadFromCbor(response.getPayload(), HashMap.class);
        if (payloadData.containsKey(propertyName))
            return true;
        else
            return false;
    }

    private boolean methodCheck(IResponse response,
            ResponseStatus responseStatus) {
        if (responseStatus == response.getStatus())
            return true;
        else
            return false;
    }

    private void getTestMethodName() {
        StackTraceElement[] stacks = new Throwable().getStackTrace();
        StackTraceElement currentStack = stacks[1];
        System.out.println("\t---Test Name : " + currentStack.getMethodName());
    }

    public void resetDB() throws Exception {
        MongoDB mongoDB = new MongoDB("127.0.0.1", Constants.DB_NAME);
        mongoDB.deleteTable(Constants.GROUP_TABLE);
        mongoDB.createTable(Constants.GROUP_TABLE);
        mongoDB.deleteTable(Constants.USER_TABLE);
        mongoDB.createTable(Constants.USER_TABLE);
        mongoDB.deleteTable(Constants.TOKEN_TABLE);
        mongoDB.createTable(Constants.TOKEN_TABLE);
    }
}
