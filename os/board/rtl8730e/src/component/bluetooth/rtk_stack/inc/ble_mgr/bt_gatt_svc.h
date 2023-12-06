#ifndef _BT_GATT_SVC_H_
#define _BT_GATT_SVC_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "profile_server_ext.h"
#include "gatt.h"

/** @defgroup BT_GATT_SVC BT GATT Service
  * @brief BT GATT Service
  * @{
  */

/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup BT_GATT_SERVICE_Exported_Types BT GATT Service Types
    * @{
    */

/**
 * @brief BT GATT service characteristic UUID
 */
typedef struct
{
    uint16_t index;
    uint8_t  uuid_size;
    union
    {
        uint16_t char_uuid16;
        uint8_t char_uuid128[16];
    } uu;
} T_CHAR_UUID;

/**
 * @brief GATT service mode
 */
typedef enum
{
    GATT_SVC_DISABLE,             /**< Not use BT GATT service APIs*/
    GATT_SVC_USE_NORMAL_SERVER,   /**< GATT service use normal profile service APIs*/
    GATT_SVC_USE_EXT_SERVER       /**< GATT service use extended profile service APIs*/
} T_GATT_SVC_MODE;

/**
 * @brief Define GATT service send data callback function point
 *        Function pointer used to handle the result of send notification or indication.
 */
typedef void(*P_FUN_GATT_EXT_SEND_DATA_CB)(T_EXT_SEND_DATA_RESULT result);

/** End of BT_GATT_SERVICE_Exported_Types
    * @}
    */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup BT_GATT_SERVICE_Exported_Functions BT GATT Service Functions
    * @{
    */

/**
*@brief    Register specific service.
*@note     Add specific service infomation to gatt_svc_table struct, will be registered to GATT later.
*@param[in]  p_out_service_id       Service ID of specific service.
*@param[in]  p_database             Database pointer of specific service.
*@param[in]  length                 Length of Database of specific service. @ref T_FUN_GATT_EXT_SERVICE_CBS.
*@param[in]  p_srv_ext_cbs          Service callback functions of specific service.
                                    @ref P_FUN_GATT_EXT_SEND_DATA_CB.
*@param[in]  p_srv_ext_send_data_cb Callback functions for service to handle the result of send notification
                                    or indication.
* \arg    NULL    Application needs to handle PROFILE_EVT_SEND_DATA_COMPLETE directly.
* \arg    other   Send result through the callback function p_srv_ext_send_data_cb.
*@return   bool
*@retval   true   Add service success
*@retval   false  Add service failed
*
* <b>Example usage</b>
* \code{.c}

//case 1
    bool bass_add_service(uint8_t brs_num)
    {
        T_SERVER_ID service_id;
        uint16_t attr_tbl_size = sizeof(bass_attr_tbl);

        attr_tbl_size -= (BASS_BRS_CHAR_MAX_NUM - brs_num) * 3 * sizeof(T_ATTRIB_APPL);

        if (false == gatt_svc_add(&service_id,
                                     (uint8_t *)bass_attr_tbl,
                                     attr_tbl_size,
                                     &bass_cbs, bass_send_data_cb))
        {
            PROTOCOL_PRINT_ERROR0("bass_add_service: failed");
            return false;
        }
        p_bass->service_id = service_id;

        return true;
    }
    void bass_send_data_cb(T_EXT_SEND_DATA_RESULT result)
    {
        PROTOCOL_PRINT_INFO6("bass_send_data_cb: credits %d, conn_handle 0x%x, cid 0x%x, service_id %d, attr_idx %d, cause 0x%x",
                             result.credits, result.conn_handle, result.cid,
                             result.service_id, result.attrib_idx, result.cause);
    }

//case 2
    bool bass_add_service(uint8_t brs_num)
    {
        T_SERVER_ID service_id;
        uint16_t attr_tbl_size = sizeof(bass_attr_tbl);

        attr_tbl_size -= (BASS_BRS_CHAR_MAX_NUM - brs_num) * 3 * sizeof(T_ATTRIB_APPL);

        if (false == gatt_svc_add(&service_id,
                                     (uint8_t *)bass_attr_tbl,
                                     attr_tbl_size,
                                     &bass_cbs, NULL))
        {
            PROTOCOL_PRINT_ERROR0("bass_add_service: failed");
            return false;
        }
        p_bass->service_id = service_id;

        return true;
    }
* \endcode
*/
bool gatt_svc_add(T_SERVER_ID *p_out_service_id, uint8_t *p_database, uint16_t length,
                  const T_FUN_GATT_EXT_SERVICE_CBS *p_srv_ext_cbs,
                  P_FUN_GATT_EXT_SEND_DATA_CB p_srv_ext_send_data_cb);

/**
*@brief    Get the service table number registered by gatt_svc_add.
 *\xrefitem Added_API_2_12_0_0 "Added Since 2.12.0.0" "Added API"
*@return   Registered service table number
*/
uint8_t gatt_svc_get_num(void);

/**
*@brief    Confirm from application when receive read Request from client.
*@note     When use this api, the cause shall be set to APP_RESULT_PENDING in the attr_read_cb.
*@param[in]  conn_handle   Connection handle of the ACL link.
*@param[in]  cid           Local CID assigned by Bluetooth stack.
*@param[in]  service_id    Service ID.
*@param[in]  attrib_index  Attribute index of attribute to read confirm from application.
*@param[in]  p_data        Point to the readed value.
*@param[in]  data_len      The length of the read data.
*@param[in]  total_len     The total length of the data to read.
*@param[in]  cause         Cause for read confirm. @ref T_APP_RESULT.
*@return   bool
*@retval   true: confirm from app OK.
*@retval   false: confirm from app failed.
*
* <b>Example usage</b>
* \code{.c}

    T_APP_RESULT bass_attr_read_cb(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id,
                                   uint16_t attrib_index,
                                   uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
    {
        T_APP_RESULT cause = APP_RESULT_SUCCESS;
        uint8_t *p_data = NULL;
        uint16_t len;
        uint8_t source_id = bass_get_source_id(attrib_index, false);
        PROTOCOL_PRINT_INFO2("bass_attr_read_cb: attrib_index %d, offset %x", attrib_index, offset);
        if (source_id == BASS_INVALID_SOURCE_ID)
        {
            return APP_RESULT_ATTR_NOT_FOUND;
        }

        if (bass_brs_gen_char_data(source_id,
                                   &p_data,
                                   &len))
        {
            if (offset > len)
            {
                cause = APP_RESULT_INVALID_OFFSET;
            }
            else
            {
                if (gatt_svc_read_confirm(conn_handle, cid, p_bass->service_id,
                                             attrib_index,
                                             p_data + offset,
                                             len - offset, len, APP_RESULT_SUCCESS))
                {
                    cause = APP_RESULT_PENDING;
                }
            }
            if (p_data)
            {
                ble_audio_mem_free(p_data);
            }
        }
        return (cause);
    }
* \endcode
*/
bool gatt_svc_read_confirm(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id,
                           uint16_t attrib_index, uint8_t *p_data, uint16_t data_len,
                           uint16_t total_len, T_APP_RESULT cause);

/**
*@brief    Confirm from application when receive Write Request from client.
*@note     If the process of writing attribute value does not end immediately, the specific service will call
           gatt_svc_write_confirm, and the cause shall be set to APP_RESULT_PENDING in the attr_write_cb.
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
*@param[in]  conn_handle   Connection handle of the ACL link.
*@param[in]  cid           Local CID assigned by Bluetooth stack.
*@param[in]  service_id    Service ID.
*@param[in]  attrib_index  Attribute index of attribute to write confirm from application.
*@param[in]  cause         Write request app handle result, APP_RESULT_SUCCESS or other. @ref T_APP_RESULT.
*@return   bool
*@retval   true: confirm from app OK.
*@retval   false: confirm from app failed.
*
* <b>Example usage</b>
* \code{.c}

    T_USER_CMD_PARSE_RESULT cmd_writeconf(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        uint16_t cid = p_parse_value->dw_param[1];
        T_SERVER_ID service_id = p_parse_value->dw_param[2];
        uint16_t attrib_index = p_parse_value->dw_param[3];

        if (!gatt_svc_write_confirm(conn_id, cid, service_id, attrib_index,
                                       APP_RESULT_SUCCESS))
        {
            return (RESULT_ERR);
        }

        return (RESULT_SUCESS);
    }
* \endcode
*/
bool gatt_svc_write_confirm(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id,
                            uint16_t attrib_index, T_APP_RESULT cause);

/**
*@brief    Send notification or indication to peer device.
*@param[in]  conn_handle   Connection handle of the ACL link.
*@param[in]  cid           Local CID assigned by Bluetooth stack.
*@param[in]  service_id    Service ID.
*@param[in]  attrib_index  Attribute index of characteristic.
*@param[in]  p_data        Point to data to be sent.
*@param[in]  data_len      Length of value to be sent, range: 0~(mtu_size - 3).
                           uint16_t mtu_size is acquired by le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id).
*@param[in]  type          GATT pdu type.
*@return   Data sent result
*@retval   true  Success.
*@retval   false Failed.
*
* <b>Example usage</b>
* \code{.c}

    bool bass_send_brs_notify(T_BLE_AUDIO_LINK *p_link, T_BASS_BRS_DB *p_db)
    {
        uint8_t *p_data = NULL;
        uint16_t len = 0;
        uint16_t attrib_idx;
        bool ret = true;

        attrib_idx = BASS_CHAR_BRS_INDEX + 3 * (p_db->brs_data.source_id - 1);
        if (bass_brs_gen_char_data(p_db->brs_data.source_id, &p_data,  &len))
        {
            if (len > p_link->mtu_size - 3)
            {
                len = p_link->mtu_size - 3;
            }
            if (gatt_svc_send_data(p_link->conn_handle, L2C_FIXED_CID_ATT, p_bass->service_id, attrib_idx,
                                      p_data, len,
                                      GATT_PDU_TYPE_NOTIFICATION) == false)
            {
                ret = false;
            }
        }
        if (p_data)
        {
            ble_audio_mem_free(p_data);
        }
        return ret;
    }
* \endcode
*/
bool gatt_svc_send_data(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id,
                        uint16_t attrib_index,
                        uint8_t *p_data, uint16_t data_len, T_GATT_PDU_TYPE type);

/**
*@brief    Initialize the GATT Service mode and service number.
*@param[in]  mode     GATT Service mode. @ref T_GATT_SVC_MODE
*@param[in]  svc_num  Set the number of services that needs to register.
* \arg    0       Application shall call server_register_app_cb and server_init to register callback
                  and set service number.
* \arg    other   Application not need to call server_register_app_cb and server_init.
*@return   The result of initialize the GATT Service mode.
*@retval   true  Success.
*@retval   false Failed.
*
* <b>Example usage</b>
* \code{.c}

//case 1
void app_ble_service_init(void)
{
    uint8_t server_num = 1;
    server_register_app_cb(app_ble_service_general_srv_cb);
    server_init(server_num);
    gatt_svc_init(GATT_SVC_USE_NORMAL_SERVER, 0);
}

//case 2
void app_ble_service_init(void)
{
    uint8_t server_num = 1;
    gatt_svc_init(GATT_SVC_USE_NORMAL_SERVER, server_num);
}
* \endcode
*/
bool gatt_svc_init(T_GATT_SVC_MODE mode, uint8_t svc_num);

/**
*@brief    Handle profile send data complete event. Used when receive the message
           PROFILE_EVT_SEND_DATA_COMPLETE.
*@note     If application uses the BT GATT Service module and the second parameter of gatt_svc_init
           is 0, this function shall be called after receive PROFILE_EVT_SEND_DATA_COMPLETE.
*@param[in]  conn_handle  Connection handle of the ACL link.
*@param[in]  cid          Local CID assigned by Bluetooth stack.
*@param[in]  service_id   Service ID.
*@param[in]  attrib_idx   Attribute index of characteristic.
*@param[in]  credits      Credits number.
*@param[in]  cause        Cause of service send data.
*@return   The result of handle profile send data complete event.
*@retval   true  Success.
*@retval   false Failed.
*
* <b>Example usage</b>
* \code{.c}

    T_APP_RESULT app_bqb_ext_profile_callback(T_SERVER_ID service_id, void *p_data)
    {
        T_APP_RESULT app_result = APP_RESULT_SUCCESS;
        if (service_id == SERVICE_PROFILE_GENERAL_ID)
        {
            T_SERVER_EXT_APP_CB_DATA *p_param = (T_SERVER_EXT_APP_CB_DATA *)p_data;
            switch (p_param->eventId)
            {
            case PROFILE_EVT_SEND_DATA_COMPLETE:
                if (!gatt_svc_handle_profile_data_cmpl(p_param->event_data.send_data_result.conn_handle,
                                                          p_param->event_data.send_data_result.cid,
                                                          p_param->event_data.send_data_result.service_id,
                                                          p_param->event_data.send_data_result.attrib_idx,
                                                          p_param->event_data.send_data_result.credits,
                                                          p_param->event_data.send_data_result.cause))
                {
                    APP_PRINT_ERROR0("gatt_svc_handle_profile_data_cmpl failed");
                }
                break;

            default:
                break;
            }
        }
    }
* \endcode
*/
bool gatt_svc_handle_profile_data_cmpl(uint16_t conn_handle, uint16_t cid,
                                       T_SERVER_ID service_id, uint16_t attrib_idx,
                                       uint16_t credits, uint16_t cause);

/**
*@brief    Find service characteristic uuid by attribute index.
*@param[in]  p_srv  Pointer to service table. @ref T_ATTRIB_APPL.
*@param[in]  index  Attribute index of characteristic.
*@return   The characteristic uuid. @ref T_CHAR_UUID.
*
* <b>Example usage</b>
* \code{.c}

    T_APP_RESULT tmas_attr_read_cb(uint16_t conn_handle, uint16_t cid, T_SERVER_ID service_id,
                                          uint16_t attrib_index,
                                          uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
    {
        T_APP_RESULT cause = APP_RESULT_SUCCESS;

        PROTOCOL_PRINT_INFO2("tmas_attr_read_cb: attrib_index %d, offset %x", attrib_index, offset);

        T_CHAR_UUID char_uuid = gatt_svc_find_char_uuid_by_index(tmas_attr_tbl, attrib_index);
        if (char_uuid.uuid_size != UUID_16BIT_SIZE)
        {
            PROTOCOL_PRINT_ERROR1("tmas_attr_read_cb Error: attrib_index 0x%x", attrib_index);
            return APP_RESULT_ATTR_NOT_FOUND;
        }
        *p_length = 0;
        switch (char_uuid.uu.char_uuid16)
        {
        case TMAS_UUID_CHAR_ROLE:
            {
                *pp_value = (uint8_t *)&p_tmas->role;
                *p_length = 2;
            }
            break;
        default:
            cause = APP_RESULT_ATTR_NOT_FOUND;
            break;
        }

        return (cause);
    }
* \endcode
*/
T_CHAR_UUID gatt_svc_find_char_uuid_by_index(const T_ATTRIB_APPL *p_srv, uint16_t index);

/**
*@brief    Find service characteristic attribute index by uuid.
*@param[in]  p_srv      Pointer to service table. @ref T_ATTRIB_APPL.
*@param[in]  char_uuid  Service characteristic uuid.
*@param[in]  attr_num   Total attribute numer of service.
*@param[in,out]  index  Attribute index of characteristic.
*@return   The result of find service characteristic attribute index.
*@retval   true  Success.
*@retval   false Failed.
*
* <b>Example usage</b>
* \code{.c}

    bool ccp_send_all_notify(T_TBS_CB *p_entry, uint16_t update_cfg,
                             uint16_t uuid, uint8_t *p_data, uint16_t data_len)
    {
        uint8_t i;
        uint16_t attrib_idx;

        T_CHAR_UUID char_uuid;
        char_uuid.index = 1;
        char_uuid.uuid_size = UUID_16BIT_SIZE;
        char_uuid.uu.char_uuid16 = uuid;

        if (!gatt_svc_find_char_index_by_uuid(p_entry->p_attr_tbl, char_uuid,
                                         p_entry->attr_tbl_size / sizeof(T_ATTRIB_APPL),
                                         &attrib_idx))
        {
            PROTOCOL_PRINT_ERROR1("[CCP]ccp_send_all_notify: find index failed, uuid 0x%x", uuid);
            return false;
        }

        for (i = 0; i < MAX_BLE_LINK_NUM; i++)
        {
            if ((ble_audio_db.le_link[i].used == true) &&
                (ble_audio_db.le_link[i].state == GAP_CONN_STATE_CONNECTED))
            {
                T_LE_SRV_CFG *p_srv = ble_srv_find_by_srv_id(ble_audio_db.le_link[i].conn_handle,
                                                             p_entry->service_id);
                if (p_srv != NULL && (p_srv->cccd_cfg[0] & update_cfg))
                {
                    if (tbs_send_notify(p_entry, ble_audio_db.le_link[i].conn_handle, attrib_idx, p_data,
                                        data_len) == false)
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }
* \endcode
*/
bool gatt_svc_find_char_index_by_uuid(const T_ATTRIB_APPL *p_srv, T_CHAR_UUID char_uuid,
                                      uint16_t attr_num, uint16_t *index);

/**
*@brief    Find service characteristic attribute index by uuid16.
*@param[in]  p_srv        Pointer to service table. @ref T_ATTRIB_APPL.
*@param[in]  char_uuid16  characteristic uuid16.
*@param[in]  attr_num     Total attribute numer of service.
*@return   The characteristic attribute index.
*@retval   0 find service characteristic attribute index failed.
*
* <b>Example usage</b>
* \code{.c}

    bool mics_send_mute_value_notify(uint16_t conn_handle, uint8_t *p_mute_value)
    {
        uint16_t attrib_idx = gatt_svc_find_char_index_by_uuid16(p_mics->p_mics_attr_tbl,
                                                            MICS_UUID_CHAR_MUTE,
                                                            p_mics->attr_num);
        return gatt_svc_send_data(conn_handle, L2C_FIXED_CID_ATT, p_mics->service_id,
                                     attrib_idx,
                                     p_mute_value,
                                     1, GATT_PDU_TYPE_NOTIFICATION);
    }
* \endcode
*/
uint8_t gatt_svc_find_char_index_by_uuid16(const T_ATTRIB_APPL *p_srv, uint16_t char_uuid16,
                                           uint16_t attr_num);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
