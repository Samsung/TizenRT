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

import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorManager;
import org.oic.simulator.SimulatorResourceModel;
import org.oic.simulator.server.SimulatorCollectionResource;
import org.oic.simulator.server.SimulatorResource;
import org.oic.simulator.test.ExceptionType;

import junit.framework.TestCase;

public class SimulatorCollectionResourceTest extends TestCase {

    private static final String         COLLECTION_RES_RAML = "./ramls/oic.d.airconditioner.raml";
    private SimulatorCollectionResource collectionResource  = null;

    protected void setUp() throws Exception {
        super.setUp();
        collectionResource = (SimulatorCollectionResource) SimulatorManager
                .createResource(COLLECTION_RES_RAML);
    }

    protected void tearDown() throws Exception {
        super.tearDown();
        collectionResource = null;
    }

    public void testAddChildResource_P01() {
        SimulatorResource childResource = null;

        try {
            String name = "child-resource";
            String uri = "/child/resource";
            String resType = "child.resource";

            childResource = SimulatorManager.createResource(
                    SimulatorResource.Type.SINGLE, name, uri, resType);
        } catch (InvalidArgsException e1) {
            e1.printStackTrace();
        } catch (SimulatorException e1) {
            e1.printStackTrace();
        }

        if (null == childResource)
            return;

        Vector<SimulatorResource> result = null;
        try {
            collectionResource.addChildResource(childResource);
            result = collectionResource.getChildResources();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(result);
        assertEquals(1, result.size());
    }

    public void testAddChildResource_P02() {
        SimulatorResource childResource = null;

        try {
            String name = "child-resource";
            String uri = "/child/resource";
            String resType = "child.resource";

            childResource = SimulatorManager.createResource(
                    SimulatorResource.Type.COLLECTION, name, uri, resType);
        } catch (InvalidArgsException e1) {
            e1.printStackTrace();
        } catch (SimulatorException e1) {
            e1.printStackTrace();
        }

        if (null == childResource)
            return;

        Vector<SimulatorResource> result = null;
        try {
            collectionResource.addChildResource(childResource);
            result = collectionResource.getChildResources();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(result);
        assertEquals(1, result.size());
    }

    public void testAddChildResource_P03() {
        SimulatorResource childResource1 = null;
        SimulatorResource childResource2 = null;

        try {
            String name = "child-resource";
            String uri = "/child/resource";
            String resType = "child.resource";

            childResource1 = SimulatorManager.createResource(
                    SimulatorResource.Type.SINGLE, name, uri, resType);
            childResource2 = SimulatorManager.createResource(
                    SimulatorResource.Type.COLLECTION, name + "-2", uri + "/2",
                    resType);
        } catch (InvalidArgsException e1) {
            e1.printStackTrace();
        } catch (SimulatorException e1) {
            e1.printStackTrace();
        }

        if (null == childResource1 || null == childResource2)
            return;

        Vector<SimulatorResource> result = null;
        try {
            collectionResource.addChildResource(childResource1);
            collectionResource.addChildResource(childResource2);
            result = collectionResource.getChildResources();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(result);
        assertEquals(2, result.size());
    }

    public void testAddChildResource_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            collectionResource.addChildResource(null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testRemoveChildResource_P01() {
        SimulatorResource childResource = null;
        Vector<SimulatorResource> result = null;

        try {
            String name = "child-resource";
            String uri = "/child/resource";
            String resType = "child.resource";

            childResource = SimulatorManager.createResource(
                    SimulatorResource.Type.COLLECTION, name, uri, resType);
            collectionResource.addChildResource(childResource);
            result = collectionResource.getChildResources();
        } catch (InvalidArgsException e1) {
            e1.printStackTrace();
        } catch (SimulatorException e1) {
            e1.printStackTrace();
        }

        if (null == childResource || null == result)
            return;

        int count = result.size();
        try {
            collectionResource.removeChildResource(childResource);
            result = collectionResource.getChildResources();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue((count == 1 && result == null)
                || (count > 1 && result.size() == count - 1));
    }

    public void testRemoveChildResource_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            collectionResource.removeChildResource(null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testRemoveChildResourceByUri_P01() {
        SimulatorResource childResource = null;
        Vector<SimulatorResource> result = null;

        try {
            String name = "child-resource";
            String uri = "/child/resource";
            String resType = "child.resource";

            childResource = SimulatorManager.createResource(
                    SimulatorResource.Type.COLLECTION, name, uri, resType);
            collectionResource.addChildResource(childResource);
            result = collectionResource.getChildResources();
        } catch (InvalidArgsException e1) {
            e1.printStackTrace();
        } catch (SimulatorException e1) {
            e1.printStackTrace();
        }

        if (null == childResource || null == result)
            return;

        int count = result.size();
        try {
            collectionResource.removeChildResourceByUri(childResource.getURI());
            result = collectionResource.getChildResources();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue((count == 1 && result == null)
                || (count > 1 && result.size() == count - 1));
    }

    public void testRemoveChildResourceByUri_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            collectionResource.removeChildResourceByUri(null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testRemoveChildResourceByUri_N02() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            String uri = "";
            collectionResource.removeChildResourceByUri(uri);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testGetResourceModel_P01() {
        SimulatorResourceModel result = null;

        try {
            result = collectionResource.getResourceModel();
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(result);
        assertTrue(result.size() > 0);
    }
}
