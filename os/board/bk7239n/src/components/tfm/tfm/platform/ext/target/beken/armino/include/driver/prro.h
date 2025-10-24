// Copyright 2022-2023 Beken
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

#include <driver/prro_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the PRRO driver
 *
 * This API init the resoure common:
 *   - Init PRRO driver control memory
 *
 * @attention 1. This API should be called before any other PRRO APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_prro_driver_init(void);

/**
 * @brief     Deinit the PRRO driver
 *
 * This API free all resource related to PRRO.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_prro_driver_deinit(void);

/**
 * @brief     Set privilege attribute of specified peripheral
 *
 * @param dev the peripheral
 * @param privilege_type the privilege type
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_privilege(prro_dev_t dev, prro_privilege_type_t privilege_type);

/**
 * @brief     Set privilege attribute of all AHB peripherals
 *
 * @param ahb_dev_privilege_bits the privilege bitmap of AHB peripherals,
 *        each bit indicates a devices, 1 means PRRO_PRIVILEGED, and 0 means PRRO_PRIVILEGED.
 *        refers to the PRRO manual for bits and peripheral map.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_ahb_dev_privilege(uint32_t ahb_dev_privilege_bits);

/**
 * @brief     Set privilege attribute of all APB peripherals
 *
 * @param apb_dev_privilege_bits the privilege bitmap of APB peripherals,
 *        each bit indicates a devices, 1 means PRRO_PRIVILEGED, and 0 means PRRO_PRIVILEGED.
 *        refers to the PRRO manual for bits and peripheral map.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_apb_dev_privilege(uint32_t apb_dev_privilege_bits);

/**
 * @brief     Set peripherals secure attribute
 *
 * @param dev the peripherals
 * @param secure_type the secure type
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_secure(prro_dev_t dev, prro_secure_type_t secure_type);

/**
 * @brief     Get peripherals secure attribute
 *
 * @param dev the peripherals
 *
 * @return the secure type
 */
prro_secure_type_t bk_prro_get_secure(prro_dev_t dev);

/**
 * @brief     Set secure attribute of all GPIOs
 *
 * @param gpio_0_31_secure_bits the secure bitmap of GPIO 0 to 31,
 *        each bit indicates a GPIO, 1 means PRRO_SECURE, and 0 means PRRO_NON_SECURE.
 *        refers to the PRRO manual for bits and peripheral map.
 * @param gpio_31_63_secure_bits the privilege bitmap of GPIO 32 to 63,
 *        if the related GPIO ID is not supported, the bit will be skipped.
 * @param secure_type the secure type, with each element must be PRRO_SECURE or PRRO_NON_SECURE
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_gpios_secure(uint32_t gpio_0_31_secure_bits, uint32_t gpio_32_63_secure_bits);

/**
 * @brief     Set secure attribute of all AHB peripherals
 *
 * @param ahb_dev_secure_bits the privilege bitmap of AHB peripherals,
 *        each bit indicates a AHB peripherals, 1 means PRRO_SECURE, and 0 means PRRO_NON_SECURE.
 *        refers to the PRRO manual for bits and peripheral map.

 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_ahb_dev_secure(uint32_t ahb_dev_secure_bits);

/**
 * @brief     Set secure attribute of all APB peripherals
 *
 * @param apb_dev_secure_bits the privilege bitmap of APB peripherals,
 *        each bit indicates a APB peripherals, 1 means PRRO_SECURE, and 0 means PRRO_NON_SECURE.
 *        refers to the PRRO manual for bits and peripheral map.

 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_apb_dev_secure(uint32_t apb_dev_secure_bits);

/**
 * @brief     Set secure attribute of all hnonsec peripherals
 *
 * @param hnonsec_dev_secure_bits the privilege bitmap of hnonsec peripherals,
 *        each bit indicates a APB peripherals, 1 means PRRO_SECURE, and 0 means PRRO_NON_SECURE.
 *        refers to the PRRO manual for bits and peripheral map.

 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_hnonsec_dev_secure(uint32_t hnonsec_dev_bits);

/**
 * @brief     Set hardware comparison condition
 *
 * @param prro_cmp_id hardware comparison id
 * @param start the start address, the hardware comparison is triggered when
 *        CPU fetch the first instruction from address start to end
 * @param end the end address

 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_hw_cmp_condition(prro_cmp_id_t it, uint32_t start, uint32_t end);

/**
 * @brief     Set hardware comparison source value
 *
 * @param prro_cmp_id hardware comparison id
 * @param src the hardware comparison source value

 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_hw_cmp_src(prro_cmp_id_t id, uint32_t src);

/**
 * @brief     Set hardware comparison destination value
 *
 * @param prro_cmp_id hardware comparison id
 * @param src the hardware comparison destination value

 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MPC_DRIVER_NOT_INIT: MPC driver is not init
 */
bk_err_t bk_prro_set_hw_cmp_dst(prro_cmp_id_t it, uint32_t dst);

#ifdef __cplusplus
}
#endif

