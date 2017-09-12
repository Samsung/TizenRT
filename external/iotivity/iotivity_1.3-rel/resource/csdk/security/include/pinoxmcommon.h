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

#include "experimental/securevirtualresourcetypes.h"
#include "casecurityinterface.h"

#ifdef __cplusplus
 extern "C" {
#endif // __cplusplus

#define OXM_RANDOM_PIN_DEFAULT_SIZE (8)
#define OXM_RANDOM_PIN_DEFAULT_PIN_TYPE (NUM_PIN | LOWERCASE_CHAR_PIN | UPPERCASE_CHAR_PIN)
#define OXM_RANDOM_PIN_MIN_SIZE (4)
#define OXM_RANDOM_PIN_MAX_SIZE (32)
#define OXM_PRECONFIG_PIN_MAX_SIZE (OXM_RANDOM_PIN_MAX_SIZE)

/** Number of PIN type */
#define OXM_PIN_TYPE_COUNT 3

/**
 * PIN type definition.
 * This type supports multiple bit set.
 * e.g.) NUM_PIN | UPPERCASE_CHAR_PIN
 */
typedef enum OicSecPinType{
    NUM_PIN            = (0x1 << 0),    //Numeric PIN
    UPPERCASE_CHAR_PIN = (0x1 << 1),    //uppercase character PIN
    LOWERCASE_CHAR_PIN = (0x1 << 2)     //lowercase character PIN
}OicSecPinType_t;

/**
 * Function pointer to display pin code.
 */
typedef void (OC_CALL *GeneratePinCallback)(char* pinData, size_t pinSize);

/**
 * Function pointer to display pin code, with context.
 */
typedef void(OC_CALL *DisplayPinCallbackWithContext)(char* pinData, size_t pinSize, void* context);

/**
 * Function pointer to close the displied PIN.
 */
typedef void (OC_CALL *ClosePinDisplayCallback)(void);

/**
 * Function pointer to input pin code.
 */
typedef void (OC_CALL *InputPinCallback)(char* pinBuf, size_t bufSize);

/**
 * Function pointer to input pin code, with context and device information.
 */
typedef void(OC_CALL *InputPinCallbackWithContext)(OicUuid_t deviceId, char* pinBuffer, size_t pinBufferSize, void* context);

/**
 * Function to set the display PIN callback from the user.
 *
 * @deprecated Use SetDisplayPinWithContextCB instead.
 *
 * @param pinCB implementation of generate PIN callback.
 */
void OC_CALL SetGeneratePinCB(GeneratePinCallback pinCB);

/**
 * Function to set the display PIN callback from the user with context.
 *
 * @param displayPinCB  implementation of display PIN callback.
 * @param context       context to return in the callback.
 *
 * @return OC_STACK_OK in case of success or other value in case of error.
 *         OC_STACK_INVALID_PARAM if pinCB is invalid.
 *         OC_STACK_DUPLICATE_REQUEST if a display pin callback has already been set.
 */
OCStackResult OC_CALL SetDisplayPinWithContextCB(DisplayPinCallbackWithContext displayPinCB, void* context);

/**
 * Function to set the input PIN callback from the user.
 *
 * @deprecated Use SetInputPinWithContextCB instead.
 *
 * @param pinCB implementation of input PIN callback.
 */
void OC_CALL SetInputPinCB(InputPinCallback pinCB);

/**
 * Function to set the input PIN callback from the user with context.
 *
 * @param inputPinCB  implementation of input PIN callback.
 * @param context     context to return in the callback.
 *
 * @return OC_STACK_OK in case of success or other value in case of error.
 *         OC_STACK_INVALID_PARAM if pinCB is invalid.
 *         OC_STACK_DUPLICATE_REQUEST if an input pin callback has already been set.
 */
OCStackResult OC_CALL SetInputPinWithContextCB(InputPinCallbackWithContext inputPinCB, void* context);

/**
 * Function to set the close PIN callback
 * This callback will be invoked when PIN based OTM is finished.
 *
 * @param closeCB implementation of close PIN callback.
 */
void OC_CALL SetClosePinDisplayCB(ClosePinDisplayCallback closeCB);

/**
 * Function to unset the input PIN callback.
 * NOTE : Do not call this function while PIN based ownership transfer is in progress.
 *
 * @deprecated Use UnsetInputPinWithContextCB instead.
 *
 */
void OC_CALL UnsetInputPinCB();

/**
 * Function to unset the input PIN callback.
 * NOTE : Do not call this function while PIN based ownership transfer is in progress.
 */
void OC_CALL UnsetInputPinWithContextCB();

/**
 * Function to unset the PIN generation callback.
 * NOTE : Do not call this function while PIN based ownership transfer is in progress.
 *
 * @deprecated Use UnsetDisplayPinWithContextCB instead.
 *
 */
void OC_CALL UnsetGeneratePinCB();

/**
 * Function to unset the PIN display callback.
 * NOTE : Do not call this function while PIN based ownership transfer is in progress.
 */
void OC_CALL UnsetDisplayPinWithContextCB();

/**
 * Function to unset the PIN close callback.
 * NOTE : Do not call this function while PIN based ownership transfer is in progress.
 */
void OC_CALL UnsetClosePinCB();

/**
 * Function to generate a random PIN.
 * This function will send a generated PIN to the user via the callback that was set in
 * SetGeneratePinCB or SetGeneratePinWithContextCB.
 *
 * @param pinBuffer is the reference to the buffer to store the generated PIN data.
 * @param bufferSize is the size of buffer.
 *
 * @return ::OC_STACK_OK in case of success or other value in case of error.
 */
OCStackResult OC_CALL GeneratePin(char* pinBuffer, size_t bufferSize);

/**
 * Function to get a pin for a device.
 * This function will acquire a pin from the user via the callback that was set in
 * SetInputPinCB or SetInputPinWithContextCB.
 *
 * @param[in] deviceId is the device that is requesting a pin
 * @param[in,out] pinBuffer is the reference to the buffer to store the inputed PIN data.
 * @param[in] bufferSize is the size of buffer.
 *
 * @return ::OC_STACK_OK in case of success or other value in ccase of error.
 */
OCStackResult InputPin(OicUuid_t deviceId, char* pinBuffer, size_t bufferSize);

/**
 * Function to invoke the callback for close a PIN display.
 */
void ClosePinDisplay();


#ifdef MULTIPLE_OWNER
/**
 * Function to save the Pre-configured PIN.
 *
 * @param[in] pinBuffer PIN data
 * @param[in] pinLength byte length of PIN
 *
 * @return ::OC_STACK_SUCCESS in case of success or other value in ccase of error.
 */
OCStackResult OC_CALL SetPreconfigPin(const char *pinBuffer, size_t pinLength);
#endif

/**
 * Function to set the policy for random PIN generation
 *
 * @param[in] pinSize Byte length of random PIN
 * @param[in] pinType Type of random PIN (ref OicSecPinType)
 *
 * @return ::OC_STACK_OK in case of success or other value in case of error.
 */
OCStackResult OC_CALL SetRandomPinPolicy(size_t pinSize, OicSecPinType_t pinType);

#ifdef __WITH_DTLS__

/**
 * This function is used by OTM and SRM to
 * register device UUID is required to derive the temporal PSK.
 */
void SetUuidForPinBasedOxm(const OicUuid_t* uuid);

/**
 * This internal callback is used while Random PIN based OTM.
 * This callback will be used to establish a temporary secure session according to
 * TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256.
 *
 * @param[in]  type type of PSK data required by tinyDTLS layer during DTLS handshake.
 * @param[in]  UNUSED1 UNUSED.
 * @param[in]  UNUSED2 UNUSED.
 * @param[out] result  Must be filled with the requested information.
 * @param[in]  result_length  Maximum size of @p result.
 *
 * @return The number of bytes written to @p result or a value
 *         less than zero on error.
 */
int32_t GetDtlsPskForRandomPinOxm( CADtlsPskCredType_t type,
              const unsigned char *UNUSED1, size_t UNUSED2,
              unsigned char *result, size_t result_length);

#ifdef MULTIPLE_OWNER
/**
 * This internal callback is used while Random PIN based MOT.
 * This callback will be used to establish a temporary secure session according to
 * TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256.
 *
 * @param[in]  type type of PSK data required by tinyDTLS layer during DTLS handshake.
 * @param[in]  UNUSED1 UNUSED.
 * @param[in]  UNUSED2 UNUSED.
 * @param[out] result  Must be filled with the requested information.
 * @param[in]  result_length  Maximum size of @p result.
 *
 * @return The number of bytes written to @p result or a value
 *         less than zero on error.
 */
int32_t GetDtlsPskForMotRandomPinOxm( CADtlsPskCredType_t type,
              const unsigned char *UNUSED1, size_t UNUSED2,
              unsigned char *result, size_t result_length);


/**
 * This internal callback is used while Preconfigured-PIN OTM.
 * This callback will be used to establish a temporary secure session according to
 * TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256.
 *
 * @param[in]  type type of PSK data required by tinyDTLS layer during DTLS handshake.
 * @param[in]  UNUSED1 UNUSED.
 * @param[in]  UNUSED2 UNUSED.
 * @param[out] result  Must be filled with the requested information.
 * @param[in]  result_length  Maximum size of @p result.
 *
 * @return The number of bytes written to @p result or a value
 *         less than zero on error.
 */
int32_t GetDtlsPskForPreconfPinOxm( CADtlsPskCredType_t type,
              const unsigned char *UNUSED1, size_t UNUSED2,
              unsigned char *result, size_t result_length);


/**
 * This internal callback is used while Preconfigured-PIN MOT.
 * This callback will be used to establish a temporary secure session according to
 * TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256.
 *
 * @param[in]  type type of PSK data required by tinyDTLS layer during DTLS handshake.
 * @param[in]  UNUSED1 UNUSED.
 * @param[in]  UNUSED2 UNUSED.
 * @param[out] result  Must be filled with the requested information.
 * @param[in]  result_length  Maximum size of @p result.
 *
 * @return The number of bytes written to @p result or a value
 *         less than zero on error.
 */
int32_t GetDtlsPskForMotPreconfPinOxm( CADtlsPskCredType_t type,
              const unsigned char *UNUSED1, size_t UNUSED2,
              unsigned char *result, size_t result_length);

#endif //MULTIPLE_OWNER


/**
 * API to derive the PSK based on PIN and new device's UUID.
 * New device's UUID should be set through SetUuidForPinBasedOxm() API before this API is invoked.
 *
 * @param[out] result generated PSK
 *
 * @return 0 for success, otherwise error.
 */
int DerivePSKUsingPIN(uint8_t* result);

#endif //__WITH_DTLS__

#ifdef __cplusplus
}
#endif

#endif //PIN_CALLBACK_DEF_H_
