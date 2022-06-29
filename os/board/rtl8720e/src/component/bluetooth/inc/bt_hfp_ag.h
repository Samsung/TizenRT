/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_HFP_AG_H_
#define _BT_HFP_AG_H_

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

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
#define BT_HFP_AG_LOCAL_CAPABILITY_IN_BAND_RINGING        (1 << 3)
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
#define BT_HFP_AG_REMOTE_VOICE_REOGNITION_ACTIVATION  (1 << 3)
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
 * \brief  Define BT HFP AG call status.
 *
 * \ingroup BT_HFP_AG
 */
typedef enum t_bt_hfp_ag_call_status
{
    BT_HFP_AG_CALL_IDLE                     = 0x00,
    BT_HFP_AG_INCOMING_CALL_ONGOING         = 0x01,
    BT_HFP_AG_OUTGOING_CALL_ONGOING         = 0x02,
    BT_HFP_AG_CALL_ACTIVE                   = 0x03,
    BT_HFP_AG_CALL_HOLD                     = 0x04,
    BT_HFP_AG_CALL_ACTIVE_WITH_CALL_WAITING = 0x05,
    BT_HFP_AG_CALL_ACTIVE_WITH_CALL_HOLD    = 0x06,
} T_BT_HFP_AG_CALL_STATUS;

/**
 * bt_hfp_ag.h
 *
 * \brief  Define BT HFP AG service status indicator.
 *
 * \ingroup BT_HFP_AG
 */
typedef enum t_bt_hfp_ag_service_status
{
    BT_HFP_AG_SERVICE_STATUS_UNAVAILABLE  = 0x00,
    BT_HFP_AG_SERVICE_STATUS_AVAILABLE    = 0x01,
} T_BT_HFP_AG_SERVICE_STATUS;

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
 * \brief  Define BT HFP AG roam status indicator.
 *
 * \ingroup BT_HFP_AG
 */
typedef enum t_bt_hfp_ag_roam_status
{
    BT_HFP_AG_ROAM_INACTIVE = 0x00,
    BT_HFP_AG_ROAM_ACTIVE   = 0x01,
} T_BT_HFP_AG_ROAM_STATUS;

/**
 * bt_hfp_ag.h
 *
 * \brief  Define callback function for vendor AT CMD.
 *
 * \ingroup BT_HFP_AG
 */
typedef bool (*P_BT_HFP_AG_VND_AT_CMD_CALLBACK)(uint8_t *bd_addr, char *at_cmd);

/**
 * \brief   Initialize HFP AG profile.
 *
 * \param[in]  link_num                   HFP HF/HSP AG maximum connected link number.
 * \param[in]  rfc_hfp_ag_chann_num       Rfcomm channel num used for hfp.
 * \param[in]  rfc_hsp_ag_chann_num       Rfcomm channel num used for hsp.
 * \param[in]  ag_supported_features      HFP AG hf supported features \ref BT_HFP_SUPPORTED_FEATURE.
 * \param[in]  vendor_cmd_cback           callback function for vendor AT CMD.
 *
 * \return          The status of initializing HFP AG profile.
 * \retval true     HFP AG profile was initialized successfully.
 * \retval false    HFP AG profile was failed to initialize.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_init(uint8_t                         link_num,
                    uint8_t                         rfc_hfp_ag_chann_num,
                    uint8_t                         rfc_hsp_ag_chann_num,
                    uint16_t                        ag_supported_features,
                    P_BT_HFP_AG_VND_AT_CMD_CALLBACK vendor_cmd_cback);

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
 *
 * \return         The status of sending the Error result code.
 * \retval true    Error result code was sent successfully.
 * \retval false   Error result code was failed to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_error_send(uint8_t *bd_addr);

/**
 * bt_hfp_ag.h
 *
 * \brief  Send HFP AG indicators status during HFP service level connection establishment.
 *
 * \param[in]  bd_addr           Remote BT address.
 * \param[in]  service_status    HFP AG service status.
 * \param[in]  call_status       HFP AG call status.
 * \param[in]  signal_status     HFP AG signal status.
 * \param[in]  roam_status       HFP AG roam status.
 * \param[in]  batt_chg_status   HFP AG battery change status.
 *
 * \return         The status of sending the command request.
 * \retval true    Request has been sent successfully.
 * \retval false   Request was fail to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_indicators_status_send(uint8_t                    *bd_addr,
                                      T_BT_HFP_AG_SERVICE_STATUS  service_status,
                                      T_BT_HFP_AG_CALL_STATUS     call_status,
                                      uint8_t                     signal_status,
                                      T_BT_HFP_AG_ROAM_STATUS     roam_status,
                                      uint8_t                     batt_chg_status);

/**
 * bt_hfp_ag.h
 *
 * \brief  Notify the incoming call.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  call_num        HFP AG incoming call number.
 * \param[in]  call_num_length HFP AG incoming call number length with the maximum of 20 including '\0'.
 * \param[in]  call_num_type   HFP AG incoming call number type.
 *
 * \return         The status of sending the command request.
 * \retval true    Command has been sent successfully.
 * \retval false   Command was fail to send.
 *
 * \ingroup BT_HFP_AG
 */
bool bt_hfp_ag_call_incoming(uint8_t *bd_addr,
                             char    *call_num,
                             uint8_t  call_num_length,
                             uint8_t  call_num_type);

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
 * \brief  Reject the incoming call, teminate the outgoing or ongoing call.
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
 * \brief  Set HF microphone volumn level.
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
 * \brief  Set HF speaker volumn level.
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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

#endif /* _BT_HFP_AG_H_ */
