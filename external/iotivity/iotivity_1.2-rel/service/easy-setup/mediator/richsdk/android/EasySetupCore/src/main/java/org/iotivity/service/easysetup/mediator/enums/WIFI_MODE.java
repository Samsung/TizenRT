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
 * This enum class indicates a WiFi mode like 11A and 11N.
 */
public enum WIFI_MODE
{
    /**
     * 801.11A
     */
    WIFI_11A(0),

    /**
     * 801.11B
     */
    WIFI_11B(1),

    /**
     * 801.11G
     */
    WIFI_11G(2),

    /**
     * 801.11N
     */
    WIFI_11N(3),

    /**
     * 801.11AC
     */
    WIFI_11AC(4);

    private int value;

    private WIFI_MODE(int value)
    {
        this.value = value;
    }

    /**
     * Get WiFi mode as an integer value
     *
     * @return int WiFi mode as an integer value
     */
    public int getValue()
    {
        return value;
    }

    /**
     * Convert integer to WIFI_MODE enum value
     *
     * @param i An integer value converting
     * @return WIFI_MODE enum value corresponding to its integer value
     */
    public static WIFI_MODE fromInt(int i)
    {
        for (WIFI_MODE b : WIFI_MODE.values())
        {
            if (b.getValue() == i)
                return b;
        }
        return null;
    }
}
