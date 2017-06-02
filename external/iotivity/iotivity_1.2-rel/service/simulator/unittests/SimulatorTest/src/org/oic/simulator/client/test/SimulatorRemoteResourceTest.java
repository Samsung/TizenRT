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

package org.oic.simulator.client.test;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import org.oic.simulator.AttributeValue;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorManager;
import org.oic.simulator.SimulatorResourceAttribute;
import org.oic.simulator.SimulatorResourceModel;
import org.oic.simulator.SimulatorResult;
import org.oic.simulator.client.FindResourceListener;
import org.oic.simulator.client.SimulatorRemoteResource;
import org.oic.simulator.server.SimulatorSingleResource;
import org.oic.simulator.test.ExceptionType;
import org.oic.simulator.utils.ObjectHolder;

import junit.framework.TestCase;

/**
 * This class tests the APIs of SimulatorRemoteResource class.
 */
public class SimulatorRemoteResourceTest extends TestCase {
    private static final String            SINGLE_RES_RAML = "./ramls/oic.r.light.raml";
    private static SimulatorSingleResource singleResource  = null;
    private static SimulatorRemoteResource remoteResource  = null;

    protected void setUp() throws Exception {
        super.setUp();

        // Create single resource for first time
        if (null == singleResource) {
            singleResource = (SimulatorSingleResource) SimulatorManager
                    .createResource(SINGLE_RES_RAML);
            singleResource.start();
        }

        // Find the created resource for first time
        if (null != singleResource && null == remoteResource) {
            CountDownLatch lockObject = new CountDownLatch(1);
            ObjectHolder<SimulatorRemoteResource> resourceHolder = new ObjectHolder<>();
            FindResourceCallbackListener listener = new FindResourceCallbackListener(
                    lockObject, resourceHolder);

            try {
                SimulatorManager.findResource(singleResource.getResourceType(),
                        listener);

                try {
                    lockObject.await(10, TimeUnit.SECONDS);
                } catch (InterruptedException e) {
                }

            } catch (InvalidArgsException e) {
                e.printStackTrace();
            } catch (SimulatorException e) {
                e.printStackTrace();
            }

            remoteResource = resourceHolder.get();
        }
    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }

    public void testGetUri_P01() {
        String serverURI = null;
        try {
            serverURI = singleResource.getURI();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertNotNull(remoteResource.getURI());
        assertEquals(serverURI, remoteResource.getURI());
    }

    public void testGetConnectivityType_P01() {
        assertNotNull(remoteResource.getConnectivityType());
    }

    public void testGetResourceTypes_P01() {
        assertNotNull(remoteResource.getResourceTypes());
        assertTrue(remoteResource.getResourceTypes().size() > 0);
    }

    public void testGetResourceInterfaces_P01() {
        assertNotNull(remoteResource.getResourceInterfaces());
        assertTrue(remoteResource.getResourceInterfaces().size() > 0);
    }

    public void testGetHost_P01() {
        assertNotNull(remoteResource.getHost());
    }

    public void testGetId_P01() {
        assertNotNull(remoteResource.getId());
    }

    public void testIsObservable_P01() {
        boolean serverObserveState = false;
        try {
            serverObserveState = singleResource.isObservable();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertEquals(serverObserveState, remoteResource.isObservable());
    }

    public void testGet_P01() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        GetResponseCallbackListener listener = new GetResponseCallbackListener(
                lockObject, response);

        try {
            remoteResource.get(null, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testGet_P02() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        GetResponseCallbackListener listener = new GetResponseCallbackListener(
                lockObject, response);

        try {
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.get(queryParams, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testGet_P03() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        GetResponseCallbackListener listener = new GetResponseCallbackListener(
                lockObject, response);

        try {
            remoteResource.get(remoteResource.getResourceInterfaces().get(0),
                    null, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testGet_P04() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        GetResponseCallbackListener listener = new GetResponseCallbackListener(
                lockObject, response);

        try {
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.get(remoteResource.getResourceInterfaces().get(0),
                    queryParams, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testGet_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            remoteResource.get(null, null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testGet_N02() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            remoteResource.get(remoteResource.getResourceInterfaces().get(0),
                    null, null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testGet_N03() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.get(null, queryParams,
                    new SimulatorRemoteResource.GetResponseListener() {
                        @Override
                        public void onGetResponse(String uid,
                                SimulatorResult result,
                                SimulatorResourceModel resourceModel) {
                        }
                    });
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testPut_P01() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        PutResponseCallbackListener listener = new PutResponseCallbackListener(
                lockObject, response);

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            remoteResource.put(null, resModel, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertNotNull(response.get().resourceModel());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testPut_P02() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        PutResponseCallbackListener listener = new PutResponseCallbackListener(
                lockObject, response);

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.put(queryParams, resModel, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertNotNull(response.get().resourceModel());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testPut_P03() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        PutResponseCallbackListener listener = new PutResponseCallbackListener(
                lockObject, response);

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            remoteResource.put(remoteResource.getResourceInterfaces().get(0),
                    null, resModel, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertNotNull(response.get().resourceModel());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testPut_P05() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        PutResponseCallbackListener listener = new PutResponseCallbackListener(
                lockObject, response);

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.put(remoteResource.getResourceInterfaces().get(0),
                    queryParams, resModel, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertNotNull(response.get().resourceModel());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testPut_P04() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        PutResponseCallbackListener listener = new PutResponseCallbackListener(
                lockObject, response);

        try {
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.put(queryParams, null, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertNotNull(response.get().resourceModel());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testPut_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            remoteResource.put(null, resModel, null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testPut_N02() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            remoteResource.put(remoteResource.getResourceInterfaces().get(0),
                    null, resModel, null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testPut_N03() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.put(null, queryParams, resModel,
                    new SimulatorRemoteResource.PutResponseListener() {
                        @Override
                        public void onPutResponse(String uid,
                                SimulatorResult result,
                                SimulatorResourceModel resourceModel) {
                        }
                    });
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testPost_P01() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        PostResponseCallbackListener listener = new PostResponseCallbackListener(
                lockObject, response);

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            remoteResource.post(null, resModel, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertNotNull(response.get().resourceModel());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testPost_P02() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        PostResponseCallbackListener listener = new PostResponseCallbackListener(
                lockObject, response);

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.post(queryParams, resModel, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertNotNull(response.get().resourceModel());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testPost_P03() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        PostResponseCallbackListener listener = new PostResponseCallbackListener(
                lockObject, response);

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            remoteResource.post(remoteResource.getResourceInterfaces().get(0),
                    null, resModel, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertNotNull(response.get().resourceModel());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testPost_P04() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        PostResponseCallbackListener listener = new PostResponseCallbackListener(
                lockObject, response);

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.post(remoteResource.getResourceInterfaces().get(0),
                    queryParams, resModel, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertNotNull(response.get().resourceModel());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testPost_P05() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        PostResponseCallbackListener listener = new PostResponseCallbackListener(
                lockObject, response);

        try {
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.post(queryParams, null, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertNotNull(response.get().resourceModel());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testPost_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            remoteResource.post(null, resModel, null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testPost_N02() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            remoteResource.post(remoteResource.getResourceInterfaces().get(0),
                    null, resModel, null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testPost_N03() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            SimulatorResourceModel resModel = singleResource.getResourceModel();
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.post(null, queryParams, resModel,
                    new SimulatorRemoteResource.PostResponseListener() {
                        @Override
                        public void onPostResponse(String uid,
                                SimulatorResult result,
                                SimulatorResourceModel resourceModel) {
                        }
                    });
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testStartObserve_P01() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        ObserveNotificationCallbackListener listener = new ObserveNotificationCallbackListener(
                lockObject, response);

        try {
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.observe(queryParams, listener);
            singleResource.addAttribute(new SimulatorResourceAttribute(
                    "boolean", new AttributeValue(true), null));
            singleResource.removeAttribute("boolean");

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            remoteResource.stopObserve();
            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertNotNull(response.get().resourceModel());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testStartObserve_P02() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        ObserveNotificationCallbackListener listener = new ObserveNotificationCallbackListener(
                lockObject, response);

        try {
            remoteResource.observe(listener);
            singleResource.addAttribute(new SimulatorResourceAttribute(
                    "boolean", new AttributeValue(true), null));
            singleResource.removeAttribute("boolean");

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            remoteResource.stopObserve();
            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertNotNull(response.get().resourceModel());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testStartObserve_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.observe(queryParams, null);

        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testStopObserve_P01() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        ObserveNotificationCallbackListener listener = new ObserveNotificationCallbackListener(
                lockObject, response);

        try {
            Map<String, String> queryParams = new HashMap<>();
            remoteResource.observe(queryParams, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            response.set(null);
            remoteResource.stopObserve();
            singleResource.addAttribute(new SimulatorResourceAttribute(
                    "boolean", new AttributeValue(true), null));
            singleResource.removeAttribute("boolean");

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNull(response.get());
    }

    public void testSetConfigInfo_P01() {
        boolean syncResult = false;
        try {
            remoteResource.setConfigInfo(SINGLE_RES_RAML);
            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
    }

    public void testSetConfigInfo_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            remoteResource.setConfigInfo("");
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testSetConfigInfo_N02() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            remoteResource.setConfigInfo(null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testStartVerification_P01() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        VerificationCallbackListener listener = new VerificationCallbackListener(
                lockObject, response);

        try {
            remoteResource.setConfigInfo(SINGLE_RES_RAML);
            remoteResource.startVerification(
                    SimulatorRemoteResource.RequestType.GET, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testStartVerification_P02() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        VerificationCallbackListener listener = new VerificationCallbackListener(
                lockObject, response);

        try {
            remoteResource.setConfigInfo(SINGLE_RES_RAML);
            remoteResource.startVerification(
                    SimulatorRemoteResource.RequestType.PUT, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testStartVerification_P03() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        VerificationCallbackListener listener = new VerificationCallbackListener(
                lockObject, response);

        try {
            remoteResource.setConfigInfo(SINGLE_RES_RAML);
            remoteResource.startVerification(
                    SimulatorRemoteResource.RequestType.POST, listener);

            try {
                lockObject.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
            }

            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
        assertNotNull(response.get());
        assertEquals(remoteResource.getId(), response.get().uid());
    }

    public void testStartVerification_N01() {
        ExceptionType exType = ExceptionType.UNKNOWN;

        try {
            remoteResource.setConfigInfo(SINGLE_RES_RAML);
            remoteResource.startVerification(
                    SimulatorRemoteResource.RequestType.GET, null);
        } catch (InvalidArgsException e) {
            exType = ExceptionType.INVALID_ARGS;
        } catch (SimulatorException e) {
            exType = ExceptionType.SIMULATOR;
        }

        assertTrue(exType == ExceptionType.INVALID_ARGS);
    }

    public void testStopVerification_N01() {
        CountDownLatch lockObject = new CountDownLatch(1);
        boolean syncResult = false;
        ObjectHolder<ResponseDetails> response = new ObjectHolder<>();
        VerificationCallbackListener listener = new VerificationCallbackListener(
                lockObject, response);

        try {
            remoteResource.setConfigInfo(SINGLE_RES_RAML);
            int id = remoteResource.startVerification(
                    SimulatorRemoteResource.RequestType.POST, listener);
            remoteResource.stopVerification(id);
            syncResult = true;
        } catch (InvalidArgsException e) {
            e.printStackTrace();
        } catch (SimulatorException e) {
            e.printStackTrace();
        }

        assertTrue(syncResult);
    }
}

class ResponseDetails {
    private String                 mUid      = null;
    private SimulatorResourceModel mResModel = null;
    private SimulatorResult        mResult   = SimulatorResult.SIMULATOR_ERROR;

    ResponseDetails(String uid, SimulatorResult result,
            SimulatorResourceModel resModel) {
        mUid = uid;
        mResModel = resModel;
        mResult = result;
    }

    String uid() {
        return mUid;
    }

    SimulatorResourceModel resourceModel() {
        return mResModel;
    }

    SimulatorResult errorCode() {
        return mResult;
    }
}

class GetResponseCallbackListener
        implements SimulatorRemoteResource.GetResponseListener {
    private CountDownLatch                mLockObject;
    private ObjectHolder<ResponseDetails> mResponse;

    public GetResponseCallbackListener(CountDownLatch lockObject,
            ObjectHolder<ResponseDetails> response) {
        mLockObject = lockObject;
        mResponse = response;
    }

    @Override
    public void onGetResponse(String uid, SimulatorResult result,
            SimulatorResourceModel resourceModel) {
        mResponse.set(new ResponseDetails(uid, result, resourceModel));
        mLockObject.countDown();
    }
}

class PutResponseCallbackListener
        implements SimulatorRemoteResource.PutResponseListener {
    private CountDownLatch                mLockObject;
    private ObjectHolder<ResponseDetails> mResponse;

    public PutResponseCallbackListener(CountDownLatch lockObject,
            ObjectHolder<ResponseDetails> response) {
        mLockObject = lockObject;
        mResponse = response;
    }

    @Override
    public void onPutResponse(String uid, SimulatorResult result,
            SimulatorResourceModel resourceModel) {
        mResponse.set(new ResponseDetails(uid, result, resourceModel));
        mLockObject.countDown();
    }
}

class PostResponseCallbackListener
        implements SimulatorRemoteResource.PostResponseListener {
    private CountDownLatch                mLockObject;
    private ObjectHolder<ResponseDetails> mResponse;

    public PostResponseCallbackListener(CountDownLatch lockObject,
            ObjectHolder<ResponseDetails> response) {
        mLockObject = lockObject;
        mResponse = response;
    }

    @Override
    public void onPostResponse(String uid, SimulatorResult result,
            SimulatorResourceModel resourceModel) {
        mResponse.set(new ResponseDetails(uid, result, resourceModel));
        mLockObject.countDown();
    }
}

class ObserveNotificationCallbackListener
        implements SimulatorRemoteResource.ObserveNotificationListener {
    private CountDownLatch                mLockObject;
    private ObjectHolder<ResponseDetails> mResponse;

    public ObserveNotificationCallbackListener(CountDownLatch lockObject,
            ObjectHolder<ResponseDetails> response) {
        mLockObject = lockObject;
        mResponse = response;
    }

    @Override
    public void onObserveNotification(String uid,
            SimulatorResourceModel resourceModel, int sequenceNumber) {
        mResponse.set(new ResponseDetails(uid, SimulatorResult.SIMULATOR_OK,
                resourceModel));
        mLockObject.countDown();
    }
}

class VerificationCallbackListener
        implements SimulatorRemoteResource.VerificationListener {
    private CountDownLatch                mLockObject;
    private ObjectHolder<ResponseDetails> mResponse;

    public VerificationCallbackListener(CountDownLatch lockObject,
            ObjectHolder<ResponseDetails> response) {
        mLockObject = lockObject;
        mResponse = response;
    }

    @Override
    public void onVerificationStarted(String uid, int id) {
        mResponse.set(
                new ResponseDetails(uid, SimulatorResult.SIMULATOR_OK, null));
        mLockObject.countDown();
    }

    @Override
    public void onVerificationAborted(String uid, int id) {
        mResponse.set(
                new ResponseDetails(uid, SimulatorResult.SIMULATOR_OK, null));
        mLockObject.countDown();
    }

    @Override
    public void onVerificationCompleted(String uid, int id) {
        mResponse.set(
                new ResponseDetails(uid, SimulatorResult.SIMULATOR_OK, null));
        mLockObject.countDown();
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