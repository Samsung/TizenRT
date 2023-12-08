/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     profile_server_ext.h
  * @brief    Head file for server structure.
  * @details  This file can use when parameter use_ext of the server_cfg_use_ext_api is true.
  * @author
  * @date     2021-04-07
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef PROFILE_SERVER_EXT_H
#define PROFILE_SERVER_EXT_H

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
#include "bt_types.h"
#include "gatt.h"
#include "gap_le.h"
#include "profile_server_def.h"

/** @addtogroup GATT_SERVER_API GATT Server API
  * @brief GATT Server API
  * @{
  */

/** @defgroup GATT_SERVER_EXT_API GATT Extension Server API
  * @brief The GATT Server APIs can use when parameter use_ext of the server_cfg_use_ext_api is true.
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GATT_SERVER_EXT_Exported_Macros GATT Server Extension Exported Macros
  * @brief
  * @{
  */
/** @defgroup GATT_SERVER_EXT_CB_DATA Server Extension Callback data
  * @brief data for profile to inform application.
  * @{
  */
/** @brief The callback data of PROFILE_EVT_SEND_DATA_COMPLETE */
typedef struct
{
    uint16_t        credits;
    uint16_t        conn_handle;
    uint16_t        cid;
    T_SERVER_ID     service_id;
    uint16_t        attrib_idx;
    uint16_t        cause;
} T_EXT_SEND_DATA_RESULT;

/** @brief Service callback data */
typedef union
{
    T_SERVER_RESULT     service_reg_result;
    T_EXT_SEND_DATA_RESULT  send_data_result;
    T_SERVER_REG_AFTER_INIT_RESULT    server_reg_after_init_result;
    T_SERVER_CLEAR_SERVICE_AFTER_INIT_RESULT  clear_service_after_init_result;
} T_SERVER_EXT_CB_DATA;

typedef struct
{
    T_SERVER_CB_TYPE   eventId;    /**<  @brief EventId defined upper */
    T_SERVER_EXT_CB_DATA   event_data; /**<  @brief Event data */
} T_SERVER_EXT_APP_CB_DATA;
/** @} End of GATT_SERVER_EXT_CB_DATA */

/** @defgroup P_FUN_EXT_WRITE_IND_POST_PROC Write Post Function Point Definition
  * @brief Call back function to execute some post procedure after handle write request from client.
  * @{
  */
typedef void (* P_FUN_EXT_WRITE_IND_POST_PROC)(uint16_t conn_handle, uint16_t cid,
                                               T_SERVER_ID service_id,
                                               uint16_t attrib_index, uint16_t length,
                                               uint8_t *p_value);
/** @} End of P_FUN_EXT_WRITE_IND_POST_PROC */

/** @defgroup P_FUN_SERVER_EXT_SPECIFIC_CB Specific Service Callback Function Point Definition
  * @{ Function pointer used in each specific service module, to send events to specific service module.
  */
typedef T_APP_RESULT(*P_FUN_GATT_EXT_READ_ATTR_CB)(uint16_t conn_handle, uint16_t cid,
                                                   T_SERVER_ID service_id,
                                                   uint16_t attrib_index,
                                                   uint16_t offset, uint16_t *p_length, uint8_t **pp_value);
typedef T_APP_RESULT(*P_FUN_GATT_EXT_WRITE_ATTR_CB)(uint16_t conn_handle, uint16_t cid,
                                                    T_SERVER_ID service_id,
                                                    uint16_t attrib_index, T_WRITE_TYPE write_type,
                                                    uint16_t length, uint8_t *p_value, P_FUN_EXT_WRITE_IND_POST_PROC *p_write_post_proc);
typedef void (*P_FUN_GATT_EXT_CCCD_UPDATE_CB)(uint16_t conn_handle, uint16_t cid,
                                              T_SERVER_ID service_id,
                                              uint16_t attrib_index, uint16_t ccc_bits);
/** End of P_FUN_SERVER_EXT_SPECIFIC_CB
  * @}
  */

/** @defgroup P_FUN_SERVER_EXT_GENERAL_CB General Server Callback Function Point Definition
  * @brief function pointer Type used to generate Call back, to send events to application.
  * @{
  */
typedef T_APP_RESULT(*P_FUN_EXT_SERVER_GENERAL_CB)(T_SERVER_ID service_id, void *p_para);
/** @} End of P_FUN_SERVER_EXT_GENERAL_CB */


/** @brief GATT extension service callbacks */
typedef struct
{
    P_FUN_GATT_EXT_READ_ATTR_CB read_attr_cb;     /**< Read callback function pointer.
                                                   Return value: @ref T_APP_RESULT. */
    P_FUN_GATT_EXT_WRITE_ATTR_CB write_attr_cb;   /**< Write callback function pointer.
                                                   Return value: @ref T_APP_RESULT. */
    P_FUN_GATT_EXT_CCCD_UPDATE_CB cccd_update_cb; /**< Update cccd callback function pointer. */
} T_FUN_GATT_EXT_SERVICE_CBS;

/** End of GATT_SERVER_EXT_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GATT_SERVER_EXT_Exported_Functions GATT Server Extension Exported Functions
  * @brief
  * @{
  */
#if F_BT_GATT_SERVER_EXT_API
/**
 * @brief Register specific service without start handle
 *
 * Add specific service infomation to gatt_svc_table struct, will be registered to GATT later.
 *
 * @param[in,out] p_out_service_id  Service ID of specific service.
 * @param[in] p_database            Database pointer of specific service.
 * @param[in] length                Length of Database of specific service.
 * @param[in] p_srv_ext_cbs         Service callback functions of specific service.
 * @retval true Add service success
 * @retval false Add service failed
 *
 * <b>Example usage</b>
 * \code{.c}
    T_SERVER_ID bas_ext_add_service(void *p_func)
    {
        T_SERVER_ID service_id;
        if (false == server_ext_add_service(&service_id,
                                       (uint8_t *)bas_attr_tbl,
                                       bas_attr_tbl_size,
                                       bas_cbs))
        {
            APP_PRINT_ERROR1("bas_add_service: service_id %d", service_id);
            service_id = 0xff;
        }
        pfn_bas_cb = (P_FUN_EXT_SERVER_GENERAL_CB)p_func;
        return service_id;
    }
 * \endcode
 */
bool server_ext_add_service(T_SERVER_ID *p_out_service_id, uint8_t *p_database, uint16_t length,
                            const T_FUN_GATT_EXT_SERVICE_CBS *p_srv_ext_cbs);

/**
 * @brief Register specific service with start handle
 *
 * Add specific service infomation to gatt_svc_table struct, will be registered to GATT later.
 *
 * @param[in,out] p_out_service_id  Service ID of specific service.
 * @param[in] p_database            Database pointer of specific service.
 * @param[in] length                Length of Database of specific service.
 * @param[in] p_srv_ext_cbs         Service callback functions of specific service.
 * @param[in] start_handle          Start handle of this service.
 * @retval true Add service success
 * @retval false Add service failed
 *
 * <b>Example usage</b>
 * \code{.c}
    T_SERVER_ID bas_ext_add_service(void *p_func)
    {
        T_SERVER_ID service_id;
        if (false == server_ext_add_service_by_start_handle(&service_id,
                                       (uint8_t *)bas_attr_tbl,
                                       bas_attr_tbl_size,
                                       bas_cbs, 0x00f0))
        {
            APP_PRINT_ERROR1("bas_add_service: service_id %d", service_id);
            service_id = 0xff;
        }
        pfn_bas_cb = (P_FUN_EXT_SERVER_GENERAL_CB)p_func;
        return service_id;
    }
 * \endcode
 */
bool server_ext_add_service_by_start_handle(uint8_t *p_out_service_id, uint8_t *p_database,
                                            uint16_t length,
                                            T_FUN_GATT_EXT_SERVICE_CBS *p_srv_ext_cbs, uint16_t start_handle);
/**
 * @brief Register callback function to send events to application.
 *
 * @param[in] p_fun_cb          Callback function.
 * @retval None
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_profile_init(void)
    {
        server_init(1);
        simp_srv_id = simp_ble_service_ext_add_service(app_ext_profile_callback);
        server_ext_register_app_cb(app_ext_profile_callback);
    }
 * \endcode
 */
void server_ext_register_app_cb(P_FUN_EXT_SERVER_GENERAL_CB p_fun_cb);

/**
  * @brief  Confirm from application when receive read Request from client.
  * @param[in]  conn_handle   Connection handle of the ACL link.
  * @param[in]  cid           Local CID. The value shall be get from \ref P_FUN_GATT_EXT_READ_ATTR_CB callback.
  * @param[in]  service_id    Service ID.
  * @param[in]  attrib_index  Attribute index of attribute to read confirm from application.
  * @param[in]  p_data        Point to the readed value.
  * @param[in]  length        The length of the data.
  * @param[in]  cause         Cause for read confirm. @ref T_APP_RESULT
  * @retval true: confirm from app OK.
  * @retval false: confirm from app failed.
  */
bool server_ext_attr_read_confirm(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id,
                                  uint16_t attrib_index, uint8_t *p_data, uint16_t data_len,
                                  uint16_t total_len, T_APP_RESULT cause);
/**
  * @brief  Confirm from application when receive Execute Write Request from client.
  * @param[in]  conn_handle Connection handle of the ACL link.
  * @param[in]  cid         Local CID. The value shall be get from \ref P_FUN_GATT_EXT_WRITE_ATTR_CB callback.
  * @param[in]  cause       Cause for execute write confirm. @ref T_APP_RESULT
  * @param[in]  handle      Gatt attribute handle.
  * @retval true: confirm from app OK.
  * @retval false: confirm from app failed.
  */
bool server_ext_exec_write_confirm(uint16_t conn_handle, uint16_t cid, uint16_t cause,
                                   uint16_t handle);

/**
  * @brief  Confirm from application when receive Write Request from client.
  * @param[in]  conn_handle  Connection handle of the ACL link.
  * @param[in]  cid          Local CID. The value shall be get from \ref P_FUN_GATT_EXT_WRITE_ATTR_CB callback.
  * @param[in]  service_id   Service ID.
  * @param[in]  attrib_index Attribute index of attribute to write confirm from application.
  * @param[in]  cause        Write request app handle result, APP_RESULT_SUCCESS or other. @ref T_APP_RESULT
  * @retval true: confirm from app OK.
  * @retval false: confirm from app failed.
  */
bool server_ext_attr_write_confirm(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id,
                                   uint16_t attrib_index, T_APP_RESULT cause);

/**
 * @brief Send characteristic value to peer device.
 *
 * @param[in] conn_handle     Connection handle of the ACL link.
 * @param[in] cid             Local CID assigned by Bluetooth stack. Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
 * @param[in] service_id      Service ID.
 * @param[in] attrib_index    Attribute index of characteristic.
 * @param[in] p_data          Point to data to be sent.
 * @param[in] data_len        Length of value to be sent, range: 0~(mtu_size - 3).
                              uint16_t mtu_size is acquired by le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id).
 * @param[in] type            GATT pdu type.
 * @return Data sent result
 * @retval true Success.
 * @retval false Failed.
 *
 * <b>Example usage</b>
 * \code{.c}
  bool simp_ext_service_send_v3_notify(uint16_t conn_handle,  uint16_t cid, T_SERVER_ID service_id,
                                      void *p_value,
                                      uint16_t length)
  {
      APP_PRINT_INFO0("simp_ext_service_send_v3_notify");
      // send notification to client
      return server_ext_send_data(conn_handle, cid, service_id, SIMPLE_EXT_SERVICE_CHAR_V3_NOTIFY_INDEX,
                                  p_value,
                                  length,
                                  GATT_PDU_TYPE_ANY);
  }
 * \endcode
 */
bool server_ext_send_data(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id,
                          uint16_t attrib_index,
                          uint8_t *p_data, uint16_t data_len, T_GATT_PDU_TYPE type);

#if F_BT_ATT_MULTIPLE_VALUE_NTF
/**
 * @brief  Send the multiple variable notification.
 * @param[in] conn_handle   Connection handle of the ACL link.
 * @param[in] cid           Local CID assigned by Bluetooth stack. Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
 * @param[in] p_data        Point to data to be sent.
 * @param[in] data_len      Length of value to be sent, range: 0~(mtu_size - 1).
                            uint16_t mtu_size is acquired by le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id).
 * @return Data sent result
 * @retval true Success.
 * @retval false Failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    bool simp_ble_service_ext_send_v3_v8_notify(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id, void *p_v3_value,
                                            uint16_t v3_length, void *p_v8_value, uint16_t v8_length)
    {
        bool ret;
        uint8_t *p_value;
        uint8_t *p_data;
        uint16_t length = v3_length + v8_length + 2 * 4;
        uint16_t handle;

        p_value = os_mem_zalloc(RAM_TYPE_DATA_ON, length);
        if (p_value == NULL)
        {
            return false;
        }
        p_data = p_value;
        handle = server_get_start_handle(service_id) + SIMPLE_BLE_SERVICE_CHAR_V3_NOTIFY_INDEX;
        LE_UINT16_TO_STREAM(p_data, handle);
        LE_UINT16_TO_STREAM(p_data, v3_length);
        memcpy(p_data, p_v3_value, v3_length);
        p_data += v3_length;

        handle = server_get_start_handle(service_id) + SIMPLE_BLE_SERVICE_CHAR_V8_NOTIFY_INDICATE_INDEX;
        LE_UINT16_TO_STREAM(p_data, handle);
        LE_UINT16_TO_STREAM(p_data, v8_length);
        memcpy(p_data, p_v8_value, v8_length);
        p_data += v8_length;

        ret = server_ext_send_multi_notify(conn_handle, cid, p_value, length);
        if (p_value)
        {
            os_mem_free(p_value);
        }
        return ret;
    }
 * \endcode
 */
bool server_ext_send_multi_notify(uint16_t conn_handle, uint16_t cid,
                                  uint8_t *p_data, uint16_t data_len);
#endif

/**
 * @brief  Get the CCCD information.
 * @param[in]  conn_handle  Connection handle of the ACL link.
 * @param[in]  service_id   Service ID.
 * @param[in]  attrib_index Attribute index of attribute to get CCCD.
 * @param[in,out] p_cccd    The CCCD information.
 * @retval true Get success.
 * @retval false Get failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    bool simp_ble_service_ext_send_v3_notify(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id, void *p_value,
                                        uint16_t length)
    {
        uint16_t cccd_bits;
        APP_PRINT_INFO0("simp_ble_service_ext_send_v3_notify");
        if (server_ext_get_cccd_info(conn_handle, service_id, SIMPLE_BLE_SERVICE_CHAR_NOTIFY_CCCD_INDEX,
                                &cccd_bits))
        {
            APP_PRINT_INFO1("simp_ble_service_ext_send_v3_notify: cccd_bits 0x%x", cccd_bits);
        }
        return server_ext_send_data(conn_handle, cid, service_id, SIMPLE_BLE_SERVICE_CHAR_V3_NOTIFY_INDEX, p_value,
                                length,
                                GATT_PDU_TYPE_ANY);
    }
 * \endcode
*/
bool server_ext_get_cccd_info(uint16_t conn_handle, T_SERVER_ID service_id, uint16_t attrib_index,
                              uint16_t *p_cccd);

/** @} End of GATT_SERVER_EXT_Exported_Functions */

/** @} End of GATT_SERVER_EXT_API */

/** @} End of GATT_SERVER_API */
#endif
#endif

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif

#endif /* PROFILE_SERVER_EXT_H */
