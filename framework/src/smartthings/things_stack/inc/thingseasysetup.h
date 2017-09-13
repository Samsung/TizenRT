/*
 * oiceasysetup.h
 *
 *  Created on: 2017. 2. 20.
 *      Author: eunseok
 */

#ifndef OICEASYSETUP_H_
#define OICEASYSETUP_H_

#define OIC_STRING_MAX_DNSNAME  256
#define OIC_STRING_MAX_LENGTH   100

#define MAX_SSIDLEN     33
#define MAX_CREDLEN     64
#define IP_PORT                 22

typedef enum
{
    WiFi_11A = 0,
    WiFi_11B,
    WiFi_11G,
    WiFi_11N,
    WiFi_11AC,
    WiFi_EOF = 999,
} WIFI_MODE;

typedef enum
{
    WiFi_24G = 0,
    WiFi_5G,
    WiFi_BOTH,
    WiFi_FREQ_EOF
} WIFI_FREQ;

typedef enum
{
    NONE_AUTH = 0,
    WEP,
    WPA_PSK,
    WPA2_PSK
} WIFI_AUTHTYPE;

typedef enum
{
    NONE_ENC = 0,
    WEP_64,
    WEP_128,
    TKIP,
    AES,
    TKIP_AES
} WIFI_ENCTYPE;

typedef struct
{
    char ssid[MAX_SSIDLEN]; /**< ssid of the Enroller**/
    char pwd[MAX_CREDLEN]; /**< pwd of the Enroller**/
    WIFI_AUTHTYPE authtype; /**< auth type of the Enroller**/
    WIFI_ENCTYPE enctype; /**< encryption type of the Enroller**/
    int discoveryChannel;   // samsung specific property
} ESWiFiProvData;

typedef struct
{
    char language[OIC_STRING_MAX_LENGTH];
    char country[OIC_STRING_MAX_LENGTH];
    char datetime[OIC_STRING_MAX_LENGTH];
} ESDevConfProvData;

typedef struct
{
    char authCode[OIC_STRING_MAX_LENGTH];
    char authProvider[OIC_STRING_MAX_LENGTH];
    char ip[IP_PORT];
    char port[IP_PORT];
    char hostName[OIC_STRING_MAX_DNSNAME];
    char ciServer[OIC_STRING_MAX_LENGTH];
    char serverID[OIC_STRING_MAX_LENGTH];
    char clientID[OIC_STRING_MAX_LENGTH];
    char accesstoken[OIC_STRING_MAX_LENGTH];
    int  actokenType;
    char refreshtoken[OIC_STRING_MAX_LENGTH];
    char uid[OIC_STRING_MAX_LENGTH];
} ESCloudProvData;

typedef enum
{
    RST_NEED_CONFIRM = 0,

    RST_AUTO_RESET = 1,

    RST_ENUM_EOF
} ESEnrolleeReset;

typedef enum
{
    /**
     * Default Abort State.
     */
    ABORT_INIT = 0,

    /**
     * Abort Status indicating cancel Reset Confirm.
     */
    ABORT_BEFORE_RESET_CONFIRM = 1,

    /**
     * Abort Status indicating cancel Security Confirm.
     */
    ABORT_BEFORE_SEC_CONFIRM = 2,

    /**
     * Abort Status indicating being security-process to delete Owned Security Data.
     */
    ABORT_BEFORE_DATA_PROVISIONING = 3

} ESEnrolleeAbort;

/**
 * @brief Indicate enrollee and provisioning status. Provisioning status is shown in "provisioning
 *        status" property in provisioning resource.
 */
typedef enum
{
    /**
     * Default state of the device
     */
    ES_STATE_INIT = 0,

    /**
     * Status indicating being connecting to target network
     */
    ES_STATE_CONNECTING_TO_ENROLLER,

    /**
     * Status indicating successful connection to target network
     */
    ES_STATE_CONNECTED_TO_ENROLLER,

    /**
     * Status indicating connection failure to target network
     */
    ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER,

    /**
     * Status indicating being registering to cloud
     */
    ES_STATE_REGISTERING_TO_CLOUD,

    /**
     * Status indicating successful registration to cloud
     */
    ES_STATE_REGISTERED_TO_CLOUD,

    /**
     * Status indicating registration failure to cloud
     */
    ES_STATE_FAILED_TO_REGISTER_TO_CLOUD,

    /**
     * Status indicating being publishing resources to cloud
     */
    ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD,

    /**
     * Status indicating successful resource publish to cloud
     */
    ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD,

    /**
     * Status indicating resource publish failure to cloud
     */
    ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD,

    /**
     * Status indicating Logout from cloud
     */
    ES_STATE_LOGOUT_FROM_CLOUD,

    /**
     * End of Easy setup status
     */
    ES_STATE_EOF = 255
} ESEnrolleeState, ProvStatus, CloudStatus;


#endif /* OICEASYSETUP_H_ */
