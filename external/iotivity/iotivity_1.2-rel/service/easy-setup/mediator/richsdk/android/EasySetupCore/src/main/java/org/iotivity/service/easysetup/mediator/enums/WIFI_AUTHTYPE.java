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
 * This enum class indicates a WiFi authentication type
 */
public enum WIFI_AUTHTYPE
{
    NONE_AUTH(0),
    WEP(1),
    WPA_PSK(2),
    WPA2_PSK(3);

    private int value;

    private WIFI_AUTHTYPE(int value)
    {
        this.value = value;
    }

    public int getValue()
    {
        return value;
    }

    public static WIFI_AUTHTYPE fromInt(int i)
    {
        for (WIFI_AUTHTYPE b : WIFI_AUTHTYPE.values())
        {
            if (b.getValue() == i)
                return b;
        }
        return null;
    }
}
