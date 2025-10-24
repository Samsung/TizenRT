// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __AEC_H__
#define __AEC_H__
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

typedef struct _AECContext AECContext;
/**
 * @brief AEC enum defines
 * @defgroup AEC enums
 * @ingroup AEC
 * @{
 */

enum AEC_CTRL_CMD
{
	AEC_CTRL_CMD_NULL = 0,

	AEC_CTRL_CMD_GET_MIC_DELAY,    /**< Get mic delay */
	AEC_CTRL_CMD_SET_MIC_DELAY,    /**< Set mic delay */

	AEC_CTRL_CMD_GET_EC_DEPTH,     /**< Get ec depth */
	AEC_CTRL_CMD_SET_EC_DEPTH,     /**< Set ec depth */

	AEC_CTRL_CMD_GET_NS_LEVEL,     /**<Get ns level */
	AEC_CTRL_CMD_SET_NS_LEVEL,     /**<Set ns level */

	AEC_CTRL_CMD_GET_DRC,          /**<Get drc */
	AEC_CTRL_CMD_SET_DRC,          /**<Set drc */

	AEC_CTRL_CMD_GET_FLAGS,        /**<Get flags */
	AEC_CTRL_CMD_SET_FLAGS,        /**<Set flags */

	AEC_CTRL_CMD_SET_VOL,          /**<Set volum */
	AEC_CTRL_CMD_SET_REF_SCALE,    /**<Set reference scale */
	AEC_CTRL_CMD_SET_TxRxThr,      /**<Set TxRxThr */
	AEC_CTRL_CMD_SET_TxRxFlr,      /**<Set TxRxFlr */
    AEC_CTRL_CMD_SET_ECRSD1,       /**<Set ECRSD1 */
	AEC_CTRL_CMD_SET_ECRSD2,       /**<Set ECRSD2 */
	AEC_CTRL_CMD_SET_RSDBAND,      /**<Set RSDBAND */
	AEC_CTRL_CMD_SET_NS_PARA,      /**<Set ns parameter */
	AEC_CTRL_CMD_SET_PARAMS,       /**<Set parameters */
    AEC_CTRL_CMD_SET_EC_COE,       /**<Set EC_COE */
    AEC_CTRL_CMD_SET_DRC_TAB,      /**<Set DRC_TAB */
	AEC_CTRL_CMD_GET_RX_BUF,       /**<Get rx buffer addr */
	AEC_CTRL_CMD_GET_TX_BUF,       /**<Get tx buffer addr */
	AEC_CTRL_CMD_GET_OUT_BUF,      /**<Get output buffer addr */
	AEC_CTRL_CMD_GET_FRAME_SAMPLE, /**<Get frame sample */
};

/**
 * @}
 */
/**
 * @brief AUD struct defines
 * @defgroup bk_api_aud_structs structs in AUD
 * @ingroup bk_api_aud
 * @{
 */


/**
 * @}
 */


/**
 * @brief AEC API
 * @defgroup AEC API group
 * @{
 */


/**
 * @brief     Get AECContext size (byte)
 *
 * @param delay delay samples
 *
 * @return
 *    - uint32_t: size(byte)
 */
uint32_t  aec_size    (uint32_t delay);

/**
 * @brief     Init aec
 *
 * This API init aec function :
 *  - Set aec parameters
 *  - Init fft parameters
 *
 * Usage example:
 *
 *
 *     AECContext* aec = NULL;
 *
 *     //malloc aec memory
 *     aec = (AECContext*)os_malloc(aec_size());
 *
 *     //init aec
 *     aec_init(aec, 16000);
 *
 *
 * @param aec aec parameters
 * @param fs frame sample 8K/16K
 *
 * @return none
 */
void aec_init    (AECContext* aec, int16_t fs);

/**
 * @brief     Control aec
 *
 * This API control aec function :
 *  - Set aec parameters
 *  - Get data addr
 *
 * Usage example:
 *
 *
 *     AECContext* aec = NULL;
 *
 *     //malloc aec memory
 *     aec = (AECContext*)os_malloc(aec_size());
 *
 *     //init aec
 *     aec_init(aec, 16000);
 *
 *     //set mic delay
 *     aec_ctrl(aec, AEC_CTRL_CMD_SET_MIC_DELAY, 10);
 *
 *
 * @param aec aec parameters
 * @param cmd control command in enum AEC_CTRL_CMD
 * @param arg value
 *
 * @return none
 */
void aec_ctrl    (AECContext* aec, uint32_t cmd, uint32_t arg);

/**
 * @brief     Control aec
 *
 * This API control aec function :
 *  - Set aec parameters
 *  - Get data addr
 *
 * Usage example:
 *
 *
 *     AECContext* aec = NULL;
 *
 *     //malloc aec memory
 *     aec = (AECContext*)os_malloc(aec_size());
 *
 *     //init aec
 *     aec_init(aec, 16000);
 *
 *     //aec excute work
 *     aec_proc (aec, rin, sin, out);
 *
 *
 * @param aec aec parameters
 * @param rin reference data
 * @param sin source data
 * @param out output data
 *
 * @return none
 */
void aec_proc    (AECContext* aec, int16_t* rin, int16_t* sin, int16_t* out);

/**
 * @}
 */

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__AEC_H__
