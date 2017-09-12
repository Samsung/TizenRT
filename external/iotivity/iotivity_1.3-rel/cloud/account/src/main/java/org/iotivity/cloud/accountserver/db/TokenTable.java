/*
 * //******************************************************************
 * //
 * // Copyright 2016 Samsung Electronics All Rights Reserved.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * //
 * // Licensed under the Apache License, Version 2.0 (the "License");
 * // you may not use this file except in compliance with the License.
 * // You may obtain a copy of the License at
 * //
 * //      http://www.apache.org/licenses/LICENSE-2.0
 * //
 * // Unless required by applicable law or agreed to in writing, software
 * // distributed under the License is distributed on an "AS IS" BASIS,
 * // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * // See the License for the specific language governing permissions and
 * // limitations under the License.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */
package org.iotivity.cloud.accountserver.db;

import org.iotivity.cloud.accountserver.Constants;

/**
 *
 * This class provides a set of APIs to store session information of authorized
 * users.
 *
 */
public class TokenTable {

    private String uuid         = null;
    private String did          = null;
    private String accesstoken  = null;
    private String refreshtoken = null;
    private String provider     = null;
    private long   expiredtime  = Constants.TOKEN_INFINITE;
    private String issuedtime   = null;

    /**
     * API to get user ID
     * 
     * @return user ID
     */
    public String getUuid() {
        return uuid;
    }

    /**
     * API to set user ID
     * 
     * @param uuid
     *            user ID to be registered
     */
    public void setUuid(Object uuid) {
        this.uuid = uuid.toString();
    }

    /**
     * API to get device ID
     * 
     * @return device ID
     */
    public String getDid() {
        return did;
    }

    /**
     * API to set device ID
     * 
     * @param did
     *            device ID to be registered
     */
    public void setDid(String did) {
        this.did = did;
    }

    /**
     * API to get access token
     * 
     * @return access token
     */
    public String getAccesstoken() {
        return accesstoken;
    }

    /**
     * API to set the access token to the DB
     * 
     * @param accesstoken
     *            access token to be registered
     */
    public void setAccesstoken(Object accesstoken) {
        if (accesstoken != null)
            this.accesstoken = accesstoken.toString();
    }

    /**
     * API to get the refresh token from the DB
     * 
     * @return refresh token
     */
    public String getRefreshtoken() {
        return refreshtoken;
    }

    /**
     * API to set the refresh token to the DB
     * 
     * @param refreshtoken
     *            refresh token to be registered
     */
    public void setRefreshtoken(Object refreshtoken) {
        if (refreshtoken != null)
            this.refreshtoken = refreshtoken.toString();
    }

    /**
     * API to get the auth provider name
     * 
     * @return auth provider name
     */
    public String getProvider() {
        return provider;
    }

    /**
     * API to set the auth provider name to the DB
     * 
     * @param provider
     *            auth provider name to be registered
     */
    public void setProvider(Object provider) {
        if (provider != null)
            this.provider = provider.toString();
    }

    /**
     * API to get the expiration time of the access token
     * 
     * @return time remain
     */
    public long getExpiredtime() {
        return expiredtime;
    }

    /**
     * API to set the expiration time of the access token
     * 
     * @param expiredtime
     *            expiration time of the access token
     */
    public void setExpiredtime(Object expiredtime) {
        if (expiredtime != null)
            this.expiredtime = Long.valueOf(expiredtime.toString());
    }

    /**
     * API to get the issued time of the token
     * 
     * @return issued time
     */
    public String getIssuedtime() {
        return issuedtime;
    }

    /**
     * API to get the issued time of the token
     * 
     * @param issuedtime
     *            issued time to be registered
     */
    public void setIssuedtime(Object issuedtime) {
        if (issuedtime != null)
            this.issuedtime = issuedtime.toString();
    }
}