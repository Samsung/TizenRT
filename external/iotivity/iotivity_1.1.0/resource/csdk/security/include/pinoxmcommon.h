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

#ifndef PIN_CALLBACK_DEF_H_
#define PIN_CALLBACK_DEF_H_

#include "securevirtualresourcetypes.h"
#include "casecurityinterface.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define OXM_RANDOM_PIN_SIZE 8

/**
 * Function pointer to print pin code.
 */
typedef void (*GeneratePinCallback)(char *pinData, size_t pinSize);

/**
 * Function pointer to input pin code.
 */
typedef void (*InputPinCallback)(char *pinBuf, size_t bufSize);

/**
 * Function to setting generate PIN callback from user.
 *
 * @param pinCB implementation of generate PIN callback.
 */
void SetGeneratePinCB(GeneratePinCallback pinCB);

/**
 * Function to setting input PIN callback from user.
 *
 * @param pinCB implementation of input PIN callback.
 */
void SetInputPinCB(InputPinCallback pinCB);

/**
 * Function to generate random PIN.
 * This function will send generated PIN to user via callback.
 *
 * @param pinBuffer is the reference to the buffer to store the generated PIN data.
 * @param bufferSize is the size of buffer.
 *
 * @return ::OC_STACK_SUCCESS in case of success or other value in case of error.
 */
OCStackResult GeneratePin(char *pinBuffer, size_t bufferSize);

/**
 * Function to input PIN callback via input callback.
 *
 * @param[in,out] pinBuffer is the reference to the buffer to store the inputed PIN data.
 * @param[in] bufferSize is the size of buffer.
 *
 * @return ::OC_STACK_SUCCESS in case of success or other value in ccase of error.
 */
OCStackResult InputPin(char *pinBuffer, size_t bufferSize);

#ifdef __WITH_DTLS__

/**
 * This function is used by OTM and SRM to
 * register device UUID is required to derive the temporal PSK.
 */
void SetUuidForRandomPinOxm(const OicUuid_t *uuid);

/**
 * This internal callback is used while PIN based ownership transfer.
 * This callback will be used to establish a temporary secure session according to
 * TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256.
 *
 * @param[in]  type type of PSK data required by tinyDTLS layer during DTLS handshake.
 * @param[in]  desc UNUSED.
 * @param[in]  desc_len UNUSED.
 * @param[out] result  Must be filled with the requested information.
 * @param[in]  result_length  Maximum size of @p result.
 *
 * @return The number of bytes written to @p result or a value
 *         less than zero on error.
 */
int32_t GetDtlsPskForRandomPinOxm(CADtlsPskCredType_t type,
                                  const unsigned char *UNUSED1, size_t UNUSED2,
                                  unsigned char *result, size_t result_length);
#endif //__WITH_DTLS__

#ifdef __cplusplus
}
#endif

#endif //PIN_CALLBACK_DEF_H_
