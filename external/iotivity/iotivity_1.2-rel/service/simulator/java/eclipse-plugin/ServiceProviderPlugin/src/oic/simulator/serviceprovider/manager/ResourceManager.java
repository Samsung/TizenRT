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

package oic.simulator.serviceprovider.manager;

import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.swt.widgets.Display;

import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashSet;
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
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorManager;
import org.oic.simulator.SimulatorResourceAttribute;
import org.oic.simulator.SimulatorResourceModel;
import org.oic.simulator.StringProperty;
import org.oic.simulator.server.Observer;
import org.oic.simulator.server.SimulatorResource;
import org.oic.simulator.server.SimulatorResource.AutoUpdateListener;
import org.oic.simulator.server.SimulatorResource.AutoUpdateType;
import org.oic.simulator.server.SimulatorResource.ObserverListener;
import org.oic.simulator.server.SimulatorResource.ResourceModelChangeListener;
import org.oic.simulator.server.SimulatorSingleResource;

import oic.simulator.serviceprovider.Activator;
import oic.simulator.serviceprovider.model.AttributeElement;
import oic.simulator.serviceprovider.model.MetaProperty;
import oic.simulator.serviceprovider.model.Resource;
import oic.simulator.serviceprovider.model.ResourceType;
import oic.simulator.serviceprovider.model.SingleResource;
import oic.simulator.serviceprovider.utils.AttributeValueStringConverter;
import oic.simulator.serviceprovider.utils.Constants;
import oic.simulator.serviceprovider.utils.Utility;

/**
 * This class acts as an interface between the simulator java SDK and the
 * various UI modules. It maintains all the details of resources and provides
 * other UI modules with the information required. It also handles model change,
 * automation, and observer related events from native layer and propagates
 * those events to the registered UI listeners.
 */
public class ResourceManager {

    private Data                           data;

    private Resource                       currentResourceInSelection;

    private ResourceModelChangeListener    resourceModelChangeListener;

    private AutoUpdateListener             automationListener;

    private ObserverListener               observer;

    private DeviceListener                 deviceListener;

    private NotificationSynchronizerThread synchronizerThread;

    private Thread                         threadHandle;

    private DeviceInfo                     deviceInfo;
    private PlatformInfo                   platformInfo;

    private String                         deviceName;

    public ResourceManager() {
        data = new Data();

        deviceListener = new DeviceListener() {

            @Override
            public void onDeviceFound(final String host,
                    final DeviceInfo deviceInfo) {
                if (null != ResourceManager.this.deviceInfo
                        || null == deviceInfo || null == host) {
                    return;
                }
                synchronizerThread.addToQueue(new Runnable() {
                    @Override
                    public void run() {
                        String rcvdDeviceName = deviceInfo.getName();
                        if (null == rcvdDeviceName) {
                            return;
                        }
                        if (deviceName.equalsIgnoreCase(rcvdDeviceName)) {
                            ResourceManager.this.deviceInfo = deviceInfo;

                            // Notify the UI Listeners
                            UiListenerHandler.getInstance()
                                    .deviceInfoReceivedNotification();
                        }
                    }
                });
            }
        };

        resourceModelChangeListener = new ResourceModelChangeListener() {

            @Override
            public void onResourceModelChanged(final String resourceURI,
                    final SimulatorResourceModel resourceModelN) {
                synchronizerThread.addToQueue(new Runnable() {

                    @Override
                    public void run() {
                        if (null == resourceURI || null == resourceModelN) {
                            return;
                        }

                        Display.getDefault().asyncExec(new Runnable() {
                            @Override
                            public void run() {
                                Resource resource = data
                                        .getResourceByURI(resourceURI);
                                if (null != resource) {
                                    try {
                                        resource.updateResourceRepresentation(resourceModelN);
                                    } catch (NumberFormatException e) {
                                        Activator
                                                .getDefault()
                                                .getLogManager()
                                                .log(Level.ERROR.ordinal(),
                                                        new Date(),
                                                        "Error while trying to update the attributes.\n"
                                                                + Utility
                                                                        .getSimulatorErrorString(
                                                                                e,
                                                                                null));
                                    }
                                }
                            }
                        });
                    }
                });
            }
        };

        automationListener = new AutoUpdateListener() {

            @Override
            public void onUpdateComplete(final String resourceURI,
                    final int automationId) {
                synchronizerThread.addToQueue(new Runnable() {

                    @Override
                    public void run() {
                        SingleResource resource = data
                                .getSingleResourceByURI(resourceURI);
                        if (null == resource) {
                            return;
                        }
                        // Checking whether this notification is for an
                        // attribute or a resource
                        if (resource.isResourceAutomationInProgress()) {
                            changeResourceLevelAutomationStatus(resource, false);
                            // Notify the UI listeners
                            UiListenerHandler.getInstance()
                                    .automationCompleteUINotification(resource,
                                            null);
                        } else if (resource.isAttributeAutomationInProgress()) {
                            // Find the attribute with the given automation id
                            final AttributeElement attribute = getAttributeWithGivenAutomationId(
                                    resource, automationId);
                            if (null != attribute) {
                                attribute.setAutoUpdateState(false);
                                resource.setAttributeAutomationInProgress(isAnyAttributeInAutomation(resource));
                            } else {
                                // Setting the attribute automation status to
                                // false.
                                resource.setAttributeAutomationInProgress(false);
                            }
                        }
                    }
                });
            }
        };

        observer = new ObserverListener() {

            public void onObserverChanged(final String resourceURI,
                    final int status, final Observer observer) {
                new Thread() {
                    @Override
                    public void run() {
                        if (null == resourceURI || null == observer) {
                            return;
                        }
                        Resource resource = data.getResourceByURI(resourceURI);
                        if (null == resource) {
                            return;
                        }
                        // Update the observers information
                        if (status == 0) {
                            resource.addObserverInfo(observer);
                        } else {
                            resource.removeObserverInfo(observer);
                        }
                        // Notify the UI listeners
                        UiListenerHandler.getInstance()
                                .observerListChangedUINotification(resource);
                    }
                }.start();
            }

            @Override
            public void onObserverAdded(String resourceURI, Observer observer) {
                onObserverChanged(resourceURI, 0, observer);
            }

            @Override
            public void onObserverRemoved(String resourceURI, Observer observer) {
                onObserverChanged(resourceURI, 1, observer);
            }
        };

        synchronizerThread = new NotificationSynchronizerThread();
        threadHandle = new Thread(synchronizerThread);
        threadHandle.setName("Simulator service provider event queue");
        threadHandle.start();

        // Set the default device name.
        deviceName = "IoTivity Simulator";
        try {
            SimulatorManager.setDeviceInfo(deviceName);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "Error while registering the device info.\n"
                                    + Utility.getSimulatorErrorString(e, null));
        }

        // Set the default platform information.
        platformInfo = new PlatformInfo();
        platformInfo.setPlatformID("Samsung Platform Identifier");
        platformInfo.setManufacturerName("Samsung");
        platformInfo.setManufacturerUrl("www.samsung.com");
        platformInfo.setModelNumber("Samsung Model Num01");
        platformInfo.setDateOfManufacture("2015-09-10T11:10:30Z");
        platformInfo.setPlatformVersion("PlatformVersion01");
        platformInfo.setOperationSystemVersion("OSVersion01");
        platformInfo.setHardwareVersion("HardwareVersion01");
        platformInfo.setFirmwareVersion("FirwareVersion01");
        platformInfo.setSupportUrl("http://www.samsung.com/support");
        platformInfo.setSystemTime("2015-09-10T11:10:30Z");
        try {
            SimulatorManager.setPlatformInfo(platformInfo);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "Error while registering the platform info.\n"
                                    + Utility.getSimulatorErrorString(e, null));
        }

        // Get the device information to show other details of the device in UI.
        try {
            SimulatorManager.findDevices("", deviceListener);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "Failed to get the local device information.\n"
                                    + Utility.getSimulatorErrorString(e, null));
        }
    }

    private static class NotificationSynchronizerThread implements Runnable {

        LinkedList<Runnable> notificationQueue = new LinkedList<Runnable>();

        @Override
        public void run() {
            while (!Thread.interrupted()) {
                synchronized (this) {
                    try {
                        while (notificationQueue.isEmpty()) {
                            this.wait();
                            break;
                        }
                    } catch (InterruptedException e) {
                        return;
                    }
                }

                Runnable thread;
                synchronized (this) {
                    thread = notificationQueue.pop();
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
                notificationQueue.add(event);
                this.notify();
            }
        }
    }

    public void setDeviceInfo(List<MetaProperty> metaProperties) {
        if (null == metaProperties || metaProperties.size() < 1) {
            return;
        }
        Iterator<MetaProperty> itr = metaProperties.iterator();
        MetaProperty prop;
        String propName;
        String propValue;
        boolean found = false;
        while (itr.hasNext()) {
            prop = itr.next();
            propName = prop.getPropName();
            propValue = prop.getPropValue();
            if (propName.equals(Constants.DEVICE_NAME)) {
                this.deviceName = propValue;
                found = true;
                break;
            }
        }

        if (!found) {
            return;
        }

        try {
            SimulatorManager.setDeviceInfo(deviceName);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "Error while registering the device info.\n"
                                    + Utility.getSimulatorErrorString(e, null));
        }
    }

    public boolean isDeviceInfoValid(List<MetaProperty> metaProperties) {
        if (null == metaProperties || metaProperties.size() < 1) {
            return false;
        }

        Iterator<MetaProperty> itr = metaProperties.iterator();
        MetaProperty prop;
        String propName;
        String propValue;
        while (itr.hasNext()) {
            prop = itr.next();
            propName = prop.getPropName();
            propValue = prop.getPropValue();
            if (propName.equals(Constants.DEVICE_NAME)) {
                if (null == propValue || propValue.length() < 1) {
                    return false;
                }
                break;
            }
        }
        return true;
    }

    public List<MetaProperty> getDeviceInfo() {
        List<MetaProperty> metaProperties = new ArrayList<MetaProperty>();
        metaProperties.add(new MetaProperty(Constants.DEVICE_NAME, deviceName));
        if (null != deviceInfo) {
            metaProperties.add(new MetaProperty(Constants.DEVICE_ID, deviceInfo
                    .getID()));
            metaProperties.add(new MetaProperty(Constants.DEVICE_SPEC_VERSION,
                    deviceInfo.getSpecVersion()));
            metaProperties.add(new MetaProperty(Constants.DEVICE_DMV,
                    deviceInfo.getDataModelVersion()));
        }
        return metaProperties;
    }

    public List<MetaProperty> getPlatformInfo() {
        List<MetaProperty> metaProperties = new ArrayList<MetaProperty>();
        metaProperties.add(new MetaProperty(Constants.PLATFORM_ID, platformInfo
                .getPlatformID()));
        metaProperties.add(new MetaProperty(Constants.PLATFORM_MANUFAC_NAME,
                platformInfo.getManufacturerName()));
        metaProperties.add(new MetaProperty(Constants.PLATFORM_MANUFAC_URL,
                platformInfo.getManufacturerUrl()));
        metaProperties.add(new MetaProperty(Constants.PLATFORM_MODEL_NO,
                platformInfo.getModelNumber()));
        metaProperties.add(new MetaProperty(Constants.PLATFORM_DATE_OF_MANUFAC,
                platformInfo.getDateOfManufacture()));
        metaProperties.add(new MetaProperty(Constants.PLATFORM_VERSION,
                platformInfo.getPlatformVersion()));
        metaProperties.add(new MetaProperty(Constants.PLATFORM_OS_VERSION,
                platformInfo.getOperationSystemVersion()));
        metaProperties.add(new MetaProperty(
                Constants.PLATFORM_HARDWARE_VERSION, platformInfo
                        .getHardwareVersion()));
        metaProperties.add(new MetaProperty(
                Constants.PLATFORM_FIRMWARE_VERSION, platformInfo
                        .getFirmwareVersion()));
        metaProperties.add(new MetaProperty(Constants.PLATFORM_SUPPORT_URL,
                platformInfo.getSupportUrl()));
        metaProperties.add(new MetaProperty(Constants.PLATFORM_SYSTEM_TIME,
                platformInfo.getSystemTime()));
        return metaProperties;
    }

    public void setPlatformInfo(List<MetaProperty> metaProperties) {
        if (null == metaProperties || metaProperties.size() < 1) {
            return;
        }
        Iterator<MetaProperty> itr = metaProperties.iterator();
        MetaProperty prop;
        String propName;
        String propValue;
        while (itr.hasNext()) {
            prop = itr.next();
            propName = prop.getPropName();
            propValue = prop.getPropValue();
            if (propName.equals(Constants.PLATFORM_ID)) {
                platformInfo.setPlatformID(propValue);
            } else if (propName.equals(Constants.PLATFORM_MANUFAC_NAME)) {
                platformInfo.setManufacturerName(propValue);
            } else if (propName.equals(Constants.PLATFORM_MANUFAC_URL)) {
                platformInfo.setManufacturerUrl(propValue);
            } else if (propName.equals(Constants.PLATFORM_MODEL_NO)) {
                platformInfo.setModelNumber(propValue);
            } else if (propName.equals(Constants.PLATFORM_DATE_OF_MANUFAC)) {
                platformInfo.setDateOfManufacture(propValue);
            } else if (propName.equals(Constants.PLATFORM_VERSION)) {
                platformInfo.setPlatformVersion(propValue);
            } else if (propName.equals(Constants.PLATFORM_OS_VERSION)) {
                platformInfo.setOperationSystemVersion(propValue);
            } else if (propName.equals(Constants.PLATFORM_HARDWARE_VERSION)) {
                platformInfo.setHardwareVersion(propValue);
            } else if (propName.equals(Constants.PLATFORM_FIRMWARE_VERSION)) {
                platformInfo.setFirmwareVersion(propValue);
            } else if (propName.equals(Constants.PLATFORM_SUPPORT_URL)) {
                platformInfo.setSupportUrl(propValue);
            } else if (propName.equals(Constants.PLATFORM_SYSTEM_TIME)) {
                platformInfo.setSystemTime(propValue);
            }
        }
        try {
            SimulatorManager.setPlatformInfo(platformInfo);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "Error while registering the platform info.\n"
                                    + Utility.getSimulatorErrorString(e, null));
        }
    }

    public boolean isPlatformInfoValid(List<MetaProperty> metaProperties) {
        if (null == metaProperties || metaProperties.size() < 1) {
            return false;
        }
        Iterator<MetaProperty> itr = metaProperties.iterator();
        MetaProperty prop;
        String propValue;
        while (itr.hasNext()) {
            prop = itr.next();
            propValue = prop.getPropValue();
            if (null == propValue || propValue.length() < 1) {
                return false;
            }
        }
        return true;
    }

    public synchronized Resource getCurrentResourceInSelection() {
        return currentResourceInSelection;
    }

    public synchronized void setCurrentResourceInSelection(Resource resource) {
        this.currentResourceInSelection = resource;
    }

    public boolean isResourceExist(String resourceURI) {
        return data.isResourceExist(resourceURI);
    }

    public boolean isAnyResourceExist() {
        return data.isAnyResourceExist();
    }

    public boolean createSingleResource(SingleResource resource,
            Map<String, SimulatorResourceAttribute> attributes)
            throws SimulatorException {
        if (null == resource) {
            return false;
        }

        try {
            // Create the resource.
            SimulatorResource jSimulatorResource = SimulatorManager
                    .createResource(SimulatorResource.Type.SINGLE,
                            resource.getResourceName(),
                            resource.getResourceURI(),
                            resource.getResourceType());
            if (null == jSimulatorResource
                    || !(jSimulatorResource instanceof SimulatorSingleResource)) {
                return false;
            }
            SimulatorSingleResource jSimulatorSingleResource = (SimulatorSingleResource) jSimulatorResource;
            resource.setSimulatorResource(jSimulatorSingleResource);

            // Cancel discoverable property if requested by user.
            if (!resource.isDiscoverable()) {
                jSimulatorSingleResource.setDiscoverable(false);
            }

            // Cancel observable property if requested by user.
            if (!resource.isObservable()) {
                jSimulatorSingleResource.setObservable(false);
            }

            // Set the model change listener.
            jSimulatorSingleResource
                    .setResourceModelChangeListener(resourceModelChangeListener);

            // Set the observer listener if the resource is observable.
            if (resource.isObservable()) {
                jSimulatorSingleResource.setObserverListener(observer);
            }

            // Add attributes.
            if (null != attributes && !attributes.isEmpty()) {
                SimulatorResourceAttribute value;
                for (Map.Entry<String, SimulatorResourceAttribute> entry : attributes
                        .entrySet()) {
                    value = entry.getValue();
                    if (null != value)
                        jSimulatorSingleResource.addAttribute(value);
                }

                // Get the resource model java object reference.
                resource.setResourceModel(jSimulatorSingleResource
                        .getResourceModel());

                resource.createResourceRepresentation(jSimulatorSingleResource
                        .getAttributes());
            }

            // Set the resource interfaces.
            Set<String> interfaces = resource.getResourceInterfaces();
            if (null != interfaces && !interfaces.isEmpty()) {
                jSimulatorSingleResource.setInterface(Utility
                        .convertSetToVectorString(resource
                                .getResourceInterfaces()));
            }

            // Register the resource with the platform.
            jSimulatorSingleResource.start();

            // Read the interfaces from the native layer and set it to the
            // interface list.
            resource.setResourceInterfaces(Utility
                    .convertVectorToSet(jSimulatorSingleResource.getInterface()));

            resource.setStarted(true);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
            throw e;
        }

        // Add to local cache.
        data.addResource(resource);

        // Update UI listeners
        UiListenerHandler.getInstance().resourceCreatedUINotification(
                ResourceType.SINGLE);

        Activator
                .getDefault()
                .getLogManager()
                .log(Level.INFO.ordinal(), new Date(),
                        "Resource created [" + resource.getResourceURI() + "].");

        return true;
    }

    public Resource createResourceByRAML(String configFilePath)
            throws SimulatorException {
        SimulatorResource jSimulatorResource;
        try {
            // Create the resource
            jSimulatorResource = SimulatorManager
                    .createResource(configFilePath);
            if (null == jSimulatorResource) {
                return null;
            }
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
            throw e;
        }

        if (!(jSimulatorResource instanceof SimulatorSingleResource)) {
            return null;
        }

        Resource resource = new SingleResource();
        resource.setSimulatorResource(jSimulatorResource);

        // Fetch and locally store the resource name, type and uri.
        String uri = jSimulatorResource.getURI();
        if (null == uri || uri.trim().isEmpty()) {
            // URI is mandatory to be present in RAML. If it is not there, then
            // resource can't be created.
            return null;
        }
        resource.setResourceURI(uri.trim());

        String type = jSimulatorResource.getResourceType();
        if (null != type) {
            type = type.trim();
        }
        resource.setResourceType(type);

        String name = jSimulatorResource.getName();
        if (null != name) {
            name = name.trim();
        }
        resource.setResourceName(name);

        return resource;
    }

    /**
     * This method can set/change the resource uri and name of an already
     * created resource which is not yet registered with the platform. This
     * method registers the model change and observer listeners, registers the
     * resource, fetches the resource attributes, updates the local cache and
     * notifies the UI listeners.
     */
    public boolean completeSingleResourceCreationByRAML(Resource resource,
            String uri, String name, String type, boolean multiInstance)
            throws SimulatorException {
        if (null == resource || !(resource instanceof SingleResource)) {
            return false;
        }
        try {
            SingleResource singleRes = (SingleResource) resource;

            SimulatorSingleResource jSimulatorSingleResource = (SimulatorSingleResource) resource
                    .getSimulatorResource();
            if (null == jSimulatorSingleResource) {
                return false;
            }

            // Update resource URI and Name if they are changed.
            String newUri = "";
            String newName = "";
            String newType = "";

            if (null != uri)
                newUri = uri.trim();
            if (null != name)
                newName = name.trim();
            if (null != type)
                newType = type.trim();

            if (!multiInstance && !singleRes.getResourceURI().equals(newUri)) {
                jSimulatorSingleResource.setURI(newUri);
                singleRes.setResourceURI(newUri);
            }
            String currentName = singleRes.getResourceName();
            if (null == currentName || !currentName.equals(newName)) {
                jSimulatorSingleResource.setName(newName);
                singleRes.setResourceName(newName);
            }
            String currentType = singleRes.getResourceType();
            if (null == currentType || !currentType.equals(newType)) {
                jSimulatorSingleResource.setResourceType(newType);
                singleRes.setResourceType(newType);
            }

            // Set the model change listener.
            jSimulatorSingleResource
                    .setResourceModelChangeListener(resourceModelChangeListener);

            // Set the observer listener if the resource is observable.
            if (jSimulatorSingleResource.isObservable()) {
                jSimulatorSingleResource.setObserverListener(observer);
                singleRes.setObservable(true);
            }

            // Fetch the resource model.
            SimulatorResourceModel jResModel = jSimulatorSingleResource
                    .getResourceModel();
            if (null == jResModel) {
                return false;
            }
            singleRes.setResourceModel(jResModel);

            // Fetch the basic details of the resource.
            singleRes
                    .setResourceInterfaces(Utility
                            .convertVectorToSet(jSimulatorSingleResource
                                    .getInterface()));

            // Fetch the resource attributes.
            singleRes.createResourceRepresentation(jSimulatorSingleResource
                    .getAttributes());

            // Register the resource with the platform.
            jSimulatorSingleResource.start();
            singleRes.setStarted(true);

            // Add to local cache.
            data.addResource(singleRes);

            // Update UI listeners for single instance creation
            if (!multiInstance)
                UiListenerHandler.getInstance().resourceCreatedUINotification(
                        ResourceType.SINGLE);
        } catch (Exception e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
            throw e;
        }

        Activator
                .getDefault()
                .getLogManager()
                .log(Level.INFO.ordinal(), new Date(),
                        "Resource created [" + resource.getResourceURI() + "].");
        return true;
    }

    public Set<SingleResource> createSingleResourceMultiInstances(
            String configFile, int count, IProgressMonitor progressMonitor)
            throws SimulatorException {
        Set<SingleResource> resultSet;
        try {
            resultSet = new HashSet<SingleResource>();
            Vector<SimulatorResource> jSimulatorResources = SimulatorManager
                    .createResource(configFile, count);
            if (null == jSimulatorResources || jSimulatorResources.size() < 1) {
                return null;
            }
            SimulatorSingleResource jResource;
            SingleResource resource;
            for (SimulatorResource jSimulatorResource : jSimulatorResources) {
                // If the resource creation progress is canceled, then stop the
                // creation and stop/delete
                // the resources created already.
                if (progressMonitor.isCanceled()) {
                    removeSingleResources(resultSet);
                    return null;
                }
                jResource = (SimulatorSingleResource) jSimulatorResource;
                resource = new SingleResource();
                resource.setSimulatorResource(jResource);
                try {
                    String uri = jSimulatorResource.getURI();
                    if (null == uri || uri.trim().isEmpty()) {
                        // URI is mandatory to be present in RAML. If it is not
                        // there, then resource will be ignored.
                        continue;
                    }
                    resource.setResourceURI(uri.trim());

                    String type = jSimulatorResource.getResourceType();
                    if (null != type) {
                        type = type.trim();
                    }
                    resource.setResourceType(type);

                    String name = jSimulatorResource.getName();
                    if (null != name) {
                        name = name.trim();
                    }
                    resource.setResourceName(name);

                    resultSet.add(resource);
                } catch (SimulatorException eInner) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(),
                                    new Date(),
                                    Utility.getSimulatorErrorString(eInner,
                                            null));
                }
                progressMonitor.worked(1);
            }
        } catch (SimulatorException eOuter) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(eOuter, null));
            throw eOuter;
        }
        return resultSet;
    }

    public List<Resource> getResourceList() {
        List<Resource> resourceList = data.getResources();
        if (null == resourceList) {
            return null;
        }
        // Sort the list
        Collections.sort(resourceList, Utility.resourceComparator);

        return resourceList;
    }

    public List<SingleResource> getSingleResourceList() {
        List<SingleResource> resourceList = data.getSingleResources();
        if (null == resourceList) {
            return null;
        }
        // Sort the list
        Collections.sort(resourceList, Utility.singleResourceComparator);

        return resourceList;
    }

    public void removeSingleResources(Set<SingleResource> resources)
            throws SimulatorException {
        if (null == resources) {
            return;
        }
        Iterator<SingleResource> itr = resources.iterator();
        while (itr.hasNext()) {
            removeResource(itr.next());
        }
    }

    public void removeResource(Resource res) throws SimulatorException {
        // Unregister the resource from the platform.
        SimulatorResource simRes = res.getSimulatorResource();
        try {
            simRes.stop();
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
            throw e;
        }

        // Delete this resource
        data.deleteResource(res);

        Activator
                .getDefault()
                .getLogManager()
                .log(Level.INFO.ordinal(), new Date(),
                        "Resource deleted [" + res.getResourceURI() + "].");
    }

    public boolean isUriUnique(List<MetaProperty> properties) {
        if (null == properties) {
            return false;
        }
        MetaProperty prop;
        Iterator<MetaProperty> itr = properties.iterator();
        while (itr.hasNext()) {
            prop = itr.next();
            if (prop.getPropName().equals(Constants.RESOURCE_URI)) {
                String uri = prop.getPropValue();
                return !data.isResourceExist(uri);
            }
        }
        return false;
    }

    public void resourceSelectionChanged(final Resource selectedResource) {
        new Thread() {
            @Override
            public void run() {
                if (null != selectedResource) {
                    setCurrentResourceInSelection(selectedResource);
                } else {
                    setCurrentResourceInSelection(null);
                }
                // Notify all observers for resource selection change event
                UiListenerHandler.getInstance()
                        .resourceSelectionChangedUINotification(
                                selectedResource);
            }
        }.start();
    }

    public List<MetaProperty> getMetaProperties(Resource resource) {
        if (null != resource) {
            String propName;
            StringBuilder propValue;
            List<MetaProperty> metaPropertyList = new ArrayList<MetaProperty>();

            for (int index = 0; index < Constants.META_PROPERTY_COUNT; index++) {
                propName = Constants.META_PROPERTIES[index];
                propValue = new StringBuilder();
                if (propName.equals(Constants.RESOURCE_NAME)) {
                    propValue.append(resource.getResourceName());
                } else if (propName.equals(Constants.RESOURCE_URI)) {
                    propValue.append(resource.getResourceURI());
                } else if (propName.equals(Constants.RESOURCE_TYPE)) {
                    propValue.append(resource.getResourceType());
                } else if (propName.equals(Constants.INTERFACE_TYPES)) {
                    Set<String> ifTypes = resource.getResourceInterfaces();
                    if (null != ifTypes && !ifTypes.isEmpty()) {
                        Iterator<String> itr = ifTypes.iterator();
                        while (itr.hasNext()) {
                            propValue.append(itr.next());
                            if (itr.hasNext()) {
                                propValue.append(", ");
                            }
                        }
                    } else {
                        propValue = null;
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

    public boolean startResource(Resource resource) throws SimulatorException {
        if (null == resource) {
            return false;
        }
        SimulatorResource server = resource.getSimulatorResource();
        if (null == server) {
            return false;
        }
        try {
            server.start();
            resource.setStarted(true);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "There is an error while starting the resource.\n"
                                    + Utility.getSimulatorErrorString(e, null));
            throw e;
        }
        return true;
    }

    public boolean stopResource(Resource resource) throws SimulatorException {
        if (null == resource) {
            return false;
        }
        SimulatorResource server = resource.getSimulatorResource();
        if (null == server) {
            return false;
        }
        try {
            server.stop();
            resource.setStarted(false);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "There is an error while stopping the resource.\n"
                                    + Utility.getSimulatorErrorString(e, null));
            throw e;
        }
        return true;
    }

    public boolean changeResourceName(Resource resource, String newName)
            throws SimulatorException {
        if (null == resource || null == newName) {
            return false;
        }

        if (!stopResource(resource)) {
            return false;
        }

        SimulatorResource server = resource.getSimulatorResource();
        try {
            server.setName(newName);
            resource.setResourceName(newName);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "There is an error while changing the resource name.\n"
                                    + Utility.getSimulatorErrorString(e, null));
            throw e;
        }

        if (!startResource(resource)) {
            return false;
        }

        return true;
    }

    public boolean changeResourceURI(Resource resource, String newURI)
            throws SimulatorException {
        if (null == resource || null == newURI) {
            return false;
        }

        if (!stopResource(resource)) {
            return false;
        }

        String curURI = resource.getResourceURI();
        setResourceURI(resource, newURI);

        try {
            if (!startResource(resource)) {
                return false;
            }
        } catch (SimulatorException e) {
            setResourceURI(resource, curURI);
        }

        return true;
    }

    public boolean changeResourceType(Resource resource, String newResourceType)
            throws SimulatorException {
        if (null == resource || null == newResourceType) {
            return false;
        }

        if (!stopResource(resource)) {
            return false;
        }

        String curResourceType = resource.getResourceType();
        setResourceType(resource, newResourceType);

        try {
            if (!startResource(resource)) {
                return false;
            }
        } catch (SimulatorException e) {
            setResourceType(resource, curResourceType);
        }

        return true;
    }

    public void setResourceURI(Resource resource, String newURI)
            throws SimulatorException {
        String curURI = resource.getResourceURI();
        SimulatorResource server = resource.getSimulatorResource();
        try {
            server.setURI(newURI);
            data.changeResourceURI(resource, curURI, newURI);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "There is an error while changing the resource URI.\n"
                                    + Utility.getSimulatorErrorString(e, null));
            throw e;
        }
    }

    public void setResourceType(Resource resource, String newResourceType)
            throws SimulatorException {
        SimulatorResource server = resource.getSimulatorResource();
        try {
            server.setResourceType(newResourceType);
            resource.setResourceType(newResourceType);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "There is an error while changing the resource Type.\n"
                                    + Utility.getSimulatorErrorString(e, null));
            throw e;
        }
    }

    public boolean updateResourceProperties(Resource resource,
            List<MetaProperty> properties, boolean uriChanged,
            boolean nameChanged, boolean resTypeChanged)
            throws SimulatorException {
        if (null == resource || null == properties) {
            return false;
        }

        // Updating the properties
        Iterator<MetaProperty> itr = properties.iterator();
        MetaProperty property;
        String propName;
        String propValue;
        String resName = null;
        String resURI = null;
        String resType = null;
        while (itr.hasNext()) {
            property = itr.next();
            if (null == property) {
                continue;
            }
            propName = property.getPropName();
            propValue = property.getPropValue();
            if (propName.equals(Constants.RESOURCE_NAME)) {
                resName = propValue;
            } else if (propName.equals(Constants.RESOURCE_URI)) {
                resURI = propValue;
            } else if (propName.equals(Constants.RESOURCE_TYPE)) {
                resType = propValue;
            }
        }

        if (nameChanged) {
            if (!changeResourceName(resource, resName)) {
                return false;
            }

            // Notify UI Listeners
            UiListenerHandler.getInstance().propertiesChangedUINotification(
                    Resource.class);
        }

        if (uriChanged) {
            if (!changeResourceURI(resource, resURI)) {
                return false;
            }
        }

        if (resTypeChanged) {
            if (!changeResourceType(resource, resType)) {
                return false;
            }
        }

        return true;
    }

    public boolean updateResourceInterfaces(Resource resource,
            Set<String> newIfSet) throws SimulatorException {
        if (null == resource || null == newIfSet || newIfSet.isEmpty()) {
            return false;
        }

        SimulatorResource jResource = resource.getSimulatorResource();
        if (null == jResource) {
            return false;
        }

        Set<String> curIfSet = resource.getResourceInterfaces();
        if (null == curIfSet || curIfSet.isEmpty()) {
            return false;
        }

        Iterator<String> itr = curIfSet.iterator();
        String interfaceType;
        boolean resourceRestartRequired = false;
        while (itr.hasNext()) {
            interfaceType = itr.next();
            if (!newIfSet.contains(interfaceType)) {
                resourceRestartRequired = true;
                break;
            }
        }

        try {
            // As there is no support from native layer for interface removal,
            // supporting it from the simulator requires restarting the resource
            // if any existing interfaces are removed.

            if (resourceRestartRequired) {
                stopResource(resource);
                jResource.setInterface(Utility
                        .convertSetToVectorString(newIfSet));
                startResource(resource);
            } else {
                // Existing interfaces are not removed.
                itr = newIfSet.iterator();
                while (itr.hasNext()) {
                    interfaceType = itr.next();
                    if (!curIfSet.contains(interfaceType)) {
                        jResource.addInterface(interfaceType);
                    }
                }
            }
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "There is an error while changing the interface types."
                                    + Utility.getSimulatorErrorString(e, null));
            throw e;
        }

        // Set the resource interfaces.
        resource.setResourceInterfaces(newIfSet);

        return true;
    }

    public boolean attributeValueUpdated(SingleResource resource,
            String attributeName, AttributeValue value) {
        if (null != resource && null != attributeName && null != value) {
            SimulatorSingleResource simRes = (SimulatorSingleResource) resource
                    .getSimulatorResource();
            if (null != simRes) {
                try {
                    simRes.updateAttribute(attributeName, value);
                    return true;
                } catch (SimulatorException e) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(), new Date(),
                                    Utility.getSimulatorErrorString(e, null));
                }
            }
        }
        return false;
    }

    public boolean isResourceStarted(Resource resource) {
        if (null == resource) {
            return false;
        }
        return resource.isStarted();
    }

    public boolean isPropertyValueInvalid(Resource resource,
            List<MetaProperty> properties, String propName) {
        if (null == resource || null == properties || null == propName) {
            return false;
        }
        boolean invalid = false;
        MetaProperty prop;
        Iterator<MetaProperty> itr = properties.iterator();
        while (itr.hasNext()) {
            prop = itr.next();
            if (prop.getPropName().equals(propName)) {
                String value = prop.getPropValue();
                if (propName.equals(Constants.RESOURCE_URI)) {
                    if (!Utility.isUriValid(value)) {
                        invalid = true;
                    }
                } else if (propName.equals(Constants.RESOURCE_TYPE)) {
                    if (!Utility.isResourceTypeValid(value)) {
                        invalid = true;
                    }
                } else {
                    if (null == value || value.trim().isEmpty()) {
                        invalid = true;
                    }
                }
            }
        }
        return invalid;
    }

    public boolean isPropValueChanged(Resource resource,
            List<MetaProperty> properties, String propName) {
        if (null == resource || null == properties || null == propName) {
            return false;
        }
        boolean changed = false;
        MetaProperty prop;
        String oldValue;
        Iterator<MetaProperty> itr = properties.iterator();
        while (itr.hasNext()) {
            prop = itr.next();
            if (prop.getPropName().equals(propName)) {
                oldValue = getPropertyValueFromResource(resource, propName);
                if (null != oldValue && !prop.getPropValue().equals(oldValue)) {
                    changed = true;
                }
                break;
            }
        }
        return changed;
    }

    private String getPropertyValueFromResource(Resource resource,
            String propName) {
        if (null == resource || null == propName) {
            return null;
        }
        if (propName.equals(Constants.RESOURCE_URI)) {
            return resource.getResourceURI();
        } else if (propName.equals(Constants.RESOURCE_NAME)) {
            return resource.getResourceName();
        } else if (propName.equals(Constants.RESOURCE_TYPE)) {
            return resource.getResourceType();
        } else {
            return null;
        }
    }

    public boolean isAttHasRangeOrAllowedValues(SimulatorResourceAttribute att) {
        if (null == att) {
            return false;
        }
        AttributeProperty prop = att.property();
        if (null == prop) {
            return false;
        }

        if (prop.getType() == Type.INTEGER) {
            IntegerProperty intProperty = prop.asInteger();
            if (null != intProperty) {
                return (intProperty.hasRange() || intProperty.hasValues());
            } else {
                return false;
            }
        } else if (prop.getType() == Type.DOUBLE) {
            DoubleProperty dblProperty = prop.asDouble();
            if (null != dblProperty) {
                return (dblProperty.hasRange() || dblProperty.hasValues());
            } else {
                return false;
            }
        } else if (prop.getType() == Type.STRING) {
            StringProperty stringProperty = prop.asString();
            if (null != stringProperty) {
                return stringProperty.hasValues();
            } else {
                return false;
            }
        }

        return true;
    }

    public int startAutomation(SingleResource resource,
            AttributeElement attribute, AutoUpdateType autoType,
            int autoUpdateInterval) {
        int autoId = -1;
        if (null != resource && null != attribute) {
            SimulatorSingleResource server = (SimulatorSingleResource) resource
                    .getSimulatorResource();
            if (null != server) {
                String attrName = attribute.getSimulatorResourceAttribute()
                        .name();
                try {
                    autoId = server.startAttributeUpdation(attrName, autoType,
                            autoUpdateInterval, automationListener);
                } catch (SimulatorException e) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(),
                                    new Date(),
                                    "[" + e.getClass().getSimpleName() + "]"
                                            + e.code().toString() + "-"
                                            + e.message());
                    return -1;
                }
                if (-1 != autoId) {
                    attribute.setAutoUpdateId(autoId);
                    attribute.setAutoUpdateType(autoType);
                    attribute.setAutoUpdateInterval(autoUpdateInterval);
                    attribute.setAutoUpdateState(true);
                    resource.setAttributeAutomationInProgress(true);
                }
            }
        }
        return autoId;
    }

    public void stopAutomation(SingleResource resource, AttributeElement att,
            int autoId) {
        if (null != resource) {
            SimulatorSingleResource server = (SimulatorSingleResource) resource
                    .getSimulatorResource();
            if (null != server) {
                try {
                    server.stopUpdation(autoId);
                } catch (SimulatorException e) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(),
                                    new Date(),
                                    "[" + e.getClass().getSimpleName() + "]"
                                            + e.code().toString() + "-"
                                            + e.message());
                    return;
                }
                // Change the automation status
                att.setAutoUpdateState(false);
                resource.setAttributeAutomationInProgress(isAnyAttributeInAutomation(resource));
            }
        }
    }

    public boolean startResourceAutomationUIRequest(AutoUpdateType autoType,
            int autoUpdateInterval, final SingleResource resource) {
        if (null == resource) {
            return false;
        }
        boolean status = false;
        // Invoke the native automation method
        SimulatorSingleResource resourceServer = (SimulatorSingleResource) resource
                .getSimulatorResource();
        if (null != resourceServer) {
            int autoId = -1;
            try {
                autoId = resourceServer.startResourceUpdation(autoType,
                        autoUpdateInterval, automationListener);
            } catch (SimulatorException e) {
                Activator
                        .getDefault()
                        .getLogManager()
                        .log(Level.ERROR.ordinal(), new Date(),
                                Utility.getSimulatorErrorString(e, null));
                autoId = -1;
            }
            if (-1 != autoId) {
                // Automation request accepted.
                changeResourceLevelAutomationStatus(resource, true);

                resource.setAutomationId(autoId);

                // Notify the UI listeners in a different thread.
                Thread notifyThread = new Thread() {
                    public void run() {
                        UiListenerHandler.getInstance()
                                .resourceAutomationStartedUINotification(
                                        resource);
                    };
                };
                notifyThread.setPriority(Thread.MAX_PRIORITY);
                notifyThread.start();

                status = true;
            }
        }
        return status;
    }

    public boolean stopResourceAutomationUIRequest(final SingleResource resource) {
        if (null == resource) {
            return false;
        }
        final int autoId = resource.getAutomationId();
        if (-1 == autoId) {
            return false;
        }
        SimulatorSingleResource resourceServer = (SimulatorSingleResource) resource
                .getSimulatorResource();
        if (null == resourceServer) {
            return false;
        }
        // Call native method
        try {
            resourceServer.stopUpdation(autoId);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
            return false;
        }

        // Notify the UI Listeners. Invoke the automation complete callback.
        Thread stopThread = new Thread() {
            public void run() {
                automationListener.onUpdateComplete(resource.getResourceURI(),
                        autoId);
            }
        };
        stopThread.start();
        return true;
    }

    private boolean isAnyAttributeInAutomation(SingleResource resource) {
        if (null == resource || null == resource.getResourceRepresentation()) {
            return false;
        }

        Map<String, AttributeElement> attributes = resource
                .getResourceRepresentation().getAttributes();
        if (null == attributes || 0 == attributes.size())
            return false;

        for (Map.Entry<String, AttributeElement> entry : attributes.entrySet()) {
            if (entry.getValue().isAutoUpdateInProgress())
                return true;
        }

        return false;
    }

    // Changes the automation state of the resource and its attributes
    private void changeResourceLevelAutomationStatus(SingleResource resource,
            boolean status) {

        if (null == resource || null == resource.getResourceRepresentation()) {
            return;
        }

        Map<String, AttributeElement> attributes = resource
                .getResourceRepresentation().getAttributes();
        if (null == attributes || 0 == attributes.size())
            return;

        for (Map.Entry<String, AttributeElement> entry : attributes.entrySet()) {
            entry.getValue().setAutoUpdateState(status);
        }

        resource.setResourceAutomationInProgress(status);
    }

    private AttributeElement getAttributeWithGivenAutomationId(
            SingleResource resource, int automationId) {
        if (null == resource || null == resource.getResourceRepresentation()) {
            return null;
        }

        Map<String, AttributeElement> attributes = resource
                .getResourceRepresentation().getAttributes();
        if (null == attributes || 0 == attributes.size())
            return null;

        for (Map.Entry<String, AttributeElement> entry : attributes.entrySet()) {
            if (automationId == entry.getValue().getAutoUpdateId())
                return entry.getValue();
        }

        return null;
    }

    public boolean isResourceAutomationStarted(SingleResource resource) {
        boolean status = false;
        if (null != resource) {
            status = resource.isResourceAutomationInProgress();
        }
        return status;
    }

    public boolean isAttributeAutomationStarted(SingleResource resource) {
        if (null == resource) {
            return false;
        }
        return resource.isAttributeAutomationInProgress();
    }

    public void notifyObserverRequest(Resource resource, int observerId) {
        if (null == resource) {
            return;
        }
        SimulatorResource simulatorResource = resource.getSimulatorResource();
        if (null == simulatorResource) {
            return;
        }
        try {
            simulatorResource.notifyObserver(observerId);
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(),
                            Utility.getSimulatorErrorString(e, null));
        }
    }

    public List<String> getAllValuesOfAttribute(SimulatorResourceAttribute att) {
        if (null == att) {
            return null;
        }

        AttributeValue value = att.value();
        if (null == value || null == value.get()) {
            return null;
        }

        TypeInfo type = value.typeInfo();

        if (type.mBaseType == ValueType.RESOURCEMODEL) {
            return null;
        }

        List<String> values = new ArrayList<String>();

        AttributeProperty prop = att.property();
        if (null == prop) {
            // Adding the current value of the attribute.
            values.add(new AttributeValueStringConverter(value).toString());
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

    public int getResourceCount() {
        return data.getResourceCount();
    }

    public void shutdown() {
        threadHandle.interrupt();
    }
}