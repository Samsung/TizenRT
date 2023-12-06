#ifndef _BT_GATT_CLIENT_H_
#define _BT_GATT_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "gap.h"
#include "profile_client_def.h"
#include "gap_conn_le.h"

/** @defgroup BT_GATT_CLT BT GATT Client
  * @brief BT GATT Client
  * @{
  */

/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup BT_GATT_CLIENT_Exported_Macros BT GATT Client Macros
    * @{
    */

/** @defgroup GATT_DISCOV_MODE  GATT Client discover mode. Used for gatt_client_init.
  * @{
  */
#define GATT_CLIENT_DISCOV_MODE_CLOSED           0       /**< Not use bt gatt client module */
#define GATT_CLIENT_DISCOV_MODE_REG_SVC_BIT      0x01    /**< Only discover the services registered by gatt_client_spec_register */
#define GATT_CLIENT_DISCOV_MODE_ALL_BIT          0x02    /**< Discover all the services */
#define GATT_CLIENT_DISCOV_MODE_CCCD_STORAGE_BIT 0x04    /**< Storage CCCD, send GATT_STORAGE_EVENT_SVC_CCCD_GET_IND
                                                       and GATT_STORAGE_EVENT_SVC_CCCD_SET_IND to application */
#define GATT_CLIENT_DISCOV_MODE_USE_EXT_CLIENT   0x10    /**< Use the interfaces provided by profile_client_ext.h */
#define GATT_CLIENT_DISCOV_MODE_GATT_SVC         0x20    /**< Open GATT service, send GATT_CLIENT_EVENT_GATT_SERVICE_INFO to 
                                                       callback registered by gatt_client_start_discovery_all.*/
/**
  * @}
  */

/** @defgroup GATT_CLT_CONFIG  GATT Client configure CCCD.
  * @{
  */
#define  GATT_CLIENT_CONFIG_DISABLE          0x00    /**< Client disable service CCCD */
#define  GATT_CLIENT_CONFIG_NOTIFICATION     0x01    /**< Client enable service notification */
#define  GATT_CLIENT_CONFIG_INDICATION       0x02    /**< Client enable service indication */
#define  GATT_CLIENT_CONFIG_ALL              0x03    /**< Client enable service notification or indication */
#define  ATTR_INSTANCE_NUM_MAX               20
/**
  * @}
  */

/** @defgroup GATT_SVC_DEF  GATT Service definations.
  * @{
  */
/** @brief GATT Service Database Hash length. */
#define GATT_SVC_DATABASE_HASH_LEN 16
/** @brief GATT Service Client supported features maximum length. */
#define GATT_SVC_CLIENT_SUPPORTED_FEATURES_MAX_LEN 1
/** @brief GATT Service Server supported features maximum length. */
#define GATT_SVC_SERVER_SUPPORTED_FEATURES_MAX_LEN 1

/** @defgroup GATT_SVC_CHAR  GATT Service Characteristic flag.
  * @{
  */
#define GATT_SVC_DATABASE_HASH_FLAG              0x01 /**< Database hash characteristic. */
#define GATT_SVC_CLIENT_SUPPORTED_FEATURES_FLAG  0x02 /**< Client supported features characteristic. */
#define GATT_SVC_SERVER_SUPPORTED_FEATURES_FLAG  0x04 /**< Server supported features characteristic. */
#define GATT_SVC_SERVICE_CHANGED_FLAG            0x08 /**< Service changed characteristic. */
/**
  * @}
  */

/** @brief GATT Service Client Supported Features bit field. */
#define GATT_SVC_CLIENT_SUPPORTED_FEATURES_DEFAULT_VALUE       0x00 /**< Server shall not use any of the features associated with that bit when communicating with this client. */
#define GATT_SVC_CLIENT_SUPPORTED_FEATURES_ROBUST_CACHING_BIT  0x01 /**< The client supports robust caching. */
#define GATT_SVC_CLIENT_SUPPORTED_FEATURES_EATT_BEARER_BIT     0x02 /**< The client supports Enhanced ATT bearer. */
#define GATT_SVC_CLIENT_SUPPORTED_FEATURES_MULTI_NOTIF_BIT     0x04 /**< The client supports receiving ATT_MULTIPLE_HANDLE_VALUE_NTF PDUs. */
#define GATT_SVC_CLIENT_SUPPORTED_FEATURES_MASK                0x07

/** @brief GATT Service Server Supported Features bit field. */
#define GATT_SVC_SERVER_SUPPORTED_FEATURES_EATT_BIT  0x01
/**
  * @}
  */

/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup BT_GATT_CLIENT_Exported_Types BT GATT Client Types
    * @{
    */

/**
 * @brief GATT client state of discover services
 */
typedef enum
{
    GATT_CLIENT_STATE_IDLE      = 0x00,    /**< Idle */
    GATT_CLIENT_STATE_DISCOVERY = 0x01,    /**< Discover services */
    GATT_CLIENT_STATE_DONE      = 0x02,    /**< Discover services done */
    GATT_CLIENT_STATE_FAILED    = 0x03,    /**< Discover services failed */
} T_GATT_CLIENT_STATE;

/**
 * @brief GATT client storage state of discover services
 */
typedef enum
{
    GATT_STORAGE_EVENT_SVC_TBL_GET_IND  = 0x01,
    GATT_STORAGE_EVENT_SVC_TBL_SET_IND  = 0x02,
    GATT_STORAGE_EVENT_SVC_CCCD_GET_IND  = 0x03,
    GATT_STORAGE_EVENT_SVC_CCCD_SET_IND  = 0x04,
    GATT_STORAGE_EVENT_SVC_GATT_GET_IND  = 0x05,
    GATT_STORAGE_EVENT_SVC_GATT_SET_IND  = 0x06,
} T_GATT_STORAGE_EVENT;

/**
 * @brief GATT service data information
 */
typedef struct
{
    uint8_t  char_exist; /**< Characteristic exist bit field.  @ref group GATT_SVC_CHAR*/
    uint8_t  data_exist; /**< Data exist bit field.  @ref group GATT_SVC_CHAR*/
    uint8_t  database_hash[GATT_SVC_DATABASE_HASH_LEN]; /**< The value can be used when the bit of GATT_SVC_DATABASE_HASH_FLAG in data_exist is set.*/
    uint8_t  client_supported_features_len; /**< The value can be used when the bit of GATT_SVC_CLIENT_SUPPORTED_FEATURES_FLAG in data_exist is set.*/
    uint8_t  client_supported_features[GATT_SVC_CLIENT_SUPPORTED_FEATURES_MAX_LEN]; /**< The value can be used when the bit of GATT_SVC_CLIENT_SUPPORTED_FEATURES_FLAG in data_exist is set.*/
    uint8_t  server_supported_features_len; /**< The value can be used when the bit of GATT_SVC_SERVER_SUPPORTED_FEATURES_FLAG in data_exist is set.*/
    uint8_t  server_supported_features[GATT_SVC_SERVER_SUPPORTED_FEATURES_MAX_LEN]; /**< The value can be used when the bit of GATT_SVC_SERVER_SUPPORTED_FEATURES_FLAG in data_exist is set.*/
} T_GATT_SERVICE_DATA;

/**
 * @brief Get stored services table indication data.
 *        The event data for GATT_STORAGE_EVENT_SVC_TBL_GET_IND.
 *        After application calls gatt_client_start_discovery_all, bt gatt client will send
 *        GATT_STORAGE_EVENT_SVC_TBL_GET_IND first to check whether application has saved the service table.
 */
typedef struct
{
    uint8_t  addr[6];
    uint8_t  remote_bd_type;
    uint16_t data_len;        /**< Provided by Application */
    uint8_t  *p_data;         /**< Provided by Application */
} T_GATT_STORAGE_SVC_TBL_GET_IND;

/**
 * @brief Set stored services table indication data.
 *        The event data for GATT_STORAGE_EVENT_SVC_TBL_SET_IND.
 *        If the services are actually discovered after the
 *        gatt_client_start_discovery_all is called, notify the application
 *        to store the service table.
 */
typedef struct
{
    uint8_t   addr[6];
    uint8_t   remote_bd_type;
    uint16_t  cccd_data_len;
    uint16_t  data_len;       /**< Provided by BT GATT Client module */
    uint8_t  *p_data;         /**< Provided by BT GATT Client module */
} T_GATT_STORAGE_SVC_TBL_SET_IND;

/**
 * @brief Get stored services CCCD data.
 *        The event data for GATT_STORAGE_EVENT_SVC_CCCD_GET_IND.
 */
typedef struct
{
    uint8_t  addr[6];
    uint8_t  remote_bd_type;
    uint16_t data_len;
    uint8_t  *p_data;
} T_GATT_STORAGE_SVC_CCCD_GET_IND;

/**
 * @brief Set stored services CCCD data.
 *        The event data for GATT_STORAGE_EVENT_SVC_CCCD_SET_IND.
 */
typedef struct
{
    uint8_t   addr[6];
    uint8_t   remote_bd_type;
    uint16_t  data_len;
    uint8_t  *p_data;
} T_GATT_STORAGE_SVC_CCCD_SET_IND;

/**
 * @brief Get stored GATT service data.
 *        The event data for GATT_STORAGE_EVENT_SVC_GATT_GET_IND.
 */
typedef struct
{
    uint8_t  addr[6];
    uint8_t  remote_bd_type;
    T_GATT_SERVICE_DATA  *p_gatt_data;
} T_GATT_STORAGE_SVC_GATT_GET_IND;

/**
 * @brief Set stored GATT service data.
 *        The event data for GATT_STORAGE_EVENT_SVC_GATT_SET_IND.
 */
typedef struct
{
    uint8_t   addr[6];
    uint8_t   remote_bd_type;
    T_GATT_SERVICE_DATA  *p_gatt_data;
} T_GATT_STORAGE_SVC_GATT_SET_IND;

/**
 * @brief Define GATT client event type
 */
typedef enum
{
    GATT_CLIENT_EVENT_DIS_DONE      = 0x01,
    GATT_CLIENT_EVENT_READ_RESULT   = 0x02,
    GATT_CLIENT_EVENT_WRITE_RESULT  = 0x03,
    GATT_CLIENT_EVENT_NOTIFY_IND    = 0x04,
    GATT_CLIENT_EVENT_CCCD_CFG      = 0x05,
    GATT_CLIENT_EVENT_CONN_DEL      = 0x07,

    GATT_CLIENT_EVENT_DIS_ALL_STATE = 0x30,     /**< General client callback event: Discovery done event*/
    GATT_CLIENT_EVENT_GATT_SERVICE_INFO = 0x31, /**< General client callback event: GATT service infomation*/
} T_GATT_CLIENT_EVENT;

/**
 * @brief Define characteristic type
 */
typedef enum
{
    GATT_CHAR_UNKNOWN       = 0x00,
    GATT_CHAR_VALUE         = 0x01,   /**< Characteristic value*/
    GATT_CHAR_CCCD          = 0x02,   /**< Characteristic CCCD*/
    GATT_CHAR_DESCRIPTOR    = 0x03,   /**< Characteristic descriptor*/
} T_GATT_CHAR_TYPE;

/**
 * @brief Define service UUID structure
 */
typedef struct
{
    bool    is_uuid16;
    uint8_t instance_id;            /**< Used when the uuid is the same. Start from zero. */
    union
    {
        uint16_t    uuid16;
        uint8_t     uuid128[16];
    } p;
} T_ATTR_UUID;

/**
 * @brief Client discover all services result
 *        The event data for GATT_CLIENT_EVENT_DIS_ALL_STATE.
 */
typedef struct
{
    T_GATT_CLIENT_STATE state;
    bool load_from_ftl;
} T_GATT_CLIENT_DIS_ALL_DONE;

/**
 * @brief GATT service information
 *        The event data for GATT_CLIENT_EVENT_GATT_SERVICE_INFO.
 */
typedef struct
{
    bool is_found;
    bool load_from_ftl;
    T_GATT_SERVICE_DATA  *p_gatt_data;
} T_GATT_CLIENT_GATT_SERVICE_INFO;

/**
 * @brief Client discover service result
 *        The event data for GATT_CLIENT_EVENT_DIS_DONE.
 */
typedef struct
{
    bool is_found;
    bool load_from_ftl;
    uint8_t srv_instance_num;
} T_GATT_CLIENT_DIS_DONE;

/**
 * @brief Client read characteristic result
 *        The event data for GATT_CLIENT_EVENT_READ_RESULT.
 */
typedef struct
{
    T_GATT_CHAR_TYPE char_type;
    uint8_t  srv_instance_id;
    T_ATTR_UUID char_uuid;
    uint16_t cause;
    uint16_t handle;
    uint16_t value_size;
    uint8_t *p_value;
} T_GATT_CLIENT_READ_RESULT;

/**
 * @brief Client write characteristic result
 *        The event data for GATT_CLIENT_EVENT_WRITE_RESULT.
 */
typedef struct
{
    T_GATT_CHAR_TYPE char_type;
    uint8_t  srv_instance_id;
    T_ATTR_UUID char_uuid;
    uint16_t cause;
    T_GATT_WRITE_TYPE type;
    uint16_t handle;
} T_GATT_CLIENT_WRITE_RESULT;

/**
 * @brief Client notification or indication result
 *        The event data for GATT_CLIENT_EVENT_NOTIFY_IND.
 */
typedef struct
{
    uint8_t  srv_instance_id;
    T_ATTR_UUID char_uuid;
    bool notify;
    uint16_t handle;
    uint16_t value_size;
    uint8_t *p_value;
    uint16_t cid;
} T_GATT_CLIENT_NOTIFY_IND;

/**
 * @brief Client configure CCCD result
 *        The event data for GATT_CLIENT_EVENT_CCCD_CFG.
 */
typedef struct
{
    bool    srv_cfg;
    uint8_t srv_instance_id;
    uint16_t cccd_cfg;
    uint16_t cause;
    T_ATTR_UUID srv_uuid;
    T_ATTR_UUID char_uuid;
} T_GATT_CLIENT_CCCD_CFG;

/**
 * @brief Client handle disconnect result
 *        The event data for GATT_CLIENT_EVENT_CONN_DEL.
 */
typedef struct
{
    uint16_t cid;
    T_GAP_CHANN_TYPE chann_type;
} T_GATT_CLIENT_CONN_DEL;

/**
 * @brief GATT client data for client event @ref T_GATT_CLIENT_EVENT.
 */
typedef union
{
    T_GATT_CLIENT_DIS_DONE     dis_done;
    T_GATT_CLIENT_READ_RESULT  read_result;
    T_GATT_CLIENT_WRITE_RESULT write_result;
    T_GATT_CLIENT_NOTIFY_IND   notify_ind;
    T_GATT_CLIENT_CCCD_CFG     cccd_cfg;
    T_GATT_CLIENT_CONN_DEL     conn_del;
} T_GATT_CLIENT_DATA;

/**
 * @brief GATT client data.
 */
typedef union
{
    T_GATT_CLIENT_DATA *p_gatt_data;
} T_GATT_CLIENT_CB_DATA;

/**
 * @brief Service attribute instance.
 */
typedef struct
{
    uint8_t instance_num;
    uint8_t instance_id[ATTR_INSTANCE_NUM_MAX];
} T_ATTR_INSTANCE;

/**
 * @brief P_FUN_GATT_CLIENT_CB Specific Client Callback Function Point Definition
 *        Function pointer used in each specific client module, to send events to specific client module.
 *        The events @ref T_GATT_CLIENT_EVENT.
 */
typedef T_APP_RESULT(*P_FUN_GATT_CLIENT_CB)(uint16_t conn_handle, T_GATT_CLIENT_EVENT type,
                                            void *p_data);

/**
 * @brief P_FUN_GATT_STORAGE_CB GATT Storage Callback Function Point Definition
 *        Function pointer used to handle GATT storage event @ref T_GATT_STORAGE_EVENT.
 */
typedef T_APP_RESULT(*P_FUN_GATT_STORAGE_CB)(T_GATT_STORAGE_EVENT type, void *p_data);

/** End of BT_GATT_CLIENT_Exported_Types
    * @}
    */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup BT_GATT_CLIENT_Exported_Functions BT GATT Client Functions
    * @{
    */

/**
*@brief    Send discovery all primary services request.
*@param[in] conn_handle  Connection handle of the ACL link.
*@param[in] p_general_cb General client callback function for this ACL link.
* \arg    NULL    Not send any general client callback event to application.
* \arg    other   Send general client callback  event to application, such as GATT_CLIENT_EVENT_DIS_ALL_STATE.
*@return   T_GAP_CAUSE
*@retval   GAP_CAUSE_SUCCESS   Discovery request success.
*@retval   other               Discovery request failed.
*
* <b>Example usage</b>
* \code{.c}

    void app_handle_authen_state_evt(uint8_t conn_id, uint8_t new_state, uint16_t cause)
    {
        switch (new_state)
        {
        case GAP_AUTHEN_STATE_COMPLETE:
            {
    #if BT_GATT_CLIENT_SUPPORT
                T_BLE_LINK *p_link;
                p_link = ble_link_find_by_conn_id(conn_id);
                if (p_link)
                {
                    if (cause == GAP_SUCCESS)
                    {
                        p_link->auth_cmpl = true;
                        if (p_link->mtu_received)
                        {
                            APP_PRINT_INFO0("app_handle_authen_state_evt: gatt_client_start_discovery_all");
                            gatt_client_start_discovery_all(le_get_conn_handle(conn_id), app_ble_gap_client_cb);
                        }
                    }
                    else
                    {
                        p_link->auth_cmpl = false;
                    }
                }
    #endif
        default:
            break;
        }
    }
    T_APP_RESULT app_ble_gap_client_cb(uint16_t conn_handle, T_GATT_CLIENT_EVENT type,
                                              void *p_data)
    {
        if (type == GATT_CLIENT_EVENT_DIS_ALL_STATE)
        {
            APP_PRINT_INFO2("app_ble_gap_client_cb:is_success %d, load_from_ftl %d",
                            p_disc->state, p_disc->load_from_ftl);
        }
        return APP_RESULT_SUCCESS;
    }
* \endcode
*/
T_GAP_CAUSE gatt_client_start_discovery_all(uint16_t conn_handle,
                                            P_FUN_GATT_CLIENT_CB p_general_cb);

/**
*@brief    Enable all the characteristic cccd in a service.
*@param[in] conn_handle  Connection handle of the ACL link.
*@param[in] p_srv_uuid   Pointer to service UUID @ref T_ATTR_UUID.
                         The instance_id need to be the actual value of service instance_id.
*@param[in] cccd_cfg     CCCD value @ref group GATT_CLT_CONFIG.
*@return   T_GAP_CAUSE
*@retval   GAP_CAUSE_SUCCESS   Enable cccd request success.
*@retval   other               Enable cccd request failed.
*
* <b>Example usage</b>
* \code{.c}

    bool bass_enable_cccd(uint16_t conn_handle)
    {
        T_ATTR_UUID srv_uuid;
        T_GAP_CAUSE cause = GAP_CAUSE_INVALID_PARAM;
        srv_uuid.is_uuid16 = true;
        srv_uuid.instance_id = 0;
        srv_uuid.p.uuid16 = GATT_UUID_BASS;
        cause = gatt_client_enable_srv_cccd(conn_handle, &srv_uuid, GATT_CLIENT_CONFIG_ALL);
        if (cause == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
* \endcode
*/
T_GAP_CAUSE gatt_client_enable_srv_cccd(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                                        uint8_t cccd_cfg);

/**
*@brief    Send enable characteristic cccd request.
*@param[in] conn_handle  Connection handle of the ACL link.
*@param[in] p_srv_uuid   Pointer to service UUID @ref T_ATTR_UUID.
*                        The instance_id need to be the actual value of service instance_id.
*@param[in] p_char_uuid  Pointer to characteristic UUID @ref T_ATTR_UUID.
*                        The instance_id can be set to 0.
*@param[in] cccd_cfg     CCCD value @ref group GATT_CLT_CONFIG.
*@return   T_GAP_CAUSE
*@retval   GAP_CAUSE_SUCCESS   Enable cccd request success.
*@retval   other               Enable cccd request failed.
*
* <b>Example usage</b>
* \code{.c}

    bool ascs_client_enable_cccd(uint16_t conn_handle)
    {
        T_ATTR_UUID srv_uuid;
        T_ATTR_UUID char_uuid;
        T_GAP_CAUSE cause = GAP_CAUSE_SUCCESS;
        srv_uuid.is_uuid16 = true;
        srv_uuid.instance_id = 0;
        srv_uuid.p.uuid16 = GATT_UUID_ASCS;
        char_uuid.is_uuid16 = true;
        char_uuid.instance_id = 0;
        T_ASCS_CLIENT_DB *p_ascs_db = ascs_check_link(conn_handle);
        if (p_ascs_db != 0)
        {
            char_uuid.p.uuid16 = ASCS_UUID_CHAR_ASE_CONTROL_POINT;
            if (gatt_client_check_cccd_enabled(conn_handle, &srv_uuid, &char_uuid) == false)
            {
                cause = gatt_client_enable_char_cccd(conn_handle, &srv_uuid, &char_uuid,
                                                     GATT_CLIENT_CONFIG_ALL);
            }
        }

        if (cause == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
* \endcode
*/
T_GAP_CAUSE gatt_client_enable_char_cccd(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                                         T_ATTR_UUID *p_char_uuid, uint8_t cccd_cfg);

/**
*@brief    Get characteristic cccd bits value.
* \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
*@param[in] conn_handle     Connection handle of the ACL link.
*@param[in] handle          Attribute handle.
*@param[in,out] p_ccc_bits  Pointer to characteristic cccd value.
*@return   bool
*@retval   true   Get characteristic cccd bits value success.
*@retval   false  Get characteristic cccd bits value failed.
*
* <b>Example usage</b>
* \code{.c}

    void test(uint16_t conn_handle)
    {
        T_ATTR_UUID srv_uuid;
        T_ATTR_UUID char_uuid;
        uint16_t cccd_bits = 0;
        uint16_t handle = 0;
        uint8_t cccd_prop = 0;
        srv_uuid.is_uuid16 = true;
        srv_uuid.instance_id = 0;
        srv_uuid.p.uuid16 = GATT_UUID_BASS;
        char_uuid.is_uuid16 = true;
        char_uuid.instance_id = instance_id;
        char_uuid.p.uuid16 = BASS_UUID_CHAR_BROADCAST_RECEIVE_STATE;
        if (gatt_client_find_char_cccd_handle(conn_handle, &srv_uuid, &char_uuid, &handle, &cccd_prop))
        {
            gatt_client_get_char_cccd(conn_handle, handle, &cccd_bits);
        }
    }
* \endcode
*/
bool gatt_client_get_char_cccd(uint16_t conn_handle, uint16_t handle, uint16_t *p_ccc_bits);

/**
*@brief    Check whether characteristic cccd is enabled.
* \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
*@param[in] conn_handle  Connection handle of the ACL link.
*@param[in] p_srv_uuid   Pointer to service UUID @ref T_ATTR_UUID.
*                        The instance_id need to be the actual value of service instance_id.
*@param[in] p_char_uuid  Pointer to characteristic UUID @ref T_ATTR_UUID.
* \arg    NULL    Check all the cccd of service.
* \arg    other   Check the specific characteristic cccd.
*                 The instance_id can be set to 0 when p_char_uuid is not NULL.
*@return   bool
*@retval   true   Characteristic cccd is enabled.
*@retval   false  Characteristic cccd is not enabled.
*
* <b>Example usage</b>
* \code{.c}

    bool bass_enable_cccd(uint16_t conn_handle)
    {
        T_ATTR_UUID srv_uuid;
        T_GAP_CAUSE cause = GAP_CAUSE_SUCCESS;
        srv_uuid.is_uuid16 = true;
        srv_uuid.instance_id = 0;
        srv_uuid.p.uuid16 = GATT_UUID_BASS;
        if (gatt_client_check_cccd_enabled(conn_handle, &srv_uuid, NULL) == false)
        {
            cause = gatt_client_enable_srv_cccd(conn_handle, &srv_uuid, GATT_CLIENT_CONFIG_ALL);
        }

        if (cause == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
* \endcode
*/
bool gatt_client_check_cccd_enabled(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                                    T_ATTR_UUID *p_char_uuid);

/**
*@brief    Read characteristic by handle request.
*@param[in] conn_handle  Connection handle of the ACL link.
*@param[in] handle       Request handle.
*@param[in] p_req_cb     Specific client read request callback function.
* \arg    NULL    Send GATT_CLIENT_EVENT_READ_RESULT through the callback function
                  registered by gatt_client_spec_register.
* \arg    other   Send GATT_CLIENT_EVENT_READ_RESULT through the callback function p_req_cb.
*@return   T_GAP_CAUSE
*@retval   GAP_CAUSE_SUCCESS   Read request success.
*@retval   other               Read request failed.
*
* <b>Example usage</b>
* \code{.c}

//case 1
    bool bass_read_brs_value(uint16_t conn_handle, uint8_t instance_id)
    {
        T_ATTR_UUID srv_uuid;
        T_ATTR_UUID char_uuid;
        T_GAP_CAUSE cause = GAP_CAUSE_INVALID_PARAM;
        uint16_t handle = 0;
        srv_uuid.is_uuid16 = true;
        srv_uuid.instance_id = 0;
        srv_uuid.p.uuid16 = GATT_UUID_BASS;
        char_uuid.is_uuid16 = true;
        char_uuid.instance_id = instance_id;
        char_uuid.p.uuid16 = BASS_UUID_CHAR_BROADCAST_RECEIVE_STATE;
        if (gatt_client_find_char_handle(conn_handle, &srv_uuid, &char_uuid, &handle))
        {
            cause = gatt_client_read(conn_handle, handle, read_brs_cb);
        }
        if (cause == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    T_APP_RESULT read_brs_cb(uint16_t conn_handle, T_GATT_CLIENT_EVENT type, void *p_data)
    {
        PROTOCOL_PRINT_TRACE2("read_brs_cb: conn_handle 0x%x, type 0x%x",
                              conn_handle, type);
        return APP_RESULT_SUCCESS;
    }

//case 2
    bool bass_read_brs_value(uint16_t conn_handle, uint8_t instance_id)
    {
        T_ATTR_UUID srv_uuid;
        T_ATTR_UUID char_uuid;
        T_GAP_CAUSE cause = GAP_CAUSE_INVALID_PARAM;
        uint16_t handle = 0;
        srv_uuid.is_uuid16 = true;
        srv_uuid.instance_id = 0;
        srv_uuid.p.uuid16 = GATT_UUID_BASS;
        char_uuid.is_uuid16 = true;
        char_uuid.instance_id = instance_id;
        char_uuid.p.uuid16 = BASS_UUID_CHAR_BROADCAST_RECEIVE_STATE;
        if (gatt_client_find_char_handle(conn_handle, &srv_uuid, &char_uuid, &handle))
        {
            cause = gatt_client_read(conn_handle, handle, NULL);
        }
        if (cause == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool bass_client_init(void)
    {
        T_ATTR_UUID srv_uuid = {0};
        srv_uuid.is_uuid16 = true;
        srv_uuid.p.uuid16 = GATT_UUID_BASS;
        gatt_client_spec_register(&srv_uuid, bass_client_cb);
        ble_dm_cback_register(bass_client_dm_cback);
        return true;
    }
    T_APP_RESULT bass_client_cb(uint16_t conn_handle, T_GATT_CLIENT_EVENT type, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        T_GATT_CLIENT_DATA *p_client_cb_data = (T_GATT_CLIENT_DATA *)p_data;
        T_BASS_CLIENT_DB *p_bass_db = bass_check_link(conn_handle);
        if (p_bass_db == NULL && type != GATT_CLIENT_EVENT_DIS_DONE)
        {
            goto error;
        }
        switch (type)
        {
        case GATT_CLIENT_EVENT_READ_RESULT:
            {
                PROTOCOL_PRINT_INFO7("[BAP][BASS] GATT_CLIENT_EVENT_READ_RESULT: conn_handle 0x%x, cause 0x%x, char_type %d, srv_instance_id %d, uuid16[%d] 0x%x, handle 0x%x",
                                     conn_handle, p_client_cb_data->read_result.cause,
                                     p_client_cb_data->read_result.char_type,
                                     p_client_cb_data->read_result.srv_instance_id,
                                     p_client_cb_data->read_result.char_uuid.instance_id,
                                     p_client_cb_data->read_result.char_uuid.p.uuid16,
                                     p_client_cb_data->read_result.handle);
                PROTOCOL_PRINT_INFO2("[BAP][BASS] GATT_CLIENT_EVENT_READ_RESULT: value[%d] %b",
                                     p_client_cb_data->read_result.value_size,
                                     TRACE_BINARY(p_client_cb_data->read_result.value_size,
                                                  p_client_cb_data->read_result.p_value));
            }
            break;

        default:
            break;
        }

        return result;
    error:
        return APP_RESULT_APP_ERR;
    }
* \endcode
*/
T_GAP_CAUSE gatt_client_read(uint16_t conn_handle, uint16_t handle, P_FUN_GATT_CLIENT_CB p_req_cb);

/**
*@brief    Read characteristic by handle request.
*@param[in] conn_handle   Connection handle of the ACL link.
*@param[in] start_handle  Start handle of range to be searched.
*@param[in] end_handle    End handle of range to be searched.
*@param[in] uuid16        Request 16 bit UUID.
*@param[in] p_req_cb      Specific client read request callback function.
* \arg    NULL    Send GATT_CLIENT_EVENT_READ_RESULT through the callback function
                  registered by gatt_client_spec_register.
* \arg    other   Send GATT_CLIENT_EVENT_READ_RESULT through the callback function p_req_cb.
*@return   T_GAP_CAUSE
*@retval   GAP_CAUSE_SUCCESS   Read request success.
*@retval   other               Read request failed.
*
* <b>Example usage</b>
* \code{.c}

    bool tmas_read_role_by_uuid(uint16_t conn_handle)
    {
        if (gatt_client_read_uuid(conn_handle, 0x01,
                                  0xffff, TMAS_UUID_CHAR_ROLE, NULL) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    T_APP_RESULT tmap_client_cb(uint16_t conn_handle, T_GATT_CLIENT_EVENT type, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        T_GATT_CLIENT_DATA *p_client_cb_data = (T_GATT_CLIENT_DATA *)p_data;

        switch (type)
        {
        case GATT_CLIENT_EVENT_READ_RESULT:
            {
                PROTOCOL_PRINT_INFO2("GATT_CLIENT_EVENT_READ_RESULT: conn_handle 0x%x, cause 0x%x",
                                     conn_handle, p_client_cb_data->read_result.cause);
            }
            break;

        default:
            break;
        }

        return result;
    }
* \endcode
*/
T_GAP_CAUSE gatt_client_read_uuid(uint16_t conn_handle, uint16_t start_handle,
                                  uint16_t end_handle, uint16_t uuid16, P_FUN_GATT_CLIENT_CB p_req_cb);

/**
*@brief    Write characteristic by handle request.
*@param[in] conn_handle  Connection handle of the ACL link.
*@param[in] write_type   Type of write.
*@param[in] handle       Attribute handle.
*@param[in] length       Length of data to be written.
                         If write_type is GATT_WRITE_TYPE_REQ, range of length is from 0 to 512.
                         If write_type is GATT_WRITE_TYPE_CMD, range of length is from 0 to (mtu_size - 3).
                         If write_type is GATT_WRITE_TYPE_SIGNED_CMD, range of length is from 0 to (mtu_size - 15).
                         uint16_t mtu_size is acquired by le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id) or gap_chann_get_info().
*@param[in] p_data       Pointer to the data to be written.
*@param[in] p_req_cb     Specific client write request callback function.
* \arg    NULL    Send GATT_CLIENT_EVENT_WRITE_RESULT through the callback function
                  registered by gatt_client_spec_register.
* \arg    other   Send GATT_CLIENT_EVENT_WRITE_RESULT through the callback function p_req_cb.
*@return   T_GAP_CAUSE
*@retval   GAP_CAUSE_SUCCESS   Write request success.
*@retval   other               Write request failed.
*
* <b>Example usage</b>
* \code{.c}

    bool mics_write_mute_value(uint16_t conn_handle, uint8_t mic_mute)
    {
        T_BLE_AUDIO_LINK *p_link = ble_audio_link_find_by_conn_handle(conn_handle);

        if (p_link == NULL)
        {
            return false;
        }

        T_ATTR_UUID srv_uuid;
        T_ATTR_UUID char_uuid;
        T_GAP_CAUSE cause = GAP_CAUSE_INVALID_PARAM;
        uint16_t handle = 0;
        srv_uuid.is_uuid16 = true;
        srv_uuid.instance_id = 0;
        srv_uuid.p.uuid16 = GATT_UUID_MICS;
        char_uuid.is_uuid16 = true;
        char_uuid.instance_id = 0;
        char_uuid.p.uuid16 = MICS_UUID_CHAR_MUTE;

        if (gatt_client_find_char_handle(conn_handle, &srv_uuid, &char_uuid, &handle))
        {
            cause = gatt_client_write(conn_handle, GATT_WRITE_TYPE_REQ, handle, 1, &mic_mute, NULL);
        }

        if (cause == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    static T_APP_RESULT mics_client_callback(uint16_t conn_handle, T_GATT_CLIENT_EVENT type,
                                             void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        T_GATT_CLIENT_DATA *p_client_cb_data = (T_GATT_CLIENT_DATA *)p_data;
        T_BLE_AUDIO_LINK *p_link = ble_audio_link_find_by_conn_handle(conn_handle);

        if (p_link == NULL)
        {
            return APP_RESULT_APP_ERR;
        }

        switch (type)
        {
        case GATT_CLIENT_EVENT_WRITE_RESULT:
            {
                PROTOCOL_PRINT_INFO2("GATT_CLIENT_EVENT_WRITE_RESULT: conn_handle 0x%x, cause 0x%x",
                                     conn_handle, p_client_cb_data->write_result.cause);
            }
            break;

        default:
            break;
        }

        return result;
    }
* \endcode
*/
T_GAP_CAUSE gatt_client_write(uint16_t conn_handle, T_GATT_WRITE_TYPE write_type,
                              uint16_t handle, uint16_t length, uint8_t *p_data, P_FUN_GATT_CLIENT_CB p_req_cb);

/**
*@brief  Confirm from application when receive indication from server.
* \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
*@param[in]  conn_handle    Connection handle of the ACL link.
*@param[in]  cid            Local CID. The value shall be get from \ref GATT_CLIENT_EVENT_NOTIFY_IND callback.
*@return   T_GAP_CAUSE
*@retval   GAP_CAUSE_SUCCESS   Write request success.
*@retval   other               Write request failed.
*
* <b>Example usage</b>
* \code{.c}
    T_APP_RESULT test_client_cb(uint16_t conn_handle, T_GATT_CLIENT_EVENT type, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        T_GATT_CLIENT_DATA *p_client_cb_data = (T_GATT_CLIENT_DATA *)p_data;
        switch (type)
        {
        case GATT_CLIENT_EVENT_NOTIFY_IND:
            {
                gatt_client_ind_confirm(conn_handle, p_client_cb_data->notify_ind.cid);
                result = APP_RESULT_PENDING;
            }
            break;
        default:
            break;
        }
    }
* \endcode
*/
T_GAP_CAUSE gatt_client_ind_confirm(uint16_t conn_handle, uint16_t cid);

/**
*@brief    Used by specific client, register callback.
*@param[in] p_srv_uuid  Pointer to service UUID @ref T_ATTR_UUID.
*@param[in] p_fun_cb    Specific client callback function.
*@return   T_GAP_CAUSE
*@retval   GAP_CAUSE_SUCCESS   Register callback request success.
*@retval   other               Register callback request failed.
*
* <b>Example usage</b>
* \code{.c}

    bool bass_client_init(void)
    {
        T_ATTR_UUID srv_uuid = {0};
        srv_uuid.is_uuid16 = true;
        srv_uuid.p.uuid16 = GATT_UUID_BASS;
        gatt_client_spec_register(&srv_uuid, bass_client_cb);
        ble_dm_cback_register(bass_client_dm_cback);
        return true;
    }
    T_APP_RESULT bass_client_cb(uint16_t conn_handle, T_GATT_CLIENT_EVENT type, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        T_GATT_CLIENT_DATA *p_client_cb_data = (T_GATT_CLIENT_DATA *)p_data;
        T_BASS_CLIENT_DB *p_bass_db = bass_check_link(conn_handle);
        if (p_bass_db == NULL && type != GATT_CLIENT_EVENT_DIS_DONE)
        {
            goto error;
        }
        switch (type)
        {
        case GATT_CLIENT_EVENT_DIS_DONE:
            PROTOCOL_PRINT_INFO2("[BAP][BASS] GATT_CLIENT_EVENT_DIS_DONE: is_found %d, srv_instance_num %d",
                                 p_client_cb_data->dis_done.is_found,
                                 p_client_cb_data->dis_done.srv_instance_num);
            break;
        case GATT_CLIENT_EVENT_READ_RESULT:
            break;
        case GATT_CLIENT_EVENT_NOTIFY_IND:
            break;
        case GATT_CLIENT_EVENT_WRITE_RESULT:
            break;
        default:
            break;
        }
    }
* \endcode
*/
T_GAP_CAUSE gatt_client_spec_register(T_ATTR_UUID *p_srv_uuid, P_FUN_GATT_CLIENT_CB p_fun_cb);

/**
*@brief    Get characteristic descriptor handle range.
 *\xrefitem Added_API_2_12_0_0 "Added Since 2.12.0.0" "Added API"
*@param[in] conn_handle         Connection handle of the ACL link.
*@param[in] p_srv_uuid          Pointer to service UUID @ref T_ATTR_UUID.
*                               The instance_id need to be the actual value of service instance_id.
*@param[in] p_char_uuid         Pointer to characteristic UUID @ref T_ATTR_UUID.
*                               The instance_id need to be the actual value of characteristic instance_id.
*@param[in,out] p_start_handle  Pointer to start handle.
*@param[in,out] p_end_handle    Pointer to end handle.
*@return   bool
*@retval   true   Get characteristic descriptor handle range success.
*@retval   false  Get characteristic descriptor handle range failed.
*
* <b>Example usage</b>
* \code{.c}

    void test(uint16_t conn_handle)
    {
        T_ATTR_UUID srv_uuid;
        T_ATTR_UUID char_uuid;
        uint16_t start_handle = 0;
        uint16_t end_handle = 0;
        srv_uuid.is_uuid16 = true;
        srv_uuid.instance_id = 0;
        srv_uuid.p.uuid16 = GATT_UUID_BASS;
        char_uuid.is_uuid16 = true;
        char_uuid.instance_id = instance_id;
        char_uuid.p.uuid16 = BASS_UUID_CHAR_BROADCAST_RECEIVE_STATE;
        gatt_client_find_char_descriptor_range(conn_handle, &srv_uuid, &char_uuid,
                                               &start_handle, &end_handle);
    }
* \endcode
*/
bool gatt_client_find_char_descriptor_range(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                                            T_ATTR_UUID *p_char_uuid, uint16_t *p_start_handle, uint16_t *p_end_handle);

/**
*@brief    Find characteristic attribute handle.
*@param[in] conn_handle   Connection handle of the ACL link.
*@param[in] p_srv_uuid    Pointer to service UUID @ref T_ATTR_UUID.
*                         The instance_id need to be the actual value of service instance_id.
*@param[in] p_char_uuid   Pointer to characteristic UUID @ref T_ATTR_UUID.
*                         The instance_id need to be the actual value of service instance_id.
*@param[in,out] p_handle  Pointer to characteristic attribute handle.
*@return   bool
*@retval   true   Find characteristic attribute handle success.
*@retval   false  Find characteristic attribute handle failed.
*
* <b>Example usage</b>
* \code{.c}

    bool bass_read_brs_value(uint16_t conn_handle, uint8_t instance_id)
    {
        T_ATTR_UUID srv_uuid;
        T_ATTR_UUID char_uuid;
        T_GAP_CAUSE cause = GAP_CAUSE_INVALID_PARAM;
        uint16_t handle = 0;
        srv_uuid.is_uuid16 = true;
        srv_uuid.instance_id = 0;
        srv_uuid.p.uuid16 = GATT_UUID_BASS;
        char_uuid.is_uuid16 = true;
        char_uuid.instance_id = instance_id;
        char_uuid.p.uuid16 = BASS_UUID_CHAR_BROADCAST_RECEIVE_STATE;
        if (gatt_client_find_char_handle(conn_handle, &srv_uuid, &char_uuid, &handle))
        {
            cause = gatt_client_read(conn_handle, handle, NULL);
        }
        if (cause == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
* \endcode
*/
bool gatt_client_find_char_handle(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                                  T_ATTR_UUID *p_char_uuid, uint16_t *p_handle);

/**
*@brief    Find characteristic cccd attribute handle.
* \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
*@param[in] conn_handle   Connection handle of the ACL link.
*@param[in] p_srv_uuid    Pointer to service UUID @ref T_ATTR_UUID.
*                         The instance_id need to be the actual value of service instance_id.
*@param[in] p_char_uuid   Pointer to characteristic UUID @ref T_ATTR_UUID.
*                         The instance_id need to be the actual value of characteristic instance_id.
*@param[in,out] p_handle  Pointer to characteristic attribute handle.
*@return   bool
*@retval   true   Find characteristic attribute handle success.
*@retval   false  Find characteristic attribute handle failed.
*
* <b>Example usage</b>
* \code{.c}

    void test(uint16_t conn_handle, uint16_t instance_id)
    {
        T_ATTR_UUID srv_uuid;
        T_ATTR_UUID char_uuid;
        uint16_t cccd_bits = 0;
        uint16_t handle = 0;
        uint8_t cccd_prop = 0;
        srv_uuid.is_uuid16 = true;
        srv_uuid.instance_id = 0;
        srv_uuid.p.uuid16 = GATT_UUID_BASS;
        char_uuid.is_uuid16 = true;
        char_uuid.instance_id = instance_id;
        char_uuid.p.uuid16 = BASS_UUID_CHAR_BROADCAST_RECEIVE_STATE;
        if (gatt_client_find_char_cccd_handle(conn_handle, &srv_uuid, &char_uuid, &handle, &cccd_prop))
        {
            gatt_client_get_char_cccd(conn_handle, handle, &cccd_bits);
        }
    }
* \endcode
*/
bool gatt_client_find_char_cccd_handle(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                                       T_ATTR_UUID *p_char_uuid, uint16_t *p_handle, uint8_t *p_cccd_prop);

/**
*@brief    Find primary service by included service.
*@param[in] conn_handle        Connection handle of the ACL link.
*@param[in] p_included_srv     Pointer to included service UUID @ref T_ATTR_UUID.
*                              The instance_id need to be the actual value of service instance_id.
*@param[in,out] p_primary_srv  Pointer to primary service UUID @ref T_ATTR_UUID.
*@return   bool
*@retval   true   Find primary service by included service success.
*@retval   false  Find primary service by included service failed.
*
* <b>Example usage</b>
* \code{.c}

    uint16_t csip_ts_handle_msg(T_LE_AUDIO_MSG msg, void *buf)
    {
        uint16_t cb_result = BLE_AUDIO_CB_RESULT_SUCCESS;
        switch (msg)
        {
        case LE_AUDIO_MSG_CSIS_CLIENT_DIS_DONE:
            {
                T_CSIS_CLIENT_DIS_DONE *p_dis_done = (T_CSIS_CLIENT_DIS_DONE *)buf;
                APP_PRINT_INFO4("LE_AUDIO_MSG_CSIS_CLIENT_DIS_DONE: conn_handle 0x%x, is_found %d, load_from_ftl %d, srv_num %d",
                                p_dis_done->conn_handle, p_dis_done->is_found,
                                p_dis_done->load_from_ftl, p_dis_done->srv_num);
                if (p_dis_done->is_found)
                {
                    if (p_dis_done->srv_num)
                    {
                        T_ATTR_UUID srv_uuid;
                        T_ATTR_UUID include_srv;
                        srv_uuid.is_uuid16 = true;
                        srv_uuid.p.uuid16 = GATT_UUID_CSIS;
                        for (uint8_t i = 0; i < p_dis_done->srv_num; i++)
                        {
                            srv_uuid.instance_id = i;
                            if (gatt_client_find_primary_srv_by_include(p_dis_done->conn_handle, &srv_uuid,
                                                                        &include_srv))
                            {
                                PROTOCOL_PRINT_INFO2("LE_AUDIO_MSG_CSIS_CLIENT_DIS_DONE: instance_id %d, serv 0x%x",
                                                     include_srv.instance_id, include_srv.p.uuid16);
                            }
                        }
                    }
                }
            }
            break;

        default:
            break;
        }
    }
* \endcode
*/
bool gatt_client_find_primary_srv_by_include(uint16_t conn_handle, T_ATTR_UUID *p_included_srv,
                                             T_ATTR_UUID *p_primary_srv);

/**
*@brief    Find included service by primary service.
*@param[in] conn_handle          Connection handle of the ACL link.
*@param[in] p_primary_srv        Pointer to primary service UUID @ref T_ATTR_UUID.
*                                The instance_id need to be the actual value of service instance_id.
*@param[in] p_included_srv       Pointer to included service UUID @ref T_ATTR_UUID.
*                                The instance_id can be set to 0.
*@param[in,out] p_attr_instance  Pointer to service attribute instance @ref T_ATTR_INSTANCE.
*@return   bool
*@retval   true   Find included service by primary service success.
*@retval   false  Find included service by primary service failed.
*
* <b>Example usage</b>
* \code{.c}

    T_APP_RESULT cap_cas_callback(uint16_t conn_handle, T_GATT_CLIENT_EVENT type, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        T_GATT_CLIENT_DATA *p_client_cb_data = (T_GATT_CLIENT_DATA *)p_data;

        switch (type)
        {
        case GATT_CLIENT_EVENT_DIS_DONE:
            {
                T_BLE_AUDIO_LINK *p_link = ble_audio_link_find_by_conn_handle(conn_handle);
                if (p_link == NULL)
                {
                    return APP_RESULT_APP_ERR;
                }
                if (p_client_cb_data->dis_done.is_found)
                {
                    T_ATTR_UUID srv_uuid;
                    T_ATTR_UUID inc_uuid;
                    T_ATTR_INSTANCE attr_instance;

                    p_link->cap_discov_flags |= CAP_CAS_FLAG;

                    srv_uuid.is_uuid16 = true;
                    srv_uuid.instance_id = 0;
                    srv_uuid.p.uuid16 = GATT_UUID_CAS;
                    inc_uuid.is_uuid16 = true;
                    inc_uuid.instance_id = 0;
                    inc_uuid.p.uuid16 = GATT_UUID_CSIS;
                    if (gatt_client_find_include_srv_by_primary(conn_handle, &srv_uuid, &inc_uuid, &attr_instance))
                    {
                        if (attr_instance.instance_num == 1)
                        {
                            PROTOCOL_PRINT_INFO0("[CAP][VCP] cap_cas_callback: support csis");
                        }
                    }
                }
            }
            break;

        default:
            break;
        }

        return result;
    }
* \endcode
*/
bool gatt_client_find_include_srv_by_primary(uint16_t conn_handle, T_ATTR_UUID *p_primary_srv,
                                             T_ATTR_UUID *p_included_srv,
                                             T_ATTR_INSTANCE *p_attr_instance);

/**
*@brief    Get the number of characteristics with the same uuid.
*@param[in] conn_handle  Connection handle of the ACL link.
*@param[in] p_srv_uuid   Pointer to service UUID @ref T_ATTR_UUID.
*                        The instance_id need to be the actual value of service instance_id.
*@param[in] p_char_uuid  Pointer to characteristic UUID @ref T_ATTR_UUID.
*                        The instance_id can be set to 0.
*@return   The number of service characteristic.
*
* <b>Example usage</b>
* \code{.c}

    void test(uint16_t conn_handle)
    {
        uint8_t brs_num = 0;
        T_ATTR_UUID srv_uuid;
        T_ATTR_UUID char_uuid;
        srv_uuid.is_uuid16 = true;
        srv_uuid.instance_id = 0;
        srv_uuid.p.uuid16 = GATT_UUID_BASS;
        char_uuid.is_uuid16 = true;
        char_uuid.instance_id = 0;
        char_uuid.p.uuid16 = BASS_UUID_CHAR_BROADCAST_RECEIVE_STATE;
        brs_num = gatt_client_get_char_num(conn_handle, &srv_uuid, &char_uuid);
        return;
    }
* \endcode
*/
uint8_t gatt_client_get_char_num(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                                 T_ATTR_UUID *p_char_uuid);

/**
*@brief    Get service characteristic property.
*@param[in] conn_handle       Connection handle of the ACL link.
*@param[in] p_srv_uuid        Pointer to service UUID @ref T_ATTR_UUID.
*                             The instance_id need to be the actual value of service instance_id.
*@param[in] p_char_uuid       Pointer to characteristic UUID @ref T_ATTR_UUID.
*                             The instance_id need to be the actual value of characteristic instance_id.
*@param[in,out] p_properties  Pointer to characteristic property @ref GATT_ATTRIBUTE.
*@return   bool.
*@retval   true   Get service characteristic property success.
*@retval   false  Get service characteristic property failed.
*
* <b>Example usage</b>
* \code{.c}

    void test(void)
    {
        T_PACS_CLIENT_DIS_DONE dis_done;
        T_PACS_CLIENT_DB *p_pacs_db;
        uint16_t properties;
        memset(&dis_done, 0, sizeof(T_PACS_CLIENT_DIS_DONE));
        T_ATTR_UUID srv_uuid;
        T_ATTR_UUID char_uuid;
        srv_uuid.is_uuid16 = true;
        srv_uuid.instance_id = 0;
        srv_uuid.p.uuid16 = GATT_UUID_PACS;
        char_uuid.is_uuid16 = true;
        char_uuid.instance_id = 0;
        char_uuid.p.uuid16 = PACS_UUID_CHAR_SINK_PAC;
        if (gatt_client_get_char_prop(conn_handle, &srv_uuid, &char_uuid, &properties) == false)
        {
            dis_done.sink_loc_exist = false;
        }
        else
        {
            if (properties & GATT_CHAR_PROP_WRITE)
            {
                p_pacs_db->sink_loc_writable = true;
                dis_done.sink_loc_writable = true;
            }
        }
        return;
    }
* \endcode
*/
bool gatt_client_get_char_prop(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                               T_ATTR_UUID *p_char_uuid, uint16_t *p_properties);

/**
*@brief    Whether get the service table form FTL
 *\xrefitem Added_API_2_12_0_0 "Added Since 2.12.0.0" "Added API"
*@param[in] conn_handle  Connection handle of the ACL link.
*@return   bool
*@retval   true   Get the service table form FTL.
*@retval   false  Not get the service table form FTL.
*
* <b>Example usage</b>
* \code{.c}

    void cap_check_disc_done(T_BLE_AUDIO_LINK *p_link, uint16_t conn_handle, uint8_t flags)
    {
        PROTOCOL_PRINT_INFO2("[CAP][VCP] cap_check_disc_done: cap_init_flags 0x%x, flags 0x%x",
                             p_link->cap_init_flags, flags);
        if (p_link->cap_init_flags & flags)
        {
            p_link->cap_init_flags &= ~flags;
            if (p_link->cap_init_flags == 0)
            {
                T_CAP_DIS_DONE dis_done = {0};

                dis_done.conn_handle = conn_handle;
                dis_done.load_from_ftl = gatt_client_is_load_from_ftl(conn_handle);
                if (p_link->cap_discov_flags & CAP_CAS_FLAG)
                {
                    dis_done.cas_is_found = true;
                    if (p_link->cap_discov_flags & CAP_CSIS_FLAG)
                    {
                        dis_done.cas_inc_csis = true;
                    }
                }
            }
        }
    }
* \endcode
*/
bool gatt_client_is_load_from_ftl(uint16_t conn_handle);

/**
*@brief    Configure default client supported features
* \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
*@param[in] feature  Client supported features.
*@return   bool
*@retval   true   Configure the feature success.
*@retval   false  Configure the feature failed.
*
* <b>Example usage</b>
* \code{.c}
    void test(void)
    {
        gatt_client_init(GATT_CLIENT_DISCOV_MODE_REG_SVC_BIT |
                            GATT_CLIENT_DISCOV_MODE_CCCD_STORAGE_BIT |
                            GATT_CLIENT_DISCOV_MODE_USE_EXT_CLIENT |
                            GATT_CLIENT_DISCOV_MODE_GATT_SVC);
        gatt_client_cfg_client_supported_feature(GATT_SVC_CLIENT_SUPPORTED_FEATURES_EATT_BEARER_BIT);
    }

* \endcode
*/
bool gatt_client_cfg_client_supported_feature(uint8_t feature);

/**
*@brief    Initialize GATT client discover mode.
 *\xrefitem Added_API_2_12_0_0 "Added Since 2.12.0.0" "Added API"
*@param[in] mode  Discover mode @ref group GATT_DISCOV_MODE.
*@return   bool
*@retval   true   Initialize GATT client success.
*@retval   false  Initialize GATT client failed.
*@note     Application can't use functions client_init() and client_cfg_use_ext_api()
*          when use function gatt_client_init() to initialize the GATT client module.
* <b>Example usage</b>
* \code{.c}

    void test(void)
    {
        gatt_client_init(GATT_CLIENT_DISCOV_MODE_REG_SVC_BIT | GATT_CLIENT_DISCOV_MODE_CCCD_STORAGE_BIT);
    }
* \endcode
*/
bool gatt_client_init(uint16_t mode);

/**
*@brief    Register storage callback function.
*@param[in] p_fun_cb  Storage callback function @ref P_FUN_GATT_STORAGE_CB.
*@return   bool
*@retval   true   Register storage callback function success.
*@retval   false  Register storage callback function failed.
*
* <b>Example usage</b>
* \code{.c}

    void test(void)
    {
        gatt_client_storage_register(gattc_tbl_storage_cb);
    }
* \endcode
*/
bool gatt_client_storage_register(P_FUN_GATT_STORAGE_CB p_fun_cb);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
