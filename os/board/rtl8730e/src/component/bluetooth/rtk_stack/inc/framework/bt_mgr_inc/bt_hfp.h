/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_HFP_H_
#define _BT_HFP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup    BT_HFP BT HFP Profile
 *
 * \brief   Provide BT HFP profile interfaces.
 */

/**
 * bt_hfp.h
 *
 * \defgroup BT_HFP_HF_LOCAL_SUPPORTED_FEATURE BT HFP HF Local Supported Feature
 *
 * \brief Define BT HFP HF Local Supported Feature.
 *
 * \ingroup BT_HFP
 * @{
 */
#define BT_HFP_HF_LOCAL_EC_NR_FUNCTION               (1 << 0)
#define BT_HFP_HF_LOCAL_THREE_WAY_CALLING            (1 << 1)
#define BT_HFP_HF_LOCAL_CLI_PRESENTATION_CAPABILITY  (1 << 2)
#define BT_HFP_HF_LOCAL_VOICE_RECOGNITION_ACTIVATION (1 << 3)
#define BT_HFP_HF_LOCAL_REMOTE_VOLUME_CONTROL        (1 << 4)
#define BT_HFP_HF_LOCAL_ENHANCED_CALL_STATUS         (1 << 5)
#define BT_HFP_HF_LOCAL_ENHANCED_CALL_CONTROL        (1 << 6)
#define BT_HFP_HF_LOCAL_CODEC_NEGOTIATION            (1 << 7)
#define BT_HFP_HF_LOCAL_HF_INDICATORS                (1 << 8)
#define BT_HFP_HF_LOCAL_ESCO_S4_SETTINGS             (1 << 9)
/**
 * @}
 */

/**
 * bt_hfp.h
 *
 * \defgroup BT_HFP_HF_REMOTE_SUPPORTED_FEATURE BT HFP HF Remote Supported Feature
 *
 * \brief Define BT HFP HF Remote Supported Feature.
 *
 * \ingroup BT_HFP
 * @{
 */
#define BT_HFP_HF_REMOTE_CAPABILITY_3WAY                   (1 << 0)
#define BT_HFP_HF_REMOTE_CAPABILITY_EC_NR                  (1 << 1)
#define BT_HFP_HF_REMOTE_CAPABILITY_VOICE_RECOGNITION      (1 << 2)
#define BT_HFP_HF_REMOTE_CAPABILITY_INBAND_RINGING         (1 << 3)
#define BT_HFP_HF_REMOTE_CAPABILITY_VOICE_TAG              (1 << 4)
#define BT_HFP_HF_REMOTE_CAPABILITY_REJECT_CALLS           (1 << 5)
#define BT_HFP_HF_REMOTE_CAPABILITY_ENHANCED_CALL_STATUS   (1 << 6)
#define BT_HFP_HF_REMOTE_CAPABILITY_ENHANCED_CALL_CONTROL  (1 << 7)
#define BT_HFP_HF_REMOTE_CAPABILITY_EXTENED_ERROR_RESULT   (1 << 8)
#define BT_HFP_HF_REMOTE_CAPABILITY_CODEC_NEGOTIATION      (1 << 9)
#define BT_HFP_HF_REMOTE_CAPABILITY_HF_INDICATORS          (1 << 10)
#define BT_HFP_HF_REMOTE_CAPABILITY_ESCO_S4_T2_SUPPORTED   (1 << 11)
/**
 * @}
 */

/**
 * bt_hfp.h
 *
 * \brief  Define BT HFP call status.
 *
 * \ingroup BT_HFP
 */
typedef enum t_bt_hfp_call_status
{
    BT_HFP_CALL_IDLE                              = 0x00,
    BT_HFP_CALL_INCOMING                          = 0x01,
    BT_HFP_CALL_OUTGOING                          = 0x02,
    BT_HFP_CALL_ACTIVE                            = 0x03,
    BT_HFP_CALL_HELD                              = 0x04,
    BT_HFP_CALL_ACTIVE_WITH_CALL_WAITING          = 0x05,
    BT_HFP_CALL_ACTIVE_WITH_CALL_HELD             = 0x06,
} T_BT_HFP_CALL_STATUS;

/**
 * bt_hfp.h
 *
 * \brief   Initialize HFP profile.
 *
 * \param[in]  link_num                HFP/HSP maximum connected link number.
 * \param[in]  rfc_hfp_chann_num       RFCOMM channel num used for hfp.
 * \param[in]  rfc_hsp_chann_num       RFCOMM channel num used for hsp.
 * \param[in]  hf_supported_features   HFP hf supported features \ref BT_HFP_HF_LOCAL_SUPPORTED_FEATURE.
 *
 * \return          The status of initializing HFP profile.
 * \retval true     HFP profile was initialized successfully.
 * \retval false    HFP profile was failed to initialize.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_init(uint8_t link_num,
                 uint8_t rfc_hfp_chann_num,
                 uint8_t rfc_hsp_chann_num,
                 uint16_t hf_supported_features);

/**
 * bt_hfp.h
 *
 * \brief   Send an HFP or HSP connection request.
 *
 * \param[in]  bd_addr       Remote BT address.
 * \param[in]  server_chann  Remote dlci which can be found from the sdp info.
 * \param[in]  is_hfp        Choose to create an hfp or hsp connection.
 * \arg    true          Create a hfp connection.
 * \arg    false         Create a hsp connection.
 *
 * \return         The status of sending the HFP or HSP connection request.
 * \retval true    HFP or HSP connection request was sent successfully.
 * \retval false   HFP or HSP connection request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_connect_req(uint8_t *bd_addr, uint8_t server_chann, bool is_hfp);

/**
 * bt_hfp.h
 *
 * \brief   Send an HFP connection confirmation.
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
 * \ingroup BT_HFP
 */
bool bt_hfp_connect_cfm(uint8_t *bd_addr, bool accept);

/**
 * bt_hfp.h
 *
 * \brief  Send an HFP disconnection request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the HFP disconnection request.
 * \retval true    HFP disconnection request was sent successfully.
 * \retval false   HFP disconnection request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_disconnect_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a vendor AT command request.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  at_cmd     The start address of buffer that stores AT command.
 *
 * \return         The status of sending the AT command request.
 * \retval true    AT command request was sent successfully.
 * \retval false   AT command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_send_vnd_at_cmd_req(uint8_t *bd_addr, const char *at_cmd);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to dial a specific phone number.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  number     The start address of string buffer that stores phone number.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_dial_with_number_req(uint8_t *bd_addr, const char *number);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to recalling the last number dialed by the remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_dial_last_number_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to answer the incoming call.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_call_answer_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to terminate the phone call.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_call_terminate_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to hold the phone call.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_call_hold_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to accept the held phone call.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_held_call_accept_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to release the held phone call.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_held_call_release_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to query current call list.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_current_call_list_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to release held or waiting call.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_release_held_or_waiting_call_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to release active call and accept held or waiting_call.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_release_active_call_accept_held_or_waiting_call_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to hold active call and accept held or waiting_call.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_hold_active_call_accept_held_or_waiting_call_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to join two calls.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_join_two_calls_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
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
 *         HF implementations that comply with the Hands-Free Profile specification
 *         Rev. 0.96,1.0 or 1.5, shall not indicate support for the Codec Negotiation
 *         feature and shall neither send the AT+BAC command nor the AT+BCC command to
 *         trigger an audio connection establishment by the AG.
 *         In order to guarantee backward compatibility, HFP Rev. x.y implementations shall
 *         be able to handle establishment of synchronous connections according to
 *         Hands-Free Profile specification Rev. 1.0 or 1.5.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_audio_connect_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a request to disconnect audio connection.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_audio_disconnect_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to enable voice recognition.
 *
 * \param[in]  bd_addr    Remote BT address
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_voice_recognition_enable_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to disable voice recognition.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_voice_recognition_disable_req(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to synchronize speaker gain level.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  level      Local gain level, range from 0 to 15.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_speaker_gain_level_report(uint8_t *bd_addr, uint8_t level);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to synchronize microphone gain level.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  level      Local gain level, range from 0 to 15.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_microphone_gain_level_report(uint8_t *bd_addr, uint8_t level);

/**
 * bt_hfp.h
 *
 * \brief  Send a specific DTMF code to its network connection.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  code       DTMF codes that are '0'-'9','*','#','A','B','C','and 'D'.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_dtmf_code_transmit(uint8_t *bd_addr, char code);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to query the current network operator name.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_network_operator_name_query(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to query the AG subscriber number information.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_subscriber_number_query(uint8_t *bd_addr);

/**
 * bt_hfp.h
 *
 * \brief  Send a command to report current battery level.
 *
 * \param[in]  bd_addr     Remote BT address.
 * \param[in]  level       The current battery level, range from 0 to 100.
 *
 * \return         The status of sending the command request.
 * \retval true    Command request was sent successfully.
 * \retval false   Command request was failed to send.
 *
 * \ingroup BT_HFP
 */
bool bt_hfp_batt_level_report(uint8_t *bd_addr, uint8_t level);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BT_HFP_H_ */
