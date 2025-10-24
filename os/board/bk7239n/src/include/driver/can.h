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
#include <driver/can_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief  The CAN config
 *
 * @param[in]  can  structure of parameters
 *
 * @return     bk_err_t error code
 */
bk_err_t bk_can_init(can_dev_t *can);

/**
 * @brief CAN software module init
 *
 * @return     bk_err_t error code
 */
bk_err_t bk_can_driver_init(void);

/**
 * @brief CAN software module deinit
 *
 * @return     bk_err_t error code
 */
bk_err_t bk_can_driver_deinit(void);

/**
 * @brief free the CAN config
 *
 * @return     bk_err_t error code
 */
bk_err_t bk_can_deinit(void);

/**
 * @brief CAN receive frames
 *
 * @note    Pop out the data from software fifo
 *
 * @param[in]   data            buffer to store data in
 * @param[in]   expect_size     size of data expect to get
 * @param[in]   recv_size       size of data actually get
 * @param[in]   timeout         max wait time to achieve the expect size
 *
 * @return     bk_err_t error code
 */
bk_err_t bk_can_receive(uint8_t *data, uint32_t expect_size, uint32_t *recv_size, uint32_t timeout);

/**
 * @brief CAN send frame through primary transmit buffer
 *
 * @note    Put in fifo one message that would be transmitted at the next possible positon
 *
 * @param[in]   frame            One frame for transmission
 *
 * @return     bk_err_t error code
 */
bk_err_t bk_can_send_ptb(can_frame_s *frame);

/**
 * @brief CAN send frame through secondary transmit buffer
 *
 * @note    Put in fifo frames that would be sent out together
 *          Data in fifo would be conbined to reach the longest legal length
 *          Data share the same arbitration parameter
 *
 * @param[in]   frame            One frame for transmission
 * @param[in]   timeout          max wait time to send out all data
 *
 * @return     bk_err_t error code
 */
bk_err_t bk_can_send(can_frame_s *frame, uint32_t timeout);

/**
 * @brief CAN acceptance filter set
 *
 * @note    Only filter 0 accpet all messages by default
 *          Disabled bits result in accepting the message
 *
 * @param[in]   cmd            structure of parameter
 *
 * @return     bk_err_t error code
 */
bk_err_t bk_can_acc_filter_set(can_acc_filter_cmd_s *cmd);

/**
 * @brief CAN register callback
 *
 * @note    Register to send and receive completed interrupt callback funcitons.
 *          The callback funciton is called in the interrupt.
 *          Pay attention to the operation.
 *
 * @param[in]   rx_cb          Receive callback functions and parameters
 * @param[in]   tx_cb          Send callback functions and parameters
 *
 */
void bk_can_register_isr_callback(can_callback_des_t *rx_cb, can_callback_des_t *tx_cb);

/**
 * @brief CAN register callback
 *
 * @note    Register to Error interrupt callback funcitons.
 *          The callback funciton is called in the interrupt.
 *          Pay attention to the operation.
 *
 * @param[in]   err_cb          Error callback functions and parameters
 *
 */
void bk_can_register_err_callback(can_callback_des_t *err_cb);

/**
 * @brief   Adjusting the Baud Rate
 *
 * @note    Berfore sending data, the baud rate can be adjusted
 *          Pay attention to the protocol used to prevent speed mismatch.
 *
 * @param[in]   s_speed       CAN2.0 Baud rate
 * @param[in]   f_speed       CAN FD Baud rate
 *
 */
bk_err_t can_driver_bit_rate_config(can_bit_rate_e s_speed, can_bit_rate_e f_speed);

#ifdef __cplusplus
}
#endif

