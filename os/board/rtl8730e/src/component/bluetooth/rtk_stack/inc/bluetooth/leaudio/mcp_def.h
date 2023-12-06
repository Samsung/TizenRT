/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mcp_def.h
  * @brief    Head file for Media Control Server and Media Control Client.
  * @details  This file defines Media Control Server and Media Control Client related definition.
  * @author
  * @date
  * @version
  * *************************************************************************************
  */

#ifndef _MCP_DEF_H_
#define _MCP_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * \defgroup    LEA_GAF_MCP_Def Media Control Profile definition
 *
 * \brief   Definition for Media Control Server and Media Control Client
 */

/**
 * \defgroup MCP_Def_Exported_Macros Media Control Profile Definition Exported Macros
 *
 * \ingroup LEA_GAF_MCP_Def
 * \{
 */

/**
 * mcp_def.h
 *
 * \defgroup MCP_Def_UUID_Service Service UUID
 *
 * \brief Service UUID of Generic Media Control Service (GMCS) or Media Control Service (MCS)
 *
 * \ingroup MCP_Def_Exported_Macros
 * \{
 */
#define GATT_UUID_MEDIA_CONTROL_SERVICE                              0x1848
#define GATT_UUID_GENERIC_MEDIA_CONTROL_SERVICE                      0x1849
/**
 * End of MCP_Def_UUID_Service
 * \}
 */

/**
 * mcp_def.h
 *
 * \defgroup MCS_UUID_CHAR Characteristic UUID
 *
 * \brief Characteristic UUID in Generic Media Control Service (GMCS) or Media Control Service (MCS)
 *
 * \ingroup MCP_Def_Exported_Macros
 * \{
 */
#define MCS_UUID_CHAR_MEDIA_PLAYER_NAME                              0x2B93
#define MCS_UUID_CHAR_TRACK_CHANGED                                  0x2B96
#define MCS_UUID_CHAR_TRACK_TITLE                                    0x2B97
#define MCS_UUID_CHAR_TRACK_DURATION                                 0x2B98
#define MCS_UUID_CHAR_TRACK_POSITION                                 0x2B99
#define MCS_UUID_CHAR_PLAYING_ORDERS_SUPPORTED                       0x2BA2
#define MCS_UUID_CHAR_MEDIA_STATE                                    0x2BA3
#define MCS_UUID_CHAR_MEDIA_CONTROL_POINT                            0x2BA4
#define MCS_UUID_CHAR_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED          0x2BA5
#define MCS_UUID_CHAR_CONTENT_CONTROL_ID                             0x2BBA
/**
 * End of MCS_UUID_CHAR
 * \}
 */

/**
 * mcp_def.h
 *
 * \defgroup MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE Media Control Point characteristic opcodes
 *
 * \brief Media Control Point characteristic opcodes in Generic Media Control Service (GMCS) or Media Control Service (MCS)
 *
 * \ingroup MCP_Def_Exported_Macros
 * \{
 */
#define MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_PLAY                     0x01
#define MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_PAUSE                    0x02
#define MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_FAST_REWIND              0x03
#define MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_FAST_FORWARD             0x04
#define MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_STOP                     0x05
#define MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_MOVE_RELATIVE            0x10
#define MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_PREVIOUS_TRACK           0x30
#define MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_NEXT_TRACK               0x31
#define MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_FIRST_TRACK              0x32
#define MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_LAST_TRACK               0x33
/**
 * End of MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE
 * \}
 */

/**
 * mcp_def.h
 *
 * \defgroup MCS_MEDIA_CONTROL_POINT_NOTIFICATION_RESULT_CODES Media Control Point Notification Result Codes
 *
 * \brief Media Control Point Notification Result Codes in Generic Media Control Service (GMCS) or Media Control Service (MCS)
 *
 * \ingroup MCP_Def_Exported_Macros
 * \{
 */
#define MCS_MEDIA_CONTROL_POINT_NOTIFICATION_RESULT_CODES_SUCCESS                       0x01
#define MCS_MEDIA_CONTROL_POINT_NOTIFICATION_RESULT_CODES_OPCODE_NOT_SUPPORTED          0x02
#define MCS_MEDIA_CONTROL_POINT_NOTIFICATION_RESULT_CODES_MEDIA_PLAYER_INACTIVE         0x03
#define MCS_MEDIA_CONTROL_POINT_NOTIFICATION_RESULT_CODES_COMMAND_CANNOT_BE_COMPLETED   0x04
/**
 * End of MCS_MEDIA_CONTROL_POINT_NOTIFICATION_RESULT_CODES
 * \}
 */

/**
 * mcp_def.h
 *
 * \defgroup MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE Media Control Point Opcodes Supported characteristic bit values
 *
 * \brief Media Control Point Opcodes Supported characteristic bit values in Generic Media Control Service (GMCS) or Media Control Service (MCS)
 *
 * \ingroup MCP_Def_Exported_Macros
 * \{
 */
#define MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_PLAY                     0x00000001
#define MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_PAUSE                    0x00000002
#define MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_FAST_REWIND              0x00000004
#define MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_FAST_FORWARD             0x00000008
#define MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_STOP                     0x00000010
#define MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_MOVE_RELATIVE            0x00000020
#define MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_PREVIOUS_TRACK           0x00000800
#define MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_NEXT_TRACK               0x00001000
#define MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_FIRST_TRACK              0x00002000
#define MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_LAST_TRACK               0x00004000
/**
 * End of MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE
 * \}
 */

/**
 * mcp_def.h
 *
 * \defgroup MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE Playing Orders Supported characteristic bit values
 *
 * \brief Playing Orders Supported characteristic bit values in Generic Media Control Service (GMCS) or Media Control Service (MCS)
 *
 * \ingroup MCP_Def_Exported_Macros
 * \{
 */
#define MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_SINGLE_ONCE                     0x0001
#define MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_SINGLE_REPEAT                   0x0002
#define MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_IN_ORDER_ONCE                   0x0004
#define MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_IN_ORDER_REPEAT                 0x0008
#define MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_OLDEST_ONCE                     0x0010
#define MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_OLDEST_REPEAT                   0x0020
#define MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_NEWEST_ONCE                     0x0040
#define MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_NEWEST_REPEAT                   0x0080
#define MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_SHUFFLE_ONCE                    0x0100
#define MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_SHUFFLE_REPEAT                  0x0200
/**
 * End of MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE
 * \}
 */

/**
 * mcp_def.h
 *
 * \defgroup MCS_TRACK_DURATION_CHAR_VALUE_UNKNOWN Track Duration characteristic value unknown
 *
 * \brief No current track or the duration of the current track is unknown in Generic Media Control Service (GMCS) or Media Control Service (MCS)
 *
 * \ingroup MCP_Def_Exported_Macros
 * \{
 */
#define MCS_TRACK_DURATION_CHAR_VALUE_UNKNOWN                                           0xFFFFFFFF
/**
 * End of MCS_TRACK_DURATION_CHAR_VALUE_UNKNOWN
 * \}
 */

/**
 * mcp_def.h
 *
 * \defgroup MCS_TRACK_POSITION_CHAR_VALUE_UNAVAILABLE Track Position characteristic value unavailable
 *
 * \brief No current track or the start of the track is not well defined in Generic Media Control Service (GMCS) or Media Control Service (MCS)
 *
 * \ingroup MCP_Def_Exported_Macros
 * \{
 */
#define MCS_TRACK_POSITION_CHAR_VALUE_UNAVAILABLE                                       0xFFFFFFFF
/**
 * End of MCS_TRACK_POSITION_CHAR_VALUE_UNAVAILABLE
 * \}
 */

/**
 * End of MCP_Def_Exported_Macros
 * \}
 */

/**
 * \defgroup MCP_Def_Exported_Types Media Control Profile Definition Exported Types
 *
 * \ingroup LEA_GAF_MCP_Def
 * \{
 */

/**
 * mcp_def.h
 *
 * \defgroup T_MCS_MEDIA_STATE Media State characteristic values
 *
 * \brief Media State characteristic values in Generic Media Control Service (GMCS) or Media Control Service (MCS)
 *
 * \ingroup MCP_Def_Exported_Types
 * \{
 */
typedef enum
{
    MCS_MEDIA_STATE_INACTIVE        = 0x00, /**< The current track is invalid, and no track has been selected. */
    MCS_MEDIA_STATE_PLAYING         = 0x01, /**< The media player is playing the current track. */
    MCS_MEDIA_STATE_PAUSED          = 0x02, /**< The current track is paused. The media player has a current track, but it is not being played. */
    MCS_MEDIA_STATE_SEEKING         = 0x03, /**< The current track is fast forwarding or fast rewinding. */
} T_MCS_MEDIA_STATE;
/**
 * End of T_MCS_MEDIA_STATE
 * \}
 */

/**
 * End of MCP_Def_Exported_Types
 * \}
 */

#ifdef  __cplusplus
}
#endif

#endif
