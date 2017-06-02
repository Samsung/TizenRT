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
 * This class indicates a result if a getConfiguartion API succeed or fails and
 * Enrollee's configuration delivered by a response.
 */
public class GetConfigurationStatus
{
    private ESResult mResult;
    private EnrolleeConf mEnrolleeConf;

    /**
     * Constructor
     *
     * @param result a result if a getConfiguartion API succeed or fail
     * @param conf Enrollee's configuration
     *
     */
    public GetConfigurationStatus(int result, EnrolleeConf conf)
    {
        mResult = ESResult.fromInt(result);
        mEnrolleeConf = conf;
    }

    /**
     * Get a result of a getConfiguartion API call
     *
     * @return ESResult
     *
     */
    public ESResult getESResult()
    {
        return mResult;
    }

    /**
     * Get Enrollee's configuration
     *
     * @return EnrolleeConf includes a WiFi and Device configuration of Enrollee
     *
     * @see EnrolleeConf
     */
    public EnrolleeConf getEnrolleeConf()
    {
        return mEnrolleeConf;
    }
};
