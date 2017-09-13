#ifndef _OIC_SECURITY_MANAGER_H_
#define _OIC_SECURITY_MANAGER_H_

#include "srmutility.h"

#ifdef __SECURED__
/**
 * @brief   Authentication type for ownership transfer
 *              b00000001  (1):  Just works authentication
 *              b00000010  (2):  Random PIN based authentication
 *              b00000100  (4):  Certificate based authentication
 *              b00001000  (8):  Decentralized public key based authentication
 *              b00010000 (16):  Preconfigured PIN based authentication
 *              b00100000 (32):  Mutual Verification Just works authentication
 */
typedef enum OICAuthType
{
    AUTH_UNKNOW = (0x0),
    AUTH_JUST_WORKS                 = (0x1 << 0),
    AUTH_RANDOM_PIN                 = (0x1 << 1),
    AUTH_CERTIFICATE                = (0x1 << 2),
    AUTH_DECENTRALIZED_PUB_KEY      = (0x1 << 3),
    AUTH_PRECONF_PIN                = (0x1 << 4),
    AUTH_JUST_WORKS_MUTUAL_VERIFIED = (0x1 << 5),
    AUTH_CERTIFICATE_CONFIRM        = (0x1 << 6),
} OICAuthType_t;


/**
 * Access policy in least significant bits (from Spec):
 *              b00000001  (1):  CREATE
 *              b00000010  (2):  READ
 *              b00000100  (4):  WRITE
 *              b00001000  (8):  DELETE
 *              b00010000 (16):  NOTIFY
 *              b00011111 (31) Full permission
 */
#define ACL_PERMISSION_CREATE       (0x1 << 0)
#define ACL_PERMISSION_READ         (0x1 << 1)
#define ACL_PERMISSION_WRITE        (0x1 << 2)
#define ACL_PERMISSION_DELETE       (0x1 << 3)
#define ACL_PERMISSION_NOTIFY       (0x1 << 4)
#define ACL_PERMISSION_FULL (ACL_PERMISSION_CREATE | \
                                 ACL_PERMISSION_READ | \
                                 ACL_PERMISSION_WRITE | \
                                 ACL_PERMISSION_DELETE | \
                                 ACL_PERMISSION_NOTIFY)


/**
 * Function to reset the SVR DB(security provisioning data)
 *
 * @param authTypes authentication type for ownership transfer
 * @ret 0 is success, otherwise fail.
 */
extern int SM_InitOICSecurity(int authType, const char* dbPath);

/**
 * Function to init SVR DB(security provisioning data)
 * If SVR DB not exist, SVR DB will be generated.
 *
 * @ret 0 is success, otherwise fail.
 */
extern int SM_InitSvrDb();

/**
 * Function to reset SVR DB
 *
 * @ret 0 is success, otherwise fail.
 */
extern int SM_ResetSvrDb();

/**
 * Function to generate MAC based UUID.
 * NOTE : device uuid will be changed to the MD5 hash value of MAC address
 *
 * @param isForced if true, Device UUID will be created forcibly.
 * @ret 0 is success, otherwise fail.
 */
extern int SM_GenerateMacBasedDeviceId(bool isForced);


/**
 * Function to save the Cloud ACL
 *
 * @param cloudUuid UUID for cloud
 * @ret 0 is success, otherwise fail.
 */
extern int SM_SaveCloudAcl(const char* cloudUuid);

/**
 * Function to save the ACL
 *
 * @param uuid                 UUID of controller or wildcard("*")
 * @param resourceUri          resource URI to be added into access control list.
 * @param resourceType         Type name of resource
 * @param interfaceName        Interface name of resource
 * @param permission           Access permission value
 *               Access policy in least significant bits (from Spec):
 *                   b00000001  (1):  CREATE
 *                   b00000010  (2):  READ
 *                   b00000100  (4):  WRITE
 *                   b00001000  (8):  DELETE
 *                   b00010000 (16):  NOTIFY
 *                   b00011111 (31) Full permission
 * @ret 0 is success, otherwise fail.
 */
extern int SM_SaveAcl(const char* uuid, const char* resourceUri, const char* resourceType, const char* interfaceName, uint16_t permission);

/**
 * API to register OTM event handler
 *
 * @param otmEventHandler implementation of OTM event handler (@ref OicSecOtmEventHandler_t)
 */
extern void SM_SetOTMEventHandler(OicSecOtmEventHandler_t otmEventHandler);

/**
 * Function to set a MOT status
 *
 * @param enable whether the MOT is enabled. (true=enable, false=disable)
 *
 * @ret 0 is success, otherwise fail.
 */
extern int SM_SetMOTStatus(bool enable);

/**
 * API to add preconfigured PIN for MOT
 *
 * @param[in] preconfPin string type of preconfigured PIN
 *
 * @ret 0 is success, otherwise fail.
 */
extern int SM_SavePreconfiguredPin(const char* pin);

#endif //__SECURED__

#endif //_OIC_SECURITY_MANAGER_H_
