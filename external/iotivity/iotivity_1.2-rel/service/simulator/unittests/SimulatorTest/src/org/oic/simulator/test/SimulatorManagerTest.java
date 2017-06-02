/*
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.oic.simulator.test;

import java.util.Vector;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import org.oic.simulator.DeviceInfo;
import org.oic.simulator.DeviceListener;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.PlatformInfo;
import org.oic.simulator.PlatformListener;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorManager;
import org.oic.simulator.client.FindResourceListener;
import org.oic.simulator.client.SimulatorRemoteResource;
import org.oic.simulator.server.SimulatorResource;
import org.oic.simulator.utils.ObjectHolder;
import org.oic.simulator.utils.SampleSingleResource;

import junit.framework.TestCase;

/**
 * This class tests the functionality of Simulator Manager class APIs.
 */
public class SimulatorManagerTest extends TestCase {
    private static final String SINGLE_RES_RAML     = "./ramls/oic.r.light.raml";
    private static final String COLLECTION_RES_RAML = "./ramls/oic.d.airconditioner.raml";

    @Override
    protected void setUp() throws Exception {
        super.setUp();
    }

    @Override
    protected void tearDown() throws Exception {
        super.tearDown();
    }

    public void testCreateResource_P01() {
        SimulatorResource resource = null;
        SimulatorResource.Type resType = SimulatorResource.Type.SINGLE;

        try {
            resource = SimulatorManager.createResource(SINGLE_RES_RAML);
            resType = resource.getType();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(resource);
        assertTrue(resType == SimulatorResource.Type.SINGLE);
    }

    public void testCreateResource_P02() {
        SimulatorResource resource = null;
        SimulatorResource.Type resType = SimulatorResource.Type.COLLECTION;

        try {
            resource = SimulatorManager.createResource(COLLECTION_RES_RAML);
            resType = resource.getType();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(resource);
        assertTrue(resType == SimulatorResource.Type.COLLECTION);
    }

    public void testCreateResource_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            String configPath = "";
            SimulatorManager.createResource(configPath);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testCreateResource_N02() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorManager.createResource(null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testCreateResourceWithCount_P01() {
        Vector<SimulatorResource> resources = null;
        SimulatorResource.Type resType = SimulatorResource.Type.SINGLE;

        try {
            resources = SimulatorManager.createResource(SINGLE_RES_RAML, 1);
            resType = resources.elementAt(0).getType();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(resources);
        assertTrue(resources.size() == 1);
        assertTrue(resType == SimulatorResource.Type.SINGLE);
    }

    public void testCreateResourceWithCount_P02() {
        Vector<SimulatorResource> resources = null;
        SimulatorResource.Type resType = SimulatorResource.Type.COLLECTION;

        try {
            resources = SimulatorManager.createResource(COLLECTION_RES_RAML, 1);
            resType = resources.elementAt(0).getType();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(resources);
        assertTrue(resources.size() == 1);
        assertTrue(resType == SimulatorResource.Type.COLLECTION);
    }

    public void testCreateResourceWithCount_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            String configPath = "";
            SimulatorManager.createResource(configPath, 1);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testCreateResourceWithCount_N02() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorManager.createResource(null, 1);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testCreateResourceWithCount_N03() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorManager.createResource(SINGLE_RES_RAML, 0);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testCreateResourceWithCount_N04() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorManager.createResource(SINGLE_RES_RAML, -1);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testCreateResourceByType_P01() {
        SimulatorResource resource = null;
        SimulatorResource.Type resType = SimulatorResource.Type.SINGLE;

        try {
            String name = "test-singleresource";
            String uri = "/test/singleresource";
            String resourceType = "test.singleresource";

            resource = SimulatorManager.createResource(
                    SimulatorResource.Type.SINGLE, name, uri, resourceType);
            resType = resource.getType();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(resource);
        assertTrue(resType == SimulatorResource.Type.SINGLE);
    }

    public void testCreateResourceByType_P02() {
        SimulatorResource resource = null;
        SimulatorResource.Type resType = SimulatorResource.Type.COLLECTION;

        try {
            String name = "test-collectionresource";
            String uri = "/test/collectionresource";
            String resourceType = "test.collectionresource";

            resource = SimulatorManager.createResource(
                    SimulatorResource.Type.COLLECTION, name, uri, resourceType);
            resType = resource.getType();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(resource);
        assertTrue(resType == SimulatorResource.Type.COLLECTION);
    }

    public void testCreateResourceByType_N01() {
        ExceptionType exType = ExceptionType.INVALID_ARGS;

        try {
            String name = "";
            String uri = "/test/resource";
            String resourceType = "test.resource";

            SimulatorManager.createResource(SimulatorResource.Type.SINGLE,
                    name, uri, resourceType);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testCreateResourceByType_N02() {
        ExceptionType exType = ExceptionType.INVALID_ARGS;

        try {
            String name = null;
            String uri = "/test/resource";
            String resourceType = "test.resource";

            SimulatorManager.createResource(SimulatorResource.Type.SINGLE,
                    name, uri, resourceType);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testCreateResourceByType_N03() {
        ExceptionType exType = ExceptionType.INVALID_ARGS;

        try {
            String name = "test-resource";
            String uri = "";
            String resourceType = "test.resource";

            SimulatorManager.createResource(SimulatorResource.Type.SINGLE,
                    name, uri, resourceType);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testCreateResourceByType_N04() {
        ExceptionType exType = ExceptionType.INVALID_ARGS;

        try {
            String name = "test-resource";
            String uri = null;
            String resourceType = "test.resource";

            SimulatorManager.createResource(SimulatorResource.Type.SINGLE,
                    name, uri, resourceType);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testCreateResourceByType_N05() {
        ExceptionType exType = ExceptionType.INVALID_ARGS;

        try {
            String name = "test-resource";
            String uri = "/test/resource";;
            String resourceType = "";

            SimulatorManager.createResource(SimulatorResource.Type.SINGLE,
                    name, uri, resourceType);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testCreateResourceByType_N06() {
        ExceptionType exType = ExceptionType.INVALID_ARGS;

        try {
            String name = "test-resource";
            String uri = "/test/resource";;
            String resourceType = null;

            SimulatorManager.createResource(SimulatorResource.Type.SINGLE,
                    name, uri, resourceType);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testFindResource_P01() {
        // Creating sample single resource to test this API
        SampleSingleResource testResource = new SampleSingleResource();
        if (false == testResource.start())
            return;

        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<SimulatorRemoteResource> resourceHolder = new ObjectHolder<>();
        FindResourceCallbackListener listener = new FindResourceCallbackListener(
                lockObject, resourceHolder);

        try {
            String resourceType = testResource.mResourceType;
            SimulatorManager.findResource(resourceType, listener);
            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        if (false == syncResult)
            testResource.stop();

        assertTrue(syncResult);

        // Wait for the resource to found
        try {
            lockObject.await(10, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
        }

        testResource.stop();
        assertNotNull(resourceHolder.get());
    }

    public void testFindResource_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            String resourceType = "test.singleresource";
            SimulatorManager.findResource(resourceType, null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testFindResource_N02() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            String resourceType = "";
            SimulatorManager.findResource(resourceType,
                    new FindResourceListener() {
                        @Override
                        public void onResourceFound(
                                SimulatorRemoteResource resource) {
                        }
                    });
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testFindResource_N03() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            String resourceType = null;
            SimulatorManager.findResource(resourceType,
                    new FindResourceListener() {
                        @Override
                        public void onResourceFound(
                                SimulatorRemoteResource resource) {
                        }
                    });
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testFindResourceAll_P01() {
        // Creating sample single resource to test this API
        SampleSingleResource testResource = new SampleSingleResource();
        if (false == testResource.start())
            return;

        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<SimulatorRemoteResource> resourceHolder = new ObjectHolder<>();
        FindResourceCallbackListener listener = new FindResourceCallbackListener(
                lockObject, resourceHolder);

        try {
            SimulatorManager.findResource(listener);
            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        if (false == syncResult)
            testResource.stop();

        assertTrue(syncResult);

        // Wait for the resource to found
        try {
            lockObject.await(10, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
        }

        testResource.stop();
        assertNotNull(resourceHolder.get());
    }

    public void testFindResourceAll_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorManager.findResource(null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testregisterDeviceInfo_P01() {
        boolean syncResult = false;

        try {
            String deviceName = "test-device";
            SimulatorManager.setDeviceInfo(deviceName);
            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
    }

    public void testregisterDeviceInfo_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            String deviceName = "";
            SimulatorManager.setDeviceInfo(deviceName);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testregisterDeviceInfo_N02() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            String deviceName = null;
            SimulatorManager.setDeviceInfo(deviceName);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testregisterPlatformInfo_P01() {
        boolean syncResult = false;

        try {
            PlatformInfo platformInfo = new PlatformInfo();
            platformInfo.setManufacturerName("Samsung");

            SimulatorManager.setPlatformInfo(platformInfo);
            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
    }

    public void testregisterPlatformInfo_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            PlatformInfo platformInfo = null;
            SimulatorManager.setPlatformInfo(platformInfo);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testFindDevices_P01() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<DeviceInfo> infoHolder = new ObjectHolder<>();
        DeviceInfoListener listener = new DeviceInfoListener(lockObject,
                infoHolder);

        try {
            SimulatorManager.setDeviceInfo("Samsung");
            SimulatorManager.findDevices(null, listener);
            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);

        // Wait for the asynchronous response
        try {
            lockObject.await(10, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
        }

        assertNotNull(infoHolder.get());
    }

    public void testFindDevices_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorManager.findDevices(null, null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testGetPlatformInfo_P01() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<PlatformInfo> infoHolder = new ObjectHolder<>();
        PlatformInfoListener listener = new PlatformInfoListener(lockObject,
                infoHolder);

        try {
            SimulatorManager.getPlatformInformation(null, listener);
            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);

        // Wait for the asynchronous response
        try {
            lockObject.await(10, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
        }

        assertNotNull(infoHolder.get());
    }

    public void testGetPlatformInfo_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorManager.getPlatformInformation(null, null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }
}

class FindResourceCallbackListener implements FindResourceListener {
    private CountDownLatch                        mLockObject;
    private ObjectHolder<SimulatorRemoteResource> mResourceHolder;

    public FindResourceCallbackListener(CountDownLatch lockObject,
            ObjectHolder<SimulatorRemoteResource> resourceHolder) {
        mLockObject = lockObject;
        mResourceHolder = resourceHolder;
    }

    @Override
    public void onResourceFound(SimulatorRemoteResource resource) {
        mResourceHolder.set(resource);
        mLockObject.countDown();
    }
}

class DeviceInfoListener implements DeviceListener {
    private CountDownLatch           mLockObject;
    private ObjectHolder<DeviceInfo> mInfoHolder;

    public DeviceInfoListener(CountDownLatch lockObject,
            ObjectHolder<DeviceInfo> infoHolder) {
        mLockObject = lockObject;
        mInfoHolder = infoHolder;
    }

    @Override
    public void onDeviceFound(String hostUri, DeviceInfo devInfo) {
        mInfoHolder.set(devInfo);
        mLockObject.countDown();
    }
}

class PlatformInfoListener implements PlatformListener {

    private CountDownLatch             mLockObject;
    private ObjectHolder<PlatformInfo> mInfoHolder;

    public PlatformInfoListener(CountDownLatch lockObject,
            ObjectHolder<PlatformInfo> infoHolder) {
        mLockObject = lockObject;
        mInfoHolder = infoHolder;
    }

    @Override
    public void onPlatformFound(String hostUri, PlatformInfo platformInfo) {
        mInfoHolder.set(platformInfo);
        mLockObject.countDown();
    }
}
