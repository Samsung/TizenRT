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

import org.oic.simulator.server.SimulatorResource.AutoUpdateType;

/**
 * This class represents a simulated resource. It maintains all the necessary
 * information about the resource.
 */
public class SingleResource extends Resource {
    private boolean        getAllowed;

    private boolean        putAllowed;

    private boolean        postAllowed;

    private int            automationId;

    private boolean        resourceAutomationInProgress;

    private boolean        attributeAutomationInProgress;

    private int            automationUpdateInterval;

    private AutoUpdateType automationType;

    public boolean isGetAllowed() {
        return getAllowed;
    }

    public void setGetAllowed(boolean getAllowed) {
        this.getAllowed = getAllowed;
    }

    public boolean isPutAllowed() {
        return putAllowed;
    }

    public void setPutAllowed(boolean putAllowed) {
        this.putAllowed = putAllowed;
    }

    public boolean isPostAllowed() {
        return postAllowed;
    }

    public void setPostAllowed(boolean postAllowed) {
        this.postAllowed = postAllowed;
    }

    public int getAutomationUpdateInterval() {
        return automationUpdateInterval;
    }

    public void setAutomationUpdateInterval(int automationUpdateInterval) {
        this.automationUpdateInterval = automationUpdateInterval;
    }

    public AutoUpdateType getAutomationType() {
        return automationType;
    }

    public void setAutomationType(AutoUpdateType automationType) {
        this.automationType = automationType;
    }

    public int getAutomationId() {
        return automationId;
    }

    public void setAutomationId(int automationId) {
        this.automationId = automationId;
    }

    public boolean isResourceAutomationInProgress() {
        return resourceAutomationInProgress;
    }

    public void setResourceAutomationInProgress(
            boolean resourceAutomationInProgress) {
        this.resourceAutomationInProgress = resourceAutomationInProgress;
    }

    public boolean isAttributeAutomationInProgress() {
        return attributeAutomationInProgress;
    }

    public void setAttributeAutomationInProgress(
            boolean attributeAutomationInProgress) {
        this.attributeAutomationInProgress = attributeAutomationInProgress;
    }
}