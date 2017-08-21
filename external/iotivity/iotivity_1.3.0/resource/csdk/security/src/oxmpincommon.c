//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include <memory.h>
#include <limits.h>

#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocrandom.h"
#include "logger.h"
#include "pinoxmcommon.h"
#include "pbkdf2.h"
#include "base64.h"
#include "securevirtualresourcetypes.h"
#include "srmresourcestrings.h"
#include "doxmresource.h"
#include "credresource.h"
#include "cainterface.h"
#include "oic_string.h"

#define TAG "OIC_PIN_OXM_COMMON"

#define NUMBER_OF_PINNUM (10)
#define NUMBER_OF_ALPHABET (26)

/**
 * Callbacks for displaying a pin.
 */
typedef struct DisplayPinCallbacks
{
    GeneratePinCallback callback;
    DisplayPinCallbackWithContext contextCallback;
    ClosePinDisplayCallback closePinDisplayCallback;
    void* context;
} DisplayPinCallbacks_t;

/**
 * Callbacks for pin input.
 */
typedef struct InputPinCallbacks
{
    InputPinCallback callback;
    InputPinCallbackWithContext contextCallback;
    void* context;
} InputPinCallbacks_t;

static DisplayPinCallbacks_t g_displayPinCallbacks = { .callback = NULL, .contextCallback = NULL, .closePinDisplayCallback = NULL, .context = NULL };
static InputPinCallbacks_t g_inputPinCallbacks = { .callback = NULL, .contextCallback = NULL, .context = NULL };

typedef struct PinOxmData {
    uint8_t pinData[OXM_RANDOM_PIN_MAX_SIZE + 1];
    size_t pinSize;
    OicSecPinType_t pinType;
    OicUuid_t newDevice;
}PinOxmData_t;

static PinOxmData_t g_PinOxmData = {
        .pinData={0},
        .pinSize = OXM_RANDOM_PIN_DEFAULT_SIZE,
        .pinType = (OicSecPinType_t)(OXM_RANDOM_PIN_DEFAULT_PIN_TYPE),
    };

/**
 * Internal function to check pinType
 */
static bool IsValidPinType(OicSecPinType_t pinType)
{
    return ((NUM_PIN & pinType) ||
            (LOWERCASE_CHAR_PIN & pinType) ||
            (UPPERCASE_CHAR_PIN & pinType));
}

OCStackResult SetRandomPinPolicy(size_t pinSize, OicSecPinType_t pinType)
{
    if(OXM_RANDOM_PIN_MIN_SIZE > pinSize)
    {
        OIC_LOG(ERROR, TAG, "PIN size is too small");
        return OC_STACK_INVALID_PARAM;
    }
    if(OXM_RANDOM_PIN_MAX_SIZE < pinSize)
    {
        OIC_LOG_V(ERROR, TAG, "PIN size can not exceed %d bytes", OXM_RANDOM_PIN_MAX_SIZE);
        return OC_STACK_INVALID_PARAM;
    }
    if(false == IsValidPinType(pinType))
    {
        OIC_LOG(ERROR, TAG, "Invalid PIN type.");
        return OC_STACK_INVALID_PARAM;
    }

    g_PinOxmData.pinSize = pinSize;
    g_PinOxmData.pinType = pinType;

    return OC_STACK_OK;
}

void SetInputPinCB(InputPinCallback pinCB)
{
    if(NULL == pinCB)
    {
        OIC_LOG(ERROR, TAG, "Failed to set callback for input pin.");
        return;
    }

    if ((NULL != g_inputPinCallbacks.callback) || (NULL != g_inputPinCallbacks.contextCallback))
    {
        OIC_LOG(ERROR, TAG, "Callback for input pin is already set.");
        return;
    }

    g_inputPinCallbacks.callback = pinCB;
}

OCStackResult SetInputPinWithContextCB(InputPinCallbackWithContext inputPinCB, void* context)
{
    if (NULL == inputPinCB)
    {
        OIC_LOG(ERROR, TAG, "Failed to set callback for input pin.");
        return OC_STACK_INVALID_PARAM;
    }

    if ((NULL != g_inputPinCallbacks.callback) || (NULL != g_inputPinCallbacks.contextCallback))
    {
        OIC_LOG(ERROR, TAG, "Callback for input pin is already set.");
        return OC_STACK_DUPLICATE_REQUEST;
    }

    g_inputPinCallbacks.contextCallback = inputPinCB;
    g_inputPinCallbacks.context = context;

    return OC_STACK_OK;
}

void SetGeneratePinCB(GeneratePinCallback pinCB)
{
    if(NULL == pinCB)
    {
        OIC_LOG(ERROR, TAG, "Failed to set callback for generate pin.");
        return;
    }

    if ((NULL != g_displayPinCallbacks.callback) || (NULL != g_displayPinCallbacks.contextCallback))
    {
        OIC_LOG(ERROR, TAG, "Callback for generate pin is already set.");
        return;
    }

    g_displayPinCallbacks.callback = pinCB;
}

OCStackResult SetDisplayPinWithContextCB(DisplayPinCallbackWithContext displayPinCB, void* context)
{
    if (NULL == displayPinCB)
    {
        OIC_LOG(ERROR, TAG, "Failed to set a callback for displaying a pin.");
        return OC_STACK_INVALID_PARAM;
    }

    if ((NULL != g_displayPinCallbacks.callback) || (NULL != g_displayPinCallbacks.contextCallback))
    {
        OIC_LOG(ERROR, TAG, "Callback for displaying a pin is already set.");
        return OC_STACK_DUPLICATE_REQUEST;
    }

    g_displayPinCallbacks.contextCallback = displayPinCB;
    g_displayPinCallbacks.context = context;

    return OC_STACK_OK;
}

void SetClosePinDisplayCB(ClosePinDisplayCallback closeCB)
{
    if (NULL == closeCB)
    {
        OIC_LOG(ERROR, TAG, "Failed to set a callback for closing a pin.");
        return;
    }

    if (NULL != g_displayPinCallbacks.closePinDisplayCallback)
    {
        OIC_LOG(ERROR, TAG, "Callback for closing a pin is already set.");
        return;
    }

    g_displayPinCallbacks.closePinDisplayCallback = closeCB;
}

void UnsetInputPinCB()
{
    UnsetInputPinWithContextCB();
}

void UnsetInputPinWithContextCB()
{
    g_inputPinCallbacks.callback = NULL;
    g_inputPinCallbacks.contextCallback = NULL;
    g_inputPinCallbacks.context = NULL;
}

void UnsetGeneratePinCB()
{
    UnsetDisplayPinWithContextCB();
}

void UnsetDisplayPinWithContextCB()
{
    g_displayPinCallbacks.callback = NULL;
    g_displayPinCallbacks.contextCallback = NULL;
    g_displayPinCallbacks.context = NULL;
}

void UnsetClosePinDisplayCB()
{
    g_displayPinCallbacks.closePinDisplayCallback = NULL;
}

void ClosePinDisplay()
{
    if (g_displayPinCallbacks.closePinDisplayCallback)
    {
        g_displayPinCallbacks.closePinDisplayCallback();
    }
}

/**
 * Internal function to generate PIN element according to pinType.
 * This function assumes the pinType is valid.
 * In case of invalid pinType, '0' will be returned as default vaule.
 */
static char GenerateRandomPinElement(OicSecPinType_t pinType)
{
    const char defaultRetValue = '0';
    char allowedCharacters[NUMBER_OF_PINNUM + NUMBER_OF_ALPHABET * 2];
    uint32_t curIndex = 0;

    if(NUM_PIN & pinType)
    {
        for(char pinEle = '0'; pinEle <= '9'; pinEle++)
        {
            allowedCharacters[curIndex++] = pinEle;
        }
    }
    if(UPPERCASE_CHAR_PIN & pinType)
    {
        for(char pinEle = 'A'; pinEle <= 'Z'; pinEle++)
        {
            allowedCharacters[curIndex++] = pinEle;
        }
    }
    if(LOWERCASE_CHAR_PIN & pinType)
    {
        for(char pinEle = 'a'; pinEle <= 'z'; pinEle++)
        {
            allowedCharacters[curIndex++] = pinEle;
        }
    }

    if(0 == curIndex)
    {
        return defaultRetValue;
    }
    else
    {
        curIndex -= 1;
    }

    return allowedCharacters[OCGetRandomRange(0, curIndex)];
}

OCStackResult GeneratePin(char* pinBuffer, size_t bufferSize)
{
    if(!pinBuffer)
    {
        OIC_LOG(ERROR, TAG, "PIN buffer is NULL");
        return OC_STACK_INVALID_PARAM;
    }
    if(g_PinOxmData.pinSize + 1 > bufferSize)
    {
        OIC_LOG(ERROR, TAG, "PIN buffer size is too small");
        return OC_STACK_INVALID_PARAM;
    }
    if(false == IsValidPinType(g_PinOxmData.pinType))
    {
        OIC_LOG(ERROR, TAG, "Invalid PIN type.");
        OIC_LOG(ERROR, TAG, "Please set the PIN type using SetRandomPinPolicy API.");
        return OC_STACK_ERROR;
    }

    for(size_t i = 0; i < g_PinOxmData.pinSize; i++)
    {
        pinBuffer[i] = GenerateRandomPinElement(g_PinOxmData.pinType);
        g_PinOxmData.pinData[i] = pinBuffer[i];
    }

    pinBuffer[g_PinOxmData.pinSize] = '\0';
    g_PinOxmData.pinData[g_PinOxmData.pinSize] = '\0';

    if(g_displayPinCallbacks.callback)
    {
        g_displayPinCallbacks.callback(pinBuffer, g_PinOxmData.pinSize);
    }
    else if (g_displayPinCallbacks.contextCallback)
    {
        g_displayPinCallbacks.contextCallback(pinBuffer, g_PinOxmData.pinSize, g_displayPinCallbacks.context);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Invoke PIN callback failed!");
        OIC_LOG(ERROR, TAG, "Callback for genrate PIN should be registered to use PIN based OxM.");
        return OC_STACK_ERROR;
    }

    OicUuid_t deviceID;
    if(OC_STACK_OK == GetDoxmDeviceID(&deviceID))
    {
        //Set the device id to derive temporal PSK
        SetUuidForPinBasedOxm(&deviceID);

        /**
         * Since PSK will be used directly by DTLS layer while PIN based ownership transfer,
         * Credential should not be saved into SVR.
         * For this reason, use a temporary get_psk_info callback to random PIN OxM.
         */
        if(CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskForRandomPinOxm))
        {
            OIC_LOG(ERROR, TAG, "Failed to register DTLS credential handler for Random PIN OxM.");
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Failed to read device ID");
        return OC_STACK_ERROR;
    }

    return OC_STACK_OK;
}

OCStackResult InputPin(OicUuid_t deviceId, char* pinBuffer, size_t bufferSize)
{
    if(!pinBuffer)
    {
        OIC_LOG(ERROR, TAG, "PIN buffer is NULL");
        return OC_STACK_INVALID_PARAM;
    }
    if(g_PinOxmData.pinSize + 1 > bufferSize)
    {
        OIC_LOG(ERROR, TAG, "PIN buffer size is too small");
        return OC_STACK_INVALID_PARAM;
    }

    if(g_inputPinCallbacks.callback)
    {
        g_inputPinCallbacks.callback(pinBuffer, bufferSize);
        OICStrcpy((char*)(g_PinOxmData.pinData), OXM_RANDOM_PIN_MAX_SIZE + 1, pinBuffer);
        g_PinOxmData.pinSize = strlen((char*)(g_PinOxmData.pinData));
    }
    else if (g_inputPinCallbacks.contextCallback)
    {
        g_inputPinCallbacks.contextCallback(deviceId, pinBuffer, bufferSize, g_inputPinCallbacks.context);
        OICStrcpy((char*)(g_PinOxmData.pinData), OXM_RANDOM_PIN_MAX_SIZE + 1, pinBuffer);
        g_PinOxmData.pinSize = strlen((char*)(g_PinOxmData.pinData));
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Invoke PIN callback failed!");
        OIC_LOG(ERROR, TAG, "Callback for input PIN should be registered to use Random PIN based OxM.");
        return OC_STACK_ERROR;
    }

    return OC_STACK_OK;
}

#ifdef MULTIPLE_OWNER
OCStackResult SetPreconfigPin(const char *pinBuffer, size_t pinLength)
{
    if(NULL == pinBuffer || OXM_PRECONFIG_PIN_MAX_SIZE < pinLength)
    {
        return OC_STACK_INVALID_PARAM;
    }

    memcpy(g_PinOxmData.pinData, pinBuffer, pinLength);
    g_PinOxmData.pinData[pinLength] = '\0';

    return OC_STACK_OK;
}
#endif //MULTIPLE_OWNER

#ifdef __WITH_DTLS__

void SetUuidForPinBasedOxm(const OicUuid_t* uuid)
{
    if(NULL != uuid)
    {
        memcpy(g_PinOxmData.newDevice.id, uuid->id, UUID_LENGTH);
    }
}

int DerivePSKUsingPIN(uint8_t* result)
{
    int dtlsRes = DeriveCryptoKeyFromPassword(
                                              (const unsigned char *)g_PinOxmData.pinData,
                                              g_PinOxmData.pinSize,
                                              g_PinOxmData.newDevice.id,
                                              UUID_LENGTH, PBKDF_ITERATIONS,
                                              OWNER_PSK_LENGTH_128, result);

    OIC_LOG_V(DEBUG, TAG, "DeriveCryptoKeyFromPassword Completed (%d)", dtlsRes);
    OIC_LOG_V(DEBUG, TAG, "PIN : %s", g_PinOxmData.pinData);
    OIC_LOG(DEBUG, TAG, "UUID : ");
    OIC_LOG_BUFFER(DEBUG, TAG, g_PinOxmData.newDevice.id, UUID_LENGTH);

    return dtlsRes;
}

int32_t GetDtlsPskForRandomPinOxm( CADtlsPskCredType_t type,
              const unsigned char *UNUSED1, size_t UNUSED2,
              unsigned char *result, size_t result_length)
{
    int32_t ret = -1;

    (void)UNUSED1;
    (void)UNUSED2;

    if ((NULL == result)
        || (result_length < OWNER_PSK_LENGTH_128)
        || (result_length > INT32_MAX))
    {
        return ret;
    }

    switch (type)
    {
        case CA_DTLS_PSK_HINT:
        case CA_DTLS_PSK_IDENTITY:
            {
                /**
                 * The server will provide PSK hint to identify PSK according to RFC 4589 and RFC 4279.
                 *
                 * At this point, The server generate random hint and
                 * provide it to client through server key exchange message.
                 */
                if (!OCGetRandomBytes(result, result_length))
                {
                    OIC_LOG(ERROR, TAG, "Failed to generate random PSK hint");
                    break;
                }
                ret = (int32_t)result_length;

                OIC_LOG(DEBUG, TAG, "PSK HINT : ");
                OIC_LOG_BUFFER(DEBUG, TAG, result, result_length);
            }
            break;

        case CA_DTLS_PSK_KEY:
            {
                if(0 == DerivePSKUsingPIN((uint8_t*)result))
                {
                    ret = OWNER_PSK_LENGTH_128;
                }
                else
                {
                    OIC_LOG_V(ERROR, TAG, "Failed to derive crypto key from PIN");
                }
            }
            break;

        default:
            {
                OIC_LOG (ERROR, TAG, "Wrong value passed for CADtlsPskCredType_t.");
            }
            break;
    }

    return ret;
}

#ifdef MULTIPLE_OWNER
int32_t GetDtlsPskForMotRandomPinOxm( CADtlsPskCredType_t type,
              const unsigned char *UNUSED1, size_t UNUSED2,
              unsigned char *result, size_t result_length)
{
    int32_t ret = -1;

    (void)UNUSED1;
    (void)UNUSED2;

    if (NULL == result || result_length < OWNER_PSK_LENGTH_128)
    {
        return ret;
    }

    const OicSecDoxm_t* doxm = GetDoxmResourceData();
    if(doxm)
    {
        switch (type)
        {
            case CA_DTLS_PSK_HINT:
            case CA_DTLS_PSK_IDENTITY:
                {
                    memcpy(result, doxm->deviceID.id, sizeof(doxm->deviceID.id));
                    return (sizeof(doxm->deviceID.id));
                }
                break;

            case CA_DTLS_PSK_KEY:
                {
                    if(0 == DerivePSKUsingPIN((uint8_t*)result))
                    {
                        ret = OWNER_PSK_LENGTH_128;
                    }
                    else
                    {
                        OIC_LOG_V(ERROR, TAG, "Failed to derive crypto key from PIN : result");
                        ret = -1;
                    }
                }
                break;

            default:
                {
                    OIC_LOG (ERROR, TAG, "Wrong value passed for CADtlsPskCredType_t.");
                    ret = -1;
                }
                break;
        }
    }

    return ret;
}


int32_t GetDtlsPskForPreconfPinOxm( CADtlsPskCredType_t type,
              const unsigned char *UNUSED1, size_t UNUSED2,
              unsigned char *result, size_t result_length)
{
    int32_t ret = -1;

    (void)UNUSED1;
    (void)UNUSED2;

    if ((NULL == result)
        || (result_length < OWNER_PSK_LENGTH_128)
        || (result_length > INT_MAX))
    {
        return ret;
    }

    const OicSecDoxm_t* doxm = GetDoxmResourceData();
    if(doxm)
    {
        switch (type)
        {
            case CA_DTLS_PSK_HINT:
            case CA_DTLS_PSK_IDENTITY:
                {
                    /**
                     * The server will provide PSK hint to identify PSK according to RFC 4589 and RFC 4279.
                     *
                     * At this point, The server generate random hint and
                     * provide it to client through server key exchange message.
                     */
                    if (!OCGetRandomBytes(result, result_length))
                    {
                        OIC_LOG(ERROR, TAG, "Failed to generate random PSK hint");
                        ret = -1;
                        break;
                    }
                    ret = (int32_t)result_length;

                    OIC_LOG(DEBUG, TAG, "PSK HINT : ");
                    OIC_LOG_BUFFER(DEBUG, TAG, result, result_length);
                }
                break;

            case CA_DTLS_PSK_KEY:
                {
                    OicUuid_t uuid;
                    memcpy(&uuid, &WILDCARD_SUBJECT_ID, sizeof(uuid));

                    //Load PreConfigured-PIN
                    const OicSecCred_t* cred = GetCredResourceData(&uuid);
                    if(cred)
                    {
                        char* pinBuffer = NULL;
                        size_t pinLength = 0;
                        if(OIC_ENCODING_RAW == cred->privateData.encoding)
                        {
                            pinBuffer = (char*)OICCalloc(1, cred->privateData.len + 1);
                            if(NULL == pinBuffer)
                            {
                                OIC_LOG (ERROR, TAG, "Failed to allocate memory");
                                return ret;
                            }
                            pinLength = cred->privateData.len;
                            memcpy(pinBuffer, cred->privateData.data, pinLength);
                        }
                        else if(OIC_ENCODING_BASE64 == cred->privateData.encoding)
                        {
                            size_t pinBufSize = B64DECODE_OUT_SAFESIZE((cred->privateData.len + 1));
                            pinBuffer = (char*)OICCalloc(1, pinBufSize);
                            if(NULL == pinBuffer)
                            {
                                OIC_LOG (ERROR, TAG, "Failed to allocate memory");
                                return ret;
                            }

                            if(B64_OK != b64Decode((char*)cred->privateData.data, cred->privateData.len, (uint8_t*)pinBuffer, pinBufSize, &pinLength))
                            {
                                OIC_LOG (ERROR, TAG, "Failed to base64 decoding.");
                                OICFree(pinBuffer);
                                return ret;
                            }
                        }
                        else
                        {
                            OIC_LOG(ERROR, TAG, "Unknown encoding type of PIN/PW credential.");
                            return ret;
                        }

                        if (g_PinOxmData.pinSize < pinLength)
                        {
                            OIC_LOG (ERROR, TAG, "PIN length too long");
                            OICFree(pinBuffer);
                            return ret;
                        }
                        memcpy(g_PinOxmData.pinData, pinBuffer, pinLength);
                        OICFree(pinBuffer);
                    }

                    if(0 == DerivePSKUsingPIN((uint8_t*)result))
                    {
                        ret = OWNER_PSK_LENGTH_128;
                    }
                    else
                    {
                        OIC_LOG_V(ERROR, TAG, "Failed to derive crypto key from PIN : result");
                        ret = -1;
                    }
                }
                break;

            default:
                {
                    OIC_LOG (ERROR, TAG, "Wrong value passed for CADtlsPskCredType_t.");
                    ret = -1;
                }
                break;
        }
    }

    return ret;
}


int32_t GetDtlsPskForMotPreconfPinOxm( CADtlsPskCredType_t type,
              const unsigned char *UNUSED1, size_t UNUSED2,
              unsigned char *result, size_t result_length)
{
    int32_t ret = -1;

    (void)UNUSED1;
    (void)UNUSED2;

    if (NULL == result || result_length < OWNER_PSK_LENGTH_128)
    {
        return ret;
    }

    const OicSecDoxm_t* doxm = GetDoxmResourceData();
    if(doxm)
    {
        switch (type)
        {
            case CA_DTLS_PSK_HINT:
            case CA_DTLS_PSK_IDENTITY:
                {
                    memcpy(result, doxm->deviceID.id, sizeof(doxm->deviceID.id));
                    return (sizeof(doxm->deviceID.id));
                }
                break;
            case CA_DTLS_PSK_KEY:
                {
                    OicUuid_t uuid;
                    memcpy(&uuid, &WILDCARD_SUBJECT_ID, sizeof(uuid));

                    //Load PreConfigured-PIN
                    const OicSecCred_t* cred = GetCredResourceData(&uuid);
                    if(cred)
                    {
                        char* pinBuffer = NULL;
                        size_t pinLength = 0;
                        if(OIC_ENCODING_RAW == cred->privateData.encoding)
                        {
                            pinBuffer = (char*)OICCalloc(1, cred->privateData.len + 1);
                            if(NULL == pinBuffer)
                            {
                                OIC_LOG (ERROR, TAG, "Failed to allocate memory");
                                return ret;
                            }
                            pinLength = cred->privateData.len;
                            memcpy(pinBuffer, cred->privateData.data, pinLength);
                        }
                        else if(OIC_ENCODING_BASE64 == cred->privateData.encoding)
                        {
                            size_t pinBufSize = B64DECODE_OUT_SAFESIZE((cred->privateData.len + 1));
                            pinBuffer = (char*)OICCalloc(1, pinBufSize);
                            if(NULL == pinBuffer)
                            {
                                OIC_LOG (ERROR, TAG, "Failed to allocate memory");
                                return ret;
                            }

                            if(B64_OK != b64Decode((char*)cred->privateData.data, cred->privateData.len, (uint8_t*)pinBuffer, pinBufSize, &pinLength))
                            {
                                OIC_LOG (ERROR, TAG, "Failed to base64 decoding.");
                                OICFree(pinBuffer);
                                return ret;
                            }
                        }
                        else
                        {
                            OIC_LOG(ERROR, TAG, "Unknown encoding type of PIN/PW credential.");
                            return ret;
                        }

                        if (g_PinOxmData.pinSize < pinLength)
                        {
                            OIC_LOG (ERROR, TAG, "PIN length is too long");
                            OICFree(pinBuffer);
                            return ret;
                        }
                        memcpy(g_PinOxmData.pinData, pinBuffer, pinLength);
                        OICFree(pinBuffer);
                    }

                    if(0 == DerivePSKUsingPIN((uint8_t*)result))
                    {
                        ret = OWNER_PSK_LENGTH_128;
                    }
                    else
                    {
                        OIC_LOG_V(ERROR, TAG, "Failed to derive crypto key from PIN : result");
                        ret = -1;
                    }
                }
                break;

            default:
                {
                    OIC_LOG (ERROR, TAG, "Wrong value passed for CADtlsPskCredType_t.");
                    ret = -1;
                }
                break;
        }
    }

    return ret;
}
#endif //MULTIPLE_OWNER

#endif //__WITH_DTLS__
