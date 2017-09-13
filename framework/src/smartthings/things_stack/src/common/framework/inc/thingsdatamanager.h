#ifndef _THINGSDATAMANAGER_H_
#define _THINGSDATAMANAGER_H_

#include "oicserverbuilder.h"
#include "cloud_manager.h"
#include "thingsapi.h"

#include <wifi_manager/wifi_manager.h>

#define MAX_RID_LENGTH                 (10)
#define MAX_DEVICE_NUM_LENGTH          (10)
#define MAX_DEVICE_TYPE_LENGTH         (25)
#define MAX_DEVICE_NAME_LENGTH         (32)
#define MAX_DEVICE_VER_P               (32)
#define MAX_DEVICE_VER_OS              (32)
#define MAX_DEVICE_VER_HW              (32)
#define MAX_DEVICE_VER_FW              (32)
#define MAX_DEVICE_ID_LENGTH           (37)
#define MAX_DEVICE_VENDER_ID           (255)
#define MAX_DEVICE_DESCRIPTION_LENGTH  (255)
#define MAX_DEVICE_CAPABILTY_CNT       (50)
#define MAX_DEVICE_MODEL_ID_LENGTH     (64)
#define MAX_DEVICE_MODEL_IDS_LENGTH    ( (MAX_DEVICE_MODEL_ID_LENGTH * 3) + 3 )

#define MAX_FILE_ACCESS_CNT         10

#define MAX_SPEC_LENGTH             7
#define MAX_URI_LENGTH_OCF          128
#define MAX_CODE_LENGTH             128
#define MAX_TYPES_CNT               5   

#define MAX_PROPERTY_CNT            20

#define MAX_FILE_CNT                10
#define MAX_FILE_PATH_LENGTH        250
#define MAX_TYPE_LENGTH             (10)
#define MAX_PROPERTY_LENGTH_OCF     (64)

#define MAX_NUM_DEV                 1

struct st_resourceType {
    char rt[MAX_PROPERTY_LENGTH_OCF];
    int prop_cnt;
    struct st_attribute * prop [MAX_PROPERTY_CNT];
} st_resourceType;

struct col_resource {   
    char uri                   [MAX_URI_LENGTH_OCF];
    char* interfaceTypes       [MAX_IT_CNT];
    struct st_resource* links           [MAX_DEVICE_CAPABILTY_CNT];    
    char* resourceTypes        [MAX_RT_CNT];

    int ifcnt;
    int rtcnt;
    int linkcnt;
    int policy;
} col_resource;

static struct st_file {
    int id;
    char path [MAX_FILE_PATH_LENGTH];
} st_file;

typedef struct ST_device {
    int no;
    char type       [MAX_DEVICE_TYPE_LENGTH];
    char name       [MAX_DEVICE_NAME_LENGTH];
    char modelNum   [MAX_DEVICE_MODEL_ID_LENGTH];
    char ver_p      [MAX_DEVICE_VER_P];            // mnpv
    char ver_os     [MAX_DEVICE_VER_OS];            // mnhw
    char ver_hw     [MAX_DEVICE_VER_HW];            // mnhw
    char ver_fw     [MAX_DEVICE_VER_FW];            // mnfv
    char device_id  [MAX_DEVICE_ID_LENGTH];            // mnfv
    char vender_id  [MAX_DEVICE_VENDER_ID];            // mnfv
    char description[MAX_DEVICE_DESCRIPTION_LENGTH];
    struct col_resource collection [MAX_DEVICE_CAPABILTY_CNT];
    struct st_resource single     [MAX_DEVICE_CAPABILTY_CNT];

    int capaCnt;
    int colCnt;
    int sigCnt;
    int isPhysical;

    struct OicResource * pChildResources[MAX_DEVICE_CAPABILTY_CNT];
} STDevice;

//jun
int DM_InitModule                   (const char* info_Path);

int DM_TermiateModule               (void);

const char * DM_GetSVRDBFilePath    (void);
const char * DM_GetCertificateFilePath   (void);
const char * DM_GetPrivateKeyFilePath    (void);
const char * DM_GetFileName         (int filenum);
const char * DM_GetOCFDeviceType    (int deviceId);
const char * DM_GetResourceUri      (int deviceNum, int index);
const char * DM_GetOCFCloudAddress  (char* customizedCIserver);

const int DM_GetFileID              (int filenum);
const int DM_GetNumOfFiles          (void);
const int DM_GetNumofChildren       (int deviceNum);


struct OicResource *
            DM_GetResourceInstance  (const char* uri,
                                     const int id);

int DM_GetDeviceInformation         (int *cnt, STDevice*** list);

long DM_GetNumOfDevCnt              (void);
STDevice* DM_GetInfoOfDev           (unsigned long number);
bool DM_RegisterUserDefineDeviceID  ( const int seq_ocfInfo, const char* devID );
bool DM_RegisterDeviceID            (void);
int DM_RegisterResource             (OICServerBuilder * builder);

int SaveAccesPointInfo               (wifi_manager_ap_config_s* connect_config);
int DM_UpdateOCFCloud               (ESCloud_SignUp* clData);
int DM_LoadLegacyCloudData          (ESCloud_SignUp** clData);

bool DM_IsRscPublished              (void);
bool DM_IsInCollectionResource      (int deviceNum, const char* uri);

int DM_DelOCFCloudData(void);
bool DM_isThereOCFCloud(void);

int DM_ValidateAttributeInRequest(char* resType, const void* payload);



typedef enum
{
    es_conn_type_NONE       = 0,
    es_conn_type_SoftAP     = 1,
    es_conn_type_BLE     = 2,
} easySetup_Connectivity_type;

easySetup_Connectivity_type DM_GetEasySetupConnectivityType(void);
//jun
wifi_manager_softap_config_s* DM_GetSoftAPWifiConfig(void);
wifi_manager_ap_config_s* DM_getHomeAPWifiConfig();

#endif // _THINGSDATAMANAGER_H_
