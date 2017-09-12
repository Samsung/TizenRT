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
package org.iotivity.cloud.ciserver;

import org.iotivity.cloud.base.OICConstants;

public class Constants extends OICConstants {
    public static final int    DEFAULT_COAP_PORT               = 5683;
    public static final int    DEFAULT_RESOURCE_DIRECTORY_PORT = 5684;
    public static final int    DEFAULT_ACCOUNT_SERVER_PORT     = 5685;
    public static final int    DEFAULT_MESSAGE_QUEUE_PORT      = 5686;
    public static final int    DEFAULT_HC_PROXY_PORT           = 80;
    public static final int    DEFAULT_WEBSOCKET_PORT          = 8000;

    public static final int    MIN_TO_LIVE                     = 10;

    public static final String USER_ID                         = "uid";
    public static final String DEVICE_ID                       = "di";
    public static final String PRESENCE_STATE                  = "state";

    public static final String REQ_LOGIN                       = "login";

    public static final String ACCESS_TOKEN                    = "accesstoken";
    public static final String REFRESH_TOKEN                   = "refreshtoken";
    public static final String AUTH_CODE                       = "authcode";
    public static final String AUTH_PROVIDER                   = "authprovider";
    public static final String EXPIRES_IN                      = "expiresin";

    public static final String REQ_GROUP_MASTER_ID             = "gmid";
    public static final String REQ_MEMBER_ID                   = "mid";
    public static final String REQ_MEMBER_LIST                 = "members";
    public static final String REQ_DEVICE_ID                   = "di";
    public static final String REQ_DEVICE_ROUTE                = "route";
    public static final String REQ_DEVICE_LIST                 = "devices";
    public static final String REQ_INVITE                      = "invite";
    public static final String REQ_PING_ARRAY                  = "inarray";
    public static final String REQ_PING                        = "in";
    public static final String REQ_SEARCH_USER_ID              = "sid";
    public static final String REQ_REQUEST_METHOD              = "rm";
    public static final String REQ_REQUEST_URI                 = "uri";

    public static final String REQ_GROUP_DEVICES               = "devices";

    public static final String RESP_GRANT_POLICY               = "gp";
    public static final String RESP_ACL_ALLOWED                = "Allowed";
    public static final String RESP_ACL_DENIED                 = "Denied";

    public static final String REQ_LINKS                       = "links";
    public static final String REQ_HREF                        = "href";
    public static final String REQ_CRL                         = "crl";
}
