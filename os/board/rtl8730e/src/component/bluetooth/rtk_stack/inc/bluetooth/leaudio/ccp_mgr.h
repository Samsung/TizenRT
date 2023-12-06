/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ccp_mgr.h
  * @brief    Head file for Call Control Server.
  * @details  This file defines Call Control Server related definition.
  * @author
  * @date
  * @version
  * *************************************************************************************
  */

#ifndef _CCP_MGR_H_
#define _CCP_MGR_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "ccp_def.h"

/**
 * \defgroup    LEA_GAF_CCP_Server Call Control Server
 *
 * \brief Provide status and control of telephone bearer
 */

/**
 * \defgroup CCP_Server_Exported_Types Call Control Server Exported Types
 *
 * \ingroup LEA_GAF_CCP_Server
 * \{
 */

/**
 * ccp_mgr.h
 *
 * \brief  Register service parameter
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    bool gtbs;                             /**<
                                                * \arg    true  : Generic Telephone Bearer Service (GTBS)
                                                * \arg    false : Telephone Bearer Service (TBS) */

    struct
    {
        bool optional_property_notify;     /**<
                                                * \arg    true  : Support Notify property
                                                * \arg    false : Not support Notify property */
    } char_bearer_uri_schemes_supported_list;

    struct
    {
        bool support;                      /**<
                                                * \arg    true  : Support characteristic
                                                * \arg    false : Not support characteristic */
    } char_bearer_signal_strength;         /**< Bearer Signal Strength characteristic */

    struct
    {
        bool support;
    } char_incoming_call_target_bearer_uri;

    struct
    {
        bool support;
    } char_call_friendly_name;
} T_CCP_SERVER_REG_SRV_PARAM;

/**
 * ccp_mgr.h
 *
 * \brief  Bearer Provider Name characteristic
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t     *p_bearer_provider_name;                 /**< UTF-8 string */
    uint16_t    bearer_provider_name_len;
} T_CCP_SERVER_BEARER_PROVIDER_NAME;

/**
 * ccp_mgr.h
 *
 * \brief  Bearer Uniform Caller Identifier (UCI) characteristic
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t     *p_bearer_uci;                           /**<
                                                              * Identifier of the telephone bearer
                                                              *
                                                              * The "client_id" field of the UCI table is used to populate the Bearer UCI characteristic as a UTF-8 string. */
    uint16_t    bearer_uci_len;
} T_CCP_SERVER_BEARER_UCI;

/**
 * ccp_mgr.h
 *
 * \brief  Bearer Technology characteristic
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t     *p_bearer_technology; /**<
                                           * Type of technology for telephone bearer: @ref TBS_BEARER_TECHNOLOGY_CHAR_VALUE */
    uint16_t    bearer_technology_len;
} T_CCP_SERVER_BEARER_TECHNOLOGY;

/**
 * ccp_mgr.h
 *
 * \brief  Bearer URI Schemes Supported List characteristic
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t     *p_bearer_uri_schemes_supported_list;  /**<
                                                            * A comma-delimited list of supported URI schemes expressed as a UTF-8 string */
    uint16_t    bearer_uri_schemes_supported_list_len;
} T_CCP_SERVER_BEARER_URI_SCHEMES_SUPPORTED_LIST;

/**
 * ccp_mgr.h
 *
 * \brief  Incoming Call Target Bearer URI characteristic
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t     call_index;
    uint8_t    *p_incoming_call_target_uri;          /**<
                                                          * URI of the target of an incoming call
                                                          *
                                                          * The URI is comprised of the URI scheme followed by the identity of the target of the call.
                                                          *
                                                          * uri_scheme:identity_target_call */
    uint16_t    incoming_call_target_uri_len;
} T_CCP_SERVER_INCOMING_CALL_TARGET_BEARER_URI;

/**
 * ccp_mgr.h
 *
 * \brief  Termination Reason characteristic
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t     call_index;
    uint8_t     reason_code;              /**< @ref TBS_TERMINATION_REASON_CODES */
} T_CCP_SERVER_TERMINATION_REASON;

/**
 * ccp_mgr.h
 *
 * \brief  Incoming Call characteristic
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t     call_index;
    uint8_t    *p_uri;     /**<
                                * URI of the incoming caller
                                *
                                * The URI of the incoming call is comprised of the URI scheme followed by the Caller ID of the call as a UTF-8 string.
                                *
                                * uri_scheme:caller_id */
    uint16_t    uri_len;
} T_CCP_SERVER_INCOMING_CALL;

/**
 * ccp_mgr.h
 *
 * \brief  Call Friendly Name characteristic
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t     call_index;
    uint8_t    *p_friendly_name;             /**<
                                                  * Friendly name of the incoming call or outgoing call
                                                  *
                                                  * UTF-8 string */
    uint16_t    friendly_name_len;
} T_CCP_SERVER_CALL_FRIENDLY_NAME;

/**
 * ccp_mgr.h
 *
 * \brief  Set parameter
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint16_t     char_uuid;        /**<
                                        * \arg    @ref TBS_UUID_CHAR_CALL_CONTROL_POINT_OPTIONAL_OPCODES : Set parameter
                                        * \arg    @ref TBS_UUID_CHAR_INCOMING_CALL_TARGET_BEARER_URI : Set parameter and send notification
                                        * \arg    @ref TBS_UUID_CHAR_INCOMING_CALL : Set parameter
                                        * \arg    @ref TBS_UUID_CHAR_CALL_FRIENDLY_NAME : Set parameter and send notification */

    union
    {
        uint16_t                                        call_control_point_optional_opcodes;       /**<
                                                                                                        * Optional opcodes of the Call Control Point
                                                                                                        *
                                                                                                        * A bit field
                                                                                                        *
                                                                                                        * @ref TBS_CALL_CONTROL_POINT_OPTIONAL_OPCODES_CHAR_BIT_Def */
        T_CCP_SERVER_INCOMING_CALL_TARGET_BEARER_URI    incoming_call_target_bearer_uri;
        T_CCP_SERVER_INCOMING_CALL                      incoming_call;
        T_CCP_SERVER_CALL_FRIENDLY_NAME                 call_friendly_name;
    } param;
} T_CCP_SERVER_SET_PARAM;

/**
 * ccp_mgr.h
 *
 * \brief  Parameter of opcode Originate in @ref T_CCP_SERVER_WRITE_CALL_CP_IND
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t     *p_uri;    /**<
                                * Uniform Resource Identifier (URI) of the outgoing call
                                *
                                * The URI is comprised of the URI scheme followed by the Caller ID.
                                *
                                * uri_scheme:caller_id */
    uint16_t    uri_len;
} T_CCP_SERVER_WRITE_CALL_CP_PARAM_ORIGINATE_OPCODE_PARAM;

/**
 * ccp_mgr.h
 *
 * \brief  Parameter of opcode Join in @ref T_CCP_SERVER_WRITE_CALL_CP_IND
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t     *p_list_of_call_indexes;
    uint16_t    list_of_call_indexes_len;
} T_CCP_SERVER_WRITE_CALL_CP_PARAM_JOIN_OPCODE_PARAM;

/**
 * ccp_mgr.h
 *
 * \brief  Data for @ref LE_AUDIO_MSG_CCP_SERVER_WRITE_CALL_CP_IND
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint16_t        conn_handle;
    uint16_t        cid;                   /**< Channel Identifier assigned by Bluetooth stack */
    uint8_t         service_id;            /**< Service ID */

    uint8_t         opcode;                /**< @ref TBS_CALL_CONTROL_POINT_CHAR_OPCODE */

    union
    {
        uint8_t                                                   accept_opcode_call_index;
        uint8_t                                                   terminate_opcode_call_index;
        uint8_t                                                   local_hold_opcode_call_index;
        uint8_t                                                   local_retrieve_opcode_call_index;
        T_CCP_SERVER_WRITE_CALL_CP_PARAM_ORIGINATE_OPCODE_PARAM   originate_opcode_param;
        T_CCP_SERVER_WRITE_CALL_CP_PARAM_JOIN_OPCODE_PARAM        join_opcode_param;
    } param;
} T_CCP_SERVER_WRITE_CALL_CP_IND;

/**
 * ccp_mgr.h
 *
 * \brief  Data for @ref LE_AUDIO_MSG_CCP_SERVER_READ_IND
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint16_t    conn_handle;
    uint16_t    cid;
    uint8_t     service_id;
    uint16_t    char_uuid;         /**<
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_LIST_CURRENT_CALLS
                                        * \arg    @ref TBS_UUID_CHAR_INCOMING_CALL_TARGET_BEARER_URI
                                        * \arg    @ref TBS_UUID_CHAR_CALL_STATE
                                        * \arg    @ref TBS_UUID_CHAR_CALL_CONTROL_POINT_OPTIONAL_OPCODES
                                        * \arg    @ref TBS_UUID_CHAR_INCOMING_CALL
                                        * \arg    @ref TBS_UUID_CHAR_CALL_FRIENDLY_NAME
                                        *
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_PROVIDER_NAME
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_TECHNOLOGY
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_URI_SCHEMES_SUPPORTED_LIST
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH
                                        * \arg    @ref TBS_UUID_CHAR_STATUS_FLAGS
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_UCI
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH_REPORTING_INTERVAL
                                        * \arg    @ref TBS_UUID_CHAR_CONTENT_CONTROL_ID */

    uint16_t    offset;
} T_CCP_SERVER_READ_IND;

/**
 * ccp_mgr.h
 *
 * \brief  Confirmation of read
 *
 * \ingroup CCP_Server_Exported_Types
 */
typedef struct
{
    uint16_t    cause;
    uint16_t    conn_handle;
    uint16_t    cid;
    uint8_t     service_id;
    uint16_t    char_uuid;         /**<
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_PROVIDER_NAME
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_TECHNOLOGY
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_URI_SCHEMES_SUPPORTED_LIST
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH
                                        * \arg    @ref TBS_UUID_CHAR_STATUS_FLAGS
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_UCI
                                        * \arg    @ref TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH_REPORTING_INTERVAL
                                        * \arg    @ref TBS_UUID_CHAR_CONTENT_CONTROL_ID */
    uint16_t    offset;

    union
    {
        T_CCP_SERVER_BEARER_PROVIDER_NAME               bearer_provider_name;
        T_CCP_SERVER_BEARER_TECHNOLOGY                  bearer_technology;
        T_CCP_SERVER_BEARER_URI_SCHEMES_SUPPORTED_LIST  bearer_uri_schemes_supported_list;
        uint8_t                                         bearer_signal_strength;                    /**<
                                                                                                        * Level of the signal of the telephone bearer
                                                                                                        *
                                                                                                        * \arg    0 : No service
                                                                                                        * \arg    1 to 99 : Meaning of the values is left up to the implementation
                                                                                                        * \arg    100 : Maximum signal strength
                                                                                                        * \arg    101 to 254: Reserved for future use
                                                                                                        * \arg    255 : Signal strength is unavailable or has no meaning for this particular bearer */
        uint16_t                                        status_flags;                              /**<
                                                                                                        * Current status of features that Call Control Server device supports
                                                                                                        *
                                                                                                        * A bit field
                                                                                                        *
                                                                                                        * @ref TBS_STATUS_FLAGS_CHAR_BIT_Def */
        T_CCP_SERVER_BEARER_UCI                         bearer_uci;
        uint8_t                                         bearer_signal_strength_reporting_interval; /**<
                                                                                                        * Reporting interval expressed in seconds
                                                                                                        *
                                                                                                        * \arg    0 : Signal strength is reported as soon as the signal strength changes
                                                                                                        * \arg    1 to 255 */
        uint8_t                                         content_control_id;                        /**<
                                                                                                        * Represent a unique instance of a service that either controls
                                                                                                        * or provides status information on an audio-related feature */
    } param;
} T_CCP_SERVER_READ_CFM;
/**
 * End of CCP_Server_Exported_Types
 * \}
 */

/**
 * \defgroup CCP_Server_Exported_Functions Call Control Server Exported Functions
 *
 * \ingroup LEA_GAF_CCP_Server
 * \{
 */

/**
 *
 * \brief  Register service
 *
 * ccp_mgr.h
 *
 * \param[in]  p_param          Point to service registration parameter: @ref T_CCP_SERVER_REG_SRV_PARAM
 *
 * \return         Service ID of service registration operation
 * \retval 0xFF    Service registration operation is failed
 * \retval Others  Service ID of specific service. Service registration operation is successful
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t service_id = ccp_server_reg_srv(p_param);
    }
 * \endcode
 *
 * \ingroup CCP_Server_Exported_Functions
 */
uint8_t ccp_server_reg_srv(T_CCP_SERVER_REG_SRV_PARAM *p_param);

/**
 *
 * \brief  Set parameter
 *
 * ccp_mgr.h
 *
 * \param[in]  service_id       Service ID
 * \param[in]  p_param          Point to parameter: @ref T_CCP_SERVER_SET_PARAM
 *
 * \return         The result of set parameter operation
 * \retval true    Set parameter operation is successful
 * \retval false   Set parameter operation is failed
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool retval = ccp_server_set_param(service_id, p_param);
    }
 * \endcode
 *
 * \ingroup CCP_Server_Exported_Functions
 */
bool ccp_server_set_param(uint8_t service_id, T_CCP_SERVER_SET_PARAM *p_param);

/**
 *
 * \brief  Create a Call State member
 *
 * ccp_mgr.h
 *
 * \param[in]  service_id       Service ID
 * \param[in]  p_call_uri       Point to call URI
 * \param[in]  call_uri_len     Length of call URI
 *
 * \return         Call index of creation for a Call State member
 * \retval 0x00    Creation for a Call State member is failed
 * \retval Others  Call index to identify call. Creation for a Call State member is successful
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t call_index = ccp_server_create_call(service_id, p_call_uri, call_uri_len);
    }
 * \endcode
 *
 * \ingroup CCP_Server_Exported_Functions
 */
uint8_t ccp_server_create_call(uint8_t service_id, uint8_t *p_call_uri, uint16_t call_uri_len);

/**
 *
 * \brief  Update call state by call index
 *
 * ccp_mgr.h
 *
 * \param[in]  service_id       Service ID
 * \param[in]  call_index       Call index to identify call
 * \param[in]  call_state       @ref T_TBS_CALL_STATE
 * \param[in]  call_flags       A bit field. @ref TBS_CALL_FLAGS_BIT_Def
 * \param[in]  send_notification
 * \arg    true  : Send notification
 * \arg    false : Not send notification
 *
 * \return         The result of update operation
 * \retval true    Update operation is successful
 * \retval false   Update operation is failed
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool retval = ccp_server_update_call_state_by_call_index(service_id, call_index, call_state, call_flags, send_notification);
    }
 * \endcode
 *
 * \ingroup CCP_Server_Exported_Functions
 */
bool ccp_server_update_call_state_by_call_index(uint8_t service_id, uint8_t call_index,
                                                uint8_t call_state, uint8_t call_flags, bool send_notification);

/**
 *
 * \brief  Terminate a Call State member
 *
 * ccp_mgr.h
 *
 * \param[in]  service_id                 Service ID
 * \param[in]  p_termination_reason       Point to parameter: @ref T_CCP_SERVER_TERMINATION_REASON
 *
 * \return         The result of termination for a Call State member
 * \retval true    Termination for a Call State member is successful
 * \retval false   Termination for a Call State member is failed
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool retval = ccp_server_terminate_call(service_id, p_termination_reason);
    }
 * \endcode
 *
 * \ingroup CCP_Server_Exported_Functions
 */
bool ccp_server_terminate_call(uint8_t service_id,
                               T_CCP_SERVER_TERMINATION_REASON *p_termination_reason);

/**
 *
 * \brief  Confirmation of read request sent by client
 *
 *         This API should be used if all of the following conditions are true:
 *            \arg char_uuid of @ref T_CCP_SERVER_READ_IND is @ref TBS_UUID_CHAR_BEARER_PROVIDER_NAME , @ref TBS_UUID_CHAR_BEARER_TECHNOLOGY ,
 *                 @ref TBS_UUID_CHAR_BEARER_URI_SCHEMES_SUPPORTED_LIST , @ref TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH , @ref TBS_UUID_CHAR_STATUS_FLAGS ,
 *                 @ref TBS_UUID_CHAR_BEARER_UCI , @ref TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH_REPORTING_INTERVAL , or @ref TBS_UUID_CHAR_CONTENT_CONTROL_ID
 *            \arg return value of callback registered by @ref ble_audio_cback_register with msg @ref LE_AUDIO_MSG_CCP_SERVER_READ_IND
 *                 is @ref BLE_AUDIO_CB_RESULT_PENDING
 *
 * ccp_mgr.h
 *
 * \param[in]  p_read_cfm          Point to parameter: @ref T_CCP_SERVER_READ_CFM
 *
 * \return         The result of operation
 * \retval true    Operation is successful
 * \retval false   Operation is failed
 *
 * <b>Example usage</b>
 * \code{.c}
    // Callback registered by @ref ble_audio_cback_register
    {
        uint16_t cb_result = BLE_AUDIO_CB_RESULT_SUCCESS;

        switch (msg)
        {
        case LE_AUDIO_MSG_CCP_SERVER_READ_IND:
            {
                T_CCP_SERVER_READ_IND *p_read_ind = (T_CCP_SERVER_READ_IND *)buf;

                if (p_read_ind)
                {
                    T_CCP_SERVER_READ_CFM read_cfm = {0};

                    read_cfm.cause = BLE_AUDIO_CB_RESULT_SUCCESS;
                    read_cfm.conn_handle = p_read_ind->conn_handle;
                    read_cfm.cid = p_read_ind->cid;
                    read_cfm.service_id = p_read_ind->service_id;
                    read_cfm.char_uuid = p_read_ind->char_uuid;
                    read_cfm.offset = p_read_ind->offset;

                    switch (p_read_ind->char_uuid)
                    {
                    case TBS_UUID_CHAR_BEARER_PROVIDER_NAME:
                        {
                            read_cfm.param.bearer_provider_name.p_bearer_provider_name = ;
                            read_cfm.param.bearer_provider_name.bearer_provider_name_len = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            ccp_server_read_cfm(&read_cfm);
                        }
                        break;

                    case TBS_UUID_CHAR_BEARER_TECHNOLOGY:
                        {
                            read_cfm.param.bearer_technology.p_bearer_technology = ;
                            read_cfm.param.bearer_technology.bearer_technology_len = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            ccp_server_read_cfm(&read_cfm);
                        }
                        break;

                    case TBS_UUID_CHAR_BEARER_URI_SCHEMES_SUPPORTED_LIST:
                        {
                            read_cfm.param.bearer_uri_schemes_supported_list.p_bearer_uri_schemes_supported_list = ;
                            read_cfm.param.bearer_uri_schemes_supported_list.bearer_uri_schemes_supported_list_len = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            ccp_server_read_cfm(&read_cfm);
                        }
                        break;

                    case TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH:
                        {
                            read_cfm.param.bearer_signal_strength = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            ccp_server_read_cfm(&read_cfm);
                        }
                        break;

                    case TBS_UUID_CHAR_STATUS_FLAGS:
                        {
                            read_cfm.param.status_flags = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            ccp_server_read_cfm(&read_cfm);
                        }
                        break;

                    case TBS_UUID_CHAR_BEARER_UCI:
                        {
                            read_cfm.param.bearer_uci.p_bearer_uci = ;
                            read_cfm.param.bearer_uci.bearer_uci_len = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            ccp_server_read_cfm(&read_cfm);
                        }
                        break;

                    case TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH_REPORTING_INTERVAL:
                        {
                            read_cfm.param.bearer_signal_strength_reporting_interval = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            ccp_server_read_cfm(&read_cfm);
                        }
                        break;

                    case TBS_UUID_CHAR_CONTENT_CONTROL_ID:
                        {
                            read_cfm.param.content_control_id = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            ccp_server_read_cfm(&read_cfm);
                        }
                        break;

                    default:
                        break;
                    }
                }
            }
            break;

        ......
        }

        return cb_result;
    }
 * \endcode
 *
 * \ingroup CCP_Server_Exported_Functions
 */
bool ccp_server_read_cfm(T_CCP_SERVER_READ_CFM *p_read_cfm);
/**
 * End of CCP_Server_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif

#endif
