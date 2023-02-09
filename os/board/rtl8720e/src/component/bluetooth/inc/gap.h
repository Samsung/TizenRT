/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap.h
  * @brief   This file contains all the constants and functions prototypes for GAP.
  * @details This file is used both bredr and le.
  * @author  jane
  * @date    2017-02-18
  * @version v1.0
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_H
#define GAP_H

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2019

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <bt_flags.h>
#include <stdint.h>
#include <stdbool.h>
#include <bt_types.h>


/** @addtogroup GAP_COMMON GAP Common
  * @brief GAP common module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_Common_Exported_Macros GAP Common Exported Macros
  * @{
  */

/** @defgroup GAP_Common_Macros GAP Common Macros
  * @{
  */
#define GAP_BD_ADDR_LEN               6     //!< Default Bluetooth Device Address Length.
#define GAP_KEY_LEN                   16    //!< Default Long Term Key(LTK) length.
#define GAP_DEVICE_NAME_LEN           (39+1)//!< Max length of device name, if device name length exceeds it, it will be truncated.
#define GAP_PASSCODE_MAX              999999//!< Maximum Pairing Passcode/Passkey value.  Range of a passkey can be 0 - 999,999.
#define GAP_OOB_LEN                   16    //!< Length of out of band data.
/**
  * @}
  */

/** @defgroup BOND_PAIRING_MODE_DEFINES Pairing Modes
 * @{
 */
#define GAP_PAIRING_MODE_NO_PAIRING          0x00  //!< Pairing is not allowed.
#define GAP_PAIRING_MODE_PAIRABLE            0x01  //!< Pairable, Wait for a pairing request from master or security request from slave.
/**
  * @}
  */

/** @defgroup BOND_MITM_DEFINES Mitm And Bonding Flags
 * @{
 */
#define GAP_AUTHEN_BIT_NONE                  0      //!<  No authentication required.
#define GAP_AUTHEN_BIT_BONDING_FLAG          0x0001 //!<  Bonding is required
#define GAP_AUTHEN_BIT_MITM_FLAG             0x0004 //!<  Mitm is preferred
#if F_BT_LE_4_2_SC_SUPPORT
#define GAP_AUTHEN_BIT_SC_FLAG               0x0008 //!<  Secure connection is preferred
#define GAP_AUTHEN_BIT_SC_ONLY_FLAG          0x0200 //!<  Secure connection only mode for BLE is required
#endif
#define GAP_AUTHEN_BIT_FORCE_BONDING_FLAG    0x0100 //!<  Force bonding is required

/**
  * @}
  */

/** @defgroup GAP_COMMON_MSG_TYPE GAP Msg Type
 * @{
 */
#if F_BT_CONTROLLER_POWER_CONTROL
#define GAP_MSG_BT_POWER_ON_RSP         0x05 //!<gap_bt_power_on
#define GAP_MSG_BT_POWER_OFF_RSP        0x06 //!<gap_bt_power_off
#endif
/**
  * @}
  */

/** @defgroup GAP_VENDOR_MSG_TYPE GAP Vendor Msg Type
 * @{
 */
#define GAP_MSG_VENDOR_CMD_RSP          0x00 //!<gap_vendor_cmd_req
#define GAP_MSG_VENDOR_EVT_INFO         0x01
/**
  * @}
  */

/** End of GAP_Common_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_Common_Exported_Types GAP Common Exported Types
  * @{
  */

/** @brief I/O Capabilities */
typedef enum {
	GAP_IO_CAP_DISPLAY_ONLY,        //!<  Only a Display present, no Keyboard or Yes/No Keys.
	GAP_IO_CAP_DISPLAY_YES_NO,      //!<  Display and Yes/No Keys present.
	GAP_IO_CAP_KEYBOARD_ONLY,       //!<  Only a Keyboard present, no Display.
	GAP_IO_CAP_NO_INPUT_NO_OUTPUT,  //!<  No input/output capabilities.
	GAP_IO_CAP_KEYBOARD_DISPLAY,    //!<  Keyboard and Display present.
} T_GAP_IO_CAP;

/** @brief Key types */
typedef enum {
	GAP_KEY_LE_LOCAL_LTK          = 0x11, /**< BLE Long Term Key Blob. */
	GAP_KEY_LE_REMOTE_LTK         = 0x12, /**< BLE Long Term Key Blob. */
	GAP_KEY_LE_LOCAL_IRK          = 0x13, /**< BLE Identity Resolving Key. */
	GAP_KEY_LE_REMOTE_IRK         = 0x14, /**< BLE Identity Resolving Key. */
	GAP_KEY_LE_LOCAL_CSRK         = 0x15, /**< BLE Local CSR Key. */
	GAP_KEY_LE_REMOTE_CSRK        = 0x16, /**< BLE Remote CSR Key. */
	GAP_KEY_DELETE                = 0xFF  /**< Link key is no longer valid and should be deleted. */
} T_GAP_KEY_TYPE;

/** @brief GAP Remote Address Type */
typedef enum {
	GAP_REMOTE_ADDR_LE_PUBLIC          = 0x00, /**< LE Public device address type. */
	GAP_REMOTE_ADDR_LE_RANDOM          = 0x01, /**< LE Random device address type. */
#if F_BT_LE_PRIVACY_SUPPORT
	GAP_REMOTE_ADDR_LE_PUBLIC_IDENTITY = 0x02, /**< LE Public identity address type. */
	GAP_REMOTE_ADDR_LE_RANDOM_IDENTITY = 0x03, /**< LE Random identity address type. */
#endif
	GAP_REMOTE_ADDR_LE_ANONYMOUS       = 0xff, /**< LE anonymous device address type. */
} T_GAP_REMOTE_ADDR_TYPE;

/** @brief GAP Cause List */
typedef enum {
	GAP_CAUSE_SUCCESS           = 0x00,//!< Operation success.
	GAP_CAUSE_ALREADY_IN_REQ    = 0x01,//!< Operation already in progress.
	GAP_CAUSE_INVALID_STATE     = 0x02,//!< Invalid state.
	GAP_CAUSE_INVALID_PARAM     = 0x03,//!< Invalid parameter.
	GAP_CAUSE_NON_CONN          = 0x04,//!< No connection establishment.
	GAP_CAUSE_NOT_FIND_IRK      = 0x05,//!< IRK not found.
	GAP_CAUSE_ERROR_CREDITS     = 0x06,//!< Credits error.
	GAP_CAUSE_SEND_REQ_FAILED   = 0x07,//!< Send Request failed.
	GAP_CAUSE_NO_RESOURCE       = 0x08,//!< No resource.
	GAP_CAUSE_INVALID_PDU_SIZE  = 0x09,//!< Invalid PDU size.
	GAP_CAUSE_NOT_FIND          = 0x0a,//!< Not Found.
	GAP_CAUSE_CONN_LIMIT        = 0x0b,//!< Connection reachs limited count.
	GAP_CAUSE_NO_BOND           = 0x0c,//!< No Bond.
	GAP_CAUSE_ERROR_UNKNOWN     = 0xFF,//!< Unknown error.
} T_GAP_CAUSE;

/** @brief  GAP Confirmation Cause List */
typedef enum {
	GAP_CFM_CAUSE_ACCEPT = (GAP_ERR | GAP_ACCEPT),    /**< Accept for indication, only used when confirmation. */
	GAP_CFM_CAUSE_REJECT = (GAP_ERR | GAP_ERR_REJECT),/**< Reject for indication, only used when confirmation. */
} T_GAP_CFM_CAUSE;

/** @brief  APP Return Result List */
typedef enum {
	APP_RESULT_SUCCESS                    = (APP_SUCCESS),
	APP_RESULT_PENDING                    = (APP_ERR | APP_ERR_PENDING),
	APP_RESULT_ACCEPT                     = (APP_ERR | APP_ERR_ACCEPT),
	APP_RESULT_REJECT                     = (APP_ERR | APP_ERR_REJECT),
	APP_RESULT_NOT_RELEASE                = (APP_ERR | APP_ERR_NOT_RELEASE),

	APP_RESULT_PREP_QUEUE_FULL            = (ATT_ERR | ATT_ERR_PREP_QUEUE_FULL),
	APP_RESULT_INVALID_OFFSET             = (ATT_ERR | ATT_ERR_INVALID_OFFSET),
	APP_RESULT_INVALID_VALUE_SIZE         = (ATT_ERR | ATT_ERR_INVALID_VALUE_SIZE),
	APP_RESULT_INVALID_PDU                = (ATT_ERR | ATT_ERR_INVALID_PDU),
	APP_RESULT_ATTR_NOT_FOUND             = (ATT_ERR | ATT_ERR_ATTR_NOT_FOUND),
	APP_RESULT_ATTR_NOT_LONG              = (ATT_ERR | ATT_ERR_ATTR_NOT_LONG),
	APP_RESULT_INSUFFICIENT_RESOURCES     = (ATT_ERR | ATT_ERR_INSUFFICIENT_RESOURCES),
	APP_RESULT_APP_ERR                    = (ATT_ERR | ATT_ERR_MIN_APPLIC_CODE),
	APP_RESULT_CCCD_IMPROPERLY_CONFIGURED = (ATT_ERR | ATT_ERR_CCCD_IMPROPERLY_CONFIGURED),
	APP_RESULT_PROC_ALREADY_IN_PROGRESS   = (ATT_ERR | ATT_ERR_PROC_ALREADY_IN_PROGRESS),
} T_APP_RESULT;

/** @brief GAP Parameter List */
typedef enum {
	GAP_PARAM_BD_ADDR                        = 0x200,//!< Locol Device's Address. Read Only. Size is uint8[GAP_BD_ADDR_LEN]. This item is read from the controller.
	GAP_PARAM_BOND_PAIRING_MODE              = 0x202,//!< Pairing Mode of LE and BR/EDR. Write Only. Size is uint8_t. Default is GAP_PAIRING_MODE_PAIRABLE (@ref BOND_PAIRING_MODE_DEFINES).
	GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS = 0x203,//!< Man-In-The-Middle (MITM) basically turns on Passkey protection in the pairing algorithm. Read/Write. Size is uint8_t. Default is GAP_AUTHEN_NO_MITM_YES_BOND (@ref BOND_MITM_DEFINES).
	GAP_PARAM_BOND_IO_CAPABILITIES           = 0x204,//!< I/O capabilities.  Read/Write. Size is uint8_t. Default is GAP_IO_CAP_NO_INPUT_NO_OUTPUT (@ref T_GAP_IO_CAP).
#if F_BT_LE_SMP_OOB_SUPPORT
	GAP_PARAM_BOND_OOB_ENABLED               = 0x205,//!< OOB data available for pairing algorithm. Read/Write. Size is uint8_t. Default is 0(disabled).
#endif
	GAP_PARAM_BOND_LE_PAIRING_MODE           = 0x207,//!< LE Pairing Mode. Read/Write. Size is uint8_t. Default is GAP_PAIRING_MODE_PAIRABLE (@ref BOND_PAIRING_MODE_DEFINES).
} T_GAP_PARAM_TYPE;


typedef struct {
	uint16_t    command;        /**< Vendor command. Should be the same as that in request. */
	uint16_t    cause;          /**< Result of executing command. */
	bool        is_cmpl_evt;    /**< Whether the response is generated from command complete event. */
	uint8_t     param_len;      /**< Response parameter length. */
	uint8_t     param[1];       /**< Response parameter buffer. */
} T_GAP_VENDOR_CMD_RSP;

typedef struct {
	uint8_t     param_len;  /**< Information parameter length. */
	uint8_t     param[1];   /**< Information parameter buffer. */
} T_GAP_VENDOR_EVT_INFO;

#if F_BT_CONTROLLER_POWER_CONTROL
typedef struct {
	uint16_t     cause;
} T_GAP_BT_POWER_ON_RSP;

typedef struct {
	uint16_t     cause;
} T_GAP_BT_POWER_OFF_RSP;
#endif

/** @brief Reponse for airplane mode */
typedef union {
	uint16_t cause;
#if F_BT_CONTROLLER_POWER_CONTROL
	T_GAP_BT_POWER_ON_RSP        *p_gap_bt_power_on_rsp;
	T_GAP_BT_POWER_OFF_RSP       *p_gap_bt_power_off_rsp;
#endif
} T_GAP_CB_DATA;

/** @brief Reponse for gap_vendor_cmd_req */
typedef union {
	T_GAP_VENDOR_CMD_RSP         *p_gap_vendor_cmd_rsp;
	T_GAP_VENDOR_EVT_INFO        *p_gap_vendor_evt_info;
} T_GAP_VENDOR_CB_DATA;

/** End of GAP_Common_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * @defgroup GAP_COMMON_EXPORT_Functions GAP Common Exported Functions
 *
 * @{
 */

/**
  * @brief Callback for gap to notify app
  * @param[in] cb_type callback msy type @ref GAP_COMMON_MSG_TYPE.
  * @param[in] p_cb_data point to callback data @ref T_GAP_CB_DATA.
  * @retval void
  */
typedef void(*P_FUN_GAP_APP_CB)(uint8_t cb_type, void *p_cb_data);

/**
 * @brief  Make upper stack ready to use by sending the register request.
 * @param[in] evt_queue     Event queue handle which is created by APP.
 * @param[in] io_queue      IO message queue handle which is created by APP.
 * @param[in] msg_queue_elem_num  Number of GAP message queue which will be created by GAP module.
 * @return result
 * @retval true  Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_main_task(void *p_param)
    {
        uint8_t event;
        os_msg_queue_create(&io_queue_handle, MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
        os_msg_queue_create(&evt_queue_handle, MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

        gap_start_bt_stack(evt_queue_handle, io_queue_handle, MAX_NUMBER_OF_GAP_MESSAGE);

        while (true)
        {
            if (os_msg_recv(evt_queue_handle, &event, 0xFFFFFFFF) == true)
            {
                if (event == EVENT_IO_TO_APP)
                {
                    T_IO_MSG io_msg;
                    if (os_msg_recv(io_queue_handle, &io_msg, 0) == true)
                    {
                        app_handle_io_msg(io_msg);
                    }
                }
                else
                {
                    gap_handle_msg(event);
                }
            }
        }
    }
 * \endcode
 */
bool gap_start_bt_stack(void *evt_queue, void *io_queue, uint16_t msg_queue_elem_num);

/**
 * @brief  Register callback to gap, when messages in @ref GAP_COMMON_MSG_TYPE happens, it will callback to app.
 * @param[in] app_callback Callback function provided by the APP to handle gap common messages sent from the GAP.
 *              @arg NULL -> Not send gap common messages to APP.
 *              @arg Other -> Use application defined callback function.
 * @return void
 *
 * <b>Example usage</b>
 * \code{.c}
   void app_le_gap_init()
   {
       ...
       le_register_app_cb(app_gap_callback);
       gap_register_app_cb(app_gap_common_callback);
   }
   \endcode
 */
void gap_register_app_cb(P_FUN_GAP_APP_CB app_callback);

/**
  * @brief  Set a GAP parameter.
  *
  *         NOTE: You can call this function with a GAP Common Parameter type and it will set the
  *         GAP Parameter.  GAP Peripheral Parameters are defined in @ref T_GAP_PARAM_TYPE.
  *         If the "len" field sets to the size of a "uint16_t" ,the
  *         "p_value" field must point to a data with type of "uint16_t".
  *
  * @param[in]  param GAP parameter ID: @ref T_GAP_PARAM_TYPE.
  * @param[in]  len Length of data to write.
  * @param[in]  p_value Pointer to data to write.  This is dependent on
  *                  the parameter type and WILL be cast to the appropriate
  *                  data type (For example: if data type param is uint16, p_value will be cast to
  *                  pointer of uint16_t).
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_le_gap_init(void)
    {
        ...
        //GAP Bond Manager parameters
        uint8_t  auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;
        uint16_t auth_flags = GAP_AUTHEN_BIT_BONDING_FLAG | GAP_AUTHEN_BIT_MITM_FLAG;
        uint8_t  auth_io_cap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
        uint8_t  auth_oob = false;

        // Setup the GAP Bond Manager
        gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
        gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
        gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(auth_io_cap), &auth_io_cap);
        gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
        ...
    }
  * \endcode
  */
T_GAP_CAUSE gap_set_param(T_GAP_PARAM_TYPE param, uint8_t len, void *p_value);

/**
  * @brief  Get a GAP Common parameter.
  *
  *         NOTE: You can call this function with a GAP Common Parameter type and it will get a
  *         GAP Common Parameter.  GAP Common Parameters are defined in @ref T_GAP_PARAM_TYPE.
  *
  * @param[in]  param GAP parameter type: @ref T_GAP_PARAM_TYPE
  * @param[in,out]  p_value Pointer to location to get the value.  This is dependent on
  *                  the parameter ID and WILL be cast to the appropriate
  *                  data type (For example: if data type param is uint16, p_value will be cast to
  *                  pointer of uint16_t).
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        uint8_t bt_addr[6];
        gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
    }
  * \endcode
  */
T_GAP_CAUSE gap_get_param(T_GAP_PARAM_TYPE param, void *p_value);

/**
  * @brief  Make the device in pairable mode.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  */
T_GAP_CAUSE gap_set_pairable_mode(void);


/**
 * @brief  Handle GAP message received from BT stack or timeout event for GAP timer.
 * @param[in] event Event type which is received from APP event queue.
 * @return None.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_main_task(void *p_param)
    {
        uint8_t event;
        os_msg_queue_create(&io_queue_handle, MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
        os_msg_queue_create(&evt_queue_handle, MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

        gap_start_bt_stack(evt_queue_handle, io_queue_handle, MAX_NUMBER_OF_GAP_MESSAGE);

        while (true)
        {
            if (os_msg_recv(evt_queue_handle, &event, 0xFFFFFFFF) == true)
            {
                if (event == EVENT_IO_TO_APP)
                {
                    T_IO_MSG io_msg;
                    if (os_msg_recv(io_queue_handle, &io_msg, 0) == true)
                    {
                        app_handle_io_msg(io_msg);
                    }
                }
                else
                {
                    gap_handle_msg(event);
                }
            }
        }
    }
 * \endcode
 */
void gap_handle_msg(uint8_t event);

#if F_BT_CONTROLLER_POWER_CONTROL
/**
 * @brief  Turn on the power of the BT controller.
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_poweron(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_GAP_CAUSE cause = gap_bt_power_on();

        return (T_USER_CMD_PARSE_RESULT)cause;
    }
 * \endcode
 */
T_GAP_CAUSE gap_bt_power_on(void);
#endif

#if F_BT_CONTROLLER_POWER_CONTROL
/**
 * @brief  Turn off the power of the BT controller.
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval Others Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_poweroff(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_GAP_CAUSE cause = gap_bt_power_off();

        return (T_USER_CMD_PARSE_RESULT)cause;
    }
 * \endcode
 */
T_GAP_CAUSE gap_bt_power_off(void);
#endif

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

/** @} */ /* End of group GAP_COMMON_EXPORT_Functions */

/** @} */ /* End of group GAP_COMMON */

/*-------------------------------------------------------------------
-------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#elif UPPER_STACK_VERSION == VERSION_2021

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
#include "bt_types.h"
#include "gap_chann.h"

/** @addtogroup GAP_COMMON GAP Common
  * @brief GAP common module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_Common_Exported_Macros GAP Common Exported Macros
  * @{
  */

/** @defgroup GAP_Common_Macros GAP Common Macros
  * @{
  */
#define GAP_BD_ADDR_LEN               6     //!< Default Bluetooth Device Address Length.
#define GAP_KEY_LEN                   16    //!< Default Long Term Key(LTK) length.
#define GAP_DEVICE_NAME_LEN           (39+1)//!< Max length of device name, if device name length exceeds it, it will be truncated.
#define GAP_PASSCODE_MAX              999999//!< Maximum Pairing Passcode/Passkey value.  Range of a passkey can be 0 - 999,999.
#define GAP_OOB_LEN                   16    //!< Length of out of band data.
/**
  * @}
  */

/** @defgroup BOND_PAIRING_MODE_DEFINES Pairing Modes
 * @{
 */
#define GAP_PAIRING_MODE_NO_PAIRING          0x00  //!< Pairing is not allowed.
#define GAP_PAIRING_MODE_PAIRABLE            0x01  //!< Pairable, Wait for a pairing request from master or security request from slave.
/**
  * @}
  */

/** @defgroup BOND_MITM_DEFINES Mitm And Bonding Flags
 * @{
 */
#define GAP_AUTHEN_BIT_NONE                  0      //!<  No authentication required.
#define GAP_AUTHEN_BIT_BONDING_FLAG          0x0001 //!<  Bonding is required
#if F_BT_BREDR_SUPPORT
#define GAP_AUTHEN_BIT_GENERAL_BONDING_FLAG  0x0002 //!<  General bonding is required
#endif
#define GAP_AUTHEN_BIT_MITM_FLAG             0x0004 //!<  Mitm is preferred
#if F_BT_LE_4_2_SC_SUPPORT
#define GAP_AUTHEN_BIT_SC_FLAG               0x0008 //!<  Secure connection is preferred
#define GAP_AUTHEN_BIT_SC_ONLY_FLAG          0x0200 //!<  Secure connection only mode for BLE is required
#endif
#if F_BT_KEY_PRESS_SUPPORT
#define GAP_AUTHEN_BIT_KEYPRESS_FLAG         0x0010 //!<  Keypress notifaction is preferred
#endif
#if F_BT_SC_LINK_CONVERT_SUPPORT
#define GAP_AUTHEN_BIT_CT2_FLAG              0x0020 //!<  CT2 flag is preferred
#endif
#define GAP_AUTHEN_BIT_FORCE_BONDING_FLAG    0x0100 //!<  Force bonding is required
#if F_BT_BREDR_SUPPORT
#define GAP_AUTHEN_BIT_SC_BR_FLAG            0x0400 //!<  Secure connection for BR/EDR is required
#endif
#if F_BT_LE_GAP_CENTRAL_SUPPORT
#define GAP_AUTHEN_BIT_FORCE_CENTRAL_ENCRYPT_FLAG 0x0800 //!<  When stored keys are generated by no MITM pairing method, and MITM requirement is preferred, Central role prefers encrypting a connection directly rather than re-pair.
#endif

/**
  * @}
  */

#if F_BT_SC_LINK_CONVERT_SUPPORT
/** @defgroup BOND_KEY_CONVERT Link Key Conversion Flags
 * @{
 */
#define GAP_SC_KEY_CONVERT_NONE              0x00
#define GAP_SC_KEY_CONVERT_LE_TO_BREDR_FLAG  0x01
#endif
/**
  * @}
  */

/** @defgroup GAP_COMMON_MSG_TYPE GAP Msg Type
 * @{
 */
#define GAP_MSG_BT_HW_ERROR             0x07
#define GAP_MSG_GAP_CHANN_ADDED         0x08 //!<Information for L2CAP channel added. This L2CAP channel can transmit ATT PDU.
#define GAP_MSG_GAP_CHANN_DEL           0x09 //!<Information for L2CAP channel removed.
#define GAP_MSG_GAP_CHANN_MTU_UPDATE    0x0A //!<Information for L2CAP channel MTU updated.

#define GAP_MSG_GAP_READ_CONNECTION_ACCEPT_TIMEOUT             0x10
#define GAP_MSG_GAP_WRITE_CONNECTION_ACCEPT_TIMEOUT            0x11

#define GAP_MSG_GAP_CONFIGURE_DATA_PATH                        0x12
#define GAP_MSG_GAP_READ_LOCAL_SUPPORTED_CODECS_V2             0x13
#define GAP_MSG_GAP_READ_LOCAL_SUPPORTED_CODEC_CAPABILITIES    0x14
#define GAP_MSG_GAP_READ_LOCAL_SUPPORTED_CONTROLLER_DELAY      0x15
/**
  * @}
  */

/** End of GAP_Common_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_Common_Exported_Types GAP Common Exported Types
  * @{
  */

/** @brief I/O Capabilities */
typedef enum
{
    GAP_IO_CAP_DISPLAY_ONLY,        //!<  Only a Display present, no Keyboard or Yes/No Keys.
    GAP_IO_CAP_DISPLAY_YES_NO,      //!<  Display and Yes/No Keys present.
    GAP_IO_CAP_KEYBOARD_ONLY,       //!<  Only a Keyboard present, no Display.
    GAP_IO_CAP_NO_INPUT_NO_OUTPUT,  //!<  No input/output capabilities.
    GAP_IO_CAP_KEYBOARD_DISPLAY,    //!<  Keyboard and Display present.
} T_GAP_IO_CAP;

/** @brief Key types */
typedef enum
{
#if F_BT_BREDR_SUPPORT
    GAP_KEY_COMBINATION           = 0x00, /**< Combination key. */
    GAP_KEY_LOCAL_UNIT            = 0x01, /**< Local unit key. */
    GAP_KEY_REMOTE_UNIT           = 0x02, /**< Remote unit key. */
    GAP_KEY_DEBUG_COMBINATION     = 0x03, /**< Debug combination key. */
    GAP_KEY_UNAUTHEN              = 0x04, /**< SSP generated link key without MITM protection. */
    GAP_KEY_AUTHEN                = 0x05, /**< SSP generated link key with MITM protection. */
    GAP_KEY_CHANGED_COMBINATION   = 0x06, /**< Changed combination key. */
    GAP_KEY_UNAUTHEN_P256         = 0x07, /**< Security Connections generated link key without MITM protection. */
    GAP_KEY_AUTHEN_P256           = 0x08, /**< Security Connections link key with MITM protection. */
#endif
    GAP_KEY_LE_LOCAL_LTK          = 0x11, /**< BLE Long Term Key Blob. */
    GAP_KEY_LE_REMOTE_LTK         = 0x12, /**< BLE Long Term Key Blob. */
    GAP_KEY_LE_LOCAL_IRK          = 0x13, /**< BLE Identity Resolving Key. */
    GAP_KEY_LE_REMOTE_IRK         = 0x14, /**< BLE Identity Resolving Key. */
    GAP_KEY_LE_LOCAL_CSRK         = 0x15, /**< BLE Local CSR Key. */
    GAP_KEY_LE_REMOTE_CSRK        = 0x16, /**< BLE Remote CSR Key. */
#if F_BT_BREDR_SUPPORT
    GAP_KEY_REQ_BR                = 0x80, /**< Only used to request a BR/EDR link key. */
#endif
    GAP_KEY_DELETE                = 0xFF  /**< Link key is no longer valid and should be deleted. */
} T_GAP_KEY_TYPE;

/** @brief  Keypress notification type.*/
typedef enum
{
    GAP_PASSKEY_ENTRY_STARTED,     /**< The passkey key entry protocol procedure is started. */
    GAP_PASSKEY_DIGIT_ENTERED,     /**< A digit is entered by the remote user. */
    GAP_PASSKEY_DIGIT_ERASED,      /**< A digit is erased by the remote user. */
    GAP_PASSKEY_CLEARED,           /**< The display is cleared by the remote user. */
    GAP_PASSKEY_ENTRY_CMPL,        /**< The passkey key entry protocol procedure is completed. */
} T_GAP_KEYPRESS_NOTIF_TYPE;

/** @brief GAP Remote Address Type */
typedef enum
{
    GAP_REMOTE_ADDR_LE_PUBLIC          = 0x00, /**< LE Public device address type. */
    GAP_REMOTE_ADDR_LE_RANDOM          = 0x01, /**< LE Random device address type. */
#if F_BT_LE_PRIVACY_SUPPORT
    GAP_REMOTE_ADDR_LE_PUBLIC_IDENTITY = 0x02, /**< LE Public identity address type. */
    GAP_REMOTE_ADDR_LE_RANDOM_IDENTITY = 0x03, /**< LE Random identity address type. */
#endif
#if F_BT_BREDR_SUPPORT
    GAP_REMOTE_ADDR_CLASSIC            = 0x10, /**< BR/EDR device address type. */
#endif
    GAP_REMOTE_ADDR_LE_ANONYMOUS       = 0xff, /**< LE anonymous device address type. */
} T_GAP_REMOTE_ADDR_TYPE;

/** @brief GAP Cause List */
typedef enum
{
    GAP_CAUSE_SUCCESS           = 0x00,//!< Operation success.
    GAP_CAUSE_ALREADY_IN_REQ    = 0x01,//!< Operation already in progress.
    GAP_CAUSE_INVALID_STATE     = 0x02,//!< Invalid state.
    GAP_CAUSE_INVALID_PARAM     = 0x03,//!< Invalid parameter.
    GAP_CAUSE_NON_CONN          = 0x04,//!< No connection establishment.
    GAP_CAUSE_NOT_FIND_IRK      = 0x05,//!< IRK not found.
    GAP_CAUSE_ERROR_CREDITS     = 0x06,//!< Credits error.
    GAP_CAUSE_SEND_REQ_FAILED   = 0x07,//!< Send Request failed.
    GAP_CAUSE_NO_RESOURCE       = 0x08,//!< No resource.
    GAP_CAUSE_INVALID_PDU_SIZE  = 0x09,//!< Invalid PDU size.
    GAP_CAUSE_NOT_FIND          = 0x0a,//!< Not Found.
    GAP_CAUSE_CONN_LIMIT        = 0x0b,//!< Connection reachs limited count.
    GAP_CAUSE_NO_BOND           = 0x0c,//!< No Bond.
    GAP_CAUSE_ERROR_UNKNOWN     = 0xFF,//!< Unknown error.
} T_GAP_CAUSE;

/** @brief  GAP Confirmation Cause List */
typedef enum
{
    GAP_CFM_CAUSE_ACCEPT = (GAP_ERR | GAP_ACCEPT),    /**< Accept for indication, only used when confirmation. */
    GAP_CFM_CAUSE_REJECT = (GAP_ERR | GAP_ERR_REJECT),/**< Reject for indication, only used when confirmation. */
} T_GAP_CFM_CAUSE;

/** @brief  APP Return Result List */
typedef enum
{
    APP_RESULT_SUCCESS                    = (APP_SUCCESS),
    APP_RESULT_PENDING                    = (APP_ERR | APP_ERR_PENDING),
    APP_RESULT_ACCEPT                     = (APP_ERR | APP_ERR_ACCEPT),
    APP_RESULT_REJECT                     = (APP_ERR | APP_ERR_REJECT),
    APP_RESULT_NOT_RELEASE                = (APP_ERR | APP_ERR_NOT_RELEASE),

    APP_RESULT_PREP_QUEUE_FULL            = (ATT_ERR | ATT_ERR_PREP_QUEUE_FULL),
    APP_RESULT_INVALID_OFFSET             = (ATT_ERR | ATT_ERR_INVALID_OFFSET),
    APP_RESULT_INVALID_VALUE_SIZE         = (ATT_ERR | ATT_ERR_INVALID_VALUE_SIZE),
    APP_RESULT_INVALID_PDU                = (ATT_ERR | ATT_ERR_INVALID_PDU),
    APP_RESULT_ATTR_NOT_FOUND             = (ATT_ERR | ATT_ERR_ATTR_NOT_FOUND),
    APP_RESULT_ATTR_NOT_LONG              = (ATT_ERR | ATT_ERR_ATTR_NOT_LONG),
    APP_RESULT_INSUFFICIENT_RESOURCES     = (ATT_ERR | ATT_ERR_INSUFFICIENT_RESOURCES),
    APP_RESULT_VALUE_NOT_ALLOWED          = (ATT_ERR | ATT_ERR_VALUE_NOT_ALLOWED),
    APP_RESULT_APP_ERR                    = (ATT_ERR | ATT_ERR_MIN_APPLIC_CODE),
    APP_RESULT_CCCD_IMPROPERLY_CONFIGURED = (ATT_ERR | ATT_ERR_CCCD_IMPROPERLY_CONFIGURED),
    APP_RESULT_PROC_ALREADY_IN_PROGRESS   = (ATT_ERR | ATT_ERR_PROC_ALREADY_IN_PROGRESS),
} T_APP_RESULT;

/** @brief GAP Parameter List */
typedef enum
{
    GAP_PARAM_BD_ADDR                        = 0x200,//!< Locol Device's Address. Read Only. Size is uint8[GAP_BD_ADDR_LEN]. This item is read from the controller.
    GAP_PARAM_BOND_PAIRING_MODE              = 0x202,//!< Pairing Mode of LE and BR/EDR. Write Only. Size is uint8_t. Default is GAP_PAIRING_MODE_PAIRABLE (@ref BOND_PAIRING_MODE_DEFINES).
    GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS = 0x203,//!< Man-In-The-Middle (MITM) basically turns on Passkey protection in the pairing algorithm. Read/Write. Size is uint8_t. Default is GAP_AUTHEN_NO_MITM_YES_BOND (@ref BOND_MITM_DEFINES).
    GAP_PARAM_BOND_IO_CAPABILITIES           = 0x204,//!< I/O capabilities.  Read/Write. Size is uint8_t. Default is GAP_IO_CAP_NO_INPUT_NO_OUTPUT (@ref T_GAP_IO_CAP).
#if F_BT_OOB_SUPPORT
    GAP_PARAM_BOND_OOB_ENABLED               = 0x205,//!< OOB data available for pairing algorithm. Read/Write. Size is uint8_t. Default is 0(disabled).
#endif
#if F_BT_SC_LINK_CONVERT_SUPPORT
    GAP_PARAM_BOND_LINK_KEY_CONVERT          = 0x206,
#endif
#if F_BT_BREDR_SUPPORT
    GAP_PARAM_BOND_BR_PAIRING_MODE              = 0x208,//!< BR/EDR Pairing Mode. Read/Write. Size is uint8_t. Default is GAP_PAIRING_MODE_PAIRABLE (@ref BOND_PAIRING_MODE_DEFINES)
    GAP_PARAM_BOND_BR_AUTHEN_REQUIREMENTS_FLAGS = 0x209,//!< BR/EDR Man-In-The-Middle (MITM) basically turns on Passkey protection in the pairing algorithm. Write. Size is uint8_t. Default is GAP_AUTHEN_NO_MITM_YES_BOND (@ref BOND_MITM_DEFINES).
    GAP_PARAM_BOND_BR_IO_CAPABILITIES           = 0x20a,//!< BR/EDR I/O capabilities. Write. Size is uint8_t. Default is GAP_IO_CAP_NO_INPUT_NO_OUTPUT (@ref T_GAP_IO_CAP).
#endif
    GAP_PARAM_BOND_LE_PAIRING_MODE              = 0x207,//!< LE Pairing Mode. Read/Write. Size is uint8_t. Default is GAP_PAIRING_MODE_PAIRABLE (@ref BOND_PAIRING_MODE_DEFINES).
    GAP_PARAM_BOND_LE_AUTHEN_REQUIREMENTS_FLAGS = 0x20b,//!< LE Man-In-The-Middle (MITM) basically turns on Passkey protection in the pairing algorithm. Write. Size is uint8_t. Default is GAP_AUTHEN_NO_MITM_YES_BOND (@ref BOND_MITM_DEFINES).
    GAP_PARAM_BOND_LE_IO_CAPABILITIES           = 0x20c,//!< LE I/O capabilities. Write. Size is uint8_t. Default is GAP_IO_CAP_NO_INPUT_NO_OUTPUT (@ref T_GAP_IO_CAP).
} T_GAP_PARAM_TYPE;

#if F_BT_LE_5_0_PA_SYNC_SUPPORT
/** @brief GAP PA Sync Advertiser Address Type */
typedef enum
{
    PA_SYNC_ADV_ADDR_PUBLIC              = 0x00, /**< Public Device Address for reception of info; Otherwise, Public Device Address or Public Identity Address. */
    PA_SYNC_ADV_ADDR_RANDOM              = 0x01, /**< Random Device Address for reception of info; Otherwise, Random Device Address or Random (static) Identity Address. */
#if F_BT_LE_PRIVACY_SUPPORT
    PA_SYNC_ADV_ADDR_RPA_PUBLIC_IDENTITY = 0x02, /**< Public Identity Address (corresponds to Resolved Private Address), only for reception of info. */
    PA_SYNC_ADV_ADDR_RPA_RANDOM_IDENTITY = 0x03, /**< Random (static) Identity Address (corresponds to Resolved Private Address), only for reception of info. */
#endif
} T_GAP_PA_SYNC_ADV_ADDR_TYPE;
#endif

#if F_BT_CONNECTION_ACCEPT_TIMEOUT_SUPPORT
typedef struct
{
    uint16_t                cause;
    uint16_t                connection_accept_timeout;
} T_GAP_READ_CONNECTION_ACCEPT_TIMEOUT_RSP;

typedef struct
{
    uint16_t                cause;
} T_GAP_WRITE_CONNECTION_ACCEPT_TIMEOUT_RSP;
#endif

#if F_BT_5_2_READ_LOCAL_SUPPORTED_CODECS_V2_SUPPORT
typedef struct
{
    uint16_t                cause;
} T_GAP_CONFIGURE_DATA_PATH_RSP;

typedef struct
{
    uint16_t                cause;
    uint8_t                 num_supported_standard_codecs;
    uint8_t                 num_supported_vendor_specific_codecs;
    uint8_t                 *p_standard_codecs;
    uint8_t                 *p_vendor_specific_codecs;
} T_GAP_READ_LOCAL_SUPPORTED_CODECS_V2_RSP;

typedef struct
{
    uint16_t                cause;
    uint8_t                 num_codec_capabilities;
    uint8_t                 *p_codec_capability_combination;
    uint8_t                 codec_capability_combination_len;
} T_GAP_READ_LOCAL_SUPPORTED_CODEC_CAPABILITIES_RSP;

typedef struct
{
    uint16_t                cause;
    uint32_t                min_controller_delay;
    uint32_t                max_controller_delay;
} T_GAP_READ_LOCAL_SUPPORTED_CONTROLLER_DELAY_RSP;
#endif

/** @brief Information for L2CAP channel added. This L2CAP channel can transmit ATT PDU. */
typedef struct
{
    T_GAP_CHANN_TYPE chann_type;
    uint16_t         conn_handle;
    uint16_t         cid;
    uint16_t         mtu_size;
} T_GAP_CHANN_ADDED;

/** @brief Information for L2CAP channel removed.*/
typedef struct
{
    uint16_t         conn_handle;
    uint16_t         cid;
} T_GAP_CHANN_DEL;

/** @brief Information for L2CAP channel MTU updated.*/
typedef struct
{
    uint16_t         conn_handle;
    uint16_t         cid;
    uint16_t         mtu_size;
} T_GAP_CHANN_MTU_UPDATE;

/** @brief Reponse for airplane mode */
typedef union
{
    uint16_t cause;
#if F_BT_CONNECTION_ACCEPT_TIMEOUT_SUPPORT
    T_GAP_READ_CONNECTION_ACCEPT_TIMEOUT_RSP           *p_read_connection_accept_timeout_rsp;
    T_GAP_WRITE_CONNECTION_ACCEPT_TIMEOUT_RSP          *p_write_connection_accept_timeout_rsp;
#endif
#if F_BT_5_2_READ_LOCAL_SUPPORTED_CODECS_V2_SUPPORT
    T_GAP_CONFIGURE_DATA_PATH_RSP                      *p_configure_data_path_rsp;
    T_GAP_READ_LOCAL_SUPPORTED_CODECS_V2_RSP           *p_read_local_supported_codecs_v2_rsp;
    T_GAP_READ_LOCAL_SUPPORTED_CODEC_CAPABILITIES_RSP  *p_read_local_supported_codec_capabilities_rsp;
    T_GAP_READ_LOCAL_SUPPORTED_CONTROLLER_DELAY_RSP    *p_read_local_supported_controller_delay_rsp;
#endif
    T_GAP_CHANN_ADDED            *p_gap_chann_added;
    T_GAP_CHANN_DEL              *p_gap_chann_del;
    T_GAP_CHANN_MTU_UPDATE       *p_gap_chann_mtu_update;
} T_GAP_CB_DATA;
/** End of GAP_Common_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * @defgroup GAP_COMMON_EXPORT_Functions GAP Common Exported Functions
 *
 * @{
 */

/**
  * @brief Callback for gap to notify app
  * @param[in] cb_type callback msy type @ref GAP_COMMON_MSG_TYPE.
  * @param[in] p_cb_data point to callback data @ref T_GAP_CB_DATA.
  * @retval void
  */
typedef void(*P_FUN_GAP_APP_CB)(uint8_t cb_type, void *p_cb_data);

/**
 * @brief  Make upper stack ready to use by sending the register request.
 * @param[in] evt_queue     Event queue handle which is created by APP.
 * @param[in] io_queue      IO message queue handle which is created by APP.
 * @param[in] msg_queue_elem_num  Number of GAP message queue which will be created by GAP module.
 * @return result
 * @retval true  Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_main_task(void *p_param)
    {
        uint8_t event;
        os_msg_queue_create(&io_queue_handle, MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
        os_msg_queue_create(&evt_queue_handle, MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

        gap_start_bt_stack(evt_queue_handle, io_queue_handle, MAX_NUMBER_OF_GAP_MESSAGE);

        while (true)
        {
            if (os_msg_recv(evt_queue_handle, &event, 0xFFFFFFFF) == true)
            {
                if (event == EVENT_IO_TO_APP)
                {
                    T_IO_MSG io_msg;
                    if (os_msg_recv(io_queue_handle, &io_msg, 0) == true)
                    {
                        app_handle_io_msg(io_msg);
                    }
                }
                else
                {
                    gap_handle_msg(event);
                }
            }
        }
    }
 * \endcode
 */
bool gap_start_bt_stack(void *evt_queue, void *io_queue, uint16_t msg_queue_elem_num);

/**
 * @brief  Get the version information of the upper stack lib.
 * @param[out] p_ver    Version
 * @param[out] p_time   Build time
 * @param[out] p_lib    Lib name
 * @param[out] p_gcid_l Git commit id
 * @param[out] p_gcid_h Git commit id
 * @return result
 * @retval true  Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        char version[20];
        char build_time[40];
        char lib_name[20];
        uint32_t gcid_l;
        uint32_t gcid_h;
        gap_get_version_info(version, build_time, lib_name, &gcid_l, &gcid_h);
        data_uart_print("version: %s, build time: %s, LIB Name: %s, GCID: 0x%08x%08x\r\n",
                        version, build_time, lib_name, gcid_l, gcid_h);
    }
 * \endcode
 */
bool gap_get_version_info(char *p_ver, char *p_time, char *p_lib, uint32_t *p_gcid_l,
                          uint32_t *p_gcid_h);

/**
 * @brief  Register callback to gap, when messages in @ref GAP_COMMON_MSG_TYPE happens, it will callback to app.
 * @param[in] app_callback Callback function provided by the APP to handle gap common messages sent from the GAP.
 *              @arg NULL -> Not send gap common messages to APP.
 *              @arg Other -> Use application defined callback function.
 * @return void
 *
 * <b>Example usage</b>
 * \code{.c}
   void app_le_gap_init()
   {
       ...
       le_register_app_cb(app_gap_callback);
       gap_register_app_cb(app_gap_common_callback);
   }
   \endcode
 */
void gap_register_app_cb(P_FUN_GAP_APP_CB app_callback);

/**
  * @brief  Set a GAP parameter.
  *
  *         NOTE: You can call this function with a GAP Common Parameter type and it will set the
  *         GAP Parameter.  GAP Peripheral Parameters are defined in @ref T_GAP_PARAM_TYPE.
  *         If the "len" field sets to the size of a "uint16_t" ,the
  *         "p_value" field must point to a data with type of "uint16_t".
  *
  * @param[in]  param GAP parameter ID: @ref T_GAP_PARAM_TYPE.
  * @param[in]  len Length of data to write.
  * @param[in]  p_value Pointer to data to write.  This is dependent on
  *                  the parameter type and WILL be cast to the appropriate
  *                  data type (For example: if data type param is uint16, p_value will be cast to
  *                  pointer of uint16_t).
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_le_gap_init(void)
    {
        ...
        //GAP Bond Manager parameters
        uint8_t  auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;
        uint16_t auth_flags = GAP_AUTHEN_BIT_BONDING_FLAG | GAP_AUTHEN_BIT_MITM_FLAG;
        uint8_t  auth_io_cap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
        uint8_t  auth_oob = false;

        // Setup the GAP Bond Manager
        gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
        gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
        gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(auth_io_cap), &auth_io_cap);
        gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
        ...
    }
  * \endcode
  */
T_GAP_CAUSE gap_set_param(T_GAP_PARAM_TYPE param, uint8_t len, void *p_value);

/**
  * @brief  Get a GAP Common parameter.
  *
  *         NOTE: You can call this function with a GAP Common Parameter type and it will get a
  *         GAP Common Parameter.  GAP Common Parameters are defined in @ref T_GAP_PARAM_TYPE.
  *
  * @param[in]  param GAP parameter type: @ref T_GAP_PARAM_TYPE
  * @param[in,out]  p_value Pointer to location to get the value.  This is dependent on
  *                  the parameter ID and WILL be cast to the appropriate
  *                  data type (For example: if data type param is uint16, p_value will be cast to
  *                  pointer of uint16_t).
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        uint8_t bt_addr[6];
        gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
    }
  * \endcode
  */
T_GAP_CAUSE gap_get_param(T_GAP_PARAM_TYPE param, void *p_value);

/**
  * @brief  Make the device in pairable mode.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  */
T_GAP_CAUSE gap_set_pairable_mode(void);


/**
 * @brief  Handle GAP message received from BT stack or timeout event for GAP timer.
 * @param[in] event Event type which is received from APP event queue.
 * @return None.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_main_task(void *p_param)
    {
        uint8_t event;
        os_msg_queue_create(&io_queue_handle, MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
        os_msg_queue_create(&evt_queue_handle, MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

        gap_start_bt_stack(evt_queue_handle, io_queue_handle, MAX_NUMBER_OF_GAP_MESSAGE);

        while (true)
        {
            if (os_msg_recv(evt_queue_handle, &event, 0xFFFFFFFF) == true)
            {
                if (event == EVENT_IO_TO_APP)
                {
                    T_IO_MSG io_msg;
                    if (os_msg_recv(io_queue_handle, &io_msg, 0) == true)
                    {
                        app_handle_io_msg(io_msg);
                    }
                }
                else
                {
                    gap_handle_msg(event);
                }
            }
        }
    }
 * \endcode
 */
void gap_handle_msg(uint8_t event);

/** @} */ /* End of group GAP_COMMON_EXPORT_Functions */

/** @} */ /* End of group GAP_COMMON */

/*-------------------------------------------------------------------
-------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

#endif /* GAP_H */
