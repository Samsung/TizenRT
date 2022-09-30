/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ble_tizenrt_scatternet_link_mgr.c
   * @brief     Multilink manager functions.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <ble_tizenrt_scatternet_link_mgr.h>
#include <trace_app.h>
#include <string.h>

/*============================================================================*
 *                              Constants
 *============================================================================*/
#if F_BT_LE_USE_STATIC_RANDOM_ADDR
/** @brief  Define start offset of the flash to save static random address. */
#define BLE_SCATTERNET_APP_STATIC_RANDOM_ADDR_OFFSET 0
#endif

/*============================================================================*
 *                              Variables
 *============================================================================*/
/** @addtogroup  CENTRAL_CLIENT_GAP_MSG
    * @{
    */

#if F_BT_LE_USE_STATIC_RANDOM_ADDR
/**
 * @brief   Save static random address information into flash.
 * @param[in] p_addr Pointer to the buffer for saving data.
 * @retval 0 Save success.
 * @retval other Failed.
 */
uint32_t ble_tizenrt_scatternet_app_save_static_random_address(T_APP_STATIC_RANDOM_ADDR *p_addr)
{
    APP_PRINT_INFO0("ble_central_app_save_static_random_address");
    return ftl_save(p_addr, BLE_CENTRAL_APP_STATIC_RANDOM_ADDR_OFFSET, sizeof(T_APP_STATIC_RANDOM_ADDR));
}
/**
  * @brief  Load static random address information from storage.
  * @param[out]  p_addr Pointer to the buffer for loading data.
  * @retval 0 Load success.
  * @retval other Failed.
  */
uint32_t ble_tizenrt_scatternet_app_load_static_random_address(T_APP_STATIC_RANDOM_ADDR *p_addr)
{
    uint32_t result;
    result = ftl_load(p_addr, BLE_SCATTERNET_APP_STATIC_RANDOM_ADDR_OFFSET,
                      sizeof(T_APP_STATIC_RANDOM_ADDR));
    APP_PRINT_INFO1("ble_central_app_load_static_random_address: result 0x%x", result);
    if (result)
    {
        memset(p_addr, 0, sizeof(T_APP_STATIC_RANDOM_ADDR));
    }
    return result;
}
#endif

/** @} */
