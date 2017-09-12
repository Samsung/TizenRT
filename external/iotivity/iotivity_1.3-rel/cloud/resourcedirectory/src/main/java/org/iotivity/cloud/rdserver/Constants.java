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
package org.iotivity.cloud.rdserver;

import org.iotivity.cloud.base.OICConstants;

public class Constants extends OICConstants {

    public static final int    DEFAULT_COAP_PORT       = 5684;

    /** Database, Table name */
    public static final String RD_DB_NAME              = "RD_DB";
    public static final String RD_TABLE                = "RD_TABLE";
    public static final String PRESENCE_TABLE          = "PRESENCE_TABLE";

    /** A human friendly name of device */
    public static final String DEVICE_NAME             = "n";
    /** An unique identifier of device */
    public static final String DEVICE_ID               = "di";
    /** policies that apply for resource */
    public static final String POLICY                  = "p";
    /** To represent bitmap. */
    public static final String BITMAP                  = "bm";
    /** An ordinal number that is not repeated */
    public static final String INS                     = "ins";
    /** Time (in seconds) to indicate how long RD should publish this item */
    public static final String DEVICE_TTL              = "lt";
    /** Time to live for this link */
    public static final String RESOURCE_TTL            = "ttl";
    /** URI Reference */
    public static final String HREF                    = "href";
    /** Resource Types */
    public static final String RESOURCE_TYPE           = "rt";
    /** Resource interface */
    public static final String INTERFACE               = "if";
    /** Relation between target URI and context URI */
    public static final String REL                     = "rel";
    /** Title for the link relation */
    public static final String TITLE                   = "title";
    /** This is used to override the context URI */
    public static final String ANCHOR                  = "anchor";
    /** Media type. Default : application/json */
    public static final String MEDIA_TYPE              = "type";
    /** To represent links. */
    public static final String LINKS                   = "links";
    /** To represent resource type with Publish RD. */
    public static final String RESOURCE_TYPE_RDPUBLISH = "oic.wk.rdpub";

    /** '/oic/prs' resource property */
    public static final String PRESENCE_STATE          = "state";
    public static final String PRESENCE_ON             = "on";
    public static final String PRESENCE_OFF            = "off";
    public static final String PRESENCE_LIST           = "prslist";
    public static final String DEVICE_PRESENCE         = "device_presence";

    /** '/oic/ad' resource property */
    public static final String NON                     = "non";
    public static final String TRIGGER                 = "trg";
    public static final byte   RES_CREATE              = 0;
    public static final byte   RES_CHANGE              = 1;
    public static final byte   RES_DELETE              = 2;
    public static final String RESOURCE_PRESENCE       = "resource_presence";

}
