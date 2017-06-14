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

package oic.simulator.clientcontroller.remoteresource;

import java.util.Map;

import org.oic.simulator.SimulatorResourceModel;
import org.oic.simulator.client.SimulatorRemoteResource;
import org.oic.simulator.client.SimulatorRemoteResource.RequestType;
import org.oic.simulator.client.SimulatorRequestModel;

/**
 * This class represents a remote resource. It maintains all the necessary
 * information about the resource.
 */
public class RemoteResource {

    private boolean                                 observed;

    // Native object references
    private SimulatorRemoteResource                 remoteResourceRef;
    private SimulatorResourceModel                  resourceModelRef;
    private Map<RequestType, SimulatorRequestModel> requestModels;
    private ResourceRepresentation                  mResourceRepresentation;

    private boolean                                 configUploaded;

    private boolean                                 getAutomtnInProgress;
    private boolean                                 putAutomtnInProgress;
    private boolean                                 postAutomtnInProgress;

    private int                                     getAutomtnId;
    private int                                     putAutomtnId;
    private int                                     postAutomtnId;

    private boolean                                 isFavorite;

    public SimulatorResourceModel getResourceModelRef() {
        return resourceModelRef;
    }

    public void setResourceModelRef(SimulatorResourceModel resourceModel) {
        this.resourceModelRef = resourceModel;
    }

    public Map<RequestType, SimulatorRequestModel> getRequestModels() {
        return requestModels;
    }

    public void setRequestModels(
            Map<RequestType, SimulatorRequestModel> requestModels) {
        this.requestModels = requestModels;
    }

    public int getGetAutomtnId() {
        return getAutomtnId;
    }

    public void setGetAutomtnId(int getAutomtnId) {
        this.getAutomtnId = getAutomtnId;
    }

    public int getPutAutomtnId() {
        return putAutomtnId;
    }

    public void setPutAutomtnId(int putAutomtnId) {
        this.putAutomtnId = putAutomtnId;
    }

    public int getPostAutomtnId() {
        return postAutomtnId;
    }

    public void setPostAutomtnId(int postAutomtnId) {
        this.postAutomtnId = postAutomtnId;
    }

    public boolean isGetAutomtnInProgress() {
        return getAutomtnInProgress;
    }

    public void setGetAutomtnInProgress(boolean getAutomtnInProgress) {
        this.getAutomtnInProgress = getAutomtnInProgress;
    }

    public boolean isPutAutomtnInProgress() {
        return putAutomtnInProgress;
    }

    public void setPutAutomtnInProgress(boolean putAutomtnInProgress) {
        this.putAutomtnInProgress = putAutomtnInProgress;
    }

    public boolean isPostAutomtnInProgress() {
        return postAutomtnInProgress;
    }

    public void setPostAutomtnInProgress(boolean postAutomtnInProgress) {
        this.postAutomtnInProgress = postAutomtnInProgress;
    }

    public boolean isConfigUploaded() {
        return configUploaded;
    }

    public void setConfigUploaded(boolean configUploaded) {
        this.configUploaded = configUploaded;
    }

    public SimulatorRemoteResource getRemoteResourceRef() {
        return remoteResourceRef;
    }

    public void setRemoteResourceRef(SimulatorRemoteResource resource) {
        this.remoteResourceRef = resource;
    }

    public boolean isObserved() {
        return observed;
    }

    public void setObserved(boolean observed) {
        this.observed = observed;
    }

    public RequestType getAutomationtype(int autoId) {
        if (getAutomtnId == autoId) {
            return RequestType.GET;
        } else if (putAutomtnId == autoId) {
            return RequestType.PUT;
        } else {
            return RequestType.POST;
        }
    }

    public void updateAutomationStatus(int autoId, boolean status) {
        if (getAutomtnId == autoId) {
            getAutomtnInProgress = status;
        } else if (putAutomtnId == autoId) {
            putAutomtnInProgress = status;
        } else {
            postAutomtnInProgress = status;
        }
    }

    public boolean isFavorite() {
        return isFavorite;
    }

    public void setFavorite(boolean isFavorite) {
        this.isFavorite = isFavorite;
    }

    public void setResourceRepresentation(SimulatorResourceModel resModel) {
        mResourceRepresentation = new ResourceRepresentation(resModel);
    }

    public ResourceRepresentation getResourceRepresentation() {
        return mResourceRepresentation;
    }
}
