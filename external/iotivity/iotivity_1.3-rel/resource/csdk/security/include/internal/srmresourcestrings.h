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

#ifndef IOTVT_SRM_RSRC_STRINGS_H
#define IOTVT_SRM_RSRC_STRINGS_H

#include "experimental/securevirtualresourcetypes.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char * SVR_DB_FILE_NAME;
extern const char * SVR_DB_DAT_FILE_NAME;

//AMACL
extern const char * OIC_RSRC_TYPE_SEC_AMACL;
extern const char * OIC_RSRC_AMACL_URI;
extern const char * OIC_JSON_AMACL_NAME;

//ACL
extern const char * OIC_RSRC_TYPE_SEC_ACL;
extern const char * OIC_RSRC_ACL_URI;
extern const char * OIC_JSON_ACL_NAME;
extern const char * OIC_JSON_ACLIST_NAME;
extern const char * OIC_JSON_ACLIST2_NAME;
extern const char * OIC_JSON_ACES_NAME;
extern const char * OIC_JSON_ACEID_NAME;

extern const char * OIC_RSRC_TYPE_SEC_ACL2;
extern const char * OIC_RSRC_TYPE_SEC_ACE2;
extern const char * OIC_RSRC_ACL2_URI;
extern const char * OIC_JSON_ACL2_NAME;

//PSTAT
extern const char * OIC_RSRC_TYPE_SEC_PSTAT;
extern const char * OIC_RSRC_PSTAT_URI;
extern const char * OIC_JSON_PSTAT_NAME;


//DOXM
extern const char * OIC_RSRC_TYPE_SEC_DOXM;
extern const char * OIC_RSRC_DOXM_URI;
extern const char * OIC_JSON_DOXM_NAME;

//cred
extern const char * OIC_RSRC_TYPE_SEC_CRED;
extern const char * OIC_RSRC_CRED_URI;
extern const char * OIC_JSON_CRED_NAME;
extern const char * OIC_JSON_CREDS_NAME;

//csr
extern const char * OIC_RSRC_TYPE_SEC_CSR;
extern const char * OIC_RSRC_CSR_URI;
extern const char * OIC_JSON_CSR_NAME;

//roles
extern const char * OIC_RSRC_TYPE_SEC_ROLES;
extern const char * OIC_RSRC_ROLES_URI;
extern const char * OIC_JSON_ROLES_NAME;

//CRL
extern const char * OIC_RSRC_TYPE_SEC_CRL;
extern const char * OIC_RSRC_CRL_URI;
extern const char * OIC_JSON_CRL_NAME;

//SACL
extern const char * OIC_RSRC_TYPE_SEC_SACL;
extern const char * OIC_RSRC_SACL_URI;
extern const char * OIC_JSON_SACL_NAME;

//SVC
extern const char * OIC_RSRC_TYPE_SEC_SVC;
extern const char * OIC_RSRC_SVC_URI;
extern const char * OIC_JSON_SVC_NAME;

//version
extern const char * OIC_RSRC_TYPE_SEC_VER;
extern const char * OIC_RSRC_VER_URI;
extern const char * OIC_JSON_VER_NAME;

//reset profile
extern const char * OIC_JSON_RESET_PF_NAME;
extern const char * OIC_JSON_SUBJECTID_NAME;
extern const char * OIC_JSON_SUBJECT_NAME;
extern const char * OIC_JSON_RESOURCES_NAME;
extern const char * OIC_JSON_AMSS_NAME;
extern const char * OIC_JSON_AMS_NAME;
extern const char * OIC_JSON_PERMISSION_NAME;
extern const char * OIC_JSON_OWNERS_NAME;
extern const char * OIC_JSON_OWNER_NAME;
extern const char * OIC_JSON_DEVOWNERID_NAME;
#ifdef MULTIPLE_OWNER
extern const char * OIC_JSON_SUBOWNERID_NAME;
#endif //MULTIPLE_OWNER
extern const char * OIC_JSON_OWNED_NAME;
extern const char * OIC_JSON_OXMS_NAME;
#ifdef MULTIPLE_OWNER
extern const char * OIC_JSON_MOM_NAME;
#endif //MULTIPLE_OWNER
extern const char * OIC_JSON_OXM_TYPE_NAME;
extern const char * OIC_JSON_OXM_SEL_NAME;
extern const char * OIC_JSON_DEVICE_ID_FORMAT_NAME;
extern const char * OIC_JSON_CREDID_NAME;
extern const char * OIC_JSON_ROLEID_NAME;
extern const char * OIC_JSON_ROLE_NAME;
extern const char * OIC_JSON_AUTHORITY_NAME;
extern const char * OIC_JSON_CREDTYPE_NAME;
extern const char * OIC_JSON_PUBLICDATA_NAME;
extern const char * OIC_JSON_PRIVATEDATA_NAME;
extern const char * OIC_JSON_PUBDATA_NAME;
extern const char * OIC_JSON_PRIVDATA_NAME;
extern const char * OIC_JSON_OPTDATA_NAME;
extern const char * OIC_JSON_REVOCATION_STATUS_NAME;
extern const char * OIC_JSON_CREDUSAGE_NAME;
extern const char * OIC_JSON_CRMS_NAME;
extern const char * OIC_JSON_VALIDITY_NAME;
extern const char * OIC_JSON_PERIOD_NAME;
extern const char * OIC_JSON_PERIODS_NAME;
extern const char * OIC_JSON_RECURRENCES_NAME;
extern const char * OIC_JSON_ISOP_NAME;
extern const char * OIC_JSON_COMMIT_HASH_NAME;
extern const char * OIC_JSON_DEVICE_ID_NAME;
extern const char * OIC_JSON_CM_NAME;
extern const char * OIC_JSON_TM_NAME;
extern const char * OIC_JSON_OM_NAME;
extern const char * OIC_JSON_SM_NAME;
extern const char * OIC_JSON_SERVICE_DEVICE_ID;
extern const char * OIC_JSON_SERVICE_TYPE;
extern const char * OIC_JSON_SUPPORTED_CRED_TYPE_NAME;
extern const char * OIC_JSON_DPC_NAME;
extern const char * OIC_JSON_EDP_NAME;
extern const char * OIC_JSON_PIN_NAME;
extern const char * OIC_JSON_PDACL_NAME;
extern const char * OIC_JSON_PDDEV_LIST_NAME;
extern const char * OIC_JSON_PRM_NAME;
extern const char * OIC_JSON_SPM_NAME;
extern const char * OIC_JSON_PDEVICE_ID_NAME;
extern const char * OIC_JSON_RLIST_NAME;
extern const char * OIC_JSON_HREF_NAME;
extern const char * OIC_JSON_REL_NAME;
extern const char * OIC_JSON_RT_NAME;
extern const char * OIC_JSON_IF_NAME;
extern const char * OIC_JSON_ROWNERID_NAME;
#ifdef MULTIPLE_OWNER
extern const char * OIC_JSON_EOWNERID_NAME;
#endif //MULTIPLE_OWNER
extern const char * OIC_JSON_ENCODING_NAME;
extern const char * OIC_JSON_DATA_NAME;
extern const char * OIC_JSON_SEC_V_NAME;
extern const char * OIC_JSON_DOS_NAME;
extern const char * OIC_JSON_S_NAME;
extern const char * OIC_JSON_P_NAME;
extern const char * OIC_JSON_UUID_NAME;
extern const char * OIC_JSON_CONNTYPE_NAME;
extern const char * OIC_JSON_AUTHCRYPT_NAME;
extern const char * OIC_JSON_ANONCLEAR_NAME;
extern const char * OIC_JSON_WC_NAME;
extern const char * OIC_JSON_WC_PLUS_NAME;
extern const char * OIC_JSON_WC_MINUS_NAME;
extern const char * OIC_JSON_WC_ASTERISK_NAME;

extern const char * OIC_JSON_EMPTY_STRING;

// Certificates provided by Cloud or OBT/CMS
extern const char * TRUST_CA;
extern const char * PRIMARY_CERT;
extern const char * PRIMARY_KEY;
extern const char * ROLE_CERT;

// Certificates provided by manufacturer
extern const char * MF_TRUST_CA;
extern const char * MF_PRIMARY_CERT;

extern OicUuid_t WILDCARD_SUBJECT_ID;
extern OicUuid_t WILDCARD_SUBJECT_B64_ID;
extern size_t WILDCARD_SUBJECT_ID_LEN;
extern const char * WILDCARD_RESOURCE_URI;

//Ownership Transfer Methods
extern const char * OXM_JUST_WORKS;
extern const char * OXM_RANDOM_DEVICE_PIN;
extern const char * OXM_MANUFACTURER_CERTIFICATE;
#ifdef MULTIPLE_OWNER
extern const char * OXM_PRECONF_PIN;
#endif //MULTIPLE_OWNER
extern const char * OXM_MV_JUST_WORKS;
extern const char * OXM_CON_MFG_CERT;

//Mutual Verified Just-Works Message Prefix
extern const char * MUTUAL_VERIF_NUM;

extern const char * OIC_SEC_ENCODING_BASE64;
extern const char * OIC_SEC_ENCODING_RAW;
extern const char * OIC_SEC_ENCODING_PEM;
extern const char * OIC_SEC_ENCODING_DER;

extern const char * OIC_SEC_TRUE;
extern const char * OIC_SEC_FALSE;

extern const char * OIC_SEC_REST_QUERY_SEPARATOR;
extern char OIC_SEC_REST_QUERY_DELIMETER;

//Security Version
extern const char * DEFAULT_SPEC_VERSION;

// Preconfigured Pin credential usage
#ifdef MULTIPLE_OWNER
extern const char * PRECONFIG_PIN_CRED;
#endif //MULTIPLE_OWNER

#ifdef __cplusplus
}
#endif

#endif //IOTVT_SRM_RSRC_STRINGS_H

