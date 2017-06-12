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

import org.iotivity.service.easysetup.mediator.CloudPropProvisioningStatus;

/**
 * This interface class is used as a callback function called after receiving
 * a response of cloud property provisioning result
 *
 * @see CloudPropProvisioningStatus
 */
public abstract class CloudPropProvisioningCallback {

    /**
     * Called after receiving a response of cloud property provisioning result
     *
     * @param status
     *          a result of cloud property provisioning
     *          a state of cloud property provisioning
     */
	public abstract void onProgress(CloudPropProvisioningStatus status);
}

