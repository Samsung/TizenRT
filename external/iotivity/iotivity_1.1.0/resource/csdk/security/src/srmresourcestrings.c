//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <stdlib.h>
#include "securevirtualresourcetypes.h"

const char *SVR_DB_FILE_NAME = "oic_svr_db.json";
const char *SVR_DB_DAT_FILE_NAME = "oic_svr_db.dat";
const char *OIC_MI_DEF = "oic.mi.def";

//AMACL
const char *OIC_RSRC_TYPE_SEC_AMACL = "oic.sec.amacl";
const char *OIC_RSRC_AMACL_URI =  "/oic/sec/amacl";
const char *OIC_JSON_AMACL_NAME = "amacl";

//ACL
const char *OIC_RSRC_TYPE_SEC_ACL = "oic.sec.acl";
const char *OIC_RSRC_ACL_URI =  "/oic/sec/acl";
const char *OIC_JSON_ACL_NAME = "acl";
const char *OIC_JSON_ACLIST_NAME = "aclist";
const char *OIC_JSON_ACES_NAME = "aces";

//Pstat
const char *OIC_RSRC_TYPE_SEC_PSTAT = "oic.sec.pstat";
const char *OIC_RSRC_PSTAT_URI =  "/oic/sec/pstat";
const char *OIC_JSON_PSTAT_NAME = "pstat";

//doxm
const char *OIC_RSRC_TYPE_SEC_DOXM = "oic.sec.doxm";
const char *OIC_RSRC_DOXM_URI =  "/oic/sec/doxm";
const char *OIC_JSON_DOXM_NAME = "doxm";

//cred
const char *OIC_RSRC_TYPE_SEC_CRED = "oic.sec.cred";
const char *OIC_RSRC_CRED_URI =  "/oic/sec/cred";
const char *OIC_JSON_CRED_NAME = "cred";
const char *OIC_JSON_CREDS_NAME = "creds";

//CRL
const char *OIC_RSRC_TYPE_SEC_CRL = "oic.sec.crl";
const char *OIC_RSRC_CRL_URI =  "/oic/sec/crl";
const char *OIC_JSON_CRL_NAME = "crl";

//SACL
const char *OIC_RSRC_TYPE_SEC_SACL = "oic.sec.sacl";
const char *OIC_RSRC_SACL_URI =  "/oic/sec/sacl";
const char *OIC_JSON_SACL_NAME = "sacl";

//svc
const char *OIC_RSRC_TYPE_SEC_SVC = "oic.sec.svc";
const char *OIC_RSRC_SVC_URI =  "/oic/sec/svc";
const char *OIC_JSON_SVC_NAME = "svc";

//pconf
const char *OIC_RSRC_TYPE_SEC_PCONF = "oic.sec.pconf";
const char *OIC_RSRC_PCONF_URI =  "/oic/sec/pconf";
const char *OIC_JSON_PCONF_NAME = "pconf";

//dpairing
const char *OIC_RSRC_TYPE_SEC_DPAIRING = "oic.sec.dpairing";
const char *OIC_RSRC_DPAIRING_URI =  "/oic/sec/dpairing";
const char *OIC_JSON_DPAIRING_NAME = "dpairing";

//version
const char *OIC_RSRC_TYPE_SEC_VER = "oic.sec.ver";
const char *OIC_RSRC_VER_URI =  "/oic/sec/ver";
const char *OIC_JSON_VER_NAME = "ver";

const char *OIC_JSON_SUBJECT_NAME = "subject";
const char *OIC_JSON_RESOURCES_NAME = "resources";
const char *OIC_JSON_AMSS_NAME = "amss";
const char *OIC_JSON_AMS_NAME = "ams";
const char *OIC_JSON_PERMISSION_NAME = "permission";
const char *OIC_JSON_OWNERS_NAME = "ownrs";
const char *OIC_JSON_OWNER_NAME = "ownr";
const char *OIC_JSON_DEVOWNERID_NAME = "devowneruuid";
const char *OIC_JSON_OWNED_NAME = "owned";
const char *OIC_JSON_OXM_NAME = "oxm";
const char *OIC_JSON_OXMS_NAME = "oxms";
const char *OIC_JSON_OXM_TYPE_NAME = "oxmtype";
const char *OIC_JSON_OXM_SEL_NAME = "oxmsel";
const char *OIC_JSON_DEVICE_ID_FORMAT_NAME = "didformat";
const char *OIC_JSON_ISOP_NAME = "isop";
const char *OIC_JSON_COMMIT_HASH_NAME = "ch";
const char *OIC_JSON_DEVICE_ID_NAME = "deviceuuid";
const char *OIC_JSON_CM_NAME = "cm";
const char *OIC_JSON_TM_NAME = "tm";
const char *OIC_JSON_OM_NAME = "om";
const char *OIC_JSON_SM_NAME = "sm";
const char *OIC_JSON_CREDID_NAME = "credid";
const char *OIC_JSON_SUBJECTID_NAME = "subjectuuid";
const char *OIC_JSON_ROLEIDS_NAME = "roleid";
const char *OIC_JSON_CREDTYPE_NAME = "credtype";
const char *OIC_JSON_PUBLICDATA_NAME = "publicdata";
const char *OIC_JSON_PRIVATEDATA_NAME = "privatedata";
const char *OIC_JSON_PUBDATA_NAME = "pubdata";
const char *OIC_JSON_PRIVDATA_NAME = "privdata";
const char *OIC_JSON_OPTDATA_NAME = "optdata";
const char *OIC_JSON_SERVICE_DEVICE_ID = "svcdid";
const char *OIC_JSON_SERVICE_TYPE = "svct";
const char *OIC_JSON_PERIOD_NAME = "period";
const char *OIC_JSON_PERIODS_NAME = "prds";
const char *OIC_JSON_CRMS_NAME = "crms";
const char *OIC_JSON_RECURRENCES_NAME = "recurrence";
const char *OIC_JSON_SUPPORTED_CRED_TYPE_NAME = "sct";
const char *OIC_JSON_DPC_NAME = "dpc";
const char *OIC_JSON_EDP_NAME = "edp";
const char *OIC_JSON_PIN_NAME = "pin";
const char *OIC_JSON_PDACL_NAME = "pdacl";
const char *OIC_JSON_PDDEV_LIST_NAME = "pddev";
const char *OIC_JSON_ROWNER_NAME = "rowner";
const char *OIC_JSON_PRM_NAME = "prm";
const char *OIC_JSON_SPM_NAME = "spm";
const char *OIC_JSON_PDEVICE_ID_NAME = "pdeviceuuid";
const char *OIC_JSON_RLIST_NAME = "rlist";
const char *OIC_JSON_HREF_NAME = "href";
const char *OIC_JSON_REL_NAME = "rel";
const char *OIC_JSON_RT_NAME = "rt";
const char *OIC_JSON_IF_NAME = "if";
const char *OIC_JSON_ROWNERID_NAME = "rowneruuid";
const char *OIC_JSON_ENCODING_NAME = "encoding";
const char *OIC_JSON_DATA_NAME = "data";
const char *OIC_JSON_SEC_V_NAME = "secv";

const char *OIC_JSON_EMPTY_STRING = "";

OicUuid_t WILDCARD_SUBJECT_ID = {"*"};
OicUuid_t WILDCARD_SUBJECT_B64_ID = { .id = {'2', '2', '2', '2', '2', '2', '2', '2',
                                             '2', '2', '2', '2', '2', '2', '2', '2'
                                            }
                                    };
size_t WILDCARD_SUBJECT_ID_LEN = 1;
const char *WILDCARD_RESOURCE_URI = "*";

//Ownership Transfer Methods
const char *OXM_JUST_WORKS = "oic.sec.doxm.jw";
const char *OXM_RANDOM_DEVICE_PIN = "oic.sec.doxm.rdp";
const char *OXM_MANUFACTURER_CERTIFICATE = "oic.sec.doxm.mfgcert";

//Credential data encoding methods
const char *OIC_SEC_ENCODING_BASE64 = "oic.sec.encoding.base64";
const char *OIC_SEC_ENCODING_RAW = "oic.sec.encoding.raw";

const char *OIC_SEC_TRUE = "true";
const char *OIC_SEC_FALSE = "false";

const char *OIC_SEC_REST_QUERY_SEPARATOR = ";";
char OIC_SEC_REST_QUERY_DELIMETER = '=';

//Security Version
const char *DEFAULT_SEC_VERSION = "0.0.0";

