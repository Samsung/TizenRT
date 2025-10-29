/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __FLASH_OTP_NV_COUNTERS_BACKEND_H__
#define __FLASH_OTP_NV_COUNTERS_BACKEND_H__

#include "cmsis_compiler.h"

#define FLASH_NV_COUNTER_AM 3

#ifdef __cplusplus
extern "C" {
#endif
#define OTP_NV_COUNTERS_INITIALIZED 0xC0DE8112U
#define OTP_NV_COUNTERS_IS_VALID    0x3072C0DEU

__PACKED_STRUCT flash_otp_nv_counters_region_t {
    /* Must be the first item */
    uint32_t init_value;

#ifdef PLATFORM_DEFAULT_OTP
    __PACKED_STRUCT {
        uint8_t huk[32];
        uint8_t iak[32];
        uint8_t iak_len[4];
        uint8_t iak_type[4];
        uint8_t iak_id[32];

        uint8_t boot_seed[32];
        uint8_t lcs[4];
        uint8_t implementation_id[32];
        uint8_t hw_version[32];
        uint8_t verification_service_url[32];
        uint8_t profile_definition[32];

#ifdef BL2
        uint8_t bl2_rotpk_0[32];
        uint8_t bl2_rotpk_1[32];

        uint8_t bl2_nv_counter_0[64];
        uint8_t bl2_nv_counter_1[64];

        uint8_t bl2_nv_counter_2[64];
        uint8_t bl2_rotpk_2[32];
#endif /* BL2 */

#ifdef BL1
        uint8_t bl1_rotpk_0[32];
        uint8_t bl1_nv_counter_0[16];
#endif /* BL1 */

        uint8_t entropy_seed[64];

        uint8_t secure_debug_pk[32];
    };
#endif /* PLATFORM_DEFAULT_OTP */

#ifdef PLATFORM_DEFAULT_NV_COUNTERS
    __PACKED_STRUCT {
        uint32_t flash_nv_counters[FLASH_NV_COUNTER_AM];
    };
#endif /* PLATFORM_DEFAULT_NV_COUNTERS */

    /* Must be last item, so that it can be written separately after the main
     * write operation has succeeded
     */
    uint32_t is_valid;
};

/**
 * \brief                               Initialise the OTP / NV counter flash
 *                                      area.
 *
 * \return                              TFM_PLAT_ERR_SUCCESS if the
 *                                      initialization succeeds, otherwise
 *                                      TFM_PLAT_ERR_SYSTEM_ERR
 */
enum tfm_plat_err_t init_otp_nv_counters_flash(void);

/**
 * \brief                               Reads the OTP / NV counter area at the
 *                                      given offset into the buffer.
 *
 * \param[in]  offset                   offset at which to read from.
 * \param[out] data                     buffer into which to copy the data.
 * \param[in]  cnt                      number of bytes to read. Must not be
 *                                      larger than the size of the buffer.
 *
 * \retval TFM_PLAT_ERR_SUCCESS         The data is read successfully.
 * \retval TFM_PLAT_ERR_INVALID_INPUT   An input parameter has an invalid value.
 * \retval TFM_PLAT_ERR_SYSTEM_ERR      An unspecified error occurred.
 */
enum tfm_plat_err_t read_otp_nv_counters_flash(uint32_t offset, void *data, uint32_t cnt);

/**
 * \brief                               Writes from the given buffer into the
 *                                      OTP / NV counter area at the given
 *                                      offset.
 *
 * \param[in]  offset                   offset at which to write to.
 * \param[out] data                     buffer from which to copy the data.
 * \param[in]  cnt                      number of bytes to write. Should not be
 *                                      larger than the size of the buffer.
 *
 * \retval TFM_PLAT_ERR_SUCCESS         The data is written successfully.
 * \retval TFM_PLAT_ERR_INVALID_INPUT   An input parameter has an invalid value.
 * \retval TFM_PLAT_ERR_SYSTEM_ERR      An unspecified error occurred.
 */
enum tfm_plat_err_t write_otp_nv_counters_flash(uint32_t offset, const void *data, uint32_t cnt);

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_OTP_NV_COUNTERS_BACKEND_H__ */
