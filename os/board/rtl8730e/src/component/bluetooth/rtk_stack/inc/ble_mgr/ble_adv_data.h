/**
 * @file   ble_adv_data.h
 * @brief  struct and interface about ble adv data manager
 * @author leon
 * @date   2020.9.3
 * @version 1.0
 * @par Copyright (c):
         Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BLE_ADV_DATA__
#define _BLE_ADV_DATA__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup BLE_ADV_DATA Ble Adv Data
  * @brief Ble extended advertising data manager module
  * @{
  */

#include <stdint.h>
#include <stdbool.h>
#include "gap_msg.h"

/**
 *  @brief interface of enable adv for app user
 *  @param[in]  pp_handle   when add adv data success, manager return a handle for deleting
 *  @param[in]  adv_data_len  adv data length
 *  @param[in]  p_adv_data    pointer to adv data
 *  @param[in]  scan_resp_len scan rsp data length
 *  @param[in]  p_scan_resp   pointer to scan rsp data
 *  @return operation result
 *  @retval true  success
 *  @retval false failed
 * <b>Example usage</b>
 * \code{.c}
    void *p_le_xxxx_adv_handle = NULL;
    static uint8_t xxxx_adv_data[] =
    {
        0x02,
        GAP_ADTYPE_POWER_LEVEL,
        0x00,
    };
    static uint8_t xxxx_scan_rsp_data[] =
    {
        0x03,
        GAP_ADTYPE_APPEARANCE,
        LO_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
        HI_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
    };
    bool le_xxxx_adv_start()
    {
        if (p_le_xxxx_adv_handle != NULL)
        {
            APP_PRINT_WARN0("le_xxxx_adv_start: already started");
            return true;
        }
        if (ble_adv_data_add(&p_le_xxxx_adv_handle, sizeof(xxxx_adv_data),(uint8_t *)xxxx_adv_data,
                            sizeof(xxxx_scan_rsp_data), (uint8_t *)xxxx_scan_rsp_data))
        {
            return true;
        }
        return false;
    }
 * \endcode
 */
bool ble_adv_data_add(void **pp_handle, uint16_t adv_data_len, uint8_t *p_adv_data,
                      uint16_t scan_resp_len, uint8_t *p_scan_resp);

/**
 *  @brief interface of disable adv for app user
 *  @param[in]  pp_handle  adv set handle which get from add operation
 *  @return operation result
 *  @retval true  success
 *  @retval false failed
 *  \code{.c}
    void *p_le_xxxx_adv_handle = NULL;
    bool le_xxxx_adv_stop(void)
    {
        if (p_le_xxxx_adv_handle == NULL)
        {
            APP_PRINT_WARN0("le_xxxx_adv_stop: already stopped");
            return true;
        }

        if (ble_adv_data_del(p_le_xxxx_adv_handle))
        {
            p_le_xxxx_adv_handle = NULL;
            return true;
        }
        return false;
    }
 * \endcode
 */
bool ble_adv_data_del(void *p_handle);

/**
 *  @brief interface of enable adv data manager
 *  @return operation result
 *  @retval true  success
 *  @retval false failed
 *  \code{.c}
    void app_ble_device_handle_power_on(void)
    {
        ......
        ble_adv_data_enable();
        ......
    }
 * \endcode
 */
bool ble_adv_data_enable(void);

/**
 *  @brief interface of disenable adv data manager
 *  @return operation result
 *  @retval true  success
 *  @retval false failed
 *  \code{.c}
    void app_ble_device_handle_power_off(void)
    {
        ......
        ble_adv_data_disable();
        ......
    }
 * \endcode
 */
bool ble_adv_data_disable(void);

/** End of BLE_ADV_DATA
* @}
*/

#ifdef __cplusplus
}
#endif

#endif
