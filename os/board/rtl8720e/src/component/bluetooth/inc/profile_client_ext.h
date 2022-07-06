/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     profile_client_ext.h
  * @brief    Head file for profile client structure.
  * @details  This file can use when parameter use_ext of the client_cfg_use_ext_api is true.
  * @author   ethan_su
  * @date     2016-02-18
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef PROFILE_CLIENT_EXT_H
#define PROFILE_CLIENT_EXT_H

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "upperstack_config.h"
#if F_BT_LE_GATT_CLIENT_SUPPORT
#include "gatt.h"
#include "bt_types.h"
#include "gap_le.h"
#include "profile_client_def.h"

/** @addtogroup GATT_CLIENT_API GATT Client API
  * @brief GATT client API
  * @{
  */
/** @defgroup GATT_CLIENT_EXT_API GATT Client API
  * @brief The GATT Client APIs can use when parameter use_ext of the client_cfg_use_ext_api is true.
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GATT_Client_Ext_Exported_Macros GATT Client Extension Exported Macros
  * @{
  */


/** @defgroup P_FUN_EXT_GENERAL_APP_CB General Extension Client Callback Function Point Definition
  * @{ function pointer used in each specific profile, to send events to application
  */
typedef T_APP_RESULT(*P_FUN_EXT_GENERAL_APP_CB)(T_CLIENT_ID client_id, uint16_t conn_handle,
		uint16_t cid, void *p_data);
/** End of P_FUN_EXT_GENERAL_APP_CB
  * @}
  */


/** @defgroup T_FUN_EXT_CLIENT_CBS_DEF Specific Client Extension Callback Function Point Definition
  * @{ Function pointer used in each specific client module, to send events to specific client module.
  */
typedef void (*P_FUN_EXT_DISCOVER_STATE_CB)(uint16_t conn_handle, uint16_t cid,
		T_DISCOVERY_STATE discovery_state);
typedef void (*P_FUN_EXT_DISCOVER_RESULT_CB)(uint16_t conn_handle, uint16_t cid,
		T_DISCOVERY_RESULT_TYPE result_type,
		T_DISCOVERY_RESULT_DATA result_data);
typedef void (*P_FUN_EXT_READ_RESULT_CB)(uint16_t conn_handle, uint16_t cid, uint16_t cause,
		uint16_t handle,
		uint16_t value_size, uint8_t *p_value);
typedef void (*P_FUN_EXT_WRITE_RESULT_CB)(uint16_t conn_handle, uint16_t cid,
		T_GATT_WRITE_TYPE type, uint16_t handle,
		uint16_t cause, uint8_t credits);
typedef T_APP_RESULT(*P_FUN_EXT_NOTIFY_IND_RESULT_CB)(uint16_t conn_handle, uint16_t cid,
		bool notify, uint16_t handle,
		uint16_t value_size, uint8_t *p_value);
typedef void (*P_FUN_EXT_DISCONNECT_CB)(uint16_t conn_handle, uint16_t cid);
/** End of T_FUN_EXT_CLIENT_CBS_DEF
  * @}
  */

/** @defgroup T_FUN_EXT_CLIENT_CBS Specific Client Extension Callback Functions Struct
  * @{
  */
typedef struct {
	P_FUN_EXT_DISCOVER_STATE_CB    discover_state_cb;   //!< Discovery state callback function pointer
	P_FUN_EXT_DISCOVER_RESULT_CB   discover_result_cb;  //!< Discovery result callback function pointer
	P_FUN_EXT_READ_RESULT_CB       read_result_cb;      //!< Read response callback function pointer
	P_FUN_EXT_WRITE_RESULT_CB      write_result_cb;     //!< Write result callback function pointer
	P_FUN_EXT_NOTIFY_IND_RESULT_CB notify_ind_result_cb;//!< Notify Indication callback function pointer
} T_FUN_EXT_CLIENT_CBS;
/** End of T_FUN_EXT_CLIENT_CBS * @} */

/** End of GATT_Client_Ext_Exported_Types
* @}
*/
/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GATT_Client_Ext_Exported_Functions GATT Client Extension Exported Functions
  * @{
  */
#if F_BT_GATT_CLIENT_EXT_API
/**
  * @brief  Used by application, register general client callback.
  * @param[in]  p_fun_cb  Fuction offered by application.
  * @retval None
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_profile_init(void)
    {
        client_init(1);
        simple_ble_client_id = simp_ble_ext_add_client(app_client_ext_callback);
        client_ext_register_general_client_cb(app_client_ext_callback);
    }
 * \endcode
 */
void client_ext_register_general_client_cb(P_FUN_EXT_GENERAL_APP_CB p_fun_cb);

/**
 * @brief  Used by specific client, register callback.
 * @param[in,out]  p_out_client_id  Client ID generated for registered specific client module.
 * @param[in]      client_cbs       Callback functions implemented in specific client module.
 * @retval true   Register successful.
 * @retval false  Register failed.
 *
 * <b>Example usage</b>
 * \code{.c}

    T_CLIENT_ID simp_ble_ext_add_client(P_FUN_EXT_GENERAL_APP_CB app_cb, uint8_t link_num)
    {
        uint16_t size;
        if (link_num > SIMP_MAX_LINKS)
        {
            APP_PRINT_ERROR1("simp_ble_add_client: invalid link_num %d", link_num);
            return 0xff;
        }
        if (false == client_ext_register_spec_client_cb(&simp_client, &simp_ble_client_cbs))
        {
            simp_client = CLIENT_PROFILE_GENERAL_ID;
            APP_PRINT_ERROR0("simp_ble_add_client failed");
            return simp_client;
        }
        APP_PRINT_INFO1("simp_ble_add_client: simp_client %d", simp_client);

        simp_client_cb = app_cb;

        return simp_client;
    }
 * \endcode
 */
bool client_ext_register_spec_client_cb(T_CLIENT_ID *p_out_client_id,
										const T_FUN_EXT_CLIENT_CBS *client_cbs);

/**
  * @brief  Send discovery all primary services request.
  * @param[in] conn_handle     Connection handle of the ACL link.
  * @param[in] cid             Local CID assigned by Bluetooth stack.
  *                            @arg 0     Profile client layer will automatically search for available channels.
  *                            @arg other Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
  * @param[in]  client_id      Client ID of specific client module.
  * @retval GAP_CAUSE_SUCCESS  Discovery request success.
  * @retval other              Discovery request failed.
  */
T_GAP_CAUSE client_ext_all_primary_srv_discovery(uint16_t conn_handle, uint16_t cid,
		T_CLIENT_ID client_id);

/**
  * @brief  Send discovery services by 16 bit UUID request.
  * @param[in] conn_handle     Connection handle of the ACL link.
  * @param[in] cid             Local CID assigned by Bluetooth stack.
  *                            @arg 0     Profile client layer will automatically search for available channels.
  *                            @arg other Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
  * @param[in]  client_id      Client ID of specific client module.
  * @param[in]  uuid16         16 bit UUID.
  * @retval GAP_CAUSE_SUCCESS  Discovery request success.
  * @retval other              Discovery request failed.
  */
T_GAP_CAUSE client_ext_by_uuid_srv_discovery(uint16_t conn_handle, uint16_t cid,
		T_CLIENT_ID client_id, uint16_t uuid16);

/**
  * @brief  Send discovery services by 128 bit UUID request.
  * @param[in] conn_handle     Connection handle of the ACL link.
  * @param[in] cid             Local CID assigned by Bluetooth stack.
  *                            @arg 0     Profile client layer will automatically search for available channels.
  *                            @arg other Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
  * @param[in]  client_id      Client ID of specific client module.
  * @param[in]  p_uuid128      128 bit UUID.
  * @retval GAP_CAUSE_SUCCESS  Discovery request success.
  * @retval other              Discovery request failed.
  */
T_GAP_CAUSE client_ext_by_uuid128_srv_discovery(uint16_t conn_handle, uint16_t cid,
		T_CLIENT_ID client_id,
		uint8_t *p_uuid128);

/**
  * @brief  Send discovery relationship services request.
  * @param[in]  conn_handle    Connection handle of the ACL link.
  * @param[in]  cid            Local CID assigned by Bluetooth stack.
  *                            @arg 0     Profile client layer will automatically search for available channels.
  *                            @arg other Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
  * @param[in]  client_id      Client ID of specific client module.
  * @param[in]  start_handle   Start handle of range to be searched.
  * @param[in]  end_handle     End handle of range to be searched.
  * @retval GAP_CAUSE_SUCCESS  Discovery request success.
  * @retval other              Discovery request failed.
  */
T_GAP_CAUSE client_ext_relationship_discovery(uint16_t conn_handle, uint16_t cid,
		T_CLIENT_ID client_id,
		uint16_t start_handle, uint16_t end_handle);

/**
  * @brief  Send discovery characteristics request.
  * @param[in] conn_handle     Connection handle of the ACL link.
  * @param[in] cid             Local CID assigned by Bluetooth stack.
  *                            @arg 0     Profile client layer will automatically search for available channels.
  *                            @arg other Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
  * @param[in]  client_id      Client ID of specific client module.
  * @param[in]  start_handle   Start handle of range to be searched.
  * @param[in]  end_handle     End handle of range to be searched.
  * @retval GAP_CAUSE_SUCCESS  Discovery request success.
  * @retval other              Discovery request failed.
  */
T_GAP_CAUSE client_ext_all_char_discovery(uint16_t conn_handle, uint16_t cid, T_CLIENT_ID client_id,
		uint16_t start_handle,
		uint16_t end_handle);

/**
  * @brief  Send discovery characteristics request by caracteristic uuid.
  * @param[in] conn_handle     Connection handle of the ACL link.
  * @param[in] cid             Local CID assigned by Bluetooth stack.
  *                            @arg 0     Profile client layer will automatically search for available channels.
  *                            @arg other Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
  * @param[in]  client_id      Client ID of specific client module.
  * @param[in]  start_handle   Start handle of range to be searched.
  * @param[in]  end_handle     End handle of range to be searched.
  * @param[in]  uuid16         16bit characteristic uuid to be searched.
  * @retval GAP_CAUSE_SUCCESS  Discovery request success.
  * @retval other              Discovery request failed.
  */
T_GAP_CAUSE client_ext_by_uuid_char_discovery(uint16_t conn_handle, uint16_t cid,
		T_CLIENT_ID client_id,
		uint16_t start_handle,
		uint16_t end_handle, uint16_t uuid16);
/**
  * @brief  Send discovery characteristics request by caracteristic uuid.
  * @param[in] conn_handle     Connection handle of the ACL link.
  * @param[in] cid             Local CID assigned by Bluetooth stack.
  *                            @arg 0     Profile client layer will automatically search for available channels.
  *                            @arg other Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
  * @param[in]  client_id      Client ID of specific client module.
  * @param[in]  start_handle   Start handle of range to be searched.
  * @param[in]  end_handle     End handle of range to be searched.
  * @param[in]  p_uuid128      128bit characteristic uuid to be searched.
  * @retval GAP_CAUSE_SUCCESS  Discovery request success.
  * @retval other              Discovery request failed.
  */
T_GAP_CAUSE client_ext_by_uuid128_char_discovery(uint16_t conn_handle, uint16_t cid,
		T_CLIENT_ID client_id,
		uint16_t start_handle,
		uint16_t end_handle, uint8_t *p_uuid128);
/**
  * @brief  Send discovery characteristics descriptor request.
  * @param[in] conn_handle     Connection handle of the ACL link.
  * @param[in] cid             Local CID assigned by Bluetooth stack.
  *                            @arg 0     Profile client layer will automatically search for available channels.
  *                            @arg other Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
  * @param[in]  client_id      Client ID of specific client module.
  * @param[in]  start_handle   Start handle of range to be searched.
  * @param[in]  end_handle     End handle of range to be searched.
  * @retval GAP_CAUSE_SUCCESS  Discovery request success.
  * @retval other              Discovery request failed.
  */
T_GAP_CAUSE client_ext_all_char_descriptor_discovery(uint16_t conn_handle, uint16_t cid,
		T_CLIENT_ID client_id,
		uint16_t start_handle, uint16_t end_handle);

/**
  * @brief  Read characteristic by handle request.
  * @param[in] conn_handle     Connection handle of the ACL link.
  * @param[in] cid             Local CID assigned by Bluetooth stack.
  *                            @arg 0     Profile client layer will automatically search for available channels.
  *                            @arg other Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
  * @param[in]  client_id      Client ID of specific client module.
  * @param[in]  handle         Request handle.
  * @retval GAP_CAUSE_SUCCESS  Read request success.
  * @retval other              Read request failed.
  */
T_GAP_CAUSE client_ext_attr_read(uint16_t conn_handle, uint16_t cid, T_CLIENT_ID client_id,
								 uint16_t handle);

/**
  * @brief  Read characteristic by 16 bit UUID request.
  * @param[in] conn_handle    Connection handle of the ACL link.
  * @param[in] cid            Local CID assigned by Bluetooth stack.
  *                           @arg 0     Profile client layer will automatically search for available channels.
  *                           @arg other Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
  * @param[in]  client_id     Client ID of specific client module.
  * @param[in]  start_handle  Start handle of range to be searched.
  * @param[in]  end_handle    End handle of range to be searched.
  * @param[in]  uuid16        Request 16 bit UUID.
  * @param[in]  p_uuid128     Request 128 bit UUID.
  * @retval GAP_CAUSE_SUCCESS Read request success.
  * @retval other             Read request failed.
  */
T_GAP_CAUSE client_ext_attr_read_using_uuid(uint16_t conn_handle, uint16_t cid,
		T_CLIENT_ID client_id,
		uint16_t start_handle,
		uint16_t end_handle, uint16_t uuid16, uint8_t *p_uuid128);
/**
 * @brief  Write characteristic request.
 * @param[in] conn_handle Connection handle of the ACL link.
 * @param[in] cid         Local CID assigned by Bluetooth stack.
 *                        @arg 0     Profile client layer will automatically search for available channels.
 *                        @arg other Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
 * @param[in]  client_id  Client ID of specific client module.
 * @param[in]  write_type Type of write.
 * @param[in]  handle     Attribute handle.
 * @param[in]  length     Length of data to be written.
                          If write_type is GATT_WRITE_TYPE_REQ, range of length is from 0 to 512.
                          If write_type is GATT_WRITE_TYPE_CMD, range of length is from 0 to (mtu_size - 3).
                          If write_type is GATT_WRITE_TYPE_SIGNED_CMD, range of length is from 0 to (mtu_size - 15).
                          uint16_t mtu_size is acquired by le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id).
 * @param[in]  p_data     Point to the data to be written.
 * @retval GAP_CAUSE_SUCCESS: Write request success.
 * @retval other: Write request failed.
 */
T_GAP_CAUSE client_ext_attr_write(uint16_t conn_handle, uint16_t cid, T_CLIENT_ID client_id,
								  T_GATT_WRITE_TYPE write_type,
								  uint16_t handle, uint16_t length, uint8_t *p_data);

/**
  * @brief  Confirm from application when receive indication from server.
  * @param[in]  conn_handle    Connection handle of the ACL link.
  * @param[in]  cid            Local CID. The value shall be get from \ref P_FUN_EXT_NOTIFY_IND_RESULT_CB callback.
  * @retval true: Confirm OK.
  * @retval false: Confirm failed.
  */
T_GAP_CAUSE client_ext_attr_ind_confirm(uint16_t conn_handle, uint16_t cid);


#if F_BT_ATT_READ_MULTIPLE_VARIABLE
/**
 * @brief  Read multiple variable characteristic values.
 * @param[in]  conn_handle    Connection handle of the ACL link.
 * @param[in]  cid            Local CID assigned by Bluetooth stack.
 *                            @arg 0     Profile client layer will automatically search for available channels.
 *                            @arg other Shall use the GAP_CHANNEL_STATE_CONNECTED state channel.
 * @param[in]  client_id      Client ID of specific client module.
 * @param[in]  num_handle     The number of p_handle.
 * @param[in]  p_handle       Handle array.
 * @retval GAP_CAUSE_SUCCESS  Read request success.
 * @retval other              Read request failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_APP_RESULT app_client_ext_callback(T_CLIENT_ID client_id, uint16_t conn_handle,
                                         uint16_t cid, void *p_data);
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        if (client_id == CLIENT_PROFILE_GENERAL_ID)
        {
            T_CLIENT_APP_CB_DATA *p_client_app_cb_data = (T_CLIENT_APP_CB_DATA *)p_data;
            switch (p_client_app_cb_data->cb_type)
            {
#if F_BT_ATT_READ_MULTIPLE_VARIABLE
            case CLIENT_APP_CB_TYPE_READ_MULTI_RESULT:
                APP_PRINT_INFO3("CLIENT_APP_CB_TYPE_READ_MULTI_RESULT: client_id %d, cause 0x%x, num_handle %d",
                                p_client_app_cb_data->cb_content.read_multi_result_data.client_id,
                                p_client_app_cb_data->cb_content.read_multi_result_data.cause,
                                p_client_app_cb_data->cb_content.read_multi_result_data.num_handle);
                if (p_client_app_cb_data->cb_content.read_multi_result_data.p_read_tbl)
                {
                    T_READ_MULTI_DATA *p_read_tbl = p_client_app_cb_data->cb_content.read_multi_result_data.p_read_tbl;
                    for (uint16_t i = 0; i < p_client_app_cb_data->cb_content.read_multi_result_data.num_handle; i++)
                    {
                        APP_PRINT_INFO5("p_read_tbl[%d]: handle 0x%x, read_len %d, total_len %d, p_data %b",
                                        i, p_read_tbl[i].handle, p_read_tbl[i].read_len, p_read_tbl[i].total_len,
                                        TRACE_BINARY(p_read_tbl[i].read_len, p_read_tbl[i].p_data));
                    }
                }
                break;
#endif
            default:
                break;
            }
        }
    }
    bool simp_ble_client_read_multi(uint16_t conn_handle, uint8_t read_type)
    {
        bool hdl_valid = false;
        uint16_t  handle[2];
        if (simp_table[conn_id].hdl_cache[HDL_SIMBLE_V1_READ])
        {
            handle[0] = simp_table[conn_id].hdl_cache[HDL_SIMBLE_V1_READ];
            hdl_valid = true;
        }
        if (simp_table[conn_id].hdl_cache[HDL_SIMBLE_V3_NOTIFY_CCCD])
        {
            handle[1] = simp_table[conn_id].hdl_cache[HDL_SIMBLE_V3_NOTIFY_CCCD];
            hdl_valid = true;
        }

        if (hdl_valid)
        {
            if (client_ext_attr_read_multi_variable(conn_handle, simp_client, 2, handle) == GAP_CAUSE_SUCCESS)
            {
                return true;
            }
        }

        APP_PRINT_WARN0("simp_ble_client_read_multi: Request fail! Please check!");
        return false;
    }
 * \endcode
 */
T_GAP_CAUSE client_ext_attr_read_multi_variable(uint16_t conn_handle, uint16_t cid,
		T_CLIENT_ID client_id,
		uint16_t num_handle, uint16_t *p_handle);
#endif
#endif
/** End of GATT_Client_Ext_Exported_Functions
* @}
*/
/** End of GATT_CLIENT_EXT_API
* @}
*/
/** End of GATT_CLIENT_API
* @}
*/
#endif

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif

#endif /* PROFILE_CLIENT_EXT_H */
