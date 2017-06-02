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

package oic.simulator.clientcontroller.manager;

import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Vector;

import org.oic.simulator.ArrayProperty;
import org.oic.simulator.AttributeProperty;
import org.oic.simulator.AttributeProperty.Type;
import org.oic.simulator.AttributeValue;
import org.oic.simulator.AttributeValue.TypeInfo;
import org.oic.simulator.AttributeValue.ValueType;
import org.oic.simulator.BooleanProperty;
import org.oic.simulator.DeviceInfo;
import org.oic.simulator.DeviceListener;
import org.oic.simulator.DoubleProperty;
import org.oic.simulator.ILogger.Level;
import org.oic.simulator.IntegerProperty;
import org.oic.simulator.PlatformInfo;
import org.oic.simulator.PlatformListener;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorManager;
import org.oic.simulator.SimulatorResourceAttribute;
import org.oic.simulator.SimulatorResourceModel;
import org.oic.simulator.SimulatorResult;
import org.oic.simulator.StringProperty;
import org.oic.simulator.client.FindResourceListener;
import org.oic.simulator.client.SimulatorRemoteResource;
import org.oic.simulator.client.SimulatorRemoteResource.GetResponseListener;
import org.oic.simulator.client.SimulatorRemoteResource.ObserveNotificationListener;
import org.oic.simulator.client.SimulatorRemoteResource.PostResponseListener;
import org.oic.simulator.client.SimulatorRemoteResource.PutResponseListener;
import org.oic.simulator.client.SimulatorRemoteResource.RequestType;
import org.oic.simulator.client.SimulatorRemoteResource.VerificationListener;
import org.oic.simulator.client.SimulatorRequestModel;

import oic.simulator.clientcontroller.Activator;
import oic.simulator.clientcontroller.remoteresource.DeviceAndPlatformInfo;
import oic.simulator.clientcontroller.remoteresource.MetaProperty;
import oic.simulator.clientcontroller.remoteresource.RemoteResource;
import oic.simulator.clientcontroller.utils.AttributeValueStringConverter;
import oic.simulator.clientcontroller.utils.Constants;
import oic.simulator.clientcontroller.utils.Utility;

/**
 * This class acts as an interface between the simulator java SDK and the
 * various UI modules. It maintains all the details of resources and provides
 * other UI modules with the information required. It also handles responses for
 * find, GET, PUT, POST, Observe and automatic verification operations from
 * native layer and propagates those events to the registered UI listeners.
 */
public class ResourceManager {

    private Set<String>                        lastKnownSearchTypes;

    private RemoteResource                     currentResourceInSelection;

    private FindResourceListener               findResourceListener;
    private GetResponseListener                getListener;
    private PutResponseListener                putListener;
    private PostResponseListener               postListener;
    private ObserveNotificationListener        observeListener;
    private VerificationListener               verifyListener;
    private DeviceListener                     deviceListener;
    private PlatformListener                   platformListener;

    private ResponseSynchronizerThread         synchronizerThread;

    private Thread                             threadHandle;

    // Map with Server ID as key and the complete object as the value
    private Map<String, RemoteResource>        resourceMap;
    private List<RemoteResource>               favoriteResources;
    // Maintaining a list of resource URIs for favorite resources feature.
    private List<String>                       favoriteURIList;

    // Maintaining a list of observed resource URIs.
    private List<String>                       observedResourceURIList;

    private Map<String, DeviceAndPlatformInfo> hostDeviceAndPlatformMap;

    public ResourceManager() {
        resourceMap = new HashMap<String, RemoteResource>();
        favoriteResources = new ArrayList<RemoteResource>();
        favoriteURIList = new ArrayList<String>();
        observedResourceURIList = new ArrayList<String>();
        hostDeviceAndPlatformMap = new HashMap<String, DeviceAndPlatformInfo>();

        findResourceListener = new FindResourceListener() {

            @Override
            public void onResourceFound(final SimulatorRemoteResource resourceN) {
                synchronizerThread.addToQueue(new Runnable() {
                    @Override
                    public void run() {
                        if (null == resourceN) {
                            return;
                        }

                        // Ignore the response if the resource is a device or
                        // platform.
                        Vector<String> resTypes = resourceN.getResourceTypes();
                        if (null != resTypes
                                && (resTypes.contains("oic.wk.d") || resTypes
                                        .contains("oic.wk.p"))) {
                            return;
                        }

                        // If id is not available, then it cannot be added to
                        // the local map as null value should not be allowed as
                        // key.
                        String uid = resourceN.getId();
                        if (null == uid) {
                            return;
                        }

                        // If resource already exist, then ignore it.
                        boolean exist = isUidExist(uid);
                        if (exist) {
                            handleExistingResource(resourceN);
                            return;
                        }

                        RemoteResource resource = new RemoteResource();
                        resource.setRemoteResourceRef(resourceN);

                        boolean observeRequestSent = false;

                        String uri = resourceN.getURI();
                        if (null != uri && uri.trim().length() > 0) {
                            // Add resource to favorite list if it was in
                            // favorites list during find/refresh operation.
                            synchronized (favoriteURIList) {
                                if (favoriteURIList.contains(uri)) {
                                    addResourcetoFavorites(resource);
                                }
                            } // Add resource to observed resources list if it
                              // was
                              // in observe list during find/refresh operation.
                            synchronized (observedResourceURIList) {
                                if (observedResourceURIList.contains(uri)) {
                                    observeRequestSent = sendObserveRequest(resource);
                                }
                            }
                        } else {
                            Activator
                                    .getDefault()
                                    .getLogManager()
                                    .log(Level.INFO.ordinal(), new Date(),
                                            "Found a resource without URI. Ignoring it.");
                            return;
                        }

                        // Add the resource in local data structure
                        addResourceDetails(resource);

                        // Notify the UI listener
                        UiListenerHandler.getInstance()
                                .newResourceFoundNotification(resource);

                        Activator
                                .getDefault()
                                .getLogManager()
                                .log(Level.INFO.ordinal(),
                                        new Date(),
                                        "Resource Found [" + resourceN.getURI()
                                                + "].");

                        // Send an initial GET request(If observe request has
                        // not been sent already) to get the resource
                        // attributes on an interface supported by the resource.
                        if (!observeRequestSent) {
                            try {
                                String ifType = null;
                                Vector<String> resInterfaces = resourceN
                                        .getResourceInterfaces();
                                if (null != resInterfaces) {
                                    ifType = resInterfaces.get(0);
                                }
                                resourceN.get(
                                        formQueryParameters(ifType, null),
                                        getListener);
                            } catch (SimulatorException e) {
                                Activator
                                        .getDefault()
                                        .getLogManager()
                                        .log(Level.ERROR.ordinal(),
                                                new Date(),
                                                Utility.getSimulatorErrorString(
                                                        e, null));
                            }
                        }

                        // Get the device information
                        if (!isDeviceInfoExist(resourceN.getHost())) {
                            try {
                                SimulatorManager.findDevices(resource
                                        .getRemoteResourceRef().getHost(),
                                        deviceListener);
                            } catch (SimulatorException e) {
                                Activator
                                        .getDefault()
                                        .getLogManager()
                                        .log(Level.ERROR.ordinal(),
                                                new Date(),
                                                Utility.getSimulatorErrorString(
                                                        e, null));
                            }
                        }

                        // Get the platform information
                        if (!isPlatformInfoExist(resourceN.getHost())) {
                            try {
                                SimulatorManager.getPlatformInformation(
                                        resource.getRemoteResourceRef()
                                                .getHost(), platformListener);
                            } catch (SimulatorException e) {
                                Activator
                                        .getDefault()
                                        .getLogManager()
                                        .log(Level.ERROR.ordinal(),
                                                new Date(),
                                                Utility.getSimulatorErrorString(
                                                        e, null));
                            }
                        }
                    }
                });
            }
        };

        // Listeners for device and platform information.
        deviceListener = new DeviceListener() {

            @Override
            public void onDeviceFound(final String host,
                    final DeviceInfo deviceInfo) {
                if (null == deviceInfo || null == host) {
                    return;
                }
                synchronizerThread.addToQueue(new Runnable() {
                    @Override
                    public void run() {
                        synchronized (hostDeviceAndPlatformMap) {
                            DeviceAndPlatformInfo info = hostDeviceAndPlatformMap
                                    .get(host);
                            if (null == info) {
                                info = new DeviceAndPlatformInfo();
                                info.setHost(host);
                                hostDeviceAndPlatformMap.put(host, info);
                            }
                            info.setDeviceInfo(deviceInfo);
                        }

                        // Notify UI listeners
                        UiListenerHandler.getInstance()
                                .deviceInfoReceivedNotification();
                    }
                });
            }
        };

        platformListener = new PlatformListener() {

            @Override
            public void onPlatformFound(final String host,
                    final PlatformInfo platformInfo) {
                if (null == platformInfo || null == host) {
                    return;
                }
                synchronizerThread.addToQueue(new Runnable() {
                    @Override
                    public void run() {
                        synchronized (hostDeviceAndPlatformMap) {
                            DeviceAndPlatformInfo info = hostDeviceAndPlatformMap
                                    .get(host);
                            if (null == info) {
                                info = new DeviceAndPlatformInfo();
                                info.setHost(host);
                                hostDeviceAndPlatformMap.put(host, info);
                            }
                            info.setPlatformInfo(platformInfo);
                        }

                        // Notify UI listeners
                        UiListenerHandler.getInstance()
                                .platformInfoReceivedNotification();
                    }
                });
            }
        };

        getListener = new GetResponseListener() {
            @Override
            public void onGetResponse(final String uid,
                    final SimulatorResult result,
                    final SimulatorResourceModel resourceModelN) {
                if (result.ordinal() > SimulatorResult.SIMULATOR_RESOURCE_CHANGED.ordinal()) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(),
                                    new Date(),
                                    "["
                                            + result.toString()
                                            + "] Received error response for GET request.");
                    return;
                }
                synchronizerThread.addToQueue(new Runnable() {
                    @Override
                    public void run() {
                        // Handling the response which includes retrieving the
                        // attributes and updating the local model.
                        RemoteResource resource = handleResponse(uid,
                                resourceModelN);
                        if (null != resource) {
                            // Notify the UI listeners
                            UiListenerHandler.getInstance()
                                    .getCompleteNotification(resource);
                        }
                    }
                });
            }
        };

        putListener = new PutResponseListener() {

            @Override
            public void onPutResponse(final String uid,
                    final SimulatorResult result,
                    final SimulatorResourceModel resourceModelN) {
                if (result.ordinal() > SimulatorResult.SIMULATOR_RESOURCE_CHANGED.ordinal()) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(),
                                    new Date(),
                                    "["
                                            + result.toString()
                                            + "] Received error response for PUT request.");
                    return;
                }
                synchronizerThread.addToQueue(new Thread() {
                    @Override
                    public void run() {
                        // Handling the response which includes retrieving the
                        // attributes and updating the local model.
                        RemoteResource resource = handleResponse(uid,
                                resourceModelN);
                        if (null != resource) {
                            // Notify the UI listeners
                            UiListenerHandler.getInstance()
                                    .putCompleteNotification(resource);
                        }
                    }
                });
            }
        };

        postListener = new PostResponseListener() {
            @Override
            public void onPostResponse(final String uid,
                    final SimulatorResult result,
                    final SimulatorResourceModel resourceModelN) {
                if (result.ordinal() > SimulatorResult.SIMULATOR_RESOURCE_CHANGED.ordinal()) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(),
                                    new Date(),
                                    "["
                                            + result.toString()
                                            + "] Received error response for POST request.");
                    return;
                }
                synchronizerThread.addToQueue(new Runnable() {
                    @Override
                    public void run() {
                        // Handling the response which includes retrieving the
                        // attributes and updating the local model.
                        RemoteResource resource = handleResponse(uid,
                                resourceModelN);
                        if (null != resource) {
                            // Notify the UI listeners
                            UiListenerHandler.getInstance()
                                    .postCompleteNotification(resource);
                        }
                    }
                });
            }
        };

        observeListener = new ObserveNotificationListener() {

            @Override
            public void onObserveNotification(final String uid,
                    final SimulatorResourceModel resourceModelN, final int seq) {
                synchronizerThread.addToQueue(new Runnable() {
                    @Override
                    public void run() {
                        // Handling the response which includes retrieving the
                        // attributes and updating the local model.
                        RemoteResource resource = handleResponse(uid,
                                resourceModelN);
                        if (null != resource) {
                            // Notify the UI listeners
                            UiListenerHandler.getInstance()
                                    .observeCompleteNotification(resource);
                        }
                    }
                });
            }
        };

        verifyListener = new VerificationListener() {

            @Override
            public void onVerificationStarted(final String uid, final int autoId) {
                synchronizerThread.addToQueue(new Runnable() {
                    @Override
                    public void run() {
                        RemoteResource resource = getResource(uid);
                        if (null == resource) {
                            return;
                        }
                        // Update the automation status.
                        resource.updateAutomationStatus(autoId, true);

                        RequestType reqType = resource
                                .getAutomationtype(autoId);

                        // Notify the listeners.
                        UiListenerHandler.getInstance()
                                .verificationStartedNotification(resource,
                                        reqType);
                    }
                });
            }

            @Override
            public void onVerificationCompleted(final String uid,
                    final int autoId) {
                synchronizerThread.addToQueue(new Runnable() {
                    @Override
                    public void run() {
                        RemoteResource resource = getResource(uid);
                        if (null == resource) {
                            return;
                        }
                        // Update the automation status.
                        resource.updateAutomationStatus(autoId, false);

                        RequestType reqType = resource
                                .getAutomationtype(autoId);

                        // Notify the listeners.
                        UiListenerHandler.getInstance()
                                .verificationCompletedNotification(resource,
                                        reqType);
                    }
                });
            }

            @Override
            public void onVerificationAborted(final String uid, final int autoId) {
                synchronizerThread.addToQueue(new Runnable() {
                    @Override
                    public void run() {
                        RemoteResource resource = getResource(uid);
                        if (null == resource) {
                            return;
                        }
                        // Update the automation status.
                        resource.updateAutomationStatus(autoId, false);

                        RequestType reqType = resource
                                .getAutomationtype(autoId);

                        // Notify the listeners.
                        UiListenerHandler.getInstance()
                                .verificationAbortedNotification(resource,
                                        reqType);
                    }
                });
            }
        };

        synchronizerThread = new ResponseSynchronizerThread();
        threadHandle = new Thread(synchronizerThread);
        threadHandle.setName("Simulator Client Controller Event Queue");
        threadHandle.start();
    }

    private void handleExistingResource(
            final SimulatorRemoteResource newNativeResourceRef) {
        if (null == newNativeResourceRef) {
            return;
        }

        RemoteResource existingResource = getResource(newNativeResourceRef
                .getId());
        if (null == existingResource) {
            return;
        }

        SimulatorRemoteResource existingNativeResourceRef = existingResource
                .getRemoteResourceRef();
        if (null == existingNativeResourceRef) {
            return;
        }

        // Compare the resource properties(resource types, interface types and
        // observable)
        // of the received resource with the existing resource.
        // If there is a change, then replace the existing resource properties
        // and send
        // a GET request to receive the latest resource representation.
        boolean change = compareResourceProperties(existingNativeResourceRef,
                newNativeResourceRef);
        if (!change) {
            return;
        }

        existingResource.setRemoteResourceRef(newNativeResourceRef);

        try {
            String ifType = null;
            Vector<String> resInterfaces = newNativeResourceRef
                    .getResourceInterfaces();
            if (null != resInterfaces) {
                ifType = resInterfaces.get(0);
            }
            newNativeResourceRef.get(formQueryParameters(ifType, null),
                    getListener);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
        }

        // Notify the UI listener which may be looking for this callback for
        // further processing.
        UiListenerHandler.getInstance().newResourceFoundNotification(
                existingResource);

        // Notify the UI listeners by re-selecting the same resource.
        // This is just to refresh the resource properties being shown.
        RemoteResource resourceInSelection = getCurrentResourceInSelection();
        if (null != resourceInSelection) {
            if (resourceInSelection.getRemoteResourceRef().getURI()
                    .equals(newNativeResourceRef.getURI())) {
                UiListenerHandler.getInstance()
                        .resourceSelectionChangedUINotification(
                                existingResource);
            }
        }
    }

    private boolean compareResourceProperties(
            SimulatorRemoteResource existingNativeResourceRef,
            SimulatorRemoteResource newNativeResourceRef) {
        boolean change = false;

        try {
            // Compare URI.
            if (!existingNativeResourceRef.getURI().equals(
                    existingNativeResourceRef.getURI())) {
                change = true;
            }

            // Compare ID.
            if (!change
                    && !existingNativeResourceRef.getId().equals(
                            existingNativeResourceRef.getId())) {
                change = true;
            }

            // Compare Host.
            if (!change
                    && !existingNativeResourceRef.getHost().equals(
                            existingNativeResourceRef.getHost())) {
                change = true;
            }

            // Compare Observable flag.
            if (!change
                    && existingNativeResourceRef.isObservable() != existingNativeResourceRef
                            .isObservable()) {
                change = true;
            }

            // Compare Resource Types.
            Vector<String> existingResTypes = existingNativeResourceRef
                    .getResourceTypes();
            Vector<String> newResTypes = newNativeResourceRef
                    .getResourceTypes();

            if (!change) {
                if (existingResTypes.size() != newResTypes.size()) {
                    change = true;
                } else {
                    // Compare both lists.
                    Iterator<String> itr = existingResTypes.iterator();
                    while (itr.hasNext()) {
                        if (!newResTypes.contains(itr.next())) {
                            change = true;
                            break;
                        }
                    }
                }
            }

            // Compare Interface Types.
            Vector<String> existingInterfaceTypes = existingNativeResourceRef
                    .getResourceInterfaces();
            Vector<String> newInterfaceTypes = newNativeResourceRef
                    .getResourceInterfaces();

            if (!change) {
                if (existingInterfaceTypes.size() != newInterfaceTypes.size()) {
                    change = true;
                } else {
                    // Compare both lists.
                    Iterator<String> itr = existingInterfaceTypes.iterator();
                    while (itr.hasNext()) {
                        if (!newInterfaceTypes.contains(itr.next())) {
                            change = true;
                            break;
                        }
                    }
                }
            }
        } catch (Exception e) {
            change = true;
        }

        return change;
    }

    private RemoteResource handleResponse(String uid,
            SimulatorResourceModel resourceModelN) {
        if (null == uid || null == resourceModelN) {
            return null;
        }

        // Update the local model
        RemoteResource resource;
        resource = getResource(uid);
        if (null == resource) {
            return null;
        }

        resource.setResourceModelRef(resourceModelN);
        resource.setResourceRepresentation(resourceModelN);

        return resource;
    }

    public synchronized boolean isDeviceInfoExist(String host) {
        DeviceAndPlatformInfo info = hostDeviceAndPlatformMap.get(host);
        if (null == info) {
            return false;
        }
        if (null == info.getDeviceInfo()) {
            return false;
        }
        return true;
    }

    public synchronized boolean isPlatformInfoExist(String host) {
        DeviceAndPlatformInfo info = hostDeviceAndPlatformMap.get(host);
        if (null == info) {
            return false;
        }
        if (null == info.getPlatformInfo()) {
            return false;
        }
        return true;
    }

    private static class ResponseSynchronizerThread implements Runnable {

        LinkedList<Runnable> responseQueue = new LinkedList<Runnable>();

        @Override
        public void run() {
            while (!Thread.interrupted()) {
                synchronized (this) {
                    try {
                        while (responseQueue.isEmpty()) {
                            this.wait();
                            break;
                        }
                    } catch (InterruptedException e) {
                        return;
                    }
                }

                Runnable thread;
                synchronized (this) {
                    thread = responseQueue.pop();
                }
                try {
                    thread.run();
                } catch (Exception e) {
                    if (e instanceof InterruptedException) {
                        return;
                    }
                    e.printStackTrace();
                }
            }
        }

        public void addToQueue(Runnable event) {
            synchronized (this) {
                responseQueue.add(event);
                this.notify();
            }
        }
    }

    public void addResourcetoFavorites(RemoteResource resource) {
        if (null == resource) {
            return;
        }
        resource.setFavorite(true);
        synchronized (favoriteResources) {
            favoriteResources.add(resource);
        }
    }

    public void removeResourceFromFavorites(RemoteResource resource) {
        if (null == resource) {
            return;
        }
        resource.setFavorite(false);
        synchronized (favoriteResources) {
            favoriteResources.remove(resource);
        }
    }

    public void addResourceURItoFavorites(RemoteResource resource) {
        if (null == resource) {
            return;
        }
        synchronized (favoriteURIList) {
            favoriteURIList.add(resource.getRemoteResourceRef().getURI());
        }
    }

    public void removeResourceURIFromFavorites(RemoteResource resource) {
        if (null == resource) {
            return;
        }
        synchronized (favoriteURIList) {
            favoriteURIList.remove(resource.getRemoteResourceRef().getURI());
        }
    }

    public void addObservedResourceURI(String resourceURI) {
        synchronized (observedResourceURIList) {
            if (!observedResourceURIList.contains(resourceURI))
                observedResourceURIList.add(resourceURI);
        }
    }

    public void removeObservedResourceURI(String resourceURI) {
        synchronized (observedResourceURIList) {
            observedResourceURIList.remove(resourceURI);
        }
    }

    public boolean isResourceObserved(String resourceURI) {
        boolean observed;
        synchronized (observedResourceURIList) {
            observed = observedResourceURIList.contains(resourceURI);
        }
        return observed;
    }

    public synchronized RemoteResource getCurrentResourceInSelection() {
        return currentResourceInSelection;
    }

    public synchronized void setCurrentResourceInSelection(
            RemoteResource resource) {
        this.currentResourceInSelection = resource;
    }

    private void addResourceDetails(RemoteResource remoteResource) {
        if (null != remoteResource) {
            synchronized (resourceMap) {
                resourceMap.put(remoteResource.getRemoteResourceRef().getId(),
                        remoteResource);
            }
        }
    }

    private boolean isUidExist(String uid) {
        boolean exist;
        synchronized (resourceMap) {
            exist = resourceMap.containsKey(uid);
        }
        return exist;
    }

    private RemoteResource getResource(String uid) {
        if (null == uid) {
            return null;
        }
        RemoteResource resource;
        synchronized (resourceMap) {
            resource = resourceMap.get(uid);
        }
        return resource;
    }

    public synchronized Set<String> getLastKnownSearchTypes() {
        return lastKnownSearchTypes;
    }

    public synchronized void setLastKnownSearchTypes(
            Set<String> lastKnownSearchTypes) {
        this.lastKnownSearchTypes = lastKnownSearchTypes;
    }

    public boolean findResourceRequest(Set<String> searchTypes) {
        boolean result = false;
        if (null == searchTypes || searchTypes.size() < 1) {
            try {
                SimulatorManager.findResource(findResourceListener);
                result = true;
            } catch (SimulatorException e) {
                Activator
                        .getDefault()
                        .getLogManager()
                        .log(Level.ERROR.ordinal(), new Date(),
                                Utility.getSimulatorErrorString(e, null));
            }
        } else {
            Iterator<String> searchItr = searchTypes.iterator();
            String rType;
            while (searchItr.hasNext()) {
                rType = searchItr.next();
                try {
                    SimulatorManager.findResource(rType, findResourceListener);
                    result = true;
                } catch (SimulatorException e) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(), new Date(),
                                    Utility.getSimulatorErrorString(e, null));
                }
            }
        }
        return result;
    }

    public void deleteResources(final Set<String> searchTypes) {
        synchronized (resourceMap) {
            if (resourceMap.isEmpty()) {
                return;
            }
        }
        new Thread() {
            public void run() {
                if (null == searchTypes || searchTypes.size() < 1) {
                    synchronized (resourceMap) {
                        // Stop observing all the resources
                        Iterator<String> itr = resourceMap.keySet().iterator();
                        while (itr.hasNext()) {
                            sendCancelObserveRequest(
                                    resourceMap.get(itr.next()), false);
                        }
                        // Delete all cached details of resources
                        resourceMap.clear();

                        synchronized (favoriteResources) {
                            favoriteResources.clear();
                        }

                        // Clearing the device and platform information
                        synchronized (hostDeviceAndPlatformMap) {
                            hostDeviceAndPlatformMap.clear();
                        }
                    }
                    // Change the current resource in selection
                    setCurrentResourceInSelection(null);
                    UiListenerHandler.getInstance()
                            .resourceSelectionChangedUINotification(null);
                } else {
                    Iterator<String> typeItr = searchTypes.iterator();
                    String resType;
                    while (typeItr.hasNext()) {
                        resType = typeItr.next();
                        deleteResourcesByType(resType);

                        // Change the current resource in selection
                        updateCurrentResourceInSelection(searchTypes);
                    }
                }
            }
        }.start();
    }

    private void updateCurrentResourceInSelection(Set<String> searchTypes) {
        if (null == searchTypes || searchTypes.size() < 1) {
            return;
        }
        RemoteResource resourceInSelection = getCurrentResourceInSelection();
        if (null == resourceInSelection) {
            return;
        }
        List<String> typesOfSelection = resourceInSelection
                .getRemoteResourceRef().getResourceTypes();
        if (null == typesOfSelection || typesOfSelection.size() < 1) {
            return;
        }
        Iterator<String> itr = typesOfSelection.iterator();
        String type;
        while (itr.hasNext()) {
            type = itr.next();
            if (searchTypes.contains(type)) {
                setCurrentResourceInSelection(null);
                UiListenerHandler.getInstance()
                        .resourceSelectionChangedUINotification(null);
                break;
            }
        }
    }

    private void deleteResourcesByType(String resourceType) {
        if (null == resourceType) {
            return;
        }
        synchronized (resourceMap) {
            Set<String> keySet = resourceMap.keySet();
            if (null == keySet) {
                return;
            }
            Iterator<String> keyItr = keySet.iterator();
            String uId;
            RemoteResource resource;
            boolean exist;
            List<String> types;
            while (keyItr.hasNext()) {
                uId = keyItr.next();
                resource = resourceMap.get(uId);
                if (null == resource) {
                    continue;
                }
                types = resource.getRemoteResourceRef().getResourceTypes();
                if (null != types) {
                    exist = types.contains(resourceType);
                    if (exist) {
                        // Cancel observing the resource.
                        sendCancelObserveRequest(resource, false);
                        // Remove the resource from favorites list.
                        removeResourceFromFavorites(resource);
                        // Remove the resource
                        keyItr.remove();
                        // Remove the device and platform information
                        synchronized (hostDeviceAndPlatformMap) {
                            hostDeviceAndPlatformMap.remove(resource
                                    .getRemoteResourceRef().getHost());
                        }
                    }
                }
            }
        }
    }

    public void resourceSelectionChanged(final RemoteResource resource) {
        new Thread() {
            @Override
            public void run() {
                setCurrentResourceInSelection(resource);
                // Notify all observers for resource selection change event
                UiListenerHandler.getInstance()
                        .resourceSelectionChangedUINotification(resource);
            }
        }.start();
    }

    public List<MetaProperty> getDefaultProperties(RemoteResource resource) {
        if (null != resource) {
            String propName;
            StringBuilder propValue;

            List<MetaProperty> metaPropertyList = new ArrayList<MetaProperty>();

            for (int index = 0; index < Constants.META_PROPERTY_COUNT; index++) {
                propName = Constants.META_PROPERTIES[index];
                propValue = new StringBuilder();
                if (propName.equals(Constants.RESOURCE_URI)) {
                    propValue.append(resource.getRemoteResourceRef().getURI());
                } else if (propName.equals(Constants.CONNECTIVITY_TYPE)) {
                    propValue.append(resource.getRemoteResourceRef()
                            .getConnectivityType().toString());
                } else if (propName.equals(Constants.ADDRESS)) {
                    propValue.append(resource.getRemoteResourceRef().getHost());
                } else if (propName.equals(Constants.OBSERVABLE)) {
                    propValue.append(Utility.getObservableInString(resource
                            .getRemoteResourceRef().isObservable()));
                } else if (propName.equals(Constants.RESOURCE_TYPES)) {
                    Vector<String> resTypes = resource.getRemoteResourceRef()
                            .getResourceTypes();
                    if (null != resTypes && !resTypes.isEmpty()) {
                        Iterator<String> itr = resTypes.iterator();
                        while (itr.hasNext()) {
                            propValue.append(itr.next());
                            if (itr.hasNext()) {
                                propValue.append(", ");
                            }
                        }
                    } else {
                        propValue.append(Constants.NOT_AVAILABLE);
                    }
                } else if (propName.equals(Constants.RESOURCE_INTERFACES)) {
                    Vector<String> interfaces = resource.getRemoteResourceRef()
                            .getResourceInterfaces();
                    if (null != interfaces && !interfaces.isEmpty()) {
                        Iterator<String> itr = interfaces.iterator();
                        while (itr.hasNext()) {
                            propValue.append(itr.next());
                            if (itr.hasNext()) {
                                propValue.append(", ");
                            }
                        }
                    } else {
                        propValue.append(Constants.NOT_AVAILABLE);
                    }
                } else {
                    propValue = null;
                }
                if (null != propValue) {
                    metaPropertyList.add(new MetaProperty(propName, propValue
                            .toString()));
                }
            }

            return metaPropertyList;
        }
        return null;
    }

    public List<MetaProperty> getDeviceProperties() {
        RemoteResource resourceInSelection = getCurrentResourceInSelection();
        if (null == resourceInSelection) {
            return null;
        }

        SimulatorRemoteResource remoteResource = resourceInSelection
                .getRemoteResourceRef();
        if (null == remoteResource) {
            return null;
        }

        String host = remoteResource.getHost();
        if (null == host) {
            return null;
        }

        if (!isDeviceInfoExist(host)) {
            // Device Information
            try {
                SimulatorManager.findDevices(host, deviceListener);
            } catch (SimulatorException e) {
                Activator
                        .getDefault()
                        .getLogManager()
                        .log(Level.ERROR.ordinal(), new Date(),
                                Utility.getSimulatorErrorString(e, null));
            }
            return null;
        }

        List<MetaProperty> metaProperties = new ArrayList<MetaProperty>();
        synchronized (hostDeviceAndPlatformMap) {
            DeviceInfo devInfo = hostDeviceAndPlatformMap.get(host)
                    .getDeviceInfo();
            metaProperties.add(new MetaProperty(Constants.DEVICE_ID, devInfo
                    .getID()));
            metaProperties.add(new MetaProperty(Constants.DEVICE_NAME, devInfo
                    .getName()));
            metaProperties.add(new MetaProperty(Constants.DEVICE_SPEC_VERSION,
                    devInfo.getSpecVersion()));
            metaProperties.add(new MetaProperty(Constants.DEVICE_DMV_VERSION,
                    devInfo.getDataModelVersion()));
        }

        return metaProperties;
    }

    public List<MetaProperty> getPlatformProperties() {
        RemoteResource resourceInSelection = getCurrentResourceInSelection();
        if (null == resourceInSelection) {
            return null;
        }

        SimulatorRemoteResource remoteResource = resourceInSelection
                .getRemoteResourceRef();
        if (null == remoteResource) {
            return null;
        }

        String host = remoteResource.getHost();
        if (null == host) {
            return null;
        }

        if (!isPlatformInfoExist(host)) {
            // Platform Information
            try {
                SimulatorManager.getPlatformInformation(host, platformListener);
            } catch (SimulatorException e) {
                Activator
                        .getDefault()
                        .getLogManager()
                        .log(Level.ERROR.ordinal(), new Date(),
                                Utility.getSimulatorErrorString(e, null));
            }
            return null;
        }

        List<MetaProperty> metaProperties = new ArrayList<MetaProperty>();
        synchronized (hostDeviceAndPlatformMap) {
            PlatformInfo platInfo = hostDeviceAndPlatformMap.get(host)
                    .getPlatformInfo();
            metaProperties.add(new MetaProperty(Constants.PLATFORM_ID, platInfo
                    .getPlatformID()));
            metaProperties.add(new MetaProperty(
                    Constants.PLATFORM_MANUFAC_NAME, platInfo
                            .getManufacturerName()));
            metaProperties.add(new MetaProperty(Constants.PLATFORM_MANUFAC_URL,
                    platInfo.getManufacturerUrl()));
            metaProperties.add(new MetaProperty(Constants.PLATFORM_MODEL_NO,
                    platInfo.getModelNumber()));
            metaProperties.add(new MetaProperty(
                    Constants.PLATFORM_DATE_OF_MANUFAC, platInfo
                            .getDateOfManufacture()));
            metaProperties.add(new MetaProperty(Constants.PLATFORM_VERSION,
                    platInfo.getPlatformVersion()));
            metaProperties.add(new MetaProperty(Constants.PLATFORM_OS_VERSION,
                    platInfo.getOperationSystemVersion()));
            metaProperties.add(new MetaProperty(
                    Constants.PLATFORM_HARDWARE_VERSION, platInfo
                            .getHardwareVersion()));
            metaProperties.add(new MetaProperty(
                    Constants.PLATFORM_FIRMWARE_VERSION, platInfo
                            .getFirmwareVersion()));
            metaProperties.add(new MetaProperty(Constants.PLATFORM_SUPPORT_URL,
                    platInfo.getSupportUrl()));
            metaProperties.add(new MetaProperty(Constants.PLATFORM_SYSTEM_TIME,
                    platInfo.getSystemTime()));
        }
        return metaProperties;
    }

    public Map<String, Boolean> getAutomationStatus(RemoteResource resource) {
        if (null == resource) {
            return null;
        }
        Map<String, Boolean> autoStatus = new HashMap<String, Boolean>();
        autoStatus.put(Constants.GET, resource.isGetAutomtnInProgress());
        autoStatus.put(Constants.PUT, resource.isPutAutomtnInProgress());
        autoStatus.put(Constants.POST, resource.isPostAutomtnInProgress());
        return autoStatus;
    }

    public List<RemoteResource> getResourceList() {
        List<RemoteResource> resourceList = new ArrayList<RemoteResource>();
        synchronized (resourceMap) {
            Set<String> idSet = resourceMap.keySet();
            Iterator<String> idItr = idSet.iterator();
            RemoteResource resource;
            while (idItr.hasNext()) {
                resource = resourceMap.get(idItr.next());
                if (null != resource) {
                    resourceList.add(resource);
                }
            }
        }
        // Sort the list
        Collections.sort(resourceList, new Comparator<RemoteResource>() {
            public int compare(RemoteResource res1, RemoteResource res2) {
                String s1 = res1.getRemoteResourceRef().getURI();
                String s2 = res2.getRemoteResourceRef().getURI();

                String s1Part = s1.replaceAll("\\d", "");
                String s2Part = s2.replaceAll("\\d", "");

                if (s1Part.equalsIgnoreCase(s2Part)) {
                    return extractInt(s1) - extractInt(s2);
                }
                return s1.compareTo(s2);
            }

            int extractInt(String s) {
                String num = s.replaceAll("\\D", "");
                // return 0 if no digits found
                return num.isEmpty() ? 0 : Integer.parseInt(num);
            }
        });

        return resourceList;
    }

    public List<RemoteResource> getFavResourceList() {
        List<RemoteResource> resourceList;
        synchronized (favoriteResources) {
            resourceList = new ArrayList<RemoteResource>(favoriteResources);
        }
        return resourceList;
    }

    public List<String> getAllValuesOfAttribute(SimulatorResourceAttribute att) {
        if (null == att) {
            return null;
        }

        AttributeValue val = att.value();
        if (null == val || null == val.get()) {
            return null;
        }

        TypeInfo type = val.typeInfo();

        if (type.mBaseType == ValueType.RESOURCEMODEL) {
            return null;
        }

        List<String> values = new ArrayList<String>();

        AttributeProperty prop = att.property();
        if (null == prop) {
            values.add(new AttributeValueStringConverter(val).toString());
            return values;
        }

        if (type.mType == ValueType.ARRAY) {
            if (type.mDepth == 1) {
                ArrayProperty arrayProperty = prop.asArray();
                if (null != arrayProperty) {
                    AttributeProperty childProp = arrayProperty
                            .getElementProperty();
                    switch (childProp.getType()) {
                        case INTEGER:
                            IntegerProperty intProperty = childProp.asInteger();
                            if (null != intProperty) {
                                values.addAll(getAllValues(att, intProperty,
                                        Type.INTEGER));
                            }
                            break;
                        case DOUBLE:
                            DoubleProperty dblProperty = childProp.asDouble();
                            if (null != dblProperty) {
                                values.addAll(getAllValues(att, dblProperty,
                                        Type.DOUBLE));
                            }
                            break;
                        case BOOLEAN:
                            BooleanProperty boolProperty = childProp
                                    .asBoolean();
                            if (null != boolProperty) {
                                values.addAll(getAllValues(att, boolProperty,
                                        Type.BOOLEAN));
                            }
                            break;
                        case STRING:
                            StringProperty stringProperty = childProp
                                    .asString();
                            if (null != stringProperty) {
                                values.addAll(getAllValues(att, stringProperty,
                                        Type.STRING));
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        } else {
            switch (prop.getType()) {
                case INTEGER:
                    IntegerProperty intProperty = prop.asInteger();
                    if (null != intProperty) {
                        values.addAll(getAllValues(att, intProperty,
                                Type.INTEGER));
                    }
                    break;
                case DOUBLE:
                    DoubleProperty dblProperty = prop.asDouble();
                    if (null != dblProperty) {
                        values.addAll(getAllValues(att, dblProperty,
                                Type.DOUBLE));
                    }
                    break;
                case BOOLEAN:
                    BooleanProperty boolProperty = prop.asBoolean();
                    if (null != boolProperty) {
                        values.addAll(getAllValues(att, boolProperty,
                                Type.BOOLEAN));
                    }
                    break;
                case STRING:
                    StringProperty stringProperty = prop.asString();
                    if (null != stringProperty) {
                        values.addAll(getAllValues(att, stringProperty,
                                Type.STRING));
                    }
                    break;
                default:
                    break;
            }
        }

        return values;
    }

    public List<String> getAllValues(SimulatorResourceAttribute attribute,
            IntegerProperty intProperty, AttributeProperty.Type type) {
        List<String> values = new ArrayList<String>();

        if (intProperty.hasRange()) {
            int min = (int) intProperty.min();
            int max = (int) intProperty.max();
            for (int iVal = min; iVal <= max; iVal++) {
                values.add(String.valueOf(iVal));
            }
        } else if (intProperty.hasValues()) {
            for (Integer val : intProperty.getValues()) {
                values.add(String.valueOf(val));
            }
        } else {
            AttributeValue value = attribute.value();
            if (null != value && null != value.get()) {
                // Adding the current value of the attribute.
                values.add(String.valueOf(value.get()));
            }
        }
        return values;
    }

    public List<String> getAllValues(SimulatorResourceAttribute attribute,
            DoubleProperty dblProperty, AttributeProperty.Type type) {
        NumberFormat formatter = NumberFormat.getInstance();
        List<String> values = new ArrayList<String>();

        if (dblProperty.hasRange()) {
            double min = (double) dblProperty.min();
            double max = (double) dblProperty.max();
            for (double val = min; val <= max; val += 0.1) {
                formatter.setMaximumFractionDigits(1);
                formatter.setMinimumFractionDigits(1);
                values.add(formatter.format(val));
            }
        } else if (dblProperty.hasValues()) {
            for (Double val : dblProperty.getValues()) {
                values.add(String.valueOf(val));
            }
        } else {
            AttributeValue value = attribute.value();
            if (null != value && null != value.get()) {
                // Adding the current value of the attribute.
                values.add(String.valueOf(value.get()));
            }
        }
        return values;
    }

    public List<String> getAllValues(SimulatorResourceAttribute attribute,
            BooleanProperty boolProperty, AttributeProperty.Type type) {
        List<String> values = new ArrayList<String>();
        values.add("true");
        values.add("false");
        return values;
    }

    public List<String> getAllValues(SimulatorResourceAttribute attribute,
            StringProperty stringProperty, AttributeProperty.Type type) {
        List<String> values = new ArrayList<String>();

        if (stringProperty.hasValues()) {
            for (String val : stringProperty.getValues()) {
                values.add(String.valueOf(val));
            }
        } else {
            AttributeValue value = attribute.value();
            if (null != value && null != value.get()) {
                // Adding the current value of the attribute.
                values.add(String.valueOf(value.get()));
            }
        }
        return values;
    }

    public void sendGetRequest(String ifType, String query,
            RemoteResource resource) {
        if (null == resource) {
            return;
        }
        SimulatorRemoteResource resourceN = resource.getRemoteResourceRef();
        if (null == resourceN) {
            return;
        }

        Map<String, String> queryParams = formQueryParameters(ifType, query);
        try {
            resourceN.get(queryParams, getListener);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
        }
    }

    public void sendPutRequest(String ifType, RemoteResource resource,
            SimulatorResourceModel model) {
        if (null == resource || null == model) {
            return;
        }
        SimulatorRemoteResource resourceN = resource.getRemoteResourceRef();
        if (null == resourceN) {
            return;
        }
        Map<String, String> queryParams = formQueryParameters(ifType, null);
        try {
            resourceN.put(queryParams, model, putListener);
        } catch (Exception e) {
            String addlInfo;
            addlInfo = "Invalid Attribute Value. Cannot send PUT request.";
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, addlInfo));
        }
    }

    public void sendPostRequest(String ifType, RemoteResource resource,
            SimulatorResourceModel model) {
        if (null == resource || null == model) {
            return;
        }
        SimulatorRemoteResource resourceN = resource.getRemoteResourceRef();
        if (null == resourceN) {
            return;
        }
        Map<String, String> queryParams = formQueryParameters(ifType, null);
        try {
            resourceN.post(queryParams, model, postListener);
        } catch (Exception e) {
            String addlInfo;
            addlInfo = "Invalid Attribute Value. Cannot send POST request.";
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, addlInfo));
        }
    }

    public boolean sendObserveRequest(RemoteResource resource) {
        if (null == resource) {
            return false;
        }
        SimulatorRemoteResource resourceN = resource.getRemoteResourceRef();
        if (null == resourceN) {
            return false;
        }
        try {
            resourceN.observe(observeListener);
            resource.setObserved(true);
            // Add observed resource URI to show the proper status after every
            // find/refresh operations.
            addObservedResourceURI(resource.getRemoteResourceRef().getURI());
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
            return false;
        }
        return true;
    }

    private Map<String, String> formQueryParameters(String ifType, String query) {
        Map<String, String> queryParams = new HashMap<String, String>();

        // Including the interface type, if given,
        if (null != ifType) {
            ifType = ifType.trim();
            if (ifType.length() > 0)
                queryParams.put("if", ifType);
        }

        // Including other queries, if given.
        if (null != query) {
            query = query.trim();
            if (query.length() > 0) {
                // Parse the query parameters and fill the map.
                String queries[] = query.split(";");
                if (queries.length > 0) {
                    for (String pair : queries) {
                        String tok[] = pair.split("=");
                        if (null != tok && tok.length == 2) {
                            queryParams.put(tok[0].trim(), tok[1].trim());
                        }
                    }
                }
            }
        }
        return queryParams;
    }

    public boolean sendCancelObserveRequest(RemoteResource resource,
            boolean removeEntry) {
        if (null == resource || !resource.isObserved()) {
            return false;
        }
        SimulatorRemoteResource resourceN = resource.getRemoteResourceRef();
        if (null == resourceN) {
            return false;
        }
        try {
            resourceN.stopObserve();
            resource.setObserved(false);
            // Remove observed resource URI to show the proper status after
            // every find/refresh operations.
            if (removeEntry)
                removeObservedResourceURI(resource.getRemoteResourceRef()
                        .getURI());
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
            return false;
        }
        return true;
    }

    public void startAutomationRequest(RequestType reqType,
            RemoteResource resource) {
        if (null == resource) {
            return;
        }
        SimulatorRemoteResource resourceN = resource.getRemoteResourceRef();
        if (null == resourceN) {
            return;
        }
        if (null == reqType) {
            return;
        }
        int autoId;
        try {
            autoId = resourceN.startVerification(reqType, verifyListener);
            if (autoId != -1) {
                if (reqType == RequestType.GET) {
                    resource.setGetAutomtnId(autoId);
                } else if (reqType == RequestType.PUT) {
                    resource.setPutAutomtnId(autoId);
                } else {
                    resource.setPostAutomtnId(autoId);
                }
            }
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.INFO.ordinal(),
                            new Date(),
                            "[" + reqType.toString()
                                    + "] Verification Started for \""
                                    + resourceN.getURI() + "\".");
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
        }
    }

    public void stopAutomationRequest(RequestType reqType,
            RemoteResource resource) {
        if (null == resource) {
            return;
        }
        SimulatorRemoteResource resourceN = resource.getRemoteResourceRef();
        if (null == resourceN) {
            return;
        }
        int autoId;
        if (reqType == RequestType.GET) {
            resource.setGetAutomtnInProgress(false);
            autoId = resource.getGetAutomtnId();
        } else if (reqType == RequestType.PUT) {
            resource.setPutAutomtnInProgress(false);
            autoId = resource.getPutAutomtnId();
        } else {
            resource.setPostAutomtnInProgress(false);
            autoId = resource.getPostAutomtnId();
        }
        try {
            resourceN.stopVerification(autoId);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
        }
    }

    public boolean setConfigFilePath(RemoteResource resource,
            String configFilePath) throws SimulatorException {

        if (null == resource) {
            return false;
        }
        SimulatorRemoteResource resourceN = resource.getRemoteResourceRef();
        if (null == resourceN) {
            return false;
        }
        try {
            Map<RequestType, SimulatorRequestModel> requestModels;
            requestModels = resourceN.setConfigInfo(configFilePath);
            if (null == requestModels) {
                return false;
            }

            // Store the resource model in the local cache
            resource.setRequestModels(requestModels);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
            throw e;
        } catch (Exception e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            Utility.getSimulatorErrorString(e,
                                    "Error while configuring the attribute properties"));
        }
        // Update the status
        resource.setConfigUploaded(true);

        // Notify the UI listeners
        UiListenerHandler.getInstance().configUploadedNotification(resource);

        return true;
    }

    public void shutdown() {
    }
}
