/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_vendor.h
  * @brief   This file contains all the vendor functions prototypes for GAP.
  * @details This file is used both bredr and le.
  * @author  jane
  * @date    2017-02-18
  * @version v1.0
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_VENDOR_H
#define GAP_VENDOR_H

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include "upperstack_config.h"
#include "gap.h"
#include "bt_types.h"

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_VENDOR GAP Vendor
  * @brief GAP vendor module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_Vendor_Exported_Macros GAP Vendor Exported Macros
  * @{
  */

/** @defgroup GAP_VENDOR_MSG_TYPE GAP Vendor Msg Type
 * @{
 */
#define GAP_MSG_VENDOR_CMD_RSP          0x00 //!<gap_vendor_cmd_req
#define GAP_MSG_VENDOR_EVT_INFO         0x01
/**
  * @}
  */

/** End of GAP_Vendor_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_Vendor_Exported_Types GAP Common Exported Types
  * @{
  */

typedef struct
{
    uint16_t    command;        /**< Vendor command. Should be the same as that in request. */
    uint16_t    cause;          /**< Result of executing command. */
    bool        is_cmpl_evt;    /**< Whether the response is generated from command complete event. */
    uint8_t     param_len;      /**< Response parameter length. */
    uint8_t     param[1];       /**< Response parameter buffer. */
} T_GAP_VENDOR_CMD_RSP;

typedef struct
{
    uint8_t     param_len;  /**< Information parameter length. */
    uint8_t     param[1];   /**< Information parameter buffer. */
} T_GAP_VENDOR_EVT_INFO;

/** @brief Reponse for gap_vendor_cmd_req */
typedef union
{
    T_GAP_VENDOR_CMD_RSP         *p_gap_vendor_cmd_rsp;
    T_GAP_VENDOR_EVT_INFO        *p_gap_vendor_evt_info;
} T_GAP_VENDOR_CB_DATA;

/** End of GAP_Vendor_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * @defgroup GAP_Vendor_EXPORT_Functions GAP Vendor Exported Functions
 *
 * @{
 */

/**
 * @brief  Register callback to gap, when messages in @ref GAP_VENDOR_MSG_TYPE happens, it will callback to app.
 * @param[in] vendor_callback Callback function provided by the APP to handle gap vendor command messages sent from the GAP.
 *              @arg NULL -> Not send gap vendor command messages to APP.
 *              @arg Other -> Use application defined callback function.
 * @return void
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_gap_init(void)
    {
        ......
        gap_register_vendor_cb(app_gap_vendor_callback);
    }
    void app_gap_vendor_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_GAP_VENDOR_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_GAP_VENDOR_CB_DATA));
        APP_PRINT_INFO1("app_gap_common_callback: cb_type = %d", cb_type);
        switch (cb_type)
        {
        case GAP_MSG_VENDOR_CMD_RSP:
            APP_PRINT_INFO4("GAP_MSG_VENDOR_CMD_RSP: command 0x%x, cause 0x%x, is_cmpl_evt %d, param_len %d",
                            cb_data.p_gap_vendor_cmd_rsp->command,
                            cb_data.p_gap_vendor_cmd_rsp->cause,
                            cb_data.p_gap_vendor_cmd_rsp->is_cmpl_evt,
                            cb_data.p_gap_vendor_cmd_rsp->param_len);
            break;
        case GAP_MSG_VENDOR_EVT_INFO:
            APP_PRINT_INFO1("GAP_MSG_VENDOR_EVT_INFO: param_len %d",
                            cb_data.p_gap_vendor_evt_info->param_len);
            break;
        default:
            break;
        }
        return;
    }
   \endcode
 */
void gap_register_vendor_cb(P_FUN_GAP_APP_CB vendor_callback);

/**
 * @brief  Send vendor command.
 * @param[in] op       Opcode of command.
 * @param[in] len      Length of parameters.
 * @param[in] p_param  Pointer to parameters to write.
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS Send request successfully.
 * @retval Others Failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    //send vendor command
    void test(void)
    {
        T_GAP_CAUSE cause = GAP_CAUSE_SUCCESS;
        uint16_t opcode;
        uint8_t param[2];
        param[0] = 0;
        param[1] = 0;
        cause = gap_vendor_cmd_req(opcode, 2, param);
    }

    //register callback, and handle response
    void app_le_gap_init(void)
    {
        ......
        gap_register_vendor_cb(app_gap_vendor_callback);
    }
    void app_gap_vendor_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_GAP_VENDOR_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_GAP_VENDOR_CB_DATA));
        APP_PRINT_INFO1("app_gap_common_callback: cb_type = %d", cb_type);
        switch (cb_type)
        {
        case GAP_MSG_VENDOR_CMD_RSP:
            APP_PRINT_INFO4("GAP_MSG_VENDOR_CMD_RSP: command 0x%x, cause 0x%x, is_cmpl_evt %d, param_len %d",
                            cb_data.p_gap_vendor_cmd_rsp->command,
                            cb_data.p_gap_vendor_cmd_rsp->cause,
                            cb_data.p_gap_vendor_cmd_rsp->is_cmpl_evt,
                            cb_data.p_gap_vendor_cmd_rsp->param_len);
            break;
        case GAP_MSG_VENDOR_EVT_INFO:
            APP_PRINT_INFO1("GAP_MSG_VENDOR_EVT_INFO: param_len %d",
                            cb_data.p_gap_vendor_evt_info->param_len);
            break;
        default:
            break;
        }
        return;
    }
 * \endcode
 */
T_GAP_CAUSE gap_vendor_cmd_req(uint16_t op, uint8_t len, uint8_t *p_param);

/** @} */ /* End of group GAP_Vendor_EXPORT_Functions */

/** @} */ /* End of group GAP_VENDOR */

/** @} */  /* End of group GAP */

/*-------------------------------------------------------------------
-------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

#endif /* GAP_VENDOR_H */
