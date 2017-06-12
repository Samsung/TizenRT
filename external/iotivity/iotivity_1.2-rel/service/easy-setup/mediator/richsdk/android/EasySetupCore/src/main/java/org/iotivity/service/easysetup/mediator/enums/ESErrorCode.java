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
 * @brief Indicate last error code to describe a reason of error during easy setup.
 */
public enum ESErrorCode {

    /**
     * Init Error Code
     */
    ES_ERRCODE_NO_ERROR(0),

    /**
     * Error Code that given WiFi's SSID is not found
     */
    ES_ERRCODE_SSID_NOT_FOUND(1),

    /**
     * Error Code that given WiFi's Password is wrong
     */
    ES_ERRCODE_PW_WRONG(2),

    /**
     * Error Code that IP address is not allocated
     */
    ES_ERRCODE_IP_NOT_ALLOCATED(3),

    /**
     * Error Code that there is no Internet connection
     */
    ES_ERRCODE_NO_INTERNETCONNECTION(4),

    /**
     * Error Code that Timeout occured
     */
    ES_ERRCODE_TIMEOUT(5),

	/**
     * Error Code that cloud server is not reachable due to wrong URL of cloud server, for example.
     */
    ES_ERRCODE_FAILED_TO_ACCESS_CLOUD_SERVER(6),

    /**
     * Error Code that no response is arrived from cloud server
     */
    ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER(7),

    /**
     * Error Code that a delivered authcode is not valid.
     */
    ES_ERRCODE_INVALID_AUTHCODE(8),

    /**
     * Error Code that a given access token is not valid due to its expiration, for example.
     */
    ES_ERRCODE_INVALID_ACCESSTOKEN(9),

    /**
     * Error Code that a refresh of expired access token is failed due to some reasons.
     */
    ES_ERRCODE_FAILED_TO_REFRESH_ACCESSTOKEN(10),

    /**
     * Error Code that a target device is not discovered in cloud server
     */
    ES_ERRCODE_FAILED_TO_FIND_REGISTERED_DEVICE_IN_CLOUD(11),

    /**
     * Error Code that a target user does not exist in cloud server.
     */
    ES_ERRCODE_FAILED_TO_FIND_REGISTERED_USER_IN_CLOUD(12),

    /**
     * Error Code that Unknown error occured
     */
    ES_ERRCODE_UNKNOWN(255);

    private int value;

    private ESErrorCode(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }

    public static ESErrorCode fromInt(int i) {
        for (ESErrorCode b : ESErrorCode.values()) {
            if (b.getValue() == i) { return b; }
        }
        return null;
    }
};

