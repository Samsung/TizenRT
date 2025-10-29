// Copyright 2020-2025 Beken
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

#include <common/bk_include.h>
#include <driver/ckmn_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/* @brief Overview about this API header
 *
 */

/**
 * @brief CKMN API
 * @defgroup bk_api_ckmn CKMN API group
 * @{
 */

/**
 * @brief     Init the CKMN driver
 *
 * This API init the resoure common:
 *   - Init CKMN driver control memory
 *
 * @attention 1. This API should be called before any other CKMN APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_driver_init(void);

/**
 * @brief     Deinit the CKMN driver
 *
 * This API free all resource related to CKMN and disable CKMN.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_driver_deinit(void);

/**
 * @brief     Register the interrupt service routine for CKMN
 *
 * This API regist ckmn isr callback function.
 *
 * @param int_type register interrupt type
 * @param int_isr ckmn callback
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_register_isr(ckmn_int_type_t int_type, ckmn_isr_t int_isr);

/**
 * @brief     Unregister the interrupt service routine for CKMN
 *
 * This API regist ckmn isr callback function.
 *
 * @param int_type register interrupt type
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_unregister_isr(ckmn_int_type_t int_type);

/**
 * @brief     ckmn soft reset
 *
 * This API reset ckmn module
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_soft_reset(void);

/**
 * @brief    Set the number of counts for RC32K accuracy statistics
 *
 * This API 26m digital clock measurement 32k analog clock.
 *
 * @param count number of clock cycles
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_set_rc32k_count(uint32_t count);

/**
 * @brief    Get the number of counts for RC32K accuracy statistics
 *
 * This API 26m digital clock measurement 32k analog clock.
 *
 * @return
 *    - count number of clock cycles
 */
bk_err_t bk_ckmn_get_rc32k_count(void);

/**
 * @brief    enable RC32K accuracy statistics
 *
 * This API 26m digital clock measurement 32k analog clock.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_ckest_enable(void);

/**
 * @brief    disable RC32K accuracy statistics
 *
 * This API 26m digital clock measurement 32k analog clock.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_ckest_disable(void);

/**
 * @brief    check if ckest is busy
 *
 * This API checks if chest is on measurement.
 *
 * @return
 *    - BK_OK: ckest is ready
 *    - others: ckest is busy.
 */
bk_err_t bk_ckmn_ckest_check_busy(void);

/**
 * @brief    get ther number of mappings of RC32K accuracy statistics in the 26m domain
 *
 * This API 26m digital clock measurement 32k analog clock.
 *
 * @return
 *    - ther number of mappings
 */
bk_err_t bk_ckmn_get_rc26m_count(void);

/**
 * @brief    Set 26m clock accuracy related targets
 *
 * This API auto clock switching.
 *
 * @param target accuracy percentage
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_set_26m_target(ckmn_acc_t target);

/**
 * @brief    enable 26m clock auto switching
 *
 * This API auto clock switching..
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_enable_autosw_26m(void);

/**
 * @brief   disable 26m clock auto switching
 *
 * This API auto clock switching..
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_disable_autosw_26m(void);

/**
 * @brief   enable 26m clock accuracy detection and enable interrupt
 *
 * This API auto clock switching..
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_enable_corr_26m(void);

/**
 * @brief   disable 26m clock accuracy detection and disable interrupt
 *
 * This API auto clock switching..
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_disable_corr_26m(void);

/**
 * @brief    Set 32k clock accuracy related targets
 *
 * This API auto clock switching.
 *
 * @param target accuracy percentage
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_set_32k_target(ckmn_acc_t target);

/**
 * @brief    enable 32k clock auto switching
 *
 * This API auto clock switching..
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_enable_autosw_32k(void);

/**
 * @brief    disable 32k clock auto switching
 *
 * This API auto clock switching..
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_disable_autosw_32k(void);

/**
 * @brief   enable 32k clock accuracy detection and enable interrupt
 *
 * This API clock accuracy detection..
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_enable_corr_32k(void);

/**
 * @brief   disable 32k clock accuracy detection and disable interrupt
 *
 * This API clock accuracy detection..
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_disable_corr_32k(void);

/**
 * @brief    enable 26m clock auto switching interrupt
 *
 * This API clock accuracy detection interrupt
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_cor26m_intr_enable(void);

/**
 * @brief    disable 26m clock auto switching interrupt
 *
 * This API clock accuracy detection interrupt
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_cor26m_intr_disable(void);

/**
 * @brief    enable 32k clock auto switching interrupt
 *
 * This API clock accuracy detection interrupt
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_cor32k_intr_enable(void);

/**
 * @brief    disable 32k clock auto switching interrupt
 *
 * This API clock accuracy detection interrupt
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_cor32k_intr_disable(void);

/**
 * @brief    Get all interrput status
 *
 * This API clock accuracy detection interrupt
 *
 * @return
 *    - all interrupt value
 */
bk_err_t bk_ckmn_get_intr_status(void);

/**
 * @brief    Get chest interrput status
 *
 * This API clock accuracy detection interrupt
 *
 * @return
 *    - chest interrupt value
 */
bk_err_t bk_ckmn_get_ckest_intr_status(void);

/**
 * @brief    Get cor26m interrput status
 *
 * This API clock accuracy detection interrupt
 *
 * @return
 *    - cor26m interrupt value
 */
bk_err_t bk_ckmn_get_cor26m_intr_status(void);

/**
 * @brief    Get 32k interrput status
 *
 * This API clock accuracy detection interrupt
 *
 * @return
 *    - 32k interrupt value
 */
bk_err_t bk_ckmn_get_cor32k_intr_status(void);

/**
 * @brief    clear ckest interrput status
 *
 * This API clock accuracy detection interrupt
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_clear_ckest_intr_status(void);

/**
 * @brief    clear cor26m interrput status
 *
 * This API clock accuracy detection interrupt
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_clear_cor26m_intr_status(void);

/**
 * @brief    clear cor32k interrput status
 *
 * This API clock accuracy detection interrupt
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_ckmn_clear_cor32k_intr_status(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


