/**
 * ***************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ****************************************************************
 */

package org.iotivity.service.easysetup.mediator.enums;

/**
 * It defines various states of the cloud provisioning during easy setup process
 */
public enum ESCloudProvState {

    /**
     * Some error occurs during cloud data provisioning
     */
    ES_CLOUD_PROVISIONING_ERROR(-1),

    /**
     * Cloud data provisioning is successfully done
     */
    ES_CLOUD_PROVISIONING_SUCCESS(0),

    /**
     * Target enrollee which needs a cloud provisioning is found in a network
     */
    ES_CLOUD_ENROLLEE_FOUND(1),

    /**
     * Target enrollee which needs a cloud provisioning is NOT found in a network
     */
    ES_CLOUD_ENROLLEE_NOT_FOUND(2);

    private int value;

    private ESCloudProvState(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }

    public static ESCloudProvState fromInt(int i) {
        for (ESCloudProvState b : ESCloudProvState.values()) {
            if (b.getValue() == i) { return b; }
        }
        return null;
    }
};

