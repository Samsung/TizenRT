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

/**
 * This interface class is used as a callback function called after receiving
 * Enrollee's configuration.
 *
 * @see GetConfigurationStatus
 */
public abstract class GetConfigurationCallback {

    /**
     * Called after receiving Enrollee's configuration
     *
     * @param status
     *          result if the request succeeds or fails
     *          Enrollee's configuration like supported WiFi freq and device name
     */
    public abstract void onProgress(GetConfigurationStatus status);
}
