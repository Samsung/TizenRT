/**
 * ***************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
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
 * This enum class indicates a WiFi frequency like 2.4G and 5G
 */
public enum WIFI_FREQ
{
    WIFI_24G(0),
    WIFI_5G(1),
    WIFI_BOTH(2),
    WIFI_FREQ_NONE(999);

    private int value;

    private WIFI_FREQ(int value)
    {
        this.value = value;
    }

    /**
     * Get WiFi frequency as an integer value
     *
     * @return int WiFi freq. as an integer value
     */
    public int getValue()
    {
        return value;
    }

    /**
     * Get WiFi frequency as an integer value
     *
     * @return WIFI_FREQ enum value corresponding to its integer value
     */
    public static WIFI_FREQ fromInt(int i)
    {
        for (WIFI_FREQ b : WIFI_FREQ.values())
        {
            if (b.getValue() == i)
                return b;
        }
        return null;
    }
}
