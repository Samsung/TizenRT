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
#include <driver/i2s_types.h>
#include <modules/audio_ring_buff.h>


#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief I2S API
 * @defgroup bk_api_i2s I2S group
 * @{
 */


/**
 * @brief     Init i2s module driver
 *
 * This API init i2s driver :
 *  - Power on clock
 *  - config apll clock
 *  - enable interrupts
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_i2s_driver_init(void);

/**
 * @brief     Deinit i2s module driver
 *
 * This API deinit i2s driver :
 *  - Power down clock
 *  - reconfig apll clock to defailt value
 *  - disable interrupts
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_i2s_driver_deinit(void);

/**
 * @brief     Init i2s module
 *
 * This API init the i2s module:
 *  - Init gpios i2s used
 *  - Configure the i2s parameters
 *
 * Usage example:
 *
 *     //init i2s driver
 *     bk_i2s_driver_init();
 *
 *     //init i2s configure
 *     i2s_config_t i2s_config;
 *     i2s_config.i2s_en = I2S_DISABLE;
 *     i2s_config.role = I2S_ROLE_MASTER;
 *     i2s_config.work_mode = I2S_WORK_MODE_I2S;
 *     i2s_config.lrck_invert = I2S_LRCK_INVERT_DISABLE;
 *     i2s_config.sck_invert = I2S_SCK_INVERT_DISABLE;
 *     i2s_config.lsb_first_en = I2S_LSB_FIRST_DISABLE;
 *     i2s_config.sync_length = 0;
 *     i2s_config.data_length = 15;
 *     i2s_config.pcm_dlength = 0;
 *     i2s_config.sample_ratio = 0;
 *     i2s_config.sck_ratio = 0;
 *     i2s_config.parallel_en = I2S_PARALLEL_DISABLE;
 *     i2s_config.store_mode = I2S_LRCOM_STORE_16R16L;
 *     i2s_config.sck_ratio_h4b = 0;
 *     i2s_config.sample_ratio_h2b = 0;
 *     i2s_config.txint_level = I2S_TXINT_LEVEL_1;
 *     i2s_config.rxint_level = I2S_RXINT_LEVEL_1;
 *     bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
 *     CLI_LOGI("init i2s driver and config successful \r\n");
 *
 * @param id gpio config "GPIO6, GPIO7, GPIO8, GPIO9"/"GPIO40, GPIO41, GPIO42, GPIO43"
 * @param config i2s parameters include work role, work mode and so on
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_I2S_PARAM: config is NULL
 *    - BK_ERR_I2S_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_init(i2s_gpio_group_id_t id, const i2s_config_t *config);

/**
 * @brief     Deinit i2s module
 *
 * This API deinit the i2s module:
 *   - reconfig i2s parameters to default value
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_i2s_deinit(void);

/**
 * @brief     Get the i2s Rx fifo status
 *
 * This API get the i2s Rx fifo status that check whether the fifo data can be read
 *
 * @param read_flag save Rx fifo status
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_I2S_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_get_read_ready(uint32_t *read_flag);

/**
 * @brief     Get the i2s Tx fifo status
 *
 * This API get the i2s Tx fifo status that check whether the fifo data can be write
 *
 * @param read_flag save Tx fifo status
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_I2S_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_get_write_ready(uint32_t *write_flag);

/**
 * @brief     Enable i2s
 *
 * This API enable i2s
 *
 * @param en_value disable/enable i2s
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_enable(i2s_en_t en_value);

/**
 * @brief     Config i2s interrupt
 *
 * This API config i2s interrupt
 *
 * @param int_id interrupt id
 * @param value 0:disable 1:enable
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_int_enable(i2s_isr_id_t int_id, uint32_t value);

/**
 * @brief     Config i2s work role
 *
 * This API config i2s work role
 *
 * @param role work role slave/master
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_set_role(i2s_role_t role);

/**
 * @brief     Config i2s work mode
 *
 * This API config i2s work mode
 *
 * @param work_mode i2s work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_set_work_mode(i2s_work_mode_t work_mode);

/**
 * @brief     Config i2s lrck invert
 *
 * This API config i2s lrck invert
 *
 * @param lrckrp i2s lrck invert enable/disable
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_set_lrck_invert(i2s_lrck_invert_en_t lrckrp);

/**
 * @brief     Config i2s sck invert
 *
 * This API config i2s sck invert
 *
 * @param sck_invert i2s sck invert enable/disable
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_set_sck_invert(i2s_sck_invert_en_t sck_invert);

/**
 * @brief     Config i2s lsb first
 *
 * This API config i2s lsb first
 *
 * @param lsb_first i2s lsb first enable/disable
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_set_lsb_first(i2s_lsb_first_en_t lsb_first);

/**
 * @brief     Config i2s sync length
 *
 * This API config i2s sync length
 *
 * @param sync_len i2s sync length
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_set_sync_len(uint32_t sync_len);

/**
 * @brief     Config i2s data length
 *
 * This API config i2s data length
 *
 * @param data_len i2s data length
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_set_data_len(uint32_t data_len);

/**
 * @brief     Config i2s pcm D length of 2B+D
 *
 * This API config i2s pcm D length of 2B+D
 *
 * @param pcm_dlen pcm D length
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_set_pcm_dlen(uint32_t pcm_dlen);

/**
 * @brief     Config i2s store mode
 *
 * This API config i2s store mode
 *
 * @param store_mode lrcom store mode LRLR/16R16L
 *
 * @attention 1. LRLR mode: left and right channel data are written alternately in chronological order(L->R->L->R->......)
 * @attention 2. 16R16L mode: left and right channel data are written to FIFO in 32 bits, in which the lower 16 bits 
                 correspond to the left channel and the higher 16 bits correspond to the right channel({R,L}->{R,L}->......)
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_set_store_mode(i2s_lrcom_store_mode_t store_mode);

/**
 * @brief     clear i2s rx fifo
 *
 * This API clear i2s rx fifo
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_clear_rxfifo(void);

/**
 * @brief     clear i2s tx fifo
 *
 * This API clear i2s tx fifo
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_clear_txfifo(void);

/**
 * @brief     config i2s tx interrupt level
 *
 * This API config i2s tx interrupt level
 *
 * @param txint_level tx interrupt trigger level
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_set_txint_level(i2s_txint_level_t txint_level);

/**
 * @brief     config i2s rx interrupt level
 *
 * This API config i2s rx interrupt level
 *
 * @param rxint_level rx interrupt trigger level
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_set_rxint_level(i2s_rxint_level_t rxint_level);

/**
 * @brief     write data to i2s tx fifo
 *
 * This API write data to i2s tx fifo
 *
 * Usage example:
 *
 *     //init i2s driver
 *     bk_i2s_driver_init();
 *
 *     //init i2s configure
 *     i2s_config.i2s_en = I2S_DISABLE;
 *     i2s_config.role = I2S_ROLE_MASTER;
 *     i2s_config.work_mode = I2S_WORK_MODE_I2S;
 *     i2s_config.lrck_invert = I2S_LRCK_INVERT_DISABLE;
 *     i2s_config.sck_invert = I2S_SCK_INVERT_DISABLE;
 *     i2s_config.lsb_first_en = I2S_LSB_FIRST_DISABLE;
 *     i2s_config.sync_length = 0;
 *     i2s_config.data_length = 15;
 *     i2s_config.pcm_dlength = 0;
 *     i2s_config.sample_ratio = 0;
 *     i2s_config.sck_ratio = 0;
 *     i2s_config.parallel_en = I2S_PARALLEL_DISABLE;
 *     i2s_config.store_mode = I2S_LRCOM_STORE_16R16L;
 *     i2s_config.sck_ratio_h4b = 0;
 *     i2s_config.sample_ratio_h2b = 0;
 *     i2s_config.txint_level = I2S_TXINT_LEVEL_1;
 *     i2s_config.rxint_level = I2S_RXINT_LEVEL_1;
 *     bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
 *     CLI_LOGI("init i2s driver and config successful \r\n");
 *
 *     //set sample and bitclk ratio
 *     rate.datawidth = i2s_rate_table[i].datawidth;
 *     rate.samp_rate = i2s_rate_table[i].samp_rate;
 *     bk_i2s_set_ratio(&rate);
 *
 *     //enable i2s
 *     bk_i2s_enable(I2S_ENABLE);
 *     CLI_LOGI("enable i2s successful \r\n");
 *
 *     bk_i2s_get_write_ready(&write_flag);
 *     if (write_flag) 
 *         bk_i2s_write_data(1, &data_buf_rl, 1);
 *
 * Usage example:
 *
 *     //init i2s driver
 *     bk_i2s_driver_init();
 *
 *     //init i2s configure
 *     i2s_config_t i2s_config;
 *     i2s_config.i2s_en = I2S_DISABLE;
 *     i2s_config.role = I2S_ROLE_MASTER;
 *     i2s_config.work_mode = I2S_WORK_MODE_I2S;
 *     i2s_config.lrck_invert = I2S_LRCK_INVERT_DISABLE;
 *     i2s_config.sck_invert = I2S_SCK_INVERT_DISABLE;
 *     i2s_config.lsb_first_en = I2S_LSB_FIRST_DISABLE;
 *     i2s_config.sync_length = 0;
 *     i2s_config.data_length = 15;
 *     i2s_config.pcm_dlength = 0;
 *     i2s_config.sample_ratio = 0;
 *     i2s_config.sck_ratio = 0;
 *     i2s_config.parallel_en = I2S_PARALLEL_DISABLE;
 *     i2s_config.store_mode = I2S_LRCOM_STORE_16R16L;
 *     i2s_config.sck_ratio_h4b = 0;
 *     i2s_config.sample_ratio_h2b = 0;
 *     i2s_config.txint_level = I2S_TXINT_LEVEL_1;
 *     i2s_config.rxint_level = I2S_RXINT_LEVEL_1;
 *     bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
 *     CLI_LOGI("init i2s driver and config successful \r\n");
 *
 *     //set sample and bitclk ratio
 *     rate.datawidth = i2s_rate_table[i].datawidth;
 *     rate.samp_rate = i2s_rate_table[i].samp_rate;
 *     bk_i2s_set_ratio(&rate);
 *
 *     //enable i2s
 *     bk_i2s_enable(I2S_ENABLE);
 *     CLI_LOGI("enable i2s successful \r\n");
 *
 *     //get write ready status and write data
 *     bk_i2s_get_write_ready(&write_flag);
 *     if (write_flag) 
 *         bk_i2s_write_data(1, &data_buf_rl, 1);
 *
 * @param channel_id i2s channel 1/2/3/4
 * @param data_buf data to write
 * @param data_len data length to write
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_write_data(uint32_t channel_id, uint32_t *data_buf, uint32_t data_len);

/**
 * @brief     read data from i2s rx fifo
 *
 * This API read data from i2s rx fifo
 *
 * Usage example:
 *
 *     //init i2s driver
 *     bk_i2s_driver_init();
 *
 *     //init i2s configure
 *     i2s_config_t i2s_config;
 *     i2s_config.i2s_en = I2S_DISABLE;
 *     i2s_config.role = I2S_ROLE_MASTER;
 *     i2s_config.work_mode = I2S_WORK_MODE_I2S;
 *     i2s_config.lrck_invert = I2S_LRCK_INVERT_DISABLE;
 *     i2s_config.sck_invert = I2S_SCK_INVERT_DISABLE;
 *     i2s_config.lsb_first_en = I2S_LSB_FIRST_DISABLE;
 *     i2s_config.sync_length = 0;
 *     i2s_config.data_length = 15;
 *     i2s_config.pcm_dlength = 0;
 *     i2s_config.sample_ratio = 0;
 *     i2s_config.sck_ratio = 0;
 *     i2s_config.parallel_en = I2S_PARALLEL_DISABLE;
 *     i2s_config.store_mode = I2S_LRCOM_STORE_16R16L;
 *     i2s_config.sck_ratio_h4b = 0;
 *     i2s_config.sample_ratio_h2b = 0;
 *     i2s_config.txint_level = I2S_TXINT_LEVEL_1;
 *     i2s_config.rxint_level = I2S_RXINT_LEVEL_1;
 *     bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
 *     CLI_LOGI("init i2s driver and config successful \r\n");
 *
 *     //set sample and bitclk ratio
 *     rate.datawidth = i2s_rate_table[i].datawidth;
 *     rate.samp_rate = i2s_rate_table[i].samp_rate;
 *     bk_i2s_set_ratio(&rate);
 *
 *     //enable i2s
 *     bk_i2s_enable(I2S_ENABLE);
 *     CLI_LOGI("enable i2s successful \r\n");
 *
 *     //get read ready status and read data
 *     bk_i2s_get_read_ready(&read_flag);
 *     if (read_flag) 
 *         bk_i2s_read_data(&data_buf, 1);
 *
 * @param data_buf save data read from rx fifo
 * @param data_len: data length to read
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_read_data(uint32_t *data_buf, uint32_t data_len);

/**
 * @brief     get i2s data address
 *
 * This API get i2s data address
 *
 * @param i2s_data_addr save i2s data address
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_get_data_addr(i2s_channel_id_t chl_id, uint32_t *i2s_data_addr);

/**
 * @brief     config i2s sample rate
 *
 * This API config i2s sample rate
 *
 * @param rate sample rate and data width
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_set_samp_rate(i2s_samp_rate_t samp_rate);

/**
 * @brief     register i2s isr
 *
 * This API config register i2s isr
 *
 * Usage example:
 *
 *     void cli_i2s_master_txudf_isr(void *param)
 *     {
 *         CLI_LOGI("enter cli_i2s_txudf_isr \r\n");
 *     }
 *
 *     //register isr
 *     bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXUDF, cli_i2s_master_txudf_isr, NULL);
 *     CLI_LOGI("register i2s isr successful \r\n");
 *
 *     //enable i2s rxovf interrupt
 *     bk_i2s_int_enable(I2S_ISR_CHL1_TXUDF, I2S_INT_ENABLE);
 *     CLI_LOGI("enable i2s interrupt successful \r\n");
 *
 * @param isr_id i2s isr id
 * @param isr isr function
 * @param param isr parameters
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - BK_ERR_I2S_ISR_ID: i2s isr id is invalid
 *    - others: other errors.
 */
bk_err_t bk_i2s_register_i2s_isr(i2s_isr_id_t isr_id, i2s_isr_t isr, void *param);

/**
 * @brief     init i2s channel to send or receive data by dma
 *
 * This API config dma to carry i2s data
 *
 * @param chl i2s channel
 * @type Tx or Rx config
 * @buff_size the size(byte) of ring buffer to send or receive data
 * @data_handle_cb the callback called by dma finish isr when dma carry "buff_size/2" data complete
 * @rb save RingBufferContext, and send or receive data use it
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_chl_init(i2s_channel_id_t chl, i2s_txrx_type_t type, uint32_t buff_size, i2s_data_handle_cb data_handle_cb, RingBufferContext **rb);

/**
 * @brief     deinit i2s channel
 *
 * This API config dma to carry i2s data
 *
 * @param chl i2s channel
 * @type Tx or Rx config
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_chl_deinit(i2s_channel_id_t chl, i2s_txrx_type_t type);

/**
 * @brief     start i2s
 *
 * This API start dma and i2s to send and receive data
 *
 * Usage example:
 *
 *     static RingBufferContext *ch1_tx_rb;
 *
 *     //channel 1 tx callback
 *     static int ch1_tx_data_handle_cb(uint32_t size)
 *     {
 *         ring_buffer_write(ch1_tx_rb, ch1_temp, size);
 *         return size;
 *     }
 *
 *     i2s_config_t i2s_config = DEFAULT_I2S_CONFIG();
 *     //init i2s driver
 *     bk_i2s_driver_init();
 *     //init and config i2s
 *     bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
 *     bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, 640, ch1_tx_data_handle_cb, &ch1_tx_rb);
 *     uint8_t *temp_data = (uint8_t *)os_malloc(640);
 *     os_memset(temp_data, 0x00, 640);
 *     size = ring_buffer_write(ch1_tx_rb, temp_data, 640);
 *     os_printf("ring_buffer_write, size: %d \n", size);
 *     os_free(temp_data);
 *     bk_i2s_start();
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - BK_ERR_I2S_ISR_ID: i2s isr id is invalid
 *    - others: other errors.
 */
bk_err_t bk_i2s_start(void);

/**
 * @brief     stop i2s
 *
 * This API stop i2s and dma to carry i2s data
 *
 * Usage example:
 *
 *     bk_i2s_stop();
 *     bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
 *     bk_i2s_deinit();
 *     bk_i2s_driver_deinit();
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_stop(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
