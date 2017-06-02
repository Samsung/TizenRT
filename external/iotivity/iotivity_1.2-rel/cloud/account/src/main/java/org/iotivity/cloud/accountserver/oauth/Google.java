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

import java.util.HashMap;

import org.apache.oltu.oauth2.client.OAuthClient;
import org.apache.oltu.oauth2.client.URLConnectionClient;
import org.apache.oltu.oauth2.client.request.OAuthBearerClientRequest;
import org.apache.oltu.oauth2.client.request.OAuthClientRequest;
import org.apache.oltu.oauth2.client.response.OAuthAccessTokenResponse;
import org.apache.oltu.oauth2.client.response.OAuthJSONAccessTokenResponse;
import org.apache.oltu.oauth2.client.response.OAuthResourceResponse;
import org.apache.oltu.oauth2.common.OAuth;
import org.apache.oltu.oauth2.common.OAuthProviderType;
import org.apache.oltu.oauth2.common.exception.OAuthProblemException;
import org.apache.oltu.oauth2.common.exception.OAuthSystemException;
import org.apache.oltu.oauth2.common.message.types.GrantType;
import org.iotivity.cloud.accountserver.db.TokenTable;
import org.iotivity.cloud.accountserver.db.UserTable;
import org.iotivity.cloud.base.exception.ServerException.InternalServerErrorException;
import org.iotivity.cloud.util.JSONUtil;
import org.iotivity.cloud.util.Log;;

/**
 *
 * This class provides a sample to make .jar that communicates with OAuth2
 * provider.
 *
 */
public class Google implements OAuthProvider {

    // do not use 'client_id' and 'secret' variables.
    // should use values that are obtained from github.
    final static private String client_id    = "447649044559-f9r5sl6op3kkk0312u384o4g6hhucje1.apps.googleusercontent.com";
    final static private String secret       = "LyTe5_EQkv8-v9Zbq20PSCLR";
    final static private String redirect_url = "http://www.example.com/oauth2callback";
    final static private String resource_url = "https://www.googleapis.com/userinfo/v2/me";

    @Override
    public TokenTable requestAccessTokenInfo(String authCode, Object options) {

        TokenTable tokenInfo = new TokenTable();

        if (authCode == null) {

            Log.w("authCode is null!");
            return tokenInfo;
        }

        try {

            OAuthClientRequest request = OAuthClientRequest
                    .tokenProvider(OAuthProviderType.GOOGLE)
                    .setGrantType(GrantType.AUTHORIZATION_CODE)
                    .setClientId(client_id).setClientSecret(secret)
                    .setCode(authCode).setRedirectURI(redirect_url)
                    .buildBodyMessage();

            OAuthClient oauthClient = new OAuthClient(
                    new URLConnectionClient());

            OAuthAccessTokenResponse oauthResponse = null;
            Class<? extends OAuthAccessTokenResponse> cl = OAuthJSONAccessTokenResponse.class;

            oauthResponse = oauthClient.accessToken(request, cl);

            Log.d("OAuth response: " + oauthResponse.getBody());

            tokenInfo.setAccesstoken(oauthResponse.getAccessToken());
            tokenInfo.setRefreshtoken(oauthResponse.getRefreshToken());
            tokenInfo.setExpiredtime(oauthResponse.getExpiresIn());

        } catch (OAuthSystemException | OAuthProblemException e) {

            e.printStackTrace();
            throw new InternalServerErrorException(
                    "OAuth provider(Google) error");
        }

        return tokenInfo;
    }

    @Override
    public TokenTable requestRefreshTokenInfo(String refreshToken) {

        TokenTable tokenInfo = new TokenTable();

        if (refreshToken == null) {

            Log.w("refreshToken is null!");
            return tokenInfo;
        }

        try {

            OAuthClientRequest request = OAuthClientRequest
                    .tokenProvider(OAuthProviderType.GOOGLE)
                    .setGrantType(GrantType.REFRESH_TOKEN)
                    .setClientId(client_id).setClientSecret(secret)
                    .setRefreshToken(refreshToken).buildBodyMessage();

            OAuthClient oauthClient = new OAuthClient(
                    new URLConnectionClient());

            OAuthAccessTokenResponse oauthResponse = null;
            Class<? extends OAuthAccessTokenResponse> cl = OAuthJSONAccessTokenResponse.class;

            oauthResponse = oauthClient.accessToken(request, cl);

            Log.d("OAuth response: " + oauthResponse.getBody());

            tokenInfo.setAccesstoken(oauthResponse.getAccessToken());
            // Google provides refreshToken in one time.
            tokenInfo.setRefreshtoken(refreshToken);
            tokenInfo.setExpiredtime(oauthResponse.getExpiresIn());

        } catch (OAuthSystemException | OAuthProblemException e) {

            e.printStackTrace();
            throw new InternalServerErrorException(
                    "OAuth provider(Google) error");
        }

        return tokenInfo;
    }

    @Override
    public UserTable requestGetUserInfo(String accessToken, Object options) {

        UserTable userInfo = new UserTable();

        if (accessToken == null) {
            Log.w("accessToken is null!");
            return userInfo;
        }

        String response = null;

        try {

            OAuthClientRequest request = new OAuthBearerClientRequest(
                    resource_url).setAccessToken(accessToken)
                            .buildQueryMessage();

            OAuthClient oAuthClient = new OAuthClient(
                    new URLConnectionClient());
            OAuthResourceResponse resourceResponse = oAuthClient.resource(
                    request, OAuth.HttpMethod.GET, OAuthResourceResponse.class);

            response = resourceResponse.getBody();
            Log.d("response: " + response);

        } catch (OAuthSystemException | OAuthProblemException e) {
            e.printStackTrace();
            throw new InternalServerErrorException(
                    "OAuth provider(Github) error");
        }

        String userIdKey = "name";

        JSONUtil<HashMap<String, String>> util = new JSONUtil<>();
        HashMap<String, String> parsedData = util.parseJSON(response,
                HashMap.class);

        if (parsedData == null) {
            Log.d("parsedData is null!");
            return userInfo;
        }

        String userId = parsedData.get(userIdKey);
        userInfo.setUserid(userId);

        return userInfo;
    }
}
