/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ccp_def.h
  * @brief    Head file for Call Control Server and Call Control Client.
  * @details  This file defines Call Control Server and Call Control Client related definition.
  * @author
  * @date
  * @version
  * *************************************************************************************
  */

#ifndef _CCP_DEF_H_
#define _CCP_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * \defgroup    LEA_GAF_CCP_Def Call Control Profile definition
 *
 * \brief   Definition for Call Control Server and Call Control Client
 */

/**
 * \defgroup CCP_Def_Exported_Macros Call Control Profile Definition Exported Macros
 *
 * \ingroup LEA_GAF_CCP_Def
 * \{
 */

/**
 * ccp_def.h
 *
 * \defgroup CCP_Def_UUID_Service Service UUID
 *
 * \brief Service UUID of Generic Telephone Bearer Service (GTBS) or Telephone Bearer Service (TBS)
 *
 * \ingroup CCP_Def_Exported_Macros
 * \{
 */
#define GATT_UUID_TELEPHONE_BEARER_SERVICE                                         0x184B
#define GATT_UUID_GENERIC_TELEPHONE_BEARER_SERVICE                                 0x184C
/**
 * End of CCP_Def_UUID_Service
 * \}
 */

/**
 * ccp_def.h
 *
 * \defgroup TBS_UUID_CHAR Characteristic UUID
 *
 * \brief Characteristic UUID in Generic Telephone Bearer Service (GTBS) or Telephone Bearer Service (TBS)
 *
 * \ingroup CCP_Def_Exported_Macros
 * \{
 */
#define TBS_UUID_CHAR_BEARER_PROVIDER_NAME                                         0x2BB3
#define TBS_UUID_CHAR_BEARER_UCI                                                   0x2BB4
#define TBS_UUID_CHAR_BEARER_TECHNOLOGY                                            0x2BB5
#define TBS_UUID_CHAR_BEARER_URI_SCHEMES_SUPPORTED_LIST                            0x2BB6
#define TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH                                       0x2BB7
#define TBS_UUID_CHAR_BEARER_SIGNAL_STRENGTH_REPORTING_INTERVAL                    0x2BB8
#define TBS_UUID_CHAR_BEARER_LIST_CURRENT_CALLS                                    0x2BB9
#define TBS_UUID_CHAR_CONTENT_CONTROL_ID                                           0x2BBA
#define TBS_UUID_CHAR_STATUS_FLAGS                                                 0x2BBB
#define TBS_UUID_CHAR_INCOMING_CALL_TARGET_BEARER_URI                              0x2BBC
#define TBS_UUID_CHAR_CALL_STATE                                                   0x2BBD
#define TBS_UUID_CHAR_CALL_CONTROL_POINT                                           0x2BBE
#define TBS_UUID_CHAR_CALL_CONTROL_POINT_OPTIONAL_OPCODES                          0x2BBF
#define TBS_UUID_CHAR_TERMINATION_REASON                                           0x2BC0
#define TBS_UUID_CHAR_INCOMING_CALL                                                0x2BC1
#define TBS_UUID_CHAR_CALL_FRIENDLY_NAME                                           0x2BC2
/**
 * End of TBS_UUID_CHAR
 * \}
 */

/**
 * ccp_def.h
 *
 * \defgroup TBS_BEARER_TECHNOLOGY_CHAR_VALUE Bearer Technology characteristic values
 *
 * \brief Bearer Technology characteristic values in Generic Telephone Bearer Service (GTBS) or Telephone Bearer Service (TBS)
 *
 * \ingroup CCP_Def_Exported_Macros
 * \{
 */
#define TBS_BEARER_TECHNOLOGY_CHAR_VALUE_3G                                        0x01
#define TBS_BEARER_TECHNOLOGY_CHAR_VALUE_4G                                        0x02
#define TBS_BEARER_TECHNOLOGY_CHAR_VALUE_LTE                                       0x03
#define TBS_BEARER_TECHNOLOGY_CHAR_VALUE_WIFI                                      0x04
#define TBS_BEARER_TECHNOLOGY_CHAR_VALUE_5G                                        0x05
#define TBS_BEARER_TECHNOLOGY_CHAR_VALUE_GSM                                       0x06
#define TBS_BEARER_TECHNOLOGY_CHAR_VALUE_CDMA                                      0x07
#define TBS_BEARER_TECHNOLOGY_CHAR_VALUE_2G                                        0x08
#define TBS_BEARER_TECHNOLOGY_CHAR_VALUE_WCDMA                                     0x09
/**
 * End of TBS_BEARER_TECHNOLOGY_CHAR_VALUE
 * \}
 */

/**
 * ccp_def.h
 *
 * \defgroup TBS_CALL_CONTROL_POINT_CHAR_OPCODE Call Control Point characteristic opcodes
 *
 * \brief Call Control Point characteristic opcodes in Generic Telephone Bearer Service (GTBS) or Telephone Bearer Service (TBS)
 *
 * \ingroup CCP_Def_Exported_Macros
 * \{
 */
#define TBS_CALL_CONTROL_POINT_CHAR_OPCODE_ACCEPT                                  0x00
#define TBS_CALL_CONTROL_POINT_CHAR_OPCODE_TERMINATE                               0x01
#define TBS_CALL_CONTROL_POINT_CHAR_OPCODE_LOCAL_HOLD                              0x02
#define TBS_CALL_CONTROL_POINT_CHAR_OPCODE_LOCAL_RETRIEVE                          0x03
#define TBS_CALL_CONTROL_POINT_CHAR_OPCODE_ORIGINATE                               0x04
#define TBS_CALL_CONTROL_POINT_CHAR_OPCODE_JOIN                                    0x05
/**
 * End of TBS_CALL_CONTROL_POINT_CHAR_OPCODE
 * \}
 */

/**
 * ccp_def.h
 *
 * \defgroup TBS_CALL_CONTROL_POINT_NOTIFICATION_RESULT_CODES Call Control Point Notification Result Codes
 *
 * \brief Call Control Point Notification Result Codes in Generic Telephone Bearer Service (GTBS) or Telephone Bearer Service (TBS)
 *
 * \ingroup CCP_Def_Exported_Macros
 * \{
 */
#define TBS_CALL_CONTROL_POINT_NOTIFICATION_RESULT_CODES_SUCCESS                   0x00
#define TBS_CALL_CONTROL_POINT_NOTIFICATION_RESULT_CODES_OPCODE_NOT_SUPPORTED      0x01
#define TBS_CALL_CONTROL_POINT_NOTIFICATION_RESULT_CODES_OPERATION_NOT_POSSIBLE    0x02
#define TBS_CALL_CONTROL_POINT_NOTIFICATION_RESULT_CODES_INVALID_CALL_INDEX        0x03
#define TBS_CALL_CONTROL_POINT_NOTIFICATION_RESULT_CODES_STATE_MISMATCH            0x04
#define TBS_CALL_CONTROL_POINT_NOTIFICATION_RESULT_CODES_LACK_OF_RESOURCES         0x05
#define TBS_CALL_CONTROL_POINT_NOTIFICATION_RESULT_CODES_INVALID_OUTGOING_URI      0x06
/**
 * End of TBS_CALL_CONTROL_POINT_NOTIFICATION_RESULT_CODES
 * \}
 */

/**
 * ccp_def.h
 *
 * \defgroup TBS_TERMINATION_REASON_CODES Termination Reason Codes
 *
 * \brief Termination Reason Codes in Generic Telephone Bearer Service (GTBS) or Telephone Bearer Service (TBS)
 *
 * \ingroup CCP_Def_Exported_Macros
 * \{
 */
#define TBS_TERMINATION_REASON_CODES_ORIGINATE_URI_FORMED_IMPROPERLY               0x00
#define TBS_TERMINATION_REASON_CODES_CALL_FAIL                                     0x01
#define TBS_TERMINATION_REASON_CODES_REMOTE_END_CALL                               0x02
#define TBS_TERMINATION_REASON_CODES_SERVER_END_CALL                               0x03
#define TBS_TERMINATION_REASON_CODES_LINE_BUSY                                     0x04
#define TBS_TERMINATION_REASON_CODES_NETWORK_CONGESTION                            0x05
#define TBS_TERMINATION_REASON_CODES_CLIENT_TERMINATE_CALL                         0x06
#define TBS_TERMINATION_REASON_CODES_NO_SERVICE                                    0x07
#define TBS_TERMINATION_REASON_CODES_NO_ANSWER                                     0x08
#define TBS_TERMINATION_REASON_CODES_UNSPECIFIED                                   0x09
/**
 * End of TBS_TERMINATION_REASON_CODES
 * \}
 */

/**
 * ccp_def.h
 *
 * \defgroup TBS_STATUS_FLAGS_CHAR_BIT_Def Status Flags characteristic bit definitions
 *
 * \brief Status Flags characteristic bit definitions in Generic Telephone Bearer Service (GTBS) or Telephone Bearer Service (TBS)
 *
 * \ingroup CCP_Def_Exported_Macros
 * \{
 */
#define TBS_STATUS_FLAGS_CHAR_BIT_INBAND_RINGTONE                                  0  /**< Bit 0: Inband ringtone
                                                                                           * \arg 0 : Inband ringtone disabled
                                                                                           * \arg 1 : Inband ringtone enabled */
#define TBS_STATUS_FLAGS_CHAR_BIT_SILENT_MODE                                      1  /**< Bit 1: Silent mode
                                                                                           * \arg 0: Server is not in silent mode
                                                                                           * \arg 1: Server is in silent mode */
/**
 * End of TBS_STATUS_FLAGS_CHAR_BIT_Def
 * \}
 */

/**
 * ccp_def.h
 *
 * \defgroup TBS_CALL_CONTROL_POINT_OPTIONAL_OPCODES_CHAR_BIT_Def Call Control Point Optional Opcodes characteristic bit definitions
 *
 * \brief Call Control Point Optional Opcodes characteristic bit definitions in Generic Telephone Bearer Service (GTBS) or Telephone Bearer Service (TBS)
 *
 * \ingroup CCP_Def_Exported_Macros
 * \{
 */
#define TBS_CALL_CONTROL_POINT_OPTIONAL_OPCODES_CHAR_BIT_LOCAL_HOLD                0  /**< Bit 0: Local Hold
                                                                                           * \arg 0: Local Hold and Local Retrieve Call Control Point Opcodes not supported
                                                                                           * \arg 1: Local Hold and Local Retrieve Call Control Point Opcodes supported */
#define TBS_CALL_CONTROL_POINT_OPTIONAL_OPCODES_CHAR_BIT_JOIN                      1  /**< Bit 1: Join
                                                                                           * \arg 0: Join Call Control Point Opcode not supported
                                                                                           * \arg 1: Join Call Control Point Opcode supported */
/**
 * End of TBS_CALL_CONTROL_POINT_OPTIONAL_OPCODES_CHAR_BIT_Def
 * \}
 */

/**
 * ccp_def.h
 *
 * \defgroup TBS_CALL_FLAGS_BIT_Def Call_Flags bit definitions
 *
 * \brief Call_Flags bit definitions in Generic Telephone Bearer Service (GTBS) or Telephone Bearer Service (TBS)
 *
 * \ingroup CCP_Def_Exported_Macros
 * \{
 */
#define TBS_CALL_FLAGS_BIT_INCOMING_OUTGOING                                       0  /**< Bit 0: Incoming/Outgoing
                                                                                           * \arg 0: Call is an incoming call
                                                                                           * \arg 1: Call is an outgoing call */
#define TBS_CALL_FLAGS_BIT_INFO_WITHHELD_BY_SERVER                                 1  /**< Bit 1: Information withheld by server
                                                                                           * \arg 0: Not withheld
                                                                                           * \arg 1: Withheld */
#define TBS_CALL_FLAGS_BIT_INFO_WITHHELD_BY_NETWORK                                2  /**< Bit 2: Information withheld by network
                                                                                           * \arg 0: Provided by network
                                                                                           * \arg 1: Withheld by network */
/**
 * End of TBS_CALL_FLAGS_BIT_Def
 * \}
 */

/**
 * End of CCP_Def_Exported_Macros
 * \}
 */

/**
 * \defgroup CCP_Def_Exported_Types Call Control Profile Definition Exported Types
 *
 * \ingroup LEA_GAF_CCP_Def
 * \{
 */

/**
 * ccp_def.h
 *
 * \defgroup T_TBS_CALL_STATE Call State of the call
 *
 * \brief Call State of the call in Generic Telephone Bearer Service (GTBS) or Telephone Bearer Service (TBS)
 *
 * \ingroup CCP_Def_Exported_Types
 * \{
 */
typedef enum
{
    TBS_CALL_STATE_INCOMING                                                        = 0x00, /**< A remote party is calling (incoming call). */
    TBS_CALL_STATE_DIALING                                                         = 0x01, /**< The process to call the remote party has started on the server, but the remote party is not being alerted (outgoing call). */
    TBS_CALL_STATE_ALERTING                                                        = 0x02, /**< A remote party is being alerted (outgoing call). */
    TBS_CALL_STATE_ACTIVE                                                          = 0x03, /**< The call is in an active conversation. */
    TBS_CALL_STATE_LOCALLY_HELD                                                    = 0x04, /**< The call is connected but held locally. "Locally Held" implies that either the server or the client can affect the state. */
    TBS_CALL_STATE_REMOTELY_HELD                                                   = 0x05, /**< The call is connected but held remotely. "Remotely Held" means that the state is controlled by the remote party of a call. */
    TBS_CALL_STATE_LOCALLY_AND_REMOTELY_HELD                                       = 0x06, /**< The call is connected but held both locally and remotely. */

    TBS_CALL_STATE_RFU,                                                                    /**< Reserved for future use. */
} T_TBS_CALL_STATE;
/**
 * End of T_TBS_CALL_STATE
 * \}
 */

/**
 * End of CCP_Def_Exported_Types
 * \}
 */

#ifdef  __cplusplus
}
#endif

#endif
