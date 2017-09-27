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

#ifndef _SAMSUNG_WHITE_BOX_AES_PROJECT_VERSION2_H_
#define _SAMSUNG_WHITE_BOX_AES_PROJECT_VERSION2_H_

#ifdef __cplusplus
extern "C"
{


#endif							//__cplusplus

/*! SWAPv2 return values */
typedef enum {
	SWAPV2_RETURN_SUCCESS = 0x00000000, /*!< success case */
	SWAPV2_RETURN_INVALID_PARAMETER = 0x80000001, /*!< null parameter */
	SWAPV2_RETURN_INVALID_TBLSIZE = 0x80000002, /*!< invalid table size */
} SWAPV2_RETURN_VAL;



/**
 * @fn		SWAPV2_RETURN_VAL SWAPV2_Process(unsigned char *Table, unsigned int TableLen, unsigned char* in, unsigned char* out, unsigned int OptimizLev)
 * @brief	processing encryption or decryption according to the table
 *
 * @param	[in] Table		look-up table for SWAPv2
 * @param	[in] TableLen	byte-length of look-up table
 * @param	[in] in			input bytes(16-byte length)
 * @param	[out] out		output bytes(16-byte length)
 * @param	[in] OptimizLev	Performance Optimization Level [0~2]
 * @return	SWAPv2_RETURN_VAL
 */
SWAPV2_RETURN_VAL swapv2_process(unsigned char *table, unsigned int table_len, unsigned char *in, unsigned char *out);



#ifdef __cplusplus
}
#endif							//__cplusplus

#endif							//_SAMSUNG_WHITE_BOX_AES_PROJECT_H_
