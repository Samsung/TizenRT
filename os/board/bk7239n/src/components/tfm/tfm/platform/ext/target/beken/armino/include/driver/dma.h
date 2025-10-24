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
//

#pragma once

#include <common/bk_include.h>
#include <driver/dma_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the DMA driver
 *
 * This API init the resoure common to all dma channels:
 *   - Init dma driver control memory
 *
 * This API should be called before any other dma APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_driver_init(void);

/**
 * @brief     Deinit the DMA driver
 *
 * This API free all resource related to dma and power down all dma channels.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_driver_deinit(void);

/**
 * @brief     Allocate a DMA channel
 *
 * @attention: This API can only be called in task context, 
 *             - and can't be called in context that interrupt is disabled.
 *
 * This API should be called before any other dma channel APIs.
 *
 * @param user_id DMA channel applicant
 *
 * @return DMA channel id.
 *     -  > DMA_ID_MAX:  no free DMA channel.
 */
dma_id_t bk_dma_alloc(u16 user_id);

/**
 * @brief     Free the DMA channel
 *
 * @attention: This API can only be called in task context, 
 *             - and can't be called in context that interrupt is disabled.
 *
 * @param user_id DMA channel applicant, the same as in bk_dma_alloc.
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_free(u16 user_id, dma_id_t id);

/**
 * @brief     get the user of DMA channel
 *
 * @param id DMA channel
 *
 * @return DMA channel user_id.
 *     -  u32:  high u16 is the CPU_ID, low 16 bits is the applicant_id.
 */
uint32_t bk_dma_user(dma_id_t id);

/**
 * @brief     Init the DMA channel
 *
 * @attention 1. the higher channel priority value, the higher the priority
 *
 * @param id DMA channel
 * @param config DMA configuration
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_DMA_NOT_INIT: DMA driver not init
 *    - BK_ERR_NULL_PARAM: config is NULL
 *    - BK_ERR_DMA_ID: invalid DMA channel
 *    - BK_ERR_DMA_INVALID_ADDR: invalid DMA address
 *    - others: other errors.
 */
bk_err_t bk_dma_init(dma_id_t id, const dma_config_t *config);

/**
 * @brief     Deinit a DMA channel
 *
 * This API deinit the DMA channel:
 *   - Stop the DMA channel
 *   - Reset all configuration of DMA channel to default value
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_deinit(dma_id_t id);

/**
 * @brief     Start a DMA channel
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_start(dma_id_t id);

/**
 * @brief     Stop a DMA channel
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_stop(dma_id_t id);

/**
 * @brief     Transfer data from memory to peripheral
 *
 * @param id DMA channel
 * @param data DMA transfer data
 * @param size data size
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_write(dma_id_t id, const uint8_t *data, uint32_t size);

/**
 * @brief     Transfer data from peripheral to memory
 *
 * @param id DMA channel
 * @param data DMA transfer data
 * @param size data size
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_read(dma_id_t id, uint8_t *data, uint32_t size);

/**
 * @brief     Enable DMA finish intterrup
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_enable_finish_interrupt(dma_id_t id);

/**
 * @brief     Disable DMA finish intterrup
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_disable_finish_interrupt(dma_id_t id);

/**
 * @brief     Enable DMA half finish intterrup
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_enable_half_finish_interrupt(dma_id_t id);

/**
 * @brief     Disable DMA half finish intterrup
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_disable_half_finish_interrupt(dma_id_t id);

/**
 * @brief     Register the interrupt service routine for DMA channel
 *
 * This API regist dma isr callback function.
 *
 * @param id DMA channel
 * @param half_finish_isr DMA half finish callback
 * @param finish_isr DMA finish callback
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_register_isr(dma_id_t id, dma_isr_t half_finish_isr, dma_isr_t finish_isr);

/**
 * @brief     Set DMA transfer length
 *
 * @param id DMA channel
 * @param tran_len DMA transfer length
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_set_transfer_len(dma_id_t id, uint32_t tran_len);

/**
 * @brief     Set DMA source address
 *
 * @attention 1. address should be zero when there is no value, e.g. bk_dma_set_src_addr(1, 0x80210C, 0)
 *
 * @param id DMA channel
 * @param start_addr DMA source start address
 * @param end_addr DMA source end address
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_set_src_addr(dma_id_t id, uint32_t start_addr, uint32_t end_addr);

/**
 * @brief     Set DMA source start address
 *
 * @param id DMA channel
 * @param start_addr DMA source start address
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_set_src_start_addr(dma_id_t id, uint32_t start_addr);

/**
 * @brief     Set DMA dest address
 *
 * @attention 1. address should be zero when there is no value, e.g. bk_dma_set_dest_addr(1, 0x80210C, 0)
 *
 * @param id DMA channel
 * @param start_addr DMA dest start address
 * @param end_addr DMA dest end address
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_set_dest_addr(dma_id_t id, uint32_t start_addr, uint32_t end_addr);

/**
 * @brief     Set DMA dest start address
 *
 * @param id DMA channel
 * @param start_addr DMA dest start address
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_set_dest_start_addr(dma_id_t id, uint32_t start_addr);

/**
 * @brief     Enable DMA source address increase
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_enable_src_addr_increase(dma_id_t id);

/**
 * @brief     Disable DMA source address increase
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_disable_src_addr_increase(dma_id_t id);

/**
 * @brief     Enable DMA source address loop
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_enable_src_addr_loop(dma_id_t id);

/**
 * @brief     Disable DMA source address loop
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_disable_src_addr_loop(dma_id_t id);

/**
 * @brief     Enable DMA dest address increase
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_enable_dest_addr_increase(dma_id_t id);

/**
 * @brief     Disable DMA dest address increase
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_disable_dest_addr_increase(dma_id_t id);

/**
 * @brief     Enable DMA dest address loop
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_enable_dest_addr_loop(dma_id_t id);

/**
 * @brief     Disable DMA dest address loop
 *
 * @param id DMA channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_dma_disable_dest_addr_loop(dma_id_t id);

/**
 * @brief     Get DMA transfer remain length
 *
 * @param id DMA channel
 *
 * @return DMA transfer remain length
 */
uint32_t bk_dma_get_remain_len(dma_id_t id);

/**
 * @brief     Gets the current DMA channel working status
 *
 * @param id DMA channel
 *
 * @return
 *    - 0: Channel idle state
 *    - others: Channel busy state.
 */
uint32_t bk_dma_get_enable_status(dma_id_t id);

#ifdef CONFIG_SPE
bk_err_t bk_dma_set_dest_burst_len(dma_id_t id, dma_burst_len_e len);
uint32_t bk_dma_get_dest_burst_len(dma_id_t id);
bk_err_t bk_dma_set_src_burst_len(dma_id_t id, dma_burst_len_e len);
uint32_t bk_dma_get_src_burst_len(dma_id_t id);

/**
 * @brief     Enable the current DMA channel bus err interrupt
 *
 * @param id DMA channel
 *
 * @return
 *    - 0: enable success state
 *    - others: Channel busy state.
 */
bk_err_t bk_dma_bus_err_int_enable(dma_id_t id);

/**
 * @brief     Disable the current DMA channel bus err interrupt
 *
 * @param id DMA channel
 *
 * @return
 *    - 0: disable success state
 *    - others: Channel busy state.
 */
bk_err_t bk_dma_bus_err_int_disable(dma_id_t id);

/**
 * @brief     Set the current DMA channel dest secure attr
 *
 * @param id DMA channel
 * @param attr DMA secure attr
 *
 * @return
 *    - 0: Channel idle state
 *    - others: Channel busy state.
 */
bk_err_t bk_dma_set_dest_sec_attr(dma_id_t id, dma_sec_attr_t attr);

/**
 * @brief     Set the current DMA channel src secure attr
 *
 * @param id DMA channel
 * @param attr DMA secure attr
 *
 * @return
 *    - 0: Channel idle state
 *    - others: Channel busy state.
 */
bk_err_t bk_dma_set_src_sec_attr(dma_id_t id, dma_sec_attr_t attr);
#endif

#if (CONFIG_SPE)
/**
 * @brief     Set the all DMA channel secure attr
 *
 * @param id DMA channel
 * @param attr DMA secure attr
 *
 * @return
 *    - 0: Channel idle state
 *    - others: Channel busy state.
 */
bk_err_t bk_dma_set_sec_attr(dma_id_t id, dma_sec_attr_t attr);

/**
 * @brief     Set the all DMA channel privileded attr
 *
 * @param id DMA channel
 * @param attr DMA privileged attr
 *
 * @return
 *    - 0: Channel idle state
 *    - others: Channel busy state.
 */
bk_err_t bk_dma_set_privileged_attr(dma_id_t id, dma_sec_attr_t attr);
#endif

#ifdef __cplusplus
}
#endif

