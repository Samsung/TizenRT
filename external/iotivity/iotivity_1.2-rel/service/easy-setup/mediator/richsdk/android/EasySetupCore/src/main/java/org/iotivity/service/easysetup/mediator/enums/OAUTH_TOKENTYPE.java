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
 * This enum class indicates an OAuth Token type like "bearer" and "mac"
 */
public enum OAUTH_TOKENTYPE
{
    NONE_OAUTH_TOKENTYPE(0),
    OAUTH_TOKENTYPE_BEARER(1),
    OAUTH_TOKENTYPE_MAC(2);

    private int value;

    private OAUTH_TOKENTYPE(int value)
    {
        this.value = value;
    }

    /**
     * Get OAuth Token type as an integer value
     *
     * @return int OAuth Token type as an integer value
     */
    public int getValue()
    {
        return value;
    }

    /**
     * Get OAuth Token type as OAUTH_TOKENTYPE type value
     *
     * @return OAuth Token type enum value corresponding to its integer value
     */
    public static OAUTH_TOKENTYPE fromInt(int i)
    {
        for (OAUTH_TOKENTYPE b : OAUTH_TOKENTYPE.values())
        {
            if (b.getValue() == i)
                return b;
        }
        return null;
    }
}

