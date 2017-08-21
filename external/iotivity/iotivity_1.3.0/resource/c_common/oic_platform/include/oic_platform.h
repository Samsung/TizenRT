/* *****************************************************************
 *
 * Copyright 2017 Microsoft
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef OIC_OICPLATFORM_H_
#define OIC_OICPLATFORM_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define OIC_UUID_LENGTH     16

/**
 * Declares OIC Platform Results & Errors.
 */
typedef enum
{
    /** Success status code.*/
    OIC_PLATFORM_OK = 0,

    /** Error status code - START HERE.*/
    OIC_PLATFORM_INVALID_PARAM,
    OIC_PLATFORM_NO_MEMORY,
    OIC_PLATFORM_NOTIMPL,

    /** Generic ERROR.*/
    OIC_PLATFORM_ERROR = 255
    /** Error status code - END HERE.*/
} OICPlatformResult_t;

/**
 * Function returns a UUID that can be used for platform ID.
 *
 * @param[in] platformUuid  The generated UUID.
 *
 * @return true if successful.
 */
bool OICGetPlatformUuid(uint8_t platformUuid[OIC_UUID_LENGTH]);

/**
 * This function returns the platform and application specific local application data path.
 * This path will be used to store the iotivity metadata. For example, the security databases.
 * The path will contain the trailing backslash (\) or forward slash (/) and the null terminating
 * character.
 * To get the needed buffer size, call this function with buffer NULL.
 *
 * @param[out]    buffer    The buffer to store the path.
 * @param[in,out] bufferLen The size of the buffer passed in. If buffer is NULL, the function will
 *                          return the buffer size needed including the null character. If buffer
 *                          passed in doesn't have sufficient size, then bufferLen will also be set
 *                          to the buffer size needed.
 *
 * @return OIC_PLATFORM_OK on success, appropriate error code on failure.
 */
OICPlatformResult_t OICGetLocalAppDataPath(char* buffer, size_t* bufferLen);

/**
 * This function returns the platform and application specific data path for temporary files.
 * This path will be used to store sqlite3 metadata that might need to be created temporarily.
 * The path will contain the trailing backslash (\) or forward slash (/) and the null terminating
 * character.
 * To get the needed buffer size, call this function with buffer NULL.
 *
 * @param[out]    buffer    The buffer to store the path.
 * @param[in,out] bufferLen The size of the buffer passed in. If buffer is NULL, the function will
 *                          return the buffer size needed including the null character. If buffer
 *                          passed in doesn't have sufficient size, then bufferLen will also be set
 *                          to the buffer size needed.
 *
 * @return OIC_PLATFORM_OK on success, appropriate error code on failure.
 */
OICPlatformResult_t OICGetTempAppDataPath(char* buffer, size_t* bufferLen);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif
