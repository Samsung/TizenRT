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

import org.iotivity.service.easysetup.mediator.enums.ESResult;

/**
 * This class indicates a result if a provisionDeviceProperties API succeed or fails
 */
public class DevicePropProvisioningStatus
{
    private ESResult m_result;

    /**
     * Constructor
     *
     * @param result a result if a provisionDeviceProperties API succeed or fails
     */
    public DevicePropProvisioningStatus(int result)
    {
        m_result = ESResult.fromInt(result);
    }

    /**
     * Get a result if a provisionDeviceProperties API succeed or fails
     *
     * @return ESResult a result if a provisionDeviceProperties API succeed or fails
     */
    public ESResult getESResult()
    {
        return m_result;
    }
};
