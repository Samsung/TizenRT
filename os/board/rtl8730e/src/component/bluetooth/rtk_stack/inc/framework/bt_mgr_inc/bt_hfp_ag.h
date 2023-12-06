/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */


#ifndef _BT_HFP_AG_H_
#define _BT_HFP_AG_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup    BT_HFP_AG BT HFP AG Profile
 *
 * \brief   Provide BT HFP AG profile interfaces.
 */

/**
 * bt_hfp.h
 *
 * \defgroup BT_HFP_AG_LOCAL_SUPPORTED_FEATURE BT HFP AG Local Supported Feature
 *
 * \brief Define BT HFP AG Local Supported Feature.
 *
 * \ingroup BT_HFP
 * @{
 */
#define BT_HFP_AG_LOCAL_CAPABILITY_3WAY                   (1 << 0)
#define BT_HFP_AG_LOCAL_CAPABILITY_EC_NR                  (1 << 1)
#define BT_HFP_AG_LOCAL_CAPABILITY_VOICE_RECOGNITION      (1 << 2)
#define BT_HFP_AG_LOCAL_CAPABILITY_INBAND_RINGING         (1 << 3)
#define BT_HFP_AG_LOCAL_CAPABILITY_VOICE_TAG              (1 << 4)
#define BT_HFP_AG_LOCAL_CAPABILITY_REJECT_CALLS           (1 << 5)
#define BT_HFP_AG_LOCAL_CAPABILITY_ENHANCED_CALL_STATUS   (1 << 6)
#define BT_HFP_AG_LOCAL_CAPABILITY_ENHANCED_CALL_CONTROL  (1 << 7)
#define BT_HFP_AG_LOCAL_CAPABILITY_EXTENED_ERROR_RESULT   (1 << 8)
#define BT_HFP_AG_LOCAL_CAPABILITY_CODEC_NEGOTIATION      (1 << 9)
#define BT_HFP_AG_LOCAL_CAPABILITY_HF_INDICATORS          (1 << 10)
#define BT_HFP_AG_LOCAL_CAPABILITY_ESCO_S4_T2_SUPPORTED   (1 << 11)
/**
 * @}
 */

/**
 * bt_hfp_ag.h
 *
 * \defgroup BT_HFP_AG_REMOTE_SUPPORTED_FEATURE BT HFP AG Remote Supported Feature
 *
 * \brief Define BT HFP AG Remote Supported Feature.
 *
 * \ingroup BT_HFP_AG
 * @{
 */
#define BT_HFP_AG_REMOTE_EC_NR_FUNCTION               (1 << 0)
#define BT_HFP_AG_REMOTE_THREE_WAY_CALLING            (1 << 1)
#define BT_HFP_AG_REMOTE_CLI_PRESENTATION_CAPABILITY  (1 << 2)
#define BT_HFP_AG_REMOTE_VOICE_RECOGNITION_ACTIVATION (1 << 3)
#define BT_HFP_AG_REMOTE_REMOTE_VOLUME_CONTROL        (1 << 4)
#define BT_HFP_AG_REMOTE_ENHANCED_CALL_STATUS         (1 << 5)
#define BT_HFP_AG_REMOTE_ENHANCED_CALL_CONTROL        (1 << 6)
#define BT_HFP_AG_REMOTE_CODEC_NEGOTIATION            (1 << 7)
#define BT_HFP_AG_REMOTE_HF_INDICATORS                (1 << 8)
#define BT_HFP_AG_REMOTE_ESCO_S4_SETTINGS             (1 << 9)
/**
 * @}
 */

/**
 * bt_hfp_ag.h
 *
 * \defgroup BT_HFP_AG_EXTENDED_ERROR_RESULT_CODE BT HFP AG Extended Audio Gateway Error Result Code
 *
 * \brief Define BT HFP AG Remote Supported Feature.
 *
 * \ingroup BT_HFP_AG
 * @{
 */
/**/
#define BT_HFP_AG_ERR_AG_FAILURE           0  /* AG Failure */
#define BT_HFP_AG_ERR_NO_CONN_PHONE        1  /* No connection to phone */
#define BT_HFP_AG_ERR_OP_NOT_ALLOWED       3  /* Operation not allowed */
#define BT_HFP_AG_ERR_OP_NOT_SUPPORTED     4  /* Operation not supported */
#define BT_HFP_AG_ERR_PHSIM_PIN_REQ        5  /* PH-SIM PIN required */
#define BT_HFP_AG_ERR_SIM_NOT_INSERTED     10 /* SIM not inserted */
#define BT_HFP_AG_ERR_SIM_PIN_REQ          11 /* SIM PIN required */
#define BT_HFP_AG_ERR_SIM_PUK_REQ          12 /* SIM PUK required */
#define BT_HFP_AG_ERR_SIM_FAILURE          13 /* SIM failure */
#define BT_HFP_AG_ERR_SIM_BUSY             14 /* SIM busy */
#define BT_HFP_AG_ERR_INCORRECT_PWD        16 /* Incorrect password */
#define BT_HFP_AG_ERR_SIM_PIN2_REQ         17 /* SIM PIN2 required */
#define BT_HFP_AG_ERR_SIM_PUK2_REQ         18 /* SIM PUK2 required */
#define BT_HFP_AG_ERR_MEMORY_FULL          20 /* Memory full */
#define BT_HFP_AG_ERR_INVALID_INDEX        21 /* Invalid index */
#define BT_HFP_AG_ERR_MEMORY_FAILURE       23 /* Memory failure */
#define BT_HFP_AG_ERR_TEXT_TOO_LONG        24 /* Text string too long */
#define BT_HFP_AG_ERR_INVALID_CHAR_IN_TSTR 25 /* Invalid characters in text string */
#define BT_HFP_AG_ERR_DSTR_TOO_LONG        26 /* Dial string too long */
#define BT_HFP_AG_ERR_INVALID_CHAR_IN_DSTR 27 /* Invalid characters in dial string */
#define BT_HFP_AG_ERR_NO_NETWORK_SERVICE   30 /* No network service */
#define BT_HFP_AG_ERR_NETWORK_TIMEOUT      31 /* Network timeout */
#define BT_HFP_AG_ERR_NETWORK_NOT_ALLOWED  32 /* Network not allowed - emergency service only */
/**
 * @}
 */

/**
 * bt_hfp_ag.h
 *
 * \brief  Define BT HFP AG call status.
 *
 * \ingroup BT_HFP_AG
 */
typedef enum t_bt_hfp_ag_call_status
{
    BT_HFP_AG_CALL_IDLE                     = 0x00,
    BT_HFP_AG_CALL_INCOMING                 = 0x01,
    BT_HFP_AG_CALL_OUTGOING                 = 0x02,
    BT_HFP_AG_CALL_ACTIVE                   = 0x03,
    BT_HFP_AG_CALL_ACTIVE_WITH_CALL_WAITING = 0x04,
    BT_HFP_AG_CALL_ACTIVE_WITH_CALL_HOLD    = 0x05,
} T_BT_HFP_AG_CALL_STATUS;

/**
 * bt_hfp_ag.h
 *
 * \brief  Define BT HFP AG codec type.
 *
 * \ingroup BT_HFP_AG
 */
typedef enum t_bt_hfp_ag_codec_type
{
    BT_HFP_AG_CODEC_TYPE_CVSD = 0x01,
    BT_HFP_AG_CODEC_TYPE_MSBC = 0x02,
} T_BT_HFP_AG_CODEC_TYPE;

/**
 * bt_hfp_ag.h
 *
 * \brief  Define BT HFP AG service indicator .
 *
 * \ingroup BT_HFP_AG
 */
typedef enum t_bt_hfp_ag_service_indicator
{
    BT_HFP_AG_SERVICE_STATUS_AVAILABLE     = 0x00,
    BT_HFP_AG_SERVICE_STATUS_UNAVAILABLE   = 0x01,
} T_BT_HFP_AG_SERVICE_INDICATOR;

/**
 * bt_hfp_ag.h
 *
 * \brief  Define BT HFP AG call indicator.
 *
 * \ingroup BT_HFP_AG
 */
typedef enum t_bt_hfp_ag_call_indicator
{
    BT_HFP_AG_NO_CALL_IN_PROGRESS  = 0x00,
    BT_HFP_AG_CALL_IN_PROGRESS     = 0x01,
} T_BT_HFP_AG_CALL_INDICATOR;

/**
 * bt_hfp_ag.h
 *
 * \brief  Define BT HFP AG call setup indicator.
 *
 * \ingroup BT_HFP_AG
 */
typedef enum t_bt_hfp_ag_call_setup_indicator
{
    BT_HFP_AG_CALL_SETUP_STATUS_IDLE              = 0x00,
    BT_HFP_AG_CALL_SETUP_STATUS_INCOMING_CALL     = 0x01,
    BT_HFP_AG_CALL_SETUP_STATUS_OUTGOING_CALL     = 0x02,
    BT_HFP_AG_CALL_SETUP_STATUS_ALERTING          = 0x03,
} T_BT_HFP_AG_CALL_SETUP_INDICATOR;

/**
 * bt_hfp_ag.h
 *
 * \brief  Define BT HFP AG call held indicator.
 *
 * \ingroup BT_HFP_AG
 */
typedef enum t_bt_hfp_ag_call_held_indicator
{
    BT_HFP_AG_CALL_HELD_STATUS_IDLE                  = 0x00,
    BT_HFP_AG_CALL_HELD_STATUS_HOLD_AND_ACTIVE_CALL  = 0x01,
    BT_HFP_AG_CALL_HELD_STATUS_HOLD_NO_ACTIVE_CALL   = 0x02,
} T_BT_HFP_AG_CALL_HELD_INDICATOR;

/**
 * bt_hfp_ag.h
 *
 * \brief  Define BT HFP AG roaming indicator.
 *
 * \ingroup BT_HFP_AG
 */
typedef enum t_bt_hfp_ag_roaming_indicator
{
    BT_HFP_AG_ROAMING_STATUS_INACTIVE = 0x00,
    BT_HFP_AG_ROAMING_STATUS_ACTIVE   = 0x01,
} T_BT_HFP_AG_ROAMING_INICATOR;

/**
 * bt_hfp_ag.h
 *
 * \brief  Define BT HFP AG current call status.
 *
 * \ingroup BT_HFP_AG
 */
typedef enum t_bt_hfp_ag_current_call_status
{
    BT_HFP_AG_CURRENT_CALL_STATUS_ACTIVE                    = 0x00,
    BT_HFP_AG_CURRENT_CALL_STATUS_HELD                      = 0x01,
    BT_HFP_AG_CURRENT_CALL_STATUS_DIALING                   = 0x02,
    BT_HFP_AG_CURRENT_CALL_STATUS_ALERTING                  = 0x03,
    BT_HFP_AG_CURRENT_CALL_STATUS_INCOMING                  = 0x04,
    BT_HFP_AG_CURRENT_CALL_STATUS_WAITING                   = 0x05,
    BT_HFP_AG_CURRENT_CALL_STATUS_CALL_HELD_BY_RSP_AND_HOLD = 0x06,
} T_BT_HFP_AG_CURRENT_CALL_STATUS;

/**
 * bt_hfp_ag.h
 *
 * \brief  Define BT HFP AG current call mode.
 *
 * \ingroup BT_HFP_AG
 */
typedef enum t_bt_hfp_ag_current_call_mode
{
    BT_HFP_AG_CURRENT_CALL_MODE_VOICE = 0x00,
    BT_HFP_AG_CURRENT_CALL_MODE_DATA  = 0x01,
    BT_HFP_AG_CURRENT_CALL_MODE_FAX   = 0x02,
} T_BT_HFP_AG_CURRENT_CALL_MODE;

/**
 * bt_hfp_ag.h
 *
 * \brief  Define callback function for vendor AT CMD.
 *
 * \ingroup BT_HFP_AG
 */
typedef bool (*P_BT_HFP_AG_VND_AT_CMD_CBACK)(uint8_t *bd_addr, char *at_cmd);

/**
 * \brief   Initialize HFP AG profile.
 *
 * \param[in]  link_num                   HFP HF/HSP AG maximum connected link number.
 * \param[in]  rfc_hfp_ag_chann_num       RFCOMM channel num used for hfp.
 * \param[in]  rfc_hsp_ag_chann_num       RFCOMM channel num used for hsp.
 * \param[in]  ag_supported_features      HFP AG hf supported features \ref BT_HFP_AG_LOCAL_SUPPORTED_FEATURE.
 * \param[in]  vendor_cmd_cback           callback function for vendor AT CMD.
 *
 * \return          The status of initializing HFP AG profile.
 * \retval true     HFP AG profile was initialized successfully.
 * \retval false    HFP AG profile was failed to initialize.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_init(uint8_t                       link_num,
                    uint8_t                       rfc_hfp_ag_chann_num,
                    uint8_t                       rfc_hsp_ag_chann_num,
                    uint16_t                      ag_supported_features,
                    P_BT_HFP_AG_VND_AT_CMD_CBACK  vendor_cmd_cback);

/**
 * bt_hfp_ag.h
 *
 * \brief   Send an HFP AG connection request.
 *
 * \param[in]  bd_addr       Remote BT address.
 * \param[in]  server_chann  Server channel which can be found from the sdp info.
 * \param[in]  is_hfp        Choose to create an hfp or hsp connection.
 * \arg    true          Create an hfp connection.
 * \arg    false         Create an hsp connection.
 *
 * \return         The status of sending the HFP or HSP connection request.
 * \retval true    HFP or HSP connection request was sent successfully.
 * \retval false   HFP or HSP connection request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_connect_req(uint8_t *bd_addr, uint8_t server_chann, bool is_hfp);

/**
 * bt_hfp_ag.h
 *
 * \brief   Send an HFP AG connection confirmation.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  accept     Accept or reject the connection indication.
 * \arg    true     Accept the connection indication.
 * \arg    false    Reject the connection indication
 *
 * \return         The status of sending the HFP connection confirmation.
 * \retval true    HFP connection confirmation was sent successfully.
 * \retval false   HFP connection confirmation was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_connect_cfm(uint8_t *bd_addr, bool accept);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send an HFP AG disconnection request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the HFP disconnection request.
 * \retval true    HFP disconnection request was sent successfully.
 * \retval false   HFP disconnection request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_disconnect_req(uint8_t *bd_addr);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send a request to creat audio connection.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \note   Although the Audio Connection may be triggered by either the AG or the HF,
 *         the Codec Connection and the Synchronous Connection shall always be
 *         established by the AG (unless one of the devices is a legacy device).
 *         AG implementations that comply with the Hands-Free Profile specification
 *         Rev. 0.96,1.0 or 1.5, shall not indicate support for the Codec Negotiation
 *         feature and shall neither send the +BCS unsolicited response to the HF.
 *         In order to guarantee backward compatibility, HFP Rev. x.y implementations shall
 *         be able to handle establishment of synchronous connections according to
 *         Hands-Free Profile specification Rev. 1.0 or 1.5.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_audio_connect_req(uint8_t *bd_addr);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send a request to disconnect audio connection.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_audio_disconnect_req(uint8_t *bd_addr);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send an vendor AT command request.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  at_cmd     The start address of buffer that stores AT command.
 *
 * \return         The status of sending the AT command request.
 * \retval true    AT command request was sent successfully.
 * \retval false   AT command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_send_vnd_at_cmd_req(uint8_t *bd_addr, const char *at_cmd);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send an OK result code.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the OK result code.
 * \retval true    OK result code was sent successfully.
 * \retval false   OK result code was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_ok_send(uint8_t *bd_addr);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send an Error result code.
 *
 * \param[in]  bd_addr     Remote BT address.
 * \param[in]  error_code  HFP AG Extended Audio Gateway Error Result Code \ref BT_HFP_AG_EXTENDED_ERROR_RESULT_CODE.
 *
 * \return         The status of sending the Error result code.
 * \retval true    Error result code was sent successfully.
 * \retval false   Error result code was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_error_send(uint8_t *bd_addr, uint8_t error_code);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send HFP AG indicators during HFP service level connection establishment.
 *
 * \param[in]  bd_addr              Remote BT address.
 * \param[in]  service_indicator    HFP AG service availability indicator.
 * \param[in]  call_indicator       HFP AG standard call status indicator.
 * \param[in]  call_setup_indicator HFP AG call set up status indicator.
 * \param[in]  call_held_indicator  HFP AG call hold status indicator.
 * \param[in]  signal_indicator     HFP AG signal strength indicator.
 * \param[in]  roaming_indicator    HFP AG roaming status indicator.
 * \param[in]  batt_chg_indicator   HFP AG battery charge indicator.
 *
 * \return         The status of sending the command request.
 * \retval true    Request has been sent successfully.
 * \retval false   Request was fail to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_indicators_send(uint8_t                          *bd_addr,
                               T_BT_HFP_AG_SERVICE_INDICATOR     service_indicator,
                               T_BT_HFP_AG_CALL_INDICATOR        call_indicator,
                               T_BT_HFP_AG_CALL_SETUP_INDICATOR  call_setup_indicator,
                               T_BT_HFP_AG_CALL_HELD_INDICATOR   call_held_indicator,
                               uint8_t                           signal_indicator,
                               T_BT_HFP_AG_ROAMING_INICATOR      roaming_indicator,
                               uint8_t                           batt_chg_indicator);

/**
 * bt_hfp_ag.h
 *
 * \brief  Notify the incoming call.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  call_num        HFP AG incoming call number.
 * \param[in]  call_num_len    HFP AG incoming call number length with the maximum of 20 including '\0'.
 * \param[in]  call_num_type   HFP AG incoming call number type.
 *
 * \return         The status of sending the command request.
 * \retval true    Command has been sent successfully.
 * \retval false   Command was fail to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_call_incoming(uint8_t    *bd_addr,
                             const char *call_num,
                             uint8_t     call_num_len,
                             uint8_t     call_num_type);

/**
 * bt_hfp_ag.h
 *
 * \brief  Answer the incoming or outgoing alerting call.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command has been sent successfully.
 * \retval false   Command was fail to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_call_answer(uint8_t *bd_addr);

/**
 * bt_hfp_ag.h
 *
 * \brief  Reject the incoming call, terminate the outgoing call, or ongoing call.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command has been sent successfully.
 * \retval false   Command was fail to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_call_terminate(uint8_t *bd_addr);

/**
 * bt_hfp_ag.h
 *
 * \brief  Dial the outgoing call.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command has been sent successfully.
 * \retval false   Command was fail to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_call_dial(uint8_t *bd_addr);

/**
 * bt_hfp_ag.h
 *
 * \brief  Notify the outgoing call alerted.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command has been sent successfully.
 * \retval false   Command was fail to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_call_alert(uint8_t *bd_addr);

/**
 * bt_hfp_ag.h
 *
 * \brief  send the call waiting notification
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  call_num        HFP AG call waiting number.
 * \param[in]  call_num_len    HFP AG call waiting number length with the maximum of 20 including '\0'.
 * \param[in]  call_num_type   HFP AG call waiting number type.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_call_waiting_send(uint8_t    *bd_addr,
                                 const char *call_num,
                                 uint8_t     call_num_len,
                                 uint8_t     call_num_type);

/**
 * bt_hfp_ag.h
 *
 * \brief  Set HF microphone volume level.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  level      HF microphone gain level to set that ranges from 0 to 15.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_microphone_gain_set(uint8_t *bd_addr, uint8_t level);

/**
 * bt_hfp_ag.h
 *
 * \brief  Set HF speaker volume level.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  level      HF speaker gain level to set that ranges from 0 to 15.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_speaker_gain_set(uint8_t *bd_addr, uint8_t level);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send service availability indicator.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  status     HF AG service availability indicator.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_service_indicator_send(uint8_t *bd_addr, T_BT_HFP_AG_SERVICE_INDICATOR status);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send call indicator.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  status     HF AG call indicator.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_call_indicator_send(uint8_t *bd_addr, T_BT_HFP_AG_CALL_INDICATOR status);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send call setup indicator.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  status     HF AG call setup indicator.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_call_setup_indicator_send(uint8_t *bd_addr,
                                         T_BT_HFP_AG_CALL_SETUP_INDICATOR status);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send call hold indicator.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  status     HF AG call hold indicator.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_call_held_indicator_send(uint8_t *bd_addr,
                                        T_BT_HFP_AG_CALL_HELD_INDICATOR status);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send signal strength indicator.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  value      HF AG signal strength, ranges from 0 to 5.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_signal_strength_send(uint8_t *bd_addr, uint8_t value);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send roaming indicator.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  status     HF AG roaming indicator.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_roaming_indicator_send(uint8_t *bd_addr, T_BT_HFP_AG_ROAMING_INICATOR status);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send battery charge indicator.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  value      HF AG battery level, ranges from 0 to 5.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_battery_charge_send(uint8_t *bd_addr, uint8_t value);

/**
 * bt_hfp_ag.h
 *
 * \brief  Set AG ring inteval.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  interval   AG ring interval in milliseconds with default value as 5000ms.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_ring_interval_set(uint8_t *bd_addr, uint16_t interval);

/**
 * bt_hfp_ag.h
 *
 * \brief  Change HFP or HSP AG in-band ring tone setting dynamically after service
 *         level connection establishment.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  enable     Enable HFP or HSP AG in-band ring tone or not dynamically.
 *
 * \return         The status of changing HFP or HSP AG in-band ring tone setting dynamically.
 * \retval true    Success to change HFP or HSP AG in-band ring tone setting dynamically.
 * \retval false   Failed to change HFP or HSP AG in-band ring tone setting dynamically.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_inband_ringing_set(uint8_t *bd_addr, bool enable);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send HFP AG current calls list.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  call_idx        HFP AG current call index.
 * \param[in]  call_dir        HFP AG current call direction.
 * \arg    0   outgoing.
 * \arg    1   incoming.
 * \param[in]  call_status     HFP AG current call status.
 * \param[in]  call_mode       HFP AG current call mode.
 * \param[in]  mpty            HFP AG current multi-party call or not.
 * \arg    0   this call is NOT a member of a multi-party (conference) call.
 * \arg    1   this call IS a member of a multi-party (conference) call.
 * \param[in]  call_num        HFP AG current call number.
 * \param[in]  call_num_type   HFP AG current call number type.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP_AG
*/
bool bt_hfp_ag_current_calls_list_send(uint8_t                         *bd_addr,
                                       uint8_t                          call_idx,
                                       uint8_t                          call_dir,
                                       T_BT_HFP_AG_CURRENT_CALL_STATUS  call_status,
                                       T_BT_HFP_AG_CURRENT_CALL_MODE    call_mode,
                                       uint8_t                          mpty,
                                       const char                      *call_num,
                                       uint8_t                          call_num_type);

/**
* bt_hfp_ag.h
*
* \brief  Indicate the currently selected operator.
*
* \param[in]  bd_addr         Remote BT address.
* \param[in]  operator        HFP AG the currently selected operator.
* \param[in]  operator_len    HFP AG the currently selected operator length with the maximum of 17 including '\0'.
*
* \return         The status of sending the command request.
* \retval true    Command has been sent successfully.
* \retval false   Command was fail to send.
*
* \ingroup BT_HFP_AG
*/
bool bt_hfp_ag_network_operator_name_send(uint8_t    *bd_addr,
                                          const char *operator,
                                          uint8_t     operator_len);

/**
* bt_hfp_ag.h
*
* \brief  Send subscriber number information.
*
* \param[in]  bd_addr         Remote BT address.
* \param[in]  call_num        HFP AG subscriber number.
* \param[in]  call_num_len    HFP AG subscriber number length with the maximum of 20 including '\0'.
* \param[in]  call_num_type   HFP AG subscriber number type.
* \param[in]  service         HFP AG service the subscriber number relates to, Shall be either 4 (voice) or 5 (fax).
*
* \return         The status of sending the command request.
* \retval true    Command has been sent successfully.
* \retval false   Command was fail to send.
*
* \ingroup BT_HFP_AG
*/
bool bt_hfp_ag_subscriber_number_send(uint8_t    *bd_addr,
                                      const char *call_num,
                                      uint8_t     call_num_len,
                                      uint8_t     call_num_type,
                                      uint8_t     service);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BT_HFP_AG_H_ */
