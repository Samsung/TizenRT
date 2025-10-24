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

#ifdef __cplusplus
extern "C" {
#endif
#include <common/bk_err.h>
#include <driver/int_types.h>

/* @brief Overview about this API header
 *
 */

/**
 * @brief ICU API
 * @defgroup bk_api_icu ICU API group
 * @{
 */

/**
 * @brief     Init the ICU driver
 *
 * This API init the icu modules  :
 *   - Init ICU driver control memory, enable fiq and irq
 *
 * This API should be called before any other ICU APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_icu_driver_init(void);

/**
 * @brief     deinit the ICU driver
 *
 * This API deinit the icu module
 *
 * This API disable all devices' intterrupts
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_icu_driver_deinit(void);

#ifdef CONFIG_ISR_REG_DISABLE
#define bk_int_isr_register(dev, isr, arg)
#define bk_int_isr_unregister(src)
#else

/**
 * @brief     register interrupt service handler
 *
 * @attention 1. If this is called, the default interrupt handler is replaced by the
 *               new handler, the caller is fully responsible for handling the interrupt,
 *               such as, read the interrupt status, clear the interrupt status and
 *               processing the interrupt etc.
 *
 * @param dev  interrupt source device
 * @param isr int service function
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NO_MEM: no memoery
 *    - BK_ERR_INT_DEVICE_NONE: no int device
 *    - others: other errors.
 */
bk_err_t bk_int_isr_register(icu_int_src_t dev, int_group_isr_t isr, void*arg);

/**
 * @brief     unregister interrupt service handler
 *
 * @param dev  interrupt source device
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_INT_DEVICE_NONE:no int device
 *    - others: other errors.
 */
bk_err_t bk_int_isr_unregister(icu_int_src_t src);
#endif

/**
 * @brief     set/change interrupt priority
 *
 * Every interrupt source has a two-level priority, the group priority and the
 * priority. The interrupt source has higher group priority will be schedule
 * first. The interrupt sources has higher priority will be schedule first if
 * they has the same group priority.
 *
 * This API is used to configure the pririty.
 *
 * @param dev  interrupt source device
 * @param isr int service function
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NOT_SUPPORT :not support
 *    - others: other errors.
 */
bk_err_t bk_int_set_priority(icu_int_src_t src, uint32_t int_priority);


/**
 * @brief     set the interrupt group priority
 *
 * Every interrupt source has a two-level priority, the group priority and the
 * priority. The interrupt source has higher group priority will be schedule
 * first. The interrupt sources has higher priority will be schedule first if
 * they has the same group priority.
 *
 * This API is used to configure the group priority of the interrupt source.
 *
 * @attention 1. For ARM9, it has two group priorities which are mapped to
 *               ARM9 FIQ and IRQ. Currently all interrupt source are statically
 *               mapped to the group priority and can't change by this API.
 *               We may support it in future.
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NOT_SUPPORT :not support
 *    - others: other errors.
 */
bk_err_t bk_int_set_group(void);

/**
 * @brief     Register callback for mac ps
 *
 * @param mac_ps_cb  mac ps callback
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_int_register_mac_ps_callback(int_mac_ps_callback_t mac_ps_cb);

#ifdef __cplusplus

}
#endif
