/**
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __SECURE_BOOT_CORE_H__
#define __SECURE_BOOT_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal.h"

typedef void *sec_boot_ctx_t;

typedef struct _sec_boot_img_info_t {
    hal_addr_t static_addr;
    hal_addr_t loading_addr;
    hal_addr_t entry_addr;
    size_t size;
    bool img_enc_enable;
} sec_boot_img_info_t;

/**
 * @ingroup secure_boot_core
 * @brief	sec_boot_init function, the first function of ROM code library,
 * initial the resources for ROM code, and get the secure boot context
 * @param[out] sec_boot_ctx secure boot context
 * @return	HAL_OK on success. error codes otherwise
 */

hal_ret_t sec_boot_init(sec_boot_ctx_t *sec_boot_ctx);

/**
 * @ingroup secure_boot_core
 * @brief	sec_boot_cleanup function
 * @param[in] sec_boot_ctx secure boot context
 * @return	HAL_OK on success. error codes otherwise
 */

hal_ret_t sec_boot_cleanup(sec_boot_ctx_t sec_boot_ctx);

/**
 * @ingroup	secure_boot_core
 * @brief	sec_boot_verify_imgs function, this is the main function
 * to do the secureboot authentication, before call this function,
 * you should call the "sec_boot_init" function to initial the context.
 * @param[in] sec_boot_ctx secure boot context
 * @return	HAL_OK on success. error codes otherwise
 */

hal_ret_t sec_boot_verify_imgs(sec_boot_ctx_t sec_boot_ctx);

/**
 * @ingroup	secure_boot_core
 * @brief	sec_boot_get_img_num function, this function should call
 * after "sec_boot_verify_imgs" return seccuss, it is used to get the
 * total image number.
 * @param[out]  img_num  image number
 * @param[in]   sec_boot_ctx  sec boot context
 * @return	HAL_OK on success. error codes otherwise
 */
hal_ret_t sec_boot_get_img_num(sec_boot_ctx_t sec_boot_ctx, uint32_t *img_num);

/**
 * @ingroup	secure_boot_core
 * @brief	sec_boot_get_img_info function, this function should call
 * after "sec_boot_verify_imgs" return seccuss, it is used to get the
 * image info according to the input image number.
 * @param[out]  img_info  image info structure
 * @param[in]   sec_boot_ctx  sec boot context
 * @param[in]   img_idx  image index number
 * @return	HAL_OK on success. error codes otherwise
 */

hal_ret_t sec_boot_get_img_info(sec_boot_ctx_t sec_boot_ctx,
                                uint32_t img_idx,
                                sec_boot_img_info_t *img_info);

#ifdef __cplusplus
}
#endif

#endif

/*************************** The End Of File*****************************/
