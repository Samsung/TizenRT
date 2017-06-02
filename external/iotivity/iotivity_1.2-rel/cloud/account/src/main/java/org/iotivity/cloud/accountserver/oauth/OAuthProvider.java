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
package org.iotivity.cloud.accountserver.oauth;

import org.iotivity.cloud.accountserver.db.TokenTable;
import org.iotivity.cloud.accountserver.db.UserTable;

/**
 *
 * This class provides abstraction of APIs relating authorization for OAuth 2.0
 *
 */
public abstract interface OAuthProvider {

    /**
     * API to request access token
     * 
     * @param authCode
     *            authorization code
     * @param options
     *            option field of region authserver url, apiserver url
     * @return token information or error message if error occurs
     */
    public abstract TokenTable requestAccessTokenInfo(String authCode,
            Object options);

    /**
     * API to request refresh token
     * 
     * @param refreshToken
     *            Refresh token used to refresh the access token in cloud before
     *            getting expired
     * @return token information or error message if error occurs
     */
    public abstract TokenTable requestRefreshTokenInfo(String refreshToken);

    /**
     * API to get user information
     * 
     * @param accessToken
     *            access token
     * @param options
     *            option field of region authserver url, apiserver url
     * @return user information or error message if error occurs
     */
    public abstract UserTable requestGetUserInfo(String accessToken,
            Object options);
}
