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

package org.oic.simulator.server.test;

import java.util.Vector;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.NoSupportException;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorManager;
import org.oic.simulator.SimulatorResourceModel;
import org.oic.simulator.client.FindResourceListener;
import org.oic.simulator.client.SimulatorRemoteResource;
import org.oic.simulator.server.SimulatorResource;
import org.oic.simulator.server.SimulatorSingleResource;
import org.oic.simulator.test.ExceptionType;
import org.oic.simulator.utils.ObjectHolder;

import junit.framework.TestCase;

/**
 * This class tests the APIs of SimulatorResource class
 */
public class SimulatorResourceTest extends TestCase {
    private static final String RES_NAME = "test-resource";
    private static final String RES_URI  = "/test/resource";
    private static final String RES_TYPE = "test.resource";
    private SimulatorResource   resource = null;

    protected void setUp() throws Exception {
        super.setUp();
        resource = SimulatorManager.createResource(
                SimulatorResource.Type.SINGLE, RES_NAME, RES_URI, RES_TYPE);
    }

    protected void tearDown() throws Exception {
        super.tearDown();
        resource = null;
    }

    public void testIsCollection_P01() {
        boolean isCollection = false;
        isCollection = resource.isCollection();
        assertFalse(isCollection);
    }

    public void testIsCollection_P02() {
        boolean isCollection = true;

        try {
            SimulatorResource res = SimulatorManager.createResource(
                    SimulatorResource.Type.COLLECTION, RES_NAME, RES_URI,
                    RES_TYPE);
            isCollection = res.isCollection();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(isCollection);
    }

    public void testGetName_P01() {
        String name = null;

        try {
            name = resource.getName();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(RES_NAME, name);
    }

    public void testGetType_P01() {
        SimulatorResource.Type type = SimulatorResource.Type.SINGLE;
        type = resource.getType();
        assertEquals(SimulatorResource.Type.SINGLE, type);
    }

    public void testGetURI_P01() {
        String uri = null;

        try {
            uri = resource.getURI();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(RES_URI, uri);
    }

    public void testGetResourceType_P01() {
        String resType = null;

        try {
            resType = resource.getResourceType();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(RES_TYPE, resType);
    }

    public void testGetInterface_P01() {
        Vector<String> interfaces = null;

        try {
            interfaces = resource.getInterface();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(interfaces);
        assertTrue(interfaces.size() > 0);
    }

    public void testSetName_P01() {
        String name = "new-name";
        String newName = null;

        try {
            resource.setName(name);
            newName = resource.getName();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(name, newName);
    }

    public void testSetName_N01() {
        String newName = "";
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.setName(newName);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testSetName_N02() {
        String newName = null;
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.setName(newName);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testSetURI_P01() {
        String newUri = "/test/newuri/1";
        String uri = null;

        try {
            resource.setURI(newUri);
            uri = resource.getURI();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(newUri, uri);
    }

    public void testSetURI_N01() {
        String newUri = "";
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.setURI(newUri);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testSetURI_N02() {
        String newUri = null;
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.setURI(newUri);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testSetResourceType_P01() {
        String newResType = "test.newresource";
        String resType = null;

        try {
            resource.setResourceType(newResType);
            resType = resource.getResourceType();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(newResType, resType);
    }

    public void testSetResourceType_N01() {
        String newResType = "";
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.setResourceType(newResType);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testSetResourceType_N02() {
        String newResType = null;
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.setResourceType(newResType);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testSetInterface_P01() {
        String newInterface = "oic.if.r";
        Vector<String> interfaces = null;

        try {
            resource.setInterface(newInterface);
            interfaces = resource.getInterface();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(interfaces);
        assertTrue(interfaces.size() == 1
                && interfaces.get(0).equals(newInterface));
    }

    public void testSetInterface_N01() {
        String newInterface = "";
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.setInterface(newInterface);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testSetInterface_N02() {
        String newInterface = null;
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.setInterface(newInterface);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testSetInterface_N03() {
        String newInterface = "oic.if.s";
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.start();
            resource.setInterface(newInterface);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        try {
            resource.stop();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(ExceptionType.SIMULATOR, exType);
    }

    public void testSetInterfaces_P01() {
        String interface1 = "oic.if.s";
        String interface2 = "oic.if.a";
        Vector<String> interfaces = null;
        Vector<String> newInterfaces = new Vector<String>();
        newInterfaces.add(interface1);
        newInterfaces.add(interface2);
        try {
            resource.setInterface(newInterfaces);
            interfaces = resource.getInterface();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(interfaces);
        assertTrue(interfaces.size() == 2 && interfaces.contains(interface1)
                && interfaces.contains(interface2));
    }

    public void testSetInterfaces_N01() {
        Vector<String> newInterfaces = new Vector<String>();
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.setInterface(newInterfaces);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testSetInterfaces_N02() {
        Vector<String> newInterfaces = null;
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.setInterface(newInterfaces);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testSetInterfaces_N03() {
        String interface1 = "oic.if.s";
        String interface2 = "oic.if.a";
        ExceptionType exType = ExceptionType.UNKNOWN;
        Vector<String> newInterfaces = new Vector<String>();
        newInterfaces.add(interface1);
        newInterfaces.add(interface2);

        try {
            resource.start();
            resource.setInterface(newInterfaces);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        try {
            resource.stop();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(ExceptionType.SIMULATOR, exType);
    }

    public void testAddInterface_P01() {
        String newInterface = "oic.if.rw";
        Vector<String> interfaces = null;

        try {
            resource.addInterface(newInterface);
            interfaces = resource.getInterface();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (NoSupportException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(interfaces);
        assertTrue(interfaces.contains(newInterface));
    }

    public void testAddInterface_N01() {
        String newInterface = "";
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.addInterface(newInterface);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (NoSupportException e) {
            exType = ExceptionType.NOT_SUPPORTED;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testAddInterface_N02() {
        String newInterface = null;
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            resource.addInterface(newInterface);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (NoSupportException e) {
            exType = ExceptionType.NOT_SUPPORTED;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.INVALID_ARGS, exType);
    }

    public void testAddInterface_N03() {
        String newInterface = "oic.if.a";
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorResource res = SimulatorManager.createResource(
                    SimulatorResource.Type.COLLECTION, RES_NAME, RES_URI,
                    RES_TYPE);
            res.addInterface(newInterface);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (NoSupportException e) {
            exType = ExceptionType.NOT_SUPPORTED;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertEquals(ExceptionType.NOT_SUPPORTED, exType);
    }

    public void testSetObservable_P01() {
        boolean newState = true;
        boolean state = false;
        try {
            resource.setObservable(newState);
            state = resource.isObservable();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(newState, state);
    }

    public void testSetObservable_P02() {
        boolean newState = false;
        boolean state = true;
        try {
            resource.setObservable(newState);
            state = resource.isObservable();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(newState, state);
    }

    public void testSetDiscoverable_P01() {
        boolean newState = true;
        boolean state = false;
        try {
            resource.setDiscoverable(newState);
            state = resource.isDiscoverable();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(newState, state);
    }

    public void testSetDiscoverable_P02() {
        boolean newState = false;
        boolean state = true;
        try {
            resource.setDiscoverable(newState);
            state = resource.isDiscoverable();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(newState, state);
    }

    public void testIsObservable_P01() {
        boolean state = false;

        try {
            state = resource.isObservable();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(true, state);
    }

    public void testIsDiscoverable_P01() {
        boolean state = false;

        try {
            state = resource.isDiscoverable();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(true, state);
    }

    public void testGetResourceModel_P01() {
        SimulatorResourceModel resModel = null;
        String singleResourceRaml = "./ramls/oic.r.light.raml";
        try {
            SimulatorSingleResource resource = (SimulatorSingleResource) SimulatorManager
                    .createResource(singleResourceRaml);
            resModel = resource.getResourceModel();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(resModel);
        assertTrue(resModel.size() > 0);
    }

    public void testIsStarted_P01() {
        boolean state = true;

        try {
            state = resource.isStarted();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(false, state);
    }

    public void testIsStarted_P02() {
        boolean state = false;

        try {
            resource.start();
            state = resource.isStarted();
            resource.stop();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(true, state);
    }

    public void testIsStarted_P03() {
        boolean state = true;

        try {
            resource.start();
            resource.stop();
            state = resource.isStarted();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(false, state);
    }

    public void testStart_P01() {
        CountDownLatch lockObject = new CountDownLatch(1);
        ObjectHolder<SimulatorRemoteResource> resourceHolder = new ObjectHolder<>();
        FindResourceCallbackListener listener = new FindResourceCallbackListener(
                lockObject, resourceHolder);

        try {
            resource.start();
            SimulatorManager.findResource(resource.getResourceType(), listener);

            // Wait for the resource to found
            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        try {
            resource.stop();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(resourceHolder.get());
    }

    public void testStop_P01() {
        CountDownLatch lockObject = new CountDownLatch(1);
        ObjectHolder<SimulatorRemoteResource> resourceHolder = new ObjectHolder<>();
        FindResourceCallbackListener listener = new FindResourceCallbackListener(
                lockObject, resourceHolder);

        try {
            resource.start();
            resource.stop();
            SimulatorManager.findResource(resource.getResourceType(), listener);

            // Wait for the resource to found
            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        try {
            resource.stop();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNull(resourceHolder.get());
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