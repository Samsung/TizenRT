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

#ifndef __TINYARA__
#include <memory.h>
#endif
#include "ocstack.h"
#include "ocrandom.h"
#include "logger.h"
#include "pinoxmcommon.h"
#include "pbkdf2.h"
#include "securevirtualresourcetypes.h"

#define TAG "PIN_OXM_COMMON"

static GeneratePinCallback gGenPinCallback = NULL;
static InputPinCallback gInputPinCallback = NULL;

typedef struct PinOxmData
{
  uint8_t pinData[OXM_RANDOM_PIN_SIZE + 1];
  OicUuid_t newDevice;
} PinOxmData_t;

static PinOxmData_t g_PinOxmData;


void SetInputPinCB(InputPinCallback pinCB)
{
  if (NULL == pinCB)
  {
    OIC_LOG(ERROR, TAG, "Failed to set callback for input pin.");
    return;
  }

  gInputPinCallback = pinCB;
}

void SetGeneratePinCB(GeneratePinCallback pinCB)
{
  if (NULL == pinCB)
  {
    OIC_LOG(ERROR, TAG, "Failed to set callback for generate pin.");
    return;
  }

  gGenPinCallback = pinCB;
}

OCStackResult GeneratePin(char *pinBuffer, size_t bufferSize)
{
  if (!pinBuffer)
  {
    OIC_LOG(ERROR, TAG, "PIN buffer is NULL");
    return OC_STACK_INVALID_PARAM;
  }
  if (OXM_RANDOM_PIN_SIZE + 1 > bufferSize)
  {
    OIC_LOG(ERROR, TAG, "PIN buffer size is too small");
    return OC_STACK_INVALID_PARAM;
  }
  for (size_t i = 0; i < OXM_RANDOM_PIN_SIZE; i++)
  {
    pinBuffer[i] = OCGetRandomRange((uint32_t)'0', (uint32_t)'9');
    g_PinOxmData.pinData[i] = pinBuffer[i];
  }
  pinBuffer[OXM_RANDOM_PIN_SIZE] = '\0';
  g_PinOxmData.pinData[OXM_RANDOM_PIN_SIZE] = '\0';

  if (gGenPinCallback)
  {
    gGenPinCallback(pinBuffer, OXM_RANDOM_PIN_SIZE);
  }
  else
  {
    OIC_LOG(ERROR, TAG, "Invoke PIN callback failed!");
    OIC_LOG(ERROR, TAG, "Callback for genrate PIN should be registered to use PIN based OxM.");
    return OC_STACK_ERROR;
  }

  return OC_STACK_OK;
}


OCStackResult InputPin(char *pinBuffer, size_t bufferSize)
{
  if (!pinBuffer)
  {
    OIC_LOG(ERROR, TAG, "PIN buffer is NULL");
    return OC_STACK_INVALID_PARAM;
  }
  if (OXM_RANDOM_PIN_SIZE + 1 > bufferSize)
  {
    OIC_LOG(ERROR, TAG, "PIN buffer size is too small");
    return OC_STACK_INVALID_PARAM;
  }

  if (gInputPinCallback)
  {
    gInputPinCallback(pinBuffer, OXM_RANDOM_PIN_SIZE + 1);
    memcpy(g_PinOxmData.pinData, pinBuffer, OXM_RANDOM_PIN_SIZE);
    g_PinOxmData.pinData[OXM_RANDOM_PIN_SIZE] = '\0';
  }
  else
  {
    OIC_LOG(ERROR, TAG, "Invoke PIN callback failed!");
    OIC_LOG(ERROR, TAG, "Callback for input PIN should be registered to use PIN based OxM.");
    return OC_STACK_ERROR;
  }

  return OC_STACK_OK;
}

#ifdef __WITH_DTLS__

void SetUuidForRandomPinOxm(const OicUuid_t *uuid)
{
  if (NULL != uuid)
  {
    memcpy(g_PinOxmData.newDevice.id, uuid->id, UUID_LENGTH);
  }
}

int32_t GetDtlsPskForRandomPinOxm(CADtlsPskCredType_t type,
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

  switch (type)
  {
    case CA_DTLS_PSK_HINT:
    case CA_DTLS_PSK_IDENTITY:
      /**
       * The server will provide PSK hint to identify PSK according to RFC 4589 and RFC 4279.
       *
       * At this point, The server generate random hint and
       * provide it to client through server key exchange message.
       */
      OCFillRandomMem(result, result_length);
      ret = result_length;

      OIC_LOG(DEBUG, TAG, "PSK HINT : ");
      OIC_LOG_BUFFER(DEBUG, TAG, result, result_length);
      break;

    case CA_DTLS_PSK_KEY:
    {
      int dtlsRes = DeriveCryptoKeyFromPassword(
                      (const unsigned char *)g_PinOxmData.pinData,
                      OXM_RANDOM_PIN_SIZE,
                      g_PinOxmData.newDevice.id,
                      UUID_LENGTH, PBKDF_ITERATIONS,
                      OWNER_PSK_LENGTH_128, (uint8_t *)result);

      OIC_LOG_V(DEBUG, TAG, "DeriveCryptoKeyFromPassword Completed (%d)", dtlsRes);
      OIC_LOG_V(DEBUG, TAG, "PIN : %s", g_PinOxmData.pinData);
      OIC_LOG(DEBUG, TAG, "UUID : ");
      OIC_LOG_BUFFER(DEBUG, TAG, g_PinOxmData.newDevice.id, UUID_LENGTH);

      if (0 == dtlsRes)
      {
        ret = OWNER_PSK_LENGTH_128;
      }
      else
      {
        OIC_LOG_V(ERROR, TAG, "Failed to derive crypto key from PIN : result=%d", dtlsRes);
        ret = -1;
      }
    }
    break;

    default:
    {
      OIC_LOG(ERROR, TAG, "Wrong value passed for CADtlsPskCredType_t.");
      ret = -1;
    }
    break;
  }

  return ret;
}
#endif //__WITH_DTLS__
