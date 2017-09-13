//#******************************************************************
//#
//# Copyright 2016 SAMSUNG ELECTRONICS All Rights Reserved.
//#
//#******************************************************************
#define _POSIX_C_SOURCE 200809L

#include <string.h>
#include <stdlib.h>
#include <apps/netutils/cJSON.h>

#include "octypes.h"
#include "ocpayload.h"
#include "easysetup.h"
#include "ocstackconfig.h"
#include "ocrandom.h"
#include "oicdef.h"
#include "dalogger.h"
#include "damalloc.h"
#include "oicstringutil.h"
#include "oichashmap.h"
#include "oicresource.h"
#include "ocfdatamanager.h"
#include "oicserverbuilder.h"
#include "cloud_manager.h"
#include "cloud_connector.h"
#include "oiceasysetup.h"

#define TAG "[oicdatamgr]"

#define KEY_CLOUD       "cloud"
#define KEY_WIFI       "wifi"

#define KEY_WIFI_SSID       "ssid"
#define KEY_WIFI_PASSPHARASE       "passphrase"
#define KEY_WIFI_AUTH_TYPE       "auth_type"
#define KEY_WIFI_CRYPTO_TYPE       "crypto_type"

#define KEY_CLOUD_DOMAIN   "domain"
#define KEY_CLOUD_ADDRESS  "address"
#define KEY_CLOUD_PORT     "port"
#define KEY_FILE_PATH    "fileRWpath"
#define KEY_ATTR_PATH2   "path"

//#define KEY_AUTHTYPE        "authtype"
#define KEY_DEVICE         "device"
///kkk
#define KEY_DEVICE_SPECIFICATION    "specification"
#define KEY_DEVICE_SPECIFICATION_DEVICE     "device"
#define KEY_DEVICE_SPECIFICATION_DEVICE_DEVICETYPE      "deviceType"
#define KEY_DEVICE_SPECIFICATION_DEVICE_DEVICENAME      "deviceName"
#define KEY_DEVICE_SPECIFICATION_DEVICE_SPECVERSION     "specVersion"
#define KEY_DEVICE_SPECIFICATION_DEVICE_DATAMODELVERSION    "dataModelVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM   "platform"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURERNAME  "manufacturerName"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURERURI   "manufacturerUri"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURINGDATE "manufacturingDate"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_MODELNUMBER   "modelNumber"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_PLATFORMVERSION   "platformVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_OSVERSION     "osVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_HARDWAREVERSION   "hardwareVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_FIRMWAREVERSION   "firmwareVersion"
#define KEY_DEVICE_SPECIFICATION_PLATFORM_VENDORID  "vendorId"

#define KEY_RESOURCES       "resources"
#define KEY_RESOURCES_TYPE  "resourceTypes"

#define KEY_RESOURCES_COL   "collection"
#define KEY_RESOURCES_SIG   "single"

#define KEY_CONFIGURATION    "configuration"
#define KEY_CONFIGURATION_EASYSETUP    "easySetup"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY    "connectivity"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_TYPE    "type"

#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP    "softAP"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_MANUFACTUREID    "manufacturerId"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_SETUPID    "setupId"
#define KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_ARTIK    "artik"
#define KEY_CONFIGURATION_EASYSETUP_OWNERSHIP       "ownershipTransferMethod"

#define KEY_CONFIGURATION_WIFI    "wifi"
#define KEY_CONFIGURATION_WIFI_INTERFACES    "interfaces"
#define KEY_CONFIGURATION_WIFI_FREQUENCY    "frequency"

#define KEY_CONFIGURATION_FILEPATH      "filePath"
#define KEY_CONFIGURATION_FILEPATH_SVRDB    "svrdb"
#define KEY_CONFIGURATION_FILEPATH_PROVISIONING     "provisioning"
#define KEY_CONFIGURATION_FILEPATH_CERTIFICATE     "certificate"
#define KEY_CONFIGURATION_FILEPATH_PRIVATEKEY       "privateKey"
#define KEY_CONFIGURATION_DIR_RW_PATH    "dirRWPath"

#define KEY_DEVICE_RESOURCE_URI "uri"
#define KEY_DEVICE_RESOURCE_TYPES "types"
#define KEY_DEVICE_RESOURCE_INTERFACES "interfaces"
#define KEY_DEVICE_RESOURCE_POLICY "policy"
#define KEY_DEVICE_RESOURCE_COLLECTION_LINKS    "links"

#define KEY_DEVICE_RESOURCETYPE_TYPE   "type"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES    "properties"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES_KEY    "key"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES_TYPE    "type"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES_MANDATORY    "mandatory"
#define KEY_DEVICE_RESOURCETYPE_PROPERTIES_RW    "rw"

#define KEY_ATTR_DEVICE_DEVICEID     "devid"

#define CHECK_BIT(var,pos)  (((var)>>(pos)) & 1)

#define CHECK_READABLE(var) CHECK_BIT(var,0)
#define CHECK_WRITABLE(var) CHECK_BIT(var,1)

#define CHECK_DISCOVERABLE(var) CHECK_BIT(var,0)
#define CHECK_OBSERVABLE(var)   CHECK_BIT(var,1)
#define CHECK_SECURE(var)       CHECK_BIT(var,2)

#define MAX_FILE_ACCESS_CNT          (10)
#define MAX_KEY_LENGTH               (50)

#define MAX_URI_LENGTH_OCF          (128)
#define MAX_IT_CNT                    (5)
#define MAX_RT_CNT                   (10)

#define MAX_ATTRIBUTE_LENGTH_OCF     (64)

#define MAX_CLOUD_ADDRESS           (256) // [Jay] Need to match with the Cloud Spec.
#define MAX_CLOUD_SESSIONKEY        (128) // [Jay] Need to match with the Cloud Spec.

#define MAX_FILE_PATH_LENGTH        (250)

#define MAX_SUBDEVICE_EA            (100)

static bool isSupportUserDefDevList = true;         // It's allow to apply user-defined device-ID only when start Stack.
static char* UserDefDevList[MAX_SUBDEVICE_EA+1] = {0,};

static volatile int resourceTypeCnt = 0;

static char gOCFCloudFilePath       [MAX_FILE_PATH_LENGTH] = {0};
static char gOCFInfoFilePath        [MAX_FILE_PATH_LENGTH] = {0};
static char gSVRDBFilePath          [MAX_FILE_PATH_LENGTH] = {0};
static char gCertificateFilePath    [MAX_FILE_PATH_LENGTH] = {0};
static char gPrivateKeyFilePath     [MAX_FILE_PATH_LENGTH] = {0};

static char gCloudAddress           [MAX_CLOUD_ADDRESS] = {0};

static char *gManufacturerName;
static char *gSetupId;
static bool isArtik;

static struct hashmap * gResourceTypeHMap = NULL;  // map for resource types
static struct hashmap * gDeviceHMap = NULL;


static bool isThereCloudData = false;
static const char* originCloudJsonStr = "{\n\
        \"cloud\":    {\n\
            \"address\":    \"52.40.216.160:5683\"\n\
        },\n\
        \"wifi\": {\n\
            \"ssid\": \"unknown\"\n\        
        }\n\
}";

//jun
static wifi_manager_softap_config_s ap_config;
static easySetup_Connectivity_type es_conn_type = es_conn_type_NONE;
static wifi_manager_ap_config_s homeAP_info;

typedef int8_t     INT8;

#define STD_CI_CLOUD_ADDRESS "52.40.216.160:5683"
#define MAX_CI_CLOUD_ADDRESS 256
static ESCloud_SignUp *gpstCloudData = NULL;
static char gCICloudAddress           [MAX_CI_CLOUD_ADDRESS] = {0};

struct st_attribute const gstAttr_x_com_samsung_provisioninginfo[] = {		
    {		
        .key = "x.com.samsung.provisioning.targets", 		
        .type = 9, 		
        .mandatory = false,		
        .rw= 1		
    },		
    {		
        .key = "x.com.samsung.provisioning.owned",		
        .type = 0, 		
        .mandatory = false,		
        .rw= 1		
    },		
    {		
        .key = "x.com.samsung.provisioning.easysetupdi",		
        .type = 3, 		
        .mandatory = false,		
        .rw= 1		
    },		
    {		
        .key = "x.com.samsung.provisioning.reset",		
        .type = 0, 		
        .mandatory = false,		
        .rw= 3		
    },		
    {		
        .key = "x.com.samsung.provisioning.stopAP",		
        .type = 0, 		
        .mandatory = false,		
        .rw= 3		
    }
};

const struct st_attribute const gstAttr_x_com_samsung_accesspointlist[] = {		
    {		
        .key = "x.com.samsung.accesspoint.items", 		
        .type = 9, 		
        .mandatory = true,		
        .rw= 1		
    }		
};

static const struct st_resourceType const gstResourceTypes[] = {		
    {		
        .rt = "x.com.samsung.provisioninginfo", 		
        .prop_cnt = 5, 		
        .prop[0] = &gstAttr_x_com_samsung_provisioninginfo[0], 		
        .prop[1] = &gstAttr_x_com_samsung_provisioninginfo[1], 		
        .prop[2] = &gstAttr_x_com_samsung_provisioninginfo[2], 		
        .prop[3] = &gstAttr_x_com_samsung_provisioninginfo[3], 		
        .prop[4] = &gstAttr_x_com_samsung_provisioninginfo[4]		
    },	
    {		
        .rt = "x.com.samsung.accesspointlist", 		
        .prop_cnt = 1, 		
        .prop[0] = &gstAttr_x_com_samsung_accesspointlist[0]		
    }
};

static const struct st_resource const gstResources[] = {		
    {		
        .uri="/sec/provisioninginfo",		
        .interfaceTypes = {"oic.if.a"},		
        .resourceTypes= {"x.com.samsung.provisioninginfo"},		
        .ifcnt=1,		
        .rtcnt=1,		
        .policy= 3		
    },
    {		
        .uri="/sec/accesspointlist",		
        .interfaceTypes = {"oic.if.s"},		
        .resourceTypes= {"x.com.samsung.accesspointlist"},		
        .ifcnt=1,		
        .rtcnt=1,		
        .policy= 3		
    }    	
};


bool UtilStrCopy(char **dest,const char *src)
{
  if(NULL == src || NULL == dest) {
    return false;
  }

  int srcLen = strlen(src);

  (*dest) = (char *)DAMalloc(srcLen+1);
  if(NULL == *dest) {
	  THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Malloc failed");
    return false;
  }

  memcpy((*dest),src,srcLen+1);
  return true;
}

bool getSignUPDataforCloud(ESCloud_SignUp **pstDstCloudData)
{
#if 0
	bool retValue = false;
	Remote *pstRemote = NULL;

	CHAR * pchCertifiacte = NULL;

	if(pstDstCloudData)
	{
		pstRemote = CreateRemote();

		memset(pstRemote, 0, sizeof(Remote));

		if(ReadRemoteAccessInfo(pstRemote) == DAWIT_FALSE)
		{
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Failed to read the RA details");
			DeleteRemote(pstRemote , DAWIT_FALSE);
			return false;
		}

		THINGS_LOG_D(THINGS_INFO, TAG, "Read Remote details [%s]",pstRemote->pchaccessToken);
		THINGS_LOG_D(THINGS_INFO, TAG, "pchdomain [%s]",pstRemote->pchdomain);
		THINGS_LOG_D(THINGS_INFO, TAG, "pchport [%s]",pstRemote->pchport);
		THINGS_LOG_D(THINGS_INFO, TAG, "pchaddress [%s]",pstRemote->pchaddress);
		THINGS_LOG_D(THINGS_INFO, TAG, "pchredirectURI [%s]",pstRemote->pchredirectURI);

		pchCertifiacte = malloc(4096);

		if(pchCertifiacte == NULL)
		{
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "alloc failed");
			goto SE_READ_END;
		}

		memset(pchCertifiacte, 0, 4096);

		if(DawitGetCertificate(SIGN_IN_CERT_CA, &pchCertifiacte, 0x00) == DAWIT_FALSE)
		{
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Failed to read Certificate details");
			retValue = false;
			goto SE_READ_END;
		}

		UtilStrCopy(&(*pstDstCloudData)->accessToken, pstRemote->pchaccessToken);
		UtilStrCopy(&(*pstDstCloudData)->address, pstRemote->pchaddress);
		UtilStrCopy(&(*pstDstCloudData)->domain, pstRemote->pchdomain);
		UtilStrCopy(&(*pstDstCloudData)->port, pstRemote->pchport);
		UtilStrCopy(&(*pstDstCloudData)->redirectURI, pstRemote->pchredirectURI);
		UtilStrCopy(&(*pstDstCloudData)->refreshToken, pstRemote->pchrefreshToken);
		UtilStrCopy(&(*pstDstCloudData)->sid, pstRemote->pchsid);
		(*pstDstCloudData)->tokenType = NULL;
		UtilStrCopy(&((*pstDstCloudData)->uid), pstRemote->pchuid);

		UtilStrCopy(&(*pstDstCloudData)->certificate, pchCertifiacte);
		THINGS_LOG_D(THINGS_DEBUG, TAG,"Certificate[%s]",(*pstDstCloudData)->certificate);
		dawit_task_sleep(300);
		retValue =  true;
	}

	SE_READ_END:
	if(pchCertifiacte) free(pchCertifiacte);
	DeleteRemote(pstRemote , DAWIT_FALSE);

    return retValue;
#endif
    return 0;
}

bool notifyCloudDataTOApplication(ESCloud_SignUp *pstCloudData)
{
    #if 0
	if(pstCloudData)
	{
		//pfnCallbackForApplnUser(pstCloudData);
		Remote stRemote;
		memset(&stRemote, 0 , sizeof(stRemote));
		stRemote.pchaccessToken = pstCloudData->accessToken;
		stRemote.pchaddress = pstCloudData->address;
		stRemote.pchdomain = pstCloudData->domain;
		stRemote.pchport = pstCloudData->port;
		stRemote.pchredirectURI = pstCloudData->redirectURI;
		stRemote.pchrefreshToken = pstCloudData->refreshToken;
		stRemote.pchsid = pstCloudData->sid;
		stRemote.pchuid = pstCloudData->uid;
		stRemote.udwexpireTime = pstCloudData->expireTime;
		INT8 ret =0;

		if(pstCloudData->certificate == NULL)
		{
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Not Received Certificates for SignIN");
			return false;
		}

		THINGS_LOG_D(THINGS_DEBUG, TAG, "Saving the Certificate [%s]", pstCloudData->certificate);

		//DawitSaveCertificate(SIGN_IN_CERT_CA, pstCloudData->certificate , 0); // pkss

		//SaveRemoteAccessInfo(&stRemote,  OCGetServerInstanceIDString(), 0); // pkss

	return true;

    }
    #endif
	return false;
}
////

bool UpdateAndSaveDeviceInfo(STDevice** devList, int devCnt)
{
	assert(1);
	return false;
}
/**
 *	@fn GetCloudServerAddress
 *	@brief Function to get cloud server address
 * @param pchCurrentServer
 * @return
 */
char *GetCloudServerAddress(char* pchCurrentServer)
{
	if(gpstCloudData != NULL)
	{
		memset(gCICloudAddress, 0, MAX_CI_CLOUD_ADDRESS);
		memcpy( gCICloudAddress, gpstCloudData->address, strlen(gpstCloudData->address)+1);
	}
	else
	{
		memcpy( gCICloudAddress, STD_CI_CLOUD_ADDRESS, strlen(STD_CI_CLOUD_ADDRESS) );
	}
	THINGS_LOG_D(THINGS_DEBUG, TAG, "CLOUD CI Address : %s", gCICloudAddress);

	return gCICloudAddress;
}

static struct st_attribute * CreateProperty()
{
    struct st_attribute * property = DAMalloc(sizeof(st_attribute));

    memset(property->key,0, (size_t)MAX_KEY_LENGTH);
    property->type = 0;
    property->mandatory = false;
    property->rw = 0;

    return property;
}

static struct st_resourceType * CreateResourceType()
{
    struct st_resourceType * type = DAMalloc(sizeof(st_resourceType));

    memset(type->rt,    0, (size_t)MAX_ATTRIBUTE_LENGTH_OCF);
    memset(type->prop,  0, (size_t)MAX_PROPERTY_CNT);
    type->prop_cnt = 0;

    return type;
}

static struct st_resource * CreateResource()
{
    struct st_resource * resource = DAMalloc(sizeof(st_resource));

    memset(resource->uri,   0,(size_t)MAX_URI_LENGTH_OCF);
    memset(resource->interfaceTypes,   0, (size_t)MAX_IT_CNT);
    memset(resource->resourceTypes,   0, (size_t)MAX_RT_CNT);
    resource->policy = 0;
    resource->ifcnt = 0;
    resource->rtcnt = 0;

    return resource;
}

static STDevice * CreateDevice()
{
    STDevice * device = DAMalloc(sizeof(STDevice));

    memset(device->type,        0,(size_t)MAX_DEVICE_TYPE_LENGTH);
    memset(device->name,        0,(size_t)MAX_DEVICE_NAME_LENGTH);
    memset(device->modelNum,    0,(size_t)MAX_DEVICE_MODEL_ID_LENGTH);
    memset(device->ver_p,       0,(size_t)MAX_DEVICE_VER_P);
    memset(device->ver_os,      0,(size_t)MAX_DEVICE_VER_OS);
    memset(device->ver_hw,      0,(size_t)MAX_DEVICE_VER_HW);
    memset(device->ver_fw,      0,(size_t)MAX_DEVICE_VER_FW);
    memset(device->device_id,   0,(size_t)MAX_DEVICE_ID_LENGTH);
    memset(device->vender_id,   0,(size_t)MAX_DEVICE_VENDER_ID);
    memset(device->description, 0,(size_t)MAX_DEVICE_DESCRIPTION_LENGTH);

    device->no = -1;
    device->capaCnt = 0;
    device->colCnt = 0;
    device->sigCnt = 0;
    device->isPhysical = 0;

    for(int index = 0 ; index < MAX_DEVICE_CAPABILTY_CNT ; index++)
    {
        device->pChildResources[index] = NULL;
    }

    return device;
}

static struct st_file * CreateFile()
{
    struct st_file* file = DAMalloc(sizeof(st_file));

    memset(file->path,        0,(size_t)MAX_FILE_PATH_LENGTH);

    file->id = -1;

    return file;
}

static size_t GetJSONFileSize(const char *fileName)
{
    size_t size = 0;
    FILE* fp = fopen(fileName, "r");

    if (fp)
    {
        if( 0 == fseek(fp, 0L, SEEK_END) )
        {
            size = ftell(fp);
            rewind(fp);
        }
        fclose(fp);
    }
    return size;
}

static char* GetJsonStringFromFile(const char* fileName)
{
    FILE* fp = NULL;
    char* jsonStr = NULL;
    size_t size = GetJSONFileSize(fileName);
    size_t readedT = 0;
    int fpAcessCnt = 0;

    if ( size <= 0)
    {
        THINGS_LOG_D (THINGS_ERROR, TAG, "Failed converting to JSON");
        return NULL;
    }
    else
    {
        THINGS_LOG_D (THINGS_DEBUG, TAG, "File size: %d ", size);
    }

    jsonStr = (char *)DAMalloc(size + 1);

    // 1. File Read
    fp = fopen(fileName, "r");
    if(NULL == fp)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Failed to open %s", fileName);
        DAFree(jsonStr);
        return NULL;
    }
    // 2. Json String read from the given file.
    size_t bytesRead = 0;
    while(readedT < size && fpAcessCnt < MAX_FILE_ACCESS_CNT)
    {
        bytesRead = fread(&(jsonStr[readedT]), 1, size-readedT, fp);
        readedT += bytesRead;
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Read Size: %d, total Size =%d", readedT, size);
        fpAcessCnt++;
    }

    if( fpAcessCnt >= MAX_FILE_ACCESS_CNT )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Access-Times is Over for File Read. (Readed Size: %d, total Size =%d)", readedT, size);
        DAFree(jsonStr);
        fclose(fp);
        return NULL;
    }

    jsonStr[readedT] = '\0';

    fclose(fp);
    fp = NULL;

    return jsonStr;
}

//[Jay] Will be used later
static int SetJsonStringIntoFile(const char* fileName, const char* jsonStr)
{
    FILE* fp = NULL;
    // 1. File Read
    fp = fopen(fileName, "w");
    if(NULL == fp)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Failed to open \"%s\" with Write permission.", fileName);
        return 0;
    }
    // 2. Json String read from the given file.
    fwrite(jsonStr, 1, strlen(jsonStr), fp);

    fclose(fp);
    fp = NULL;
    return 1;
}

static int InitOCFInfoFile(cJSON * cjson, char* originJsonStr)
{
    int res = 1;

    if( gOCFInfoFilePath[0] == 0 )
    {
        int length = 0;
        cJSON * fileRWPath = cJSON_GetObjectItem(cjson, KEY_FILE_PATH);
        if(NULL == fileRWPath || (length=strlen(fileRWPath->valuestring)) <= 0)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "\"%s\" key is not exist in ocf_info file. or value is empty.", KEY_FILE_PATH);
            return 0;
        }

        memset(gOCFInfoFilePath, 0, MAX_FILE_PATH_LENGTH);
        memcpy(gOCFInfoFilePath, fileRWPath->valuestring, length+1);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "OCF_INFO file path : %s", gOCFInfoFilePath);
    }

    FILE* fp = fopen(gOCFInfoFilePath, "r");
    if (fp)
    {
        fclose(fp);


        int isValid = 1;

        char* jsonStr = GetJsonStringFromFile(gOCFInfoFilePath);
        if(jsonStr != NULL)
        {
            cJSON * root = cJSON_Parse((const char *)jsonStr);
            if(root == NULL)
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "%s fail to parsed.", gOCFInfoFilePath);
                isValid = 0;
            }
        }
        else
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "%s is empty.", gOCFInfoFilePath);
            isValid = 0;
        }

        if(isValid == 0)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Try to reset the ocf_info.");
            if(SetJsonStringIntoFile(gOCFInfoFilePath, originJsonStr) == 0 )
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "original ocf_info.json file copy failed.");
                res = 0;
            }
        }
    }
    else
    {
        if( SetJsonStringIntoFile(gOCFInfoFilePath, originJsonStr) == 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "original ocf_info.json file copy failed.");
            res = 0;
        }
    }

    return res;
}

static int GetJSONInt(cJSON* json, int64_t* variable)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter");

    int res = 1;

    if( json == NULL || variable == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "json = %d, variable = %d", json, variable);
        return 0;
    }

    if( json->type != cJSON_Number )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "json-value Type is Not String value.");
        return 0;
    }

    *variable = json->valueint;

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit");

    return res;
}

static int GetJSONString(cJSON* json, char** variable)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter");

    int res = 1;
    int length = 0;

    if( json == NULL || variable == NULL || *variable != NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "There is NULL value.(json = %d, variable = %d)", json, variable);
        if(variable != NULL)
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "variable value is not NULL.(*variable = %d)", *variable);
        return 0;
    }

    if( json->type != json->type )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "json-value Type is Not String value.");
        return 0;
    }

    if( (length = strlen(json->valuestring)) == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "length is NULL");
        return 0;
    }

    *variable = (char*)DAMalloc(sizeof(char) * (length+1));
    if( *variable == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.");
        return 0;
    }

    memset(*variable, 0, length+1);
    memcpy(*variable, json->valuestring, length+1);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit");

    return res;
}

static int LoadCloudSignUPData(cJSON* json, ESCloud_SignUp** clData)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter");

    int res = 1;

    if( json == NULL || clData == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "json = %d, clData = %d", json, clData);
        return -1;
    }

    cJSON * domain  = cJSON_GetObjectItem(json, KEY_CLOUD_DOMAIN);
    cJSON * address = cJSON_GetObjectItem(json, KEY_CLOUD_ADDRESS);
    cJSON * port    = cJSON_GetObjectItem(json, KEY_CLOUD_PORT);
    cJSON * accesstoken = cJSON_GetObjectItem(json, KEY_TOKEN_ACCESS);
    cJSON * refreshaccesstoken = cJSON_GetObjectItem(json, KEY_TOKEN_ACCESS_REFRESH);
    cJSON * tokentype = cJSON_GetObjectItem(json, KEY_TOKEN_TYPE);
    cJSON * expiretime = cJSON_GetObjectItem(json, KEY_EXPIRE_TIME);
    cJSON * userID = cJSON_GetObjectItem(json, KEY_ID_USER);
    cJSON * redirecturi = cJSON_GetObjectItem(json, KEY_SERVER_REDIRECT_URI);
    cJSON * certificate = cJSON_GetObjectItem(json, KEY_CERTIFICATE_FILE);
    cJSON * serverID = cJSON_GetObjectItem(json, KEY_SERVER_ID);

    if( address == NULL || accesstoken == NULL || refreshaccesstoken == NULL || userID == NULL || expiretime == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "address = %d, accesstoken = %d, refreshaccesstoken = %d, userID = %d, expiretime = %d", address, accesstoken, refreshaccesstoken, userID, expiretime);
        return -2;
    }

    if( ESCloud_SignUp_init( clData ) == false )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ESCloud data initialization is failed.");
        return -1;
    }

    if( GetJSONString( address, &((*clData)->address) ) == 0 )
     {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Load JSON String address is failed");
        res = -1;
     }

    if( GetJSONString( accesstoken, &((*clData)->accessToken) ) == 0 )
     {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Load JSON String accesstoken is failed");
        res = -1;
     }

    if( GetJSONString( refreshaccesstoken, &((*clData)->refreshToken) ) == 0 )
     {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Load JSON String refreshaccesstoken is failed");
        res = -1;
     }

    if( GetJSONString( userID, &((*clData)->uid) ) == 0 )
     {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Load JSON String userID is failed");
        res = -1;
     }

    if( GetJSONInt( expiretime, &((*clData)->expireTime) ) == 0 )
     {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Load JSON String expiretime is failed");
        res = -1;
     }

    GetJSONString( tokentype, &((*clData)->tokenType) );
    GetJSONString( redirecturi, &((*clData)->redirectURI) );
    GetJSONString( certificate, &((*clData)->certificate) );
    GetJSONString( serverID, &((*clData)->sid) );
    GetJSONString( domain, &((*clData)->domain) );
    GetJSONString( port, &((*clData)->port) );

    if( res == -1 )
    {
        ESCloud_SignUp_clear(*clData);
        *clData = NULL;
    }
    else
    {
        memcpy( gCloudAddress, (*clData)->address, strlen((*clData)->address) + 1 );
        THINGS_LOG_D(THINGS_DEBUG, TAG, "CLOUD CI Address : %s", gCloudAddress);
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit");

    return res;
}

static char* Get_RW_OCFInfoFilePath(const char* fileName)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    char* jsonStr = GetJsonStringFromFile(fileName);

    if(jsonStr != NULL && strlen(jsonStr) > 0)
    {
        // 3. Parse the Json string
        cJSON *root = cJSON_Parse((const char *)jsonStr);
        if(root == NULL)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "root of cJSON is NULL. (cJSON format error)");
            DAFree(jsonStr);
            return NULL;
        }

        if( InitOCFInfoFile(root, jsonStr) == 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ocf_info.json file Initialization failed.");
            cJSON_Delete(root);
            DAFree(jsonStr);
            return NULL;
        }

        cJSON_Delete(root);
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "json string of ocf_info.json is empty.");
    }

    if(jsonStr != NULL)
    {
        DAFree(jsonStr);
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return gOCFInfoFilePath;
}

static int IFneedInitOCFCloudFile(const char* fileName)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    int res = 0;

    FILE* fp = fopen(fileName, "r");
    if (fp)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Already Exist : provisioning file.");
        fclose(fp);
    }
    else
    {
        if( SetJsonStringIntoFile(fileName, originCloudJsonStr) == 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Creating privisioning file is failed.");
            res = -1;
        }
        else
        {
            THINGS_LOG_D(THINGS_INFO, TAG, "***Creating privisioning file is Success.");
            res = 1;
        }
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return res;
}

// djb2 algorithm
unsigned long GetHashKey(const char* str)
{
    unsigned long hash = 5381;
    int index = 0;
    int c;

    while(c = str[index++])
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

//jun
easySetup_Connectivity_type DM_GetEasySetupConnectivityType(void){
    return es_conn_type;
    
}
//jun
wifi_manager_softap_config_s* DM_GetSoftAPWifiConfig(void){

    char* tempSSID = "samsung_E1";
    char* tempPASSPHRASE = "1111122222";
    int tempCHANNEL = 1;

    strncpy(ap_config.ssid, tempSSID, strlen(tempSSID)+1);
    strncpy(ap_config.passphrase, tempPASSPHRASE, strlen(tempPASSPHRASE)+1);                                   
    ap_config.channel = tempCHANNEL;
    
    return &ap_config;
}

wifi_manager_ap_config_s* DM_getHomeAPWifiConfig(void){
    return &homeAP_info;
}

int ParseOCFCloudJson(const char* fileName)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    int ret = 0;
    char* jsonStr = NULL;

    if( IFneedInitOCFCloudFile(fileName) == -1 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "RW CloudFile-loading is failed. please check your \"%s\"", fileName);
        return 0;
    }

RETRY_JSON:
    jsonStr = GetJsonStringFromFile(fileName);

    if(jsonStr == NULL)
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "ocf_cloud.json file initialization.");
        if( SetJsonStringIntoFile(fileName, originCloudJsonStr) == 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] Creating ocf_cloud.json file is failed.");
            return 0;
        }
        goto RETRY_JSON;
    }

    if(strlen(jsonStr) > 0)
    {
        // 3. Parse the Json string
        cJSON *root = cJSON_Parse((const char *)jsonStr);

        if( root == NULL )
        {
            THINGS_LOG_V(THINGS_INFO, TAG, "ocf_cloud.json file context has a error about json-format.");
            DAFree(jsonStr);
            jsonStr = NULL;

            THINGS_LOG_V(THINGS_INFO, TAG, "ocf_cloud.json file initialization.");
            if( SetJsonStringIntoFile(fileName, originCloudJsonStr) == 0 )
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] Creating ocf_cloud.json file is failed.");
                return 0;
            }
            goto RETRY_JSON;
        }

        cJSON *cloud = cJSON_GetObjectItem(root, KEY_CLOUD);
        cJSON *wifi = cJSON_GetObjectItem(root, KEY_WIFI);

        if(NULL != wifi && NULL != cloud)
        {
            /***** Cloud *****/
            cJSON * address = cJSON_GetObjectItem(cloud, KEY_CLOUD_ADDRESS);
            if(NULL != address)
            {
                memset(gCloudAddress, 0, (size_t)MAX_CLOUD_ADDRESS);
                memcpy(gCloudAddress, address->valuestring, strlen(address->valuestring)+1);
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[CLOUD] CI Address : %s", gCloudAddress);
                ret = 1;
            }

            cJSON * accesstoken = cJSON_GetObjectItem(cloud, KEY_TOKEN_ACCESS);
            cJSON * refreshaccesstoken = cJSON_GetObjectItem(cloud, KEY_TOKEN_ACCESS_REFRESH);
            cJSON * userID = cJSON_GetObjectItem(cloud, KEY_ID_USER);
            cJSON * expiretime = cJSON_GetObjectItem(cloud, KEY_EXPIRE_TIME);


            /***** Wifi *****/
            cJSON * ssid = cJSON_GetObjectItem(wifi, KEY_WIFI_SSID);
            if(NULL != ssid)
            {
                memcpy(homeAP_info.ssid, ssid->valuestring, strlen(ssid->valuestring)+1);
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[WIFI] SSID Address : %s", homeAP_info.ssid);
                ret = 1;
                homeAP_info.ssid_length = strlen(homeAP_info.ssid);
            }

            cJSON * passphrase = cJSON_GetObjectItem(wifi, KEY_WIFI_PASSPHARASE);
            if(NULL != passphrase)
            {
                memcpy(homeAP_info.passphrase, passphrase->valuestring, strlen(passphrase->valuestring)+1);
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[WIFI] PASSPHRASE Address : %s", homeAP_info.passphrase);
                ret = 1;
                homeAP_info.passphrase_length = strlen(homeAP_info.passphrase);
            }

            cJSON * auth_type = cJSON_GetObjectItem(wifi, KEY_WIFI_AUTH_TYPE);
          
            if(NULL != auth_type)
            {
                homeAP_info.ap_auth_type = auth_type->valueint;
            }

            cJSON * crypto_type = cJSON_GetObjectItem(wifi, KEY_WIFI_CRYPTO_TYPE);
            if(NULL != crypto_type)
            {
                homeAP_info.ap_crypto_type = auth_type->valueint;
            }


            if( address == NULL || accesstoken == NULL || refreshaccesstoken == NULL || userID == NULL || expiretime == NULL || ssid == NULL || passphrase == NULL || auth_type == NULL || crypto_type == NULL)
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ProvisionInfo] address = %d, accesstoken = %d, refreshaccesstoken = %d, userID = %d, expiretime = %d", address, accesstoken, refreshaccesstoken, userID, expiretime);
                isThereCloudData = false;
            }
            else
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ProvisionInfo] There is mandatory elements in ocf_cloud.json.");
                isThereCloudData = true;
            }
        }

       

           

        if(NULL!= root)
            cJSON_Delete(root);
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Can not Read \"%s\" file.", fileName);
    }

    if(jsonStr != NULL)
    {
        DAFree(jsonStr);
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return ret;
}

static int ParseOCFInfoJson(const char* fileName)
{

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    int ret = 1;
    char* jsonStr = GetJsonStringFromFile(fileName);
  
    if(jsonStr != NULL && strlen(jsonStr) > 0)
    {
        // 3. Parse the Json string
        cJSON *root = cJSON_Parse((const char *)jsonStr);
        assert(root != NULL);

        STDevice * node = NULL;

        // Device Items
        cJSON *device = cJSON_GetObjectItem(root, KEY_DEVICE);
        int deviceCnt = cJSON_GetArraySize(device);

        THINGS_LOG_D(THINGS_DEBUG, TAG, "deviceCnt = %d", deviceCnt);
        if( gDeviceHMap == NULL )
            gDeviceHMap = hashmapCreate(deviceCnt);

        if( gDeviceHMap == NULL )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "gDeviceHMap is NULL");
            cJSON_Delete(root);

            if(jsonStr != NULL)
            {
                DAFree(jsonStr);
            }
            return 0;
        }

        THINGS_LOG_D(THINGS_DEBUG, TAG, "deviceCnt of hashmap = %d", hashmapCount(gDeviceHMap) );

        for (int deviceNum = 0; deviceNum < deviceCnt; deviceNum++)
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] ============================================");

            node = CreateDevice();
            node->no = deviceNum; ///kkk temp
            node->isPhysical = 1;

            cJSON * device_item = cJSON_GetArrayItem(device, deviceNum);
            cJSON *specification = cJSON_GetObjectItem(device_item, KEY_DEVICE_SPECIFICATION);
            if(NULL != specification)
            {
                cJSON *spec_device = cJSON_GetObjectItem(specification, KEY_DEVICE_SPECIFICATION_DEVICE);
                if(NULL != spec_device)
                {
                    cJSON *deviceType = cJSON_GetObjectItem(spec_device, KEY_DEVICE_SPECIFICATION_DEVICE_DEVICETYPE);
                    cJSON *deviceName = cJSON_GetObjectItem(spec_device, KEY_DEVICE_SPECIFICATION_DEVICE_DEVICENAME);
                    cJSON *specVersion = cJSON_GetObjectItem(spec_device, KEY_DEVICE_SPECIFICATION_DEVICE_SPECVERSION);
                    cJSON *dataModelVersion = cJSON_GetObjectItem(spec_device, KEY_DEVICE_SPECIFICATION_DEVICE_DATAMODELVERSION);

                    if(NULL != deviceType)
                    {
                        memcpy(node->type, deviceType->valuestring, strlen(deviceType->valuestring)+1);
                    }

                    if(NULL != deviceName){
                        memcpy(node->name, deviceName->valuestring, strlen(deviceName->valuestring)+1);
                    }
                }

                cJSON *spec_platform = cJSON_GetObjectItem(specification, KEY_DEVICE_SPECIFICATION_PLATFORM);
                if(NULL != spec_platform)
                {
                    cJSON *manufacturerName = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURERNAME);
                    cJSON *manufacturerUri = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURERURI);
                    cJSON *manufacturingDate = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MANUFACTURINGDATE);
                    cJSON *modelNumber = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_MODELNUMBER);
                    cJSON *platformVersion = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_PLATFORMVERSION);
                    cJSON *osVersion = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_OSVERSION);
                    cJSON *hardwareVersion = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_HARDWAREVERSION);
                    cJSON *firmwareVersion = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_FIRMWAREVERSION);
                    cJSON *vendorId = cJSON_GetObjectItem(spec_platform, KEY_DEVICE_SPECIFICATION_PLATFORM_VENDORID);

                    if(NULL != modelNumber) memcpy(node->modelNum, modelNumber->valuestring, strlen(modelNumber->valuestring)+1);
                    if(NULL != platformVersion)     memcpy(node->ver_p, platformVersion->valuestring, strlen(platformVersion->valuestring)+1);
                    if(NULL != osVersion)    memcpy(node->ver_os, osVersion->valuestring, strlen(osVersion->valuestring)+1);
                    if(NULL != hardwareVersion)    memcpy(node->ver_hw, hardwareVersion->valuestring, strlen(hardwareVersion->valuestring)+1);
                    if(NULL != firmwareVersion)    memcpy(node->ver_fw, firmwareVersion->valuestring, strlen(firmwareVersion->valuestring)+1);
                    if(NULL != vendorId) memcpy(node->vender_id, vendorId->valuestring, strlen(vendorId->valuestring)+1);
                }
            }
            THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] type : %s", (node->type));
            THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] name : %s", (node->name));
            THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] model num : %s", (node->modelNum));
            THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] plat. ver : %s", (node->ver_p));
            THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] os version : %s", (node->ver_os));
            THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] hw version : %s", (node->ver_hw));
            THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] fw version : %s", (node->ver_fw));
            THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] vender id : %s", (node->vender_id));
            
            ///kkk resources
            cJSON * resources   = cJSON_GetObjectItem(device_item, KEY_RESOURCES);
            if(resources != NULL)
            {              
                // 1. Read thsoe resource(s) which will be binded into the Collection resource(/device/#)
                cJSON * collection = cJSON_GetObjectItem(resources, KEY_RESOURCES_COL);

                if(collection)
                {
                    node->colCnt = cJSON_GetArraySize(collection);
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] Resources for Collection Cnt : %d", node->colCnt);

                    for (int iter = 0; iter < node->colCnt; iter++)
                    {
                        cJSON * res = cJSON_GetArrayItem(collection, iter);
                        if( res->type != NULL)
                        {                            
                            cJSON * uri = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_URI);
                            memcpy(node->collection[iter].uri, uri->valuestring, strlen(uri->valuestring)+1);
                            THINGS_LOG_D(THINGS_INFO, TAG, "***[COLLECTION] collection[0].uri : %s", (node->collection[iter].uri));
                            
                            cJSON * types = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_TYPES);
                            if(types){
                                int typeCnt = cJSON_GetArraySize(types);   
                                node->collection[iter].rtcnt = typeCnt;         
                                for (int typeiter = 0; typeiter < typeCnt; typeiter++)
                                {
                                    cJSON * type = cJSON_GetArrayItem(types, typeiter);
                                    node->collection[iter].resourceTypes[typeiter] = DAMalloc(sizeof(char*) * strlen(type->valuestring)+1);
                                    memcpy(node->collection[iter].resourceTypes[typeiter], type->valuestring, strlen(type->valuestring)+1);
                                    THINGS_LOG_D(THINGS_INFO, TAG, "***[COLLECTION] collection[iter].resourceTypes[typeiter] : %s", (node->collection[iter].resourceTypes[typeiter]));
                                }
                            }else{
                                return 0;
                            }
                           
                            cJSON * interfaces = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_INTERFACES);
                            if(interfaces){
                                int ifCnt = cJSON_GetArraySize(interfaces);   
                                node->collection[iter].ifcnt = ifCnt;         
                                for (int ifiter = 0; ifiter < ifCnt; ifiter++)
                                {
                                    cJSON * interface = cJSON_GetArrayItem(interfaces, ifiter);
                                    node->collection[iter].interfaceTypes[ifiter] = DAMalloc(sizeof(char*) * strlen(interface->valuestring)+1);
                                    memcpy(node->collection[iter].interfaceTypes[ifiter], interface->valuestring, strlen(interface->valuestring)+1);
                                }
                            }else{
                                return 0;
                            }
                            
                            cJSON * policy = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_POLICY);
                            node->collection[iter].policy = policy->valueint;
                            THINGS_LOG_D(THINGS_INFO, TAG, "***[COLLECTION] collection[iter].policy : %d", (node->collection[iter].policy));
                          
                            cJSON * links = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_COLLECTION_LINKS);
                            if(links){
                                int linkCnt = cJSON_GetArraySize(links);    
                                node->collection[iter].linkcnt = linkCnt;  
                                THINGS_LOG_D(THINGS_INFO, TAG, "***[COLLECTION] collection[iter].linkcnt : %d", (node->collection[iter].linkcnt));        
                                for (int linkiter = 0; linkiter < linkCnt; linkiter++)
                                {                                
                                    cJSON * link = cJSON_GetArrayItem(links, linkiter);

                                    struct st_resource * link_resource = CreateResource();
                     
                                    cJSON * uri = cJSON_GetObjectItem(link, KEY_DEVICE_RESOURCE_URI);
                                    memcpy(link_resource->uri, uri->valuestring, strlen(uri->valuestring)+1);
                                    THINGS_LOG_D(THINGS_INFO, TAG, "***[COLLECTION] link_resource->uri : %s", (link_resource->uri)); 
        
                                    cJSON * types = cJSON_GetObjectItem(link, KEY_DEVICE_RESOURCE_TYPES);
                                    if(types){
                                        int typeCnt = cJSON_GetArraySize(types); 
                                        link_resource->rtcnt = typeCnt;
                                        for (int typeiter = 0; typeiter < typeCnt; typeiter++)
                                        {
                                            cJSON * type = cJSON_GetArrayItem(types, typeiter);
                                            link_resource->resourceTypes[typeiter] = DAMalloc(sizeof(char*) * strlen(type->valuestring)+1);
                                            memcpy(link_resource->resourceTypes[typeiter], type->valuestring, strlen(type->valuestring)+1);
                                        }
                                    }else{
                                        return 0;
                                    }
                                    cJSON * interfaces = cJSON_GetObjectItem(link, KEY_DEVICE_RESOURCE_INTERFACES);
                                    if(interfaces){
                                        int ifCnt = cJSON_GetArraySize(interfaces);  
                                        link_resource->ifcnt = ifCnt;          
                                        for (int ifiter = 0; ifiter < ifCnt; ifiter++)
                                        {
                                            cJSON * interface = cJSON_GetArrayItem(interfaces, ifiter);
                                            link_resource->interfaceTypes[ifiter] = DAMalloc(sizeof(char*) * strlen(interface->valuestring)+1);
                                            memcpy(link_resource->interfaceTypes[ifiter], interface->valuestring, strlen(interface->valuestring)+1);
                                        }
                                    }else{
                                        return 0;
                                    }
                                    cJSON * policy = cJSON_GetObjectItem(link, KEY_DEVICE_RESOURCE_POLICY);
                                    link_resource->policy = policy->valueint;
                                    node->collection[iter].links[linkiter] =  link_resource;                                  
                                }
                            }else{
                                return 0;
                            }
                        }
                       
                    }
                    ret = 1;

                    THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] URI. : %s", node->collection[0].uri);
                    THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] interfaceType. : %s", node->collection[0].interfaceTypes[0]);
                    THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] resourceTypes. : %s", node->collection[0].resourceTypes[0]);
                    THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] ifcnt. : %d", node->collection[0].ifcnt);
                    THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] rtcnt. : %d", node->collection[0].rtcnt);
                    THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] linkcnt. : %d", node->collection[0].linkcnt);
                    THINGS_LOG_D(THINGS_INFO, TAG, "[COLLECTION] policy. : %d", node->collection[0].policy);
                }
                else
                {
                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Children Reosurces Not Exist");
                    //ret = 0;
                }
              
                // 2. Read thsoe resource(s) which will not be binded into the Collection resource
                cJSON * single = cJSON_GetObjectItem(resources, KEY_RESOURCES_SIG);

                if(single)
                {                   
                    node->sigCnt = cJSON_GetArraySize(single);
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] Resources for Single Usage Cnt : %d", node->sigCnt);

                    for (int iter = 0; iter < node->sigCnt; iter++)
                    {
                        cJSON * res = cJSON_GetArrayItem(single, iter);
                        if(res->type != NULL)
                        {                         
                            cJSON * uri = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_URI);
                            memcpy(node->single[iter].uri, uri->valuestring, strlen(uri->valuestring)+1);

                            cJSON * types = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_TYPES);
                            if(types){
                                int typeCnt = cJSON_GetArraySize(types); 
                                node->single[iter].rtcnt = typeCnt;
                                for (int typeiter = 0; typeiter < typeCnt; typeiter++)
                                {
                                    cJSON * type = cJSON_GetArrayItem(types, typeiter);
                                    node->single[iter].resourceTypes[typeiter] = DAMalloc(sizeof(char*) * strlen(type->valuestring)+1);
                                    memcpy(node->single[iter].resourceTypes[typeiter], type->valuestring, strlen(type->valuestring)+1);
                                }
                            }else{
                                return 0;
                            }
                            cJSON * interfaces = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_INTERFACES);
                            if(interfaces){
                                int ifCnt = cJSON_GetArraySize(interfaces);    
                                node->single[iter].ifcnt = ifCnt;        
                                for (int ifiter = 0; ifiter < ifCnt; ifiter++)
                                {
                                    cJSON * interface = cJSON_GetArrayItem(interfaces, ifiter);
                                    node->single[iter].interfaceTypes[ifiter] = DAMalloc(sizeof(char*) * strlen(interface->valuestring)+1);
                                    memcpy(node->single[iter].interfaceTypes[ifiter], interface->valuestring, strlen(interface->valuestring)+1);
                                }
                            }else{
                                return 0;
                            }
                            cJSON * policy = cJSON_GetObjectItem(res, KEY_DEVICE_RESOURCE_POLICY);
                            node->single[iter].policy = policy->valueint;
                             
                        }
                    }
                   
                    /***** register additional resource(provisininginfo / accesspointlist) *****/
                    int resCnt = sizeof(gstResources) / sizeof(st_resource);
                    for(int itr = 0; itr < resCnt; itr++)		
                    {		
                        memcpy(node->single[node->sigCnt].uri, gstResources[itr].uri , strlen(gstResources[itr].uri)+1);
                        int typeCnt = gstResources[itr].rtcnt;		
                        node->single[node->sigCnt].rtcnt = typeCnt;		
                        for (int typeiter = 0; typeiter < typeCnt; typeiter++)		
                        {		
                            node->single[node->sigCnt].resourceTypes[typeiter] = gstResources[itr].resourceTypes[typeiter];                            
                        }		
    		
                        int ifCnt = gstResources[itr].ifcnt;    		
                        node->single[node->sigCnt].ifcnt = ifCnt;       		
                        for (int ifiter = 0; ifiter < ifCnt; ifiter++)		
                        {		
                            node->single[node->sigCnt].interfaceTypes[ifiter] = gstResources[itr].interfaceTypes[ifiter];	
                        }		
                        node->single[node->sigCnt].policy = gstResources[itr].policy;		
                        node->sigCnt++;		
                    }	

                    THINGS_LOG_D(THINGS_INFO, TAG, "Resources for Single Usage Cnt : %d", node->sigCnt);
                }
                else
                {
                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Single Reosurces Not Exist");
                }                
            }
            else
            {
                THINGS_LOG_D(THINGS_INFO, TAG, "Reosurces Not Exist");
                ret = 0;
            }
            hashmapInsert(gDeviceHMap, node, (unsigned long)deviceNum);
        }

        THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] ============================================");
        THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] Total Device Num : %d", (int)hashmapCount(gDeviceHMap));

        // for resourceType
        struct st_resourceType * restype = NULL;
        int resTypeCnt = sizeof(gstResourceTypes)/sizeof(st_resourceType);
        cJSON * resourceTypes = cJSON_GetObjectItem(root, KEY_RESOURCES_TYPE);
        resourceTypeCnt = cJSON_GetArraySize( resourceTypes ) + resTypeCnt;
        gResourceTypeHMap = hashmapCreate( resourceTypeCnt );
        		
        THINGS_LOG_D(THINGS_INFO, TAG, "Resource Types Cnt : %d", resourceTypeCnt);
        for (int i = 0; i < resourceTypeCnt; i++ )
        {
            int index = 0;

            restype = CreateResourceType();

            cJSON * cj_rt      = cJSON_GetArrayItem(resourceTypes, i);
            cJSON * rtype     = cJSON_GetObjectItem(cj_rt, KEY_DEVICE_RESOURCETYPE_TYPE);
            cJSON * properties = cJSON_GetObjectItem(cj_rt, KEY_DEVICE_RESOURCETYPE_PROPERTIES);

            if(NULL != rtype)
            {
                index = hashmapGetHashVal((unsigned char*)rtype->valuestring);
                memcpy(restype->rt, rtype->valuestring, strlen(rtype->valuestring)+1);

                if(NULL != properties)
                {
                    restype->prop_cnt = cJSON_GetArraySize(properties);

                    for (int iter2 = 0; iter2 < (restype->prop_cnt); iter2++)
                    {
                        cJSON * attr = cJSON_GetArrayItem(properties, iter2);
                        cJSON * key = cJSON_GetObjectItem(attr, KEY_DEVICE_RESOURCETYPE_PROPERTIES_KEY);
                        cJSON * type = cJSON_GetObjectItem(attr, KEY_DEVICE_RESOURCETYPE_PROPERTIES_TYPE);
                        cJSON * mandatory = cJSON_GetObjectItem(attr, KEY_DEVICE_RESOURCETYPE_PROPERTIES_MANDATORY);
                        cJSON * rw = cJSON_GetObjectItem(attr, KEY_DEVICE_RESOURCETYPE_PROPERTIES_RW);
                        restype->prop[iter2] = CreateProperty();
                        if(key->valuestring != NULL) memcpy(restype->prop[iter2]->key, key->valuestring, strlen(key->valuestring)+1);
                        if(type->valueint != NULL) restype->prop[iter2]->type = type->valueint;

                        if (mandatory->type == cJSON_True)
                        {
						    restype->prop[iter2]->mandatory = true;
                        }
                        else
                        {
                            restype->prop[iter2]->mandatory = false;
                        }

                        restype->prop[iter2]->rw = rw->valueint;
                    }
                }
                else
                {
                    THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Not Attribute Exist~!!!! ");
                }
                hashmapInsert(gResourceTypeHMap, restype, index);
            }
        }

        /***** register additional resourcetype(provisininginfo / accesspointlist) *****/
        for(int rtItr = 0; rtItr < resTypeCnt; rtItr++)		
        {		
            int idx = hashmapGetHashVal((unsigned char*)gstResourceTypes[rtItr].rt);		
            hashmapInsert(gResourceTypeHMap, &gstResourceTypes[rtItr], idx);		
            resourceTypeCnt++;
        }

        //for configuration   
        cJSON *configuration = cJSON_GetObjectItem(root, KEY_CONFIGURATION);
        if(NULL != configuration)
        {
            int connectivity_type = 0;
            int authentication_type = 0;
            
            cJSON *easySetup = cJSON_GetObjectItem(configuration, KEY_CONFIGURATION_EASYSETUP);
            if(NULL != easySetup)
            {
                cJSON *connectivity = cJSON_GetObjectItem(easySetup, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY);
                if(NULL != connectivity){                    
                    cJSON *type = cJSON_GetObjectItem(connectivity, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_TYPE);
                    connectivity_type = type->valueint;
                        THINGS_LOG_D(THINGS_INFO, TAG, "[configuration] type       : %d", connectivity_type);
                    if(connectivity_type == 1){
                        es_conn_type = es_conn_type_SoftAP;
                        cJSON *softAP = cJSON_GetObjectItem(connectivity, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP);
                            if(NULL != softAP){  
                                cJSON *manufatureId = cJSON_GetObjectItem(softAP, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_MANUFACTUREID);
                                cJSON *setupId = cJSON_GetObjectItem(softAP, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_SETUPID);
                                cJSON *artik = cJSON_GetObjectItem(softAP, KEY_CONFIGURATION_EASYSETUP_CONNECTIVITY_SOFTAP_ARTIK);
                                
                                if(NULL != manufatureId && NULL != setupId){
                                    bool isArtik = false;
                                    if (artik->type == cJSON_True)
                                    {
                                        isArtik = true;
                                    }
                                    else
                                    {
                                        isArtik = false;
                                    }

                                    THINGS_LOG_D(THINGS_INFO, TAG, "[configuration] manufatureId : %s / setupId : %s / artik : %d", 
                                    manufatureId->valuestring, setupId->valuestring, isArtik);

                                    gManufacturerName = DAMalloc(sizeof(char*) * strlen(manufatureId->valuestring)+1);
                                    strncpy(gManufacturerName, manufatureId->valuestring, strlen(manufatureId->valuestring)+1);
                                    
                                    gSetupId = DAMalloc(sizeof(char*) * strlen(setupId->valuestring)+1);
                                    strncpy(gSetupId, setupId->valuestring, strlen(setupId->valuestring)+1);                     
                                }else{
                                    return 0;
                                }   
                            }
                    }else if(connectivity_type == 2){
                        //TO DO
                        es_conn_type = es_conn_type_BLE;
                    }
                    else{
                        return 0;
                    }
                }else{
                    return 0;
                }

                cJSON *ownershipTransferMethod = cJSON_GetObjectItem(easySetup, KEY_CONFIGURATION_EASYSETUP_OWNERSHIP);
                if(NULL != ownershipTransferMethod){
                    authentication_type = ownershipTransferMethod->valueint;
                    THINGS_LOG_D(THINGS_INFO, TAG, "[configuration] ownershipTransferMethod : %d", authentication_type);
                }else{
                    return 0;
                }

            }
            cJSON *wifi = cJSON_GetObjectItem(configuration, KEY_CONFIGURATION_WIFI);
            if(NULL != wifi)
            {
                cJSON *wifiInterfaces = cJSON_GetObjectItem(wifi, KEY_CONFIGURATION_WIFI_INTERFACES);
                cJSON *wifiFrequency = cJSON_GetObjectItem(wifi, KEY_CONFIGURATION_WIFI_FREQUENCY);
                if(NULL != wifiInterfaces && NULL != wifiInterfaces)
                {
                    THINGS_LOG_D(THINGS_INFO, TAG, "[configuration] wifiInterfaces : %d / wifiFrequency : %d", wifiInterfaces->valueint, wifiFrequency->valueint);
                }else{
                    return 0;
                }
            }else{
                return 0;
            }
                
            ///kkk
            cJSON *filePath = cJSON_GetObjectItem(configuration, KEY_CONFIGURATION_FILEPATH);
            if(NULL != filePath)
            {
                cJSON *svrdb = cJSON_GetObjectItem(filePath, KEY_CONFIGURATION_FILEPATH_SVRDB);
                cJSON *provisioning = cJSON_GetObjectItem(filePath, KEY_CONFIGURATION_FILEPATH_PROVISIONING);
                cJSON *certificate = cJSON_GetObjectItem(filePath, KEY_CONFIGURATION_FILEPATH_CERTIFICATE);
                cJSON *privateKey = cJSON_GetObjectItem(filePath, KEY_CONFIGURATION_FILEPATH_PRIVATEKEY);

                if(NULL != svrdb && NULL != provisioning && NULL != certificate && NULL != privateKey)
                {
                    memset(gSVRDBFilePath, 0, (size_t)MAX_FILE_PATH_LENGTH);
                    memset(gCertificateFilePath, 0, (size_t)MAX_FILE_PATH_LENGTH);
                    memset(gPrivateKeyFilePath, 0, (size_t)MAX_FILE_PATH_LENGTH);
                    memcpy(gSVRDBFilePath, svrdb->valuestring, strlen(svrdb->valuestring)+1);
                    memcpy(gCertificateFilePath, certificate->valuestring, strlen(certificate->valuestring)+1);
                    memcpy(gPrivateKeyFilePath, privateKey->valuestring, strlen(privateKey->valuestring)+1);
                    memcpy(gOCFCloudFilePath, provisioning->valuestring, strlen(provisioning->valuestring)+1);

                    THINGS_LOG_D(THINGS_INFO, TAG, "Security SVR DB file path : %s", gSVRDBFilePath);
                    THINGS_LOG_D(THINGS_INFO, TAG, "[configuration] svrdb : %s / provisioning : %s", svrdb->valuestring, provisioning->valuestring);
                    THINGS_LOG_D(THINGS_INFO, TAG, "[configuration] certificate : %s / privateKey : %s", certificate->valuestring, privateKey->valuestring);

                    ret = 1;
                }else{
                    return 0;
                }
            }else{
                return 0;
            }
        }
        if(NULL!= root) cJSON_Delete(root);
    
    }

    if(jsonStr != NULL)
    {
        DAFree(jsonStr);
    }

    ParseOCFCloudJson(gOCFCloudFilePath);

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return ret;
}

static int GetSignUPDataFromJson(const char* fileName, ESCloud_SignUp** clData)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    int ret = 1;
    char* jsonStr = GetJsonStringFromFile(fileName);

    if(jsonStr != NULL && strlen(jsonStr) > 0)
    {
        // 3. Parse the Json string
        cJSON *root = cJSON_Parse((const char *)jsonStr);
        cJSON *cloud = cJSON_GetObjectItem(root, KEY_CLOUD);
        if( cloud == NULL )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "cloud cJSON is NULL.");
            ret = 0;
        }
        else if( (ret = LoadCloudSignUPData( cloud , clData)) != 1 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Load Cloud SignUp Data Failed.");
        }

        cJSON_Delete(root);
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ocf_cloud.json file Reading is failed.");
        ret = 0;
    }

    if(jsonStr != NULL)
    {
        DAFree(jsonStr);
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return ret;
}

static int UpdateOCFInfoJson(const char* fileName , STDevice** devList, int devCnt )
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    int ret = 0;
    cJSON *root = NULL;
    int devCntInfile = 0;
    char* jsonUpdate = NULL;
    char* jsonStr = GetJsonStringFromFile(fileName);

    if(jsonStr != NULL && strlen(jsonStr) > 0)
    {
        // 3. Parse the Json string
        root = cJSON_Parse((const char *)jsonStr);

        // Devices Items
        cJSON *devices = cJSON_GetObjectItem(root, KEY_DEVICE);
        if( devices == NULL )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "devices cJSON is NULL.");
            goto GOTO_OUT;
        }

        if( (devCntInfile = cJSON_GetArraySize(devices)) != devCnt )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Mismatching device Count with \"ocf_info.json\" file.");
            goto GOTO_OUT;
        }

        for (int i = 0; i < devCnt; i++)
        {
            if( devList[i] != NULL && strlen(devList[i]->device_id) > 0 )
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Read ocf_ingo.json for Device[%d] ============================================", i);
                cJSON * device = cJSON_GetArrayItem(devices, i);

                cJSON_DeleteItemFromObject(device, KEY_ATTR_DEVICE_DEVICEID);
                cJSON_AddStringToObject(device, KEY_ATTR_DEVICE_DEVICEID, devList[i]->device_id);
            }
        }

        jsonUpdate = cJSON_Print(root);

        if( SetJsonStringIntoFile(fileName, jsonUpdate) == 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Fail : Store data to ocf_info file.");
            goto GOTO_OUT;
        }

        ret = 1;
    } // End of if
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not exist \"%s\" file or jsonStr(0x%X) is invalid.", fileName, jsonStr);
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Update Success in \"%s\" file.", fileName);

GOTO_OUT:
    if( root )
    {
        cJSON_Delete(root);
        root = NULL;
    }

    if(jsonStr != NULL)
    {
        DAFree(jsonStr);
    }

     if(jsonUpdate != NULL)
     {
         DAFree(jsonUpdate);
     }

     THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return ret;
}

int SaveAccesPointInfo(dawit_ap_connect_config* connect_config)
{
    THINGS_LOG_D(THINGS_INFO, TAG, THINGS_FUNC_ENTRY);

    char* fileName = gOCFCloudFilePath;

    int ret = 0;
    cJSON *root = NULL;
    char* jsonUpdate = NULL;
    char* jsonStr = GetJsonStringFromFile(fileName);

    if(jsonStr != NULL && strlen(jsonStr) > 0)
    {
        root = cJSON_Parse((const char *)jsonStr);

        cJSON *wifi = cJSON_GetObjectItem(root, KEY_WIFI);
        if( wifi == NULL )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "cloud cJSON is NULL.");
            goto GOTO_OUT;
        }

        cJSON_DeleteItemFromObject(wifi, KEY_WIFI_SSID);
        cJSON_DeleteItemFromObject(wifi, KEY_WIFI_PASSPHARASE);
        cJSON_DeleteItemFromObject(wifi, KEY_WIFI_AUTH_TYPE);
        cJSON_DeleteItemFromObject(wifi, KEY_WIFI_CRYPTO_TYPE);

        if( connect_config->ssid != NULL && strlen(connect_config->ssid) > 0 )
            cJSON_AddStringToObject(wifi, KEY_WIFI_SSID, connect_config->ssid);
        if( connect_config->passphrase != NULL && strlen(connect_config->passphrase) > 0 )
            cJSON_AddStringToObject(wifi, KEY_WIFI_PASSPHARASE, connect_config->passphrase);
        cJSON_AddNumberToObject(wifi, KEY_WIFI_AUTH_TYPE, connect_config->ap_auth_type);
        cJSON_AddNumberToObject(wifi, KEY_WIFI_CRYPTO_TYPE, connect_config->ap_crypto_type);
        
        jsonUpdate = cJSON_Print(root);

        if( SetJsonStringIntoFile(fileName, jsonUpdate) == 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Fail : Store data to ocf_info file.");
            goto GOTO_OUT;
        }

        ret = 1;
    } // End of if

    THINGS_LOG_D(THINGS_INFO, TAG, "Update Success in \"%s\" file.", fileName);

GOTO_OUT:
    if( root )
    {
        cJSON_Delete(root);
        root = NULL;
    }

    if(jsonStr != NULL)
    {
        DAFree(jsonStr);
    }

     if(jsonUpdate != NULL)
     {
         DAFree(jsonUpdate);
     }

     THINGS_LOG_D(THINGS_INFO, TAG, THINGS_FUNC_EXIT);
    return ret;
}

static int UpdateOCFCloudJsonByCloud_SignUP(const char* fileName, ESCloud_SignUp* clData)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    int ret = 0;
    cJSON *root = NULL;
    char* jsonUpdate = NULL;
    char* jsonStr = GetJsonStringFromFile(fileName);

    if(jsonStr != NULL && strlen(jsonStr) > 0)
    {
        // 3. Parse the Json string
        root = cJSON_Parse((const char *)jsonStr);

        // Cloud Items
        cJSON *cloud = cJSON_GetObjectItem(root, KEY_CLOUD);
        if( cloud == NULL )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "cloud cJSON is NULL.");
            goto GOTO_OUT;
        }


        if( clData != NULL )
        {
            cJSON_DeleteItemFromObject(cloud, KEY_CLOUD_ADDRESS);
        }
        cJSON_DeleteItemFromObject(cloud, KEY_CLOUD_DOMAIN);
        cJSON_DeleteItemFromObject(cloud, KEY_CLOUD_PORT);
        cJSON_DeleteItemFromObject(cloud, KEY_TOKEN_ACCESS);
        cJSON_DeleteItemFromObject(cloud, KEY_TOKEN_ACCESS_REFRESH);
        cJSON_DeleteItemFromObject(cloud, KEY_TOKEN_TYPE);
        cJSON_DeleteItemFromObject(cloud, KEY_EXPIRE_TIME);
        cJSON_DeleteItemFromObject(cloud, KEY_ID_USER);
        cJSON_DeleteItemFromObject(cloud, KEY_SERVER_REDIRECT_URI);
        cJSON_DeleteItemFromObject(cloud, KEY_CERTIFICATE_FILE);
        cJSON_DeleteItemFromObject(cloud, KEY_SERVER_ID);

        memset( gCloudAddress, 0, MAX_CLOUD_ADDRESS);
        if( clData != NULL )
        {
            if( DAStrcat( gCloudAddress, MAX_CLOUD_ADDRESS, clData->address) == NULL )
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Fail : Copy to gCloudAddress.");
                goto GOTO_OUT;
            }
            if( clData->domain != NULL && strlen(clData->domain) > 0 )
               cJSON_AddStringToObject(cloud, KEY_CLOUD_DOMAIN, clData->domain);
            if( clData->address != NULL && strlen(clData->address) > 0 )
               cJSON_AddStringToObject(cloud, KEY_CLOUD_ADDRESS, clData->address);
            if( clData->port != NULL && strlen(clData->port) > 0 )
               cJSON_AddStringToObject(cloud, KEY_CLOUD_PORT, clData->port);
            if( clData->accessToken != NULL && strlen(clData->accessToken) > 0 )
                cJSON_AddStringToObject(cloud, KEY_TOKEN_ACCESS, clData->accessToken);
            if( clData->refreshToken != NULL && strlen(clData->refreshToken) > 0 )
                cJSON_AddStringToObject(cloud, KEY_TOKEN_ACCESS_REFRESH, clData->refreshToken );
            if( clData->tokenType != NULL && strlen(clData->tokenType) > 0 )
                cJSON_AddStringToObject(cloud, KEY_TOKEN_TYPE, clData->tokenType );
            if( clData->expireTime != CLOUD_EXPIRESIN_INVALID )
                cJSON_AddNumberToObject(cloud, KEY_EXPIRE_TIME, clData->expireTime );
            if( clData->uid != NULL && strlen(clData->uid) > 0 )
                cJSON_AddStringToObject(cloud, KEY_ID_USER, clData->uid );
            if( clData->redirectURI != NULL && strlen(clData->redirectURI) > 0 )
                cJSON_AddStringToObject(cloud, KEY_SERVER_REDIRECT_URI, clData->redirectURI );
            if( clData->certificate != NULL && strlen(clData->certificate) > 0 )
                cJSON_AddStringToObject(cloud, KEY_CERTIFICATE_FILE, clData->certificate );
            if( clData->sid != NULL && strlen(clData->sid) > 0 )
                cJSON_AddStringToObject(cloud, KEY_SERVER_ID, clData->sid );
        }

        jsonUpdate = cJSON_Print(root);
//        printf("json Value = %s\n", jsonUpdate);

        if( SetJsonStringIntoFile(fileName, jsonUpdate) == 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Fail : Store data to ocf_info file.");
            goto GOTO_OUT;
        }

        ret = 1;
    } // End of if

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Update Success in \"%s\" file.", fileName);

GOTO_OUT:
    if( root )
    {
        cJSON_Delete(root);
        root = NULL;
    }

    if(jsonStr != NULL)
    {
        DAFree(jsonStr);
    }

     if(jsonUpdate != NULL)
     {
         DAFree(jsonUpdate);
     }

     THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return ret;
}

static int ParseOCFFiles(const char* info_Path)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    int res = 1;

    THINGS_LOG_D(THINGS_DEBUG,TAG,"orginal ocf_info.json file path = %s", info_Path);
    if(!ParseOCFInfoJson(info_Path))
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "OCF Info file parsing Failed");
        res = 0;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return res;
}

int DM_LoadLegacyCloudData(ESCloud_SignUp** clData)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    int ret = 1;
// #ifdef OCF_RTOS
//     ret  = getSignUPDataforCloud(clData);
// #else
    if ( gOCFCloudFilePath[0] == 0 || clData == NULL)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Provisioning file path is NULL.(%s) or Invalid argument.(0x%X)", gOCFCloudFilePath, clData);
        return 0;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "gOCFCloudFilePath = %s", gOCFCloudFilePath);


    if( (ret = GetSignUPDataFromJson(gOCFCloudFilePath, clData)) != 1 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not Get sign-up data failed from Provisioning file.");
    }

    things_task_sleep(500);

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
// #endif
    return ret;
}

const char* DM_GetOCFDeviceType(int deviceId)
{
    //THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    STDevice * device = NULL;

    device = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long) deviceId);
    if(device == NULL){

        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device Not Exist");
        return NULL;
    }

    return device->type;
}

const char* DM_GetOCFDeviceModel(int deviceId)
{
    STDevice * device = NULL;

    device = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long) deviceId);
    if(device == NULL)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device Not Exist");
        return NULL;
    }

    //device = (STDevice*)item->data;
    return device->modelNum;
}

const char* DM_GetSVRDBFilePath(void)
{
    return gSVRDBFilePath;
}

const char* DM_GetCertificateFilePath(void)
{
    return gCertificateFilePath;
}

const char* DM_GetPrivateKeyFilePath(void)
{
    return gPrivateKeyFilePath;
}

const char* DM_GetOCFCloudAddress(char* customizedCIserver)
{
    if( customizedCIserver && customizedCIserver[0] != 0 )
    {
        return customizedCIserver;
    }
    return GetCloudServerAddress(customizedCIserver);
}


bool DM_IsRscPublished(void)
{
    return (ESGetState() == ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD? true : false);
}

static OicResource * RegisterResource(OICServerBuilder * pBuilder, struct st_resource * resource, const char* id)
{
    OicResource * ret = NULL;
    char resUri[MAX_URI_LENGTH_OCF] = {0};

    if(NULL != resource)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "RESOURCE TO REGISTER : %s", resource->uri);

        memset(resUri, 0, (size_t)MAX_URI_LENGTH_OCF);
				/*SVACE warning fix*/
        strncat(resUri, resource->uri, MAX_URI_LENGTH_OCF);

        ret = pBuilder->CreateResource(pBuilder,
                                        resUri,
                                        resource->resourceTypes[0],
                                        resource->interfaceTypes[0],
                                        CHECK_DISCOVERABLE(resource->policy),
                                        CHECK_OBSERVABLE(resource->policy),
                                        CHECK_SECURE(resource->policy));

        THINGS_LOG_D(THINGS_DEBUG, TAG, "AddResourceType : %s", resource->resourceTypes[0]);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "AddInterfaceType : %s", resource->interfaceTypes[0]);
        for(int rtNum = 1 ; rtNum < resource->rtcnt ; rtNum++)
        {
            pBuilder->AddResourceType(ret, resource->resourceTypes[rtNum]);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "AddResourceType : %s", resource->resourceTypes[rtNum]);
        }

        for(int itNum = 1 ; itNum < resource->ifcnt ; itNum++)
        {
            pBuilder->AddInterfaceType(ret, resource->interfaceTypes[itNum]);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "AddInterfaceType : %s", resource->interfaceTypes[itNum]);
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Input Param ");
    }

    return ret;
}

static OicResource * RegisterColResource(OICServerBuilder * pBuilder, struct col_resource * col_resource, const char* id)
{
    OicResource * ret = NULL;
    char resUri[MAX_URI_LENGTH_OCF] = {0};

    if(NULL != col_resource)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "RESOURCE TO REGISTER : %s", col_resource->uri);

        memset(resUri, 0, (size_t)MAX_URI_LENGTH_OCF);
				/*SVACE warning fix*/
        strncat(resUri, col_resource->uri, MAX_URI_LENGTH_OCF);
        ret = pBuilder->CreateResource(pBuilder,
                                        resUri,
                                        col_resource->resourceTypes[0],
                                        col_resource->interfaceTypes[0],
                                        CHECK_DISCOVERABLE(col_resource->policy),
                                        CHECK_OBSERVABLE(col_resource->policy),
                                        CHECK_SECURE(col_resource->policy));

        for(int rtNum = 1 ; rtNum < col_resource->rtcnt ; rtNum++)
        {
            pBuilder->AddResourceType(ret, col_resource->resourceTypes[rtNum]);
        }

        for(int itNum = 1 ; itNum < col_resource->ifcnt ; itNum++)
        {
            pBuilder->AddInterfaceType(ret, col_resource->interfaceTypes[itNum]);
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Input Param ");
    }

    return ret;
}

static OicResource * RegisterDeviceResource(OICServerBuilder * pBuilder, STDevice * device, const char* id)
{
    OicResource * ret = NULL;
    char resUri[MAX_URI_LENGTH_OCF] = {0};

    if(NULL != device)
    {
        memset(resUri, 0, (size_t)MAX_URI_LENGTH_OCF);
        strcat(resUri, OC_RSRVD_DEVICE_URI);

        ret = pBuilder->CreateResource(pBuilder,
                                        resUri,
                                        OC_RSRVD_RESOURCE_TYPE_DEVICE,
                                        OC_RSRVD_INTERFACE_READ,
                                        0,0,1);

        pBuilder->AddResourceType(ret, device->type);

        ret->rep = CreateRepresentationInst(NULL);
        if(ret->rep)
        {
            THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/d] name :%s",device->name);
            THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/d] type :%s",device->type);

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_DEVICE_NAME,
                           device->name );

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_DEVICE_ID,
                           device->device_id );

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_SPEC_VERSION,
                           OC_SPEC_VERSION );

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_DATA_MODEL_VERSION,
                           OC_DATA_MODEL_VERSION );
        }
        else
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not able to create representation");
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Input Param ");
    }

    return ret;
}


static OicResource * RegisterPlatformResource(OICServerBuilder * pBuilder, STDevice * device, const char* id)
{
    OicResource * ret = NULL;
    char resUri[MAX_URI_LENGTH_OCF] = {0};

    if(NULL != device)
    {
        memset(resUri, 0, (size_t)MAX_URI_LENGTH_OCF);
        strcat(resUri, OC_RSRVD_PLATFORM_URI);

        ret = pBuilder->CreateResource(pBuilder,
                                        resUri,
                                        OC_RSRVD_RESOURCE_TYPE_PLATFORM,
                                        OC_RSRVD_INTERFACE_READ,
                                        0,0,1);

        ret->rep = CreateRepresentationInst(NULL);
        if(ret->rep)
        {
            THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Manufacturer :%s",MANUFACTURER_NAME);
            THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Model Name :%s",device->modelNum);
            THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Ver. Plaform :%s",device->ver_p);
            THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Ver. OS :%s",device->ver_os);
            THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Ver. HW :%s",device->ver_hw);
            THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Ver. FW :%s",device->ver_fw);
            THINGS_LOG_D(THINGS_DEBUG,TAG, "[/oic/p] Ver. vid :%s",device->vender_id);

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_PLATFORM_ID,
                           device->device_id );

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_MFG_NAME,
                           MANUFACTURER_NAME );

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_MODEL_NUM,
                           device->modelNum );

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_PLATFORM_VERSION,
                           device->ver_p );

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_OS_VERSION,
                           device->ver_os );

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_HARDWARE_VERSION,
                           device->ver_hw );

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_FIRMWARE_VERSION,
                           device->ver_fw );

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_MFG_URL,
                           MANUFACTURER_URL );

            ret->rep->SetValue(ret->rep,
                           OC_RSRVD_VID,
                           device->vender_id );
        }
        else
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not able to create representation");
        }


    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Input Param ");
    }

    return ret;
}

long DM_GetNumOfDevCnt(void)
{
    return hashmapCount(gDeviceHMap);
}

STDevice* DM_GetInfoOfDev(unsigned long number)
{
    return (STDevice *)hashmapGet(gDeviceHMap, number);
}

bool DM_RegisterUserDefineDeviceID( const int seq_ocfInfo, const char* devID )
{
    if( isSupportUserDefDevList == false )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No More Support User define Device ID.");
        return false;
    }

    if( seq_ocfInfo < 1 || seq_ocfInfo > MAX_SUBDEVICE_EA || devID == NULL || strlen(devID) == 0 )
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "It Support Max %d EA for device ID", MAX_SUBDEVICE_EA);
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(seq_ocfInfo=%d, devID=0x%X)", seq_ocfInfo, devID);
        return false;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "seq_ocfInfo=%d, devID=%s", seq_ocfInfo, devID);

    if( strlen(devID) >= MAX_DEVICE_ID_LENGTH )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Overflow device ID length.(It Support Max length: %d)", MAX_DEVICE_ID_LENGTH);
        return false;
    }
    UserDefDevList[seq_ocfInfo] = strdup(devID);

    return true;
}

static void DM_DeleteUserDefineDeviceID( void )
{
    int i=0;

    for(i=0; i < MAX_SUBDEVICE_EA; i++)
    {
        DAFree(UserDefDevList[i]);
        UserDefDevList[i] = NULL;
    }
}

bool DM_RegisterDeviceID(void)
{
    int i = 0;
    char* id = NULL;
    bool writeFile4deviceID = false;
    STDevice ** devList = NULL;
    int deviceCnt = (int)hashmapCount(gDeviceHMap);
    if(deviceCnt < 1)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "No Device Information to Register");
        return false;
    }

    if( (devList = (STDevice**)DAMalloc(sizeof(STDevice*)*deviceCnt)) == NULL )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "STDevice mem allocation is failed.");
        return false;
    }

    // Set Main-Device ID
    id = OCGetServerInstanceIDString();
    devList[0] = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long)0);

    if( id == NULL || devList[0] == NULL || devList[0]->device_id == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Error : id = %s, Main-device =0x%X", id, devList[0]);
        goto GOTO_ERROR;
    }
    memcpy(devList[0]->device_id, id, strlen(id)+1);

    // Set Sub-Device ID
    for( i=1; i < deviceCnt; i++ )
    {
        devList[i] = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long)i);
        if( devList[i] == NULL )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Error : device[%d] =0x%X", i, devList[i]);
            goto GOTO_ERROR;
        }

        if(UserDefDevList[i] != NULL || strlen(devList[i]->device_id) == 0)
        {
            // a. If logical device is in a hosting(physical) device.
            if(devList[i]->isPhysical == 1)
            {
                OCRandomUuidResult idGenRet = RAND_UUID_CONVERT_ERROR;

                if( UserDefDevList[i] )
                {
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "Set Sub-device ID with user-defined device ID.");
                    memcpy(devList[i]->device_id, UserDefDevList[i], strlen(UserDefDevList[i])+1);
                    idGenRet = RAND_UUID_OK;
                }
                else
                {
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "Set Sub-device ID with Auto-Generated device ID.");
                    idGenRet = OCGenerateUuidString(devList[i]->device_id);
                }

                if(idGenRet != RAND_UUID_OK)
                {
                    THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "[DEVICE] Device ID Creation Failed : %d", idGenRet);
                }
                else
                {
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "[DEVICE] ID Created : %s", (devList[i]->device_id) );
                    writeFile4deviceID = true;
                }
            }
            // b. If logical device is in the other physical device
            else
            {
                THINGS_LOG_D(THINGS_INFO, TAG, "[DEVICE] Not Physically Separated Device ");
            }
        }
    }

    // Write "ocf_info.json" file for Sub-device ID
    if( writeFile4deviceID == true )
    {
        DM_UpdateDeviceInfoList(devList, deviceCnt);
    }
    isSupportUserDefDevList = false;    //  It's allow permission to apply user defined device-ID only when start THINGS_Stack.

    DM_DeleteUserDefineDeviceID();
    DAFree(devList);
    devList = NULL;
    return true;

GOTO_ERROR:
    DAFree(devList);
    devList = NULL;
    return false;
}

int DM_RegisterResource(OICServerBuilder * pBuilder)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    if(pBuilder == NULL)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Builder instance is NULL");

        return 0;
    }

    int deviceCnt = (int)hashmapCount(gDeviceHMap);
    if(deviceCnt < 1)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "No Device Information to Register");

        return 0;
    }

    for(int deviceNum = 0 ; deviceNum < deviceCnt ; deviceNum++)
    {
        char id[11] = {0};
        char resUri[MAX_URI_LENGTH_OCF] = {0};
        int key = 0;
        int nCountofChildren = 0;

        STDevice * device = NULL;
        struct st_resource * resource = NULL;
        struct col_resource * col_resource = NULL;
        struct OicResource * pCollectionResource = NULL;

        // 2. Device Capability Resources Registration
        THINGS_LOG_D(THINGS_DEBUG, TAG, "BEFORE SEARCHING THE DEVICE  ITEM FROM HASH TABLE");

        device = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long)deviceNum);

        if(NULL != device)
        {
						/*SVACE warning fix*/
            sprintf(id,"%d",device->no);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "==================== Device (%s) ===================="
                    , id);

            if(device->colCnt < 1)
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "NO COLLECTION & ITS CHILDREN RESOURCE(S)");
            }
            else
            {
                THINGS_LOG_D(THINGS_INFO, TAG, "COLLECTION CHILDREN RESOURCE(S) CNT : %d", device->colCnt);
                
                memset(resUri, 0, (size_t)MAX_URI_LENGTH_OCF);
                strcat(resUri, device->collection[0].uri);

                pCollectionResource = pBuilder->CreateCollectionResource(pBuilder,
                    resUri, device->collection[0].resourceTypes[0], device->collection[0].interfaceTypes[0]);

                for(int rtNum = 1 ; rtNum < device->collection[0].rtcnt ; rtNum++)
                {
                    OCBindResourceTypeToResource( pBuilder, device->collection[0].resourceTypes[rtNum]);
                }
            
                // THINGS_LOG_D(THINGS_DEBUG, TAG, "IT TO BIND : %d", resource->ifcnt);
                for(int itNum = 1 ; itNum < device->collection[0].ifcnt ; itNum++)
                {
                    OCBindResourceInterfaceToResource( pBuilder, device->collection[0].interfaceTypes[itNum]);
                }

                THINGS_LOG_D(THINGS_DEBUG, TAG, "AFTER REGISTERGING DEVICE RESOURCE");

                if(pCollectionResource != NULL)
                {
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "DEVICE RESOURCE(S) CNT : %d", device->colCnt);
                    for(int capaNum = 0 ; capaNum < device->collection[0].linkcnt  ; capaNum++)
                    {
                            resource = device->collection[0].links[capaNum];
                            device->pChildResources[nCountofChildren++] = RegisterResource(pBuilder, resource, id);
                    } // End of for children resource(s) registration loop
                } // End of single device

                pBuilder->BindAll(pCollectionResource, device->pChildResources, nCountofChildren);

                device->pChildResources[nCountofChildren++] = pCollectionResource;
            }

            THINGS_LOG_D(THINGS_DEBUG, TAG, "SINGLE RESOURCE(S) CNT : %d", device->sigCnt);

            for(int capaNum = 0 ; capaNum < device->sigCnt ; capaNum++)
            {
                resource = &device->single[capaNum];
                device->pChildResources[nCountofChildren++] = RegisterResource(pBuilder, resource, id);
            } // End of for single resource registration loop

            // Create the Device(/oic/d) and Platform (/oic/p)
            // ex) Aircon Multi-Model
            if(deviceNum < 1)
            {
                // for hosting device
                pBuilder->SetDeviceInfo(pBuilder, device->name, device->type);
                pBuilder->SetPlatformInfo(pBuilder,
                                            device->modelNum,   // gDeviceModel,
                                            device->ver_p,      // gPlatformVersion,
                                            device->ver_os,     // gOSVersion,
                                            device->ver_hw,     // gHWVersions,
                                            device->ver_fw,     // gFWVersions,
                                            device->vender_id); // gVenderId);
            }
            else
            {
                if(device->isPhysical == 1)
                {
                    THINGS_LOG_V(THINGS_INFO, TAG,
                        "It's Physically Separated Device : %s",device->device_id);
                    // for those physically separated devices
                    // a. /oic/d/# resource
                    device->pChildResources[nCountofChildren++] = RegisterDeviceResource(pBuilder, device, id);
                    // b. /oic/p/# resource
                    device->pChildResources[nCountofChildren++] = RegisterPlatformResource(pBuilder, device, id);
                }
                else
                {
                    THINGS_LOG_V(THINGS_INFO, TAG,
                        "It's Logical Device in One Physical Device : [%d]", deviceNum);
                }
            }

            // Total Size of the device->pChildresources : colCnt + sigCnt + 1(/device) + a(/oic/d & /oic/p)
            device->capaCnt = nCountofChildren;
        } // End of device NULL Check
    } // End of for device loop
    THINGS_LOG_D(THINGS_DEBUG, TAG, "=====================================================");

    if ( pBuilder->BroadcastPresence(pBuilder, 20) == 1 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Broadcast Presence Failed.");
        return 0;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return 1;
}

struct OicResource * DM_GetResourceInstance  (const char* uri,
                                              const int id)
{
    OicResource * ret = NULL;

    int deviceCnt = (int)hashmapCount(gDeviceHMap);
    if(deviceCnt > 0 && deviceCnt > id)
    {
        STDevice* device = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long)(id));

        if(device)
        {
            for(int index = 0 ; index < MAX_DEVICE_CAPABILTY_CNT ; index++)
            {
                if(device->pChildResources[index] == NULL)
                {
                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Resource with URI : %s not exist !!!!", uri);
                    break;
                }
                else if(strcmp(device->pChildResources[index]->uri, uri) == 0  )
                {
                    THINGS_LOG_V(THINGS_DEBUG, TAG, "Found %s from device[%d] : %s!!!!", uri,
                                                                    id, device->device_id);


                    char *sValue = NULL;

                    device->pChildResources[index]->rep->GetValue(device->pChildResources[index]->rep,
                                                                  OC_RSRVD_DEVICE_ID,
                                                                  &sValue);
                    ret = device->pChildResources[index];
                    break;
                }
                else
                {
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "[%d] Resource URI : %s",
                                                    index, device->pChildResources[index]->uri);
                }
            }
        }
    }

    return ret;

}

int DM_GetDeviceInformation(int *cnt, STDevice ***list)
{
    int ret = 0;

    int deviceCnt = (int)hashmapCount(gDeviceHMap);
    if(deviceCnt > 0)
    {
        STDevice** devices = NULL;
        if( (devices = (STDevice**)DAMalloc( sizeof(STDevice*)*deviceCnt )) == NULL )
        {
            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "STDevice memory allocation is failed.");
            return ret;
        }

        for(int index = 0 ; index < deviceCnt ; index++)
        {
            devices[index] = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long)index);
        }

        (*list) = devices;
        (*cnt) = deviceCnt;
        ret = 1;
    }

    return ret;
}

const int DM_GetNumofChildren(int deviceNum)
{
    int ret = 0;

    STDevice * device = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long)deviceNum);
    if(NULL != device)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "DEVICE CHILDREN RESOURCE(S) : %d", device->colCnt);
        ret = device->colCnt;
    }

    return ret;
}

const char* DM_GetResourceUri(int deviceNum, int index)
{
    STDevice * device = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long)deviceNum);
    if(NULL != device)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "DEVICE CHILDREN RESOURCE(S) : %d", device->colCnt);
        if( (index) >= device->colCnt)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Index : %d", index);
        }
        else
        {
            
            return device->collection[0].links[index]->uri;
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Device Num to Search : %d", deviceNum);
    }

    return NULL;
}

int DM_UpdateDeviceInfoList(STDevice** devList, int devCnt)
{
    int ret = 1;
    UpdateAndSaveDeviceInfo(devList, devCnt);
    return ret;
}

int DM_UpdateOCFCloud(ESCloud_SignUp* clData)
{
    int ret = 1;

    THINGS_LOG_V(THINGS_INFO,TAG, "RTOS Recived Signup Data domain[%s], address[%s], port [%s],  accessToken[%s],  refreshToken[%s], tokenType[%s],  "
    "uid[%s]", clData->domain,clData->address,clData->port,clData->accessToken,clData->refreshToken,clData->tokenType,clData->uid);
    THINGS_LOG_V(THINGS_INFO,TAG, "RTOS Recived Signup Data redirectURI[%s],  certificate[%s], sid[%s]",clData->redirectURI,clData->certificate,clData->sid);

    if ( gOCFCloudFilePath[0] == 0 || clData == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ocf_cloud file path is NULL(%s). or Invalid argument.(0x%X)", gOCFCloudFilePath, clData );
        ret = 0;
        return ret;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "gOCFCloudFilePath = %s", gOCFCloudFilePath);

    if( UpdateOCFCloudJsonByCloud_SignUP( gOCFCloudFilePath , clData) == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Provisioning file update failed.");
        ret = 0;
    }

    if(ret == 1)
    {
        int res = 0;
        ESCloud_SignUp* confirmData = NULL;

        if( (res = DM_LoadLegacyCloudData(&confirmData)) == 1 )
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Update Success.");
            isThereCloudData = true;
            ESCloud_SignUp_clear(confirmData);
            confirmData = NULL;
        }
        else
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Update Failed.(%d)", res);     // 1 : Success , 0/-1 : system error. , -2 : saved invalid Cloud-Data.
            isThereCloudData = false;
        }
    }

    return ret;
}

int DM_DelOCFCloudData(void)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    int ret = 1;
    if ( gOCFCloudFilePath[0] == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ocf_cloud file path is NULL.");
        ret = 0;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "gOCFCloudFilePath = %s", gOCFCloudFilePath);

    if( UpdateOCFCloudJsonByCloud_SignUP( gOCFCloudFilePath , NULL) == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "It's failed to delete Cloud-Data in ocf_cloud.json.");
        ret = 0;
    }

    if(ret == 1)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Delete Success.");
        isThereCloudData = false;
    }

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return ret;
}

bool DM_isThereOCFCloud(void)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "isThereCloudData = %d", isThereCloudData);
    return isThereCloudData;
}

int DM_ValidateAttributeInRequest(char* rt, const void* payload)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    int ret = 0;

    if(rt != NULL && payload != NULL)
    {
        // 1. Get index with resource type
        unsigned long index = hashmapGetHashVal((unsigned char*)rt);
        OCRepPayload* rPayload = (OCRepPayload*)payload;

        // 2. Get resource type instance
        THINGS_LOG_D(THINGS_INFO, TAG, "Index : %u  rt : %s", index, rt);
        struct st_resourceType * resType = (struct st_resourceType*)hashmapGet(gResourceTypeHMap, index);

        if(NULL != resType)
        {
            int iter = 0;
            // 3. Get attributes & check
            //    POLICY
            //    a. Check exsitence of read-only attributes in the payload which should not be..
            for( iter = 0 ; iter < resType->prop_cnt; iter++)
            {
                if( NULL != resType->prop[iter]
                    && (  1 == CHECK_READABLE( resType->prop[iter]->rw )
                       && 1 != CHECK_WRITABLE( resType->prop[iter]->rw ) )
                    )
                {
                    THINGS_LOG_D(THINGS_INFO, TAG, "\tCHECKING READ-ONLY ATTR : %s", resType->prop[iter]->key);
                    if( !OCRepPayloadIsNull(rPayload, resType->prop[iter]->key))
                    {
                        THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> EXIST");
                        goto VALIDATION_EXIT;
                    }
                    else
                    {
                        THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> NOT EXIST");
                    }
                }
            }
            //    b. Check existence of rw attribute in the payload which should exist at least 1
            for( iter = 0 ; iter < resType->prop_cnt; iter++)
            {
                if(NULL != resType->prop[iter]
                && 1 == CHECK_WRITABLE( resType->prop[iter]->rw ) )
                {
                    THINGS_LOG_D(THINGS_INFO, TAG, "\tCHECKING WRITE-ABLE ATTR : %s", resType->prop[iter]->key);
                    if( !OCRepPayloadIsNull(rPayload, resType->prop[iter]->key))
                    {
                        THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> EXIST");

                        ret = 1;
                        break;
                    }
                    else
                    {
                        THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> NOT EXIST");
                    }
                }
            }
            //    c. Make it pass if it's collection resource
            if(0 == strcmp(rt, OC_RSRVD_RESOURCE_TYPE_COLLECTION)
                || 0 == strcmp(rt, SEC_RTYPE_THINGS_DEVICE))
            {
                // TODO : Need to check whether there an array delivered or not..
                THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> Child %s", (rPayload->next != NULL ? "EXIST":"NOT EXIST") );
                THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> Request On Collection Resource. Making it PASS");
                ret = 1;
            }
        }
        else
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Not Supporting rt : %s", rt);
        }
    }
    else
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid Input - rt : %s, payload %d", rt, payload);
    }

VALIDATION_EXIT:

    if(ret == 0) THINGS_LOG_D(THINGS_DEBUG, TAG, "\t==> INVALID REQUEST~!!!");
    return ret;
}

//jun
//for PHASE#2
char* OICGetDeviceType(void){
    return DM_GetOCFDeviceType(0);
}

char* OICGetDeviceName(void){
    int deviceCnt = (int)hashmapCount(gDeviceHMap);
    if(deviceCnt > 0)
    {            
        STDevice * device = NULL;

        device = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long) 0);
        if(device == NULL){
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device Not Exist");
            return NULL;
        }
        
        return device->name;
    }
    return NULL;
}

int OICGetResourceType(const char* resource_uri, int* count, char*** resource_types){
    THINGS_LOG_D(THINGS_DEBUG, TAG, "OICGetResourceType : %s", resource_uri);
    int deviceCnt = (int)hashmapCount(gDeviceHMap);
    if(deviceCnt > 0)
    {
        STDevice* device = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long)0);

        if(device != NULL)
        {
            if(device->sigCnt > 0){
                for(int index = 0 ; index < device->sigCnt ; index++)
                {
                   if(strcmp(device->single[index].uri, resource_uri) == 0  )
                    {
                        (*count) = device->single[index].rtcnt;
                        (*resource_types) = device->single[index].resourceTypes;
                        return 1;
                    }                   
                }
            }
            if(device->colCnt > 0){
                for(int index = 0 ; index < device->colCnt ; index++)
                {
                   if(strcmp(device->collection[index].uri, resource_uri) == 0  )
                    {
                        (*count) = device->collection[index].rtcnt;
                        (*resource_types) = device->collection[index].resourceTypes;
                        return 1;
                    }else{
                        for(int index_links = 0 ; index_links < device->collection[index].linkcnt ; index_links++)
                        {
                           if(strcmp(device->collection[index].links[index_links]->uri, resource_uri) == 0  )
                            {
                                (*count) = device->collection[index].links[index_links]->rtcnt;
                                (*resource_types) = device->collection[index].links[index_links]->resourceTypes;
                                return 1;
                            }
                        }
                    }                   
                }
            }
        }
    }
    return 0;
}

char* OICGetServerUUID(void){
    int deviceCnt = (int)hashmapCount(gDeviceHMap);
    if(deviceCnt > 0)
    {            
        STDevice * device = NULL;

        device = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long) 0);
        if(device == NULL){
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device Not Exist");
            return NULL;
        }
        
        return device->device_id;
    }
    return NULL;
}

int OICGetAttributesByResourceType(const char *res_type, int *count, st_attribute ***attributes){
    int resourceTypeCnt = (int)hashmapCount(gResourceTypeHMap);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "OICGetAttributesByResourceType : %s, resourceTypeCnt : %d", res_type, resourceTypeCnt);
    


    if(resourceTypeCnt > 0){
        int index = hashmapGetHashVal(res_type);
        struct st_resourceType * resType = (struct st_resourceType*)hashmapGet(gResourceTypeHMap, index);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "resType : %s, res_type : %s", resType->rt, res_type);
        if( strcmp(resType->rt, res_type) == 0 ){
            THINGS_LOG_D(THINGS_DEBUG, TAG, "resType->prop_cnt : %d", resType->prop_cnt);
            (*count) = resType->prop_cnt;
            (*attributes) = resType->prop;
            return 1;
        }
    }
    return 0;
}

bool OICIsCollectionResource(const char *res_uri){
    int deviceCnt = (int)hashmapCount(gDeviceHMap);
    if(deviceCnt > 0)
    {            
        STDevice * device = NULL;

        device = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long) 0);
        if(device == NULL){
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device Not Exist");
            return NULL;
        }

        if(device->colCnt > 0){
            for(int index = 0 ; index < device->colCnt ; index++)
            {
               if(strcmp(device->collection[index].uri, res_uri) == 0  )
                {
                    return true;
                }              
            }
        }
    }
    return false;
}

int OICGetChildResources(const char *col_res_uri, int *count, st_resource ***child_resources){
    STDevice * device = NULL;
    
    device = (STDevice *)hashmapGet(gDeviceHMap, (unsigned long) 0);
    if(device == NULL){
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device Not Exist");
        return NULL;
    }

    if(device->colCnt > 0){
        for(int index = 0 ; index < device->colCnt ; index++)
        {
            if(strcmp(device->collection[index].uri, col_res_uri) == 0  )
            {
                (*count) = device->collection[index].linkcnt;
                (*child_resources) = device->collection[index].links;
                return 1;
            }              
        }
    }
    return 0;
}


int DM_InitModule(const char *devJsonPath)
{
	if (devJsonPath == NULL) {
		THINGS_LOG_D(THINGS_ERROR, TAG, "devJsonPath is NULL");
		return 0;
	}

	memset(gSVRDBFilePath, 0, (size_t) MAX_FILE_PATH_LENGTH);
	memset(gOCFCloudFilePath, 0, (size_t) MAX_FILE_PATH_LENGTH);
	memset(gCloudAddress, 0, (size_t) MAX_CLOUD_ADDRESS);

	return ParseOCFFiles(devJsonPath);
}
//

int DM_TermiateModule()
{
    //[Jay] Need to backup all the keys in order to delete from the map.
    hashmapDelete(gDeviceHMap);
    hashmapDelete(gResourceTypeHMap);
    return 1;
}




