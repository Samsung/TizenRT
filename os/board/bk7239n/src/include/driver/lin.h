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

#include <stdbool.h>
#include <driver/lin_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief     Select the LIN gpio
 *
 * This API adaptation hardware link channel
 *
 * @param chn gpio channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_gpio_init(lin_channel_t chn);

/**
 * @brief     the LIN config
 *
 * This API lin configure interface
 *
 * @param cfg basic configuration information
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_cfg(lin_config_t *cfg);

/**
 * @brief     Init the LIN driver
 *
 * This API init the resoure common:
 *   - Init LIN driver control memory
 *
 * @attention 1. This API should be called before any other LIN APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_driver_init(void);

/**
 * @brief     Deinit the LIN driver
 *
 * This API free all resource related to LIN and disable LIN.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_driver_deinit(void);


/**
 * @brief    Enable LIN interrupt
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_interrupt_enable(void);

/**
 * @brief    Disable LIN interrupt
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_interrupt_disable(void);

//bk_err_t bk_lin_cfg(lin_config_t *cfg);

/**
 * @brief     Register the interrupt service routine for LIN
 *
 * This API register lin isr callback function.
 *
 * @param type register interrupt type
 * @param isr lin callback
 * @param arg lin callback arg
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_register_isr(lin_int_type_t type, lin_isr_t isr, void *arg);

/**
 * @brief     Set this device as master or slave
 *
 * @param lin_dev_t device type
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_set_dev(lin_dev_t dev);

/**
 * @brief     Get this device as master or slave
 *
 * @param lin_dev_t device type
 *
 * @return
 *    - 0: slave
 *    - 1: master
 *    - others: other errors.
 */
bk_err_t bk_lin_get_dev(void);

/**
 * @brief     Set this device ident
 *
 * This interface will set the id and transmission direction of the data frame
 *
 * @param ident bit3 for master indicates whether this frame is a sending or receving
 * bit [4] is direction :0 recv,1 send bit[3:0] is id number
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_set_ident(uint32_t ident);

/**
 * @brief     get this device ident
 *
 * get identifer and transmittion direction
 *
 * @return
 *    - bit [4] is direction :0 recv,1 send bit[3:0] is id number
 */
bk_err_t bk_lin_get_ident(void);

/**
 * @brief     Set this device bit rate (master only)
 *
 * This interface will set data rate between 1 Kbit/s and 20 Kbit/s (for master)
 * automatic bit rate detection (for slave)
 *
 * @param rate data rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_set_rate(double rate);

/**
 * @brief     Get this device bit rate (master only)
 *
 * get data rate
 *
 * @return
 *    - data rate value
 */
double bk_lin_get_rate(void);

/**
 * @brief     Set the transmission data length
 *
 * the max data length is 8bytes
 *
 * @param len data length
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_set_data_length(lin_data_len_t len);

/**
 * @brief     Get the transmission data length
 *
 * the max data length is 8bytes
 *
 * @return
 *    - data length value.
 */
bk_err_t bk_lin_get_data_length(void);

/**
 * @brief     Set the checksum type
 *
 * @param checksum type:0 classic checksum,1 enhanced checksum
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_set_enh_check(lin_checksum_t check);

/**
 * @brief     Get the checksum type
 *
 * @return
 *    - checksum value.
 */
bk_err_t bk_lin_get_enh_check(void);

/**
 * @brief     Set bus inactivity time(slave only),
 *
 * This API will generate an bus idle timeout interrupt when it detectes no bus activity for a set time.
 * @param  time set timeout, 0:4s, 1:6s, 2:8s, 3:10s
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_set_bus_inactivity_time(lin_bus_inactivity_time_t time);

/**
 * @brief     Get bus inactivity time(slave only),
 *
 * @return
 *    - bus inactivity enum
 */
bk_err_t bk_lin_get_bus_inactivity_time(void);

/**
 * @brief     Set wake up repeat time(slave only),
 *
 * The time for repeating of wake up because of no reaction on the bus
 * @param  time set time, 0:180ms, 1:200ms, 2:220ms, 3:240ms
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_set_wup_repeat_time(lin_wup_repeat_time_t time);

/**
 * @brief     Get wake up repeat time(slave only),
 *
 * @return
 *    - repeat time enum
 */
bk_err_t bk_lin_get_wup_repeat_time(void);

/**
 * @brief     Set transmit operation,
 *
 * The API determines whether the current frame is transmit frame or a receive frame for the LIN node.
 *
 * @param  trans 1 - transmit operation, 0 - receive operation
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_set_transmit(lin_trans_t trans);

/**
 * @brief     get transmit operation,
 *
 * @param  trans 1 - transmit operation, 0 - receive operation
 *
 * @return
 *    - 1: transmit operation,
 *    - 0: receive operation,
 *    - others: other errors.
 */
bk_err_t bk_lin_get_transmit(void);

/**
 * @brief     load buf to the data bytes
 *
 *
 * @param  buf source data buffer
 * @param  len data length,max 8 bytes
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_send(uint8_t *buf, uint32_t len);

/**
 * @brief     load data bytes to buf,
 *
 * @param  buf destination data buffer
 * @param  len data length,max 8 bytes
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_recv(uint8_t *buf, uint32_t len);

/**
 * @brief     Send data based on specified id,
 * 
 * @param  id target id of sending data
 * @param  tx send data buffer
 * @param  len data length,max 8 bytes
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_tx(lin_id_t id, uint8_t *tx, uint32_t len);

/**
 * @brief     Recevie data based on specified id
 *
 *  This interface has blocking function. using freeROTS task notification mechanism
 *
 * The API recevie data  max 8 bytes
 *
 * @param  id target id to recevie data
 * @param  rx recevie data buffer
 * @param  len data length,max 8 bytes
 * @param  timeout The maximum amount of time that the task should wait in
 * the Blocked state for a notification to be received.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_rx(lin_id_t id, uint8_t *rx, uint32_t len, uint32_t timeout);

/**
 * @brief     Set start transmission request,
 *
 * The API is celled to start the LIN transmission after loading identifier,
 * data length and data buffer
 *
 * master only,slaver reserved
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */

bk_err_t bk_lin_set_start_req(void);

/**
 * @brief     Get start transmission request status,
 *
 * The API is celled to start the LIN transmission after loading identifier,
 * data length and data buffer
 *
 * master only,slaver reserved
 *
 * @return
 *    - 0: The transmission is finished or an error is occurred
 *    - 1: Transmitting.
 */
bk_err_t bk_lin_get_start_req(void);

/**
 * @brief     Set sleep mode,
 *
 * The API is used by LIN to determine whether the LIN bus is in sleep mode or not
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_set_sleep(void);

/**
 * @brief     Get sleep mode,
 *
 * @return
 *    - 1: sleep
 *	  - 0: wakeup
 *    - others: other errors.
 */
bk_err_t bk_lin_get_sleep(void);

/**
 * @brief     Set wakeup request,
 *
 * The API terminate the sleep mode of LIN bus by sending a wakeup signal
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_lin_set_wakeup(void);


#ifdef __cplusplus
}
#endif

