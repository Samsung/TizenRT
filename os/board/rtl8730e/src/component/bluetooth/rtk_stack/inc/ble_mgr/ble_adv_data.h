/**
 * @file   ble_adv_data.h
 * @brief  Struct and interface about LE ADV data manager
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

/** @defgroup LE_ADV_DATA LE Extended ADV Data
  * @brief LE extended advertising data manager module
  * @{
  */

#include <stdint.h>
#include <stdbool.h>
#include "gap_msg.h"

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup LE_ADV_DATA_Exported_Functions LE Extended ADV Data Exported Functions
  * @{
  */

/**
 *  @brief Interface of enable ADV for APP user
 *  @param[in]  pp_handle     When add ADV data success, manager return a handle for deleting.
 *  @param[in]  adv_data_len  ADV data length.
 *  @param[in]  p_adv_data    Pointer to ADV data.
 *  @param[in]  scan_resp_len Scan rsp data length.
 *  @param[in]  p_scan_resp   Pointer to scan rsp data.
 *  @return Operation result.
 *  @retval true  Success.
 *  @retval false Failed.
 *
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
 *  @brief Interface of disable ADV for APP user
 *  @param[in]  p_handle  ADV set handle which get from add operation.
 *  @return Operation result.
 *  @retval true  Success.
 *  @retval false Failed.
 *
 * <b>Example usage</b>
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
 *  @brief Interface of enable ADV data manager
 *  @return Operation result.
 *  @retval true  Success.
 *  @retval false Failed.
 *
 * <b>Example usage</b>
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
 *  @brief Interface to disable ADV data manager
 *  @return Operation result.
 *  @retval true  Success.
 *  @retval false Failed.
 *
 * <b>Example usage</b>
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
/** End of LE_ADV_DATA_Exported_Functions
 * @}
 */

/** End of LE_ADV_DATA
* @}
*/

#ifdef __cplusplus
}
#endif

#endif
