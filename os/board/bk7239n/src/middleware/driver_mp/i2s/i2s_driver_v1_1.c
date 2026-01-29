/*
 * Copyright 2020-2025 Beken
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <common/bk_include.h>
#include "i2s_hal.h"
#include "i2s_driver.h"
#include "sys_driver.h"
#include "clock_driver.h"
#include <soc/soc.h>
#include <driver/i2s_types.h>
#include <os/os.h>
#include <os/mem.h>
#include <driver/int.h>
#include "gpio_driver.h"
#include <driver/gpio.h>
#include <modules/pm.h>
#include <driver/audio_ring_buff.h>
#include "bk_general_dma.h"
#include <bk_misc.h>
#include <tinyara/mm/mm.h>


//#define  i2s_driver_mem_malloc(size)  kmm_zalloc(size)
//#define  i2s_driver_mem_free(ptr)  kmm_free(ptr)
#define  i2s_driver_mem_zalloc(size)  os_zalloc(size)
#define  i2s_driver_mem_free(ptr)  os_free(ptr)

static bool s_i2s_driver_is_init = false;
#if (SOC_I2S_UNIT_NUM > 1)
static bool s_i2s1_driver_is_init = false;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
static bool s_i2s2_driver_is_init = false;
#endif
static i2s_callback_t s_i2s_isr[SOC_I2S_ISR_NUM] = {NULL};
#if (SOC_I2S_UNIT_NUM > 1)
static i2s_callback_t s_i2s1_isr[SOC_I2S_ISR_NUM] = {NULL};
#endif
#if (SOC_I2S_UNIT_NUM > 2)
static i2s_callback_t s_i2s2_isr[SOC_I2S_ISR_NUM] = {NULL};
#endif
static i2s_drv_info_t *i2s_drv_info = NULL;
#if (SOC_I2S_UNIT_NUM > 1)
static i2s_drv_info_t *i2s1_drv_info = NULL;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
static i2s_drv_info_t *i2s2_drv_info = NULL;
#endif
static dma_id_t i2s_dma_idx = 0;

static void i2s_isr(void);
static void i2s_chl1_tx_dma_finish_isr(void);
static void i2s_chl1_rx_dma_finish_isr(void);
static void i2s_chl2_tx_dma_finish_isr(void);
static void i2s_chl2_rx_dma_finish_isr(void);
static void i2s_chl3_tx_dma_finish_isr(void);
static void i2s_chl3_rx_dma_finish_isr(void);
static bk_err_t i2s_dma_config(dma_id_t dma_id, uint32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len, i2s_channel_id_t chl_id, i2s_txrx_type_t type, i2s_data_handle_cb data_handle_cb);
extern void delay(int num); /* TODO: fix me */
bk_err_t bk_i2s_set_samp_rate(i2s_samp_rate_t samp_rate);
bk_err_t bk_i2s_chl_deinit(i2s_channel_id_t chl, i2s_txrx_type_t type);

#define DMA_CARRY_I2S_RINGBUF_SAFE_INTERVAL    (8)

#define I2S_RETURN_ON_NOT_INIT() do {\
        if (!s_i2s_driver_is_init) {\
            return BK_ERR_I2S_NOT_INIT;\
        }\
    } while(0)

#if (SOC_I2S_UNIT_NUM > 1)
#define I2S1_RETURN_ON_NOT_INIT() do {\
        if (!s_i2s1_driver_is_init) {\
            return BK_ERR_I2S_NOT_INIT;\
        }\
    } while(0)
#endif

#if (SOC_I2S_UNIT_NUM > 2)
#define I2S2_RETURN_ON_NOT_INIT() do {\
        if (!s_i2s2_driver_is_init) {\
            return BK_ERR_I2S_NOT_INIT;\
        }\
    } while(0)
#endif

#define I2S_RETURN_ON_INVALID_ISR_ID(isr_id) do {\
        if ((isr_id) >= SOC_I2S_ISR_NUM) {\
            return BK_ERR_I2S_ISR_ID;\
        }\
    } while(0)

/* ============================================================================
 * Common Utility Functions
 * ============================================================================ */

/**
 * @brief Initialize channel configuration for legacy function
 * @param drv_info Driver info pointer
 * @param chl Channel ID
 * @param type TX or RX type
 * @param buff_size Buffer size
 * @param data_handle_cb Data handling callback
 * @param rb Ring buffer context
 * @return BK_OK on success, error code otherwise
 */
static bk_err_t i2s_init_channel_legacy(i2s_drv_info_t *drv_info, i2s_channel_id_t chl, i2s_txrx_type_t type, uint32_t buff_size, i2s_data_handle_cb data_handle_cb, RingBufferContext **rb)
{
    bk_err_t ret = BK_OK;
    i2s_chl_cfg_t *chl_cfg = NULL;
    i2s_txrx_cfg_t *txrx_cfg = NULL;

    /* Get or create channel configuration */
    switch (chl) {
    case I2S_CHANNEL_1:
        if (drv_info->chl1_cfg == NULL) {
            chl_cfg = (i2s_chl_cfg_t *)i2s_driver_mem_zalloc(sizeof(i2s_chl_cfg_t));
            if (chl_cfg == NULL) {
                I2S_LOGE("malloc chl1_cfg fail\n");
                return BK_FAIL;
            }
            drv_info->chl1_cfg = chl_cfg;
        } else {
            chl_cfg = drv_info->chl1_cfg;
        }
        break;
    case I2S_CHANNEL_2:
        if (drv_info->chl2_cfg == NULL) {
            chl_cfg = (i2s_chl_cfg_t *)i2s_driver_mem_zalloc(sizeof(i2s_chl_cfg_t));
            if (chl_cfg == NULL) {
                I2S_LOGE("malloc chl2_cfg fail\n");
                return BK_FAIL;
            }
            drv_info->chl2_cfg = chl_cfg;
        } else {
            chl_cfg = drv_info->chl2_cfg;
        }
        break;
    case I2S_CHANNEL_3:
        if (drv_info->chl3_cfg == NULL) {
            chl_cfg = (i2s_chl_cfg_t *)i2s_driver_mem_zalloc(sizeof(i2s_chl_cfg_t));
            if (chl_cfg == NULL) {
                I2S_LOGE("malloc chl3_cfg fail\n");
                return BK_FAIL;
            }
            drv_info->chl3_cfg = chl_cfg;
        } else {
            chl_cfg = drv_info->chl3_cfg;
        }
        break;
    default:
        return BK_ERR_I2S_PARAM;
    }

    /* Allocate TX/RX configuration */
    if (type == I2S_TXRX_TYPE_TX) {
        txrx_cfg = (i2s_txrx_cfg_t *)i2s_driver_mem_zalloc(sizeof(i2s_txrx_cfg_t));
        if (txrx_cfg == NULL) {
            I2S_LOGE("malloc tx_cfg fail\n");
            return BK_FAIL;
        }
        txrx_cfg->dma_id = DMA_ID_MAX;
        txrx_cfg->state = I2S_TXRX_STATE_NULL;
        chl_cfg->tx_cfg = txrx_cfg;
    } else {
        txrx_cfg = (i2s_txrx_cfg_t *)i2s_driver_mem_zalloc(sizeof(i2s_txrx_cfg_t));
        if (txrx_cfg == NULL) {
            I2S_LOGE("malloc rx_cfg fail\n");
            return BK_FAIL;
        }
        txrx_cfg->dma_id = DMA_ID_MAX;
        txrx_cfg->state = I2S_TXRX_STATE_NULL;
        chl_cfg->rx_cfg = txrx_cfg;
    }

    /* Configure buffer and ring buffer */
    txrx_cfg->buff_size = buff_size;
    if (type == I2S_TXRX_TYPE_TX) {
        txrx_cfg->buff_addr = (uint8_t *)i2s_driver_mem_zalloc(buff_size + DMA_CARRY_I2S_RINGBUF_SAFE_INTERVAL);
    } else {
        txrx_cfg->buff_addr = (uint8_t *)i2s_driver_mem_zalloc(buff_size + DMA_CARRY_I2S_RINGBUF_SAFE_INTERVAL);
    }
    
    if (txrx_cfg->buff_addr == NULL) {
        I2S_LOGE("malloc buff_addr fail\n");
        return BK_FAIL;
    }

    txrx_cfg->rb = (RingBufferContext *)i2s_driver_mem_zalloc(sizeof(RingBufferContext));
    if (txrx_cfg->rb == NULL) {
        I2S_LOGE("malloc rb fail\n");
        return BK_FAIL;
    }
    *rb = txrx_cfg->rb;

    /* Initialize DMA driver */
    ret = bk_dma_driver_init();
    if (ret != BK_OK) {
        I2S_LOGE("dma driver init failed\n");
        return BK_FAIL;
    }

    /* Allocate DMA channel */
    txrx_cfg->dma_id = bk_dma_alloc(DMA_DEV_I2S);
    if ((txrx_cfg->dma_id < DMA_ID_0) || (txrx_cfg->dma_id >= DMA_ID_MAX)) {
        I2S_LOGE("malloc dma fail\n");
        return BK_FAIL;
    }

    /* Configure DMA */
    ret = i2s_dma_config(txrx_cfg->dma_id, (uint32_t *)txrx_cfg->buff_addr, 
                        buff_size + DMA_CARRY_I2S_RINGBUF_SAFE_INTERVAL, 
                        buff_size/2, chl, type, data_handle_cb);
    if (ret != BK_OK) {
        I2S_LOGE("dma config fail\n");
        return BK_FAIL;
    }

    /* Initialize ring buffer */
    ring_buffer_init(txrx_cfg->rb, txrx_cfg->buff_addr, 
                    buff_size + DMA_CARRY_I2S_RINGBUF_SAFE_INTERVAL, 
                    txrx_cfg->dma_id, 
                    (type == I2S_TXRX_TYPE_TX) ? RB_DMA_TYPE_READ : RB_DMA_TYPE_WRITE);

    /* Set callback and state */
    txrx_cfg->data_handle_cb = data_handle_cb;
    txrx_cfg->state = I2S_TXRX_STATE_IDLE;

    return BK_OK;
}

/**
 * @brief Validate I2S index and get corresponding driver info
 * @param i2s_index I2S index (0, 1, 2)
 * @param drv_info Output parameter for driver info pointer
 * @return BK_OK if valid, error code otherwise
 */
static bk_err_t i2s_validate_and_get_drv_info(uint8_t i2s_index, i2s_drv_info_t **drv_info)
{
    switch (i2s_index) {
    case 0:
        if (!s_i2s_driver_is_init) {
            return BK_ERR_I2S_NOT_INIT;
        }
        *drv_info = i2s_drv_info;
        break;

#if (SOC_I2S_UNIT_NUM > 1)
    case 1:
        if (!s_i2s1_driver_is_init) {
            return BK_ERR_I2S_NOT_INIT;
        }
        *drv_info = i2s1_drv_info;
        break;
#endif

#if (SOC_I2S_UNIT_NUM > 2)
    case 2:
        if (!s_i2s2_driver_is_init) {
            return BK_ERR_I2S_NOT_INIT;
        }
        *drv_info = i2s2_drv_info;
        break;
#endif

    default:
        return BK_ERR_I2S_PARAM;
    }
    return BK_OK;
}

/**
 * @brief Validate I2S index and get corresponding ISR callback array
 * @param i2s_index I2S index (0, 1, 2)
 * @param isr_cb Output parameter for ISR callback array
 * @return BK_OK if valid, error code otherwise
 */
static bk_err_t i2s_validate_and_get_isr_cb(uint8_t i2s_index, i2s_callback_t **isr_cb)
{
    switch (i2s_index) {
    case 0:
        if (!s_i2s_driver_is_init) {
            return BK_ERR_I2S_NOT_INIT;
        }
        *isr_cb = s_i2s_isr;
        break;
#if (SOC_I2S_UNIT_NUM > 1)
    case 1:
        if (!s_i2s1_driver_is_init) {
            return BK_ERR_I2S_NOT_INIT;
        }
        *isr_cb = s_i2s1_isr;
        break;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
    case 2:
        if (!s_i2s2_driver_is_init) {
            return BK_ERR_I2S_NOT_INIT;
        }
        *isr_cb = s_i2s2_isr;
        break;
#endif
    default:
        return BK_ERR_I2S_PARAM;
    }
    return BK_OK;
}

/**
 * @brief Validate I2S index only (for simple operations)
 * @param i2s_index I2S index (0, 1, 2)
 * @return BK_OK if valid, error code otherwise
 */
static bk_err_t i2s_validate_index(uint8_t i2s_index)
{
    switch (i2s_index) {
    case 0:
        if (!s_i2s_driver_is_init) {
            return BK_ERR_I2S_NOT_INIT;
        }
        break;
#if (SOC_I2S_UNIT_NUM > 1)
    case 1:
        if (!s_i2s1_driver_is_init) {
            return BK_ERR_I2S_NOT_INIT;
        }
        break;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
    case 2:
        if (!s_i2s2_driver_is_init) {
            return BK_ERR_I2S_NOT_INIT;
        }
        break;
#endif
    default:
        return BK_ERR_I2S_PARAM;
    }
    return BK_OK;
}

/**
 * @brief Stop a specific channel's TX or RX operation
 * @param chl_cfg Channel configuration pointer
 * @param type TX or RX type
 * @return BK_OK if successful, error code otherwise
 */
static bk_err_t i2s_stop_channel(i2s_chl_cfg_t *chl_cfg, i2s_txrx_type_t type)
{
    i2s_txrx_cfg_t *txrx_cfg;
    
    if (!chl_cfg) {
        return BK_OK; /* Channel not configured, nothing to stop */
    }
    
    if (type == I2S_TXRX_TYPE_TX) {
        txrx_cfg = chl_cfg->tx_cfg;
    } else {
        txrx_cfg = chl_cfg->rx_cfg;
    }
    
    if (!txrx_cfg || txrx_cfg->state != I2S_TXRX_STATE_START) {
        return BK_OK; /* Not started, nothing to stop */
    }
    
    bk_dma_stop(txrx_cfg->dma_id);
    txrx_cfg->state = I2S_TXRX_STATE_STOP;
    
    return BK_OK;
}

/**
 * @brief Stop all channels for a driver info
 * @param drv_info Driver info pointer
 * @return BK_OK if successful, error code otherwise
 */
static bk_err_t i2s_stop_all_channels(i2s_drv_info_t *drv_info)
{
    bk_err_t ret;
    
    /* Stop all TX channels */
    ret = i2s_stop_channel(drv_info->chl1_cfg, I2S_TXRX_TYPE_TX);
    if (ret != BK_OK) return ret;
    
    ret = i2s_stop_channel(drv_info->chl1_cfg, I2S_TXRX_TYPE_RX);
    if (ret != BK_OK) return ret;
    
    ret = i2s_stop_channel(drv_info->chl2_cfg, I2S_TXRX_TYPE_TX);
    if (ret != BK_OK) return ret;
    
    ret = i2s_stop_channel(drv_info->chl2_cfg, I2S_TXRX_TYPE_RX);
    if (ret != BK_OK) return ret;
    
    ret = i2s_stop_channel(drv_info->chl3_cfg, I2S_TXRX_TYPE_TX);
    if (ret != BK_OK) return ret;
    
    ret = i2s_stop_channel(drv_info->chl3_cfg, I2S_TXRX_TYPE_RX);
    if (ret != BK_OK) return ret;
    
    return BK_OK;
}

/**
 * @brief Get I2S data address based on index and channel
 * @param i2s_index I2S index (0, 1, 2)
 * @param chl_id Channel ID
 * @return I2S data address
 */
static uint32_t i2s_get_data_addr(uint8_t i2s_index, i2s_channel_id_t chl_id)
{
    switch (chl_id) {
    case I2S_CHANNEL_1:
        switch (i2s_index) {
        case 0: return I2S_PCM_DAT_ADDR;
#if (SOC_I2S_UNIT_NUM > 1)
        case 1: return I2S1_PCM_DAT_ADDR;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
        case 2: return I2S2_PCM_DAT_ADDR;
#endif
        default: return 0;
        }
    case I2S_CHANNEL_2:
        switch (i2s_index) {
        case 0: return I2S_PCM_DAT2_ADDR;
#if (SOC_I2S_UNIT_NUM > 1)
        case 1: return I2S1_PCM_DAT2_ADDR;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
        case 2: return I2S2_PCM_DAT2_ADDR;
#endif
        default: return 0;
        }
    case I2S_CHANNEL_3:
        switch (i2s_index) {
        case 0: return I2S_PCM_DAT3_ADDR;
#if (SOC_I2S_UNIT_NUM > 1)
        case 1: return I2S1_PCM_DAT3_ADDR;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
        case 2: return I2S2_PCM_DAT3_ADDR;
#endif
        default: return 0;
        }
    default:
        return 0;
    }
}

/**
 * @brief Get DMA device for I2S RX based on index and channel
 * @param i2s_index I2S index (0, 1, 2)
 * @param chl_id Channel ID
 * @return DMA device ID
 */
static dma_dev_t i2s_get_rx_dma_dev(uint8_t i2s_index, i2s_channel_id_t chl_id)
{
    switch (chl_id) {
    case I2S_CHANNEL_1:
        switch (i2s_index) {
        case 0: return DMA_DEV_I2S_RX;
#if (SOC_I2S_UNIT_NUM > 1)
        case 1: return DMA_DEV_I2S1_RX;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
        case 2: return DMA_DEV_I2S2_RX;
#endif
        default: return DMA_DEV_MAX;
        }
    case I2S_CHANNEL_2:
        switch (i2s_index) {
        case 0: return DMA_DEV_I2S_RX_CH1;
#if (SOC_I2S_UNIT_NUM > 1)
        case 1: return DMA_DEV_I2S1_RX;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
        case 2: return DMA_DEV_I2S2_RX;
#endif
        default: return DMA_DEV_MAX;
        }
    case I2S_CHANNEL_3:
        switch (i2s_index) {
        case 0: return DMA_DEV_I2S_RX_CH2;
#if (SOC_I2S_UNIT_NUM > 1)
        case 1: return DMA_DEV_I2S1_RX;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
        case 2: return DMA_DEV_I2S2_RX;
#endif
        default: return DMA_DEV_MAX;
        }
    default:
        return DMA_DEV_MAX;
    }
}

/**
 * @brief Get DMA device for I2S TX based on index and channel
 * @param i2s_index I2S index (0, 1, 2)
 * @param chl_id Channel ID
 * @return DMA device ID
 */
static dma_dev_t i2s_get_tx_dma_dev(uint8_t i2s_index, i2s_channel_id_t chl_id)
{
    switch (chl_id) {
    case I2S_CHANNEL_1:
        switch (i2s_index) {
        case 0: return DMA_DEV_I2S;
#if (SOC_I2S_UNIT_NUM > 1)
        case 1: return DMA_DEV_I2S1;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
        case 2: return DMA_DEV_I2S2;
#endif
        default: return DMA_DEV_MAX;
        }
    case I2S_CHANNEL_2:
        switch (i2s_index) {
        case 0: return DMA_DEV_I2S_CH1;
#if (SOC_I2S_UNIT_NUM > 1)
        case 1: return DMA_DEV_I2S1;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
        case 2: return DMA_DEV_I2S2;
#endif
        default: return DMA_DEV_MAX;
        }
    case I2S_CHANNEL_3:
        switch (i2s_index) {
        case 0: return DMA_DEV_I2S_CH2;
#if (SOC_I2S_UNIT_NUM > 1)
        case 1: return DMA_DEV_I2S1;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
        case 2: return DMA_DEV_I2S2;
#endif
        default: return DMA_DEV_MAX;
        }
    default:
        return DMA_DEV_MAX;
    }
}

/**
 * @brief Register DMA ISR for specific channel and type
 * @param dma_id DMA ID
 * @param chl_id Channel ID
 * @param type TX or RX type
 * @return BK_OK if successful, error code otherwise
 */
static bk_err_t i2s_register_dma_isr(dma_id_t dma_id, i2s_channel_id_t chl_id, i2s_txrx_type_t type)
{
    switch (chl_id) {
    case I2S_CHANNEL_1:
        if (type == I2S_TXRX_TYPE_TX) {
            bk_dma_register_isr(dma_id, NULL, (void *)i2s_chl1_tx_dma_finish_isr);
        } else {
            bk_dma_register_isr(dma_id, NULL, (void *)i2s_chl1_rx_dma_finish_isr);
        }
        break;
    case I2S_CHANNEL_2:
        if (type == I2S_TXRX_TYPE_TX) {
            bk_dma_register_isr(dma_id, NULL, (void *)i2s_chl2_tx_dma_finish_isr);
        } else {
            bk_dma_register_isr(dma_id, NULL, (void *)i2s_chl2_rx_dma_finish_isr);
        }
        break;
    case I2S_CHANNEL_3:
        if (type == I2S_TXRX_TYPE_TX) {
            bk_dma_register_isr(dma_id, NULL, (void *)i2s_chl3_tx_dma_finish_isr);
        } else {
            bk_dma_register_isr(dma_id, NULL, (void *)i2s_chl3_rx_dma_finish_isr);
        }
        break;
    default:
        return BK_ERR_I2S_PARAM;
    }
    return BK_OK;
}

/**
 * @brief Get driver info for ISR functions (no validation needed)
 * @param i2s_index I2S index (0, 1, 2)
 * @param drv_info Output parameter for driver info pointer
 * @return BK_OK if valid, BK_FAIL otherwise
 */
static bk_err_t i2s_get_drv_info_for_isr(uint8_t i2s_index, i2s_drv_info_t **drv_info)
{
    switch (i2s_index) {
    case 0:
        *drv_info = i2s_drv_info;
        break;
#if (SOC_I2S_UNIT_NUM > 1)
    case 1:
        *drv_info = i2s1_drv_info;
        break;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
    case 2:
        *drv_info = i2s2_drv_info;
        break;
#endif
    default:
        return BK_FAIL;
    }
    return BK_OK;
}

/**
 * @brief Get ISR callback array for specific I2S index
 * @param i2s_index I2S index (0, 1, 2)
 * @param isr_cb Output parameter for ISR callback array
 * @return BK_OK if valid, BK_FAIL otherwise
 */
static bk_err_t i2s_get_isr_cb_for_isr(uint8_t i2s_index, i2s_callback_t **isr_cb)
{
    switch (i2s_index) {
    case 0:
        *isr_cb = s_i2s_isr;
        break;
#if (SOC_I2S_UNIT_NUM > 1)
    case 1:
        *isr_cb = s_i2s1_isr;
        break;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
    case 2:
        *isr_cb = s_i2s2_isr;
        break;
#endif
    default:
        return BK_FAIL;
    }
    return BK_OK;
}

/**
 * @brief Get channel configuration pointer based on channel ID
 * @param drv_info Driver info pointer
 * @param chl_id Channel ID
 * @param chl_cfg Output parameter for channel configuration pointer
 * @return BK_OK if successful, error code otherwise
 */
static bk_err_t i2s_get_channel_cfg(i2s_drv_info_t *drv_info, i2s_channel_id_t chl_id, i2s_chl_cfg_t **chl_cfg)
{
    switch (chl_id) {
    case I2S_CHANNEL_1:
        *chl_cfg = drv_info->chl1_cfg;
        break;
    case I2S_CHANNEL_2:
        *chl_cfg = drv_info->chl2_cfg;
        break;
    case I2S_CHANNEL_3:
        *chl_cfg = drv_info->chl3_cfg;
        break;
    default:
        return BK_ERR_I2S_PARAM;
    }
    return BK_OK;
}

/**
 * @brief Initialize channel configuration if not exists
 * @param chl_cfg Channel configuration pointer
 * @return BK_OK if successful, error code otherwise
 */
static bk_err_t i2s_init_channel_cfg(i2s_chl_cfg_t **chl_cfg)
{
    if (*chl_cfg == NULL) {
        *chl_cfg = (i2s_chl_cfg_t *)i2s_driver_mem_zalloc(sizeof(i2s_chl_cfg_t));
        if (*chl_cfg == NULL) {
            I2S_LOGE("malloc chl_cfg fail \n");
            return BK_FAIL;
        }
    }
    return BK_OK;
}

/**
 * @brief Initialize TX/RX configuration for a channel
 * @param chl_cfg Channel configuration pointer
 * @param type TX or RX type
 * @param buff_size Buffer size
 * @param rb Output parameter for ring buffer
 * @param data_handle_cb Data handle callback
 * @return BK_OK if successful, error code otherwise
 */
static bk_err_t i2s_init_txrx_cfg(i2s_chl_cfg_t *chl_cfg, i2s_txrx_type_t type, uint32_t buff_size, 
                                  RingBufferContext **rb, i2s_data_handle_cb data_handle_cb)
{
    i2s_txrx_cfg_t **txrx_cfg;
    bk_err_t ret;
    uint32_t alloc_size;

    /* Get TX or RX configuration pointer */
    if (type == I2S_TXRX_TYPE_TX) {
        txrx_cfg = &chl_cfg->tx_cfg;
        alloc_size = buff_size + DMA_CARRY_I2S_RINGBUF_SAFE_INTERVAL;
    } else {
        txrx_cfg = &chl_cfg->rx_cfg;
        alloc_size = buff_size;
    }

    /* Allocate TX/RX configuration */
    *txrx_cfg = (i2s_txrx_cfg_t *)i2s_driver_mem_zalloc(sizeof(i2s_txrx_cfg_t));
    if (*txrx_cfg == NULL) {
        I2S_LOGE("malloc %s_cfg fail \n", (type == I2S_TXRX_TYPE_TX) ? "tx" : "rx");
        return BK_FAIL;
    }

    /* Initialize TX/RX configuration */
    (*txrx_cfg)->dma_id = DMA_ID_MAX;
    (*txrx_cfg)->state = I2S_TXRX_STATE_NULL;
    (*txrx_cfg)->buff_size = buff_size;
    (*txrx_cfg)->buff_addr = (uint8_t *)i2s_driver_mem_zalloc(alloc_size);
    if ((*txrx_cfg)->buff_addr == NULL) {
        I2S_LOGE("malloc buff_addr fail \n");
        i2s_driver_mem_free(*txrx_cfg);
        *txrx_cfg = NULL;
        return BK_FAIL;
    }

    /* Allocate ring buffer */
    (*txrx_cfg)->rb = (RingBufferContext *)i2s_driver_mem_zalloc(sizeof(RingBufferContext));
    if ((*txrx_cfg)->rb == NULL) {
        I2S_LOGE("malloc rb fail \n");
        i2s_driver_mem_free((*txrx_cfg)->buff_addr);
        (*txrx_cfg)->buff_addr = NULL;
        i2s_driver_mem_free(*txrx_cfg);
        *txrx_cfg = NULL;
        return BK_FAIL;
    }

    *rb = (*txrx_cfg)->rb;

    /* Initialize DMA driver */
    ret = bk_dma_driver_init();
    if (ret != BK_OK) {
        I2S_LOGE("dma driver init failed\r\n");
        i2s_driver_mem_free((*txrx_cfg)->buff_addr);
        (*txrx_cfg)->buff_addr = NULL;
        i2s_driver_mem_free((*txrx_cfg)->rb);
        (*txrx_cfg)->rb = NULL;
        i2s_driver_mem_free(*txrx_cfg);
        *txrx_cfg = NULL;
        return BK_FAIL;
    }

    return BK_OK;
}

/**
 * @brief Configure DMA for a specific channel and type
 * @param chl_cfg Channel configuration pointer
 * @param chl_id Channel ID
 * @param type TX or RX type
 * @param data_handle_cb Data handle callback
 * @return BK_OK if successful, error code otherwise
 */
static bk_err_t i2s_configure_dma_for_channel(i2s_chl_cfg_t *chl_cfg, i2s_channel_id_t chl_id, 
                                              i2s_txrx_type_t type, i2s_data_handle_cb data_handle_cb)
{
    i2s_txrx_cfg_t *txrx_cfg;
    bk_err_t ret;
    uint32_t transfer_len;

    /* Get TX or RX configuration */
    if (type == I2S_TXRX_TYPE_TX) {
        txrx_cfg = chl_cfg->tx_cfg;
    } else {
        txrx_cfg = chl_cfg->rx_cfg;
    }

    /* Allocate DMA channel */
    txrx_cfg->dma_id = bk_dma_alloc(DMA_DEV_I2S);
    if ((txrx_cfg->dma_id < DMA_ID_0) || (txrx_cfg->dma_id >= DMA_ID_MAX)) {
        I2S_LOGE("malloc dma fail \r\n");
        return BK_FAIL;
    }

    /* Configure DMA */
    transfer_len = (type == I2S_TXRX_TYPE_TX) ? 
        (txrx_cfg->buff_size + DMA_CARRY_I2S_RINGBUF_SAFE_INTERVAL) / 2 : 
        txrx_cfg->buff_size / 2;

    ret = i2s_dma_config(txrx_cfg->dma_id, (uint32_t *)txrx_cfg->buff_addr, 
                         txrx_cfg->buff_size + ((type == I2S_TXRX_TYPE_TX) ? DMA_CARRY_I2S_RINGBUF_SAFE_INTERVAL : 0),
                         transfer_len, chl_id, type, data_handle_cb);
    if (ret != BK_OK) {
        I2S_LOGE("dma config fail \r\n");
        return BK_FAIL;
    }

    /* Initialize ring buffer */
    ring_buffer_init(txrx_cfg->rb, txrx_cfg->buff_addr, 
                     txrx_cfg->buff_size + ((type == I2S_TXRX_TYPE_TX) ? DMA_CARRY_I2S_RINGBUF_SAFE_INTERVAL : 0),
                     txrx_cfg->dma_id, (type == I2S_TXRX_TYPE_TX) ? RB_DMA_TYPE_READ : RB_DMA_TYPE_WRITE);

    /* Set callback and state */
    txrx_cfg->data_handle_cb = data_handle_cb;
    txrx_cfg->state = I2S_TXRX_STATE_IDLE;

    return BK_OK;
}

void i2s_set_dma_index(dma_id_t dma_idx)
{
    i2s_dma_idx = dma_idx;
}

dma_id_t i2s_get_dma_index(void)
{
    return i2s_dma_idx;
}

static void i2s_init_gpio(i2s_gpio_group_id_t id)
{
    switch (id) {
        case I2S_GPIO_GROUP_0:
            gpio_dev_unmap(GPIO_6);
            gpio_dev_map(GPIO_6, GPIO_DEV_I2S1_CLK);
            gpio_dev_unmap(GPIO_7);
            gpio_dev_map(GPIO_7, GPIO_DEV_I2S1_SYNC);
            gpio_dev_unmap(GPIO_8);
            gpio_dev_map(GPIO_8, GPIO_DEV_I2S1_DIN);
            gpio_dev_unmap(GPIO_9);
            gpio_dev_map(GPIO_9, GPIO_DEV_I2S1_DOUT);
#if (CONFIG_SOC_BK7236N || CONFIG_SOC_BK7239N)
            gpio_dev_unmap(GPIO_12);
            gpio_dev_map(GPIO_12, GPIO_DEV_I2S1_MCLK);
#else
            gpio_dev_unmap(GPIO_28);
            gpio_dev_map(GPIO_28, GPIO_DEV_I2S1_MCLK);
#endif
            break;

        case I2S_GPIO_GROUP_1:
            gpio_dev_unmap(GPIO_40);
            gpio_dev_map(GPIO_40, GPIO_DEV_I2S2_CLK);
            gpio_dev_unmap(GPIO_41);
            gpio_dev_map(GPIO_41, GPIO_DEV_I2S2_SYNC);
            gpio_dev_unmap(GPIO_42);
            gpio_dev_map(GPIO_42, GPIO_DEV_I2S2_DIN);
            gpio_dev_unmap(GPIO_43);
            gpio_dev_map(GPIO_43, GPIO_DEV_I2S2_DOUT);
            gpio_dev_unmap(GPIO_28);
            gpio_dev_map(GPIO_28, GPIO_DEV_I2S1_MCLK);
            break;

        case I2S_GPIO_GROUP_2:
            gpio_dev_unmap(GPIO_44);
            gpio_dev_map(GPIO_44, GPIO_DEV_I2S3_CLK);
            gpio_dev_unmap(GPIO_45);
            gpio_dev_map(GPIO_45, GPIO_DEV_I2S3_SYNC);
            gpio_dev_unmap(GPIO_46);
            gpio_dev_map(GPIO_46, GPIO_DEV_I2S3_DIN);
            gpio_dev_unmap(GPIO_47);
            gpio_dev_map(GPIO_47, GPIO_DEV_I2S3_DOUT);
            gpio_dev_unmap(GPIO_28);
            gpio_dev_map(GPIO_28, GPIO_DEV_I2S1_MCLK);
            break;

        default:
            break;
    }
}

/**
 * @brief Initialize I2S driver
 * @return BK_OK on success, error code otherwise
 */
bk_err_t bk_i2s_driver_init(void)
{
    bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_AUDP_I2S, PM_POWER_MODULE_STATE_ON);
    sys_drv_i2s_select_clock(1);
    bk_pm_clock_ctrl(PM_CLK_ID_I2S_1, CLK_PWR_CTRL_PWR_UP);

    sys_drv_apll_en(1);
    sys_drv_apll_cal_val_set(0x8973CA6F);
    sys_drv_apll_config_set(0xC2A0AE86);
    sys_drv_apll_spi_trigger_set(1);
    delay(10);
    sys_drv_apll_spi_trigger_set(0);

    return BK_OK;
}

/**
 * @brief Deinitialize I2S driver
 * @return BK_OK on success, error code otherwise
 */
bk_err_t bk_i2s_driver_deinit(void)
{
    i2s_int_config_t int_config_table;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_AUDP_I2S, PM_POWER_MODULE_STATE_OFF);
    bk_pm_clock_ctrl(PM_CLK_ID_I2S_1, CLK_PWR_CTRL_PWR_DOWN);
    sys_drv_i2s_disckg_set(0);

    if (i2s_index == 0) {
        I2S_RETURN_ON_NOT_INIT();
        sys_drv_i2s_clock_en(CLK_PWR_CTRL_PWR_DOWN);
        sys_drv_i2s_int_en(0);
        int_config_table.int_src = INT_SRC_I2S0;
        s_i2s_driver_is_init = false;
    }
#if (SOC_I2S_UNIT_NUM > 1)
    else if (i2s_index == 1) {
        I2S1_RETURN_ON_NOT_INIT();
        sys_drv_i2s1_clock_en(CLK_PWR_CTRL_PWR_DOWN);
        sys_drv_i2s1_int_en(0);
        int_config_table.int_src = INT_SRC_I2S1;
        s_i2s1_driver_is_init = false;
    } else if (i2s_index == 2) {
        I2S2_RETURN_ON_NOT_INIT();
        sys_drv_i2s2_clock_en(CLK_PWR_CTRL_PWR_DOWN);
        sys_drv_i2s2_int_en(0);
        int_config_table.int_src = INT_SRC_I2S2;
        s_i2s2_driver_is_init = false;
    }
#endif
    else {
        return BK_FAIL;
    }

    sys_drv_apll_en(0);

    bk_int_isr_unregister(int_config_table.int_src);

    return BK_OK;
}

/**
 * @brief Initialize I2S with GPIO group and configuration
 * @param id GPIO group ID
 * @param config I2S configuration parameters
 * @return BK_OK on success, error code otherwise
 */
bk_err_t bk_i2s_init(i2s_gpio_group_id_t id, const i2s_config_t *config)
{
    i2s_int_config_t int_config_table;
    i2s_drv_info_t *drv_info;

    if (!config)
        return BK_ERR_I2S_PARAM;

    switch (id) {
        case I2S_GPIO_GROUP_0:
            if (s_i2s_driver_is_init) {
                return BK_OK;
            }
            i2s_hal_set_cfg_index(0);
            sys_drv_i2s_clock_en(CLK_PWR_CTRL_PWR_UP);
            i2s_hal_soft_reset();
            sys_drv_i2s_int_en(1);
            int_config_table.int_src = INT_SRC_I2S0;
            int_config_table.isr = i2s_isr;
            s_i2s_driver_is_init = true;
            if (i2s_drv_info == NULL) {
                i2s_drv_info = (i2s_drv_info_t *)i2s_driver_mem_zalloc(sizeof(i2s_drv_info_t));
                if (!i2s_drv_info) {
                    return BK_FAIL;
                }
            }
            drv_info = i2s_drv_info;
            break;
            
#if (SOC_I2S_UNIT_NUM > 1)
        case I2S_GPIO_GROUP_1:
            if (s_i2s1_driver_is_init) {
                return BK_OK;
            }
            i2s_hal_set_cfg_index(1);
            sys_drv_i2s1_clock_en(CLK_PWR_CTRL_PWR_UP);
            i2s_hal_soft_reset();
            sys_drv_i2s1_int_en(1);
            int_config_table.int_src = INT_SRC_I2S1;
            int_config_table.isr = i2s_isr;
            s_i2s1_driver_is_init = true;
            if (i2s1_drv_info == NULL) {
                i2s1_drv_info = (i2s_drv_info_t *)i2s_driver_mem_zalloc(sizeof(i2s_drv_info_t));
                if (!i2s1_drv_info) {
                    return BK_FAIL;
                }
            }
            drv_info = i2s1_drv_info;
            break;

        case I2S_GPIO_GROUP_2:
            if (s_i2s2_driver_is_init) {
                return BK_OK;
            }
            i2s_hal_set_cfg_index(2);
            sys_drv_i2s2_clock_en(CLK_PWR_CTRL_PWR_UP);
            i2s_hal_soft_reset();
            sys_drv_i2s2_int_en(1);
            int_config_table.int_src = INT_SRC_I2S2;
            int_config_table.isr = i2s_isr;
            s_i2s2_driver_is_init = true;
            if (i2s2_drv_info == NULL) {
                i2s2_drv_info = (i2s_drv_info_t *)i2s_driver_mem_zalloc(sizeof(i2s_drv_info_t));
                if (!i2s2_drv_info) {
                    return BK_FAIL;
                }
            }
            drv_info = i2s2_drv_info;
            break;
#endif
        default:
            return BK_ERR_I2S_PARAM;
    }
    bk_int_isr_register(int_config_table.int_src, int_config_table.isr, NULL);

    i2s_init_gpio(id);

    drv_info->config.i2s_en = I2S_DISABLE;
    drv_info->config.role = config->role;
    drv_info->config.work_mode = config->work_mode;
    drv_info->config.lrck_invert = config->lrck_invert;
    drv_info->config.sck_invert = config->sck_invert;
    drv_info->config.lsb_first_en = config->lsb_first_en;
    drv_info->config.sync_length = config->sync_length;
    drv_info->config.data_length = config->data_length;
    drv_info->config.pcm_dlength = config->pcm_dlength;
    drv_info->config.sample_ratio = 0;
    drv_info->config.sck_ratio = 0;

    drv_info->config.store_mode = config->store_mode;
    drv_info->config.sck_ratio_h4b = 0;
    drv_info->config.sample_ratio_h2b = 0;
    drv_info->config.txint_level = 0;
    drv_info->config.rxint_level = 0;

    drv_info->config.pcm_chl_num = config->pcm_chl_num;
    drv_info->config.samp_rate = config->samp_rate;

    /* set parallel_en according to work mode */
    switch (drv_info->config.work_mode) {
        case I2S_WORK_MODE_I2S:
        case I2S_WORK_MODE_LEFTJUST:
        case I2S_WORK_MODE_RIGHTJUST:
            drv_info->config.parallel_en = I2S_PARALLEL_ENABLE;
            break;

        case I2S_WORK_MODE_SHORTFAMSYNC:
        case I2S_WORK_MODE_LONGFAMSYNC:
            I2S_LOGE("pcm_chl_num: %d, data_length: %d \n", drv_info->config.pcm_chl_num, drv_info->config.data_length);
            if ((drv_info->config.pcm_chl_num * drv_info->config.data_length) > 32) {
                drv_info->config.parallel_en = I2S_PARALLEL_DISABLE;
                I2S_LOGE("disenable parallel \n");
            } else {
                drv_info->config.parallel_en = I2S_PARALLEL_ENABLE;
            }
            break;

        case I2S_WORK_MODE_NORMAL2BD:
        case I2S_WORK_MODE_DELAY2BD:
            drv_info->config.parallel_en = I2S_PARALLEL_ENABLE;
            break;

        case I2S_WORK_MODE_RSVD:
            break;

        default:
            break;
    }

    i2s_hal_config(&(drv_info->config));

    if (bk_i2s_set_samp_rate(drv_info->config.samp_rate) != BK_OK) {
        I2S_LOGE("config sample rate fail \n");
        return BK_FAIL;
    }

    drv_info->i2s_enable_state = false;

    return BK_OK;
}

bk_err_t bk_i2s_deinit(void)
{
    i2s_drv_info_t *drv_info;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_and_get_drv_info(i2s_index, &drv_info);
    if (ret != BK_OK) {
        return ret;
    }

    /* Clear the driver info pointer */
    switch (i2s_index) {
    case 0:
        i2s_drv_info = NULL;
        break;
#if (SOC_I2S_UNIT_NUM > 1)
    case 1:
        i2s1_drv_info = NULL;
        break;
#endif
#if (SOC_I2S_UNIT_NUM > 2)
    case 2:
        i2s2_drv_info = NULL;
        break;
#endif
    }

    i2s_hal_deconfig();

    if (drv_info->chl1_cfg) {
        bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
        bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
    }

    if (drv_info->chl2_cfg) {
        bk_i2s_chl_deinit(I2S_CHANNEL_2, I2S_TXRX_TYPE_TX);
        bk_i2s_chl_deinit(I2S_CHANNEL_2, I2S_TXRX_TYPE_RX);
    }

    if (drv_info->chl3_cfg) {
        bk_i2s_chl_deinit(I2S_CHANNEL_3, I2S_TXRX_TYPE_TX);
        bk_i2s_chl_deinit(I2S_CHANNEL_3, I2S_TXRX_TYPE_RX);
    }

    if (drv_info) {
        i2s_driver_mem_free(drv_info);
        drv_info = NULL;
    }

    return BK_OK;
}

bk_err_t bk_i2s_get_read_ready(uint32_t *read_flag)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();
    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_read_ready_get(read_flag);
    return BK_OK;
}

bk_err_t bk_i2s_get_write_ready(uint32_t *write_flag)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();
    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_write_ready_get(write_flag);
    return BK_OK;
}

bk_err_t bk_i2s_enable(i2s_en_t en_value)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();
    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_en_set(en_value);
    return BK_OK;
}

bk_err_t bk_i2s_int_enable(i2s_isr_id_t int_id, uint32_t value)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_int_set(int_id, value);
    return BK_OK;
}

bk_err_t bk_i2s_set_role(i2s_role_t role)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_role_set(role);
    return BK_OK;
}

bk_err_t bk_i2s_set_work_mode(i2s_work_mode_t work_mode)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_work_mode_set(work_mode);
    return BK_OK;
}

bk_err_t bk_i2s_set_lrck_invert(i2s_lrck_invert_en_t lrckrp)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_lrck_invert_set(lrckrp);
    return BK_OK;
}

bk_err_t bk_i2s_set_sck_invert(i2s_sck_invert_en_t sck_invert)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_sck_invert_set(sck_invert);
    return BK_OK;
}

bk_err_t bk_i2s_set_lsb_first(i2s_lsb_first_en_t lsb_first)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_lsb_first_set(lsb_first);
    return BK_OK;
}

bk_err_t bk_i2s_set_sync_len(uint32_t sync_len)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_sync_len_set(sync_len);
    return BK_OK;
}

bk_err_t bk_i2s_set_data_len(uint32_t data_len)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_data_len_set(data_len);
    return BK_OK;
}

bk_err_t bk_i2s_set_pcm_dlen(uint32_t pcm_dlen)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_pcm_dlen_set(pcm_dlen);
    return BK_OK;
}

bk_err_t bk_i2s_set_store_mode(i2s_lrcom_store_mode_t store_mode)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_store_mode_set(store_mode);
    return BK_OK;
}

bk_err_t bk_i2s_clear_rxfifo(void)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_rxfifo_clear();
    return BK_OK;
}

bk_err_t bk_i2s_clear_txfifo(void)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_txfifo_clear();
    return BK_OK;
}

bk_err_t bk_i2s_clear_txudf_int(i2s_channel_id_t channel_id)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_txudf_int_clear(channel_id);
    return BK_OK;
}

bk_err_t bk_i2s_clear_rxovf_int(i2s_channel_id_t channel_id)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_rxovf_int_clear(channel_id);
    return BK_OK;
}

bk_err_t bk_i2s_set_txint_level(i2s_txint_level_t txint_level)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_txint_level_set(txint_level);
    return BK_OK;
}

bk_err_t bk_i2s_set_rxint_level(i2s_rxint_level_t rxint_level)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    i2s_hal_rxint_level_set(rxint_level);
    return BK_OK;
}

bk_err_t bk_i2s_write_data(uint32_t channel_id, uint32_t *data_buf, uint32_t data_len)
{
    uint32_t i = 0;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    for (i=0; i<data_len; i++)
        i2s_hal_data_write(channel_id, data_buf[i]);
    return BK_OK;
}

bk_err_t bk_i2s_read_data(uint32_t *data_buf, uint32_t data_len)
{
    uint32_t i = 0;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }

    for (i=0; i<data_len; i++)
        i2s_hal_data_read(&data_buf[i]);
    return BK_OK;
}

bk_err_t bk_i2s_get_data_addr(i2s_channel_id_t chl_id, uint32_t *i2s_data_addr)
{
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_index(i2s_index);
    if (ret != BK_OK) {
        return ret;
    }
    switch (chl_id) {
        case I2S_CHANNEL_1:
            if (i2s_index == 0) {
                *i2s_data_addr = I2S_PCM_DAT_ADDR;
            }
#if (SOC_I2S_UNIT_NUM > 1)
            else if (i2s_index == 1) {
                *i2s_data_addr = I2S1_PCM_DAT_ADDR;
            } else if (i2s_index == 2) {
                *i2s_data_addr = I2S2_PCM_DAT_ADDR;
            }
#endif
            break;

        case I2S_CHANNEL_2:
            if (i2s_index == 0) {
                *i2s_data_addr = I2S_PCM_DAT2_ADDR;
            }
#if (SOC_I2S_UNIT_NUM > 1)
            else if (i2s_index == 1) {
                *i2s_data_addr = I2S1_PCM_DAT2_ADDR;
            } else if (i2s_index == 2) {
                *i2s_data_addr = I2S2_PCM_DAT2_ADDR;
            }
#endif
            break;

        case I2S_CHANNEL_3:
            if (i2s_index == 0) {
                *i2s_data_addr = I2S_PCM_DAT3_ADDR;
            }
#if (SOC_I2S_UNIT_NUM > 1)
            else if (i2s_index == 1) {
                *i2s_data_addr = I2S1_PCM_DAT3_ADDR;
            } else if (i2s_index == 2) {
                *i2s_data_addr = I2S2_PCM_DAT3_ADDR;
            }
#endif
            break;

        default:
            return BK_FAIL;
    }

    return BK_OK;
}

bk_err_t bk_i2s_set_samp_rate(i2s_samp_rate_t samp_rate)
{
    uint32_t sample_rate = 0;
    uint32_t smp_ratio = 0;
    uint32_t bit_ratio = 0;
    uint32_t bit_ratio_b = 0;
    uint32_t apll_clk = 0;
    uint32_t data_len;
    uint32_t pcm_chl_num;
    i2s_drv_info_t *drv_info;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_and_get_drv_info(i2s_index, &drv_info);
    if (ret != BK_OK) {
        return ret;
    }
    data_len = drv_info->config.data_length;
    pcm_chl_num = (uint32_t)drv_info->config.pcm_chl_num;

    /* get sample rate number */
    switch (samp_rate) {
        case I2S_SAMP_RATE_8000:
            sample_rate = 8000;
            break;
        case I2S_SAMP_RATE_12000:
            sample_rate = 12000;
            break;
        case I2S_SAMP_RATE_16000:
            sample_rate = 16000;
            break;
        case I2S_SAMP_RATE_24000:
            sample_rate = 24000;
            break;
        case I2S_SAMP_RATE_32000:
            sample_rate = 32000;
            break;
        case I2S_SAMP_RATE_48000:
            sample_rate = 48000;
            break;
        case I2S_SAMP_RATE_96000:
            sample_rate = 96000;
            break;
        case I2S_SAMP_RATE_8018:
            sample_rate = 80182;
            break;
        case I2S_SAMP_RATE_11025:
            sample_rate = 11025;
            break;
        case I2S_SAMP_RATE_22050:
            sample_rate = 22050;
            break;
        case I2S_SAMP_RATE_44100:
            sample_rate = 44100;
            break;
        case I2S_SAMP_RATE_88200:
            sample_rate = 88200;
            break;
        default:
            break;
    }

#if (CONFIG_CLK_APLL)
    /* get apll clock according to sample rate */
    if (samp_rate == I2S_SAMP_RATE_8000 ||
        samp_rate == I2S_SAMP_RATE_12000 ||
        samp_rate == I2S_SAMP_RATE_16000 ||
        samp_rate == I2S_SAMP_RATE_24000 ||
        samp_rate == I2S_SAMP_RATE_32000 ||
        samp_rate == I2S_SAMP_RATE_48000 ||
        samp_rate == I2S_SAMP_RATE_96000)
    {
        apll_clk = 98304000;
    }
    else if (samp_rate == I2S_SAMP_RATE_8018 ||
        samp_rate == I2S_SAMP_RATE_11025 ||
        samp_rate == I2S_SAMP_RATE_22050 ||
        samp_rate == I2S_SAMP_RATE_44100 ||
        samp_rate == I2S_SAMP_RATE_88200)
    {
        apll_clk = 90316800;
    } else {
        I2S_LOGE("cannot support sample rate \r\n");
        return BK_ERR_I2S_PARAM;
    }
#else
    apll_clk = CONFIG_XTAL_FREQ;
#endif

    /* get samp_ratio: Fs = Fsck / 2*(SMPRATIO+1) */
    switch (drv_info->config.work_mode) {
        case I2S_WORK_MODE_I2S:
        case I2S_WORK_MODE_LEFTJUST:
        case I2S_WORK_MODE_RIGHTJUST:
            smp_ratio = data_len - 1;
            break;

        case I2S_WORK_MODE_SHORTFAMSYNC:
        case I2S_WORK_MODE_LONGFAMSYNC:
            smp_ratio = data_len * pcm_chl_num - 1;
            break;

        case I2S_WORK_MODE_NORMAL2BD:
        case I2S_WORK_MODE_DELAY2BD:
            smp_ratio = data_len - 1;
            break;

        case I2S_WORK_MODE_RSVD:
            break;

        default:
            break;
    }

#if CONFIG_CLK_APLL
    /* get bit_ratio: Fsck = Fsys_clk / 2*(BITRATIO) */
    if (sample_rate == 80182) {
        if (drv_info->config.work_mode == I2S_WORK_MODE_SHORTFAMSYNC || drv_info->config.work_mode == I2S_WORK_MODE_LONGFAMSYNC) {
            bit_ratio = (apll_clk*10)/((sample_rate*(smp_ratio+1))*2);
            bit_ratio_b = (apll_clk*10)%((sample_rate*(smp_ratio+1))*2);
        } else {
            bit_ratio = (apll_clk*10)/((sample_rate*2*(smp_ratio+1))*2);
            bit_ratio_b = (apll_clk*10)%((sample_rate*2*(smp_ratio+1))*2);
        }
    } else {
        I2S_LOGE("apll_clk: %d, sample_rate: %d, smp_ratio: %d \r\n", apll_clk, sample_rate, smp_ratio);
        if (drv_info->config.work_mode == I2S_WORK_MODE_SHORTFAMSYNC || drv_info->config.work_mode == I2S_WORK_MODE_LONGFAMSYNC) {
            bit_ratio = apll_clk/((sample_rate*(smp_ratio+1))*2);
            bit_ratio_b = apll_clk%((sample_rate*(smp_ratio+1))*2);
        } else {
            bit_ratio = apll_clk/((sample_rate*2*(smp_ratio+1))*2);
            bit_ratio_b = apll_clk%((sample_rate*2*(smp_ratio+1))*2);
        }
    }
    I2S_LOGE("bit_ratio: %d, bit_ratio_b: %d \r\n", bit_ratio, bit_ratio_b);
    if (bit_ratio_b != 0) {
        I2S_LOGE("cannot support sample rate \r\n");
        return BK_ERR_I2S_PARAM;
    }

    if (apll_clk == 98304000)
        sys_drv_apll_cal_val_set(0x8973CA6F);
    else if (apll_clk == 90316800)
        sys_drv_apll_cal_val_set(0x88AF2EC9);
    else
        sys_drv_apll_cal_val_set(0x8973CA6F);
    sys_drv_apll_config_set(0xC2A0AE86);
    sys_drv_i2s0_ckdiv_set(0);
    sys_drv_apll_spi_trigger_set(1);
    delay(10);
    sys_drv_apll_spi_trigger_set(0);
#else
    I2S_LOGE("apll_clk: %d, sample_rate: %d, smp_ratio: %d \r\n", apll_clk, sample_rate, smp_ratio);
    if (drv_info->config.work_mode == I2S_WORK_MODE_SHORTFAMSYNC || drv_info->config.work_mode == I2S_WORK_MODE_LONGFAMSYNC) {
        bit_ratio = apll_clk / ((sample_rate * (smp_ratio + 1)) * 2);
        bit_ratio_b = apll_clk % ((sample_rate * (smp_ratio + 1)) * 2);
    } else {
        bit_ratio = apll_clk / ((sample_rate * 2 * (smp_ratio + 1)) * 2);
        bit_ratio_b = apll_clk % ((sample_rate * 2 * (smp_ratio + 1)) * 2);
    }
    I2S_LOGE("bit_ratio: %d, bit_ratio_b: %d\r\n", bit_ratio, bit_ratio_b);
#endif

    if (smp_ratio > 0x1F) {
        i2s_hal_sample_ratio_set(smp_ratio & 0x1F);
        i2s_hal_sample_ratio_h2b_set((smp_ratio >> 5) & 0x3);
    } else {
        i2s_hal_sample_ratio_set(smp_ratio);
    }
    if (bit_ratio > 0xFF) {
        i2s_hal_sck_ratio_set(bit_ratio & 0xFF);
        i2s_hal_sck_ratio_h4b_set((bit_ratio >> 8) & 0xF);
    } else {
        i2s_hal_sck_ratio_set(bit_ratio);
    }

    return BK_OK;
}

bk_err_t bk_i2s_set_clk_div(uint32_t div_val)
{
    I2S_RETURN_ON_NOT_INIT();

    sys_drv_i2s0_ckdiv_set(div_val);

    return BK_OK;
}

/* register i2s interrupt */
bk_err_t bk_i2s_register_i2s_isr(i2s_isr_id_t isr_id, i2s_isr_t isr, void *param)
{
    uint32_t int_level;
    i2s_callback_t *isr_cb = NULL;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret;

    ret = i2s_validate_and_get_isr_cb(i2s_index, &isr_cb);
    if (ret != BK_OK) {
        return ret;
    }
    I2S_RETURN_ON_INVALID_ISR_ID(isr_id);

    int_level = rtos_enter_critical();
    isr_cb[isr_id].callback = isr;
    isr_cb[isr_id].param = param;
    rtos_exit_critical(int_level);

    return BK_OK;
}

void i2s_isr(void)
{
    i2s_int_status_t i2s_status = {0};
    i2s_callback_t *isr_cb = NULL;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    i2s_status.channel_id = I2S_CHANNEL_1;
    i2s_hal_int_status_get(&i2s_status);

    if (i2s_validate_and_get_isr_cb(i2s_index, &isr_cb) != BK_OK) {
        return;
    }

    if (i2s_status.tx_udf) {
        i2s_hal_txudf_int_clear(I2S_CHANNEL_1);
        if (isr_cb[I2S_ISR_CHL1_TXUDF].callback) {
            isr_cb[I2S_ISR_CHL1_TXUDF].callback(isr_cb[I2S_ISR_CHL1_TXUDF].param);
        }
    }

    if (i2s_status.rx_ovf) {
        i2s_hal_rxovf_int_clear(I2S_CHANNEL_1);
        if (isr_cb[I2S_ISR_CHL1_RXOVF].callback) {
            isr_cb[I2S_ISR_CHL1_RXOVF].callback(isr_cb[I2S_ISR_CHL1_RXOVF].param);
        }
    }

    if (i2s_status.tx_int) {
        if (isr_cb[I2S_ISR_CHL1_TXINT].callback) {
            isr_cb[I2S_ISR_CHL1_TXINT].callback(isr_cb[I2S_ISR_CHL1_TXINT].param);
        }
    }

    if (i2s_status.rx_int) {
        if (isr_cb[I2S_ISR_CHL1_RXINT].callback) {
            isr_cb[I2S_ISR_CHL1_RXINT].callback(isr_cb[I2S_ISR_CHL1_RXINT].param);
        }
    }

    i2s_status.channel_id = I2S_CHANNEL_2;
    i2s_hal_int_status_get(&i2s_status);
    if (i2s_status.tx_udf) {
        i2s_hal_txudf_int_clear(I2S_CHANNEL_2);
        if (isr_cb[I2S_ISR_CHL2_TXUDF].callback) {
            isr_cb[I2S_ISR_CHL2_TXUDF].callback(isr_cb[I2S_ISR_CHL2_TXUDF].param);
        }
    }

    if (i2s_status.rx_ovf) {
        i2s_hal_rxovf_int_clear(I2S_CHANNEL_2);
        if (isr_cb[I2S_ISR_CHL2_RXOVF].callback) {
            isr_cb[I2S_ISR_CHL2_RXOVF].callback(isr_cb[I2S_ISR_CHL2_RXOVF].param);
        }
    }

    if (i2s_status.tx_int) {
        if (isr_cb[I2S_ISR_CHL2_TXINT].callback) {
            isr_cb[I2S_ISR_CHL2_TXINT].callback(isr_cb[I2S_ISR_CHL2_TXINT].param);
        }
    }

    if (i2s_status.rx_int) {
        if (isr_cb[I2S_ISR_CHL2_RXINT].callback) {
            isr_cb[I2S_ISR_CHL2_RXINT].callback(isr_cb[I2S_ISR_CHL2_RXINT].param);
        }
    }

    i2s_status.channel_id = I2S_CHANNEL_3;
    i2s_hal_int_status_get(&i2s_status);
    if (i2s_status.tx_udf) {
        i2s_hal_txudf_int_clear(I2S_CHANNEL_3);
        if (isr_cb[I2S_ISR_CHL3_TXUDF].callback) {
            isr_cb[I2S_ISR_CHL3_TXUDF].callback(isr_cb[I2S_ISR_CHL3_TXUDF].param);
        }
    }

    if (i2s_status.rx_ovf) {
        i2s_hal_rxovf_int_clear(I2S_CHANNEL_3);
        if (isr_cb[I2S_ISR_CHL3_RXOVF].callback) {
            isr_cb[I2S_ISR_CHL3_RXOVF].callback(isr_cb[I2S_ISR_CHL3_RXOVF].param);
        }
    }

    if (i2s_status.tx_int) {
        if (isr_cb[I2S_ISR_CHL3_TXINT].callback) {
            isr_cb[I2S_ISR_CHL3_TXINT].callback(isr_cb[I2S_ISR_CHL3_TXINT].param);
        }
    }

    if (i2s_status.rx_int) {
        if (isr_cb[I2S_ISR_CHL3_RXINT].callback) {
            isr_cb[I2S_ISR_CHL3_RXINT].callback(isr_cb[I2S_ISR_CHL3_RXINT].param);
        }
    }
}

/**
 * @brief I2S channel 1 TX DMA finish interrupt handler
 */
static void i2s_chl1_tx_dma_finish_isr(void)
{
    i2s_drv_info_t *drv_info = NULL;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    if (i2s_get_drv_info_for_isr(i2s_index, &drv_info) != BK_OK) {
        return;
    }

    if (drv_info->chl1_cfg->tx_cfg->data_handle_cb) {
        drv_info->chl1_cfg->tx_cfg->data_handle_cb((drv_info->chl1_cfg->tx_cfg->buff_size)/2);
    }
}

/**
 * @brief I2S channel 1 RX DMA finish interrupt handler
 */
static void i2s_chl1_rx_dma_finish_isr(void)
{
    i2s_drv_info_t *drv_info = NULL;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    if (i2s_get_drv_info_for_isr(i2s_index, &drv_info) != BK_OK) {
        return;
    }

    if (drv_info->chl1_cfg->rx_cfg->data_handle_cb) {
        drv_info->chl1_cfg->rx_cfg->data_handle_cb((drv_info->chl1_cfg->rx_cfg->buff_size)/2);
    }
}

/**
 * @brief I2S channel 2 TX DMA finish interrupt handler
 */
static void i2s_chl2_tx_dma_finish_isr(void)
{
    i2s_drv_info_t *drv_info = NULL;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    if (i2s_get_drv_info_for_isr(i2s_index, &drv_info) != BK_OK) {
        return;
    }

    if (drv_info->chl2_cfg->tx_cfg->data_handle_cb) {
        drv_info->chl2_cfg->tx_cfg->data_handle_cb((drv_info->chl2_cfg->tx_cfg->buff_size)/2);
    }
}

/**
 * @brief I2S channel 2 RX DMA finish interrupt handler
 */
static void i2s_chl2_rx_dma_finish_isr(void)
{
    i2s_drv_info_t *drv_info = NULL;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    if (i2s_get_drv_info_for_isr(i2s_index, &drv_info) != BK_OK) {
        return;
    }

    if (drv_info->chl2_cfg->rx_cfg->data_handle_cb) {
        drv_info->chl2_cfg->rx_cfg->data_handle_cb((drv_info->chl2_cfg->rx_cfg->buff_size)/2);
    }
}

/**
 * @brief I2S channel 3 TX DMA finish interrupt handler
 */
static void i2s_chl3_tx_dma_finish_isr(void)
{
    i2s_drv_info_t *drv_info = NULL;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    if (i2s_get_drv_info_for_isr(i2s_index, &drv_info) != BK_OK) {
        return;
    }

    if (drv_info->chl3_cfg->tx_cfg->data_handle_cb) {
        drv_info->chl3_cfg->tx_cfg->data_handle_cb((drv_info->chl3_cfg->tx_cfg->buff_size)/2);
    }
}

/**
 * @brief I2S channel 3 RX DMA finish interrupt handler
 */
static void i2s_chl3_rx_dma_finish_isr(void)
{
    i2s_drv_info_t *drv_info = NULL;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    if (i2s_get_drv_info_for_isr(i2s_index, &drv_info) != BK_OK) {
        return;
    }

    if (drv_info->chl3_cfg->rx_cfg->data_handle_cb) {
        drv_info->chl3_cfg->rx_cfg->data_handle_cb((drv_info->chl3_cfg->rx_cfg->buff_size)/2);
    }
}

static bk_err_t i2s_dma_config(dma_id_t dma_id, uint32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len, i2s_channel_id_t chl_id, i2s_txrx_type_t type, i2s_data_handle_cb data_handle_cb)
{
    bk_err_t ret = BK_OK;
    dma_config_t dma_config = {0};
    uint32_t i2s_data_addr;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    dma_config.mode = DMA_WORK_MODE_REPEAT;
    dma_config.chan_prio = 1;
    dma_config.src.width = DMA_DATA_WIDTH_32BITS;
    dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
    dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
    dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
    dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
    dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
    bk_i2s_get_data_addr(chl_id, &i2s_data_addr);

    if (type == I2S_TXRX_TYPE_RX) {
        dma_config.src.dev = i2s_get_rx_dma_dev(i2s_index, chl_id);
        dma_config.dst.dev = DMA_DEV_DTCM;
        dma_config.src.start_addr = i2s_data_addr;
        dma_config.src.end_addr = i2s_data_addr + 4;
        dma_config.dst.start_addr = (uint32_t)ring_buff_addr;
        dma_config.dst.end_addr = (uint32_t)ring_buff_addr + ring_buff_size;
    } else {
        dma_config.src.dev = DMA_DEV_DTCM;
        dma_config.dst.dev = i2s_get_tx_dma_dev(i2s_index, chl_id);
        dma_config.src.start_addr = (uint32_t)ring_buff_addr;
        dma_config.src.end_addr = (uint32_t)ring_buff_addr + ring_buff_size;
        dma_config.dst.start_addr = i2s_data_addr;
        dma_config.dst.end_addr = i2s_data_addr + 4;
    }

    ret = bk_dma_init(dma_id, &dma_config);
    if (ret != BK_OK) {
        I2S_LOGE("audio adc dma channel init fail \r\n");
        return BK_FAIL;
    }

    bk_dma_set_transfer_len(dma_id, transfer_len);

    if (data_handle_cb) {
        ret = i2s_register_dma_isr(dma_id, chl_id, type);
        if (ret != BK_OK) {
            I2S_LOGE("Failed to register DMA ISR\r\n");
            return ret;
        }

        bk_dma_enable_finish_interrupt(dma_id);
    }
#if (CONFIG_SPE)
    bk_dma_set_src_sec_attr(dma_id, DMA_ATTR_SEC);
    bk_dma_set_dest_sec_attr(dma_id, DMA_ATTR_SEC);
#endif

    return BK_OK;
}

/**
 * @brief Initialize I2S channel
 * @param chl Channel ID
 * @param type TX or RX type
 * @param buff_size Buffer size
 * @param data_handle_cb Data handling callback
 * @param rb Ring buffer context
 * @return BK_OK on success, error code otherwise
 */
bk_err_t bk_i2s_chl_init(i2s_channel_id_t chl, i2s_txrx_type_t type, uint32_t buff_size, i2s_data_handle_cb data_handle_cb, RingBufferContext **rb)
{
    bk_err_t ret = BK_OK;
    i2s_drv_info_t *drv_info;
    uint8_t i2s_index = i2s_hal_get_cfg_index();

    ret = i2s_validate_and_get_drv_info(i2s_index, &drv_info);
    if (ret != BK_OK) {
        return ret;
    }

    /* Get channel configuration pointer */
    i2s_chl_cfg_t *chl_cfg;
    ret = i2s_get_channel_cfg(drv_info, chl, &chl_cfg);
    if (ret != BK_OK) {
        return ret;
    }

    /* Initialize channel configuration if not exists */
    ret = i2s_init_channel_cfg(&chl_cfg);
    if (ret != BK_OK) {
        return ret;
    }

    /* Set the channel configuration back to driver info */
    switch (chl) {
    case I2S_CHANNEL_1:
        drv_info->chl1_cfg = chl_cfg;
        break;
    case I2S_CHANNEL_2:
        drv_info->chl2_cfg = chl_cfg;
        break;
    case I2S_CHANNEL_3:
        drv_info->chl3_cfg = chl_cfg;
        break;
    default:
        return BK_ERR_I2S_PARAM;
    }

    /* Initialize TX/RX configuration */
    ret = i2s_init_txrx_cfg(chl_cfg, type, buff_size, rb, data_handle_cb);
    if (ret != BK_OK) {
        return ret;
    }

    /* Configure DMA for the channel */
    ret = i2s_configure_dma_for_channel(chl_cfg, chl, type, data_handle_cb);
    if (ret != BK_OK) {
        return ret;
    }

    return BK_OK;
}

/* Legacy function - keeping for compatibility but should be removed */
bk_err_t bk_i2s_chl_init_legacy(i2s_channel_id_t chl, i2s_txrx_type_t type, uint32_t buff_size, i2s_data_handle_cb data_handle_cb, RingBufferContext **rb)
{
    i2s_drv_info_t *drv_info;
    uint8_t i2s_index = i2s_hal_get_cfg_index();
    bk_err_t ret;

    ret = i2s_validate_and_get_drv_info(i2s_index, &drv_info);
    if (ret != BK_OK) {
        return ret;
    }

    return i2s_init_channel_legacy(drv_info, chl, type, buff_size, data_handle_cb, rb);
}

bk_err_t bk_i2s_chl_deinit(i2s_channel_id_t chl, i2s_txrx_type_t type)
{
    i2s_drv_info_t *drv_info;
    uint8_t i2s_index = i2s_hal_get_cfg_index();
    bk_err_t ret;

    ret = i2s_validate_and_get_drv_info(i2s_index, &drv_info);
    if (ret != BK_OK) {
        return ret;
    }

    switch (chl) {
        case I2S_CHANNEL_1:
            if (type == I2S_TXRX_TYPE_TX) {
                if (drv_info->chl1_cfg && drv_info->chl1_cfg->tx_cfg) {
                    bk_dma_stop(drv_info->chl1_cfg->tx_cfg->dma_id);
                    ring_buffer_clear(drv_info->chl1_cfg->tx_cfg->rb);
                    bk_dma_deinit(drv_info->chl1_cfg->tx_cfg->dma_id);
                    bk_dma_free(DMA_DEV_I2S, drv_info->chl1_cfg->tx_cfg->dma_id);

                    if (drv_info->chl1_cfg->tx_cfg->rb)
                        i2s_driver_mem_free(drv_info->chl1_cfg->tx_cfg->rb);
                    drv_info->chl1_cfg->tx_cfg->rb = NULL;
                    if (drv_info->chl1_cfg->tx_cfg->buff_addr)
                        i2s_driver_mem_free(drv_info->chl1_cfg->tx_cfg->buff_addr);
                    drv_info->chl1_cfg->tx_cfg->buff_addr = NULL;
                    drv_info->chl1_cfg->tx_cfg->buff_size = 0;
                    drv_info->chl1_cfg->tx_cfg->dma_id = DMA_ID_MAX;
                    drv_info->chl1_cfg->tx_cfg->state = I2S_TXRX_STATE_NULL;
                    drv_info->chl1_cfg->tx_cfg->data_handle_cb = NULL;
                    if (drv_info->chl1_cfg->tx_cfg)
                        i2s_driver_mem_free(drv_info->chl1_cfg->tx_cfg);
                    drv_info->chl1_cfg->tx_cfg = NULL;
                }
            } else {
                if (drv_info->chl1_cfg && drv_info->chl1_cfg->rx_cfg) {
                    bk_dma_stop(drv_info->chl1_cfg->rx_cfg->dma_id);
                    bk_dma_deinit(drv_info->chl1_cfg->rx_cfg->dma_id);
                    bk_dma_free(DMA_DEV_I2S, drv_info->chl1_cfg->rx_cfg->dma_id);

                    ring_buffer_clear(drv_info->chl1_cfg->rx_cfg->rb);
                    if (drv_info->chl1_cfg->rx_cfg->rb)
                        i2s_driver_mem_free(drv_info->chl1_cfg->rx_cfg->rb);
                    drv_info->chl1_cfg->rx_cfg->rb = NULL;
                    if (drv_info->chl1_cfg->rx_cfg->buff_addr)
                        i2s_driver_mem_free(drv_info->chl1_cfg->rx_cfg->buff_addr);
                    drv_info->chl1_cfg->rx_cfg->buff_addr = NULL;
                    drv_info->chl1_cfg->rx_cfg->buff_size = 0;
                    drv_info->chl1_cfg->rx_cfg->dma_id = DMA_ID_MAX;
                    drv_info->chl1_cfg->rx_cfg->state = I2S_TXRX_STATE_NULL;
                    drv_info->chl1_cfg->rx_cfg->data_handle_cb = NULL;
                    if (drv_info->chl1_cfg->rx_cfg)
                        i2s_driver_mem_free(drv_info->chl1_cfg->rx_cfg);
                    drv_info->chl1_cfg->rx_cfg = NULL;
                }
            }
            if (drv_info->chl1_cfg->tx_cfg == NULL && drv_info->chl1_cfg->rx_cfg == NULL) {
                if (drv_info->chl1_cfg) {
                    i2s_driver_mem_free(drv_info->chl1_cfg);
                    drv_info->chl1_cfg = NULL;
                }
            }
            break;

        case I2S_CHANNEL_2:
            if (type == I2S_TXRX_TYPE_TX) {
                if (drv_info->chl2_cfg && drv_info->chl2_cfg->tx_cfg) {
                    bk_dma_stop(drv_info->chl2_cfg->tx_cfg->dma_id);
                    bk_dma_deinit(drv_info->chl2_cfg->tx_cfg->dma_id);
                    bk_dma_free(DMA_DEV_I2S, drv_info->chl2_cfg->tx_cfg->dma_id);

                    ring_buffer_clear(drv_info->chl2_cfg->tx_cfg->rb);
                    if (drv_info->chl2_cfg->tx_cfg->rb)
                        i2s_driver_mem_free(drv_info->chl2_cfg->tx_cfg->rb);
                    drv_info->chl2_cfg->tx_cfg->rb = NULL;
                    if (drv_info->chl2_cfg->tx_cfg->buff_addr)
                        i2s_driver_mem_free(drv_info->chl2_cfg->tx_cfg->buff_addr);
                    drv_info->chl2_cfg->tx_cfg->buff_addr = NULL;
                    drv_info->chl2_cfg->tx_cfg->buff_size = 0;
                    drv_info->chl2_cfg->tx_cfg->dma_id = DMA_ID_MAX;
                    drv_info->chl2_cfg->tx_cfg->state = I2S_TXRX_STATE_NULL;
                    drv_info->chl2_cfg->tx_cfg->data_handle_cb = NULL;
                    if (drv_info->chl2_cfg->tx_cfg)
                        i2s_driver_mem_free(drv_info->chl2_cfg->tx_cfg);
                    drv_info->chl2_cfg->tx_cfg = NULL;
                }
            } else {
                if (drv_info->chl2_cfg && drv_info->chl2_cfg->rx_cfg) {
                    bk_dma_stop(drv_info->chl2_cfg->rx_cfg->dma_id);
                    bk_dma_deinit(drv_info->chl2_cfg->rx_cfg->dma_id);
                    bk_dma_free(DMA_DEV_I2S, drv_info->chl2_cfg->rx_cfg->dma_id);

                    ring_buffer_clear(drv_info->chl2_cfg->rx_cfg->rb);
                    if (drv_info->chl2_cfg->rx_cfg->rb)
                        i2s_driver_mem_free(drv_info->chl2_cfg->rx_cfg->rb);
                    drv_info->chl2_cfg->rx_cfg->rb = NULL;
                    if (drv_info->chl2_cfg->rx_cfg->buff_addr)
                        i2s_driver_mem_free(drv_info->chl2_cfg->rx_cfg->buff_addr);
                    drv_info->chl2_cfg->rx_cfg->buff_addr = NULL;
                    drv_info->chl2_cfg->rx_cfg->buff_size = 0;
                    drv_info->chl2_cfg->rx_cfg->dma_id = DMA_ID_MAX;
                    drv_info->chl2_cfg->rx_cfg->state = I2S_TXRX_STATE_NULL;
                    drv_info->chl2_cfg->rx_cfg->data_handle_cb = NULL;
                    if (drv_info->chl2_cfg->rx_cfg)
                        i2s_driver_mem_free(drv_info->chl2_cfg->rx_cfg);
                    drv_info->chl2_cfg->rx_cfg = NULL;
                }
            }
            if (drv_info->chl2_cfg->tx_cfg == NULL && drv_info->chl2_cfg->rx_cfg == NULL) {
                if (drv_info->chl2_cfg) {
                    i2s_driver_mem_free(drv_info->chl2_cfg);
                    drv_info->chl2_cfg = NULL;
                }
            }
            break;

        case I2S_CHANNEL_3:
            if (type == I2S_TXRX_TYPE_TX) {
                if (drv_info->chl3_cfg && drv_info->chl3_cfg->tx_cfg) {
                    bk_dma_stop(drv_info->chl3_cfg->tx_cfg->dma_id);
                    bk_dma_deinit(drv_info->chl3_cfg->tx_cfg->dma_id);
                    bk_dma_free(DMA_DEV_I2S, drv_info->chl3_cfg->tx_cfg->dma_id);

                    ring_buffer_clear(drv_info->chl3_cfg->tx_cfg->rb);
                    if (drv_info->chl3_cfg->tx_cfg->rb)
                        i2s_driver_mem_free(drv_info->chl3_cfg->tx_cfg->rb);
                    drv_info->chl3_cfg->tx_cfg->rb = NULL;
                    if (drv_info->chl3_cfg->tx_cfg->buff_addr)
                        i2s_driver_mem_free(drv_info->chl3_cfg->tx_cfg->buff_addr);
                    drv_info->chl3_cfg->tx_cfg->buff_addr = NULL;
                    drv_info->chl3_cfg->tx_cfg->buff_size = 0;
                    drv_info->chl3_cfg->tx_cfg->dma_id = DMA_ID_MAX;
                    drv_info->chl3_cfg->tx_cfg->state = I2S_TXRX_STATE_NULL;
                    drv_info->chl3_cfg->tx_cfg->data_handle_cb = NULL;
                    if (drv_info->chl3_cfg->tx_cfg)
                        i2s_driver_mem_free(drv_info->chl3_cfg->tx_cfg);
                    drv_info->chl3_cfg->tx_cfg = NULL;
                }
            } else {
                if (drv_info->chl3_cfg && drv_info->chl3_cfg->rx_cfg) {
                    bk_dma_stop(drv_info->chl3_cfg->rx_cfg->dma_id);
                    bk_dma_deinit(drv_info->chl3_cfg->rx_cfg->dma_id);
                    bk_dma_free(DMA_DEV_I2S, drv_info->chl3_cfg->rx_cfg->dma_id);

                    ring_buffer_clear(drv_info->chl3_cfg->rx_cfg->rb);
                    if (drv_info->chl3_cfg->rx_cfg->rb)
                        i2s_driver_mem_free(drv_info->chl3_cfg->rx_cfg->rb);
                    drv_info->chl3_cfg->rx_cfg->rb = NULL;
                    if (drv_info->chl3_cfg->rx_cfg->buff_addr)
                        i2s_driver_mem_free(drv_info->chl3_cfg->rx_cfg->buff_addr);
                    drv_info->chl3_cfg->rx_cfg->buff_addr = NULL;
                    drv_info->chl3_cfg->rx_cfg->buff_size = 0;
                    drv_info->chl3_cfg->rx_cfg->dma_id = DMA_ID_MAX;
                    drv_info->chl3_cfg->rx_cfg->state = I2S_TXRX_STATE_NULL;
                    drv_info->chl3_cfg->rx_cfg->data_handle_cb = NULL;
                    if (drv_info->chl3_cfg->rx_cfg)
                        i2s_driver_mem_free(drv_info->chl3_cfg->rx_cfg);
                    drv_info->chl3_cfg->rx_cfg = NULL;
                }
            }
            if (drv_info->chl3_cfg->tx_cfg == NULL && drv_info->chl3_cfg->rx_cfg == NULL) {
                if (drv_info->chl3_cfg) {
                    i2s_driver_mem_free(drv_info->chl3_cfg);
                    drv_info->chl3_cfg = NULL;
                }
            }
            break;

        default:
            break;
    }

    return BK_OK;
}

/**
 * @brief Start I2S transmission
 * @return BK_OK on success, error code otherwise
 */
bk_err_t bk_i2s_start(void)
{
    uint32_t temp_data = 0xF0F0F0F0;
    i2s_drv_info_t *drv_info;
    uint8_t i2s_index = i2s_hal_get_cfg_index();
    bk_err_t ret;

    ret = i2s_validate_and_get_drv_info(i2s_index, &drv_info);
    if (ret != BK_OK) {
        return ret;
    }

    bk_i2s_enable(I2S_ENABLE);

    if (drv_info->chl1_cfg &&
        drv_info->chl1_cfg->tx_cfg &&
        (drv_info->chl1_cfg->tx_cfg->state == I2S_TXRX_STATE_IDLE || drv_info->chl1_cfg->tx_cfg->state == I2S_TXRX_STATE_STOP))
    {
        bk_dma_start(drv_info->chl1_cfg->tx_cfg->dma_id);
        drv_info->chl1_cfg->tx_cfg->state = I2S_TXRX_STATE_START;
    }

    if (drv_info->chl1_cfg &&
        drv_info->chl1_cfg->rx_cfg &&
        (drv_info->chl1_cfg->rx_cfg->state == I2S_TXRX_STATE_IDLE || drv_info->chl1_cfg->rx_cfg->state == I2S_TXRX_STATE_STOP))
    {
        bk_dma_start(drv_info->chl1_cfg->rx_cfg->dma_id);
        drv_info->chl1_cfg->rx_cfg->state = I2S_TXRX_STATE_START;
    }

    if (drv_info->chl2_cfg &&
        drv_info->chl2_cfg->tx_cfg &&
        (drv_info->chl2_cfg->tx_cfg->state == I2S_TXRX_STATE_IDLE || drv_info->chl2_cfg->tx_cfg->state == I2S_TXRX_STATE_STOP))
    {
        bk_dma_start(drv_info->chl2_cfg->tx_cfg->dma_id);
        drv_info->chl2_cfg->tx_cfg->state = I2S_TXRX_STATE_START;
    }

    if (drv_info->chl2_cfg &&
        drv_info->chl2_cfg->rx_cfg &&
        (drv_info->chl2_cfg->rx_cfg->state == I2S_TXRX_STATE_IDLE || drv_info->chl2_cfg->rx_cfg->state == I2S_TXRX_STATE_STOP))
    {
        bk_dma_start(drv_info->chl2_cfg->rx_cfg->dma_id);
        drv_info->chl2_cfg->rx_cfg->state = I2S_TXRX_STATE_START;
    }

    if (drv_info->chl3_cfg &&
        drv_info->chl3_cfg->tx_cfg &&
        (drv_info->chl3_cfg->tx_cfg->state == I2S_TXRX_STATE_IDLE || drv_info->chl3_cfg->tx_cfg->state == I2S_TXRX_STATE_STOP))
    {
        bk_dma_start(drv_info->chl3_cfg->tx_cfg->dma_id);
        drv_info->chl3_cfg->tx_cfg->state = I2S_TXRX_STATE_START;
    }

    if (drv_info->chl3_cfg &&
        drv_info->chl3_cfg->rx_cfg &&
        (drv_info->chl3_cfg->rx_cfg->state == I2S_TXRX_STATE_IDLE || drv_info->chl3_cfg->rx_cfg->state == I2S_TXRX_STATE_STOP))
    {
        bk_dma_start(drv_info->chl3_cfg->rx_cfg->dma_id);
        drv_info->chl3_cfg->rx_cfg->state = I2S_TXRX_STATE_START;
    }

    switch (drv_info->config.work_mode) {
        case I2S_WORK_MODE_I2S:
        case I2S_WORK_MODE_LEFTJUST:
        case I2S_WORK_MODE_RIGHTJUST:
            bk_i2s_write_data(I2S_CHANNEL_2, &temp_data, 1);
            bk_i2s_write_data(I2S_CHANNEL_3, &temp_data, 1);
            break;

        case I2S_WORK_MODE_SHORTFAMSYNC:
        case I2S_WORK_MODE_LONGFAMSYNC:
            if (drv_info->config.parallel_en == I2S_PARALLEL_ENABLE) {
                bk_i2s_write_data(I2S_CHANNEL_2, &temp_data, 1);
                bk_i2s_write_data(I2S_CHANNEL_3, &temp_data, 1);
            } else {
                if (drv_info->chl2_cfg != NULL) {
                    bk_i2s_write_data(I2S_CHANNEL_2, &temp_data, 1);
                }
                if (drv_info->chl3_cfg != NULL) {
                    bk_i2s_write_data(I2S_CHANNEL_3, &temp_data, 1);
                }
            }
            break;

        case I2S_WORK_MODE_NORMAL2BD:
        case I2S_WORK_MODE_DELAY2BD:
            drv_info->config.parallel_en = I2S_PARALLEL_ENABLE;
            bk_i2s_write_data(I2S_CHANNEL_2, &temp_data, 1);
            bk_i2s_write_data(I2S_CHANNEL_3, &temp_data, 1);
            break;

        case I2S_WORK_MODE_RSVD:
            break;

        default:
            break;
    }

    return BK_OK;
}

/**
 * @brief Resume I2S transmission
 * @return BK_OK on success, error code otherwise
 */
bk_err_t bk_i2s_resume(i2s_txrx_type_t txrx_type)
{
	i2s_drv_info_t *drv_info;
	uint8_t i2s_index = i2s_hal_get_cfg_index();

    bk_err_t ret = i2s_validate_and_get_drv_info(i2s_index, &drv_info);
    if (ret != BK_OK) {
        return ret;
    }

	if (txrx_type == I2S_TXRX_TYPE_TX) {
		/* chl1_tx */
		if (drv_info->chl1_cfg &&
			drv_info->chl1_cfg->tx_cfg &&
			(drv_info->chl1_cfg->tx_cfg->state == I2S_TXRX_STATE_IDLE || drv_info->chl1_cfg->tx_cfg->state == I2S_TXRX_STATE_STOP))
		{
			bk_dma_start(drv_info->chl1_cfg->tx_cfg->dma_id);
			drv_info->chl1_cfg->tx_cfg->state = I2S_TXRX_STATE_START;
		}

		/* chl2_tx */
		if (drv_info->chl2_cfg &&
			drv_info->chl2_cfg->tx_cfg &&
			(drv_info->chl2_cfg->tx_cfg->state == I2S_TXRX_STATE_IDLE || drv_info->chl2_cfg->tx_cfg->state == I2S_TXRX_STATE_STOP))
		{
			bk_dma_start(drv_info->chl2_cfg->tx_cfg->dma_id);
			drv_info->chl2_cfg->tx_cfg->state = I2S_TXRX_STATE_START;
		}

		/* chl3_tx */
		if (drv_info->chl3_cfg &&
			drv_info->chl3_cfg->tx_cfg &&
			(drv_info->chl3_cfg->tx_cfg->state == I2S_TXRX_STATE_IDLE || drv_info->chl3_cfg->tx_cfg->state == I2S_TXRX_STATE_STOP))
		{
			bk_dma_start(drv_info->chl3_cfg->tx_cfg->dma_id);
			drv_info->chl3_cfg->tx_cfg->state = I2S_TXRX_STATE_START;
		}
	} else if (txrx_type == I2S_TXRX_TYPE_RX) {
		/* chl1_rx */
		if (drv_info->chl1_cfg &&
			drv_info->chl1_cfg->rx_cfg &&
			(drv_info->chl1_cfg->rx_cfg->state == I2S_TXRX_STATE_IDLE || drv_info->chl1_cfg->rx_cfg->state == I2S_TXRX_STATE_STOP))
		{
			bk_dma_start(drv_info->chl1_cfg->rx_cfg->dma_id);
			drv_info->chl1_cfg->rx_cfg->state = I2S_TXRX_STATE_START;
		}

		/* chl2_rx */
		if (drv_info->chl2_cfg &&
			drv_info->chl2_cfg->rx_cfg &&
			(drv_info->chl2_cfg->rx_cfg->state == I2S_TXRX_STATE_IDLE || drv_info->chl2_cfg->rx_cfg->state == I2S_TXRX_STATE_STOP))
		{
			bk_dma_start(drv_info->chl2_cfg->rx_cfg->dma_id);
			drv_info->chl2_cfg->rx_cfg->state = I2S_TXRX_STATE_START;
		}

		/* chl3_rx */
		if (drv_info->chl3_cfg &&
			drv_info->chl3_cfg->rx_cfg &&
			(drv_info->chl3_cfg->rx_cfg->state == I2S_TXRX_STATE_IDLE || drv_info->chl3_cfg->rx_cfg->state == I2S_TXRX_STATE_STOP))
		{
			bk_dma_start(drv_info->chl3_cfg->rx_cfg->dma_id);
			drv_info->chl3_cfg->rx_cfg->state = I2S_TXRX_STATE_START;
		}
	}

	return BK_OK;
}

/**
 * @brief Stop I2S transmission
 * @return BK_OK on success, error code otherwise
 */
bk_err_t bk_i2s_stop(void)
{
    i2s_drv_info_t *drv_info;
    uint8_t i2s_index = i2s_hal_get_cfg_index();
    bk_err_t ret;

    ret = i2s_validate_and_get_drv_info(i2s_index, &drv_info);
    if (ret != BK_OK) {
        return ret;
    }

    ret = i2s_stop_all_channels(drv_info);
    if (ret != BK_OK) {
        return ret;
    }

    bk_i2s_enable(I2S_DISABLE);

    return BK_OK;
}

/**
 * @brief Pause I2S transmission
 * @return BK_OK on success, error code otherwise
 */
bk_err_t bk_i2s_pause(i2s_txrx_type_t txrx_type)
{
	i2s_drv_info_t *drv_info;
	uint8_t i2s_index = i2s_hal_get_cfg_index();
    bk_err_t ret = i2s_validate_and_get_drv_info(i2s_index, &drv_info);
    if (ret != BK_OK) {
        return ret;
    }

	if (txrx_type == I2S_TXRX_TYPE_TX) {
		/* chl1_tx */
		if (drv_info->chl1_cfg &&
			drv_info->chl1_cfg->tx_cfg &&
			drv_info->chl1_cfg->tx_cfg->state == I2S_TXRX_STATE_START)
		{
			bk_dma_stop(drv_info->chl1_cfg->tx_cfg->dma_id);
			drv_info->chl1_cfg->tx_cfg->state = I2S_TXRX_STATE_STOP;
		}

		/* chl2_tx */
		if (drv_info->chl2_cfg &&
			drv_info->chl2_cfg->tx_cfg &&
			drv_info->chl2_cfg->tx_cfg->state == I2S_TXRX_STATE_START)
		{
			bk_dma_stop(drv_info->chl2_cfg->tx_cfg->dma_id);
			drv_info->chl2_cfg->tx_cfg->state = I2S_TXRX_STATE_STOP;
		}

		/* chl3_tx */
		if (drv_info->chl3_cfg &&
			drv_info->chl3_cfg->tx_cfg &&
			drv_info->chl3_cfg->tx_cfg->state == I2S_TXRX_STATE_START)
		{
			bk_dma_stop(drv_info->chl3_cfg->tx_cfg->dma_id);
			drv_info->chl3_cfg->tx_cfg->state = I2S_TXRX_STATE_STOP;
		}
	} else if (txrx_type == I2S_TXRX_TYPE_RX) {
		/* chl1_rx */
		if (drv_info->chl1_cfg &&
			drv_info->chl1_cfg->rx_cfg &&
			drv_info->chl1_cfg->rx_cfg->state == I2S_TXRX_STATE_START)
		{
			bk_dma_stop(drv_info->chl1_cfg->rx_cfg->dma_id);
			drv_info->chl1_cfg->rx_cfg->state = I2S_TXRX_STATE_STOP;
		}

		/* chl2_rx */
		if (drv_info->chl2_cfg &&
			drv_info->chl2_cfg->rx_cfg &&
			drv_info->chl2_cfg->rx_cfg->state == I2S_TXRX_STATE_START)
		{
			bk_dma_stop(drv_info->chl2_cfg->rx_cfg->dma_id);
			drv_info->chl2_cfg->rx_cfg->state = I2S_TXRX_STATE_STOP;
		}

		/* chl3_rx */
		if (drv_info->chl3_cfg &&
			drv_info->chl3_cfg->rx_cfg &&
			drv_info->chl3_cfg->rx_cfg->state == I2S_TXRX_STATE_START)
		{
			bk_dma_stop(drv_info->chl3_cfg->rx_cfg->dma_id);
			drv_info->chl3_cfg->rx_cfg->state = I2S_TXRX_STATE_STOP;
		}
	}

	return BK_OK;
}
