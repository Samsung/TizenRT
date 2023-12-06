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

/** @defgroup BLE_CONN Ble Conn
  * @brief Ble Conn Param Update
  * @{
  */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup BLE_CONN_Exported_Functions Ble Conn Functions
    * @{
    */

/**
*@brief  used to set prefer conn param
*@note
* @param[in] conn_id  Connection ID for this link.
* @param[in] conn_interval_min  Value range: 0x0006 - 0x0C80 (7.5ms - 4000ms, 1.25ms/step).
* @param[in] conn_interval_max  Value range: 0x0006 - 0x0C80 (7.5ms - 4000ms, 1.25ms/step).
* @param[in] conn_latency  Value range: 0x0000 - 0x01F3.
* @param[in] supervision_timeout  Value range: 0x000A - 0x0C80 (100ms - 32000ms, 10ms/step).
*@return  bool
*@retval  true  1.current conn param has successfully set into ble stack. or
*               2.current connection paramter has already meet the conditions, do not need to update. or
*               2.current link state is busy, may be update after link state change into idle.
*@retval  false has some errors.
*/
bool ble_set_prefer_conn_param(uint8_t conn_id, uint16_t conn_interval_min,
                               uint16_t conn_interval_max, uint16_t conn_latency,
                               uint16_t supervision_timeout);

/** @} */ /* End of group BLE_CONN_Exported_Functions */
/** End of BLE_CONN
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* _BLE_CONN_H_ */
