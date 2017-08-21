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
package org.iotivity.cloud.base;

public class OICConstants {
    /*
     * OCF version 1.0.0 default value for Accept Version and Content-Format
     * Version
     */
    public static final int    DEFAULT_VERSION_VALUE         = 2048;

    /* resource Interface for keep-alive */
    public static final String DEFAULT_INTERFACE             = "oic.if.baseline";

    public static final String LINK_INTERFACE                = "oic.if.ll";

    public static final String PREFIX_OIC                    = "oic";

    public static final String RS_INTERFACE                  = "if";

    public static final String RS_RESOURCE_TYPE              = "rt";

    /* resource url for account */
    public static final String ACCOUNT_URI                   = "account";

    public static final String SESSION_URI                   = "session";

    public static final String DEVICE_URI                    = "device";

    public static final String TOKEN_REFRESH_URI             = "tokenrefresh";

    /* resource url for acl */
    public static final String CREDPROV_URI                  = "credprov";

    public static final String CERT_URI                      = "cert";

    /* resource url for acl */
    public static final String ACL_URI                       = "acl";

    public static final String GROUP_URI                     = "group";

    public static final String VERIFY_URI                    = "verify";

    public static final String ID_URI                        = "id";

    public static final String ROUTE_URI                     = "route";

    public static final String INVITE_URI                    = "invite";

    public static final String SEARCH_URI                    = "search";

    /* resource uri to publish, update, delete resource info */
    public static final String RD_URI                        = "rd";

    /* resource uri for keep-alive */
    public static final String KEEP_ALIVE_URI                = "ping";

    /* resource uri to discover resource info */
    public static final String WELL_KNOWN_URI                = "res";

    /* resource uri for resource presence */
    public static final String RES_PRESENCE_URI              = "ad";

    /* resource uri for device presence */
    public static final String DEVICE_PRESENCE_URI           = "prs";

    /* resource uri for message queue */
    public static final String MQ_BROKER_URI                 = "ps";
    
    /* file path for tls communication - Modify filePath to yours */
    public static final String CERTS_PATH                    = "../certificate";

    public static final String ROOT_CERT_FILE                = CERTS_PATH + "/rootca.crt";

    public static final String CLOUD_CERT_FILE               = CERTS_PATH + "/iotivitycloud.crt";

    public static final String CLOUD_KEY_FILE                = CERTS_PATH + "/iotivitycloud.key";

    /* cloud uuid */
    public static final String CLOUD_UUID                    = "2a6085d1-815d-4277-baba-4e4e4df91308";

    public static final String ACCOUNT_FULL_URI              = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.ACCOUNT_URI;

    public static final String ACCOUNT_SEARCH_FULL_URI       = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.ACCOUNT_URI + "/"
            + OICConstants.SEARCH_URI;

    public static final String ACCOUNT_SESSION_FULL_URI      = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.ACCOUNT_URI + "/"
            + OICConstants.SESSION_URI;

    public static final String ACCOUNT_TOKENREFRESH_FULL_URI = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.ACCOUNT_URI + "/"
            + OICConstants.TOKEN_REFRESH_URI;

    public static final String ACL_ID_FULL_URI               = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.ACL_URI + "/"
            + OICConstants.ID_URI;

    public static final String ACL_VERIFY_FULL_URI           = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.ACL_URI + "/"
            + OICConstants.VERIFY_URI;

    public static final String GROUP_FULL_URI                = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.ACL_URI + "/"
            + OICConstants.GROUP_URI;

    public static final String INVITE_FULL_URI               = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.ACL_URI + "/"
            + OICConstants.INVITE_URI;

    public static final String KEEP_ALIVE_FULL_URI           = "/" + PREFIX_OIC
            + "/" + OICConstants.KEEP_ALIVE_URI;

    public static final String ROUTE_FULL_URI                = "/" + PREFIX_OIC
            + "/" + OICConstants.ROUTE_URI;

    public static final String MQ_BROKER_FULL_URI            = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.MQ_BROKER_URI;

    public static final String RD_FULL_URI                   = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.RD_URI;

    public static final String WELL_KNOWN_FULL_URI           = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.WELL_KNOWN_URI;

    public static final String RESOURCE_PRESENCE_FULL_URI    = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.RES_PRESENCE_URI;

    public static final String DEVICE_PRESENCE_FULL_URI      = "/"
            + OICConstants.PREFIX_OIC + "/" + OICConstants.DEVICE_PRESENCE_URI;

    public static final String CREDPROV_CERT_FULL_URI        = "/" + PREFIX_OIC
            + "/" + CREDPROV_URI + "/" + CERT_URI;
}