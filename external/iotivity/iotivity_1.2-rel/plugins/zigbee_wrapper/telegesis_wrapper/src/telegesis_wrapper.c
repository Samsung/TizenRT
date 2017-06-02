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


#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <inttypes.h>

#include <unistd.h>
#include <sys/stat.h>

#include <termios.h>
#include <string.h>
#include <errno.h>

#include "oic_string.h"
#include "oic_malloc.h"
#include "logger.h"

#include "twtypes.h"
#include "telegesis_socket.h"
#include "telegesis_wrapper.h"
#include "twsocketlist.h"


#define TAG PCF("telegesiswrapper")     // Module Name

#define ARRAY_LENGTH    100

#define RESPONSE_PARAMS_COUNT_NETWORK_INFO_1                (1)
#define RESPONSE_PARAMS_COUNT_NETWORK_INFO_5                (5)
#define RESPONSE_PARAMS_COUNT_JPAN                          (3)
#define RESPONSE_PARAMS_COUNT_DEVICE_JOINED                 (2)
#define RESPONSE_PARAMS_COUNT_MATCH_DESC                    (3)
#define RESPONSE_PARAMS_COUNT_SIMPLE_DESC                   (2)
#define RESPONSE_PARAMS_COUNT_SIMPLE_DESC_IN_CLUSTER_MIN    (1)
#define RESPONSE_PARAMS_COUNT_WRITE_ATTR_4                  (4)
#define RESPONSE_PARAMS_COUNT_WRITE_ATTR_5                  (5)
#define RESPONSE_PARAMS_COUNT_TEMPERATURE                   (5)
#define RESPONSE_PARAMS_COUNT_RESPATTR                      (6)
#define RESPONSE_PARAMS_COUNT_ADDRESS_RESPONSE              (3)
#define RESPONSE_PARAMS_COUNT_DFTREP                        (5)
#define RESPONSE_PARAMS_COUNT_DRLOCKUNLOCKRSP               (3)
#define RESPONSE_PARAMS_COUNT_ZENROLLREQ                    (4)
#define RESPONSE_PARAMS_COUNT_ENROLLED                      (3)
#define RESPONSE_PARAMS_COUNT_ZONESTATUS_4                  (4)
#define RESPONSE_PARAMS_COUNT_ZONESTATUS_6                  (6)

#define IN_CLUSTER_COUNT_STRING    "05"
#define OUT_CLUSTER_COUNT_STRING   "00"

#define TOKEN_ADDRRESP_STATUS_CODE                  (0)
#define TOKEN_ADDRRESP_NODEID                       (1)
#define TOKEN_ADDRRESP_EUI                          (2)

#define TOKEN_PLUS_N_DEVICE_TYPE                    (0)
#define TOKEN_PLUS_N_PANID                          (3)
#define TOKEN_PLUS_N_PANID_EXTENDED                 (4)

#define TOKEN_JPAN_PANID                            (1)
#define TOKEN_JPAN_PANID_EXTENDED                   (2)

#define TOKEN_PJOIN_RESPONSE_IEEE_ADDRESS           (0)
#define TOKEN_PJOIN_RESPONSE_NODEID                 (1)

#define TOKEN_MATCHDESC_NODEID                      (0)
#define TOKEN_MATCHDESC_STATUS_CODE                 (1)
#define TOKEN_MATCHDESC_ENDPOINTID                  (2)

#define TOKEN_SIMPLEDESC_SIMPLEDESC_NODEID          (0)
#define TOKEN_SIMPLEDESC_SIMPLEDESC_STATUS_CODE     (1)
#define TOKEN_SIMPLEDESC_INCLUSTER_STRING           (0)

#define TOKEN_WRITEATTR_STATUS_CODE                 (3)
#define TOKEN_WRITEATTR_STATUS_CODE_ALTERNATIVE     (4)

#define TOKEN_TEMPERATURE_STATUS_CODE               (3)
#define TOKEN_TEMPERATURE_VALUE                     (4)

#define TOKEN_RESPATTR_STATUS_CODE                  (4)
#define TOKEN_RESPATTR_ATTRIBUTE_VALUE              (5)

#define TOKEN_DFTREP_NODEID                         (0)
#define TOKEN_DFTREP_ENDPOINTID                     (1)
#define TOKEN_DFTREP_CLUSTERID                      (2)
#define TOKEN_DFTREP_COMMANDID                      (3)
#define TOKEN_DFTREP_STATUS_CODE                    (4)

#define TOKEN_DRLOCKRSP_NODEID                      (0)
#define TOKEN_DRLOCKRSP_ENDPOINTID                  (1)
#define TOKEN_DRLOCKRSP_STATUS_CODE                 (2)

#define TOKEN_ZENROLLREQ_NODEID                     (0)
#define TOKEN_ZENROLLREQ_ENDPOINTID                 (1)
#define TOKEN_ZENROLLREQ_ZONETYPE                   (2)
#define TOKEN_ZENROLLREQ_MANUFACTURE_CODE           (3)
#define TOKEN_ENROLLED_ZONEID                       (0)
#define TOKEN_ENROLLED_ZONETYPE                     (1)
#define TOKEN_ENROLLED_EUI                          (2)

#define TOKEN_ZONESTATUS_NODEID                     (0)
#define TOKEN_ZONESTATUS_ENDPOINTID                 (1)
#define TOKEN_ZONESTATUS_ZONESTATUS                 (2)
#define TOKEN_ZONESTATUS_ZONESTATUS_EXTENDED        (3)
#define TOKEN_ZONESTATUS_ZONEID                     (4)
#define TOKEN_ZONESTATUS_DELAY                      (5)


typedef struct TWContext{
    PIPlugin_Zigbee* g_plugin;
    const char* g_port;

    char g_LocalEUI[SIZE_EUI];
    char g_WIPRemoteEUI[SIZE_EUI];
    char g_WIPRemoteNodeId[SIZE_NODEID];

    TWStatus g_ZigBeeStatus;
    TWDeviceList* g_FoundMatchedDeviceList;
    TWDevice* g_WIPDevice;

    TWDeviceFoundCallback g_DeviceFoundCallback;
    TWEnrollmentSucceedCallback g_EnrollmentSucceedCallback;
    TWDeviceStatusUpdateCallback g_DeviceStatusUpdateCallback;
    TWDeviceNodeIdChangedCallback g_EndDeviceNodeIdChangedCallback;

    struct TWContext* next;
} TWContext;

typedef TWResultCode (*TWATResultHandler)(int count, char** tokens, TWContext* ctx);

typedef struct
{
    const char *resultTxt;
    TWATResultHandler handler;
} TWATResultHandlerPair;


//-----------------------------------------------------------------------------
// Private internal function prototypes
//-----------------------------------------------------------------------------

static TWResultCode HandleATResponse(TWEntry* entry, TWContext* ctx);

static TWResultCode processEntry(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntryNETWORK_INFO(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntryJPAN(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntryEndDevice(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntryMatchDesc(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntrySimpleDesc(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntryWriteAttr(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntryReadAttr(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntryTemperature(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntrySwitchDoorLockState(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntryZCLDefaultResponse(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntryZoneEnrollRequest(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntryEnrolled(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntryZoneStatus(TWEntry* entry, TWContext* ctx);
static TWResultCode processEntryAddressResponse(TWEntry* entry, TWContext* ctx);

static TWResultCode Reset(TWContext* ctx);
static TWResultCode GetRemoteEUI(char *nodeId, char* outRemoteEUI, TWContext* ctx);
static TWResultCode CreatePAN(TWContext* ctx);
static TWResultCode EnableJoin(bool isKeyEncrypted, TWContext* ctx);
static TWResultCode FindMatchNodes(TWContext* ctx);
static TWResultCode FindClusters(char nodeId[], char endpoint[], TWContext* ctx);

static TWResultCode TelNetworkInfoHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelJpanHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelEndDeviceJoinHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelMatchDescHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelAddressResponseHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelSimpleDescHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelSimpleDescInClusterHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelWriteAttrHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelReadAttrHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelReadAttrHandlerTemperature(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelZCLDefaultResponseHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelSwitchDoorLockStateHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelZoneEnrollRequestHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelEnrolledHandler(int count, char* tokens[], TWContext* ctx);
static TWResultCode TelZoneStatusHandler(int count, char* tokens[], TWContext* ctx);

static TWResultCode AsciiHexToValue(char* hexString, int length, uint64_t* value);
static int AsciiToHex(char c);
static int Tokenize(const char *input, const char* delimiters, char* output[]);

static void DeallocateTWDeviceList(TWContext* ctx);
static TWContext* GetTWContext(PIPlugin_Zigbee * plugin);

//-----------------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------------

static TWATResultHandlerPair g_TWATResultHandlerPairArray[] =
{
    {"+N=",         TelNetworkInfoHandler},
    {"JPAN:",       TelJpanHandler},
    {"RFD:",        TelEndDeviceJoinHandler},       //e.g SmartThings Open/Closed Sensor
    {"FFD:",        TelEndDeviceJoinHandler},       //e.g SmartThings Plug
    {"SED:",        TelEndDeviceJoinHandler},
    {"ZED:",        TelEndDeviceJoinHandler},
    {"MatchDesc:",  TelMatchDescHandler},
    {"SimpleDesc:", TelSimpleDescHandler},
    {"InCluster:",  TelSimpleDescInClusterHandler},
    {"WRITEATTR:",  TelWriteAttrHandler},
    {"RESPATTR:",   TelReadAttrHandler},
    {"TEMPERATURE:",TelReadAttrHandlerTemperature},
    {"DFTREP",      TelZCLDefaultResponseHandler},
    {"DRLOCRSP:",   TelSwitchDoorLockStateHandler},
    {"DRUNLOCKRSP:",TelSwitchDoorLockStateHandler},
    {"ZENROLLREQ:", TelZoneEnrollRequestHandler},
    {"ENROLLED:",   TelEnrolledHandler},
    {"ZONESTATUS:", TelZoneStatusHandler},
    {"AddrResp:",   TelAddressResponseHandler},
    {"Unknown:",    TelNetworkInfoHandler}
};

static TWContext* g_twContextList = NULL;

void InitializeContext(TWContext* context)
{
    context->g_port = NULL;
    memset(context->g_LocalEUI, '\0', sizeof(context->g_LocalEUI));
    memset(context->g_WIPRemoteEUI, '\0', sizeof(context->g_WIPRemoteEUI));
    memset(context->g_WIPRemoteNodeId, '\0', sizeof(context->g_WIPRemoteNodeId));

    context->g_ZigBeeStatus.state = ZB_STATE_UNKNOWN;
    context->g_ZigBeeStatus.panId = 0;
    context->g_ZigBeeStatus.extPanId = 0;
    context->g_ZigBeeStatus.remoteAttributeValueRead = NULL;
    context->g_ZigBeeStatus.remoteAtrributeValueReadLength = 0;

    context->g_FoundMatchedDeviceList = NULL;
    context->g_WIPDevice = NULL;

    context->g_DeviceFoundCallback = NULL;
    context->g_EnrollmentSucceedCallback = NULL;
    context->g_DeviceStatusUpdateCallback = NULL;
    context->g_EndDeviceNodeIdChangedCallback = NULL;
}

/*****************************************************************************/
/*                                                                           */
/* Public functions                                                          */
/*                                                                           */
/*****************************************************************************/
OCStackResult TWInitialize(PIPlugin_Zigbee* plugin, const char* deviceDevPath)
{
    OIC_LOG(INFO, TAG, "Enter TWInitialize()");

    TWResultCode twCode = TW_RESULT_ERROR;

    if ((plugin == NULL) || (deviceDevPath == NULL))
    {
        return OC_STACK_INVALID_PARAM;
    }

    if (g_twContextList != NULL)
    {
        TWContext* out = NULL;
        TWContext* temp = NULL;
        LL_FOREACH_SAFE(g_twContextList, out, temp)
        {
            if (out->g_plugin == plugin)
            {
                //Ignore because it's already in the list.
                return TW_RESULT_OK;
            }
        }
    }

    TWContext* ctx = (TWContext*)OICCalloc(1, sizeof(*ctx));
    if (ctx == NULL)
    {
        return OC_STACK_NO_MEMORY;
    }
    InitializeContext(ctx);
    ctx->g_plugin = plugin;
    ctx->g_port = deviceDevPath;

    OIC_LOG_V(INFO, TAG, "Attempt to open %s", deviceDevPath);

    twCode = TWStartSock(ctx->g_plugin, deviceDevPath);  //TODO:
    if (twCode != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to open %s because of error: %d", deviceDevPath, twCode);
        OICFree(ctx);
        return OC_STACK_ERROR;
    }

    char* eui = NULL;
    twCode = TWGetEUI(ctx->g_plugin, &eui);
    if (twCode != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to get EUI because of error: %d", twCode);
        OICFree(ctx);
        return OC_STACK_ERROR;
    }
    OICStrcpy(ctx->g_LocalEUI, sizeof(ctx->g_LocalEUI), eui);
    OIC_LOG_V(INFO, TAG, "LocalEUI=%s", ctx->g_LocalEUI);
    OICFree(eui);

    bool wantReset = false;     //TODO:
    if (wantReset)
    {
        twCode = Reset(ctx);
        if (twCode != TW_RESULT_OK)
        {
            OIC_LOG(ERROR, TAG, "ZigBee Initialization - Reset");
            OICFree(ctx);
            return OC_STACK_ERROR;
        }
    }

    twCode = CreatePAN(ctx);
    if (twCode != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "CreatePan Failed");
        OIC_LOG(ERROR, TAG, "TWInitialize() - MUST STOP NOW");
        ctx->g_ZigBeeStatus.state = ZB_STATE_UNKNOWN;
        OICFree(ctx);
        return OC_STACK_ERROR;
    }
    else
    {
        OIC_LOG(INFO, TAG, "CreatePan Succeed");
        OIC_LOG(INFO, TAG, "TWInitialize() Succeed");
        ctx->g_ZigBeeStatus.state = ZB_STATE_INIT;
        LL_APPEND(g_twContextList, ctx);
        return OC_STACK_OK;
    }
}

OCStackResult TWDiscover(PIPlugin_Zigbee* plugin)
{
    OIC_LOG(INFO, TAG, "Enter TWDiscover()");

    OCStackResult ret = OC_STACK_ERROR;
    TWResultCode twRet = TW_RESULT_ERROR;

    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        OIC_LOG(ERROR, TAG, "Invalid Param");
        return OC_STACK_INVALID_PARAM;
    }

    if (ctx->g_DeviceFoundCallback == NULL)
    {
        OIC_LOG(INFO, TAG, "Required TWDeviceFoundCallback.");
        return OC_STACK_ERROR;
    }

    twRet = EnableJoin(false, ctx);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "EnableJoin");
        return OC_STACK_ERROR;
    }

    twRet = FindMatchNodes(ctx);
    if (twRet == TW_RESULT_OK)
    {
        OIC_LOG(INFO, TAG, "FindMatchNodes");
        ret = OC_STACK_OK;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "FindMatchNodes");
        ret = OC_STACK_ERROR;
    }

    OIC_LOG_V(INFO, TAG, "Leave TWDiscover() with ret=%d", ret);
    return ret;
}

OCStackResult TWSetAttribute(char* extendedUniqueId, char* nodeId, char* endpointId,
                             char* clusterId, char* attributeId, char* attributeType,
                             char* newValue, PIPlugin_Zigbee* plugin)
{
    //Ask:  AT+WRITEATR:5DA7,01,0,0003,0000,21,01

    OIC_LOG(INFO, TAG, "Enter TWSetAttribute()");

    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        OIC_LOG(ERROR, TAG, "Invalid Param");
        return OC_STACK_INVALID_PARAM;
    }

    (void)extendedUniqueId;

    OCStackResult ret = OC_STACK_ERROR;
    TWResultCode twRet = TW_RESULT_ERROR;

    int size =  strlen(AT_CMD_WRITE_ATR) + strlen(nodeId) +
                SEPARATOR_LENGTH + strlen(endpointId) +
                SEPARATOR_LENGTH + strlen(SENDMODE) +
                SEPARATOR_LENGTH + strlen(clusterId) +
                SEPARATOR_LENGTH + strlen(attributeId) +
                SEPARATOR_LENGTH + strlen(attributeType) +
                SEPARATOR_LENGTH + strlen(newValue) + 1;

    TWEntry* entry = NULL;

    char* cmdString = (char*)OICMalloc(size * sizeof(char));
    if (cmdString == NULL)
    {
        OIC_LOG(ERROR, TAG, "No Memory");
        ret = OC_STACK_ERROR;
        goto exit;
    }
    snprintf(cmdString, size, "%s%s,%s,%s,%s,%s,%s,%s",
             AT_CMD_WRITE_ATR, nodeId, endpointId, SENDMODE,
             clusterId, attributeId, attributeType, newValue);

    twRet = TWIssueATCommand(ctx->g_plugin, cmdString);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Write %s", cmdString);
        ret = OC_STACK_ERROR;
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Write %s", cmdString);

    twRet = TWDequeueEntry(ctx->g_plugin, &entry, TW_WRITEATTR);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "TWDequeueEntry");
        ret = OC_STACK_ERROR;
        goto exit;
    }
    if (entry == NULL)
    {
        OIC_LOG(INFO, TAG, "TWEntry is NULL.");
        ret = OC_STACK_ERROR;
        goto exit;
    }

    twRet = processEntry(entry, ctx);
    if (twRet == TW_RESULT_ERROR_INVALID_OP)
    {
        OIC_LOG_V(INFO, TAG, "Write %s - Invalid Operation", cmdString);
        ret = OC_STACK_INVALID_OPTION;
        goto exit;
    }
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "processEntry");
        ret = OC_STACK_ERROR;
        goto exit;
    }

    ret = OC_STACK_OK;

exit:
    if (entry != NULL)
    {
        TWDeleteEntry(ctx->g_plugin, entry);
    }
    OICFree(cmdString);
    OIC_LOG_V(INFO, TAG, "Leave TWSetAttribute() with ret=%d", ret);
    return ret;
}

OCStackResult TWGetAttribute(char* extendedUniqueId, char* nodeId, char* endpointId,
                             char* clusterId, char* attributeId,
                             char** outValue, uint8_t* outValueLength,
                             PIPlugin_Zigbee* plugin)
{
    //Ask:  AT+READATR:FE5A,01,0,0402,0002

    OIC_LOG(INFO, TAG, "Enter TWGetAttribute()");

    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        return OC_STACK_INVALID_PARAM;
    }

    (void)extendedUniqueId;

    OCStackResult ret = OC_STACK_ERROR;
    TWResultCode twRet = TW_RESULT_ERROR;

    int size =  strlen(AT_CMD_READ_ATR) + strlen(nodeId) +
                SEPARATOR_LENGTH + strlen(endpointId) +
                SEPARATOR_LENGTH + strlen(SENDMODE) +
                SEPARATOR_LENGTH + strlen(clusterId) +
                SEPARATOR_LENGTH + strlen(attributeId) + 1;

    TWEntry* entry = NULL;

    char* cmdString = (char*)OICMalloc(size * sizeof(char));
    if (cmdString == NULL)
    {
        OIC_LOG(ERROR, TAG, "No Memory");
        ret = OC_STACK_NO_MEMORY;
        goto exit;
    }
    int stringRet = snprintf(cmdString, size, "%s%s%s%s%s%s%s%s%s%s",
                             AT_CMD_READ_ATR, nodeId,
                             SEPARATOR, endpointId,
                             SEPARATOR, SENDMODE,
                             SEPARATOR, clusterId,
                             SEPARATOR, attributeId);
    if(stringRet <= 0)
    {
        OIC_LOG(ERROR, TAG, "Build command error.");
        ret = OC_STACK_ERROR;
        goto exit;
    }
    twRet = TWIssueATCommand(ctx->g_plugin, cmdString);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Write %s", cmdString);
        ret = OC_STACK_ERROR;
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Write %s", cmdString);

    twRet = TWDequeueEntry(ctx->g_plugin, &entry, TW_RESPATTR);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "TWDequeueEntry");
        ret = OC_STACK_ERROR;
        goto exit;
    }
    if (entry == NULL)
    {
        OIC_LOG(INFO, TAG, "TWEntry is NULL");
        ret = OC_STACK_ERROR;
        goto exit;
    }

    twRet = processEntry(entry, ctx);
    if (twRet != TW_RESULT_REMOTE_ATTR_HAS_VALUE)
    {
        OIC_LOG(ERROR, TAG, "processEntry.");
        ret = OC_STACK_ERROR;
        goto exit;
    }

    size = strlen(ctx->g_ZigBeeStatus.remoteAttributeValueRead) + 1;
    *outValue = (char*)OICMalloc(sizeof(char) * size);
    if (*outValue == NULL)
    {
        OIC_LOG(ERROR, TAG, "No Memory");
        ret = OC_STACK_NO_MEMORY;
        goto exit;
    }

    OICStrcpy(*outValue, size, ctx->g_ZigBeeStatus.remoteAttributeValueRead);
    *outValueLength = ctx->g_ZigBeeStatus.remoteAtrributeValueReadLength;
    OICFree(ctx->g_ZigBeeStatus.remoteAttributeValueRead);
    ctx->g_ZigBeeStatus.remoteAttributeValueRead = NULL;
    ctx->g_ZigBeeStatus.remoteAtrributeValueReadLength = 0;
    OIC_LOG(INFO, TAG, "TWGetAttribute() gets an attribute value.");
    ret = OC_STACK_OK;

exit:
    if (entry != NULL)
    {
        TWDeleteEntry(ctx->g_plugin, entry);
    }
    OICFree(cmdString);
    OIC_LOG_V(INFO, TAG, "Leave TWGetAttribute() with ret=%d", ret);
    return ret;
}

OCStackResult TWSwitchOnOff(char* nodeId, char* endpointId, char* newState,
                            PIPlugin_Zigbee* plugin)
{
    //AT+RONOFF:<Address>,<EP>,<SendMode>[,<ON/OFF>]
    //AT+RONOFF:9E2B,01,0,1
    //      OK
    //      DFTREP:9E2B,01,0006,01,00

    OIC_LOG(INFO, TAG, "Enter TWSwitchOnOff()");

    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        OIC_LOG(ERROR, TAG, "Invalid Param");
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    TWResultCode twRet = TW_RESULT_UNKNOWN;

    int size = 0;
    if (newState == NULL)
    {
        size =  strlen(AT_CMD_RUN_ON_OFF) + strlen(nodeId) +
                SEPARATOR_LENGTH + strlen(endpointId) +
                SEPARATOR_LENGTH + strlen(SENDMODE) + 1;
    }
    else
    {
        size =  strlen(AT_CMD_RUN_ON_OFF) + strlen(nodeId) +
                SEPARATOR_LENGTH + strlen(endpointId) +
                SEPARATOR_LENGTH + strlen(SENDMODE) +
                SEPARATOR_LENGTH + strlen(newState) + 1;
    }

    TWEntry* entry = NULL;

    char* cmdString = (char*)OICMalloc(size * sizeof(char));
    if (cmdString == NULL)
    {
        OIC_LOG(ERROR, TAG, "No Memory");
        ret = OC_STACK_NO_MEMORY;
        goto exit;
    }

    int stringRet = snprintf(cmdString, size, "%s%s%s%s%s%s",
                             AT_CMD_RUN_ON_OFF, nodeId, SEPARATOR,
                             endpointId, SEPARATOR, SENDMODE);
    if(stringRet <= 0)
    {
        OIC_LOG(ERROR, TAG, "Build command error.");
        ret = OC_STACK_ERROR;
        goto exit;
    }
    if (newState != NULL)
    {
        OICStrcat(cmdString, size, SEPARATOR);
        OICStrcat(cmdString, size, newState);
    }

    twRet = TWIssueATCommand(ctx->g_plugin, cmdString);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Write %s", cmdString);
        ret = OC_STACK_ERROR;
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Write %s", cmdString);

    twRet = TWDequeueEntry(ctx->g_plugin, &entry, TW_DFTREP);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "TWDequeueEntry");
        ret = OC_STACK_ERROR;
        goto exit;
    }
    if (entry == NULL)
    {
        OIC_LOG(ERROR, TAG, "TWEntry is NULL");
        ret = OC_STACK_ERROR;
        goto exit;
    }

    twRet = processEntry(entry, ctx);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "processEntry - %s", cmdString);
        ret = OC_STACK_ERROR;
        goto exit;
    }

    ret = OC_STACK_OK;

exit:
    if (entry != NULL)
    {
        TWDeleteEntry(ctx->g_plugin, entry);
    }
    OICFree(cmdString);
    OIC_LOG_V(INFO, TAG, "Leave TWSwitchOnOff() with ret=%d", ret);
    return ret;
}

OCStackResult TWMoveToLevel(char* nodeId, char* endpointId,
                            char* onOffState, char* level, char* transTime,
                            PIPlugin_Zigbee* plugin)
{
    //AT+LCMVTOLEV:<Address>,<EP>,<SendMode>,<ON/OFF>,<LevelValue>,<TransTime>

    OIC_LOG(INFO, TAG, "Enter TWMoveToLevel()");

    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        OIC_LOG(ERROR, TAG, "Invalid Param");
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    TWResultCode twRet = TW_RESULT_UNKNOWN;

    int size = 0;
    size =  strlen(AT_CMD_MOVE_TO_LEVEL) + strlen(nodeId) +
            SEPARATOR_LENGTH + strlen(endpointId) +
            SEPARATOR_LENGTH + strlen(SENDMODE) +
            SEPARATOR_LENGTH + strlen(onOffState) +
            SEPARATOR_LENGTH + strlen(level) +
            SEPARATOR_LENGTH + strlen(transTime) + 1;

    TWEntry* entry = NULL;

    char* cmdString = (char*)OICMalloc(size * sizeof(char));
    if (cmdString == NULL)
    {
        OIC_LOG(ERROR, TAG, "No Memory");
        ret = OC_STACK_NO_MEMORY;
        goto exit;
    }

    int stringRet = snprintf(cmdString, size, "%s%s%s%s%s%s%s%s%s%s%s%s",
                             AT_CMD_MOVE_TO_LEVEL, nodeId,
                             SEPARATOR, endpointId,
                             SEPARATOR, SENDMODE,
                             SEPARATOR, onOffState,
                             SEPARATOR, level,
                             SEPARATOR, transTime);
    if(stringRet <= 0)
    {
        OIC_LOG(ERROR, TAG, "Build command error.");
        ret = OC_STACK_ERROR;
        goto exit;
    }

    twRet = TWIssueATCommand(ctx->g_plugin, cmdString);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Write %s", cmdString);
        ret = OC_STACK_ERROR;
        goto exit;
    }

    OIC_LOG_V(INFO, TAG, "Write %s", cmdString);

    twRet = TWDequeueEntry(ctx->g_plugin, &entry, TW_DFTREP);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "TWDequeueEntry");
        ret = OC_STACK_ERROR;
        goto exit;
    }
    if (entry == NULL)
    {
        OIC_LOG(ERROR, TAG, "TWEntry is NULL");
        ret = OC_STACK_ERROR;
        goto exit;
    }

    twRet = processEntry(entry, ctx);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "processEntry - %s", cmdString);
        ret = OC_STACK_ERROR;
        goto exit;
    }

    ret = OC_STACK_OK;

exit:
    if (entry != NULL)
    {
        TWDeleteEntry(ctx->g_plugin, entry);
    }
    OICFree(cmdString);
    OIC_LOG_V(INFO, TAG, "Leave TWMoveToLevel() with ret=%d", ret);
    return ret;
}

OCStackResult TWSwitchDoorLockState(char* nodeId, char* endpointId, char* newState,
                                    PIPlugin_Zigbee* plugin)
{
    //AT+DRLOCK:<Address>,<EP>,<SendMode>,<Lock/Unlock>

    OIC_LOG(INFO, TAG, "Enter TWSwitchDoorLockState()");

    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        OIC_LOG(ERROR, TAG, "Invalid Param");
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    TWResultCode twRet = TW_RESULT_UNKNOWN;

    int size = 0;
    size =  strlen(AT_CMD_DOOR_LOCK) + strlen(nodeId) +
            SEPARATOR_LENGTH + strlen(endpointId) +
            SEPARATOR_LENGTH + strlen(SENDMODE) +
            SEPARATOR_LENGTH + strlen(newState) + 1;

    TWEntry* entry = NULL;

    char* cmdString = (char*)OICMalloc(size * sizeof(char));
    if (cmdString == NULL)
    {
        OIC_LOG(ERROR, TAG, "No Memory");
        ret = OC_STACK_NO_MEMORY;
        goto exit;
    }

    int stringRet = snprintf(cmdString, size, "%s%s%s%s%s%s%s%s",
                             AT_CMD_DOOR_LOCK, nodeId,
                             SEPARATOR, endpointId,
                             SEPARATOR, SENDMODE,
                             SEPARATOR, newState);
    if(stringRet <= 0)
    {
        OIC_LOG(ERROR, TAG, "Build command error.");
        ret = OC_STACK_ERROR;
        goto exit;
    }

    twRet = TWIssueATCommand(ctx->g_plugin, cmdString);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Write %s", cmdString);
        ret = OC_STACK_ERROR;
        goto exit;
    }

    OIC_LOG_V(INFO, TAG, "Write %s", cmdString);

    twRet = TWDequeueEntry(ctx->g_plugin, &entry, TW_DFTREP);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "TWDequeueEntry");
        ret = OC_STACK_ERROR;
        goto exit;
    }

    if (entry == NULL)
    {
        OIC_LOG(ERROR, TAG, "TWEntry is NULL");
        ret = OC_STACK_ERROR;
        goto exit;
    }

    twRet = processEntry(entry, ctx);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "processEntry - %s", cmdString);
        ret = OC_STACK_ERROR;
        goto exit;
    }

    ret = OC_STACK_OK;

exit:
    if (entry != NULL)
    {
        TWDeleteEntry(ctx->g_plugin, entry);
    }
    OICFree(cmdString);
    OIC_LOG_V(INFO, TAG, "Leave TWSwitchDoorLockState() with ret=%d", ret);
    return ret;
}

OCStackResult TWColorMoveToColorTemperature(char* nodeId, char* endpointId,
                                            char* colorTemperature, char* transTime,
                                            PIPlugin_Zigbee* plugin)
{

    //AT+CCMVTOCT:<Address>,<EP>,<SendMode>,<ColorTemperature>,<TransTime>
    //  OK
    //  ERROR:<errorcode>

    OIC_LOG(INFO, TAG, "Enter TWColorMoveToColorTemperature()");

    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        OIC_LOG(ERROR, TAG, "Invalid Param");
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    TWResultCode twRet = TW_RESULT_UNKNOWN;

    int size = 0;
    size =  strlen(AT_CMD_COLOR_CTRL_MOVE_TO_COLOR_TEMPERATURE) + strlen(nodeId) +
            SEPARATOR_LENGTH + strlen(endpointId) +
            SEPARATOR_LENGTH + strlen(SENDMODE) +
            SEPARATOR_LENGTH + strlen(colorTemperature) +
            SEPARATOR_LENGTH + strlen(transTime) + 1;

    TWEntry* entry = NULL;

    char* cmdString = (char*)OICMalloc(size * sizeof(char));
    if (cmdString == NULL)
    {
        OIC_LOG(ERROR, TAG, "No Memory");
        ret = OC_STACK_NO_MEMORY;
        goto exit;
    }

    int stringRet = snprintf(cmdString, size, "%s%s%s%s%s%s%s%s%s%s",
                             AT_CMD_COLOR_CTRL_MOVE_TO_COLOR_TEMPERATURE, nodeId,
                             SEPARATOR, endpointId,
                             SEPARATOR, SENDMODE,
                             SEPARATOR, colorTemperature,
                             SEPARATOR, transTime);
    if(stringRet <= 0)
    {
        OIC_LOG(ERROR, TAG, "Build command error.");
        ret = OC_STACK_ERROR;
        goto exit;
    }
    twRet = TWIssueATCommand(ctx->g_plugin, cmdString);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Write %s", cmdString);
        ret = OC_STACK_ERROR;
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Write %s", cmdString);

    twRet = TWDequeueEntry(ctx->g_plugin, &entry, TW_DFTREP);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "TWDequeueEntry - %s", cmdString);
        ret = OC_STACK_ERROR;
        goto exit;
    }
    if (entry == NULL)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry is NULL - %s", cmdString);
        ret = OC_STACK_ERROR;
        goto exit;
    }

    twRet = processEntry(entry, ctx);
    if (twRet != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "processEntry - %s", cmdString);
        ret = OC_STACK_ERROR;
        goto exit;
    }

    ret = OC_STACK_OK;

exit:
    if (entry != NULL)
    {
        TWDeleteEntry(ctx->g_plugin, entry);
    }
    OICFree(cmdString);
    OIC_LOG_V(INFO, TAG, "Leave TWColorMoveToColorTemperature() with ret=%d", ret);
    return ret;
}

OCStackResult TWSetDiscoveryCallback(const TWDeviceFoundCallback callback, PIPlugin_Zigbee* plugin)
{
    OIC_LOG(INFO, TAG, "Enter TWSetDiscoveryCallback()");

    OCStackResult ret = OC_STACK_OK;

    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        ret = OC_STACK_INVALID_PARAM;
    }
    else
    {
        if (callback != NULL)
        {
            ctx->g_DeviceFoundCallback= callback;
        }
        else
        {
            ctx->g_DeviceFoundCallback = NULL;
        }
    }

    OIC_LOG_V(INFO, TAG, "Leave TWSetDiscoveryCallback() with ret=%d", ret);
    return ret;
}

OCStackResult TWSetEndDeviceNodeIdChangedCallback(TWDeviceNodeIdChangedCallback callback,
                                                  PIPlugin_Zigbee* plugin)
{
    OIC_LOG(INFO, TAG, "Enter TWSetEndDeviceNodeIdChangedCallback()");

    OCStackResult ret = OC_STACK_OK;

    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        ret = OC_STACK_INVALID_PARAM;
    }
    else
    {
        if (callback != NULL)
        {
            ctx->g_EndDeviceNodeIdChangedCallback= callback;
        }
        else
        {
            ctx->g_EndDeviceNodeIdChangedCallback = NULL;
        }
    }
    OIC_LOG_V(INFO, TAG, "Leave TWSetEndDeviceNodeIdChangedCallback() with ret=%d", ret);
    return ret;
}

OCStackResult TWSetStatusUpdateCallback(TWDeviceStatusUpdateCallback callback,
                                        PIPlugin_Zigbee* plugin)
{
    OIC_LOG(INFO, TAG, "Enter TWSetStatusUpdateCallback()");

    OCStackResult ret = OC_STACK_OK;

    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        ret = OC_STACK_INVALID_PARAM;
    }
    else
    {
        if (callback != NULL)
        {
            ctx->g_DeviceStatusUpdateCallback= callback;
        }
        else
        {
            ctx->g_DeviceStatusUpdateCallback = NULL;
        }
    }

    OIC_LOG_V(INFO, TAG, "Leave TWSetStatusUpdateCallback() with ret=%d", ret);
    return ret;
}

OCStackResult TWListenForStatusUpdates(char* nodeId, char* endpointId, PIPlugin_Zigbee* plugin)
{
    OIC_LOG(INFO, TAG, "Enter TWListenForStatusUpdates()");

    OCStackResult ret = OC_STACK_OK;

    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        ret = OC_STACK_INVALID_PARAM;
    }
    else
    {
        char* zoneClusterID = "0500";
        char* zoneAttributeID = "0010";
        char* attributeDateType = "F0";

        ret = TWSetAttribute(NULL, nodeId, endpointId,
                             zoneClusterID, zoneAttributeID, attributeDateType,
                             ctx->g_LocalEUI, ctx->g_plugin);

        if (ret == OC_STACK_INVALID_OPTION)
        {
            OIC_LOG(INFO, TAG, "Already registered for ZoneStatusUpdate");
            ret = OC_STACK_OK;
        }
    }
    OIC_LOG_V(INFO, TAG, "Leave TWListenForStatusUpdates() with ret=%d", ret);
    return ret;
}

OCStackResult TWProcess(PIPlugin_Zigbee* plugin)
{
    if (plugin == NULL)
    {
        return OC_STACK_INVALID_PARAM;
    }
    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        return OC_STACK_INVALID_PARAM;
    }

    TWResultCode ret = TW_RESULT_UNKNOWN;

    while (true)
    {
        TWEntry* entry = NULL;
        ret = TWDequeueEntry(ctx->g_plugin, &entry, TW_NONE);
        if (ret != TW_RESULT_OK)
        {
            OIC_LOG(ERROR, TAG, "TWDequeueEntry");
            ret = OC_STACK_ERROR;
            break;
        }
        if (entry == NULL)
        {
            ret = OC_STACK_OK;
            break;
        }

        ret = processEntry(entry, ctx);
        if (ret != TW_RESULT_OK)
        {
            OIC_LOG(ERROR, TAG, "processEntry");
            ret = TWDeleteEntry(ctx->g_plugin, entry);
            if(ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "Failed to delete entry.");
                ret = OC_STACK_ERROR;
                break;
            }
        }
        else
        {
            OIC_LOG(INFO, TAG, "processEntry");
            ret = TWDeleteEntry(ctx->g_plugin, entry);
            if(ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "Failed to delete entry.");
                ret = OC_STACK_ERROR;
                break;
            }
        }
    }

    return ret;
}

OCStackResult TWUninitialize(PIPlugin_Zigbee* plugin)
{
    OIC_LOG(INFO, TAG, "Enter TWUninitializeZigBee()");
    OCStackResult ret = OC_STACK_ERROR;

    TWContext* ctx = GetTWContext(plugin);
    if (ctx == NULL)
    {
        ret = OC_STACK_INVALID_PARAM;
    }
    else
    {
        TWResultCode twRet = TWStopSock(ctx->g_plugin);
        if (twRet == TW_RESULT_OK)
        {
            OIC_LOG(INFO, TAG, "TWStopSock");
            ret = OC_STACK_OK;
        }
        else
        {
            OIC_LOG(ERROR, TAG, "TWStopSock");
            ret = OC_STACK_ERROR;
        }
        DeallocateTWDeviceList(ctx);

        LL_DELETE(g_twContextList, ctx);
        OICFree(ctx);
    }

    OIC_LOG_V(INFO, TAG, "Leave TWUninitializeZigBee() with ret=%d", ret);
    return ret;
}

//-----------------------------------------------------------------------------
// Internal functions
//-----------------------------------------------------------------------------

TWResultCode processEntry(TWEntry *entry, TWContext* ctx)
{
    OIC_LOG(INFO, TAG, "Enter processEntry()");

    TWResultCode ret = TW_RESULT_UNKNOWN;
    switch(entry->type)
    {
        case TW_NETWORK_INFO:
            ret = processEntryNETWORK_INFO(entry, ctx);
            if ((ret != TW_RESULT_NO_LOCAL_PAN) &&
                (ret != TW_RESULT_HAS_LOCAL_PAN))
            {
                OIC_LOG(ERROR, TAG, "processEntryNETWORK_INFO.");
            }
            break;
        case TW_JPAN:
            ret = processEntryJPAN(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntryJPAN.");
            }
            break;
        case TW_SED:
            ret = processEntryEndDevice(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntrySED.");
            }
            break;
        case TW_RFD:
            ret = processEntryEndDevice(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntryRFD.");
            }
            break;
        case TW_FFD:
            ret = processEntryEndDevice(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntryFFD.");
            }
            break;
        case TW_ZED:
            ret = processEntryEndDevice(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntryZED.");
            }
            break;
        case TW_MATCHDESC:
            ret = processEntryMatchDesc(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntryMatchDesc.");
            }
            break;
        case TW_SIMPLEDESC:
            ret = processEntrySimpleDesc(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntrySimpleDesc.");
            }
            break;
        case TW_WRITEATTR:
            ret = processEntryWriteAttr(entry, ctx);
            if (ret == TW_RESULT_ERROR_INVALID_OP)
            {
               OIC_LOG_V(INFO, TAG, "processEntryWriteAttr - ret=%d", TW_RESULT_ERROR_INVALID_OP);
            }
            else if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntryWriteAttr.");
            }
            break;
        case TW_RESPATTR:
            ret = processEntryReadAttr(entry, ctx);
            if (ret != TW_RESULT_REMOTE_ATTR_HAS_VALUE)
            {
                OIC_LOG(ERROR, TAG, "processEntryReadAttr.");
            }
            break;
        case TW_TEMPERATURE:
            ret = processEntryTemperature(entry, ctx);
            if (ret != TW_RESULT_REMOTE_ATTR_HAS_VALUE)
            {
                OIC_LOG(ERROR, TAG, "processEntryTemperature.");
            }
            break;
        case TW_DRLOCRSP:
            ret = processEntrySwitchDoorLockState(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntrySwitchDoorLockState.");
            }
            break;
        case TW_DRUNLOCKRSP:
            ret = processEntrySwitchDoorLockState(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntrySwitchDoorLockState.");
            }
            break;
        case TW_DFTREP:
            ret = processEntryZCLDefaultResponse(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntryZCLDefaultResponse.");
            }
            break;
        case TW_ZENROLLREQ:
            ret = processEntryZoneEnrollRequest(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntryZoneEnrollRequest.");
            }
            break;
        case TW_ENROLLED:
            ret = processEntryEnrolled(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntryEnrolled.");
            }
            break;
        case TW_ZONESTATUS:
            ret = processEntryZoneStatus(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntryZoneStatus.");
            }
            break;
        case TW_ADDRESS_RESPONSE:
            ret = processEntryAddressResponse(entry, ctx);
            if (ret != TW_RESULT_OK)
            {
                OIC_LOG(ERROR, TAG, "processEntryAddressResponse.");
            }
            break;
        default:
            OIC_LOG(ERROR, TAG, "processEntry() doesn't receive an valid entry.");
            ret = TW_RESULT_ERROR;
            break;
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntry() with ret=%d", ret);
    return ret;
}

TWResultCode processEntryNETWORK_INFO(TWEntry* entry, TWContext* ctx)
{
    /*
    //at+n
    //      +N=NoPAN
    //      OK

    //at+n
    //      +N=COO,26,-6,7306,133F04EA669C6B24
    //      OK
    */

    OIC_LOG(INFO, TAG, "Enter processEntryNETWORK_INFO()");

    TWResultCode ret = TW_RESULT_UNKNOWN;

    if (strcmp(entry->atErrorCode, AT_STR_ERROR_EVERYTHING_OK) != 0)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR: %s", entry->atErrorCode);
        ret = TW_RESULT_ERROR;
        goto exit;
    }

    ret = HandleATResponse(entry,ctx);

exit:
    OIC_LOG_V(INFO, TAG, "Leave processEntryNETWORK_INFO() with ret=%d", ret);
    return ret;
}

TWResultCode processEntryJPAN(TWEntry* entry, TWContext* ctx)
{
    /*
    //at+en
    //      OK
    //      JPAN:26,7306,133F04EA669C6B24

    //at+en
    //      ERROR:28
    */

    OIC_LOG(INFO, TAG, "Enter processEntryJPAN()");

    TWResultCode ret = TW_RESULT_UNKNOWN;
    if (strcmp(entry->atErrorCode, AT_STR_ERROR_EVERYTHING_OK) == 0)
    {
        ret = HandleATResponse(entry,ctx);
        if (ret == TW_RESULT_NEW_LOCAL_PAN_ESTABLISHED)
        {
            OIC_LOG(INFO, TAG, "New Local PAN established.");
            ret =  TW_RESULT_OK;
        }
        else
        {
            ret = TW_RESULT_ERROR;
        }
    }
    else if (strcmp(entry->atErrorCode, AT_STR_ERROR_NODE_IS_PART_OF_PAN) == 0)
    {
        OIC_LOG(INFO, TAG, "Already Established PAN.");
        ret = TW_RESULT_OK;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR: %s", entry->atErrorCode);
        ret = TW_RESULT_ERROR;
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntryJPAN() with ret=%d", ret);
    return ret;
}

TWResultCode processEntryEndDevice(TWEntry* entry, TWContext* ctx)
{
    OIC_LOG(INFO, TAG, "Enter processEntryEndDevice()");

    TWResultCode ret = TW_RESULT_UNKNOWN;
    ret = HandleATResponse(entry,ctx);
    if (ret != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "HandleATResponse");
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntryEndDevice() with ret=%d", ret);
    return ret;
}

TWResultCode processEntryMatchDesc(TWEntry* entry, TWContext* ctx)
{
    //MatchDesc:0B4A,00,01

    OIC_LOG(INFO, TAG, "Enter processEntryMatchDesc()");
    TWResultCode ret = TW_RESULT_UNKNOWN;

    if (strcmp(entry->atErrorCode, AT_STR_ERROR_EVERYTHING_OK) != 0)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR = %s", entry->atErrorCode);
        ret = TW_RESULT_ERROR;
    }
    else
    {
        ret = HandleATResponse(entry,ctx);
        if (ret == TW_RESULT_OK)
        {
            OIC_LOG(INFO, TAG, "HandleATResponse");
            ret = FindClusters(ctx->g_WIPDevice->nodeId,
                               ctx->g_WIPDevice->endpointOfInterest->endpointId,
                               ctx);
            if (ret == TW_RESULT_OK)
            {
                OIC_LOG(INFO, TAG, "FindClusters - Found a match node");
                if (ctx->g_DeviceFoundCallback != NULL)
                {
                    OIC_LOG(INFO, TAG, "Found a match node -- invoke callback");
                    ctx->g_DeviceFoundCallback(ctx->g_WIPDevice, ctx->g_plugin);
                }
                ret =  TW_RESULT_OK;
            }
            else
            {
                OIC_LOG(ERROR, TAG, "FindClusters");
                ret = TW_RESULT_ERROR;
            }
        }
        else
        {
            OIC_LOG(ERROR, TAG, "HandleATResponse");
            ret = TW_RESULT_ERROR;
        }

        ctx->g_WIPDevice = NULL; //reset and do not deallocate it
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntryMatchDesc() with ret=%d", ret);
    return ret;
}

TWResultCode processEntrySimpleDesc(TWEntry* entry, TWContext* ctx)
{
    /*
    //AT+SIMPLEDESC:3746,3746,01
    //      SEQ:97
    //      OK
    //
    //      SimpleDesc:3746,00
    //      EP:01
    //      ProfileID:0104
    //      DeviceID:0402v00
    //      InCluster:0000,0001,0003,0402,0500,0020,0B05
    //      OutCluster:0019
    //
    //      ACK:97
    */
    OIC_LOG(INFO, TAG, "Enter processEntrySimpleDesc()");

    TWResultCode ret = TW_RESULT_UNKNOWN;

    if (strcmp((entry->atErrorCode), AT_STR_ERROR_EVERYTHING_OK) != 0)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR = %s", (entry->atErrorCode));
        ret = TW_RESULT_ERROR;
    }
    else
    {
        if (entry->count == 6)   //must be 6 as it is the number of lines to expect
        {
            ret = HandleATResponse(entry,ctx);
            if (ret == TW_RESULT_HAS_CLUSTERS)
            {
                OIC_LOG(INFO, TAG, "has clusters.");
                ret = TW_RESULT_OK;
            }
        }
        else
        {
            OIC_LOG(INFO, TAG, "Received an invalid Simple Descriptor.");
            ret = TW_RESULT_ERROR;
        }
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntrySimpleDesc() returns with ret=%d", ret);
    return ret;
}

TWResultCode processEntryWriteAttr(TWEntry* entry, TWContext* ctx)
{
    //AT+WRITEATR:3A3D,01,0,0003,0000,21,00
    //      OK
    //      WRITEATTR:3A3D,01,0003,,00

    OIC_LOG(INFO, TAG, "Enter processEntryWriteAttr()");

    TWResultCode ret = TW_RESULT_UNKNOWN;

    if (strcmp((entry->atErrorCode), AT_STR_ERROR_EVERYTHING_OK) != 0)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR = %s", entry->atErrorCode);
        ret = TW_RESULT_ERROR;
    }
    else
    {
        ret = HandleATResponse(entry,ctx);
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntryWriteAttr() returns with ret=%d", ret);
    return ret;
}

TWResultCode processEntryReadAttr(TWEntry* entry, TWContext* ctx)
{
    OIC_LOG(INFO, TAG, "Enter processEntryReadAttr()");

    TWResultCode ret = TW_RESULT_UNKNOWN;

    if (strcmp((entry->atErrorCode), AT_STR_ERROR_EVERYTHING_OK) != 0)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR = %s", entry->atErrorCode);
        ret = TW_RESULT_ERROR;
    }
    else
    {
        ret = HandleATResponse(entry,ctx);
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntryReadAttr() returns with ret=%d", ret);
    return ret;
}

TWResultCode processEntryTemperature(TWEntry* entry, TWContext* ctx)
{
    OIC_LOG(INFO, TAG, "Enter processEntryTemperature()");

    TWResultCode ret = TW_RESULT_UNKNOWN;

    if (strcmp((entry->atErrorCode), AT_STR_ERROR_EVERYTHING_OK) != 0)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR = %s", entry->atErrorCode);
        ret = TW_RESULT_ERROR;
    }
    else
    {
        ret = HandleATResponse(entry,ctx);
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntryTemperature() returns with ret=%d", ret);
    return ret;
}

TWResultCode processEntrySwitchDoorLockState(TWEntry* entry, TWContext* ctx)
{
    OIC_LOG(INFO, TAG, "Enter processEntrySwitchDoorLockState()");

    TWResultCode ret = TW_RESULT_UNKNOWN;

    if (strcmp((entry->atErrorCode), AT_STR_ERROR_EVERYTHING_OK) != 0)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR = %s", entry->atErrorCode);
        ret = TW_RESULT_ERROR;
    }
    else
    {
        ret = HandleATResponse(entry,ctx);
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntrySwitchDoorLockState() returns with ret=%d", ret);
    return ret;
}

TWResultCode processEntryZCLDefaultResponse(TWEntry* entry, TWContext* ctx)
{
    OIC_LOG(INFO, TAG, "Enter processEntryZCLDefaultResponse()");

    TWResultCode ret = TW_RESULT_UNKNOWN;

    if (strcmp((entry->atErrorCode), AT_STR_ERROR_EVERYTHING_OK) != 0)
    {
        if (strcmp(entry->atErrorCode, AT_STR_ERROR_MESSAGE_NOT_SENT_TO_TARGET_SUCCESSFULLY) == 0)
        {
            OIC_LOG(ERROR, TAG, "Send to the target not succeed.");
            ret = TW_RESULT_ERROR;
        }
        else
        {
            OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR = %s", entry->atErrorCode);
            ret = TW_RESULT_ERROR;
        }
    }
    else
    {
        ret = HandleATResponse(entry,ctx);
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntryZCLDefaultResponse() returns with ret=%d", ret);
    return ret;
}

TWResultCode processEntryZoneEnrollRequest(TWEntry* entry, TWContext* ctx)
{
    OIC_LOG(INFO, TAG, "Enter processEntryZoneEnrollRequest()");

    TWResultCode ret = TW_RESULT_UNKNOWN;
    if (strcmp(entry->atErrorCode, AT_STR_ERROR_EVERYTHING_OK) != 0)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR: %s", entry->atErrorCode);
        ret = TW_RESULT_ERROR;
    }
    else
    {
        ret = HandleATResponse(entry,ctx);
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntryZoneEnrollRequest() with ret=%d", ret);
    return ret;
}

TWResultCode processEntryEnrolled(TWEntry* entry, TWContext* ctx)
{
    OIC_LOG(INFO, TAG, "Enter processEntryEnrolled()");

    TWResultCode ret = TW_RESULT_UNKNOWN;
    if (strcmp(entry->atErrorCode, AT_STR_ERROR_EVERYTHING_OK) != 0)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR: %s", entry->atErrorCode);
        ret = TW_RESULT_ERROR;
    }
    else
    {
        ret = HandleATResponse(entry,ctx);
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntryEnrolled() with ret=%d", ret);
    return ret;
}

TWResultCode processEntryZoneStatus(TWEntry* entry, TWContext* ctx)
{
    OIC_LOG(INFO, TAG, "Enter processEntryZoneStatus()");

    TWResultCode ret = TW_RESULT_UNKNOWN;
    if (strcmp(entry->atErrorCode, AT_STR_ERROR_EVERYTHING_OK) != 0)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR: %s", entry->atErrorCode);
        ret = TW_RESULT_ERROR;
    }
    else
    {
        ret = HandleATResponse(entry,ctx);
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntryZoneStatus() with ret=%d", ret);
    return ret;
}

TWResultCode processEntryAddressResponse(TWEntry* entry, TWContext* ctx)
{
    OIC_LOG(INFO, TAG, "Enter processEntryAddressResponse()");

    TWResultCode ret = TW_RESULT_UNKNOWN;
    if (strcmp(entry->atErrorCode, AT_STR_ERROR_EVERYTHING_OK) != 0)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry contains AT_ERROR: %s", entry->atErrorCode);
        ret = TW_RESULT_ERROR;
    }
    else
    {
        ret = HandleATResponse(entry,ctx);
    }

    OIC_LOG_V(INFO, TAG, "Leave processEntryAddressResponse() with ret=%d", ret);
    return ret;
}

TWResultCode Reset(TWContext* ctx)
{
    OIC_LOG(INFO, TAG, "Enter Reset()");

    TWResultCode ret = TW_RESULT_ERROR;
    ret = TWIssueATCommand(ctx->g_plugin, AT_CMD_RESET);
    if (ret == TW_RESULT_OK)
    {
        OIC_LOG_V(INFO, TAG, "Write %s", AT_CMD_RESET);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Write %s", AT_CMD_RESET);
    }
    OIC_LOG_V(INFO, TAG, "Leave Reset() with ret=%d", ret);
    return ret;
}

TWResultCode CreatePAN(TWContext* ctx)
{
    /*
    //at+n
    //      +N=NoPAN
    //      OK

    //at+n
    //      +N=COO,26,-6,7306,133F04EA669C6B24
    //      OK

    //at+en
    //      OK
    //      JPAN:26,7306,133F04EA669C6B24

    //at+en
    //      ERROR:28
    */

    OIC_LOG(INFO, TAG, "Enter CreatePAN()");

    TWEntry* entry = NULL;
    TWEntry* entry2 = NULL;
    TWResultCode twRet1 = TW_RESULT_UNKNOWN;
    TWResultCode twRet2 = TW_RESULT_UNKNOWN;
    TWResultCode ret = TW_RESULT_UNKNOWN;
    ret = TWIssueATCommand(ctx->g_plugin, AT_CMD_GET_NETWORK_INFO);
    if (ret != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Write %s", AT_CMD_GET_NETWORK_INFO);
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Write %s", AT_CMD_GET_NETWORK_INFO);
    ret = TWDequeueEntry(ctx->g_plugin, &entry, TW_NETWORK_INFO);
    if (ret != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "TWDequeueEntry - %s", AT_CMD_GET_NETWORK_INFO);
        goto exit;
    }
    if (entry == NULL)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry is NULL - %s", AT_CMD_GET_NETWORK_INFO);
        ret = TW_RESULT_ERROR;
        goto exit;
    }
    ret = processEntry(entry, ctx);
    if (ret == TW_RESULT_HAS_LOCAL_PAN)
    {
        OIC_LOG(INFO, TAG, "Has local PAN.");
        ret = TW_RESULT_OK;
    }
    else if (ret == TW_RESULT_NO_LOCAL_PAN)
    {
        OIC_LOG(INFO, TAG, "Has no local PAN.");
        ret = TWIssueATCommand(ctx->g_plugin, AT_CMD_ESTABLISH_NETWORK);
        if (ret != TW_RESULT_OK)
        {
            OIC_LOG_V(ERROR, TAG, "Write %s", AT_CMD_ESTABLISH_NETWORK);
            goto exit;
        }
        OIC_LOG_V(INFO, TAG, "Write %s", AT_CMD_ESTABLISH_NETWORK);

        ret = TWDequeueEntry(ctx->g_plugin, &entry2, TW_JPAN);
        if (ret != TW_RESULT_OK)
        {
            OIC_LOG_V(ERROR, TAG, "TWDequeueEntry - %s", AT_CMD_ESTABLISH_NETWORK);
            goto exit;
        }
        if (entry2 == NULL)
        {
            OIC_LOG_V(ERROR, TAG, "TWEntry is NULL - %s", AT_CMD_ESTABLISH_NETWORK);
            ret = TW_RESULT_ERROR;
            goto exit;
        }
        ret = processEntry(entry2, ctx);
        if (ret == TW_RESULT_OK)
        {
            OIC_LOG_V(INFO, TAG, "processEntry - %s", AT_CMD_ESTABLISH_NETWORK);
            ret = TW_RESULT_OK;
        }
        else
        {
            OIC_LOG_V(ERROR, TAG, "processEntry - %s", AT_CMD_ESTABLISH_NETWORK);
            ret = TW_RESULT_ERROR;
        }
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "processEntry - unexpected return code: %d", ret);
        ret = TW_RESULT_ERROR;
    }

exit:
    if (entry != NULL)
    {
        twRet1 = TWDeleteEntry(ctx->g_plugin, entry);
        if(twRet1 != TW_RESULT_OK)
        {
            OIC_LOG_V(ERROR, TAG, "TWDeleteEntry 1 - ret=%d", twRet1);
        }
    }
    if (entry2)
    {
        twRet2 = TWDeleteEntry(ctx->g_plugin, entry2);
        if(twRet2 != TW_RESULT_OK)
        {
            OIC_LOG_V(ERROR, TAG, "TWDeleteEntry 2 - ret=%d", twRet2);
        }
    }

    OIC_LOG_V(INFO, TAG, "Leave CreatePan with ret=%d", ret);
    return ret;
}

TWContext * GetTWContext(PIPlugin_Zigbee* plugin)
{
    if(!plugin)
    {
        return NULL;
    }
    TWContext * out = NULL;
    TWContext * tmp = NULL;
    LL_FOREACH_SAFE(g_twContextList, out, tmp)
    {
        if(out->g_plugin == plugin)
        {
            return out;
        }
    }
    return NULL;
}

TWResultCode EnableJoin(bool isKeyEncrypted, TWContext* ctx)
{
    //Ask:          AT+PJOIN
    //Response:     OK

    //Ask:          AT+PJOIN
    //Response:     ERROR:70

    (void)isKeyEncrypted;
    isKeyEncrypted = false;         //TODO: for now - don't encrypt

    TWResultCode ret = TW_RESULT_ERROR;
    char* joinTimeHex = "0F";       //TODO: for now - 15 seconds
    char* broadcast = "FFFC";

    int size =  strlen(AT_CMD_PERMIT_JOIN) + strlen(joinTimeHex) +
                SEPARATOR_LENGTH + strlen(broadcast) + 1;
    char* cmdString = (char*)OICMalloc(size * sizeof(char));
    if (cmdString == NULL)
    {
        OIC_LOG(ERROR, TAG, "No Memory");
        ret = TW_RESULT_ERROR_NO_MEMORY;
        goto exit;
    }
    snprintf(cmdString, size, "%s%s%s%s",
             AT_CMD_PERMIT_JOIN, joinTimeHex, SEPARATOR, broadcast);
    ret = TWIssueATCommand(ctx->g_plugin, cmdString);
    if (ret != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Write %s", cmdString);
        ret = TW_RESULT_ERROR;
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Write %s", cmdString);

    sleep(15);  //must sleep here to permit joining for 15 seconds

exit:
    OICFree(cmdString);
    OIC_LOG_V(INFO, TAG, "Leave EnableJoin() with ret=%d", ret);
    return ret;
}

TWResultCode FindMatchNodes(TWContext* ctx)
{
    //AT+MATCHREQ:0104,03,0003,0006,0402,00
    //      OK
    //      MatchDesc:0B4A,00,01

    //AT+MATCHREQ:0104,03,0999,0999,0999,00
    //      OK

    OIC_LOG(INFO, TAG, "Enter FindMatchNodes()");

    TWResultCode ret = TW_RESULT_UNKNOWN;

    //TODO: add more clusters if needed

    int size = strlen(AT_CMD_MATCH_REQUEST) + strlen(ZB_PROFILE_ID_HOME_AUTOMATION) +
               SEPARATOR_LENGTH + strlen(IN_CLUSTER_COUNT_STRING) +
               SEPARATOR_LENGTH + strlen(ZB_CLUSTER_IDENTIFY) +
               SEPARATOR_LENGTH + strlen(ZB_CLUSTER_ON_OFF) +
               SEPARATOR_LENGTH + strlen(ZB_CLUSTER_TEMPERATURE_MEASUREMENT) +
               SEPARATOR_LENGTH + strlen(ZB_CLUSTER_IAS_ZONE) +
               SEPARATOR_LENGTH + strlen(ZB_CLUSTER_COLOR_CONTROL) +
               SEPARATOR_LENGTH + strlen(OUT_CLUSTER_COUNT_STRING) + 1;

    char* cmdString = (char*)OICMalloc(size * sizeof(char));
    if (cmdString == NULL)
    {
        OIC_LOG(ERROR, TAG, "No Memory");
        ret = TW_RESULT_ERROR_NO_MEMORY;
        goto exit;
    }

    int stringRet = snprintf(cmdString, size, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
                             AT_CMD_MATCH_REQUEST, ZB_PROFILE_ID_HOME_AUTOMATION,
                             SEPARATOR, IN_CLUSTER_COUNT_STRING,
                             SEPARATOR, ZB_CLUSTER_IDENTIFY,
                             SEPARATOR, ZB_CLUSTER_ON_OFF,
                             SEPARATOR, ZB_CLUSTER_TEMPERATURE_MEASUREMENT,
                             SEPARATOR, ZB_CLUSTER_IAS_ZONE,
                             SEPARATOR, ZB_CLUSTER_COLOR_CONTROL,
                             SEPARATOR, OUT_CLUSTER_COUNT_STRING);
    if(stringRet <= 0)
    {
        OIC_LOG(ERROR, TAG, "Build command error.");
        ret = OC_STACK_ERROR;
        goto exit;
    }
    ret = TWIssueATCommand(ctx->g_plugin, cmdString);
    if (ret != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Write %s.", cmdString);
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Write %s.", cmdString);

exit:
    OICFree(cmdString);
    OIC_LOG_V(INFO, TAG, "Leave FindMatchNodes() with ret=%d", ret);
    return ret;
}

TWResultCode FindClusters(char nodeId[], char endpoint[], TWContext* ctx)
{
    /*
    //AT+SIMPLEDESC:3746,3746,01
    //      SEQ:97
    //      OK
    //
    //      SimpleDesc:3746,00
    //      EP:01
    //      ProfileID:0104
    //      DeviceID:0402v00
    //      InCluster:0000,0001,0003,0402,0500,0020,0B05
    //      OutCluster:0019
    //
    //      ACK:97
    */

    OIC_LOG(INFO, TAG, "Enter FindClusters()");

    TWResultCode ret = TW_RESULT_UNKNOWN;

    int size = strlen(AT_CMD_SIMPLE_DESC) + strlen(nodeId) +
               SEPARATOR_LENGTH + strlen(nodeId) +
               SEPARATOR_LENGTH + strlen(endpoint) + 1;

    TWEntry* entry = NULL;

    char* cmdString = (char*)OICMalloc(size * sizeof(char));
    if (cmdString == NULL)
    {
        OIC_LOG(ERROR, TAG, "No Memory");
        ret = TW_RESULT_ERROR_NO_MEMORY;
        goto exit;
    }

    int stringRet = snprintf(cmdString, size, "%s%s%s%s%s%s",
                             AT_CMD_SIMPLE_DESC, nodeId,
                             SEPARATOR, nodeId,
                             SEPARATOR, endpoint);
    if(stringRet <= 0)
    {
        OIC_LOG(ERROR, TAG, "Build command error.");
        ret = OC_STACK_ERROR;
        goto exit;
    }
    ret = TWIssueATCommand(ctx->g_plugin, cmdString);
    if (ret != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Write %s", cmdString);
        ret = TW_RESULT_ERROR;
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Write %s", cmdString);

    ret = TWDequeueEntry(ctx->g_plugin, &entry, TW_SIMPLEDESC);
    if (ret != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "TWDequeueEntry - %s", cmdString);
        goto exit;
    }
    if (entry == NULL)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry is NULL - %s", cmdString);
        ret = TW_RESULT_ERROR;
        goto exit;
    }

    ret = processEntry(entry, ctx);
    if (ret == TW_RESULT_OK)
    {
        OIC_LOG_V(INFO, TAG, "processEntry - %s", cmdString);
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "processEntry - %s", cmdString);
    }

exit:
    if (entry != NULL)
    {
        TWDeleteEntry(ctx->g_plugin, entry);
    }
    OICFree(cmdString);
    OIC_LOG_V(INFO, TAG, "Leave FindClusters() with ret=%d", ret);
    return ret;
}

TWResultCode GetRemoteEUI(char *nodeId, char* outRemoteEUI, TWContext* ctx)
{
    //AT+EUIREQ:< Address>,<NodeID>[,XX]
    //  AddrResp:<errorcode>[,<NodeID>,<EUI64>]
    //  AddrResp:00,15ED,000D6F00040574B8

    OIC_LOG(INFO, TAG, "Enter GetRemoteEUI()");
    TWResultCode ret = TW_RESULT_UNKNOWN;

    int size = strlen(AT_CMD_REMOTE_EUI_REQUEST) + strlen(nodeId) +
               SEPARATOR_LENGTH + strlen(nodeId) + 1;
    TWEntry* entry = NULL;

    char* cmdString = (char*)OICMalloc(size * sizeof(char));
    if (cmdString == NULL)
    {
        OIC_LOG(ERROR, TAG, "No Memory");
        ret = TW_RESULT_ERROR_NO_MEMORY;
        goto exit;
    }

    int stringRet = snprintf(cmdString, size, "%s%s%s%s",
                         AT_CMD_REMOTE_EUI_REQUEST, nodeId,
                         SEPARATOR, nodeId);
    if(stringRet <= 0)
    {
        OIC_LOG(ERROR, TAG, "Build command error.");
        ret = OC_STACK_ERROR;
        goto exit;
    }
    ret = TWIssueATCommand(ctx->g_plugin, cmdString);
    if (ret != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "Write %s", cmdString);
        ret = TW_RESULT_ERROR;
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Write %s", cmdString);

    ret = TWDequeueEntry(ctx->g_plugin, &entry, TW_ADDRESS_RESPONSE);
    if (ret != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "TWDequeueEntry - %s", cmdString);
        ret = TW_RESULT_ERROR;
        goto exit;
    }
    if (entry == NULL)
    {
        OIC_LOG_V(ERROR, TAG, "TWEntry is NULL - %s", cmdString);
        ret = TW_RESULT_ERROR;
        goto exit;
    }

    ret = processEntry(entry, ctx);
    if (ret != TW_RESULT_OK)
    {
        OIC_LOG_V(ERROR, TAG, "processEntry - %s", cmdString);
        ret = TW_RESULT_ERROR;
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "Wanted   eui of NodeID=%s ", nodeId);
    OIC_LOG_V(INFO, TAG, "Received eui of g_WIPRemoteNodeId=%s ", ctx->g_WIPRemoteNodeId);
    if (strcmp(nodeId, ctx->g_WIPRemoteNodeId) != 0)
    {
        OIC_LOG(ERROR, TAG, "Received eui for an unexpected remote node id.");
        ret = TW_RESULT_ERROR;
        goto exit;
    }

    OIC_LOG_V(INFO, TAG, "Remote NodeId:%s has EUI: %s \n",
                        ctx->g_WIPRemoteNodeId, ctx->g_WIPRemoteEUI);
    OICStrcpy(outRemoteEUI, sizeof(ctx->g_WIPRemoteEUI), ctx->g_WIPRemoteEUI);

    ret = TW_RESULT_OK;

exit:
    memset(ctx->g_WIPRemoteEUI, '\0', sizeof(ctx->g_WIPRemoteEUI));
    memset(ctx->g_WIPRemoteNodeId, '\0', sizeof(ctx->g_WIPRemoteNodeId));
    if (entry != NULL)
    {
        TWDeleteEntry(ctx->g_plugin, entry);
    }
    OICFree(cmdString);
    OIC_LOG_V(INFO, TAG, "Leave GetRemoteEUI() with ret=%d", ret);
    return ret;
}

TWResultCode HandleATResponse(TWEntry* entry, TWContext* ctx)
{
    OIC_LOG(INFO, TAG, "Enter HandleATResponse()");

    TWResultCode ret = TW_RESULT_ERROR;

    int32_t i = 0;
    for (; i < entry->count; i++)
    {
        uint32_t k = 0;
        for (; k < sizeof(g_TWATResultHandlerPairArray)/sizeof(TWATResultHandlerPair); ++k)
        {
            const char* line = (entry)->lines[i].line;
            if (strncmp(line,
                        g_TWATResultHandlerPairArray[k].resultTxt,
                        strlen(g_TWATResultHandlerPairArray[k].resultTxt)
                        ) == 0)
            {
                char* tokens[ARRAY_LENGTH] = {};
                const char* delimiters = ",\r\n";
                int paramCount = Tokenize((entry)->lines[i].line +
                                           strlen(g_TWATResultHandlerPairArray[k].resultTxt),
                                           delimiters, tokens);
                if (paramCount > 0)
                {
                    ret = g_TWATResultHandlerPairArray[k].handler(paramCount, tokens, ctx);
                }

                int n = 0;
                for (; n < paramCount; n++)
                {
                    OICFree(tokens[n]);
                }

                break;
            }
        }
    }

    OIC_LOG_V(INFO, TAG, "Leave HandleATResponse() with ret=%d", ret);
    return ret;
}

//-----------------------------------------------------------------------------
// Internal functions - AT Response/Prompt Handlers
//-----------------------------------------------------------------------------

TWResultCode TelAddressResponseHandler(int count, char* tokens[], TWContext* ctx)
{
    //AT+EUIREQ:< Address>,<NodeID>[,XX]
    //  AddrResp:<errorcode>[,<NodeID>,<EUI64>]
    //  AddrResp:00,15ED,000D6F00040574B8

    OIC_LOG(INFO, TAG, "Enter TelAddressResponseHandler()");

    TWResultCode ret = TW_RESULT_UNKNOWN;

    if(!tokens || count != RESPONSE_PARAMS_COUNT_ADDRESS_RESPONSE)
    {
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
    }
    else
    {
        if (strcmp(tokens[TOKEN_ADDRRESP_STATUS_CODE], AT_STR_ERROR_OK) != 0)
        {
            OIC_LOG(ERROR, TAG, "AddrResp prompt contained error status.");
            ret = TW_RESULT_ERROR;
        }
        else
        {
            OICStrcpy(ctx->g_WIPRemoteNodeId, SIZE_NODEID, tokens[TOKEN_ADDRRESP_NODEID]);
            OICStrcpy(ctx->g_WIPRemoteEUI, SIZE_EUI, tokens[TOKEN_ADDRRESP_EUI]);
            OIC_LOG_V(INFO, TAG, "Received eui %s for g_WIPRemoteNodeId=%s ",
                     ctx->g_WIPRemoteEUI,
                     ctx->g_WIPRemoteNodeId);
            ret = TW_RESULT_OK;
        }
    }

    OIC_LOG_V(INFO, TAG, "Leave TelAddressResponseHandler() with ret=%d", ret);
    return ret;
}

TWResultCode TelNetworkInfoHandler(int count, char* tokens[], TWContext* ctx)
{
    // Ask:         AT+N
    // Response:    +N=COO,24,-6,9726,12BB200F073AB573
    //                          or
    //              +N=NoPAN
    //
    //              +N=<devicetype>,<channel>,<power>,<PANID>,<EPANID>

    OIC_LOG(INFO, TAG, "Enter TelNetworkInfoHandler()");

    TWResultCode ret = TW_RESULT_UNKNOWN;

    if(!tokens ||
       ((count != RESPONSE_PARAMS_COUNT_NETWORK_INFO_1) &&
        (count != RESPONSE_PARAMS_COUNT_NETWORK_INFO_5)))
    {
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    int i = 0;
    for (; i < count; ++i)
    {
        if (tokens[i] != NULL)
        {
            OIC_LOG_V(INFO, TAG, "Token[%d] = %s", i, tokens[i]);
        }
    }

    char* temp = tokens[TOKEN_PLUS_N_DEVICE_TYPE];
    if (strcmp(temp, "NoPAN") == 0)
    {
        OIC_LOG(INFO, TAG, "It is NoPan.");
        ret = TW_RESULT_NO_LOCAL_PAN;
        goto exit;
    }

    OIC_LOG(INFO, TAG, "Already have an established network.");
    ret = AsciiHexToValue(tokens[TOKEN_PLUS_N_PANID],
                          strlen(tokens[TOKEN_PLUS_N_PANID]),
                          &(ctx->g_ZigBeeStatus.panId));
    if(ret != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "AsciiHexToValue - panId");
        goto exit;
    }

    ret = AsciiHexToValue(tokens[TOKEN_PLUS_N_PANID_EXTENDED],
                          strlen(tokens[TOKEN_PLUS_N_PANID_EXTENDED]),
                          &(ctx->g_ZigBeeStatus.extPanId));
    if(ret != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "AsciiHexToValue - extPanId");
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "PanId=%" PRId64 , ctx->g_ZigBeeStatus.panId);
    OIC_LOG_V(INFO, TAG, "ExtPanId=%" PRId64 , ctx->g_ZigBeeStatus.extPanId);
    OIC_LOG_V(INFO, TAG, "PanId=%s", tokens[TOKEN_PLUS_N_PANID]);
    OIC_LOG_V(INFO, TAG, "ExtPanId=%s", tokens[TOKEN_PLUS_N_PANID_EXTENDED]);

    OIC_LOG_V(INFO, TAG, "TelNetworkInfoHandler set ExtPanId to %08X%08X",
             (unsigned int)(ctx->g_ZigBeeStatus.extPanId >> 32),
             (unsigned int)(ctx->g_ZigBeeStatus.extPanId & 0xFFFFFFFF));

    ret = TW_RESULT_HAS_LOCAL_PAN;

exit:
    OIC_LOG_V(INFO, TAG, "Leave TelNetworkInfoHandler() with ret=%d", ret);
    return ret;
}

TWResultCode TelJpanHandler(int count, char* tokens[], TWContext* ctx)
{
    //Ask:        AT+EN:[<channel>],[<POWER>],[<PANID>]
    //Response:   JPAN:<channel>,<PANID>,<EPANID>

    OIC_LOG(INFO, TAG, "Enter TelJpanHandler()");

    TWResultCode ret = TW_RESULT_UNKNOWN;

    if(!tokens || count != RESPONSE_PARAMS_COUNT_JPAN)
    {
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    ret = AsciiHexToValue(tokens[TOKEN_JPAN_PANID],
                          strlen(tokens[TOKEN_JPAN_PANID]),
                          &(ctx->g_ZigBeeStatus.panId));

    if(ret != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "AsciiHexToValue - panId");
        goto exit;
    }

    ret = AsciiHexToValue(tokens[TOKEN_JPAN_PANID_EXTENDED],
                          strlen(tokens[TOKEN_JPAN_PANID_EXTENDED]),
                          &(ctx->g_ZigBeeStatus.extPanId));
    if(ret != TW_RESULT_OK)
    {
        OIC_LOG(ERROR, TAG, "AsciiHexToValue - extPanId");
        goto exit;
    }
    OIC_LOG_V(INFO, TAG, "PanId = %" PRId64 "\n", ctx->g_ZigBeeStatus.panId);
    OIC_LOG_V(INFO, TAG, "ExtPanId = %" PRId64 "\n", ctx->g_ZigBeeStatus.extPanId);
    ret = TW_RESULT_NEW_LOCAL_PAN_ESTABLISHED;

exit:
    OIC_LOG_V(INFO, TAG, "Leave TelJpanHandler() with ret=%d", ret);
    return ret;
}

TWResultCode TelEndDeviceJoinHandler(int count, char* tokens[], TWContext* ctx)
{
    //Ask:      AT+PJOIN
    //
    //Prompt:   RFD:<IEEE Address>,<NodeID>
    //Prompt:   FFD:<IEEE Address>,<NodeID>
    //Prompt:   SED:<IEEE Address>,<NodeID>
    //Prompt:   ZED:<IEEE Address>,<NodeID>

    OIC_LOG(INFO, TAG, "Enter TelEndDeviceJoinHandler()");
    TWResultCode ret = TW_RESULT_UNKNOWN;

    if(!tokens || count != RESPONSE_PARAMS_COUNT_DEVICE_JOINED)
    {
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    //TODO: Might need to add into the list if needed - log it for now.
    OIC_LOG_V(INFO, TAG, "Received RFD/FFD/SED/ZED - EUI:%s; NodeID:%s.\n",
            tokens[TOKEN_PJOIN_RESPONSE_IEEE_ADDRESS],
            tokens[TOKEN_PJOIN_RESPONSE_NODEID]);

    if (ctx->g_EndDeviceNodeIdChangedCallback != NULL)
    {
        ctx->g_EndDeviceNodeIdChangedCallback(tokens[TOKEN_PJOIN_RESPONSE_IEEE_ADDRESS],
                                              tokens[TOKEN_PJOIN_RESPONSE_NODEID],
                                              ctx->g_plugin);
    }

    ret = TW_RESULT_OK;

exit:
    OIC_LOG_V(INFO, TAG, "Leave TelEndDeviceJoinHandler() with ret=%d", ret);
    return ret;
}

TWResultCode TelMatchDescHandler(int count, char* tokens[], TWContext* ctx)
{
    //Prompt:       MatchDesc:0B4A,00,01

    OIC_LOG(INFO, TAG, "Enter TelMatchDescHandler()");
    TWResultCode ret = TW_RESULT_ERROR;

    if(!tokens || count != RESPONSE_PARAMS_COUNT_MATCH_DESC)
    {
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    if (strcmp(tokens[TOKEN_MATCHDESC_STATUS_CODE], AT_STR_ERROR_OK) != 0)
    {
        OIC_LOG(INFO, TAG, "MatchDesc prompt contained error status.");
        ret = TW_RESULT_ERROR;
        goto exit;
    }
    else
    {
        char remoteEUI[SIZE_EUI];
        ret = GetRemoteEUI(tokens[TOKEN_MATCHDESC_NODEID], remoteEUI, ctx);
        if (ret != TW_RESULT_OK)
        {
            OIC_LOG(ERROR, TAG, "GetRemoteEUI()");
            goto exit;
        }
        else
        {
            //Step 1: Create TWDevice
            TWDevice* device = (TWDevice*)OICCalloc(1, sizeof(TWDevice));
            if (device == NULL)
            {
                ret = TW_RESULT_ERROR_NO_MEMORY;
                goto exit;
            }
            else
            {
                device->endpointOfInterest = (TWEndpoint*)OICCalloc(1, sizeof(TWEndpoint));
                if (device->endpointOfInterest == NULL)
                {
                    OICFree(device);
                    ret = TW_RESULT_ERROR_NO_MEMORY;
                }
                else
                {
                    OICStrcpy(device->eui, SIZE_EUI, remoteEUI);
                    OICStrcpy(device->nodeId, SIZE_NODEID, tokens[TOKEN_MATCHDESC_NODEID]);
                    OICStrcpy(device->endpointOfInterest->endpointId,
                              SIZE_ENDPOINTID,
                              tokens[TOKEN_MATCHDESC_ENDPOINTID]);
                    ctx->g_WIPDevice = device;

                    //Step 2: Add to list
                    if (ctx->g_FoundMatchedDeviceList == NULL)
                    {
                        //Create a list of promptCount entries
                        ctx->g_FoundMatchedDeviceList =
                                (TWDeviceList*)OICMalloc(sizeof(TWDeviceList));
                        if (ctx->g_FoundMatchedDeviceList == NULL)
                        {
                            OICFree(device->endpointOfInterest);
                            OICFree(device);
                            ret = TW_RESULT_ERROR_NO_MEMORY;
                        }
                        else
                        {
                            ctx->g_FoundMatchedDeviceList->count = 1;
                            ctx->g_FoundMatchedDeviceList->deviceList =
                                    (TWDevice*)OICMalloc(sizeof(TWDevice));
                            if (ctx->g_FoundMatchedDeviceList->deviceList == NULL)
                            {
                                OICFree(device->endpointOfInterest);
                                OICFree(device);
                                ret = TW_RESULT_ERROR_NO_MEMORY;
                            }
                            else
                            {
                                memcpy(ctx->g_FoundMatchedDeviceList->deviceList,
                                       device,
                                       sizeof(TWDevice));
                                ret = TW_RESULT_OK;
                            }
                        }
                    }
                    else
                    {
                        //Expand the list
                        int newSize = sizeof(TWDevice)*(ctx->g_FoundMatchedDeviceList->count + 1);
                        TWDevice* temp =
                                (TWDevice*)realloc(ctx->g_FoundMatchedDeviceList->deviceList,
                                                   newSize);
                        if (temp == NULL)
                        {
                            OICFree(device->endpointOfInterest);
                            OICFree(device);
                            ret =TW_RESULT_ERROR_NO_MEMORY;
                        }
                        else
                        {
                            ctx->g_FoundMatchedDeviceList->deviceList = temp;

                            //Add to the end of list
                            int count = ctx->g_FoundMatchedDeviceList->count;
                            memcpy(&(ctx->g_FoundMatchedDeviceList->deviceList[count]),
                                   device,
                                   sizeof(TWDevice));

                            //Increase the count
                            ctx->g_FoundMatchedDeviceList->count++;

                            ret = TW_RESULT_OK;
                        }
                    }
                }
            }
        }
    }

exit:
    OIC_LOG_V(INFO, TAG, "Leave TelMatchDescHandler() with ret=%d", ret);
    return ret;
}

TWResultCode TelSimpleDescHandler(int count, char* tokens[], TWContext* ctx)
{
    //AT+SIMPLEDESC:3746,3746,01
    //      SEQ:97
    //      OK
    //
    //      SimpleDesc:3746,00              <<<<<<<---------------------
    //      EP:01
    //      ProfileID:0104
    //      DeviceID:0402v00
    //      InCluster:0000,0001,0003,0402,0500,0020,0B05
    //      OutCluster:0019
    //      ACK:97

    OIC_LOG(INFO, TAG, "Enter TelSimpleDescHandler().");
    TWResultCode ret = TW_RESULT_UNKNOWN;

    if(!tokens || count != RESPONSE_PARAMS_COUNT_SIMPLE_DESC)
    {
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    if (ctx->g_WIPDevice == NULL)
    {
        OIC_LOG_V(ERROR, TAG,
                 "Receive simple descriptor unexpectedly - %s",
                 tokens[TOKEN_SIMPLEDESC_SIMPLEDESC_NODEID]);
        ret = TW_RESULT_ERROR;
        goto exit;
    }

    if (strcmp(tokens[TOKEN_SIMPLEDESC_SIMPLEDESC_STATUS_CODE], AT_STR_ERROR_OK) != 0)
    {
        OIC_LOG_V(ERROR,
                 TAG,
                 "SimpleDesc: prompt contained error status %s.",
                 tokens[TOKEN_SIMPLEDESC_SIMPLEDESC_STATUS_CODE]);
        ret = TW_RESULT_ERROR;
    }
    else
    {
        if (strcmp(tokens[TOKEN_SIMPLEDESC_SIMPLEDESC_NODEID],
                   ctx->g_WIPDevice->nodeId) == 0)
        {
            OIC_LOG_V(INFO,
                     TAG,
                     "Got simple descriptor for nodeid %s",
                     tokens[TOKEN_SIMPLEDESC_SIMPLEDESC_NODEID]);
            ret = TW_RESULT_OK;
        }
        else
        {
            OIC_LOG_V(ERROR,
                     TAG,
                     "Finding simple descriptor for non existing nodeid %s.",
                     tokens[TOKEN_SIMPLEDESC_SIMPLEDESC_NODEID]);
            ret = TW_RESULT_ERROR;
        }
    }

exit:
    OIC_LOG_V(INFO, TAG, "Leave TelSimpleDescHandler() with ret=%d", ret);
    return ret;
}

TWResultCode TelSimpleDescInClusterHandler(int count, char* tokens[], TWContext* ctx)
{
    //AT+SIMPLEDESC:3746,3746,01
    //      SEQ:97
    //      OK
    //
    //      SimpleDesc:3746,00
    //      EP:01
    //      ProfileID:0104
    //      DeviceID:0402v00
    //      InCluster:0000,0001,0003,0402,0500,0020,0B05        <<<<<<<<--------------
    //      OutCluster:0019
    //      ACK:97

    OIC_LOG(INFO, TAG, "Enter TelSimpleDescInClusterHandler()");
    TWResultCode ret = TW_RESULT_ERROR;

    if (!tokens || count < RESPONSE_PARAMS_COUNT_SIMPLE_DESC_IN_CLUSTER_MIN )
    {
        OIC_LOG(ERROR, TAG, "Invalid Params");
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    if (ctx->g_WIPDevice == NULL)
    {
        OIC_LOG_V(ERROR, TAG,
                 "Receive simple descriptor unexpectedly - %s",
                 tokens[TOKEN_SIMPLEDESC_INCLUSTER_STRING]);
        ret = TW_RESULT_ERROR;
        goto exit;
    }

    if (ctx->g_WIPDevice->endpointOfInterest->clusterList != NULL)
    {
        OIC_LOG(ERROR, TAG, "Expected an empty cluster list.");
        ret = TW_RESULT_ERROR;
        goto exit;
    }

    //Add found clusters for the node.
    ctx->g_WIPDevice->endpointOfInterest->clusterList =
            (TWClusterList*)OICMalloc(sizeof(TWClusterList));
    if (ctx->g_WIPDevice->endpointOfInterest->clusterList == NULL)
    {
        OIC_LOG(ERROR, TAG, "No Memory - clusterList");
        ret = TW_RESULT_ERROR_NO_MEMORY;
        goto exit;
    }

    ctx->g_WIPDevice->endpointOfInterest->clusterList->clusterIds =
            (TWClusterId*)OICMalloc(sizeof(TWClusterId) * count);
    if (ctx->g_WIPDevice->endpointOfInterest->clusterList->clusterIds == NULL)
    {
        OICFree(ctx->g_WIPDevice->endpointOfInterest->clusterList);
        OIC_LOG(ERROR, TAG, "No Memory - clusterIds");
        ret = TW_RESULT_ERROR_NO_MEMORY;
        goto exit;
    }

    int i = 0;
    for (; i < count; i++)
    {
        OICStrcpy(ctx->g_WIPDevice->endpointOfInterest->clusterList->clusterIds[i].clusterId,
                  SIZE_CLUSTERID,
                  tokens[i]);

        OIC_LOG_V(INFO, TAG, "ClusterIds[%d]=%s",
                 i,
                 ctx->g_WIPDevice->endpointOfInterest->clusterList->clusterIds[i].clusterId);
    }
    ctx->g_WIPDevice->endpointOfInterest->clusterList->count = count;
    ret = TW_RESULT_HAS_CLUSTERS;

exit:
    OIC_LOG_V(INFO, TAG, "Leave TelSimpleDescInClusterHandler() with ret=%d", ret);
    return ret;
}

TWResultCode TelWriteAttrHandler(int count, char* tokens[], TWContext* ctx)
{
    //AT+WRITEATR:3A3D,01,0,0003,0000,21,00
    //      OK
    //      WRITEATTR:3A3D,01,0003,,00
    //
    //AT+WRITEATR:B826,01,0,0500,0010,F0,000D6F0000D59E92
    //      OK
    //      WRITEATTR:B826,01,0500,0010,70

    OIC_LOG(INFO, TAG, "Enter TelWriteAttrHandler()");
    (void)ctx;

    TWResultCode ret = TW_RESULT_ERROR;

    if(!tokens ||
       (count < RESPONSE_PARAMS_COUNT_WRITE_ATTR_4) ||
       (count > RESPONSE_PARAMS_COUNT_WRITE_ATTR_5))
    {
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    if (count == RESPONSE_PARAMS_COUNT_WRITE_ATTR_4)
    {
        if (strcmp(tokens[TOKEN_WRITEATTR_STATUS_CODE], AT_STR_ERROR_OK) == 0)
        {
            ret = TW_RESULT_OK;
        }
        else if (strcmp(tokens[TOKEN_WRITEATTR_STATUS_CODE], AT_STR_ERROR_INVALID_OP) == 0)
        {
            ret = TW_RESULT_ERROR_INVALID_OP;
        }
    }
    else if (count == RESPONSE_PARAMS_COUNT_WRITE_ATTR_5)
    {
        if (strcmp(tokens[TOKEN_WRITEATTR_STATUS_CODE_ALTERNATIVE], AT_STR_ERROR_OK) == 0)
        {
            ret = TW_RESULT_OK;
        }
        else if (strcmp(tokens[TOKEN_WRITEATTR_STATUS_CODE_ALTERNATIVE],
                        AT_STR_ERROR_INVALID_OP) == 0)
        {
            ret = TW_RESULT_ERROR_INVALID_OP;
        }
    }

exit:
    OIC_LOG_V(INFO, TAG, "Leave TelWriteAttrHandler() with ret=%d", ret);
    return ret;
}

TWResultCode TelReadAttrHandlerTemperature(int count, char* tokens[], TWContext* ctx)
{
    //AT+READATR:F2D7,01,0,0402,0002
    //      OK
    //      TEMPERATURE:F2D7,01,0002,00,1770
    //
    //AT+READATR:F2D7,01,0,0402,0002
    //      OK
    //      ERROR:66

    OIC_LOG(INFO, TAG, "Enter TelReadAttrHandlerTemperature().");
    TWResultCode ret = TW_RESULT_UNKNOWN;

    if(!tokens || count != RESPONSE_PARAMS_COUNT_TEMPERATURE)
    {
        OIC_LOG(ERROR, TAG, "Invalid Params");
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    if (strcmp(tokens[TOKEN_TEMPERATURE_STATUS_CODE], AT_STR_ERROR_OK) != 0)
    {
        OIC_LOG(ERROR, TAG, "TEMPERATURE prompt contained error status.");
        ret = TW_RESULT_ERROR;
        goto exit;
    }

    // AttrInfo is 16-bit value representing (100 * Degrees Celsius)
    // so 0x812 = 20.66 C = 69.188 F
    if (ctx->g_ZigBeeStatus.remoteAttributeValueRead != NULL)
    {
        OICFree(ctx->g_ZigBeeStatus.remoteAttributeValueRead);
        ctx->g_ZigBeeStatus.remoteAttributeValueRead = NULL;
    }
    OIC_LOG_V(INFO, TAG, "Read Attribute Value: %s", tokens[TOKEN_TEMPERATURE_VALUE]);
    ctx->g_ZigBeeStatus.remoteAttributeValueRead =
            (char*)OICMalloc(sizeof(char) * strlen(tokens[TOKEN_TEMPERATURE_VALUE]));
    if (ctx->g_ZigBeeStatus.remoteAttributeValueRead == NULL)
    {
        OIC_LOG_V(ERROR, TAG, "No Memory");
        ret = TW_RESULT_ERROR_NO_MEMORY;
    }
    else
    {
        strcpy(ctx->g_ZigBeeStatus.remoteAttributeValueRead,
               tokens[TOKEN_TEMPERATURE_VALUE]);
        ctx->g_ZigBeeStatus.remoteAtrributeValueReadLength =
                     strlen(tokens[TOKEN_TEMPERATURE_VALUE]);
        ret = TW_RESULT_REMOTE_ATTR_HAS_VALUE;
    }

exit:
    OIC_LOG_V(INFO, TAG, "Leave TelReadAttrHandlerTemperature() with ret=%d", ret);
    return ret;
}

TWResultCode TelReadAttrHandler(int count, char* tokens[], TWContext* ctx)
{
    //AT+READATR:F2D7,01,0,0402,0002
    //      OK
    //      RESPATTR:<NodeID>,<EP>,<ClusterID>,<AttrID>,<Status>,<AttrInfo>
    //
    //AT+READATR:F2D7,01,0,0402,0002
    //      OK
    //      ERROR:66

    OIC_LOG(INFO, TAG, "Enter TelReadAttrHandler()");
    TWResultCode ret = TW_RESULT_UNKNOWN;

    if(!tokens || count != RESPONSE_PARAMS_COUNT_RESPATTR)
    {
        OIC_LOG(ERROR, TAG, "Invalid Params");
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    if (strcmp(tokens[TOKEN_RESPATTR_STATUS_CODE], AT_STR_ERROR_OK) != 0)
    {
        OIC_LOG(INFO, TAG, "READATTR prompt contained error status.");
        ret = TW_RESULT_ERROR;
        goto exit;
    }

    if (ctx->g_ZigBeeStatus.remoteAttributeValueRead != NULL)
    {
        OICFree(ctx->g_ZigBeeStatus.remoteAttributeValueRead);
    }
    OIC_LOG_V(INFO, TAG, "Read Attribute Value: %s.", tokens[TOKEN_RESPATTR_ATTRIBUTE_VALUE]);
    ctx->g_ZigBeeStatus.remoteAttributeValueRead =
            (char*)OICMalloc(sizeof(char) * strlen(tokens[TOKEN_RESPATTR_ATTRIBUTE_VALUE]));
    if (ctx->g_ZigBeeStatus.remoteAttributeValueRead != NULL)
    {
        strcpy(ctx->g_ZigBeeStatus.remoteAttributeValueRead,
               tokens[TOKEN_RESPATTR_ATTRIBUTE_VALUE]);
        ctx->g_ZigBeeStatus.remoteAtrributeValueReadLength =
                     strlen(tokens[TOKEN_RESPATTR_ATTRIBUTE_VALUE]);
        ret = TW_RESULT_REMOTE_ATTR_HAS_VALUE;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "No Memory");
        ret = TW_RESULT_ERROR_NO_MEMORY;
    }

exit:
    OIC_LOG(INFO, TAG, "Leave TelReadAttrHandler()");
    return ret;
}

TWResultCode TelZCLDefaultResponseHandler(int count, char* tokens[], TWContext* ctx)
{
    //AT+RONOFF:<Address>,<EP>,<SendMode>[,<ON/OFF>]
    //      DFTREP:<NodeID>,<EP>,<ClusterID>,<CMD>,<Status>
    //
    //AT+DRLOCK:<Address>,<EP>,<SendMode>,<Lock/Unlock>
    //      DFTREP:<NodeID>,<EP>,<ClusterID>,<CMD>,<Status>
    //
    //AT+LCMVTOLEV:<Address>,<EP>,<SendMode>,<ON/OFF>,<LevelValue>,<TransTime>
    //      DFTREP:<NodeID>,<EP>,<ClusterID>,<CMD>,<Status>

    OIC_LOG(INFO, TAG, "Enter TelZCLDefaultResponseHandler()");
    (void)ctx;
    TWResultCode ret = TW_RESULT_UNKNOWN;

    if(!tokens || count != RESPONSE_PARAMS_COUNT_DFTREP)
    {
        OIC_LOG(ERROR, TAG, "Invalid Params");
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    OIC_LOG_V(INFO, TAG,
             "DFTREP prompt succeed for NodeId:%s, EP:%s, ClusterId:%s, CMD:%s.\n",
             tokens[TOKEN_DFTREP_NODEID],
             tokens[TOKEN_DFTREP_ENDPOINTID],
             tokens[TOKEN_DFTREP_CLUSTERID],
             tokens[TOKEN_DFTREP_COMMANDID]);

    if (strcmp(tokens[TOKEN_DFTREP_STATUS_CODE], AT_STR_ERROR_OK) != 0)
    {
        ret = TW_RESULT_ERROR;
    }
    else
    {
        ret = TW_RESULT_OK;
    }

exit:
    OIC_LOG(INFO, TAG, "Leave TelZCLDefaultResponseHandler()");
    return ret;
}

TWResultCode TelSwitchDoorLockStateHandler(int count, char* tokens[], TWContext* ctx)
{
    //AT+DRLOCK:<Address>,<EP>,<SendMode>,<Lock/Unlock>
    //      DRLOCRSP:<nodeID>,<ep>,<status>
    //      or
    //      DRUNLOCKRSP:<nodeID>,<ep>,<status>

    OIC_LOG(INFO, TAG, "Enter TelSwitchDoorLockStateHandler()");
    (void)ctx;
    TWResultCode ret = TW_RESULT_UNKNOWN;

    if(!tokens || count != RESPONSE_PARAMS_COUNT_DRLOCKUNLOCKRSP)
    {
        OIC_LOG(ERROR, TAG, "Invalid Params");
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    if (strcmp(tokens[TOKEN_DRLOCKRSP_STATUS_CODE], AT_STR_ERROR_OK) != 0)
    {
        OIC_LOG_V(INFO,
                 TAG,
                 "DRLOCRSP/DRUNLOCKRSP prompt contained error status %s.",
                 tokens[TOKEN_DRLOCKRSP_STATUS_CODE]);
        ret = TW_RESULT_ERROR;
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "DRLOCRSP/DRUNLOCKRSP prompt succeed for nodeId:%s, ep:%s.",
                 tokens[TOKEN_DRLOCKRSP_NODEID],
                 tokens[TOKEN_DRLOCKRSP_ENDPOINTID]);
        ret = TW_RESULT_OK;
    }

exit:
    OIC_LOG_V(INFO, TAG, "Leave TelSwitchDoorLockStateHandler() with ret=%d", ret);
    return ret;
}

TWResultCode TelZoneEnrollRequestHandler(int count, char* tokens[], TWContext* ctx)
{
    //ZENROLLREQ:<NodeID>,<EndPoint>,<ZoneType>,<ManufactureCode>

    OIC_LOG(INFO, TAG, "Enter TelZoneEnrollRequestHandler()");
    (void)ctx;
    TWResultCode ret = TW_RESULT_UNKNOWN;

    if(!tokens || count != RESPONSE_PARAMS_COUNT_ZENROLLREQ)
    {
        OIC_LOG(ERROR, TAG, "Invalid Params");
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    OIC_LOG(INFO, TAG, "Received zone request from:");
    OIC_LOG_V(INFO, TAG, "Node:%s", tokens[TOKEN_ZENROLLREQ_NODEID]);
    OIC_LOG_V(INFO, TAG, "EP:%s", tokens[TOKEN_ZENROLLREQ_ENDPOINTID]);
    OIC_LOG_V(INFO, TAG, "ZoneType:%s", tokens[TOKEN_ZENROLLREQ_ZONETYPE]);
    OIC_LOG_V(INFO, TAG, "ManufactureCode:%s", tokens[TOKEN_ZENROLLREQ_MANUFACTURE_CODE]);
    ret = TW_RESULT_OK;

exit:
    OIC_LOG_V(INFO, TAG, "Leave TelZoneEnrollRequestHandler() with ret=%d", ret);
    return ret;
}

TWResultCode TelEnrolledHandler(int count, char* tokens[], TWContext* ctx)
{
    //ENROLLED:<ZID>,<ZoneType>,<EUI>

    OIC_LOG(INFO, TAG, "Enter TelEnrolledHandler()");
    TWResultCode ret = TW_RESULT_OK;

    if(!tokens || count != RESPONSE_PARAMS_COUNT_ENROLLED)
    {
        OIC_LOG(ERROR, TAG, "Invalid Params");
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    OIC_LOG(INFO, TAG, "Received zone enrollment for:");
    OIC_LOG_V(INFO, TAG, "ZID:%s", tokens[TOKEN_ENROLLED_ZONEID]);
    OIC_LOG_V(INFO, TAG, "ZoneType:%s", tokens[TOKEN_ENROLLED_ZONETYPE]);
    OIC_LOG_V(INFO, TAG, "EUI:%s", tokens[TOKEN_ENROLLED_EUI]);

    TWEnrollee enrollee;
    OICStrcpy(enrollee.zoneId, SIZE_ZONEID, tokens[TOKEN_ENROLLED_ZONEID]);
    OICStrcpy(enrollee.zoneType, SIZE_ZONETYPE, tokens[TOKEN_ENROLLED_ZONETYPE]);
    OICStrcpy(enrollee.eui, SIZE_EUI, tokens[TOKEN_ENROLLED_EUI]);

    if (ctx->g_EnrollmentSucceedCallback != NULL)
    {
        OIC_LOG_V(INFO, TAG, "Enrolled - Invoke callback");
        ctx->g_EnrollmentSucceedCallback(&enrollee, ctx->g_plugin);
    }
    ret = TW_RESULT_OK;

exit:
    OIC_LOG_V(INFO, TAG, "Leave TelEnrolledHandler() with ret=%d", ret);
    return ret;
}

TWResultCode TelZoneStatusHandler(int count, char* tokens[], TWContext* ctx)
{
    //ZONESTATUS:<NodeID>,<EP>,<ZoneStatus>,<ExtendStatus>[,<ZoneID>,<Delay>]
    //ZONESTATUS:5FBA,01,0021,00,01,00AF

    OIC_LOG(INFO, TAG, "Enter TelZoneStatusHandler()");
    TWResultCode ret = TW_RESULT_UNKNOWN;
    if(!tokens ||
       ((count != RESPONSE_PARAMS_COUNT_ZONESTATUS_4) &&
        (count != RESPONSE_PARAMS_COUNT_ZONESTATUS_6)))
    {
        OIC_LOG(ERROR, TAG, "Invalid Params");
        ret = TW_RESULT_ERROR_INVALID_PARAMS;
        goto exit;
    }

    TWUpdate update;
    OICStrcpy(update.nodeId, SIZE_NODEID, tokens[TOKEN_ZONESTATUS_NODEID]);
    OICStrcpy(update.endpoint, SIZE_NODEID, tokens[TOKEN_ZONESTATUS_ENDPOINTID]);
    OICStrcpy(update.status, SIZE_NODEID, tokens[TOKEN_ZONESTATUS_ZONESTATUS]);
    OICStrcpy(update.extendedStatus, SIZE_NODEID, tokens[TOKEN_ZONESTATUS_ZONESTATUS_EXTENDED]);

    if (count == RESPONSE_PARAMS_COUNT_ZONESTATUS_6)
    {
        OICStrcpy(update.zoneId, SIZE_NODEID, tokens[TOKEN_ZONESTATUS_ZONEID]);
        OICStrcpy(update.delay, SIZE_NODEID, tokens[TOKEN_ZONESTATUS_DELAY]);
    }

    if (ctx->g_DeviceStatusUpdateCallback != NULL)
    {
        OIC_LOG(INFO, TAG, "device status update - invoke callback");
        ctx->g_DeviceStatusUpdateCallback(&update, ctx->g_plugin);
        OIC_LOG(INFO, TAG, "device status update - callback done");
    }
    ret = TW_RESULT_OK;

exit:
    OIC_LOG_V(INFO, TAG, "Leave TelZoneStatusHandler() with ret=%d", ret);
    return ret;
}

//-----------------------------------------------------------------------------
// Internal functions - Helpers
//-----------------------------------------------------------------------------

/**
 *
 * Tokenize 'input' parameter by 'delimiters' into 'output' array.
 *
 */
int Tokenize(const char *input, const char* delimiters, char* output[])
{
    OIC_LOG_V(INFO, TAG, "Enter Tokenize() - %s", input);

    if (output == NULL)
    {
        OIC_LOG(INFO, TAG, "Invalid parameter.");
        return -1;
    }

    int length = strlen(input);
    char * str = (char *) OICCalloc(1, length + 1);
    OICStrcpy(str, length+1, input);

    char* savePtr = NULL;
    char* p   = strtok_r(str, delimiters, &savePtr);
    int index = 0;
    while (p && index <= ARRAY_LENGTH)
    {
        int size = strlen(p) + 1;   //for null char
        output[index] = (char*)OICCalloc(size, sizeof(char));
        OICStrcpy(output[index], size, p);
        OIC_LOG_V(INFO, TAG, "Token[%d]=%s", index, output[index]);
        p = strtok_r (NULL, delimiters, &savePtr);
        index++;
    }

    OICFree(str);
    OIC_LOG(INFO, TAG, "Leave Tokenize()");
    return index;
}

int AsciiToHex(char c)
{
    int num = (int) c;
    if(c >= '0' && c <= '9')
    {
        return num - '0';
    }

    if(num >= 'A' && num <= 'F')
    {
        return num - 'A' + 10;
    }
    return -1;
}

TWResultCode AsciiHexToValue(char* hexString, int length, uint64_t* value)
{
    if(!hexString || !value || length < 0)
    {
        return TW_RESULT_ERROR_INVALID_PARAMS;
    }
    int retVal = AsciiToHex(hexString[0]);
    if(retVal == -1)
    {
        OIC_LOG(ERROR, TAG, "Bad conversion from ASCII To Hex.");
        return TW_RESULT_ERROR;
    }
    *value = (uint64_t)retVal;
    for (int i = 1; i < length; ++i)
    {
        if (sizeof(hexString) > (uint32_t)i)
        {
            *value <<= 4;
            retVal = AsciiToHex(hexString[i]);
            if(retVal == -1)
            {
                OIC_LOG(ERROR, TAG, "Bad conversion from ASCII To Hex.");
                return TW_RESULT_ERROR;
            }
            *value |= (uint64_t)retVal;
        }
    }
    return TW_RESULT_OK;
}

/**
 *
 * Deallocate device list.
 *
 */
void DeallocateTWDeviceList(TWContext* ctx)
{
    if (ctx->g_FoundMatchedDeviceList == NULL)
    {
        return;
    }

    if (ctx->g_FoundMatchedDeviceList->deviceList == NULL)
    {
        OICFree(ctx->g_FoundMatchedDeviceList);
        ctx->g_FoundMatchedDeviceList = NULL;
        return;
    }

    if (ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest == NULL)
    {
        OICFree(ctx->g_FoundMatchedDeviceList->deviceList);
        ctx->g_FoundMatchedDeviceList->deviceList = NULL;

        OICFree(ctx->g_FoundMatchedDeviceList);
        ctx->g_FoundMatchedDeviceList = NULL;
        return;
    }

    if (ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest->clusterList == NULL)
    {
        OICFree(ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest);
        ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest = NULL;

        OICFree(ctx->g_FoundMatchedDeviceList->deviceList);
        ctx->g_FoundMatchedDeviceList->deviceList = NULL;

        OICFree(ctx->g_FoundMatchedDeviceList);
        ctx->g_FoundMatchedDeviceList = NULL;
        return;
    }

    if (ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest->clusterList->clusterIds
            == NULL)
    {
        OICFree(ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest->clusterList);
        ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest->clusterList = NULL;

        OICFree(ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest);
        ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest = NULL;

        OICFree(ctx->g_FoundMatchedDeviceList->deviceList);
        ctx->g_FoundMatchedDeviceList->deviceList = NULL;

        OICFree(ctx->g_FoundMatchedDeviceList);
        ctx->g_FoundMatchedDeviceList = NULL;
        return;
    }

    OICFree(ctx->g_FoundMatchedDeviceList->deviceList->
            endpointOfInterest->clusterList->clusterIds);
    ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest->clusterList->clusterIds = NULL;

    OICFree(ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest->clusterList);
    ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest->clusterList = NULL;

    OICFree(ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest);
    ctx->g_FoundMatchedDeviceList->deviceList->endpointOfInterest = NULL;

    OICFree(ctx->g_FoundMatchedDeviceList->deviceList);
    ctx->g_FoundMatchedDeviceList->deviceList = NULL;

    OICFree(ctx->g_FoundMatchedDeviceList);
    ctx->g_FoundMatchedDeviceList = NULL;
}
