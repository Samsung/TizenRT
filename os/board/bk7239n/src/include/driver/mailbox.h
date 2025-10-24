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
#include <driver/mailbox_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief MAILBOX API
 * @defgroup bk_api_mailbox MAILBOX API group
 * @{
 */

/**
 * @brief     Init the MAILBOX driver
 *
 * This API init the resoure common to all MAILBOXs:
 *   - Init MAILBOX driver control register
 *   - Configure MAILBOX interrupt handling function
 *
 * This API should be called before any other MAILBOX APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_mailbox_init(void);

/**
 * @brief     Deinit the MAILBOX driver
 *
 * This API free all resource related to MAILBOX.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_mailbox_deinit(void);
/**
 * @brief     Configure the MAILBOX cmd and data.
 *
 * This API is used to configure data. 
 * Write parameters to variables as required by the structure, mailbox_data_t.
 *
 * example:
 *       mailbox_data_t  send_data;
 *       uint32_t param0 = 123456;
 *       uint32_t param1 = 111;
 *       uint32_t param2 = 666;
 *       uint32_t param3 = 888;
 * mailbox_set_param(&send_data, param0, param1, param2, param3);
 *
 * @return
 *  - BK_OK: succeed
 *  - others: other errors. 
 */
bk_err_t bk_mailbox_set_param(mailbox_data_t *data, uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3);

/**
 * @brief     Register the callback function for MAILBOX
 *
 * @attention src : Associated with the CPU that needs to send data
 *            dst : Associated with the CPU of the current code
 *            execution system
 *            callback: After receiving the data, it is transmitted to
 *            the correspongding processing function.
 * example:
 *   The current code runs in CPU0 and receives data from CPU1.
 *
 *          src = MAILBOX_CPU1;
 *          dst = MAILBOX_CPU0;
 *          callback = receive_from_CPU_1;
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MAILBOX_SRC_DST: MAILBOX data transmission direction error 
 *    - BK_ERR_MAILBOX_CALLBACK: Insufficient space or wrong format of registered function, etc
 *    - others: other errors.
 */
bk_err_t bk_mailbox_recv_callback_register(mailbox_endpoint_t src, mailbox_endpoint_t dst, mailbox_callback_t callback);

/**
 * @brief     Unregister the callback function for MAILBOX
 *
 * @attention src : Associated with the CPU that needs to send data
 *            dst : Associated with the CPU of the current code
 *            execution system
 *            callback: After receiving the data, it is transmitted to
 *            the correspongding processing function.
 * example:
 *   The current code runs in CPU0 and receives data from CPU1.
 *
 *          src = MAILBOX_CPU1
 *          dst = MAILBOX_CPU0
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MAILBOX_SRC_DST: MAILBOX data transmission direction error.
 *    - BK_ERR_MAILBOX_CALLBACK: Callback function is not registered.
 *    - others: other errors.
 */
bk_err_t bk_mailbox_recv_callback_unregister(mailbox_endpoint_t src, mailbox_endpoint_t dst);

/**
 * @brief     Unregister the callback function for MAILBOX
 *
 * @attention Before sending data, need to use the mailbox_set_param()
 *            to configure the data format.
 *
 * example:
 *   The current code runs in CPU0 and send data to CPU1.
 *
 *          src = MAILBOX_ENDPOINT_CPU0
 *          dst = MAILBOX_CPU1
 *          data : mbx ('by mailbox_set_param(&mbx, p0, p1, p2, p3)')
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_MAILBOX_SRC_DST: MAILBOX data transmission direction error.
 *    - BK_ERR_MAILBOX_BOX:  No empty box store data.
 *    - BK_ERR_MAILBOX_TIMEOUT: Timeout waiting for the opposite end to fetch data.
 *    - others: other errors.
 */
bk_err_t bk_mailbox_send(mailbox_data_t *data, mailbox_endpoint_t src, mailbox_endpoint_t dst, void *arg);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif


