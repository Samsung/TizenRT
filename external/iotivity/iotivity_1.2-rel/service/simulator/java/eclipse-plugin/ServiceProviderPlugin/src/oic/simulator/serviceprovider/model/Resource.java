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

package oic.simulator.serviceprovider.model;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import org.oic.simulator.SimulatorResourceAttribute;
import org.oic.simulator.SimulatorResourceModel;
import org.oic.simulator.server.Observer;
import org.oic.simulator.server.SimulatorResource;

public abstract class Resource {

    // Java SDK object reference
    SimulatorResource                    simulatorResource;
    private SimulatorResourceModel       resourceModel;
    private ResourceRepresentation       mResourceRepresentation;

    private String                       resourceURI;
    private String                       resourceName;
    private String                       resourceType;
    private Set<String>                  resourceInterfaces;
    private boolean                      started;
    private boolean                      observable;
    private boolean                      discoverable;

    private Map<Integer, ObserverDetail> observers;

    public Resource() {
        resourceInterfaces = new HashSet<String>();
        observers = new HashMap<Integer, ObserverDetail>();
    }

    public SimulatorResource getSimulatorResource() {
        return simulatorResource;
    }

    public void setSimulatorResource(SimulatorResource simulatorResource) {
        this.simulatorResource = simulatorResource;
    }

    public SimulatorResourceModel getResourceModel() {
        return resourceModel;
    }

    public void setResourceModel(SimulatorResourceModel resourceModel) {
        this.resourceModel = resourceModel;
    }

    public String getResourceURI() {
        return resourceURI;
    }

    public void setResourceURI(String resourceURI) {
        this.resourceURI = resourceURI;
    }

    public String getResourceName() {
        return resourceName;
    }

    public void setResourceName(String resourceName) {
        this.resourceName = resourceName;
    }

    public String getResourceType() {
        return resourceType;
    }

    public void setResourceType(String resourceType) {
        this.resourceType = resourceType;
    }

    public Set<String> getResourceInterfaces() {
        if (null != resourceInterfaces) {
            return new HashSet<String>(resourceInterfaces);
        }
        return null;
    }

    public void setResourceInterfaces(Set<String> resourceInterfaces) {
        this.resourceInterfaces = resourceInterfaces;
    }

    public boolean isStarted() {
        return started;
    }

    public void setStarted(boolean started) {
        this.started = started;
    }

    public boolean isObservable() {
        return observable;
    }

    public void setObservable(boolean observable) {
        this.observable = observable;
    }

    public boolean isDiscoverable() {
        return discoverable;
    }

    public void setDiscoverable(boolean discoverable) {
        this.discoverable = discoverable;
    }

    public void addInterfaceType(String ifType) {
        if (null == ifType) {
            return;
        }
        if (null == resourceInterfaces) {
            resourceInterfaces = new HashSet<String>();
        }
        resourceInterfaces.add(ifType);
    }

    public void removeInterfaceType(String ifType) {
        if (null == ifType || null == resourceInterfaces) {
            return;
        }
        resourceInterfaces.remove(ifType);
    }

    public boolean isInterfaceTypeExist(String ifType) {
        if (null == ifType || null == resourceInterfaces) {
            return false;
        }
        return resourceInterfaces.contains(ifType);
    }

    public Map<Integer, ObserverDetail> getObserver() {
        return observers;
    }

    public void addObserverInfo(Observer observer) {
        if (null == observer) {
            return;
        }
        int id = observer.getId();
        if (!observers.containsKey(id)) {
            ObserverDetail obsDetail = new ObserverDetail();
            obsDetail.setObserverInfo(observer);
            observers.put(id, obsDetail);
        }
    }

    public void removeObserverInfo(Observer observer) {
        if (null == observer) {
            return;
        }
        observers.remove(observer.getId());
    }

    public void createResourceRepresentation(
            Map<String, SimulatorResourceAttribute> attributes)
            throws NullPointerException {
        if (mResourceRepresentation == null)
            mResourceRepresentation = new ResourceRepresentation(attributes);
    }

    public void updateResourceRepresentation(SimulatorResourceModel model)
            throws NullPointerException {
        mResourceRepresentation.update(model);
    }

    public ResourceRepresentation getResourceRepresentation() {
        return mResourceRepresentation;
    }
}
