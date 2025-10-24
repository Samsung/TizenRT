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

#pragma once

#include <common/bk_err.h>


#ifdef __cplusplus
extern "C" {
#endif

#define USE_JPEG_DEC_COMPLETE_CALLBACKS 1 /**< set 1, register jpeg decode complete callback, set 0, register jpeg dec cpu isr*/

/** jpeg dec isr return value */
typedef struct
{
	bool ok;			/**< jpeg decoder success */
	uint16_t pixel_x;  /**< jpeg x pixel */
	uint16_t pixel_y;  /**< jpeg y pixel */
	uint32_t size;     /**< jpeg size */
} jpeg_dec_res_t;

/** Error code */
typedef enum {
	JDR_OK = 0,	/**< 0: Succeeded */
	JDR_INTR,	/**< 1: Interrupted by output function */
	JDR_INP,	/**< 2: Device error or wrong termination of input stream */
	JDR_MEM1,	/**< 3: Insufficient memory pool for the image */
	JDR_MEM2,	/**< 4: Insufficient stream input buffer */
	JDR_PAR,	/**< 5: Parameter error */
	JDR_FMT1,	/**< 6: Data format error (may be damaged data) */
	JDR_FMT2,	/**< 7: Right format but not supported */
	JDR_FMT3	/**< 8: Not supported JPEG standard */
} JRESULT;


typedef enum {
	DEC_END_OF_FRAME = 0, /**< select jpeg decode a complete frame enter isr callback */
	DEC_END_OF_LINE_NUM,  /**<  select jpeg decode line num enter isr callback, used with api line_en*/
	DEC_ISR_MAX
}jpeg_dec_isr_type_t;

#if (USE_JPEG_DEC_COMPLETE_CALLBACKS == 1)
/**
* @brief refistrer type jpeg decode complete isr function
*
* param  return jpeg x pixel , y pixel , and jpeg size
*
* @return
*	  - BK_OK: succeed
*	  - others: other errors.
*/
typedef void (*jpeg_dec_isr_cb_t)(jpeg_dec_res_t *result);               /**< jpegdec int isr register func type */
#else
typedef void (*jpeg_dec_isr_t)(void);                                    /**< jpegdec int isr register func type */
#endif

/*
 * @}
 */

#ifdef __cplusplus
}
#endif


