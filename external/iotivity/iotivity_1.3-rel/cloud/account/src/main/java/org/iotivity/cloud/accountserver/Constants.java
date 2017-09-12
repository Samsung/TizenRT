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
package org.iotivity.cloud.accountserver;

import java.io.File;

import org.iotivity.cloud.base.OICConstants;

public class Constants extends OICConstants {

    public static final int    DEFAULT_COAP_PORT          = 5685;

    public static final String PROPERTIES_FILE_NAME       = "properties"
            + File.separator + "config.properties";

    // Database name

    public static final String DB_NAME                    = "ACCOUNTSERVER_DB";

    // Database table

    public static final String CRL_TABLE                  = "CRL_TABLE";

    public static final String CERTIFICATE_TABLE          = "CERTIFICATE_TABLE";

    public static final String USER_TABLE                 = "USER_TABLE";

    public static final String TOKEN_TABLE                = "TOKEN_TABLE";

    public static final String GROUP_TABLE                = "GROUP_TABLE";

    public static final String INVITE_TABLE               = "INVITE_TABLE";

    public static final String DEVICE_TABLE               = "DEVICE_TABLE";

    public static final String ACL_TABLE                  = "ACL_TABLE";

    public static final String ACE_TABLE                  = "ACE_TABLE";

    public static final String ACLTEMPLATE_TABLE          = "ACLTEMPLATE_TABLE";

    // Database table key

    public static final String RESP_GROUP_PERMISSION      = "gp";

    public static final String KEYFIELD_SN                = "serialNumber";

    public static final String KEYFIELD_NA                = "notAfter";

    public static final String KEYFIELD_NB                = "notBefore";

    public static final String KEYFIELD_REVOKED           = "revoked";

    public static final String KEYFIELD_UUID              = "uuid";

    public static final String KEYFIELD_ACCESSTOKEN       = "accesstoken";

    public static final String KEYFIELD_GID               = "gid";

    public static final String KEYFIELD_DID               = "did";

    public static final String KEYFIELD_UID               = "uid";

    public static final String KEYFIELD_OID               = "oid";

    public static final String KEYFIELD_ACLID             = "aclid";

    public static final String KEYFIELD_DI                = "di";

    public static final String KEYFIELD_GROUP_GACL        = "gacl";

    public static final String KEYFIELD_GROUP_PARENT      = "parent";

    public static final String KEYFIELD_GROUP_OWNER       = "owner";

    public static final String KEYFIELD_GROUP_MASTERS     = "masters";

    public static final String KEYFIELD_GROUP_MEMBERS     = "members";

    public static final String KEYFIELD_GROUP             = "group";

    public static final String KEYFIELD_GROUP_NAME        = "gname";

    public static final String KEYFIELD_GROUP_DEVICES     = "devices";

    public static final String KEYFIELD_GROUP_RESOURCES   = "resources";

    public static final String KEYFIELD_GROUP_SUBGROUPS   = "subgroups";

    public static final String KEYFIELD_USERID            = "userid";

    public static final String KEYFIELD_PROVIDER          = "provider";

    public static final String KEYFIELD_ISSUED_TIME       = "issuedtime";

    public static final String KEYFIELD_EXPIRED_TIME      = "expiredtime";

    public static final String KEYFIELD_INVITE_USER       = "inviteUser";

    public static final String KEYFIELD_INVITED_USER      = "invitedUser";

    public static final String KEYFIELD_ACE_ID            = "aceid";

    public static final String KEYFIELD_ACE_SUBJECT_ID    = "subjectuuid";

    public static final String KEYFIELD_ACE_SUBJECT_TYPE  = "stype";

    public static final String KEYFIELD_ACE_RESOURCE      = "resources";

    public static final String KEYFIELD_ACE_VALIDITY      = "validity";

    public static final String KEYFIELD_ACE_PERMISSION    = "permission";

    public static final String KEYFIELD_ACE_RESOURCE_HREF = "href";

    public static final String KEYFIELD_ACE_RESOURCE_RT   = "rt";

    public static final String KEYFIELD_ACE_RESOURCE_IF   = "if";

    public static final String KEYFIELD_RESOURCE_RT       = "rt";

    public static final String KEYFIELD_RESOURCE_IF       = "if";

    // Request key

    public static final String REQ_DEVICE_ID              = "di";

    public static final String REQ_DEVICE_ID_LIST         = "dilist";

    public static final String REQ_UUID_ID                = "uid";

    public static final String REQ_ACE_ID                 = "aceid";

    public static final String REQ_ACL_ID                 = "aclid";

    public static final String REQ_ROWNER_ID              = "rowneruuid";

    public static final String REQ_ACL_LIST               = "aclist";

    public static final String REQ_OWNER_ID               = "oid";

    public static final String REQ_AUTH_CODE              = "authcode";

    public static final String REQ_AUTH_PROVIDER          = "authprovider";

    public static final String REQ_ACCESS_TOKEN           = "accesstoken";

    public static final String REQ_LOGIN                  = "login";

    public static final String REQ_REFRESH_TOKEN          = "refreshtoken";

    public static final String REQ_GRANT_TYPE             = "granttype";

    public static final String REQ_AUTH_OPTIONS           = "options";

    public static final String REQ_REQUEST_METHOD         = "rm";

    public static final String REQ_REQUEST_URI            = "uri";

    public static final String REQ_SEARCH_USER_ID         = "sid";

    public static final String REQ_GROUP_ID               = "gid";

    public static final String REQ_GROUP_MASTER_ID        = "gmid";

    public static final String REQ_GROUP_TYPE             = "gtype";

    public static final String REQ_GROUP_QUERY_OPERATION  = "op";

    public static final String REQ_GROUP_QUERY_ADD        = "add";

    public static final String REQ_GROUP_QUERY_DELETE     = "delete";

    public static final String REQ_MEMBER                 = "mid";

    public static final String REQ_LAST_UPDATE            = "lu";

    public static final String REQ_THIS_UPDATE            = "tu";

    public static final String REQ_NEXT_UPDATE            = "nu";

    public static final String REQ_CRL                    = "crl";

    public static final String REQ_SERIAL_NUMBER          = "rcsn";

    public static final String REQ_CSR                    = "csr";

    public static final String REQ_INVITE                 = "invite";

    public static final String REQ_INVITE_ACCEPT          = "accept";

    public static final String SEARCH_USER_LIST           = "ulist";

    // Response key

    public static final String RESP_ACCESS_TOKEN          = "accesstoken";

    public static final String RESP_REFRESH_TOKEN         = "refreshtoken";

    public static final String RESP_TOKEN_TYPE            = "tokentype";

    public static final String RESP_EXPIRES_IN            = "expiresin";

    public static final String RESP_REDIRECT_URI          = "redirecturi";

    public static final String RESP_CERTIFICATE           = "certificate";

    public static final String RESP_SERVER_ID             = "sid";

    public static final String RESP_UUID                  = "uid";

    public static final String RESP_USER_INFO             = "uinfo";

    public static final String RESP_USER_LIST             = "ulist";

    public static final String RESP_DEVICE_ID             = "di";

    public static final String RESP_CERT                  = "cert";

    public static final String RESP_CACERT                = "cacert";

    public static final String RESP_INVITE                = "invite";

    public static final String RESP_INVITED               = "invited";

    public static final String RESP_ACL_ALLOWED           = "Allowed";

    public static final String RESP_ACL_DENIED            = "Denied";

    public static final String ENCODING                   = "encoding";

    public static final String DATA                       = "data";

    public static final String CERT                       = "cert";

    public static final String CERT_CHAIN                 = "certchain";

    public static final String RESP_GROUPS                = "groups";

    // constants

    public static final String INVITE_ACCEPT              = "1";

    public static final String INVITE_DENY                = "0";

    public static final String ACL_RESOURCE_TYPE          = "oic.wk.acl";

    // static token type

    public static final String TOKEN_TYPE_BEARER          = "bearer";

    public static final int    TOKEN_INFINITE             = -1;

    // oauth

    public static final String OAUTH_LIBRARIES_PATH       = "oauth/";

}
