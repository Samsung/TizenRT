/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mcp_mgr.h
  * @brief    Head file for Media Control Server.
  * @details  This file defines Media Control Server related API.
  * @author
  * @date
  * @version
  * *************************************************************************************
  */

#ifndef _MCP_MGR_H_
#define _MCP_MGR_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "mcp_def.h"

/**
 * \defgroup    LEA_GAF_MCP_Server Media Control Server
 *
 * \brief Provide status and control of media player
 */

/**
 * \defgroup MCP_Server_Exported_Types Media Control Server Exported Types
 *
 * \ingroup LEA_GAF_MCP_Server
 * \{
 */

/**
 * mcp_mgr.h
 *
 * \brief  Register service parameter
 *
 * \ingroup MCP_Server_Exported_Types
 */
typedef struct
{
    bool gmcs;                    /**<
                                       * \arg    true  : Generic Media Control Service (GMCS)
                                       * \arg    false : Media Control Service (MCS) */

    struct
    {
        bool support;             /**<
                                       * \arg    true  : Support characteristic
                                       * \arg    false : Not support characteristic */
        bool char_media_control_point_opcodes_supported_optional_property_notify; /**<
                                                                                       * \arg    true  : Support Notify property of Media Control Point Opcodes Supported characteristic
                                                                                       * \arg    false : Not support Notify property of Media Control Point Opcodes Supported characteristic */
    } char_media_control_point;   /**< Media Control Point characteristic */

    struct
    {
        bool optional_property_notify; /**<
                                            * \arg    true  : Support Notify property
                                            * \arg    false : Not support Notify property */
    } char_media_player_name;

    struct
    {
        bool optional_property_notify;
    } char_track_title;

    struct
    {
        bool optional_property_notify;
    } char_track_duration;

    struct
    {
        bool optional_property_notify;
    } char_track_position;

    struct
    {
        bool support;
        bool optional_property_notify;
    } char_playing_order;

    struct
    {
        bool support;
    } char_playing_orders_supported;

    struct
    {
        bool support;
        bool optional_property_notify;
    } char_playback_speed;

    struct
    {
        bool support;
        bool optional_property_notify;
    } char_seeking_speed;

    struct
    {
        bool support;
    } char_media_player_icon_url;
} T_MCP_SERVER_REG_SRV_PARAM;

/**
 * mcp_mgr.h
 *
 * \brief  Media Player Name characteristic
 *
 * \ingroup MCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t       *p_media_player_name;     /**< UTF-8 string */
    uint16_t      media_player_name_len;
} T_MCP_SERVER_MEDIA_PLAYER_NAME;

/**
 * mcp_mgr.h
 *
 * \brief  Media Player Icon URL characteristic
 *
 * \ingroup MCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t       *p_media_player_icon_url;     /**< UTF-8 string */
    uint16_t      media_player_icon_url_len;
} T_MCP_SERVER_MEDIA_PLAYER_ICON_URL;

/**
 * mcp_mgr.h
 *
 * \brief  Track Title characteristic
 *
 * \ingroup MCP_Server_Exported_Types
 */
typedef struct
{
    uint8_t       *p_track_title;     /**< UTF-8 string */
    uint16_t      track_title_len;
} T_MCP_SERVER_TRACK_TITLE;

/**
 * mcp_mgr.h
 *
 * \brief  Set parameter
 *
 * \ingroup MCP_Server_Exported_Types
 */
typedef struct
{
    uint16_t     char_uuid;        /**<
                                      * \arg    @ref MCS_UUID_CHAR_MEDIA_STATE : Set parameter and send notification
                                      * \arg    @ref MCS_UUID_CHAR_PLAYING_ORDERS_SUPPORTED : Set parameter
                                      * \arg    @ref MCS_UUID_CHAR_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED : Set parameter and send notification */

    union
    {
        uint8_t                                         media_state;  /**< @ref T_MCS_MEDIA_STATE */
        uint16_t                                        playing_orders_supported; /**<
                                                                                      * Combination of @ref MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE */
        uint32_t                                        media_control_point_opcodes_supported; /**<
                                                                                                   * Combination of @ref MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE */
    } param;
} T_MCP_SERVER_SET_PARAM;

/**
 * mcp_mgr.h
 *
 * \brief  Send data parameter
 *
 * \ingroup MCP_Server_Exported_Types
 */
typedef struct
{
    uint16_t     char_uuid;        /**<
                                      * \arg    @ref MCS_UUID_CHAR_MEDIA_PLAYER_NAME : Send notification
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_CHANGED : Send notification
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_TITLE : Send notification
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_DURATION : Send notification
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_POSITION : Send notification */

    union
    {
        T_MCP_SERVER_MEDIA_PLAYER_NAME       media_player_name;
        T_MCP_SERVER_TRACK_TITLE             track_title;
        int32_t                              track_duration;       /**<
                                                                        * Length of the current track in 0.01-second resolution
                                                                        * \arg    zero or greater
                                                                        * \arg    @ref MCS_TRACK_DURATION_CHAR_VALUE_UNKNOWN : No current
                                                                        *         track or the duration of the current track is unknown */
        int32_t                              track_position;       /**<
                                                                        * Current track position of the current track in 0.01-second resolution
                                                                        *
                                                                        * Offset from the start of the track to the current playing position
                                                                        * \arg    0: Starting position or start of the track is not well defined
                                                                        * \arg    @ref MCS_TRACK_POSITION_CHAR_VALUE_UNAVAILABLE : No current
                                                                        *         track or the start of the track is not well defined
                                                                        * \arg    other values */
        int8_t                               playback_speed;
        int8_t                               seeking_speed;
        uint8_t                              playing_order;
    } param;
} T_MCP_SERVER_SEND_DATA_PARAM;

/**
 * mcp_mgr.h
 *
 * \brief  Data for @ref LE_AUDIO_MSG_MCP_SERVER_WRITE_MEDIA_CP_IND
 *
 * \ingroup MCP_Server_Exported_Types
 */
typedef struct
{
    uint16_t        conn_handle;
    uint16_t        cid;                   /**< Channel Identifier assigned by Bluetooth stack */
    uint8_t         service_id;            /**< Service ID */

    uint8_t         opcode;                /**< @ref MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE */

    union
    {
        int32_t   move_relative_opcode_offset;
        int32_t   goto_segment_opcode_n;
        int32_t   goto_track_opcode_n;
        int32_t   goto_group_opcode_n;
    } param;
} T_MCP_SERVER_WRITE_MEDIA_CP_IND;

/**
 * mcp_mgr.h
 *
 * \brief  Data for @ref LE_AUDIO_MSG_MCP_SERVER_READ_IND
 *
 * \ingroup MCP_Server_Exported_Types
 */
typedef struct
{
    uint16_t  conn_handle;
    uint16_t  cid;
    uint8_t   service_id;
    uint16_t  char_uuid;         /**<
                                      * \arg    @ref MCS_UUID_CHAR_PLAYING_ORDERS_SUPPORTED
                                      * \arg    @ref MCS_UUID_CHAR_MEDIA_STATE
                                      * \arg    @ref MCS_UUID_CHAR_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED
                                      *
                                      * \arg    @ref MCS_UUID_CHAR_MEDIA_PLAYER_NAME
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_TITLE
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_DURATION
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_POSITION
                                      * \arg    @ref MCS_UUID_CHAR_CONTENT_CONTROL_ID */

    uint16_t  offset;
} T_MCP_SERVER_READ_IND;

/**
 * mcp_mgr.h
 *
 * \brief  Confirmation of read
 *
 * \ingroup MCP_Server_Exported_Types
 */
typedef struct
{
    uint16_t  cause;
    uint16_t  conn_handle;
    uint16_t  cid;
    uint8_t   service_id;
    uint16_t  char_uuid;         /**<
                                      * \arg    @ref MCS_UUID_CHAR_MEDIA_PLAYER_NAME
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_TITLE
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_DURATION
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_POSITION
                                      * \arg    @ref MCS_UUID_CHAR_CONTENT_CONTROL_ID */
    uint16_t  offset;

    union
    {
        T_MCP_SERVER_MEDIA_PLAYER_NAME       media_player_name;
        T_MCP_SERVER_MEDIA_PLAYER_ICON_URL   media_player_icon_url;
        T_MCP_SERVER_TRACK_TITLE             track_title;
        int32_t                              track_duration;
        int32_t                              track_position;
        int8_t                               playback_speed;
        int8_t                               seeking_speed;
        uint8_t                              playing_order;
        uint8_t                              content_control_id;   /**<
                                                                        * Represent a unique instance of a service that either controls
                                                                        * or provides status information on an audio-related feature */
    } param;
} T_MCP_SERVER_READ_CFM;
/**
 * End of MCP_Server_Exported_Types
 * \}
 */

/**
 * \defgroup MCP_Server_Exported_Functions Media Control Server Exported Functions
 *
 * \ingroup LEA_GAF_MCP_Server
 * \{
 */

/**
 *
 * \brief  Register service
 *
 * mcp_mgr.h
 *
 * \param[in]  p_param          Point to service registration parameter: @ref T_MCP_SERVER_REG_SRV_PARAM
 *
 * \return         Service ID of service registration operation
 * \retval 0xFF    Service registration operation is failed
 * \retval Others  Service ID of specific service. Service registration operation is successful
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t service_id = mcp_server_reg_srv(p_param);
    }
 * \endcode
 *
 * \ingroup MCP_Server_Exported_Functions
 */
uint8_t mcp_server_reg_srv(T_MCP_SERVER_REG_SRV_PARAM *p_param);

/**
 *
 * \brief  Set parameter
 *
 * mcp_mgr.h
 *
 * \param[in]  service_id       Service ID
 * \param[in]  p_param          Point to parameter: @ref T_MCP_SERVER_SET_PARAM
 *
 * \return         The result of set parameter operation
 * \retval true    Set parameter operation is successful
 * \retval false   Set parameter operation is failed
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool retval = mcp_server_set_param(service_id, p_param);
    }
 * \endcode
 *
 * \ingroup MCP_Server_Exported_Functions
 */
bool mcp_server_set_param(uint8_t service_id, T_MCP_SERVER_SET_PARAM *p_param);

/**
 *
 * \brief  Send data
 *
 * mcp_mgr.h
 *
 * \param[in]  service_id       Service ID
 * \param[in]  p_param          Point to parameter: @ref T_MCP_SERVER_SEND_DATA_PARAM
 *
 * \return         The result of sending request
 * \retval true    Sending request is successful
 * \retval false   Sending request is failed
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool retval = mcp_server_send_data(service_id, p_param);
    }
 * \endcode
 *
 * \ingroup MCP_Server_Exported_Functions
 */
bool mcp_server_send_data(uint8_t service_id, T_MCP_SERVER_SEND_DATA_PARAM *p_param);

/**
 *
 * \brief  Confirmation of read request sent by client
 *
 *         This API should be used if all of the following conditions are true:
 *            \arg char_uuid of @ref T_MCP_SERVER_READ_IND is @ref MCS_UUID_CHAR_MEDIA_PLAYER_NAME , @ref MCS_UUID_CHAR_TRACK_TITLE ,
 *                 @ref MCS_UUID_CHAR_TRACK_DURATION , @ref MCS_UUID_CHAR_TRACK_POSITION , or @ref MCS_UUID_CHAR_CONTENT_CONTROL_ID
 *            \arg return value of callback registered by @ref ble_audio_cback_register with msg @ref LE_AUDIO_MSG_MCP_SERVER_READ_IND
 *                 is @ref BLE_AUDIO_CB_RESULT_PENDING
 *
 * mcp_mgr.h
 *
 * \param[in]  p_read_cfm          Point to parameter: @ref T_MCP_SERVER_READ_CFM
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
        case LE_AUDIO_MSG_MCP_SERVER_READ_IND:
            {
                T_MCP_SERVER_READ_IND *p_read_ind = (T_MCP_SERVER_READ_IND *)buf;

                if (p_read_ind)
                {
                    T_MCP_SERVER_READ_CFM read_cfm = {0};

                    read_cfm.cause = BLE_AUDIO_CB_RESULT_SUCCESS;
                    read_cfm.conn_handle = p_read_ind->conn_handle;
                    read_cfm.cid = p_read_ind->cid;
                    read_cfm.service_id = p_read_ind->service_id;
                    read_cfm.char_uuid = p_read_ind->char_uuid;
                    read_cfm.offset = p_read_ind->offset;

                    switch (p_read_ind->char_uuid)
                    {
                    case MCS_UUID_CHAR_MEDIA_PLAYER_NAME:
                        {
                            read_cfm.param.media_player_name.p_media_player_name = ;
                            read_cfm.param.media_player_name.media_player_name_len = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            mcp_server_read_cfm(&read_cfm);
                        }
                        break;

                    case MCS_UUID_CHAR_TRACK_TITLE:
                        {
                            read_cfm.param.track_title.p_track_title = ;
                            read_cfm.param.track_title.track_title_len = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            mcp_server_read_cfm(&read_cfm);
                        }
                        break;

                    case MCS_UUID_CHAR_TRACK_DURATION:
                        {
                            read_cfm.param.track_duration = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            mcp_server_read_cfm(&read_cfm);
                        }
                        break;

                    case MCS_UUID_CHAR_TRACK_POSITION:
                        {
                            read_cfm.param.track_position = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            mcp_server_read_cfm(&read_cfm);
                        }
                        break;

                    case MCS_UUID_CHAR_CONTENT_CONTROL_ID:
                        {
                            read_cfm.param.content_control_id = ;

                            cb_result = BLE_AUDIO_CB_RESULT_PENDING;

                            mcp_server_read_cfm(&read_cfm);
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
 * \ingroup MCP_Server_Exported_Functions
 */
bool mcp_server_read_cfm(T_MCP_SERVER_READ_CFM *p_read_cfm);
/**
 * End of MCP_Server_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif

#endif
