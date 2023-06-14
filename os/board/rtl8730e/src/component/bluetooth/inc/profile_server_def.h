/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     profile_server_def.h
  * @brief    Head file for server structure.
  * @details  Common data struct definition.
  * @author
  * @date     2017-02-18
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef PROFILE_SERVER_DEF_H
#define PROFILE_SERVER_DEF_H

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "upperstack_config.h"
#if F_BT_LE_GATT_SERVER_SUPPORT

/** @defgroup GATT_SERVER_API GATT Server API
  * @brief GATT Server API
  * @{
  */
/** @defgroup GATT_SERVER_COMMON_API GATT Common Server API
  * @brief GATT Server Common API
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GATT_SERVER_COMMON_Exported_Macros GATT Common Server Exported Macros
  * @brief
  * @{
  */

/** @defgroup General_Service_ID General Service ID
  * @brief Service ID for general profile events.
  * @{
  */
#define SERVICE_PROFILE_GENERAL_ID  0xff
/** @} */

/** End of GATT_SERVER_COMMON_Exported_Macros
  * @}
  */


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GATT_SERVER_COMMON_Exported_Types GATT Common Server Exported Types
  * @brief
  * @{
  */

typedef uint8_t T_SERVER_ID;    //!< Service ID

/** @brief  GATT write data type*/
typedef enum
{
	WRITE_REQUEST,                      /**< Write request. */
	WRITE_WITHOUT_RESPONSE,             /**< Write without response. */
	WRITE_SIGNED_WITHOUT_RESPONSE,      /**< Signed write without response. */
	WRITE_LONG,                         /**< Write long request. */
} T_WRITE_TYPE;

/** @brief  GATT PDU type*/
typedef enum
{
	GATT_PDU_TYPE_ANY           = 0x00, /**<  Any PDU type. */
	GATT_PDU_TYPE_NOTIFICATION  = 0x01, /**<  Notification PDU type. */
	GATT_PDU_TYPE_INDICATION    = 0x02  /**<  Indication PDU type. */
} T_GATT_PDU_TYPE;

/** @brief Event type to inform app*/
typedef enum
{
	SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION = 1,    /**< CCCD update event */
	SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE = 2,              /**< client read event */
	SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE = 3,             /**< client write event */
} T_SERVICE_CALLBACK_TYPE;

/** @defgroup GATT_SERVER_COMMON_CB_DATA Server Common Callback data
  * @brief data for profile to inform application.
  * @{
  */
/** @brief Event ID */
typedef enum
{
	PROFILE_EVT_SRV_REG_COMPLETE,              /**< Services register complete event
    when application calls server_add_service before calling gap_start_bt_stack. */
	PROFILE_EVT_SEND_DATA_COMPLETE,            /**< Notification or indication data send complete event. */
	PROFILE_EVT_SRV_REG_AFTER_INIT_COMPLETE,   /**< Services register complete event
    when application calls server_add_service after receiving @ref GAP_INIT_STATE_STACK_READY. */
	PROFILE_EVT_SRV_CLEAR_AFTER_INIT_COMPLETE, /**< Services clear complete event
    when application calls server_clear_service after receiving @ref GAP_INIT_STATE_STACK_READY. */
} T_SERVER_CB_TYPE;

/** @brief  The callback data of PROFILE_EVT_SRV_REG_COMPLETE */
typedef enum
{
	GATT_SERVER_SUCCESS,
	GATT_SERVER_FAIL
} T_SERVER_RESULT;

/** @brief  The callback data of PROFILE_EVT_SRV_REG_AFTER_INIT_COMPLETE */
typedef struct
{
	T_SERVER_RESULT result;
	T_SERVER_ID     service_id;
	uint16_t        cause;
} T_SERVER_REG_AFTER_INIT_RESULT;

/** @brief  The callback data of PROFILE_EVT_SRV_CLEAR_AFTER_INIT_COMPLETE */
typedef struct
{
	uint16_t        cause;
	uint16_t        svc_changed_char_cccd_handle; /**< 0x0000: Invalid handle. */
} T_SERVER_CLEAR_SERVICE_AFTER_INIT_RESULT;

/** @} End of GATT_SERVER_COMMON_CB_DATA */

/** End of GATT_SERVER_COMMON_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GATT_SERVER_COMMON_Exported_Functions GATT Common Server Exported Functions
  * @brief
  * @{
  */

/**
 * @brief Initialize parameters of GATT Server.
 *
 * @param[in] service_num Set the number of services that needs to register.
 * @retval None
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_profile_init(void)
    {
        server_init(1);
    }
 * \endcode
 */
void server_init(uint8_t service_num);

/**
 * @brief Register builtin services including GAP and GATT service.
 *
 * If application does not need to register GAP and GATT service.
 * Application shall call server_builtin_service_reg(false) before server_init().
 *
 * @param[in] reg Whether to register builtin services. Default value is true.
 * @retval None
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_profile_init(void)
    {
        server_builtin_service_reg(false);
        server_init(1);
        simp_srv_id = simp_ble_service_add_service(app_profile_callback);
        server_register_app_cb(app_profile_callback);
    }
 * \endcode
 */
void server_builtin_service_reg(bool reg);

/**
 * @brief Get the start handle of the service
 *
 * @param[in] service_id Service ID.
 * @return Start handle
 * @retval 0 Failed.
 * @retval other Success.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint16_t start_handle;
        start_handle = server_get_start_handle(simp_srv_id);
    }
 * \endcode
 */
uint16_t server_get_start_handle(T_SERVER_ID service_id);

/**
 * @brief Configure the server interface
 *
 * @param[in] use_ext Wether use the extension API. Default value is false.
 *                    @arg true Use the API in profile_server_ext.h.
 *                    @arg false Use the API in profile_server.h.
 * @return Configure result
 * @retval true Success.
 * @retval false Failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        server_cfg_use_ext_api(true);
    }
 * \endcode
 */
bool server_cfg_use_ext_api(bool use_ext);

/**
 * @brief Clear service.
 *
 *         NOTE: This function can only be used when:
 *                  standby state (i.e., no link, not initiating, etc.), and @ref GAP_INIT_STATE_STACK_READY, and not add service
 *
 *               If sending request operation is success, the result of clear service will be returned by callback depends on APP:
 *                   Default or APP use server_cfg_use_ext_api(false):
 *                         callback registered by @ref server_register_app_cb with eventId @ref PROFILE_EVT_SRV_CLEAR_AFTER_INIT_COMPLETE
 *                   APP use server_cfg_use_ext_api(true):
 *                         callback registered by @ref server_ext_register_app_cb with eventId @ref PROFILE_EVT_SRV_CLEAR_AFTER_INIT_COMPLETE
 *
 *               If clear service operation is success and APP not clear bond info:
 *                   APP clear CCCD except CCCD of Service Changed characteristic
 *                   APP send Indication of Service Changed after reconnection
 *
 * @return Send request operation.
 * @retval true  Send request operation success.
 * @retval false Send request operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        server_clear_service();
    }

    // Default or APP use server_cfg_use_ext_api(false): callback registered by @ref server_register_app_cb
    T_APP_RESULT app_profile_callback(T_SERVER_ID service_id, void *p_data)
    {
        ......
        if (service_id == SERVICE_PROFILE_GENERAL_ID)
        {
            T_SERVER_APP_CB_DATA *p_param = (T_SERVER_APP_CB_DATA *)p_data;
            switch (p_param->eventId)
            {
            case PROFILE_EVT_SRV_CLEAR_AFTER_INIT_COMPLETE:
                APP_PRINT_INFO2("app_profile_callback: PROFILE_EVT_SRV_CLEAR_AFTER_INIT_COMPLETE, cause 0x%x, svc_changed_char_cccd_handle 0x%x",
                                p_param->event_data.clear_service_after_init_result.cause,
                                p_param->event_data.clear_service_after_init_result.svc_changed_char_cccd_handle);
                ......
                break;
        ......
    }

    // APP use server_cfg_use_ext_api(true): callback registered by @ref server_ext_register_app_cb
    T_APP_RESULT app_profile_ext_callback(T_SERVER_ID service_id, void *p_data)
    {
        ......
        if (service_id == SERVICE_PROFILE_GENERAL_ID)
        {
            T_SERVER_EXT_APP_CB_DATA *p_param = (T_SERVER_EXT_APP_CB_DATA *)p_data;
            switch (p_param->eventId)
            {
            case PROFILE_EVT_SRV_CLEAR_AFTER_INIT_COMPLETE:
                APP_PRINT_INFO2("app_profile_ext_callback: PROFILE_EVT_SRV_CLEAR_AFTER_INIT_COMPLETE, cause 0x%x, svc_changed_char_cccd_handle 0x%x",
                                p_param->event_data.clear_service_after_init_result.cause,
                                p_param->event_data.clear_service_after_init_result.svc_changed_char_cccd_handle);
                ......
                break;
            ......
    }
 * \endcode
 */
bool server_clear_service(void);

/** @} End of GATT_SERVER_COMMON_Exported_Functions */

/** @} End of GATT_SERVER_COMMON_API */

/** @} End of GATT_SERVER_API */
#endif

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif

#endif /* PROFILE_SERVER_DEF_H */
