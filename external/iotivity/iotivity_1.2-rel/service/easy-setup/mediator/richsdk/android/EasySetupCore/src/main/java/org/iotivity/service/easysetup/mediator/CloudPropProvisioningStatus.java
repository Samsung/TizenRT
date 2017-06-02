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

package org.iotivity.service.easysetup.mediator;

import org.iotivity.service.easysetup.mediator.enums.ESCloudProvState;
import org.iotivity.service.easysetup.mediator.enums.ESResult;

/**
 * This interface facilitates Application to get progress & result of Cloud provisioning
 * process in easy setup
 */
public class CloudPropProvisioningStatus
{
    private ESResult m_result;

    /**
     * Constructor
     *
     * @param result a result of cloud property provisioning
     * @param state a state of cloud property provisioning.
     *              ES_CLOUD_PROVISIONING_ERROR(-1), ES_CLOUD_PROVISIONING_SUCCESS(0),
     *              ES_CLOUD_ENROLLEE_FOUND(1), ES_CLOUD_ENROLLEE_NOT_FOUND(2)
     *
     * @see ESCloudProvState
     */
    public CloudPropProvisioningStatus(int result)
    {
        m_result = ESResult.fromInt(result);
    }

    /**
     * Get a result of cloud property provisioning
     *
     * @result ESResult a result of cloud property provisioning
     */
    public ESResult getESResult()
    {
        return m_result;
    }
};
