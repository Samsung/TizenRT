/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BLE_CONN_H_
#define _BLE_CONN_H_

#include "gap_le.h"
#include "gap_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup LE_CONN LE Connection
  * @brief LE Connection Param Update
  * @{
  */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup LE_CONN_Exported_Functions LE Connection Exported Functions
    * @{
    */

/**
*@brief  Used to set preferred connection parameters
*
* @param[in] conn_id  Connection ID for this link.
* @param[in] conn_interval_min  Value range: 0x0006 - 0x0C80 (7.5ms - 4000ms, 1.25ms/step).
* @param[in] conn_interval_max  Value range: 0x0006 - 0x0C80 (7.5ms - 4000ms, 1.25ms/step).
* @param[in] conn_latency  Value range: 0x0000 - 0x01F3.
* @param[in] supervision_timeout  Value range: 0x000A - 0x0C80 (100ms - 32000ms, 10ms/step).
*@return  Operation result.
*@retval  true
* \arg      Current connection parameters have successfully been set into the LE Host.
* \arg      Current connection parameters already meet the conditions, no need to update.
* \arg      Current link state is busy, it may be updated after the link state changes to idle.
*@retval  false There are some errors.
*/
bool ble_set_prefer_conn_param(uint8_t conn_id, uint16_t conn_interval_min,
                               uint16_t conn_interval_max, uint16_t conn_latency,
                               uint16_t supervision_timeout);

/** @} */ /* End of group LE_CONN_Exported_Functions */
/** End of LE_CONN
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* _BLE_CONN_H_ */
