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

/**
 * Data type definitions for all oic.sec.* types defined in the
 * OIC Security Specification.
 *
 * Note that throughout, ptrs are used rather than arrays.  There
 * are two primary reasons for this:
 * 1) The Spec defines many structures with optional fields, so pre-
 *    allocating these would be wasteful.
 * 2) There are in many cases arrays of Strings or arrays of Structs,
 *    which could not be defined as variable length arrays (e.g. array[])
 *    without breaking from the structure order and definition in the Spec.
 *
 * The primary drawback to this decision is that marshalling functions
 * will have to be written by hand to marshal these structures (e.g. to/from
 * Persistent Storage, or across memory boundaries).
 *
 * We're using uint typedefs for all enum types to avoid C++ type conversion
 * errors and enable bitfield operations on these fields (causes compilation
 * errors on arduino builds). This may miss some type checks, but it's
 * nearly impossible to do int/bitfield operations on enum types in C++.
 *
 * TODO reconcile against latest OIC Security Spec to ensure all fields correct.
 * (Last checked against v0.95)
 */

#ifndef OC_SECURITY_RESOURCE_TYPES_H
#define OC_SECURITY_RESOURCE_TYPES_H

#include "iotivity_config.h"

#include <stdint.h> // for uint8_t typedef
#include <stdbool.h>
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
#include "experimental/byte_array.h"
#endif /* __WITH_DTLS__  or __WITH_TLS__*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Values used to create bit-maskable enums for single-value response with
 * embedded code.
 */
#define ACCESS_GRANTED_DEF                      (1 << 0)
#define ACCESS_DENIED_DEF                       (1 << 1)
#define INSUFFICIENT_PERMISSION_DEF             (1 << 2)
#define SUBJECT_NOT_FOUND_DEF                   (1 << 3)
#define RESOURCE_NOT_FOUND_DEF                  (1 << 4)
#define POLICY_ENGINE_ERROR_DEF                 (1 << 5)
#define INVALID_PERIOD_DEF                      (1 << 6)
#define SEC_RESOURCE_OVER_UNSECURE_CHANNEL_DEF  (1 << 7)
#define REASON_MASK_DEF               (INSUFFICIENT_PERMISSION_DEF | \
                                       INVALID_PERIOD_DEF | \
                                       SUBJECT_NOT_FOUND_DEF | \
                                       RESOURCE_NOT_FOUND_DEF | \
                                       POLICY_ENGINE_ERROR_DEF | \
                                       SEC_RESOURCE_OVER_UNSECURE_CHANNEL_DEF)


/**
 * Access policy in least significant bits (from Spec):
 * 1st lsb:  C (Create)
 * 2nd lsb:  R (Read, Observe, Discover)
 * 3rd lsb:  U (Write, Update)
 * 4th lsb:  D (Delete)
 * 5th lsb:  N (Notify)
 */
#define PERMISSION_ERROR        (0x0)
#define PERMISSION_CREATE       (1 << 0)
#define PERMISSION_READ         (1 << 1)
#define PERMISSION_WRITE        (1 << 2)
#define PERMISSION_DELETE       (1 << 3)
#define PERMISSION_NOTIFY       (1 << 4)
#define PERMISSION_FULL_CONTROL (PERMISSION_CREATE | \
                                 PERMISSION_READ | \
                                 PERMISSION_WRITE | \
                                 PERMISSION_DELETE | \
                                 PERMISSION_NOTIFY)

/**
 * @brief   Response type for all Action requests from CA layer;
 *          may include a reason code.
 *
 * To extract codes use GetReasonCode function on SRMAccessResponse:
 *
 * SRMAccessResponse_t response = SRMRequestHandler(obj, info);
 * if(SRM_TRUE == IsAccessGranted(response)) {
 *     SRMAccessResponseReasonCode_t reason = GetReasonCode(response);
 *     switch(reason) {
 *         case INSUFFICIENT_PERMISSION:
 *         ...etc.
 *     }
 * }
 */
enum
{
    ACCESS_GRANTED = ACCESS_GRANTED_DEF,
    ACCESS_DENIED = ACCESS_DENIED_DEF,
    ACCESS_DENIED_INVALID_PERIOD = ACCESS_DENIED_DEF
        | INVALID_PERIOD_DEF,
    ACCESS_DENIED_INSUFFICIENT_PERMISSION = ACCESS_DENIED_DEF
        | INSUFFICIENT_PERMISSION_DEF,
    ACCESS_DENIED_SUBJECT_NOT_FOUND = ACCESS_DENIED_DEF
        | SUBJECT_NOT_FOUND_DEF,
    ACCESS_DENIED_RESOURCE_NOT_FOUND = ACCESS_DENIED_DEF
        | RESOURCE_NOT_FOUND_DEF,
    ACCESS_DENIED_POLICY_ENGINE_ERROR = ACCESS_DENIED_DEF
        | POLICY_ENGINE_ERROR_DEF,
    ACCESS_DENIED_SEC_RESOURCE_OVER_UNSECURE_CHANNEL = ACCESS_DENIED_DEF
        | SEC_RESOURCE_OVER_UNSECURE_CHANNEL_DEF,
};

typedef unsigned int SRMAccessResponse_t;

/**
 * Reason code for SRMAccessResponse.
 */
enum
{
    NO_REASON_GIVEN = 0,
    INSUFFICIENT_PERMISSION = INSUFFICIENT_PERMISSION_DEF,
    SUBJECT_NOT_FOUND = SUBJECT_NOT_FOUND_DEF,
    RESOURCE_NOT_FOUND = RESOURCE_NOT_FOUND_DEF,
};

typedef unsigned int SRMAccessResponseReasonCode_t;

/**
 * Extract Reason Code from Access Response.
 */
INLINE_API SRMAccessResponseReasonCode_t GetReasonCode(
    SRMAccessResponse_t response)
{
    SRMAccessResponseReasonCode_t reason =
        (SRMAccessResponseReasonCode_t)(response & REASON_MASK_DEF);
    return reason;
}

/**
 * Returns 'true' iff request should be passed on to RI layer.
 */
INLINE_API bool IsAccessGranted(SRMAccessResponse_t response)
{
    if(ACCESS_GRANTED == (response & ACCESS_GRANTED))
    {
        return true;
    }
    else
    {
        return false;
    }
}

typedef struct OicSecRsrc OicSecRsrc_t;

typedef struct OicSecValidity OicSecValidity_t;

typedef struct OicSecAce OicSecAce_t;

typedef struct OicSecAcl OicSecAcl_t;

typedef struct OicSecAmacl OicSecAmacl_t;

typedef struct OicSecCred OicSecCred_t;

/**
 * Aid for assigning/testing vals with OicSecCredType_t.
 * Example:
 *  OicSecCredType_t ct = PIN_PASSWORD | ASYMMETRIC_KEY;
 *  if((ct & PIN_PASSWORD) == PIN_PASSWORD)
 *  {
 *      // ct contains PIN_PASSWORD flag.
 *  }
 */
enum OSCTBitmask
{
    NO_SECURITY_MODE                = 0x0,
    SYMMETRIC_PAIR_WISE_KEY         = (0x1 << 0),
    SYMMETRIC_GROUP_KEY             = (0x1 << 1),
    ASYMMETRIC_KEY                  = (0x1 << 2),
    SIGNED_ASYMMETRIC_KEY           = (0x1 << 3),
    PIN_PASSWORD                    = (0x1 << 4),
    ASYMMETRIC_ENCRYPTION_KEY       = (0x1 << 5),
};

typedef unsigned int OSCTBitmask_t;

/**
 * /oic/sec/credtype (Credential Type) data type.
 * Derived from OIC Security Spec /oic/sec/cred; see Spec for details.
 *              0:  no security mode
 *              1:  symmetric pair-wise key
 *              2:  symmetric group key
 *              4:  asymmetric key
 *              8:  signed asymmetric key (aka certificate)
 *              16: PIN /password
 */
typedef OSCTBitmask_t OicSecCredType_t;

typedef enum OicSecDeviceOnboardingState
{
    DOS_RESET = 0,
    DOS_RFOTM,
    DOS_RFPRO,
    DOS_RFNOP,
    DOS_SRESET,
    DOS_STATE_COUNT
} OicSecDeviceOnboardingState_t;

typedef struct OicSecDostype
{
    OicSecDeviceOnboardingState_t state;
    bool                          pending;
} OicSecDostype_t;

typedef struct OicSecDoxm OicSecDoxm_t;

/**
 * The oic.sec.dpmtype
 */
enum OicSecDpm
{
    NORMAL                          = 0x0,
    RESET                           = (0x1 << 0),
    TAKE_OWNER                      = (0x1 << 1),
    BOOTSTRAP_SERVICE               = (0x1 << 2),
    SECURITY_MANAGEMENT_SERVICES    = (0x1 << 3),
    PROVISION_CREDENTIALS           = (0x1 << 4),
    PROVISION_ACLS                  = (0x1 << 5),
    VERIFY_SOFTWARE_VERSION         = (0x1 << 6),
    UPDATE_SOFTWARE                 = (0x1 << 7),
#ifdef MULTIPLE_OWNER
    TAKE_SUB_OWNER                  = (0x1 << 13),
#endif
};

typedef unsigned int OicSecDpm_t;

// These types are taken from the Security Spec v1.1.12 /pstat resource definition
// Note that per the latest spec, there is NO definition for Multiple Service Client Directed
// provisioning mode, so that enum value has been removed.
enum OicSecDpom
{
    MULTIPLE_SERVICE_SERVER_DRIVEN    = (0x1 << 0),
    SINGLE_SERVICE_SERVER_DRIVEN      = (0x1 << 1),
    SINGLE_SERVICE_CLIENT_DRIVEN      = (0x1 << 2),
};

typedef unsigned int OicSecDpom_t;

enum OicSecSvcType
{
    SERVICE_UNKNOWN                 = 0x0,
    ACCESS_MGMT_SERVICE             = 0x1,  //urn:oic.sec.ams
};

typedef unsigned int OicSecSvcType_t;

//TODO: Need more clarification on deviceIDFormat field type.
#if 0
enum
{
    URN = 0x0
};

typedef unsigned int OicSecDvcIdFrmt_t;
#endif

enum
{
    OIC_RESOURCE_TYPE_ERROR = 0,
    OIC_R_ACL_TYPE,
    OIC_R_AMACL_TYPE,
    OIC_R_CRED_TYPE,
    OIC_R_CRL_TYPE,
    OIC_R_DOXM_TYPE,
    OIC_R_DPAIRING_TYPE,
    OIC_R_PCONF_TYPE,
    OIC_R_PSTAT_TYPE,
    OIC_R_SACL_TYPE,
    OIC_R_SVC_TYPE,
    OIC_R_CSR_TYPE,
    OIC_R_ACL2_TYPE,
    OIC_R_ROLES_TYPE,
    OIC_SEC_SVR_TYPE_COUNT, //define the value to number of SVR
    NOT_A_SVR_RESOURCE = 99
};

typedef unsigned int OicSecSvrType_t;

enum
{
    OIC_JUST_WORKS                          = 0x0,
    OIC_RANDOM_DEVICE_PIN                   = 0x1,
    OIC_MANUFACTURER_CERTIFICATE            = 0x2,
    OIC_DECENTRALIZED_PUBLIC_KEY            = 0x3,
    OIC_OXM_COUNT,
#ifdef MULTIPLE_OWNER
    OIC_PRECONFIG_PIN                       = 0xFF00,
#endif //MULTIPLE_OWNER
    OIC_MV_JUST_WORKS                       = 0xFF01,
    OIC_CON_MFG_CERT                        = 0xFF02,
};

typedef unsigned int OicSecOxm_t;

enum
{
    OIC_ENCODING_UNKNOW = 0,
    OIC_ENCODING_RAW = 1,
    OIC_ENCODING_BASE64 = 2,
    OIC_ENCODING_PEM = 3,
    OIC_ENCODING_DER = 4
};

typedef unsigned int OicEncodingType_t;

#ifdef MULTIPLE_OWNER
enum
{
    MOT_STATUS_READY = 0,
    MOT_STATUS_IN_PROGRESS = 1,
    MOT_STATUS_DONE = 2,
};

typedef unsigned int MotStatus_t;
#endif //MULTIPLE_OWNER

/**
 * oic.sec.mom type definition
 * TODO: This type will be included to OIC Security Spec.
 * 0 : Disable multiple owner
 * 1 : Enable multiple owner (Always on)
 * 2 : Timely multiple owner enable
 */
enum
{
    OIC_MULTIPLE_OWNER_DISABLE = 0,
    OIC_MULTIPLE_OWNER_ENABLE = 1,
    OIC_MULTIPLE_OWNER_TIMELY_ENABLE = 2,
    OIC_NUMBER_OF_MOM_TYPE = 3
};

typedef unsigned int OicSecMomType_t;

typedef struct OicSecKey OicSecKey_t;

typedef struct OicSecOpt OicSecOpt_t;

typedef struct OicSecPstat OicSecPstat_t;

typedef struct OicSecRole OicSecRole_t;

typedef struct OicSecSvc OicSecSvc_t;

typedef char *OicUrn_t; //TODO is URN type defined elsewhere?

typedef struct OicUuid OicUuid_t; //TODO is UUID type defined elsewhere?

#ifdef MULTIPLE_OWNER
typedef struct OicSecSubOwner OicSecSubOwner_t;
typedef struct OicSecMom OicSecMom_t;
#endif //MULTIPLE_OWNER


#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
typedef struct OicSecCrl OicSecCrl_t;
typedef ByteArray_t OicSecCert_t;
#else
typedef void OicSecCert_t;
#endif /* __WITH_DTLS__ or __WITH_TLS__*/

/**
 * /oic/uuid (Universal Unique Identifier) data type.
 */
#define UUID_LENGTH 128/8 // 128-bit GUID length
//TODO: Confirm the length and type of ROLEID.
#define ROLEID_LENGTH 64 // 64-byte authority max length
#define ROLEAUTHORITY_LENGTH 64 // 64-byte authority max length
#define OWNER_PSK_LENGTH_128 128/8 //byte size of 128-bit key size
#define OWNER_PSK_LENGTH_256 256/8 //byte size of 256-bit key size

struct OicUuid
{
    // <Attribute ID>:<Read/Write>:<Multiple/Single>:<Mandatory?>:<Type>
    //TODO fill in unless this is defined elsewhere?
    uint8_t             id[UUID_LENGTH];
};

/**
 * /oic/sec/jwk (JSON Web Key) data type.
 * See JSON Web Key (JWK)  draft-ietf-jose-json-web-key-41
 */
#define JWK_LENGTH 256/8 // 256 bit key length
struct OicSecKey
{
    uint8_t                *data;
    size_t                  len;

    // TODO: This field added as workaround. Will be replaced soon.
    OicEncodingType_t encoding;

};

struct OicSecOpt
{
    uint8_t                *data;
    size_t                  len;

    OicEncodingType_t encoding;
    bool                revstat;
};

typedef enum OicSecAceResourceWildcard
{
    NO_WILDCARD = 0,
    ALL_DISCOVERABLE,       // maps to "+" in JSON/CBOR
    ALL_NON_DISCOVERABLE,   // maps to "-" in JSON/CBOR
    ALL_RESOURCES           // maps to "*" in JSON/CBOR
} OicSecAceResourceWildcard_t;

struct OicSecRsrc
{
    char *href; // 0:R:S:Y:String
    char *rel; // 1:R:S:N:String
    char** types; // 2:R:S:N:String Array
    size_t typeLen; // the number of elts in types
    char** interfaces; // 3:R:S:N:String Array
    size_t interfaceLen; // the number of elts in interfaces
    OicSecAceResourceWildcard_t wildcard;
    OicSecRsrc_t *next;
};

struct OicSecValidity
{
    char* period; // 0:R:S:Y:String
    char** recurrences; // 1:R:M:Y:Array of String
    size_t recurrenceLen; // the number of elts in recurrence
    OicSecValidity_t *next;
};

typedef enum
{
    OIC_SEC_ACL_UNKNOWN = 0,
    OIC_SEC_ACL_V1 = 1,
    OIC_SEC_ACL_V2 = 2
} OicSecAclVersion_t;

typedef enum
{
    DISCOVERABLE_NOT_KNOWN = 0,
    DISCOVERABLE_TRUE = 1,
    DISCOVERABLE_FALSE = 2
} OicSecDiscoverable_t;

#define OIC_SEC_ACL_LATEST OIC_SEC_ACL_V2

typedef enum
{
    OicSecAceUuidSubject = 0, /* Default to this type. */
    OicSecAceRoleSubject,
    OicSecAceConntypeSubject
} OicSecAceSubjectType;

/**
 * /oic/sec/role (Role) data type.
 * Derived from OIC Security Spec; see Spec for details.
 */
struct OicSecRole
{
    // <Attribute ID>:<Read/Write>:<Multiple/Single>:<Mandatory?>:<Type>
    char id[ROLEID_LENGTH];                 // 0:R:S:Y:String
    char authority[ROLEAUTHORITY_LENGTH];   // 1:R:S:N:String
};

typedef enum OicSecConntype
{
    AUTH_CRYPT, // any subject requesting over authenticated and encrypted channel
    ANON_CLEAR, // any subject requesting over anonymous and unencrypted channel
} OicSecConntype_t;

struct OicSecAce
{
    // <Attribute ID>:<Read/Write>:<Multiple/Single>:<Mandatory?>:<Type>
    OicSecAceSubjectType subjectType;
    union                               // 0:R:S:Y:{roletype|didtype|"*"}
    {
        OicUuid_t subjectuuid;          // Only valid for subjectType == OicSecAceUuidSubject
        OicSecRole_t subjectRole;       // Only valid for subjectType == OicSecAceRoleSubject
        OicSecConntype_t subjectConn;   // Only valid for subjectType == OicSecAceConntypeSubject
    };
    OicSecRsrc_t *resources;            // 1:R:M:Y:Resource
    uint16_t permission;                // 2:R:S:Y:UINT16
    OicSecValidity_t *validities;       // 3:R:M:N:Time-interval
    uint16_t aceid;                     // mandatory in ACE2
#ifdef MULTIPLE_OWNER
    OicUuid_t* eownerID;                //4:R:S:N:oic.uuid
#endif
    OicSecAce_t *next;
};

/**
 * /oic/sec/acl (Access Control List) data type.
 * Derived from OIC Security Spec; see Spec for details.
 */
struct OicSecAcl
{
    // <Attribute ID>:<Read/Write>:<Multiple/Single>:<Mandatory?>:<Type>
    OicUuid_t           rownerID;        // 0:R:S:Y:oic.uuid
    OicSecAce_t         *aces; // 1:R:M:N:ACE
};

/**
 * /oic/sec/amacl (Access Manager Service Accesss Control List) data type.
 * Derived from OIC Security Spec; see Spec for details.
 */
struct OicSecAmacl
{
    // <Attribute ID>:<Read/Write>:<Multiple/Single>:<Mandatory?>:<Type>
    size_t              resourcesLen;   // the number of elts in Resources
    char                **resources;    // 0:R:M:Y:String
    OicSecAmacl_t         *next;
};

/**
 * /oic/sec/cred (Credential) data type.
 * Derived from OIC Security Spec; see Spec for details.
 */
struct OicSecCred
{
    // <Attribute ID>:<Read/Write>:<Multiple/Single>:<Mandatory?>:<Type>
    uint16_t            credId;         // 0:R:S:Y:UINT16
    OicUuid_t           subject;        // 1:R:S:Y:oic.uuid
    // If roleId.id is all zeroes, this property is not set.
    OicSecRole_t        roleId;         // 2:R:M:N:oic.sec.roletype
    OicSecCredType_t    credType;       // 3:R:S:Y:oic.sec.credtype
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    OicSecKey_t         publicData;     // own cerificate chain
    char            *credUsage;            // 4:R:S:N:String
    OicSecOpt_t        optionalData;   // CA's cerificate chain
#endif /* __WITH_DTLS__  or __WITH_TLS__*/
    OicSecKey_t         privateData;    // 6:R:S:N:oic.sec.key
    char                *period;        // 7:R:S:N:String
//    OicUuid_t            rownerID;      // 8:R:S:Y:oic.uuid
#ifdef MULTIPLE_OWNER
    OicUuid_t            *eownerID;     //9:R:S:N:oic.uuid
#endif //MULTIPLE_OWNER
    OicSecCred_t        *next;
};

#ifdef MULTIPLE_OWNER
struct OicSecSubOwner {
    OicUuid_t uuid;
    MotStatus_t status;
    OicSecSubOwner_t* next;
};

struct OicSecMom{
    OicSecMomType_t mode;
};
#endif //MULTIPLE_OWNER

/**
 * /oic/sec/doxm (Device Owner Transfer Methods) data type
 * Derived from OIC Security Spec; see Spec for details.
 * @note If the struct is updated please update
 * DoxmUpdateWriteableProperty appropriately.
 */
struct OicSecDoxm
{
    // <Attribute ID>:<Read/Write>:<Multiple/Single>:<Mandatory?>:<Type>
    OicSecOxm_t         *oxm;           // 1:R:M:N:UINT16
    size_t              oxmLen;         // the number of elts in Oxm
    OicSecOxm_t         oxmSel;         // 2:R/W:S:Y:UINT16
    OicSecCredType_t    sct;            // 3:R:S:Y:oic.sec.credtype
    bool                owned;          // 4:R:S:Y:Boolean
    OicUuid_t           deviceID;       // 6:R:S:Y:oic.uuid
    bool                dpc;            // 7:R:S:Y:Boolean
    OicUuid_t           owner;          // 8:R:S:Y:oic.uuid
#ifdef MULTIPLE_OWNER
    OicSecSubOwner_t* subOwners;        //9:R/W:M:N:oic.uuid
    OicSecMom_t *mom;                   //10:R/W:S:N:oic.sec.mom
#endif //MULTIPLE_OWNER
    OicUuid_t           rownerID;       // 11:R:S:Y:oic.uuid
};

/**
 * /oic/sec/pstat (Provisioning Status) data type.
 */
struct OicSecPstat
{
    // <Attribute ID>:<Read/Write>:<Multiple/Single>:<Mandatory?>:<Type>
    OicSecDostype_t     dos;            // -:RW:S:Y:oic.sec.dostype
    bool                isOp;           // 0:R:S:Y:Boolean
    OicSecDpm_t         cm;             // 1:R:S:Y:oic.sec.dpmtype
    OicSecDpm_t         tm;             // 2:RW:S:Y:oic.sec.dpmtype
    OicSecDpom_t        om;             // 4:RW:M:Y:oic.sec.dpom
    size_t              smLen;          // the number of elts in Sm
    OicSecDpom_t        *sm;            // 5:R:M:Y:oic.sec.dpom
    uint16_t            commitHash;     // 6:R:S:Y:oic.sec.sha256
    OicUuid_t           rownerID;       // 7:R:S:Y:oic.uuid
};


#if defined(__WITH_DTLS__) ||  defined(__WITH_TLS__)
struct OicSecCrl
{
    uint16_t CrlId;
    ByteArray_t ThisUpdate;
    OicSecKey_t CrlData;
};
#endif /* __WITH_DTLS__ or __WITH_TLS__ */

/**
 * @brief   direct pairing data type
 */
typedef struct OicPin OicDpPin_t;

typedef struct OicSecPdAcl OicSecPdAcl_t;

#define DP_PIN_LENGTH 8 // temporary length

/**
 * @brief   /oic/sec/prmtype (Pairing Method Type) data type.
 *              0:  not allowed
 *              1:  pre-configured pin
 *              2:  random pin
 */
enum PRMBitmask
{
    PRM_NOT_ALLOWED             = 0x0,
    PRM_PRE_CONFIGURED        = (0x1 << 0),
    PRM_RANDOM_PIN               = (0x1 << 1),
};

typedef unsigned int OicSecPrm_t;


struct OicPin
{
    uint8_t             val[DP_PIN_LENGTH];
};

/**
 * @brief   oic.sec.dpacltype (Device Pairing Access Control List) data type.
 */
struct OicSecPdAcl
{
    // <Attribute ID>:<Read/Write>:<Multiple/Single>:<Mandatory?>:<Type>
    char                  **resources;        // 0:R:M:Y:String
    size_t                resourcesLen;      // the number of elts in Resources
    uint16_t             permission;        // 1:R:S:Y:UINT16
    char                  **periods;            // 2:R:M*:N:String (<--M*; see Spec)
    char                  **recurrences;    // 3:R:M:N:String
    size_t                prdRecrLen;         // the number of elts in Periods/Recurrences
    OicSecPdAcl_t    *next;
};

#define SPEC_MAX_VER_LEN (sizeof("core.x.x.x") + 1) // Spec Version length.
/**
 * @def GET_ACL_VER(specVer)
 * Gets ACL version depending on spec. version.
 * Version value has "ocf.x.x.x" for ocf device and "core.x.x.x" for oic device.
 *
 * @param specVer spec. version string
 * @return ACL version
 */

#define GET_ACL_VER(specVer) ((specVer)[0] == 'o' && (specVer)[1] == 'c' && (specVer)[2] == 'f' ?      \
                             OIC_SEC_ACL_V2 : OIC_SEC_ACL_V1)
#define IS_OIC(specVer) ((specVer)[0] == 'c' && (specVer)[1] == 'o' && (specVer)[2] == 'r' && (specVer)[3] == 'e')

#ifdef __cplusplus
}
#endif

#endif //OC_SECURITY_RESOURCE_TYPES_H
