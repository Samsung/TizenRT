/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef _SECURE_STORAGE_DEFINES_H_
#define _SECURE_STORAGE_DEFINES_H_

#ifdef __cplusplus
extern "C" {


#endif	/* 
 */

#define SS_HEADER_PREFIX "SS_HDR"
#define SS_HEADER_PREFIX_SIZE 6
#define SS_HEADER_MAC_SIZE 16
#define SS_HEADER_SIZE 32

#define SS_CMK_MAX_SIZE 16
#define SS_DEVICE_ID_MAX_SIZE 256
#define SS_SSK_MAX_SIZE (SS_CMK_MAX_SIZE + (SS_DEVICE_ID_MAX_SIZE * 4) + 1)
#define SECURE_STORAGE_FORMAT 0

#define SS_CRYPTO_KEY "CRYPTO_KEY"
#define SS_CRYPTO_KEY_SIZE 16
#define SS_CRYPTO_IV_SIZE 16
#define SS_AES_BLK_SIZE 16
#define SS_BUFFER_BLOCK_SIZE (1024 * 4)

#define CBC_MAC_SIZE 32

enum SECURE_STORAGE_CODE {
	SS_SUCCESS = 0 /*1 */ ,
	SS_NO_EXIST_PROPERTY = -1000,
	SS_NULL_REFERENCE = -1001,
	SS_INVALID_ARGUMENT = -1002,
	SS_MEMORY_ALLOCATION_FAIL = -1003,
	SS_MEMORY_OVERFLOW = -1005,
	SS_DATA_VALIDATION_FAIL = -1006,
	SS_INVALID_DEV_INFO_PATH = -1007,
	SS_BUFFER_OVERRUN = -1008,
	SS_COULD_NOT_FOUND_CRYPTO_KEY = -1009,
	SS_CRYPTO_FAILED = -1010,
	SS_CRYPTO_INIT_FAILED = -1011,
	SS_ERROR = -1100
};



typedef struct __ss_cxt {


	unsigned char ssMasterKey[SS_CRYPTO_KEY_SIZE];	//machine specific key
	unsigned int ssMasterKeyLen;	//length of key
	unsigned char ssIV[SS_CRYPTO_IV_SIZE];	//machine specific key
	unsigned int ssIVLen;	//length of key
	unsigned char *ssBuffer;


	unsigned int ssBufferSize;


	unsigned int cur_pos;


	int isOpend;


} secure_storage_ctx_s;



#ifdef __cplusplus
}
#endif	/* 
 */

#endif	/* 
 */
