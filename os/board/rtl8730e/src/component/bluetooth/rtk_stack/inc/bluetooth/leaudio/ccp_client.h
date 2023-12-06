/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ccp_client.h
  * @brief    Head file for Call Control Client.
  * @details  This file defines Call Control Client related definition.
  * @author
  * @date
  * @version
  * *************************************************************************************
  */

#ifndef _CCP_CLIENT_H_
#define _CCP_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "ccp_def.h"

/**
 * \defgroup    LEA_GAF_CCP_Client Call Control Client
 *
 * \brief Make, receive and manage call
 */

/**
 * \defgroup CCP_Client_Exported_Macros Call Control Client Exported Macros
 *
 * \ingroup LEA_GAF_CCP_Client
 * \{
 */

/**
 * ccp_client.h
 *
 * \defgroup CCP_CLIENT_CFG_CCCD_FLAG CCP Client Configure CCCD Flag
 *
 * \brief    Use the combination of macro definitions to specify whether to configure CCCD
 *
 * \ingroup CCP_Client_Exported_Macros
 * \{
 */
#define CCP_CLIENT_CFG_CCCD_FLAG_BEARER_PROVIDER_NAME                      0x00000001
#define CCP_CLIENT_CFG_CCCD_FLAG_BEARER_TECHNOLOGY                         0x00000002
#define CCP_CLIENT_CFG_CCCD_FLAG_BEARER_URI_SCHEMES_SUPPORTED_LIST         0x00000004
#define CCP_CLIENT_CFG_CCCD_FLAG_BEARER_SIGNAL_STRENGTH                    0x00000008
#define CCP_CLIENT_CFG_CCCD_FLAG_BEARER_LIST_CURRENT_CALLS                 0x00000010
#define CCP_CLIENT_CFG_CCCD_FLAG_STATUS_FLAGS                              0x00000020
#define CCP_CLIENT_CFG_CCCD_FLAG_INCOMING_CALL_TARGET_BEARER_URI           0x00000040
#define CCP_CLIENT_CFG_CCCD_FLAG_CALL_STATE                                0x00000080
#define CCP_CLIENT_CFG_CCCD_FLAG_CALL_CONTROL_POINT                        0x00000100
#define CCP_CLIENT_CFG_CCCD_FLAG_TERMINATION_REASON                        0x00000200
#define CCP_CLIENT_CFG_CCCD_FLAG_INCOMING_CALL                             0x00000400
#define CCP_CLIENT_CFG_CCCD_FLAG_CALL_FRIENDLY_NAME                        0x00000800
/**
 * End of CCP_CLIENT_CFG_CCCD_FLAG
 * \}
 */

/**
 * End of CCP_Client_Exported_Macros
 * \}
 */

/**
 * \defgroup CCP_Client_Exported_Types Call Control Client Exported Types
 *
 * \ingroup LEA_GAF_CCP_Client
 * \{
 */

/**
 * ccp_client.h
 *
 * \brief  Parameter of opcode Originate in @ref T_CCP_CLIENT_WRITE_CALL_CP_PARAM
 *
 * \ingroup CCP_Client_Exported_Types
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
} T_CCP_CLIENT_WRITE_CALL_CP_PARAM_ORIGINATE_OPCODE_PARAM;

/**
 * ccp_client.h
 *
 * \brief  Parameter of opcode Join in @ref T_CCP_CLIENT_WRITE_CALL_CP_PARAM
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t     *p_list_of_call_indexes;
    uint16_t    list_of_call_indexes_len;
} T_CCP_CLIENT_WRITE_CALL_CP_PARAM_JOIN_OPCODE_PARAM;

/**
 * ccp_client.h
 *
 * \brief  Write Call Control Point characteristic parameter
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t       opcode;                                /**< @ref TBS_CALL_CONTROL_POINT_CHAR_OPCODE */

    union
    {
        uint8_t                                                   accept_opcode_call_index;
        uint8_t                                                   terminate_opcode_call_index;
        uint8_t                                                   local_hold_opcode_call_index;
        uint8_t                                                   local_retrieve_opcode_call_index;
        T_CCP_CLIENT_WRITE_CALL_CP_PARAM_ORIGINATE_OPCODE_PARAM   originate_opcode_param;
        T_CCP_CLIENT_WRITE_CALL_CP_PARAM_JOIN_OPCODE_PARAM        join_opcode_param;
    } param;
} T_CCP_CLIENT_WRITE_CALL_CP_PARAM;

/**
 * ccp_client.h
 *
 * \brief  Bearer Provider Name characteristic
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t     *p_bearer_provider_name;                 /**< UTF-8 string */
    uint16_t    bearer_provider_name_len;
} T_CCP_CLIENT_BEARER_PROVIDER_NAME;

/**
 * ccp_client.h
 *
 * \brief  Bearer Uniform Caller Identifier (UCI) characteristic
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t     *p_bearer_uci;                           /**<
                                                              * Identifier of the telephone bearer
                                                              *
                                                              * The "client_id" field of the UCI table is used to populate the Bearer UCI characteristic as a UTF-8 string. */
    uint16_t    bearer_uci_len;
} T_CCP_CLIENT_BEARER_UCI;

/**
 * ccp_client.h
 *
 * \brief  Bearer Technology characteristic
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t     *p_bearer_technology; /**<
                                           * Type of technology for telephone bearer: @ref TBS_BEARER_TECHNOLOGY_CHAR_VALUE */
    uint16_t    bearer_technology_len;
} T_CCP_CLIENT_BEARER_TECHNOLOGY;

/**
 * ccp_client.h
 *
 * \brief  Bearer URI Schemes Supported List characteristic
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t     *p_bearer_uri_schemes_supported_list;  /**<
                                                            * A comma-delimited list of supported URI schemes expressed as a UTF-8 string */
    uint16_t    bearer_uri_schemes_supported_list_len;
} T_CCP_CLIENT_BEARER_URI_SCHEMES_SUPPORTED_LIST;

/**
 * ccp_client.h
 *
 * \brief  Bearer List Current Calls characteristic
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t    *p_bearer_list_current_calls;         /**<
                                                          * A list of current calls
                                                          *
                                                          * Each list item in the list of calls shall have the structure
                                                          * \arg    List_Item_Length[i] (1 octet) : Total length of the item, excluding the octet for the List_Item_Length field itself
                                                          * \arg    Call_Index[i] (1 octet): A number chosen by Call Control Server to identify call
                                                          * \arg    Call_State[i] (1 octet): @ref T_TBS_CALL_STATE
                                                          * \arg    Call_Flags[i] (1 octet): A bit field, @ref TBS_CALL_FLAGS_BIT_Def
                                                          * \arg    Call_URI[i] (variable): Incoming Call URI or Outgoing Call URI */
    uint16_t    bearer_list_current_calls_len;       /**<
                                                          * \arg    0 : No calls are available
                                                          * \arg    other values */
} T_CCP_CLIENT_BEARER_LIST_CURRENT_CALLS;

/**
 * ccp_client.h
 *
 * \brief  Incoming Call Target Bearer URI characteristic
 *
 * \ingroup CCP_Client_Exported_Types
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
} T_CCP_CLIENT_INCOMING_CALL_TARGET_BEARER_URI;

/**
 * ccp_client.h
 *
 * \brief  Call State characteristic
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t    *p_call_state;                        /**<
                                                          * An array of members
                                                          *
                                                          * Each 3 octets, and represents an abstraction of the different states that telephone calls are in.
                                                          *
                                                          * Each member in the array shall have the structure
                                                          * \arg    Call_Index[i] (1 octet): A number chosen by Call Control Server to identify call
                                                          * \arg    State[i] (1 octet): @ref T_TBS_CALL_STATE
                                                          * \arg    Call_Flags[i] (1 octet): A bit field, @ref TBS_CALL_FLAGS_BIT_Def */
    uint16_t    call_state_len;                      /**<
                                                          * \arg    0 : No calls are available
                                                          * \arg    other values */
} T_CCP_CLIENT_CALL_STATE;

/**
 * ccp_client.h
 *
 * \brief  Termination Reason characteristic
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t     call_index;
    uint8_t     reason_code;              /**< @ref TBS_TERMINATION_REASON_CODES */
} T_CCP_CLIENT_TERMINATION_REASON;

/**
 * ccp_client.h
 *
 * \brief  Incoming Call characteristic
 *
 * \ingroup CCP_Client_Exported_Types
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
} T_CCP_CLIENT_INCOMING_CALL;

/**
 * ccp_client.h
 *
 * \brief  Call Friendly Name characteristic
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t     call_index;
    uint8_t    *p_friendly_name;             /**<
                                                  * Friendly name of the incoming call or outgoing call
                                                  *
                                                  * UTF-8 string */
    uint16_t    friendly_name_len;
} T_CCP_CLIENT_CALL_FRIENDLY_NAME;

/**
 * ccp_client.h
 *
 * \brief  Data for @ref LE_AUDIO_MSG_CCP_CLIENT_DIS_DONE
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint16_t    conn_handle;
    bool        is_found;        /**<
                                      * \arg    true  : Service is found
                                      * \arg    false : Service is not found */
    bool        load_from_ftl;   /**<
                                      * \arg    true  : Service table is loaded from FTL
                                      * \arg    false : Service table is not loaded from FTL */
    bool        gtbs;            /**<
                                      * \arg    true  : Generic Telephone Bearer Service (GTBS)
                                      * \arg    false : Telephone Bearer Service (TBS) */
    uint8_t     srv_num;         /**< Service instance number */
} T_CCP_CLIENT_DIS_DONE;

/**
 * ccp_client.h
 *
 * \brief  Data for @ref LE_AUDIO_MSG_CCP_CLIENT_READ_RESULT
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint16_t          conn_handle;
    bool              gtbs;
    uint8_t           srv_instance_id;
    uint16_t          char_uuid;
    uint16_t          cause;

    union
    {
        T_CCP_CLIENT_BEARER_PROVIDER_NAME                   bearer_provider_name;
        T_CCP_CLIENT_BEARER_UCI                             bearer_uci;
        T_CCP_CLIENT_BEARER_TECHNOLOGY                      bearer_technology;
        T_CCP_CLIENT_BEARER_URI_SCHEMES_SUPPORTED_LIST      bearer_uri_schemes_supported_list;
        uint8_t                                             bearer_signal_strength;                    /**<
                                                                                                            * Level of the signal of the telephone bearer
                                                                                                            *
                                                                                                            * \arg    0 : No service
                                                                                                            * \arg    1 to 99 : Meaning of the values is left up to the implementation
                                                                                                            * \arg    100 : Maximum signal strength
                                                                                                            * \arg    101 to 254: Reserved for future use
                                                                                                            * \arg    255 : Signal strength is unavailable or has no meaning for this particular bearer */
        uint8_t                                             bearer_signal_strength_reporting_interval; /**<
                                                                                                            * Reporting interval expressed in seconds
                                                                                                            *
                                                                                                            * \arg    0 : Signal strength is reported as soon as the signal strength changes
                                                                                                            * \arg    1 to 255 */
        T_CCP_CLIENT_BEARER_LIST_CURRENT_CALLS              bearer_list_current_calls;
        uint16_t                                            status_flags;                              /**<
                                                                                                            * Current status of features that Call Control Server device supports
                                                                                                            *
                                                                                                            * A bit field
                                                                                                            *
                                                                                                            * @ref TBS_STATUS_FLAGS_CHAR_BIT_Def */
        T_CCP_CLIENT_INCOMING_CALL_TARGET_BEARER_URI        incoming_call_target_bearer_uri;
        T_CCP_CLIENT_CALL_STATE                             call_state;
        uint16_t                                            call_control_point_optional_opcodes;       /**<
                                                                                                            * Optional opcodes of the Call Control Point
                                                                                                            *
                                                                                                            * A bit field
                                                                                                            *
                                                                                                            * @ref TBS_CALL_CONTROL_POINT_OPTIONAL_OPCODES_CHAR_BIT_Def */
        T_CCP_CLIENT_INCOMING_CALL                          incoming_call;
        T_CCP_CLIENT_CALL_FRIENDLY_NAME                     call_friendly_name;
        uint8_t                                             content_control_id;                        /**<
                                                                                                            * Represent a unique instance of a service that either controls
                                                                                                            * or provides status information on an audio-related feature */
    } data;
} T_CCP_CLIENT_READ_RESULT;

/**
 * ccp_client.h
 *
 * \brief  Data for @ref LE_AUDIO_MSG_CCP_CLIENT_NOTIFY
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint16_t          conn_handle;
    bool              gtbs;            /**<
                                            * \arg    true  : Generic Telephone Bearer Service (GTBS)
                                            * \arg    false : Telephone Bearer Service (TBS) */
    uint8_t           srv_instance_id;
    uint16_t          char_uuid;       /**<
                                            * \arg    @ref TBS_UUID_CHAR_BEARER_PROVIDER_NAME
                                            * \arg    @ref TBS_UUID_CHAR_BEARER_TECHNOLOGY
                                            * \arg    @ref TBS_UUID_CHAR_BEARER_URI_SCHEMES_SUPPORTED_LIST
                                            * \arg    @ref TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH
                                            * \arg    @ref TBS_UUID_CHAR_BEARER_LIST_CURRENT_CALLS
                                            * \arg    @ref TBS_UUID_CHAR_STATUS_FLAGS
                                            * \arg    @ref TBS_UUID_CHAR_INCOMING_CALL_TARGET_BEARER_URI
                                            * \arg    @ref TBS_UUID_CHAR_CALL_STATE
                                            * \arg    @ref TBS_UUID_CHAR_TERMINATION_REASON
                                            * \arg    @ref TBS_UUID_CHAR_INCOMING_CALL
                                            * \arg    @ref TBS_UUID_CHAR_CALL_FRIENDLY_NAME */

    union
    {
        T_CCP_CLIENT_BEARER_PROVIDER_NAME                   bearer_provider_name;
        T_CCP_CLIENT_BEARER_TECHNOLOGY                      bearer_technology;
        T_CCP_CLIENT_BEARER_URI_SCHEMES_SUPPORTED_LIST      bearer_uri_schemes_supported_list;
        uint8_t                                             bearer_signal_strength;
        T_CCP_CLIENT_BEARER_LIST_CURRENT_CALLS              bearer_list_current_calls;
        uint16_t                                            status_flags;
        T_CCP_CLIENT_INCOMING_CALL_TARGET_BEARER_URI        incoming_call_target_bearer_uri;
        T_CCP_CLIENT_CALL_STATE                             call_state;
        T_CCP_CLIENT_TERMINATION_REASON                     termination_reason;
        T_CCP_CLIENT_INCOMING_CALL                          incoming_call;
        T_CCP_CLIENT_CALL_FRIENDLY_NAME                     call_friendly_name;
    } data;
} T_CCP_CLIENT_NOTIFY;

/**
 * ccp_client.h
 *
 * \brief  Data for @ref LE_AUDIO_MSG_CCP_CLIENT_CALL_CP_NOTIFY
 *
 * \ingroup CCP_Client_Exported_Types
 */
typedef struct
{
    uint16_t    conn_handle;
    bool        gtbs;              /**<
                                        * \arg    true  : Generic Telephone Bearer Service (GTBS)
                                        * \arg    false : Telephone Bearer Service (TBS) */
    uint8_t     srv_instance_id;

    uint8_t     requested_opcode;  /**< @ref TBS_CALL_CONTROL_POINT_CHAR_OPCODE */
    uint8_t     call_index;
    uint8_t     result_code;       /**< @ref TBS_CALL_CONTROL_POINT_NOTIFICATION_RESULT_CODES */
} T_CCP_CLIENT_CALL_CP_NOTIFY;
/**
 * End of CCP_Client_Exported_Types
 * \}
 */

/**
 * \defgroup CCP_Client_Exported_Functions Call Control Client Exported Functions
 *
 * \ingroup LEA_GAF_CCP_Client
 * \{
 */

/**
 *
 * \brief  Configure CCCD
 *
 * ccp_client.h
 *
 * \param[in]  conn_handle      Connection handle
 * \param[in]  srv_instance_id  Service instance id
 * \param[in]  cfg_flags        @ref CCP_CLIENT_CFG_CCCD_FLAG
 * \param[in]  enable
 * \arg    true  : Enable CCCD
 * \arg    false : Disable CCCD
 * \param[in]  gtbs
 * \arg    true  : Generic Telephone Bearer Service (GTBS)
 * \arg    false : Telephone Bearer Service (TBS)
 *
 * \return void
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        ccp_client_cfg_cccd(conn_handle, srv_instance_id, cfg_flags, enable, gtbs);
    }
 * \endcode
 *
 * \ingroup CCP_Client_Exported_Functions
 */
void ccp_client_cfg_cccd(uint16_t conn_handle, uint8_t srv_instance_id, uint32_t cfg_flags,
                         bool enable, bool gtbs);

/**
 *
 * \brief  Read characteristic value
 *
 *         If sending operation is successful, the result of read characteristic value will be returned by
 *         callback registered by @ref ble_audio_cback_register with msg @ref LE_AUDIO_MSG_CCP_CLIENT_READ_RESULT
 *         and buf @ref T_CCP_CLIENT_READ_RESULT.
 *
 * ccp_client.h
 *
 * \param[in]  conn_handle      Connection handle
 * \param[in]  srv_instance_id  Service instance id
 * \param[in]  char_uuid
 * \arg    @ref TBS_UUID_CHAR_BEARER_PROVIDER_NAME
 * \arg    @ref TBS_UUID_CHAR_BEARER_UCI
 * \arg    @ref TBS_UUID_CHAR_BEARER_TECHNOLOGY
 * \arg    @ref TBS_UUID_CHAR_BEARER_URI_SCHEMES_SUPPORTED_LIST
 * \arg    @ref TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH
 * \arg    @ref TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH_REPORTING_INTERVAL
 * \arg    @ref TBS_UUID_CHAR_BEARER_LIST_CURRENT_CALLS
 * \arg    @ref TBS_UUID_CHAR_STATUS_FLAGS
 * \arg    @ref TBS_UUID_CHAR_INCOMING_CALL_TARGET_BEARER_URI
 * \arg    @ref TBS_UUID_CHAR_CALL_STATE
 * \arg    @ref TBS_UUID_CHAR_CALL_CONTROL_POINT_OPTIONAL_OPCODES
 * \arg    @ref TBS_UUID_CHAR_INCOMING_CALL
 * \arg    @ref TBS_UUID_CHAR_CALL_FRIENDLY_NAME
 * \arg    @ref TBS_UUID_CHAR_CONTENT_CONTROL_ID
 * \param[in]  gtbs
 * \arg    true  : Generic Telephone Bearer Service (GTBS)
 * \arg    false : Telephone Bearer Service (TBS)
 *
 * \return         The result of sending operation
 * \retval true    Sending operation is successful
 * \retval false   Sending operation is failed
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool retval = ccp_client_read_char_value(conn_handle, srv_instance_id, char_uuid, gtbs);
    }

    // Callback registered by @ref ble_audio_cback_register
    {
        switch (msg)
        {
        case LE_AUDIO_MSG_CCP_CLIENT_READ_RESULT:
            {
                ......
            }
            break;
        ......
    }
 * \endcode
 *
 * \ingroup CCP_Client_Exported_Functions
 */
bool ccp_client_read_char_value(uint16_t conn_handle, uint8_t srv_instance_id, uint16_t char_uuid,
                                bool gtbs);

/**
 *
 * \brief  Write Call Control Point characteristic
 *
 *         If sending operation is successful and Call Control Server sends Call Control Point Notification,
 *         Call Control Point Notification will be returned by callback registered by @ref ble_audio_cback_register
 *         with msg @ref LE_AUDIO_MSG_CCP_CLIENT_CALL_CP_NOTIFY and buf @ref T_CCP_CLIENT_CALL_CP_NOTIFY.
 *
 * ccp_client.h
 *
 * \param[in]  conn_handle      Connection handle
 * \param[in]  srv_instance_id  Service instance id
 * \param[in]  gtbs
 * \arg    true  : Generic Telephone Bearer Service (GTBS)
 * \arg    false : Telephone Bearer Service (TBS)
 * \param[in]  is_req
 * \arg    true  : Write request
 * \arg    false : Write command
 * \param[in]  p_param          Point to call control point parameter: @ref T_CCP_CLIENT_WRITE_CALL_CP_PARAM
 *
 * \return         The result of sending operation
 * \retval true    Sending operation is successful
 * \retval false   Sending operation is failed
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool retval = ccp_client_write_call_cp(conn_handle, srv_instance_id, gtbs, is_req, p_param);
    }

    // If sending operation is successful and Call Control Server sends Call Control Point Notification
    // Callback registered by @ref ble_audio_cback_register
    {
        switch (msg)
        {
        case LE_AUDIO_MSG_CCP_CLIENT_CALL_CP_NOTIFY:
            {
                ......
            }
            break;
        ......
    }
 * \endcode
 *
 * \ingroup CCP_Client_Exported_Functions
 */
bool ccp_client_write_call_cp(uint16_t conn_handle, uint8_t srv_instance_id, bool gtbs, bool is_req,
                              T_CCP_CLIENT_WRITE_CALL_CP_PARAM *p_param);
/**
 * End of CCP_Client_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif

#endif
