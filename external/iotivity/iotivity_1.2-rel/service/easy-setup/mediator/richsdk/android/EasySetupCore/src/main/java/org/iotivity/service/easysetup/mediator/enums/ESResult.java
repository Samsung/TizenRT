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
 * It defines an result during easy setup process, which is same as one in Easy Setup C++ SDK
 */
public enum ESResult {

    ES_OK(0),
    ES_ENROLLEE_DISCOVERY_FAILURE(11),
    ES_COMMUNICATION_ERROR(12),
    ES_SEC_OPERATION_IS_NOT_SUPPORTED(20),
    ES_SECURE_RESOURCE_DISCOVERY_FAILURE(21),
    ES_OWNERSHIP_TRANSFER_FAILURE(22),
    ES_ACL_PROVISIONING_FAILURE(23),
    ES_CERT_PROVISIONING_FAILURE(24),
    ES_ERROR(255);

    private int value;

    private ESResult(int value) {
        this.value = value;
    }

	public int getValue() {
        return value;
    }

	public static ESResult fromInt(int i) {
        for (ESResult b : ESResult.values()) {
            if (b.getValue() == i) { return b; }
        }
        return null;
    }
};