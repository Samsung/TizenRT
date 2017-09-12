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

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Set;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.TokenTable;
import org.iotivity.cloud.accountserver.db.UserTable;
import org.iotivity.cloud.base.exception.ServerException.InternalServerErrorException;
import org.iotivity.cloud.util.Log;
import org.reflections.Reflections;
import org.reflections.scanners.SubTypesScanner;
import org.reflections.util.ClasspathHelper;
import org.reflections.util.ConfigurationBuilder;

/**
 *
 * This class provides factory method for OAuth2.0 Servers.
 *
 */
public class OAuthProviderFactory {

    private Class<?> authProviderClass = null;
    private Object   authProviderObj   = null;

    /**
     * API to get token table which includes access token, refresh token, and
     * token expiration time
     * 
     * @param authCode:
     *            authorization code
     * @param options:
     *            authserver url and apiserver url option
     * @return : token table which includes access token, refresh token, and
     *         token expiration time
     */
    public TokenTable requestAccessTokenInfo(String authCode, Object options) {
        TokenTable tokenInfo = null;
        try {
            Method method = authProviderClass.getMethod(
                    "requestAccessTokenInfo",
                    new Class[] { String.class, Object.class });
            tokenInfo = (TokenTable) method.invoke(authProviderObj, authCode,
                    options);

        } catch (Exception e) {
            e.printStackTrace();
            throw new InternalServerErrorException(
                    "method loading error from 3rd party library");
        }
        return tokenInfo;
    }

    /**
     * API to get renewed token table which includes new access token, new
     * refresh token, and new token expiration time
     * 
     * @param refreshToken:
     *            Refresh token used to refresh the access token in cloud before
     *            getting expired
     * @return: token table which includes new access token, new refresh token,
     *          and new token expiration time
     */
    public TokenTable requestRefreshTokenInfo(String refreshToken) {

        TokenTable tokenInfo = null;

        try {
            Method method = authProviderClass.getMethod(
                    "requestRefreshTokenInfo", new Class[] { String.class });
            tokenInfo = (TokenTable) method.invoke(authProviderObj,
                    refreshToken);

        } catch (Exception e) {
            e.printStackTrace();
            throw new InternalServerErrorException(
                    "method loading error from 3rd party library");
        }
        return tokenInfo;
    }

    /**
     * API to get user information table
     * 
     * @param accessToken:
     *            Access token used for communication with cloud after account
     *            creation
     * @param options:
     *            authserver url and apiserver url option
     * @return: user information table
     */
    public UserTable requestGetUserInfo(String accessToken, Object options) {

        UserTable userInfo = null;

        try {
            Method method = authProviderClass.getMethod("requestGetUserInfo",
                    new Class[] { String.class, Object.class });
            userInfo = (UserTable) method.invoke(authProviderObj, accessToken,
                    options);

        } catch (Exception e) {
            e.printStackTrace();
            throw new InternalServerErrorException(
                    "method loading error from 3rd party library");
        }
        return userInfo;
    }

    /**
     * API for creating OAuth server object
     *
     * @param authProviderName
     *            Valid name of auth server.
     * @return OAuthServer - object to handle authorization
     * @throws Exception
     */
    public boolean load(String authProviderName) {

        File jarFile = new File(Constants.OAUTH_LIBRARIES_PATH + authProviderName + ".jar");
        URLClassLoader classLoader = null;

        try {
            URL jarUrl = new URL("jar:" + jarFile.toURI() + "!/");
            URL[] urls = new URL[] { jarUrl, ClasspathHelper.forClass(this.getClass()) };
            classLoader = new URLClassLoader(urls);

            String authProvider = this.getClass().getPackage().getName() + "." + authProviderName;
            Reflections reflections = new Reflections(new ConfigurationBuilder().setUrls(urls)
                    .setScanners(new SubTypesScanner(false)));
            for (String type : reflections.getAllTypes())
                if (type.equalsIgnoreCase(authProvider))
                    authProvider = type;

            Class<?> authProviderClass = classLoader
                    .loadClass(authProvider);

            this.authProviderClass = authProviderClass;
            Object object = authProviderClass.newInstance();
            this.authProviderObj = object;

        } catch (Exception e) {
            e.printStackTrace();
            return false;

        } finally {
            try {
                classLoader.close();
            } catch (IOException e) {
                e.printStackTrace();
                throw new InternalServerErrorException(
                        "3rd party library loading error");
            }
        }

        return true;
    }
}
