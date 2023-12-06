/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mcp_client.h
  * @brief    Head file for Media Control Client.
  * @details  This file defines Media Control Client related API.
  * @author
  * @date
  * @version
  * *************************************************************************************
  */

#ifndef _MCP_CLIENT_H_
#define _MCP_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "mcp_def.h"

/**
 * \defgroup    LEA_GAF_MCP_Client Media Control Client
 *
 * \brief Control and interact with media player
 */

/**
 * \defgroup MCP_Client_Exported_Macros Media Control Client Exported Macros
 *
 * \ingroup LEA_GAF_MCP_Client
 * \{
 */

/**
 * mcp_client.h
 *
 * \defgroup MCP_CLIENT_CFG_CCCD_FLAG MCP Client Configure CCCD Flag
 *
 * \brief    Use the combination of macro definitions to specify whether to configure CCCD
 *
 * \ingroup MCP_Client_Exported_Macros
 * \{
 */
#define MCP_CLIENT_CFG_CCCD_FLAG_MEDIA_PLAYER_NAME                       0x00000001
#define MCP_CLIENT_CFG_CCCD_FLAG_TRACK_CHANGED                           0x00000002
#define MCP_CLIENT_CFG_CCCD_FLAG_TRACK_TITLE                             0x00000004
#define MCP_CLIENT_CFG_CCCD_FLAG_TRACK_DURATION                          0x00000008
#define MCP_CLIENT_CFG_CCCD_FLAG_TRACK_POSITION                          0x00000010
#define MCP_CLIENT_CFG_CCCD_FLAG_MEDIA_STATE                             0x00000020
#define MCP_CLIENT_CFG_CCCD_FLAG_MEDIA_CONTROL_POINT                     0x00000040
#define MCP_CLIENT_CFG_CCCD_FLAG_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED   0x00000080
/**
 * End of MCP_CLIENT_CFG_CCCD_FLAG
 * \}
 */

/**
 * End of MCP_Client_Exported_Macros
 * \}
 */

/**
 * \defgroup MCP_Client_Exported_Types Media Control Client Exported Types
 *
 * \ingroup LEA_GAF_MCP_Client
 * \{
 */

/**
 * mcp_client.h
 *
 * \brief  Write Media Control Point characteristic parameter
 *
 * \ingroup MCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t       opcode;                          /**< @ref MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE */

    union
    {
        int32_t   move_relative_opcode_offset;
        int32_t   goto_segment_opcode_n;
        int32_t   goto_track_opcode_n;
        int32_t   goto_group_opcode_n;
    } param;
} T_MCP_CLIENT_WRITE_MEDIA_CP_PARAM;

/**
 * mcp_client.h
 *
 * \brief  Media Player Name characteristic
 *
 * \ingroup MCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t       *p_media_player_name;     /**< UTF-8 string */
    uint16_t      media_player_name_len;
} T_MCP_CLIENT_MEDIA_PLAYER_NAME;

/**
 * mcp_client.h
 *
 * \brief  Track Title characteristic
 *
 * \ingroup MCP_Client_Exported_Types
 */
typedef struct
{
    uint8_t       *p_track_title;           /**< UTF-8 string */
    uint16_t      track_title_len;
} T_MCP_CLIENT_TRACK_TITLE;

/**
 * mcp_client.h
 *
 * \brief  Data for @ref LE_AUDIO_MSG_MCP_CLIENT_DIS_DONE
 *
 * \ingroup MCP_Client_Exported_Types
 */
typedef struct
{
    uint16_t      conn_handle;
    bool          is_found;      /**<
                                      * \arg    true  : Service is found
                                      * \arg    false : Service is not found */
    bool          load_from_ftl; /**<
                                      * \arg    true  : Service table is loaded from FTL
                                      * \arg    false : Service table is not loaded from FTL */
    bool          gmcs;          /**<
                                      * \arg    true  : Generic Media Control Service (GMCS)
                                      * \arg    false : Media Control Service (MCS) */
    uint8_t       srv_num;       /**< Service instance number */
} T_MCP_CLIENT_DIS_DONE;

/**
 * mcp_client.h
 *
 * \brief  Data for @ref LE_AUDIO_MSG_MCP_CLIENT_READ_RESULT
 *
 * \ingroup MCP_Client_Exported_Types
 */
typedef struct
{
    uint16_t      conn_handle;
    bool          gmcs;
    uint8_t       srv_instance_id;
    uint16_t      char_uuid;
    uint16_t      cause;

    union
    {
        T_MCP_CLIENT_MEDIA_PLAYER_NAME       media_player_name;
        T_MCP_CLIENT_TRACK_TITLE             track_title;
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
        uint8_t                              media_state;          /**< @ref T_MCS_MEDIA_STATE */
        uint8_t                              content_control_id;   /**<
                                                                        * Represent a unique instance of a service that either controls
                                                                        * or provides status information on an audio-related feature */
        uint16_t                             playing_orders_supported; /**<
                                                                           * Combination of @ref MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE */
        uint32_t                             media_control_point_opcodes_supported; /**<
                                                                                         * Combination of @ref MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE */
    } data;
} T_MCP_CLIENT_READ_RESULT;

/**
 * mcp_client.h
 *
 * \brief  Data for @ref LE_AUDIO_MSG_MCP_CLIENT_NOTIFY
 *
 * \ingroup MCP_Client_Exported_Types
 */
typedef struct
{
    uint16_t      conn_handle;
    bool          gmcs;            /**<
                                      * \arg    true  : Generic Media Control Service (GMCS)
                                      * \arg    false : Media Control Service (MCS) */
    uint8_t       srv_instance_id;
    uint16_t      char_uuid;       /**<
                                      * \arg    @ref MCS_UUID_CHAR_MEDIA_PLAYER_NAME
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_CHANGED
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_TITLE
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_DURATION
                                      * \arg    @ref MCS_UUID_CHAR_TRACK_POSITION
                                      * \arg    @ref MCS_UUID_CHAR_MEDIA_STATE
                                      * \arg    @ref MCS_UUID_CHAR_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED */

    union
    {
        T_MCP_CLIENT_MEDIA_PLAYER_NAME       media_player_name;
        T_MCP_CLIENT_TRACK_TITLE             track_title;
        int32_t                              track_duration;
        int32_t                              track_position;
        int8_t                               playback_speed;
        int8_t                               seeking_speed;
        uint8_t                              playing_order;
        uint8_t                              media_state;        /**< @ref T_MCS_MEDIA_STATE */
        uint32_t                             media_control_point_opcodes_supported;
    } data;
} T_MCP_CLIENT_NOTIFY;

/**
 * mcp_client.h
 *
 * \brief  Data for @ref LE_AUDIO_MSG_MCP_CLIENT_MEDIA_CP_NOTIFY
 *
 * \ingroup MCP_Client_Exported_Types
 */
typedef struct
{
    uint16_t    conn_handle;
    bool        gmcs;              /**<
                                      * \arg    true  : Generic Media Control Service (GMCS)
                                      * \arg    false : Media Control Service (MCS) */
    uint8_t     srv_instance_id;

    uint8_t     requested_opcode;  /**< @ref MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE */
    uint8_t     result_code;       /**< @ref MCS_MEDIA_CONTROL_POINT_NOTIFICATION_RESULT_CODES */
} T_MCP_CLIENT_MEDIA_CP_NOTIFY;
/**
 * End of MCP_Client_Exported_Types
 * \}
 */

/**
 * \defgroup MCP_Client_Exported_Functions Media Control Client Exported Functions
 *
 * \ingroup LEA_GAF_MCP_Client
 * \{
 */

/**
 *
 * \brief  Configure CCCD
 *
 * mcp_client.h
 *
 * \param[in]  conn_handle      Connection handle
 * \param[in]  cfg_flags        @ref MCP_CLIENT_CFG_CCCD_FLAG
 * \param[in]  gmcs
 * \arg    true  : Generic Media Control Service (GMCS)
 * \arg    false : Media Control Service (MCS)
 * \param[in]  srv_instance_id  Service instance id
 * \param[in]  enable
 * \arg    true  : Enable CCCD
 * \arg    false : Disable CCCD
 *
 * \return void
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        mcp_client_cfg_cccd(conn_handle, cfg_flags, gmcs, srv_instance_id, enable);
    }
 * \endcode
 *
 * \ingroup MCP_Client_Exported_Functions
 */
void mcp_client_cfg_cccd(uint16_t conn_handle, uint32_t cfg_flags, bool gmcs,
                         uint8_t srv_instance_id, bool enable);

/**
 *
 * \brief  Read characteristic value
 *
 *         If sending operation is successful, the result of read characteristic value will be returned by
 *         callback registered by @ref ble_audio_cback_register with msg @ref LE_AUDIO_MSG_MCP_CLIENT_READ_RESULT
 *         and buf @ref T_MCP_CLIENT_READ_RESULT.
 *
 * mcp_client.h
 *
 * \param[in]  conn_handle      Connection handle
 * \param[in]  srv_instance_id  Service instance id
 * \param[in]  char_uuid
 * \arg    @ref MCS_UUID_CHAR_MEDIA_PLAYER_NAME
 * \arg    @ref MCS_UUID_CHAR_TRACK_TITLE
 * \arg    @ref MCS_UUID_CHAR_TRACK_DURATION
 * \arg    @ref MCS_UUID_CHAR_TRACK_POSITION
 * \arg    @ref MCS_UUID_CHAR_MEDIA_STATE
 * \arg    @ref MCS_UUID_CHAR_CONTENT_CONTROL_ID
 * \arg    @ref MCS_UUID_CHAR_PLAYING_ORDERS_SUPPORTED
 * \arg    @ref MCS_UUID_CHAR_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED
 * \param[in]  gmcs
 * \arg    true  : Generic Media Control Service (GMCS)
 * \arg    false : Media Control Service (MCS)
 *
 * \return         The result of sending operation
 * \retval true    Sending operation is successful
 * \retval false   Sending operation is failed
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool retval = mcp_client_read_char_value(conn_handle, srv_instance_id, char_uuid, gmcs);
    }

    // Callback registered by @ref ble_audio_cback_register
    {
        switch (msg)
        {
        case LE_AUDIO_MSG_MCP_CLIENT_READ_RESULT:
            {
                ......
            }
            break;
        ......
    }
 * \endcode
 *
 * \ingroup MCP_Client_Exported_Functions
 */
bool mcp_client_read_char_value(uint16_t conn_handle, uint8_t srv_instance_id, uint16_t char_uuid,
                                bool gmcs);

/**
 *
 * \brief  Write Media Control Point characteristic
 *
 *         If sending operation is successful and Media Control Server sends Media Control Point Notification,
 *         Media Control Point Notification will be returned by callback registered by @ref ble_audio_cback_register
 *         with msg @ref LE_AUDIO_MSG_MCP_CLIENT_MEDIA_CP_NOTIFY and buf @ref T_MCP_CLIENT_MEDIA_CP_NOTIFY.
 *
 * mcp_client.h
 *
 * \param[in]  conn_handle      Connection handle
 * \param[in]  srv_instance_id  Service instance id
 * \param[in]  gmcs
 * \arg    true  : Generic Media Control Service (GMCS)
 * \arg    false : Media Control Service (MCS)
 * \param[in]  p_param          Point to media control point parameter: @ref T_MCP_CLIENT_WRITE_MEDIA_CP_PARAM
 * \param[in]  is_req
 * \arg    true  : Write request
 * \arg    false : Write command
 *
 * \return         The result of sending operation
 * \retval true    Sending operation is successful
 * \retval false   Sending operation is failed
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool retval = mcp_client_write_media_cp(conn_handle, srv_instance_id, gmcs, p_param, is_req);
    }

    // If sending operation is successful and Media Control Server sends Media Control Point Notification
    // Callback registered by @ref ble_audio_cback_register
    {
        switch (msg)
        {
        case LE_AUDIO_MSG_MCP_CLIENT_MEDIA_CP_NOTIFY:
            {
                ......
            }
            break;
        ......
    }
 * \endcode
 *
 * \ingroup MCP_Client_Exported_Functions
 */
bool mcp_client_write_media_cp(uint16_t conn_handle, uint8_t srv_instance_id, bool gmcs,
                               T_MCP_CLIENT_WRITE_MEDIA_CP_PARAM *p_param, bool is_req);
/**
 * End of MCP_Client_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif

#endif
